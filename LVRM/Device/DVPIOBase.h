#ifndef VR_DVPIOBASE_H
#define VR_DVPIOBASE_H

#include "DVVirtualDevice.h"

namespace LVRM {;
//----------------------------------------------------------------------------
//	DVPioBase
///	入出力ポートのための定数の定義などだけを行う．
class DVPortEnum{
	enum TLevel {LO, HI};
	enum TDir {IN, OUT};
};
///	入力ポートの1端子をあらわす．
class DVInputPortBase:public DVVirtualDevice, public DVPortEnum{
public:
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	入力ポートの電圧レベル(Hi:1, Lo:0) を返す．
	virtual int Get()=0;

	///	デバイスの種類
	static const char* TypeS() { return "input port"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

///	出力ポートの1端子をあらわす．
class DVOutputPortBase:public DVVirtualDevice, public DVPortEnum{
public:
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	入力ポートの電圧レベル(Hi:1, Lo:0) を返す．
	virtual void Set(int l)=0;

	///	デバイスの種類
	static const char* TypeS() { return "output port"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};


///	PIOの1端子を表す．
class DVPioBase:public DVVirtualDevice, public DVPortEnum{
public:
	enum TLevel {LO, HI};
	///	ポートのロジックレベルの入力。Hi:true Lo:false
	virtual int Get()=0;
	///	ポートのロジックレベルの出力。Hi:true Lo:false
	virtual void Set(int l)=0;
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	デバイスの種類
	static const char* TypeS() { return "PIO"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace LVRM
#endif
