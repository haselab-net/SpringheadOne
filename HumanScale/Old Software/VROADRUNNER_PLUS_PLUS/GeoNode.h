#ifndef	_TGeoNode_H_
#define	_TGeoNode_H_

/*
#include <Base/Utilities.h>
#include <Base/VRBase.h>
#define GLUT_NO_LIB_PRAGMA
#include <GL/glut.h>
#include <SceneGraph/Wrl2view/Wrl2view.h>
#include <algorithm>
*/
//#include "CalcPenalty.h"

#include <penalty.h>
#include <Base/Utilities.h>
#include <Base/VRBase.h>
#define GLUT_NO_LIB_PRAGMA
#include <GL/glut.h>
#include <SceneGraph/Wrl2view/Wrl2view.h>
#include <algorithm>



using namespace LVRM;
class Object;

class TGLView;

/**	自分のシーングラフを構成するノード．名前つきの(DEFを持つ)VRMLノード
	１つに付き1ノードが作成される．*/
class TGeoNode : public UTRefCount{
public:
	typedef vector<Vec3f> TVertices;
	typedef vector<int> TFace;
	typedef vector< TFace > TFaces;
protected:
	///	ノード名
	string name;
	///	このノードの姿勢
	Affinef	posture;
	///	対応するVRMLノード
	UTRef<TNode> vrmlNode;
	/// 親ノード
	UTRef<TGeoNode> parent;
	///	OpenGLのDisplayListのID
	int	displayList;
	///	swift の ObjectID
	int swiftObjectID;
	///	decomposer が出力したファイルの名前
	string decompFile;
	///	decomposer に入力するファイルの名前
	string polyFile;
public:
	virtual void InitInstance();
	// アバターの位置のポインタ
	//Affinef *p_afBody;
	TGLView* p_TGLView;

	///	全てのTGeoNodeを入れておく入れ物
	TVertices vertices;
	TFaces faces;
	
	///	SOLIDのオブジェクトへのポインタ
	Object* solid;

	//  オブジェクトを囲むバウンディングボックス
	Vec4f VecObjectBox[4];
	//  オブジェクトのID
	int objectID;
	///
	TGeoNode(){
		objectID = 0;
		displayList = 0;
		swiftObjectID = 0;
	}
	///
	TGeoNode(const TGeoNode& n):posture(n.posture), displayList(n.displayList){
		objectID = 0;
		swiftObjectID = 0;
	}
	///	対応するVRMLノード
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	///	親ノードに対するこのノードの姿勢の取得
	Affinef GetPosture(){ return posture; }
	///	親ノードに対するこのノードの姿勢の設定
	void SetPosture(Affinef a){ posture = a; }
	/// オブジェクトのワールド座標を取得
	Affinef GetGlobalPosture(){
		if (parent) return parent->GetGlobalPosture() * posture;
		return posture;
	}
	///	ノードを描画
	virtual void Draw();
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
	bool bDontDisplay;
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
	///
	int countID;
	///	
	void DontDisplay();
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

