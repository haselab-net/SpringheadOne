#include "Physics.h"
#pragma hdrstop

using namespace PTM;
namespace Spr{;

//////////////////////////////////////////////////////////
SGOBJECTIMP(PHConstraint, SGObject);
SGObject* PHConstraint::ChildObject(size_t i){
	return solid[i];
}

size_t PHConstraint::NChildObjects(){
	return 2;
}

bool PHConstraint::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		if(solid[0] == NULL)
			solid[0] = (PHSolid*)o;
		else if(solid[1] == NULL)
			solid[1] = (PHSolid*)o;
		else return false;
		return true;
	}
	return false;
}

bool PHConstraint::DelChildObject(SGObject* o, SGScene* s){
	/*PHSolid* s = DCAST(PHSolid, o);
	if(lhs == s){
		lhs = NULL;
		return true;
	}
	if(rhs == s){
		rhs = NULL;
		return true;
	}*/
	return false;
}

const UTTypeInfo** PHConstraint::ChildCandidates(){
	static const UTTypeInfo* rv[] = {NULL};
	return rv;
}

////////////////////////////////////////////////////////////////////

SGOBJECTIMP(PHConstraintEngine, SGBehaviorEngine);

void PHConstraintEngine::Loaded(SGScene* scene){
	//Solid�ɑΉ�����ConstrainedState�I�u�W�F�N�g���쐬���A���т���
	for(int i = 0; i < cons.size(); i++){
		for(int j = 0; j < 2; j++){
			for(int k = 0; k < objs.size(); k++){
				//���ɑΉ�����ConstrainedState�I�u�W�F�N�g�����݂����ꍇ
				if(states[k]->solid == cons[i]->solid[j]){
					cons[i]->state[j] = states[k];
					cons[i]->idx[j] = states[k]->nConstraints;
					states[k]->nConstraints++;
				}
				//�܂����������ꍇ
				else{
					PHConstrainedState* state = new PHConstrainedState;
					state->solid = cons[i]->solid[j];
					cons[i]->state[j] = state;
					cons[i]->idx[j] = 0;
					state->nConstraints = 1;
					states.push_back(state);
				}
			}
		}
	}
}

void PHConstraintEngine::Step(SGScene* s){
	//�S���������āA
	SolveConstraint();
	//Solid�ɔ��f
	UpdateState();
}

//�S���𖞂���Solid�̈ʒu�E�p�����v�Z
void PHConstraintEngine::SolveConstraint(){
	//��͓I��@�F
	
	//������@�F
	double error = 0.0;
	const double error_max = 1.0e-5;	//���e�S���덷
	const int itr_max = 20;				//�ő唽����
	while(itr_max--){
		//�e�S���ɂ��āA��S��Solid�̖ڕW�ʒu�ƍS���덷���v�Z
		for(int i = 0; i < cons.size(); i++){
			error += cons[i]->CompDesiredPosture();
		}
		//�I������
		if(error < error_max)
			return;
		//�ڕW�ʒu�ƍS���덷�Ɋ�Â���Solid�̈ʒu���v�Z
		for(int i = 0; i < states.size(); i++)
			states[i]->UpdatePosture();
	}
}

SGObject* PHConstraintEngine::ChildObject(size_t i){
	return cons[i];
}

size_t PHConstraintEngine::NChildObjects(){
	return cons.size();
}

bool PHConstraintEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHConstraint, o)){
		cons.push_back((PHConstraint*)o);
		return true;
	}
	return false;
}

bool PHConstraintEngine::DelChildObject(SGObject* o, SGScene* s){
	return false; 
}

const UTTypeInfo** PHConstraintEngine::ChildCandidates(){
	static const UTTypeInfo* rv[] = {
		PHConstraint::GetTypeInfoStatic(), NULL
	};
	return rv;
}

//////////////////////////////////////////////////////////////////////

class PHConstraintEngineLoader : public FIObjectLoader<PHConstraintEngine>{
public:
	void LoadData(FILoadScene* ctx, PHConstraintEngine* engine){
	}
};

class PHConstraintEngineSaver : public FIBaseSaver
{
public:
	UTString GetType() const{ return "PHConstraintEngine";}
	void Save(FISaveScene* ctx, SGObject* arg){
		PHConstraintEngine* ce = (PHConstraintEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("ConstraintEngine", ce);
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(unsigned i=0; i<ce->cons.size(); ++i){
			ctx->SaveRecursive(ce->cons[i]);
		}
		ctx->docs.pop_back();
	}
};

///////////////////////////////////////////////////////////////////////

class PHPointConstraintLoader : public FIObjectLoader<PHPointConstraint>{
	virtual void LoadData(FILoadScene* ctx, PHPointConstraint* con){
		ctx->docs.Top()->GetData(con->info);
	}
};

class PHPointConstraintSaver:public FIBaseSaver{
	virtual UTString GetType() const{return "PHPointConstraint";}
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHPointConstraint* con = (PHSolid*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("PointConstraint", con);
		ctx->docs.back()->AddChild(doc);
		doc->SetData(con->info);
		if(con->lhs)
			doc->AddChild(ctx->CreateDocNode("Solid", s->lhs, true));
		if(con->rhs)
			doc->AddChild(ctx->CreateDocNode("Solid", s->rhs, true));
	}
};

DEF_REGISTER_BOTH(PHPointConstraint);
DEF_REGISTER_BOTH(PHConstraintEngine);

}
