#include <FileIO/FISpirit.h>
#include <map>
#include <list>
#include <stack>
#include <sstream>
#include <fstream>
#pragma hdrstop

#include "FIVrmlParser.h"
using namespace std;
using namespace boost::spirit;
//---------------------------------------------------------------------------

int __cdecl main(int argc, char* argv[])
{
	using namespace Spr;
	string protos, target, text;
	getline(ifstream("protos.wrl"), protos, '\0');
	getline(ifstream("example3.wrl"), target, '\0');
	text = protos;
	text.append(target);

	FIPTInfo info= FIVrmlParser::Parse(text);
	if (!info.full){
		cout << "Parse error." << endl;
		FIString stop(info.stop, 40);
		cout << stop << endl;
		return 1;
	}
	FIVrmlParser::PrintXml(info, text.c_str());
	return 0;
}
