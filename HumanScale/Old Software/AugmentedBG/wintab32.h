
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

// GetProcAddress (DLL�ǂݏo��) �Q�Ɨp
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
  HWND m_hwnd;   // Window�n���h��
  HCTX m_handle; // Wintab32�擾�n���h��

  TTabletPacket m_packet; // �����p�P�b�g��ێ�
  BOOL m_recognition;  // �F���\��(WT_PROXIMITY�ŕω�)
  int  m_queue;        // ���܂��Ă�L���[
  int  m_queueSize;    // �L���[�T�C�Y

  int m_maxPressure; // �ő刳��
  int m_maxTwist;    // �ő��]
  int m_maxAltitude; // �ő�X��
  int m_maxAzimuth;  // �ő����

  BOOL m_ifTwist;
  BOOL m_ifAltitude;
  BOOL m_ifAzimuth;

  // DLL�ǂݏo���֐��|�C���^
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
  void Open( HWND handle ); // WinTab32��L���ɂ���
  void Close();             // WinTab32�����i�N���X���͔̂j������Ȃ��j

  // �ŐV�p�P�b�g�擾( OnMessage�œn��->msg, wparam, lparam ) �F �ԋp�l�͒l���擾�o�������ǂ����B
  BOOL GetPacket( UINT message, WPARAM wParam, LPARAM lParam );

  double Pressure(); // �M��(0.0�`1.0�ɐ��K��)
  double Twist();    // ��](0.0�`1.0�ɐ��K���c���ݑΉ����Ă���^�u���b�g�͑��݂��Ȃ�)
  double Altitude(); // �X��(0.0�`1.0�ɐ��K��)
  double Azimuth();  // ����(0.0�`1.0�ɐ��K��)

  double X(){ return m_packet.X; } // �w�ʒu
  double Y(){ return m_packet.Y; } // �x�ʒu
  int Queue(){ return m_queue; }
  int QueueSize(){ return m_queueSize; }

  // ���K�����Ă��Ȃ���� ( �����B�{���̒l )
  int iPressure(){ return m_packet.Pressure; }
  int iTwist(){ return m_packet.Twist; }
  int iAltitude(){ return m_packet.Altitude; }
  int iAzimuth(){ return m_packet.Azimuth; }

  // �e�l�̍ő���
  int mPressure(){ return m_maxPressure; }
  int mTwist(){ return m_maxTwist; }
  int mAltitude(){ return m_maxAltitude; }
  int mAzimuth(){ return m_maxAzimuth; }

  // �l���L���ł��邩 (��]�E�X���E������)
  BOOL IfTwist(){ return m_ifTwist; }
  BOOL IfAltitude(){ return m_ifAltitude; }
  BOOL IfAzimuth(){ return m_ifAzimuth; }

  BOOL PenRecognition(){ return m_recognition; } // �y���F���o����͈͂�
  BOOL Enabled(){ return m_enabled; }            // �L���ł��� (�^�u���b�g���g�p�o����)
};

#endif

