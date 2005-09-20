//---------------------------------------------------------------------------
#ifndef SPH_SHPropEditH
#define SPH_SHPropEditH
//---------------------------------------------------------------------------
#include <DesignEditors.hpp>
//---------------------------------------------------------------------------
//表示名を変更するプロパティエディタ
template <char Name[], class TBaseProperty>
  //Name: 表示されるプロパティ名
class TNamedProperty: public TBaseProperty
{
public:
  __fastcall TNamedProperty(const Designintf::_di_IDesigner ADesigner,
      int APropCount)
    : TBaseProperty(ADesigner, APropCount){}
  //表示されるプロパティ名
  virtual AnsiString __fastcall GetName() { return Name; }

};

//---------------------------------------------------------------------------
#endif
