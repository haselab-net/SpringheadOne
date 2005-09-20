#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcss.h>
///////////////////////////
#include "joint.h"
Anim_info Anim;
///////////////////////////

#define MAXBUFLEN   255

SQLHENV      henv = SQL_NULL_HENV;
SQLHDBC      hdbc1 = SQL_NULL_HDBC;     
SQLHSTMT     hstmt1 = SQL_NULL_HSTMT;

static char logstring[MAXBUFLEN] = "";
//query time test
static DWORD m_StartTime=0.0;
static DWORD m_ElapsedTime=0.0;
///////////////////////////////////////
//double PREV_FRAME[MAX_BONES+1][3]; 
static int sel_throw[30][2];

static int temp_id=0;
static int frame_idx=0;


void  ProcessLogMessages(SQLSMALLINT plm_handle_type,
                     SQLHANDLE plm_handle, char *logstring,int ConnInd);

int select_motion(int init_type, double k_frm, double b_pos[3]);
int load_model(int model_type);
int load_basic(int m_type, int mm_type);
int load_motion(int frame_cnt, int m_type);
int random_throw();
void Com_Interp(int frm);

// DATABASE CONNECT : only one-time 
int connection()
{
   RETCODE retcode;
    // Allocate the ODBC Environment and save handle.
   retcode = SQLAllocHandle (SQL_HANDLE_ENV, NULL, &henv);
   // Notify ODBC that this is an ODBC 3.0 application.
   retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
                     (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
   // Allocate an ODBC connection and connect.
   retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc1);
   retcode = SQLConnect(hdbc1,
            (SQLCHAR*) "Motion_DB", SQL_NTS, (SQLCHAR*) "demo", SQL_NTS,
            (SQLCHAR*) "demo", SQL_NTS);

   if ( (retcode != SQL_SUCCESS) &&
        (retcode != SQL_SUCCESS_WITH_INFO) ) {
         ProcessLogMessages(SQL_HANDLE_DBC, hdbc1,
               "SQLConnect() Failed\n\n", FALSE);
         return(9);
   }
   else {
         ProcessLogMessages(SQL_HANDLE_DBC, hdbc1,
                     "\nConnect Successful\n\n", FALSE);
		return(1);
   }
   
 }

