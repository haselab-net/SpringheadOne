#include "GeoNode.h"
#include <strstream>
using namespace LVRM;

///////////////////////////////////////////////////////////////
// Class Member Funcs

//----------------------------------------------------------------------------
//	TGeoNode
void TGeoNode::Draw(){
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf( posture );	// 位置・姿勢を変更する。
	glCallList(displayList);
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
}
void TGeoNode::DrawGeometry(){
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf( posture );	// 位置・姿勢を変更する。
	for(TFaces::iterator itf = faces.begin(); itf != faces.end(); ++itf){
		glBegin(GL_POLYGON);
		for(vector<int>::iterator itv = itf->begin(); itv!= itf->end(); ++itv){
			glVertex3fv(vertices[*itv]);
		}
		glEnd();
	}
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
}

//----------------------------------------------------------------------------
//	TGeoNodeBuilder
TGeoNodeBuilder::TGeoNodeBuilder(){
	afStack.push_back(Affinef());
	bDontDisplay = false;
	countID = 0;
}
TGeoNode* TGeoNodeBuilder::CreateTGeoNode(TNode* n){
	TField* f = n->GetField("nodeID");
	string name;
	if (f){
		name = (*f->begin())->GetString();
	}
	TGeoNode* rv = new TGeoNode;
	rv->Name(name);
	rv->SetPosture(afStack.back());
	return rv;
}
void TGeoNodeBuilder::FoundTransformBefore(TNode *pn){
	afStack.push_back(afStack.back());
	Affinef& GetPosture = afStack.back();
	float		center[] = {0, 0, 0};
	float		translation[] = {0, 0, 0};
	float		rotation[] = {0, 0, 1, 0};
	float		scale[] = {1, 1, 1};
	float		scaleOrientation[] = {0, 0, 1, 0};
		
	CopyElm(pn, "center", center);
	CopyElm(pn, "translation", translation);
	CopyElm(pn, "rotation", rotation);
	CopyElm(pn, "scale", scale);
	CopyElm(pn, "scaleOrientation", scaleOrientation);
	
	GetPosture.Pos() += GetPosture.Rot() * (Vec3f&) translation;
	
	GetPosture.Pos() += GetPosture.Rot() * (Vec3f&) center;
	if(rotation[3]){
		Affinef rot(rotation[3], (Vec3f&)rotation);
		GetPosture = GetPosture * rot;
	}
	
	if(scaleOrientation[3]){
		Affinef rot(scaleOrientation[3], (Vec3f&)scaleOrientation);
		GetPosture = GetPosture * rot;
	}
	if(scale[0] != 1 || scale[1] != 1 || scale[2] != 1){
		Affinef sc;
		sc.Ex() *= scale[0];
		sc.Ey() *= scale[1];
		sc.Ez() *= scale[2];
		GetPosture = GetPosture * sc;
	}
	if(scaleOrientation[3]){
		Affinef rot(scaleOrientation[3], (Vec3f&)scaleOrientation);
		GetPosture = GetPosture * rot;
	}
	GetPosture.Pos() -= GetPosture.Rot() * (Vec3f&) center;
}
void TGeoNodeBuilder::FoundTransformAfter(TNode *pn){
	afStack.pop_back();
}
void TGeoNodeBuilder::FoundNamed(TNode *pn){
	if (!pn->GetParent()
		|| strcmp(pn->GetType()->GetName(), "Transform")==0
		|| strcmp(pn->GetType()->GetName(), "Group")==0){
		FoundTransformBefore(pn);
	
		TGeoNode* node = CreateTGeoNode(pn);
		node->vrmlNode = pn;
		allNodes.push_back(node);
		pn->GetType()->Traverse(TType::EnumNamedNode, pn);

		FoundTransformAfter(pn);
	}else{
		pn->GetType()->Traverse(TType::EnumNamedNode, pn);
	}
}
bool TGeoNodeBuilder::FoundTypeNode(TNode *pn){
	if (strcmp(pn->GetType()->GetName(), "Background")==0){
		CopyElm(pn, "skyColor", (float*)&clearColor);
	}else if (strcmp(pn->GetType()->GetName(), "IndexedFaceSet")==0){
		vector<int> v;
		vector<float> p;
/*		// Xの最大値、最小値
		float maxX = -999;
		float minX = 999;
		// Zの最大値、最小値
		float maxZ = -999;
		float minZ = 999;*/
		TSts* pSts = 0;
		PushbackElm(pn, "coordIndex", v);
		if(CopyElm(pn, "coord", &pSts)) PushbackElm(*pSts->begin(), "point", p);
		if(v.size() == 0){
			cerr << "IndexedFaceSet has invalid data. skipping..." << endl;
			return false;				// error
		}
		if(v.back() >= 0) v.push_back(-1);
		int idBase = curNode->vertices.size();
		for(vector<float>::iterator it = p.begin(); it != p.end(); it+=3){
			Vec3f v;
			v[0] = it[0];
			v[1] = it[1];
			v[2] = it[2];
			v = afStack.back() * v;
			curNode->vertices.push_back(v);
	/*		if(maxX < v[0]){
				maxX = v[0];
			}
			if(minX > v[0]){
				minX = v[0];
			}
			if(maxZ < v[3]){
				maxZ = v[3];
			}
			if(minZ > v[3]){
				minZ = v[3];
			}*/
		}
		/*Affinef afGlobalPosture = curNode->GetGlobalPosture();
		curNode->VecObjectBox[0] = afGlobalPosture * Vec4f(minX, 0, maxZ, 1);
		curNode->VecObjectBox[1] = afGlobalPosture * Vec4f(minX, 0, minZ, 1);
		curNode->VecObjectBox[2] = afGlobalPosture * Vec4f(maxX, 0, minZ, 1);
		curNode->VecObjectBox[3] = afGlobalPosture * Vec4f(maxX, 0, maxZ, 1);
		for (int m = 0; m < 4; m++)
			cout << "VecObjectBox[" << m << "] = " << curNode->VecObjectBox[m] << endl;
		cout << endl;*/
		vector<int> oneFace;
		for(vector<int>::iterator it = v.begin(); it != v.end(); ++it){
			if (*it < 0){
				curNode->faces.push_back(oneFace);
				oneFace.clear();
			}else{
				oneFace.push_back(*it + idBase);
			}
		}
	}
	return true;
}
namespace LVRM{
	extern GLUquadricObj* qobj;
}
void TGeoNodeBuilder::DontDisplay(){
	bDontDisplay = true;
}
void TGeoNodeBuilder::Build(TSts* sts){
	if (!sts) return;
	for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
		TNode* pn = (*it);
		FoundTransformBefore(pn);
		TGeoNode* node = CreateTGeoNode(pn);
		node->vrmlNode = pn;
		allNodes.push_back(node);
		FoundTransformAfter(pn);
	}
