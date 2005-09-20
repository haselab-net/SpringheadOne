#ifndef __UDPRECV_H
#define __UDPRECV_H

#include <WinBasis/WBSocket.h>

struct ReceiveInfo{
	int port;
	DWORD sleepTime;
	int bufSize;
	char *buf;
	bool stop;
	int x;
	int y;
	bool captured;
};

extern "C" void ReceiveFunc(ReceiveInfo* info);

class CUdpRecv
{
	ReceiveInfo m_recvinfo;
	char m_buf[1024];
	DWORD m_threadID;
	HANDLE m_threadHandle;
public:
	CUdpRecv(int port,int sleepTime); // �|�[�g�ƃf�[�^����M����Ԋu(ms)���w��
	~CUdpRecv();
	bool Start(); // ��M�J�n
	void Stop(); // ��M��~
	int GetX(); // ��M�����ŐV��X���W�𓾂�
	int GetY(); // ��M�����ŐV��Y���W�𓾂�
	bool IsCaptured(); // ���[�U�[�|�C���^���L���v�`���������ǂ���
	POINT GetPoint(); // ��M�����ŐV�̍��W��POINT�\���̂œ���(�K�v���͕s��)
};

#endif // __UDPRECV_H
