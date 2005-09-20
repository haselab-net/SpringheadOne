#if !defined(VHRENDER__H__INCLUDED_)
#define VHRENDER_H__INCLUDED_

#include "mymath.h"
#include "joint.h"

const double INCH_MITER = 39.37; //1m=39.37m 3m->118inch
jointID joint;

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


void Anim_info::init()
{
	 AnimBlend = 0.3;//0.097
	 unit_Time = 0.033;
	 Frame_num=0;
	 Joint_cnt=0;
	 Key_frm=0;
	 Key_time = 0.0;
	 Catch_frm =0;
	 Off_frm=1;

     catch_type=0;
     throw_type=0;

	 Throw_num= 20; 
	 Pre_frm=0;
	 Throw_frm =0;

	 Init_code = 1100;  
	 Channels =60;
	// motion_name ="";
	 // camera 
     fov = 30.0;
     distZ = -120; //180.0;
	 
    camera.x=0.0f;  //-5.0
	camera.y=-5.0f;
	camera.z=0.0f;
	camera.xMax=360.0f;
	camera.yMax=360.0f;
	camera.zMax=360.0f;
	 
}

// local matrix -> global matrix
// I know this isn't good method, but,I may be use transiently for a while (-_-;)
void CVH_RENDER::setup_endeffector(int group_id, int t_frame)
{
	double cx, sx, cy, sy, cz, sz;
	double roll, pitch, yaw; 
	MyVector tpos;
	MyVector endMyVector;
	MyVector endEffector;
    int child_cnt;

	if (group_id == SKULL) //skull
	{
		endEffector.x = 0.0;
	    endEffector.y = 4.14;
		endEffector.z = 0.0;
		child_cnt = 5;
	}
	else
	{
		endEffector.x = 0.0;
	    endEffector.y = -3.12;
		endEffector.z = 0.0;
		child_cnt = 7;
	}
 
	for(int i=0; i<t_frame; i++)
	{
		double local[4][4];
		double result[4][4];
		double v[4] ;
		double v2[4];
        endMyVector.x= 0.0;
		endMyVector.y= 0.0;
		endMyVector.z= 0.0;

		MRot rot ={1, 0, 0, 0, 0,1,0,0, 0,0,1,0, 0,0,0,1};

		for(int j=0; j<=child_cnt; j++)
		{
		   int idx = j-3;
	       if (j<=2 )
		   {
			// ROTATE Vector COUNTERCLOCKWISE USING CUMULATIVE ROTATIONS
			   if (j == 0) tpos = CopyVector(A_bone[j].trans[i]); //if root
			   else  tpos =CopyVector(A_bone[j].b_trans); 
		   }
		   else if (j== child_cnt)
		   {
			   tpos =CopyVector(endEffector);
		   }
		   else 
		   { 
			    switch(group_id)
				{
				case LEFT_ARM:
					tpos = CopyVector(LA_bone[idx].b_trans); 
					break;
				case RIGHT_ARM:
					tpos = CopyVector(RA_bone[idx].b_trans); 
					break;
				case SKULL:
					tpos = CopyVector(H_bone[idx].b_trans); 
					break;
				}
			   
		   }
			v[0] = tpos.x;
			v[1] = tpos.y;
			v[2] = tpos.z;
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
		else if (j==child_cnt)
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
				 case SKULL:
					yaw = DegreesToRadians(H_bone[idx].rot[i].z);
					roll = DegreesToRadians(H_bone[idx].rot[i].x);
					pitch = DegreesToRadians(H_bone[idx].rot[i].y);
					H_bone[idx].quat = MakeQFromEulerAngles(roll, pitch, yaw);
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
				if (j== child_cnt) idx = child_cnt - 4;
				switch(group_id)
				{
					case LEFT_ARM:
						LA_bone[idx].position_global[i] = endMyVector;
						break;
					case RIGHT_ARM:
						RA_bone[idx].position_global[i] = endMyVector;
						break;
					case SKULL:
						H_bone[idx].position_global[i] =  endMyVector;
						break;
				 }
			} //idx= child_cnt - 4  end effector index
		
		}
	}