#if 0
	TType::listener = this;
	//	名前の無いルートノードに名前を付けてしまう．
	int num=0;
	for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
		TField* f = (*it)->GetField("nodeID");
		if (!f){
			char name[1024];
			ostrstream(name, sizeof(name)) << "noname_root_" << num << '\0';
			TField f;
			UTRef<TFieldBase> str = new TFieldBaseString(name);
			f.push_back(str);
			(*it)->SetElm("nodeID", &f);
			num ++;
		}
	}	

	//	名前付きTNodeからTGeoNodeを作る．
	for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
		TNode* pn = *it;
		pn->GetType()->EnumNamedNode(pn);
	}

	//	見つけたTNodeを親から切り離す．
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		TNode* cur = (*it)->VrmlNode();
		TNode* parent = cur->GetParent();
		if (parent){
			TField* field = parent->GetField("children");
			if (field){
				for(TField::iterator it = field->begin(); it != field->end(); ++it){
					TSts* sts = (*it)->GetSts();
					for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
						if (*it == cur){
							it = sts->erase(it);
						break;
						}
					}
				}
			}
		}
	}
#endif
	//	DrawListを作る．
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricTexture(qobj, GL_TRUE);
	static int nDisplayList = 0;
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		glNewList(++nDisplayList, GL_COMPILE);
		TNode* node = (*it)->VrmlNode();
		if (!bDontDisplay){
			if (strcmp(node->GetType()->GetName(), "Transform") == 0){
				node->GetType()->Traverse(TType::Draw, node);
			}else{
				node->GetType()->Draw(node);
			}
		}
		glEndList();
		(*it)->displayList = nDisplayList;
	}
	gluDeleteQuadric(qobj);

	//	geometryを取り出す．
	TType::listener = this;
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		curNode=*it;
		TNode* node = (*it)->VrmlNode();
		if (strcmp(node->GetType()->GetName(), "Transform") == 0){
			node->GetType()->Traverse(TType::EnumTypeNode, node);
		}else{
			node->GetType()->EnumTypeNode(node);
		}
	}
	///  バウンディングボックスの作成
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		// Xの最大値、最小値
		float maxX, minX;
		// Zの最大値、最小値
		float maxZ, minZ;
		// Yの最大値
		float maxY, minY;

		for(int i = 0; i < (*it)->vertices.size(); i++){
			if(i){
				if(maxX < (*it)->vertices[i].X())
					maxX = (*it)->vertices[i].X();
				if(minX > (*it)->vertices[i].X())
					minX = (*it)->vertices[i].X();
				if(maxZ < (*it)->vertices[i].Z())
					maxZ = (*it)->vertices[i].Z();
				if(minZ > (*it)->vertices[i].Z())
					minZ = (*it)->vertices[i].Z();
				if(maxY < (*it)->vertices[i].Y())
					maxY = (*it)->vertices[i].Y();
				if(minY > (*it)->vertices[i].Y())
					minY = (*it)->vertices[i].Y();
			}else{
				maxX = (*it)->vertices[i].X();
				minX = (*it)->vertices[i].X();
				maxZ = (*it)->vertices[i].Z();
				minZ = (*it)->vertices[i].Z();
				maxY = (*it)->vertices[i].Y();
				minY = (*it)->vertices[i].Y();
			}
		}
		Affinef afGlobalPosture = (*it)->GetGlobalPosture();
		(*it)->VecObjectBox[0] = afGlobalPosture * Vec4f(minX, maxY, maxZ, 1);
		(*it)->VecObjectBox[1] = afGlobalPosture * Vec4f(minX, maxY, minZ, 1);
		(*it)->VecObjectBox[2] = afGlobalPosture * Vec4f(maxX, maxY, minZ, 1);
		(*it)->VecObjectBox[3] = afGlobalPosture * Vec4f(maxX, maxY, maxZ, 1);
		(*it)->VecObjectBox[4] = afGlobalPosture * Vec4f(minX, minY, maxZ, 1);
		(*it)->VecObjectBox[5] = afGlobalPosture * Vec4f(minX, minY, minZ, 1);
		(*it)->VecObjectBox[6] = afGlobalPosture * Vec4f(maxX, minY, minZ, 1);
		(*it)->VecObjectBox[7] = afGlobalPosture * Vec4f(maxX, minY, maxZ, 1);
