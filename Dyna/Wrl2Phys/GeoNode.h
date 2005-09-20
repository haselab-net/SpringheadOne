#ifndef	_TGeoNode_H_
#define	_TGeoNode_H_

#include "CalcPenalty.h"
#include <penalty.h>
#include <Base/Utilities.h>
#include <Base/VRBase.h>
#define GLUT_NO_LIB_PRAGMA
#include <GL/glut.h>
#include <SceneGraph/Wrl2view/Wrl2view.h>
#include <algorithm>

class Object;
using namespace LVRM;

/**	自分のシーングラフを構成するノード．名前つきの(DEFを持つ)VRMLノード
	１つに付き1ノードが作成される．*/
class TGeoNode : public UTRefCount{
public:
	///	このノードの姿勢．(5/6変更)
	Affinef	posture;
protected:
	///	ノード名
	string name;
	///	対応するVRMLノード 
	UTRef<TNode> vrmlNode;
	///	OpenGLのDisplayListのID
	int	displayList;

public:
	//	形状 --------------------------------------------------------
	///	頂点の座標
	TCoords vertices;
	///	多面体たち
	TPolytopes polytopes;
	///	SOLIDのオブジェクトへのポインタ
	Object* solid;

	//	ダイナミクス ------------------------------------------------
	///	質量
	float mass;
	///	慣性テンソル
	Matrix3f inertia;
	///	運動量
	Vec3f momentum;
	///	角運動量
	Vec3f angularMomentum;
	///	重心の位置
	Vec3f center;
	///
	Vec3f penalty, angularPenalty;
	///
	Vec3f lastPenalty, lastAngularPenalty;
	/// 摩擦力ベースボックス
/*	class FricBase{
	public:
		Vec3f friction;
		TGeoNode* g;
		float dt;
	};*/
//	typedef std::pair<TPenaltyDetector*, Vec3f> FricBase;
//	vector<FricBase> fricBases;	//(5/9追加)
	// --------------------------------------------------------------
	///
	TGeoNode();
	///
	TGeoNode(const TGeoNode& n);
	///	対応するVRMLノード
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	///	親ノードに対するこのノードの姿勢の取得
	Affinef GetPosture(){ return posture; }
	///	親ノードに対するこのノードの姿勢の設定
	void SetPosture(Affinef a){ posture = a; }
	///	重心の位置
	Vec3f Center(){ return center; }
	///	速度

	// --------------------------------------------------------------
	/// 速度
	Vec3f Vel(){ return momentum / mass; }
	///	角速度
	Vec3f AngVel(){
		Matrix3f rot = posture.Rot();
		return rot.trans() * inertia.inv() * rot * angularMomentum;
	}
	///	姿勢の更新
	void UpdatePosture(float dt);
	///	ペナルティを加える．ワールド系原点を基準とする．
	void AddPenalty(Vec3f p, Vec3f a);
	///	ペナルティを力に変換する．
	void ConvertToForce(float dt);
	///	重力を加える
	void AddGravity(Vec3f gravity, float dt);
	///	空気抵抗
	void AirResistance(float a, float dt){
		momentum = momentum * (1 - a*dt);
		angularMomentum = angularMomentum * (1 - a*dt);
	}

 	// --------------------------------------------------------------
	///	ノードを描画
	void Draw();
	void DrawGeometry();
	///	名前
	string Name() { return name; }
	void Name(string n) { name = n; }

	friend class TGeoNodeBuilder;
};
typedef vector< UTRef<TGeoNode> > TGeoNodes;
/**	VRMLシーングラフから，TGeoNodeのシーングラフを作り出す．
	VRMLシーングラフのリスナクラスになっている．*/
class TGeoNodeBuilder:public TType::TListener, public UTRefCount{
protected:
	typedef vector<Affinef> TAffineStack;
	typedef vector< UTRef<TGeoNode> > TNodeStack;
	TAffineStack afStack;
	TGeoNode* curNode;
public:
	///	生成したノード	
	TGeoNodes allNodes;
	///
	Vec3f clearColor;
	///
	Affinef viewPoint;
	///
	Affinef projection;
	///
	TGeoNodeBuilder();
	///	TGeoNodeシーングラフの作成．
	void Build(TSts* sts);
	///	VRMLのノードからTGeoNodeを作り出す関数．
	TGeoNode* CreateTGeoNode(TNode* n);
protected:
	///	名前付きノードが見つかったときのコールバック
	virtual void FoundNamed(TNode *pn);
	///	何でもコールバック
	bool FoundTypeNode(TNode *pn);
	///	Transformノードが見つかったときのコールバック
	virtual void FoundTransformBefore(TNode *pn);
	///	Transformノードが見つかったときのコールバック
	virtual void FoundTransformAfter(TNode *pn);
};

#endif // _TGeoNode_H_
