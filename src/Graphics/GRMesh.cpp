#include "Graphics.h"
#include <WinBasis/WBPath.h>
#pragma hdrstop

namespace Spr {;

//----------------------------------------------------------------------------
//	GRMesh
SGOBJECTIMP(GRMesh, SGObject);

bool GRMesh::AddChildObject(SGObject* obj, SGScene* s){
	if (DCAST(GRMaterial, obj)){
		GRMaterial* mat = (GRMaterial*)obj;
		materials.push_back(mat);
		return true;
	}
	return false;
}
size_t GRMesh::NChildObjects(){
	return materials.size();
}
SGObject* GRMesh::ChildObject(size_t i){
	return materials[i];
}

#pragma pack(push, 4)

DEF_RECORD(XMeshFace, {
	DWORD nFaceVertexIndices;
	FIVVector<DWORD> faceVertexIndices	VSIZE(nFaceVertexIndices);
})

DEF_RECORD(XMesh, {
	DWORD nVertices;
	FIVVector<Vec3f> vertices	VSIZE(nVertices);
	DWORD nFaces;
	FIVVector<XMeshFace> faces	VSIZE(nFaces);
})

DEF_RECORD(XMeshNormals, {
	DWORD nNormals;
	FIVVector<Vec3f> normals	VSIZE(nNormals);
	DWORD nFaceNormals;
	FIVVector<XMeshFace> faceNormals VSIZE(nFaceNormals);
})

DEF_RECORD(XMeshMaterialList, {
	DWORD nMaterials;
	DWORD nFaceIndexes;
	FIVVector<DWORD> faceIndexes	VSIZE(nFaceIndexes);
})

DEF_RECORD(XCoords2d, {
	float u;
	float v;
})

DEF_RECORD(XMeshTextureCoords, {
	DWORD nTextureCoords;
	FIVVector<XCoords2d> textureCoords	VSIZE(nTextureCoords);
})

#pragma pack(pop)

class XMeshLoadContext: public SGObject{
public:
	SGOBJECTDEF(XMeshLoadContext);
	XMeshNormals xnormals;
	GRMesh* mesh;					//	ロード中のメッシュ
	XMesh xmesh;					
	std::vector<int> nFaceTris;		//	1つの面が持つ3角形の数
	int nTri;						//	メッシュの3角形数
	XMeshLoadContext(GRMesh* m=NULL):mesh(m){}
};
SGOBJECTIMP(XMeshLoadContext, SGObject);


class GRMeshLoader:public FIObjectLoader<GRMesh>{
public:
	UTRef<FITypeDescDb> db;
	GRMeshLoader(){
		db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(DWORD);
		db->REG_FIELD(Vec3f);
		db->REG_RECORD(XMeshFace);
		db->REG_RECORD(XMesh);
	}
	virtual bool LoadData(FILoadScene* ctx, GRMesh* mesh){
		UTRef<XMeshLoadContext> ml = new XMeshLoadContext(mesh);
		ctx->objects.push_back(ml);
		FIDocNodeBase* doc = ctx->docs.Top();

		FITypeDesc* desc = db->Find("Mesh");
		desc->Read(&ml->xmesh, doc);
/*		DSTR << "Mesh:";
		for(int i=0; i<ml->xmesh.vertices.size(); ++i){
			DSTR << ml->xmesh.vertices[i];
		}
		DSTR << std::endl;
*/
		ml->mesh->normals.resize(ml->xmesh.nVertices);
		ml->mesh->vertices = ml->xmesh.vertices;
		ml->nTri = 0;
		for(unsigned int f=0; f<ml->xmesh.faces.size(); ++f){
			ml->nFaceTris.push_back(ml->xmesh.faces[f].nFaceVertexIndices - 2);
			ml->nTri += ml->nFaceTris.back();
		}
		return true;
	}
	virtual void Loaded(FILoadScene* ctx){
		UTRef<XMeshLoadContext> ml = ACAST(XMeshLoadContext, ctx->objects.back());
		ctx->objects.pop_back();

		//	法線情報を頂点に書き込む．必要に応じて頂点をコピーする．
		std::vector<bool> usedFlags;					//	使用済みのしるし
		usedFlags.resize(ml->xmesh.nVertices);
		for(unsigned i=0; i<usedFlags.size(); ++i) usedFlags[i]=false;
		//	使用済みフラグをチェックしながら，必要に応じて頂点をコピーする．
		for(unsigned int f=0; f<ml->xnormals.faceNormals.size(); ++f){
			XMeshFace& face = ml->xmesh.faces[f];
			XMeshFace& normalFace = ml->xnormals.faceNormals[f];
			for(unsigned int v=0; v<face.nFaceVertexIndices; ++v){
				unsigned long& vtxIdx = face.faceVertexIndices[v];
				unsigned long& normalIdx = normalFace.faceVertexIndices[v];
				if (vtxIdx<usedFlags.size() && !usedFlags[vtxIdx]){	//	初めてこの頂点を使うとき
					ml->mesh->normals[vtxIdx] = ml->xnormals.normals[normalIdx];
					usedFlags[vtxIdx] = true;
				}else if (ml->mesh->normals[vtxIdx] != ml->xnormals.normals[normalIdx]){
					//	すでに頂点に値の異なる法線が割り当てられているとき，頂点をコピーする
					//	法線をコピー
					ml->mesh->normals.push_back(ml->xnormals.normals[normalIdx]);
					//	頂点をコピー
					ml->mesh->vertices.push_back(ml->xmesh.vertices[vtxIdx]);
					//	テクスチャ座標をコピー
					if (ml->mesh->texCoords.size()) ml->mesh->texCoords.push_back(ml->mesh->texCoords[vtxIdx]);
					//	面の頂点IDを書き換え
					vtxIdx = ml->mesh->vertices.size()-1;
				}
			}
		}		
		//	左手系→右手系への変換
		for(unsigned i=0; i<ml->mesh->vertices.size(); ++i){
			ml->mesh->vertices[i].Z() *= -1;
			ml->mesh->normals[i].Z() *= -1;
		}

		//	インデックスバッファの確保
		ml->mesh->triangles.resize(ml->nTri*3);
		DWORD* indexBuf = &*ml->mesh->triangles.begin();
		//	インデックスの書き込み
		for(unsigned int f=0; f<ml->xmesh.faces.size(); ++f){
			XMeshFace& face = ml->xmesh.faces[f];
			for(unsigned int v=0; v < face.faceVertexIndices.size()-2; ++v){
				unsigned  vi[3] = {
					face.faceVertexIndices[0],
					face.faceVertexIndices[v+2],	//	左手系→右手系変換
					face.faceVertexIndices[v+1]
				};
				for(int i=0; i<3; ++i) *indexBuf++ = vi[i];
				//	法線が設定されていない頂点に法線を計算して設定する．
				for(int i=0; i<3; ++i){
					if (vi[i] < usedFlags.size() && !usedFlags[vi[i]]){
						Vec3f a = ml->mesh->vertices[vi[1]] - ml->mesh->vertices[vi[0]];
						Vec3f b = ml->mesh->vertices[vi[2]] - ml->mesh->vertices[vi[0]];
						Vec3f axb = a^b;
						Vec3f n = axb.square() ? (axb).unit() : Vec3f();
						ml->mesh->normals[vi[i]] += n;
					}
				}
			}
		}
		//	計算で作った法線を正規化
		for (unsigned i=0; i<ml->mesh->normals.size(); ++i){
			if (i < usedFlags.size() && !usedFlags[i]){
				ml->mesh->normals[i].unitize();
			}
		}
	}
};

class GRMeshSaver : public FIObjectSaver<GRMesh, true, false>
{
public:
	UTRef<FITypeDescDb> db;
	GRMeshSaver(){
		db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(DWORD);
		db->REG_FIELD(Vec3f);
		db->REG_RECORD(XMeshFace);
		db->REG_RECORD(XMesh);
		db->REG_RECORD(XMeshMaterialList);
		db->REG_RECORD(XMeshNormals);
	}
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, GRMesh* mesh){	
		//	Mesh本体のセーブ
		XMesh xmesh;
		xmesh.nVertices = mesh->vertices.size();
		xmesh.vertices = mesh->vertices;
		xmesh.nFaces = mesh->triangles.size()/3;
		for(unsigned i=0; i<xmesh.nFaces; ++i){
			XMeshFace face;
			face.nFaceVertexIndices = 3;
			face.faceVertexIndices.push_back(mesh->triangles[i*3]);
			//	右手系→左手系の変換
			face.faceVertexIndices.push_back(mesh->triangles[i*3+2]);
			face.faceVertexIndices.push_back(mesh->triangles[i*3+1]);
			xmesh.faces.push_back(face);
		}
		//	右手系→左手系の変換
		for(unsigned i=0; i<xmesh.vertices.size(); ++i) xmesh.vertices[i].Z() *= -1;
		FITypeDesc* descMesh = db->Find("Mesh");
		descMesh->Write(doc, &xmesh);

		//	MeshMaterialListのセーブ
		XMeshMaterialList xMatList;
		xMatList.nMaterials = mesh->materials.size();
		xMatList.nFaceIndexes = mesh->attributes.size();
		for(unsigned i=0; i<mesh->attributes.size(); ++i){
			xMatList.faceIndexes.push_back(mesh->attributes[i]);
		}
		UTRef<FIDocNodeBase> docMatList = ctx->CreateDocNode("MeshMaterialList");	
		FITypeDesc* descML = db->Find("MeshMaterialList");
		descML->Write(docMatList, &xMatList);
		doc->AddChild(docMatList);

		//	Materialのセーブ
		ctx->docs.Push(docMatList);
		for(unsigned i=0; i<mesh->materials.size(); ++i){
			if (!ctx->IsSavedObject(mesh->materials[i])){
				ctx->SaveRecursive(mesh->materials[i]);
			}else{
				ctx->SaveReference(mesh->materials[i]);
			}
		}
		ctx->docs.Pop();
		
		//	MeshNormals のセーブ
		XMeshNormals xnormals;
		xnormals.nNormals = mesh->normals.size();
		xnormals.normals.resize(mesh->normals.size());
		for(unsigned i=0; i<mesh->normals.size(); ++i) xnormals.normals[i] = mesh->normals[i];
		xnormals.nFaceNormals = mesh->triangles.size()/3;
		for(unsigned i=0; i<mesh->triangles.size()/3; ++i){
			XMeshFace face;
			face.nFaceVertexIndices = 3;
			face.faceVertexIndices.push_back(mesh->triangles[i*3]);
			//	右手系→左手系の変換
			face.faceVertexIndices.push_back(mesh->triangles[i*3+2]);
			face.faceVertexIndices.push_back(mesh->triangles[i*3+1]);
			xnormals.faceNormals.push_back(face);
		}
		//	右手系→左手系の変換
		for(unsigned i=0; i<xnormals.normals.size(); ++i){
			xnormals.normals[i].Z() *= -1;
		}

		FITypeDesc* descN = db->Find("MeshNormals");
		UTRef<FIDocNodeBase> docNormals = ctx->CreateDocNode("MeshNormals");
		descN->Write(docNormals, &xnormals);
		doc->AddChild(docNormals);
	}
};
DEF_REGISTER_BOTH(GRMesh);


