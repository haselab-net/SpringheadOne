// Tokin.cpp: Tokin クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "OpenGL.h"

#include "Tokin.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Tokin::Tokin()
{

}

Tokin::~Tokin()
{

}

void Tokin::Init_tokin()
{
	cout << "Initializing COM" << endl;

   if ( FAILED( CoInitialize( NULL )))
   {
      cout << "Unable to initialize COM" << endl;
   }

   HRESULT hr;
   // This time use CoCreateInstance
   hr = CoCreateInstance( CLSID_Tk3dInput,
                          NULL,
//                          CLSCTX_LOCAL_SERVER,
						  CLSCTX_INPROC,
                          IID_ITk3dInput,
                          (void**) &pTk3dInput );

   if ( FAILED( hr ))
   {
      cout.setf( ios::hex, ios::basefield );
      cout << "Failed to create server instance. HR = " << hr << endl;
      CoUninitialize();
   }

   // Access the IMath interface
   retinfo = 0xff;
   pTk3dInput->CreateDevice( &retinfo );
   cout << "CreateDevice :retinfo = 0x" << hex << retinfo << endl;

}

void Tokin::Uninit_tokin()
{
	// The completion of driver
	pTk3dInput->CloseDevice( &retinfo );
	printf("CloseDevice :retinfo = 0x%x\n",retinfo);

	// Release all of our interfaces
	pTk3dInput->Release();

	CoUninitialize();

	cout << "Uninitialized COM" << endl;
}

void Tokin::SetStdData()	//	Set the A/D standard data 
{
	TK3DADSTDDATA AdcalcData;
	FILE *fp = NULL;
	DWORD dwRet = TRUE;
    int i;

	if ((fp = fopen("dataset.dat", "r")) != NULL) {

		for (i = 0; i < 3; i++) {
			fscanf(fp, "%lf,%lf,%lf", &AdcalcData.Offset[i], &AdcalcData.Gain[i], &AdcalcData.Non_Move[i]);
		}
		for (i = 3; i < 7; i++) {
			fscanf(fp, "%lf,%lf", &AdcalcData.Offset[i], &AdcalcData.Gain[i]);
		}
		fscanf(fp, "%lf,%lf,%lf", &AdcalcData.Ht_Z, &AdcalcData.Ipsiron1, &AdcalcData.Comp_Lev);
		fclose(fp);

	} else {
		dwRet = FALSE;

	}

	retinfo = 0x11;
	pTk3dInput->SetAdStdData( 0, &AdcalcData, &retinfo );
	cout << "SetAdStdData(0):retinfo = 0x" << hex << retinfo << endl;
}

void Tokin::Poll()
{
	pTk3dInput->Poll( 0, &retinfo );
	cout << "Poll(0):retinfo = 0x" << hex << retinfo << endl;
	Sleep(500);
}

void Tokin::GetDeviceData()
{
	TK3DISTATE Tk3dIState;

	ZeroMemory( &Tk3dIState, sizeof(TK3DISTATE) );
	pTk3dInput->GetDeviceData( 0, &Tk3dIState, &retinfo );
	angle = Tk3dIState.Alfa;	//Ｚ軸まわりの回転角
//	printf("GetDeviceData(0):retonfo = 0x%x\n", retinfo);
/*	if( retinfo == 0x00 )
	{
		printf("        Alfa=%f,Beta=%f,Gamma=%f,Button=0x%x\n",Tk3dIState.Alfa,Tk3dIState.Beta,Tk3dIState.Gamma,Tk3dIState.Button);
		printf("        ubFlag=0x%x,wRoll=0x%x,wPitch=0x%x,wTurn=0x%x,wLeanFB=0x%x,",Tk3dIState.ubFlag,Tk3dIState.wRoll,Tk3dIState.wPitch,Tk3dIState.wTurn,Tk3dIState.wLeanFB);
		printf("wLeanRL=0x%x,wMoveFB=0x%x,wMoveRL=0x%x,uwButton=0x%x,uwLED=0x%x\n",Tk3dIState.wLeanRL,Tk3dIState.wMoveFB,Tk3dIState.wMoveRL,Tk3dIState.uwButton,Tk3dIState.uwLED);
	}
*/	
}