#ifndef DOM_TRANSFORMER_H
#define DOM_TRANSFORMER_H
//---------------------------------------------------------------------------
#include <utility>
#include <map>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/parse_tree.hpp>
#include <boost/utility.hpp>
//---------------------------------------------------------------------------
#ifdef _MSC_VER
//	標準のassertだと，シンボル数が多すぎてコンパイルできないとVCが言うので
#undef assert
#ifdef _DEBUG
# define assert(x) assert_impl(x, #x);
inline void assert_impl(bool x, const char* str){
	if (!x){
		OutputDebugString((std::string(str)+"\n").c_str());
		DebugBreak();
	}
}
#else
# define assert(x)
#endif

#endif
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
	//ノード列をなめるためのイテレータ
	typedef typename TTraits::TInIterator TInIterator;
	//ノード
	typedef typename TTraits::TOutAttribute TOutAttribute;
	//ノードの値
	typedef typename TTraits::TString TString;
	//ノードのキー
	typedef typename TTraits::TKey TKey;
	
	//イテレータの対
	typedef std::pair<TInIterator, TInIterator> TIteratorPair;
	//ノード列
	typedef std::vector<TOutAttribute> TOutAttributes;
	//属性
	typedef std::map<TKey, TOutAttributes> TAttributes;
	//キーの配列
	typedef std::vector<TKey> TKeys;

	//ラベルを付けられたノード列の情報
	struct TLabeledInfo{
		TInIterator first;//ノード列の先頭
		TInIterator last;//ノード列の末尾の次
		TLabeledInfo(): first(TInIterator()), last(TInIterator()){}
		TLabeledInfo(TInIterator f, TInIterator l)
			: first(f), last(l){}
	};
	//ノード列マッチング中に、ラベルに対応するノード列を記録するためのマップ
	typedef std::map<int, TLabeledInfo> TSourceLabelMap;
	//ノード列生成時に、ラベルに対応するノード列を与えるためのマップ
	typedef std::map<int, TOutAttributes> TTargetLabelMap;

	//値ノードか
	static bool IsValue(const TInIterator& it){ return TTraits::IsValue(it); }
	//ノードの属性のキーを取得
	static TKeys GetAttributeKeys(const TInIterator& it){
		return TTraits::GetAttributeKeys(it);
	}
	//ノードの属性を取得
	static TIteratorPair GetAttribute(const TInIterator& it, const TKey& key){
		return TTraits::GetAttribute(it, key);
	}
	//ノードの値を取得
	static TString GetString(const TInIterator& it){
		return TTraits::GetString(it);
	}
	//指定の属性を持つノードを作る
	static TOutAttribute CreateNode(const TAttributes& attributes){
		return TTraits::CreateNode(attributes);
	}
	//指定の値のノードを作る
	static TOutAttribute CreateValue(const TString& value){
		return TTraits::CreateValue(value);
	}

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
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap)= 0;
	//パターンを元にノード列を作る
	//（自分にラベルが付いていたら、ラベルに対応するノード列を返す）
	TOutAttributes Generate(const TTargetLabelMap* lmap);
	//ラベルを付ける
	void SetLabel(int l){ label= l; }
	//ラベルが付いていたら、それをlmapに記録する
	void SaveIfLabeled(TInIterator first, TInIterator last, 
		TSourceLabelMap* lmap);

protected:
	//パターンを元にノード列を作る
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap)= 0;

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
	explicit FIPattern(FIPatternImpl<TTraits>* i= NULL): impl(i){}
	//ノード列がパターンにマッチするかを調べる
	std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap)const{
			if (impl) return impl->Match(first, last, lmap);
			else return std::make_pair(false, TInIterator());
		}
	//パターンを元にノード列を作る
	//（自分にラベルが付いていたら、ラベルに対応するノード列を返す）
	TOutAttributes Generate(const TTargetLabelMap* lmap)const{
		assert(impl);
		return impl->Generate(lmap);
	}
	//ラベルを付ける
	FIPattern& operator[](int l){
		if (impl) impl->SetLabel(l);
		return *this;
	}
	//自身をFIOptionalPatternImplでくるんで返す
	FIPattern operator!();

private:
	boost::shared_ptr<FIPatternImpl<TTraits> > impl;//実装

};
//---------------------------------------------------------------------------
//属性列を表すパターン
template <class TTraits>
struct FIAttributesPattern: public FITreeDefs<TTraits>
{
	typedef typename std::map<TKey, FIPattern<TTraits> > TMap;
	typedef typename TMap::const_iterator TInIterator;