// hand 3D position search 
int select_motion(int init_type, double k_frm, double b_pos[3])
{
   RETCODE retcode;
   // SQLBindCol variables
   SQLINTEGER   cbValue[10];
   char strSQL[600];
   char buffer[10]="";
   char strWhere[256]="";
   char strCmp[256]="";
   int t_frame=0, tt=0;
   int sel=0, row_cnt=0;
   int m_type=0, mm_type=0;
   // Allocate a statement handle.

   if (init_type != 0)
   {
      retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
      // key_motion search
   
	   strcpy(strWhere, " where key_id=1 and joint2 <> 0 and abs(touch_frame-");
	    _gcvt(k_frm, 7,buffer);
       strcat(strWhere, buffer);
	   strcat(strWhere, ")<= 4 and abs((pos_z1+pos_z2)/2-(");
       _gcvt(b_pos[2], 7,buffer);
       strcat(strWhere, buffer);
       strcat(strWhere, ")) <= 6 and ");
// z position is compared
	   strcpy(strCmp, "sqrt(power((pos_x1+pos_x2)/2-convert(real,");
	   _gcvt(b_pos[0], 7,buffer);
       strcat(strCmp, buffer);
	   strcat(strCmp, " ), 2) + power((pos_y1+pos_y2)/2-convert(real,");
	    _gcvt(b_pos[1], 7,buffer);
	   strcat(strCmp, buffer);
	   strcat(strCmp, " ), 2)) ");
	   _gcvt(6, 7,buffer); // ball radius

	   strcat(strWhere, strCmp);
	   strcat(strWhere, " <=");
       strcat(strWhere, buffer);
	  
	     //strcat(strCmp, " ), 2) + power((pos_z1+pos_z2)/2-convert(real,");
	   // _gcvt(b_pos[2], 7,buffer);
	   //strcat(strCmp, buffer);
	   strcpy(strSQL," Select motion_type,touch_frame, c=");
	   strcat(strSQL, strCmp);
	   strcat(strSQL, " from key_motion");
	   strcat(strSQL, strWhere);
	   strcat(strSQL, " order by c desc");

	   //printf("SQL = %s \n", strSQL);
	   retcode = SQLExecDirect(hstmt1,
						 (SQLCHAR*) strSQL,
						 SQL_NTS);
    
	   if ( (retcode != SQL_SUCCESS) &&
			(retcode != SQL_SUCCESS_WITH_INFO) ) {
			 ProcessLogMessages(SQL_HANDLE_STMT, hstmt1,
					  "SQLExecute() Failed\n\n", TRUE);
	   // init_pose fixed or randomly able to select(dance, waiting)
			 return(0);
	   }
	   else {
            
			while ( (retcode = SQLFetch(hstmt1) ) != SQL_NO_DATA )
		   {
       			SQLGetData(hstmt1, 1, SQL_C_LONG,&mm_type, sizeof(long), &cbValue[0]);
				SQLGetData(hstmt1, 2, SQL_C_LONG,&t_frame, sizeof(long), &cbValue[1]);
				row_cnt = row_cnt + 1;
		   }
		}

	   SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);
   }

   //frame_num =0;
    Anim.init();
   if (row_cnt != 0)
   {
	   sel = rand() % Anim.Throw_num;
	   m_type = sel_throw[sel][0];
	   Anim.Off_frm= sel_throw[sel][1];

	   tt=load_basic(m_type, mm_type);
	   
	   Anim.Off_frm = Anim.Off_frm +tt; 
	   Anim.Pre_frm =tt;

	   load_motion(0,mm_type);
	   load_motion(tt, m_type);
	  // frist frame 
	   // Com_Interp(tt);
	   Anim.S_status = 1;
   }
   else  // no data
   {
	   mm_type= Anim.Init_code;
	   Anim.Off_frm =1;
	   tt=load_basic(0, mm_type); // waiting 
	   Anim.Pre_frm =tt;
	   load_motion(0,mm_type);
	   if (init_type !=0 ) Anim.S_status = 2;  // 
	   else Anim.S_status = 0;  // first pose
   }

   return(Anim.S_status);
}

int random_throw()
{
   RETCODE retcode;
   // SQLBindCol variables
   SQLINTEGER   cbValue[10];
   char strSQL[400]="";
   char strWhere[256]="";
   int row_cnt=0;
    // Allocate a statement handle.

   retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
   
   // throwing search
       strcpy(strSQL," Select motion_type,touch_frame from key_motion where key_id=2 order by motion_type");

	   retcode = SQLExecDirect(hstmt1,
						 (SQLCHAR*) strSQL,
						 SQL_NTS);
    
	   if ( (retcode == SQL_SUCCESS) ||	(retcode == SQL_SUCCESS_WITH_INFO) ) 
	   {

			while ( (retcode = SQLFetch(hstmt1) ) != SQL_NO_DATA )
			{
				    SQLGetData(hstmt1, 1, SQL_C_LONG,&sel_throw[row_cnt][0], sizeof(long), &cbValue[0]);
       				SQLGetData(hstmt1, 2, SQL_C_LONG,&sel_throw[row_cnt][1], sizeof(long), &cbValue[1]);
					row_cnt = row_cnt +1;
			 }

	   }
	   else
	   {
			 ProcessLogMessages(SQL_HANDLE_STMT, hstmt1,
					  "SQLExecute() Failed\n\n", TRUE);

			 return(9);
	   }
   
	    /* Clean up. */
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);

	return(row_cnt);
} //keycode ==2

