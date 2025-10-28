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
- PCB 3Dイメージ
![専用基板Rev.0.1 3Dイメージ](./kicad/Pico2MEMEmuCosmac_PCB_3D.jpg)

## ライセンス
本プロジェクトのソースコードは[Pico2ROMEmuBR](https://github.com/kyo-ta04/Pico2ROMEmuBR)のライセンスに従います。

## 免責事項
本ソフトウェアは現状のまま提供されます。いかなる損害についても作者は責任を負いません。

## 謝辞
kyo-ta04(DragonballEZ)さんをはじめとするPico2ROMEmuプロジェクトに関わる全ての方々
