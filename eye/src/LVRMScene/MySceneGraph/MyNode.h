#ifndef	_TMyNode_H_
#define	_TMyNode_H_

#include <Base/BaseUtility.h>
#define GLUT_NO_LIB_PRAGMA
#include <GL/glut.h>
#include <LVRMScene/Wrl2view/Wrl2view.h>
#include <algorithm>

using namespace Spr;

/**	�����̃V�[���O���t���\������m�[�h�D���O����(DEF������)VRML�m�[�h
	�P�ɕt��1�m�[�h���쐬�����D*/
class TMyNode : public UTRefCount{
public:
	typedef vector<Vec3f> TVertices;
	typedef vector<int> TFace;
	typedef vector< TFace > TFaces;
protected:
	///	�m�[�h��
	string name;
	///	���̃m�[�h�̎p���D
	Affinef	posture;
	///	�Ή�����VRML�m�[�h
	UTRef<TNode> vrmlNode;
	///	OpenGL��DisplayList��ID
	int	displayList;
	///	swift �� ObjectID
	int swiftObjectID;
	///	decomposer ���o�͂����t�@�C���̖��O
	string decompFile;
	///	decomposer �ɓ��͂���t�@�C���̖��O
	string polyFile;

public:
	///	BBox
	float radius;
	float depth;
	void CalcZ(const Affinef& afView);
	///	�S�Ă�TMyNode�����Ă������ꕨ
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
	///	�Ή�����VRML�m�[�h
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	///	�e�m�[�h�ɑ΂��邱�̃m�[�h�̎p���̎擾
	Affinef GetPosture(){ return posture; }
	///	�e�m�[�h�ɑ΂��邱�̃m�[�h�̎p���̐ݒ�
	void SetPosture(Affinef a){ posture = a; }
	///	�m�[�h��`��
	void Draw();
	void DrawGeometry();
	///	���O
	string Name() { return name; }
	void Name(string n) { name = n; }

	friend class TMyNodeBuilder;
};
class TMyNodes: public vector< UTRef<TMyNode> >{
public:
	void ZSort(const Affinef& afView);
	void Draw();
};

/**	VRML�V�[���O���t����CTMyNode�̃V�[���O���t�����o���D
	VRML�V�[���O���t�̃��X�i�N���X�ɂȂ��Ă���D*/
class TMyNodeBuilder:public TType::TListener, public UTRefCount{
protected:
	typedef vector<Affinef> TAffineStack;
	typedef vector< UTRef<TMyNode> > TNodeStack;
	TAffineStack afStack;
	TMyNode* curNode;
public:
	///	���������m�[�h	
	TMyNodes allNodes;
	///
	Vec3f clearColor;
	///
	Affinef viewPoint;
	///
	Affinef projection;
	///
	TMyNodeBuilder();
	///	TMyNode�V�[���O���t�̍쐬�D
	void Build(TSts* sts);
	///	VRML�̃m�[�h����TMyNode�����o���֐��D
	TMyNode* CreateTMyNode(TNode* n);
protected:
	///	���O�t���m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundNamed(TNode *pn);
	///	���ł��R�[���o�b�N
	bool FoundTypeNode(TNode *pn);
	///	Transform�m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundTransformBefore(TNode *pn);
	///	Transform�m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundTransformAfter(TNode *pn);
};

#endif // _TMyNode_H_
