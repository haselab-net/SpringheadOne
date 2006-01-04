/*	このファイルは一般パーサ(GenParser)のための文法定義ファイルです．
	
・機能について
	一般パーサは，このファイルを(文法定義ファイル)をロードして，パーサ
	を生成します．生成されたパーサが，ターゲットファイルをロードします．
	ロードすると，一般文書ノードツリー(DOMもどき)が生成されます．
	ユーザは一般文書ノードツリーを目的のデータ構造に変換して使用する
	ことになります．
	
・パーサの構成について
	パーサはファイルを読みながら，アクションを行い，一般文書ノードを生成
	していきます．パーサにはデータスタックがあります．
	また，属性の追加などの操作対象となる，「現在のノード」という物があります．

・文法について
	BNF記法で文法を記述するのが基本です．
	START が開始記号です(STARTは予約語)．
	終端記号には，次のものがあります．
	  ""で囲まれた文字列:	文字列を読み出します．
	  IDSTR	:				ID文字列を読み出します．[A-Za-z_][A-Za-z_]*
	  NUMBER:				数値を読み出します．
	正規文法的な書き方ができます．
	  A*	:				Aの0回以上の繰り返し
	  A+	:				Aの1回以上の繰り返し

	また，読み出したときの動作もBNFの中で指定します．スタック(dataStack)
	に対する操作と，一般文書ノード(DOMもどき)に対する操作ができます．
	  ''で囲まれた文字列:	文字列をスタックに積みます．
	  $文字列	:			変数をスタックに積みます．文字列が変数名になります．
	  IDSTR	:				読み出した文字列をスタックに積みます．
	  NUMBER:				読み出した数値をスタックに積みます．
	  {		:				ノードを作成します．作成したノードが現在のノードに
	  						なります．
	  }		:				作成したノードを閉じて，スタックに積みます．
	  						現在のノードは作成前の状態に戻ります．
	  =		:				スタックから，2つ(A,B)を取り出して，属性名Bに
	  						値Aを最後に作成したノードに設定します．
	  						= は右辺のあとで処理されます．
	  +=	:				スタックから，2つ(A,B)を取り出して，属性名Bに
	  						値Aを最後に作成したノードに追加します．
	  						+= は右辺のあとで処理されます．
	  [式]	:				式を計算した結果をスタックに積みます．
	  						式では，文字列と数値を扱うことができます．
	  						文字列の足し算は連結になります．
	  UNSET					スタックから1つ(A)を取り出して，属性名Aをクリアします．

	コメントとホワイトスペース
		コメントとホワイトスペースは COMMENT, WHITE に指定します．
		
	テンプレート
		テンプレートは，文法を増やす文法です．
		テンプレート定義の例：
		TEMPLATE	TNewType:	//	TNewType はテンプレート名
				"TNewType"/ 'children' += {
					'type'='TNewType' ('name'=IDSTR|) "{" "}" } /
				SELECT('def')(  )
				;
		テンプレート使用の例：
		template	:	"template" 'def'+={
					$tplType = IDSTR /
					'TNewType'=$tplType
					CREATE_INSTANCE('TNewType' 'data')	}/;	
					//	TNewType というテンプレート名のテンプレートを
					//	data に追加する．
					//	CREATE_INSTANCE するときの現在のノードの属性は，
					//	テンプレート内の文字列を置き換える．
					//	例えば，テンプレート内の TNewType は，
					//	IDSTR に置き換えられる．
	
	関数リファレンス
	CREATE_INSTANCE(tmpl, node)
		tmpl のインスタンスを作成し，nodeに加えます．

	UNTIL(from=0, delta=1, end)
		from から delta づつ end まで繰り返します．
	
	


*/


COMMENT		:	"/*" "*/" | "//" "\n" | "#" "\n";
WHITE		:	" " "\t" "\n";

START		:	{ sentence* }/;
sentence	:	'sentence' += { sentenceId ":"/ select ";" }/;
sentenceId	:	'name' = IDSTR | "TEMPLATE" 'name' = IDSTR 'template'='true';
select		:	'series'+={series} selcont*;
selcont		:	"|"/ 'series'+={series};
series		:	('repeat'+={repeat})*;
repeat		:	'phrase'={phrase}	(	"*" 'repNum'='zeroMore'
								|	"+" 'repNum'='oneMore'
								|	'repNum'='once'	);
phrase		:	"/" 'op'='fix'
			|	'left'={left} "="	'right'={right} 'op'='assign'
			|	'left'={left} "+="	'right'={right} 'op'='append'
			|						'right'={right} 'op'='none';

arg			:	'arg' += (SQSTR | NUMBER);
left		:	"[" 'left'={form} "]" 'type'='form'
			|	'value'=SQSTR 'type'='sqstr'
			|	"$" 'value'=IDSTR 'type'='var'
			|	'value'=IDSTR "(" arg* ")" 'type'='func'
			|	'value'=IDSTR 'type'='id';


right		:	"("/ 'sentence'={select} ")"/ 'type'='paren'/
			|	"{"	'value'={select} "}" 'type'='node'
			|	'value'=DQSTR 'type'='dqstr'
			|	'value'=NUMBER 'type'='number'
			|	left;

form		:	'left'={exp} (ePlus | eMinus )*;
ePlus		:	"+" 'right' += {exp 'op'='plus'};
eMinus		:	"-" 'right' += {exp 'op'='minus'};
exp			:	'left'={term} (tMul | tDiv)*;
tMul		:	"*" 'right' += {term 'op'='mul'};
tDiv		:	"/" 'right' += {term 'op'='div'};
term		:	"(" 'left' = {form} ")" 'type'='form'
			|	'value'=IDSTR 'type'='id'
			|	'value'=SQSTR 'type'='sqstr'
			|	'value'=NUMBER 'type'='number'
			|	"$" 'value'=IDSTR 'type'='var';
