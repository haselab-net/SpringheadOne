#ifndef _LIBWRL2VIEW_H_
#define	_LIBWRL2VIEW_H_

// Wrl2view���C�u�����֐��̃v���g�^�C�v�錾

#include "type.h"

namespace Spr {

// �ϐ�
extern float	fieldOfView;
extern float	viewpos[];
extern float	vieworient[];
extern float	viewMatrix[16];

// �֐�
extern map<string, TNode*>& NodeIDMap();
extern void InitList(TSts* sts);
extern int DrawList(TSts* sts);

}
#endif /* _LIBWRL2VIEW_H_ */
