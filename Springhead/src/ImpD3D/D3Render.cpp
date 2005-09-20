#include "ImpD3D.h"
#pragma hdrstop
#include <Graphics/GRCamera.h>
#include <Graphics/GRLight.h>
#include <Graphics/GRSphere.h>
namespace Spr {;
//----------------------------------------------------------------------------
//	D3TextureManager

WXINTFV(Direct3DTexture) D3TextureManager::Get(UTString filename){
	WXINTFV(Direct3DTexture) tex = textures[filename];
	if (!tex){
		D3DXCreateTextureFromFileEx(render->device, filename.c_str(), D3DX_DEFAULT, D3DX_DEFAULT,
				D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_FILTER_BOX, 0, NULL, NULL, &tex.Intf());
		textures[filename] = tex;
	}
	return tex;
}

//----------------------------------------------------------------------------
//	D3Render
SGOBJECTIMP(D3Render, GRRender);

D3Render::D3Render(){
	textureManager.render = this;
	worldMatrixStack.push_back(Affinef());
	nLights = 0;
}
void D3Render::Loaded(SGScene* scene){
	InitTree(scene->GetWorld(), scene);
//	scene->GetWorld()->Print(DSTR);
	if (!camera) camera = new GRCamera;
}
void D3Render::InitTree(SGFrame* fr, SGScene* scene){
	for(unsigned i=0; i<fr->contents.size(); ++i){
		GRMesh* gmesh = DCAST(GRMesh, fr->contents[i]);
		if (gmesh){
			UTRef<D3Mesh> mesh = new D3Mesh;
			fr->contents.Push(mesh);
			mesh->Set(gmesh, scene);
			continue;
		}
		GRSphere* gsphere = DCAST(GRSphere, fr->contents[i]);
		if (gsphere){
			UTRef<D3Sphere> sphere = new D3Sphere;
			fr->contents.Push(sphere);
			sphere->Set(gsphere, scene);
			continue;
		}
	}
	for(unsigned i=0; i<fr->Children().size(); ++i){
		InitTree(fr->Children()[i], scene);
	}
}
void D3Render::Render(SGScene* s){
	//	視点行列の設定
    if (camera) device->SetTransform(D3DTS_VIEW, (D3DMATRIX*)& camera->data.view);
	//	不透明部の描画
	drawState = DRAW_OPAQUE;
	RenderRecurse(s->GetWorld());
	s->GetBehaviors().Render(this, s);

	//	半透明部の描画
	drawState = DRAW_TRANSPARENT;
	WXCHECK(device->SetRenderState(D3DRS_ZWRITEENABLE, false));
	WXCHECK(device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	WXCHECK(device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));
	RenderRecurse(s->GetWorld());
	s->GetBehaviors().Render(this, s);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	drawState = DRAW_BOTH;
}
void D3Render::RenderRecurse(SGFrame* n){
	//	World行列の設定
	worldMatrixStack.push_back(worldMatrixStack.back() * n->GetPosture());
	device->SetTransform(D3DTS_WORLD, (const D3DMATRIX*)&worldMatrixStack.back());
	//	Visualと子フレームの描画
	GRRender::RenderRecurse(n);
	//	World行列を戻す
	worldMatrixStack.pop_back();
}
void D3Render::BeginScene(){
	device.BeginScene();


}

void D3Render::EndScene(){

device.EndScene();
}
void D3Render::ClearBuffer(){

	device.Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0),1.0f,0);
}

