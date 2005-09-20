#ifndef WINCMP_CWINENV_H
#define WINCMP_CWINENV_H

#include "../Base/Debug.h"

#if defined _MSC_VER && defined _CPPUNWIND
 #define SUPPORT_EXCEPTION
#endif

// export all classes
#ifdef EXPORT_DLL
 #ifdef _MSC_VER
  #define __declspec( dllexport )
  #pragma warning (disable: 4275 4251)
 #elif __BORLANDC__ >= 0x0530
  #define PACKAGE
 #else
  #error We don't know any special keyword for this compiler to export symbols for DLL.
 #endif
#else
 #ifdef _MSC_VER
  #ifdef IMPORT_DLL
   #define __declspec( dllimport )
  #else
   #define DLLCLASS
  #endif
 #elif __BORLANDC__ >= 0x0530
  #define PACKAGE
 #else
  #define DLLCLASS
 #endif
#endif

// use/don't use MFC
#if (!defined NOMFC) && (!defined USEMFC)
 #if (!defined _MSC_VER) && (!defined _MFC_VER) && (!defined _AFXDLL)
  #define NOMFC
 #else
  #define USEMFC
 #endif
#endif

//	for Visual C++ 's strange spec of stl.
#ifdef _MSC_VER
#undef min
#undef max
namespace std{
template <class T>
T min(T t1, T t2){
	return t1 < t2 ? t1 : t2; 
}
template <class T>
T max(T t1, T t2){
	return t1 > t2 ? t1 : t2;
}
}
#endif

#endif
