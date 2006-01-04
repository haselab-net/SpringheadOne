//---------------------------------------------------------------------------
#ifndef SPH_CBPTMPropEditH
#define SPH_CBPTMPropEditH
//---------------------------------------------------------------------------
#include <DesignEditors.hpp>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//TSHVec3fDummy�p�̃v���p�e�B�G�f�B�^
template <char Name[]>
	//Name: �\�������v���p�e�B��
class TVectorProperty: public TClassProperty
{
public:
	__fastcall TVectorProperty(const Designintf::_di_IDesigner ADesigner,
  		int APropCount)
		: TClassProperty(ADesigner, APropCount){}
  //�\�������v���p�e�B��
	virtual AnsiString __fastcall GetName() {return Name;}
  //�v���p�e�B�̕�����\��
	virtual AnsiString __fastcall GetValue() {return "(SHVec3f)";}

};

//---------------------------------------------------------------------------
//TSHMatrix3fDummy�p�̃v���p�e�B�G�f�B�^
template <char Name[]>
	//Name: �\�������v���p�e�B��
class TMatrixProperty: public TClassProperty
{
public:
	__fastcall TMatrixProperty(const Designintf::_di_IDesigner ADesigner,
  		int APropCount)
		: TClassProperty(ADesigner, APropCount){}
  //�\�������v���p�e�B��
	virtual AnsiString __fastcall GetName() {return Name;}
  //�v���p�e�B�̕�����\��
	virtual AnsiString __fastcall GetValue() {return "(SHMatrix3f)";}

};
//---------------------------------------------------------------------------
//TSHAffinefDummy�p�̃v���p�e�B�G�f�B�^
template <char Name[]>
	//Name: �\�������v���p�e�B��
class TAffineProperty: public TClassProperty
{
public:
	__fastcall TAffineProperty(const Designintf::_di_IDesigner ADesigner,
  		int APropCount)
		: TClassProperty(ADesigner, APropCount){}
	//�\�������v���p�e�B��
	virtual AnsiString __fastcall GetName() {return Name;}
	//�v���p�e�B�̕�����\��
	virtual AnsiString __fastcall GetValue() {return "(SHAffinef)";}
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
