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
	void SendId();
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
	int pidFrom;
	WBSockAddr adrTo;
	char buffer[40*1024];
	char sendBuffer[40*1024];
	int sendLen;
	bool bSameAddress;
public:
	WBNetInterfaces nifs;
	WBSocket sock;
	WBSocket recvSock;
	std::set<Command*, CmdLess> cmdSet;
	CmdSocket();
	void Init(int portFrom, int portTo);
	bool Receive();
	void Register(Command* c);
	void Send(const void* data, size_t len);
	void Flush();
};


#endif
