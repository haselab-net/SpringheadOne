#ifndef DOMNODE_H
#define DOMNODE_H
//---------------------------------------------------------------------------
#include <iostream>
#include <vector>
//---------------------------------------------------------------------------
//Model1のテスト用に作られたノード。
class FIDomNode
{
	typedef std::vector<FIDomNode> TChildren;

public:
	typedef TChildren::const_iterator TInIterator;
	
	explicit FIDomNode(const std::string& n= "", const std::string& v= "", 
		const TChildren& c= TChildren())
			: name(n), value(v), children(c){}
	std::string GetKey()const{ return name; }
	void SetKey(const std::string& n){ name= n; }
	std::string GetString()const{ return value; }
	void SetString(const std::string& v){ value= v; }
	TInIterator Begin()const{ return children.begin(); }
	TInIterator End()const{ return children.end(); }
	void PrintXml(int indent= 0){
		std::cout << std::string(indent*2, ' ') << '<' << name << " value=\"" << value << "\">\n";
		for (std::size_t i= 0; i<children.size(); ++i)
			children[i].PrintXml(indent+1);
		std::cout << std::string(indent*2, ' ') << "</" << name << ">\n";
	}

private:
	std::string name;
	std::string value;
	TChildren children;

};
//---------------------------------------------------------------------------
//Model3対応のTraits。
//FIDomNodeの木構造はModel1風のものだが、これをModel3の木構造に見せかけている。
struct FIDomTreeTraits
{
	typedef std::string TKey;
	typedef std::string TString;

	typedef FIDomNode::TInIterator TInIterator;
	typedef std::iterator_traits<TInIterator>::value_type TInAttribute;
	typedef TInAttribute TInNode;
	typedef TInAttribute TInValue;

	static TKey GetKey(const TInAttribute& node){
		return node.GetKey();
	}
	static bool IsString(const TInValue& value){
		return value.GetString()!="";//仮
	}
	static TString GetString(const TInValue& value){
		return value.GetString();
	}
	static TInNode GetNode(const TInValue& value){
		return value;
	}
	static TInIterator GetBegin(const TInNode& node){
		return node.Begin();
	}
	static TInIterator GetEnd(const TInNode& node){
		return node.End();
	}
	static TInAttribute GetAttribute(const TInNode& node, TInIterator i){
		return *i;
	}

	typedef std::iterator_traits<TInIterator>::value_type TOutAttribute;
	typedef TOutAttribute TOutNode;
	typedef TOutAttribute TOutValue;
	typedef std::vector<TOutAttribute> TOutAttributes;
	static void CreateNode(TOutNode& node, const TOutAttributes& attrs){
		node= FIDomNode("", "", attrs);
	}
	static void CreateValue(TOutValue& value, const TString& s){
		value= FIDomNode("", s);
	}
	static void CreateValue(TOutValue& value, const TOutNode& node){
		value= node;
	}
	static void CreateAttribute(TOutAttribute& attr, const TKey& key, const TOutValue& value){
		attr= value;
		attr.SetKey(key);
	}
  static void Add(TOutAttributes& attrs, const TOutAttribute& attr){
		attrs.push_back(attr);
	}

};
//---------------------------------------------------------------------------
#endif