#include "Physics.h"
#pragma hdrstop
#include "PHWater.h"
#include <Graphics/GRMaterial.h>
#include <ImpD3D/D3Render.h>
#include <ImpD3D/D3Mesh.h>
#include <ImpD3D/D3Material.h>
#include <GraphicsGL/GLRender.h>
#include <GraphicsGL/GLMesh.h>
#include <Physics/PHSolid.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma hdrstop

namespace Spr{;

////////////////////////////////////////////////////////////////
// PHWaterEngine

SGOBJECTIMP(PHWaterEngine, PHSolverBase);
    // insert new phwater object to the end of the vector
bool PHWaterEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHWater, o)){
		waters.push_back((PHWater*)o);
		return true;
	}
	return false;
}

    // delete the specific phwater object
bool PHWaterEngine::DelChildObject(SGObject* o, SGScene* s){
	PHWater* w = DCAST(PHWater, o);
	if(w){
		waters.Erase(w);
		return true;
	}
	return false;
}

    // call the step function of all phwater objects
    // this method updates information of all phwater objects in the waters vector
void PHWaterEngine::Step(SGScene* s){
	for(PHWaters::iterator it = waters.begin(); it != waters.end(); ++it){
		(*it)->Step(s);
	}
}

    // call clearforce method of all phwater objects in the waters vector
void PHWaterEngine::ClearForce(){
	for(PHWaters::iterator it = waters.begin(); it != waters.end(); ++it){
		(*it)->ClearForce();
	}
}

void PHWaterEngine::Clear(SGScene* s){
	waters.clear();
}

