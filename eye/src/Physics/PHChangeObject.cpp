#include "Physics.h"
#pragma hdrstop
#include "PHChangeObject.h"

namespace Spr{;

//----------------------------------------------------------------------------
//	PHChangeObject
//
SGOBJECTIMPABST(PHChangeObject, SGObject);

void PHChangeObject::Step(){
	if(IsChange()) Change();
}

void PHChangeObject::Change(){	
	Vec3d tmpPos, tmpVel, tmpAngVel;
	Matrix3d tmpRot;
	// solid[0]�̏���ޔ�
	tmpPos = solid[0]->GetFramePosition();
	tmpRot = solid[0]->GetRotation();
	tmpVel = solid[0]->GetVelocity();
	tmpAngVel = solid[0]->GetAngularVelocity();
	// solid[0]��solid[1]�̈ʒu�ɃZ�b�g
	solid[0]->SetFramePosition(solid[1]->GetFramePosition());
	solid[0]->SetRotation(solid[1]->GetRotation());
	solid[0]->SetVelocity(solid[1]->GetVelocity());
	solid[0]->SetAngularVelocity(solid[1]->GetAngularVelocity());
	// solid[1]��solid[0]�̌������ʒu�ɃZ�b�g
	solid[1]->SetFramePosition(tmpPos);
	solid[1]->SetRotation(tmpRot);
	solid[1]->SetVelocity(tmpVel);
	solid[1]->SetAngularVelocity(tmpAngVel);
}


//----------------------------------------------------------------------------
//	PHChangeObjectCollision
//

SGOBJECTIMP(PHChangeObjectCollision, PHChangeObject);

void PHChangeObjectCollision::Clear(){
	listenerPos = -1;
	detectorFrame = NULL;
	solid[0] = NULL;
	solid[1] = NULL;
}

bool PHChangeObjectCollision::IsChange(){
	if(bCollision){
		bCollision = FALSE;		// ���̃X�e�b�v�ɔ����ă��Z�b�g
		return TRUE;
	}
	return FALSE;
}

bool PHChangeObjectCollision::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(SGFrame, o)){
		detectorFrame = (SGFrame*)o;
		return true;
	}
	if (DCAST(PHSolid, o)){
		if(detectorFrame) solid[solid[0] ? 1 : 0] = (PHSolid*)o;
		else detectorFrame = ((PHSolid*)o)->GetFrame();
		return true;
	}
	return false;
}

void PHChangeObjectCollision::Loaded(SGScene* scene){
	UTRef<CDCollisionEngine> ce;
	scene->GetBehaviors().Find(ce);
	// CDCollisionEngine ���Ȃ��ꍇ�͍��
	if (!ce){
		ce = new CDCollisionEngine;
		ce->SetName("theCollisionEngine", scene);
		scene->GetBehaviors().Add(ce);
	}

	// PHChangeObjectCollision��CDCollisionEngine�̃��X�i�ɓo�^����
	listenerPos = ce->AddListener(this);

	// detectorFrame��solid[0]�̑΂�CDCollisionEngine�̐ڐG���X�g�ɓo�^
	int f1, f2;
	ce->AddFrame(detectorFrame);
	ce->AddFrame(solid[0]->GetFrame());
	f1 = ce->GetFrameID(detectorFrame);
	f2 = ce->GetFrameID(solid[0]->GetFrame());
	ce->AddActive(f1, f2, listenerPos);
	
	//	������(frames ���� pairs�����)
	ce->Init();
}

void PHChangeObjectCollision::After(SGScene* scene, CDFramePairWithRecord* fp){
	bCollision = TRUE;
}

//----------------------------------------------------------------------------
//	PHChangeObjectOrientation
//
SGOBJECTIMP(PHChangeObjectOrientation, PHChangeObject);

void PHChangeObjectOrientation::Clear(){
	selectedAxis = Vec3d();
	comparativeOrientation = Matrix3d();
	targetedInnerProduct = Vec3d();
	accuracy = Vec3d();
	comparativeFrame = NULL;
}

bool PHChangeObjectOrientation::AddChildObject(SGObject* o, SGScene* s){
	// PHSolid��solid[0]��solid[1]��comparativeFrame�̏��ŃZ�b�g����
	if (DCAST(PHSolid, o)){
		if(solid[0]){
			if(solid[1]){
				comparativeFrame = ((PHSolid*)o)->GetFrame();
			} else {
				solid[1] = (PHSolid*)o;
			}
		} else {
			solid[0] = (PHSolid*)o;
		}
		return true;
	}
	// �t���[���̏ꍇ��comparativeFrame�ɃZ�b�g
	if (DCAST(SGFrame, o)){
		comparativeFrame = ((SGFrame*)o);
		return true;
	}
	return false;
}

bool PHChangeObjectOrientation::IsChange(){
	assert(solid[0]);
//	DSTR << solid[0]->GetRotation() << std::endl;
	if(solid[0]->GetFramePosition().norm() < 30){   // ���E�̊O�ɑޔ������Ă�����͔̂��肩��r��
		// ��r�p�t���[�����ݒ肳��Ă����炻�̃t���[���̎p�����r�p�p���s��ɃZ�b�g����		
		if(comparativeFrame) comparativeOrientation = comparativeFrame->GetRotation();

		// solid[0]�̎p���Ɣ�r�p�p���s��̓��ς��v�Z
		Matrix3d matInnerProduct;
		matInnerProduct = comparativeOrientation.trans() * solid[0]->GetRotation();
		
		bool flag = FALSE;
		if(selectedAxis.norm() > 0.9){	
			flag = TRUE;
			// ���ό��ʂ̍s��̑Ίp�������ڕW���ϒl+-���x�ɂ����܂��Ă��邩���f����
			if(selectedAxis.x) {
				if(matInnerProduct.xx < targetedInnerProduct.x - accuracy.x || matInnerProduct.xx > targetedInnerProduct.x + accuracy.x) flag = FALSE;
			}
			if(selectedAxis.y) {
				if(matInnerProduct.yy < targetedInnerProduct.y - accuracy.y || matInnerProduct.yy > targetedInnerProduct.y + accuracy.y) flag = FALSE;
			}
			if(selectedAxis.z) {
				if(matInnerProduct.zz < targetedInnerProduct.z - accuracy.z || matInnerProduct.zz > targetedInnerProduct.z + accuracy.z) flag = FALSE;
			}
		}
		return flag;
	}
	return FALSE;
}


