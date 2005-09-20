#if !defined(DIREXEC__H__INCLUDED_)
#define DIREXEC_H__INCLUDED_

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdlib.h>
#include <odbcss.h>
#include "joint.h"

#define MAX_DATA 100
#define MYSQLSUCCESS(rc) ((rc==SQL_SUCCESS)||(rc==SQL_SUCCESS_WITH_INFO))

//JEONG 10/23
#define CATCHSUCCESS 1
#define CATCHFAIL 2
#define NOEVENT 0
#define CATCHMISSING 3
/////////////////////////////////
#define FREE_MOVEMENT 1100
#define STANCE_POSE 1101
#define CATCHMISS_LEFT 1103
#define CATCHMISS_RIGHT 1104
///////////////////////////
///status transition/////////
#define READY_PHASE 11
#define CATCH_PHASE 12 
#define THROW_PHASE 13 
#define MISS_PHASE  14
//////////////////////////////
//query time test
static DWORD m_StartTime=0;
static DWORD m_ElapsedTime=0;

struct Key_Param
{
	long key_type;
    long key_frm;
	double pos[3];
};


class Cdirexec
{
   RETCODE rc;        // ODBC return code
   HENV henv;         // Environment   
   HDBC hdbc;         // Connection handle
   HSTMT hstmt;       // Statement handle
   unsigned char szData[MAX_DATA];   // Returned data storage
   SDWORD cbData;     // Output length of data
   unsigned char chr_ds_name[SQL_MAX_DSN_LENGTH];   // Data source name
   unsigned char chr_us_name[8];   // User name
   unsigned char chr_ps_name[8];   // Password name

  public:
   Cdirexec();           // Constructor
   virtual ~Cdirexec();
    //ODBC
 
   int sqlconn();      // Allocate env, stat, and conn
   int sqlexec(char *);   // Execute SQL statement
   int sqlexec(char *, int);   // Execute SQL statementunsigned 
   void sqldisconn();   // Free pointers to env, stat, conn,
                        // and disconnect
   void error_out();    // Displays errors
   char* make_sql(int, int, double b_pos[]);
   char* make_sql(int);
   //MY DB
   CVH_RENDER vh;
   Key_Param param; //key parameter
   //void Alloc_VH(CVH_RENDER ref);
   int select_motion(int init_type, double k_frm, double b_pos[3]);
   int search_key(double radius, int k_frm, double b_pos[3]);
   //int random_throw();
   int load_basic(int throw_type, int catch_type);
   int load_model(int model_type);
    // motion_data per frame and bone
   int load_motion(int idx);

};

#endif

