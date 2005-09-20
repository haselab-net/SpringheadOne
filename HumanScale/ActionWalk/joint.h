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
#define TIME_PER_FRAME 30 // minumum elapsed time per frame (in milliseconds)

typedef struct rotInfo {
  GLfloat x, xMin, xMax;
  GLfloat y, yMin, yMax;
  GLfloat z, zMin, zMax;
} rotInfo;
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
} joint;

struct MyBone 
{
	long	id;							// BONE ID
	long    group_id;
	int		group_index;
	char	name[80];					// BONE NAME
	int 	flags;						// BONE FLAGS
    int     chk_quat;
	int		cur_joint;
	// HIERARCHY INFO
	//MyBone	*parent;				    // POINTER TO PARENT BONE
	//int 	childCnt;				    // COUNT OF CHILD BONES
	//MyBone	*children;				    // POINTER TO CHILDREN
	// TRANSFORMATION INFO
	
    int cur_frm;

	MyVector	b_trans;				// BASE TRANSLATION FACTORS
	
	MyVector	scale;					// SCALE FACTORS
	MyVector	trans[MAX_FRAMES];					// CURRENT TRANSLATION FACTORS
	MyVector	rot[MAX_FRAMES];					// CURRENT ROTATION FACTORS

	MyVector  position_global[MAX_FRAMES];
	tQuaternion quat;					// QUATERNION USEFUL FOR ANIMATION

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

    void reset();

};

void MyBone::reset()
{
   	id =0;						
	group_id=100;
	group_index=0;
	//name[80];					// BONE NAME
	flags=0;					
    chk_quat=0;
	cur_joint=0;
//	childCnt=1;				
	cur_frm=1;
    length = 0.75;

	for (int i=0; i<MAX_FRAMES;i++)
	{
	   trans[i].x=0.0;		
	   	   trans[i].y=0.0;		
		   	   trans[i].z=0.0;		
	   rot[i].x=0.0;		
	   rot[i].y=0.0;		
	   rot[i].z=0.0;		
	   position_global[i].x=0.0;
	   position_global[i].y=0.0;
	   position_global[i].z=0.0;
	}
	tQuaternion quat;					// 

}

MyBone LL_bone[3];
MyBone RL_bone[3];
MyBone LA_bone[4];
MyBone RA_bone[4];
MyBone H_bone[2];
MyBone A_bone[3];
MyBone IK_bone[7];

struct bone_Hierarchy
{
	MyBone root;
    MyBone *child;
    long  g_id;
    int   childCnt;

} B_Hier;

struct Anim_info
{
    int Init_code;
	char motion_name[128];
	int  AnimBlend;
	double  unit_Time;
    long Frame_num;
	long Joint_cnt;
    
	double Key_time;

    int Throw_num;
	long Key_frm;
    long Off_frm;
    long Pre_frm;

	int  S_status;
	int cur_frm;
	//////////camera
	float fov;
    float distZ;
    rotInfo camera;
	void init();
};

void Anim_info::init()
{
	 AnimBlend = 0.3;//0.097
	 unit_Time = 0.033;
	 Frame_num=0;
	 Joint_cnt=0;
	 //Key_frm=0.0;
	 
	 Throw_num= 30; 
	 Off_frm=1;
	 
	 Pre_frm=0;
	 Init_code = 1100; 
	// motion_name ="";
	 // camera 
     fov = 30.0;//50
     distZ = 200; //180.0;
	 
    camera.x=10.0f;  //-5.0
	camera.xMax=360.0f;
	camera.y=0.0f;
	camera.yMax=360.0f;
	camera.zMax=360.0f;
	 
}

