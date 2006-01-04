#include "stdafx.h"
#include <sstream>
#include <fstream>
#include "FIVrmlParser.h"
#include "../../FITreeConverter.h"
#include "FIVrmlSpiritTreeTraits.h"
//#include "FIModel3Tree.h"
#include "FIDocTreeTrait.h"
#include <tchar.h>
using namespace std;
using namespace boost::spirit;
using namespace Spr;
//---------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	//ファイルor標準入力から読み込む。
  string text;
	if (argc>=2){
	  ifstream fs(argv[1]);
		getline(fs, text, '\0');
	}else{
		getline(cin, text, '\0');
	}

	//パース。
	tree_parse_info<> info= FIVrmlParser::Parse(text);
  if (!info.full){
    cout << "Parse error." << endl;
    return 1;
  }
//	FIVrmlParser::PrintXml(info, text.c_str());
	
	//木の変換の準備。
	typedef FITreePatterns<FIVrmlSpiritTreeTraits> S;
	typedef FITreePatterns<FIDocNodeTraits> D;
	FIHalfTreeConverter<FIVrmlSpiritTreeTraits, FIDocNodeTraits> conv;

#define SA S::Attribute
#define SN S::Node
#define SS S::String
#define S1 S::Value()[1]
#define S2 S::Value()[2]
#define S3 S::Value()[3]
#define S4 S::Value()[4]
#define S5 S::Value()[5]
#define DA D::Attribute
#define DN D::Node
#define DS D::String
#define D1 D::Value()[1]
#define D2 D::Value()[2]
#define D3 D::Value()[3]
#define D4 D::Value()[4]
#define D5 D::Value()[5]
//メモ： S::AnyAttributes()[n], D::AnyAttributes()[n] で「残りの全部の属性」にマッチする変数ができます。
	
	//	プロトタイプ宣言のトップの変換
	conv.Add(
		SA("type", SS("statement")) >>
		SA("child", SN(
			SA("type", SS("protoStatement")) >>
			SA("child", SN(
				SA("type", SS("proto")) >>
				SA("child", SS("PROTO")) >>
				SA("child", SN(
					SA("type", SS("id")) >>
					SA("child", S1)
				)) >>
				SA("child", SS("[")) >>
				SA("child", S2) >>
				SA("child", SS("]")) >>
				SA("child", SS("{")) >>
				SA("child", S3) >>
				SA("child", SS("}"))
			))
		)),
		DA("PROTO", D1)
	);		

	//	普通のノードのトップの変換
	conv.Add(
//		SA("type", SS("statement")) >>
//		SA("child", SN(
			SA("type", SS("nodeStatement")) >>
			!(	
				SA("child", SS("DEF")) >>
				SA("child", SN(
					SA("type", SS("id")) >> 
					SA("child", S1)				
				))
			) >>
			SA("child", SN(
				SA("type", SS("node")) >>
				SA("child", S2) >>		//	S1: 型名
				SA("child", SS("{")) >>
//	S::AnyAttributes() (変数になれる「任意個の属性」) がサポートされたので、
				SA("child", SN(
					SA("type", SS("nodeBody")) >>
					S::AnyAttributes()[3]	//こう書ける
				)) >>

//	前はこう書くしかなかったが、これだと一つノードが余分に出来てしまう．(インデントが深くなってしまう．)
//				SA("child", S3) >>		//	S2:	nodeの中身

				SA("child", SS("}"))
		)),
		DA("NODE", DN(
			DA("NAME", D1) >> 
			DA("PROTO", D2) >> 
//			DA("NODE", D3)
			D::AnyAttributes()[3]
		))
	);
	
	//	ノードの中身の変換
	conv.Add(
		SA("child", SN(
			SA("type", SS("nodeBodyElement")) >>
			SA("child", S1) >>		//	S1: フィールド名
			SA("child", SN(
				SA("type", S2) >>	//	S2:	型名
				S::AnyAttributes()[3]
			))
		)),
		DA("FIELD", DN(
			DA("NAME", D1) >>
			DA("TYPE", D2) >>
			//DA("VALUE", D::AnyAttributes()[3])
			D::AnyAttributes()[3]
		))
	);
	//	ノードの中身の変換	SFVec3f用
	conv.Add(
		SA("type", SS("sfvec3fValue")) >>
		SA("child", SN(
			SA("type", SS("sffloatValue")) >>
			SA("child", S1)
		)) >>
		SA("child", SN(
			SA("type", SS("sffloatValue")) >>
			SA("child", S2)
		)) >>
		SA("child", SN(
			SA("type", SS("sffloatValue")) >>
			SA("child", S3)
		)),
		DA("TYPE", DS("sfvec3fValue")) >>
		DA("VALUE", D1) >>
		DA("VALUE", D2) >>
		DA("VALUE", D3)
	);

	//	MFほげほげの変換
	conv.Add(
//		SA("child", SN(
//			SA("type", SS("nodeBodyElement")) >>
			SA("child", S1) >>		//	S1: フィールド名
			SA("child", SN(
				SA("type", S2) >>	//	S2:	型名
				SA("child", SS("[")) >>
				//	属性に * Any() が使えれば
				//	* Any();
				//	ないので3要素専用
				S::AAS(SA("child", SS("]")))[3]
				>> SA("child", SS("]"))
//			))
		)),
		DA("FIELD", DN(
			DA("NAME", D1) >>
			DA("TYPE", D2) >>
			D::AnyAttributes()[3]
		))
	);


	//木の変換。
	typedef FIVrmlSpiritTreeTraits STT;
	typedef FIDocNodeTraits DTT;
	STT::TInAttribute root(info.trees.begin(), 0);
	DTT::TOutAttributes attrs;
	conv.Convert(
		attrs,
		root,
		STT::GetBegin(root),
		STT::GetEnd(root)
	);
	std::cout << "Print PT" << std::endl;
	PrintNode<STT>(std::cout, STT::GetNode(STT::GetValue(root)));
	std::cout << "Print DocNode" << std::endl;
	for(DTT::TOutAttributes::iterator it=attrs.begin(); it!= attrs.end(); ++it){
		for(unsigned i=0; i<it->nodes.size(); ++i){
			PrintNode<DTT>(std::cout, it->nodes[i]);
		}
	}

  return 0;
}
