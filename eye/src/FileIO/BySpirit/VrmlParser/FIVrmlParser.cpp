// VrmlParser.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <list>
#include <stack>
#include <boost/spirit/utility/confix.hpp>
#include "FIVrmlParser.h"

using namespace std;
using namespace boost;
using namespace boost::spirit;
//---------------------------------------------------------------------------
class FIVrmlGrammar: public grammar<FIVrmlGrammar>
{
public:
  template <class Scanner>
  struct definition
  {
    typedef definition<Scanner> This;
    
    struct TWildParser: parser<TWildParser>{
      typedef typename match_result<Scanner, 
        typename parser_context::context_linker_t::attr_t>::type Result;
      TWildParser(){}
      TWildParser(const TWildParser& wp): ptr_(wp.ptr_? wp.ptr_->clone() : NULL){}
      template <class T>
      TWildParser& operator=(const T& p){
        ptr_.reset(new impl::concrete_parser<T, Scanner, Result>(p));
        return *this;
      }
      Result parse(const Scanner& scan)const{
        if (ptr_)
          return ptr_->do_parse_virtual(scan);
        else
          return scan.no_match();
      }
      scoped_ptr<impl::abstract_parser<Scanner, Result> > ptr_;
    };
    
    template <FIVrmlNodeType id>
    struct TRule{
      typedef rule<Scanner, parser_context, parser_tag<id> > T;
    };
    
    struct TFieldInfo{
      string type;
      string id;
    };

		struct TProtoInfo{
	    vector<TFieldInfo> fieldInfos;
		  string protoId;
		};
    
    definition(const FIVrmlGrammar&);
    
    const typename TRule<vntVrmlScene>::T& start()const{ return vrmlScene_; }
    
    void OnBeginProto(const char* first, const char*last);
    void OnEndProto(const char*, const char*);
    void OnFieldType(const char* first, const char*last);
    void OnFieldId(const char* first, const char*last);
    
    typename TRule<vntVrmlScene>::T vrmlScene_;
    typename TRule<vntStatements>::T statements_;
    typename TRule<vntStatement>::T statement_;
    typename TRule<vntNodeStatement>::T nodeStatement_;
    typename TRule<vntProtoStatement>::T protoStatement_;
    typename TRule<vntProtoStatements>::T protoStatements_;
    typename TRule<vntProto>::T proto_;
    typename TRule<vntProtoBody>::T protoBody_;
    typename TRule<vntInterfaceDeclarations>::T interfaceDeclarations_;
    typename TRule<vntRestrictedInterfaceDeclaration>::T restrictedInterfaceDeclaration_;
    typename TRule<vntInterfaceDeclaration>::T interfaceDeclaration_;
    typename TRule<vntExternproto>::T externproto_;
    typename TRule<vntExternInterfaceDeclarations>::T externInterfaceDeclarations_;
    typename TRule<vntExternInterfaceDeclaration>::T externInterfaceDeclaration_;
    typename TRule<vntRouteStatement>::T routeStatement_;
    typename TRule<vntURLList>::T URLList_;
  
    typename TRule<vntNode>::T node_;
    typename TRule<vntScriptBody>::T scriptBody_;
    typename TRule<vntScriptBodyElement>::T scriptBodyElement_;
    typename TRule<vntNodeBodyElement>::T nodeBodyElement_;
    typename TRule<vntId>::T id_;
  
