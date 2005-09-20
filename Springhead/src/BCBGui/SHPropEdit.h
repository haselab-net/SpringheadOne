//---------------------------------------------------------------------------
#ifndef SPH_SHPropEditH
#define SPH_SHPropEditH
//---------------------------------------------------------------------------
#include <DesignEditors.hpp>
//---------------------------------------------------------------------------
//�\������ύX����v���p�e�B�G�f�B�^
template <char Name[], class TBaseProperty>
  //Name: �\�������v���p�e�B��
class TNamedProperty: public TBaseProperty
{
public:
  __fastcall TNamedProperty(const Designintf::_di_IDesigner ADesigner,
      int APropCount)
    : TBaseProperty(ADesigner, APropCount){}
  //�\�������v���p�e�B��
  virtual AnsiString __fastcall GetName() { return Name; }

};

//---------------------------------------------------------------------------
#endif
