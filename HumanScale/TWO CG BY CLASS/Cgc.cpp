////////////////////////////////////////////////////////////////////////////
// Cg class                                               aogawa
/*
Cg�̃\�[�X�́A�P�̃\�[�X�t�@�C���ɂP�̃G���g���֐��݂̂�����ꍇ��z�肵�Ă��܂��B
�\�[�X�t�@�C���ɂ���āA���m�ɏ������e�𕪂������l�����ł��B
void Cgc::LoadCgVertexPrograms(int i, char* FileName, char* FuncName)
void Cgc::LoadCgFragmentPrograms(int i, char* FileName, char* FuncName)
���g���āA���ʔԍ��A�\�[�X�t�@�C�����A�G���g���֐����������ɂ��ă��[�h���Ă��������B

�P�̃\�[�X�t�@�C���ɕ����̃G���g���֐��A�Ƃ����`�ŁA�t�@�C���������炵�����l�́A
void Cgc::LoadCgVertexPrograms(int i, char* FuncName)
void Cgc::LoadCgFragmentPrograms(int i, char* FuncName)
���g���āA���ʔԍ��A�G���g���֐����������ɂ��ă��[�h���Ă��������B
�Ȃ��A���̏ꍇ�t�@�C�����͒��_�V�F�[�_��vs.cg  �t���O�����g�V�F�[�_�� fs.cg�ɓ����Ă���̂Ƃ݂Ȃ��܂�

���_�V�F�[�_�ɓn���s��̂�����̕�����TGeoNode�ɓ��������d�l�ɂȂ��Ă܂��B
�ق��̕����ɂ��Ă͂��낢��ȃv���O�����Ŏg����Ǝv���܂�
���̊֐��̎g�����́A�֐��ʂɃR�����g���Ă���̂ŁA����������Ă�������
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

static void handleCgError(void){//�Ȃ����O���[�o���łȂ��ƃ|�C���^��n���Ȃ�
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

//���̊֐����g��Ȃ������E�E�E
//VertexShader, FragmentShader����������g��Ȃ��Ȃ炱�����̂ق����֗�����
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

	//�Ƃ肠���������ɂ��Ă݂�
	DisableVertexProfile();
	DisableFragmentProfile();
}

///////////////////////////////////////////////////////////////////////////////////
//
//						�v���O�����̃��[�h
//
//////////////////////////////////////////////////////////////////////////////////

//���ʔԍ��ACg�\�[�X�t�@�C�����A�G���g���֐����������ɁA���̃v���O������load����
void Cgc::LoadCgVertexPrograms(int i, char* FileName, char* FuncName)
{
    vertexProgram[i] = cgCreateProgramFromFile(context, CG_SOURCE, /*CWD*/ FileName,
                                            vertexProfile, FuncName, NULL);
    if (!cgIsProgramCompiled(vertexProgram[i]))
        cgCompileProgram(vertexProgram[i]);

    cgGLEnableProfile(vertexProfile);
    cgGLLoadProgram(vertexProgram[i]);
}

//���ʔԍ��ACg�\�[�X�t�@�C�����A�G���g���֐����������ɁA���̃v���O������load����
void Cgc::LoadCgFragmentPrograms(int i, char* FileName, char* FuncName){
	fragmentProgram[i] = cgCreateProgramFromFile(context, CG_SOURCE, /*CWD*/ FileName,
                                              fragmentProfile, FuncName, NULL);
    if (!cgIsProgramCompiled(fragmentProgram[i]))
        cgCompileProgram(fragmentProgram[i]);

    cgGLEnableProfile(fragmentProfile);
    cgGLLoadProgram(fragmentProgram[i]);
}

//vs.cg����A�G���g���֐����w�肵�ăv���O���������[�h
void Cgc::LoadCgVertexPrograms(int i, char* FuncName)
{
    vertexProgram[i] = cgCreateProgramFromFile(context, CG_SOURCE, CWD "vs.cg",
                                            vertexProfile, FuncName, NULL);
    if (!cgIsProgramCompiled(vertexProgram[i]))
        cgCompileProgram(vertexProgram[i]);

    cgGLEnableProfile(vertexProfile);
    cgGLLoadProgram(vertexProgram[i]);
}

//fs.cg����A�G���g���֐����w�肵�ăv���O���������[�h
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
//						�v���t�@�C���̗L���^����
//
//					�ȉ��̗L���E�����ŁA�V�F�[�_���p�C�v���C����ON/OFF�ł���
//
//////////////////////////////////////////////////////////////////////////////////

//profile�𒸓_�V�F�[�_�����L���ɂ��鎞
void Cgc::EnableVertexProfile(void){
	cgGLEnableProfile(vertexProfile);
}

//profile�𒸓_�V�F�[�_�����L���ɂ��鎞
void Cgc::EnableFragmentProfile(void){
	cgGLEnableProfile(fragmentProfile);
}

//�܂Ƃ߂Ă݂��BCg�̗L����
void Cgc::EnableCgProfile(void){
	cgGLEnableProfile(vertexProfile);
	cgGLEnableProfile(fragmentProfile);
}

//profile�𒸓_�V�F�[�_���������ɂ��鎞
void Cgc::DisableVertexProfile(void){
	cgGLDisableProfile(vertexProfile);
}

//profile���t���O�����g�V�F�[�_���������ɂ��鎞
void Cgc::DisableFragmentProfile(void){
	cgGLDisableProfile(fragmentProfile);
}

//�܂Ƃ߂Ă݂��BCg�̖�����
void Cgc::DisableCgProfile(void){
	cgGLDisableProfile(vertexProfile);
	cgGLDisableProfile(fragmentProfile);
}

///////////////////////////////////////////////////////////////////////////////////
//
//						�V�F�[�_�v���O�����̐؂�ւ�
//
//			�ȉ���Bind����֐����g���āA�p�C�v���C���ɓK�p����V�F�[�_��؂�ւ�����
//
//////////////////////////////////////////////////////////////////////////////////

//���ʔԍ��������ɂ��āA���̔ԍ��̒��_�v���O������bind����
void Cgc::BindVertexProgram(int i){
	cgGLBindProgram(vertexProgram[i]);
}

//���ʔԍ��������ɂ��āA���̔ԍ��̃t���O�����g�v���O������bind����
void Cgc::BindFragmentProgram(int i){
	cgGLBindProgram(fragmentProgram[i]);
}

///////////////////////////////////////////////////////////////////////////////////
//
//						�R���X�g���N�^�A�f�X�g���N�^�Ȃ�
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
	//����̓R�����g�A�E�g�����Ⴂ�܂���
	//�l�ɂ����Cg�\�[�X�t�@�C���̐��͈Ⴄ�̂ŁA�e���ŌĂяo���Ă��炤�K�v���肩�ƁE�E�E
	//LoadCgPrograms();
}

//���f���r���[�s��̖��O�͐l�ɂ���ĈႤ�̂ŁA����͊e���ŌĂяo���̕K�v���肩��
void Cgc::SetMatrix(int i){
	cgGLSetStateMatrixParameter(cgGetNamedParameter(vertexProgram[i], "modelViewProj"), CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
}

//TGeoNode�΍�
void Cgc::SetLocalMatrix(int i){
	cgGLSetStateMatrixParameter(cgGetNamedParameter(vertexProgram[i], "localMatrix"), CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
}
