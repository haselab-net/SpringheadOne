#ifndef SG_FRAME_H
#define SG_FRAME_H

#include "SGObject.h"

namespace Spr {;
///	バウンディングボックスの実装
class SGBBox{
	Vec3f bboxCenter;	///<	BBoxの中心(ローカル系)
	Vec3f bboxExtent;	///<	BBoxの大きさ(ローカル系)
public:
	///	バウンディングボックスの設定
	void SetBBoxCenterExtent(Vec3f c, Vec3f e){
		bboxCenter = c;
		bboxExtent = e;
	}
	///	バウンディングボックスの設定
	void SetBBoxMinMax(Vec3f bmin, Vec3f bmax){
		bboxCenter = (bmin+bmax)*0.5f;
		bboxExtent = (bmax-bmin)*0.5f;
	}
	///	バウンディングボックスの追加
	void AddBBox(Vec3f bmin, Vec3f bmax){
		Vec3f bboxMin = GetBBoxMin();
		Vec3f bboxMax = GetBBoxMax();
		bboxMin.element_min(bmin);
		bboxMax.element_max(bmax);
		SetBBoxMinMax(bboxMin, bboxMax);
	}
	///	中心
	Vec3f GetBBoxCenter(){ return bboxCenter; }
	///	大きさ
	Vec3f GetBBoxExtent(){ return bboxExtent; }
	///	小さい端点
	Vec3f GetBBoxMin(){ return bboxCenter-bboxExtent; }
	///	大きい端点
	Vec3f GetBBoxMax(){ return bboxCenter+bboxExtent; }
	///	与えられたベクトルとの内積が最大と最小の点
	void GetSupport(const Vec3f& dir, float& minS, float& maxS){
		Vec3f ext0( bboxExtent.X(),  bboxExtent.Y(),  bboxExtent.Z());
		Vec3f ext1(-bboxExtent.X(),  bboxExtent.Y(),  bboxExtent.Z());
		Vec3f ext2( bboxExtent.X(), -bboxExtent.Y(),  bboxExtent.Z());
		Vec3f ext3( bboxExtent.X(),  bboxExtent.Y(), -bboxExtent.Z());
		float d = abs(dir*ext0);
		float d1 = abs(dir*ext1);
		if (d < d1) d = d1;
		float d2 = abs(dir*ext2);
		if (d < d2) d = d2;
		float d3 = abs(dir*ext3);
		if (d < d3) d = d3;
		float c = dir * bboxCenter;
		minS = c-d;
		maxS = c+d;
	}
};

class SGFrame;
///	Spr::Frame の配列．
class SPR_DLL SGFrames:public std::vector< UTRef<SGFrame> >{
public:
};

/**	シーングラフのインタフェースノード．
	シーングラフの操作を受け付けるノード．位置姿勢を保持する．
	他のデータのポインタを保持する．	*/
class SPR_DLL SGFrame: public SGObject, public UTTreeNode<SGFrame, SGFrames>{
public:
	SGOBJECTDEF(SGFrame);
	typedef UTTreeNode<SGFrame, SGFrames> Tree;
protected:
	///	姿勢
	Affinef posture;
public:
	///	バウンディングボックス
	SGBBox bbox;
	///	フレームの中身(子フレームは含まない)
	SGObjects contents;

	///@name シーングラフ構造の操作
	//@{
	///	所有しているオブジェクトの数
	virtual size_t NChildObjects();
	///	所有しているオブジェクト
	virtual SGObject* ChildObject(size_t i);
	///	子オブジェクトの追加
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	子オブジェクトの削除
	virtual bool DelChildObject(SGObject* o, SGScene* s);

	virtual SGFrame* FindFrame(UTString);

	///	子になりえるオブジェクトの型情報の配列
	virtual const UTTypeInfo** ChildCandidates();
	template <class T>
	void FindObject(T*& obj){
		for(unsigned i=0; i<NChildObjects(); ++i){
			obj = DCAST(T, ChildObject(i));
			if (obj) return;
		}
	}
	template <class T> void FindObject(UTRef<T>& obj){
		T* t = &*obj;
		FindObject(t);
		obj = t;
	}


	///	子ノードなどをすべて消去する．
	void Clear();
	
	///	関連付けられたdocを開放
	virtual void ReleaseDoc(){
		doc = NULL;
		ForEachChild(&SGFrame::ReleaseDoc);
		for(SGObjects::iterator it = contents.begin(); it != contents.end(); ++it){
			(*it)->ReleaseDoc();
		}
	}
	///	ノードのダンプ表示
	void Print(std::ostream& os) const;
	//@}
	
	///@name 姿勢(位置，向き)
	//@{
	const Vec3f& GetPosition(){ return posture.Pos(); }
	Matrix3f GetRotation(){ return posture.Rot(); }
	template<class T> void SetPosition(T p){ posture.Pos() = p; }
	template<class T> void SetRotation(T p){ posture.Rot() = p; }
	///	親ノードに対する姿勢(位置と回転をあらわす4x4行列)を返す．
	const Affinef& GetPosture(){ return posture;}
	/**	親ノードに対する姿勢(位置と回転をあらわす4x4行列)を設定する．
		PHSolidを使っているときは，SGFrame::SetPosture()は使ってはならない．
		毎回，Solidが姿勢を上書きしてしまうので． */
	void SetPosture(const Affinef& a){ posture = a; }
	///	ワールド座標系に対する姿勢(位置と回転をあらわす4x4行列)を返す．
	Affinef GetWorldPosture(){
		Affinef af;
		SGFrame* fr = this;
		while(fr){
			af = fr->posture * af;
			fr = fr->GetParent();
		}
		return af;
	}
	/**	ワールド座標系に対する姿勢(位置と回転をあらわす4x4行列)を設定する．
		PHSolidを使っているときは，SGFrame::SetPosture()は使ってはならない．
		毎回，Solidが姿勢を上書きしてしまうので． */
	void SetWorldPosture(const Affinef& a){
		if (GetParent()){
			posture = GetParent()->GetWorldPosture().inv() * a;
		}else{
			posture = a;
		}
	}
	///	バウンディングボックスの計算
	virtual void CalcBBox();
	///	バウンディングボックスの両端の取得
	void GetBBoxSupport(const Vec3f& dir, float& minS, float& maxS){
		bbox.GetSupport(GetPosture().Rot().inv()*dir, minS, maxS);
		float c = GetPosture().Pos() * dir;
		minS += c;
		maxS += c;
	}
	//@}
	///	ロード後，参照の解決が終わってから呼び出す関数．
	void Loaded(SGScene* scene);

	///	子フレームを含めて，すべてのContactsを列挙する
	void EnumContents(SGObjects& objs);
	void EnumContents(SGObjects& objs, SGFrames& frames);
};

}
#endif
