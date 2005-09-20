// TGeoNodeNum.h: TGeoNodeNum �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGEONODENUM_H__0E5906F9_2C69_4828_ADE5_565C1CA5EE93__INCLUDED_)
#define AFX_TGEONODENUM_H__0E5906F9_2C69_4828_ADE5_565C1CA5EE93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeoNode.h"
#include "GLView.h"

class TGeoNodeNum : public TGeoNode  
{
public:
	bool isDraw;		//�`�悷�邩�ǂ���	
	Affinef afTrans;	//�`��ʒu	
	int number;			//���̃I�u�W�F�N�g�̌`
	int digit;			//���̃I�u�W�F�N�g�̌�
	int vn;				//

public:
	virtual void InitInstance(string name);
	virtual void Draw();
	virtual void control();
//	virtual void isOffDraw();
	virtual Affinef setTrans(int digit);
	TGeoNodeNum();
//	TGeoNodeNum(string name);
	virtual ~TGeoNodeNum();
};
	
class TGeoNodeTimer : public TGeoNodeNum  
{
public:
	void control();
//	void Draw();
	virtual ~TGeoNodeTimer(){}
	TGeoNodeTimer(){TGeoNodeNum();}
};

class TGeoNodeScore : public TGeoNodeNum  
{
public:
	int size;
	Affinef setTrans(int digit);
//	void Draw();
	void control();
	void InitInstance(string name);
	virtual ~TGeoNodeScore(){}
	TGeoNodeScore(){TGeoNodeNum();}
};

#endif // !defined(AFX_TGEONODENUM_H__0E5906F9_2C69_4828_ADE5_565C1CA5EE93__INCLUDED_)