/*		(*it)->VecViewBox[0] = afGlobalPosture * Vec4f(maxX, maxY, maxZ, 1);
		(*it)->VecViewBox[1] = afGlobalPosture * Vec4f(minX, maxY, maxZ, 1);
		(*it)->VecViewBox[2] = afGlobalPosture * Vec4f(minX, minY, maxZ, 1);
		(*it)->VecViewBox[3] = afGlobalPosture * Vec4f(maxX, minY, maxZ, 1);
		(*it)->VecViewBox[4] = afGlobalPosture * Vec4f(maxX, maxY, minZ, 1);
		(*it)->VecViewBox[5] = afGlobalPosture * Vec4f(minX, maxY, minZ, 1);
		(*it)->VecViewBox[6] = afGlobalPosture * Vec4f(minX, minY, minZ, 1);
		(*it)->VecViewBox[7] = afGlobalPosture * Vec4f(maxX, minY, minZ, 1);*/

	
		// バウンディングボックスの中心を求める
		(*it)->VecCenterOfBox.X() = ((*it)->VecObjectBox[0].X() + (*it)->VecObjectBox[2].X()) / 2;
		(*it)->VecCenterOfBox.Z() = ((*it)->VecObjectBox[0].Z() + (*it)->VecObjectBox[2].Z()) / 2;
		(*it)->VecCenterOfBox.Y() = 0;
		(*it)->VecCenterOfBox.W() = 1;

		(*it)->objectID = countID;
		countID++;
	}
}
