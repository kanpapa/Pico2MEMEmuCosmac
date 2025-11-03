# Pico2MEMEmuCOSMAC

このリポジトリはkyo-ta04(DragonballEZ)さんの[Pico2ROMEmuBR](https://github.com/kyo-ta04/Pico2ROMEmuBR)のコードを応用して、COSMAC（CDP1802）のROM/RAMエミュレータとして動かしてみた実験的なものです。

## 概要
- COSMAC CPUの動作クロックは1.79MHzです。
- ROM/RAMは各32KBを割り当てていますが、全アドレス空間内であれば設定はずです。（ただし、未確認）
- ROMにはテストプログラムとしてQ出力のLチカを組み込んでいます。rom_basic_const.cのコメントを参照してください。
- 専用基板Rev.0.1にはSBC1802にあるインジケーターをオプション回路として追加しています。

詳細はソースをご確認ください。

## 回路図
- ブレッドボード版回路図
![ブレッドボード版回路図](./IMG/Pico2MEMEmuCOSMAC_proto01.jpg)
- 専用基板Rev.0.1 回路図
![専用基板回路図](./IMG/Pico2MEMEmuCosmac_sch_rev01.jpg)

## PCB
- [KiCADデータ](./kicad)
- [Gerberデータ](./kicad/gerber/)
- PCB 3Dイメージ
![専用基板Rev.0.1 3Dイメージ](./kicad/Pico2MEMEmuCosmac_PCB_3D.jpg)

## BOM
|Item|Qty|Reference(s)|Value|Description|Datasheet|
| --- | --- | --- | --- | --- | --- |
|1|1|A1|RaspberryPi_Pico2|Raspberry Pi Pico 2|https://akizukidenshi.com/catalog/g/g129604/|
|2|1|U1|CDP1802ACE|CDP1802ACE microprocessor|http://www.cosmacelf.com/publications/data-sheets/cdp1802.pdf|
|3|1|C1|0.1uF|リード型積層セラミックコンデンサー 0.1μF50V F 2.54mm|https://akizukidenshi.com/catalog/g/g100090/|
|4|1|D1|1N4148|汎用小信号高速スイッチング・ダイオード 1N4148 100V200mA|https://akizukidenshi.com/catalog/g/g100941/|
|5|1|R1|100|カーボン抵抗(炭素皮膜抵抗) 1/6W100Ω|https://akizukidenshi.com/catalog/g/g116101/|
|6|2|RN1,RN2|10K|集合抵抗 4素子 10kΩ|https://akizukidenshi.com/catalog/g/g111909/|
|7|1|SW1|RUN|タクトスイッチ(水色)|https://akizukidenshi.com/catalog/g/g103649/|
|8|1|U1 socket||丸ピンICソケット (40P)|https://akizukidenshi.com/catalog/g/g100034/|
|9※|1|J1|Conn_01x06|ピンヘッダー (L型) 1×6 (6P)|https://akizukidenshi.com/catalog/g/g105336/|
|10※|1|U2|74HC04|6回路入インバーター TC74HC04AF|https://akizukidenshi.com/catalog/g/g111490/|
|11※|5|D2,D3,D4,D5,D6|LED|緑色チップLED 1608 OSG50603C1E|https://akizukidenshi.com/catalog/g/g108275/|
|12※|5|R2,R3,R4,R5,R6|1k|チップ抵抗 1608 1/8W1kΩ|https://akizukidenshi.com/catalog/g/g130351/|

※9～12はオプションです。

## ライセンス
本プロジェクトのソースコードは[Pico2ROMEmuBR](https://github.com/kyo-ta04/Pico2ROMEmuBR)のライセンスに従います。

## 免責事項
本ソフトウェアは現状のまま提供されます。いかなる損害についても作者は責任を負いません。

## 謝辞
kyo-ta04(DragonballEZ)さんをはじめとするPico2ROMEmuプロジェクトに関わる全ての方々