// basic_motion 
int load_basic(int m_type, int mm_type)
{
      RETCODE retcode;
	  SQLINTEGER   cbValue[10];
	  char strSQL[256]="";
      char buffer[10]="";
	  SQLCHAR  szName[MAXNAME+1];	
      int row_cnt =0;
	  int f_frame=0, pre_frame = 0;
      int motion_type=0;

       retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
    strcpy(strSQL, 
		"Select frame_num, channel_cnt, motion_type, motion_name from basic_motion where motion_type=");
    _itoa(mm_type, buffer,10);
    strcat(strSQL, buffer);
   if (m_type !=0 )  // throw 
   {
	  strcat(strSQL, " or motion_type =");
      _itoa(m_type, buffer,10);
      strcat(strSQL, buffer);
   }
   strcat(strSQL, " order by motion_type");

   retcode = SQLExecDirect(hstmt1,
                     (SQLCHAR*) strSQL,
                     SQL_NTS);
    
   if ( (retcode != SQL_SUCCESS) &&
        (retcode != SQL_SUCCESS_WITH_INFO) ) {
         ProcessLogMessages(SQL_HANDLE_STMT, hstmt1,
                  "SQLExecute() Failed\n\n", TRUE);
         return(9);
   }
   
   while ( (retcode = SQLFetch(hstmt1) ) != SQL_NO_DATA )
   {
         
	   	SQLGetData(hstmt1, 1, SQL_C_LONG,&f_frame, sizeof(long), &cbValue[0]);
        SQLGetData(hstmt1, 2, SQL_C_LONG,&Anim.Joint_cnt, sizeof(long), &cbValue[1]);
	    SQLGetData(hstmt1, 3, SQL_C_LONG,&motion_type, sizeof(long), &cbValue[2]);
	    SQLGetData(hstmt1, 4, SQL_C_CHAR,szName, MAXBUFLEN, &cbValue[3]);
		if (m_type !=0)  Anim.Frame_num = Anim.Frame_num + f_frame;
		else Anim.Frame_num = f_frame;
		row_cnt = row_cnt + 1;
   }
   if (Anim.Frame_num != f_frame ) pre_frame = Anim.Frame_num- f_frame;
   printf("total_frame = %d, pre_num= %d, Anim.motion_name= %d %s\n", Anim.Frame_num, pre_frame, mm_type,szName);
   strcpy( Anim.motion_name , (char*) szName);
   if (Anim.Frame_num > MAX_FRAMES ) Anim.Frame_num = MAX_FRAMES; //total frame is fixed by MAX_FRAMES //

   SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);
   return(pre_frame);
}