    typename TRule<vntFieldType>::T fieldType_;
    typename TRule<vntFieldValue>::T fieldValue_;
    typename TRule<vntSfboolValue>::T sfboolValue_;
    typename TRule<vntSfcolorValue>::T sfcolorValue_;
    typename TRule<vntSffloatValue>::T sffloatValue_;
    typename TRule<vntSfimageValue>::T sfimageValue_;
    typename TRule<vntSfint32Value>::T sfint32Value_;
    typename TRule<vntSfnodeValue>::T sfnodeValue_;
    typename TRule<vntSfrotationValue>::T sfrotationValue_;
    typename TRule<vntSfstringValue>::T sfstringValue_;
    typename TRule<vntSftimeValue>::T sftimeValue_;
    typename TRule<vntMftimeValue>::T mftimeValue_;
    typename TRule<vntSftimeValues>::T sftimeValues_;
    typename TRule<vntSfvec2fValue>::T sfvec2fValue_;
    typename TRule<vntSfvec3fValue>::T sfvec3fValue_;
    typename TRule<vntMfcolorValue>::T mfcolorValue_;
    typename TRule<vntSfcolorValues>::T sfcolorValues_;
    typename TRule<vntMffloatValue>::T mffloatValue_;
    typename TRule<vntSffloatValues>::T sffloatValues_;
    typename TRule<vntMfint32Value>::T mfint32Value_;
    typename TRule<vntSfint32Values>::T sfint32Values_;
    typename TRule<vntMfnodeValue>::T mfnodeValue_;
    typename TRule<vntNodeStatements>::T nodeStatements_;
    typename TRule<vntMfrotationValue>::T mfrotationValue_;
    typename TRule<vntSfrotationValues>::T sfrotationValues_;
    typename TRule<vntMfstringValue>::T mfstringValue_;
    typename TRule<vntSfstringValues>::T sfstringValues_;
    typename TRule<vntMfvec2fValue>::T mfvec2fValue_;
    typename TRule<vntSfvec2fValues>::T sfvec2fValues_;
    typename TRule<vntMfvec3fValue>::T mfvec3fValue_;
    typename TRule<vntSfvec3fValues>::T sfvec3fValues_;
    
    typedef typename TRule<vntNode>::T NodeRule;
    typedef typename TRule<vntNodeBody>::T NodeBodyRule;
    typedef typename TRule<vntNodeBodyElement>::T NodeBodyElementRule;
    
    TWildParser nodeParser_;
		map<string, TWildParser> valueParsers_;
    list<NodeBodyElementRule> nodeBodyElementRules_;
    list<NodeBodyRule> nodeBodyRules_;
		list<string> strs_;
		stack<TProtoInfo> protoInfoStack_;
    
  };
  
};

