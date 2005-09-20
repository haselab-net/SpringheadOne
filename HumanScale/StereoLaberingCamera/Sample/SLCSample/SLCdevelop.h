/*######################################################*/
//SLC_define.h
//	SLC定義関連（非公開）
//作成者
//	中山　進
/*######################################################*/

#ifndef __SLCDEVELOP_HEADER_

#define __SLCDEVELOP_HEADER_

#define SLCDEV_CAM_IMGSIZE_X	640
#define SLCDEV_CAM_IMGSIZE_Y	480

#define SLCDEV_PI_FVAL		3.141592654f

//２次元データ 1つのマーカ＆片方のデータ
typedef struct
{
	double	_dX;			//物体のX座標（pixel）
	double	_dY;			//物体のY座標（pixel）
	int _nAreaSize;			//面積
	double	_dSortKey;		//ソートキー
	int _nWorkVal;			//整数ワーク用
	BOOL _bDeciFlg;
	int _nStatus;			//ステータス 0:無効 1以上:有効
}SLCDEV_2DIMOBJ_1MKOS;

//２次元データ
typedef struct
{
	//二次元座標データ（左右）
	SLCDEV_2DIMOBJ_1MKOS _sLeft[SLCOBJECT_MARKERMAX];
	SLCDEV_2DIMOBJ_1MKOS _sRight[SLCOBJECT_MARKERMAX];
	//二次元座標データオブジェクトの数（左右）
	int _nLeftSize;
	int _nRightSize;
}SLCDEV_2DIMOBJ;

//ログ取り用構造体
typedef struct
{
	//２次元
	SLCDEV_2DIMOBJ _s2dObj;
	BOOL _b2dSetFlg;

	//３次元
	SLCOBJECT _s3dObj[SLCOBJECT_MARKERMAX];
	int _n3dMarkerNum;

}SLCDEV_LOGOBJ;


#endif