// model type search (1: BVH 2: character)
int load_model(int model_type)
{
      RETCODE retcode;
	  SQLINTEGER   cbValue[10];
	  char strSQL[256]="";
      char buffer[10]="";
	  MyBone tt_bone;

	  SQLCHAR      szName[MAXNAME+1];	
	  
    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
    strcpy(strSQL, 
		"Select b.group_id, a.bone_id, substring(b.bone_name,1,15), a.pos_x, a.pos_y, a.pos_z, b.child_id  from bone_info b, model_info a where a.bone_id=b.bone_id and a.model_id=");
    _itoa(model_type, buffer,10);
    strcat(strSQL, buffer);
    strcat(strSQL, " and a.end_site <> 1 order by a.bone_id");
		
   retcode = SQLExecDirect(hstmt1,
                     (SQLCHAR*) strSQL,
                     SQL_NTS);
    
   if ( (retcode != SQL_SUCCESS) &&
        (retcode != SQL_SUCCESS_WITH_INFO) ) {
         ProcessLogMessages(SQL_HANDLE_STMT, hstmt1,
                  "SQLExecute() Failed\n\n", TRUE);
         return(0);  //9
   }
   
   retcode = SQLBindCol(hstmt1, 1, SQL_C_LONG, &tt_bone.group_id, sizeof(long), &cbValue[0]);
   
   while ( (retcode = SQLFetch(hstmt1) ) != SQL_NO_DATA )
   {
         retcode = SQLGetData(hstmt1, 2, SQL_C_LONG, &tt_bone.id, sizeof(long), &cbValue[1]);
		 
         SQLGetData(hstmt1, 3, SQL_C_CHAR, szName, MAXBUFLEN, &cbValue[2]);
         strcpy(tt_bone.name, (char *)szName);
		
		 SQLGetData(hstmt1, 4, SQL_C_DOUBLE,&tt_bone.b_trans.x, sizeof(double), &cbValue[3]);
		 SQLGetData(hstmt1, 5, SQL_C_DOUBLE,&tt_bone.b_trans.y, sizeof(double), &cbValue[4]);
		 SQLGetData(hstmt1, 6, SQL_C_DOUBLE, &tt_bone.b_trans.z, sizeof(double), &cbValue[5]);

		 SQLGetData(hstmt1, 7, SQL_C_LONG, &tt_bone.group_index, sizeof(long), &cbValue[6]);

 		Create_Body(tt_bone);
	}
 
   /* Clean up. */
   SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);
   return(1);
}
// motion_data per frame and bone
int load_motion(int frame_cnt, int mm_type)
{
      RETCODE retcode;
	  SQLINTEGER   cbValue[10];
	  char strSQL[256]="";
      char buffer[20]="";
	  long cur_frame, bone_id;
	  int row_cnt =0;
	  long g_id, idx;

	  MyVector temp_trans;
	  MyVector temp_rot;

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
    strcpy(strSQL, 
		"Select a.frame_no, a.bone_id, a.trans_x, a.trans_y,a.trans_z, a.rot_z, a.rot_x, a.rot_y, b.group_id, b.child_id from motion_data a, bone_info b where a.bone_id=b.bone_id and a.motion_type=");
    _itoa(mm_type, buffer,10);
    strcat(strSQL, buffer);
	strcat(strSQL, " order by a.frame_no, a.bone_id");
   
   retcode = SQLExecDirect(hstmt1,
                     (SQLCHAR*) strSQL,
                     SQL_NTS);
    
   if ( (retcode != SQL_SUCCESS) &&
        (retcode != SQL_SUCCESS_WITH_INFO) ) {
         ProcessLogMessages(SQL_HANDLE_STMT, hstmt1,
                  "SQLExecute() Failed\n\n", TRUE);
         return(0);
   }
   
   retcode = SQLBindCol(hstmt1, 1, SQL_C_LONG,
                  &cur_frame, sizeof(long), &cbValue[0]);
   while ( (retcode = SQLFetch(hstmt1) ) != SQL_NO_DATA )
   {
       retcode = SQLGetData(hstmt1, 2, SQL_C_LONG, &bone_id, sizeof(long), &cbValue[1]);
	
	   temp_id = bone_id-1;
	   //frame_idx = cur_frame -1; //convert inch-> feet
	    frame_idx= frame_cnt +(cur_frame-1);
	   if (temp_id == 0 && frame_idx == 0) row_cnt++;   
	   
	   SQLGetData(hstmt1, 3, SQL_C_DOUBLE,&temp_trans.x, sizeof(double), &cbValue[2]);
	   SQLGetData(hstmt1, 4, SQL_C_DOUBLE,&temp_trans.y, sizeof(double), &cbValue[3]);
	   SQLGetData(hstmt1, 5, SQL_C_DOUBLE,&temp_trans.z, sizeof(double), &cbValue[4]);

	   SQLGetData(hstmt1, 6, SQL_C_DOUBLE,&temp_rot.z, sizeof(double), &cbValue[5]);
	   SQLGetData(hstmt1, 7, SQL_C_DOUBLE,&temp_rot.x, sizeof(double), &cbValue[6]);
	   SQLGetData(hstmt1, 8, SQL_C_DOUBLE,&temp_rot.y, sizeof(double), &cbValue[7]);

	   SQLGetData(hstmt1, 9, SQL_C_LONG,&g_id, sizeof(long), &cbValue[8]);
	   SQLGetData(hstmt1, 10, SQL_C_LONG,&idx, sizeof(long), &cbValue[9]);
	   
	   Create_Motion(temp_trans, temp_rot, g_id, idx, frame_idx);
	
   }
   /* Clean up. */
   SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);
   return(row_cnt);
}

void Com_Interp(int frm)
{
//	int i;

	//A_bone[frm][i][0]=PREV_FRAME[i][0]+(mov[frm][i][0]-PREV_FRAME[i][0])*Anim.AnimBlend;
	//A_bone[frm][i][1]=PREV_FRAME[i][1]+(mov[frm][i][1]-PREV_FRAME[i][1])*Anim.AnimBlend;
	//A_bone[frm][i][2]=PREV_FRAME[i][2]+(mov[frm][i][2]-PREV_FRAME[i][2])*Anim.AnimBlend;
	
}

