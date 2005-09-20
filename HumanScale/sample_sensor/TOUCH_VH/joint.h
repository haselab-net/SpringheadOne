#if !defined(JOINT__H__INCLUDED_)
#define JOINT_H__INCLUDED_

#include "mymath.h"

//group ID
#define LEFT_LEG 110
#define RIGHT_LEG 120
#define LEFT_ARM 130
#define RIGHT_ARM 140
#define SKULL 150
#define MAX_BONES 19
#define MAX_FRAMES 500

#define END_POS 5
#define JOINT_RAD 0.75
#define HEAD_RAD 3
//SLCameraÅ@10/21
#define EYE_RAD 0.5

#define TIME_PER_FRAME 30 // minumum elapsed time per frame (in milliseconds)
#define JOINT_CNT 19

struct Move_Data
{
   //animation info
	int Anim_Set[10];
    //joint motion per one frame
    float Cur_Bone[60]; //channel
	float End_Effector[6];
//Add	
	float Ball_Pos[3];
	float Hand_Pos[6];
	int Temp_Rot[9];
	//ADD take
	float RotBall[3];
//for experiment
	int mode;
	//char mode_text[15];
	float vcamera[3];

};

struct rotInfo
{
  float x, xMin, xMax;
  float y, yMin, yMax;
  float z, zMin, zMax;
};
///////////////////////////////
// after that, this will load from model database 
enum jointID {
	Root,
	Hips,
	LeftHip,
	LeftKnee,
	LeftAnkle,
	RightHip,
	RightKnee,
	RightAnkle,
	chest1,
	chest2,
	LeftCollar,
	LeftShoulder,
	LeftElbow,
	LeftWrist,
//	LeftEhand,
	RightCollar,
	RightShoulder,
	RightElbow,
	RightWrist,
//	RightEhand,
	Neck,
	Head
};
//2004/06/18
struct cyberhand
{
 /* MyVector  thumbj[2];   
  MyVector  indexj[2];   
  MyVector  midj[2];   
  MyVector  ringj[2];   
  MyVector  pickyj[2]; */
  //MyVector  finger[2][5];   
  MyVector  finger[18];   
  MyVector  palmj;
  MyVector  wristj;//pitch, yaw, not roll
};

struct MyBone 
{
	int 	flags;						// BONE FLAGS
    int     chk_quat;
	int		cur_joint;

	MyVector scale;					// SCALE FACTORS

	// DOF CONSTRAINTS
	float	length;
	int		min_rx, max_rx;				// ROTATION X LIMITS
	int		min_ry, max_ry;				// ROTATION Y LIMITS
	int		min_rz, max_rz;				// ROTATION Z LIMITS
	float	damp_width, damp_strength;	// DAMPENING SETTINGS
	
	// COLLISION ELEMENTS
	float	bbox[6];					// BOUNDING BOX (UL XYZ, LR XYZ)
	MyVector	center;					// CENTER OF OBJECT (MASS)
	float	bsphere;					// BOUNDING SPHERE (RADIUS)  
	
	// PHYSICS
	float	mass;						// MASS
	float	friction;					// STATIC FRICTION
	float	kfriction;					// KINETIC FRICTION
//public
	long	id;							// BONE ID
	long    group_id;
	int		group_index;
	char	name[20];					// BONE NAME
	int		cur_frm;

	MyVector  b_trans;                   //Local Translation	
	MyVector  trans[MAX_FRAMES];		 // CURRENT TRANSLATION FACTORS
	MyVector  rot[MAX_FRAMES];					// CURRENT ROTATION FACTORS
	MyVector  position_global[MAX_FRAMES];

	tQuaternion quat;					// QUATERNION USEFUL FOR ANIMATION

    void reset();

};

struct bone_Hierarchy
{
	MyBone root;
    MyBone *child;
    long  g_id;
    int   childCnt;

};

struct Model_Hieracrchy
{
    MyBone LL_bone[3];
	MyBone RL_bone[3];
	MyBone LA_bone[4];
	MyBone RA_bone[4];
	MyBone H_bone[2];
	MyBone A_bone[3];

};

struct Anim_info
{
	int Init_code;
	int first_type;
    int second_type;
	char motion_name[128];
	double AnimBlend;
	int Frame_num;
	int Joint_cnt;
	int Channels;

	double unit_Time;
    double Key_time; //catch time

    int Throw_num; //number of throw motion
	// motion control
	int Key_frm; //catch data key_frame
	int Catch_frm; //expected catch_frm (ball)
    int Off_frm;  //throw data release frame
    int Pre_frm; //catch data frame_num
	int Throw_frm; //throw data frame_num
    int cur_frm;
	int S_status;

	//////////camera
	float fov;
    float distZ;
    rotInfo camera;

	MyVector conv_trans;
	MyVector conv_rot;
	void init();
};

//in first default motion setting/insert
//2004/04/13texsig
struct Default_Motion
{
  int m_id[3];
  int total_frm[3];
  Model_Hieracrchy d_motion[3];

};

class CVH_RENDER
{
	int ID;
	int group_id;
	int group_idx;

public:
//2004.4.13 updated jeong
	Default_Motion dfmotion;
	Model_Hieracrchy MModel;

	Anim_info Anim;
    void Create_Body(MyBone t_body);
	void Create_Motion(MyVector t_trans, MyVector t_rot, long g_id, int idx, int cur_frm);
	void Create_Motion(int def_idx, MyVector t_trans, MyVector t_rot, long g_id, int idx, int cur_frm);

	void input_motion(MyVector t_rot, long g_id, int idx, int cur_frm);
	void move_motion(int, int);
	MyVector Ret_Array(int grp_id, int grp_idx, int cur_frm);
    MyVector Ret_Global(int, int, int);
	void Com_Interp(int frm, int pre_frm);
//DRAW
//void drawjoint(int cur_frm, MyBone sk_bone);
//void drawBody(int cur_frame);
//COMPUTATION
	void setup_endeffector(int group_id, int t_frame);

};

#endif