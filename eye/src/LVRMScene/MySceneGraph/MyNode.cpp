#include "MyNode.h"
#include <strstream>
using namespace Spr;

///////////////////////////////////////////////////////////////
// Class Member Funcs

static struct TDepthComp{
	bool operator () (const TMyNode* n1, const TMyNode* n2) const {
		return n1->depth < n2->depth;		
	}
}comp;
void TMyNodes::ZSort(const Affinef& afView){
	for(TMyNodes::iterator it = begin(); it != end(); ++it){
		(*it)->CalcZ(afView);
	}
	std::sort(begin(), end(), comp);
}
void TMyNodes::Draw(){
	for(TMyNodes::iterator it = begin(); it != end(); ++it){
		(*it)->Draw();
	}
}
//----------------------------------------------------------------------------
//	TMyNode
void TMyNode::CalcZ(const Affinef& afView){
	Affinef af = afView * posture;
	depth = af.Pos().Z() + radius;
}
void TMyNode::Draw(){
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf( posture );	// 位置・姿勢を変更する。
	glCallList(displayList);
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
}
void TMyNode::DrawGeometry(){
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
//	TMyNodeBuilder
TMyNodeBuilder::TMyNodeBuilder(){
	afStack.push_back(Affinef());
}
TMyNode* TMyNodeBuilder::CreateTMyNode(TNode* n){
	TField* f = n->GetField("nodeID");
	string name;
	if (f){
		name = (*f->begin())->GetString();
	}
	TMyNode* rv = new TMyNode;
	rv->Name(name);
	rv->SetPosture(afStack.back());
	return rv;
}
void TMyNodeBuilder::FoundTransformBefore(TNode *pn){
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
		Affinef rot = Affinef::Rot(rotation[3], (Vec3f&)rotation);
		GetPosture = GetPosture * rot;
	}
	
	if(scaleOrientation[3]){
		Affinef rot=Affinef::Rot(scaleOrientation[3], (Vec3f&)scaleOrientation);
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
		Affinef rot=Affinef::Rot(scaleOrientation[3], (Vec3f&)scaleOrientation);
		GetPosture = GetPosture * rot;
	}
	GetPosture.Pos() -= GetPosture.Rot() * (Vec3f&) center;
}
void TMyNodeBuilder::FoundTransformAfter(TNode *pn){
	afStack.pop_back();
}
void TMyNodeBuilder::FoundNamed(TNode *pn){
	if (!pn->GetParent()
		|| strcmp(pn->GetType()->GetName(), "Transform")==0
		|| strcmp(pn->GetType()->GetName(), "Group")==0){
		FoundTransformBefore(pn);
	
		TMyNode* node = CreateTMyNode(pn);
		node->vrmlNode = pn;
		allNodes.push_back(node);
		pn->GetType()->Traverse(TType::EnumNamedNode, pn);

		FoundTransformAfter(pn);
	}else{
		pn->GetType()->Traverse(TType::EnumNamedNode, pn);
	}
}
bool TMyNodeBuilder::FoundTypeNode(TNode *pn){
	if (strcmp(pn->GetType()->GetName(), "Background")==0){
		CopyElm(pn, "skyColor", (float*)&clearColor);
	}else if (strcmp(pn->GetType()->GetName(), "IndexedFaceSet")==0){
		vector<int> v;
		vector<float> p;
		TSts* pSts = 0;
		PushbackElm(pn, "coordIndex", v);
		if(CopyElm(pn, "coord", &pSts)) PushbackElm(*pSts->begin(), "point", p);
		if(v.size() == 0){
			cerr << "IndexedFaceSet has invalid data. skipping..." << endl;
			return false;				// error
		}
		if(v.back() >= 0) v.push_back(-1);
		int idBase = curNode->vertices.size();
//		curNode->bboxMin = Vec3f(1e20f, 1e20f, 1e20f); 
//		curNode->bboxMax = Vec3f(-1e20f, -1e20f, -1e20f); 
		curNode->radius = 0;
		for(vector<float>::iterator it = p.begin(); it != p.end(); it+=3){
			Vec3f v;
			v[0] = it[0];
			v[1] = it[1];
			v[2] = it[2];
			v = afStack.back() * v;
			curNode->vertices.push_back(v);
			curNode->radius = std::max(curNode->radius, v.norm());
//			curNode->bboxMin.element_min(v);
//			curNode->bboxMax.element_max(v);
		}
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
namespace Spr{
	extern GLUquadricObj* qobj;
}
void TMyNodeBuilder::Build(TSts* sts){
	if (!sts) return;
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

	//	名前付きTNodeからTMyNodeを作る．
	for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
		TNode* pn = *it;
		pn->GetType()->EnumNamedNode(pn);
	}
	//	見つけたTNodeを親から切り離す．
	for(TMyNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
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
	//	DrawListを作る．
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricTexture(qobj, GL_TRUE);
	static int nDisplayList = 0;
	for(TMyNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		glNewList(++nDisplayList, GL_COMPILE);
		TNode* node = (*it)->VrmlNode();
		if (strcmp(node->GetType()->GetName(), "Transform") == 0){
			node->GetType()->Traverse(TType::Draw, node);
		}else{
			node->GetType()->Draw(node);
		}
		glEndList();
		(*it)->displayList = nDisplayList;
	}
	gluDeleteQuadric(qobj);

	//	geometryを取り出す．
	TType::listener = this;
	for(TMyNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		curNode=*it;
		TNode* node = (*it)->VrmlNode();
		if (strcmp(node->GetType()->GetName(), "Transform") == 0){
			node->GetType()->Traverse(TType::EnumTypeNode, node);
		}else{
			node->GetType()->EnumTypeNode(node);
		}
	}
}
