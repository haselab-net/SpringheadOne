#ifndef FI_DOCTREETRAIT_H
#define FI_DOCTREETRAIT_H
//---------------------------------------------------------------------------
#include <FileIO/FIDocNode.h>

namespace Spr{;
template <class TR>
void PrintNode(std::ostream& os, typename TR::TInNode node, int indent=0);

struct FIDocNodeTraits
{
	typedef FIString TKey;
	typedef FIString TString;

	typedef FIIterator TInIterator;
	typedef FIIterator TInAttribute;
	typedef UTRef<const FIDocNodeBase> TInNode;
	typedef FIIterator TInValue;

	static TKey GetKey(const TInAttribute& it){
		return it.GetId();
	}
	static TInValue GetValue(const TInAttribute& it){
		return it;
	}
	static bool IsString(const TInValue& it){
		return it.GetNode() == NULL;
	}
	static TString GetString(const TInValue& it){
		size_t sz = it.GetElementSize();
		char buf[1024*10];
		assert(sz < sizeof(buf));
		it.GetDataImp(buf, sz);
		return TString(buf, sz);
	}
	static TInNode GetNode(const TInValue& it){
		return it.GetNode();
	}
	static TInIterator GetBegin(const TInNode& node){
		return node->FirstAttr();
	}
	static TInIterator GetEnd(const TInNode& node){
		return node->LastAttr();
	}
	static TInAttribute GetAttribute(const TInNode& n, TInIterator i){
		return i;
	}

	typedef FIDocNode::TAttr TOutAttribute;
	typedef UTRef<FIDocNode> TOutNode;
	typedef FIDocNode::TValue TOutValue;
	typedef FIDocNode::TAttrs TOutAttributes;
	static void CreateNode(TOutNode& node, const TOutAttributes& attrs){
		node = new FIDocNode;
		node->attrs= attrs;
/*		std::cout << "CreateNode:" << node->attrs.size() << std::endl;
		PrintNode<FIDocNodeTraits>(std::cout, node);
*/	}
	static void CreateValue(TOutValue& value, const TString& s){
		value.data.ElementSize(s.length());
		value.data.PushBack(s.c_str());
	}
	static void CreateValue(TOutValue& value, const TOutNode& node){
		value.nodes.push_back(node);
	}
	static void CreateAttribute(TOutAttribute& attr, const TKey& key, const TOutValue& value){
		attr.id= key;
		attr.data = value.data;
		attr.nodes = value.nodes;
	}
	static void Add(TOutAttributes& attrs, const TOutAttribute& attr){
		attrs.push_back(attr);

		size_t sz = attr.data.Size() * attr.data.ElementSize();
		char buf[1024];
		memcpy(buf, attr.data.Begin(), sz);
		buf[sz]='\0';
//		std::cout << "Add: " << attr.id << "-" << buf << std::endl;
	}
	static void Add(TOutAttributes& attrs, const TOutAttributes& added){
		attrs.insert(attrs.end(), added.begin(), added.end());
	}
};

template <class TR>
void PrintNode(std::ostream& os, typename TR::TInNode node, int indent){
	TR::TInIterator it = TR::GetBegin(node);
	TR::TInIterator end = TR::GetEnd(node);
	for(;it != end; ++it){
		TR::TInAttribute a = TR::GetAttribute(node, it);
		TR::TInValue v = TR::GetValue(a);
		if (TR::IsString(v)){
			FIString key = TR::GetKey(a);
			FIString value = TR::GetString(v);
			os << UTPadding(indent) << key << " = \"" << value << "\"" << std::endl;
		}else{
			FIString key = TR::GetKey(a);
			os << UTPadding(indent) << key << " <" << std::endl;
			PrintNode<TR>(os, TR::GetNode(v), indent+2);
			os << UTPadding(indent) << ">" << std::endl;
		}
	}
}


inline void Print(const FIDocNode::TAttrs& attrs){
	FIDocNode node;
	node.attrs = attrs;
	node.Print(std::cout);
}


}	//	namespace Spr
//---------------------------------------------------------------------------
#endif