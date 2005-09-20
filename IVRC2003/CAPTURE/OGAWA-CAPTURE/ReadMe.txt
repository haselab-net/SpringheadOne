カメラでキャプチャして、ウィンドウに描画するサンプル。
「Start」ボタンで映像を表示します。
「Capture」ボタンで画像を得てウィンドウに描画します。
「Stop」ボタンで映像の表示をやめます。

ソースは長いけれど、ほとんどはVC++により作られたもので、
実際に書いた処理は CTest3Dlg の
・OnCreate
・OnButtonStart
・OnButtonCapture
・OnButtonStop
・OnDestroy
です。