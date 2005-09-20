#if !defined(VHRENDER__H__INCLUDED_)
#define VHRENDER_H__INCLUDED_

#include "mymath.h"
#include "joint.h"

const double MITER_TO_INCH = 39.37; //1m=39.37inch 3m->118inch
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
     fov = 30.0; //30
     distZ = -150; //180.0;
	 
    camera.x=0.0f;  //-5.0
	camera.y=-5.0f;
	camera.z= 0.0f;
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
			   if (j == 0) tpos = CopyVector(MModel.A_bone[j].trans[i]); //if root
			   else  tpos =CopyVector(MModel.A_bone[j].b_trans); 
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
					tpos = CopyVector(MModel.LA_bone[idx].b_trans); 
					break;
				case RIGHT_ARM:
					tpos = CopyVector(MModel.RA_bone[idx].b_trans); 
					break;
				case SKULL:
					tpos = CopyVector(MModel.H_bone[idx].b_trans); 
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
			yaw = DegreesToRadians(MModel.A_bone[j].rot[i].z);
			roll = DegreesToRadians(MModel.A_bone[j].rot[i].x);
			pitch = DegreesToRadians(MModel.A_bone[j].rot[i].y);
		    MModel.A_bone[j].quat = MakeQFromEulerAngles(roll, pitch, yaw);
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
					yaw = DegreesToRadians(MModel.LA_bone[idx].rot[i].z);
					roll = DegreesToRadians(MModel.LA_bone[idx].rot[i].x);
					pitch = DegreesToRadians(MModel.LA_bone[idx].rot[i].y);
					MModel.LA_bone[idx].quat = MakeQFromEulerAngles(roll, pitch, yaw);
					break;
				case RIGHT_ARM:
					yaw = DegreesToRadians(MModel.RA_bone[idx].rot[i].z);
					roll = DegreesToRadians(MModel.RA_bone[idx].rot[i].x);
					pitch = DegreesToRadians(MModel.RA_bone[idx].rot[i].y);
					MModel.RA_bone[idx].quat = MakeQFromEulerAngles(roll, pitch, yaw);
					break;
				 case SKULL:
					yaw = DegreesToRadians(MModel.H_bone[idx].rot[i].z);
					roll = DegreesToRadians(MModel.H_bone[idx].rot[i].x);
					pitch = DegreesToRadians(MModel.H_bone[idx].rot[i].y);
					MModel.H_bone[idx].quat = MakeQFromEulerAngles(roll, pitch, yaw);
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
			   MModel.A_bone[j].position_global[i] = endMyVector;
			}
			else
			{
				if (j== child_cnt) idx = child_cnt - 4;
				switch(group_id)
				{
					case LEFT_ARM:
						MModel.LA_bone[idx].position_global[i] = endMyVector;
						break;
					case RIGHT_ARM:
						MModel.RA_bone[idx].position_global[i] = endMyVector;
						break;
					case SKULL:
						MModel.H_bone[idx].position_global[i] =  endMyVector;
						break;
				 }
			} //idx= child_cnt - 4  end effector index
		
		}
	}

//create BBox min
MModel.A_bone[0].bbox[0] = (float)MModel.LA_bone[0].position_global[0].x;
MModel.A_bone[0].bbox[1] = (float)MModel.A_bone[0].position_global[0].y;
MModel.A_bone[0].bbox[2] = (float)MModel.A_bone[0].position_global[0].z;
MModel.A_bone[0].bbox[3] = (float)MModel.RA_bone[0].position_global[0].x;
MModel.A_bone[0].bbox[4] = (float)MModel.H_bone[1].position_global[0].y;
MModel.A_bone[0].bbox[5] = (float)MModel.RA_bone[0].position_global[0].z;

MModel.A_bone[0].center.y=(MModel.A_bone[0].bbox[1]+MModel.A_bone[0].bbox[4])/2;
MModel.A_bone[0].center.x=(MModel.A_bone[0].bbox[0]+MModel.A_bone[0].bbox[3])/2;
MModel.A_bone[0].center.z=(MModel.A_bone[0].bbox[2]+MModel.A_bone[0].bbox[5])/2;
//	delete t_Bone;
}

