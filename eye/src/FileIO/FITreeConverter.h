#ifndef FI_TREE_CONVERTER_H
#define FI_TREE_CONVERTER_H
//---------------------------------------------------------------------------
//木構造から木構造への変換器(FIHalfTreeConverter)と、
//その変換ルールを記述するための各種パターン(FIPattern)。
//---------------------------------------------------------------------------
#include <utility>
#include <map>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/parse_tree.hpp>
//---------------------------------------------------------------------------
//ラベル付きノード列の種類
enum FILabeledType{
	filtNodes,//更なるマッチングの対象になるノード列
	filtValue//これ以上マッチングせず、値として扱う
};

//---------------------------------------------------------------------------
//木の変換で必要な、型の組み合わせごとに独自の定義が必要な操作の集合
//（特殊化して使う）
template <class TL, class TR>
struct FIConvertationTraits;
//---------------------------------------------------------------------------
//木変換系のクラスで共通に使う定義集（継承して使う）
template <class TTraits>
class FITreeDefs
{
public:
	//属性のキーの型。
	typedef typename TTraits::TKey TKey;
	//文字列の型。
	typedef typename TTraits::TString TString;

  //ノードの型（入力側）。
	typedef typename TTraits::TInNode TInNode;
  //値の型（入力側）。
	typedef typename TTraits::TInValue TInValue;
  //属性の型（入力側）。
	typedef typename TTraits::TInAttribute TInAttribute;
	//ノードの各属性を舐めるイテレータ。
	typedef typename TTraits::TInIterator TInIterator;

	//属性のキー。
	static TKey GetKey(const TInAttribute& attr){
		return TTraits::GetKey(attr);
	}
  //属性の値。
	static TInValue GetValue(const TInAttribute& attr){
		return TTraits::GetValue(attr);
	}
  //値が文字列ならtrue、ノードならfalse。
	static bool IsString(const TInValue& value){
		return TTraits::IsString(value);
	}
	//値の文字列。
	static TString GetString(const TInValue& value){
		return TTraits::GetString(value);
	}
  //値のノード。
	static TInNode GetNode(const TInValue& value){
		return TTraits::GetNode(value);
	}
	//子ノードの先頭を取得
	static TInIterator GetBegin(const TInNode& node){
		return TTraits::GetBegin(node);
	}
	//子ノードの末尾の次を取得
	static TInIterator GetEnd(const TInNode& node){
		return TTraits::GetEnd(node);
	}
  //iが指す属性。
	static TInAttribute GetAttribute(const TInNode& node, TInIterator i){
		return TTraits::GetAttribute(node, i);
	}

  //ノードの型（出力側）。
	typedef typename TTraits::TOutNode TOutNode;
  //値の型（出力側）。
  typedef typename TTraits::TOutValue TOutValue;
	//属性の型（出力側）。
	typedef typename TTraits::TOutAttribute TOutAttribute;
	//属性列の型（出力側）。
	typedef typename TTraits::TOutAttributes TOutAttributes;

  //ノードを作る。
	static void CreateNode(TOutNode& node, const TOutAttributes& attrs){
		TTraits::CreateNode(node, attrs);
	}
  //文字列を持つ値を作る。
  static void CreateValue(TOutValue& value, const TString& s){
		TTraits::CreateValue(value, s);
	}
  //ノードを持つ値を作る。
  static void CreateValue(TOutValue& value, const TOutNode& node){
		TTraits::CreateValue(value, node);
	}
  //属性を作る。
  static void CreateAttribute(TOutAttribute& attr, const TKey& key, const TOutValue& value){
		TTraits::CreateAttribute(attr, key, value);
	}
  //属性列の末尾に属性を追加する。
  static void Add(TOutAttributes& attrs, const TOutAttribute& attr){
		TTraits::Add(attrs, attr);
	}
  //属性列の末尾に属性列を追加する。
  static void Add(TOutAttributes& attrs, const TOutAttributes& added){
		TTraits::Add(attrs, added);
	}

