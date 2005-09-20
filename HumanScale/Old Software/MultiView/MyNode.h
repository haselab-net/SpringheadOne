#ifndef	_MYNODE_H_
#define	_MYNODE_H_

#include <Base/Utilities.h>
#include <Base/VRBase.h>
#include <GL/glut.h>
#include <SceneGraph/Wrl2view/Wrl2view.h>
#include <algorithm>

using namespace LVRM;

/**	�����̃V�[���O���t���\������m�[�h�D���O����(DEF������)VRML�m�[�h
	�P�ɕt��1�m�[�h���쐬�����D*/
class MyNode : public UTRefCount{
public:
	typedef vector< UTRef<MyNode> > TChildren;
	typedef map<string,UTRef<MyNode> > TNameMap;
protected:
	///	�m�[�h��
	string	name;
	///	�e�m�[�h
	MyNode* parent;
	///	�q�m�[�h
	TChildren children;
	///	�e�m�[�h�ɑ΂��邱�̃m�[�h�̎p���D
	Affinef	posture;
	///	�Ή�����VRML�m�[�h
	UTRef<TNode> vrmlNode;
	///	OpenGL��DisplayList��ID
	int	displayList;

	///	���O�ƃm�[�h�̑Ή��\
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
	///	�Ή�����VRML�m�[�h
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	///	�e�m�[�h�ɑ΂��邱�̃m�[�h�̎p���̎擾
	Affinef GetPosture(){ return posture; }
	///	�e�m�[�h�ɑ΂��邱�̃m�[�h�̎p���̐ݒ�
	void SetPosture(Affinef a){ posture = a; }
	///	�e�m�[�h���擾
	MyNode* GetParent(){ return parent; }
	///	�q�m�[�h��ǉ�
	void AddChild(UTRef<MyNode> inNode){
		inNode->SetParent(this);
		children.push_back(inNode);
	}
	///	�q�m�[�h���폜
	void DelChild(UTRef<MyNode> inNode){
		TChildren::iterator it = find(children.begin(), children.end(), inNode);
		if (it != children.end()){
			(*it)->parent = NULL;
			children.erase(it);
		}
	}
	///	�m�[�h���ċA�I�ɕ`��
	void Draw();
	///	���O
	string Name() { return name; }
	void Name(string n) { name = n; }
	///	�c���[�̃R�s�[�Dsuffix�ɖ��O�ɂ���T�t�B�b�N�X���w��D
	UTRef<MyNode> Copy(int suffix);
protected:
	///	���̃m�[�h�̃N���[���̍쐬
	virtual UTRef<MyNode> Clone(){ return new MyNode(*this); }
	void SetParent(MyNode* n){
		if (parent) parent->DelChild(this);
		parent = n;
	}
	friend class MyNodeBuilder;
};
/**	VRML�V�[���O���t����CMyNode�̃V�[���O���t�����o���D
	VRML�V�[���O���t�̃��X�i�N���X�ɂȂ��Ă���D*/
class MyNodeBuilder:public TType::TListener, public UTRefCount{
protected:
	typedef vector<Affinef> TAffineStack;
	typedef vector< UTRef<MyNode> > TNodeStack;
	TAffineStack afStack;
	TNodeStack nodeStack;
public:
	///
	MyNodeBuilder();
	///	MyNode�V�[���O���t�̍쐬�D
	void Build(TSts* sts);
	///	�o���オ����MyNode�V�[���O���t�̃��[�g�m�[�h
	UTRef<MyNode> Root(){ return nodeStack.front(); }
	///	VRML�̃m�[�h����MyNode�����o���֐��D
	MyNode* CreateMyNode(TNode* n);
protected:
	///	���O�t���m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundNamed(TNode *pn);
	///	Transform�m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundTransformBefore(TNode *pn);
	///	Transform�m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundTransformAfter(TNode *pn);
};

#endif // _MYNODE_H_
