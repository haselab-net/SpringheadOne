#ifndef	_TMyNode_H_
#define	_TMyNode_H_

#include <Base/BaseUtility.h>
#define GLUT_NO_LIB_PRAGMA
#include <GL/glut.h>
#include <LVRMScene/Wrl2view/Wrl2view.h>
#include <algorithm>

using namespace Spr;

/**	自分のシーングラフを構成するノード．名前つきの(DEFを持つ)VRMLノード
	１つに付き1ノードが作成される．*/
class TMyNode : public UTRefCount{
public:
	typedef vector<Vec3f> TVertices;
	typedef vector<int> TFace;
	typedef vector< TFace > TFaces;
protected:
	///	ノード名
	string name;
	///	このノードの姿勢．
	Affinef	posture;
	///	対応するVRMLノード
	UTRef<TNode> vrmlNode;
	///	OpenGLのDisplayListのID
	int	displayList;
	///	swift の ObjectID
	int swiftObjectID;
	///	decomposer が出力したファイルの名前
	string decompFile;
	///	decomposer に入力するファイルの名前
	string polyFile;

public:
	///	BBox
	float radius;
	float depth;
	void CalcZ(const Affinef& afView);
	///	全てのTMyNodeを入れておく入れ物
	TVertices vertices;
	TFaces	faces;
	///
	TMyNode(){
		displayList = 0;
		swiftObjectID = 0;
	}
	///
	TMyNode(const TMyNode& n):posture(n.posture), displayList(n.displayList){
		swiftObjectID = 0;
	}
	///	対応するVRMLノード
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	///	親ノードに対するこのノードの姿勢の取得
	Affinef GetPosture(){ return posture; }
	///	親ノードに対するこのノードの姿勢の設定
	void SetPosture(Affinef a){ posture = a; }
	///	ノードを描画
	void Draw();
	void DrawGeometry();
	///	名前
	string Name() { return name; }
	void Name(string n) { name = n; }

	friend class TMyNodeBuilder;
};
class TMyNodes: public vector< UTRef<TMyNode> >{
public:
	void ZSort(const Affinef& afView);
	void Draw();
};

/**	VRMLシーングラフから，TMyNodeのシーングラフを作り出す．
	VRMLシーングラフのリスナクラスになっている．*/
class TMyNodeBuilder:public TType::TListener, public UTRefCount{
protected:
	typedef vector<Affinef> TAffineStack;
	typedef vector< UTRef<TMyNode> > TNodeStack;
	TAffineStack afStack;
	TMyNode* curNode;
public:
	///	生成したノード	
	TMyNodes allNodes;
	///
	Vec3f clearColor;
	///
	Affinef viewPoint;
	///
	Affinef projection;
	///
	TMyNodeBuilder();
	///	TMyNodeシーングラフの作成．
	void Build(TSts* sts);
	///	VRMLのノードからTMyNodeを作り出す関数．
	TMyNode* CreateTMyNode(TNode* n);
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

#endif // _TMyNode_H_
