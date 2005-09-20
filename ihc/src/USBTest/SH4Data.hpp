// SH4Data.h: CSH4Data クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SH4DATA_H__06F7535C_8772_41D6_BF0A_61607EF66B98__INCLUDED_)
#define AFX_SH4DATA_H__06F7535C_8772_41D6_BF0A_61607EF66B98__INCLUDED_

/*
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
*/


#define CSH4DATA_CHAR_SIZE 32

#define CSH4DATA_BUFFER_SIZE 20

#define CSH4DATA_S8_SIZE 3
#define CSH4DATA_F8_SIZE 3
#define CSH4DATA_F4_SIZE 23
// impulse 力積

class CSH4Data
{
public:
	short s8_data[CSH4DATA_S8_SIZE][8][CSH4DATA_BUFFER_SIZE];
	float f8_data[CSH4DATA_F8_SIZE][8][CSH4DATA_BUFFER_SIZE];
	float f4_data[CSH4DATA_F4_SIZE][4][CSH4DATA_BUFFER_SIZE];

public:
	const static char s8_key[CSH4DATA_S8_SIZE][CSH4DATA_CHAR_SIZE];
	const static char f8_key[CSH4DATA_F8_SIZE][CSH4DATA_CHAR_SIZE];
	const static char f4_key[CSH4DATA_F4_SIZE][CSH4DATA_CHAR_SIZE];

public:
#ifdef _DEBUG
	void testVector();
#endif //#ifdef _DEBUG
	CSH4Data();
	virtual ~CSH4Data();

};

#endif // !defined(AFX_SH4DATA_H__06F7535C_8772_41D6_BF0A_61607EF66B98__INCLUDED_)