	FIAttributesPattern(){}
	//単独の属性をあらわすパターン
	FIAttributesPattern(const TKey& name, 
		const FIPattern<TTraits>& value){
			patterns[name]= value;
	}
	//複数のパターンの結合
	friend FIAttributesPattern operator>>(const FIAttributesPattern& lhs,
		const FIAttributesPattern& rhs){
			FIAttributesPattern result= lhs;
			result.patterns.insert(rhs.patterns.begin(), rhs.patterns.end());
			return result;
	}

	TMap patterns;//属性名→属性値のパターン

};
//---------------------------------------------------------------------------
//単独の属性付きノードを表すパターン
template <class TTraits>
class FIAttributedNodePatternImpl: public FIPatternImpl<TTraits>
{
public:
	//属性はどうでもいい
	explicit FIAttributedNodePatternImpl()
		: ignoreAttributes(true){}
	//属性を指定する
	FIAttributedNodePatternImpl(const FIAttributesPattern<TTraits>& p)
		: ignoreAttributes(false), attributesPattern(p){}
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//パターンを元にノード列を作る
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap);

private:
	bool ignoreAttributes;//属性はどうでもいい
	FIAttributesPattern<TTraits> attributesPattern;//属性のパターン

};
//---------------------------------------------------------------------------
//単独の値ノードのパターン
template <class TTraits>
class FIValuePatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIValuePatternImpl(const TString& v)
		: value(v){}
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//パターンを元にノード列を作る
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap);

private:
	TString value;//値

};
//---------------------------------------------------------------------------
//任意の単独のノードを表すパターン
template <class TTraits>
class FIAnyNodePatternImpl: public FIPatternImpl<TTraits>
{
public:
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//パターンを元にノード列を作る
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap){
		assert(false); return TOutAttributes();
	}

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
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//パターンを元にノード列を作る
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap);

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
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//パターンを元にノード列を作る
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap);

private:
	FIPattern<TTraits> pattern;//内包するパターン

};
//---------------------------------------------------------------------------
//任意ノード列にマッチするパターン
template <class TTraits>
class FIAnyPatternImpl: public FIPatternImpl<TTraits>
{
public:
	//ノード列がパターンにマッチするかを調べる
	virtual std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//パターンを元にノード列を作る
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap);

};
//---------------------------------------------------------------------------
//木のパターンを作るのに便利な関数群
template <class TTraits>
struct FITreePatterns: public FITreeDefs<TTraits>
{
	//ノードパターン
	typedef FIPattern<TTraits> TPattern;
	//属性パターン
	typedef FIAttributesPattern<TTraits> TAttributesPattern;

