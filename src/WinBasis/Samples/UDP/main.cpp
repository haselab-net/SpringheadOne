#include <Base/Affine.h>
#include <WinBasis/WBSocket.h>
using namespace std;
using namespace Spr;

/*
 udp.exe
  Usage: udp.exe [d,a,s]
  �N�����I�v�V�����ŃT�[�o�[(���w��),�_���v,�A�X�L�[,���x�\��

�@WinBasis/WESocket���g�p����UDP�ʐM�̃T���v���B
�@�����[�v�X�V���ꂽAffine���N���C�A���g�ɑ��M����B
�@�T�[�o���N������ƁA�u���[�h�L���X�g�A�h���X��UDP�|�[�g10000�Ɍ����ăp�P�b�g�𓊂��Â���B
�@�N���C�A���g�̓A�h���X�̎w��Ȃ��ŁA�f�[�^����M�ł���B
�@�J�������̋��L�ȂǍ������L�����_�����O�ɂ��g����B
�@���Ȃ݂ɃT�[�o�̕����N���͑z�肳��Ă��܂���B
�@Modified by A.S 2004/07/13
*/

void main(int argc, char** argv){
	if (argc > 1){
		/* �N���I�v�V�����ɉ��������Ǝ�M���B */
		WBSocket sock(AF_INET, SOCK_DGRAM, 0);
		WBSockAddr adr;
		adr.AdrIn().sin_family = AF_INET;
		adr.AdrIn().sin_port = htons(11111);
		adr.AdrIn().sin_addr.s_addr = htonl(INADDR_ANY);		//	�A�h���X
		if(bind(sock, (LPSOCKADDR)&adr, sizeof(adr))==SOCKET_ERROR){
			int error = WSAGetLastError();
			closesocket(sock);
			sock = INVALID_SOCKET;
			return;
		}
		char buf[1024*640];
		int cur=0,ptr=0;
		DWORD lasttime=0;
		while(1){
			DWORD avail=0;
			ioctlsocket(sock, FIONREAD, &avail);
			if (avail){
				WBSockAddr a;
				int rlen = sock.RecvFrom(buf+cur, sizeof(buf)-cur, a);

				/* �N���I�v�V�����ɂ��\��������ύX */
				switch (*argv[1]) {
				case 'd':
					{
						/* �o�C�i�����_���v����ꍇ */
						std::cout << rlen << " Bytes:" << std::endl;
					    long f = std::cout.flags();
					    for(int i=0; i<100 && i<rlen; i++){
							std::cout.width(2);
						    std::cout.fill('0');
						    std::cout.flags(ios::hex | ios::uppercase);
						    std::cout << (int)(unsigned char)buf[i];
							/* ���₷�����s */
						    if (i%16 == 15){
								std::cout << std::endl;
							}else{
								std::cout << " ";
							}
						}
						std::cout << std::endl;
				        std::cout.flags(f);
						break;
					}

				 case 'a': 
					{
						/* �����ŏo�͂���ꍇ */
						ptr=0;
						cur += rlen;
						while (ptr<rlen) {
							switch (buf[ptr++]) {
							 case 'A': 
								 {
									 Affinef af;
									 memcpy(&af, buf+ptr, sizeof(af));
									 ptr+=sizeof(af);
									 cout << af << endl;
									 cout << "ptr" <<ptr<< endl;
									 break;
								 }
							case 'T': 
								{
									DWORD timer;
									memcpy(&timer, buf+ptr, sizeof(timer));
									ptr+=sizeof(timer);
								    cout << "timer=" << timer << endl;
								    cout << "ptr" << ptr << endl;
								    break;
								}
							default:
								{
								    cout << "cmd" <<buf[ptr-1]<< endl;
								}
							}
						}
						cur-=ptr;
						break;
					}
				 case 's': 
					{
						/* ���x����,��ʏo�͂��Ȃ� */
						ptr=0;
						cur += rlen;
						while (ptr<rlen) {
							switch (buf[ptr++]) {
							 case 'A': 
								 {
									 /* ����������M�����͂��� */
									 Affinef af;
									 memcpy(&af, buf+ptr, sizeof(af));
									 ptr+=sizeof(af);
									 break;
								 }
							case 'T': 
								{
									static int count = 0;
									DWORD timer;
									memcpy(&timer, buf+ptr, sizeof(timer));
									ptr+=sizeof(timer);
									count++;
									/* 100���1��A���ς�\�����܂� */
									if (count % 100 == 0) {
										cout << ((float)(timer - lasttime)/100.0f) << "msec." << endl;
										lasttime = timer;
									}
								    break;
								}
							default:
								{;}
							}
						}
						cur-=ptr;
						break;
					}
				default:
					{
						cout << ".";
						break;
					}
				}
			}
		}
	}else{
		/* ���M�� */
		WBSocket sock(AF_INET, SOCK_DGRAM, 0);
		WBNetInterfaces nis;
		nis.Init();
		WBSockAddr cl_sin = nis[0].Broadcast();
		cl_sin.AdrIn().sin_port = htons(10000);	//	�|�[�g�ԍ��w��
		int count=0;
		DWORD last;
		Affinef af;
		while(1){
			last = timeGetTime(); //msec�̃^�C�}�[
			af = af * Affinef::Rot(Rad(30), 'y'); //�e�X�g�pAffine�A�������Ӗ��ɉ�]
			/*���M�o�b�t�@�쐬*/
			char buf[1024];
			int cur=0;
			buf[cur] = 'A'; cur++; //Affinef���n�܂�A�Ƃ����Ǝ��v���g�R��
			memcpy(&buf[cur], &af, sizeof(af));	cur += sizeof(af);
			buf[cur] = 'T'; cur++; //Timer���n�܂�A�Ƃ����Ǝ��v���g�R��
			memcpy(&buf[cur], &last, sizeof(last));	cur += sizeof(last);
			cout << cl_sin;
			int rv = sock.SendTo(buf, cur, cl_sin);
			//Sleep(100);
			Sleep(10); //���̒l�����������Ă�����قǑ����Ȃ�Ȃ����Ƃɒ��ӁB
		}
	}
}

