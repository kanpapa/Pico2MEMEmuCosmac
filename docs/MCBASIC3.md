# RCA BASIC3を組み込む方法
ここでは1802メンバーシップカード用に公開されているRCA BASIC3のバイナリデータをPico2MEMEmuCosmacに組み込む手順をまとめました。

## 作業環境
- Ubuntu 24.04
- Pythonが動作すれば他の環境でも同様にできると思います。

## 作業手順
1. [1802メンバーシップカードのホームページ](https://sunrise-ev.com/1802.htm)からRCA BASIC3のバイナリをダウンロードする。

    SoftwareのBASIC for 1802にBASIC3がありますので、`MCBASIC3.bin`をダウンロードします。このバイナリは開始アドレスが0x0000になっていて、16KbyteのROMに書き込めるイメージになっています。

1. RCA BASIC3のバイナリをリポジトリのディレクトリにコピーする。

    ```
    cp MBASIC3.bin ~/Pico2MEMEmuCosmac/.
    ```

1. 現在のROMデータのソースコードを退避する。

    ```
    mv rom_const.c rom_const.c.org
    ```

1. バイナリデータをソースコードに変換するpython3プログラムを実行する。

    ```
    python3 convert_tools/mcbasic3_to_src.py MCBASIC3.bin > rom_const.c
    ```

1. 作成されたソースコードを確認する。MCBASIC3.binが配列として組み込まれていればOKです。

    ```
    cat rom_const.c
    ```

1. `Pico2MEMEmuCosmac.c`のメモリ設定を確認する。ROMが16KByte, RAMの開始が0x8000で32KByteになっていれば問題ありません。

    ```
    #define RAM_START 0x8000 // RAM start address  
    #define ROM_SIZE 0x4000 // ROM size 16k bytes  
    #define RAM_SIZE 0x8000 // RAM size 32k bytes  
    ```

1. Pico2のファームウェアを[ビルド](/docs/raspberry-pi-pico-vscode-build.md)してください。
1. Pico2MEMEmuCosmacのPico2にUSBケーブルでPCを接続し、生成されたファームウェアを書き込みます。
1. PCでターミナルを立ち上げてPico2に接続します。通信速度は9600bps, Data:8bit, Stop:1bit, Parity:Nonです。遅延時間は1文字毎に20ms, 改行毎に500msぐらいが必要です。
1. Enterキーを一度押すと、エミュレーターが動きはじめます。
1. Enrerキーをもう一度押すと、RCA BASIC3が起動します。
1. samplesディレクトリにある[ASCIIART.BAS](../samples/ASCIIART.BAS)をターミナルからアップロードして実行してみてください。