void D3Render::ClearColor(float r, float g, float b, float alpha) {
	int rc = 255*r;
	int gc = 255*g;
	int bc = 255*b;
	int a = 255*alpha;

	//device.Clear(0,NULL,D3DCLEAR_TARGET, D3DCOLOR_ARGB(a,rc,gc,bc),1.0f,0);

	
DWORD asave, srcsave, destsave;
device->GetRenderState(D3DRS_ALPHABLENDENABLE,&asave);
device->GetRenderState(D3DRS_SRCBLEND,&srcsave);
device->GetRenderState(D3DRS_DESTBLEND,&destsave);

//device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
// Set the source blend state.
//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
// Set the destination blend state.
//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

DSTR<<"TLVertex "<<a<<std::endl;
TLVertex tlv[6];
D3DVIEWPORT9 pViewport;
device->GetViewport(&pViewport);
tlv[0].x = 0; 
tlv[0].y = 0;
tlv[0].z = 0.5;
tlv[0].rhw = 0.5;
tlv[0].diffuse = D3DCOLOR_ARGB(a,rc,gc,bc);
tlv[1].x = 0; 
tlv[1].y =  pViewport.Height;
tlv[1].z = 0.99f;
tlv[1].rhw = 0.5;
tlv[1].diffuse = D3DCOLOR_ARGB(a,rc,gc,bc);
tlv[2].x = pViewport.Width; 
tlv[2].y = 0;
tlv[2].z = 0.99f;
tlv[2].rhw = 0.5;
tlv[2].diffuse = D3DCOLOR_ARGB(a,rc,gc,bc);
tlv[3]=tlv[2];
tlv[4] = tlv[1];


tlv[5].x = pViewport.Width; 
tlv[5].y = pViewport.Height;
tlv[5].z = 0.99f;
tlv[5].rhw = 0.5;
tlv[5].diffuse = D3DCOLOR_ARGB(a,rc,gc,bc);

device->SetFVF(VertexFVF);
device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 6 , tlv, sizeof(TLVertex));	


/*	IDirect3DSurface9* ppRenderTarget;
	device->GetRenderTarget(0,&ppRenderTarget);
	device->ColorFill(ppRenderTarget,NULL,D3DCOLOR_ARGB(a,rc,gc,bc));
	*/
//Restore previous state
device->SetRenderState(D3DRS_ALPHABLENDENABLE, asave);
device->SetRenderState(D3DRS_SRCBLEND, srcsave);
device->SetRenderState(D3DRS_DESTBLEND, destsave);

}


void D3Render::Present(){
	device.Present(NULL, NULL, NULL, NULL);
}
void D3Render::Present(HWND hw){
	RECT rcDst;
	GetClientRect(hw, &rcDst);
	RECT rcSrc;
	rcSrc.left = rcSrc.top = 0;
	rcSrc.right = rcDst.right - rcDst.left;
	rcSrc.bottom = rcDst.bottom - rcDst.top;
	device.Present(&rcSrc, &rcDst);
}

