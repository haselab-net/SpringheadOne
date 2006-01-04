//---------------------------------------------------------------------------

#ifndef SHPathGraphH
#define SHPathGraphH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
//---------------------------------------------------------------------------
#include <FileIO/FITypeDesc.h>
#include <Base/Affine.h>
#include <Physics/PHJoint.h>
#include <vector>


class __declspec(delphiclass) TSHPathGraph;

namespace Spr{;

class PACKAGE PHControlKey{
public:
	PTM::VVector<float> proportional;	//	PDêßå‰ÇÃPÇÃåWêî
	PTM::VVector<float> differential;	//	PDêßå‰ÇÃDÇÃåWêî
	PTM::VVector<float> angle;			//	êßå‰ñ⁄ïWÇÃäpìx
	float time;
	PHControlKey();
	void Resize(int n);
	void Print(std::ostream& os) const;
	void Input(std::istream& is);
};
inline std::ostream& operator << (std::ostream& os, const PHControlKey& key){
	key.Print(os);
}
inline std::istream& operator >> (std::istream& is, PHControlKey& key){
	key.Input(is);
}


class PACKAGE PHControlPath: public UTRefCount{
public:
	typedef std::vector<PHControlKey> Keys;
	typedef std::vector<PTM::VVector<float> > AngleHist;
	typedef std::vector<TSHPathGraph*> Graphs;
public:
	AngleHist angleHist;
    Graphs graphs;
	int curHist;
	float histStep;
	Keys keys;
	int nJoint;
	UTRef<PHJoint> root;
public:
	float totalTime;	//	1é¸ä˙
	float time;         //	é¸ä˙ÇÃíÜÇÃéûçè
	float phaseStart;	//	
	float lastRead;		//	ç≈å„Ç…ì«Ç›èoÇµÇΩéûçè
	int curId;
	PHControlPath(TComponent* owner);
	void Init(PHJoint* r=NULL);
	void CalcTotalTime();
	int AddKey(int pos=-1);
	bool DelKey(int pos);
	int NKey(){ return keys.size(); }
	PHControlKey& Key(int i){ return keys[i]; }
	void WriteAngle(int i, float time);
	void ReadAngle();
	bool Step(float dt);
	void Print(std::ostream& os);
	void Input(std::istream& is);
	void Invalidate();
	void CalcScale();
};

}

class PACKAGE TSHPathGraph : public TCustomControl
{
private:
	TColor rectColor;
	TColor curRectColor;
	TColor barColor;
	float minV;
	float maxV;
	TSize rectSize;
	int curId, hitId, barId;
protected:
	int jointId;
	typedef enum {
		NONE,
		LINEAR
	} InterpolateType;
	InterpolateType interpolate;
	TPopupMenu* rectMenu;
public:
	Spr::UTRef<Spr::PHControlPath> path;
	__fastcall TSHPathGraph(TComponent* Owner);
	virtual void __fastcall Paint();
	void __fastcall CalcScale();
__published:
	__property Canvas;
	__property DoubleBuffered;
	__property Color;
	__property TColor RectColor={read=rectColor, write=rectColor};
	__property TColor CurRectColor={read=curRectColor, write=curRectColor};
	__property TColor BarColor={read=barColor, write=barColor};
	__property int JointId={read=jointId, write=jointId};
protected:
	//	event handler
	DYNAMIC void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall DoExit();
	DYNAMIC void __fastcall KeyDown(Word &Key, Classes::TShiftState Shift);
	//	utility
	void __fastcall Add(System::TObject* Sender);
	void __fastcall Del(System::TObject* Sender);

	void __fastcall CalcRect(int n, float& time, TRect& rc, TRect& bar);
	float __fastcall ToX(float t);
	float __fastcall ToY(float v);
	float __fastcall ToT(float x);
	float __fastcall ToV(float y);
	float __fastcall ToP(float h);
	float __fastcall ToH(float p);
	int __fastcall GraphWidth();
};
//---------------------------------------------------------------------------
#endif
