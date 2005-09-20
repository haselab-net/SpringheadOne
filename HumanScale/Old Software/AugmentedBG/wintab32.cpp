
#include "wintab32.h"

char* WinTabDLLName = "wintab32.dll";

CWinTab32::CWinTab32()
{
  // ��Ԃ̏�����
  m_enabled = FALSE;
  m_recognition = FALSE;
  m_handle = 0;

  // DLL �`�F�b�N
  m_lib = LoadLibrary( WinTabDLLName );
  if (m_lib != 0)
  {
    m_enabled = TRUE;

    m_wtClose = (TWTClose)GetProcAddress( (HINSTANCE__*) m_lib, "WTClose" );
    m_wtInfo  = (TWTInfo)GetProcAddress( (HINSTANCE__*) m_lib, "WTInfoA" );
    m_wtOpen  = (TWTOpen)GetProcAddress( (HINSTANCE__*) m_lib, "WTOpenA" );
    m_wtPacket= (TWTPacket)GetProcAddress( (HINSTANCE__*) m_lib, "WTPacket" );
    m_wtPacketsGet = (TWTPacketsGet)GetProcAddress( (HINSTANCE__*) m_lib, "WTPacketsGet" );
    m_wtQueuePacketsEx = (TWTQueuePacketsEx)GetProcAddress( (HINSTANCE__*) m_lib, "WTQueuePacketsEx" );
    m_wtQueueSizeGet   = (TWTQueueSizeGet)GetProcAddress( (HINSTANCE__*) m_lib, "WTQueueSizeGet" );
    m_wtQueueSizeSet   = (TWTQueueSizeSet)GetProcAddress( (HINSTANCE__*) m_lib, "WTQueueSizeSet" );
  }
}

CWinTab32::~CWinTab32()
{
  Close();

  if (m_lib != 0)
  {
    FreeLibrary( (HINSTANCE__*) m_lib );
    m_lib = 0;
  }
}

void CWinTab32::Open( HWND handle )
{
  // �I�[�v������ĂȂ��Ȃ狎��
  if (!Enabled()) return;

  // ���ɊJ����Ă����狎��
  if (m_handle != 0) return;

  tagLOGCONTEXTA logcontext;
  FIX32 maxX, maxY;
  tagAXIS AXIS;
  tagAXIS AXIS3[3];
  int pac, ret, wintabResult;

  // WinTab�擾�\�H
  wintabResult = m_wtInfo( 0,0, NULL );
  if ( wintabResult != 0 )
  {
    // �O�̂���HWND��ۑ�
    m_hwnd = handle;

    // �ő�l���擾���Đݒ�i�X�N���[���������j
    wintabResult = m_wtInfo( WTI_DEVICES, DVC_X, &AXIS );
    maxX = AXIS.axMax;
    wintabResult = m_wtInfo( WTI_DEVICES, DVC_Y, &AXIS );
    maxY = AXIS.axMax;

    // ���M��(INTUOS�Ή�/����)
    ret = m_wtInfo(WTI_DEVICES, DVC_NPRESSURE, &AXIS);
    m_maxPressure = AXIS.axMax;

    // ����]�E�X���E����(INTUOS�Ή�/����)
    ret = m_wtInfo(WTI_DEVICES, DVC_ORIENTATION, &AXIS3);

    // �ȉ���]�E�X���E�����l�̐ݒ�
    m_ifTwist = TRUE;
    m_maxTwist = AXIS3[0].axMax;
    if(m_maxTwist == 0)
    {
      m_maxTwist = 65535;
      m_ifTwist = FALSE;
    }

    m_ifAltitude  = TRUE;
    m_maxAltitude = AXIS3[1].axMax;
    if(m_maxAltitude == 0)
    {
      m_maxAltitude = 65535;
      m_ifAltitude = FALSE;
    }

    m_ifAzimuth = TRUE;
    m_maxAzimuth  = AXIS3[2].axMax;
    if(m_maxAzimuth == 0)
    {
      m_maxAzimuth = 65535;
      m_ifAzimuth  = FALSE;
    }

    // �����̏����擾�E�ǋL����
    m_wtInfo(WTI_DEFSYSCTX, 0, &logcontext);
    logcontext.lcMsgBase  = WT_DEFBASE;
    logcontext.lcPktData  = PACKETDATA;
    logcontext.lcPktMode  = PACKETMODE;
    logcontext.lcMoveMask = PACKETDATA;
    logcontext.lcBtnUpMask= logcontext.lcBtnDnMask;

    logcontext.lcOptions = logcontext.lcOptions | ( CXO_MESSAGES | CXO_SYSTEM | CXO_MARGIN);

    logcontext.lcOutOrgX = 0;
    logcontext.lcOutOrgY = 0;
    logcontext.lcOutExtX =  GetSystemMetrics(SM_CXSCREEN);
    logcontext.lcOutExtY = -GetSystemMetrics(SM_CYSCREEN);
    logcontext.lcSysOrgX = 0;

    logcontext.lcSysOrgY = 0;
    logcontext.lcSysExtX = GetSystemMetrics(SM_CXSCREEN);
    logcontext.lcSysExtY = GetSystemMetrics(SM_CYSCREEN);

    // Wintab32���J���i�V�K�ݒ聪�ŊJ���j
    m_handle = m_wtOpen( handle, &logcontext, TRUE );

    /*
    // �p�P�b�g�T�C�Y�ݒ� (��ԑ傫���ݒ�o����l�𒲂ׂ�)
    pac = 4096;
    ret = m_wtQueueSizeSet( m_handle, pac );
    while (ret==0)
    {
      pac--;
      ret = m_wtQueueSizeSet(m_handle, pac);
    }
    */

    ret = m_wtQueueSizeSet(m_handle, 256);
  }
}

