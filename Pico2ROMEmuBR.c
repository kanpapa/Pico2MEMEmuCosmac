// COSMAC test Version alpha (ROM only, No RAM)

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/vreg.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/pll.h"
#include "pico/multicore.h"
#include "rom_emu.pio.h"
#include "rom_basic_const.c" 

#define DATA_PINS_BASE 10  // GP10～GP17 (D0-D7 8bit)
#define ADDR_PINS_BASE 2   // GP2～GP9 (MA0-MA7 8bit)
#define MWR_PIN 18
#define MRD_PIN 19
#define TPA_PIN 20
#define TPB_PIN 21
#define RESETOUT_PIN 22     // GP22 (リセット出力)
#define CLKOUT_PIN 28       // GP28 (クロック出力)

// UART0の設定
#define UART_ID uart0
#define BAUD_RATE 9600
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define FLAG_VALUE 123

//#define ROM_SIZE 8192
#define ROM_SIZE 512
#define RAM_START 0x1000
#define RAM_SIZE 512

// PIO初期化
PIO pio = pio0;

const uint sm_addr = 0;
const uint sm_write = 1;
const uint sm_clock = 2;
const uint sm_reset = 3;

// IRQハンドラとメインループで共有するアドレス
volatile uint16_t current_address = 0;

uint8_t rom_data[ROM_SIZE] __attribute__((section(".data")));
uint8_t ram_data[RAM_SIZE];

// コア1のエントリポイント
// core1_entry()はPIOの状態マシンを実行し、ROMデータを送信する  
__attribute__((noinline)) void __time_critical_func(core1_entry)(void) {
    multicore_fifo_push_blocking(FLAG_VALUE);
    uint32_t g = multicore_fifo_pop_blocking();
    while (true) {
        // アドレス取得SMのFIFOをチェック
        if (!pio_sm_is_rx_fifo_empty(pio, sm_addr)) {
            // ラッチしたアドレスの取り込み
            current_address = pio_sm_get_blocking(pio, sm_addr);
            // ★★★ デバッグ用に追加 ★★★
            //printf("Address captured: 0x%04X\n", current_address);
    
            // readなのかwriteなのかわからないがとりあえずメモリのデータを送信し、あとはsm_addrにまかせる。
            uint8_t data_to_send = 0xFF;
            if (current_address < ROM_SIZE) {
                data_to_send = rom_data[current_address];
            } else if (current_address >= RAM_START && current_address < (RAM_START + RAM_SIZE)) {
                data_to_send = ram_data[current_address - RAM_START];
            }
            // ★★★ デバッグ用に追加 ★★★
            //printf("  -> Sending data: 0x%02X for address 0x%04X\n", data_to_send, current_address);
            pio_sm_put_blocking(pio, sm_addr, data_to_send);
        }

        // 書き込みSMのFIFOをチェック
        if (!pio_sm_is_rx_fifo_empty(pio, sm_write)) {
            // ここにデータがきたら無条件でメモリに書き込む
            uint8_t data_written = (uint8_t)pio_sm_get_blocking(pio, sm_write);
            // ★★★ デバッグ用に追加 ★★★
            //printf("  -> Receving data: 0x%02X for address 0x%04X\n", data_written, current_address);
            
            // RAM領域への書き込みのみ処理
            if (current_address >= RAM_START && current_address < (RAM_START + RAM_SIZE)) {
                ram_data[current_address - RAM_START] = data_written;
            }
        }
    }
}

// rom_basic[]をrom_data[]にコピーする初期化ルーチン
void init_rom_basic_code(void) {
    // z80_binary[]の内容をrom_data[]の先頭にコピー
    memcpy(rom_data, rom_basic, sizeof(rom_basic));
    // 残りのrom[]を0xFFで埋める（8Kバイトまで）
    memset(rom_data + sizeof(rom_basic), 0xFF, ROM_SIZE - sizeof(rom_basic));
}

// QSPIクロックを調整する関数
void set_qspi_clock_divider(uint32_t sys_clock_khz, uint32_t qspi_max_khz) {
    uint32_t divider = (sys_clock_khz + qspi_max_khz - 1) / qspi_max_khz;
    clock_configure(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, sys_clock_khz * 1000, sys_clock_khz * 1000 / divider);
}

