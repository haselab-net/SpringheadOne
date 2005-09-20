#include <Object.h>
#include "GeoNode.h"
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
//	fricBases.clear();		//(5/10�ǉ�)
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
		Vec3f mom;
		mom = (penalty + Kd * dPenalty) * dt;
		if (momentum.norm() > 20){
			assert(0);
		}
		angularMomentum += 0.01f * (angularPenalty + Kd * dAngularPenalty) * dt;
//		angularMomentum *= 0.7f;
	}
	lastPenalty = penalty;
	penalty = Vec3f();
	lastAngularPenalty = angularPenalty;
	angularPenalty = Vec3f();
}

void TGeoNode::AddGravity(Vec3f gravity, float dt){
	if (_finite(mass) && mass != HUGE_VAL){
//		cout << momentum.norm() << name << endl;
		momentum += mass * gravity * dt;
//		assert(-50 < momentum.norm() && momentum.norm() < 50);
	}
}
/*void TGeoNode::Add(TPenaltyDetector* pc, Vec3f fric){//(5/9�ǉ�)
	FricBase fb;

	fb.first = pc;
	fb.second = fric;
	fricBases.push_back(fb);
}*/
void TGeoNode::UpdatePosture(float dt){
	if (!solid) return;
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
	
	solid->prev = solid->curr;
	solid->curr = Transform(posture);
}
void TGeoNode::Draw(){
	glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )
	glMultMatrixf( posture );	// �ʒu�E�p����ύX����B
	glCallList(displayList);
	glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )
}
void TGeoNode::DrawGeometry(){
	glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )
	glMultMatrixf( posture );	// �ʒu�E�p����ύX����B
	for(TPolytopes::iterator ith = polytopes.begin(); ith != polytopes.end(); ++ith){
		for(TPolytope::iterator itf = ith->begin(); itf != ith->end(); ++itf){
			glBegin(GL_POLYGON);
			for(TFace::iterator itv = itf->begin(); itv!= itf->end(); ++itv){
				glVertex3dv(vertices[*itv]);
			}
			glEnd();
		}
	}
	glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )
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
	//	���O�̖������[�g�m�[�h�ɖ��O��t���Ă��܂��D
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

	//	���O�t��TNode����TGeoNode�����D
	for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
		TNode* pn = *it;
		pn->GetType()->EnumNamedNode(pn);
	}
	//	������TNode��e����؂藣���D
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
	//	DrawList�����D
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

	//	geometry�����o���D
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
