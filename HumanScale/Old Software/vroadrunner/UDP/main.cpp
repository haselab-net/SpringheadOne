#include <Base/Affine.h>
#include <WinBasis/WBSocket.h>
using namespace std;
using namespace LVRM;

void main(int argc, char** argv){
	if (argc > 1){
		WBSocket sock(AF_INET, SOCK_DGRAM, 0);
		WBSockAddr adr;
		adr.AdrIn().sin_family = AF_INET;
		adr.AdrIn().sin_port = htons(10000);
		adr.AdrIn().sin_addr.s_addr = htonl(INADDR_ANY);		//	アドレス
		if(bind(sock, (LPSOCKADDR)&adr, sizeof(adr))==SOCKET_ERROR){
			closesocket(sock);
			sock = INVALID_SOCKET;
			return;
		}
		char buf[1024*640];
		int cur=0;
		while(1){
			DWORD avail=0;
			ioctlsocket(sock, FIONREAD, &avail);
			if (avail){
				WBSockAddr a;
				int rlen = sock.RecvFrom(buf+cur, sizeof(buf)-cur, a);
				std::cout << rlen << "Bytes:" << std::endl;
				long f = std::cout.flags();
				for(int i=0; i<100 && i<rlen; i++){
					std::cout.width(2);
					std::cout.fill('0');
					std::cout.flags(ios::hex | ios::uppercase);
					std::cout << (int)(unsigned char)buf[i];
					if (i%16 == 15){
						std::cout << std::endl;
					}else{
						std::cout << " ";
					}
				}
				std::cout << std::endl;
				std::cout.flags(f);
/**/
				cur += rlen;
				int headerLen = 1+sizeof(int);
				if (cur >= headerLen){
					char cmd = buf[0];
					int len;
					memcpy(&len, buf+1, sizeof(len));
					if (cur >= headerLen+len){
						Affinef af;
						memcpy(&af, buf+headerLen, sizeof(af));
						memcpy(buf, buf+headerLen+sizeof(af), cur - headerLen+sizeof(af));
						cur = 0;
						cout << af << endl;
					}
				}
			}
		}
	}else{
		WBSocket sock(AF_INET, SOCK_DGRAM, 0);
		WBNetInterfaces nis;
		nis.Init();
		WBSockAddr cl_sin = nis[0].Broadcast();
		cl_sin.AdrIn().sin_port = htons(10000);	//	ポート番号指定
		int count=0;
		Affinef af;
		while(1){
			af = af * Affinef(Rad(30), 'y');
			char buf[1024];
			int cur=0;
			buf[cur] = 'a'; cur++;
			int len = sizeof(af);
			memcpy(&buf[cur], &len, sizeof(len));	cur += sizeof(len);
			memcpy(&buf[cur], &af, sizeof(af));	cur += sizeof(af);
			cout << cl_sin;
			int rv = sock.SendTo(buf, cur, cl_sin);
			Sleep(100);
		}
	}
}

