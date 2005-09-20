#ifndef	_GEONODE_H_
#define	_GEONODE_H_

#include <Base/Utilities.h>
#include <Base/VRBase.h>
#include <GL/glut.h>
#include <SceneGraph/Wrl2view/Wrl2view.h>
#include <algorithm>

using namespace LVRM;

/**	�����̃V�[���O���t���\������m�[�h�D���O����(DEF������)VRML�m�[�h
	�P�ɕt��1�m�[�h���쐬�����D*/
class TGeoNode : public UTRefCount{
public:
	typedef vector< UTRef<TGeoNode> > TGeoNodes;
	typedef map<string, UTRef<TGeoNode> > TNameMap;
	typedef vector<Vec3f> TVertices;
	typedef vector< vector<int> > TFaces;
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
	///	�S�Ă�TGeoNode�����Ă������ꕨ
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
	///	�Ή�����VRML�m�[�h
	UTRef<TNode> VrmlNode(){ return vrmlNode; }
	///	�e�m�[�h�ɑ΂��邱�̃m�[�h�̎p���̎擾
	Affinef GetPosture(){ return posture; }
	///	�e�m�[�h�ɑ΂��邱�̃m�[�h�̎p���̐ݒ�
	void SetPosture(Affinef a){ posture = a; }
	///	�m�[�h��`��
	void Draw();
	void DrawGeo();
	///	���O
	string Name() { return name; }
	void Name(string n) { name = n; }

	friend class TGeoNodeBuilder;
};
/**	VRML�V�[���O���t����CTGeoNode�̃V�[���O���t�����o���D
	VRML�V�[���O���t�̃��X�i�N���X�ɂȂ��Ă���D*/
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
	///	TGeoNode�V�[���O���t�̍쐬�D
	void Build(TSts* sts);
	///	�o���オ����TGeoNode�V�[���O���t�̃��[�g�m�[�h
	UTRef<TGeoNode> Root(){ return nodeStack.front(); }
	///	VRML�̃m�[�h����TGeoNode�����o���֐��D
	TGeoNode* CreateTGeoNode(TNode* n);
protected:
	///	���O�t���m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundNamed(TNode *pn);
	///	Transform�m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundTransformBefore(TNode *pn);
	///	Transform�m�[�h�����������Ƃ��̃R�[���o�b�N
	virtual void FoundTransformAfter(TNode *pn);
	///	EnumTypeNodes�Ŗ���Ăяo�����R�[���o�b�N�Dtrue��Ԃ��Ǝq�m�[�h���T���D
	virtual bool FoundTypeNode(TNode *pn);
};

#endif // _GEONODE_H
