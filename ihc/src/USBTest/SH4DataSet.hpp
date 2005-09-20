// SH4DataSet.h: CSH4DataSet クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SH4DATASET_H__05D75361_76D4_4643_9AC6_CB0026019D1F__INCLUDED_)
#define AFX_SH4DATASET_H__05D75361_76D4_4643_9AC6_CB0026019D1F__INCLUDED_

/*
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
*/


#include "SH4Data.hpp"

class CSH4DataSet : public CSH4Data  
{
protected:
	char s8_sid[CSH4DATA_S8_SIZE];
	char s8_eid[CSH4DATA_S8_SIZE];
	char s8_size[CSH4DATA_S8_SIZE];

	char f8_sid[CSH4DATA_F8_SIZE];
	char f8_eid[CSH4DATA_F8_SIZE];
	char f8_size[CSH4DATA_F8_SIZE];

	char f4_sid[CSH4DATA_F4_SIZE];
	char f4_eid[CSH4DATA_F4_SIZE];
	char f4_size[CSH4DATA_F4_SIZE];

public:
#ifdef _DEBUG
	int CHKTestVector(CSH4DataSet& ds2);
#endif //#ifdef _DEBUG
#ifdef WIN32
	static void bigToLittles(short *val);
	static void bigToLittlef(float* val);
#endif
	void GetData(char *buf);
	void Reset();
	int CheckDataSize(int maxSize = 512);
	void SetData(char* buf);
	void SetAnalysisWords(char* words);
	CSH4DataSet();
	virtual ~CSH4DataSet();

};

#endif // !defined(AFX_SH4DATASET_H__05D75361_76D4_4643_9AC6_CB0026019D1F__INCLUDED_)
