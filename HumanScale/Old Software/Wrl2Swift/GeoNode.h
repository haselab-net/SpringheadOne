#ifndef	_GEONODE_H_
#define	_GEONODE_H_

#include <Base/Utilities.h>
#include <Base/VRBase.h>
#include <GL/glut.h>
#include <SceneGraph/Wrl2view/Wrl2view.h>
#include <algorithm>

using namespace LVRM;

/**	自分のシーングラフを構成するノード．名前つきの(DEFを持つ)VRMLノード
	１つに付き1ノードが作成される．*/
class TGeoNode : public UTRefCount{
public:
	typedef vector< UTRef<TGeoNode> > TGeoNodes;
	typedef map<string, UTRef<TGeoNode> > TNameMap;
	typedef vector<Vec3f> TVertices;
	typedef vector< vector<int> > TFaces;
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
	///	全てのTGeoNodeを入れておく入れ物
	static TGeoNodes allNodes;
	TVertices vertices;
	TFaces	faces;
	///
	TGeoNode(){
		displayList = 0;
		swiftObjectID = 0;
	}
	///
	TGeoNode(const TGeoNode& n):posture(n.posture), displayList(n.displayList){
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
	void DrawGeo();
	///	名前
	string Name() { return name; }
	void Name(string n) { name = n; }

	friend class TGeoNodeBuilder;
};
/**	VRMLシーングラフから，TGeoNodeのシーングラフを作り出す．
	VRMLシーングラフのリスナクラスになっている．*/
class TGeoNodeBuilder:public TType::TListener, public UTRefCount{
protected:
	typedef vector<Affinef> TAffineStack;
	typedef vector< UTRef<TGeoNode> > TNodeStack;
	TAffineStack afStack;
	TNodeStack nodeStack;
	TGeoNode* curNode;
public:
	///
	TGeoNodeBuilder();
	///	TGeoNodeシーングラフの作成．
	void Build(TSts* sts);
	///	出来上がったTGeoNodeシーングラフのルートノード
	UTRef<TGeoNode> Root(){ return nodeStack.front(); }
	///	VRMLのノードからTGeoNodeを作り出す関数．
	TGeoNode* CreateTGeoNode(TNode* n);
protected:
	///	名前付きノードが見つかったときのコールバック
	virtual void FoundNamed(TNode *pn);
	///	Transformノードが見つかったときのコールバック
	virtual void FoundTransformBefore(TNode *pn);
	///	Transformノードが見つかったときのコールバック
	virtual void FoundTransformAfter(TNode *pn);
	///	EnumTypeNodesで毎回呼び出されるコールバック．trueを返すと子ノードも探す．
	virtual bool FoundTypeNode(TNode *pn);
};

#endif // _GEONODE_H
