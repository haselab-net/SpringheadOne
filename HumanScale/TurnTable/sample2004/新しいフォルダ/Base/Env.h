#ifndef BASE_ENV_H
#define BASE_ENV_H

/**	@file Env.h
	コンパイラ間の互換性を取るためのマクロ定義.	*/

#ifdef _MSC_VER
 #define for if(0); else for
#pragma warning (disable: 4786)
#endif

#ifdef __BORDANDC__
 #ifndef __MT__
  #error This library can work only in multi thread environment.
 #endif
#endif

//	namespace
#endif
