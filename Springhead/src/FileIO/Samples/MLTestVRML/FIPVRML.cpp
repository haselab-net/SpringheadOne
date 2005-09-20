#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <FileIO/MetaLoader/FIGenParser.h>
#include <FileIO/MetaLoader/FIPNodeBin.h>
#include <FileIO/MetaLoader/FIPContext.h>
#include <FileIO/FIDocConverter.h>
#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include <Base/Affine.h>
#include <iostream>
#include <iomanip>
#include <fstream>
namespace Spr{
	extern void RegisterXLoader(FILoadScene* l);
}

using namespace Spr;
using namespace std;

void checkMemory(){
	if (!_CrtCheckMemory()){
		DSTR << "Heap Error\n";
	}
}


DEF_RECORD(VRMLTransform, {
	Vec3f center;
	Vec4f rotation;
	Vec3f scale;
	Vec4f scaleOrientation;
	Vec3f translation;
	Vec3f bboxCenter;
	Vec3f bboxSize;
})

DEF_RECORD(XFrame, {
})

DEF_RECORD(XFrameTransformMatrix, {
	Affinef frameMatrix;
})

void main(){
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag = tmpFlag | _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag( tmpFlag );
	
	ofstream file("TestVRML.out");
	cout << "### START ###" << endl;

	//------------------------------------------------------------------------	
	//	ローダーの初期化とロード
	//
	FIGenParser FileIO;
	//	UNTILをローダーに登録
	FileIO.RegisterType(new FIPNUntil("UNTIL"));
	std::ifstream isParser("VRML97.g");
	FIPContext ctxParser(&isParser, &file);
	//	ctxParser.dumpReadNode = true;	//	trueだとロードしているノードを出力
	cout << "### Loading Parser...";
	FileIO.RegisterType(new FIPNIDString("VRMLID", "_", "_-~^()@`"));
	FileIO.LoadParser(ctxParser);		//	ローダーをロード
	cout << " done. ###" << endl;
	//	file << "********** Loader ************" << std::endl;
	//	FileIO.Print(file);

	//------------------------------------------------------------------------	
	//	VRMLノードのプロトタイプをロード
	//
	std::ifstream isPs("protos.wrl");
	FIPContext ctxPs(&isPs, &file);
	cout << "### Loading PROTO nodes ...";
	FileIO.Load(ctxPs);					//	プロトタイプファイルをロード
	cout << " done. ###" << endl;
	//	file << "********** Loader ************" << std::endl;
	//	FileIO.Print(file);
	
	//------------------------------------------------------------------------	
	//	VRMLファイルをロード
	//
	//	std::ifstream isVrml("siodome.wrl");
	std::ifstream isVrml("test.wrl");
	FIPContext ctxVrml(&isVrml, &file);
	ctxVrml.baseGDNode->ctx->typeKey = "protoNode";
	ctxVrml.baseGDNode->ctx->nameKey = "nodeID";
	UTRef<FITypeDescDb> vrmlDb = new FITypeDescDb;
	ctxVrml.baseGDNode->ctx->db = vrmlDb;

	//	ctxVrml.dumpReadNode = true;
	cout << "### Loading VRML file...";
	FileIO.Load(ctxVrml);					//	VRMLファイルをロード
	cout << " done. ###" << endl;
	file << "VRML:" << endl;
	file << ctxVrml.dataStack;			//	結果を出力
	//	file << "*********** Loader (Final) ***********" << std::endl;
	//	FileIO.Print(file);

	//------------------------------------------------------------------------	
	//	ロードしたVRMLをD3Dに変換
	//
	//	変換の定義
	vrmlDb->SetPrefix("VRML");
	vrmlDb->REG_FIELD(Vec3f);
	vrmlDb->REG_FIELD(Vec4f);
	vrmlDb->REG_RECORD(VRMLTransform);
	vrmlDb->Print(std::cout);

	FIDocConverter conv;
	UTRef<FITypeDescDb> xDb = new FITypeDescDb;
	conv.SetTypeDb(xDb);
	xDb->SetPrefix("X");
	xDb->REG_FIELD(Affinef);
	xDb->REG_RECORD(XFrameTransformMatrix);
	xDb->REG_RECORD(XFrame);

	class ConvFrame: public FIDocConvBase{
	public:
		FIString GetType() const { return "Transform" ; }
		virtual void Pre(FIDocConverter* ctx){
			ctx->CreateNode("Frame");
			ctx->To()->SetName(ctx->From()->GetName());
			VRMLTransform t;
			ctx->From()->GetData(t);
			//	T x C x R x SR x S x -SR x -C
			Affinef T; T.Pos() = t.translation;
			Affinef C; C.Pos() = t.center;
			Affinef R = Affinef::Rot( t.rotation[3], t.rotation.sub_vector(PTM::TVecDim<3>()) );
			Affinef SR = Affinef::Rot( t.scaleOrientation[3], t.scaleOrientation.sub_vector(PTM::TVecDim<3>()) );
			Affinef S; S.Ex() *= t.scale.X(); S.Ey() *= t.scale.Y(); S.Ez() *= t.scale.Z();
			Affinef trans = T*C*R*SR*S*SR.inv()*C.inv();
			if (trans != Affinef()){
				ctx->CreateNode("FrameTransformMatrix");
				ctx->To()->SetData(trans);
				ctx->UpTo();
			}
		}
		virtual void Post(FIDocConverter* ctx){
			ctx->UpTo();
		}
	};
	conv.Register(new ConvFrame);

	//	変換
	for(int i=0; i<ctxVrml.dataStack.back().size(); ++i){
		FIGDNode* n = ctxVrml.dataStack.back()[i]->GetNode();
		conv.Convert(n);
		checkMemory();
	}
	//	結果の表示
//	conv.GetRoot()->Print(std::cout);
	
	Spr::UTRef<Spr::SGScene> scene = new SGScene(NULL);
	FILoadScene loader;
	RegisterXLoader(&loader);
	loader.Load(scene, conv.GetRoot());
	scene->world->Print(std::cout);
}
