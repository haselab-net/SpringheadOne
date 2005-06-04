#include "PHWater.h"

/*
	・設計によってはWaterContainerという命名が適当かも
	・PHWaterClearForceの対象はSolidに限らず力が加わるもの全般なので、
		PHClearForceとすべき
*/

class PHWaterEngine : public /*SGBehaviorEngine*/PHSolverBase{
public:
	SGOBJECTDEF(PHWaterEngine);
	
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);
	int GetPriority() const {return SGBP_WATERENGINE;}
	virtual void Step(SGScene* s);
	virtual void ClearForce();
	virtual void Loaded(SGScene* scene);
	virtual void Clear(SGScene* s){ waters.clear(); }
	virtual size_t NChildObjects(){ return waters.size(); }
	virtual SGObject* ChildObject(size_t i){ return waters[i]; }

	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;

};

class PHWater : public GRVisual{
public:
	SGOBJECTDEF(PHWater);

	bool		AddChildObject(SGObject* o, SGScene* s);///< ロード時に使用．
	size_t		NReferenceObjects();					///< 1
	SGObject*	ReferenceObject(size_t i);				///< フレームを返す．
	void		Loaded(SGScene* scene);					///< ロード終了時の初期化
	void		Step(SGScene* s);						///< 時刻を進める．

	///	GRMesh で初期化
	//void Set(GRMesh* gm, SGScene* s);
	///	レンダリング
	virtual void Render(SGFrame* n, GRRender* render);

protected:
	typedef VMatrixCol<double> matrix_type;
	typedef VMatrixCol<Vec3d>	v3matrix_type;
	typedef VVector<double> vector_type;

	UTRef<SGFrame> frame;
	UTRef<PHWater> solid;

	//パラメータ
	size_t	my, mx;
	double	dh;
	double	depth;
	double	gravity;
	double	hscale;
	Vec4d	specular;
	Vec4d	diffuse;
	double	shininess;
	
	//自分を保持するSGFrameへの参照
	UTRef<SGFrame> frame;

	matrix_type		p;				//圧力
	matrix_type		u, utmp;		//x方向流速
	matrix_type		v, vtmp;		//y方向流速
	matrix_type		height, htmp;	//高さ
	v3matrix_type	normal;			//法線
	matrix_type		tvec;			//
	Vec3d			vlight;			//光の向き

	double
		*p_array,
		*u_array, *utmp_array,
		*v_array, *vtmp_array,
		*height_array, *htmp_array,
		*normal_array,
		*tvec_array;
	
	//境界条件を設定する
	void Bound();

};

////////////////////////////////////////////////////////////////
// PHWaterEngine

SGOBJECTIMP(PHWaterEngine, PHSolverBase);
bool PHWaterEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHWater, o)){
		waters.push_back((PHWater*)o);
		return true;
	}
	return false;
}
bool PHWaterEngine::DelChildObject(SGObject* o, SGScene* s){
	PHWater* w = DCAST(PHWater, o);
	if(w){
		waters.Erase(w);
		return true;
	}
	return false;
}

void PHWaterEngine::Step(SGScene* s){
	for(PHWaters::iterator it = waters.begin(); it != waters.end(); ++it){
		(*it)->Step(s);
	}
}

void PHWaterEngine::ClearForce(){
	for(PHWaters::iterator it = waters.begin(); it != waters.end(); ++it){
		(*it)->ClearForce();
	}
}