//---------------------------------------------------------------------------
template <class Scanner>
FIVrmlGrammar::definition<Scanner>::definition(const FIVrmlGrammar&)
{
  vrmlScene_=
    statements_;
  statements_=
    *statement_;
  statement_=
    protoStatement_ |
    routeStatement_ |
    nodeStatement_;
  nodeStatement_=
    str_p("DEF") >> id_ >> node_ |
    str_p("USE") >> id_ |
    node_;
  protoStatement_=
    proto_ |
    externproto_;
  protoStatements_=
    *protoStatement_;
  proto_=
    (str_p("PROTO")
      >> id_[bind(&This::OnBeginProto, this, _1, _2)]
      >> ch_p('[') >> interfaceDeclarations_ >> ch_p(']')
      >> ch_p('{') >> !protoBody_ >> ch_p('}'))[bind(&This::OnEndProto, this, _1, _2)];
		//既定のノードをPROTOで定義するため、本物のVRMLの定義と違って、protoBodyの省略を許している。
  protoBody_=
    protoStatements_ >> nodeStatement_ >> statements_;
  interfaceDeclarations_=
    *interfaceDeclaration_;
  externproto_=
    str_p("EXTERNPROTO") >> id_ 
      >> ch_p('[') >> !externInterfaceDeclarations_ >> ch_p(']')
      >> URLList_;
  externInterfaceDeclarations_=
    *externInterfaceDeclaration_;
  externInterfaceDeclaration_=
    str_p("eventIn") >> fieldType_ >> id_ |
    str_p("eventOut") >> fieldType_ >> id_ |
    str_p("field") >> fieldType_ >> id_ |
    str_p("exposedField") >> fieldType_ >> id_;
  routeStatement_=
    str_p("ROUTE") >> id_ >> ch_p('.') >> id_ >> str_p("TO")
      >> id_ >> ch_p('.') >> id_;
  URLList_=
    mfstringValue_;
  nodeParser_=
    str_p("Script") >> ch_p('{') >> scriptBody_ >> ch_p('}');
  node_= nodeParser_;
  scriptBody_=
    *scriptBodyElement_;
  scriptBodyElement_=
    str_p("eventIn") >> fieldType_ >> id_ >> str_p("IS") >> id_ |
    str_p("eventOut") >> fieldType_ >> id_ >> str_p("IS") >> id_ |
    str_p("field") >> fieldType_ >> id_ >> str_p("IS") >> id_ |
    restrictedInterfaceDeclaration_ |
    nodeBodyElement_;//ここで使われるのはどこのfieldId？
  nodeBodyElement_=
    routeStatement_ |
    protoStatement_ |
    id_ >> str_p("IS") >> id_ |
    id_ >> fieldValue_;
  id_= regex_p("[^\\x30-\\x39\\x00-\\x20\\x22\\x23\\x27\\x2b\\x2c\\x2d\\x2e\\x5b"
    "\\x5c\\x5d\\x7b\\x7d\\x7f]"
    "[^\\x00-\\x20\\x22\\x23\\x27\\x2c\\x2e\\x5b\\x5c\\x5d\\x7b\\x7d\\x7f]*");
  fieldType_=
    str_p("MFColor") | str_p("MFFloat") | str_p("MFInt32") | str_p("MFNode") | 
    str_p("MFRotation") | str_p("MFString") | str_p("MFTime") | str_p("MFVec2f") | 
    str_p("MFVec3f") |
    str_p("SFBool") | str_p("SFColor") | str_p("SFFloat") | str_p("SFImage") | 
    str_p("SFInt32") | str_p("SFNode") | str_p("SFRotation") | str_p("SFString") | 
    str_p("SFTime") | str_p("SFVec2f") | str_p("SFVec3f");
  fieldValue_=
    sfboolValue_ | +sffloatValue_ | sfnodeValue_ | sfstringValue_ | 
		ch_p('[') >> *(sfboolValue_ | +sffloatValue_ | sfnodeValue_ | sfstringValue_) >> ch_p(']');
  sfboolValue_=
    str_p("TRUE") |
    str_p("FALSE");
  sfcolorValue_=
    sffloatValue_ >> sffloatValue_ >> sffloatValue_;
  sffloatValue_=
    regex_p("([+\\-]?((([0-9]+(\\.)?)|([0-9]*\\.[0-9]+))([eE][+\\-]?[0-9]+)?))");
  sfimageValue_=
    +sfint32Value_;
  sfint32Value_=
    regex_p("([+\\-]?(([0-9]+)|(0[xX][0-9a-fA-F]+)))");
  sfnodeValue_=
    nodeStatement_ |
    str_p("NULL");
  sfrotationValue_=
    sffloatValue_ >> sffloatValue_ >> sffloatValue_ >> sffloatValue_;
  sfstringValue_=
    regex_p("\"([^\\\\\"]|\\\\.)*\"");
  sftimeValue_=
    sffloatValue_;
  mftimeValue_=
    sftimeValue_ |
    ch_p('[') >> *sftimeValue_ >> ch_p(']');
  sfvec2fValue_=
    sffloatValue_ >> sffloatValue_;
  sfvec3fValue_=
    sffloatValue_ >> sffloatValue_ >> sffloatValue_;
  mfcolorValue_=
    sfcolorValue_ |
    ch_p('[') >> *sfcolorValue_ >> ch_p(']');
  mffloatValue_=
    sffloatValue_ |
    ch_p('[') >> *sffloatValue_ >> ch_p(']');
  mfint32Value_=
    sfint32Value_ |
    ch_p('[') >> *sfint32Value_ >> ch_p(']');
  mfnodeValue_=
    nodeStatement_ |
    ch_p('[') >> *nodeStatement_ >> ch_p(']');
  mfrotationValue_=
    sfrotationValue_ |
    ch_p('[') >> *sfrotationValue_ >> ch_p(']');
  mfstringValue_=
    sfstringValue_ |
    ch_p('[') >> *sfstringValue_ >> ch_p(']');
  mfvec2fValue_=
    sfvec2fValue_ |
    ch_p('[') >> *sfvec2fValue_ >> ch_p(']');
  mfvec3fValue_=
    sfvec3fValue_ |
    ch_p('[') >> *sfvec3fValue_ >> ch_p(']');

	valueParsers_["MFColor"]= mfcolorValue_;
	valueParsers_["MFFloat"]= mffloatValue_;
	valueParsers_["MFInt32"]= mfint32Value_;
	valueParsers_["MFNode"]= mfnodeValue_;
	valueParsers_["MFRotation"]= mfrotationValue_;
	valueParsers_["MFString"]= mfstringValue_;
	valueParsers_["MFTime"]= mftimeValue_;
	valueParsers_["MFVec2f"]= mfvec2fValue_;
	valueParsers_["MFVec3f"]= mfvec3fValue_;
	valueParsers_["SFBool"]= sfboolValue_;
	valueParsers_["SFColor"]= sfcolorValue_;
	valueParsers_["SFFloat"]= sffloatValue_;
	valueParsers_["SFImage"]= sfimageValue_;
	valueParsers_["SFInt32"]= sfint32Value_;
	valueParsers_["SFNode"]= sfnodeValue_;
	valueParsers_["SFRotation"]= sfrotationValue_;
	valueParsers_["SFString"]= sfstringValue_;
	valueParsers_["SFTime"]= sftimeValue_;
	valueParsers_["SFVec2f"]= sfvec2fValue_;
	valueParsers_["SFVec3f"]= sfvec3fValue_;
  
  TWildParser decParser;//::= fieldType fieldId fieldValue
	for (map<string, TWildParser>::iterator it= valueParsers_.begin(); 
			it!=valueParsers_.end(); ++it){
		strs_.push_back(it->first);
		decParser= decParser | 
			str_p(strs_.back().c_str())[bind(&This::OnFieldType, this, _1, _2)]
				>> id_[bind(&This::OnFieldId, this, _1, _2)]
				>> (it->second);
	}
	
	restrictedInterfaceDeclaration_=
    str_p("eventIn") >> fieldType_ >> id_ |
    str_p("eventOut") >> fieldType_ >> id_ |
    str_p("field") >> decParser;
  interfaceDeclaration_=
    restrictedInterfaceDeclaration_ |
    str_p("exposedField") >> decParser;
}