class GRMeshNormalsLoader:public FIBaseLoader{
public:
	virtual UTString GetNodeType() const{
		return "MeshNormals";
	}
	UTRef<FITypeDescDb> db;
	GRMeshNormalsLoader(){
		db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(DWORD);
		db->REG_FIELD(Vec3f);
		db->REG_RECORD(XMeshFace);
		db->REG_RECORD(XMeshNormals);
	}
	virtual void Load(FILoadScene* ctx){
		if (ctx->docs.Top()->IsReference()) return;
		XMeshLoadContext* ml = NULL;
		ctx->objects.Find(ml);
		
		FITypeDesc*	desc = db->Find("MeshNormals");
		desc->Read(&ml->xnormals, ctx->docs.Top());
	}
};
DEF_REGISTER_LOADER(GRMeshNormals);

struct ColorRGB{
	float r, g, b;
};
struct ColorRGBA{
	float r, g, b, a;
};
struct XMaterial{
	ColorRGBA face;
	float power;
	ColorRGB specular;
	ColorRGB emissive;
};
class GRMaterialLoader:public FIObjectLoader<GRMaterial>{
	virtual bool LoadData(FILoadScene* ctx, GRMaterial* mat){
		FIDocNodeBase* docMat = ctx->docs.Top();
		XMaterial xmat;
		docMat->GetWholeData(xmat);
		mat->diffuse[0] = mat->ambient[0] = xmat.face.r;
		mat->diffuse[1] = mat->ambient[1] = xmat.face.g;
		mat->diffuse[2] = mat->ambient[2] = xmat.face.b;
		mat->diffuse[3] = mat->ambient[3] = xmat.face.a;
		mat->specular[0] = xmat.specular.r;
		mat->specular[1] = xmat.specular.g;
		mat->specular[2] = xmat.specular.b;
		mat->specular[3] = 1.0f;
		mat->emissive[0] = xmat.emissive.r;
		mat->emissive[1] = xmat.emissive.g;
		mat->emissive[2] = xmat.emissive.b;
		mat->emissive[3] = 1.0f;
		mat->power = xmat.power;
		for(int i=0; i<docMat->NChildren(); ++i){
			FIDocNodeBase* docTex = docMat->GetChild(i);
			if (docTex->GetType().compare("TextureFilename") == 0){
				const char* fn=NULL;
				docTex->GetWholeData(fn);
				WBPath xFilePath;
				xFilePath.Path(ctx->fileName);
				xFilePath.Path(xFilePath.FullPath());
				WBPath imPath;
				imPath.Path(fn);
				FIString oldCwd = imPath.GetCwd();
				imPath.SetCwd(xFilePath.Drive() + xFilePath.Dir());
				mat->textureFilename = imPath.FullPath();
			}
		}
		return true;
	}
};
class GRMaterialSaver:public FIObjectSaver<GRMaterial>{
	void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, GRMaterial* mat){
		XMaterial xmat;
		xmat.face.r = mat->diffuse[0];
		xmat.face.g = mat->diffuse[1];
		xmat.face.b = mat->diffuse[2];
		xmat.face.a = mat->diffuse[3];
		xmat.specular.r = mat->specular[0];
		xmat.specular.g = mat->specular[1];
		xmat.specular.b = mat->specular[2];
		xmat.emissive.r = mat->emissive[0];
		xmat.emissive.g = mat->emissive[1];
		xmat.emissive.b = mat->emissive[2];
		xmat.power = mat->power;
		doc->SetWholeData(xmat);
	}
};

