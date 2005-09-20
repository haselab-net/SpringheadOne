//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "SHPathGraph.h"
#pragma package(smart_init)

namespace Spr{;

PHControlKey::PHControlKey(){
	time=0.2f;
}
void PHControlKey::Resize(int n){
	angle.resize(n);
	proportional.resize(n);
	differential.resize(n);
	for(int i=0; i<n; ++i){
        angle[i] = 0;
		proportional[i] = 100.0f;
		differential[i] = 1.0f;
	}
}
void PHControlKey::Print(std::ostream& os) const{
    os << time << " ";
	os << angle << " ";
	os << proportional << " ";
	os << differential;
	os << std::endl;
}
void PHControlKey::Input(std::istream& is){
    is >> time;
	is >> angle;
	is >> proportional;
	is >> differential;
}


PHControlPath::PHControlPath(TComponent* owner){
	curHist = 0;
	histStep = 0.1f;
	angleHist.resize(30);
	PHJointInfo info;
	root = new PHJoint(new PHSolid, info);
  	root->AddChildObject(new PHJoint(new PHSolid, info), NULL);
	Init();

	for(int i=0; i<owner->ComponentCount; ++i){
		if (owner->Components[i]->InheritsFrom(__classid(TSHPathGraph))){
			graphs.push_back((TSHPathGraph*)owner->Components[i]);
			graphs.back()->path = this;
		}
	}
}
void PHControlPath::Invalidate(){
	for(unsigned i=0; i<graphs.size(); ++i){
		graphs[i]->Invalidate();
	}
}
void PHControlPath::CalcScale(){
	for(unsigned i=0; i<graphs.size(); ++i){
		graphs[i]->CalcScale();
	}
}
void PHControlPath::CalcTotalTime(){
	totalTime = 0;
	for(Keys::iterator it = keys.begin(); it!= keys.end(); ++it){
		totalTime += it->time;
	}
}
void PHControlPath::Init(PHJoint* r){
	curId = 0;
	time = 0;
	totalTime = 0;
	phaseStart = 0;
	lastRead = 0;
	if (r) root = r;
	nJoint = root->NumNodes();
	for(Keys::iterator it = keys.begin(); it!=keys.end(); ++it){
		it->Resize(nJoint);
	}
	for(AngleHist::iterator it = angleHist.begin(); it!=angleHist.end(); ++it){
		it->resize(nJoint);
	}
}
int PHControlPath::AddKey(int pos){
	if (pos < 0 || pos >= (int)keys.size()){
		keys.push_back(PHControlKey());
		pos = keys.size()-1;
		if (pos > 0){
			keys[pos].time = keys[pos-1].time;
		}
	}else{
		keys.insert(keys.begin()+pos, PHControlKey());
		keys[pos].time = keys[pos+1].time / 2;
		keys[pos+1].time /= 2;
	}
	keys[pos].Resize(nJoint);
	if (keys.size()<3){
		ReadAngle();
		keys[pos].angle = angleHist[curHist];
	}else{
		int prev = pos-1;
		int next = pos+1;
		if (prev < 0) prev += keys.size();
		if (next >= (int)keys.size()) next -= keys.size();
		keys[pos].angle = (keys[prev].angle + keys[next].angle)/2;
		keys[pos].proportional = (keys[prev].proportional + keys[next].proportional)/2;
		keys[pos].differential = (keys[prev].differential + keys[next].differential)/2;
	}
	return pos;
}
bool PHControlPath::DelKey(int pos){
	if (pos < 0 || pos >= (int)keys.size()) return false;
	keys.erase(keys.begin()+pos);
	return true;
}
struct SetJoint{
	int count;
	PTM::VVector<float> ang, prop, diff;
	SetJoint(int nJoint){
		count = 0;
		ang.resize(nJoint);
		prop.resize(nJoint);
		diff.resize(nJoint);
	}
	void Set(PHJoint* j){
		j->SetPosition(ang[count]);
		j->GetInfo().fPValue = prop[count];
		j->GetInfo().fDValue = diff[count];
		count ++;
		for(int i=0; i<(int)j->Children().size(); ++i){
			Set(j->Children()[i]);
		}
	}
};
void PHControlPath::WriteAngle(int i, float time){
	float t = time / keys[i].time;
	int prev = i-1;
	if (prev < 0) prev += keys.size();
	SetJoint sj(nJoint);
	sj.ang = keys[prev].angle*(1-t) + keys[i].angle*t;
	sj.prop = keys[prev].proportional*(1-t) + keys[i].proportional*t;
	sj.diff = keys[prev].differential*(1-t) + keys[i].differential*t;
	for(int i=0; i<(int)root->Children().size(); ++i){
		sj.Set(root->Children()[i]);
	}
}
struct GetJoint{
	int count;
	PTM::VVector<float>* realAngle;
	PTM::VVector<float>* prevAngle;
	GetJoint(PTM::VVector<float>* r, PTM::VVector<float>* p){
		count = 0;
		realAngle = r;
		prevAngle = p;
	}
	void Get(PHJoint* j){
		float a = j->GetInfo().fPosition;
		float diff = a - (*prevAngle)[count];
		if (diff > M_PI) diff = 2*M_PI;
		else if (diff < -M_PI) diff = -2*M_PI;
		else diff = 0;
		(*realAngle)[count] = a - diff;
		count ++;
		for(int i=0; i<(int)j->Children().size(); ++i){
			Get(j->Children()[i]);
		}
	}
};
void PHControlPath::ReadAngle(){
	curHist++;
	if (curHist>=(int)angleHist.size()) curHist = 0;
	GetJoint gj(&angleHist[curHist], &keys[curId].angle);
	for(int i=0; i<(int)root->Children().size(); ++i){
		gj.Get(root->Children()[i]);
	}
}
bool PHControlPath::Step(float dt){
	bool rv = false;
	if (keys.size()==0) return rv;
	WriteAngle(curId, time - phaseStart);
	time += dt;
	if (time > phaseStart + keys[curId].time){
		phaseStart += keys[curId].time;
		curId ++;
		if(curId >= (int)keys.size()){
			curId = 0;
			time -= totalTime;
			lastRead -= totalTime;
			phaseStart = 0;
		}
	}
	if (time > lastRead+histStep){
		ReadAngle();
		lastRead += histStep;
		rv = true;
		Invalidate();
	}
	return rv;
}

void PHControlPath::Print(std::ostream& os){
	os << "{" << std::endl;
	for(int i=0; i<NKey(); ++i){
		os << keys[i];
	}
	os << "}" << std::endl;
}
void PHControlPath::Input(std::istream& is){
	char ch;
	is >> ch;
	if (ch != '{') return;
	while(is.good()){
		PHControlKey key;
		is >> key;
		if (!is.good()){
			is.clear();
			is.get();
			break;
		}
		keys.push_back(key);
	}
	CalcScale();
	Invalidate();
}

}


