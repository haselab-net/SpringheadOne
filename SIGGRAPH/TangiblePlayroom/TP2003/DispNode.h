#ifndef	_DISPNODE_H_
#define	_DISPNODE_H_

#include <Base/Utilities.h>
#include <Base/VRBase.h>
#include <SceneGraph/Wrl2View/Wrl2view.h>
#include <algorithm>
#include <string>
#include <vector>


using namespace LVRM;

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
	typedef map<string, UTRef<DispNode> > TNameMap;
protected:
	bool bVisible;
	string	name;
	TChildren nodeChildren;
	Affinef	posture;
	UTRef<TNode> vrmlNode;
	int	displayList;
	UTRef<DispNode> parent;
	UTRef<ViewPoint> viewPoint;
	static TNameMap nodeMap;

public:
	DispNode(){ displayList = 0; bVisible = true; }
	DispNode(const DispNode& n):posture(n.posture), displayList(n.displayList){ bVisible = n.bVisible; }
	static TNameMap& NodeMap(){ return nodeMap; }
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	Affinef GetPosture(){ return posture; }
	void SetPosture(Affinef a){ posture = a; }
	Affinef GetGlobalPosture(){
		if (parent) return parent->GetGlobalPosture() * posture;
		return posture;
	}
	void SetGtoLPosture(Affinef);
	void SetGlobalPosture(const Affinef& af){
		SetGtoLPosture(af);
	}
	UTRef<DispNode> GetParent(){ return parent; }

	void SetParent(UTRef<DispNode> p){ parent = p; }
	///	�q�m�[�h��ǉ�
	void AddChild(UTRef<DispNode> inNode){
		inNode->SetParent(this);
		nodeChildren.push_back(inNode);
	}
	///	�q�m�[�h���폜
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
	///	�m�[�h���ċA�I�ɕ`��
	virtual void Draw();
	///	���O
	string Name() { return name; }
	void Name(string n) { name = n; }
	///	�A�j���[�V�����̂��߂̃R�[���o�b�N
	virtual void Animation(){ }
	///	�c���[�̃R�s�[
	UTRef<DispNode> Copy(int suffix);
	///	Dump
	void Dump(std::ostream& os);
	/// Visible�̐ݒ�
	void SetVisible(bool b){ bVisible = b; }
	bool GetVisible(){ return bVisible; }
protected:
	///	���̃m�[�h�̃N���[���̍쐬
	virtual UTRef<DispNode> Clone(){ return new DispNode(*this); }
	friend class DispNodeBuilder;
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


#endif // _DISPNODE_H_