	//ラベルを付けられたノード列の情報
	struct TLabeledInfo{
		TInNode node;//ノード
		TInIterator first;//ノード列の先頭
		TInIterator last;//ノード列の末尾の次
		TLabeledInfo(){}
		TLabeledInfo(const TInNode& n, TInIterator f, TInIterator l)
			: node(n), first(f), last(l){}
	};
	//値の配列（出力側）。
	typedef std::vector<TOutValue> TOutValues;

	//ノード列マッチング中に、ラベルに対応する値列を記録するためのマップ。
	typedef std::map<int, TLabeledInfo> TSourceLabelMap;
	struct TSourceLabelMaps{
		TSourceLabelMap values;
		TSourceLabelMap attrs;
	};
	//ノード列生成時に、ラベルに対応する値列を与えるためのマップ。
	typedef std::map<int, TOutValues> TTargetValueLabelMap;
	typedef std::map<int, TOutAttributes> TTargetAttributeLabelMap;
	struct TTargetLabelMaps{
		TTargetValueLabelMap values;
		TTargetAttributeLabelMap attrs;
	};

};
//---------------------------------------------------------------------------
//木のパターンの実装の基底クラス
template <class TTraits>
class FIPatternImpl: public FITreeDefs<TTraits>
{
public:
	FIPatternImpl(): label(0){}
	virtual ~FIPatternImpl(){}
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap)= 0;
	//パターンを雛型として属性列を作り、attrsに追加する。
	//自分にラベルが付いていたら、ラベルに対応する属性列を返す。
	void Generate(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//ラベルを付ける
	void SetLabel(int l){ label= l; }
	//ラベルが付いていたら、それをlmapに記録する
	void SaveIfLabeled(
		const TInNode& node, 
		TInIterator first, 
		TInIterator last, 
		TSourceLabelMaps* lmap,
		bool asAttrs= false);
	//マッチする属性のキー。
	virtual TKey GetMatchingKey()const= 0;
	virtual void SetMatchingKey(const TKey& k)= 0;

protected:
	//パターンを元に属性列を作り、attrsに追加する。
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap)= 0;
	//ラベルに対応する属性列を作り、attrsに追加する。
	virtual void GenerateLabeled(
		TOutAttributes& attrs, const TOutValues& values);

private:
	int label;//ラベル

};
//---------------------------------------------------------------------------
//木のパターン
//（値として扱えるようにFIPatternImplをラップしている）
template <class TTraits>
class FIPattern: public FITreeDefs<TTraits>
{
public:
	explicit FIPattern(FIPatternImpl<TTraits>* i): impl(i){}
	//ノード列がパターンにマッチするかを調べる
	std::pair<bool, TInIterator> Match(
		const TInNode& pnode, 
		TInIterator first, 
		TInIterator last, 
		TSourceLabelMaps* lmap)const{
			return impl->Match(pnode, first, last, lmap);
	}
	//パターンを雛型として属性列を作り、attrsに追加する。
	//自分にラベルが付いていたら、ラベルに対応する属性列を返す。
	void Generate(TOutAttributes& attrs, const TTargetLabelMaps* lmap)const{
		impl->Generate(attrs, lmap);
	}
	//ラベルを付ける
	FIPattern& operator[](int l){
		impl->SetLabel(l);
		return *this;
	}
	//自身をFIOptionalPatternImplでくるんで返す
	FIPattern operator!();
	//実装
	boost::shared_ptr<FIPatternImpl<TTraits> > GetImpl()const{ return impl; }

private:
	boost::shared_ptr<FIPatternImpl<TTraits> > impl;//実装

};
//---------------------------------------------------------------------------
//パターンをつなげる
template <class T>
inline FIPattern<T> operator>>(
  const FIPattern<T>& firstPattern, const FIPattern<T>& secondPattern);
