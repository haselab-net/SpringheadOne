#ifndef FI_TREE_CONVERTER_H
#define FI_TREE_CONVERTER_H
//---------------------------------------------------------------------------
//�؍\������؍\���ւ̕ϊ���(FIHalfTreeConverter)�ƁA
//���̕ϊ����[�����L�q���邽�߂̊e��p�^�[��(FIPattern)�B
//---------------------------------------------------------------------------
#include <utility>
#include <map>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/parse_tree.hpp>
//---------------------------------------------------------------------------
//���x���t���m�[�h��̎��
enum FILabeledType{
	filtNodes,//�X�Ȃ�}�b�`���O�̑ΏۂɂȂ�m�[�h��
	filtValue//����ȏ�}�b�`���O�����A�l�Ƃ��Ĉ���
};

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
	//�����̃L�[�̌^�B
	typedef typename TTraits::TKey TKey;
	//������̌^�B
	typedef typename TTraits::TString TString;

  //�m�[�h�̌^�i���͑��j�B
	typedef typename TTraits::TInNode TInNode;
  //�l�̌^�i���͑��j�B
	typedef typename TTraits::TInValue TInValue;
  //�����̌^�i���͑��j�B
	typedef typename TTraits::TInAttribute TInAttribute;
	//�m�[�h�̊e�������r�߂�C�e���[�^�B
	typedef typename TTraits::TInIterator TInIterator;

	//�����̃L�[�B
	static TKey GetKey(const TInAttribute& attr){
		return TTraits::GetKey(attr);
	}
  //�����̒l�B
	static TInValue GetValue(const TInAttribute& attr){
		return TTraits::GetValue(attr);
	}
  //�l��������Ȃ�true�A�m�[�h�Ȃ�false�B
	static bool IsString(const TInValue& value){
		return TTraits::IsString(value);
	}
	//�l�̕�����B
	static TString GetString(const TInValue& value){
		return TTraits::GetString(value);
	}
  //�l�̃m�[�h�B
	static TInNode GetNode(const TInValue& value){
		return TTraits::GetNode(value);
	}
	//�q�m�[�h�̐擪���擾
	static TInIterator GetBegin(const TInNode& node){
		return TTraits::GetBegin(node);
	}
	//�q�m�[�h�̖����̎����擾
	static TInIterator GetEnd(const TInNode& node){
		return TTraits::GetEnd(node);
	}
  //i���w�������B
	static TInAttribute GetAttribute(const TInNode& node, TInIterator i){
		return TTraits::GetAttribute(node, i);
	}

  //�m�[�h�̌^�i�o�͑��j�B
	typedef typename TTraits::TOutNode TOutNode;
  //�l�̌^�i�o�͑��j�B
  typedef typename TTraits::TOutValue TOutValue;
	//�����̌^�i�o�͑��j�B
	typedef typename TTraits::TOutAttribute TOutAttribute;
	//������̌^�i�o�͑��j�B
	typedef typename TTraits::TOutAttributes TOutAttributes;

  //�m�[�h�����B
	static void CreateNode(TOutNode& node, const TOutAttributes& attrs){
		TTraits::CreateNode(node, attrs);
	}
  //����������l�����B
  static void CreateValue(TOutValue& value, const TString& s){
		TTraits::CreateValue(value, s);
	}
  //�m�[�h�����l�����B
  static void CreateValue(TOutValue& value, const TOutNode& node){
		TTraits::CreateValue(value, node);
	}
  //���������B
  static void CreateAttribute(TOutAttribute& attr, const TKey& key, const TOutValue& value){
		TTraits::CreateAttribute(attr, key, value);
	}
  //������̖����ɑ�����ǉ�����B
  static void Add(TOutAttributes& attrs, const TOutAttribute& attr){
		TTraits::Add(attrs, attr);
	}
  //������̖����ɑ������ǉ�����B
  static void Add(TOutAttributes& attrs, const TOutAttributes& added){
		TTraits::Add(attrs, added);
	}

	//���x����t����ꂽ�m�[�h��̏��
	struct TLabeledInfo{
		TInNode node;//�m�[�h
		TInIterator first;//�m�[�h��̐擪
		TInIterator last;//�m�[�h��̖����̎�
		TLabeledInfo(){}
		TLabeledInfo(const TInNode& n, TInIterator f, TInIterator l)
			: node(n), first(f), last(l){}
	};
	//�l�̔z��i�o�͑��j�B
	typedef std::vector<TOutValue> TOutValues;

	//�m�[�h��}�b�`���O���ɁA���x���ɑΉ�����l����L�^���邽�߂̃}�b�v�B
	typedef std::map<int, TLabeledInfo> TSourceLabelMap;
	struct TSourceLabelMaps{
		TSourceLabelMap values;
		TSourceLabelMap attrs;
	};
	//�m�[�h�񐶐����ɁA���x���ɑΉ�����l���^���邽�߂̃}�b�v�B
	typedef std::map<int, TOutValues> TTargetValueLabelMap;
	typedef std::map<int, TOutAttributes> TTargetAttributeLabelMap;
	struct TTargetLabelMaps{
		TTargetValueLabelMap values;
		TTargetAttributeLabelMap attrs;
	};

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
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap)= 0;
	//�p�^�[���𐗌^�Ƃ��đ���������Aattrs�ɒǉ�����B
	//�����Ƀ��x�����t���Ă�����A���x���ɑΉ����鑮�����Ԃ��B
	void Generate(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//���x����t����
	void SetLabel(int l){ label= l; }
	//���x�����t���Ă�����A�����lmap�ɋL�^����
	void SaveIfLabeled(
		const TInNode& node, 
		TInIterator first, 
		TInIterator last, 
		TSourceLabelMaps* lmap,
		bool asAttrs= false);
	//�}�b�`���鑮���̃L�[�B
	virtual TKey GetMatchingKey()const= 0;
	virtual void SetMatchingKey(const TKey& k)= 0;

protected:
	//�p�^�[�������ɑ���������Aattrs�ɒǉ�����B
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap)= 0;
	//���x���ɑΉ����鑮��������Aattrs�ɒǉ�����B
	virtual void GenerateLabeled(
		TOutAttributes& attrs, const TOutValues& values);

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
	explicit FIPattern(FIPatternImpl<TTraits>* i): impl(i){}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	std::pair<bool, TInIterator> Match(
		const TInNode& pnode, 
		TInIterator first, 
		TInIterator last, 
		TSourceLabelMaps* lmap)const{
			return impl->Match(pnode, first, last, lmap);
	}
	//�p�^�[���𐗌^�Ƃ��đ���������Aattrs�ɒǉ�����B
	//�����Ƀ��x�����t���Ă�����A���x���ɑΉ����鑮�����Ԃ��B
	void Generate(TOutAttributes& attrs, const TTargetLabelMaps* lmap)const{
		impl->Generate(attrs, lmap);
	}
	//���x����t����
	FIPattern& operator[](int l){
		impl->SetLabel(l);
		return *this;
	}
	//���g��FIOptionalPatternImpl�ł����ŕԂ�
	FIPattern operator!();
	//����
	boost::shared_ptr<FIPatternImpl<TTraits> > GetImpl()const{ return impl; }

