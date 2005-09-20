#include "GeoNode.h"
#include "global.h"
#include <Object.h>
#include <strstream>
#include <float.h>
#include <penalty.h>
using namespace LVRM;

///////////////////////////////////////////////////////////////
// Class Member Funcs

//----------------------------------------------------------------------------
//	TGeoNode

TGeoNode::TGeoNode():vertices(posture){
	displayList = 0;
	solid = NULL;
	mass = 1.0f;
//	fricBases.clear();		//(5/10追加)
}
TGeoNode::TGeoNode(const TGeoNode& n):
	posture(n.posture), displayList(n.displayList), mass(n.mass), vertices(posture){
	solid=NULL;
}
void TGeoNode::AddPenalty(Vec3d p, Vec3d a){
	penalty += p;
	Vec3d dap = a - ((posture*center) ^ p);
	angularPenalty += dap;
}
void TGeoNode::ConvertToForce(double dt){
	static double T = dt*20;
	double Kp = (M_PI / T) * (M_PI / T) * 5;
	double Kd = Kp * PEN_DAM_STF;

	if (_finite(mass) && mass != HUGE_VAL){
		Vec3d dPenalty = (penalty - lastPenalty)/dt;
		Vec3d dAngularPenalty = (angularPenalty - lastAngularPenalty)/dt;
		momentum += (penalty + Kd * dPenalty) * dt;
		Vec3d mom;
		mom = (penalty + Kd * dPenalty) * dt;
/*		if (momentum.norm() > 20){
			assert(0);
		}*/
		angularMomentum += (angularPenalty + Kd * dAngularPenalty) * dt;
	}
	lastPenalty = penalty;
	penalty = Vec3d();
	lastAngularPenalty = angularPenalty;
	angularPenalty = Vec3d();
}

void TGeoNode::AddGravity(Vec3d gravity, double dt){
	if (_finite(mass) && mass != HUGE_VAL){
//		cout << momentum.norm() << name << endl;
		momentum += mass * gravity * dt;
//		assert(-50 < momentum.norm() && momentum.norm() < 50);
	}
}
/*
void TGeoNode::Add(TPenaltyDetector* pc, Vec3d fric){//(5/9追加)
	FricBase fb;

	fb.first = pc;
	fb.second = fric;
	fricBases.push_back(fb);
}
*/
void TGeoNode::UpdatePosture(double dt){
	if (!solid) return;
	Vec3d angVel = AngVel();
	double av = angVel.norm();
	if (av > 1e-10){
		Affined delta(av*dt, angVel/av);
		Vec3d wCenter = posture * center;
		Affined afC, afCInv;
		afC.Pos() = wCenter;
		afCInv.Pos() = -wCenter;
		posture = afC * delta * afCInv * posture;
	}
	posture.Pos() += Vel() * dt;
	
	solid->prev = solid->curr;
	solid->curr = Transform(posture);
}

void TGeoNode::UpdateMotion(double dt){
	momentum += (force + friction) * dt;
	angularMomentum += (torque + angularFriction) * dt;
	force = Vec3d();
	torque = Vec3d();
	friction = Vec3d();
	angularFriction = Vec3d();
}