DEF_REGISTER_BOTH(GRMaterial);

class GRMeshMaterialListLoader:public FIBaseLoader{
public:
	UTRef<FITypeDescDb> db;
	GRMeshMaterialListLoader(){
		db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(DWORD);
		db->REG_RECORD(XMeshMaterialList);
	}

	virtual UTString GetNodeType() const{
		return "MeshMaterialList";
	}
	virtual void Load(FILoadScene* ctx){
		//	データのロード
		FITypeDesc*	desc = db->Find("MeshMaterialList");
		XMeshMaterialList m;
		desc->Read(&m, ctx->docs.Top());
		//	データをメッシュに格納
		XMeshLoadContext* ml;
		ctx->objects.Find(ml);
		//	属性の設定
		for(unsigned int i=0, tri=0; i<m.nFaceIndexes; ++i){
			for(int j=0; j<ml->nFaceTris[i]; ++j){
				ml->mesh->attributes.push_back(m.faceIndexes[i]);
				tri++;
			}
		}
	}
};
DEF_REGISTER_LOADER(GRMeshMaterialList);


class GRMeshTextureCoordsLoader:public FIBaseLoader{
public:
	UTRef<FITypeDescDb> db;
	GRMeshTextureCoordsLoader(){
		db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(float);
		db->REG_FIELD(DWORD);
		db->REG_RECORD(XCoords2d);
		db->REG_RECORD(XMeshTextureCoords);
	}
	virtual UTString GetNodeType() const{
		return "MeshTextureCoords";
	}
	virtual void Load(FILoadScene* ctx){
		//	データのロード
		FITypeDesc* desc = db->Find("MeshTextureCoords");
		XMeshTextureCoords m;
		desc->Read(&m, ctx->docs.Top());
		//	データをメッシュに格納
		XMeshLoadContext* ml=NULL;
		ctx->objects.Find(ml);
		ml->mesh->texCoords.resize(m.textureCoords.size());
		for(unsigned i=0; i<m.textureCoords.size(); ++i){
			ml->mesh->texCoords[i].X() = m.textureCoords[i].u;
			ml->mesh->texCoords[i].Y() = m.textureCoords[i].v;
		}
	}
};

DEF_REGISTER_LOADER(GRMeshTextureCoords);

}