void PHWaterEngine::Loaded(SGScene* scene){
	for(PHWaters::iterator it = waters.begin(); it != waters.end(); ++it){
		(*it)->Loaded(scene);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// PHWater

SGOBJECTIMP(PHWater, GRVisual);
    // constructor
PHWater::PHWater(){
	mx = my = 0;
	dh = 0.0;
	depth = 0.0;
	hscale = 1.0;
	density = 1.0;
	loss = 0.99;
}

    // this function adds new child data to the end of the vector
    // object of SGFrame or PHSolid can be inserted
    // this function returns the result of insertion as a boolean
bool PHWater::AddChildObject(SGObject* o, SGScene* s){
	if(DCAST(SGFrame, o)){
		frame = (SGFrame*)o;
		frame->contents.push_back(this);
		return true;
	}
	if(DCAST(PHSolid, o)){
		solid = (PHSolid*)o;
		if(solid->GetFrame())
			solid->GetFrame()->contents.push_back(this);
		return true;
	}
	if(DCAST(PHWaterTrackTarget, o)){
		targets = (PHWaterTrackTarget*)o;
		return true;
	}
	return false;
}

    // refer the object
    // if object exists
    // this function returns 1, otherwise returns 0
size_t PHWater::NReferenceObjects(){
	//frameとsolidを同時に参照することはない
	if(frame) return 1;
	if(solid) return 1;
	return 0;
}

    // refer the object
    // if i is not 0, return NULL
    // if i is zero and object exists,
    // return object
SGObject* PHWater::ReferenceObject(size_t i){
	if(i != 0) return NULL;
	if(frame) return frame;
	if(solid) return solid;
	return NULL;
}

    // this method calls the init function with the given scene
void PHWater::Loaded(SGScene* scene){
	Init(scene);
}

    // this function initializes local variables
void PHWater::Init(SGScene* scene){
    //xo = -(MX - 1) / 2. * dh;	//x原点	
    //yo = -(MY - 1) / 2. * dh;	//y原点
	int i, j;

    // total number of cells
	mxy = mx * my;

    // dh = width of grid
    // mx, my = number of divisions
	dx = mx * dh / 2.0;
	dy = my * dh / 2.0;

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

    // initialize height matrix array
	height.clear();
	height[5][5] = 0.1;

    // temporary height variable
	htmp.clear();

    // initialize normal matrix array
	for (j = 0; j < my; j++)for(i = 0; i < mx; i++)
        normal[i][j].clear();

    // calculate refracted vectors
	for (j = 0; j < my; j++)for(i = 0; i < mx; i++)
        tvec[i][j] = calRefracVec(normal[i][j], vlight, 1.3333);

    // clear velocity variables and pressure
	u.clear();
	utmp.clear();
	v.clear();
	vtmp.clear();
	p.clear();

	//マテリアルの指定がなかった場合、デフォルトを作成
	if(!material){
		material = new GRMaterial;
		material->ambient  = Vec4d(0.5, 0.5, 0.5, 1.0);
		material->specular = Vec4d(0.5, 0.5, 0.5, 1.0);
		material->diffuse  = Vec4d(1.0, 1.0, 1.0, 1.0);
		material->emissive = Vec4d(0.5, 0.5, 0.5, 1.0);
		material->power = 0.5;
		//vlight = Vec3d(0.0, 0.0, -1.0);
	}
	//	描画用の頂点
/*	vtxs.resize(mxy);
	for(int x = 0; x < mx - 1; x++)for(int y = 0; y < my - 1; y++){
		vtxs[y*mx + x].pos = Vec3f(x*dh, y*dh, 0);
		vtxs[y*mx + x].normal = Vec3f(0, 0, 0);
	}
*/	//マテリアル
	materialD3 = new D3Material;
	memcpy(&(materialD3->material), (GRMaterialData*)&*material, sizeof(GRMaterialData));
	materialD3->bOpaque = material->IsOpaque();
}

double PHWater::LerpHeight(double x, double y){
	static double nx, ny, h0, h1;

    if(x < -dx || x > dx || y < -dy || y > dy)
		return 0.0;

	//座標から格子インデックスを算出
	nx = (x + dx) / dh;
	ny = (y + dy) / dh;
	int ix = floor(nx), iy = floor(ny);
    
    h0 = height[ix    ][iy] * (1.0 - ny) + height[ix    ][iy + 1] * ny;
    h1 = height[ix + 1][iy] * (1.0 - ny) + height[ix + 1][iy + 1] * ny;

    return h0 * (1.0 - nx) + h1 * nx;
}

void PHWater::Shift(){
    /*int i,j;
    for(j=0; j<MY; j++)for(i=0; i<MX; i++)
		tmp[IX(i,j)] = x[IX(i,j)];

    for(j=1; j<MY-1; j++)for(i=1; i<MX-1; i++)
		x[IX(i,j)] = (1.0-shift)*tmp[IX(i,j)]+shift*tmp[IX(i,j+1)];*/
}

void PHWater::Render(SGFrame* n, GRRender* render){
	//renderの種類を判定
	if (render->drawState&GRRender::DRAW_OPAQUE == 0) return;
	if(DCAST(D3Render, render))
		RenderD3(n, DCAST(D3Render, render));
	if(DCAST(GLRender, render))
		RenderGL(n, DCAST(GLRender, render));
}

    // drawn by Direct3D
    // if mesh is not created or initialized
    // do nothing and exit the function
void PHWater::RenderD3(SGFrame* n, D3Render* render){
    // copy the head address of the varialbe height array to pheight
	double* pheight = &height[0][0];
    // copy the head address of the variable normal array to pnormal
	Vec3d*  pnormal = &normal[0][0];
	
	if ( (materialD3->bOpaque && render->drawState & GRRender::DRAW_OPAQUE)
		|| (!materialD3->bOpaque && render->drawState & GRRender::DRAW_TRANSPARENT) ){
        // this function sets the texture
		materialD3->Render(n, render);
		render->device->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL);
		struct VtxFVF{
			Vec3f pos;
			Vec3f normal;
		};
		VtxFVF* buf= new VtxFVF[mx*2];
	    float xo = -(mx-1)/2.0 * dh, yo = -(my-1)/2.0 * dh;
		for(int y=0; y<my-1; ++y){
			int start = y*mx;
			double left = xo;
			double px = xo;
			double py = yo+y*dh;
			for(int x=0; x<mx; ++x){
				buf[x*2+1].pos = Vec3f(px, py, pheight[start+x]);
				buf[x*2+1].normal = pnormal[start+x];
				buf[x*2].pos = Vec3f(px, py+dh, pheight[start+mx+x]);
				buf[x*2].normal = pnormal[start+x+mx];
				px += dh;
			}
			render->device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (mx-1)*2, buf, sizeof(buf[0]));
		}	
		delete buf;
	}
	//	テクスチャを戻す．
	render->device->SetTexture(0,NULL);
}

    // draw objects by OpenGL
void PHWater::RenderGL(SGFrame* n, GLRender* render){
    int i = 0, j = 0;
    double xo = -(mx-1)/2.0 * dh, yo = -(my-1)/2.0 * dh;
    double x = xo, y = yo;
   
    // texture mapping
    // if material is Opaque and drawState is true
    // or material is not Opaque and drawState is true
    // rendering is done twice
    // first one is for opaque objects
    // second one is for transparent objects
    if (material->IsOpaque() && render->drawState & GRRender::DRAW_OPAQUE){
        material->Render(n, render);
        }
    
    //	if(!material->IsOpaque() && render->drawState & GRRender::DRAW_TRANSPARENT){
        material->Render(n, render);

        //        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); 
        //        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); 
        //        glEnable(GL_TEXTURE_GEN_S); 
        //        glEnable(GL_TEXTURE_GEN_T); 

		// create the mesh/triangles
        for (j = 0; j < my-1; j++) {
            glBegin(GL_QUAD_STRIP);
			x = xo;
			for (i = 0; i < mx; i++) {

				glNormal3f(normal[i][j+1].x ,normal[i][j+1].y ,normal[i][j+1].z);
				glVertex3f(x, y+dh, height[i][j+1]);

                glNormal3f(normal[i][j].x ,normal[i][j].y ,normal[i][j].z);
				glVertex3f(x, y, height[i][j]);

                x+= dh;
            }
            y+=dh;
            glEnd();
        }
        
        //        glDisable(GL_TEXTURE_GEN_T); 
        //        glDisable(GL_TEXTURE_GEN_S);
        
        // initialize the texture
        render->SetTexture(NULL);
        //   }
}

    // this function adjusts the boundary problem
