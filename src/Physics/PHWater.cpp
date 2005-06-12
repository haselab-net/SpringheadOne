#include "PHWater.h"
#pragma hdrstop

namespace Spr{;

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
	Init(scene);
}

void PHWater::Init(SGScene* scene){
    //xo = -(MX - 1) / 2. * dh;	//x原点	
    //yo = -(MY - 1) / 2. * dh;	//y原点
	//int i, j;

	int mx = info.mx, my = info.my;
    mxy = info.mx * info.my;
	dx = mx * info.dh / 2.0;
	dy = my * info.dh / 2.0;

	//その内bad_allocのcatch実装
	height.resize(mx, my);
    htmp.resize(mx, my);
    u.resize(mx, my);
    v.resize(mx, my);
    utmp.resize(mx, my);
    vtmp.resize(mx, my);
    p.resize(mx, my);
    normal.resize(mx, my);
    tvec.resize(mx, my);
    //color.resize(mx, my);

	height.clear();
	htmp.clear();
	for (j = 0; j < my; j++)for(i = 0; i < mx; i++)
        normal[i][j].clear();

	for (j = 0; j < my; j++)for(i = 0; i < mx; i++)
        tvec[i][j] = calRefracVec(normal[i][j], info.vlight, 1.3333);
        
	u.clear();
	utmp.clear();
	v.clear();
	vtmp.clear();
	p.clear();

	//マテリアルの指定がなかった場合、デフォルトを作成
	if(!material){
		material = new GRMaterial;
		material->specular = Vec4d(0.5, 0.5, 0.5, 0.5);
		material->diffuse  = Vec4d(1.0, 1.0, 1.0, 1.0);
		material->emissive = 0.0;
		//vlight = Vec3d(0.0, 0.0, -1.0);
	}

	//Direct3D用のメッシュを作成
	D3Render* render = NULL;
	scene->GetRenderers().Find(render);
	if(render){
		meshD3 = new D3Mesh;
		DWORD fvf;
		//if (gm->texCoords.size()) fvf = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
		//else
		fvf = D3DFVF_XYZ | D3DFVF_NORMAL;
		//3角形の数＝マスの数 * 2
		int nTriangles = (mx - 1) * (my - 1) * 2;
		D3DXCreateMeshFVF(nTriangles, mxy, D3DXMESH_MANAGED, fvf, render->device, &(meshD3->intf.Intf()));

		//インデックスバッファ
		WORD* indexBuf = NULL;
		meshD3->LockIndexBuffer(0, (void**)&indexBuf);
		int x, y, i = 0;
		for(x = 0; x < mx - 1; x++)for(y = 0; y < my - 1; y++){
			indexBuf[i++] = (x + 0) + mx * (y + 0);
			indexBuf[i++] = (x + 0) + mx * (y + 1);
			indexBuf[i++] = (x + 1) + mx * (y + 0);
			indexBuf[i++] = (x + 1) + mx * (y + 1);
			indexBuf[i++] = (x + 1) + mx * (y + 0);
			indexBuf[i++] = (x + 0) + mx * (y + 1);
		}
		meshD3->intf->UnlockIndexBuffer();

		//マテリアル
		materialD3 = new D3Material;
		memcpy(&materialD3->material, (GRMaterialData*)&material, sizeof(GRMaterialData));
		materials[i]->bOpaque = material->IsOpaque();
		//materials[i]->textureFilename = material->textureFilename;
		//materials[i]->texture = render->textureManager.Get(materials[i]->textureFilename);
	}
}

void PHWater::Render(SGFrame* n, GRRender* render){
	//renderの種類を判定
	if(DCAST(D3Render, render))
		return RenderD3(n, DCAST(D3Render, render));
	if(DCAST(GLRender, render))
		return RenderGL(n, DCAST(GLRender, render));
}

