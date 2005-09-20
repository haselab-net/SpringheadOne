////////////////////////////////////////////////////////////////////////////
// Cg class                                               ytakeyama
//
//Cgのソースは、頂点シェーダが vs.cg  フラグメントシェーダが fs.cg
//に入ってるものとみなしています。
//頂点シェーダに渡す行列のあたりの部分はTGeoNodeに特化した仕様になってます。
//ほかの部分についてはいろいろなプログラムで使えると思います

#include "Cgc.h"

void Cgc::CheckCgError(void)
{
  CGerror err = cgGetError();

  if (err != CG_NO_ERROR)
   {
     printf("CG error: %s\n", cgGetErrorString(err));
     exit(1);
   }
}

static void handleCgError(void){//なぜかグローバルでないとポインタを渡せない
	fprintf(stderr, "Cg error: %s\n", cgGetErrorString(cgGetError()));
	exit(1);
}

void Cgc::ChooseProfiles()
{
    // Make sure that the appropriate profiles are available on the
    // user's system.
	if (cgGLIsProfileSupported(CG_PROFILE_ARBVP1))
        vertexProfile = CG_PROFILE_ARBVP1;
    else {
        // try VP30
        if (cgGLIsProfileSupported(CG_PROFILE_VP30))
            vertexProfile = CG_PROFILE_VP30;
        else {
            fprintf(stderr, "Neither arbvp1 or vp30 vertex profiles supported on this system.\n");
            exit(1);
        }
    }

    if (cgGLIsProfileSupported(CG_PROFILE_ARBFP1))
        fragmentProfile = CG_PROFILE_ARBFP1;
    else {
        // try FP30
        if (cgGLIsProfileSupported(CG_PROFILE_FP30))
            fragmentProfile = CG_PROFILE_FP30;
        else {
            fprintf(stderr, "Neither arbfp1 or fp30 fragment profiles supported on this system.\n");
            exit(1);
        }
    }

}

//VertexShader, FragmentShaderが一つずつしか使わないならこっちのほうが便利かと
void Cgc::LoadCgPrograms()
{
	vertexProfile = CG_PROFILE_ARBVP1;
	fragmentProfile = CG_PROFILE_ARBFP1;

    // Load and compile the vertex program from demo_vert.cg; hold on to the
    // handle to it that is returned.
    vertexProgram[0] = cgCreateProgramFromFile(context, CG_SOURCE, CWD "vs.cg",
                                            vertexProfile, "Cg_vs", NULL);
    if (!cgIsProgramCompiled(vertexProgram[0]))
        cgCompileProgram(vertexProgram[0]);

    // Enable the appropriate vertex profile and load the vertex program.
    cgGLEnableProfile(vertexProfile);
    cgGLLoadProgram(vertexProgram[0]);

    // And similarly set things up for the fragment program.
	
    fragmentProgram[0] = cgCreateProgramFromFile(context, CG_SOURCE, CWD "fs.cg",
                                              fragmentProfile, "Cg_fs", NULL);
    if (!cgIsProgramCompiled(fragmentProgram[0]))
        cgCompileProgram(fragmentProgram[0]);

    cgGLEnableProfile(fragmentProfile);
    cgGLLoadProgram(fragmentProgram[0]);

	//とりあえず無効にしてみる
	DisableVertexProfile();
	DisableFragmentProfile();
}

void Cgc::LoadCgVertexPrograms(int i, char* FuncName)
{
    vertexProgram[i] = cgCreateProgramFromFile(context, CG_SOURCE, CWD "vs.cg",
                                            vertexProfile, FuncName, NULL);
    if (!cgIsProgramCompiled(vertexProgram[i]))
        cgCompileProgram(vertexProgram[i]);

    cgGLEnableProfile(vertexProfile);
    cgGLLoadProgram(vertexProgram[i]);
}

void Cgc::LoadCgFragmentPrograms(int i, char* FuncName){
	fragmentProgram[i] = cgCreateProgramFromFile(context, CG_SOURCE, CWD "fs.cg",
                                              fragmentProfile, FuncName, NULL);
    if (!cgIsProgramCompiled(fragmentProgram[i]))
        cgCompileProgram(fragmentProgram[i]);

    cgGLEnableProfile(fragmentProfile);
    cgGLLoadProgram(fragmentProgram[i]);
}

void Cgc::LoadVertexProgram(int i){
	cgGLLoadProgram(vertexProgram[i]);
}

void Cgc::LoadFragmentProgram(int i){
	cgGLLoadProgram(fragmentProgram[i]);
}

void Cgc::DisableVertexProfile(void){
	cgGLDisableProfile(vertexProfile);
}
void Cgc::DisableFragmentProfile(void){
	cgGLDisableProfile(fragmentProfile);
}
void Cgc::EnableVertexProfile(void){
	cgGLEnableProfile(vertexProfile);
}
void Cgc::EnableFragmentProfile(void){
	cgGLEnableProfile(fragmentProfile);
}

Cgc::~Cgc(void){
	for(int i=0; i<VSMAX; i++)	cgDestroyProgram(vertexProgram[i]);
    for(i=0; i<FSMAX; i++)  cgDestroyProgram(fragmentProgram[i]);
    cgDestroyContext(context);	
}

Cgc::Cgc(void){
	cgSetErrorCallback(handleCgError);
	context = cgCreateContext();
	ChooseProfiles();
	LoadCgPrograms();
}

void Cgc::SetMatrix(int i){
	cgGLSetStateMatrixParameter(cgGetNamedParameter(vertexProgram[i], "modelViewProj"), CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
}

//TGeoNode対策
void Cgc::SetLocalMatrix(int i){
	cgGLSetStateMatrixParameter(cgGetNamedParameter(vertexProgram[i], "localMatrix"), CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
}