void PHWater::Bound(){
	//行方向がx方向、列方向がy方向なことに注意
	u.row(0).clear();
	u.row(mx - 2).clear();
    u.col(0) = u.col(1);
	u.col(my - 1) = u.col(my - 2);

	v.row(0) = v.row(1);
	v.row(mx - 1) = v.row(mx - 2);
    v.col(0).clear();
	v.col(my - 2).clear();

    // update the height of the wave
	height.row(0) = height.row(1);
	height.row(mx - 1) = height.row(mx - 2);
	height.col(0) = height.col(1);
	height.col(my - 1) = height.col(my - 2);
}

    // calculate refracted vectors
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
 
    // this function calculates and updates the information
    // such as boundary problem, normal vectors,
    // refracted vectors, x-y velocities,
    // and height array by using integrate function 
void PHWater::Step(SGScene* s){
    static Vec3d vv1, vv2, vv;
    static double dis;
    int i,j;

	//return;

    // set the boundary condition
    Bound();
    
	// solve equation
	Integrate(s->GetTimeStep());
    
	// boundary condition
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
	for(j = 1; j < my - 1; j++)for(i = 1; i < mx - 1; i++){
		vv1 = Vec3d(-dh, 0.0, height[i][j] - height[i + 1][j    ]);
		vv2 = Vec3d(0.0, -dh, height[i][j] - height[i    ][j + 1]);
		normal[i][j] = Vec3d(-vv1.Z() * vv2.Y(), -vv1.X() * vv2.Z(), vv1.X() * vv2.Y());
		normal[i][j].unitize();
        
        // calculate refracted vectors
		tvec[i][j] = calRefracVec(normal[i][j], vlight, 1.3333);
    }
}

    // this method calcualtes heights and x-y velocities
void PHWater::Integrate(double dt){
    int i, j;
	const double hmul = 10.0;	//	高さを強調して描画
	const double hinv = 1.0 / hmul;
	const double pass = 300;
	static double h;			

    // calculate temporary velocities toward the z-axis for most of all cells
    for(i = 1; i <= mx - 2; i++)for(j = 1; j <= my - 2; j++){
        utmp[i][j] = u[i][j] - gravity * (dt / dh) * (height[i+1][j] - height[i][j]) * hinv + (p[i+1][j] - p[i][j]) / (density * dh);
        vtmp[i][j] = v[i][j] - gravity * (dt / dh) * (height[i][j+1] - height[i][j]) * hinv + (p[i][j+1] - p[i][j]) / (density * dh);
    }
	
	/*
		x = [i-1, i], y = [j-1, j]の四角領域の高さをh[i][j]とすると
		辺x = i-1からの流入量は
			流速u[i-1][j] * 辺dh * 水深depth * 時間dt
		同様にx = i, y = j - 1, y = jの流入出量を総和したものが四角領域の水量の変化量。
		これに四角領域の面積dh * dhを割れば高さの変化量が得られる

		水深を一定値(depth)としているがこれをdepth + height[i][j]としてみたらどうか。

		z方向流速を無視しているのはなぜ？
	 */

    // update temporal heights of almost all cells
    for(i = 1; i <= mx - 2; i++)for(j = 1; j <= my - 2; j++)
        htmp[i][j] = height[i][j] * hinv -
			depth * dt * ((utmp[i][j] - utmp[i-1][j]) * dh + (vtmp[i][j] - vtmp[i][j-1]) * dh) / (dh * dh);
    
	//	ローパスフィルタ
    for(i = 1; i < mx - 1; i++)for(j = 1; j < my - 1; j++){
		/*h = htmp[i][j] * (4 + pass) + 
			2.0 * (htmp[i-1][j  ] + htmp[i+1][j  ] + htmp[i  ][j-1] + htmp[i  ][j+1]) + 
				   htmp[i-1][j-1] + htmp[i+1][j-1] + htmp[i-1][j+1] + htmp[i+1][j+1];
		h /= (pass + 16);
		height[i][j] = h * loss * hmul;
        u[i][j] = utmp[i][j] * loss;
        v[i][j] = vtmp[i][j] * loss;*/

        // update variables
		height[i][j] = htmp[i][j] * hmul;
		u[i][j] = utmp[i][j];
		v[i][j] = vtmp[i][j];
    }
}

