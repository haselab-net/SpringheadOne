#ifndef FI_MODEL3_TREE_H
#define FI_MODEL3_TREE_H
//---------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
//---------------------------------------------------------------------------
//Model3を満たす木構造の典型例。
//---------------------------------------------------------------------------
struct FIModel3Node;
//---------------------------------------------------------------------------
struct FIModel3Value
{
	//nodeがNULLなら文字列str、そうでなければノードnodeを表す。
	std::string str;
	boost::shared_ptr<FIModel3Node> node;
};
//---------------------------------------------------------------------------
struct FIModel3Attribute
{
	std::string key;
	FIModel3Value value;
};
//---------------------------------------------------------------------------
struct FIModel3Node
{
	std::vector<FIModel3Attribute> attrs;
};
//---------------------------------------------------------------------------
struct FIModel3TreeTraits
{
	typedef std::string TKey;
	typedef std::string TString;

	typedef std::vector<FIModel3Attribute>::const_iterator TInIterator;
	typedef FIModel3Attribute TInAttribute;
	typedef boost::shared_ptr<FIModel3Node> TInNode;
	typedef FIModel3Value TInValue;

	static TKey GetKey(const TInAttribute& attr){
		return attr.key;
	}
	static TInValue GetValue(const TInAttribute& attr){
		return attr.value;
	}
	static bool IsString(const TInValue& value){
		return !value.node;
	}
	static TString GetString(const TInValue& value){
		return value.str;
	}
	static TInNode GetNode(const TInValue& value){
		return value.node;
	}
	static TInIterator GetBegin(const TInNode& node){
		return node->attrs.begin();
	}
	static TInIterator GetEnd(const TInNode& node){
		return node->attrs.end();
	}
	static TInAttribute GetAttribute(const TInNode&, TInIterator i){
		return *i;
	}

	typedef FIModel3Attribute TOutAttribute;
	typedef boost::shared_ptr<FIModel3Node> TOutNode;
	typedef FIModel3Value TOutValue;
	typedef std::vector<TOutAttribute> TOutAttributes;
	static void CreateNode(TOutNode& node, const TOutAttributes& attrs){
		node.reset(new FIModel3Node);
		node->attrs= attrs;
	}
	static void CreateValue(TOutValue& value, const TString& s){
		value.str= s;
		value.node.reset();
	}
	static void CreateValue(TOutValue& value, const TOutNode& node){
		value.node= node;
	}
	static void CreateAttribute(TOutAttribute& attr, const TKey& key, const TOutValue& value){
		attr.key= key;
		attr.value= value;
	}
  static void Add(TOutAttributes& attrs, const TOutAttribute& attr){
		attrs.push_back(attr);
	}
  static void Add(TOutAttributes& attrs, const TOutAttributes& added){
		attrs.insert(attrs.end(), added.begin(), added.end());
	}

};
//---------------------------------------------------------------------------
void Print(const std::vector<FIModel3Attribute>& attrs, int indent= 0);
//---------------------------------------------------------------------------
#endif