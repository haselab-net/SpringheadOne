#include "DispNode.h"

vector<UTRef<DispNode> > MakeDLTree()
{
	vector<UTRef<DispNode> > retroot;
	retroot.clear();

	// 床ノードの登録と座標とDLセット
	UTRef<DispDLNode> dnField = new DispDLNode("Field");
	retroot.push_back( &*dnField );

	// ペンギンノードの登録と座標のセット(DLなし)
	UTRef<DispNode> dnPen = new DispNode("Penguin");
	retroot.push_back( dnPen );

	// ペンギン頭ノードの登録と座標とDLセット
	UTRef<DispDLNode> dnPHead = new DispDLNode("PHead");
	dnPen->AddChild( &*dnPHead );

	// ペンギン胴ノードの登録と座標とDLセット
	UTRef<DispDLNode> dnPBody = new DispDLNode("PBody");
	dnPen->AddChild( &*dnPBody );

	// ペンギン足ノードの登録と座標とDLセット
	UTRef<DispFootNode> dnPFoot = new DispFootNode("PFoot");
	dnPBody->AddChild( &*dnPFoot );

	return retroot;
}
