#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <WinBasis/WBSocket.h>
#include <set>
using namespace LVRM;
using namespace std;

class CmdSocket;
class Command{
protected:
	CmdSocket* cmdSocket;
public:
	Command(CmdSocket* c):cmdSocket(c){}
	virtual ~Command(){}
	virtual int Id() const =0;
	virtual int Receive(char* data)=0;
	void SendId(){ int i=Id(); SendData(&i, sizeof(i)); }
	void SendData(const void* data, size_t len);
};

class CmdLess{
public:
	bool operator()(const Command* c1, const Command* c2) const {
		return c1->Id() < c2->Id();
	}
};
class CmdSocket{
	WBSockAddr adrFrom;
	WBSockAddr adrTo;
	char buffer[40*1024];
	char sendBuffer[40*1024];
	int sendLen;
public:
	WBSocket sock;
	WBSocket recvSock;
	std::set<Command*, CmdLess> cmdSet;
	CmdSocket();
	bool Receive();
	void Register(Command* c);
	void Send(const void* data, size_t len);
	void Flush();
};


#endif