void CWinTab32::Close()
{
  // ���ɊJ���Ă���ꍇ�A����
  if (m_handle != 0)
  {
    m_wtClose( m_handle );
    m_handle = 0;
  }
}

BOOL CWinTab32::GetPacket( UINT message, WPARAM wParam, LPARAM lParam )
{
  TPACKET pkt;
  BOOL ret;

  // �e�탁�b�Z�[�W�ɂ��ꍇ����
  switch ( message )
  {
  case WT_PACKET:
    // �^�u���b�g�̊e����
    ret = m_wtPacket( (HCTX)lParam, (UINT)wParam, &pkt ); // ���n���h��/�V���A���ԍ�/�擾�p�P�b�g
    if (ret)
    {
      m_packet.X = pkt.pkX;
      m_packet.Y = pkt.pkY;
      m_packet.Pressure = pkt.pkNormalPressure;
      m_packet.Twist = pkt.pkOrientation.orTwist;
      m_packet.Altitude = pkt.pkOrientation.orAltitude;
      m_packet.Azimuth  = pkt.pkOrientation.orAzimuth;
    }
    return TRUE;
    break;

  case WT_PROXIMITY:
    // �^�u���b�g���F���͈͓��ɓ��������A�o����
    if ((lParam & 0xFFFF) != 0) m_recognition = TRUE; else m_recognition = FALSE;
    break;
  }

  // �p�P�b�g�l�܂�̃t���b�V���i �� �K�{�I�Ȃ��ƒ���WT_PACKET���͂��Ȃ��Ȃ�܂��j
  m_wtPacketsGet( m_handle, 1, NULL);

 return FALSE;
}

double CWinTab32::Pressure()
{
  return ((double)m_packet.Pressure/m_maxPressure);
}

double CWinTab32::Twist()
{
  return ((double)m_packet.Twist/m_maxTwist);
}

double CWinTab32::Altitude()
{
  return ((double)m_packet.Altitude/m_maxAltitude);
}

double CWinTab32::Azimuth()
{
  return ((double)m_packet.Azimuth/m_maxAzimuth);
}