void TGeoNode::Draw(){
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixd( posture );	// 位置・姿勢を変更する。
	glCallList(displayList);
	if (vertices.size()){
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3d(1,1,1);
		double s = 0.1;
//		glVertex3dv(center - Vec3d(s,0,0));
//		glVertex3dv(center + Vec3d(s,0,0));
//		glVertex3dv(center - Vec3d(0,s,0));
//		glVertex3dv(center + Vec3d(0,s,0));
//		glVertex3dv(center - Vec3d(0,0,s));
//		glVertex3dv(center + Vec3d(0,0,s));
		glEnd();
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
}
void TGeoNode::DrawGeometry(){
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixd( posture );	// 位置・姿勢を変更する。
	for(TPolytopes::iterator ith = polytopes.begin(); ith != polytopes.end(); ++ith){
		for(TPolytope::iterator itf = ith->begin(); itf != ith->end(); ++itf){
			glBegin(GL_POLYGON);
			for(TFace::iterator itv = itf->begin(); itv!= itf->end(); ++itv){
				glVertex3dv(vertices[*itv]);
			}
			glEnd();
		}
	}
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
}

//----------------------------------------------------------------------------
//	TGeoNodeBuilder
TGeoNodeBuilder::TGeoNodeBuilder(){
	afStack.push_back(Affined());
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
	Affined& GetPosture = afStack.back();
	double		center[] = {0, 0, 0};
	double		translation[] = {0, 0, 0};
	double		rotation[] = {0, 0, 1, 0};
	double		scale[] = {1, 1, 1};
	double		scaleOrientation[] = {0, 0, 1, 0};
		
	CopyElm(pn, "center", center);
	CopyElm(pn, "translation", translation);
	CopyElm(pn, "rotation", rotation);
	CopyElm(pn, "scale", scale);
	CopyElm(pn, "scaleOrientation", scaleOrientation);
	
	GetPosture.Pos() += GetPosture.Rot() * (Vec3d&) translation;
	
	GetPosture.Pos() += GetPosture.Rot() * (Vec3d&) center;
	if(rotation[3]){
		Affined rot(rotation[3], (Vec3d&)rotation);
		GetPosture = GetPosture * rot;
	}
	
	if(scaleOrientation[3]){
		Affined rot(scaleOrientation[3], (Vec3d&)scaleOrientation);
		GetPosture = GetPosture * rot;
	}
	if(scale[0] != 1 || scale[1] != 1 || scale[2] != 1){
		Affined sc;
		sc.Ex() *= scale[0];
		sc.Ey() *= scale[1];
		sc.Ez() *= scale[2];
		GetPosture = GetPosture * sc;
	}
	if(scaleOrientation[3]){
		Affined rot(scaleOrientation[3], (Vec3d&)scaleOrientation);
		GetPosture = GetPosture * rot;
	}
	GetPosture.Pos() -= GetPosture.Rot() * (Vec3d&) center;
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
		CopyElm(pn, "skyColor", (double*)&clearColor);
	}else if (strcmp(pn->GetType()->GetName(), "IndexedFaceSet")==0){
		vector<int> v;
		vector<double> p;
		TSts* pSts = 0;
		PushbackElm(pn, "coordIndex", v);
		if(CopyElm(pn, "coord", &pSts)) PushbackElm(*pSts->begin(), "point", p);
		if(v.size() == 0){
			cerr << "IndexedFaceSet has invalid data. skipping..." << endl;
			return false;				// error
		}
		if(v.back() >= 0) v.push_back(-1);
		int idBase = curNode->vertices.size();
		for(vector<double>::iterator it = p.begin(); it != p.end(); it+=3){
			Vec3d v;
			v[0] = it[0];
			v[1] = it[1];
			v[2] = it[2];
			v = afStack.back() * v;
			curNode->vertices.push_back(v);
		}
		curNode->polytopes.push_back(TPolytope(curNode->vertices));
		TFace oneFace;
		Vec3d interior = CalcInterior(curNode->vertices);
		for(vector<int>::iterator id = v.begin(); id != v.end(); ++id){
			if (*id < 0){
				Vec3d A = curNode->vertices[oneFace[0]];
				Vec3d B = curNode->vertices[oneFace[1]];
				Vec3d C;

				for (TFace::iterator it=oneFace.begin()+2; it!=oneFace.end(); it++) {
					if (((curNode->vertices[*it]-A)^(B-A)).norm()) {
						C = curNode->vertices[*it];
						break;
					}
				}
				oneFace.normal = CalcNormal(C-A, B-A, interior-A);
				curNode->polytopes.back().push_back(oneFace);
				oneFace.clear();
			}else{
				oneFace.push_back(*id + idBase);
			}
		}
	}
	return true;
}
namespace LVRM{
	extern GLUquadricObj* qobj;
}
void TGeoNodeBuilder::Build(TSts* sts){
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
	//	DrawListを作る．
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricTexture(qobj, GL_TRUE);
	static int nDisplayList = 0;
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
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
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		curNode=*it;
		TNode* node = (*it)->VrmlNode();
		if (strcmp(node->GetType()->GetName(), "Transform") == 0){
			node->GetType()->Traverse(TType::EnumTypeNode, node);
		}else{
			node->GetType()->EnumTypeNode(node);
		}
	}
}
