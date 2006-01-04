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
	//�t�@�C��or�W�����͂���ǂݍ��ށB
  string text;
	if (argc>=2){
	  ifstream fs(argv[1]);
		getline(fs, text, '\0');
	}else{
		getline(cin, text, '\0');
	}

	//�p�[�X�B
	tree_parse_info<> info= FIVrmlParser::Parse(text);
  if (!info.full){
    cout << "Parse error." << endl;
    return 1;
  }
//	FIVrmlParser::PrintXml(info, text.c_str());
	
	//�؂̕ϊ��̏����B
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
//�����F S::AnyAttributes()[n], D::AnyAttributes()[n] �Łu�c��̑S���̑����v�Ƀ}�b�`����ϐ����ł��܂��B
	
	//	�v���g�^�C�v�錾�̃g�b�v�̕ϊ�
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

	//	���ʂ̃m�[�h�̃g�b�v�̕ϊ�
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
				SA("child", S2) >>		//	S1: �^��
				SA("child", SS("{")) >>
//	S::AnyAttributes() (�ϐ��ɂȂ��u�C�ӌ̑����v) ���T�|�[�g���ꂽ�̂ŁA
				SA("child", SN(
					SA("type", SS("nodeBody")) >>
					S::AnyAttributes()[3]	//����������
				)) >>

//	�O�͂������������Ȃ��������A���ꂾ�ƈ�m�[�h���]���ɏo���Ă��܂��D(�C���f���g���[���Ȃ��Ă��܂��D)
//				SA("child", S3) >>		//	S2:	node�̒��g

				SA("child", SS("}"))
		)),
		DA("NODE", DN(
			DA("NAME", D1) >> 
			DA("PROTO", D2) >> 
//			DA("NODE", D3)
			D::AnyAttributes()[3]
		))
	);
	
	//	�m�[�h�̒��g�̕ϊ�
	conv.Add(
		SA("child", SN(
			SA("type", SS("nodeBodyElement")) >>
			SA("child", S1) >>		//	S1: �t�B�[���h��
			SA("child", SN(
				SA("type", S2) >>	//	S2:	�^��
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
	//	�m�[�h�̒��g�̕ϊ�	SFVec3f�p
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

	//	MF�ق��ق��̕ϊ�
	conv.Add(
//		SA("child", SN(
//			SA("type", SS("nodeBodyElement")) >>
			SA("child", S1) >>		//	S1: �t�B�[���h��
			SA("child", SN(
				SA("type", S2) >>	//	S2:	�^��
				SA("child", SS("[")) >>
				//	������ * Any() ���g�����
				//	* Any();
				//	�Ȃ��̂�3�v�f��p
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


	//�؂̕ϊ��B
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