bool D3Render::Resize(Vec2f screen, HWND hw){
	if (device.Resize(screen, hw)){
    	Setup(screen);
    	return true;
    }
    return false;
}
bool D3Render::FitSurface(HWND hw){
	if (device.IsFullscreen()) return true;

	RECT rc;
	if (!hw) hw = device.presentParameters.hDeviceWindow;
	if (!hw) return false;
	GetClientRect(hw, &rc);
	Vec2f sz(float(rc.right-rc.left), float(rc.bottom-rc.top));
	if (sz != device.GetSurfaceSize() || hw != device.presentParameters.hDeviceWindow){
		device.Resize(sz, hw);
		Setup(sz);
	}
	return true;
}
bool D3Render::Create(void* arg){
	return device.Create((HWND)arg);
}
void D3Render::Fullscreen(){
	device.Fullscreen();
	SIZE sz = device.winMan.Size();
	Setup( Vec2f(sz.cx, sz.cy) );
	device->ShowCursor(false);
}
void D3Render::Window(){
	device.Window();
	SIZE sz = device.winMan.Size();
	Setup( Vec2f(sz.cx, sz.cy) );
	device->ShowCursor(true);
}
bool D3Render::SetViewport(void* arg){
	HWND hw = (HWND)arg;
	if (device.SetViewport(hw)){
		Setup(device.GetViewportSize());
		return true;
	}
	return false;
}
bool D3Render::SetViewport(Vec2f sz, Vec2f pos, float minZ, float maxZ){
	if (device.SetViewport(sz, pos, minZ, maxZ)){
		Setup(sz);
		return true;
	}
	return false;
}
void D3Render::Setup(Vec2f screen){
	if (!camera) return;
	WXCHECK(device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
	WXCHECK(device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE));
	WXCHECK(device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	WXCHECK(device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));

	//	射影行列の設定
	Affinef projectionMatrix;
	projectionMatrix = Affinef::ProjectionGL(Vec3f(camera->data.center.X(), camera->data.center.Y(), camera->data.front),
		Vec2f(camera->data.size.X(), camera->data.size.Y() * screen.Y()/screen.X() ), camera->data.front, camera->data.back);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&projectionMatrix);
	//	視点行列の設定
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX*)& camera->data.view);
	//	カリングの設定
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	//	アルファブレンド有効
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
}
HRESULT D3Render::SetFVF(DWORD fvf){
#if SPR_DXVER==8
	return device->SetVertexShader(D3DFVF_XYZ);
#elif SPR_DXVER==9
	device->SetVertexShader(NULL);
	return device->SetFVF(D3DFVF_XYZ);
#endif
}
void D3Render::SetModelMatrix(const Affinef& afw){
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&afw);
}
void D3Render::SetDepthTest(bool b){
	device->SetRenderState(D3DRS_ZENABLE, b ? D3DZB_TRUE : D3DZB_FALSE);
}
void D3Render::SetDepthFunc(TDepthFunc f){
	DWORD d3f;
	switch(f){
	case DF_NEVER:		d3f = D3DCMP_NEVER; break;
	case DF_LESS:		d3f = D3DCMP_LESS; break;
	case DF_EQUAL:		d3f = D3DCMP_EQUAL; break;
	case DF_LEQUAL:		d3f = D3DCMP_LESSEQUAL; break;
	case DF_GREATER:	d3f = D3DCMP_GREATER; break;
	case DF_NOTEQUAL:	d3f = D3DCMP_NOTEQUAL; break;
	case DF_GEQUAL:		d3f = D3DCMP_GREATEREQUAL; break;
	case DF_ALWAYS:		d3f = D3DCMP_ALWAYS; break;
	}
	device->SetRenderState(D3DRS_ZFUNC, d3f);
}
void D3Render::DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end){
	if (end <= begin) return;
	D3DPRIMITIVETYPE type;
	int nVtx=1;
	if (ty==POINTS){ type = D3DPT_POINTLIST; nVtx=1; }
	if (ty==LINES){ type = D3DPT_LINELIST; nVtx=2; }
	if (ty==TRIANGLES){ type = D3DPT_TRIANGLELIST; nVtx=3; }
	SetFVF(D3DFVF_XYZ);
	device->DrawPrimitiveUP(type, (end-begin)/nVtx, begin, sizeof(Vec3f));
}
void D3Render::DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx){
	D3DPRIMITIVETYPE type;
	int nVtx=1;
	if (ty==POINTS){ type = D3DPT_POINTLIST; nVtx=1; }
	if (ty==LINES){ type = D3DPT_LINELIST; nVtx=2; }
	if (ty==TRIANGLES){ type = D3DPT_TRIANGLELIST; nVtx=3; }
	SetFVF(D3DFVF_XYZ);
	device->DrawIndexedPrimitiveUP(type, 0, end-begin, (end-begin)/nVtx,
		begin, D3DFMT_INDEX32, vtx, sizeof(Vec3f));
}
void D3Render::DrawText(Vec2f pos, FIString str, const GRFont& font){
	WXINTF(D3DXFont) xf;
	HRESULT hr = D3DXCreateFont(device, font.height, font.width, font.weight,
	1, font.bItalic, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH|FF_DONTCARE, font.face.c_str(), &xf.Intf());
	if (!xf) return;
	RECT rc;
	HDC hdc = xf->GetDC();
	SIZE sz;
	GetTextExtentPoint32(hdc, str.c_str(), str.length(), &sz);
	rc.left = pos.X();
	rc.top = pos.Y();
	rc.right = rc.left + sz.cx;
	rc.bottom = rc.top + sz.cy;
	xf->DrawText(NULL, str.c_str(), str.length(), &rc, DT_LEFT, font.color);
}
Vec2f D3Render::GetTextExtent(FIString str, const GRFont& font){
	WXINTF(D3DXFont) xf;
	HRESULT hr = D3DXCreateFont(device, font.height, font.width, font.weight,
	1, font.bItalic, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
    DEFAULT_PITCH|FF_DONTCARE, font.face.c_str(), &xf.Intf());
	if (!xf) return Vec2f();
	HDC hdc = xf->GetDC();
	SIZE sz;
	GetTextExtentPoint32(hdc, str.c_str(), str.length(), &sz);
	return Vec2f(sz.cx, sz.cy);
}

