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
	bound.x = 3;
	bound.y = 3;
}

    // this function adds new child data to the end of the vector
    // object of SGFrame or PHSolid can be inserted
    // this function returns the result of insertion as a boolean
bool PHWater::AddChildObject(SGObject* o, SGScene* s){
	if(DCAST(PHWaterTrackTarget, o)){
		targets = (PHWaterTrackTarget*)o;
		return true;
	}
	if(DCAST(GRMaterial, o)){
		material = (GRMaterial*)o;
		return true;
	}
	return false;
}

    // refer the object
    // if object exists
    // this function returns 1, otherwise returns 0
size_t PHWater::NReferenceObjects(){
	return 0;
}

    // refer the object
    // if i is not 0, return NULL
    // if i is zero and object exists,
    // return object
SGObject* PHWater::ReferenceObject(size_t i){
	return NULL;
}

    // this method calls the init function with the given scene
void PHWater::Loaded(SGScene* scene){
	Init(scene);
}

    // this function initializes local variables
void PHWater::Init(SGScene* scene){
    //xo = -(MX - 1) / 2. * dh;	//x���_	
    //yo = -(MY - 1) / 2. * dh;	//y���_
	int i, j;

    // total number of cells
	mxy = mx * my;

    // dh = width of grid
    // mx, my = number of divisions
	dhinv = 1/dh;
	rx = (mx-1) * dh / 2.0;
	ry = (my-1) * dh / 2.0;

	//���̓�bad_alloc��catch����
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
	height[8][8] = 1.0;

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

	//�}�e���A���̎w�肪�Ȃ������ꍇ�A�f�t�H���g���쐬
	if(!material){
		material = new GRMaterial;
		material->ambient  = Vec4d(0.5, 0.5, 0.5, 1.0);
		material->specular = Vec4d(0.5, 0.5, 0.5, 1.0);
		material->diffuse  = Vec4d(1.0, 1.0, 1.0, 1.0);
		material->emissive = Vec4d(0.5, 0.5, 0.5, 1.0);
		material->power = 0.5;
		//vlight = Vec3d(0.0, 0.0, -1.0);
	}
	//	�`��p�̒��_
/*	vtxs.resize(mxy);
	for(int x = 0; x < mx - 1; x++)for(int y = 0; y < my - 1; y++){
		vtxs[y*mx + x].pos = Vec3f(x*dh, y*dh, 0);
		vtxs[y*mx + x].normal = Vec3f(0, 0, 0);
	}
*/	//�}�e���A��
	materialD3 = new D3Material;
	*materialD3 = *material;
}

double PHWater::LerpHeight(double x, double y){
	static double nx, ny, h0, h1;

    if(x < -rx || x > rx || y < -ry || y > ry)
		return 0.0;

	//���W����i�q�C���f�b�N�X���Z�o
	nx = (x + rx) / dh;
	ny = (y + ry) / dh;
	int ix = floor(nx), iy = floor(ny);
	nx -= (double)ix;
	ny -= (double)iy;
	ix = (ix+bound.x)%mx, iy = (iy+bound.y)%my;
    
    h0 = height[ix       ][iy] * (1.0 - ny) + height[ix       ][(iy+1)%my] * ny;
    h1 = height[(ix+1)%mx][iy] * (1.0 - ny) + height[(ix+1)%mx][(iy+1)%my] * ny;

    return h0 * (1.0 - nx) + h1 * nx;
}

void PHWater::Render(SGFrame* fr, GRRender* render){
	//render�̎�ނ𔻒�	
	if (render->drawState&GRRender::DRAW_OPAQUE == 0) return;
	if(DCAST(D3Render, render))
		RenderD3(fr, DCAST(D3Render, render));
	if(DCAST(GLRender, render))
		RenderGL(fr, DCAST(GLRender, render));
}

