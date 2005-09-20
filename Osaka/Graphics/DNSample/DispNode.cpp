#include "DispNode.h"

static map<string,UTRef<DispNode> > dnMap;

///////////////////////////////////////////////////////////////
// Class Member Funcs

DispNode::DispNode(string nodeID)
{
	nodeChildren.clear();			// �q�m�[�h�̏�����
	bCopyNode = false;				// ���̃R���X�g���N�^�͐V�K�쐬�p
	nTarg = NodeIDMap()[nodeID];	// �^�[�Q�b�g��TNode���Z�b�g
	strNodeName = nodeID;			// �m�[�h�����Z�b�g
	dnMap.insert(make_pair(nodeID, this));		// �m�[�h����\�ɃZ�b�g

	// ���W�̎擾
	TNode* nParent = nTarg->GetParent();
	while( strcmp( nParent->GetType()->GetName(), "Transform" ) != 0 ){
		nParent = nParent->GetParent();
	}
	float ftread[3];
	CopyElm( nParent, "translation", ftread );
	pos.Pos() = (Vec3f) ftread;		// ���W�̑��

	// ���ȃm�[�h��VRML�̖؂���؂�
	nParent = nTarg->GetParent();
	TField* f = nParent->GetField("children");
	for(TField::iterator it = f->begin(); it != f->end(); ++it){
		TSts* sts = ((TFieldBaseSts*)&**it)->GetSts();
		for(TSts::iterator itn = sts->begin(); itn != sts->end(); ++itn){
			if (*itn == nTarg){
				itn = sts->erase(itn);
				break;
			}
		}
	}
}

DispNode::DispNode(string nNameTarg, UTRef<DispNode> orgNode )
{
	nodeChildren.clear();			// �q�m�[�h��CopyNode�ŃZ�b�g
	bCopyNode = true;				// ���̃R���X�g���N�^�̓R�s�[�p
	nTarg = orgNode->GetTNode();	// VRML�̖؂ł͓����m�[�h���w��
	strNodeName = nNameTarg;		// �m�[�h�����Z�b�g(�R�s�[�於��)
	dnMap.insert(make_pair(nNameTarg, this));		// �m�[�h����\�ɃZ�b�g
	pos = orgNode->pos;				// ���W�E�p���̓R�s�[���Ɠ���
}

bool DispNode::DeleteCopy()
{
	// �R�s�[�m�[�h�ȊO�͏����Ȃ��B(�G���[)
	if( !IsCopy() ){
		std::cerr << "Node \"" << GetNodeName() << "\" is Not Copy!!" << std::endl;
		return false;
	}

	map<string,UTRef<DispNode> >::iterator pdnMap;
	pdnMap = dnMap.find(GetNodeName());	// �\�̃m�[�h����T���B
	if( pdnMap == dnMap.end() ){
		std::cerr << "dnMap Err!!  Name = " << GetNodeName() << std::endl;
		return false;
	} else {
		dnMap.erase( pdnMap );			// �\�̃m�[�h���������B
#ifdef _debug_disp_
		std::cout << "erase  Name = " << GetNodeName() << std::endl;
#endif
	}

	// �q�m�[�h��Draw���ċA�I�ɏ����B
	vector<UTRef<DispNode> >::iterator pDN;
	for( pDN = nodeChildren.begin();
			pDN != nodeChildren.end(); pDN++ ){
		if((*pDN)->DeleteCopy()) {
			// �q�m�[�h������
//			delete (*pDN);
			// �q�m�[�h�̃|�C���^������
//			pDN = nodeChildren.erase( pDN );
		} else {
			return false;	// �q�m�[�h���G���[�Ȃ�e���G���[
		}
	}

	return true;
}

void DispNode::MakeDL()
{
	MakeDLBase();		// ������Display List�̍쐬����B

	// �q�m�[�h��Display List���ċA�I�ɍ쐬����B
	vector<UTRef<DispNode> >::iterator pDN;
	for( pDN = nodeChildren.begin();
		pDN != nodeChildren.end(); pDN++ ){
		(*pDN)->MakeDL();
	}
}

void DispNode::Draw()
{
	glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )

	Animation();	// �A�j���[�V����������

	glMultMatrixf( pos );	// �ʒu�E�p����ύX����B

	DispDL();	// DisplayList��\������B

	// �q�m�[�h��Draw���ċA�I�ɕ\������B
	vector<UTRef<DispNode> >::iterator pDN;
	for( pDN = nodeChildren.begin();
		pDN != nodeChildren.end(); pDN++ ){
		(*pDN)->Draw();
	}

	glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )
}

UTRef<DispNode>	DispNode::CopyNode( int suffix )
{
	char	strtmp[16];
	sprintf( strtmp, "_%d", suffix );
	// ���̖��̂�suffix�����ĐV�������̂ɂ���B
	string targName = GetNodeName() + strtmp;

	// �R�s�[��̐V�����m�[�h���쐬����B
	UTRef<DispNode> newNode = CreateNode(targName);

	// �q�m�[�h��Display List���ċA�I�ɃR�s�[(CopyNode)����B
	vector<UTRef<DispNode> >::iterator pDN;
	for( pDN = nodeChildren.begin();
		pDN != nodeChildren.end(); pDN++ ){
		newNode->SetChildren( (*pDN)->CopyNode(suffix) );
	}

	// �R�s�[��̃m�[�h��Ԃ�
	return newNode;
}

//bool DispNode::GetRecPos(string strNode, Affinef *posOut)
//{
//	if( strNodeName == strNode ){
//		// ���͂Ɩ��̂���v�����Ȃ�
//		posOut = &pos;			// �ʒu�E�p���̊i�[�ꏊ���o��
//		return true;
//	} else {
//		// ���͂Ɩ��̂���v���Ȃ��Ƃ�
//		// �q�m�[�h��T���A��v������true�A��v���Ȃ��Ȃ�false
//		vector<UTRef<DispNode> >::iterator pDN;
//		for( pDN = nodeChildren.begin();
//			pDN != nodeChildren.end(); pDN++ ){
//			if( (*pDN)->GetRecPos( strNode, posOut ) )
//				return true;
//		}
//	}
//	return false;
//}

void DispDLNode::MakeDLBase()
{
	// Display List�̍쐬
	UTRef<TSts> stsTarg = new TSts;
	stsTarg->push_back( GetTNode() );
	nDisplayList = DrawList( stsTarg );
}

void DispDLNode::DispDL()
{
	glCallList( nDisplayList );	// �o�^����Ă���DisplayList��\��
}

void DispFootNode::Animation()
{
	static	Affinef	rotAf(Rad( 45 ), 'x');
	// �Ƃ肠����X������45�x�̉�]�^��

	pos = rotAf * pos;
}

///////////////////////////////////////////////////////////////
// Grobal Funcs

void	MakeDLs( vector<UTRef<DispNode> > root )
{
	vector<UTRef<DispNode> >::iterator itdnTree;
	for( itdnTree = root.begin();
			itdnTree != root.end(); itdnTree++ ){
		(*itdnTree)->MakeDL();
	}
}

void	DrawDLs( vector<UTRef<DispNode> > root )
{
	vector<UTRef<DispNode> >::iterator itdnTree;
	for( itdnTree = root.begin();
			itdnTree != root.end(); itdnTree++ ){
		(*itdnTree)->Draw();
	}
}

map<string,UTRef<DispNode> >& GetNodeMap()
{
	return dnMap;
}