void D3Render::SetMaterial(const GRMaterialData& m){
	D3DMATERIAL_SPR& mat = (D3DMATERIAL_SPR&)m;
	device->SetMaterial(&mat);
}
void D3Render::SetTexture(const char* fn){
	WXINTFV(Direct3DTexture) texture;
	if(fn) texture = textureManager.Get(fn);
	if (texture){
		WXCHECK(device->SetTexture(0, texture));
	}else{
		device->SetTexture(0, NULL);
	}
}
void D3Render::PushLight(const GRLightData& l){
	D3DLIGHT_SPR light;
	memset(&light, 0, sizeof(light));
	if (l.position.W() == 0) light.Type = D3DLIGHT_DIRECTIONAL;
	else if (l.spotCutoff == M_PI && l.spotInner == M_PI) light.Type=D3DLIGHT_POINT;
	else light.Type = D3DLIGHT_SPOT;
    light.Diffuse = (D3DCOLORVALUE&)l.diffuse;
    light.Specular = (D3DCOLORVALUE&)l.specular;
    light.Ambient = (D3DCOLORVALUE&)l.ambient;
    Vec3f pos = l.position.sub_vector(PTM::TSubVectorDim<0,3>());
	if (light.Type == D3DLIGHT_DIRECTIONAL){
		light.Direction = (D3DVECTOR&)-pos;
	}else{
		pos /= l.position.W();
		light.Position = (D3DVECTOR&)pos;
		light.Direction = (D3DVECTOR&)l.spotDirection;
		light.Range = l.range;
		light.Falloff = l.spotFalloff;
		light.Attenuation0 = l.attenuation0;
		light.Attenuation1 = l.attenuation1;
		light.Attenuation2 = l.attenuation2;
		light.Theta = l.spotInner;
		light.Phi = l.spotCutoff;
	}
	WXCHECK(device->SetLight(nLights, &light));
	WXCHECK(device->LightEnable(nLights, true));
	++nLights;
}
void D3Render::PopLight(){
	--nLights;
	WXCHECK(device->LightEnable(nLights, false));
}
void D3Render::SetLineWidth(float w){
	device->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&w));
}
bool D3Render::CanDraw(){
	return device.CanDraw();
}