void CVH_RENDER::Create_Body(MyBone t_body)
{
//10/27 JEONG
	   int idx = t_body.group_index;
       
		switch(t_body.group_id)
		 {
		     case LEFT_LEG:
				
				MModel.LL_bone[idx].group_id = t_body.group_id;
				MModel.LL_bone[idx].id = t_body.id;
				strcpy(MModel.LL_bone[idx].name, t_body.name);
				MModel.LL_bone[idx].scale = t_body.scale;
				MModel.LL_bone[idx].group_index = idx;
				MModel.LL_bone[idx].b_trans =t_body.b_trans;
				//MModel.LL_bone[idx].b_trans.y =t_body.b_trans.y;
				//MModel.LL_bone[idx].b_trans.z =t_body.b_trans.z;
				break;
			 case RIGHT_LEG:
			
				MModel.RL_bone[idx].group_id = t_body.group_id;
				MModel.RL_bone[idx].id = t_body.id;
				strcpy(MModel.RL_bone[idx].name, t_body.name);
				MModel.RL_bone[idx].scale = t_body.scale;
				MModel.RL_bone[idx].group_index = idx;
				MModel.RL_bone[idx].b_trans =t_body.b_trans;
				
				break;
			 case LEFT_ARM:
			
				MModel.LA_bone[idx].group_id = t_body.group_id;
				MModel.LA_bone[idx].id = t_body.id;
				strcpy(MModel.LA_bone[idx].name , t_body.name);
				MModel.LA_bone[idx].scale = t_body.scale;
				MModel.LA_bone[idx].group_index = idx;
				MModel.LA_bone[idx].b_trans =t_body.b_trans;
				
				break;
			 case RIGHT_ARM:
			
				MModel.RA_bone[idx].group_id = t_body.group_id;
				MModel.RA_bone[idx].id = t_body.id;
				strcpy(MModel.RA_bone[idx].name , t_body.name);
				MModel.RA_bone[idx].scale = t_body.scale;
				MModel.RA_bone[idx].group_index = idx;
				MModel.RA_bone[idx].b_trans =t_body.b_trans;
				
				break;
			 case SKULL:
				MModel.H_bone[idx].group_id = t_body.group_id;
				MModel.H_bone[idx].id = t_body.id;
				strcpy(MModel.H_bone[idx].name , t_body.name);
				MModel.H_bone[idx].scale = t_body.scale;

				MModel.H_bone[idx].group_index = idx;
				MModel.H_bone[idx].b_trans =t_body.b_trans;
				
			   break;
			 default:
	
				MModel.A_bone[idx].group_id = t_body.group_id;
				MModel.A_bone[idx].group_index = idx;
				MModel.A_bone[idx].id = t_body.id;
				strcpy(MModel.A_bone[idx].name , t_body.name);
				MModel.A_bone[idx].scale = t_body.scale;
				MModel.A_bone[idx].b_trans =t_body.b_trans;
				
				break;
		 }

}

void CVH_RENDER::Create_Motion(MyVector t_trans, MyVector t_rot, long g_id, int idx, int cur_frm)
{
		switch(g_id)
		 {
		     case LEFT_LEG:
				MModel.LL_bone[idx].trans[cur_frm] =t_trans;
				MModel.LL_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case RIGHT_LEG:
				MModel.RL_bone[idx].trans[cur_frm] =t_trans;
				MModel.RL_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case LEFT_ARM:
				MModel.LA_bone[idx].trans[cur_frm] =t_trans;
				MModel.LA_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case RIGHT_ARM:
				MModel.RA_bone[idx].trans[cur_frm] =t_trans;
				MModel.RA_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case SKULL:
				MModel.H_bone[idx].trans[cur_frm] =t_trans;
				MModel.H_bone[idx].rot[cur_frm] =t_rot;
			 break;
			 default:
				MModel.A_bone[idx].trans[cur_frm] =t_trans;
				MModel.A_bone[idx].rot[cur_frm] =t_rot;
				break;
		 }
}