//---------------------------------------------------------------------------
//単独のノードを持つ属性を表すパターン
template <class TTraits>
class FINodePatternImpl: public FIPatternImpl<TTraits>
{
public:
	//名前と子ノード列のパターンを指定する
	FINodePatternImpl(const TKey& k, const FIPattern<TTraits>& p)
		: key(k), childrenPattern(p){}
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);

protected:
	//パターンを元に属性列を作り、attrsに追加する。
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//マッチする属性のキー。
	virtual TKey GetMatchingKey()const{ return key; }
	virtual void SetMatchingKey(const TKey& k);

private:
	TKey key;//マッチする属性のキー
	FIPattern<TTraits> childrenPattern;//子ノード列のパターン

};
//---------------------------------------------------------------------------
//単独の文字列を持つ属性を表すパターン
template <class TTraits>
class FIStringPatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIStringPatternImpl(const TKey& k, const TString& v)
		: key(k), str(v){}
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);

protected:
	//パターンを元に属性列を作り、attrsに追加する。
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//マッチする属性のキー。
	virtual TKey GetMatchingKey()const{ return key; }
	virtual void SetMatchingKey(const TKey& k);

private:
	TKey key;//マッチする属性のキー
	TString str;//文字列

};
//---------------------------------------------------------------------------
//任意の値を持つ属性を表すパターン
template <class TTraits>
class FIAnyValuePatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIAnyValuePatternImpl(const TKey& k)
		: key(k){}
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);

protected:
	//パターンを元に属性列を作り、attrsに追加する。
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//マッチする属性のキー。
	virtual TKey GetMatchingKey()const{ return key; }
	virtual void SetMatchingKey(const TKey& k){ key= k; }

private:
	TKey key;//マッチする属性のキー

};
//---------------------------------------------------------------------------
//パターンのつながり
//（記述時には >> で表現する）
template <class TTraits>
class FIConjunctionPatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIConjunctionPatternImpl(const FIPattern<TTraits>& f, const FIPattern<TTraits>& s)
		: firstPattern(f), secondPattern(s){}
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);

protected:
	//パターンを元に属性列を作り、attrsに追加する。
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//マッチする属性のキー。
	virtual TKey GetMatchingKey()const;
	virtual void SetMatchingKey(const TKey& k);

private:
	FIPattern<TTraits> firstPattern;//1つ目のパターン
	FIPattern<TTraits> secondPattern;//2つ目のパターン

};
//---------------------------------------------------------------------------
//省略可能を表すパターン
//（記述時には ! で表現する）
template <class TTraits>
class FIOptionalPatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIOptionalPatternImpl(const FIPattern<TTraits>& p)
		: pattern(p){}
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);

protected:
	//パターンを元に属性列を作り、attrsに追加する。
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//マッチする属性のキー。
	virtual TKey GetMatchingKey()const;
	virtual void SetMatchingKey(const TKey& k);

private:
	FIPattern<TTraits> pattern;//内包するパターン

};
//---------------------------------------------------------------------------
//任意の値を持つ複数の属性にマッチするパターン
template <class TTraits>
class FIAnyPatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIAnyPatternImpl(const TKey& k)
		: key(k){}
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);
	//マッチする属性のキー。
	virtual TKey GetMatchingKey()const{ return key; }
	virtual void SetMatchingKey(const TKey& k){ key= k; }

protected:
	//パターンを元に属性列を作り、attrsに追加する。
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);

private:
	TKey key;//マッチする属性のキー

};
//---------------------------------------------------------------------------
//任意の属性にマッチするパターン。
//ラベルが値列ではなく属性列にマッチする。
template <class TTraits>
class FIAnyAttributesPatternImpl: public FIPatternImpl<TTraits>
{
public:
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);
	//マッチする属性のキー。
	virtual TKey GetMatchingKey()const{ assert(false); return TKey(); }
	virtual void SetMatchingKey(const TKey& k){ assert(false); }

