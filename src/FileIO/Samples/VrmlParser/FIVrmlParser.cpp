// VrmlParser.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#include <FileIO/FISpirit.h>
#include <map>
#include <list>
#include <stack>
#include <sstream>
#include <fstream>
#pragma hdrstop

#include "FIVrmlParser.h"

#ifdef _MSC_VER
//	標準のassertだと，シンボル数が多すぎてコンパイルできないとVCが言うので
#undef assert
#ifdef _DEBUG
# define assert(x) assert_impl(x, #x);
inline void assert_impl(bool x, const char* str){
	if (!x){
		DSTR << str << std::endl;
		DebugBreak();
	}
}
#else
# define assert(x)
#endif

#endif

namespace Spr{


//@{
///@name boost::spirit の token_node_d を模擬するためのコード
template <typename OpT>
struct node_parser_gen;

template <typename T, typename OpT>
struct node_parser
	:   public boost::spirit::unary<T, boost::spirit::parser<node_parser<T, OpT> > >
{
    typedef node_parser<T, OpT> self_t;
    typedef node_parser_gen<OpT> parser_generator_t;
    typedef boost::spirit::unary_parser_category parser_category_t;
//    typedef node_parser<T, OpT> const &embed_t;

    node_parser(T const& a)
    : unary<T, parser<node_parser<T, OpT> > >(a) {}

    template <typename ScannerT>
    typename boost::spirit::parser_result<self_t, ScannerT>::type
    parse(ScannerT const& scanner) const
    {
		typename boost::spirit::parser_result<self_t, ScannerT>::type hit;
		hit = this->subject().parse(scanner);
        if (hit){
			OpT()(hit);
        }
        return hit;
    }
};

//////////////////////////////////
template <typename OpT>
struct node_parser_gen{
    template <typename T>
    struct result {
        typedef node_parser<T, OpT> type;
    };

    template <typename T>
    static node_parser<T, OpT>
    generate(boost::spirit::parser<T> const& s)
    {
        return node_parser<T, OpT>(s.derived());
    }

    template <typename T>
    node_parser<T, OpT>
    operator[](boost::spirit::parser<T> const& s) const
    {
        return node_parser<T, OpT>(s.derived());
    }
};


struct leaf_node_op
{
    template <typename MatchT>
    void operator()(MatchT& m) const
    {
        if (m.trees.size() == 1)
        {
            m.trees.begin()->children.clear();
        }
        else if (m.trees.size() > 1)
        {
			std::string t;
			for(unsigned i=0; i<m.trees.size(); ++i) m.trees[i].GetTextR(t);
			m = MatchT(m.length());
			m.trees.push_back(Spr::FIPTNode());
			m.trees.back().text = t;
        }
    }
};

const node_parser_gen<leaf_node_op> token_node_d =
    node_parser_gen<leaf_node_op>();
//@}

using namespace std;
using namespace boost::spirit;
//---------------------------------------------------------------------------
FIVrmlGrammar* FIVrmlGrammar::self;
void BeginProtoHandler(const char* first, const char*last){
	FIVrmlGrammar::self->OnBeginProto(first,last);
}
void EndProtoHandler(const char* first, const char*last){
	FIVrmlGrammar::self->OnEndProto(first,last);
}
void FieldTypeHandler(const char* first, const char*last){
	FIVrmlGrammar::self->OnFieldType(first,last);
}
void FieldIdHandler(const char* first, const char*last){
	FIVrmlGrammar::self->OnFieldId(first, last);
}

FIVrmlGrammar::FIVrmlGrammar(){
  self = this;

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
	>> id_ [ &BeginProtoHandler ]
      >> ch_p('[') >> interfaceDeclarations_ >> ch_p(']')
      >> ch_p('{') >> !protoBody_ >> ch_p('}'))[&EndProtoHandler];
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
	id_ = token_node_d[ lexeme_d[
		//	0x30-0x39、0x0-0x20、 0x22、0x23、0x27、0x2c、0x2e、0x5b、0x5c、0x5d、0x7b、 0x7dは除きます。
		(	range_p('\x40','\x5a') | range_p('\x5e','\x7a') | range_p('\x7e','\x7f') | range_p('\x80','\xff')
			| ch_p('\x21') | range_p('\x24','\x26') | range_p('\x28','\x2b')
			| ch_p('\x2d') | ch_p('\x2f') | ch_p('\x7c')
		) >>
		//	0x0-0x20、0x22、0x23、 0x27、0x2c、0x2e、0x5b、0x5c、0x5d、0x7b、0x7dは除きます。 
		*(
			range_p('\x2f','\x5a') | range_p('\x5e','\x7a') | range_p('\x7e','\x7f') | range_p('\x80', '\xff')
			| ch_p('\x21') | range_p('\x24','\x26') | range_p('\x28','\x2b')
			| ch_p('\x2d') | ch_p('\x7c')
		)
	] ];
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
	token_node_d[ lexeme_d[
		!(ch_p('+')|ch_p('-')) >> (		
		(*range_p('0','9') >> ch_p('.') >> +range_p('0','9')) |
		(+range_p('0','9') >> !ch_p('.'))
	)	>>
	!( (ch_p('e')|ch_p('E')) >> !(ch_p('+')|ch_p('-'))
		>> +range_p('0','9') )
	] ];
  sfimageValue_=
    +sfint32Value_;
  sfint32Value_=
	token_node_d[ lexeme_d[
		!(ch_p('+')|ch_p('-')) >> (
			+range_p('0','9') | 
			ch_p('0') >> (ch_p('x')|ch_p('X')) >> +alnum_p
		)
	]];
  sfnodeValue_=
    nodeStatement_ |
    str_p("NULL");
  sfrotationValue_=
    sffloatValue_ >> sffloatValue_ >> sffloatValue_ >> sffloatValue_;
  sfstringValue_=
    ch_p('"') >> token_node_d[ *((ch_p('\\')>>anychar_p) | ~ch_p('"')) ] >> ch_p('"');
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

