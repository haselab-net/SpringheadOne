#include "GraphicsGL.h"
#include <Graphics/GRLoadBmp.h>
#include <Base/BaseDebug.h>

#pragma hdrstop


namespace Spr {;

struct StringLess{
	bool operator () (const std::string& s1, const std::string& s2) const {
		return strcmp(s1.c_str(), s2.c_str()) < 0;
	}
};
static const GLenum	pxfm[] = {GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_BGR_EXT, GL_BGRA_EXT};
int GLTextureManager::Get(const UTString& fn){
	Texs::iterator it = texs.find(fn);
	if ( it == texs.end() ){	//	新しいテクスチャの場合
		char *texbuf = NULL;
		int	tx=0, ty=0, nc=0;
		
		//	paintLib でファイルをロード
		int h = LoadBmpCreate(fn.c_str());
		tx = LoadBmpGetWidth(h);
		ty = LoadBmpGetHeight(h);
		nc = LoadBmpGetBytePerPixel(h);
		texbuf = new char[tx*ty*nc];
		LoadBmpGetBmp(h, texbuf);
		LoadBmpRelease(h);
		
		//	テクスチャの生成
		unsigned int texId=0;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, nc, tx, ty, pxfm[nc - 1], GL_UNSIGNED_BYTE, texbuf);
		delete texbuf;
		texs[fn] = texId;
		return texId;
	}else{	//	ロード済みの場合
		return it->second;
	}
	return 0;
}


SGOBJECTIMP(GLRender, GRRender);

