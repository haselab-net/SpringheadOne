#ifndef SPR_BASE_ENV_H
#define SPR_BASE_ENV_H

/**	@file Env.h
	�R���p�C���Ԃ̌݊�������邽�߂̃}�N����`.	*/

#if defined _MSC_VER && _MSC_VER <= 1300
 #define for if(0); else for
 #pragma warning (disable: 4786)
 #define TYPENAME
#else
 #define TYPENAME typename
#endif

#if defined __BORLANDC__
 #pragma warn -8026
 #pragma warn -8027
#endif


/**	DLL�̃G�N�X�|�[�g�̎w��
	DLL�ɃG�N�X�|�[�g���邽�߂ɂ́C
	class SPR_DLL DLLCLASS C{}; �̂悤�ɁC�N���X�錾�̑O�� DLLCLASS ������D
	DLL�����Ƃ��́CEXPORT_DLL ���}�N����`���Ă����D*/

#ifdef _MSC_VER
 #ifdef EXPORT_DLL
  #define SPR_DLL __declspec( dllexport )
  #pragma warning (disable: 4275 4251)
 #elif defined IMPORT_DLL
  #define SPR_DLL __declspec( dllimport )
 #else
  #define SPR_DLL
 #endif
#else
 #define SPR_DLL
#endif

//	__cdecl
#if defined _MSC_VER || defined __BORLANDC__
 #define SPR_CDECL	__cdecl
#else
 #define SPR_CDECL
#endif

//	for VC6 class view
#define DOUBLECOLON ::

//	for Visual C++ 's strange spec of stl.
#if defined _MSC_VER
 #if _MSC_VER <= 1300
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
 #else
  #undef min
  #undef max
  #ifndef _MAX
   #define _MIN min
   #define _MAX max
  #endif
 #endif
#endif

#endif
