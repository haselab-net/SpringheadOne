COMMENT		:	"#" "\n";
WHITE		:	" " "\t" "\n" ",";

START		:	{scene}/ EOF;
scene		:	('children'+=nodeDec | 'proto'+=protoDec)*;
nodeDec		:	"DEF"/ { 'nodeID'=VRMLID node}
			|	"USE"/ { 'nodeID'=VRMLID}
			|	{node};
protoDec	:	{proto};

TEMPLATE
	TProto	:	"VNodeT" PUNCT 'protoNode' = 'VNodeT'/	"{"
				SERIES('field')( 'VFieldName'='default'/ )
				(SELECT('field')(
					"VFieldName" PUNCT
					( "IS" 'VFieldName'=VRMLID/
					| $tmpv UNSET/ valueReader 'VFieldName'=$tmpv/)
				))*
				"}";
proto		:	"PROTO"/ 'VNodeT'=VRMLID
				"[" (intfDec)* "]"/ "{" scene "}"/
				CREATE_INSTANCE('TProto' 'node');
rstIntfDec	:	'eventIn' += {"eventIn" 'intfType'='eventIn'/
				'VFieldType'=VRMLID 'VFieldName'=VRMLID }
			|	'eventOut' += {"eventOut" 'intfType'='eventOut'/
				'VFieldType'=VRMLID 'VFieldName'=VRMLID }
			|	'field' += {"field" 'intfType'='field'/
				$type=VRMLID 'VFieldName'=VRMLID 'VFieldType'=$type
				'valueReader'=[$type+'Value'] $tmpv UNSET
				LINK('valueReader') 'default'=$tmpv };
intfDec		:	rstIntfDec
			|	'field' += { "exposedField" 'intfType'='exposedField'/
				$type=VRMLID 'VFieldName'=VRMLID 'VFieldType'=$type
				'valueReader'=[$type+'Value'] $tmpv UNSET
				LINK('valueReader') 'default'=$tmpv };
node		:	"DUMMY";

SFBoolValue	:		$tmpv += ("TRUE" 'true'| "FALSE" 'false');
SFColorValue:		$tmpv+=NUMFLOAT $tmpv+=NUMFLOAT $tmpv+=NUMFLOAT;
SFFloatValue:		$tmpv+=NUMFLOAT;
SFImageValue:		$tmpv+=NUMFLOAT+;
SFInt32Value:		$tmpv+=NUMINT;
SFNodeValue	:		$tmpv+=(nodeDec | "NULL" 'NULL');
SFRotationValue:	$tmpv+=NUMFLOAT $tmpv+=NUMFLOAT
					$tmpv+=NUMFLOAT $tmpv+=NUMFLOAT;
SFStringValue:		$tmpv+=DQSTR;
SFTimeValue:		$tmpv+=NUMFLOAT;
SFVec2fValue:		$tmpv+=NUMFLOAT $tmpv+=NUMFLOAT;
SFVec3fValue:		$tmpv+=NUMFLOAT $tmpv+=NUMFLOAT $tmpv+=NUMFLOAT;
MFColorValue:		SFColorValue | "[" SFColorValue* "]";
MFFloatValue:		SFFloatValue | "[" SFFloatValue* "]";
MFInt32Value:		SFInt32Value | "[" SFInt32Value* "]";
MFNodeValue	:		$tmpv+=nodeDec|"[" ($tmpv+=nodeDec)* "]";
MFRotationValue:	SFRotationValue  | "[" SFRotationValue* "]";
MFStringValue:		SFStringValue | "[" SFStringValue* "]";
MFVec2fValue:		SFVec2fValue | "[" SFVec2fValue * "]";
MFVec3fValue:		SFVec3fValue | "[" SFVec3fValue * "]";
