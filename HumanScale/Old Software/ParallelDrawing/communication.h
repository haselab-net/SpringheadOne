#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <WinBasis/WBSockStr.h>
#include <iostream>
#include <conio.h>
using namespace LVRM;
using namespace std;

class TSocketReader{
protected:
	string buffer;
	string cmd;
public:
	WBSockStream sock;
	int port;
	string server;
	TSocketReader(){ port = 12000; }
	TSocketReader(int p){ port = p; }
	TSocketReader(const char* s, int p){ server = s; port = p; }

	string& Command(){ return cmd; }
	bool Accept();
	bool Connect();
	bool Read();
};

class Communication{
protected:
	void Command(ostream& sock, string& cmd);
public:
	//	TCP/IPí êM
	enum TRunMode{
		SERVER,
			CLIENT
	} mode;
	bool Client(int nServer, char **argv);
	void Server();
	enum { PORTBASE = 12000 };
	TSocketReader *csock;
	TSocketReader ssock;
};
#endif