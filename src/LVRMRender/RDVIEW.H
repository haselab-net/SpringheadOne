#ifndef VR_REND_RENDER_H
#define VR_REND_RENDER_H
#include <Base/Affine.h>
#include <vector>

namespace Spr {

/**	@mainpage LVRM Render ライブラリ
*/

///	RDView  --  display class
class RDView{
protected:
	///	タイマーハンドラーの基底クラス
	class TTimerHandlerBase{
	public:
		int interval;
		virtual bool Call()=0;
	};
	///	タイマーハンドラークラスの配列
	class TTimerHandlerBases:public std::vector<TTimerHandlerBase*>{
	public:
		int Add(TTimerHandlerBase* handler){
			for(iterator it=begin(); it!=end(); ++it){
				if (!*it){
					*it = handler;
					return it - begin();
				}
			}
			push_back(handler);
			return size()-1;
		}
		void Del(int id){
			delete begin()[id];
			begin()[id] = NULL;
		}
	};
	///	タイマーハンドラー本体
	template <class T, class F> class TTimerHandler: public TTimerHandlerBase{
	public:
		F func;
		T* that;
		TTimerHandler(int i, T* t, F f): that(t), func(f){ interval=i; }
		bool Call(){ return (that->*func)(); }
	};
	///	タイマーハンドラーを入れておく配列
	TTimerHandlerBases timerHandlers;
	
	///	更新要求フラグ
	bool invalidated;
	///	タイマーを実際にセットする関数．
	virtual void DoSetTimer(int id){}
public:
	///	キーボードの状態を示す変数．
	int keyboard[256];
	///	特殊キーの状態を示す変数．
	int specialKey[256];
	///
	RDView();
	///
	virtual ~RDView();
	//	operation
	///	Init and create window.
	virtual void Init(const char* window_title)=0;
	///	Run main loop.
	virtual void Run()=0;
	///	Quit the program.
	virtual void Exit()=0;

	///	Set window position.
	virtual void WindowPosition(int x, int y, int w, int h)=0;
	///	Flip (update) buffer.
	virtual void Flip()=0;
	///	Request rendering.
	void Invalidate(){ invalidated = true; }
	///	Cancel the request of rendering.
	void Validate(){invalidated = false; }
	
	///	タイマーの設定
	template <class T>
	int SetTimer(int interval, bool (T::*func)()){
		typedef bool (T::*FUNC)();
		int id = timerHandlers.Add(new TTimerHandler<T,FUNC>(interval, (T*)this, func));
		DoSetTimer(id);
		return id;
	}
	///	タイマーの解除
	void ReleaseTimer(int id){ timerHandlers.Del(id); }

	///	キーボード(普通のキー)の状態
	virtual bool Keyboard(int code){
		if(code<0 || code>sizeof(keyboard)/sizeof(keyboard[0])) return false;
		return keyboard[code] != 0;
	}
	///	キーボード(特殊キー)の状態
	virtual bool SpecialKey(int code){
		if(code<0 || code>sizeof(specialKey)/sizeof(specialKey[0])) return false;
		return specialKey[code] != 0;
	}
public:
	//	Call backs.
	///	描画要求があったときに呼び出されるコールバック
	virtual void OnDraw(){}
	///	ひまなときに呼び出されるコールバック
	virtual void OnIdle(){}
	///	ウィンドウサイズが変化したときに呼び出されるコールバック
	virtual void OnSize(int w, int h){}
	///	最初のウィンドウを開くまえに呼び出されるコールバック
	virtual void OnOpen(){}
	///	プログラム終了前に呼び出されるコールバック
	virtual void OnClose(){}
	///	マウスのボタンを表す
	enum Button {NONE, LEFT, MIDDLE, RIGHT};
	///	マウスがクリックされたときに呼び出されるコールバック
	virtual void OnMouse(Button b, bool on, int x, int y){}
	///	マウスが動いたときに呼び出されるコールバック
	virtual void OnMouseMove(bool on, int x, int y){}
	///	キー入力時に呼び出されるコールバック
	virtual void OnKeyboard(int code, int x, int y){}
	///	矢印などの特殊キー入力時に呼び出されるコールバック
	virtual void OnSpecialKey(int code, int x, int y){}
	///	ジョイスティックのコールバック
	virtual void OnJoystick(unsigned int button, int x, int y, int z){}
};

 
}
#endif
