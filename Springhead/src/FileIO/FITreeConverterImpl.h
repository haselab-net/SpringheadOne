#ifndef FI_TREE_CONVERTER_IMPL_H
#define FI_TREE_CONVERTER_IMPL_H
//---------------------------------------------------------------------------
//FITreeConverter.hで定義されたテンプレートクラスのメンバ関数の定義とか。
//このファイルはFITreeConverter.hの末尾でincludeされています。
//---------------------------------------------------------------------------
template <class T>
void FIPatternImpl<T>::SaveIfLabeled(
	const TInNode& node, 
	TInIterator first, 
	TInIterator last, 
	TSourceLabelMaps* lmap,
	bool asAttrs/*= false*/)
{
	if (label!=0 && lmap){
		//パターンによって、属性列にマッチする場合と値列にマッチする場合がある。
		//ただ、「値のイテレータ」というのは無いので、どっちの場合も
		//記録するのは「属性のイテレータ」。
		if (asAttrs)
			lmap->attrs[label]= TLabeledInfo(node, first, last);
		else
			lmap->values[label]= TLabeledInfo(node, first, last);
	}
}

//---------------------------------------------------------------------------
template <class T>
void FIPatternImpl<T>::Generate(TOutAttributes& attrs, const TTargetLabelMaps* lmap)
{
	if (label!=0){
		//値のマップから検索。
		TTargetValueLabelMap::const_iterator vit;
		TTargetAttributeLabelMap::const_iterator ait;
		if ((vit= lmap->values.find(label))!=(lmap->values.end()))
			GenerateLabeled(attrs, vit->second);
		//属性のマップから検索。
		else if ((ait= lmap->attrs.find(label))!=(lmap->attrs.end()))
			Add(attrs, ait->second);
	}else{
		return GenerateSelf(attrs, lmap);
	}
}

//---------------------------------------------------------------------------
template <class T>
void FIPatternImpl<T>::GenerateLabeled(
	TOutAttributes& attrs, const TOutValues& values)
{
	for (std::size_t i= 0; i<values.size(); ++i){
		TOutAttribute attr;
		CreateAttribute(attr, GetMatchingKey(), values[i]);
    Add(attrs, attr);
	}
}

//---------------------------------------------------------------------------
template <class T>
inline FIPattern<T> FIPattern<T>::operator!()
{
	return FIPattern(new FIOptionalPatternImpl<T>(*this));
}

