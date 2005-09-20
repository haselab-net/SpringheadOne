// Human.h: CHuman クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUMAN_H__67166EB3_4978_48B7_A4E6_77DA55034E81__INCLUDED_)
#define AFX_HUMAN_H__67166EB3_4978_48B7_A4E6_77DA55034E81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Physics/PHJoint.h>

using namespace Spr;
class CLeg {
public:
	PHJoint* joint[3];
	bool Connect(SGScene* s, const char* prefix);
	///	Bodyに対する相対位置
	Vec3f GetPos();
	void SetPos(Vec3f l);
};
class CHuman  {
public:
	bool bGood;
	CLeg leg[2];
	int swing, stance;
	UTRef<PHSolid> soBody;

	CHuman();
	virtual ~CHuman();
	bool Connect(SGScene* s);
	void Step();
};

#endif // !defined(AFX_HUMAN_H__67166EB3_4978_48B7_A4E6_77DA55034E81__INCLUDED_)