private:
	boost::shared_ptr<FIPatternImpl<TTraits> > impl;//����

};
//---------------------------------------------------------------------------
//�p�^�[�����Ȃ���
template <class T>
inline FIPattern<T> operator>>(
  const FIPattern<T>& firstPattern, const FIPattern<T>& secondPattern);
//---------------------------------------------------------------------------
//�P�Ƃ̃m�[�h����������\���p�^�[��
template <class TTraits>
class FINodePatternImpl: public FIPatternImpl<TTraits>
{
public:
	//���O�Ǝq�m�[�h��̃p�^�[�����w�肷��
	FINodePatternImpl(const TKey& k, const FIPattern<TTraits>& p)
		: key(k), childrenPattern(p){}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);

protected:
	//�p�^�[�������ɑ���������Aattrs�ɒǉ�����B
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//�}�b�`���鑮���̃L�[�B
	virtual TKey GetMatchingKey()const{ return key; }
	virtual void SetMatchingKey(const TKey& k);

private:
	TKey key;//�}�b�`���鑮���̃L�[
	FIPattern<TTraits> childrenPattern;//�q�m�[�h��̃p�^�[��

};
//---------------------------------------------------------------------------
//�P�Ƃ̕��������������\���p�^�[��
template <class TTraits>
class FIStringPatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIStringPatternImpl(const TKey& k, const TString& v)
		: key(k), str(v){}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);

protected:
	//�p�^�[�������ɑ���������Aattrs�ɒǉ�����B
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//�}�b�`���鑮���̃L�[�B
	virtual TKey GetMatchingKey()const{ return key; }
	virtual void SetMatchingKey(const TKey& k);

private:
	TKey key;//�}�b�`���鑮���̃L�[
	TString str;//������

};
//---------------------------------------------------------------------------
//�C�ӂ̒l����������\���p�^�[��
template <class TTraits>
class FIAnyValuePatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIAnyValuePatternImpl(const TKey& k)
		: key(k){}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);

protected:
	//�p�^�[�������ɑ���������Aattrs�ɒǉ�����B
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//�}�b�`���鑮���̃L�[�B
	virtual TKey GetMatchingKey()const{ return key; }
	virtual void SetMatchingKey(const TKey& k){ key= k; }

private:
	TKey key;//�}�b�`���鑮���̃L�[

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
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);

protected:
	//�p�^�[�������ɑ���������Aattrs�ɒǉ�����B
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//�}�b�`���鑮���̃L�[�B
	virtual TKey GetMatchingKey()const;
	virtual void SetMatchingKey(const TKey& k);

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
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);

protected:
	//�p�^�[�������ɑ���������Aattrs�ɒǉ�����B
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
	//�}�b�`���鑮���̃L�[�B
	virtual TKey GetMatchingKey()const;
	virtual void SetMatchingKey(const TKey& k);

