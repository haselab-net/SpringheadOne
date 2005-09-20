#include <windows.h>
#include <iostream.h>
#include <winsock.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include "strdef.h"

#define NO_FLAGS_SET	0

#define MAXBUFLEN	512

INT main(VOID){
	cout << sizeof(MXTCMD) << "\n";
	WSADATA Data;
	SOCKADDR_IN destSockAddr;
	SOCKET destSocket;
	unsigned long destAddr;
	int status;
	int numsnt;
	int numrcv;
	char sendText[MAXBUFLEN];
	char recvText[MAXBUFLEN];
	char dst_ip_address[MAXBUFLEN];
	unsigned short port;
	char msg[MAXBUFLEN];
	char buf[MAXBUFLEN];
	char type;
	unsigned short IOSendType;
	unsigned short IORecvType;
	unsigned short IOBitTop=0;
	unsigned short IOBitMask=0xffff;
	unsigned short IOBitData=0;

	cout<<"接続先のIPアドレスを入力(192.168.0.1)->";
	cin.getline(dst_ip_address,MAXBUFLEN);
	if(dst_ip_address[0]==0)	strcpy(dst_ip_address,"192.168.138.30");

	cout<<"接続先のポート番号を入力(10000)->";
	cin.getline(msg,MAXBUFLEN);
	if(msg[0]!=0) port=atoi(msg);
	else	port=10000;

	cout<<"入出力信号を使いますか？([Y}/[N])->";
	cin.getline(msg,MAXBUFLEN);
	if(msg[0]!=0 && (msg[0]=='Y' || msg[0]=='y')){
		cout<<"対象は？　入力信号/出力信号([I]nput/[O]utput)->";
		cin.getline(msg, MAXBUFLEN);
		switch(msg[0]){
		case 'O':
		case 'o':
			IOSendType = MXT_IO_OUT;
			IORecvType = MXT_IO_OUT;
			break;
		case 'I':
		case 'i':
		default:
			IOSendType = MXT_IO_NULL;
			IORecvType = MXT_IO_IN;
			break;
		}

		cout << "先頭のビット番号を入力(0～32767) ->";
		cin.getline(msg,MAXBUFLEN);
                if(msg[0]!=0) IOBitTop=atoi(msg);
                else    IOBitTop = 0;

                if(IOSendType==MXT_IO_OUT){
                        cout << "出力時のビットマスクパターンを16進数で入力(0000～FFFF)";
                        cin.getline(msg,MAXBUFLEN);
                        if(msg[0]!=0) sscanf(msg,"%4x",&IOBitMask);
                        else    IOBitMask = 0;
                        cout << "出力時のビットデータを16進数で入力(0000～FFFF)->";
                        cin.getline(msg,MAXBUFLEN);
                        if(msg[0]!=0) sscanf(msg,"%4x",&IOBitData);
                        else    IOBitData = 0;
                }
        }
        cout << "指令するデータ型を入力([P]ose [J]oint [M]otor Pulse [N]ull)->";
        cin.getline(msg,MAXBUFLEN);
        switch (msg[0]){
                case 'J':
                case 'j':
                        type = MXT_TYP_JOINT;
                        break;
                case 'P':
                case 'p':
                        type = MXT_TYP_POSE;
                        break;
                case 'M':
                case 'm':
                        type = MXT_TYP_PULSE;
                        break;
                case 'N':
                case 'n':
                default:
                        type = MXT_TYP_NULL;
                        break;
        }

        sprintf(msg, "IP=%s / PORT=%d / SendType=%d", dst_ip_address, port, type);
        cout << msg << endl;

        status=WSAStartup(MAKEWORD(1,1),&Data);
        if (status != 0 )
        cerr << "ERROR: WSAStartup unsuccessful" << endl;

        destAddr=inet_addr(dst_ip_address);
        memcpy(&destSockAddr.sin_addr, &destAddr, sizeof(destAddr));
        destSockAddr.sin_port=htons(port);
        destSockAddr.sin_family=AF_INET;

        destSocket=socket(AF_INET, SOCK_DGRAM, 0);
        if(destSocket == INVALID_SOCKET){
                cerr << "ERROR: socket unsuccessful" <<endl;
                status=WSACleanup();
                if(status == SOCKET_ERROR)
                        cerr << "ERROR: WSACleanup unsuccessful" << endl;
                return(1);
        }

        MXTCMD MXTsend;
        MXTCMD MXTrecv;
        JOINT  jnt_now;
        POSE   pos_now;
        PULSE  pls_now;

        unsigned long counter = 0;
        int loop = 1;
        int disp = 0;
        int ch;
        float deltaX=(float)0.0;
        float deltaY=(float)0.0;
        float deltaZ=(float)0.0;
        long ratio=1;
        int retry;
        fd_set SockSet;
        timeval sTimeOut;

        memset(&MXTsend, 0, sizeof(MXTsend));
        memset(&jnt_now, 0, sizeof(JOINT));
        memset(&pos_now, 0, sizeof(POSE));
        memset(&pls_now, 0, sizeof(PULSE));

        while(loop){
		Sleep(100);

                memset(&MXTsend, 0, sizeof(MXTsend));
                memset(&MXTrecv, 0, sizeof(MXTrecv));

                if(loop==1){
                        MXTsend.Command = MXT_CMD_NULL;
                        MXTsend.SendType = MXT_TYP_NULL;
                        MXTsend.RecvType = type;
                        MXTsend.SendIOType = MXT_IO_NULL;
                        MXTsend.RecvIOType = IOSendType;
                        MXTsend.CCount = counter = 0;
                        loop = 2;
                }
                else{
                        MXTsend.Command = MXT_CMD_MOVE;
                        MXTsend.SendType = type;
                        MXTsend.RecvType = type;
                        switch(type){
                                case MXT_TYP_JOINT:
                                        memcpy(&MXTsend.dat.jnt, &jnt_now, sizeof(JOINT));
                                        MXTsend.dat.jnt.j1 += (deltaX*ratio*3.141592/180.0);
                                        MXTsend.dat.jnt.j2 += (deltaY*ratio*3.141592/180.0);
                                        MXTsend.dat.jnt.j3 += (deltaZ*ratio*3.141592/180.0);
//                                        MXTsend.dat.jnt.j1 += (float)(delta*ratio*3.141592/180.0);
                                        break;
                                case MXT_TYP_POSE:
                                        memcpy(&MXTsend.dat.pos, &pos_now, sizeof(POSE));
                                        MXTsend.dat.pos.w.x += (deltaX*ratio);
                                        MXTsend.dat.pos.w.y += (deltaY*ratio);
                                        MXTsend.dat.pos.w.z += (deltaZ*ratio);
                                        break;
                                case MXT_TYP_PULSE:
                                        memcpy(&MXTsend.dat.pls, &pls_now, sizeof(PULSE));
//                                        MXTsend.dat.pls.p1 += (long)((delta*ratio)*10);
                                        break;
                                default:
                                        break;
                        }
                        MXTsend.SendIOType = IOSendType;
                        MXTsend.RecvIOType = IORecvType;
                        MXTsend.BitTop = IOBitTop;
                        MXTsend.BitMask = IOBitMask;
                        MXTsend.IoData = IOBitData;
                        MXTsend.CCount = counter;
                }

                while(kbhit()!=0){
                        ch=getch();
                        switch(ch){
                                case 0x0d:
                                        MXTsend.Command = MXT_CMD_END;
                                        loop = 0;
                                        break;
                                case 'x':
                                        deltaX +=(float)0.1;
                                        break;
                                case 'X':
                                        deltaX -=(float)0.1;
                                        break;

                                case 'y':
                                        deltaY +=(float)0.1;
                                        break;
                                case 'Y':
                                        deltaY -=(float)0.1;
                                        break;
                                case 'z':
                                        deltaZ +=(float)0.1;
                                        break;
                                case 'Z':
                                        deltaZ -=(float)0.1;
                                        break;
                                case 'C':
                                case 'c':
                                        deltaX = (float)0.0;
                                        deltaY = (float)0.0;
                                        deltaZ = (float)0.0;
                                        break;
                                case 'D':
                                case 'd':
                                        disp = ~disp;
                                        break;
                        }
                }

                memset(sendText, 0, MAXBUFLEN);
                memcpy(sendText,&MXTsend, sizeof(MXTsend));
                if(disp){
                        sprintf(buf, "Send   (%ld):" ,counter);
                        cout << buf << endl;
                }
                numsnt=sendto(destSocket, sendText, MAXBUFLEN, NO_FLAGS_SET,
                        (LPSOCKADDR) &destSockAddr , sizeof(destSockAddr));
                if(numsnt != MAXBUFLEN){
                        cerr << "ERROR: sendto unsuccessful" << endl;
                        status=closesocket(destSocket);
                        if(status == SOCKET_ERROR)
                                cerr << "ERROR: closesocket unsuccessful" << endl;
                        status=WSACleanup();
                        if(status == SOCKET_ERROR)
                                cerr << "ERROR: WSACleanup unsuccessful" << endl;
                        return(1);
                }

                memset(recvText, 0, MAXBUFLEN);

                retry = 5;
                while(retry){
                        FD_ZERO(&SockSet);
                        FD_SET(destSocket, &SockSet);
                        sTimeOut.tv_sec = 1;
                        sTimeOut.tv_usec = 0;
                        status = select(0, &SockSet, (fd_set *)NULL, (fd_set *)NULL, &sTimeOut);
                        if(status == SOCKET_ERROR){
                                return(1);
                        }
                        if((status > 0)&&(FD_ISSET(destSocket, &SockSet) !=0)){
                                numrcv=recvfrom(destSocket, recvText, MAXBUFLEN, NO_FLAGS_SET, NULL, NULL);
                                if(numrcv == SOCKET_ERROR){
                                        cerr << "ERROR: fecvfrom unsuccessful" << endl;
                                        status=closesocket(destSocket);
                                        if(status == SOCKET_ERROR)
                                                cerr << "ERROR: closesocket unsuccessful" << endl;
                                        status=WSACleanup();
                                        if(status == SOCKET_ERROR)
                                                cerr << "ERROR: WSACleanup unsuccessful" << endl;
                                        return(1);
                                }
                                memcpy(&MXTrecv, recvText, sizeof(MXTrecv));
                                char str[10];
                                if(MXTrecv.SendIOType==MXT_IO_IN)
                                        sprintf(str,"IN%04x",MXTrecv.IoData);
                                else  if(MXTrecv.SendIOType==MXT_IO_OUT)
                                        sprintf(str,"OT%04x",MXTrecv.IoData);
                                else  sprintf(str,"------");
				   
                                switch(MXTrecv.SendType){
                                        case MXT_TYP_JOINT:
						memcpy(&jnt_now, &MXTrecv.dat.jnt, sizeof(JOINT));
                                                if(disp){
                                                        sprintf(buf, "Receive(%ld): TCount=%d Type(JOINT)\n %7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f (%s)"
                                                                ,MXTrecv.CCount,MXTrecv.TCount
                                                                ,jnt_now.j1, jnt_now.j2, jnt_now.j3
                                                                ,jnt_now.j4, jnt_now.j5, jnt_now.j6
                                                                ,jnt_now.j7, jnt_now.j8, str);
                                                        cout << buf << endl;
                                                }
                                                break;
                                        case MXT_TYP_POSE:
                                                memcpy(&pos_now, &MXTrecv.dat.pos, sizeof(POSE));
                                                if(disp){
                                                        sprintf(buf, "Receive(%ld): TCount=%d Type(POSE)\n %7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%04x,%04x (%s)"
                                                                ,MXTrecv.CCount,MXTrecv.TCount
                                                                ,pos_now.w.x, pos_now.w.y, pos_now.w.z
                                                                ,pos_now.w.a, pos_now.w.b, pos_now.w.c
                                                                ,pos_now.sflg1, pos_now.sflg2, str);
                                                        cout << buf << endl;
                                                }
                                                break;
                                        case MXT_TYP_PULSE:
                                                memcpy(&pls_now, &MXTrecv.dat.pls, sizeof(PULSE));
                                                if(disp){
                                                        sprintf(buf, "Receive(%ld): TCount=%d Type(PULSE)\n %ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld (%s)"
                                                                ,MXTrecv.CCount,MXTrecv.TCount
                                                                ,pls_now.p1, pls_now.p2, pls_now.p3
                                                                ,pls_now.p4, pls_now.p5, pls_now.p6
                                                                ,pls_now.p7, pls_now.p8, str);
                                                        cout << buf << endl;
                                                }
                                                break;
                                        case MXT_TYP_NULL:
                                                if(disp){
                                                        sprintf(buf, "Receive(%ld): TCount=%d\n (%s)"
                                                                ,MXTrecv.CCount,MXTrecv.TCount,str);
                                                        cout << buf << endl;
                                                }
                                                break;
                                        default:
                                                break;
                                }
                                counter++;
                                retry=0;
                        }
                        else{
                                cout << "...Receive Timeout! <Push [Enter] to stop the program> "<<endl;
                                retry--;
                                if(retry==0)  loop=0;
                        }
                }
        }

        cout << "///End///";
        sprintf(buf, "counter = %ld", counter);
        cout << buf << endl;

        status=closesocket(destSocket);
        if(status == SOCKET_ERROR)
                cerr << "ERROR: closesocket unsuccessful" << endl;
        if(status == SOCKET_ERROR)
                cerr << "ERROR: WSACleanup unsuccessful" << endl;

        return 0;
}