Vec3f D3Render::getPointUnderPixel(int x, int y, bool& found, SGScene* scene){


	Vec3f res;
	D3DXVECTOR3 vPickRayDir;
	D3DXVECTOR3 vPickRayOrig;

	IDirect3DDevice9 * d3ddev = device.Intf();
    D3DXMATRIXA16 matProj;
    d3ddev->GetTransform( D3DTS_PROJECTION, &matProj );
    
	// Compute the vector of the pick ray in screen space
    D3DXVECTOR3 v;	
    IDirect3DSurface9 *ppBackBuffer;

	D3DSURFACE_DESC desc;
	
	d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &ppBackBuffer);    
	ppBackBuffer->GetDesc(&desc);
	ppBackBuffer->Release();
	
    v.x =  ( ( ( 2.0f * x ) / desc.Width  ) - 1 ) / matProj._11;
    v.y = -( ( ( 2.0f * y ) / desc.Height ) - 1 ) / matProj._22;
    v.z =  1.0f;

    // Get the inverse of the composite view and world matrix
    D3DXMATRIXA16 matView, matWorld, m;
    d3ddev->GetTransform( D3DTS_VIEW, &matView );
    d3ddev->GetTransform( D3DTS_WORLD, &matWorld );
	        
    m = matWorld * matView;
    D3DXMatrixInverse( &m, NULL, &m );

    // Transform the screen space pick ray into 3D space
    vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig.x = m._41;
    vPickRayOrig.y = m._42;
    vPickRayOrig.z = m._43;
	// Use inverse of matrix
	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse,NULL,&matWorld);
	
	// Transform ray origin and direction by inv matrix
	D3DXVECTOR3 rayObjOrigin;
	D3DXVECTOR3	rayObjDirection;

	D3DXVec3TransformCoord(&rayObjOrigin,&vPickRayOrig,&matInverse);
	D3DXVec3TransformNormal(&rayObjDirection,&vPickRayDir,&matInverse);
	D3DXVec3Normalize(&rayObjDirection,&rayObjDirection);

	//We can now call the intersect function on our untransformed graphic mesh data:	
	BOOL hasHit;
	int nbObj = scene->NChildObjects();
	DSTR<<"NB objects :"<<nbObj<<std::endl;
	float minDistanceToCollision=-1;
	float distanceToCollision=-1;
	for (int i = 0; i < nbObj; i++) {
		hasHit = false;		
		SGObject* obj = scene->ChildObject(i);
		//TODO : write ray/ collision detection engine intersection proc and use it here
		
		
		
	}
	if (minDistanceToCollision >= 0) {
		D3DXVECTOR3 result = rayObjOrigin + distanceToCollision * rayObjDirection;
		res.x = result.x;
		res.y = result.y;
		res.z = result.z;
	}


	
//this method does not work if the zbuffer format is not 16... so bad idea!!!
/*	IDirect3DDevice9 * d3ddev = device.Intf();
	IDirect3DSurface9 *ppZStencilSurface;
	d3ddev->GetDepthStencilSurface(&ppZStencilSurface);
	D3DLOCKED_RECT lockedRect;
	RECT rect;
	rect.left=x;
	rect.top=y;
	rect.right=x+1;
	rect.bottom=y+1;

    ppZStencilSurface->LockRect(&lockedRect, &rect, D3DLOCK_READONLY);
	
	D3DXFLOAT16*  zvalues= (D3DXFLOAT16*) lockedRect.pBits;
	DSTR<<"Pitch :"<<lockedRect.Pitch<<std::endl;
	//DSTR<<"Depth :"<<zvalues[0].FLOAT()<<std::endl;
    ppZStencilSurface->UnlockRect();
	ppZStencilSurface->Release();
	
	D3DXVECTOR3 p2d;
	p2d.x=x;
	p2d.y=y;
	p2d.z=zvalues[0];

	
	D3DXMATRIXA16 matView, matWorld, matProj;
	D3DVIEWPORT9 pViewport;

    d3ddev->GetTransform( D3DTS_PROJECTION, &matProj );    
    d3ddev->GetTransform( D3DTS_VIEW, &matView );
    d3ddev->GetTransform( D3DTS_WORLD, &matWorld );
	d3ddev->GetViewport(&pViewport);
	
	D3DXVECTOR3 p3d;
	
	D3DXVec3Unproject(&p3d, &p2d, &pViewport, &matProj, &matView,  &matWorld);*/

	
	DSTR<<"res :"<<res<<std::endl;
	return res;
}



}	//	Spr

