#include <Object.h>
#include "GeoNode.h"
#include <strstream>
#include <float.h>
#include <penalty.h>

#include "His/HISpidar4Quad.h"
using namespace LVRM;

extern HISpidar4quad* spidar;

///////////////////////////////////////////////////////////////
// Class Member Funcs

//----------------------------------------------------------------------------
//	TGeoNode
TGeoNode::TGeoNode():vertices(posture){
	displayList = 0;
	solid = NULL;
	mass = 1.0f;
	fricBases.clear();		//(5/10追加)
}
TGeoNode::TGeoNode(const TGeoNode& n):
	posture(n.posture), displayList(n.displayList), mass(n.mass), vertices(posture){
	solid=NULL;
}
void TGeoNode::AddPenalty(Vec3f p, Vec3f a){
	penalty += p;
	Vec3f dap = a - ((posture*center) ^ p);
	angularPenalty += dap;
}
void TGeoNode::ConvertToForce(float dt){
	/*	T = 2PI sqrt(m/k)
		m/k = TT / 4PIPI
		k = 4PIPI/TT*m	*/
	static float T = dt*20;
	float Kp = (4*M_PI*M_PI)/(T*T);
	float Kd = Kp * 0.00002f;//Kp * 0.02f;
	if (_finite(mass) && mass != HUGE_VAL){
		Vec3f dPenalty = (penalty - lastPenalty)/dt;
		Vec3f dAngularPenalty = (angularPenalty - lastAngularPenalty)/dt;
		momentum += (penalty + Kd * dPenalty) * dt;
//		assert(-50 < momentum.norm() && momentum.norm() < 50);
		angularMomentum += 0.01f * (angularPenalty + Kd * dAngularPenalty) * dt;
//		angularMomentum *= 0.7f;
	}
}

void TGeoNode::UpdatePenalty(){
	lastPenalty = penalty;
	penalty = Vec3f();
	lastAngularPenalty = angularPenalty;
	angularPenalty = Vec3f();
}

void TGeoNode::AddFriction(float dt){//(5/9追加)
	float d = 0.5;
	for (vector<FricBase>::iterator itFb = fricBases.begin(); itFb != fricBases.end(); ++itFb){
		TPenaltyDetector& pc = *(itFb->first);
		Vec3f friction = itFb->second;
		Vec3f friPmom[2], damper[2];
		TGeoNode* g1 = (TGeoNode*)pc.obj[0]->ref;
		TGeoNode* g2 = (TGeoNode*)pc.obj[1]->ref;
		Vec3f gSpP[2];

		if (pc.state == TPenaltyDetector::KEEP && friction.norm() > 0/*|| pc.state == TPenaltyDetector::CREATED*/){
			gSpP[0] = g1->posture * (Vec3f&)(pc.sprPoint[0]);
			gSpP[1] = g2->posture * (Vec3f&)(pc.sprPoint[1]);
			friPmom[0] = (g1->momentum * friction.unit()) * friction.unit();
			friPmom[1] = (g2->momentum * friction.unit()) * friction.unit();
			damper[0] = -d * friPmom[0] / g1->mass;
			damper[1] = d * friPmom[1] / g2->mass;
			cout << g1->momentum.norm() << ", " << friction.norm();
			g1->momentum += (friction + damper[0]) * dt;
			g2->momentum += (-friction + damper[1]) * dt;
			assert(-50 < g1->momentum.norm() && g1->momentum.norm() < 50);
			assert(-50 < g2->momentum.norm() && g2->momentum.norm() < 50);
		}
/*			if (friPmom.size() < friction.size() * dt){
				friction = - friPmom / dt;
				gSpP[1] = gSpP[0] + friction;
				pc.sprPoint[1] = (Point)(g2->posture.inv() * gSpP[1]);
//				fric = - friPmom / dt;
			}
		}
		g1->momentum += friction * dt;
		g2->momentum -= friction * dt;*/
	}
	fricBases.clear();
	/*
	Vec3f dynFric;
	for (vector<FricBase>::iterator itFb = frictions.begin(); itFb != frictions.end(); ++itFb){
		dynFric = ((momentum - itFb.first.momentum) / dt * itFb.second.unit()) * itFb.unit();
		if(dynFric.size() < itFb.second.size()){
			gSpP[0] = itFb.first. - dynFric.size() * fric.unit();
			pc.sprPoint[1] = (Point)(g2->posture.inv() * gSpP[1]);
			fric = k * (gSpP[0] - gSpP[1]);
		}
		*/
}
void TGeoNode::AddGravity(Vec3f gravity, float dt){
	if (_finite(mass) && mass != HUGE_VAL){
//		cout << momentum.norm() << name << endl;
		momentum += mass * gravity * dt;
//		assert(-50 < momentum.norm() && momentum.norm() < 50);
	}
}
void TGeoNode::Add(TPenaltyDetector* pc, Vec3f fric){//(5/9追加)
	FricBase fb;

	fb.first = pc;
	fb.second = fric;
	fricBases.push_back(fb);
}
void TGeoNode::UpdatePosture(float dt){
	if (!solid) return;
	if (Name() != "spidar"){
		Vec3f angVel = AngVel();
		float av = angVel.norm();
		if (av > 1e-10){
			Affinef delta(angVel.norm(), angVel.unit());
			Vec3f wCenter = posture * center;
			Affinef afC, afCInv;
			afC.Pos() = wCenter;
			afCInv.Pos() = -wCenter;
			posture = afC * delta * afCInv * posture;
		}
		posture.Pos() += Vel() * dt;
	}else{
		Affinef pos;
		pos.Pos() = spidar->GetPos();
		posture = pos;
	}	
	solid->prev = solid->curr;
	solid->curr = Transform(posture);
}
void TGeoNode::Draw(){
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf( posture );	// 位置・姿勢を変更する。
	glCallList(displayList);
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
}
void TGeoNode::DrawGeometry(){
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf( posture );	// 位置・姿勢を変更する。
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
	afStack.push_back(Affinef());
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
		}
		curNode->polytopes.push_back(TPolytope(curNode->vertices));
		TFace oneFace;
		for(vector<int>::iterator it = v.begin(); it != v.end(); ++it){
			if (*it < 0){
				curNode->polytopes.back().push_back(oneFace);
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