DWORD PHWater::GetColor(float h){
	float hMax = dh/4;
	float hMin = -dh/4;
	if (h>hMax) h = hMax;
	if (h<hMin) h = hMin;
	float n = h / ((hMax-hMin)/2);
	bool bPlus = n>0;
	if (!bPlus) n *= -1;
	int a = n*255;
	int b = (1-n)*255;
	if (bPlus){
		return D3DCOLOR_ARGB(0xFF,a,b,0);
	}
	return D3DCOLOR_ARGB(0xFF,0,b,a);
}
// draw by Direct3D
void PHWater::RenderD3(SGFrame* fr, D3Render* render){
    // copy the head address of the varialbe height array to pheight
	double* pheight = &height[0][0];
    // copy the head address of the variable normal array to pnormal
	Vec3d*  pnormal = &normal[0][0];
	
	if ( (materialD3->bOpaque && render->drawState & GRRender::DRAW_OPAQUE)
		|| (!materialD3->bOpaque && render->drawState & GRRender::DRAW_TRANSPARENT) ){
#if 0	//	�ʏ�̕`��
        // this function sets the texture
		materialD3->Render(fr, render);
		
		render->device->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2);
		struct VtxFVF{
			Vec3f pos;
			Vec3f normal;
			Vec2f tex;
		};
		const float hmul = 10.0f;
		VtxFVF* buf= new VtxFVF[mx*2];
	    float xo = -rx, yo = -ry;
		float dxinv = 1/rx;
		float dyinv = 1/ry;
		int boundY_1 = (bound.y-1+my)%my;
		for(int y=0; y<my; ++y){
			double py;
			if (y < boundY_1) py = yo + (y-bound.y+my)%my*dh; 
			else if (y>boundY_1) py = yo + (y-bound.y+my)%my*dh;
			else continue;
			int start1 = y*mx;
			int start2 = ((y+1)%my) * mx;
			double left = xo;
			double px = xo;
			for(int x=bound.x; x<mx; ++x){
				buf[(x-bound.x)*2+1].pos	= Vec3f(px, py, pheight[start1+x]*hmul);
				buf[(x-bound.x)*2+1].normal	= pnormal[start1+x];
				buf[(x-bound.x)*2].pos		= Vec3f(px, py+dh, pheight[start2+x]*hmul);
				buf[(x-bound.x)*2].normal	= pnormal[start2+x];
				px += dh;
			}
			int offset = mx - bound.x;
			for(int x=0; x<bound.x; ++x){
				buf[(x+offset)*2+1].pos		= Vec3f(px, py, pheight[start1+x]*hmul);
				buf[(x+offset)*2+1].normal	= pnormal[start1+x];
				buf[(x+offset)*2].pos		= Vec3f(px, py+dh, pheight[start2+x]*hmul);
				buf[(x+offset)*2].normal	= pnormal[start2+x];
				px += dh;
			}
			const float nmul = 1.0;
			for(int i=0; i<2*mx; ++i){
				buf[i].tex.x = (buf[i].pos.x+texOffset.x*dh)*dxinv/4
					+  buf[i].normal.x*nmul + 0.5f;
				buf[i].tex.y = (buf[i].pos.y+texOffset.y*dh)*dyinv/4
					+  buf[i].normal.y*nmul + 0.5f;
			}
			render->device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (mx-1)*2, buf, sizeof(buf[0]));
		}	
		delete buf;
