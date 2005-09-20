// TGeoNodeMoveFloor.h: TGeoNodeMoveFloor クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGEONODEMOVEFLOOR_H__016B39B2_55CC_464B_BB57_8E4A15F9350A__INCLUDED_)
#define AFX_TGEONODEMOVEFLOOR_H__016B39B2_55CC_464B_BB57_8E4A15F9350A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeoNode.h"
#include "GLView.h"

class TGeoNodeMoveFloor : public TGeoNode  
{
public:
	//動く床の場所(始点の位置）
	GLfloat x;
	GLfloat y;
	GLfloat z;
	
	//床の移動幅
	GLfloat dist;
	
	//動く床の移動周期（片道の時間）
	GLint T;
	GLint reverseT;
	
	//床の速度（移動幅 / 時間）
	GLfloat vel;	
	
	//床の動く方向（周期Tで反転）
	GLint floorDirection;

	//床が動くかどうか(はじっこで止まる)
	bool bMove;

	//床の初期位置からの移動量
	GLfloat mv;
	
//	//floorTimeが周期T以上になったら動く方向が反転
	GLint floorTime;

	//Affinef afTrans;

public:
	int stopT;
	int floorNo;
	void Draw();
	void InitInstance(string name);
	TGeoNodeMoveFloor();
	virtual ~TGeoNodeMoveFloor();

};

#endif // !defined(AFX_TGEONODEMOVEFLOOR_H__016B39B2_55CC_464B_BB57_8E4A15F9350A__INCLUDED_)
