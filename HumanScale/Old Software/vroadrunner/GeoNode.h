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

/**	�����̃V�[���O���t���\������m�[�h�D���O����(DEF������)VRML�m�[�h
	�P�ɕt��1�m�[�h���쐬�����D*/
class TGeoNode : public UTRefCount{
public:
	typedef vector<Vec3f> TVertices;
	typedef vector<int> TFace;
	typedef vector< TFace > TFaces;
protected:
	///	�m�[�h��
	string name;
	///	���̃m�[�h�̎p��
	Affinef	posture;
	///	�Ή�����VRML�m�[�h
	UTRef<TNode> vrmlNode;
	/// �e�m�[�h
	UTRef<TGeoNode> parent;
	///	OpenGL��DisplayList��ID
	int	displayList;
	///	swift �� ObjectID
	int swiftObjectID;
	///	decomposer ���o�͂����t�@�C���̖��O
	string decompFile;
	///	decomposer �ɓ��͂���t�@�C���̖��O
	string polyFile;
public:
	virtual void InitInstance();
	// �A�o�^�[�̈ʒu�̃|�C���^
	//Affinef *p_afBody;
	TGLView* p_TGLView;

	///	�S�Ă�TGeoNode�����Ă������ꕨ
	TVertices vertices;
	TFaces faces;
	
	///	SOLID�̃I�u�W�F�N�g�ւ̃|�C���^
	Object* solid;

	//  �I�u�W�F�N�g���͂ރo�E���f�B���O�{�b�N�X
	Vec4f VecObjectBox[4];
	//  �I�u�W�F�N�g��ID
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
	///	�Ή�����VRML�m�[�h
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	///	�e�m�[�h�ɑ΂��邱�̃m�[�h�̎p���̎擾
	Affinef GetPosture(){ return posture; }
	///	�e�m�[�h�ɑ΂��邱�̃m�[�h�̎p���̐ݒ�
	void SetPosture(Affinef a){ posture = a; }
	/// �I�u�W�F�N�g�̃��[���h���W���擾
	Affinef GetGlobalPosture(){
		if (parent) return parent->GetGlobalPosture() * posture;
		return posture;
	}
	///	�m�[�h��`��
	virtual void Draw();
	void DrawGeometry();
	///	���O
	string Name() { return name; }
	void Name(string n) { name = n; }

	friend class TGeoNodeBuilder;
};
typedef vector< UTRef<TGeoNode> > TGeoNodes;
/**	VRML�V�[���O���t����CTGeoNode�̃V�[���O���t�����o���D
	VRML�V�[���O���t�̃��X�i�N���X�ɂȂ��Ă���D*/
class TGeoNodeBuilder:public TType::TListener, public UTRefCount{
protected:
	typedef vector<Affinef> TAffineStack;
	typedef vector< UTRef<TGeoNode> > TNodeStack;
	TAffineStack afStack;
	TGeoNode* curNode;
public:
	bool bDontDisplay;
	///	���������m�[�h	
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
	///	TGeoNode�V�[���O���t�̍쐬�D
	void Build(TSts* sts);
	///	VRML�̃m�[�h����TGeoNode�����o���֐��D
	TGeoNode* CreateTGeoNode(TNode* n);
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

#endif // _TGeoNode_H_

