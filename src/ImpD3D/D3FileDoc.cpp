#include "ImpD3D.h"
#pragma hdrstop
#include <sstream>

namespace Spr {;

void D3ProtoSaver::SaveImp(std::ostream& os, const FITypeDescDb::ProtoDesc& proto){
	os << "template " << proto.desc->GetTypeName() << " {" << std::endl;
	if (proto.desc->guid != WBGuid()){
		os << "\t<" << proto.desc->guid << ">" << std::endl;
	}
	for(unsigned i=0; i< proto.desc->GetComposit().size(); ++i){
		FITypeDesc::Field& field = proto.desc->GetComposit()[i];
		if (field.bReference) continue;
		os << "\t" << field.type->GetTypeName() << "\t";
		os << field.name;
		if (field.bVector){
			os << "[";
			if (field.lengthFieldName.length()){
				os << field.lengthFieldName;
			}else{
				os << field.length;
			}
			os << "]";
		}
		os << ";" << std::endl;
	}
	os << "\t[...]" << std::endl;
	os << "}" << std::endl;
}


D3FileDoc::D3FileDoc(){
	Init();
}
void D3FileDoc::RegisterFromTypeDB(){
	std::ostringstream os;
	D3ProtoSaver saver;
	os << "xof 0303txt 0032\n";
	saver.Save(os, "X");
	WXDXFile dxfile;
	WXCHECK( dxfile->RegisterTemplates((void*)os.str().c_str(), os.str().length()) );

	typedef FITypeDescDb::ProtoDesc Proto;
	for(unsigned i=0; i<FITypeDescDb::GetProtoDescs().size(); ++i){
		const Proto& proto = FITypeDescDb::GetProtoDescs()[i];
		if (proto.fileType.compare("X") == 0){
			RegisterType(proto.desc->GetTypeName(), &proto.desc->GetGuid());
		}
	}
}

void D3FileDoc::Init(){
	RegisterTemplates();
	RegisterFromTypeDB();

	RegisterType("Header", &WBGuid("3D82AB43-62DA-11cf-AB39-0020AF71E433"));
	RegisterType("Vector", &WBGuid("3D82AB5E-62DA-11cf-AB39-0020AF71E433"));
	RegisterType("Coords2d", &WBGuid("F6F23F44-7686-11cf-8F52-0040333594A3"));
	RegisterType("Matrix4x4", &WBGuid("F6F23F45-7686-11cf-8F52-0040333594A3"));
	RegisterType("ColorRGBA", &WBGuid("35FF44E0-6C7C-11cf-8F52-0040333594A3"));
	RegisterType("ColorRGB", &WBGuid("D3E16E81-7835-11cf-8F52-0040333594A3"));
	RegisterType("IndexedColor", &WBGuid("1630B820-7842-11cf-8F52-0040333594A3"));
	RegisterType("Boolean", &WBGuid("4885AE61-78E8-11cf-8F52-0040333594A3"));
	RegisterType("Boolean2d", &WBGuid("4885AE63-78E8-11cf-8F52-0040333594A3"));
	RegisterType("MaterialWrap", &WBGuid("4885AE60-78E8-11cf-8F52-0040333594A3"));
	RegisterType("TextureFilename", &WBGuid("A42790E1-7810-11cf-8F52-0040333594A3"));
	RegisterType("Material", &WBGuid("3D82AB4D-62DA-11cf-AB39-0020AF71E433"));
	RegisterType("MeshFace", &WBGuid("3D82AB5F-62DA-11cf-AB39-0020AF71E433"));
	RegisterType("MeshFaceWraps", &WBGuid("4885AE62-78E8-11cf-8F52-0040333594A3"));
	RegisterType("MeshTextureCoords", &WBGuid("F6F23F40-7686-11cf-8F52-0040333594A3"));
	RegisterType("MeshMaterialList", &WBGuid("F6F23F42-7686-11cf-8F52-0040333594A3"));
	RegisterType("MeshNormals", &WBGuid("F6F23F43-7686-11cf-8F52-0040333594A3"));
	RegisterType("MeshVertexColors", &WBGuid("1630B821-7842-11cf-8F52-0040333594A3"));
	RegisterType("Mesh", &WBGuid("3D82AB44-62DA-11cf-AB39-0020AF71E433"));
	RegisterType("FrameTransformMatrix", &WBGuid("F6F23F41-7686-11cf-8F52-0040333594A3"));
	RegisterType("Frame", &WBGuid("3D82AB46-62DA-11cf-AB39-0020AF71E433"));
	RegisterType("FloatKeys", &WBGuid("10DD46A9-775B-11cf-8F52-0040333594A3"));
	RegisterType("TimedFloatKeys", &WBGuid("F406B180-7B3B-11cf-8F52-0040333594A3"));
	RegisterType("AnimationKey", &WBGuid("10DD46A8-775B-11cf-8F52-0040333594A3"));
	RegisterType("AnimationOptions", &WBGuid("E2BF56C0-840F-11cf-8F52-0040333594A3"));
	RegisterType("Animation", &WBGuid("3D82AB4F-62DA-11cf-AB39-0020AF71E433"));
	RegisterType("AnimationSet", &WBGuid("3D82AB50-62DA-11cf-AB39-0020AF71E433"));

	//	新規テンプレート
	RegisterType("Light8", &WBGuid("8840e4ac-5c01-4e6a-9df9-880a45fb9e56"));
	RegisterType("Scene", &WBGuid("5B47A445-CE94-4ba6-8FE4-5CA6C46BB28C"));
	RegisterType("Matrix3x3", &WBGuid("D70232C1-EF74-42f8-B768-7083A2A939EB"));
	RegisterType("Solid", &WBGuid("A9C20358-EDEB-4c0b-97AB-37D330B6BFA4"));
	RegisterType("SolidContainer", &WBGuid("582E937C-3FED-4175-A0BF-295C6FAEA0CC"));
	RegisterType("Penalty", &WBGuid("01D5FF3C-6D41-4ea5-8E89-45F5C14EDA5F"));
	RegisterType("PenaltyEngine", &WBGuid("DE9F4F45-8631-4107-93FF-A2B8BEA3121B"));
	RegisterType("Warp", &WBGuid("407F10C7-155F-442d-B207-1DEAB18183EB"));
	RegisterType("WarpEngine", &WBGuid("EDC17F05-8D34-41f8-8722-98EBA71EB7A5"));	
	RegisterType("ChangeObjectCollision", &WBGuid("E8D8B69D-D72B-483d-8E52-2AFC4E360650"));	
	RegisterType("ChangeObjectOrientation", &WBGuid("8AD0F3DD-678C-4c4e-AE04-E249275F3DC1"));	
	RegisterType("ChangeObjectContainer", &WBGuid("7F49C507-EE60-45a9-8A10-86BA31ACE538"));

	RegisterType("Import", &WBGuid("DD0C497D-53C3-4888-A529-3BBFAB9D9216"));
	RegisterType("GravityEngine", &WBGuid("fa58a120-587e-4e15-a023-61cf996858e6"));
	RegisterType("Joint", &WBGuid("2da8b799-6d70-4645-8fe2-dbbd464d1908"));
	RegisterType("JointEngine", &WBGuid("5b229c08-eb84-4922-ae13-c22857694465"));
	RegisterType("PIDController", &WBGuid("969a34d7-6832-4f9b-8cca-09a5400c448a"));
	RegisterType("ContactEngine", &WBGuid("9F513C16-CFD9-4abc-B12A-72E497AD6586"));
	RegisterType("ContactInactive", &WBGuid("5617C081-D1CE-4231-A097-BECCBE29206B"));
	RegisterType("StickEngine", &WBGuid("C806993A-495F-46fc-9DB4-0F4FC786EE97"));
	RegisterType("Stick", &WBGuid("4F9A4676-4EF9-4a4d-A75B-5F5B68FA2616"));

	RegisterType("PhysicalMaterial", &WBGuid("F1FF6F03-8F0D-4a54-B05D-96CA937A1067"));
	RegisterType("SetPhysicalMaterial", &WBGuid("ACA4EFB4-B8DC-4272-B1B4-012825ADDB5D"));
	
	RegisterType("MeshForVisual", &WBGuid("251c1ec7-c1c1-4d00-97c9-09bc468a1147"));
	RegisterType("MeshForShape", &WBGuid("9299e06b-bc23-4f58-958d-83ff2dd7a143"));
	RegisterType("SphereForShape", &WBGuid("A6692DFA-A243-48b2-B0DD-D22723B0DABC"));
}


void D3FileDoc::RegisterType(UTString name, const GUID* guid){
	typeGuidMap.insert(std::make_pair(name, *guid));
	typeNameMap.insert(std::make_pair(*guid, name));
	typeGuids.push_back(*guid);
}
void D3FileDoc::Print(std::ostream& os) const{
	for(std::map<UTString, WBGuid, UTStringLess>::const_iterator it = typeGuidMap.begin(); it!=typeGuidMap.end(); ++it){	
		os << it->first.c_str() << " - " << it->second << std::endl;
	}
	for(std::map<WBGuid, UTString>::const_iterator it = typeNameMap.begin(); it!=typeNameMap.end(); ++it){	
		os << it->first << " - " << it->second.c_str() << std::endl;
	}
}
void D3FileDoc::RegisterTemplates(){
	static bool bDone;
	if (bDone) return;
	bDone = true;
	char* src = 
	"xof 0303txt 0032\n"
	"template Quaternion{\n"
	"	<2343C8CA-A54B-4cfe-A1AE-97C5E1294C4C>\n"
    "        float w;\n"
    "        float x;\n"
    "        float y;\n"
    "        float z;\n"
    "}\n"
	"template Scene {\n"
	" <5B47A445-CE94-4ba6-8FE4-5CA6C46BB28C>\n"
	" [...]\n"
	"}\n"
	"template Matrix3x3 {\n"
	" <D70232C1-EF74-42f8-B768-7083A2A939EB>\n"
	" array FLOAT matrix [9];"
	"}\n"
	"template Solid {\n"
	" <A9C20358-EDEB-4c0b-97AB-37D330B6BFA4>\n"
	" FLOAT mass;\n"
	" Matrix3x3 inertia;\n"
	" Vector velocity;\n"
	" Vector angularVelocity;\n"
	" Vector center;\n"
	" [Frame]\n"
	"}\n"
	"template SolidContainer {\n"
	" <582E937C-3FED-4175-A0BF-295C6FAEA0CC>\n"
	" [Solid]\n"
	"}\n"
	"template Penalty {\n"
	" <01D5FF3C-6D41-4ea5-8E89-45F5C14EDA5F>\n"
	" [Solid, Frame]\n"
	"}\n"
	"template PenaltyEngine {\n"
	" <DE9F4F45-8631-4107-93FF-A2B8BEA3121B>\n"
	" [Penalty]\n"
	"}\n"
	"template Warp {\n"
	" <407F10C7-155F-442d-B207-1DEAB18183EB>\n"
	" Vector position;\n"
	" Vector velocity;\n"
	" Vector angularVelocity;\n"
	" [Frame, Solid]\n"
	"}\n"
	"template WarpEngine {\n"
	" <EDC17F05-8D34-41f8-8722-98EBA71EB7A5>\n"
	" [Warp]\n"
	"}\n"
	"template ChangeObjectCollision {\n"
	" <E8D8B69D-D72B-483d-8E52-2AFC4E360650>\n"
	" [Frame, Solid]\n"
	"}\n"
	"template ChangeObjectOrientation {\n"
	" <8AD0F3DD-678C-4c4e-AE04-E249275F3DC1>\n"
	" Vector selectedAxis;\n"
	" Matrix3x3 comparativeOrientation;\n"
	" Vector targetedInnerProduct;\n"
	" Vector accuracy;\n"
	" [Frame, Solid]\n"
	"}\n"
	"template ChangeObjectContainer {\n"
	" <7F49C507-EE60-45a9-8A10-86BA31ACE538>\n"
	" [ChangeObjectCollision, ChangeObjectOrientation]\n"
	"}\n"
	
	"template Import {\n"
	" <DD0C497D-53C3-4888-A529-3BBFAB9D9216>\n"
	" STRING filename;\n"
	"}\n"
	"template GravityEngine {\n"
	" <fa58a120-587e-4e15-a023-61cf996858e6>\n"
	" Vector accel;\n"
	" [Solid]\n"
	"}\n"
	"template Joint {\n"
	"	<2da8b799-6d70-4645-8fe2-dbbd464d1908>\n"
	"	DWORD		nType;\n"					//関節種類
	"	Vector		v3fPositionParent;\n"
	"	Matrix3x3	m3fRotationParent;\n"
	"	Vector		v3fPositionChild;\n"		//子ノードから見た関節位置/姿勢
	"	Matrix3x3	m3fRotationChild;\n"
	"	FLOAT		fPosition;\n"				//変位
	"	FLOAT		fVelocity;\n"				//速度
	"	FLOAT		fMaxTorque;\n"				//最大トルク
	"	FLOAT		fMinPosition;\n"			//可動範囲
	"	FLOAT		fMaxPosition;\n"
	"	DWORD		nInputType;\n"
	"	FLOAT		fInput;\n"
	"	FLOAT		fPValue;\n"
	"	FLOAT		fIValue;\n"
	"	FLOAT		fDValue;\n"
	"	[...]\n"
	"}"
	"template JointEngine {\n"
	"	<5b229c08-eb84-4922-ae13-c22857694465>\n"
	"	[...]\n"
	"}\n"
	"template ContactEngine {\n"
	"	<9F513C16-CFD9-4abc-B12A-72E497AD6586>\n"
	"	[...]\n"
	"}\n"
	"template ContactInactive {\n"
	"	<5617C081-D1CE-4231-A097-BECCBE29206B>\n"
	"	DWORD nFrames;\n"
	"	array DWORD frames[nFrames];\n"
	"}\n"
	"template PhysicalMaterial {\n"
	"	<F1FF6F03-8F0D-4a54-B05D-96CA937A1067>\n"
	"	FLOAT reflexSpring;\n"
	"	FLOAT reflexDamper;\n"
	"	FLOAT frictionSpring;\n"
	"	FLOAT frictionDamper;\n"
	"	FLOAT staticFriction;\n"
	"	FLOAT dynamicFriction;\n"
	"}\n"
	"template SetPhysicalMaterial {\n"
	"	<ACA4EFB4-B8DC-4272-B1B4-012825ADDB5D>\n"
	"	[PhysicalMaterial]\n"
	"}\n"
	"template Stick {\n"
	"	<4F9A4676-4EF9-4a4d-A75B-5F5B68FA2616>\n"
	"	FLOAT force;\n"
	"	[Frame, Solid]\n"
	"}\n"
	"template StickEngine {\n"
	"	<C806993A-495F-46fc-9DB4-0F4FC786EE97>\n"
	"	[Stick]\n"
	"}\n"
	"template MeshForVisual {\n"
	"	<251c1ec7-c1c1-4d00-97c9-09bc468a1147>\n"
	"	DWORD nVertices;\n"
	"	array Vector vertices[nVertices];\n"
	"	DWORD nFaces;\n"
	"	array MeshFace faces[nFaces];\n"
	"	[...]\n"
	"}\n"
	"template MeshForShape {\n"
	"	<9299e06b-bc23-4f58-958d-83ff2dd7a143>\n"
	"	DWORD nVertices;\n"
	"	array Vector vertices[nVertices];\n"
	"	DWORD nFaces;\n"
	"	array MeshFace faces[nFaces];\n"
	"	[...]\n"
	"}\n"
	"template SphereForShape {\n"
	"	<A6692DFA-A243-48b2-B0DD-D22723B0DABC>\n"
	"	FLOAT Radius;\n"
	"	DWORD Slices;\n"
	"	DWORD Stacks;\n"
	"	[...]\n"
	"}\n"
	"template XSkinMeshHeader {\n"
	" <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>\n"
	" WORD nMaxSkinWeightsPerVertex;\n"
	" WORD nMaxSkinWeightsPerFace;\n"
	" WORD nBones;\n"
	"}\n"
	"template VertexDuplicationIndices {\n"
	" <b8d65549-d7c9-4995-89cf-53a9a8b031e3>\n"
	" DWORD nIndices;\n"
	" DWORD nOriginalVertices;\n"
	" array DWORD indices[nIndices];\n"
	"}\n"
	"template SkinWeights {\n"
	" <6f0d123b-bad2-4167-a0d0-80224f25fabb>\n"
	" STRING transformNodeName;\n"
	" DWORD nWeights;\n"
	" array DWORD vertexIndices[nWeights];\n"
	" array FLOAT weights[nWeights];\n"
	" Matrix4x4 matrixOffset;\n"
	"}\n";

	WXDXFile dxfile;
	WXCHECK( dxfile->RegisterTemplates(src, strlen(src)) );
}

bool D3FileDoc::Load(UTString fn){
	fileName = fn;
#ifdef USE_DXFILE
	enumObject=NULL;
	if (!WXCHECK( WXDXFile()->CreateEnumObject((void*)fileName.c_str(), DXFILELOAD_FROMFILE, &enumObject.Intf()) )){
		return false;
	}
	while(enumObject){
		IDirectXFileData* data=NULL;
		enumObject->GetNextDataObject(&data);
		if (!data) break;
		const GUID* typeGuid;
		data->GetType(&typeGuid);
		UTString tn = GetTypeName(typeGuid);
		docs.Push(new D3DocNode(data, NULL));
		docs.Top()->SetType(tn);
		LoadRecurse();
	}
	return true;
#else
	std::ifstream isX(fileName.c_str());
	FIPContext ctxX(&isX, &DSTR);
	ctxX.baseGDNode->ctx->nameKey = "name";
	ctxX.baseGDNode->ctx->typeKey = "type";
	ctxX.baseGDNode->ctx->refKey = "ref";

	ctxX.baseGDNode->ctx->db = new FITypeDescDb;
	ctxX.baseGDNode->ctx->db->SetPrefix("X");
	ctxX.baseGDNode->ctx->db->REG_FIELD(Vec4f);
	ctxX.baseGDNode->ctx->db->REG_FIELD(Vec3f);
	ctxX.baseGDNode->ctx->db->REG_FIELD(float);
	ctxX.baseGDNode->ctx->nameMap.clear();
//	ctxX.dumpReadNode = true;

	fileIo->Load(ctxX);					//	Xファイルをロード
	UTRef<FIGDNode> node = new FIGDNode();
	node->Children() = ctxX.dataStack.front();
	return node;
#endif
}
void D3FileDoc::LoadRecurse(){
	D3DocNode* doc = UTRef<D3DocNode>(docs.Top());
	IDirectXFileObject* next = NULL;
	while(doc->xfileLoad->GetNextObject(&next) == DXFILE_OK){
		WXINTF(DirectXFileData) fileData;
		WXINTF(DirectXFileDataReference) fileDataRef;
		WXINTF(DirectXFileBinary) fileBin;
		if (next->QueryInterface(fileData.InterfaceID(), (void**)&fileData.Intf()) == S_OK){
			const GUID* typeGuid;
			fileData->GetType(&typeGuid);
			UTString tn = GetTypeName(typeGuid);
			UTRef<D3DocNode> newDoc = new D3DocNode(fileData, NULL);
			newDoc->SetType(tn);
			doc->AddChild(newDoc);
			docs.Push(newDoc);
			LoadRecurse();
			docs.Pop();
		}else if (next->QueryInterface(fileDataRef.InterfaceID(), (void**)&fileDataRef.Intf()) == S_OK){
			UTRef<D3DocNode> newDoc = new D3DocNode(NULL, fileDataRef);
			doc->AddChild(newDoc);
		}else if (next->QueryInterface(fileBin.InterfaceID(), (void**)&fileBin.Intf()) == S_OK){
			UTRef<D3DocNode> newDoc = new D3DocNode(fileBin);
			doc->AddChild(newDoc);
		}
		next->Release();
	}
	for(int i=0; i<doc->NChildren(); ++i){
		D3DocNode* d = (D3DocNode*) doc->Child(i);
		if (d->xfileLoad){
			WXCHECK(doc->xfileLoad->AddDataObject(d->xfileLoad));
		}else if (d->xfileBin){
			GUID guid;
			d->xfileBin->GetId(&guid);
			const char* name = d->GetName().c_str();
			if (name && !name[0]) name = NULL;
			const char* mime = NULL;
			d->xfileBin->GetMimeType(&mime);
			DWORD len = 0;
			d->xfileBin->GetSize(&len);
			char* data = new char [len];
			DWORD read=0;
			d->xfileBin->Read(data, len, &read);
			WXCHECK( doc->xfileLoad->AddBinaryObject(name, &guid, mime, data, read) );
			delete data;
		}
	}
}
bool D3FileDoc::Save(UTString fn, FIDocNodeBase* docBase){
	fileName = fn;
	D3DocNode* doc = (D3DocNode*) docBase;
	saveObject = NULL;
	WXDXFile dxfile;
	WXCHECK(dxfile->CreateSaveObject(fileName.c_str(), DXFILEFORMAT_TEXT, &saveObject.Intf()));
	
	if(saveObject){
#if 0
		std::vector<const GUID*> templates;
		for(std::vector<WBGuid>::iterator it=typeGuids.begin(); it != typeGuids.end(); ++it){
			templates.push_back(&*it);
		}
		WXCHECK(saveObject->SaveTemplates(templates.size(), &*templates.begin()));
#endif
		docs.Push(doc);
		SaveRecurse();
		docs.Pop();
		saveObject->SaveData(doc->xfileSave);
//		for(FIDocNodes::iterator it=doc->Children().begin(); it != doc->Children().end(); ++it){
//			D3DocNode* di = (UTRef<D3DocNode>) *it;
//			saveObject->SaveData(di->xfileSave);
//		}
	}
	return true;
}
void D3FileDoc::SaveRecurse(){
	D3DocNode* doc = UTRef<D3DocNode>(docs.Top());
	WXINTF(DirectXFileData) xfileParent;
	D3DocNode* docParent = (D3DocNode*)doc->GetParent();
	if (docParent && docParent->xfileSave){
		xfileParent = docParent->xfileSave;
	}
	if (doc->IsReference()){	//	リファレンスの場合
		if (xfileParent){
			UTString n = doc->GetName();
			if(!n.length()){
				static int count;
				std::ostringstream(n) << "noname_error" << count;
			}
			WXCHECK( xfileParent->AddDataReference(n.c_str(), NULL) );
		}
	}else if (doc->xfileBin){	//	バイナリオブジェクトの場合
		if (xfileParent){
			GUID guid;
			doc->xfileBin->GetId(&guid);
			DWORD nameLen;
			doc->xfileBin->GetName(NULL, &nameLen);
			UTString nameStr;
			nameStr.resize(nameLen);
			doc->xfileBin->GetName(&*nameStr.begin(), &nameLen);
			const char* name = nameStr.c_str();
			if (name && !name[0]) name = NULL;
			const char* mime = NULL;
			doc->xfileBin->GetMimeType(&mime);
			DWORD len = 0;
			doc->xfileBin->GetSize(&len);
			char* data = new char [len];
			DWORD read=0;
			doc->xfileBin->Read(data, len, &read);
			WXCHECK( xfileParent->AddBinaryObject(name, &guid, mime, data, read) );
			delete data;
		}
	}else{							//	普通のデータだった場合
		if (doc->data.size()){		//	データが設定されている場合置き換える
			saveObject->CreateDataObject(GetTypeGuid(doc->GetType()), doc->GetName().c_str(), NULL,
				doc->data.size(), &*doc->data.begin(), &doc->xfileSave.Intf());
		}else if (doc->xfileLoad){	//	ロード時のドキュメントをそのまま利用
			DWORD len=0;
			void* data=NULL;
			doc->xfileLoad->GetData(NULL, &len, &data);
			saveObject->CreateDataObject(GetTypeGuid(doc->GetType()), doc->GetName().c_str(), NULL, len, len ? data : NULL, &doc->xfileSave.Intf());
		}else{						//	空のFileDataを作る．
				saveObject->CreateDataObject(GetTypeGuid(doc->GetType()), doc->GetName().c_str(), NULL, 0, NULL, &doc->xfileSave.Intf());
		}
		//	親ノードのFileDataにこのノードを登録
		if (xfileParent){
			WXCHECK(xfileParent->AddDataObject(doc->xfileSave));
		}
		//	子ノードの処理
		for(int i=0; i<doc->NChildren(); ++i){
			D3DocNode* d = (D3DocNode*)doc->Child(i);
			docs.Push(d);
			SaveRecurse();		//	子ノードをセーブ
			docs.Pop();
		}
	}
}


}