//----------------------------------------------------------------------------
//	PHChangeObjectContainer
//
SGOBJECTIMP(PHChangeObjectContainer, SGBehaviorEngine);

bool PHChangeObjectContainer::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHChangeObject, o)){
		cos.push_back((PHChangeObject*)o);
		return true;
	}
	return false;	
}

void PHChangeObjectContainer::Step(SGScene* s){
	for(PHChangeObjects::iterator it = cos.begin(); it != cos.end(); ++it){
		(*it)->Step();
	}
}

void PHChangeObjectContainer::Loaded(SGScene* s){
	for(PHChangeObjects::iterator it = cos.begin(); it != cos.end(); ++it){
		(*it)->Loaded(s);
	}
}



// ---------------------------------------------------------------------------
// Loader, Saver

//----------------------------------------------------------------------------
//	PHChangeObjectCollisionLoader
//
class PHChangeObjectCollisionLoader:public FIObjectLoader<PHChangeObjectCollision>{
	virtual bool LoadData(FILoadScene* ctx, PHChangeObjectCollision* co){
		co->bCollision = FALSE;
		return true;
	}
};

//----------------------------------------------------------------------------
//	PHChangeObjectCollisionSaver
//
class PHChangeObjectCollisionSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHChangeObjectCollision"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg);
};
void PHChangeObjectCollisionSaver::Save(FISaveScene* ctx, SGObject* arg){
	PHChangeObjectCollision* co = (PHChangeObjectCollision*)arg;
	FIDocNodeBase* doc = ctx->CreateDocNode("ChangeObjectCollision", co);
	ctx->docs.back()->AddChild(doc);
	doc->AddChild(ctx->CreateDocNode("REF", co->detectorFrame));
	doc->AddChild(ctx->CreateDocNode("REF", co->solid[0]));
	doc->AddChild(ctx->CreateDocNode("REF", co->solid[1]));
}

//----------------------------------------------------------------------------
//	PHChangeObjectOrientationLoader
//
class PHChangeObjectOrientationLoader:public FIObjectLoader<PHChangeObjectOrientation>{
	virtual bool LoadData(FILoadScene* ctx, PHChangeObjectOrientation* co){
		PHChangeObjectOrientationInfo info;
		ctx->docs.Top()->GetWholeData(info);
		co->selectedAxis = info.selectedAxis;
		co->comparativeOrientation = info.comparativeOrientation;
		co->targetedInnerProduct = info.targetedInnerProduct;
		co->accuracy = info.accuracy;
		// ��r�p�p���s��𐳋K������
		co->comparativeOrientation.Ex().unitize();
		co->comparativeOrientation.Ey().unitize();
		co->comparativeOrientation.Ez().unitize();
/*
		DSTR << co->selectedAxis << std::endl;
		DSTR << co->comparativeOrientation << std::endl;
		DSTR << co->targetedInnerProduct << std::endl;
		DSTR << co->accuracy << std::endl;
*/
		return true;
	}
};

//----------------------------------------------------------------------------
//	PHChangeObjectOrientationSaver
//
class PHChangeObjectOrientationSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHChangeObjectOrientation"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg);
};
void PHChangeObjectOrientationSaver::Save(FISaveScene* ctx, SGObject* arg){
	PHChangeObjectOrientation* co = (PHChangeObjectOrientation*)arg;
	FIDocNodeBase* doc = ctx->CreateDocNode("ChangeObjectOrientation", co);
	ctx->docs.back()->AddChild(doc);
	PHChangeObjectOrientationInfo info;
	info.selectedAxis = co->selectedAxis;
	info.comparativeOrientation = co->comparativeOrientation;
	info.targetedInnerProduct = co->targetedInnerProduct;
	info.accuracy = co->accuracy;
	doc->SetWholeData(info);
	doc->AddChild(ctx->CreateDocNode("REF", co->solid[0]));
	doc->AddChild(ctx->CreateDocNode("REF", co->solid[1]));
}

//----------------------------------------------------------------------------
//	PHChangeObjectContainerLoader
//
class PHChangeObjectContainerLoader : public FIObjectLoader<PHChangeObjectContainer>{
	virtual bool LoadData(FILoadScene* ctx, PHChangeObjectContainer* coc){
		return true;
	}
};

//----------------------------------------------------------------------------
//	PHChangeObjectContainerSaver
//
class PHChangeObjectContainerSaver : public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHChangeObjectContainer"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHChangeObjectContainer* coc = (PHChangeObjectContainer*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("ChangeObjectContainer", coc);
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(PHChangeObjects::iterator it = coc->cos.begin(); it != coc->cos.end(); ++it){
			ctx->SaveRecursive(*it);
		}
		ctx->docs.pop_back();
	}
};

DEF_REGISTER_BOTH(PHChangeObjectCollision);
DEF_REGISTER_BOTH(PHChangeObjectOrientation);
DEF_REGISTER_BOTH(PHChangeObjectContainer);

};