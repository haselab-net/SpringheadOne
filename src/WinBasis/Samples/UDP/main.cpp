#include <Base/Affine.h>
#include <WinBasis/WBSocket.h>
using namespace std;
using namespace Spr;

/*
 udp.exe
  Usage: udp.exe [d,a,s]
  起動時オプションでサーバー(無指定),ダンプ,アスキー,速度表示

　WinBasis/WESocketを使用したUDP通信のサンプル。
　毎ループ更新されたAffineをクライアントに送信する。
　サーバを起動すると、ブロードキャストアドレスのUDPポート10000に向けてパケットを投げつづける。
　クライアントはアドレスの指定なしで、データを受信できる。
　カメラ情報の共有など高速共有レンダリングにも使える。
　ちなみにサーバの複数起動は想定されていません。
　Modified by A.S 2004/07/13
*/

void main(int argc, char** argv){
	if (argc > 1){
		/* 起動オプションに何か書くと受信側。 */
		WBSocket sock(AF_INET, SOCK_DGRAM, 0);
		WBSockAddr adr;
		adr.AdrIn().sin_family = AF_INET;
		adr.AdrIn().sin_port = htons(11111);
		adr.AdrIn().sin_addr.s_addr = htonl(INADDR_ANY);		//	アドレス
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

				/* 起動オプションにより表示方式を変更 */
				switch (*argv[1]) {
				case 'd':
					{
						/* バイナリをダンプする場合 */
						std::cout << rlen << " Bytes:" << std::endl;
					    long f = std::cout.flags();
					    for(int i=0; i<100 && i<rlen; i++){
							std::cout.width(2);
						    std::cout.fill('0');
						    std::cout.flags(ios::hex | ios::uppercase);
						    std::cout << (int)(unsigned char)buf[i];
							/* 見やすく改行 */
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
						/* 文字で出力する場合 */
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
						/* 速度試験,画面出力しない */
						ptr=0;
						cur += rlen;
						while (ptr<rlen) {
							switch (buf[ptr++]) {
							 case 'A': 
								 {
									 /* いちおう受信処理はする */
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
									/* 100回に1回、平均を表示します */
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
		/* 送信側 */
		WBSocket sock(AF_INET, SOCK_DGRAM, 0);
		WBNetInterfaces nis;
		nis.Init();
		WBSockAddr cl_sin = nis[0].Broadcast();
		cl_sin.AdrIn().sin_port = htons(10000);	//	ポート番号指定
		int count=0;
		DWORD last;
		Affinef af;
		while(1){
			last = timeGetTime(); //msecのタイマー
			af = af * Affinef::Rot(Rad(30), 'y'); //テスト用Affine、ただ無意味に回転
			/*送信バッファ作成*/
			char buf[1024];
			int cur=0;
			buf[cur] = 'A'; cur++; //Affinefが始まる、という独自プロトコル
			memcpy(&buf[cur], &af, sizeof(af));	cur += sizeof(af);
			buf[cur] = 'T'; cur++; //Timerが始まる、という独自プロトコル
			memcpy(&buf[cur], &last, sizeof(last));	cur += sizeof(last);
			cout << cl_sin;
			int rv = sock.SendTo(buf, cur, cl_sin);
			//Sleep(100);
			Sleep(10); //この値を小さくしてもそれほど速くならないことに注意。
		}
	}
}

