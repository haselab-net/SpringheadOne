#ifndef VRMLPARSER_H
#define VRMLPARSER_H
//---------------------------------------------------------------------------
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
  vntInterfaceDeclaration,
  vntExternproto,
  vntExternInterfaceDeclarations,
  vntExternInterfaceDeclaration,
  vntRouteStatement,
  vntURLList,
  
  vntNode,
	vntNodeBody,
  vntScriptBody,
  vntScriptBodyElement,
  vntNodeBodyElement,
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
  vntMfvec3fValue,
  vntSfvec3fValues,
};
//---------------------------------------------------------------------------
class FIVrmlParser
{
public:
	typedef std::map<boost::spirit::parser_id, std::string> TNodeNameMap;

	static boost::spirit::tree_parse_info<> Parse(const std::string& s);
	static void PrintXml(const boost::spirit::tree_parse_info<>& info, const char* text);
	static TNodeNameMap& GetNodeNameMap();
	static std::string GetNodeName(long id);

};
//---------------------------------------------------------------------------
#endif