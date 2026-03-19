# Visual Studio Code での Raspberry Pi Pico プロジェクトのビルド方法

Raspberry Pi Pico2のファームウェアのビルド環境はWindowsだとVisual Studio Codeの拡張機能を使用するのがおすすめです。  

Windowsの場合はコンパイルに必要なツールは自動的にユーザディレクトリの.pico-sdkディレクトリにインストールされます。

## Pico2開発環境の準備  

1. Visual Studio Codeをインストールしてください。  
    https://code.visualstudio.com/
1. 拡張機能でRaspberry Pi Picoをインストールしてください。
    ![Raspberry Pi Pico Extension](/docs/raspberry-pi-pico-extension.png)

## プロジェクトのインポート

1. VSCodeで新しいウインドウを開きます。
1. `Gitリポジトリのクローン…`をクリックします。
1. リポジトリのURLを入力します。  
    https://github.com/kanpapa/Pico2MEMEmuCosmac.git
1. リポジトリを保存するフォルダを選択します。ユーザディレクトリの直下で良いでしょう。
1. リポジトリの保存が終わると以下のようなメニューが表示されます。

    ![Raspberry Pi Pico project?](/docs/raspberry-pi-pico-project-yes.png)

1. ここでYESを押すと以下のインポート画面になります。

    ![Raspberry Pi pico import project](/docs/raspberry-pi-pico-project-import.png)

    Locationが正しいことを確認して`import`を押してください。

1. 正常にインポートできれば、.vscodeフォルダが作成され、ビルドに必要な設定が保存されます。

    ![created .vscode](/docs/raspberry-pi-pico-dot-vscode.png)

## Pico2のファームウェアのビルド

1. アクティビティバーにあるPicoのアイコンをクリックすると以下のメニューが表示されます。

    ![Quick access menu](/docs/raspberry-pi-pico-quick-access.png)

1. このメニューの`Compile Project`をクリックします。
1. 正常にビルドが終了すればbuildディレクトリにファームウェア`Pico2MEMEmuCosmac.uf2`が生成されます。  

## ファームウェアのインストール

1. Pico2をUSBケーブルでPCに接続するとPico2のフォルダにアクセスできます。
1. 生成されたファームウェア`Pico2MEMEmuCosmac.uf2`をこのフォルダにドラッグ＆ドロップすることでPico2が新しいファームウェアで動き始めます。

