#ifndef DRPORTIO_H
#define DRPORTIO_H
#include <base/utilities.h>

namespace LVRM {
class WBLoadWindowsDriver;

/**	WindowsNT,2000 でI/Oポート直接アクセスをするために呼ぶ関数.
	これを呼んでおけば、_inp, _outp を行っても保護違反がおきなくなる。
	giveio.sys を exeと同じフォルダか、Windowsのフォルダか、パスの通った
	フォルダに入れておくことが必要。
*/
void WBGetPortIO();
}	//	namespace LVRM;

#if defined _MSC_VER
 #include <conio.h>
#endif

#if defined __BORLANDC__
#define _inp bcb_inp
#define _outp bcb_outp
 unsigned int bcb_inp(unsigned short adr);
 void bcb_outp(unsigned short adr, unsigned char v);
#endif



#endif
