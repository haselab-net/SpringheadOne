/*######################################################*/
//SLC_define.h
//	SLC��`�֘A�i����J�j
//�쐬��
//	���R�@�i
/*######################################################*/

#ifndef __SLCDEVELOP_HEADER_

#define __SLCDEVELOP_HEADER_

#define SLCDEV_CAM_IMGSIZE_X	640
#define SLCDEV_CAM_IMGSIZE_Y	480

#define SLCDEV_PI_FVAL		3.141592654f

//�Q�����f�[�^ 1�̃}�[�J���Е��̃f�[�^
typedef struct
{
	double	_dX;			//���̂�X���W�ipixel�j
	double	_dY;			//���̂�Y���W�ipixel�j
	int _nAreaSize;			//�ʐ�
	double	_dSortKey;		//�\�[�g�L�[
	int _nWorkVal;			//�������[�N�p
	BOOL _bDeciFlg;
	int _nStatus;			//�X�e�[�^�X 0:���� 1�ȏ�:�L��
}SLCDEV_2DIMOBJ_1MKOS;

//�Q�����f�[�^
typedef struct
{
	//�񎟌����W�f�[�^�i���E�j
	SLCDEV_2DIMOBJ_1MKOS _sLeft[SLCOBJECT_MARKERMAX];
	SLCDEV_2DIMOBJ_1MKOS _sRight[SLCOBJECT_MARKERMAX];
	//�񎟌����W�f�[�^�I�u�W�F�N�g�̐��i���E�j
	int _nLeftSize;
	int _nRightSize;
}SLCDEV_2DIMOBJ;

//���O���p�\����
typedef struct
{
	//�Q����
	SLCDEV_2DIMOBJ _s2dObj;
	BOOL _b2dSetFlg;

	//�R����
	SLCOBJECT _s3dObj[SLCOBJECT_MARKERMAX];
	int _n3dMarkerNum;

}SLCDEV_LOGOBJ;


#endif