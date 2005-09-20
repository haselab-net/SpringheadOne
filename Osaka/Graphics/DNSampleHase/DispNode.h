#ifndef	_DISPNODE_H_
#define	_DISPNODE_H_

#include <Base/Utilities.h>
#include <Base/VRBase.h>
#include <GL/glut.h>
#include <SceneGraph/libWrl2view/libWrl2view.h>
#include <algorithm>

using namespace std;

///////////////////////////////////////////////////////////////
// Class Define

class ViewPoint : public UTRefCount{
protected:
	static float position[3];
	static float orientation[4];
	static float fieldOfView;
};
class DispNode : public UTRefCount{
public:
	typedef vector< UTRef<DispNode> > TChildren;
	typedef map<string,UTRef<DispNode> > TNameMap;
protected:
	string	name;
	TChildren nodeChildren;
	Affinef	posture;
	UTRef<TNode> vrmlNode;
	int	displayList;
	UTRef<DispNode> parent;
	UTRef<ViewPoint> viewPoint;
	static TNameMap nodeMap;

public:
	DispNode(){ displayList = 0; }
	DispNode(const DispNode& n):posture(n.posture), displayList(n.displayList){}
	static TNameMap& NodeMap(){ return nodeMap; }
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	Affinef GetPosture(){ return posture; }
	void SetPosture(Affinef a){ posture = a; }
	Affinef GetGlobalPosture();
	void SetGtoLPosture(Affinef);
	UTRef<DispNode> GetParent(){ return parent; }
	void SetParent(UTRef<DispNode> p){ parent = p; }
	///	子ノードを追加
	void AddChild(UTRef<DispNode> inNode){
		inNode->SetParent(this);
		nodeChildren.push_back(inNode);
	}
	///	子ノードを削除
	void DelChild(UTRef<DispNode> inNode){
		inNode->SetParent(NULL);
		TChildren::iterator it = find(nodeChildren.begin(), nodeChildren.end(), inNode);
		if (it != nodeChildren.end()) nodeChildren.erase(it);
	}
	TChildren& Children(){ return nodeChildren; }
	void DelFromParent(){
		if(GetParent()){
			GetParent()->DelChild(this);
		}
	}
	///	ノードを再帰的に描画
	void Draw();
	///	名前
	string Name() { return name; }
	void Name(string n) { name = n; }
	///	アニメーションのためのコールバック
	virtual void Animation(){ }
	///	ツリーのコピー
	UTRef<DispNode> Copy(int suffix);
	///	Dump
	void Dump(std::ostream& os);
protected:
	///	このノードのクローンの作成
	virtual UTRef<DispNode> Clone(){ return new DispNode(*this); }
	friend class DispNodeBuilder;
};
class DispFootNode : public DispNode{
public:
	virtual UTRef<DispNode> Clone(){ return new DispFootNode(*this); }
	virtual void Animation();
};


class DispNodeBuilder:public TType::TListener, public UTRefCount{
protected:
	typedef vector<Affinef> TAffineStack;
	typedef vector< UTRef<DispNode> > TNodeStack;
	typedef vector< UTRef<ViewPoint> > TViewPoints;

	TAffineStack afStack;
	TNodeStack nodeStack;
	TViewPoints viewPoints;
public:
	DispNodeBuilder();
	void Build(TSts* sts);
	UTRef<DispNode> Root(){ return nodeStack.front(); }
	DispNode* CreateDispNode(TNode* n);
protected:
	virtual void FoundNamed(TNode *pn);
	virtual void FoundTransformBefore(TNode *pn);
	virtual void FoundTransformAfter(TNode *pn);
	virtual void FoundViewPoint(TNode *pn);
};

class Character{
public:
	DispNode* workParent;
	int characterId;
	void AddChara(DispNode*);
	void DelChara(DispNode*);
};

class Penguin : public Character{
public:
	DispNode* penguinRoot; // ペンギンのroot
	DispNode* rWing; // ペンギンの羽
	DispNode* lWing;
	DispNode* rFoot; // ペンギンの足
	DispNode* lFoot;
	//int penguinId; // ペンギンのID(0〜)
};

class Puck : public Character{
public:
	DispNode* puckRoot;
	//int puckId;
};

#endif // _DISPNODE_H_