//---------------------------------------------------------------------------
//パターンをつなげる
template <class T>
inline FIPattern<T> operator>>(
  const FIPattern<T>& firstPattern, const FIPattern<T>& secondPattern){
		return FIPattern<T>(new FIConjunctionPatternImpl<T>(
			firstPattern, secondPattern));
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FINodePatternImpl<T>::Match(
		const TInNode& pnode, 
		TInIterator first, 
		TInIterator last, 
		TSourceLabelMaps* lmap)
{
	if (first==last) return make_pair(false, TInIterator());
	TInAttribute attr= GetAttribute(pnode, first);
	TInValue value= GetValue(attr);
	if (GetKey(attr)!=GetMatchingKey() || IsString(value))
		return make_pair(false, TInIterator());
	TInNode node= GetNode(value);
	std::pair<bool, TInIterator> result
		= childrenPattern.Match(node, GetBegin(node), GetEnd(node), lmap);
	if (result.first && result.second==GetEnd(node)){
		SaveIfLabeled(pnode, first, first+1, lmap);
		return make_pair(true, first+1);
	}else{
		return make_pair(false, TInIterator());
	}
}

//---------------------------------------------------------------------------
template <class T>
void FINodePatternImpl<T>::GenerateSelf(
	TOutAttributes& attrs, const TTargetLabelMaps* lmap)
{
	TOutAttributes cattrs;
	childrenPattern.Generate(cattrs, lmap);
	TOutNode node;
	CreateNode(node, cattrs);
	TOutValue value;
	CreateValue(value, node);
	TOutAttribute attr;
	CreateAttribute(attr, GetMatchingKey(), value);
	Add(attrs, attr);
}

//---------------------------------------------------------------------------
//マッチする属性のキー。
template <class T>
void FINodePatternImpl<T>::SetMatchingKey(const TKey& k)
{
	assert(key==TKey());
	key= k;
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIStringPatternImpl<T>::Match(
		const TInNode& pnode, 
		TInIterator first, 
		TInIterator last, 
		TSourceLabelMaps* lmap)
{
	if (first==last) return make_pair(false, TInIterator());
	TInAttribute attr= GetAttribute(pnode, first);
	TInValue value= GetValue(attr);
	if (GetKey(attr)==GetMatchingKey() && IsString(value) && GetString(value)==str){
		SaveIfLabeled(pnode, first, first+1, lmap);
		return std::make_pair(true, first+1);
	}else{
		return std::make_pair(false, TInIterator());
	}
}
//---------------------------------------------------------------------------
template <class T>
void FIStringPatternImpl<T>::GenerateSelf(
	TOutAttributes& attrs, const TTargetLabelMaps* lmap)
{
	TOutValue value;
	CreateValue(value, str);
	TOutAttribute attr;
	CreateAttribute(attr, GetMatchingKey(), value);
	Add(attrs, attr);
}

//---------------------------------------------------------------------------
//マッチする属性のキー。
template <class T>
void FIStringPatternImpl<T>::SetMatchingKey(const TKey& k)
{
	assert(key==TKey());
	key= k;
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIAnyValuePatternImpl<T>::Match(
		const TInNode& pnode, 
		TInIterator first, 
		TInIterator last, 
		TSourceLabelMaps* lmap)
{
	//キーが一致してればOK。
	if (first!=last && GetKey(GetAttribute(pnode, first))==GetMatchingKey()){
		SaveIfLabeled(pnode, first, first+1, lmap);
		return std::make_pair(true, first+1);
	}else{
		return std::make_pair(false, TInIterator());
	}
}

//---------------------------------------------------------------------------
template <class T>
void FIAnyValuePatternImpl<T>::GenerateSelf(
	TOutAttributes& attrs, const TTargetLabelMaps* lmap)
{
	assert(false);
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIConjunctionPatternImpl<T>::Match(const TInNode& pnode, TInIterator first, TInIterator last, 
	TSourceLabelMaps* lmap)
{
	pair<bool, TInIterator> result
		= firstPattern.Match(pnode, first, last, lmap);
	if (!result.first) return result;
	result= secondPattern.Match(pnode, result.second, last, lmap);
	if (result.first) SaveIfLabeled(pnode, first, result.second, lmap);
	return result;
}
//---------------------------------------------------------------------------
template <class T>
void FIConjunctionPatternImpl<T>::GenerateSelf(
		TOutAttributes& attrs, const TTargetLabelMaps* lmap)
{
	firstPattern.Generate(attrs, lmap);
	secondPattern.Generate(attrs, lmap);
}

//---------------------------------------------------------------------------
//マッチする属性のキー。
template <class T>
typename FIConjunctionPatternImpl<T>::TKey
	FIConjunctionPatternImpl<T>::GetMatchingKey()const
{
	//両辺のパターンのキーが一致してない場合は、
	//どうしようもないので空のキーを返している。
	TKey firstKey= firstPattern.GetImpl()->GetMatchingKey();
	TKey secondKey= secondPattern.GetImpl()->GetMatchingKey();
	return (firstKey==secondKey)? firstKey : TKey();
}

//---------------------------------------------------------------------------
//マッチする属性のキー。
template <class T>
void FIConjunctionPatternImpl<T>::SetMatchingKey(const TKey& k)
{
	firstPattern.GetImpl()->SetMatchingKey(k);
	secondPattern.GetImpl()->SetMatchingKey(k);
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIOptionalPatternImpl<T>::Match(
		const TInNode& pnode, 
		TInIterator first, 
		TInIterator last, 
		TSourceLabelMaps* lmap)
{
	std::pair<bool, TInIterator> result
		= pattern.Match(pnode, first, last, lmap);
	if (result.first) return result;
	else return std::make_pair(true, first);
}

//---------------------------------------------------------------------------
template <class T>
void FIOptionalPatternImpl<T>::GenerateSelf(
	TOutAttributes& attrs, const TTargetLabelMaps* lmap)
{
	pattern.Generate(attrs, lmap);
}

//---------------------------------------------------------------------------
//マッチする属性のキー。
template <class T>
typename FIOptionalPatternImpl<T>::TKey
	FIOptionalPatternImpl<T>::GetMatchingKey()const
{
	return pattern.GetImpl()->GetMatchingKey();
}

//---------------------------------------------------------------------------
//マッチする属性のキー。
template <class T>
void FIOptionalPatternImpl<T>::SetMatchingKey(const TKey& k)
{
	pattern.GetImpl()->SetMatchingKey(k);
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIAnyPatternImpl<T>::Match(
		const TInNode& pnode, 
		TInIterator first, 
		TInIterator last, 
		TSourceLabelMaps* lmap)
{
	TInIterator it;
	for (it= first; it!=last; ++it){
		if (GetKey(GetAttribute(pnode, it))!=key) break;
	}
	SaveIfLabeled(pnode, first, it, lmap);
	return std::make_pair(true, it);
}

//---------------------------------------------------------------------------
template <class T>
void FIAnyPatternImpl<T>::GenerateSelf(
	TOutAttributes& attrs, const TTargetLabelMaps* lmap)
{
	assert(false);
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIAnyAttributesPatternImpl<T>::Match(
		const TInNode& pnode, 
		TInIterator first, 
		TInIterator last, 
		TSourceLabelMaps* lmap)
{
	SaveIfLabeled(pnode, first, last, lmap, true);
		//属性列として記録。
	return std::make_pair(true, last);
}
//---------------------------------------------------------------------------
template <class T>
void FIAnyAttributesPatternImpl<T>::GenerateSelf(
	TOutAttributes& attrs, const TTargetLabelMaps* lmap)
{
	assert(false);
}

//---------------------------------------------------------------------------
template <class T>
std::pair<bool, typename T::TInIterator>
	FIAASPatternImpl<T>::Match(
		const TInNode& pnode, 
		TInIterator first, 
		TInIterator last, 
		TSourceLabelMaps* lmap)
{
	TInIterator it = first;
	for(;it != last; ++it){
		if (end.Match(pnode, it, last, lmap).first) break;
	}
	SaveIfLabeled(pnode, first, it, lmap, true);
		//属性列として記録。
	return std::make_pair(true, it);
}
//---------------------------------------------------------------------------
template <class T>
void FIAASPatternImpl<T>::GenerateSelf(
	TOutAttributes& attrs, const TTargetLabelMaps* lmap)
{
	assert(false);
}

//---------------------------------------------------------------------------
template <class T, class S>
void FIHalfTreeConverter<T, S>::Add(const typename TL::TPattern& lpattern, 
	const typename TR::TPattern& rpattern)
{
	rules.push_back(std::make_pair(lpattern, rpattern));
}

//---------------------------------------------------------------------------
//parentが持つ[first,last)の範囲の属性列を変換し、その結果をoutAttrsに追加する。
template <class T, class S>
void FIHalfTreeConverter<T, S>::Convert(
    typename TR::TOutAttributes& outAttrs,
		const typename TL::TInNode& parent,
		typename TL::TInIterator first, 
		typename TL::TInIterator last)
{
	typedef typename TL::TSourceLabelMap::const_iterator TSourceLabelIterator;
	typename TL::TInIterator ait= first;
	typename TL::TSourceLabelMaps lmap;
	std::pair<bool, typename TL::TInIterator> result;
	TRules::const_iterator rit;

	while (ait!=last){
		//残りの属性列にマッチするルールを探す。
		for (rit= rules.begin(); rit!=rules.end(); ++rit){
			result= rit->first.Match(parent, ait, last, &lmap);
			if (result.first) break;
		}
		if (rit!=rules.end()){//マッチするルールを発見。
			//ラベルの部分にマッチした値列を変換する。
			typename TR::TTargetLabelMaps tmap;
			for (TSourceLabelIterator lit= lmap.values.begin(); lit!=lmap.values.end(); ++lit){
				int label= lit->first;
				TL::TLabeledInfo linfo= lit->second;
				for (typename TL::TInIterator lait= linfo.first; lait!=linfo.last; ++lait){
					typename TL::TInValue svalue= TL::GetValue(TL::GetAttribute(linfo.node, lait));
					typename TR::TOutValue tvalue;
					ConvertValue(tvalue, svalue);
					tmap.values[label].push_back(tvalue);
				}
			}
			//ラベルの部分にマッチした属性列を変換する。
			for (TSourceLabelIterator lit= lmap.attrs.begin(); lit!=lmap.attrs.end(); ++lit){
				int label= lit->first;
				TL::TLabeledInfo linfo= lit->second;
				Convert(tmap.attrs[label], linfo.node, linfo.first, linfo.last);
			}
			//マッチした属性列に対応する属性列を生成。
			rit->second.Generate(outAttrs, &tmap);
			//マッチした範囲の終わりまでイテレータを進める。
			ait= result.second;
		}else{
			//マッチするルールが見つからず。
			//デフォルトの変換をする。
			typename TL::TInAttribute sattr= TL::GetAttribute(parent, ait);
			typename TL::TKey skey= TL::GetKey(sattr);
			typename TL::TInValue svalue= TL::GetValue(sattr);
			typename TR::TKey tkey= FIConvertationTraits<T, S>::ConvertKey(skey);
			typename TR::TOutValue tvalue;
			ConvertValue(tvalue, svalue);
			typename TR::TOutAttribute tattr;
			TR::CreateAttribute(tattr, tkey, tvalue);
			TR::Add(outAttrs, tattr);
			++ait;
		}
	}
}

//---------------------------------------------------------------------------
//値を変換。
template <class T, class S>
void FIHalfTreeConverter<T, S>::ConvertValue(
	typename TR::TOutValue& outValue, 
	typename const TL::TInValue& inValue)
{
	if (TL::IsString(inValue)){
		//文字列なら、FIConversionTraitsを使って変換。
		TR::CreateValue(outValue, 
			FIConvertationTraits<T, S>::ConvertString(TL::GetString(inValue)));
	}else{
		//ノードなら、そのノードが持つ属性列を再帰的に変換。
		typename TL::TInNode snode= TL::GetNode(inValue);
		typename TR::TOutAttributes tattrs;
		Convert(tattrs, snode, TL::GetBegin(snode), TL::GetEnd(snode));
		typename TR::TOutNode tnode;
		TR::CreateNode(tnode, tattrs);
		TR::CreateValue(outValue, tnode);
	}
}

//---------------------------------------------------------------------------
#endif
