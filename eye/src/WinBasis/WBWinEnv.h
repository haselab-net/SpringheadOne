#ifndef WINCMP_CWINENV_H
#define WINCMP_CWINENV_H

#include "../Base/BaseDebug.h"

#if defined _MSC_VER && defined _CPPUNWIND
 #define SUPPORT_EXCEPTION
#endif

// use/don't use MFC
#if (!defined NOMFC) && (!defined USEMFC)
 #if (!defined _MSC_VER) && (!defined _MFC_VER) && (!defined _AFXDLL)
  #define NOMFC
 #else
  #define USEMFC
 #endif
#endif

#endif


