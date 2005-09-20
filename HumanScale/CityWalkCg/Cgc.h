//////////////////////////////////////////////////////////////////
// Cg class header                                 ytakeyama

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <windows.h>
#include <ATIExtensions.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

//読み込むシェーダの最大数
#define VSMAX 3
#define FSMAX 3

static CGcontext context = NULL;
static CGprogram program = NULL;
static CGprogram vertexProgram[VSMAX], fragmentProgram[FSMAX];
//CGprofile profile = CG_PROFILE_ARBVP1;
static CGprofile vertexProfile, fragmentProfile;

#ifndef CWD
# define CWD ""
#endif

class Cgc
{
private:
	void CheckCgError();
	void ChooseProfiles();
	//void handleCgError();
	void LoadCgPrograms();
	void LoadCgVertexPrograms(int i, char* FuncName);
	void LoadCgFragmentPrograms(int i, char* FuncName);
public:
	void LoadFragmentProgram(int i);
	void LoadVertexProgram(int i);
	void DisableVertexProfile();
	void DisableFragmentProfile();
	void EnableVertexProfile();
	void EnableFragmentProfile();
	void SetMatrix(int i);
	void SetLocalMatrix(int i);
	Cgc();
	~Cgc();
};