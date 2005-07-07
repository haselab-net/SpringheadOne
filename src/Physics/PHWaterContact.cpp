#include "Physics.h"
#pragma hdrstop
#include <Physics/PHWater.h>
#include <Physics/PHWaterContact.h>
#include <Collision/CDMesh.h>

namespace Spr{;

//----------------------------------------------------------------------------
//PHWSolid
void PHWSolid::Init(){
	frame = solid->GetFrame();
	//SGFrame::contents����CDGeometry�̔h���N���X�I�u�W�F�N�g��T��
	EnumGeometries(frame);
}

void PHWSolid::EnumGeometries(SGFrame* f){
	CDMesh* g;
	for(SGObjects::iterator ic = f->contents.begin(); ic != f->contents.end(); ic++){
		g = DCAST(CDMesh, *ic);
		if(g){
			geometries.push_back(new PHWGeometry);
			geometries.back()->Set(f, g);
		}
	}
	for(SGFrames::iterator i = f->Children().begin(); i != f->Children().end(); i++)
		EnumGeometries(*i);
}

//----------------------------------------------------------------------------
//PHWGeometry
void PHWGeometry::Set(SGFrame* f, CDMesh* g){
	frame = f;
	conveces.resize(g->conveces.size());
	std::copy(g->conveces.begin(), g->conveces.end(), conveces.begin());
	//CDFace::CalcDualVtx���ĂԁB�b��B
	for(CDGeometries::iterator ic = conveces.begin(); ic != conveces.end(); ic++){
		CDPolyhedron* poly = DCAST(CDPolyhedron, *ic);
		if(!poly)continue;
		for(CDFaces::iterator iface = poly->faces.begin(); iface != poly->faces.end(); iface++)
			iface->CalcDualVtx(poly->tvtxs);
	}
}


//----------------------------------------------------------------------------
//	PHWaterContactEngine
//
SGOBJECTIMP(PHWaterContactEngine, SGBehaviorEngine);

PHWaterContactEngine::PHWaterContactEngine(){

}
void PHWaterContactEngine::LoadState(const SGBehaviorStates& states){
}
void PHWaterContactEngine::SaveState(SGBehaviorStates& states) const{
}

bool PHWaterContactEngine::AddChildObject(SGObject* o, SGScene* scene){
	if(DCAST(PHSolid, o)){
		solids.push_back(new PHWSolid);
		solids.back()->solid = DCAST(PHSolid, o);
		return true;
	}
	if(DCAST(PHWater, o)){
		water = DCAST(PHWater, o);
		return true;
	}
	return false;
}

void PHWaterContactEngine::Clear(SGScene* s){
	water = 0;
	solids.clear();
}
void PHWaterContactEngine::Loaded(SGScene* scene){
	Init(scene);
}

void PHWaterContactEngine::Init(SGScene* scene){
	//Solid�̌`��f�[�^���z���o��
	for(PHWSolids::iterator is = solids.begin(); is != solids.end(); is++)
		(*is)->Init();
}

void PHWaterContactEngine::Step(SGScene* s){
	PHWSolid* solid;
	PHWGeometry* geo;
	CDPolyhedron* poly;
	PHWSolids::iterator is;
	PHWGeometries::iterator ig;
	CDGeometries::iterator ic;
	CDFaces::iterator iface;
	static Vec3d b, buo, tbuo;	//���͂ƕ��͂ɂ�郂�[�����g
	static Vec3d center_w;
	static double wz;
	static Affinef	Aw, Awinv,	//water-coord to world-coord transformation
					Ag,			//geomerty-coord to world-coord
					Awg,		//water-coord to geometry-coord
                    As, Asinv,	//solid-coord to world-coord
					Asg;		//solid-coord to geometry-coord
	
	//Aw = water->GetFrame()->GetWorldPosture();
	Aw = water->posture;
	Awinv = Aw.inv();

	//���̂ɉ���镂�͂��v�Z����
	//�S���̂ɂ��ĥ��
	for(is = solids.begin(); is != solids.end(); is++){
		solid = *is;
		As = solid->solid->GetFrame()->GetWorldPosture();
		Asinv = As.inv();
		int nface = 0;
		//�S�W�I���g���ɂ��ĥ��
		for(ig = solid->geometries.begin(); ig != solid->geometries.end(); ig++){
			geo = *ig;
			Ag = geo->frame->GetWorldPosture();
			Asg = Asinv * Ag;
			Awg = Awinv * Ag;
			//BBox���x���ł̐ڐG�`�F�b�N
			//...

			//�ʑ��ʑ̂̊e�ʂɉ���镂�͂��W�I���g���̒��S����Ƃ��ĐώZ
			buo.clear();
			tbuo.clear();
			for(ic = geo->conveces.begin(); ic != geo->conveces.end(); ic++){
				poly = DCAST(CDPolyhedron, *ic);
				if(!poly)continue;
				//�����œʑ��ʑ̃��x���ł�BBox���肵�����������Ȃ�C������
				//...

				//�S�ʂɂ��ĥ��
				for(iface = poly->faces.begin(); iface != poly->faces.end(); iface++){
					iface->CalcDualVtx(poly->base);
					//���̖ʂ����ʉ��ɂ��邩�ǂ������ׂ�
					center_w = Awg * iface->center;
					//normal_w = Awg * iface->normal;
					wz = water->LerpHeight(center_w.X(), center_w.Y());
					//���ʉ��Ȃ�Ε��͂��v�Z
					if(wz > center_w.Z()){
						nface++;
						//���̒i�K�ł̓W�I���g���t���[���Ōv�Z���Ă��邱�Ƃɒ���
						b = -iface->normal * ((wz - center_w.Z()) * iface->area);
						buo += b;

						//���[�����g
						tbuo += iface->center % b;
					}
				}
			}
			DSTR << nface;
			//�W�I���g���t���[�����獄�̃t���[���֕ϊ�����AddForce
			solid->solid->AddForce(Ag.Rot() * buo, Ag.Pos());
			solid->solid->AddTorque(Ag.Rot() * tbuo);
		}
	}
}
/*
//�_�C�i�~�N�X�͍l�������ɒP���ɐ��ʉ��ɒ��ݍ���ł���̐ςɔ�Ⴗ�镂��
void getBuoyancy(ThapticObj *ho, Tpoint3f *buo, Tpoint3f *tbuo) {
    Tpoint3f b, n, r;
    Tpoint3f wve;
    Trealf wz, dz;
    TsamplePoint *sp;
    int i;
    
	//�Ȃ񂩌�ŕK�v�ɂȂ肻���Ȃ̂ŃR�����g�A�E�g�ŕۗ�
    //for(i=0; i<ho->nhsrc; i++) ho->hsrc[i].a = 0.0;

    sp = ho->sp;
    for(i=0; i<ho->n_sp; i++) {
        wz = lerpWaterHeight(sp->p.X(), sp->p.Y());
		r = sp->p - ho->pos;	//�I�u�W�F�N�g���S����T���v���_�ւ̃x�N�g��
		n = sp->n;			//�T���v���_�̖@��

    	//���ʂ��T���v���_���������Ȃ��
        if(wz > sp->p.Z()) {
			//���̃T���v���_���󂯂镂��
			b = -n * (wz - sp->p.Z()) * sp->s;
			buo += b;

			//���[�����g
			tbuo += r % b;
            
            //if(ho->nhsrc > 0 && sp->i_hsrc >= 0) ho->hsrc[sp->i_hsrc].a = 1.0;
        }
        sp++;
    }
    
	//�X�P�[�����O���ȁH
	buo *= buo_scl;
    tbuo *= buo_scll;
	
    //for(i=0; i<ho->nhsrc; i++) {
    //    if(ho->hsrc[i].n_sp > 0) ho->hsrc[i].a /= (Trealf)ho->hsrc[i].n_sp;
    //}
}
*/

//----------------------------------------------------------------------------
//	PHWaterContactEngineLoader
//
DEF_RECORD(XWaterContactEngine, {
	GUID Guid(){ return WBGuid("3cec723b-36dc-433e-8ade-06a3e3fd5ee3"); } 
});

class PHWaterContactEngineLoader:public FIObjectLoader<PHWaterContactEngine>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHWaterContactEngine* pc){
		ctx->objects.Push(pc);
		return true;
	}
	virtual void Loaded(FILoadScene* ctx){
		ctx->objects.Pop();
	}
	PHWaterContactEngineLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_RECORD_PROTO(XWaterContactEngine);
	}
};

class PHWaterContactEngineSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWaterContactEngine"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHWaterContactEngine* pc = (PHWaterContactEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("WaterContactEngine", pc);
		ctx->docs.back()->AddChild(doc);
		for(PHWSolids::iterator it = pc->solids.begin(); it != pc->solids.end(); ++it){
			doc->AddChild(ctx->CreateDocNode("REF", (*it)->solid));
		}
		doc->AddChild(ctx->CreateDocNode("REF", pc->water));
	}
};
DEF_REGISTER_BOTH(PHWaterContactEngine);

}