void PHWaterEngine::Loaded(SGScene* scene){
	for(PHWaters::iterator it = waters.begin(); it != waters.end(); ++it){
		(*it)->Loaded(scene);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// PHWater

SGOBJECTIMP(PHWater, SGObject);

PHWater::PHWater(){
	/*mass = 1.0;
	inertia = Matrix3d::Unit();
	integrationMode = PHINT_ANALYTIC;
	integrationMode = PHINT_SIMPLETIC;*/
}

bool PHWater::AddChildObject(SGObject* o, SGScene* s){
	if(DCAST(SGFrame, o)){
		frame = (SGFrame*)o;
		return true;
	}
	if(DCAST(PHSolid, o)){
		solid = (PHSolid*)o;
		return true;
	}
	return false;
}

size_t PHWater::NReferenceObjects(){
	//frameとsolidを同時に参照することはない
	if(frame) return 1;
	if(solid) return 1;
	return 0;
}

SGObject* PHWater::ReferenceObject(size_t i){
	if(i != 0) return NULL;
	if(frame) return frame;
	if(solid) return solid;
	return NULL;
}

void PHWater::Loaded(SGScene* scene){
	//quat.from_matrix(frame->GetRotation());
}

void PHWater::Render(SGFrame* n, GRRender* render);

void PHWater::Allocate(){
	//その内bad_allocのcatch実装

    height.resize(mx, my);
    u.resize(mx, my);
    v.resize(mx, my);
    utmp.resize(mx, my);
    vtmp.resize(mx, my);
    p.resize(mx, my);
    htmp.resize(mx, my);
    normal.resize(mx, my);
    tvec.resize(mx, my);
    color.resize(mx, my);
}

void PHWater::Init(){
    specular = Vec4d(0.5, 0.5, 0.5, 0.5);
    diffuse  = Vec4d(1.0, 1.0, 1.0, 1.0);
    shininess = 0.0;

    xo = -(MX - 1) / 2. * dh;	//x原点	
    yo = -(MY - 1) / 2. * dh;	//y原点

	int i, j;
	height.clear();
	htmp.clear();
	for (j = 0; j < my; j++)for(i = 0; i < mx; i++)
        normal[i][j].clear();

	for (j = 0; j < my; j++)for(i = 0; i < mx; i++)
        tvec[i][j] = calRefracVec(normal[i][j], vlight, 1.3333);
        
	u.clear();
	utmp.clear();
	v.clear();
	vtmp.clear();
	p.clear();
	
}


void PHWater::Bound(){
	u.col(0).clear();
	u.col(mx - 2).clear();
    u[i][0] = f[i][1];
	u[i][my - 1] = u[i][my - 2];

	v.col(0) = v.col(1);
	v.col(mx - 1) = v.col(mx - 2);
    v.row(0).clear();
	v.row(my - 2).clear();
	
	height.col(0) = height.col(1);
	height.col(mx - 1) = height.col(mx - 2);
	height.row(0) = height.row(1);
	height.row(my - 1) = height.row(my - 2);
}

Vec3d PHWater::calRefracVec(Vec3d n, Vec3d v, double ra) {
    static Vec3d vd, vd_n, tvec;
    static double kf, vn, d;

    vd = v * (1.0 / -abs(v * n));
	vd_n = vd + n;
	kf = ra * ra * vd.square() - vd_n.square();
    if(kf < 0.0) {
        tvec.clear();
        return tvec;
    }
    kf = 1.0/sqrt(kf);

	tvec = kf * vd_n - n;
	tvec.unitize();
    
    return(tvec);
}

void PHWater::Step(SGScene* s){
    static Vec3d vv1, vv2, vv;
    static double dis;
    int i,j;

    //boundary condition
    Bound();
    
	//solve equation
    Integrate();
    
	//boundary condition
    Bound();
    
    /*if(yflow != 0.0) {
        shiftWater(height, yflow);
        shiftWater(wh1, yflow);
        shiftWater(u, yflow);
        shiftWater(v, yflow);
        shiftWater(u1, yflow);
        shiftWater(v1, yflow);
        shiftWater(pw, yflow);
    }*/

	int mx = h.width(), my = h.height();
    for(j = 1; j < my - 1; j++)for(i = 1; i < mx - 1; i++){
		vv1 = Vec3d(-dh, 0.0, h[i][j] - h[i + 1][j    ]);
		vv2 = Vec3d(0.0, -dh, h[i][j] - h[i    ][j + 1]);
		normal[i][j] = Vec3d(-vv1.Z() * vv2.Y(), -vv1.X() * vv2.Z(), vv1.X() * vv2.Y());
		normal[i][j].unitize();
		tvec[i][j] = calRefracVec(normal[i][j], vlight, 1.3333);
    }
}

void PHWater::Integrate(Treald *height,Treald *wh1,Treald *u,Treald *u1,Treald *v,Treald *v1, Treald *p){
    int i, j;
	const double hmul = 10.0;	//	高さを強調して描画
	const double hinv = 1.0 / hmul;
	const double pass = 300;
	static double h;			

    for(i = 1; i <= MX - 2; i++)for(j = 1; j <= MY - 2; j++){
        u1[i][j] = u[i][j] - gravity * (dt / dh) * (height[i+1][j] - height[i][j]) * hinv + (p[i+1][j] - p[i][j]) / (rho_w * dh);
        v1[i][j] = v[i][j] - gravity * (dt / dh) * (height[i][j+1] - height[i][j]) * hinv + (p[i][j+1] - p[i][j]) / (rho_w * dh);
    }
	
    for(i = 1; i <= MX - 2; i++)for(j = 1; j <= MY - 2; j++)
        wh1[i][j] = height[i][j] * hinv -
			depth * dt * ((u1[i][j] - u1[i-1][j]) * dh + (v1[i][j] - v1[i][j-1]) * dh) / (dh * dh);
    
	//	ローパスフィルタ
    for(i = 1; i < MX - 1; i++){
        for(j = 1; j < MY - 1; j++){
			h = wh1[i][j] * (4 + pass) + 
				2.0 * (wh1[i-1][j  ] + wh1[i+1][j  ] + wh1[i  ][j-1] + wh1[i  ][j+1]) + 
					   wh1[i-1][j-1] + wh1[i+1][j-1] + wh1[i-1][j+1] + wh1[i+1][j+1];
			h /= (pass + 16);
			height[i][j] = h * loss * hmul;
            u[i][j] = u1[i][j] * loss;
            v[i][j] = v1[i][j] * loss;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
// LoadState / SaveState

class PHWaterState{
public:
	/*PHWater* solid;
	Vec3d pos;
	Quaterniond ori;
	Vec3d vel;
	Vec3d angVel;
	Vec3d force;
	Vec3d torque;*/
};

class PHWaterEngineState : public SGBehaviorState, public std::vector<PHWaterState>{
public:
	SGOBJECTDEF(PHWaterEngineState);
};

SGOBJECTIMP(PHWaterEngineState, SGBehaviorState);

void PHWaterEngine::LoadState(const SGBehaviorStates& states){
	PHWaterEngineState* pState = DCAST(PHWaterEngineState, states.GetNext());
	assert(pState);
	PHWaterEngineState& state = *pState;
	/*for(int i = 0; i < state.size(); i++){
		solids[i]->SetFramePosition( state[i].pos );
		solids[i]->SetOrientation( state[i].ori );
		solids[i]->SetVelocity( state[i].vel);
		solids[i]->SetAngularVelocity( state[i].angVel);
		solids[i]->SetForce( state[i].force);
		solids[i]->SetTorque( state[i].torque);
	}*/
}
void PHWaterEngine::SaveState(SGBehaviorStates& states) const{
	UTRef<PHWaterEngineState> state = new PHWaterEngineState;
	states.push_back(state);
	/*for(PHWaters::const_iterator it = solids.begin(); it != solids.end(); ++it){
		state->push_back(PHWaterState());
		state->back().pos = (*it)->GetFramePosition();
		state->back().ori = (*it)->GetOrientation();
		state->back().vel = (*it)->GetVelocity();
		state->back().angVel = (*it)->GetAngularVelocity();
		state->back().force = (*it)->GetForce();
		state->back().torque = (*it)->GetTorque();
	}*/
}

///////////////////////////////////////////////////////////////////////////
// Loader / Saver

class PHWaterEngineLoader:public FIObjectLoader<PHWaterEngine>{
	virtual bool LoadData(FILoadScene* ctx, PHWaterEngine* engine){
		//ClearForceを探す→まだ無かったら生成、追加→自身をClearForceに登録
		UTRef<PHWaterClearForce> clearForce;
		ctx->scene->GetBehaviors().Find(clearForce);
		if(!clearForce){
			clearForce= new PHWaterClearForce;
			ctx->scene->GetBehaviors().Add(clearForce);
		}
		clearForce->solvers.push_back(engine);
		return true;
	}
};

class PHWaterEngineSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWaterEngine"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		//保有するPHWaterをSave
		PHWaterEngine* engine = (PHWaterEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("PHWaterEngine", engine);
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(PHWaters::iterator it = engine->waters.begin(); it != engine->waters.end(); ++it){
			ctx->SaveRecursive(*it);
		}
		ctx->docs.pop_back();
	}
};
DEF_REGISTER_BOTH(PHWaterEngine);

class PHWaterLoader:public FIObjectLoader<PHWater>{
	virtual bool LoadData(FILoadScene* ctx, PHWater* water){
		WaterInfo info;
		ctx->docs.Top()->GetWholeData(info);
		//s->SetMass				(info.mass);
		//s->SetInertia			(info.inertia);
		//s->SetCenter			(info.center);
		//s->SetVelocity			(info.velocity);
		//s->SetAngularVelocity	(info.angularVelocity);
		return true;
	}
};

class PHWaterSaver:public FIBaseSaver{
	virtual UTString GetType() const{return "PHWater";}
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHWater* water = (PHWater*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("Water", water);
		ctx->docs.back()->AddChild(doc);
		WaterInfo info;
		/*info.mass				= (float)s->GetMass();
		info.inertia			= s->GetInertia();
		info.velocity			= s->GetVelocity();
		info.angularVelocity	= s->GetAngularVelocity();
		info.center				= s->GetCenter();*/
		doc->SetWholeData(info);
		if(water->GetFrame()){
			doc->AddChild(ctx->CreateDocNode("REF", s->GetFrame()));
		}
		else if(water->GetSolid()){
			doc->AddChild(ctx->CreateDocNode("REF", s->GetSolid()));
		}
	}
};
DEF_REGISTER_BOTH(PHWater);