	//任意ノード列
	static TPattern Any(){
		return TPattern(new FIAnyPatternImpl<TTraits>);
	}
	//任意の1ノード
	static TPattern AnyNode(){
		return TPattern(new FIAnyNodePatternImpl<TTraits>);
	}
	//単独の属性付きノード（属性列を指定しない）
	static TPattern AttributedNode(){
		return TPattern(new FIAttributedNodePatternImpl<TTraits>);
	}
	//単独の属性付きノード（属性列を指定）
	static TPattern AttributedNode(const TAttributesPattern& attributesPattern){
		return TPattern(new FIAttributedNodePatternImpl<TTraits>(attributesPattern));
	}
	//単独の値ノード
	static TPattern Value(const TString& value){
		return TPattern(new FIValuePatternImpl<TTraits>(value));
	}
	//属性
	static TAttributesPattern Attribute(const TKey& key, const TPattern& nodes){
		return TAttributesPattern(key, nodes);
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
	//ノード列変換器のインタフェース
	struct TNodesConverter{
		typedef typename TL::TSourceLabelMap TLabelMap;
		virtual typename TThis::TR::TOutAttributes 
			Convert(TThis* converter, TLabelMap& labelMap)= 0;
		virtual ~TNodesConverter(){}
	};

private:
	//デフォルトのノード列変換器
	struct TDefaultNodesConverter: TNodesConverter{
		explicit TDefaultNodesConverter(const typename TR::TPattern& pattern)
			: targetPattern_(pattern){}
		virtual typename TThis::TR::TOutAttributes 
			Convert(TThis* converter, TLabelMap& labelMap);
		typename TR::TPattern targetPattern_;
	};

public:
	//ルールを追加（簡易変換：変換先パターンを指定する）
	void Add(const typename TL::TPattern& lpattern, 
		const typename TR::TPattern& rpattern);
	//ルールを追加（手動変換：変換の手順(converter)を指定する）(*2)
	void Add(const typename TL::TPattern& lpattern, 
		TNodesConverter* converter);
	//ノード列を変換
	typename TThis::TR::TOutAttributes Convert(typename TL::TInIterator first, 
		typename TL::TInIterator last);//*1

private:
	//変換ルール
	struct TRule{
		typename TL::TPattern pattern;//このパターンにマッチしたら
		boost::shared_ptr<TNodesConverter> converter;//これを使って変換
	};
	typedef std::list<TRule> TRules;

	TRules rules;//ルール

};
//*1 何故かTThis::を付けないと、VC7.1で関数の定義と宣言が一致しないと言われる。
//*2 converterは、不要になるとFIHalfTreeConverterによって勝手にdeleteされる。
//---------------------------------------------------------------------------
//パターンをつなげる
template <class T>
inline FIPattern<T> operator>>(
  const FIPattern<T>& lhs, const FIPattern<T>& rhs){
		return FIPattern<T>(new FIConjunctionPatternImpl<T>(lhs, rhs));
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//テンプレート関数の定義
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
template <class T>
void FIPatternImpl<T>::SaveIfLabeled(TInIterator first, TInIterator last, 
																	TSourceLabelMap* lmap)
{
	if (label!=0 && lmap) (*lmap)[label]= TLabeledInfo(first, last);
}

//---------------------------------------------------------------------------
template <class T>
std::vector<typename FIPatternImpl<T>::TOutAttribute>
	FIPatternImpl<T>::Generate(const TTargetLabelMap* lmap)
{
	if (label!=0){
		TTargetLabelMap::const_iterator it= lmap->find(label);
		if (it!=(lmap->end())) return it->second;
		else return TOutAttributes();
	}else{
		return GenerateSelf(lmap);
	}
}

//---------------------------------------------------------------------------
template <class T>
inline FIPattern<T> FIPattern<T>::operator!()
{
	return FIPattern(new FIOptionalPatternImpl<T>(*this));
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIAttributedNodePatternImpl<T>::Match(TInIterator first, TInIterator last, 
	TSourceLabelMap* lmap)
{
	if (first==last || IsValue(first)) return make_pair(false, TInIterator());
	if (!ignoreAttributes){
		typedef FIAttributesPattern<T>::TInIterator TAttrIterator;
		for (TAttrIterator it= attributesPattern.patterns.begin();
			it!=attributesPattern.patterns.end(); ++it){
				//各属性をマッチ
				TIteratorPair value= GetAttribute(first, it->first);
				std::pair<bool, TInIterator> result
					= it->second.Match(value.first, value.second, lmap);
				if (!result.first || result.second!=value.second)
					return std::make_pair(false, TInIterator());
						//属性値全体にマッチしなければ失敗
		}
	}
	SaveIfLabeled(first, boost::next(first), lmap);
	return std::make_pair(true, boost::next(first));
}

//---------------------------------------------------------------------------
template <class T>
std::vector<typename FIAttributedNodePatternImpl<T>::TOutAttribute>
	FIAttributedNodePatternImpl<T>::GenerateSelf(const TTargetLabelMap* lmap)
{
	std::map<TKey, TOutAttributes> attributes;
	typedef FIAttributesPattern<T>::TInIterator TAttrIterator;
	for (TAttrIterator it= attributesPattern.patterns.begin();
		it!=attributesPattern.patterns.end(); ++it){
			attributes[it->first]= it->second.Generate(lmap);
	}
	return TOutAttributes(1, CreateNode(attributes));
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIValuePatternImpl<T>::Match(TInIterator first, TInIterator last, 
	TSourceLabelMap* lmap)
{
	if (first!=last && IsValue(first) && GetString(first)==value){
		SaveIfLabeled(first, boost::next(first), lmap);
		return std::make_pair(true, boost::next(first));
	}else{
		return std::make_pair(false, TInIterator());
	}
}

//---------------------------------------------------------------------------
template <class T>
std::vector<typename FIValuePatternImpl<T>::TOutAttribute>
	FIValuePatternImpl<T>::GenerateSelf(const TTargetLabelMap* lmap)
{
	return TOutAttributes(1, CreateValue(value));
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIAnyNodePatternImpl<T>::Match(TInIterator first, TInIterator last, 
	TSourceLabelMap* lmap)
{
	if (first!=last){
		SaveIfLabeled(first, boost::next(first), lmap);
		return std::make_pair(true, boost::next(first));
	}else{
		return std::make_pair(false, TInIterator());
	}
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIConjunctionPatternImpl<T>::Match(TInIterator first, TInIterator last, 
	TSourceLabelMap* lmap)
{
	pair<bool, TInIterator> result
		= firstPattern.Match(first, last, lmap);
	if (!result.first) return result;
	result= secondPattern.Match(result.second, last, lmap);
	if (result.first) SaveIfLabeled(first, result.second, lmap);
	return result;
}
//---------------------------------------------------------------------------
template <class T>
std::vector<typename FIConjunctionPatternImpl<T>::TOutAttribute>
	FIConjunctionPatternImpl<T>::GenerateSelf(const TTargetLabelMap* lmap)
{
	TOutAttributes nodes= firstPattern.Generate(lmap);
	TOutAttributes nodes2= secondPattern.Generate(lmap);
	nodes.insert(nodes.end(), nodes2.begin(), nodes2.end());
	return nodes;
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIOptionalPatternImpl<T>::Match(TInIterator first, TInIterator last, 
	TSourceLabelMap* lmap)
{
	std::pair<bool, TInIterator> result
		= pattern.Match(first, last, lmap);
	if (result.first) return result;
	else return std::make_pair(true, first);
}

//---------------------------------------------------------------------------
template <class T>
std::vector<typename FIOptionalPatternImpl<T>::TOutAttribute>
	FIOptionalPatternImpl<T>::GenerateSelf(const TTargetLabelMap* lmap)
{
	return pattern.Generate(lmap);
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIAnyPatternImpl<T>::Match(TInIterator first, TInIterator last, 
	TSourceLabelMap* lmap)
{
	SaveIfLabeled(first, last, lmap);
	return std::make_pair(true, last);
}

//---------------------------------------------------------------------------
template <class T>
std::vector<typename FIAnyPatternImpl<T>::TOutAttribute>
	FIAnyPatternImpl<T>::GenerateSelf(const TTargetLabelMap* lmap)
{
	assert(false);
	return TOutAttributes();
}

//---------------------------------------------------------------------------
template <class T, class S>
typename FIHalfTreeConverter<T, S>::TR::TOutAttributes
FIHalfTreeConverter<T, S>::TDefaultNodesConverter::Convert(
	FIHalfTreeConverter* converter, 
	typename TL::TSourceLabelMap& labelMap)
{
	typename TR::TTargetLabelMap tmap;
	//ラベル付きノード列を変換
	for (typename TL::TSourceLabelMap::const_iterator lit= labelMap.begin();
		lit!=labelMap.end(); ++lit){
			tmap[lit->first]
				= converter->Convert(lit->second.first, lit->second.last);
	}
	//対応するノード列を生成
	return targetPattern_.Generate(&tmap);
}

//---------------------------------------------------------------------------
template <class T, class S>
void FIHalfTreeConverter<T, S>::Add(const typename TL::TPattern& lpattern, 
	const typename TR::TPattern& rpattern)
{
	TRule rule;
	rule.pattern= lpattern;
	rule.converter.reset(new TDefaultNodesConverter(rpattern));
	rules.push_back(rule);
}

//---------------------------------------------------------------------------
//ルールを追加（手動変換：変換の手順(converter)を指定する）(*2)
template <class T, class S>
void FIHalfTreeConverter<T, S>::Add(const typename TL::TPattern& lpattern, 
	TNodesConverter* converter)
{
	TRule rule;
	rule.pattern= lpattern;
	rule.converter.reset(converter);
	rules.push_back(rule);
}

//---------------------------------------------------------------------------
template <class T, class S>
typename FIHalfTreeConverter<T, S>::TR::TOutAttributes
	FIHalfTreeConverter<T, S>::Convert(typename TL::TInIterator first, 
		typename TL::TInIterator last)
{
	typename TL::TInIterator nit= first, prevNit;
	typename TR::TOutAttributes outNodes;
	while (nit!=last){
		prevNit= nit;
		for (TRules::const_iterator rit= rules.begin(); 
			rit!=rules.end(); ++rit){
				typename TL::TSourceLabelMap lmap;
				//各パターンにマッチするかどうかを調べる
				std::pair<bool, typename TL::TInIterator> result
					= rit->pattern.Match(nit, last, &lmap);
				if (result.first){
					//パターンに対応するノード変換器(TNodesConverter)を使って変換
					typename TR::TOutAttributes nodes= rit->converter->Convert(this, lmap);
					outNodes.insert(outNodes.end(), nodes.begin(), nodes.end());
					nit= result.second;
					break;
				}
		}
		if (nit==prevNit){//どれにもヒットしなかった
			if (TL::IsValue(nit)){
				//値ノードは、同じ値を持つ値ノードに変換
				typename TR::TString value= 
					FIConvertationTraits<T, S>::ConvertValue(TL::GetString(nit));
				outNodes.push_back(TR::CreateValue(value));
			}else{
				//属性持ちノードは、各属性を変換
				std::map<typename TR::TKey, typename TR::TOutAttributes> attributes;
				typename TL::TKeys keys= TL::GetAttributeKeys(nit);
				for (typename TL::TKeys::iterator kit= keys.begin(); 
					kit!=keys.end(); ++kit){
						typename TL::TIteratorPair value= TL::GetAttribute(nit, *kit);
						typename TR::TKey name
							= FIConvertationTraits<T, S>::ConvertKey(*kit);
						attributes[name]= Convert(value.first, value.second);
				}
				outNodes.push_back(TR::CreateNode(attributes));
			}
			++nit;
		}
	}
	return outNodes;
}

//---------------------------------------------------------------------------
#endif
