QuadPR.h 使用上の注意
QuadPR.h　は2次計画問題を解く関数を含んでいます。
使うためには、
	libF77.lib
	libI77.lib
	QuadPR.lib
の2つのライブラリが必要です。

使用するコンパイラと環境によって、リンクするライブラリを次のように切り替えてください。
Visual C++
 デバッグ版MFCなし	libF77D.lib	libF77D.lib	QuadPRD.lib	
 デバッグ版MFCあり	libF77MD.lib	libF77MD.lib	QuadPRMD.lib
 リリース版MFCなし	libF77.lib	libF77.lib	QuadPR.lib
 リリース版MFCあり	libF77M.lib	libF77M.lib	QuadPRM.lib

Borland C++ Builder
 libF77BCB.lib	libF77BCB.lib	QuadPRBCB.lib
