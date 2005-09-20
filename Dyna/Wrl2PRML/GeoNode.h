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
	Affined	posture;
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
	double mass;
	///	慣性テンソル
	Matrix3d inertia;
	///	運動量
	Vec3d momentum;
	///	角運動量
	Vec3d angularMomentum;
	///	重心の位置
	Vec3d center;
	///
	Vec3d penalty, angularPenalty;
	///
	Vec3d lastPenalty, lastAngularPenalty;
	/// 摩擦力ベースボックス
/*	class FricBase{
	public:
		Vec3d friction;
		TGeoNode* g;
		double dt;
	};*/
//	typedef std::pair<TPenaltyDetector*, Vec3d> FricBase;
//	vector<FricBase> fricBases;	//(5/9追加)
	// --------------------------------------------------------------
	///
	TGeoNode();
	///
	TGeoNode(const TGeoNode& n);
	///	対応するVRMLノード
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	///	親ノードに対するこのノードの姿勢の取得
	Affined GetPosture(){ return posture; }
	///	親ノードに対するこのノードの姿勢の設定
	void SetPosture(Affined a){ posture = a; }
	///	重心の位置
	Vec3d Center(){ return center; }
	///	速度

	// --------------------------------------------------------------
	/// 速度
	Vec3d Vel(){ return momentum / mass; }
	///	角速度
	Vec3d AngVel(){
		Matrix3d rot = posture.Rot();
		return rot.trans() * inertia.inv() * rot * angularMomentum;
	}
	///	姿勢の更新
	void UpdatePosture(double dt);
	///	ペナルティを加える．ワールド系原点を基準とする．
	void AddPenalty(Vec3d p, Vec3d a);
	///	ペナルティを力に変換する．
	void ConvertToForce(double dt);
	///	重力を加える
	void AddGravity(Vec3d gravity, double dt);
	///	空気抵抗
	void AirResistance(double a, double dt){
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
	typedef vector<Affined> TAffineStack;
	typedef vector< UTRef<TGeoNode> > TNodeStack;
	TAffineStack afStack;
	TGeoNode* curNode;
public:
	///	生成したノード	
	TGeoNodes allNodes;
	///
	Vec3d clearColor;
	///
	Affined viewPoint;
	///
	Affined projection;
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