GLRender::GLRender(){
	nLights = 0;
}
GLRender::~GLRender(){
}
///	レンダリング(視点を含む)
void GLRender::Render(SGScene* s){
	//	視点行列の設定
    if (camera){
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(camera->data.view);
	}
	//	不透明部の描画
	drawState = DRAW_OPAQUE;
	//glDepthMask(GL_TRUE);
	glBlendFunc(GL_ONE, GL_ZERO);
	RenderRecurse(s->GetWorld());
	//	Engineの描画
	s->GetBehaviors().Render(this, s);
	//	半透明部の描画
	drawState = DRAW_TRANSPARENT;
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderRecurse(s->GetWorld());
	
	glDepthMask(GL_TRUE);
	drawState = DRAW_BOTH;
	glBlendFunc(GL_ONE, GL_ZERO);
}
///	レンダリング(再帰部分)
void GLRender::RenderRecurse(SGFrame* n){
	//	World行列の設定
	glPushMatrix();
	glMultMatrixf(n->GetPosture());
	//	Visualと子フレームの描画
	GRRender::RenderRecurse(n);
	//	World行列を戻す
	glPopMatrix();
}
///	バッファクリア
void GLRender::ClearBuffer(){
	glClearColor(0,0,0,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
///	レンダリングの開始前に呼ぶ関数
void GLRender::BeginScene(){
}
///	レンダリングの終了後に呼ぶ関数
void GLRender::EndScene(){
	glFlush();
}
void GLRender::Resize(Vec2f screen){
	glViewport(0,0, (size_t)screen.X(), (size_t)screen.Y());
	Affinef afProj;
	afProj = Affinef::ProjectionGL(Vec3f(camera->data.center.X(), camera->data.center.Y(), camera->data.front),
		Vec2f(camera->data.size.X(), camera->data.size.Y() * screen.Y()/screen.X() ), camera->data.front, camera->data.back);
	SetProjectionMatrix(afProj);
}

void GLRender::SetViewMatrix(const Affinef& afv){
	if (camera) camera->data.view = afv;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->data.view * afModel);
}
void GLRender::SetModelMatrix(const Affinef& afw){
	afModel = afw;
	glMatrixMode(GL_MODELVIEW);
	if (camera){
		glLoadMatrixf(camera->data.view * afModel);
	}else{
		glLoadMatrixf(afModel);
	}
}
void GLRender::SetProjectionMatrix(const Affinef& afp){
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(afp);
	glMatrixMode(GL_MODELVIEW);
}
void GLRender::DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end){
	GLenum mode;
	if (ty == POINTS) mode = GL_POINTS;
	else if (ty == LINES) mode = GL_LINES;
	else if (ty == TRIANGLES) mode = GL_TRIANGLES;
	else assert(0);
	glBegin(mode);
	for(;begin!=end; ++begin) glVertex3fv(*begin);
	glEnd();
}
void GLRender::DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx){
	GLenum mode;
	if (ty == POINTS) mode = GL_POINTS;
	else if (ty == LINES) mode = GL_LINES;
	else if (ty == TRIANGLES) mode = GL_TRIANGLES;
	else assert(0);
	glBegin(mode);
	for(;begin!=end; ++begin) glVertex3fv(vtx[*begin]);
	glEnd();
}
void GLRender::SetTexture(const char* fn){
	if (fn){
		int texId = textureManager.Get(fn);
		if (texId){
			glBindTexture(GL_TEXTURE_2D, texId);
			glEnable(GL_TEXTURE_2D);
		}
	}else{
		glDisable(GL_TEXTURE_2D);
	}
}
void GLRender::SetMaterial(const GRMaterialData& m){
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m.ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m.specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m.emissive);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m.power);
}
void GLRender::PushLight(const GRLightData& l){
	if (nLights < GL_MAX_LIGHTS){
		glLightfv(GL_LIGHT0+nLights, GL_AMBIENT, l.ambient);
		glLightfv(GL_LIGHT0+nLights, GL_DIFFUSE, l.diffuse);
		glLightfv(GL_LIGHT0+nLights, GL_SPECULAR, l.specular);
		glLightfv(GL_LIGHT0+nLights, GL_POSITION, l.position);
		if (l.position.W()){
			glLightf(GL_LIGHT0+nLights, GL_CONSTANT_ATTENUATION, l.attenuation0);
			glLightf(GL_LIGHT0+nLights, GL_LINEAR_ATTENUATION, l.attenuation1);
			glLightf(GL_LIGHT0+nLights, GL_QUADRATIC_ATTENUATION, l.attenuation2);
			glLightfv(GL_LIGHT0+nLights, GL_SPOT_DIRECTION, l.spotDirection);
			glLightf(GL_LIGHT0+nLights, GL_SPOT_EXPONENT, l.spotFalloff);
			glLightf(GL_LIGHT0+nLights, GL_SPOT_CUTOFF, l.spotCutoff);
		}
		glEnable(GL_LIGHT0+nLights);
	}
	nLights ++;
}
void GLRender::PopLight(){
	nLights --;
	if (nLights < GL_MAX_LIGHTS) glDisable(GL_LIGHT0+nLights);
}
void GLRender::SetLineWidth(float w){
	glLineWidth(w);
}
void GLRender::SetDepthTest(bool b){
	if (b) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
}
void GLRender::SetDepthFunc(TDepthFunc f){
	DWORD glf;
	switch(f){
	case DF_NEVER:		glf = GL_NEVER; break;
	case DF_LESS:		glf = GL_LESS; break;
	case DF_EQUAL:		glf = GL_EQUAL; break;
	case DF_LEQUAL:		glf = GL_LEQUAL; break;
	case DF_GREATER:	glf = GL_GREATER; break;
	case DF_NOTEQUAL:	glf = GL_NOTEQUAL; break;
	case DF_GEQUAL:		glf = GL_GEQUAL; break;
	case DF_ALWAYS:		glf = GL_ALWAYS; break;
	}
	glDepthFunc(glf);
}
bool GLRender::CanDraw(){
	return true;
}
void GLRender::Loaded(SGScene* scene){
	InitTree(scene->GetWorld(), scene);
//	scene->GetWorld()->Print(DSTR);
}
void GLRender::InitTree(SGFrame* fr, SGScene* scene){
	for(unsigned i=0; i<fr->contents.size(); ++i){
		GRMesh* gmesh = DCAST(GRMesh, fr->contents[i]);
		if (gmesh){
			UTRef<GLMesh> mesh = new GLMesh;
			fr->contents.Push(mesh);
			mesh->Set(gmesh, scene);
			continue;
		}
		GRSphere* gsphere = DCAST(GRSphere, fr->contents[i]);
		if (gsphere){
			UTRef<GLSphere> sphere = new GLSphere;
			fr->contents.Push(sphere);
			sphere->Set(gsphere, scene);
			continue;
		}
	}
	for(unsigned i=0; i<fr->Children().size(); ++i){
		InitTree(fr->Children()[i], scene);
	}
}


Vec3f GLRender::getPointUnderPixel(int x, int y, bool& found, SGScene* scene){
	GLfloat depth;
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
//	glReadPixels(x, screenHeight()-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	y = viewport[3]-y;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	DSTR<<"Depth:"<<depth<<std::endl;
	if (depth < 1.0) {
		found = true;
	}
	Vec3f res;
	if(found){
		Vec3f point(x, y, depth);
		GLdouble x,y,z;
		GLdouble proj[16];  
		glGetDoublev(GL_PROJECTION_MATRIX, proj);
		GLdouble modelview[16];  
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);			
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		gluUnProject(point.x,point.y,point.z, modelview,  proj,  viewport,  &x,&y,&z);
		res.x = x; res.y = y; res.z = z;
	}
	DSTR<<"res :"<<res<<std::endl;
	return res;
}




}	//	Spr


