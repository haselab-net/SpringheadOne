#include "communication.h"
#include <process.h>

using namespace LVRM;
using namespace std;

class CmdKey:public Command{
public:
	CmdKey(CmdSocket* c):Command(c){}
	int id;
	int Receive(char*){return 0;}
	int Id() const {return id;}
};
CmdSocket::CmdSocket():sendLen(0), pidFrom(0), bSameAddress(false){
}
void CmdSocket::Init(int portFrom, int portTo){
	adrFrom.AdrIn().sin_family = AF_INET;
	adrFrom.AdrIn().sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	adrFrom.AdrIn().sin_port = htons(portFrom);
	recvSock.Bind(adrFrom);

	nifs.Init();
	int i;
	for(i=0; i<nifs.size(); ++i){
		if (nifs[i].Addr().AdrIn().sin_addr.S_un.S_un_b.s_b1 != 127){
			break;
		}
	}
	adrTo = nifs[i].Broadcast();
	//adrTo = nifs[0].Broadcast();
	adrTo.AdrIn().sin_port = htons(portTo);
//	cout << adrTo << endl;
}
bool CmdSocket::Receive(){
	if (!recvSock.CanRecv()) return false;
	int rlen = recvSock.RecvFrom(buffer, sizeof(buffer), adrFrom);
	bSameAddress = adrFrom.AdrIn().sin_addr.S_un.S_addr == nifs[0].Addr().AdrIn().sin_addr.S_un.S_addr;
	char* ptr = buffer;
	char* end = buffer + rlen;
	while(ptr < end){
		CmdKey key(this);
		key.id = *(int*)ptr;
		ptr += sizeof(int);
		std::set<Command*, CmdLess>::const_iterator it = cmdSet.find(&key);
		assert(it != cmdSet.end());
		int clen = (*it)->Receive(ptr);
		ptr += clen;
		assert(ptr <= end);
	}
	return true;
}
void CmdSocket::Register(Command* c){
	cmdSet.insert(c);
}
void CmdSocket::Send(const void* data, size_t len){
	if (sendLen+len > sizeof(sendBuffer)) Flush();
	memcpy(sendBuffer+sendLen, data, len);
	sendLen += len;
}
void CmdSocket::Flush(){
	sock.SendTo(sendBuffer, sendLen, adrTo);
	sendLen = 0;
}

void Command::SendId(){
	int i=Id();
	SendData(&i, sizeof(i));
}
void Command::SendData(const void* data, size_t len){
	cmdSocket->Send(data, len);
}