void PHWater::RenderD3(SGFrame* n, D3Render* render){
	if(!meshD3) return;

	/*if (gm->texCoords.size()){
		struct VtxFVF{
			Vec3f pos;
			Vec3f normal;
			Vec2f tex;
		};
		VtxFVF* vtxs;
		LockVertexBuffer(0, (void**)&vtxs);
		for(unsigned int i=0; i<gm->vertices.size(); ++i){
			vtxs[i].pos = gm->vertices[i];
			vtxs[i].normal = gm->normals[i];
			vtxs[i].tex = gm->texCoords[i];
		}
		intf->UnlockVertexBuffer();
	}else{*/
	double* pheight = &height[0][0];
	Vec3d*  pnormal = &normal[0][0];
	
	//頂点バッファに書き込む
	struct VtxFVF{
		Vec3f pos;
		Vec3f normal;
	};
	VtxFVF* vtxs;
	meshD3->LockVertexBuffer(0, (void**)&vtxs);
	float x = -dx, y = -dy;

	for(int i = 0; i < mxy; i++){
		vtxs[i].pos = Vec3f(x, y, pheight[i]);
		vtxs[i].normal = pnormal[i];
		if(i % mx == 0){
			y += info.dh;
			x = -dx;
		}
	}
	meshD3->intf->UnlockVertexBuffer();
	
	/*DWORD* attrs=NULL;
	intf->LockAttributeBuffer(0, &attrs);
	for(unsigned i=0; i<gm->attributes.size(); ++i) attrs[i] = gm->attributes[i];
	intf->UnlockAttributeBuffer();
	intf->OptimizeInplace(D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);
	*/

	D3Render* render = (D3Render*)renderBase;
	if (materialD3->bOpaque && render->drawState&GRRender::DRAW_OPAQUE){
		materialD3->Render(f, render);
		WXCHECK(meshD3->intf->DrawSubset(0));
	}
	//if (!materials[i]->bOpaque && render->drawState&GRRender::DRAW_TRANSPARENT){
	//	materials[i]->Render(f, renderBase);
	//	WXCHECK(intf->DrawSubset(i));
	//}
	//	テクスチャを戻す．
	render->device->SetTexture(0,NULL);

}

void PHWater::RenderGL(SGFrame* n, GRRender* render){

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
	Integrate(s->GetTimeStep());
    
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

	//法線と屈折ベクトルを計算
	int mx = info.mx, my = info.my;
    for(j = 1; j < my - 1; j++)for(i = 1; i < mx - 1; i++){
		vv1 = Vec3d(-dh, 0.0, h[i][j] - h[i + 1][j    ]);
		vv2 = Vec3d(0.0, -dh, h[i][j] - h[i    ][j + 1]);
		normal[i][j] = Vec3d(-vv1.Z() * vv2.Y(), -vv1.X() * vv2.Z(), vv1.X() * vv2.Y());
		normal[i][j].unitize();
		tvec[i][j] = calRefracVec(normal[i][j], vlight, 1.3333);
    }
}

void PHWater::Integrate(double dt){
    int i, j;
	const double hmul = 10.0;	//	高さを強調して描画
	const double hinv = 1.0 / hmul;
	const double pass = 300;
	static double h;			

    for(i = 1; i <= MX - 2; i++)for(j = 1; j <= MY - 2; j++){
        u1[i][j] = u[i][j] - gravity * (dt / dh) * (height[i+1][j] - height[i][j]) * hinv + (p[i+1][j] - p[i][j]) / (rho_w * dh);
        v1[i][j] = v[i][j] - gravity * (dt / dh) * (height[i][j+1] - height[i][j]) * hinv + (p[i][j+1] - p[i][j]) / (rho_w * dh);
    }
	
	/*
		x = [i-1, i], y = [j-1, j]の四角領域の高さをh[i][j]とすると
		辺x = i-1からの流入量は
			流速u[i-1][j] * 辺dh * 水深depth * 時間dt
		同様にx = i, y = j - 1, y = jの流入出量を総和したものが四角領域の水量の変化量。
		これに四角領域の面積dh * dhを割れば高さの変化量が得られる
	 */
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
		water->SetInfo(info);
		return true;
	}
};

class PHWaterSaver:public FIBaseSaver{
	virtual UTString GetType() const{return "PHWater";}
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHWater* water = (PHWater*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("Water", water);
		ctx->docs.back()->AddChild(doc);
		
		doc->SetWholeData(water->GetInfo(info));
		if(water->GetFrame()){
			doc->AddChild(ctx->CreateDocNode("REF", s->GetFrame()));
		}
		else if(water->GetSolid()){
			doc->AddChild(ctx->CreateDocNode("REF", s->GetSolid()));
		}
	}
};
DEF_REGISTER_BOTH(PHWater);

}