// local matrix -> global matrix
// I know this isn't good method, but,I may be use transiently for a while (-_-;)
void setup_endeffector(int group_id, long T_frame)
{
	float tx, ty, tz, cx, sx, cy, sy, cz, sz;
	double roll, pitch, yaw; 
    MyVector endMyVector;
    MyVector endEffector(0.0, -3.12, 0.0);
  //  MyBone* t_Bone;
//	unsigned long offset;
  
//	offset = sizeof(A_bone);
	//bone_Hierarchy *t_Bone = new bone_Hierarchy;
//	t_Bone= &A_bone[0];
//	switch(group_id)
//	{
//	case LEFT_ARM:
//		*(t_Bone+len1*2) = LA_bone[0];
//		break;
//	case RIGHT_ARM:
//		(*(t_Bone+len1)) = RA_bone[0];
//		break;
//	}
	//t_Bone->g_id = group_id;
    int child_cnt = 7;

	for(int i=0; i<T_frame; i++)
	{
		double local[4][4];
		double result[4][4];
		double v[4] ;
		double v2[4];
		endMyVector.x =0;
		endMyVector.y =0;
		endMyVector.z =0;
	    MRot rot ={1, 0, 0, 0, 0,1,0,0, 0,0,1,0, 0,0,0,1};

		for(int j=0; j<=child_cnt; j++)
		{
			int idx = j-3;
	       if (j<=2 )
		   {
			// ROTATE Vector COUNTERCLOCKWISE USING CUMULATIVE ROTATIONS
			   if (j==0)
			   {
					tx = A_bone[j].trans[i].x; 
					ty = A_bone[j].trans[i].y;
					tz = A_bone[j].trans[i].z;
			   }
			   else
			   {
				   tx = A_bone[j].b_trans.x; 
				   ty = A_bone[j].b_trans.y;
				   tz = A_bone[j].b_trans.z;
			   }
		   }
		   else if (j==7)
		   {
			   tx = endEffector.x;
			   ty = endEffector.y;
			   tz = endEffector.z;
		   }
		   else 
		   { 
			    switch(group_id)
				{
				case LEFT_ARM:
					tx = LA_bone[idx].b_trans.x; 
					ty = LA_bone[idx].b_trans.y;
					tz = LA_bone[idx].b_trans.z;
					break;
				case RIGHT_ARM:
					tx = RA_bone[idx].b_trans.x; 
					ty = RA_bone[idx].b_trans.y;
					tz = RA_bone[idx].b_trans.z;
					break;
				}
			   
		   }
			v[0] = tx;
			v[1] = ty;
			v[2] = tz;
			v[3] = 1;
			multMatrixMyVector(rot.Matrix, v, v2);

		// TRANSLATE SYSTEM BACK TO ORIGINAL LOCATION
		 endMyVector.x += v2[0]; endMyVector.y += v2[1]; endMyVector.z += v2[2];

		if (j<=2)
		{
			// get local rotations first
			yaw = DegreesToRadians(A_bone[j].rot[i].z);
			roll = DegreesToRadians(A_bone[j].rot[i].x);
			pitch = DegreesToRadians(A_bone[j].rot[i].y);
		    A_bone[j].quat = MakeQFromEulerAngles(roll, pitch, yaw);
		}
		else if (j==7)
		{
			  yaw =0.0;
			  roll=0.0;
			  pitch=0.0;
		}
		else
		{
	            switch(group_id)
				{
				case LEFT_ARM:
					yaw = DegreesToRadians(LA_bone[idx].rot[i].z);
					roll = DegreesToRadians(LA_bone[idx].rot[i].x);
					pitch = DegreesToRadians(LA_bone[idx].rot[i].y);
					LA_bone[idx].quat = MakeQFromEulerAngles(roll, pitch, yaw);
					break;
				case RIGHT_ARM:
					yaw = DegreesToRadians(RA_bone[idx].rot[i].z);
					roll = DegreesToRadians(RA_bone[idx].rot[i].x);
					pitch = DegreesToRadians(RA_bone[idx].rot[i].y);
					RA_bone[idx].quat = MakeQFromEulerAngles(roll, pitch, yaw);
					break;
				}
			   
		}
	   //euler->quat
       //rot order z->x->y
        cz = cos(yaw); sz = sin(yaw);
	    cx = cos(roll); sx = sin(roll);
	    cy = cos(pitch); sy = sin(pitch);

		local[0][0] = cz*cy-sz*sx*sy;
		local[0][1] = -sz*cx;
		local[0][2] = cz*sy+sz*sx*cy;
		local[0][3] = 0;
		
		local[1][0] = sz*cy+cz*sx*sy;
		local[1][1] = cz*cx;
		local[1][2] = sz*sy-cz*sx*cy;
		local[1][3] = 0;

		local[2][0] = -cx*sy;
		local[2][1] = sx;
		local[2][2] = cx*cy;
		local[2][3] = 0;

		local[3][0] = 0;
		local[3][1] = 0;
		local[3][2] = 0;
		local[3][3] = 1;
		
		// accumulate
		multMatrix(rot.Matrix, local, result);
		copyMatrix(result, rot.Matrix);
			if (j<=2)
			{
			   A_bone[j].position_global[i] = endMyVector;
			}
			else
			{
				if (j== child_cnt) idx = 3;
				 switch(group_id)
				{
					case LEFT_ARM:
						LA_bone[idx].position_global[i] = endMyVector;
						break;
					case RIGHT_ARM:
						RA_bone[idx].position_global[i] = endMyVector;
						break;
				 }
			}
		
		}
	}
//	delete t_Bone;
}