void CVH_RENDER::Create_Motion(int def_idx, MyVector t_trans, MyVector t_rot, long g_id, int idx, int cur_frm)
{
		switch(g_id)
		 {
		     case LEFT_LEG:
				dfmotion.d_motion[def_idx].LL_bone[idx].trans[cur_frm] =t_trans;
				dfmotion.d_motion[def_idx].LL_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case RIGHT_LEG:
				dfmotion.d_motion[def_idx].RL_bone[idx].trans[cur_frm] =t_trans;
				dfmotion.d_motion[def_idx].RL_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case LEFT_ARM:
				dfmotion.d_motion[def_idx].LA_bone[idx].trans[cur_frm] =t_trans;
				dfmotion.d_motion[def_idx].LA_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case RIGHT_ARM:
				dfmotion.d_motion[def_idx].RA_bone[idx].trans[cur_frm] =t_trans;
				dfmotion.d_motion[def_idx].RA_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case SKULL:
				dfmotion.d_motion[def_idx].H_bone[idx].trans[cur_frm] =t_trans;
				dfmotion.d_motion[def_idx].H_bone[idx].rot[cur_frm] =t_rot;
			 break;
			 default:
				dfmotion.d_motion[def_idx].A_bone[idx].trans[cur_frm] =t_trans;
				dfmotion.d_motion[def_idx].A_bone[idx].rot[cur_frm] =t_rot;
				break;
		 }
}

void CVH_RENDER::move_motion(int idx, int Max_num)
{
		
	for(int i=0; i< Max_num; i++)
	{
		MModel.A_bone[0].rot[i] = dfmotion.d_motion[idx].A_bone[0].rot[i];
		MModel.A_bone[1].rot[i] = dfmotion.d_motion[idx].A_bone[1].rot[i];
		MModel.A_bone[2].rot[i] = dfmotion.d_motion[idx].A_bone[2].rot[i];
        MModel.A_bone[0].trans[i] = dfmotion.d_motion[idx].A_bone[0].trans[i];
		
		MModel.RL_bone[0].rot[i] = dfmotion.d_motion[idx].RL_bone[0].rot[i];
        MModel.RL_bone[1].rot[i] = dfmotion.d_motion[idx].RL_bone[1].rot[i];
        MModel.RL_bone[2].rot[i] = dfmotion.d_motion[idx].RL_bone[2].rot[i];

		MModel.LL_bone[0].rot[i] = dfmotion.d_motion[idx].LL_bone[0].rot[i];
        MModel.LL_bone[1].rot[i] = dfmotion.d_motion[idx].LL_bone[1].rot[i];
        MModel.LL_bone[2].rot[i] = dfmotion.d_motion[idx].LL_bone[2].rot[i];

		MModel.LA_bone[0].rot[i] = dfmotion.d_motion[idx].LA_bone[0].rot[i];
		MModel.LA_bone[1].rot[i] = dfmotion.d_motion[idx].LA_bone[1].rot[i];
		MModel.LA_bone[2].rot[i] = dfmotion.d_motion[idx].LA_bone[2].rot[i];
		MModel.LA_bone[3].rot[i] = dfmotion.d_motion[idx].LA_bone[3].rot[i];

		MModel.RA_bone[0].rot[i] = dfmotion.d_motion[idx].RA_bone[0].rot[i];
		MModel.RA_bone[1].rot[i] = dfmotion.d_motion[idx].RA_bone[1].rot[i];
		MModel.RA_bone[2].rot[i] = dfmotion.d_motion[idx].RA_bone[2].rot[i];
		MModel.RA_bone[3].rot[i] = dfmotion.d_motion[idx].RA_bone[3].rot[i];

		MModel.H_bone[0].rot[i] = dfmotion.d_motion[idx].H_bone[0].rot[i];
		MModel.H_bone[1].rot[i] = dfmotion.d_motion[idx].H_bone[1].rot[i];
	}
		 
}

void CVH_RENDER::input_motion(MyVector t_rot, long g_id, int idx, int cur_frm)
{
		switch(g_id)
		 {
		     case LEFT_LEG:
				MModel.LL_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case RIGHT_LEG:
				MModel.RL_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case LEFT_ARM:
				MModel.LA_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case RIGHT_ARM:
				MModel.RA_bone[idx].rot[cur_frm] =t_rot;
				break;
			 case SKULL:
				MModel.H_bone[idx].rot[cur_frm] =t_rot;
			 break;
			 default:
				MModel.A_bone[idx].rot[cur_frm] =t_rot;
				break;
		 }
}

