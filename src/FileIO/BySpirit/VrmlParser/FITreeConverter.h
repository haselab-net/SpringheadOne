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
//	�W����assert���ƁC�V���{�������������ăR���p�C���ł��Ȃ���VC�������̂�
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
//�؂̕ϊ��ŕK�v�ȁA�^�̑g�ݍ��킹���ƂɓƎ��̒�`���K�v�ȑ���̏W��
//�i���ꉻ���Ďg���j
template <class TL, class TR>
struct FIConvertationTraits;
//---------------------------------------------------------------------------
//�ؕϊ��n�̃N���X�ŋ��ʂɎg����`�W�i�p�����Ďg���j
template <class TTraits>
class FITreeDefs
{
public:
	//�m�[�h����Ȃ߂邽�߂̃C�e���[�^
	typedef typename TTraits::TInIterator TInIterator;
	//�m�[�h
	typedef typename TTraits::TOutAttribute TOutAttribute;
	//�m�[�h�̒l
	typedef typename TTraits::TString TString;
	//�m�[�h�̃L�[
	typedef typename TTraits::TKey TKey;
	
	//�C�e���[�^�̑�
	typedef std::pair<TInIterator, TInIterator> TIteratorPair;
	//�m�[�h��
	typedef std::vector<TOutAttribute> TOutAttributes;
	//����
	typedef std::map<TKey, TOutAttributes> TAttributes;
	//�L�[�̔z��
	typedef std::vector<TKey> TKeys;

	//���x����t����ꂽ�m�[�h��̏��
	struct TLabeledInfo{
		TInIterator first;//�m�[�h��̐擪
		TInIterator last;//�m�[�h��̖����̎�
		TLabeledInfo(): first(TInIterator()), last(TInIterator()){}
		TLabeledInfo(TInIterator f, TInIterator l)
			: first(f), last(l){}
	};
	//�m�[�h��}�b�`���O���ɁA���x���ɑΉ�����m�[�h����L�^���邽�߂̃}�b�v
	typedef std::map<int, TLabeledInfo> TSourceLabelMap;
	//�m�[�h�񐶐����ɁA���x���ɑΉ�����m�[�h���^���邽�߂̃}�b�v
	typedef std::map<int, TOutAttributes> TTargetLabelMap;

	//�l�m�[�h��
	static bool IsValue(const TInIterator& it){ return TTraits::IsValue(it); }
	//�m�[�h�̑����̃L�[���擾
	static TKeys GetAttributeKeys(const TInIterator& it){
		return TTraits::GetAttributeKeys(it);
	}
	//�m�[�h�̑������擾
	static TIteratorPair GetAttribute(const TInIterator& it, const TKey& key){
		return TTraits::GetAttribute(it, key);
	}
	//�m�[�h�̒l���擾
	static TString GetString(const TInIterator& it){
		return TTraits::GetString(it);
	}
	//�w��̑��������m�[�h�����
	static TOutAttribute CreateNode(const TAttributes& attributes){
		return TTraits::CreateNode(attributes);
	}
	//�w��̒l�̃m�[�h�����
	static TOutAttribute CreateValue(const TString& value){
		return TTraits::CreateValue(value);
	}

};
//---------------------------------------------------------------------------
//�؂̃p�^�[���̎����̊��N���X
template <class TTraits>
class FIPatternImpl: public FITreeDefs<TTraits>
{
public:
	FIPatternImpl(): label(0){}
	virtual ~FIPatternImpl(){}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap)= 0;
	//�p�^�[�������Ƀm�[�h������
	//�i�����Ƀ��x�����t���Ă�����A���x���ɑΉ�����m�[�h���Ԃ��j
	TOutAttributes Generate(const TTargetLabelMap* lmap);
	//���x����t����
	void SetLabel(int l){ label= l; }
	//���x�����t���Ă�����A�����lmap�ɋL�^����
	void SaveIfLabeled(TInIterator first, TInIterator last, 
		TSourceLabelMap* lmap);

protected:
	//�p�^�[�������Ƀm�[�h������
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap)= 0;

private:
	int label;//���x��

};
//---------------------------------------------------------------------------
//�؂̃p�^�[��
//�i�l�Ƃ��Ĉ�����悤��FIPatternImpl�����b�v���Ă���j
template <class TTraits>
class FIPattern: public FITreeDefs<TTraits>
{
public:
	explicit FIPattern(FIPatternImpl<TTraits>* i= NULL): impl(i){}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap)const{
			if (impl) return impl->Match(first, last, lmap);
			else return std::make_pair(false, TInIterator());
		}
	//�p�^�[�������Ƀm�[�h������
	//�i�����Ƀ��x�����t���Ă�����A���x���ɑΉ�����m�[�h���Ԃ��j
	TOutAttributes Generate(const TTargetLabelMap* lmap)const{
		assert(impl);
		return impl->Generate(lmap);
	}
	//���x����t����
	FIPattern& operator[](int l){
		if (impl) impl->SetLabel(l);
		return *this;
	}
	//���g��FIOptionalPatternImpl�ł����ŕԂ�
	FIPattern operator!();