private:
	FIPattern<TTraits> pattern;//�����p�^�[��

};
//---------------------------------------------------------------------------
//�C�ӂ̒l���������̑����Ƀ}�b�`����p�^�[��
template <class TTraits>
class FIAnyPatternImpl: public FIPatternImpl<TTraits>
{
public:
	FIAnyPatternImpl(const TKey& k)
		: key(k){}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);
	//�}�b�`���鑮���̃L�[�B
	virtual TKey GetMatchingKey()const{ return key; }
	virtual void SetMatchingKey(const TKey& k){ key= k; }

protected:
	//�p�^�[�������ɑ���������Aattrs�ɒǉ�����B
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);

private:
	TKey key;//�}�b�`���鑮���̃L�[

};
//---------------------------------------------------------------------------
//�C�ӂ̑����Ƀ}�b�`����p�^�[���B
//���x�����l��ł͂Ȃ�������Ƀ}�b�`����B
template <class TTraits>
class FIAnyAttributesPatternImpl: public FIPatternImpl<TTraits>
{
public:
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);
	//�}�b�`���鑮���̃L�[�B
	virtual TKey GetMatchingKey()const{ assert(false); return TKey(); }
	virtual void SetMatchingKey(const TKey& k){ assert(false); }

protected:
	//�p�^�[�������ɑ���������Aattrs�ɒǉ�����B
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);

};

//---------------------------------------------------------------------------
//�C�ӂ̑����Ƀ}�b�`����p�^�[���B
//���x�����l��ł͂Ȃ�������Ƀ}�b�`����B
template <class TTraits>
class FIAASPatternImpl: public FIPatternImpl<TTraits>
{
	FIPattern<TTraits> end;
public:
	FIAASPatternImpl(const FIPattern<TTraits>& e):end(e){
	}
	//�m�[�h�񂪃p�^�[���Ƀ}�b�`���邩�𒲂ׂ�
	virtual std::pair<bool, TInIterator>
		Match(const TInNode& pnode, TInIterator first, TInIterator last, TSourceLabelMaps* lmap);
	//�}�b�`���鑮���̃L�[�B
	virtual TKey GetMatchingKey()const{ assert(false); return TKey(); }
	virtual void SetMatchingKey(const TKey& k){ assert(false); }

protected:
	//�p�^�[�������ɑ���������Aattrs�ɒǉ�����B
	virtual void GenerateSelf(TOutAttributes& attrs, const TTargetLabelMaps* lmap);
};
//---------------------------------------------------------------------------
//�؂̃p�^�[���̒�`���W�߂�����
template <class TTraits>
struct FITreePatterns: public FITreeDefs<TTraits>
{
	//�p�^�[��
	typedef FIPattern<TTraits> TPattern;

	//�C�Ӄm�[�h��
	static TPattern Any(){
		return TPattern(new FIAnyPatternImpl<TTraits>(TKey()));
	}
	//�m�[�h
	static TPattern Node(const TPattern& childrenPattern){
		return TPattern(new FINodePatternImpl<TTraits>(TKey(), childrenPattern));
	}
	//�C�ӂ̒l
	static TPattern Value(){
		return TPattern(new FIAnyValuePatternImpl<TTraits>(TKey()));
	}
	//������
	static TPattern String(const TString& str){
		return TPattern(new FIStringPatternImpl<TTraits>(TKey(), str));
	}
	//������
	static TPattern Attribute(const TKey& key, const TPattern& pattern){
		pattern.GetImpl()->SetMatchingKey(key);
		return pattern;
	}
	//�C�ӑ�����
	static TPattern AnyAttributes(){
		return TPattern(new FIAnyAttributesPatternImpl<TTraits>());
	}
	static TPattern AAS(TPattern& p){
		return TPattern(new FIAASPatternImpl<TTraits>(p));
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
	//���[����ǉ�
	void Add(
		const typename TL::TPattern& lpattern, 
		const typename TR::TPattern& rpattern);
	//parent������[first,last)�͈̔͂̑������ϊ����A���̌��ʂ�outAttrs�ɒǉ�����B
	void Convert(
		typename TR::TOutAttributes& outAttrs,
		const typename TL::TInNode& parent,
		typename TL::TInIterator first, 
		typename TL::TInIterator last);

private:
	typedef std::list<std::pair<typename TL::TPattern, typename TR::TPattern> >
		TRules;

	TRules rules;//���[��

	//�l��ϊ��B
	void ConvertValue(
		typename TR::TOutValue& outValue, 
		typename const TL::TInValue& inValue);

};
//*1 ���̂�TThis::��t���Ȃ��ƁAVC7.1�Ŋ֐��̒�`�Ɛ錾����v���Ȃ��ƌ�����B
//---------------------------------------------------------------------------
//�L�[�ƕ�����̕ϊ����@�̒�`�B
//�ϊ����ƕϊ���ŁA�L�[(Traits::TKey)�ƕ�����(Traits::TValue)�̌^�Ɍ݊���������
//�ꍇ�́AFIConvertationTraits����ꉻ���āA�K�؂�ConvertKey��ConvertString��
//��`����K�v������B
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
//�e�e���v���[�g�N���X�̃����o�֐��̒�`��include�B
#include "FITreeConverterImpl.h"
//---------------------------------------------------------------------------
#endif
