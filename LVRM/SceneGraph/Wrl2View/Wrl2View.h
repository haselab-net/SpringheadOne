#ifndef _LIBWRL2VIEW_H_
#define	_LIBWRL2VIEW_H_

// Wrl2viewライブラリ関数のプロトタイプ宣言

#include "type.h"

namespace LVRM {

// 変数
extern float	fieldOfView;
extern float	viewpos[];
extern float	vieworient[];

// 関数
extern map<string, TNode*>& NodeIDMap();
extern void InitList(TSts* sts);
extern int DrawList(TSts* sts);

}
#endif /* _LIBWRL2VIEW_H_ */
