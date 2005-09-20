COMMENT		:	"//" "\n" | "#" "\n" | "/*" "*/";
WHITE		:	" " "\t" "\n";

START		:	{ ("xof 0302txt 0064\n" | "xof 0303txt 0032") / statement* }/;
statement	:	template | data/;
TEMPLATE	TNewField:
				{
					SERIES('fields')(
						( UNTIL(1 1 'array') 'name' += type "," /)*
						'name' += type ";"*/
					)
				} /;
TEMPLATE	TNewType:
				"TNewType" 'children' += {
					'type'='TNewType' (WS 'name'=IDSTR|)
				 "{" /
					SERIES('fields')(
						( UNTIL(1 1 'array') 'name' += type "," /)*
						'name' += type ";"*/
					)
					(data | "{"'children'+={'ref'+='true' 'name'+=IDSTR }"}")*
				} "}" /;
template	:	"template" 'template'+={
					$tplType = IDSTR /
					'TNewField' = ['F' + $tplType]
					'TNewType'=$tplType
					"{" uuid define* "}" /
					CREATE_INSTANCE('TNewField')
					CREATE_INSTANCE('TNewType' 'data')
				}/;
uuid		:	'uuid' = QSTR('<>')/;
define		:	defNormal | defArray | defOpen | defRestrict;
defNormal	:	'fields' += { 'type'=['F'+IDSTR] 'name'=IDSTR ";"};
defArray	:	'fields' += { "array" 'type'=['F'+IDSTR] 'name'=IDSTR
					"[" 'array' = arraySuffix "]" ";"};
defOpen		:	"[...]" 'open' = 'true';
defRestrict	:	"[" 'restriction' = IDSTR ("," 'restriction'+=IDSTR)* "]";
arraySuffix	:	IDSTR | NUMBER;
data		:	"FAIL";

FWORD		:	NUMWORD;
FDWORD		:	NUMDWORD;
FFLOAT		:	NUMFLOAT;
FSTRING		:	DQSTR;