	TWildParser decParser;
	for (map<string, TWildParser>::iterator it= valueParsers_.begin(); 
			it!=valueParsers_.end(); ++it){
		strs_.push_back(it->first);
		decParser = decParser | 
			str_p(strs_.back().c_str())[&FieldTypeHandler]
				>> id_[&FieldIdHandler]
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
void FIVrmlGrammar::OnBeginProto(const char* first, const char*last)
{
	cout << "beginProto " << string(first, last) << endl;
	protoInfoStack_.push(TProtoInfo());
	protoInfoStack_.top().protoId= string(first, last);
}

//---------------------------------------------------------------------------
void FIVrmlGrammar::OnEndProto(const char*, const char*)
{
	cout << "endProto" << endl;
	TProtoInfo info= protoInfoStack_.top();
	protoInfoStack_.pop();
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
	elmRule= elmParser;
	strs_.push_back(info.protoId);
  nodeParser_=
    nodeParser_ | str_p(strs_.back().c_str()) >> ch_p('{') >> *elmRule >> ch_p('}');
  node_= nodeParser_;
}

void FIVrmlGrammar::OnFieldType(const char* first, const char*last)
{
	std::cout << "fieldType " << std::string(first, last) << std::endl;
	assert(protoInfoStack_.size());
	protoInfoStack_.top().fieldInfos.push_back(TFieldInfo());
	protoInfoStack_.top().fieldInfos.back().type= string(first, last);
}
//---------------------------------------------------------------------------
void FIVrmlGrammar::OnFieldId(const char* first, const char*last)
{
	cout << "fieldId " << string(first, last) << endl;
	assert(protoInfoStack_.size());
	assert(protoInfoStack_.top().fieldInfos.size()>0);
	protoInfoStack_.top().fieldInfos.back().id= string(first, last);
}
//---------------------------------------------------------------------------
void FIVrmlParser::PrintXml(const FIPTInfo& info, const char* text)
{
  map<parser_id, string> nodeNames;
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

  info.Print(std::cout);
}

//---------------------------------------------------------------------------
FIPTInfo FIVrmlParser::Parse(const std::string& s){
	FIPTSkipParser sp;
	sp = space_p | ch_p('\r') | (ch_p('#') >> *~ch_p('\n') >> ch_p('\n'));
	FIVrmlGrammar grammer;
	const char* f = s.c_str();
	const char* l = f + s.length();
	FIPTPhraseParser pp;
	pp = grammer;
	return FIPTParse(f, l, pp, sp);
}
}