#include <FileIO/MetaLoader/FIGenParser.h>
#include <FileIO/MetaLoader/FIPNode.h>
#include <FileIO/MetaLoader/FIPContext.h>
#include <iostream>
#include <fstream>
using namespace Spr;
using namespace std;
void main(){
	cout << "First.exe Start" << std::endl;
	FIGenParser loader;
	std::ifstream isTs("grammar.g");
	FIPContext ctx(&isTs, &cout);
	//	ctx.dumpReadNode = true;
	loader.LoadParser(ctx);
	loader.Print(cout);

	std::ifstream is2("grammar.g");
	FIPContext ctx2(&is2, &cout);
	//	ctx2.dumpReadNode = true;
	loader.Load(ctx2);
	cout << "*** CTX2 ***" << endl;
	cout << ctx2.dataStack;
}
