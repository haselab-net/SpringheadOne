#ifndef BASE_ENV_H
#define BASE_ENV_H

/**	@file Env.h
	�R���p�C���Ԃ̌݊�������邽�߂̃}�N����`.	*/

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
