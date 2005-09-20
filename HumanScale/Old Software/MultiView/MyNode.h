#ifndef	_MYNODE_H_
#define	_MYNODE_H_

#include <Base/Utilities.h>
#include <Base/VRBase.h>
#include <GL/glut.h>
#include <SceneGraph/Wrl2view/Wrl2view.h>
#include <algorithm>

using namespace LVRM;

/**	自分のシーングラフを構成するノード．名前つきの(DEFを持つ)VRMLノード
	１つに付き1ノードが作成される．*/
class MyNode : public UTRefCount{
public:
	typedef vector< UTRef<MyNode> > TChildren;
	typedef map<string,UTRef<MyNode> > TNameMap;
protected:
	///	ノード名
	string	name;
	///	親ノード
	MyNode* parent;
	///	子ノード
	TChildren children;
	///	親ノードに対するこのノードの姿勢．
	Affinef	posture;
	///	対応するVRMLノード
	UTRef<TNode> vrmlNode;
	///	OpenGLのDisplayListのID
	int	displayList;

	///	名前とノードの対応表
	static TNameMap nodeMap;

public:
	///
	MyNode(){
		displayList = 0;
		parent = NULL;
	}
	///
	MyNode(const MyNode& n):posture(n.posture), displayList(n.displayList){
		parent = NULL;
	}
	///	対応するVRMLノード
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	///	親ノードに対するこのノードの姿勢の取得
	Affinef GetPosture(){ return posture; }
	///	親ノードに対するこのノードの姿勢の設定
	void SetPosture(Affinef a){ posture = a; }
	///	親ノードを取得
	MyNode* GetParent(){ return parent; }
	///	子ノードを追加
	void AddChild(UTRef<MyNode> inNode){
		inNode->SetParent(this);
		children.push_back(inNode);
	}
	///	子ノードを削除
	void DelChild(UTRef<MyNode> inNode){
		TChildren::iterator it = find(children.begin(), children.end(), inNode);
		if (it != children.end()){
			(*it)->parent = NULL;
			children.erase(it);
		}
	}
	///	ノードを再帰的に描画
	void Draw();
	///	名前
	string Name() { return name; }
	void Name(string n) { name = n; }
	///	ツリーのコピー．suffixに名前につけるサフィックスを指定．
	UTRef<MyNode> Copy(int suffix);
protected:
	///	このノードのクローンの作成
	virtual UTRef<MyNode> Clone(){ return new MyNode(*this); }
	void SetParent(MyNode* n){
		if (parent) parent->DelChild(this);
		parent = n;
	}
	friend class MyNodeBuilder;
};
/**	VRMLシーングラフから，MyNodeのシーングラフを作り出す．
	VRMLシーングラフのリスナクラスになっている．*/
class MyNodeBuilder:public TType::TListener, public UTRefCount{
protected:
	typedef vector<Affinef> TAffineStack;
	typedef vector< UTRef<MyNode> > TNodeStack;
	TAffineStack afStack;
	TNodeStack nodeStack;
public:
	///
	MyNodeBuilder();
	///	MyNodeシーングラフの作成．
	void Build(TSts* sts);
	///	出来上がったMyNodeシーングラフのルートノード
	UTRef<MyNode> Root(){ return nodeStack.front(); }
	///	VRMLのノードからMyNodeを作り出す関数．
	MyNode* CreateMyNode(TNode* n);
protected:
	///	名前付きノードが見つかったときのコールバック
	virtual void FoundNamed(TNode *pn);
	///	Transformノードが見つかったときのコールバック
	virtual void FoundTransformBefore(TNode *pn);
	///	Transformノードが見つかったときのコールバック
	virtual void FoundTransformAfter(TNode *pn);
};

#endif // _MYNODE_H_