MyVector CVH_RENDER::Ret_Array(int grp_id, int grp_idx, int cur_frm)
{
	MyVector ret_vec;
		switch(grp_id)
		{
		     case LEFT_LEG:
				ret_vec = MModel.LL_bone[grp_idx].rot[cur_frm];
				break;
			 case RIGHT_LEG:
				ret_vec = MModel.RL_bone[grp_idx].rot[cur_frm];
				break;
			 case LEFT_ARM:
				ret_vec = MModel.LA_bone[grp_idx].rot[cur_frm];
				break;
			 case RIGHT_ARM:
				ret_vec = MModel.RA_bone[grp_idx].rot[cur_frm];
				break;
			 case SKULL:
				ret_vec = MModel.H_bone[grp_idx].rot[cur_frm];
			 break;
			 default:
				ret_vec = MModel.A_bone[grp_idx].rot[cur_frm];
				break;
		}
		return ret_vec;
}

MyVector CVH_RENDER::Ret_Global(int cur_frm, int grp_id)
{
	MyVector rc;
	if (grp_id == LEFT_ARM)
	{
		rc.x= MModel.LA_bone[3].position_global[cur_frm].x; 
		rc.y= MModel.LA_bone[3].position_global[cur_frm].y;
		rc.z= MModel.LA_bone[3].position_global[cur_frm].z;
	}
	else if (grp_id == RIGHT_ARM)
	{
	    rc.x= MModel.RA_bone[3].position_global[cur_frm].x; 
		rc.y= MModel.RA_bone[3].position_global[cur_frm].y;
		rc.z= MModel.RA_bone[3].position_global[cur_frm].z;
	}
	else
	{
		rc.x= (MModel.LA_bone[3].position_global[cur_frm].x+MModel.RA_bone[3].position_global[cur_frm].x)/2; 
		rc.y= (MModel.LA_bone[3].position_global[cur_frm].y+MModel.RA_bone[3].position_global[cur_frm].y)/2;
		rc.z= (MModel.LA_bone[3].position_global[cur_frm].z+MModel.RA_bone[3].position_global[cur_frm].z)/2;
	}
	return(rc);
}

void CVH_RENDER::Com_Interp(int frm, int pre_frm)
{
	for(int i=0;i<3;i++)
	{
		MModel.A_bone[i].trans[frm]=MModel.A_bone[i].trans[pre_frm]+(MModel.A_bone[i].trans[frm]-MModel.A_bone[i].trans[pre_frm])*Anim.AnimBlend;
		MModel.A_bone[i].rot[frm]  =MModel.A_bone[i].rot[pre_frm]+(MModel.A_bone[i].rot[frm]-MModel.A_bone[i].rot[pre_frm])*Anim.AnimBlend;
		
		MModel.LL_bone[i].rot[frm]=MModel.LL_bone[i].rot[pre_frm]+(MModel.LL_bone[i].rot[frm]-MModel.LL_bone[i].rot[pre_frm])*Anim.AnimBlend;
		MModel.LA_bone[i].rot[frm]=MModel.LA_bone[i].rot[pre_frm]+(MModel.LA_bone[i].rot[frm]-MModel.LA_bone[i].rot[pre_frm])*Anim.AnimBlend;
		MModel.RL_bone[i].rot[frm]=MModel.RL_bone[i].rot[pre_frm]+(MModel.RL_bone[i].rot[frm]-MModel.RL_bone[i].rot[pre_frm])*Anim.AnimBlend;
		MModel.RA_bone[i].rot[frm]=MModel.RA_bone[i].rot[pre_frm]+(MModel.RA_bone[i].rot[frm]-MModel.LA_bone[i].rot[pre_frm])*Anim.AnimBlend;
		if(i<2)
		 MModel.H_bone[i].rot[frm]=MModel.H_bone[i].rot[pre_frm]+(MModel.H_bone[i].rot[frm]-MModel.H_bone[i].rot[pre_frm])*Anim.AnimBlend;
	}
	MModel.RA_bone[4].rot[frm]=MModel.RA_bone[4].rot[pre_frm]+(MModel.RA_bone[4].rot[frm]-MModel.RA_bone[4].rot[pre_frm])*Anim.AnimBlend;
	MModel.LA_bone[4].rot[frm]=MModel.LA_bone[4].rot[pre_frm]+(MModel.LA_bone[4].rot[frm]-MModel.LA_bone[4].rot[pre_frm])*Anim.AnimBlend;
}

#endif