# 1802メンバーシップカード用スーパーモニタ＋BASIC3を組み込む方法
ここでは1802メンバーシップカード用に公開されているMCSMP Super Monitor Program plus BASIC3のバイナリデータをPico2MEMEmuCosmacに組み込む手順をまとめました。

## 作業環境
- Ubuntu 24.04
- 他の環境でも同様にできると思います。

## 作業手順
1. [1802メンバーシップカードのホームページ](https://sunrise-ev.com/1802.htm)からMCSMP Super Monitor Program plus BASIC3のバイナリをダウンロードする。

    SoftwareのMonitor programsにMCSMP Super Monitor Program plus BASIC3がありますので、MCSMP20A.binをダウンロードします。このバイナリは開始アドレスが0x0000になっていて、32KbyteのROMに書き込めるイメージになっています。

1. MCSMP20Aのバイナリをリポジトリのディレクトリにコピーする。

    ```
    cp MCSMP20A.bin ~/Pico2MEMEmuCosmac/.
    ```

1. 現在のソースコードを退避する。

    ```
    mv rom_basic_const.c rom_basic_const.c.org
    ```

1. バイナリデータをソースコードに変換するpython3プログラムを実行する。

    ```
    python3 mcsmp20a_to_src.py MCSMP20A.bin > rom_basic_const.c
    ```

1. 作成されたソースコードを確認する。MCSMP20A.binが配列として組み込まれていればOKです。

    ```
    cat rom_basic_const.c
    ```

1. Pico2ROMEmuBR.cのメモリ設定を以下のように変更する。ROMを32KByteを変更し, RAMの開始が0x8000で32KByteになっていれば問題ありません。

    ```
    #define RAM_START 0x8000 // RAM start address  
    //#define ROM_SIZE 0x4000 // ROM size 16k bytes  
    #define ROM_SIZE 0x8000 // ROM size 32k bytes  
    #define RAM_SIZE 0x8000 // RAM size 32k bytes  
    ```

1. Pico2のファームウェアをビルドする。
1. Pico2MEMEmuCosmacのPico2にUSBケーブルでPCを接続し、生成されたファームウェアを書き込みます。
1. PCでターミナルを立ち上げてPico2に接続します。通信速度は9600bps, Data:8bit, Stop:1bit, Parity:Nonです。遅延時間は1文字毎に20ms, 改行毎に500msぐらいが必要です。
1. Enterキーを一度押すと、エミュレーターが動きはじめます。
1. Enterキーをもう一度押すと、スーパーモニタが起動します。モニタとしての機能を楽しんでください。
1. モニタのBコマンドでBASIC3が起動します。サンプルプログラムのASCIIART.BASをターミナルからアップロードして実行してみてください。
