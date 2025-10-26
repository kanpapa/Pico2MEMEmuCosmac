# Pico2ROMEmu で COSMACを動かしてみた

このブランチはPico2ROMEmuのコードを使用して、COSMAC（CDP1802）のROMエミュレータとして動かしてみた実験的なものです。

## 概要
- COSMAC CPUの動作クロックは2MHzです。
- RAMは無い前提です。（MWRは見ていません）
- ROMは全アドレス空間の64KBが使用できるはずです。（ただし、未確認）
- ROMにはテストプログラムとしてQ出力のLチカを組み込んでいます。rom_basic_const.cのコメントを参照してください。

詳細はソースをご確認ください。

## 回路図
![COSMAC](./IMG/Pico2ROMEmu_COSMAC_proto01.jpg)

## ライセンス
本プロジェクトのソースコードはPico2ROMEmuのライセンスに従います。

## 免責事項
本ソフトウェアは現状のまま提供されます。いかなる損害についても作者は責任を負いません。

## 謝辞
kyo-ta04(DragonballEZ)さんをはじめとするPico2ROMEmuプロジェクトに関わる全ての方々