//---------------------------------------------------------------------------
template <class Scanner>
void FIVrmlGrammar::definition<Scanner>::OnBeginProto(const char* first, const char*last)
{
  cout << "beginProto " << string(first, last) << endl;
  protoInfoStack_.push(TProtoInfo());
	protoInfoStack_.top().protoId= string(first, last);
}

//---------------------------------------------------------------------------
template <class Scanner>
void FIVrmlGrammar::definition<Scanner>::OnEndProto(const char*, const char*)
{
  cout << "endProto" << endl;
	TProtoInfo info= protoInfoStack_.top();
	protoInfoStack_.pop();
	//新しく定義されたノードのパーサを作る
  TWildParser elmParser;
  elmParser=
    routeStatement_ |
    protoStatement_ |
    id_ >> str_p("IS") >> id_;
	for (size_t i= 0; i<info.fieldInfos.size(); ++i){
		strs_.push_back(info.fieldInfos[i].id);
    elmParser= elmParser | str_p(strs_.back().c_str()) >> valueParsers_[info.fieldInfos[i].type];
	}
  nodeBodyElementRules_.push_back(NodeBodyElementRule());
	NodeBodyElementRule& elmRule= nodeBodyElementRules_.back();
  nodeBodyRules_.push_back(NodeBodyRule());
	NodeBodyRule& bodyRule= nodeBodyRules_.back();
	elmRule= elmParser;
	bodyRule= *elmRule;
	strs_.push_back(info.protoId);
  nodeParser_=
    nodeParser_ | str_p(strs_.back().c_str()) >> ch_p('{') >> bodyRule >> ch_p('}');
  node_= nodeParser_;
}

//---------------------------------------------------------------------------
template <class Scanner>
void FIVrmlGrammar::definition<Scanner>::OnFieldType(const char* first, const char*last)
{
  cout << "fieldType " << string(first, last) << endl;
	assert(!protoInfoStack_.empty());
  protoInfoStack_.top().fieldInfos.push_back(TFieldInfo());
  protoInfoStack_.top().fieldInfos.back().type= string(first, last);
}

//---------------------------------------------------------------------------
template <class Scanner>
void FIVrmlGrammar::definition<Scanner>::OnFieldId(const char* first, const char*last)
{
  cout << "fieldId " << string(first, last) << endl;
	assert(!protoInfoStack_.empty());
  assert(protoInfoStack_.top().fieldInfos.size()>0);
  protoInfoStack_.top().fieldInfos.back().id= string(first, last);
}

//---------------------------------------------------------------------------
void FIVrmlParser::PrintXml(const tree_parse_info<>& info, const char* text)
{
  tree_to_xml(cout, info.trees, text, GetNodeNameMap());
}