#else //	�F�ň��͂�\��
		render->SetMaterial(GRMaterialData(Vec4f(0,0,0,1),0));
		render->device->SetRenderState(D3DRS_LIGHTING, false);
		render->device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
		struct VtxFVF{
			Vec3f pos;
			DWORD color;
		};
		VtxFVF* buf= new VtxFVF[mx*2];
	    float xo = -rx, yo = -ry;
		float dxinv = 1/rx;
		float dyinv = 1/ry;
		int boundY_1 = (bound.y-1+my)%my;
		for(int y=0; y<my; ++y){
			double py;
			if (y < boundY_1) py = yo + (y-bound.y+my)%my*dh; 
			else if (y>boundY_1) py = yo + (y-bound.y+my)%my*dh;
			else continue;
			int start1 = y*mx;
			int start2 = ((y+1)%my) * mx;
			double left = xo;
			double px = xo;
			for(int x=bound.x; x<mx; ++x){
				buf[(x-bound.x)*2+1].pos	= Vec3f(px, py, 0);
				buf[(x-bound.x)*2+1].color	= GetColor(pheight[start1+x]);
				buf[(x-bound.x)*2].pos		= Vec3f(px, py+dh, 0);
				buf[(x-bound.x)*2].color	= GetColor(pheight[start2+x]);
				px += dh;
			}
			int offset = mx - bound.x;
			for(int x=0; x<bound.x; ++x){
				buf[(x+offset)*2+1].pos		= Vec3f(px, py, 0);
				buf[(x+offset)*2+1].color	= GetColor(pheight[start1+x]);
				buf[(x+offset)*2].pos		= Vec3f(px, py+dh, 0);
				buf[(x+offset)*2].color		= GetColor(pheight[start2+x]);
				px += dh;
			}
			render->device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (mx-1)*2, buf, sizeof(buf[0]));
		}	
		render->device->SetRenderState(D3DRS_LIGHTING, true);
		delete buf;		
#endif
		render->SetDepthTest(false);
		GRMaterialData mate(Vec4f(1,0,0,1), 4);
		render->SetMaterial(mate);
		std::vector<Vec3f> lines;
		lines.push_back(Vec3f(-rx, -ry, 0));
		lines.push_back(Vec3f(rx, -ry, 0));
		lines.push_back(Vec3f(rx, -ry, 0));
		lines.push_back(Vec3f(rx, ry, 0));
		lines.push_back(Vec3f(rx, ry, 0));
		lines.push_back(Vec3f(-rx, ry, 0));
		lines.push_back(Vec3f(-rx, ry, 0));
		lines.push_back(Vec3f(-rx, -ry, 0));
		render->DrawDirect(GRRender::LINES, &*(lines.begin()), &*(lines.end()));
		bool bDrawVelocity = true;
		lines.resize(2);
		if (bDrawVelocity){
			render->device->SetRenderState(D3DRS_LIGHTING, false);
			render->device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
			struct VtxFVF{
				Vec3f pos;
				DWORD color;
			} vtx[2];
			vtx[0].color = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff);
			vtx[1].color = D3DCOLOR_ARGB(0x10, 0xff, 0xff, 0xff);
			for(int iy=0; iy<my; ++iy){
				for(int ix=0; ix<mx; ++ix){
					vtx[0].pos = Vec3f((ix+0.5f)*dh-rx, (iy+0.5f)*dh-ry, 0);
					int cx = (ix+bound.x)%mx;
					int cy = (iy+bound.y)%my;
					Vec2f vel(u[cx][cy], v[cx][cy]);
					vel += velocity;
					vel *= dh;
					vtx[1].pos = Vec3f((ix+0.5f)*dh-rx + vel.x, (iy+0.5f)*dh-ry + vel.y, 0);
					render->device->DrawPrimitiveUP(D3DPT_LINELIST, 2, vtx, sizeof(vtx[0]));
				}
			}
			render->device->SetRenderState(D3DRS_LIGHTING, true);
		}
		
		render->SetDepthTest(true);
		//	�e�N�X�`����߂��D
		render->device->SetTexture(0,NULL);
	}
}