private:
	boost::shared_ptr<FIPatternImpl<TTraits> > impl;//����

};
//---------------------------------------------------------------------------
//�������\���p�^�[��
template <class TTraits>
struct FIAttributesPattern: public FITreeDefs<TTraits>
{
	typedef typename std::map<TKey, FIPattern<TTraits> > TMap;
	typedef typename TMap::const_iterator TInIterator;

	FIAttributesPattern(){}
	//�P�Ƃ̑���������킷�p�^�[��
	FIAttributesPattern(const TKey& name, 
		const FIPattern<TTraits>& value){
			patterns[name]= value;
	}
	//�����̃p�^�[���̌���
	friend FIAttributesPattern operator>>(const FIAttributesPattern& lhs,
		const FIAttributesPattern& rhs){
			FIAttributesPattern result= lhs;
			result.patterns.insert(rhs.patterns.begin(), rhs.patterns.end());
			return result;
	}

	TMap patterns;//�������������l�̃p�^�[��

};
//---------------------------------------------------------------------------
//�P�Ƃ̑����t���m�[�h��\���p�^�[��
template <class TTraits>
class FIAttributedNodePatternImpl: public FIPatternImpl<TTraits>
{
public:
	//�����͂ǂ��ł�����
	explicit FIAttributedNodePatternImpl()
		: ignoreAttributes(true){}
	//�������w�肷��
	FIAttributedNodePatternImpl(const FIAttributesPattern<TTraits>& p)
		: ignoreAttributes(false), attributesPattern(p){}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//�p�^�[�������Ƀm�[�h������
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap);

private:
	bool ignoreAttributes;//�����͂ǂ��ł�����
	FIAttributesPattern<TTraits> attributesPattern;//�����̃p�^�[��

};
//---------------------------------------------------------------------------
//�P�Ƃ̒l�m�[�h�̃p�^�[��
template <class TTraits>
class FIValuePatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIValuePatternImpl(const TString& v)
		: value(v){}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//�p�^�[�������Ƀm�[�h������
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap);

private:
	TString value;//�l

};
//---------------------------------------------------------------------------
//�C�ӂ̒P�Ƃ̃m�[�h��\���p�^�[��
template <class TTraits>
class FIAnyNodePatternImpl: public FIPatternImpl<TTraits>
{
public:
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//�p�^�[�������Ƀm�[�h������
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap){
		assert(false); return TOutAttributes();
	}

};
//---------------------------------------------------------------------------
//�p�^�[���̂Ȃ���
//�i�L�q���ɂ� >> �ŕ\������j
template <class TTraits>
class FIConjunctionPatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIConjunctionPatternImpl(const FIPattern<TTraits>& f, const FIPattern<TTraits>& s)
		: firstPattern(f), secondPattern(s){}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//�p�^�[�������Ƀm�[�h������
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap);

private:
	FIPattern<TTraits> firstPattern;//1�ڂ̃p�^�[��
	FIPattern<TTraits> secondPattern;//2�ڂ̃p�^�[��

};
//---------------------------------------------------------------------------
//�ȗ��\��\���p�^�[��
//�i�L�q���ɂ� ! �ŕ\������j
template <class TTraits>
class FIOptionalPatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIOptionalPatternImpl(const FIPattern<TTraits>& p)
		: pattern(p){}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//�p�^�[�������Ƀm�[�h������
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap);

private:
	FIPattern<TTraits> pattern;//�����p�^�[��

};
//---------------------------------------------------------------------------
//�C�Ӄm�[�h��Ƀ}�b�`����p�^�[��
template <class TTraits>
class FIAnyPatternImpl: public FIPatternImpl<TTraits>
{
public:
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(TInIterator first, TInIterator last, TSourceLabelMap* lmap);

protected:
	//�p�^�[�������Ƀm�[�h������
	virtual TOutAttributes GenerateSelf(const TTargetLabelMap* lmap);

};
//---------------------------------------------------------------------------
//�؂̃p�^�[�������̂ɕ֗��Ȋ֐��Q
template <class TTraits>
struct FITreePatterns: public FITreeDefs<TTraits>
{
	//�m�[�h�p�^�[��
	typedef FIPattern<TTraits> TPattern;
	//�����p�^�[��
	typedef FIAttributesPattern<TTraits> TAttributesPattern;

