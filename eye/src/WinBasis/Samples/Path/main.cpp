#include <WinBasis/WBPath.h>

using namespace std;
using namespace Spr;

#define CK \
std::cout << "-----------------------------------" << std::endl;\
	std::cout << path.Drive().c_str() << std::endl;\
	std::cout << path.Dir().c_str() << std::endl;\
	std::cout << path.File().c_str() << std::endl;\
	std::cout << path.Main().c_str() << std::endl;\
	std::cout << path.Ext().c_str() << std::endl;\

void main(int argc, char** argv){
	WBPath path;
/*	path.Path("\\\\hasenote2\\hase\\c\\project\\archive.bat");
	CK
	path.Path("C:\\Home\\hase\\c\\project\\archive.bat");
	CK
	path.Path("c\\project\\archive.bat");
	CK
	path.Path("archive.bat");
	CK*/
//	path.Path("tmp\\archive.bat");
 	path.Path("C:\\Home\\hase\\C\\Project\\Springhead\\src\\WinBasis\\Samples\\Test\\Path\\main.cpp");
	std::cout << path.RelPath().c_str() << std::endl;

	path.Path("C:\\Home\\hase\\C\\Project\\Springhead\\src\\WinBasis\\Samplee\\Test\\Path\\main.cpp");
	std::cout << path.RelPath().c_str() << std::endl;
	
}

