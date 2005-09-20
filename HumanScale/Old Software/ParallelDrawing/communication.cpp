#include <WinBasis/WBSockStr.h>
#include <iostream>
#include <conio.h>
#include <strstream>
#include "communication.h"
#include "Base/Affine.h"
using namespace LVRM;
using namespace std;

static bool first = true;

bool Communication::Client(int nServer, char** argv){
	if (mode != CLIENT) return false;
	cout << "C";
	if (first){
		csock = new TSocketReader[nServer];
		for(int i = 0; i < nServer; i++)
			csock[i].server = argv[i+1];
		first = false;
	}
	for(int i=0; i < nServer; i++){
		csock[i].Connect();
		if (csock[i].sock.IsConnected()){
			if (csock[i].Read()){
				Command(csock[i].sock, csock[i].Command());
				csock[i].sock.flush();
			}
		}
	}
	return true;
}

void Communication::Server(){
	if (mode != SERVER) return;
	cout << "S";
	if (ssock.Accept()){
		static bFirst = true;
		if (bFirst){
			ssock.sock << 'S' << '\0';
			ssock.sock << 'S' << '\0';
			ssock.sock << 'S' << '\0';
			ssock.sock << 'S' << '\0';
			ssock.sock.flush();
			bFirst = false;
		}
		if (ssock.Read()) Command(ssock.sock, ssock.Command());
	}
	return;
}

void Communication::Command(ostream& sock, string& cmd){
	Affinef af;
	if (cmd[0] == 'S'){
		af.Pos() = Vec3f(1,1,1);
		sock << 'R';
		sock << af;
		sock << '\0';
		sock.flush();
	}
	if (cmd[0] == 'R'){
		istrstream is(cmd.c_str(), cmd.length());
		is.get();
		is >> af;
		cout << af;
		//view->Invalidate();
		sock << "S" << '\0';
		sock.flush();
	}
}

bool TSocketReader::Accept(){
	if (!sock.IsConnected()){
		if (!sock.IsListening()) sock.Listen(port);
		sock.Accept();
		return false;
	}
	return true;
}

bool TSocketReader::Connect(){
	if (!sock.IsConnected()){
		sock.Connect(server.c_str(), port);
		return false;
	}
	return true;
}

bool TSocketReader::Read(){
	int found = buffer.find('\0');
	if (found != buffer.npos){
		cmd = buffer.substr(0, found);
		buffer = buffer.substr(found+1);
		return true;
	}
	if (sock.Avail() > 0){
		char buf[1025];
		int len = min((int)sock.Avail(), (int)sizeof(buf)-1);
		sock.read(buf, len);
		buffer.append(buf, len);
	}
	return false;
}