#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <Collision/CDDetectorImp.h>
#include <Collision/CDMesh.h>
#include <Collision/CDPolyhedron.h>
#include <Collision/CDFramePair.h>
#include <Collision/CDSphere.h>
#include <strstream>
#include <fstream>
#include <iomanip>

using namespace Spr;
using namespace std;

///	計測セルの計測すべき壁面
enum MeasureWall{
	X_MINUS = 0x01,
	X_PLUS  = 0x02,
	Y_MINUS = 0x04,
	Y_PLUS  = 0x08,
	Z_MINUS = 0x10,
	Z_PLUS  = 0x20,
};

///	セル
struct Result{
	int nx, ny, nz;		//	セルの数．
	float cx, cy, cz;	//	セルの大きさ．
	float* volume;		//	体積
	unsigned char* measure;
	Vec3f* normal;
	Result():volume(0), measure(0), normal(0){}
	void Init(int x, int y, int z, float cxi, float cyi, float czi){
		nx = x;
		ny = y;
		nz = z;
		cx = cxi;
		cy = cyi;
		cz = czi;
		delete volume;
		volume = new float[nx*ny*nz];
		delete measure;
		measure = new unsigned char[nx*ny*nz];
		delete normal;
		normal = new Vec3f[nx*ny*nz];
		for(int i=0; i<nx*ny*nz; ++i){
			volume[i] = 0;
			measure[i] = 0;
		}
	}
	inline int index(int x, int y, int z){
		return x + ny*y + nx*ny*z;
	}
	float& Volume(int x, int y, int z){
		return volume[index(x,y,z)];
	}
	unsigned char& Measure(int x, int y, int z){
		return measure[index(x,y,z)];
	}
	Vec3f& Normal(int x, int y, int z){
		return normal[index(x,y,z)];
	}
};

///	セルに含まれる剛体の体積の計算
void CalcVolume(Result& r, SGScene* scene, Affinef afSolid, CDPolyhedron* polyCell);
///	計測点と壁面の計算
void FindMeasure(Result& r);
///	法線の計算(使わない)
void CalcNormal(Result& r, SGScene* scene, Affinef afSolid);


