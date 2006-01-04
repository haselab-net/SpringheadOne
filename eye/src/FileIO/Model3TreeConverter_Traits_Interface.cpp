/*
  
  [TreeConverterが扱う木のモデル]
  
  Attribute{ Key, Value }
  Value = String or Node
  Node{ Attributes }
  
  Nodeが持つAttributeの順番は意味を持つ。
  
*/
//----------------------------------------------------------------------------
struct TTreeTraits
{
  //※※※※※※※※※※入出力の両方で必要な定義※※※※※※※※※※
  
  //属性のキーの型。
  typedef … TKey;
  //文字列の型。
  typedef … TString;
  
  //※※※※※※※※※※※入力側で必要な定義※※※※※※※※※※※※
  
  //ノードの型（入力側）。
  typedef … TInNode;
  //値の型（入力側）。
  typedef … TInValue;
  //属性の型（入力側）。
  typedef … TInAttribute;
  //ノードの各属性を舐めるイテレータ。
  typedef … TInIterator;
  
  //属性のキー。
  TKey GetKey(const TInAttribute& attr);
  //属性の値。
  TInValue GetValue(const TInAttribute& attr);
  //値が文字列ならtrue、ノードならfalse。
  bool IsString(const TInValue& value);
  //値の文字列。
  //前提条件：IsString(value)
  TString GetString(const TInValue& value);
  //値のノード。
  //前提条件：!IsString(value)
  TInNode GetNode(const TInValue& value);
  //ノードの先頭の属性へのイテレータ。
  TInIterator GetBegin(const TInNode& node);
  //ノードの末尾の次の属性へのイテレータ。
  TInIterator GetEnd(const TInNode& node);
  //iが指す属性。
  TInAttribute GetAttribute(const TInNode& node, TInIterator i);
  
  //※※※※※※※※※※※出力側で必要な定義※※※※※※※※※※※※
  
  //ノードの型（出力側）。
  typedef … TOutNode;
  //値の型（出力側）。
  typedef … TOutValue;
  //属性の型（出力側）。
  typedef … TOutAttribute;
  //属性列の型（出力側）。
  typedef … TOutAttributes;
  
  //ノードを作る。
  void CreateNode(TOutNode& node, const TOutAttributes& attrs);
  //文字列を持つ値を作る。
  void CreateValue(TOutValue& value, const TString& s);
  //ノードを持つ値を作る。
  void CreateValue(TOutValue& value, const TOutNode& node);
  //属性を作る。
  void CreateAttribute(TOutAttribute& attr, const TKey& key, const TOutValue& value);
  //属性列の末尾に属性を追加する。
  void Add(TOutAttributes& attrs, const TOutAttribute& attr);
  //属性列の末尾に属性列を追加する。
  void Add(TOutAttributes& attrs, const TOutAttributes& added);
  
};

//----------------------------------------------------------------------------