// draw objects by OpenGL
void PHWater::RenderGL(SGFrame* fr, GLRender* render){
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
        material->Render(fr, render);
        }
    
    //	if(!material->IsOpaque() && render->drawState & GRRender::DRAW_TRANSPARENT){
        material->Render(fr, render);

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
	//	�ǂ����� bx1 �� +1�Ȃ̂��Ȃ��D bx1 ��bound.x�ŗǂ��C������̂����ǁD
	//	���������_�����O���ςȂ̂ł��傤���ǁC�����s���D
	int bx1 = (bound.X()+1) % mx;
	int bx1_ = (bound.X()+2) % mx;
	int bx2 = bound.X();
	int bx2_ = (bound.X()-1+mx) % mx;

	int by1 = (bound.Y()+1) % my;
	int by1_ = (bound.Y()+2) % my;
	int by2 = bound.Y() %my;
	int by2_ = (bound.Y()+my-1) % my;
	
	//�s������x�����A�������y�����Ȃ��Ƃɒ���
	u.row(bx1).clear();
	u.row(bx2).clear();
    u.col(by1) = u.col(by1_);
	u.col(by2) = u.col(by2_);

	v.row(bx1) = v.row(bx1_);
	v.row(bx2) = v.row(bx2_);
    v.col(by1).clear();
	v.col(by2).clear();

    // update the height of the wave
	height.row(bx1) = height.row(bx1_);
	height.row(bx2) = height.row(bx2_);
	height.col(by1) = height.col(by1_);
	height.col(by2) = height.col(by2_);
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
	
	double dt = s->GetTimeStep();
	Affinef posture = GetPosture();

	Vec3f velW = posture.Rot() * Vec3f(velocity.x, velocity.y, 0);
	posture.Pos() += velW * dt;
	
	if (targets && targets->targets.size()){
		SGFrame* target = targets->targets[0];
		Affinef af = posture.inv() * target->GetWorldPosture();
		Vec2f diff = af.Pos().sub_vector(0, Vec2f());
		if (diff.X() > dh){
			posture.Pos() += posture.Rot() * Vec3f(dh, 0, 0);
			bound.x = (bound.x+1) % mx;
			texOffset.x ++;
		}
		if (diff.X() < -dh){
			posture.Pos() -= posture.Rot() * Vec3f(dh, 0, 0);
			bound.x = (bound.x-1+mx) % mx;
			texOffset.x --;
		}
		if (diff.Y() > dh){
			posture.Pos() += posture.Rot() * Vec3f(0, dh, 0);
			bound.y = (bound.y+1) % my;
			texOffset.y ++;
		}
		if (diff.Y() < -dh){
			posture.Pos() -= posture.Rot() * Vec3f(0, dh, 0);
			bound.y = (bound.y-1+my) % my;
			texOffset.y --;
		}
	}
	frame->SetWorldPosture(posture);

    // set the boundary condition
    Bound();
    
	// solve equation
	Integrate(dt);
    
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

	//�@���Ƌ��܃x�N�g�����v�Z
	for(j = 0; j < my - 1; j++)for(i = 0; i < mx - 1; i++){
		vv1 = Vec3d(-dh, 0.0, height[i][j] - height[i + 1][j    ]);
		vv2 = Vec3d(0.0, -dh, height[i][j] - height[i    ][j + 1]);
		normal[i][j] = (vv1 ^ vv2).unit();        
        // calculate refracted vectors
		tvec[i][j] = calRefracVec(normal[i][j], vlight, 1.3333);
    }
	for(j = 0; j < my - 1; j++){
		vv1 = Vec3d(-dh, 0.0, height[mx-1][j] - height[0][j    ]);
		vv2 = Vec3d(0.0, -dh, height[mx-1][j] - height[mx-1    ][j + 1]);
		normal[mx-1][j] = (vv1 ^ vv2).unit();        
        // calculate refracted vectors
		tvec[mx-1][j] = calRefracVec(normal[mx-1][j], vlight, 1.3333);
	}
	for(i = 0; i < mx - 1; i++){
		vv1 = Vec3d(-dh, 0.0, height[i][my-1] - height[i + 1][my-1    ]);
		vv2 = Vec3d(0.0, -dh, height[i][my-1] - height[i    ][0]);
		normal[i][my-1] = (vv1 ^ vv2).unit();        
        // calculate refracted vectors
		tvec[i][my-1] = calRefracVec(normal[i][my-1], vlight, 1.3333);
	}
	vv1 = Vec3d(-dh, 0.0, height[mx-1][my-1] - height[0][my-1    ]);
	vv2 = Vec3d(0.0, -dh, height[mx-1][my-1] - height[mx-1    ][0]);
	normal[mx-1][my-1] = (vv1 ^ vv2).unit();        
    // calculate refracted vectors
	tvec[mx-1][my-1] = calRefracVec(normal[mx-1][my-1], vlight, 1.3333);
}

    // this method calcualtes heights and x-y velocities
