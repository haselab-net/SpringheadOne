#ifndef FI_SPIRIT_TREE_TRAITS_H
#define FI_SPIRIT_TREE_TRAITS_H
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include <iterator>
//---------------------------------------------------------------------------
struct FIVrmlSpiritTreeTraits
{
	typedef boost::spirit::tree_match<const char*>::const_tree_iterator
		TNodeIterator;

	typedef std::string TKey;
	typedef std::string TString;

	//parentが指すノードのn番目の子ノードを表す属性。
	//ただし、n==-1なら、ノードのtype（statementsとかsfint32Valueとか）を
	//表す属性。
	struct TInAttribute{
		TNodeIterator parent;
		int n;
		TInAttribute(){}
		TInAttribute(TNodeIterator p, int j): parent(p), n(j){}
		TNodeIterator GetPointee()const{
			assert(n>=0);
			return parent->children.begin()+n;
		}
	};
	typedef TInAttribute TInNode;
	typedef TInAttribute TInValue;
	typedef int TInIterator;

	static TKey GetKey(const TInAttribute& attr){
		return (attr.n>=0)? "child" : "type";
	}
	static TInValue GetValue(const TInAttribute& attr){
		return attr;
	}
	static bool IsString(const TInValue& value);
	static TString GetString(const TInValue& value);
	static TInNode GetNode(const TInValue& value){
		return value;
	}
	static TInIterator GetBegin(const TInNode& node){
		return -1;
	}
	static TInIterator GetEnd(const TInNode& node){
		return int(node.GetPointee()->children.size());
/*
		return (node.GetPointee()->children.end())
			-(node.GetPointee()->children.begin());
*/
	}
	static TInAttribute GetAttribute(const TInNode& node, TInIterator i){
		assert(node.n>=0);
		return TInAttribute(node.GetPointee(), i);
	}

	typedef std::iterator_traits<TNodeIterator>::value_type TOutAttribute;
	typedef TOutAttribute TOutNode;
	typedef TOutAttribute TOutValue;
	typedef std::vector<TOutAttribute> TOutAttributes;

	static void CreateNode(TOutNode& node, const TOutAttributes& attrs){
		assert(false);//未実装
	}
	static void CreateValue(TOutValue& value, const TString& s){
		assert(false);//未実装
	}
	static void CreateValue(TOutValue& value, const TOutNode& node){
		value= node;
	}
	static void CreateAttribute(TOutAttribute& attr, const TKey& key, const TOutValue& value){
		assert(false);//未実装
	}
  static void Add(TOutAttributes& attrs, const TOutAttribute& attr){
		attrs.push_back(attr);
	}
  static void Add(TOutAttributes& attrs, const TOutAttributes& added){
		attrs.insert(attrs.end(), added.begin(), added.end());
	}

	static std::string Trim(const std::string& s);
};

//---------------------------------------------------------------------------
#endif