__attribute__((noinline)) int __time_critical_func(main)(void) {
    uint32_t sysclk = 320 * 1000;           // Pico2 システムクロック 280/320/360MHz 
    vreg_set_voltage(VREG_VOLTAGE_1_30);    // 電圧を1.3Vに設定
    sleep_ms(100);                          // 電圧安定のための待機
    set_sys_clock_khz(sysclk, true);        // 高速動作
    set_qspi_clock_divider(sysclk, 133000); // QSPIクロックを133MHz以下に

    stdio_init_all();
    setbuf(stdout, NULL);           // 標準出力のバッファリングを無効化 

    // UART0の初期化
    uart_init(UART_ID, BAUD_RATE);
    // UARTピンの設定（GPIO0=TX, GPIO1=RX）
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Programmable I/O(PIO)初期化
    uint offset_addr = pio_add_program(pio, &cdp1802_get_address_program); // 命令用のメモリーにアセンブラのプログラムを登録します
    uint offset_write = pio_add_program(pio, &cdp1802_write_mem_program);

    // --- 各ステートマシンの設定 ---
    // 0. アドレス取得＆読み出し用SM (sm_addr)
    pio_sm_config c_addr = cdp1802_get_address_program_get_default_config(offset_addr);
    sm_config_set_in_pins(&c_addr, ADDR_PINS_BASE);
    sm_config_set_out_pins(&c_addr, DATA_PINS_BASE, 8);
    sm_config_set_jmp_pin(&c_addr, MRD_PIN); // MRDをJMPピンとして設定
    // PIOが制御するピンの方向を設定
    pio_sm_set_consecutive_pindirs(pio, sm_addr, ADDR_PINS_BASE, 8, false); // ADDRESSバスは入力
    pio_sm_set_consecutive_pindirs(pio, sm_addr, TPA_PIN, 1, false);  // TPAは入力
    pio_sm_set_consecutive_pindirs(pio, sm_addr, TPB_PIN, 1, false);  // TPBは入力
    pio_sm_set_consecutive_pindirs(pio, sm_addr, DATA_PINS_BASE, 8, false); // 初期は入力
    pio_sm_set_consecutive_pindirs(pio, sm_addr, MRD_PIN, 1, false);  // MRDは入力
    
    // 1. 書き込み用SM (sm_write)
    pio_sm_config c_write = cdp1802_write_mem_program_get_default_config(offset_write);
    sm_config_set_in_pins(&c_write, DATA_PINS_BASE); // DATAバスは入力
    // PIOが制御するピンの方向を設定
    pio_sm_set_consecutive_pindirs(pio, sm_write, MWR_PIN, 1, false); // MWR_PINは入力
    pio_sm_set_consecutive_pindirs(pio, sm_write, DATA_PINS_BASE, 8, false); // DATAバスは入力

    // 2. CPUクロック生成用SM (sm_clock)
    uint offset_clock = pio_add_program(pio, &clk_out_program);
    pio_sm_config c_clock = clk_out_program_get_default_config(offset_clock);
 
    // 3. RESET信号生成用SM (sm_reset)
    uint offset_reset = pio_add_program(pio, &reset_out_program);
    pio_sm_config c_reset = reset_out_program_get_default_config(offset_reset);

    // GPIO初期化
    // GP10-22: 出力(8ピン BUS0-BUS7)
    for (int i = 0; i < 8; i++) {
        pio_gpio_init(pio, DATA_PINS_BASE + i);
    }
    // GP2-9：入力(8ピン MA0-MA7)
    for (int i = 0; i < 8; i++) {
      pio_gpio_init(pio, ADDR_PINS_BASE + i);
    }
    
    pio_gpio_init(pio, RESETOUT_PIN); // リセット出力ピン(GP25)の初期化
    pio_gpio_init(pio, MRD_PIN); // MRDピン(GP18)の初期化
    pio_gpio_init(pio, MWR_PIN); // MWRピン(GP19)の初期化
    pio_gpio_init(pio, TPA_PIN); // TPAピン(GP20)の初期化
    pio_gpio_init(pio, TPB_PIN); // TPBピン(GP21)の初期化
    pio_gpio_init(pio, CLKOUT_PIN); // CLK出力ピン(GP28)の初期化

    // Pullup
    gpio_set_pulls(MRD_PIN, true, false);
    gpio_set_pulls(MWR_PIN, true, false);

    // シフトレジスタの設定
    sm_config_set_in_shift(&c_write, false, false, 0); // ISR（入力シフトレジスタ）は左シフト方向
    sm_config_set_out_shift(&c_write, true, false, 0); // OSR（出力シフトレジスタ）は右シフト方向

    // シフトレジスタの設定
    sm_config_set_in_shift(&c_addr, false, false, 0); // ISR（入力シフトレジスタ）は左シフト方向
    sm_config_set_out_shift(&c_addr, true, false, 0); // OSR（出力シフトレジスタ）は右シフト方向

    // sm_clock のクロックを設定 
    sm_config_set_set_pins(&c_clock, CLKOUT_PIN, 1); // GP28をクロック出力ピンとして設定
    pio_sm_set_consecutive_pindirs(pio, sm_clock, CLKOUT_PIN, 1, true); // CLKOUTピンの初期化
    sm_config_set_clkdiv(&c_clock, (float)sysclk / 4000.0f); //  4MHz : 2MHz
    //sm_config_set_clkdiv(&c_clock, (float)sysclk / 1000.0f); //  1MHz : 0.5MHz

    // sm_reset のリセット出力を設定
    sm_config_set_set_pins(&c_reset, RESETOUT_PIN, 1); // GP25をリセット出力ピンとして設定
    pio_sm_set_consecutive_pindirs(pio, sm_reset, RESETOUT_PIN, 1, true); // リセット出力ピンの初期化 
    sm_config_set_clkdiv(&c_reset, sysclk / 10); //  10kHz (リセット出力のクロック)
    
    // sm_reset のリセット出力プログラムをロード
    pio_sm_init(pio, sm_reset, offset_reset, &c_reset);
    pio_sm_set_pins(pio, sm_reset, 0); // ピン値を0（Low）に設定（set_pinsのベースからのビット値）
    pio_sm_set_enabled(pio, sm_reset, true);

    // メモリエミュレーターのプログラムをロード
    // --- ステートマシンの初期化と有効化 ---
    pio_sm_init(pio, sm_addr, offset_addr, &c_addr);
    pio_sm_init(pio, sm_write, offset_write, &c_write);
    pio_sm_set_enabled(pio, sm_addr, true);
    pio_sm_set_enabled(pio, sm_write, true);

    sleep_ms(1); // 1ms待機

    // sm_clock のクロック出力プログラムをロード
    pio_sm_init(pio, sm_clock, offset_clock, &c_clock);
    pio_sm_set_enabled(pio, sm_clock, true);
    init_rom_basic_code(); // rom_basic_const.cから初期化
    sleep_ms(3000); // 3秒待機
    // [Enter]入力を待つ
    printf("\n[Enter] を押すとPico2 ROM/RAMエミュレータ for COSMACのテスト開始します...\n");
    while (true) {
        int c = getchar_timeout_us(100000); // 100msタイムアウト
        if (c == '\r') { // [Enter]（CR）が入力されたら開始
            printf("Pico2 ROM/RAMエミュレータ for COSMACのテスト開始...\n");
            break;
        }
    }
    printf("\nPico2 システムクロック(1.3V) - %dMHz\n", sysclk / 1000);
    printf("リセット出力状態 - ON\n");
    printf("クロック出力(2MHz) - ON\n");
    printf("ROM/RAMエミュレータ起動 - core1\n");
    multicore_launch_core1(core1_entry);
    uint32_t g = multicore_fifo_pop_blocking();
    if (g != FLAG_VALUE)
        printf("うーん、コア0では正しくありません!\n");
    else {
        multicore_fifo_push_blocking(FLAG_VALUE);
        printf("コア0ではすべてうまくいきました!\n");
    }

    uint32_t tim = 1000; 
    printf("リセット解除まで - %d ms\n", tim);  
    pio_sm_put(pio, sm_reset, tim);

    // メインループ
    printf("UART-USBブリッジ動作開始...\n");

    while (true) {
        // UARTの受信データがあるかチェック
        if (uart_is_readable(UART_ID)) {
            int c = uart_getc(UART_ID);     // UARTから1文字受信
            putchar_raw(c);                 // 受信データをそのままUSBへ送信
        }
        // USBから受信データがあるかチェック
        int c;
        if ((c = getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT) {
            uart_putc_raw(UART_ID, c); // UARTへデータ送信
        }
    }
    return 0;
}