void PHWater::Integrate(double dt){
    int i, j;
	//	�Z���̓g�[���X��ɂȂ����Ă���ƍl����(��Ɖ��C�E�ƍ��͂Ȃ����Ă���)
    // calculate temporary velocities toward the z-axis
	for(i = 0; i < mx; i++)for(j = 0; j < my; j++){
        utmp[i][j] = loss*(u[i][j] - gravity * (dt / dh) * 
			0.5*(height[(i+1)%mx][j] - height[i][j] + height[(i+1)%mx][(j+1)%my] - height[i][(j+1)%my]));
        vtmp[i][j] = loss*(v[i][j] - gravity * (dt / dh) * 
			0.5*(height[i][(j+1)%my] - height[i][j] + height[(i+1)%mx][(j+1)%my] - height[(i+1)%mx][j]));
		//utmp[i][j] = loss*(u[i][j] - gravity * (dt / dh) * (height[i][j] - height[i == 0 ? mx-1 : i-1/*(i-1)%mx*/][j]) + (p[i][j] - p[i==0 ? mx-1 : i-1][j]) / (density * dh));
		//vtmp[i][j] = loss*(v[i][j] - gravity * (dt / dh) * (height[i][j] - height[i][j==0 ? my-1:j-1]) + (p[i][j] - p[i][j==0?my-1:j-1]) / (density * dh));
    }
	int hoge = -2 % 6;
	//	last row refers first row
    /*for(i = 0; i < mx-1; i++){
        utmp[i][my-1] = loss*(u[i][my-1] - gravity * (dt / dh) * (height[i+1][my-1] - height[i][my-1]) + (p[i+1][my-1] - p[i][my-1]) / (density * dh));
        vtmp[i][my-1] = loss*(v[i][my-1] - gravity * (dt / dh) * (height[i][0] - height[i][my-1]) + (p[i][0] - p[i][my-1]) / (density * dh));
    }
	//	last column refers first column
	for(j = 0; j < my-1; j++){
        utmp[mx-1][j] = loss*(u[mx-1][j] - gravity * (dt / dh) * (height[0][j] - height[mx-1][j]) + (p[0][j] - p[mx-1][j]) / (density * dh));
        vtmp[mx-1][j] = loss*(v[mx-1][j] - gravity * (dt / dh) * (height[mx-1][j+1] - height[mx-1][j]) + (p[mx-1][j+1] - p[mx-1][j]) / (density * dh));
    }
	//	right bottom cell
    utmp[mx-1][my-1] = loss*(u[mx-1][my-1] - gravity * (dt / dh) * (height[0][my-1] - height[mx-1][my-1]) + (p[0][my-1] - p[mx-1][my-1]) / (density * dh));
    vtmp[mx-1][my-1] = loss*(v[mx-1][my-1] - gravity * (dt / dh) * (height[mx-1][0] - height[mx-1][my-1]) + (p[mx-1][0] - p[mx-1][my-1]) / (density * dh));
	*/
	/*
		x = [i-1, i], y = [j-1, j]�̎l�p�̈�̍�����h[i][j]�Ƃ����
		��x = i-1����̗����ʂ�
			����u[i-1][j] * ��dh * ���[depth * ����dt
		���l��x = i, y = j - 1, y = j�̗����o�ʂ𑍘a�������̂��l�p�̈�̐��ʂ̕ω��ʁB
		����Ɏl�p�̈�̖ʐ�dh * dh������΍����̕ω��ʂ�������

		���[�����l(depth)�Ƃ��Ă��邪�����depth + height[i][j]�Ƃ��Ă݂���ǂ����B

		z���������𖳎����Ă���̂͂Ȃ��H
	 */

    // update temporal heights of all cells
    for(i = 0; i <mx; i++)for(j = 0; j<my; j++){
        htmp[i][j] = (height[i][j] -
			depth * dt * (0.5*(utmp[i][j] - utmp[i-1][j] + utmp[i][j-1] - utmp[i-1][j-1]) * dh 
			+ 0.5*(vtmp[i][j] - vtmp[i][j-1] + vtmp[i-1][j] - vtmp[i-1][j-1]) * dh) / (dh * dh) );
		//htmp[i][j] = (height[i][j] -
		//	depth * dt * ((utmp[(i+1)%mx][j] - utmp[i][j]) * dh + (vtmp[i][(j+1)%my] - vtmp[i][j]) * dh) / (dh * dh) );
	}
    /*for(i = 1; i <mx; i++){
        htmp[i][0] = (height[i][0] -
			depth * dt * ((utmp[i][0] - utmp[i-1][0]) * dh + (vtmp[i][0] - vtmp[i][my-1]) * dh) / (dh * dh) );
	}
    for(j = 1; j<my; j++){
        htmp[0][j] = (height[0][j] -
			depth * dt * ((utmp[0][j] - utmp[mx-1][j]) * dh + (vtmp[0][j] - vtmp[0][j-1]) * dh) / (dh * dh) );
	}
    htmp[0][0] = (height[0][0] -
		depth * dt * ((utmp[0][0] - utmp[mx-1][0]) * dh + (vtmp[0][0] - vtmp[0][my-1]) * dh) / (dh * dh) );
    */
	/*
	const double pass = 300;
	static double h;			
	//	���[�p�X�t�B���^
    for(i = 1; i < mx - 1; i++)for(j = 1; j < my - 1; j++){
		h = htmp[i][j] * (4 + pass) + 
			2.0 * (htmp[i-1][j  ] + htmp[i+1][j  ] + htmp[i  ][j-1] + htmp[i  ][j+1]) + 
				   htmp[i-1][j-1] + htmp[i+1][j-1] + htmp[i-1][j+1] + htmp[i+1][j+1];
		h /= (pass + 16);
		height[i][j] = h * loss * hmul;
        u[i][j] = utmp[i][j] * loss;
        v[i][j] = vtmp[i][j] * loss;
    }
	*/

	// update variables
	memcpy(&height[0][0], &htmp[0][0], sizeof(height[0][0])*mxy);
	memcpy(&u[0][0], &utmp[0][0], sizeof(u[0][0])*mxy);
	memcpy(&v[0][0], &vtmp[0][0], sizeof(v[0][0])*mxy);
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
	DWORD mx;
	DWORD my;
	FLOAT dh;
	FLOAT depth;
	FLOAT gravity;
	FLOAT hscale;
	FLOAT density;
	FLOAT loss;
	FLOAT vx;
	FLOAT vy;
 });

DEF_RECORD(XWaterTrackTarget, {
	GUID Guid(){ return WBGuid("d1343031-d541-44f0-8ac7-d678837b65a6"); }
});

class PHWaterEngineLoader:public FIObjectLoader<PHWaterEngine>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHWaterEngine* engine){
		//ClearForce��T�����܂����������琶���A�ǉ������g��ClearForce�ɓo�^
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
		//�ۗL����PHWater��Save
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
		water->velocity.x = data.vx;
		water->velocity.y = data.vy;
		water->frame = DCAST(SGFrame, ctx->objects.Top());
		return true;
	}
	PHWaterLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(DWORD);
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
		data.vx = water->velocity.x;
		data.vy = water->velocity.y;
		doc->SetWholeData(data);
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
		//�ۗL����PHWater��Save
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