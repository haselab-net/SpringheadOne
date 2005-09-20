/*
  
  [TreeConverter�������؂̃��f��]
  
  Attribute{ Key, Value }
  Value = String or Node
  Node{ Attributes }
  
  Node������Attribute�̏��Ԃ͈Ӗ������B
  
*/
//----------------------------------------------------------------------------
struct TTreeTraits
{
  //�����������������������o�̗͂����ŕK�v�Ȓ�`��������������������
  
  //�����̃L�[�̌^�B
  typedef �c TKey;
  //������̌^�B
  typedef �c TString;
  
  //�������������������������͑��ŕK�v�Ȓ�`������������������������
  
  //�m�[�h�̌^�i���͑��j�B
  typedef �c TInNode;
  //�l�̌^�i���͑��j�B
  typedef �c TInValue;
  //�����̌^�i���͑��j�B
  typedef �c TInAttribute;
  //�m�[�h�̊e�������r�߂�C�e���[�^�B
  typedef �c TInIterator;
  
  //�����̃L�[�B
  TKey GetKey(const TInAttribute& attr);
  //�����̒l�B
  TInValue GetValue(const TInAttribute& attr);
  //�l��������Ȃ�true�A�m�[�h�Ȃ�false�B
  bool IsString(const TInValue& value);
  //�l�̕�����B
  //�O������FIsString(value)
  TString GetString(const TInValue& value);
  //�l�̃m�[�h�B
  //�O������F!IsString(value)
  TInNode GetNode(const TInValue& value);
  //�m�[�h�̐擪�̑����ւ̃C�e���[�^�B
  TInIterator GetBegin(const TInNode& node);
  //�m�[�h�̖����̎��̑����ւ̃C�e���[�^�B
  TInIterator GetEnd(const TInNode& node);
  //i���w�������B
  TInAttribute GetAttribute(const TInNode& node, TInIterator i);
  
  //�����������������������o�͑��ŕK�v�Ȓ�`������������������������
  
  //�m�[�h�̌^�i�o�͑��j�B
  typedef �c TOutNode;
  //�l�̌^�i�o�͑��j�B
  typedef �c TOutValue;
  //�����̌^�i�o�͑��j�B
  typedef �c TOutAttribute;
  //������̌^�i�o�͑��j�B
  typedef �c TOutAttributes;
  
  //�m�[�h�����B
  void CreateNode(TOutNode& node, const TOutAttributes& attrs);
  //����������l�����B
  void CreateValue(TOutValue& value, const TString& s);
  //�m�[�h�����l�����B
  void CreateValue(TOutValue& value, const TOutNode& node);
  //���������B
  void CreateAttribute(TOutAttribute& attr, const TKey& key, const TOutValue& value);
  //������̖����ɑ�����ǉ�����B
  void Add(TOutAttributes& attrs, const TOutAttribute& attr);
  //������̖����ɑ������ǉ�����B
  void Add(TOutAttributes& attrs, const TOutAttributes& added);
  
};

//----------------------------------------------------------------------------