void Create_Body(MyBone temp_body)
{
	   int idx = temp_body.group_index;

		switch(temp_body.group_id)
		 {
		     case LEFT_LEG:
				strcpy(LL_bone[idx].name, temp_body.name);
				LL_bone[idx].group_id = temp_body.group_id;
				LL_bone[idx].id = temp_body.id;
				LL_bone[idx].group_index = temp_body.group_index;
				LL_bone[idx].b_trans =temp_body.b_trans;
				
				break;
			 case RIGHT_LEG:
				
				 strcpy(RL_bone[0].name, temp_body.name);
				RL_bone[idx].group_id = temp_body.group_id;
				RL_bone[idx].id = temp_body.id;
//				RL_bone[idx].childCnt = temp_body.childCnt;
				RL_bone[idx].group_index = temp_body.group_index;
				RL_bone[idx].b_trans =temp_body.b_trans;
				
				break;
			 case LEFT_ARM:
				strcpy(LA_bone[idx].name, temp_body.name);
				LA_bone[idx].group_id = temp_body.group_id;
				LA_bone[idx].id = temp_body.id;
//				LA_bone[idx].childCnt = temp_body.childCnt;
				LA_bone[idx].group_index = temp_body.group_index;
				LA_bone[idx].b_trans =temp_body.b_trans;
				
				break;
			 case RIGHT_ARM:
				
				 strcpy(RA_bone[idx].name, temp_body.name);
				RA_bone[idx].group_id = temp_body.group_id;
				RA_bone[idx].id = temp_body.id;
	//			RA_bone[idx].childCnt = temp_body.childCnt;
				RA_bone[idx].group_index = temp_body.group_index;
				RA_bone[idx].b_trans =temp_body.b_trans;
				
				break;
			 case SKULL:
				
					 strcpy(H_bone[idx].name, temp_body.name);
				H_bone[idx].group_id = temp_body.group_id;
				H_bone[idx].id = temp_body.id;
	//			H_bone[idx].childCnt = temp_body.childCnt;
				H_bone[idx].group_index = temp_body.group_index;
				H_bone[idx].b_trans =temp_body.b_trans;
				
			 break;
			 default:
				 strcpy(A_bone[idx].name, temp_body.name);
				A_bone[idx].group_id = temp_body.group_id;
				A_bone[idx].id = temp_body.id;
	//			A_bone[idx].childCnt = temp_body.childCnt;
				A_bone[idx].group_index = temp_body.group_index;
				A_bone[idx].b_trans =temp_body.b_trans;
				
				break;
		 }

}

void Create_Motion(MyVector t_trans, MyVector t_rot, long g_id, int idx, int cur_frm)
{
		switch(g_id)
		 {
		     case LEFT_LEG:
				LL_bone[idx].trans[cur_frm] =t_trans;
				LL_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case RIGHT_LEG:
				RL_bone[idx].trans[cur_frm] =t_trans;
				RL_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case LEFT_ARM:
				LA_bone[idx].trans[cur_frm] =t_trans;
				LA_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case RIGHT_ARM:
				RA_bone[idx].trans[cur_frm] =t_trans;
				RA_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case SKULL:
				H_bone[idx].trans[cur_frm] =t_trans;
				H_bone[idx].rot[cur_frm] =t_rot;
			 break;
			 default:
				A_bone[idx].trans[cur_frm] =t_trans;
				A_bone[idx].rot[cur_frm] =t_rot;
				break;
		 }
}

#endif