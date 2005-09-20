////////////////////////////////////////////////////////////////////////////
// Cg class                                               aogawa
/*
Cgのソースは、１つのソースファイルに１つのエントリ関数のみがある場合を想定しています。
ソースファイルによって、明確に処理内容を分けたい人向けです。
void Cgc::LoadCgVertexPrograms(int i, char* FileName, char* FuncName)
void Cgc::LoadCgFragmentPrograms(int i, char* FileName, char* FuncName)
を使って、識別番号、ソースファイル名、エントリ関数名を引数にしてロードしてください。

１つのソースファイルに複数のエントリ関数、という形で、ファイル数を減らしたい人は、
void Cgc::LoadCgVertexPrograms(int i, char* FuncName)
void Cgc::LoadCgFragmentPrograms(int i, char* FuncName)
を使って、識別番号、エントリ関数名を引数にしてロードしてください。
なお、この場合ファイル名は頂点シェーダがvs.cg  フラグメントシェーダが fs.cgに入ってるものとみなします

頂点シェーダに渡す行列のあたりの部分はTGeoNodeに特化した仕様になってます。
ほかの部分についてはいろいろなプログラムで使えると思います
他の関数の使い方は、関数別にコメントしてあるので、そちらを見てください
*/


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

//この関数も使わないかも・・・
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

///////////////////////////////////////////////////////////////////////////////////
//
//						プログラムのロード
//
//////////////////////////////////////////////////////////////////////////////////

//識別番号、Cgソースファイル名、エントリ関数名を引数に、そのプログラムをloadする
void Cgc::LoadCgVertexPrograms(int i, char* FileName, char* FuncName)
{
    vertexProgram[i] = cgCreateProgramFromFile(context, CG_SOURCE, /*CWD*/ FileName,
                                            vertexProfile, FuncName, NULL);
    if (!cgIsProgramCompiled(vertexProgram[i]))
        cgCompileProgram(vertexProgram[i]);

    cgGLEnableProfile(vertexProfile);
    cgGLLoadProgram(vertexProgram[i]);
}

//識別番号、Cgソースファイル名、エントリ関数名を引数に、そのプログラムをloadする
void Cgc::LoadCgFragmentPrograms(int i, char* FileName, char* FuncName){
	fragmentProgram[i] = cgCreateProgramFromFile(context, CG_SOURCE, /*CWD*/ FileName,
                                              fragmentProfile, FuncName, NULL);
    if (!cgIsProgramCompiled(fragmentProgram[i]))
        cgCompileProgram(fragmentProgram[i]);

    cgGLEnableProfile(fragmentProfile);
    cgGLLoadProgram(fragmentProgram[i]);
}

//vs.cgから、エントリ関数を指定してプログラムをロード
void Cgc::LoadCgVertexPrograms(int i, char* FuncName)
{
    vertexProgram[i] = cgCreateProgramFromFile(context, CG_SOURCE, CWD "vs.cg",
                                            vertexProfile, FuncName, NULL);
    if (!cgIsProgramCompiled(vertexProgram[i]))
        cgCompileProgram(vertexProgram[i]);

    cgGLEnableProfile(vertexProfile);
    cgGLLoadProgram(vertexProgram[i]);
}

//fs.cgから、エントリ関数を指定してプログラムをロード
void Cgc::LoadCgFragmentPrograms(int i, char* FuncName){
	fragmentProgram[i] = cgCreateProgramFromFile(context, CG_SOURCE, CWD "fs.cg",
                                              fragmentProfile, FuncName, NULL);
    if (!cgIsProgramCompiled(fragmentProgram[i]))
        cgCompileProgram(fragmentProgram[i]);

    cgGLEnableProfile(fragmentProfile);
    cgGLLoadProgram(fragmentProgram[i]);
}


///////////////////////////////////////////////////////////////////////////////////
//
//						プロファイルの有効／無効
//
//					以下の有効・無効で、シェーダをパイプラインでON/OFFできる
//
//////////////////////////////////////////////////////////////////////////////////

//profileを頂点シェーダだけ有効にする時
void Cgc::EnableVertexProfile(void){
	cgGLEnableProfile(vertexProfile);
}

//profileを頂点シェーダだけ有効にする時
void Cgc::EnableFragmentProfile(void){
	cgGLEnableProfile(fragmentProfile);
}

//まとめてみた。Cgの有効化
void Cgc::EnableCgProfile(void){
	cgGLEnableProfile(vertexProfile);
	cgGLEnableProfile(fragmentProfile);
}

//profileを頂点シェーダだけ無効にする時
void Cgc::DisableVertexProfile(void){
	cgGLDisableProfile(vertexProfile);
}

//profileをフラグメントシェーダだけ無効にする時
void Cgc::DisableFragmentProfile(void){
	cgGLDisableProfile(fragmentProfile);
}

//まとめてみた。Cgの無効化
void Cgc::DisableCgProfile(void){
	cgGLDisableProfile(vertexProfile);
	cgGLDisableProfile(fragmentProfile);
}

///////////////////////////////////////////////////////////////////////////////////
//
//						シェーダプログラムの切り替え
//
//			以下のBindする関数を使って、パイプラインに適用するシェーダを切り替えられる
//
//////////////////////////////////////////////////////////////////////////////////

//識別番号を引数にして、その番号の頂点プログラムをbindする
void Cgc::BindVertexProgram(int i){
	cgGLBindProgram(vertexProgram[i]);
}

//識別番号を引数にして、その番号のフラグメントプログラムをbindする
void Cgc::BindFragmentProgram(int i){
	cgGLBindProgram(fragmentProgram[i]);
}

///////////////////////////////////////////////////////////////////////////////////
//
//						コンストラクタ、デストラクタなど
//
//////////////////////////////////////////////////////////////////////////////////

Cgc::~Cgc(void){
	for(int i=0; i<VSMAX; i++)
		cgDestroyProgram(vertexProgram[i]);
    for(i=0; i<FSMAX; i++)
		cgDestroyProgram(fragmentProgram[i]);
    cgDestroyContext(context);	
}

Cgc::Cgc(void){
	cgSetErrorCallback(handleCgError);
	context = cgCreateContext();
	ChooseProfiles();
	//これはコメントアウトしちゃいました
	//人によってCgソースファイルの数は違うので、各自で呼び出してもらう必要ありかと・・・
	//LoadCgPrograms();
}

//モデルビュー行列の名前は人によって違うので、これは各自で呼び出しの必要ありかも
void Cgc::SetMatrix(int i){
	cgGLSetStateMatrixParameter(cgGetNamedParameter(vertexProgram[i], "modelViewProj"), CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
}

//TGeoNode対策
void Cgc::SetLocalMatrix(int i){
	cgGLSetStateMatrixParameter(cgGetNamedParameter(vertexProgram[i], "localMatrix"), CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
}