int __cdecl main(int argc, char* argv[]){
	//	引数の取得
	if (argc < 2){
		std::cout << "Usage:" << argv[0] << " filename.x [nx [cx]]" << std::endl;
		return -1;
	}
	int nx, ny, nz;
	nx = ny = nz = 10;
	if (argc >= 2){
		std::istrstream(argv[2]) >> nx;
		nz = ny = nx;
	}
	float cx, cy, cz;
	cx = cy = cz = 0.2f;
	if (argc >= 3){
		std::istrstream(argv[2]) >> cx;
		cz = cy = cx;
	}
	Result r;
	r.Init(nx, ny, nz, cx, cy, cz);

	//	ファイルのロード
	UTRef<FILoadScene> loader = new FILoadScene;
	REGISTER_LOADER(loader, SceneGraph);
	REGISTER_LOADER(loader, ImpD3D);
	REGISTER_LOADER(loader, Collision);
	D3FileDoc fileDoc;
	if (!fileDoc.Load(argv[1])){
		std::cout << "file " << argv[1] << " not found." << std::endl;
		return -1;
	}
	UTRef<SGScene> scene = new SGScene;
	loader->Load(scene, scene, &fileDoc);

	//	cellをあらわすためのPolyhedronの作成
	UTRef<CDMesh> meshCell = new CDMesh;
	meshCell->vertices.push_back(Vec3f(0,0,0));
	meshCell->vertices.push_back(Vec3f(r.cx,0,0));
	meshCell->vertices.push_back(Vec3f(0,r.cy,0));
	meshCell->vertices.push_back(Vec3f(0,0,r.cz));
	meshCell->vertices.push_back(Vec3f(0,r.cy,r.cz));
	meshCell->vertices.push_back(Vec3f(r.cx,0,r.cz));
	meshCell->vertices.push_back(Vec3f(r.cx,r.cy,0));
	meshCell->vertices.push_back(Vec3f(r.cx,r.cy,r.cz));
	meshCell->CreateTree();
	UTRef<CDPolyhedron> polyCell = UTRef<CDPolyhedron>(meshCell->conveces.front());
	
	//	セルと剛体の干渉計算．計測点の設定
	Affinef afSolid;
	CalcVolume(r, scene, afSolid, polyCell);
	FindMeasure(r);
	CalcNormal(r, scene, afSolid);
	
	//	結果の書き出し
	ostrstream fn;
	fn << argv[1] << ".txt" << '\0';
	ofstream of(fn.str());
	of << nx << " " << ny << " " << nz << endl;
	for(int z=0; z<nz; ++z){
		for(int y=0; y<ny; ++y){
			for(int x=0; x<nx; ++x){
				of << r.Volume(x,y,z) << " ";
				of << (int)r.Measure(x,y,z) << "  ";
			}
			of << endl;
		}
		of << endl;
	}

	//	結果の表示

	//	グラフィカルに計測点を表示するための文字
	//			 				 y+y+y+y+y+y+y+y+
	//					 y-y-y-y-        y-y-y-y-
	//			     x+x+    x+x+  
	//			   x-  x-  x-  x-
	char* wall = "  <  ><>__Ｌ_｣∪~~Γ￢Π〓⊂⊃□○<oo>■_oLoo｣■~o｢ooT■■■■■"
		        "×<xx>■_xLxx｣■~x｢xxT■■■■■■■■■■■■■■■■■■■■■";
	//	表示
	for(int z=0; z<r.nz; ++z){
		std::cout << "z = " << z << std::endl;
		for(int y=r.ny-1; y>=0; --y){
			std::cout << "y = " << setw(2) << y << " : ";
			for(int x=0; x<r.nx; ++x){
				if (r.Measure(x,y,z)){					//	計測点ならば true
					char* p = wall+2*r.Measure(x,y,z);	//	表示すべき文字を取り出す．
					std::cout << p[0] << p[1];
				}else{
					int v = r.Volume(x,y,z)*100;		//	セルのなかの剛体の割合
					if (v >= 100) std::cout << "00";	
					else std::cout << setw(2) << v;		//	2文字で表示
				}
				std::cout << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "----------------------------------------------------" << std::endl;
	}
	return 0;
}
	
void CalcVolume(Result& r, SGScene* scene, Affinef afSolid, CDPolyhedron* polyCell){
	CDContactAnalysis analyzer;
	for(int i=0; i<scene->GetWorld()->NChildObjects(); ++i){
		CDMesh* meshSolid = DCAST(CDMesh, scene->GetWorld()->ChildObject(i));
		if (meshSolid){
			for(int i=0; i<meshSolid->conveces.size(); ++i){
				CDPolyhedron* polySolid = DCAST(CDPolyhedron, meshSolid->conveces[i]);
				for(int x=0; x<r.nx; ++x){
					for(int y=0; y<r.ny; ++y){
						for(int z=0; z<r.nz; ++z){
							Vec3d sep;
							CDConvexPair cp;
							Affinef afCell;
							afCell.Pos() = Vec3f(x*r.cx-r.nx*r.cx/2, y*r.cy-r.ny*r.cy/2, z*r.cz-r.nz*r.cz/2);
							if (FindCommonPoint(*polySolid, *polyCell,
								afSolid, afCell, sep, cp.closestPoint[0], cp.closestPoint[1])){
								cp.convex[0] = polySolid;
								cp.convex[1] = polyCell;
								cp.commonPoint = afSolid * cp.closestPoint[0];
								Affinef af[2]={afSolid, afCell};
								analyzer.FindIntersection(cp, af);
								for(int i=0; i<analyzer.vtxs.size(); ++i){
									CDFace* face = analyzer.vtxs[i];
									if (face->NCommonVtx() == 0) continue;
									Vec3f p0 = face->CommonVtx(0);
									for(int i=0; i < face->NCommonVtx()-2; ++i){
										Vec3f p1 = face->CommonVtx(i+1);
										Vec3f p2 = face->CommonVtx(i+2);
										float s = ((p1-p0)^(p2-p0)).norm()/2;
										float v = s * face->dist / 3;
										r.Volume(x,y,z) += v / (r.cx*r.cy*r.cz);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void FindMeasure(Result& r){
	for(int x=1; x<r.nx-1; ++x){
		for(int y=1; y<r.ny-1; ++y){
			for(int z=1; z<r.nz-1; ++z){
				if (r.Volume(x,y,z) == 0){	//	このセルが物体と重なっておらず，
					char wall = 0;
					if (r.Volume(x-1,y,z)) wall |= X_MINUS;
					if (r.Volume(x+1,y,z)) wall |= X_PLUS;
					if (r.Volume(x,y-1,z)) wall |= Y_MINUS;
					if (r.Volume(x,y+1,z)) wall |= Y_PLUS;
					if (r.Volume(x,y,z-1)) wall |= Z_MINUS;
					if (r.Volume(x,y,z+1)) wall |= Z_PLUS;
					r.Measure(x,y,z) = wall;
				}
			}
		}
	}
}
void CalcNormal(Result& r, SGScene* scene, Affinef afSolid){
	CDSphere sphere;
	sphere.radius = 0;
	sphere.center = Vec3f(r.cx, r.cy, r.cz);
	for(int x=1; x<r.nx-1; ++x)
	for(int y=1; y<r.ny-1; ++y)
	for(int z=1; z<r.nz-1; ++z)
	if (r.Measure(x,y,z)){
		Affinef afCell;
		afCell.Pos() = Vec3f(x*r.cx-r.nx*r.cx/2, y*r.cy-r.ny*r.cy/2, z*r.cz-r.nz*r.cz/2);
		Vec3f(x*r.cx-r.nx*r.cx/2, y*r.cy-r.ny*r.cy/2, z*r.cz-r.nz*r.cz/2);
		Vec3f minNormal(1e5f, 1e5, 1e5);
		for(int i=0; i<scene->GetWorld()->NChildObjects(); ++i){
			CDMesh* meshSolid = DCAST(CDMesh, scene->GetWorld()->ChildObject(i));
			if (meshSolid){
				for(int i=0; i<meshSolid->conveces.size(); ++i){
					CDPolyhedron* polySolid = DCAST(CDPolyhedron, meshSolid->conveces[i]);
					Vec3d posCell, posSolid;
					FindClosestPoints(sphere, *polySolid, afCell, afSolid, posCell, posSolid);
					posCell = afCell * posCell;
					posSolid = afSolid * posSolid;
					Vec3f normal = posCell - posSolid;
					if (normal.square() < minNormal.square()){
						minNormal = normal;
					}
				}
			}
		}
		r.Normal(x,y,z) = minNormal.unit();
	}
}