///////////////////////////////////////////////////////////////////////////
// PHWaterTrackTarget

SGOBJECTIMP(PHWaterTrackTarget, SGObject);

bool PHWaterTrackTarget::AddChildObject(SGObject* o, SGScene* s){
	if(DCAST(SGFrame, o)){
		targets.push_back((SGFrame*)o);
		return true;
	}
	if(DCAST(PHSolid, o)){
		PHSolid* solid = (PHSolid*)o;
		if(solid->GetFrame())
			targets.push_back(solid->GetFrame());
		return true;
	}
	return false;
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

DEF_RECORD(XWaterEngine, {
	GUID Guid(){ return WBGuid("a09e46e6-ed5b-4965-9ce6-34aa47f71265"); } 
});
	
DEF_RECORD(XWater, {
    GUID Guid(){ return WBGuid("ebb9188d-6c15-42aa-b15d-e1c89943ec0c"); } 
	WORD mx;
	WORD my;
	FLOAT dh;
	FLOAT depth;
	FLOAT gravity;
	FLOAT hscale;
	FLOAT density;
	FLOAT loss;
 });

DEF_RECORD(XWaterTrackTarget, {
	GUID Guid(){ return WBGuid("d1343031-d541-44f0-8ac7-d678837b65a6"); }
});

class PHWaterEngineLoader:public FIObjectLoader<PHWaterEngine>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHWaterEngine* engine){
		//ClearForceを探す→まだ無かったら生成、追加→自身をClearForceに登録
		UTRef<PHSolidClearForce> clearForce;
		ctx->scene->GetBehaviors().Find(clearForce);
		if(!clearForce){
			clearForce= new PHSolidClearForce;
			ctx->scene->GetBehaviors().Add(clearForce);
		}
        // insert the engine to the clearForce variable
		clearForce->solvers.push_back(engine);
		return true;
	}
    // ?
	PHWaterEngineLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_RECORD_PROTO(XWaterEngine);
	}
};

class PHWaterEngineSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWaterEngine"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		//保有するPHWaterをSave
		PHWaterEngine* engine = (PHWaterEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("PHWaterEngine", engine);

        // save doc to the array in the ctx class object
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
        
        // refer all phwater objects
		for(PHWaters::iterator it = engine->waters.begin(); it != engine->waters.end(); ++it){
			ctx->SaveRecursive(*it);
		}
        // delete the last element
		ctx->docs.pop_back();
	}
};
DEF_REGISTER_BOTH(PHWaterEngine);

class PHWaterLoader:public FIObjectLoader<PHWater>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHWater* water){
		XWater data;
		ctx->docs.Top()->GetWholeData(data);
		water->mx = data.mx;
		water->my = data.my;
		water->dh = data.dh;
		water->depth = data.depth;
		water->gravity = data.gravity;
		water->hscale = data.hscale;
		water->density = data.density;
		water->loss = data.loss;
		return true;
	}
	PHWaterLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(WORD);
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XWater);
	}
};

class PHWaterSaver:public FIBaseSaver{
	virtual UTString GetType() const{return "PHWater";}
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHWater* water = (PHWater*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("Water", water);
		ctx->docs.back()->AddChild(doc);
		XWater data;
		data.mx = water->mx;
		data.my = water->my;
		data.dh = (FLOAT)water->dh;
		data.depth = (FLOAT)water->depth;
		data.gravity = (FLOAT)water->gravity;
		data.hscale = (FLOAT)water->hscale;
		data.density = (FLOAT)water->density;
		data.loss = (FLOAT)water->loss;
		doc->SetWholeData(data);
		if(water->GetFrame()){
			doc->AddChild(ctx->CreateDocNode("REF", water->GetFrame()));
		}
		else if(water->GetSolid()){
			doc->AddChild(ctx->CreateDocNode("REF", water->GetSolid()));
		}
	}
};
DEF_REGISTER_BOTH(PHWater);

class PHWaterTrackTargetLoader:public FIObjectLoader<PHWaterTrackTarget>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHWaterTrackTarget* wtt){
		return true;
	}
	PHWaterTrackTargetLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_RECORD_PROTO(XWaterTrackTarget);
	}
};
class PHWaterTrackTargetSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWaterTrackTarget"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		//保有するPHWaterをSave
		PHWaterTrackTarget* wtt = (PHWaterTrackTarget*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("PHWaterTrackTarget", wtt);
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(SGFrames::iterator it = wtt->targets.begin(); it != wtt->targets.end(); ++it){
			ctx->SaveRecursive(*it);
		}
		ctx->docs.pop_back();
	}
};
DEF_REGISTER_BOTH(PHWaterTrackTarget);

}