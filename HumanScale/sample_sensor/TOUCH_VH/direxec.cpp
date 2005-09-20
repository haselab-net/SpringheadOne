#include "direxec.h"
///////////////////////////////////////
static int temp_id=0;
static int frame_idx=0;

// Constructor initializes the string chr_ds_name with the
// data source name.
Cdirexec::Cdirexec()
{
   memcpy(chr_ds_name,"motion_db", 9);
   memcpy(chr_us_name,"demo", 4);
   memcpy(chr_ps_name,"demo", 4);
}

Cdirexec::~Cdirexec()
{
	  sqldisconn();
}
// Allocate environment handle, allocate connection handle,
// connect to data source, and allocate statement handle.
int Cdirexec::sqlconn(void)
{
   
   SQLAllocEnv(&henv);
   SQLAllocConnect(henv,&hdbc);
   rc=SQLConnect(hdbc,chr_ds_name,SQL_NTS,chr_us_name,SQL_NTS,chr_ps_name,SQL_NTS);
   
   // Deallocate handles, display error message, and exit.
   if (!MYSQLSUCCESS(rc))
   {
      SQLFreeEnv(henv);
      SQLFreeConnect(hdbc);
      error_out();
      return(-1);
      exit(-1);
   }
   else
   {
	   return(1);
   }
   //rc=SQLAllocStmt(hdbc,&hstmt);

}

char* Cdirexec::make_sql(int init_type, int which_model)
{
   char strSQL[80]="";
   char buffer[10]="";
   char* pset;
 
   //motion_type
    vh[which_model].Anim.first_type = init_type;
 // single motion type search
	strcpy(strSQL, "exec sp_searchmotion ");
	_itoa(init_type, buffer, 10);
	strcat(strSQL, buffer);
	strcat(strSQL, ", ");
	// start frame
	_itoa(1,buffer,10);
	pset= strcat(strSQL, buffer);
   return pset;

}

// Execute SQL command with SQLExecDirect() ODBC API.
int Cdirexec::sqlexec(char* cmdstr, int which_model)
{
	 // SQLBindCol variables
	SQLINTEGER   cbValue[10];
	long cur_frame, bone_id;
	int row_cnt =0;
	long g_id, idx;
    int frame_index=0;

	MyVector temp_trans;
	MyVector temp_rot;

    rc=SQLAllocStmt(hdbc,&hstmt);
	rc=SQLExecDirect(hstmt,(SQLCHAR*) cmdstr,SQL_NTS);

   if (!MYSQLSUCCESS(rc))  //Error
   {
      error_out();
     
   }
   else
   {
	   SQLBindCol(hstmt, 1, SQL_C_LONG, &cur_frame, sizeof(long), &cbValue[0]);
	   
	   	while ( (rc = SQLFetch(hstmt) ) != SQL_NO_DATA )
		{
		    frame_index = cur_frame-1;
			SQLGetData(hstmt, 2, SQL_C_LONG, &bone_id, sizeof(long), &cbValue[1]);
			SQLGetData(hstmt, 3, SQL_C_LONG,&g_id, sizeof(long), &cbValue[2]);
		    SQLGetData(hstmt, 4, SQL_C_LONG,&idx, sizeof(long), &cbValue[3]);
		    
		
		   SQLGetData(hstmt, 5, SQL_C_DOUBLE,&temp_trans.x, sizeof(double), &cbValue[4]);
		   SQLGetData(hstmt, 6, SQL_C_DOUBLE,&temp_trans.y, sizeof(double), &cbValue[5]);
		   SQLGetData(hstmt, 7, SQL_C_DOUBLE,&temp_trans.z, sizeof(double), &cbValue[6]);

		   SQLGetData(hstmt, 8, SQL_C_DOUBLE,&temp_rot.z, sizeof(double), &cbValue[7]);
		   SQLGetData(hstmt, 9, SQL_C_DOUBLE,&temp_rot.x, sizeof(double), &cbValue[8]);
		   SQLGetData(hstmt, 10, SQL_C_DOUBLE,&temp_rot.y, sizeof(double), &cbValue[9]);
		   
	       vh[which_model].Create_Motion(temp_trans, temp_rot, g_id, idx, frame_index);
		   row_cnt = row_cnt +1;
		 }
      }

   if (vh[which_model].Anim.second_type == 0) //if single motion
   {
		vh[which_model].Anim.Frame_num = (int) row_cnt/19;	
		vh[which_model].Anim.Pre_frm = (int) row_cnt/19;	
   }
   //clear up
   SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

   return row_cnt;
}
 
