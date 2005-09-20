// ChairWalkView.cpp : CChairWalkView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "ChairWalk.h"

#include "ChairWalkDoc.h"
#include "ChairWalkView.h"
#include <ImpD3D/SGD3D.h>
#include <Collision/CDLoader.h>
#include <Physics/PHSolid.h>
#include <fstream>
using namespace Spr;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if _WIN32_WINNT >= 0x501

WBRawInputDevice* WBRawInput::Device(HANDLE hD){
	WBRawInputDevice key;
	key.header.hDevice = hD;
	Index::const_iterator f = index.find(&key);
	if (f!= index.end()) return *f;
	return NULL;
}

void WBRawInput::Enum(){
	devices.clear();
	UINT len=0;
	GetRawInputDeviceList(NULL, &len, sizeof(RAWINPUTDEVICELIST));
	RAWINPUTDEVICELIST* devList = new RAWINPUTDEVICELIST[len];
	GetRawInputDeviceList(devList, &len, sizeof(RAWINPUTDEVICELIST));
	for(int i=0; i<len; ++i){
		WBRawInputDevice dev;
		dev.header.hDevice = devList[i].hDevice;
		dev.header.dwType = devList[i].dwType;
		char buf[1024];
		UINT len = sizeof(buf);
		GetRawInputDeviceInfo(dev.header.hDevice, RIDI_DEVICENAME, buf, &len);
		dev.name = buf;
		devices.push_back(dev);
	}
	delete devList;
	std::sort(devices.begin(), devices.end());
	for(WBRawInputDevices::iterator it = devices.begin(); it != devices.end(); ++it){
		index.insert(&*it);
	}
}
void WBRawInput::Register(HWND hWnd){
	RAWINPUTDEVICE rid={1,2,0,hWnd};
	RegisterRawInputDevices(&rid,1,sizeof(rid));
}
int WBRawInput::Get(LPARAM lParam){
	RAWINPUT rawInput;
	UINT cbri = sizeof(RAWINPUT);
	GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &rawInput, &cbri,sizeof(rawInput.header));
	WBRawInputDevice* dev = Device(rawInput.header.hDevice);
	int pos = dev - devices.begin();
	*(RAWINPUT*)dev = rawInput;
	return pos;
}


#endif

/////////////////////////////////////////////////////////////////////////////
// CChairWalkView

IMPLEMENT_DYNCREATE(CChairWalkView, CView)

BEGIN_MESSAGE_MAP(CChairWalkView, CView)
	//{{AFX_MSG_MAP(CChairWalkView)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INPUT, OnInput)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChairWalkView クラスの構築/消滅

CChairWalkView::CChairWalkView()
{
	// TODO: この場所に構築用のコードを追加してください。
	timerID = 0;
	dt = 0.01f;
	bFullscreen = false;
}

CChairWalkView::~CChairWalkView()
{
}

BOOL CChairWalkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChairWalkView クラスの描画

void CChairWalkView::OnDraw(CDC* pDC){
	CChairWalkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
	if (!render) return;

	static bool bDraw = false;

	if (bDraw){
		render->Render(scene->world, camera);
		render->EndScene();
		render->Present();
	}

	RECT rc;
	GetClientRect(&rc);
	bDraw = !render->Resize( Vec2f(rc.right, rc.bottom), m_hWnd, camera );
	render->Clear();
	if (!render->device.CanDraw()) render->BeginScene();
}
/////////////////////////////////////////////////////////////////////////////
// CChairWalkView クラスの診断

#ifdef _DEBUG
void CChairWalkView::AssertValid() const
{
	CView::AssertValid();
}

void CChairWalkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CChairWalkDoc* CChairWalkView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChairWalkDoc)));
	return (CChairWalkDoc*)m_pDocument;
}
#endif //_DEBUG