//---------------------------------------------------------------------------
// ValidCtrCheck は、定義されたコンポーネントが純粋仮想関数を含む
// 抽象クラスではないことを確認するために定義されています。
//

static inline void ValidCtrCheck(TSHPathGraph *)
{
    new TSHPathGraph(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSHPathGraph::TSHPathGraph(TComponent* Owner)
    : TCustomControl(Owner)
{
	curId = 0;
	hitId = barId = -1;
	rectSize.cx = rectSize.cy = 15;
	jointId = 0;
	interpolate = LINEAR;
	DoubleBuffered = true;
	path=NULL;

	rectMenu = new TPopupMenu(this);
	rectMenu->Items->Add(new TMenuItem(rectMenu->Items));
	rectMenu->Items->Items[0]->Caption = "Add control point";
	rectMenu->Items->Items[0]->OnClick = Add;
	rectMenu->Items->Add(new TMenuItem(rectMenu->Items));
	rectMenu->Items->Items[1]->Caption = "Delete control point";
	rectMenu->Items->Items[1]->OnClick = Del;
}
//---------------------------------------------------------------------------
const float GRAPHSCALE = 2.4;
const float PSCALE = 10000.0f;
void __fastcall TSHPathGraph::CalcScale(){
	if (!path) return;
	path->CalcTotalTime();
	minV = 1000000;
	maxV = -1000000;
	int nKey = path->NKey();
	for(int i=0; i<nKey; ++i){
		float v = path->Key(i).angle[jointId];
		if (v<minV) minV = v;
		if (v>maxV) maxV = v;
	}
	if (minV > maxV - 0.1){
		float mid = (minV+maxV)/2;
		minV = mid - 0.05f;
		maxV = mid + 0.05f;
	}
}
int __fastcall TSHPathGraph::GraphWidth(){
	return Width-rectSize.cx;
}
float __fastcall TSHPathGraph::ToY(float v){
	float r = (v-minV)/(maxV-minV);
	float y = Height * ((0.5f-r)/GRAPHSCALE) + Height/2;
	return y;
}
float __fastcall TSHPathGraph::ToV(float y){
	float r = (Height-y)/(float)Height - 0.5f;
	r *= GRAPHSCALE;
	r += 0.5f;
	float v = r*(maxV-minV) + minV;
	return v;
}
float __fastcall TSHPathGraph::ToX(float t){
	if (!path) return 1.0f;
	return (t/path->totalTime)*GraphWidth();
}
float __fastcall TSHPathGraph::ToT(float x){
	if (!path) return 1.0f;
	return (x/GraphWidth())*path->totalTime;
}
float __fastcall TSHPathGraph::ToH(float p){
	if (p<0.001) p = 0.001;
	return PSCALE/p + rectSize.cx/4;
}
float __fastcall TSHPathGraph::ToP(float h){
	if (h - rectSize.cx<0.001f) h = rectSize.cx + 0.001f;
	return PSCALE/(h - rectSize.cx/4);
}
void __fastcall TSHPathGraph::CalcRect(int n, float& time, TRect& rc, TRect& bar){
	if (!path) return;
	if (n<0) n=0;
	int nKey = path->NKey();
	if (n > nKey-1) n=nKey-1;
	time += path->Key(n).time;
	float val = path->Key(n).angle[jointId];
	rc.Left = ToX(time) - rectSize.cx/2;
	rc.Top = ToY(val) - rectSize.cy/2;
	rc.Right = rc.Left + rectSize.cx;
	rc.Bottom = rc.Top + rectSize.cy;
	rc;
	bar.Left = ToX(time) - rectSize.cx/4;
	bar.Right = bar.Left + rectSize.cx/2;
	float p = path->Key(n).proportional[jointId];
	bar.Top = ToY(val) - rectSize.cy/2;
	bar.Bottom = bar.Top + rectSize.cy;
	bar.Top -= ToH(p);
	bar.Bottom += ToH(p);
}

//---------------------------------------------------------------------------
void __fastcall TSHPathGraph::Paint(){
	if (!path) return;
	//	数値の表示
	Canvas->Brush->Color=Color;
	AnsiString str = (int)Spr::Deg(minV);
	str += "deg";
	TSize sz = Canvas->TextExtent(str);
	Canvas->TextOutA(0, Height-sz.cy, str);
	str = (int)Spr::Deg(maxV);
	str += "deg";
	sz = Canvas->TextExtent(str);
	Canvas->TextOutA(0, 0, str);
	str = path->totalTime;
	str += "sec";
	sz = Canvas->TextExtent(str);
	Canvas->TextOutA(GraphWidth()-sz.cx, Height-sz.cy, str);
	//	四角と棒の表示
	float time=0;
	TRect rc, bar;
	int nKey = path->NKey();
	for(int i=0; i< nKey; ++i){
		CalcRect(i, time ,rc, bar);
        Canvas->Brush->Color = BarColor;
		Canvas->FillRect(bar);
		if (i==curId && Focused()){
			Canvas->Brush->Color = CurRectColor;
		}else{
			Canvas->Brush->Color = RectColor;
		}
		Canvas->FillRect(rc);
	}

    //	折れ線の表示
	if (nKey>0){
		time=0;
		float val = path->Key(nKey-1).angle[jointId];
		for(int i=0; i<nKey; ++i){
			Canvas->MoveTo(ToX(time), ToY(val));
			time += path->Key(i).time;
			if (interpolate == NONE){
				Canvas->LineTo(ToX(time), ToY(val));
				val = path->Key(i).angle[jointId];
			}else if (interpolate == LINEAR){
				val = path->Key(i).angle[jointId];
				Canvas->LineTo(ToX(time), ToY(val));
			}
		}
	}
	if (path->totalTime > 0.001f){
		time = path->lastRead - path->histStep * (path->angleHist.size()-1);
		int start = path->curHist+1;
		for(unsigned i=0; i<path->angleHist.size()-1; ++i){
			unsigned cur = start+i;
			if (cur >= path->angleHist.size()) cur -= path->angleHist.size();
			float t = time;
			while (t < 0) t += path->totalTime;
			Canvas->MoveTo(ToX(t), ToY(path->angleHist[cur][jointId]));
			unsigned next = cur+1; if (next >path->angleHist.size()-1) next=0;
			Canvas->LineTo(ToX(t+path->histStep), ToY(path->angleHist[next][jointId]));
			time += path->histStep;
		}
	}
}
void __fastcall TSHPathGraph::MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y){
	if (!path) return;
	int nKey = path->NKey();
	hitId = -1;
	barId = -1;
	float time=0;
	for(int i=0; i<nKey; ++i){
		TRect rc,bar;
		CalcRect(i, time, rc, bar);
		if (rc.Left <= X && X <rc.Right
			&& rc.Top <= Y && Y <rc.Bottom){
			hitId = i;
			break;
		}
		if (bar.Left <= X && X <bar.Right
			&& bar.Top <= Y && Y <bar.Bottom){
			barId = i;
			break;
		}
	}
	SetFocus();
	if (hitId >= 0){
		curId = hitId;
		if(Shift.Contains(ssLeft)){
			Mouse->Capture = Handle;
		}
		if(Shift.Contains(ssRight)){
			TPoint pt = ClientToScreen(TPoint(X,Y));
			rectMenu->Popup(pt.x, pt.y);
		}
	}
	if (barId >= 0){
		curId = barId;
		if(Shift.Contains(ssLeft)){
			Mouse->Capture = Handle;
		}
	}
	Invalidate();
}
void __fastcall TSHPathGraph::MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y){
	CalcScale();
	Invalidate();
}
void __fastcall TSHPathGraph::MouseMove(Classes::TShiftState Shift, int X, int Y){
	if (!path) return;
	if (Shift.Contains(ssLeft)){
		if (hitId >= 0){
			float time=0;
			for(int i=0; i<curId; ++i) time += path->Key(i).time;
			float v = ToV(Y);
			float t = ToT(X);
			t -= time;
			if (t<0) t = 0.001;
			path->Key(curId).time = t;
			path->Key(curId).angle[jointId] = v;
			path->Invalidate();
		}
		if (barId >= 0){
			int d = Y - ToY(path->Key(curId).angle[jointId]);
			if (d<0) d = -d;
			if (d < 0.001f) d = 0.001f;
			d += rectSize.cy/2;
			float p = ToP(d);
			if (p<0.001f) p = 0.001f;
			path->Key(curId).proportional[jointId] = p;
			path->Key(curId).differential[jointId] = p/6;
			path->Invalidate();
		}
	}else{
		path->CalcScale();
		path->Invalidate();
	}
}
void __fastcall TSHPathGraph::DoExit(){
	Invalidate();
}
void __fastcall TSHPathGraph::KeyDown(Word &Key, Classes::TShiftState Shift){
	if (Key == 'A') Add(this);
	if (Key == 'D') Del(this);
}

void __fastcall TSHPathGraph::Add(TObject*){
	if (!path) return;
	curId = path->AddKey(curId);
	path->CalcScale();
	path->Invalidate();
}
void __fastcall TSHPathGraph::Del(TObject*){
	if (!path) return;
	if (path->NKey() > 1) path->DelKey(curId);
	path->CalcScale();
	path->Invalidate();
}


//---------------------------------------------------------------------------
namespace Shpathgraph
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TSHPathGraph)};
         RegisterComponents("SpringHead", classes, 0);
    }
}
//---------------------------------------------------------------------------
 