#include "GraphicsGL.h"
#include <Graphics/GRLoadBmp.h>
#include <Base/BaseDebug.h>

#include <windows.h>						// Header File For Windows
#include <gl/gl.h>						// Header File For The OpenGL32 Library
#include <gl/glu.h>						// Header File For The GLu32 Library
#include <gl/glaux.h>						// Header File For The GLaux Library




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
	hWnd = NULL;
	hgl = NULL;
	hdc = NULL;
	nLights = 0;
}
GLRender::~GLRender(){
	Release();
}
bool GLRender::Create(void* arg){
	hWnd = (HWND)arg;
	PIXELFORMATDESCRIPTOR pfd = { 
		  sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		  1,                     // version number 
		  PFD_DRAW_TO_WINDOW |   // support window 
		  PFD_SUPPORT_OPENGL |   // support OpenGL 
		  PFD_DOUBLEBUFFER,      // double buffered 
		  PFD_TYPE_RGBA,         // RGBA type 
		  24,                    // 24-bit color depth 
		  0, 0, 0, 0, 0, 0,      // color bits ignored 
		  0,                     // no alpha buffer 
		  0,                     // shift bit ignored 
		  0,                     // no accumulation buffer 
		  0, 0, 0, 0,            // accum bits ignored 
		  32,                    // 32-bit z-buffer 
		  0,                     // no stencil buffer 
		  0,                     // no auxiliary buffer 
		  PFD_MAIN_PLANE,        // main layer 
		  0,                     // reserved 
		  0, 0, 0                // layer masks ignored 
	}; 
	hdc = GetDC(hWnd);
    int pixelFormat=ChoosePixelFormat(hdc,&pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);	
	hgl = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hgl);
	glDrawBuffer(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	return true;
}
bool GLRender::SetViewport(void* arg){
	HWND hw = (HWND) arg;
	RECT rc;
	GetClientRect(hw, &rc);
	Resize(Vec2f(rc.right-rc.left, rc.bottom-rc.top));
	return true;
}

void GLRender::Release(){
	if (hgl){
		wglDeleteContext(hgl);
		hgl = NULL;
	}
	if (hWnd && hdc){
		ReleaseDC(hWnd, hdc);
		hWnd = NULL;
	}
}
void GLRender::Present(){
	SwapBuffers(hdc);
}


