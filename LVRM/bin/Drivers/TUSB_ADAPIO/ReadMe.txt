インストール時の注意点

古いドライバが入っているとうまくインストールできないことがあります．
この場合，レジストリエディタを使って，レジストリを削除して，再起動する
必要があるようです．(未確認)

regedit を立ち上げて，
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\ と
HKEY_LOCAL_MACHINE\SYSTEM\ControlSet？？？\の下の
	Services\WinDriver
	Services\wdusb
を削除してください．
