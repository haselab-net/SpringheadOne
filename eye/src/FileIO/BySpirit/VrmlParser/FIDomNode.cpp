#include "stdafx.h"
#include "FIDomNode.h"
using namespace std;

#undef assert
#define assert(x)	assert_imp(x)
inline void assert_imp(bool x){
	if (!x){
		std::cout << "assert failed" << std::endl;
		exit(-1);
	}
}

//---------------------------------------------------------------------------
FIDomTreeTraits::TKeys FIDomTreeTraits::GetAttributeKeys(const TIterator& it)
{
	TKeys keys;
	const TAttributes& attrs= it->GetAttributes();
	for (TAttributes::const_iterator ait= attrs.begin(); 
		ait!=attrs.end(); ++ait)
			keys.push_back(ait->first);
	sort(keys.begin(), keys.end());
	unique(keys.begin(), keys.end());
	return keys;
}

//---------------------------------------------------------------------------
FIDomTreeTraits::TIteratorPair 
	FIDomTreeTraits::GetAttribute(const TIterator& it, const TKey& key)
{
	const TAttributes& attrs= it->GetAttributes();
	TAttributes::const_iterator ait= attrs.find(key);
	if (ait!=attrs.end())
		return make_pair(ait->second.begin(), ait->second.end());
	else
		return make_pair(TIterator(), TIterator());
}


//---------------------------------------------------------------------------
FIDocNodeTraits::TKeys FIDocNodeTraits::GetAttributeKeys(const TIterator& it){
	TKeys keys;
	it->node->attrs;
	for (FIDocNode::TAttrs::iterator ait = it->node->attrs.begin();
		ait != it->node->attrs.end(); ++ait){
		keys.push_back(ait->id);
	}
	sort(keys.begin(), keys.end());
	unique(keys.begin(), keys.end());
	return keys;
}
FIDocNodeTraits::TIteratorPair FIDocNodeTraits::GetAttribute(const TIterator& it, const TKey& key){
	FIDocNodeTraits::TIteratorPair rv;
/*	if (it->nodes.size()){
		rv.first = it->node->attrs.begin();
		for(; rv.first != it->node->attrs.end(); ++rv.first){
			if (rv.first->key.compare(key) == 0) break;
		}
		rv.second = rv.first;
		for(; rv.second != it->node->attrs.end(); ++rv.second){
			if (rv.second->key.compare(key) != 0) break;
		}
	}
*/	return rv;
}
FIDocNodeTraits::TNode FIDocNodeTraits::CreateNode(const TAttributes& attributes){
	TNode node;
	node.node = new FIDocNode;
	TAttributes::const_iterator itV = attributes.find("value");
	if (itV != attributes.end()){
		TAttributes::const_iterator itT = attributes.find("type");
		if (itT != attributes.end()){
			node.node->attrs.push_back(FIDocNode::TAttr("///"));
			node.node->attrs.back().data.type = itT->second[0].value;
			node.node->attrs.back().data.ElementSize(itV->second[0].value.length());
			node.node->attrs.back().data.PushBack(itV->second[0].value.c_str());
		}else{
			std::cout << "Error: type not found." << std::endl;
		}
	}else{
		for(TAttributes::const_iterator it = attributes.begin(); it != attributes.end(); ++it){
			FIDocNode::TAttrs::iterator dit = node.node->attrs.Find(it->first);
			if (dit == node.node->attrs.end()){
				node.node->attrs.push_back(FIDocNode::TAttr(it->first));
				dit = --node.node->attrs.end(); 
			}
			if (it->second.size()){
				FIDocNode::TAttr forNode(it->first);
				for(unsigned i=0; i<it->second.size(); ++it){
					if (it->second[i].node){
						forNode.nodes.push_back(it->second[i].node);
					}else{
						dit->data.ElementSize(20);
						char buf[20];
						memset(buf, 0, 20);
						strncpy(buf, it->second[i].value.c_str(), 20);
						dit->data.PushBack(buf);
					}
				}
				if (forNode.Size()) node.node->attrs.insert(dit, forNode);
			}
		}
	}
	return node;
}
FIDocNodeTraits::TNodeValue FIDocNodeTraits::CreateValue(const std::string& value){
	FIDocNodeTraits::TNodeValue rv;
	rv.value = value;
	return rv;
}
void FIDocNodeTraits::Set(TAttributes& a, TKey key, TNodes& nodes){
	a[key] = nodes;
}


//---------------------------------------------------------------------------
void FIDomNode::PrintXml(int indent)const{
	if (isValue){
		std::cout << std::string(indent*2, ' ')
			<< "<VALUE>" << value << "</VALUE>" << std::endl;
	}else{
		std::cout << std::string(indent*2, ' ') << "<NODE>\n";
		for (TAttributes::const_iterator it= attributes.begin();
			it!=attributes.end(); ++it){
				std::cout << std::string((indent+1)*2, ' ') << "<" << it->first << ">\n";
				for (std::size_t i= 0; i<(it->second.size()); ++i)
					it->second[i].PrintXml(indent+2);
			std::cout << std::string((indent+1)*2, ' ') << "</" << it->first << ">\n";
		}
		std::cout << std::string(indent*2, ' ') << "</NODE>\n";
	}
}
//---------------------------------------------------------------------------
