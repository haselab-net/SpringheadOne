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
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf( posture );	// 位置・姿勢を変更する。
	glCallList(displayList);
	// 子ノードのDrawを再帰的に表示する。
	for(TChildren::iterator pDN = children.begin();
	pDN != children.end(); pDN++ ){
		(*pDN)->Draw();
	}
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
}

UTRef<MyNode>	MyNode::Copy(int suffix)
{
	char	strtmp[16];
	sprintf(strtmp, "_%d", suffix);
	// 元の名称にsuffixをつけて新しい名称にする。
	string newName = Name() + strtmp;

	// コピー先の新しいノードを作成する。
	UTRef<MyNode> newNode = Clone();
	newNode->name = newName;
	nodeMap[newNode->name] = newNode;

	// 子ノードのDisplay Listを再帰的にコピーする。
	for(TChildren::iterator  pDN = children.begin();
		pDN != children.end(); pDN++ ){
		newNode->AddChild( (*pDN)->Copy(suffix) );
	}

	// コピー先のノードを返す
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
	//	名前付きTNodeからMyNodeを作る．
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
	//	名前付きTNodeを親TNodeから切り離す．
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
	//	DrawListを作る．
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
