#include "DispNode.h"

vector<UTRef<DispNode> > MakeDLTree()
{
	vector<UTRef<DispNode> > retroot;
	retroot.clear();

	// ���m�[�h�̓o�^�ƍ��W��DL�Z�b�g
	UTRef<DispDLNode> dnField = new DispDLNode("Field");
	retroot.push_back( &*dnField );

	// �y���M���m�[�h�̓o�^�ƍ��W�̃Z�b�g(DL�Ȃ�)
	UTRef<DispNode> dnPen = new DispNode("Penguin");
	retroot.push_back( dnPen );

	// �y���M�����m�[�h�̓o�^�ƍ��W��DL�Z�b�g
	UTRef<DispDLNode> dnPHead = new DispDLNode("PHead");
	dnPen->AddChild( &*dnPHead );

	// �y���M�����m�[�h�̓o�^�ƍ��W��DL�Z�b�g
	UTRef<DispDLNode> dnPBody = new DispDLNode("PBody");
	dnPen->AddChild( &*dnPBody );

	// �y���M�����m�[�h�̓o�^�ƍ��W��DL�Z�b�g
	UTRef<DispFootNode> dnPFoot = new DispFootNode("PFoot");
	dnPBody->AddChild( &*dnPFoot );

	return retroot;
}
