#ifndef VRMLPARSER_H
#define VRMLPARSER_H
#include <FileIO/FISpirit.h>
#include <map>
#include <list>
#include <stack>
//---------------------------------------------------------------------------
namespace Spr{;

enum FIVrmlNodeType{
  vntVrmlScene,
  vntStatements,
  vntStatement,
  vntNodeStatement,
  vntProtoStatement,

  vntProtoStatements,
  vntProto,
  vntProtoBody,
  vntInterfaceDeclarations,
  vntRestrictedInterfaceDeclaration,
	//10
  vntInterfaceDeclaration,
  vntExternproto,
  vntExternInterfaceDeclarations,
  vntExternInterfaceDeclaration,
  vntRouteStatement,

  vntURLList,  
  vntNode,
  vntScriptBody,
  vntScriptBodyElement,
  vntNodeBodyElement,
	//20
  vntId,  
  vntFieldType,
  vntFieldValue,
  vntSfboolValue,
  vntSfcolorValue,

  vntSffloatValue,
  vntSfimageValue,
  vntSfint32Value,
  vntSfnodeValue,
  vntSfrotationValue,
	//30
  vntSfstringValue,
  vntSftimeValue,
  vntMftimeValue,
  vntSftimeValues,
  vntSfvec2fValue,

  vntSfvec3fValue,
  vntMfcolorValue,
  vntSfcolorValues,
  vntMffloatValue,
  vntSffloatValues,
	//40
  vntMfint32Value,
  vntSfint32Values,
  vntMfnodeValue,
  vntNodeStatements,
  vntMfrotationValue,

  vntSfrotationValues,
  vntMfstringValue,
  vntSfstringValues,
  vntMfvec2fValue,
  vntSfvec2fValues,
	//50
  vntMfvec3fValue,
  vntSfvec3fValues,
};

//---------------------------------------------------------------------------
class FIVrmlGrammar: public boost::spirit::parser< FIVrmlGrammar >{
public:
    typedef FIPTSkipParser::PhraseScannerT Scanner;
	typedef boost::spirit::match_result<Scanner, boost::spirit::parser_context::context_linker_t::attr_t>::type Result;
	
	struct TWildParser: boost::spirit::parser<TWildParser>{
//		typedef TWildParser          self_t;
//		typedef TWildParser const&   embed_t;
		TWildParser(){}
		TWildParser(const TWildParser& wp): ptr_(wp.ptr_? wp.ptr_->clone() : NULL){}
		template <class T>
		TWildParser& operator=(const T& p){
			ptr_.reset(new boost::spirit::impl::concrete_parser<T, Scanner, Result>(p));
			return *this;
		}
		Result parse(const Scanner& scan)const{
			if (ptr_) return ptr_->do_parse_virtual(scan);
			else return scan.no_match();
		}
		boost::scoped_ptr<boost::spirit::impl::abstract_parser<Scanner, Result> > ptr_;
    };
    
#define TRULE(id)	FIPTPhraseParserWithID< boost::spirit::parser_tag<id> >
/*
    template <FIVrmlNodeType id>
    struct TRule{
      typedef rule<Scanner, parser_context, parser_tag<id> > T;
    };
*/
    struct TFieldInfo{
		FIString type;
		FIString id;
    };

	struct TProtoInfo{
		std::vector<TFieldInfo> fieldInfos;
		FIString protoId;
	};
    
    FIVrmlGrammar();
    
	const TRULE(vntVrmlScene)& start()const{ return vrmlScene_; }
	Result parse(const Scanner& scan) const {
		return start().parse(scan);
	}

    
	void OnBeginProto(const char* first, const char*last);
	void OnEndProto(const char* first, const char*last);
    void OnFieldType(const char* first, const char*last);
    void OnFieldId(const char* first, const char*last);
    
    TRULE(vntVrmlScene) vrmlScene_;
    TRULE(vntStatements) statements_;
    TRULE(vntStatement) statement_;
    TRULE(vntNodeStatement) nodeStatement_;
    TRULE(vntProtoStatement) protoStatement_;
    TRULE(vntProtoStatements) protoStatements_;
    TRULE(vntProto) proto_;
    TRULE(vntProtoBody) protoBody_;
    TRULE(vntInterfaceDeclarations) interfaceDeclarations_;
    TRULE(vntRestrictedInterfaceDeclaration) restrictedInterfaceDeclaration_;
    TRULE(vntInterfaceDeclaration) interfaceDeclaration_;
    TRULE(vntExternproto) externproto_;
    TRULE(vntExternInterfaceDeclarations) externInterfaceDeclarations_;
    TRULE(vntExternInterfaceDeclaration) externInterfaceDeclaration_;
    TRULE(vntRouteStatement) routeStatement_;
    TRULE(vntURLList) URLList_;
  
    TRULE(vntNode) node_;
    TRULE(vntScriptBody) scriptBody_;
    TRULE(vntScriptBodyElement) scriptBodyElement_;
    TRULE(vntNodeBodyElement) nodeBodyElement_;
    TRULE(vntId) id_;
    
	TRULE(vntFieldType) fieldType_;
    TRULE(vntFieldValue) fieldValue_;
    TRULE(vntSfboolValue) sfboolValue_;
    TRULE(vntSfcolorValue) sfcolorValue_;
    TRULE(vntSffloatValue) sffloatValue_;
    TRULE(vntSfimageValue) sfimageValue_;
    TRULE(vntSfint32Value) sfint32Value_;
    TRULE(vntSfnodeValue) sfnodeValue_;
    TRULE(vntSfrotationValue) sfrotationValue_;
    TRULE(vntSfstringValue) sfstringValue_;
    TRULE(vntSftimeValue) sftimeValue_;
    TRULE(vntMftimeValue) mftimeValue_;
    TRULE(vntSftimeValues) sftimeValues_;
    TRULE(vntSfvec2fValue) sfvec2fValue_;
    TRULE(vntSfvec3fValue) sfvec3fValue_;
    TRULE(vntMfcolorValue) mfcolorValue_;
    TRULE(vntSfcolorValues) sfcolorValues_;
    TRULE(vntMffloatValue) mffloatValue_;
    TRULE(vntSffloatValues) sffloatValues_;
    TRULE(vntMfint32Value) mfint32Value_;
    TRULE(vntSfint32Values) sfint32Values_;
    TRULE(vntMfnodeValue) mfnodeValue_;
    TRULE(vntNodeStatements) nodeStatements_;
    TRULE(vntMfrotationValue) mfrotationValue_;
    TRULE(vntSfrotationValues) sfrotationValues_;
    TRULE(vntMfstringValue) mfstringValue_;
    TRULE(vntSfstringValues) sfstringValues_;
    TRULE(vntMfvec2fValue) mfvec2fValue_;
    TRULE(vntSfvec2fValues) sfvec2fValues_;
    TRULE(vntMfvec3fValue) mfvec3fValue_;
    TRULE(vntSfvec3fValues) sfvec3fValues_;
    
    typedef TRULE(vntNode) NodeRule;
    typedef TRULE(vntNodeBodyElement) NodeBodyElementRule;
    
    TWildParser nodeParser_;
		std::map<FIString, TWildParser> valueParsers_;
		std::list<NodeBodyElementRule> nodeBodyElementRules_;
		std::list<FIString> strs_;
		std::stack<TProtoInfo> protoInfoStack_;
	static FIVrmlGrammar* self;
};
//---------------------------------------------------------------------------
class FIVrmlParser
{
public:
	static FIPTInfo Parse(const std::string& s);
	static void PrintXml(const FIPTInfo & info, const char* text);
};

}
//---------------------------------------------------------------------------
#endif