//---------------------------------------------------------------------------
boost::spirit::tree_parse_info<> FIVrmlParser::Parse(const std::string& s)
{
  FIVrmlGrammar grammer;
	return pt_parse(s.c_str(), grammer, space_p | comment_p("#"));
}

//---------------------------------------------------------------------------
FIVrmlParser::TNodeNameMap& FIVrmlParser::GetNodeNameMap()
{
  static TNodeNameMap nodeNames;
	if (nodeNames.empty()){
		nodeNames[vntVrmlScene]= "vrmlScene";
		nodeNames[vntStatements]= "statements";
		nodeNames[vntStatement]= "statement";
		nodeNames[vntNodeStatement]= "nodeStatement";
		nodeNames[vntProtoStatement]= "protoStatement";
		nodeNames[vntProtoStatements]= "protoStatements";
		nodeNames[vntProto]= "proto";
		nodeNames[vntProtoBody]= "protoBody";
		nodeNames[vntInterfaceDeclarations]= "interfaceDeclarations";
		nodeNames[vntRestrictedInterfaceDeclaration]= "restrictedInterfaceDeclaration";
		nodeNames[vntInterfaceDeclaration]= "interfaceDeclaration";
		nodeNames[vntExternproto]= "externproto";
		nodeNames[vntExternInterfaceDeclarations]= "externInterfaceDeclarations";
		nodeNames[vntExternInterfaceDeclaration]= "externInterfaceDeclaration";
		nodeNames[vntRouteStatement]= "routeStatement";
		nodeNames[vntURLList]= "URLList";
		nodeNames[vntNode]= "node";
		nodeNames[vntNodeBody]= "nodeBody";
		nodeNames[vntScriptBody]= "scriptBody";
		nodeNames[vntScriptBodyElement]= "scriptBodyElement";
		nodeNames[vntNodeBodyElement]= "nodeBodyElement";
		nodeNames[vntId]= "id";
		nodeNames[vntFieldType]= "fieldType";
		nodeNames[vntFieldValue]= "fieldValue";
		nodeNames[vntSfboolValue]= "sfboolValue";
		nodeNames[vntSfcolorValue]= "sfcolorValue";
		nodeNames[vntSffloatValue]= "sffloatValue";
		nodeNames[vntSfimageValue]= "sfimageValue";
		nodeNames[vntSfint32Value]= "sfint32Value";
		nodeNames[vntSfnodeValue]= "sfnodeValue";
		nodeNames[vntSfrotationValue]= "sfrotationValue";
		nodeNames[vntSfstringValue]= "sfstringValue";
		nodeNames[vntSftimeValue]= "sftimeValue";
		nodeNames[vntMftimeValue]= "mftimeValue";
		nodeNames[vntSftimeValues]= "sftimeValues";
		nodeNames[vntSfvec2fValue]= "sfvec2fValue";
		nodeNames[vntSfvec3fValue]= "sfvec3fValue";
		nodeNames[vntMfcolorValue]= "mfcolorValue";
		nodeNames[vntSfcolorValues]= "sfcolorValues";
		nodeNames[vntMffloatValue]= "mffloatValue";
		nodeNames[vntSffloatValues]= "sffloatValues";
		nodeNames[vntMfint32Value]= "mfint32Value";
		nodeNames[vntSfint32Values]= "sfint32Values";
		nodeNames[vntMfnodeValue]= "mfnodeValue";
		nodeNames[vntNodeStatements]= "nodeStatements";
		nodeNames[vntMfrotationValue]= "mfrotationValue";
		nodeNames[vntSfrotationValues]= "sfrotationValues";
		nodeNames[vntMfstringValue]= "mfstringValue";
		nodeNames[vntSfstringValues]= "sfstringValues";
		nodeNames[vntMfvec2fValue]= "mfvec2fValue";
		nodeNames[vntSfvec2fValues]= "sfvec2fValues";
		nodeNames[vntMfvec3fValue]= "mfvec3fValue";
		nodeNames[vntSfvec3fValues]= "sfvec3fValues";
	}
	return nodeNames;
}

//---------------------------------------------------------------------------
std::string FIVrmlParser::GetNodeName(long id)
{
	return GetNodeNameMap()[parser_id(id)];
}

//---------------------------------------------------------------------------