protected:
	//パターンを元に属性列を作り、attrsに追加する。
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);

};

//---------------------------------------------------------------------------
//任意の属性にマッチするパターン。
//ラベルが値列ではなく属性列にマッチする。
template <class TTraits>
class FIAASPatternImpl: public FIPatternImpl<TTraits>
{
	FIPattern<TTraits> end;
public:
	FIAASPatternImpl(const FIPattern<TTraits>& e):end(e){
	}
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);
	//マッチする属性のキー。
	virtual TKey GetMatchingKey()const{ assert(false); return TKey(); }
	virtual void SetMatchingKey(const TKey& k){ assert(false); }

protected:
	//パターンを元に属性列を作り、attrsに追加する。
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
};
//---------------------------------------------------------------------------
//木のパターンの定義を集めたもの
template <class TTraits>
struct FITreePatterns: public FITreeDefs<TTraits>
{
	//パターン
	typedef FIPattern<TTraits> TPattern;

	//任意ノード列
	static TPattern Any(){
		return TPattern(new FIAnyPatternImpl<TTraits>(TKey()));
	}
	//ノード
	static TPattern Node(const TPattern& childrenPattern){
		return TPattern(new FINodePatternImpl<TTraits>(TKey(), childrenPattern));
	}
	//任意の値
	static TPattern Value(){
		return TPattern(new FIAnyValuePatternImpl<TTraits>(TKey()));
	}
	//文字列
	static TPattern String(const TString& str){
		return TPattern(new FIStringPatternImpl<TTraits>(TKey(), str));
	}
	//属性化
	static TPattern Attribute(const TKey& key, const TPattern& pattern){
		pattern.GetImpl()->SetMatchingKey(key);
		return pattern;
	}
	//任意属性列
	static TPattern AnyAttributes(){
		return TPattern(new FIAnyAttributesPatternImpl<TTraits>());
	}
	static TPattern AAS(TPattern& p){
		return TPattern(new FIAASPatternImpl<TTraits>(p));
	}

};
//---------------------------------------------------------------------------
//片方向（TLTraits→TRTraits）の木変換器
template <class TLTraits, class TRTraits>
class FIHalfTreeConverter
{
	typedef FIHalfTreeConverter TThis;
	typedef FITreePatterns<TLTraits> TL;
	typedef FITreePatterns<TRTraits> TR;

public:
	//ルールを追加
	void Add(
		const typename TL::TPattern& lpattern, 
		const typename TR::TPattern& rpattern);
	//parentが持つ[first,last)の範囲の属性列を変換し、その結果をoutAttrsに追加する。
	void Convert(
		typename TR::TOutAttributes& outAttrs,
		const typename TL::TInNode& parent,
		typename TL::TInIterator first, 
		typename TL::TInIterator last);

private:
	typedef std::list<std::pair<typename TL::TPattern, typename TR::TPattern> >
		TRules;

	TRules rules;//ルール

	//値を変換。
	void ConvertValue(
		typename TR::TOutValue& outValue, 
		typename const TL::TInValue& inValue);

};
//*1 何故かTThis::を付けないと、VC7.1で関数の定義と宣言が一致しないと言われる。
//---------------------------------------------------------------------------
//キーと文字列の変換方法の定義。
//変換元と変換先で、キー(Traits::TKey)と文字列(Traits::TValue)の型に互換性が無い
//場合は、FIConvertationTraitsを特殊化して、適切なConvertKeyとConvertStringを
//定義する必要がある。
template <class TL, class TR>
struct FIConvertationTraits
{
	static typename TR::TKey ConvertKey(const typename TL::TKey& key){
		return key;
	}
	static typename TR::TString ConvertString(const typename TL::TString& str){
		return str;
	}
	
};
//---------------------------------------------------------------------------
//各テンプレートクラスのメンバ関数の定義をinclude。
#include "FITreeConverterImpl.h"
//---------------------------------------------------------------------------
#endif
