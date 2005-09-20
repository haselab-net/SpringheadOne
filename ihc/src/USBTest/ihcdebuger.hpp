// IHCDebuger.hpp: IHCDebuger クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IHCDEBUGER_HPP__8545FAD5_77EB_48BB_A790_89A69B6C2376__INCLUDED_)
#define AFX_IHCDEBUGER_HPP__8545FAD5_77EB_48BB_A790_89A69B6C2376__INCLUDED_

/*
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
*/


class IHCDebuger  
{
public:
	void OpMemSet(const int address,char* data,const int size = 512);
	void OpMemGet(const int address,char* data,const int size = 512);
	IHCDebuger();
	virtual ~IHCDebuger();


};

#endif // !defined(AFX_IHCDEBUGER_HPP__8545FAD5_77EB_48BB_A790_89A69B6C2376__INCLUDED_)