//create BBox min
A_bone[0].bbox[0] = (float)LA_bone[0].position_global[0].x;
A_bone[0].bbox[1] = (float)A_bone[0].position_global[0].y;
A_bone[0].bbox[2] = (float)A_bone[0].position_global[0].z;
A_bone[0].bbox[3] = (float)RA_bone[0].position_global[0].x;
A_bone[0].bbox[4] = (float)H_bone[1].position_global[0].y;
A_bone[0].bbox[5] = (float)RA_bone[0].position_global[0].z;

A_bone[0].center.y=(A_bone[0].bbox[1]+A_bone[0].bbox[4])/2;
A_bone[0].center.x=(A_bone[0].bbox[0]+A_bone[0].bbox[3])/2;
A_bone[0].center.z=(A_bone[0].bbox[2]+A_bone[0].bbox[5])/2;
//	delete t_Bone;
}

void CVH_RENDER::Create_Body(MyBone t_body)
{
//10/27 JEONG
	   int idx = t_body.group_index;
       
		switch(t_body.group_id)
		 {
		     case LEFT_LEG:
				
				LL_bone[idx].group_id = t_body.group_id;
				LL_bone[idx].id = t_body.id;
				strcpy(LL_bone[idx].name, t_body.name);
				LL_bone[idx].scale = t_body.scale;
				LL_bone[idx].group_index = idx;
				LL_bone[idx].b_trans =t_body.b_trans;
				//LL_bone[idx].b_trans.y =t_body.b_trans.y;
				//LL_bone[idx].b_trans.z =t_body.b_trans.z;
				break;
			 case RIGHT_LEG:
			
				RL_bone[idx].group_id = t_body.group_id;
				RL_bone[idx].id = t_body.id;
				strcpy(RL_bone[idx].name, t_body.name);
				RL_bone[idx].scale = t_body.scale;
				RL_bone[idx].group_index = idx;
				RL_bone[idx].b_trans =t_body.b_trans;
				
				break;
			 case LEFT_ARM:
			
				LA_bone[idx].group_id = t_body.group_id;
				LA_bone[idx].id = t_body.id;
				strcpy(LA_bone[idx].name , t_body.name);
				LA_bone[idx].scale = t_body.scale;
				LA_bone[idx].group_index = idx;
				LA_bone[idx].b_trans =t_body.b_trans;
				
				break;
			 case RIGHT_ARM:
			
				RA_bone[idx].group_id = t_body.group_id;
				RA_bone[idx].id = t_body.id;
				strcpy(RA_bone[idx].name , t_body.name);
				RA_bone[idx].scale = t_body.scale;
				RA_bone[idx].group_index = idx;
				RA_bone[idx].b_trans =t_body.b_trans;
				
				break;
			 case SKULL:
				H_bone[idx].group_id = t_body.group_id;
				H_bone[idx].id = t_body.id;
				strcpy(H_bone[idx].name , t_body.name);
				H_bone[idx].scale = t_body.scale;

				H_bone[idx].group_index = idx;
				H_bone[idx].b_trans =t_body.b_trans;
				
			   break;
			 default:
	
				A_bone[idx].group_id = t_body.group_id;
				A_bone[idx].group_index = idx;
				A_bone[idx].id = t_body.id;
				strcpy(A_bone[idx].name , t_body.name);
				A_bone[idx].scale = t_body.scale;
				A_bone[idx].b_trans =t_body.b_trans;
				
				break;
		 }

}

void CVH_RENDER::Create_Motion(MyVector t_trans, MyVector t_rot, long g_id, int idx, int cur_frm)
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

void CVH_RENDER::input_motion(MyVector t_rot, long g_id, int idx, int cur_frm)
{
		switch(g_id)
		 {
		     case LEFT_LEG:
				LL_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case RIGHT_LEG:
				RL_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case LEFT_ARM:
				LA_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case RIGHT_ARM:
				RA_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case SKULL:
				H_bone[idx].rot[cur_frm] =t_rot;
			 break;
			 default:
				A_bone[idx].rot[cur_frm] =t_rot;
				break;
		 }
}