	//�C�Ӄm�[�h��
	static TPattern Any(){
		return TPattern(new FIAnyPatternImpl<TTraits>);
	}
	//�C�ӂ�1�m�[�h
	static TPattern AnyNode(){
		return TPattern(new FIAnyNodePatternImpl<TTraits>);
	}
	//�P�Ƃ̑����t���m�[�h�i��������w�肵�Ȃ��j
	static TPattern AttributedNode(){
		return TPattern(new FIAttributedNodePatternImpl<TTraits>);
	}
	//�P�Ƃ̑����t���m�[�h�i��������w��j
	static TPattern AttributedNode(const TAttributesPattern& attributesPattern){
		return TPattern(new FIAttributedNodePatternImpl<TTraits>(attributesPattern));
	}
	//�P�Ƃ̒l�m�[�h
	static TPattern Value(const TString& value){
		return TPattern(new FIValuePatternImpl<TTraits>(value));
	}
	//����
	static TAttributesPattern Attribute(const TKey& key, const TPattern& nodes){
		return TAttributesPattern(key, nodes);
	}

};
//---------------------------------------------------------------------------
//�Е����iTLTraits��TRTraits�j�̖ؕϊ���
template <class TLTraits, class TRTraits>
class FIHalfTreeConverter
{
	typedef FIHalfTreeConverter TThis;
	typedef FITreePatterns<TLTraits> TL;
	typedef FITreePatterns<TRTraits> TR;

public:
	//�m�[�h��ϊ���̃C���^�t�F�[�X
	struct TNodesConverter{
		typedef typename TL::TSourceLabelMap TLabelMap;
		virtual typename TThis::TR::TOutAttributes 
			Convert(TThis* converter, TLabelMap& labelMap)= 0;
		virtual ~TNodesConverter(){}
	};

private:
	//�f�t�H���g�̃m�[�h��ϊ���
	struct TDefaultNodesConverter: TNodesConverter{
		explicit TDefaultNodesConverter(const typename TR::TPattern& pattern)
			: targetPattern_(pattern){}
		virtual typename TThis::TR::TOutAttributes 
			Convert(TThis* converter, TLabelMap& labelMap);
		typename TR::TPattern targetPattern_;
	};

public:
	//���[����ǉ��i�ȈՕϊ��F�ϊ���p�^�[�����w�肷��j
	void Add(const typename TL::TPattern& lpattern, 
		const typename TR::TPattern& rpattern);
	//���[����ǉ��i�蓮�ϊ��F�ϊ��̎菇(converter)���w�肷��j(*2)
	void Add(const typename TL::TPattern& lpattern, 
		TNodesConverter* converter);
	//�m�[�h���ϊ�
	typename TThis::TR::TOutAttributes Convert(typename TL::TInIterator first, 
		typename TL::TInIterator last);//*1

private:
	//�ϊ����[��
	struct TRule{
		typename TL::TPattern pattern;//���̃p�^�[���Ƀ}�b�`������
		boost::shared_ptr<TNodesConverter> converter;//������g���ĕϊ�
	};
	typedef std::list<TRule> TRules;

	TRules rules;//���[��

};
//*1 ���̂�TThis::��t���Ȃ��ƁAVC7.1�Ŋ֐��̒�`�Ɛ錾����v���Ȃ��ƌ�����B
//*2 converter�́A�s�v�ɂȂ��FIHalfTreeConverter�ɂ���ď����delete�����B
//---------------------------------------------------------------------------
//�p�^�[�����Ȃ���
template <class T>
inline FIPattern<T> operator>>(
  const FIPattern<T>& lhs, const FIPattern<T>& rhs){
		return FIPattern<T>(new FIConjunctionPatternImpl<T>(lhs, rhs));
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//�e���v���[�g�֐��̒�`
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
				//�e�������}�b�`
				TIteratorPair value= GetAttribute(first, it->first);
				std::pair<bool, TInIterator> result
					= it->second.Match(value.first, value.second, lmap);
				if (!result.first || result.second!=value.second)
					return std::make_pair(false, TInIterator());
						//�����l�S�̂Ƀ}�b�`���Ȃ���Ύ��s
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
	//���x���t���m�[�h���ϊ�
	for (typename TL::TSourceLabelMap::const_iterator lit= labelMap.begin();
		lit!=labelMap.end(); ++lit){
			tmap[lit->first]
				= converter->Convert(lit->second.first, lit->second.last);
	}
	//�Ή�����m�[�h��𐶐�
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
//���[����ǉ��i�蓮�ϊ��F�ϊ��̎菇(converter)���w�肷��j(*2)
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
				//�e�p�^�[���Ƀ}�b�`���邩�ǂ����𒲂ׂ�
				std::pair<bool, typename TL::TInIterator> result
					= rit->pattern.Match(nit, last, &lmap);
				if (result.first){
					//�p�^�[���ɑΉ�����m�[�h�ϊ���(TNodesConverter)���g���ĕϊ�
					typename TR::TOutAttributes nodes= rit->converter->Convert(this, lmap);
					outNodes.insert(outNodes.end(), nodes.begin(), nodes.end());
					nit= result.second;
					break;
				}
		}
		if (nit==prevNit){//�ǂ�ɂ��q�b�g���Ȃ�����
			if (TL::IsValue(nit)){
				//�l�m�[�h�́A�����l�����l�m�[�h�ɕϊ�
				typename TR::TString value= 
					FIConvertationTraits<T, S>::ConvertValue(TL::GetString(nit));
				outNodes.push_back(TR::CreateValue(value));
			}else{
				//���������m�[�h�́A�e������ϊ�
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