// Execute SQL command with SQLExecDirect() ODBC API.
int Cdirexec::sqlexec(char* cmdstr)
{
	  // SQLBindCol variables
	SQLINTEGER   cbValue[10];
	long cur_frame, bone_id;
	int row_cnt =0;
	long g_id, idx;
    int frame_index=0;

	MyVector temp_trans;
	MyVector temp_rot;

    rc=SQLAllocStmt(hdbc,&hstmt);
	rc=SQLExecDirect(hstmt,(SQLCHAR*) cmdstr,SQL_NTS);

   if (!MYSQLSUCCESS(rc))  //Error
   {
      error_out();
      //Deallocate handles and disconnect.
      //SQLFreeStmt(hstmt,SQL_DROP);
      
   }
   else
   {
	   SQLBindCol(hstmt, 1, SQL_C_LONG, &cur_frame, sizeof(long), &cbValue[0]);
	   
	   	while ( (rc = SQLFetch(hstmt) ) != SQL_NO_DATA )
		{
		    frame_index = cur_frame-1;
			SQLGetData(hstmt, 2, SQL_C_LONG, &bone_id, sizeof(long), &cbValue[1]);
			SQLGetData(hstmt, 3, SQL_C_LONG,&g_id, sizeof(long), &cbValue[2]);
		    SQLGetData(hstmt, 4, SQL_C_LONG,&idx, sizeof(long), &cbValue[3]);
		    
		
		   SQLGetData(hstmt, 5, SQL_C_DOUBLE,&temp_trans.x, sizeof(double), &cbValue[4]);
		   SQLGetData(hstmt, 6, SQL_C_DOUBLE,&temp_trans.y, sizeof(double), &cbValue[5]);
		   SQLGetData(hstmt, 7, SQL_C_DOUBLE,&temp_trans.z, sizeof(double), &cbValue[6]);

		   SQLGetData(hstmt, 8, SQL_C_DOUBLE,&temp_rot.z, sizeof(double), &cbValue[7]);
		   SQLGetData(hstmt, 9, SQL_C_DOUBLE,&temp_rot.x, sizeof(double), &cbValue[8]);
		   SQLGetData(hstmt, 10, SQL_C_DOUBLE,&temp_rot.y, sizeof(double), &cbValue[9]);
		   
	       vh[1].Create_Motion(temp_trans, temp_rot, g_id, idx, frame_index);
		   row_cnt = row_cnt +1;
		 }
      }

   if (vh[1].Anim.second_type == 0) //if single motion
   {
		vh[1].Anim.Frame_num = (int) row_cnt/19;	
		vh[1].Anim.Pre_frm = (int) row_cnt/19;	
   }
   //clear up
   SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

   return row_cnt;
}

