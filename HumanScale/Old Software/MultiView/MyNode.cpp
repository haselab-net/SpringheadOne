#include "MyNode.h"
#include <strstream>
using namespace LVRM;
static map<string,UTRef<MyNode> > dnMap;

///////////////////////////////////////////////////////////////
// Class Member Funcs

//----------------------------------------------------------------------------
//	MyNode
MyNode::TNameMap MyNode::nodeMap;
void MyNode::Draw(){
	glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )
	glMultMatrixf( posture );	// �ʒu�E�p����ύX����B
	glCallList(displayList);
	// �q�m�[�h��Draw���ċA�I�ɕ\������B
	for(TChildren::iterator pDN = children.begin();
	pDN != children.end(); pDN++ ){
		(*pDN)->Draw();
	}
	glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )
}

UTRef<MyNode>	MyNode::Copy(int suffix)
{
	char	strtmp[16];
	sprintf(strtmp, "_%d", suffix);
	// ���̖��̂�suffix�����ĐV�������̂ɂ���B
	string newName = Name() + strtmp;

	// �R�s�[��̐V�����m�[�h���쐬����B
	UTRef<MyNode> newNode = Clone();
	newNode->name = newName;
	nodeMap[newNode->name] = newNode;

	// �q�m�[�h��Display List���ċA�I�ɃR�s�[����B
	for(TChildren::iterator  pDN = children.begin();
		pDN != children.end(); pDN++ ){
		newNode->AddChild( (*pDN)->Copy(suffix) );
	}

	// �R�s�[��̃m�[�h��Ԃ�
	return newNode;
}

//----------------------------------------------------------------------------
//	MyNodeBuilder
MyNodeBuilder::MyNodeBuilder(){
	afStack.push_back(Affinef());
	nodeStack.push_back(new MyNode);
	nodeStack.back()->Name("root");
}
MyNode* MyNodeBuilder::CreateMyNode(TNode* n){
	TField* f = n->GetField("nodeID");
	string name;
	if (f){
		name = (*f->begin())->GetString();
	}
	MyNode* rv = new MyNode;
	rv->Name(name);
	rv->SetPosture(afStack.back());
	return rv;
}
void MyNodeBuilder::FoundTransformBefore(TNode *pn){
	afStack.push_back(afStack.back());
	Affinef& GetPosture = afStack.back();
	float		center[] = {0, 0, 0};
	float		translation[] = {0, 0, 0};
	float		rotation[] = {0, 0, 1, 0};
	float		scale[] = {1, 1, 1};
	float		scaleOrientation[] = {0, 0, 1, 0};
		
	CopyElm(pn, "center", center);
	CopyElm(pn, "translation", translation);
	CopyElm(pn, "rotation", rotation);
	CopyElm(pn, "scale", scale);
	CopyElm(pn, "scaleOrientation", scaleOrientation);
	
	GetPosture.Pos() += GetPosture.Rot() * (Vec3f&) translation;
	
	GetPosture.Pos() += GetPosture.Rot() * (Vec3f&) center;
	if(rotation[3]){
		Affinef rot(rotation[3], (Vec3f&)rotation);
		GetPosture = GetPosture * rot;
	}
	
	if(scaleOrientation[3]){
		Affinef rot(scaleOrientation[3], (Vec3f&)scaleOrientation);
		GetPosture = GetPosture * rot;
	}
	if(scale[0] != 1 || scale[1] != 1 || scale[2] != 1){
		Affinef sc;
		sc.Ex() *= scale[0];
		sc.Ey() *= scale[1];
		sc.Ez() *= scale[2];
		GetPosture = GetPosture * sc;
	}
	if(scaleOrientation[3]){
		Affinef rot(scaleOrientation[3], (Vec3f&)scaleOrientation);
		GetPosture = GetPosture * rot;
	}
	GetPosture.Pos() -= GetPosture.Rot() * (Vec3f&) center;
}
void MyNodeBuilder::FoundTransformAfter(TNode *pn){
	afStack.pop_back();
}
void MyNodeBuilder::FoundNamed(TNode *pn){
	if (strcmp(pn->GetType()->GetName(), "Transform")==0){
		FoundTransformBefore(pn);
	
		MyNode* node = CreateMyNode(pn);
		node->vrmlNode = pn;
		MyNode::nodeMap[node->Name()] = node;
		nodeStack.back()->AddChild(node);
		nodeStack.push_back(node);
		afStack.push_back(Affinef());
		pn->GetType()->Traverse(TType::EnumNamedNode, pn);
		afStack.pop_back();
		nodeStack.pop_back();

		FoundTransformAfter(pn);
	}else{
		pn->GetType()->Traverse(TType::EnumNamedNode, pn);
	}
}
namespace LVRM{
	extern GLUquadricObj* qobj;
}
void MyNodeBuilder::Build(TSts* sts){
	if (!sts) return;
	TType::listener = this;
	//	���O�t��TNode����MyNode�����D
	int num=0;
	for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
		(*it)->GetType()->EnumNamedNode(*it);
		MyNode* node = new MyNode;
		node->vrmlNode = *it;
		char name[1024];
		ostrstream(name, sizeof(name)) << "noname_" << num << '\0';
		node->Name(name);
		MyNode::nodeMap[name] = node;
		nodeStack.front()->AddChild(node);
		num ++;
	}
	//	���O�t��TNode��eTNode����؂藣���D
	for(MyNode::TNameMap::iterator it = MyNode::nodeMap.begin(); it != MyNode::nodeMap.end(); ++it){
		TNode* cur = it->second->VrmlNode();
		TNode* parent = cur->GetParent();
		if (parent){
			TField* field = parent->GetField("children");
			if (field){
				for(TField::iterator it = field->begin(); it != field->end(); ++it){
					TSts* sts = (*it)->GetSts();
					for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
						if (*it == cur){
							it = sts->erase(it);
						break;
						}
					}
				}
			}
		}
	}
	//	DrawList�����D
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricTexture(qobj, GL_TRUE);
	static int nDisplayList = 0;
	for(MyNode::TNameMap::iterator it = MyNode::nodeMap.begin(); it != MyNode::nodeMap.end(); ++it){
		glNewList(++nDisplayList, GL_COMPILE);
		TNode* node = it->second->VrmlNode();
		if (strcmp(node->GetType()->GetName(), "Transform") == 0){
			node->GetType()->Traverse(TType::Draw, node);
		}else{
			node->GetType()->Draw(node);
		}
		glEndList();
		it->second->displayList = nDisplayList;
	}
	gluDeleteQuadric(qobj);
}