///	バッファクリア
void GLRender::ClearBuffer(){
	glClearColor(0,0,0,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
}

void GLRender::ClearColor(float r, float g, float b, float alpha) {
	glClearColor(r,g,b,alpha);
	glClear(GL_COLOR_BUFFER_BIT);
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
//	afProj = Affinef::ProjectionGL(Vec3f(camera->data.center.X(), camera->data.center.Y(), camera->data.front),
//		Vec2f(camera->data.size.X(), camera->data.size.Y() ), camera->data.front, camera->data.back);
	afProj = Affinef::ProjectionGL(Vec3f(camera->data.center.X(), camera->data.center.Y(), camera->data.front),
		Vec2f(camera->data.size.X(), camera->data.size.X()*screen.Y()/screen.X() ), camera->data.front, camera->data.back);
	SetProjectionMatrix(afProj);
}

void GLRender::SetViewMatrix(const Affinef& afv){
	if (camera) camera->data.view = afv;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->data.view * afModel);
}
void GLRender::MultModelMatrix(const Affinef& afw){
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(afw);
}
void GLRender::PushModelMatrix(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}
void GLRender::PopModelMatrix(){
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
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
void GLRender::DrawText(Vec2f pos, FIString str, const GRFont& font){

//SAMPLE TAKEN in NEHE GAMEDEV SITE. For details see the web page http://nehe.gamedev.net/

	//DSTR<<"printing "<<pos.x<<" "<<pos.y<<std::endl;
// Build Our Bitmap Font

HFONT	fontID;					// Windows Font ID
HFONT	oldfont;					// Used For Good House Keeping
GLuint	base;				// Base Display List For The Font Set
base = glGenLists(96);					// Storage For 96 Characters ( NEW )
fontID = CreateFont(font.height,// Height Of Font ( NEW )
					  font.width,// Width Of Font
	   				  0,// Angle Of Escapement
				      0,// Orientation Angle
					  FW_DONTCARE,// Font Weight		
					  FALSE,// Italic
					  FALSE,// Underline
					  FALSE,// Strikeout
					  ANSI_CHARSET,// Character Set Identifier		
					  OUT_TT_PRECIS,// Output Precision
					  CLIP_DEFAULT_PRECIS,// Clipping Precision
					  ANTIALIASED_QUALITY,// Output Quality
					  FF_DONTCARE|DEFAULT_PITCH,// Family And Pitch			
					  font.face.c_str());// Font Name
		
	oldfont = (HFONT)SelectObject(hdc, fontID);		// Selects The Font We Want
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	wglUseFontBitmaps(hdc, 32, 96, base);			// Builds 96 Characters Starting At Character 32
	SelectObject(hdc, oldfont);				// Selects The Font We Want
	DeleteObject(fontID);					// Delete The Font

	glPushMatrix();	
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glTranslatef(0.0f,0.0f,-1.0f);						// Move One Unit Into The Screen
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int height = viewport[3];	
	int width = viewport[2];
	float ratio = ((float)height) / width;
	float drawx = (((float)pos.x) / width) - 0.5;
	float drawy = -1*((((float)pos.y) / height) - 0.4)*ratio;
	//DSTR<<"Pos : "<<drawx<<" "<<drawy<<std::endl;
	glRasterPos2f(drawx, drawy);

	glPushAttrib(GL_LIST_BIT);				// Pushes The Display List Bits		( NEW )
	glListBase(base - 32);					// Sets The Base Character to 32	( NEW )
	glCallLists(str.size(), GL_UNSIGNED_BYTE, str.c_str());	// Draws The Display List Text	( NEW )
	glPopAttrib();						// Pops The Display List Bits	( NEW )
	
	glDeleteLists(base, 96);				// Delete All 96 Characters ( NEW )

	glPopMatrix();
	glEnable(GL_LIGHTING);
/*	int texId = textureManager.GetText(str, font);
	Vec2f sz = textureManager.GetTextSize();
	if (texId){
		glBindTexture(GL_TEXTURE_2D, texId);
		glEnable(GL_TEXTURE_2D);

		

		glDisable(GL_TEXTURE_2D);
	}
*/
}

    // write the code to implement the texture mapping in this method
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
void GLRender::SetDepthWrite(bool b){
	glDepthMask(b);
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

inline DWORD GetGLBlendFunc(GLRender::TBlendFunc f){
	switch(f){
	case GLRender::BF_ZERO: return GL_ZERO;
	case GLRender::BF_ONE: return GL_ONE;
	case GLRender::BF_SRCCOLOR: return GL_SRC_COLOR;
	case GLRender::BF_INVSRCCOLOR: return GL_ONE_MINUS_SRC_COLOR;
	case GLRender::BF_SRCALPHA: return GL_SRC_ALPHA;
	case GLRender::BF_INVSRCALPHA: return GL_ONE_MINUS_SRC_ALPHA;
	case GLRender::BF_DESTALPHA: return GL_DST_ALPHA;
	case GLRender::BF_INVDESTALPHA: return GL_ONE_MINUS_DST_ALPHA;
	case GLRender::BF_DESTCOLOR: return GL_DST_COLOR;
	case GLRender::BF_INVDESTCOLOR: return GL_ONE_MINUS_DST_COLOR;
	case GLRender::BF_SRCALPHASAT: return GL_SRC_ALPHA_SATURATE;
	case GLRender::BF_BOTHSRCALPHA: return 0;
	case GLRender::BF_BOTHINVSRCALPHA: return 0;
	case GLRender::BF_BLENDFACTOR: return 0;
	case GLRender::BF_INVBLENDFACTOR: return 0;
	}
}
void GLRender::SetAlphaMode(TBlendFunc src, TBlendFunc dest){
	glBlendFunc(GetGLBlendFunc(src), GetGLBlendFunc(dest));
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
	//DSTR<<"Depth:"<<depth<<std::endl;
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
	//DSTR<<"res :"<<res<<std::endl;
	return res;
}




}	//	Spr