MyVector CVH_RENDER::Ret_Array(int grp_id, int grp_idx, int cur_frm)
{
	MyVector ret_vec;
		switch(grp_id)
		{
		     case LEFT_LEG:
				ret_vec = LL_bone[grp_idx].rot[cur_frm];
				break;
			 case RIGHT_LEG:
				ret_vec = RL_bone[grp_idx].rot[cur_frm];
				break;
			 case LEFT_ARM:
				ret_vec = LA_bone[grp_idx].rot[cur_frm];
				break;
			 case RIGHT_ARM:
				ret_vec = RA_bone[grp_idx].rot[cur_frm];
				break;
			 case SKULL:
				ret_vec = H_bone[grp_idx].rot[cur_frm];
			 break;
			 default:
				ret_vec = A_bone[grp_idx].rot[cur_frm];
				break;
		}
		return ret_vec;
}

MyVector CVH_RENDER::Ret_Global(int cur_frm, int grp_id)
{
	MyVector rc;
	if (grp_id == LEFT_ARM)
	{
		rc.x= LA_bone[3].position_global[cur_frm].x; 
		rc.y= LA_bone[3].position_global[cur_frm].y;
		rc.z= LA_bone[3].position_global[cur_frm].z;
	}
	else if (grp_id == RIGHT_ARM)
	{
	    rc.x= RA_bone[3].position_global[cur_frm].x; 
		rc.y= RA_bone[3].position_global[cur_frm].y;
		rc.z= RA_bone[3].position_global[cur_frm].z;
	}
	else
	{
		rc.x= (LA_bone[3].position_global[cur_frm].x+RA_bone[3].position_global[cur_frm].x)/2; 
		rc.y= (LA_bone[3].position_global[cur_frm].y+RA_bone[3].position_global[cur_frm].y)/2;
		rc.z= (LA_bone[3].position_global[cur_frm].z+RA_bone[3].position_global[cur_frm].z)/2;
	}
	return(rc);
}

void CVH_RENDER::Com_Interp(int frm, int pre_frm)
{
	for(int i=0;i<3;i++)
	{
		A_bone[i].trans[frm]=A_bone[i].trans[pre_frm]+(A_bone[i].trans[frm]-A_bone[i].trans[pre_frm])*Anim.AnimBlend;
		A_bone[i].rot[frm]  =A_bone[i].rot[pre_frm]+(A_bone[i].rot[frm]-A_bone[i].rot[pre_frm])*Anim.AnimBlend;
		
		LL_bone[i].rot[frm]=LL_bone[i].rot[pre_frm]+(LL_bone[i].rot[frm]-LL_bone[i].rot[pre_frm])*Anim.AnimBlend;
		LA_bone[i].rot[frm]=LA_bone[i].rot[pre_frm]+(LA_bone[i].rot[frm]-LA_bone[i].rot[pre_frm])*Anim.AnimBlend;
		RL_bone[i].rot[frm]=RL_bone[i].rot[pre_frm]+(RL_bone[i].rot[frm]-RL_bone[i].rot[pre_frm])*Anim.AnimBlend;
		RA_bone[i].rot[frm]=RA_bone[i].rot[pre_frm]+(RA_bone[i].rot[frm]-LA_bone[i].rot[pre_frm])*Anim.AnimBlend;
		if(i<2)
		 H_bone[i].rot[frm]=H_bone[i].rot[pre_frm]+(H_bone[i].rot[frm]-H_bone[i].rot[pre_frm])*Anim.AnimBlend;
	}
	RA_bone[4].rot[frm]=RA_bone[4].rot[pre_frm]+(RA_bone[4].rot[frm]-RA_bone[4].rot[pre_frm])*Anim.AnimBlend;
	LA_bone[4].rot[frm]=LA_bone[4].rot[pre_frm]+(LA_bone[4].rot[frm]-LA_bone[4].rot[pre_frm])*Anim.AnimBlend;
}

#endif