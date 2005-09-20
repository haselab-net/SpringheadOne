#include <FileIO/MetaLoader/FIGenParser.h>
#include <FileIO/MetaLoader/FIPNodeBin.h>
#include <FileIO/MetaLoader/FIPContext.h>
#include <Base/Affine.h>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace Spr;
using namespace std;

DEF_RECORD(XMaterial, {
	Vec4f faceColor;
	float power;
	Vec3f specularColor;
	Vec3f emissiveColor;
})


void main(){
	ofstream file("xtest.out");
	cout << "### START ###" << endl;
	//	���[�_�[�̏������ƃ��[�h
	FIGenParser FileIO;
	//	UNTIL�����[�_�[�ɓo�^
	FileIO.RegisterType(new FIPNUntil("UNTIL"));
	std::ifstream isParser("DirectX.g");
	FIPContext ctxParser(&isParser, &file);
	//	ctxParser.dumpReadNode = true;	//	true���ƃ��[�h���Ă���m�[�h���o��
	cout << "### Loading Parser...";
	FileIO.LoadParser(ctxParser);		//	���[�_�[�����[�h
	cout << " done. ###" << endl;
//	file << "********** Loader ************" << std::endl;
//	FileIO.Print(file);
	
	std::ifstream isTemp("template.x");
	FIPContext ctxTemp(&isTemp, &file);
	cout << "Loading 'template.x' ...";
	FileIO.Load(ctxTemp);				//	X�t�@�C���̃e���v���[�g�����[�h
	cout << " done. ###" << endl;
//	file << "********** Loader (with Template) ************" << std::endl;
//	FileIO.Print(file);

	std::ifstream isTs("sample.x");
//	std::ifstream isTs("island.x");
	FIPContext ctxTs(&isTs, &file);
	ctxTs.baseGDNode->ctx->nameKey = "name";
	ctxTs.baseGDNode->ctx->typeKey = "type";
	ctxTs.baseGDNode->ctx->db = new FITypeDescDb;
	ctxTs.baseGDNode->ctx->db->SetPrefix("X");
	ctxTs.baseGDNode->ctx->db->REG_FIELD(Vec4f);
	ctxTs.baseGDNode->ctx->db->REG_FIELD(Vec3f);
	ctxTs.baseGDNode->ctx->db->REG_FIELD(float);
	ctxTs.baseGDNode->ctx->db->REG_RECORD(XMaterial);
//	ctxTs.dumpReadNode = true;
	cout << "### Loading DirectX file...";
	FileIO.Load(ctxTs);					//	X�t�@�C�������[�h
	cout << " done. ###" << endl;
//	file << "***********FINAL***********" << std::endl;
//	FileIO.Print(file);

	file << "DirectX:" << endl;
	UTRef<FIGDNode> node = new FIGDNode();
	node->Children() = ctxTs.dataStack.front();
	node->Print(file);
	FIDocNodeBase* docMate = node->Child(0)->FindChild("Material");
	Vec4f rgba;
	docMate->GetData(rgba, "faceColor");
	DSTR << rgba << std::endl;
}
