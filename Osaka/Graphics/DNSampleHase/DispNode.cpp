#include "DispNode.h"
using namespace std;

///////////////////////////////////////////////////////////////
// Class Member Funcs

//----------------------------------------------------------------------------
//	DispNode
DispNode::TNameMap DispNode::nodeMap;
Affinef DispNode::GetGlobalPosture(){
	Affinef currentPosture;
	DispNode* character = this;
	while(character){
		currentPosture = character->GetPosture() * currentPosture;
		//cout << currentPosture << endl;
		character = character->GetParent();
	}
	return currentPosture;
}
void DispNode::SetGtoLPosture(Affinef globalPosture){
	vector<DispNode*> character;
	int i = 0;
	Affinef parentPosture;
	character.push_back(this);
	while(character.at(i)){
		character.push_back(character.at(i)->GetParent());
		i++;
	}
	i --;
	while(i>1){
		i--;
		parentPosture = character.at(i)->GetPosture();
		parentPosture.Rot() = parentPosture.Rot().inv();
		parentPosture.Pos() *= -1;
		globalPosture = parentPosture * globalPosture;
	}
	this->SetPosture(globalPosture);
}

void DispNode::Draw(){
	glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )
	Animation();	// �A�j���[�V����������
	glMultMatrixf( posture );	// �ʒu�E�p����ύX����B
	glCallList(displayList);
	// �q�m�[�h��Draw���ċA�I�ɕ\������B
	for(TChildren::iterator pDN = nodeChildren.begin();
		pDN != nodeChildren.end(); pDN++ ){
		(*pDN)->Draw();
	}

	glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )
}

UTRef<DispNode>	DispNode::Copy(int suffix)
{
	char	strtmp[16];
	sprintf(strtmp, "_%d", suffix);
	// ���̖��̂�suffix�����ĐV�������̂ɂ���B
	string newName = Name() + strtmp;

	// �R�s�[��̐V�����m�[�h���쐬����B
	UTRef<DispNode> newNode = Clone();
	newNode->name = newName;
	nodeMap[newNode->name] = newNode;

	// �q�m�[�h��Display List���ċA�I�ɃR�s�[(CopyNode)����B
	for(TChildren::iterator  pDN = nodeChildren.begin();
		pDN != nodeChildren.end(); pDN++ ){
		newNode->AddChild( (*pDN)->Copy(suffix) );
	}

	// �R�s�[��̃m�[�h��Ԃ�
	return newNode;
}

//----------------------------------------------------------------------------
//	DispFootNode
void DispFootNode::Animation(){
	static	Affinef	rotAf(Rad( 45 ), 'x');
	// �Ƃ肠����X������45�x�̉�]�^��
	posture = rotAf * posture;
}

//----------------------------------------------------------------------------
//	DispNodeBuilder
DispNodeBuilder::DispNodeBuilder(){
	afStack.push_back(Affinef());
	nodeStack.push_back(new DispNode);
	nodeStack.back()->Name("root");
}
DispNode* DispNodeBuilder::CreateDispNode(TNode* n){
	TField* f = n->GetField("nodeID");
	string name = (*f->begin())->GetString();
	DispNode* rv = new DispNode;
	rv->Name(name);
	rv->SetPosture(afStack.back());
	return rv;
}
void DispNodeBuilder::FoundTransformBefore(TNode *pn){
	afStack.push_back(afStack.back());
	Affinef& posture = afStack.back();
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
	
	posture.Pos() += posture.Rot() * (Vec3f&) translation;
	
	posture.Pos() += posture.Rot() * (Vec3f&) center;
	if(rotation[3]){
		Affinef rot(rotation[3], (Vec3f&)rotation);
		posture = posture * rot;
	}
	
	if(scaleOrientation[3]){
		Affinef rot(scaleOrientation[3], (Vec3f&)scaleOrientation);
		posture = posture * rot;
	}
	if(scale[0] != 1 || scale[1] != 1 || scale[2] != 1){
		Affinef sc;
		sc.Ex() *= scale[0];
		sc.Ey() *= scale[1];
		sc.Ez() *= scale[2];
		posture = posture * sc;
	}
	if(scaleOrientation[3]){
		Affinef rot(scaleOrientation[3], (Vec3f&)scaleOrientation);
		posture = posture * rot;
	}
	posture.Pos() -= posture.Rot() * (Vec3f&) center;
}
void DispNodeBuilder::FoundViewPoint(TNode *pn){
}
void DispNodeBuilder::FoundTransformAfter(TNode *pn){
	afStack.pop_back();
}
void DispNodeBuilder::FoundNamed(TNode *pn){
	FoundTransformBefore(pn);
	
	DispNode* node = CreateDispNode(pn);
	node->vrmlNode = pn;
	DispNode::nodeMap[node->Name()] = node;
	nodeStack.back()->AddChild(node);
	nodeStack.push_back(node);
	afStack.push_back(Affinef());
	pn->GetType()->Traverse(TType::EnumNamedNode, pn);
	afStack.pop_back();
	nodeStack.pop_back();

	FoundTransformAfter(pn);
}
extern GLUquadricObj* qobj;
void DispNodeBuilder::Build(TSts* sts){
	TType::listener = this;
	//	���O�t��TNode����DispNode�����D
	for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
		(*it)->GetType()->EnumNamedNode(*it);
	}
	TType::listener = NULL;
	//	���O�t��TNode��eTNode����؂藣���D
	for(DispNode::TNameMap::iterator it = DispNode::nodeMap.begin(); it != DispNode::nodeMap.end(); ++it){
		string itName = it->first;
		while (!it->second) it = DispNode::nodeMap.erase(it);
		TNode* cur = it->second->VrmlNode();
		TNode* parent = cur->GetParent();
		if (parent){
			TField* field = parent->GetField("children");
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
	//	DrawList�����D
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricTexture(qobj, GL_TRUE);
	static int nDisplayList = 0;
	for(DispNode::TNameMap::iterator it = DispNode::nodeMap.begin(); it != DispNode::nodeMap.end(); ++it){
		glNewList(++nDisplayList, GL_COMPILE);
		TNode* node = it->second->VrmlNode();
//		node->GetType()->Draw(node);
//	�������́C
		if (strcmp(node->GetType()->GetName(), "Transform") ==0){
			//	Transform�̉e�����Ȃ����D
			node->GetType()->Traverse(TType::Draw, node);
		}else{
			//	Shape�������炱�̃m�[�h��\�����Ȃ����
			//	�ׂ�Ȃ��D
			node->GetType()->Draw(node);
		}
		glEndList();
		it->second->displayList = nDisplayList;
	}
	gluDeleteQuadric(qobj);
}
void DispNode::Dump(std::ostream& os){
	int w = os.width();
	for(int i=0; i<w; i++) os << " ";
	os << name << " - " << posture.Pos() << std::endl;
//	os << posture;
	os.width(w+2);
	for(TChildren::iterator it = nodeChildren.begin(); it != nodeChildren.end(); ++it){
		(*it)->Dump(os);
	}
	os.width(w);
}
