#ifndef SGBEHAVIORENGINE_H
#define SGBEHAVIORENGINE_H

#include "SGEngine.h"

namespace Spr {;
///	シーンの振舞い（ビヘイビア）を管理するエンジン
/// ビヘイビアの分類と優先順位
///	100の倍数は分類を表し、以下の位はその中での実行順序を規定する
enum SGBehaviorPriority{
	//初期化モジュール
	SGBP_INITIALIZER			= 100,
	SGBP_CLEARFORCE,
	//力生成モジュール
	SGBP_FORCEGENERATOR			= 200,
		SGBP_GRAVITYENGINE,
		SGBP_COLLISIONENGINE,
		SGBP_PENALTYENGINE,
		SGBP_CREATURE,
	//積分の対象
	SGBP_DYNAMICALSYSTEM		= 300,
		SGBP_JOINTENGINE,
		SGBP_SOLIDCONTAINER,
	//
	SGBP_MOTION					= 400,
		SGBP_CHANGEOBJECT,
		SGBP_CONSTRAINT,
	//他のエンジンのリスナ
	SGBP_LISTENER				= 500,
		SGBP_CONTACTENGINE,
		SGBP_STICKENGINE,
		SGBP_WARPENGINE,
};
class GRRender;
class SGBehaviorStates;
///	動作エンジンの基本クラス．
class SGBehaviorEngine:public SGEngine{
public:
	SGOBJECTDEF(SGBehaviorEngine);
	///	実行順序を決めるプライオリティ値．小さいほど早い
	virtual int GetPriority() const { return SGBP_FORCEGENERATOR; }
	///	時間を dt 進める
	virtual void Step(SGScene* s){}
	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states){}
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const {}
	///	レンダリング
	virtual void Render(GRRender* r, SGScene* s){}
};

///	SGBehaviorEngineの配列
class SGBehaviorEngines: public SGEngines{
protected:
	iterator initializer;
	iterator forceGenerator;
	iterator dynamicalSystem;
	iterator motion;
	iterator listener;
public:
	SGBehaviorEngines();
	///@name シミュレーションを進めるメソッド
	//@{	
	///	力のクリア．
	virtual void ClearForce(SGScene* s);
	///	力を生成(計算)する
	virtual void GenerateForce(SGScene* s);
	///	ODE(積分処理)．力→速度，速度→位置 の計算を行う．
	virtual void Integrate(SGScene* s);
	//@}
	void Add(SGEngine* e);
	bool Del(SGEngine* e);
	void Sort();
	///	ロード後の処理
	virtual void Loaded(SGScene* scene);
	///	シーングラフのクリア時の処理
	virtual void Clear(SGScene* s);

	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
	///	レンダリング
	virtual void Render(GRRender* r, SGScene* s){
		for(iterator it = begin2(); it != end2(); ++it){
			((SGBehaviorEngine*)&**it)->Render(r,s);
		}
	}
};

/**	BehaviorEngineの状態保存用	*/
class SGBehaviorState: public UTRefCount{
public:
	SGOBJECTDEF(SGBehaviorState);
};
///	全BehaviorEngineの状態を保持するためのクラス
class SGBehaviorStates:public std::vector< UTRef<SGBehaviorState> >{
public:
	typedef std::vector< UTRef<SGBehaviorState> > base;
	mutable unsigned cur;
	SGBehaviorStates():cur(0){}
	void clear(){ base::clear(); cur=0; }
	void SetCursorFirst() const { cur = 0; }
	const SGBehaviorState* GetNext() const { return cur<size() ? &*at(cur++) : NULL; }
	SGBehaviorState* GetNext(){ return cur<size() ? &*at(cur++) : NULL; }
};
///	
class SGBehaviorStateStack: public std::vector<SGBehaviorStates>{
};
}
#endif
