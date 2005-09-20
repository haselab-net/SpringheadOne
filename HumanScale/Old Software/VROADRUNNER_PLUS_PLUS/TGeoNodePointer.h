// TGeoNodePointer.h: TGeoNodePointer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGEONODEPOINTER_H__5D26FE86_D268_4D67_8863_91CAEE101F70__INCLUDED_)
#define AFX_TGEONODEPOINTER_H__5D26FE86_D268_4D67_8863_91CAEE101F70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeoNode.h"
#define ALGORITHM2

class TGeoNodePointer : public TGeoNode  
{
public:
	bool isDraw;
	bool isAttack;
	float g;
	float dy;
	float dz;
#ifdef ALGORITHM2
	int count; //何回Drawが実行されたかのカウント
	int upcount; //上昇終了までのカウント
	int downcount; //下降開始までのカウント
#endif

public:
	bool IsDraw();
	float radius;
	float GetRadius();
	void Draw();
	void Step(); // Stepの時に呼ぶ。前進など
	bool Attack( Affinef &_afStart ); // 打つ
	bool Attack( Affinef &afCam,Affinef &afLaser );
	Affinef GetPosition();
	void Hide();
	TGeoNodePointer();
	virtual ~TGeoNodePointer();

};

#endif // !defined(AFX_TGEONODEPOINTER_H__5D26FE86_D268_4D67_8863_91CAEE101F70__INCLUDED_)
