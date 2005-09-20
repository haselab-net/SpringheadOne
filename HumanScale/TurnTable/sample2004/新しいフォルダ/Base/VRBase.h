#ifndef VRBASE_VRBASE_H
#define VRBASE_VRBASE_H
#include "Affine.h"
#include "Debug.h"
#include "RingBuf.h"
/**	@mainpage LVRM Baseライブラリ
	VR関係のプログラムを書くためのライブラリ 
	<a href = "../../html/index.html"> LVRM </a> の中で，特に基本的な機能を持つ部分を集め，LVRM Baseライブラリとしました．Baseライブラリは @ref PTM を含みます．
Baseライブラリは，
	- @ref Affine "アフィン行列・2/3次元ベクトル" (LVRM::Vec2, LVRM::Vec3, LVRM::Affine)
	- @ref PTM "n×m行列・n次元ベクトル" (PTM::TMatrix, PTM::TVector)
	- デバッグ支援機能(Debug.h)
	- コンパイラ対応マクロ(Env.h)
	- 参照カウンタ(RefCount.h)
	- その他のユーティリティー(Utilities.h)
	から構成されます．
*/

#endif
