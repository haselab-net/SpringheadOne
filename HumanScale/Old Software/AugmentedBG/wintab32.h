
#ifndef __WINTAB32_H_
#define __WINTAB32_H_

#include <windows.h>
#include <iostream>
#include "WINTAB.H"

const PACKETDATA = PK_CONTEXT | PK_STATUS | PK_TIME | PK_CHANGED | PK_SERIAL_NUMBER | PK_CURSOR | PK_BUTTONS | PK_X | PK_Y | PK_Z | PK_NORMAL_PRESSURE | PK_TANGENT_PRESSURE | PK_ORIENTATION;
const PACKETMODE = PK_BUTTONS;

#pragma pack (1)
struct TPACKET
{
  HCTX pkContext;
  UINT pkStatus;
  LONG pkTime;
  WTPKT pkChanged;
  UINT pkSerialNumber;
  UINT pkCursor;
  DWORD pkButtons;
  DWORD pkX;
  DWORD pkY;
  DWORD pkZ;
  UINT  pkNormalPressure;
  UINT  pkTangentPressure;
  tagORIENTATION pkOrientation;
};
#pragma pack ()

// GetProcAddress (DLL読み出し) 参照用
typedef BOOL (API *TWTClose)( HCTX Ctx );
typedef UINT (API *TWTInfo)( UINT wCategory, UINT nIndex, LPVOID lpOuput );
typedef HCTX (API *TWTOpen)( HWND Wnd, tagLOGCONTEXTA* lpLogCtx, BOOL fEnable );
typedef BOOL (API *TWTPacket)( HCTX Ctx, UINT wSerial, LPVOID lpPkt );
typedef int  (API *TWTPacketsGet)( HCTX Ctx, int cMaxPkts, LPVOID lpPkts );
typedef int  (API *TWTQueuePacketsEx)( HCTX Ctx, UINT* lpOld, UINT* olNew );
typedef HCTX (API *TWTQueueSizeGet)( HCTX Ctx );
typedef int  (API *TWTQueueSizeSet)( HCTX Ctx, int nPkts );

struct TTabletPacket
{
  double X,Y,Z;
  int Pressure;
  int Twist;
  int Altitude;
  int Azimuth;
};

class CWinTab32
{
private:
  BOOL m_enabled;
  HANDLE m_lib;
  HWND m_hwnd;   // Windowハンドル
  HCTX m_handle; // Wintab32取得ハンドル

  TTabletPacket m_packet; // 得たパケットを保持
  BOOL m_recognition;  // 認識可能か(WT_PROXIMITYで変化)
  int  m_queue;        // 貯まってるキュー
  int  m_queueSize;    // キューサイズ

  int m_maxPressure; // 最大圧力
  int m_maxTwist;    // 最大回転
  int m_maxAltitude; // 最大傾き
  int m_maxAzimuth;  // 最大方向

  BOOL m_ifTwist;
  BOOL m_ifAltitude;
  BOOL m_ifAzimuth;

  // DLL読み出し関数ポインタ
  TWTClose m_wtClose; 
  TWTInfo  m_wtInfo;
  TWTOpen  m_wtOpen;
  TWTPacket m_wtPacket;
  TWTPacketsGet m_wtPacketsGet;
  TWTQueuePacketsEx m_wtQueuePacketsEx;
  TWTQueueSizeGet m_wtQueueSizeGet;
  TWTQueueSizeSet m_wtQueueSizeSet;

public:
  CWinTab32();
  ~CWinTab32();
  void Open( HWND handle ); // WinTab32を有効にする
  void Close();             // WinTab32を閉じる（クラス事体は破棄されない）

  // 最新パケット取得( OnMessageで渡す->msg, wparam, lparam ) ： 返却値は値を取得出来たかどうか。
  BOOL GetPacket( UINT message, WPARAM wParam, LPARAM lParam );

  double Pressure(); // 筆圧(0.0～1.0に正規化)
  double Twist();    // 回転(0.0～1.0に正規化…現在対応しているタブレットは存在しない)
  double Altitude(); // 傾き(0.0～1.0に正規化)
  double Azimuth();  // 方向(0.0～1.0に正規化)

  double X(){ return m_packet.X; } // Ｘ位置
  double Y(){ return m_packet.Y; } // Ｙ位置
  int Queue(){ return m_queue; }
  int QueueSize(){ return m_queueSize; }

  // 正規化していない情報 ( 整数。本来の値 )
  int iPressure(){ return m_packet.Pressure; }
  int iTwist(){ return m_packet.Twist; }
  int iAltitude(){ return m_packet.Altitude; }
  int iAzimuth(){ return m_packet.Azimuth; }

  // 各値の最大情報
  int mPressure(){ return m_maxPressure; }
  int mTwist(){ return m_maxTwist; }
  int mAltitude(){ return m_maxAltitude; }
  int mAzimuth(){ return m_maxAzimuth; }

  // 値が有効であるか (回転・傾き・方向が)
  BOOL IfTwist(){ return m_ifTwist; }
  BOOL IfAltitude(){ return m_ifAltitude; }
  BOOL IfAzimuth(){ return m_ifAzimuth; }

  BOOL PenRecognition(){ return m_recognition; } // ペン認識出来る範囲か
  BOOL Enabled(){ return m_enabled; }            // 有効である (タブレットを使用出来る)
};

#endif

