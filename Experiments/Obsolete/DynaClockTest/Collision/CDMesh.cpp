#include "Collision.h"
#pragma hdrstop

namespace Spr {;
/**	メッシュをドキュメントからロード
	現状では，
	・各メッシュは凸形状でなければならない．
	・1つのフレームの下に複数のメッシュを入れるのが良い．
	・フレームの階層があると，毎回変換をすることになる．
*/
class CDMeshLoader:public FIObjectLoader<CDMesh>{
public:
	virtual void LoadData(FILoadScene* ctx, UTRef<CDMesh>& mesh){
		//子ノードにMeshForShapeがある場合はロードしない
		for(int i = 0; i < ctx->docs.Top()->NChildren(); i++){
			if(ctx->docs.Top()->GetChild(i)->GetType().compare("MeshForVisual") == 0){
				DSTR << "ignoring visual mesh : " << ctx->docs.Top()->GetName();
				return;
			}
		}

		mesh = new CDMesh;
		int nVtxs = 0;
		ctx->docs.Top()->GetData(nVtxs, "nVertices");
		mesh->vertices.resize(nVtxs);
		ctx->docs.Top()->GetDataImp(&*mesh->vertices.begin(), sizeof(Vec3f)*nVtxs, "vertices");
		mesh->CreateTree();
	}
};
DEF_REGISTER_LOADER(CDMesh);

//------------------------------------------------------------------------
SGOBJECTIMP(CDMesh, CDGeometry);

void CDMesh::CalcBBox(Vec3f& bbMin, Vec3f& bbMax){
	for(std::vector<Vec3f>::iterator it = vertices.begin(); it != vertices.end(); ++it){
		bbMin.element_min(*it);
		bbMax.element_max(*it);
	}
}

bool CDMesh::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHPhysicalMaterial, o)){
		PHPhysicalMaterial* pm = (PHPhysicalMaterial*)o;
		for(CDGeometries::iterator it = conveces.begin(); it != conveces.end(); ++it){
			(*it)->pmaterial = pm;
		}
		return true;
	}
	return false;
}

void CDMesh::MergeVertices(){
	for(unsigned i=0; i<vertices.size(); ++i){
		for(unsigned j=i+1; j<vertices.size(); ++j){
			if ((vertices[i] - vertices[j]).square() < Square(1e-6)){
				vertices.erase(vertices.begin()+j);
				--j;
			}
		}
	}
}
void CDMesh::CreateTree(){
	MergeVertices();
	tvtxs.resize(vertices.size());
	CDPolyhedron* poly = new CDPolyhedron;
	conveces.push_back(poly);
	poly->vtxIDs.resize(vertices.size());
	for(unsigned int i=0; i<vertices.size(); ++i){
		poly->vtxIDs[i] = i;
	}
	poly->base = &*vertices.begin();
	poly->tvtxs = &*tvtxs.begin();
	poly->CalcFace();
};


int CDVertexIDs::FindPos(int id) const {
	const_iterator lower = begin();
	const_iterator upper = end();
	while(lower != upper){
		const_iterator middle = lower + (upper-lower)/2;
		if (*middle < id){
			lower = middle;
		}else if (*middle > id){
			upper = middle;
		}else{
			return middle - begin();
		}
	}
	return -1;
}

class CDQhullVtx{
public:
	static Vec3f* base;
	int vtxID;
	int VtxID() const { return vtxID; }
	Vec3f GetPos() const { return base[VtxID()]; }
	CDQHPlane<CDQhullVtx>* horizon;
};

Vec3f* CDQhullVtx::base;
void CDPolyhedron::CalcFace(){
	curPos = 0;
	faces.clear();
	neighbor.clear();
	
	std::vector<CDQhullVtx> vtxs;
	std::vector<CDQhullVtx*> pvtxs;
	vtxs.resize(vtxIDs.size());
	pvtxs.resize(vtxIDs.size());
	neighbor.resize(vtxIDs.size());
	for(unsigned int i=0; i<vtxIDs.size(); ++i){
		vtxs[i].vtxID = vtxIDs[i];
		pvtxs[i] = &vtxs[i];
	}
	CDQhullVtx::base = base;
	int n = vtxIDs.size();
//	CDQHPlanes<CDQhullVtx> planes(n*(n-1)*(n-2)/6);
	CDQHPlanes<CDQhullVtx> planes(n*10);
	planes.CreateConvexHull(&*pvtxs.begin(), &*pvtxs.end());
	std::set<int> usedVtxs;
	for(CDQHPlane<CDQhullVtx>* plane =planes.begin; plane != planes.end; ++plane){
		if (plane->deleted) continue;
		faces.push_back(CDFace());
		for(int i=0; i<3; ++i){
			faces.back().vtxs[i] = plane->vtx[i]->VtxID();
			usedVtxs.insert(plane->vtx[i]->VtxID());
		}
	}
	vtxIDs.clear();
	for(std::set<int>::iterator it = usedVtxs.begin(); it != usedVtxs.end(); ++it){
		vtxIDs.push_back(*it);
	}
	neighbor.resize(vtxIDs.size());
	for(CDFaces::iterator it = faces.begin(); it != faces.end(); ++it){
		for(int i=0; i<3; ++i){
			int pos = vtxIDs.FindPos(it->vtxs[i]);
			int next = vtxIDs.FindPos(it->vtxs[(i+1)%3]);
			neighbor[pos].push_back(next);
		}
	}
	MergeFace();
}

bool CDPolyhedron::VertexNear(int v1, int v2) const{
	if ((base[v1]-base[v2]).norm() < 1e-8) return true;
	return false;
}
void CDPolyhedron::MergeFace(){
	int nf = faces.size();
	for(unsigned int i=0; i<faces.size(); ++i){
		for(unsigned int j=i+1; j<faces.size(); ++j){
			CDFace& a = faces[i];
			CDFace& b = faces[j];
			Vec3f na =  ((base[a.vtxs[2]] - base[a.vtxs[0]]) ^ (base[a.vtxs[1]] - base[a.vtxs[0]])).unit();
			Vec3f nb =  ((base[b.vtxs[2]] - base[b.vtxs[0]]) ^ (base[b.vtxs[1]] - base[b.vtxs[0]])).unit();
			if (na*nb > 0.98f && abs(base[a.vtxs[0]]*na - base[b.vtxs[0]]*na) < 1e-5f){
				faces.erase(faces.begin() + i);
				i--;
				break;
			}
		}
	}
	//	DSTR << "Poly faces:" << nf << "->" << faces.size() << std::endl;
}


}
