//�؂̃p�^�[�����`����̂ɕ֗��Ȋ֐��Q
template <class TTraits>
struct FITreePatterns
{
  //�����Ƀ}�b�`����p�^�[���B
  TAttributesPattern Attribute(const TKey& key, const TValuesPattern& values);
  
  //�����1�̕�����Ƀ}�b�`����p�^�[��
  TValuesPattern String(const std::string& s);
  //1�̃m�[�h�Ƀ}�b�`����p�^�[��
  TValuesPattern Node(const TAttributesPattern& attrs);
  //�C�ӂ�1�̒l�Ƀ}�b�`����p�^�[��
  TValuesPattern AnyValue();
  //�C�ӌ̔C�ӂ̒l�Ƀ}�b�`����p�^�[��
  TValuesPattern AnyValues();
};

TAttributesPattern attr1, attr2;
attr1 >> attr2 //����

TValuesPattern val1, val2;
val1 >> val2 //����
!val1 //�ȗ���
