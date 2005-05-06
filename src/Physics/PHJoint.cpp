#include "Physics.h"
#pragma hdrstop
#include "PHJointPid.h"
#include <iomanip>

using namespace PTM;
namespace Spr{;

////////////////////////////////////////////////////////////////////////////////
//ツリー操作
PHJointBase* PHJointBase::Search(PHSolid* s)
{
	if(solid == s)return this;
	PHJointBase* pNode;
	array_type::const_iterator it;
	for(it = Children().begin(); it != Children().end(); it++){
		pNode = (*it)->Search(s);
		if(pNode)return pNode;
	}
	return 0;
}
//////////////////////////////////////////////////////////
//	初期化
SGOBJECTIMPABST(PHJointBase, SGObject);

PHJointBase::PHJointBase():solid(0), frame(0){
	intType = SYMPLETIC;
//	intType = ANALYTIC;
}

void PHJointBase::Loaded(SGScene* scene)
{
	//solidもframeも無指定の場合の処理
	if(!solid && !frame){
		solid = new PHSolid;
		solid->SetMass(0.1f);
		solid->SetInertia(0.1f * Matrix3d());
		solid->SetFrame(new SGFrame);
	}
	if(solid)solid->Loaded(scene);
	
	//諸変数の初期化
	UpdateSolid();
	if(GetParent()){
		CompJointAxis();
	}else{
		if(solid){
			p = solid->GetCenterPosition();
			R = solid->GetRotation();
			v_abs = solid->GetVelocity();
			w_abs = solid->GetAngularVelocity();
		}else{
			p = frame->GetPosition();
			R = frame->GetRotation();
			v_abs.clear();
			w_abs.clear();
		}		
	}
	Ia.clear();
	Za.clear();

	Reset();

	for(unsigned i=0; i<Children().size(); ++i){
		Children()[i]->Loaded(scene);
	}
}

//////////////////////////////////////////////////////////
//・木構造
SGObject* PHJointBase::ChildObject(size_t i){
	return Children()[i];
}
SGObject* PHJointBase::ReferenceObject(size_t i){
	if (i!=0) return NULL;
	if (solid) return solid;
	return frame;
}
size_t PHJointBase::NReferenceObjects(){
	if (frame || solid) return 1; 
	return 0;
}
size_t PHJointBase::NChildObjects(){
	return Children().size();
}

bool PHJointBase::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		solid = (PHSolid*)o;
		return true;
	}
	if (DCAST(PHJointBase, o)){
		((PHJointBase*)o)->SetParent(this);
		return true;
	}
	return s->AddChildObject(o, s);
}

bool PHJointBase::DelChildObject(SGObject* o, SGScene* s){
	PHJointBase* j = DCAST(PHJointBase, o);
	for(unsigned i=0; i<Children().size(); ++i){
		if (Children()[i] == j){
			j->SetParent(NULL);
			return true;
		}
	}
	return false;
}

const UTTypeInfo** PHJointBase::ChildCandidates(){
	static const UTTypeInfo* rv[] = {
		PHJointBase::GetTypeInfoStatic(), NULL
	};
	return rv;
}

void PHJointBase::UpdateSolid(){
	if(solid){
		m = solid->GetMass();
		I = solid->GetInertia();
	}
	else{
		m = 0.0;
		I.clear();
	}
	Ii.clear();
	smitem(Ii, 0, 1) = Matrix3d::Diag(m, m, m);
	smitem(Ii, 1, 0) = I;
}
void PHJointBase::UpdateJointPosture(){
	CompJointAxis();
	CompRelativePosition();
	//TODO:ここで下位ノードの位置をすべて更新
}
void PHJointBase::CompArticulatedInertia(double dt){
	//	Ia, Zaをクリア
	Ia.clear();
	Za.clear();

	//子ノードから先に計算
	for(array_type::const_iterator it = Children().end(); it != Children().begin();)
		(*--it)->CompArticulatedInertia(dt);

	//よく使う出てくる式をキャッシュ
	rcross = Matrix3d::Cross(prc);
	rpcross = Matrix3d::Cross(pRc * prc);
	rcross_cRp = rcross * cRp;
	rpcross_pRc = rpcross * pRc;
		
	//Za
	if(solid){
		svitem(Za, 0) += -R.trans() * solid->GetForce();
		svitem(Za, 1) += -R.trans() * solid->GetTorque() + Vec3d(
			(I[2][2] - I[1][1]) * w.Y() * w.Z(),
			(I[0][0] - I[2][2]) * w.Z() * w.X(),
			(I[1][1] - I[0][0]) * w.X() * w.Y());
	}
	//Ia
	Ia += Ii;
	Ia_c = Ia * c;
	Z_plus_Ia_c = Za + Ia_c;
}
void PHJointBase::CompCoriolisAccelRecursive(double dt){
	if (GetParent()) CompCoriolisAccel();
	for(array_type::const_iterator it = Children().end(); it != Children().begin();){
		(*--it)->CompCoriolisAccelRecursive(dt);
	}
}

void PHJointBase::ClearTorqueRecursive(){
	for(array_type::iterator it = Children().begin(); it != Children().end(); ++it){
		(*it)->ClearTorqueRecursive();
	}
}

void PHJointBase::Integrate(double dt){
	if(solid){
#if 0	//	hase
		if (pos.norm() > 100){
			DSTR << "Strange position:" << std::endl;
			DSTR << p << std::endl;
			DSTR << R << solid->GetCenter() << std::endl;
			DSTR << GetParent()->p << std::endl;
		}
#endif
		solid->SetCenterPosition(p);
		solid->SetRotation(R);
		solid->SetVelocity(v_abs);
		solid->SetAngularVelocity(w_abs);
		solid->SetIntegrationMode(PHINT_NONE);
	}	
	for(array_type::const_iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->Integrate(dt);
}

void PHJointBase::PropagateState()
{
	//親ノードとの相対位置、姿勢を計算
	CompRelativePosition();

	//相対位置から絶対位置を計算
	R = GetParent()->R * pRc;
	p = GetParent()->p + R * prc;

	//親ノードの速度・角速度＋関節速度　→　速度・角速度
	w = cRp * GetParent()->w;
	v = cRp * GetParent()->v + cross(w, prc);
	CompRelativeVelocity();
	v += pvc;
	w += pwc;
	v_abs = R * v;
	w_abs = R * w;
}
void PHJointBase::Reset(){
	//非ルートノード
	if(GetParent()){	
		PropagateState();
		solid->SetCenterPosition(p);
		solid->SetRotation(R);
		solid->UpdateFrame();
	}
	//ルートノード
	else{
		quat.from_matrix(R);
		v = R.trans() * v_abs;
		w = R.trans() * w_abs;
	}
}

void PHJointBase::LoadState(const SGBehaviorStates& states){
	Reset();
	//	子ジョイントをロード
	for(unsigned i=0; i<Children().size(); ++i){
		Children()[i]->LoadState(states);
	}
}

void PHJointBase::SaveState(SGBehaviorStates& states) const{
	for(unsigned i=0; i<Children().size(); ++i){
		Children()[i]->SaveState(states);
	}
}
void PHJointBase::SaveX(XJointBase& x) const{
	x.v3fPositionChild = v3fPositionChild;
	x.v3fPositionParent = v3fPositionParent;
	x.m3fRotationChild = m3fRotationChild;
	x.m3fRotationParent = m3fRotationParent;
}
void PHJointBase::LoadX(const XJointBase& x){
	v3fPositionChild = x.v3fPositionChild;
	v3fPositionParent = x.v3fPositionParent;
	m3fRotationChild = x.m3fRotationChild;
	m3fRotationParent = x.m3fRotationParent;
}


//-----------------------------------------------------------------------------
SGOBJECTIMP(PHJointRoot, PHJointBase);
void PHJointRoot::Reset(){
	PHJointBase::Reset();
}

class PHJointStateRoot: public SGBehaviorState{
public:
	SGOBJECTDEF(PHJointStateRoot);
	//ルートノードの状態
	Vec3d p;
	Matrix3d R;
	Vec3d v_abs;
	Vec3d w_abs;
};
SGOBJECTIMP(PHJointStateRoot, SGBehaviorState);
void PHJointRoot::LoadState(const SGBehaviorStates& states){
	PHJointStateRoot* js = DCAST(PHJointStateRoot, states.GetNext());	
	//ロード
	p = js->p;
	R = js->R;
	v_abs = js->v_abs;
	w_abs = js->w_abs;
	PHJointBase::LoadState(states);
}
void PHJointRoot::SaveState(SGBehaviorStates& states) const{
	UTRef<PHJointStateRoot> js = new PHJointStateRoot;
	states.push_back(js);
	js->p = p;
	js->R = R;
	js->v_abs = v_abs;
	js->w_abs = w_abs;
	PHJointBase::SaveState(states);
}
void PHJointRoot::CalcAccel(double dt){
	if(solid){	//	ルートノードが剛体の場合
		//加速度を計算
		a = (Ia.inv() * Za) * -1;
	}else{	//if(frame)
		a.clear();
	}
}
void PHJointRoot::Integrate(double dt){
	//physical
	if(solid){
		//加速度を計算
		a = (Ia.inv() * Za) * -1;
		//速度変化量
		Vec3d dv_abs = R * svitem(a, 1) * dt;
		//角速度変化量
		Vec3d dw_abs = R * svitem(a, 0) * dt;

		//位置を更新
		p += (v_abs + 0.5 * dv_abs) * dt;
		//回転量を更新
		quat += quat.derivative(w_abs + 0.5 * dw_abs) * dt;
		quat.unitize();
		quat.to_matrix(R);

		//速度を更新
		v_abs += dv_abs;
		w_abs += dw_abs;
		v = quat.conjugated() * v_abs;
		w = quat.conjugated() * w_abs;

		Vec3d a_rot = R * svitem(a, 0);
		Vec3d a_trn = R * svitem(a, 1);

		/* 旧形式
		//加速度を積分して新しい速度を求める
		Vec3d w_new = (w_abs + a_rot * dt);
		Vec3d v_new = (v_abs + a_trn * dt);

		//速度を積分して位置を求める
		p += (v_abs + (0.5 * dt) * (R * a_trn)) * dt;

		double wnorm = w.norm();
		//クヲータニオンを微小回転
		if(wnorm > 1.0e-10){
			quat = Quaterniond::Rot(wnorm * dt, w_abs / wnorm) * quat;
			quat.unitize();
		}
		//これを回転行列に変換
		quat.to_matrix(R);

		//速度を更新
		v_abs = v_new;
		w_abs = w_new;
		v = R.trans() * v_abs;
		w = R.trans() * w_abs;
		*/
	}else{		//non-physical
		a.clear();
	}
	//関連コンポーネントの位置、速度、関節変位、関節速度を更新
	PHJointBase::Integrate(dt);
}

//-----------------------------------------------------------------------------
SGOBJECTIMP(PHJointClearForce, SGBehaviorEngine);
void PHJointClearForce::Step(SGScene* s){
	je->root->ClearTorqueRecursive();
}


SGOBJECTIMP(PHJointEngine, SGBehaviorEngine);
void PHJointEngineEnumSolid(PHJointBase* j, PHJointEngine* e){
	e->solids.insert(j->solid);
}
void PHJointEngine::Loaded(SGScene* scene){
	root->Loaded(scene);
	root->Traverse(&PHJointEngineEnumSolid, this);
}
void PHJointEngine::Step(SGScene* scene)
{
	timer.Start();
	//	featherstone's algorithm
	double dt = scene->GetTimeStep();
	root->CompCoriolisAccelRecursive(dt);
	root->CompArticulatedInertia(dt);
	root->Integrate(dt);
	timer.Stop();
}

bool PHJointEngine::Has(SGObject* o){
	PHSolid* solid = DCAST(PHSolid, o);
	if (solid){
		if (solids.find(solid) != solids.end()) return true;
	}
	PHJointBase* joint = DCAST(PHJointBase, o);
	while (joint){
		if (joint == root) return true;
		joint = joint->GetParent();
	}
	return false;
}
SGObject* PHJointEngine::ChildObject(size_t i){
	return root;
}
size_t PHJointEngine::NChildObjects(){
	return root ? 1 : 0;
}
bool PHJointEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		root->solid = (PHSolid*)o;
		return true;
	}
	if (DCAST(SGFrame, o)){
		root->frame= (SGFrame*)o;
		return true;
	}
	if (DCAST(PHJointBase, o)){
		((PHJointBase*)o)->SetParent(root);
		return true;
	}
	return false;
}
bool PHJointEngine::DelChildObject(SGObject* o, SGScene* s){
	if (root == o){
		root = NULL;
		return true;
	}
	return false; 
}
const UTTypeInfo** PHJointEngine::ChildCandidates(){
	static const UTTypeInfo* rv[] = {
		PHJointBase::GetTypeInfoStatic(), NULL
	};
	return rv;
}
void PHJointEngine::LoadState(const SGBehaviorStates& states){
	root->LoadState(states);
}
void PHJointEngine::SaveState(SGBehaviorStates& states) const{
	root->SaveState(states);
}

//////////////////////////////////////////////////////////////////////
class PHJointEngineLoader : public FIObjectLoader<PHJointEngine>{
public:
	bool LoadData(FILoadScene* ctx, PHJointEngine* engine){
		engine->root = new PHJointRoot;
		PHJointClearForce* jcf = new PHJointClearForce;
		jcf->je = engine;
		ctx->scene->AddChildObject(jcf, ctx->scene);
		return true;
	}
};

class PHJointEngineSaver : public FIBaseSaver
{
public:
	UTString GetType() const{ return "PHJointEngine";}
	void Save(FISaveScene* ctx, SGObject* arg){
		PHJointEngine* je = (PHJointEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("JointEngine", je);
		//接続対象を保存
		if (je->root->solid){
			doc->AddChild(ctx->CreateDocNode("REF", je->root->solid));
		}else if(je->root->frame){
			doc->AddChild(ctx->CreateDocNode("REF", je->root->frame));
		}

		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(unsigned i=0; i<je->root->Children().size(); ++i){
			ctx->SaveRecursive(je->root->Children()[i]);
		}
		ctx->docs.pop_back();
	}
};


//関節の種類
enum PHJointType{
	TYPE_HINGE,			//回転
	TYPE_SLIDER,		//直動
	//TYPE_BALLJOINT,	//ボールジョイント
};
//入力の種類
enum PHInputType{
	TYPE_TORQUE,		//トルク
	TYPE_ACCEL,			//加速度
	TYPE_VELOCITY,		//速度
	TYPE_POSITION		//位置
};
///関節情報．そのままファイルのJointタグのエントリと同じ
struct PHJointInfo{
	DWORD		nType;				///<	関節種類 @see PHJointType
	Vec3f		v3fPositionParent;	///<	親剛体のフレームから見た関節位置
	Matrix3f	m3fRotationParent;	///<	親剛体のフレームから見た関節姿勢
	Vec3f		v3fPositionChild;	///<	子剛体のフレームから見た関節位置
	Matrix3f	m3fRotationChild;	///<	子剛体のフレームから見た関節姿勢

	float		fPosition;			///<	変位
	float		fVelocity;			///<	速度
	float		fMaxTorque;			///<	最大トルク
	float		fMinPosition;		///<	可動範囲(最大値)
	float		fMaxPosition;		///<	可動範囲(最小値)
	DWORD		nInputType;			///<	入力の種類 @see PHJointInputType
	float		fInput;				///<	入力値の初期値
	float		fPValue;			///<	PID制御のPの値
	float		fIValue;			///<	PID制御のIの値
	float		fDValue;			///<	PID制御のDの値
};

class PHJointLoader: public FIBaseLoader
{
protected:
	virtual UTString GetNodeType() const {
		return "Joint";
	}
	void Load(FILoadScene* ctx){
		//関節の種類とパラメータ
		PHJointInfo info;
		ctx->docs.Top()->GetWholeData(info);
		//	左手系→右手系変換
		info.m3fRotationChild.ExZ() *= -1;
		info.m3fRotationChild.EyZ() *= -1;
		info.m3fRotationChild.EzX() *= -1;
		info.m3fRotationChild.EzY() *= -1;
		info.v3fPositionChild.Z() *= -1;
		info.m3fRotationParent.ExZ() *= -1;
		info.m3fRotationParent.EyZ() *= -1;
		info.m3fRotationParent.EzX() *= -1;
		info.m3fRotationParent.EzY() *= -1;
		info.v3fPositionParent.Z() *= -1;

		UTRef<PHJoint1D> joint=NULL;
		if (info.nType == TYPE_HINGE){
			joint = new PHJointHinge;
		}else if(info.nType == TYPE_SLIDER){
			joint = new PHJointSlider;
		}else{
			DSTR << "Unknown joint type " << info.nType << std::endl;
		}
		joint->SetName(ctx->docs.Top()->GetName().c_str(), ctx->scene);
		ctx->AddObject(joint);
		ctx->AddContainer(joint);
		
		joint->m3fRotationChild = info.m3fRotationChild;
		joint->m3fRotationParent = info.m3fRotationParent;
		joint->v3fPositionChild = info.v3fPositionChild;
		joint->v3fPositionParent = info.v3fPositionParent;
		joint->position = info.fPosition;
		joint->velocity = info.fVelocity;
		joint->maxPosition = info.fMaxPosition;
		joint->minPosition = info.fMinPosition;
		
		//	PIDの分離
		UTRef<PHJointPid> pid = new PHJointPid;
		std::string name = joint->GetName();
		if (name[0] == 'j') name = name.substr(1);
		if (name[0] == 'o') name = name.substr(1);
		name = std::string("pid").append(name);
		pid->SetName(name.c_str(), ctx->scene);
		ctx->scene->AddChildObject(pid, ctx->scene);
		pid->proportional = info.fPValue;
		pid->differential = info.fDValue;
		pid->integral = info.fIValue;
		pid->goal = info.fInput;
		pid->joint = joint;
		pid->type = (info.nInputType == TYPE_VELOCITY) ? 1 : 0;
	}
};

DEF_REGISTER_LOADER(PHJoint);
DEF_REGISTER_BOTH(PHJointEngine);

}