void CChairWalkView::Load(const char* filename){
	scene->Clear();
	//	std::ofstream file("SceneGraph.txt"); 	
	scene->cameras.clear();

	D3FileDoc fileDoc;
	fileDoc.render = (D3Render*)render;
	UTRef<FIDocNode> doc = fileDoc.Load(filename);
//	doc->Print(DSTR);
	FILoadScene loader;
	RegisterD3DLoader(&loader);
	RegisterXCDLoader(&loader);
	RegisterPHLoader(&loader);
	loader.Load(scene, doc);

	//*************
	//*****************
	camera->data.InitData();
	if (scene->cameras.size() == 0) {
		scene->cameras.push_back(camera);
	} else {
		camera = scene->cameras.back();
	}
	afBody = camera->data.view.inv();
	afHead = Affinef();
	//*************
	scene->world->Print(DSTR);
}
void CChairWalkView::Save(const char* filename){
	if (!scene->world) return;
	//**************
	scene->cameras.back() = camera;
	//**************
	D3SaveScene saver;
	UTRef<D3DocNode> doc = new D3DocNode("ROOT");
	RegisterD3DSaver(&saver);
	RegisterPHSaver(&saver);
	saver.Save(doc, scene);
	D3FileDoc fileDoc;
	for (unsigned int i=0; i<doc->NChildren(); ++i){
		fileDoc.Save(filename, doc->Child(i));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChairWalkView クラスのメッセージ ハンドラ
LRESULT CChairWalkView::OnInput(WPARAM wParam, LPARAM lParam){
	int pos = rawInput.Get(lParam);
	if (rawInput.devices[pos].header.dwType == RIM_TYPEMOUSE){
		float ppm = 800.0f / 2.54f * 100.0f;
		if (pos == 3){
			angleMouse.X() += rawInput.devices[pos].data.mouse.lLastX/ppm;
			angleMouse.Z() += rawInput.devices[pos].data.mouse.lLastY/ppm;
		}else if (pos == 4){
			floorMouse[0].X() = rawInput.devices[pos].data.mouse.lLastX/ppm;
			floorMouse[0].Z() = rawInput.devices[pos].data.mouse.lLastY/ppm;
		}else if (pos == 6){
			floorMouse[1].X() = rawInput.devices[pos].data.mouse.lLastX/ppm;
			floorMouse[1].Z() = rawInput.devices[pos].data.mouse.lLastY/ppm;
		}else if (pos == 5){
			rotMouse.X() += rawInput.devices[pos].data.mouse.lLastX/ppm;
			rotMouse.Y() += rawInput.devices[pos].data.mouse.lLastY/ppm;
		}else{
			DPF("pos:%d x:%d y:%d\n", pos, rawInput.devices[pos].data.mouse.lLastX, rawInput.devices[pos].data.mouse.lLastY);
		}
		Affinef afDelta;
		afDelta.Pos() = (floorMouse[0] + floorMouse[1]) / 2 * 100;
		afDelta.Pos().Z() *= -1;
		const float gapOfFloorSensor = 1.0f;
		afDelta.Ex() = floorMouse[0] - floorMouse[1] + Vec3f(gapOfFloorSensor, 0, 0);
		afDelta.Ex().unitize();
		afDelta.Ez() = Affinef(Rad(-90), 'Y') * afDelta.Ex();
		afDelta.Ey() = afDelta.Ez() ^ afDelta.Ex();
		afLower = afLower * afDelta;
		afUpper = Affinef(Rad(360.0f * rotMouse.Y() / 0.154876f), 'Y') * Affinef(Rad(90.0f * angleMouse.Z() / 0.1f), 'X');
		Affinef afCam = afLower * afUpper * Affinef(Rad(0), 'X');
		afCam.Pos().Y() = 16;
		camera->data.view =	afCam.inv();
	}
	Invalidate();
	return lParam;
}


void CChairWalkView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	float df = 1.0f ; //移動変化量
	if ( nChar == VK_SPACE ) Step();						//	1ステップ実行
	if ( nChar == 'K' ) KillTimer(timerID);					//	連続実行の停止
	if ( nChar == 'T' ) timerID = SetTimer(1, 10, NULL);	//	連続実行の再開

	if ( nChar == 'Q' ) afBody.Pos() = afBody.Pos() - afBody.Ex() * df;
	if ( nChar == 'W' ) afBody.Pos() = afBody.Pos() + afBody.Ex() * df;
	if ( nChar == 'Z' ) afBody.Pos() = afBody.Pos() - afBody.Ey() * df;
	if ( nChar == 'A' ) afBody.Pos() = afBody.Pos() + afBody.Ey() * df;
	if ( nChar == VK_DOWN ) afBody.Pos() = afBody.Pos() - afBody.Ez() * df;
	if ( nChar == VK_UP ) afBody.Pos() = afBody.Pos() + afBody.Ez() * df;
	if ( nChar == VK_LEFT ) afBody = afBody * Affinef(Rad(-5),'y');
	if ( nChar == VK_RIGHT ) afBody = afBody * Affinef(Rad(5),'y');
	if ( nChar == 'S' ) afHead = Affinef(Rad(-5),'x') * afHead;
	if ( nChar == 'X' ) afHead = Affinef(Rad(5),'x') * afHead;
	if ( nChar == 'R' ){
		afBody = Affinef(0,0,-20);
		afBody.LookAt(Vec3f());
		afHead = Affinef();
	}
	if (nChar == 'F'){
		render->EndScene();
		render->device.Fullscreen();
		
		RECT rc;
		GetClientRect(&rc);
		render->Setup( Vec2f(rc.right, rc.bottom), camera );
		render->BeginScene();
		bFullscreen = true;
	}
	if (nChar == 'G'){
		render->EndScene();
		render->device.Window();

		RECT rc;
		GetClientRect(&rc);
		render->Setup( Vec2f(rc.right, rc.bottom), camera );
		render->BeginScene();
		bFullscreen = false;
	}
	if ( nChar == 'N' ){
		if (dt > 0.01f) dt -= 0.01f;
		else dt -= 0.001f;
		DSTR << "dt:" << dt << std::endl;
	}
	if ( nChar == 'M' ){
		if (dt >= 0.01f) dt += 0.01f;
		else dt += 0.001f;
		DSTR << "dt:" << dt << std::endl;
	}
	camera->data.view = (afBody*afHead).inv();
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CChairWalkView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	return true;
}

void CChairWalkView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	//	入力デバイスの列挙
	rawInput.Enum();
	rawInput.Register(m_hWnd);


	Spr::CDFramePair::InitCommonPointMap();
	GetDocument()->view = this;
	if (!render){
		render = new D3Render;
		render->device.Create(m_hWnd);
		scene = new SGScene(render);
		if (!camera) camera = new SGCamera;
	}
	timerID = SetTimer(1, 10, NULL);
}

void CChairWalkView::Step(){
	DWORD t = GetTickCount();
#if 0
	for(int i=0; i<30; ++i){
		scene->Step(dt);
	}
	DWORD dt = GetTickCount() - t;
	DSTR << dt << std::endl;
#else
	scene->Step(dt);
#endif
	Invalidate();
}

void CChairWalkView::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	Step();
	CView::OnTimer(nIDEvent);
/*	static int count;
	count++;
	if (count > 100){
		DSTR << floorMouse[0] << " " << floorMouse[1] << " " << rotMouse << std::endl;
		count = 0;
	}
	*/
}

BOOL CChairWalkView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	SetCursor(NULL);
	render->device->ShowCursor(!bFullscreen);
	return true;
}
