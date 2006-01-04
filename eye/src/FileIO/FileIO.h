/**	@mainpage FileIOライブラリ
シーングラフをファイルから読み出したり，保存したりするライブラリ．

ファイルのロードは次の手順で行う．
 -# ファイルをパースしてメモリに読み出し(Spr::FIFileDoc)，
    ドキュメントオブジェクト(Spr::FIDocNodeBase)を生成する．
 -# ドキュメントオブジェクトを変換し(Spr::FIDocConverter)，
    ロード可能なドキュメントオブジェクトを作成する．
 -# 作成したドキュメントオブジェクトを読み出して，
    シーングラフ(Spr::SGScene)を作成する．
*/
#include <Base/Base.h>
#include "FIDocNode.h"
#include "FIDocScene.h"
#include "FIFileDoc.h"
#include "FITypeDesc.h"
#include "FIDocConverter.h"
#include <sstream>