// Free the statement handle, disconnect, free the connection handle, and
// free the environment handle.
void Cdirexec::sqldisconn(void)
{
  // SQLFreeStmt(hstmt,SQL_DROP);
   //SQLFreeConnect(hdbc);
   //SQLFreeEnv(henv);

  // SQLDisconnect(hdbc);
   SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
   SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

// model type search (1: Bvh[1] 2: character 3:jeong)
int Cdirexec::load_model(int model_type)
{
      
	  SQLINTEGER   cbValue[10];
	  SQLCHAR      szName[MAXNAME+1];	
	  char strSQL[256]="";
      char buffer[10]="";
	  MyBone model_bone;
	  int m_id= 1;
	  
	if (model_type == 3 ) m_id =0; //9
	  //alloc  
    rc = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    strcpy(strSQL, 
		"Select b.group_id, a.bone_id, convert(char(16),b.bone_name), a.scale, a.pos_x, a.pos_y, a.pos_z, b.child_id  from bone_info b, model_info a where a.bone_id=b.bone_id and a.model_id=");
    _itoa(model_type, buffer,10);
    strcat(strSQL, buffer);
    strcat(strSQL, " and a.end_site = b.end_site and a.end_site = 0 order by b.group_id, b.bone_id");
		
   rc = SQLExecDirect(hstmt, (SQLCHAR*) strSQL,SQL_NTS);
    
   if ( !MYSQLSUCCESS(rc) )
   {
        error_out();
	    return(9);
   }
   else
   {
	   rc = SQLBindCol(hstmt, 1, SQL_C_LONG, &model_bone.group_id, sizeof(long), &cbValue[0]);
   
	   while ( (rc = SQLFetch(hstmt) ) != SQL_NO_DATA )
	   {
			 SQLGetData(hstmt, 2, SQL_C_LONG, &model_bone.id, sizeof(long), &cbValue[1]);
			 SQLGetData(hstmt, 3, SQL_C_CHAR, szName, MAX_DATA, &cbValue[2]);
			 strcpy(model_bone.name, (char *)szName);

			 SQLGetData(hstmt, 4, SQL_C_DOUBLE,&model_bone.scale.x, sizeof(double), &cbValue[3]);
			 model_bone.scale.y=model_bone.scale.x;
			 model_bone.scale.z=model_bone.scale.x;
			 SQLGetData(hstmt, 5, SQL_C_DOUBLE,&model_bone.b_trans.x, sizeof(double), &cbValue[4]);
			 SQLGetData(hstmt, 6, SQL_C_DOUBLE,&model_bone.b_trans.y, sizeof(double), &cbValue[5]);
			 SQLGetData(hstmt, 7, SQL_C_DOUBLE, &model_bone.b_trans.z, sizeof(double), &cbValue[6]);
			 //tt_bone.b_trans*=(1/39.37); 
			 SQLGetData(hstmt, 8, SQL_C_LONG, &model_bone.group_index, sizeof(long), &cbValue[7]);
			 vh[m_id].Create_Body(model_bone);
		}
   }
   /* Clean up. */
   SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
   return(1);
}
// motion_data per frame and bone

int Cdirexec::load_motion(int de_idx)
{
      
	  SQLINTEGER  cbValue[10];
	  char strSQL[256]="";
      char buffer[20]="";
	  long cur_frame, bone_id;
	  int row_cnt =0;
	  long g_id, idx;
      long mm_type;

	  MyVector temp_trans;
	  MyVector temp_rot;

	  switch(de_idx)
	  {
	  case 0:
		  mm_type = FREE_TALK;
		  break;
	  case 1:
		  mm_type = TURN_LEFT;
		  break;
      case 2:
		  mm_type = TURN_RIGHT;
		  break;
	  }     
      vh[1].dfmotion.m_id[de_idx] = mm_type;  
	 //SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    
	//motion_type, start_frm
	strcpy(strSQL, "exec sp_searchmotion ");
	_itoa(mm_type,buffer,10);
	strcat(strSQL, buffer);
	strcat(strSQL, ", ");
	_itoa(1,buffer,10);
	strcat(strSQL, buffer);
   
    rc=SQLAllocStmt(hdbc,&hstmt);
	rc=SQLExecDirect(hstmt,(SQLCHAR*) strSQL, SQL_NTS);
    
   if ( !MYSQLSUCCESS(rc))
   {
         error_out();
	     return(9);
   }
   else
   {
		SQLBindCol(hstmt, 1, SQL_C_LONG, &cur_frame, sizeof(long), &cbValue[0]);
	   while ( (rc = SQLFetch(hstmt) ) != SQL_NO_DATA )
	   {
		   SQLGetData(hstmt, 2, SQL_C_LONG, &bone_id, sizeof(long), &cbValue[1]);
		   SQLGetData(hstmt, 3, SQL_C_LONG,&g_id, sizeof(long), &cbValue[2]);
		   SQLGetData(hstmt, 4, SQL_C_LONG,&idx, sizeof(long), &cbValue[3]);

		   temp_id = bone_id-1;
		   frame_idx = cur_frame -1; 
		   if (temp_id == 0 ) row_cnt++;   
		   
		   SQLGetData(hstmt, 5, SQL_C_DOUBLE,&temp_trans.x, sizeof(double), &cbValue[4]);
		   SQLGetData(hstmt, 6, SQL_C_DOUBLE,&temp_trans.y, sizeof(double), &cbValue[5]);
		   SQLGetData(hstmt, 7, SQL_C_DOUBLE,&temp_trans.z, sizeof(double), &cbValue[6]);

		   SQLGetData(hstmt, 8, SQL_C_DOUBLE,&temp_rot.z, sizeof(double), &cbValue[7]);
		   SQLGetData(hstmt, 9, SQL_C_DOUBLE,&temp_rot.x, sizeof(double), &cbValue[8]);
		   SQLGetData(hstmt, 10, SQL_C_DOUBLE,&temp_rot.y, sizeof(double), &cbValue[9]);
		   
		   vh[1].Create_Motion(de_idx, temp_trans, temp_rot, g_id, idx, frame_idx);
	   }
       vh[1].dfmotion.total_frm[de_idx] = row_cnt;  
   }
   /* Clean up. */
   SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
   return(row_cnt);
}

// Display error message in a message box that has an OK button.
void Cdirexec::error_out(void)
{
   unsigned char szSQLSTATE[10];
   SDWORD nErr;
   unsigned char msg[SQL_MAX_MESSAGE_LENGTH+1];
   SWORD cbmsg;

   while(SQLError(0,0,hstmt,szSQLSTATE,&nErr,msg,sizeof(msg),&cbmsg)==
   SQL_SUCCESS)
   {
      wsprintf((char *)szData,"Error:\nSQLSTATE=%s,Native error=%ld,msg='%s'",szSQLSTATE,nErr,msg);
      MessageBox(NULL,(const char *)szData,"ODBC Error",MB_OK);

   }

}
