//---------------------------------------------------------------------------
#ifndef SPH_CBPTMPropEditH
#define SPH_CBPTMPropEditH
//---------------------------------------------------------------------------
#include <DesignEditors.hpp>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//TSHVec3fDummy用のプロパティエディタ
template <char Name[]>
	//Name: 表示されるプロパティ名
class TVectorProperty: public TClassProperty
{
public:
	__fastcall TVectorProperty(const Designintf::_di_IDesigner ADesigner,
  		int APropCount)
		: TClassProperty(ADesigner, APropCount){}
  //表示されるプロパティ名
	virtual AnsiString __fastcall GetName() {return Name;}
  //プロパティの文字列表現
	virtual AnsiString __fastcall GetValue() {return "(SHVec3f)";}

};

//---------------------------------------------------------------------------
//TSHMatrix3fDummy用のプロパティエディタ
template <char Name[]>
	//Name: 表示されるプロパティ名
class TMatrixProperty: public TClassProperty
{
public:
	__fastcall TMatrixProperty(const Designintf::_di_IDesigner ADesigner,
  		int APropCount)
		: TClassProperty(ADesigner, APropCount){}
  //表示されるプロパティ名
	virtual AnsiString __fastcall GetName() {return Name;}
  //プロパティの文字列表現
	virtual AnsiString __fastcall GetValue() {return "(SHMatrix3f)";}

};
//---------------------------------------------------------------------------
//TSHAffinefDummy用のプロパティエディタ
template <char Name[]>
	//Name: 表示されるプロパティ名
class TAffineProperty: public TClassProperty
{
public:
	__fastcall TAffineProperty(const Designintf::_di_IDesigner ADesigner,
  		int APropCount)
		: TClassProperty(ADesigner, APropCount){}
	//表示されるプロパティ名
	virtual AnsiString __fastcall GetName() {return Name;}
	//プロパティの文字列表現
	virtual AnsiString __fastcall GetValue() {return "(SHAffinef)";}
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