// DATABASE DISCONNECT : only one-time exit
void disconnect()
{
   SQLDisconnect(hdbc1);
   SQLFreeHandle(SQL_HANDLE_DBC, hdbc1);
   SQLFreeHandle(SQL_HANDLE_ENV, henv);

}
void ProcessLogMessages(SQLSMALLINT plm_handle_type,
                  SQLHANDLE plm_handle,
                  char *logstring, int ConnInd)
{
   RETCODE      plm_retcode = SQL_SUCCESS;
   UCHAR      plm_szSqlState[MAXBUFLEN] = "",
            plm_szErrorMsg[MAXBUFLEN] = "";
   SDWORD      plm_pfNativeError = 0L;
   SWORD      plm_pcbErrorMsg = 0;
   SQLSMALLINT   plm_cRecNmbr = 1;
   SDWORD      plm_SS_MsgState = 0, plm_SS_Severity = 0;
   SQLINTEGER   plm_Rownumber = 0;
   USHORT      plm_SS_Line;
   SQLSMALLINT   plm_cbSS_Procname, plm_cbSS_Srvname;
   SQLCHAR      plm_SS_Procname[MAXNAME], plm_SS_Srvname[MAXNAME];

   printf(logstring);

   while (plm_retcode != SQL_NO_DATA_FOUND) {
      plm_retcode = SQLGetDiagRec(plm_handle_type, plm_handle,
         plm_cRecNmbr, plm_szSqlState, &plm_pfNativeError,
         plm_szErrorMsg, MAXBUFLEN - 1, &plm_pcbErrorMsg);

      // Note that if the application has not yet made a
      // successful connection, the SQLGetDiagField
      // information has not yet been cached by ODBC
      // Driver Manager and these calls to SQLGetDiagField
      // will fail.
      if (plm_retcode != SQL_NO_DATA_FOUND) {
         if (ConnInd) {
            plm_retcode = SQLGetDiagField(
               plm_handle_type, plm_handle, plm_cRecNmbr,
               SQL_DIAG_ROW_NUMBER, &plm_Rownumber,
               SQL_IS_INTEGER,
               NULL);
            plm_retcode = SQLGetDiagField(
               plm_handle_type, plm_handle, plm_cRecNmbr,
               SQL_DIAG_SS_LINE, &plm_SS_Line,
               SQL_IS_INTEGER,
               NULL);
            plm_retcode = SQLGetDiagField(
               plm_handle_type, plm_handle, plm_cRecNmbr,
               SQL_DIAG_SS_MSGSTATE, &plm_SS_MsgState,
               SQL_IS_INTEGER,
               NULL);
            plm_retcode = SQLGetDiagField(
               plm_handle_type, plm_handle, plm_cRecNmbr,
               SQL_DIAG_SS_SEVERITY, &plm_SS_Severity,
               SQL_IS_INTEGER,
               NULL);
            plm_retcode = SQLGetDiagField(
               plm_handle_type, plm_handle, plm_cRecNmbr,
               SQL_DIAG_SS_PROCNAME, &plm_SS_Procname,
               sizeof(plm_SS_Procname),
               &plm_cbSS_Procname);
            plm_retcode = SQLGetDiagField(
               plm_handle_type, plm_handle, plm_cRecNmbr,
               SQL_DIAG_SS_SRVNAME, &plm_SS_Srvname,
               sizeof(plm_SS_Srvname),
               &plm_cbSS_Srvname);
         }
         printf("szSqlState = %s\n",plm_szSqlState);
         printf("pfNativeError = %d\n",plm_pfNativeError);
         printf("szErrorMsg = %s\n",plm_szErrorMsg);
         printf("pcbErrorMsg = %d\n\n",plm_pcbErrorMsg);
         if (ConnInd) {
            printf("ODBCRowNumber = %d\n", plm_Rownumber);
            printf("SSrvrLine = %d\n", plm_Rownumber);
            printf("SSrvrMsgState = %d\n",plm_SS_MsgState);
            printf("SSrvrSeverity = %d\n",plm_SS_Severity);
            printf("SSrvrProcname = %s\n",plm_SS_Procname);
            printf("SSrvrSrvname = %s\n\n",plm_SS_Srvname);
         }
      }
      plm_cRecNmbr++; //Increment to next diagnostic record.
   } // End while.
}

