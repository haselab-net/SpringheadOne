//木のパターンを定義するのに便利な関数群
template <class TTraits>
struct FITreePatterns
{
  //属性にマッチするパターン。
  TAttributesPattern Attribute(const TKey& key, const TValuesPattern& values);
  
  //特定の1つの文字列にマッチするパターン
  TValuesPattern String(const std::string& s);
  //1つのノードにマッチするパターン
  TValuesPattern Node(const TAttributesPattern& attrs);
  //任意の1つの値にマッチするパターン
  TValuesPattern AnyValue();
  //任意個の任意の値にマッチするパターン
  TValuesPattern AnyValues();
};

TAttributesPattern attr1, attr2;
attr1 >> attr2 //結合

TValuesPattern val1, val2;
val1 >> val2 //結合
!val1 //省略可
