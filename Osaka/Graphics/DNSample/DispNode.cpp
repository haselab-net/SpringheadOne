#include "DispNode.h"

static map<string,UTRef<DispNode> > dnMap;

///////////////////////////////////////////////////////////////
// Class Member Funcs

DispNode::DispNode(string nodeID)
{
	nodeChildren.clear();			// 子ノードの初期化
	bCopyNode = false;				// このコンストラクタは新規作成用
	nTarg = NodeIDMap()[nodeID];	// ターゲットのTNodeをセット
	strNodeName = nodeID;			// ノード名をセット
	dnMap.insert(make_pair(nodeID, this));		// ノード名を表にセット

	// 座標の取得
	TNode* nParent = nTarg->GetParent();
	while( strcmp( nParent->GetType()->GetName(), "Transform" ) != 0 ){
		nParent = nParent->GetParent();
	}
	float ftread[3];
	CopyElm( nParent, "translation", ftread );
	pos.Pos() = (Vec3f) ftread;		// 座標の代入

	// 自己ノードをVRMLの木から切る
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
	nodeChildren.clear();			// 子ノードはCopyNodeでセット
	bCopyNode = true;				// このコンストラクタはコピー用
	nTarg = orgNode->GetTNode();	// VRMLの木では同じノードを指す
	strNodeName = nNameTarg;		// ノード名をセット(コピー先名称)
	dnMap.insert(make_pair(nNameTarg, this));		// ノード名を表にセット
	pos = orgNode->pos;				// 座標・姿勢はコピー元と同じ
}

bool DispNode::DeleteCopy()
{
	// コピーノード以外は消せない。(エラー)
	if( !IsCopy() ){
		std::cerr << "Node \"" << GetNodeName() << "\" is Not Copy!!" << std::endl;
		return false;
	}

	map<string,UTRef<DispNode> >::iterator pdnMap;
	pdnMap = dnMap.find(GetNodeName());	// 表のノード名を探す。
	if( pdnMap == dnMap.end() ){
		std::cerr << "dnMap Err!!  Name = " << GetNodeName() << std::endl;
		return false;
	} else {
		dnMap.erase( pdnMap );			// 表のノード名を消す。
#ifdef _debug_disp_
		std::cout << "erase  Name = " << GetNodeName() << std::endl;
#endif
	}

	// 子ノードのDrawを再帰的に消す。
	vector<UTRef<DispNode> >::iterator pDN;
	for( pDN = nodeChildren.begin();
			pDN != nodeChildren.end(); pDN++ ){
		if((*pDN)->DeleteCopy()) {
			// 子ノードを消す
//			delete (*pDN);
			// 子ノードのポインタも消す
//			pDN = nodeChildren.erase( pDN );
		} else {
			return false;	// 子ノードがエラーなら親もエラー
		}
	}

	return true;
}

void DispNode::MakeDL()
{
	MakeDLBase();		// 自分のDisplay Listの作成する。

	// 子ノードのDisplay Listを再帰的に作成する。
	vector<UTRef<DispNode> >::iterator pDN;
	for( pDN = nodeChildren.begin();
		pDN != nodeChildren.end(); pDN++ ){
		(*pDN)->MakeDL();
	}
}

void DispNode::Draw()
{
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )

	Animation();	// アニメーションをする

	glMultMatrixf( pos );	// 位置・姿勢を変更する。

	DispDL();	// DisplayListを表示する。

	// 子ノードのDrawを再帰的に表示する。
	vector<UTRef<DispNode> >::iterator pDN;
	for( pDN = nodeChildren.begin();
		pDN != nodeChildren.end(); pDN++ ){
		(*pDN)->Draw();
	}

	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
}

UTRef<DispNode>	DispNode::CopyNode( int suffix )
{
	char	strtmp[16];
	sprintf( strtmp, "_%d", suffix );
	// 元の名称にsuffixをつけて新しい名称にする。
	string targName = GetNodeName() + strtmp;

	// コピー先の新しいノードを作成する。
	UTRef<DispNode> newNode = CreateNode(targName);

	// 子ノードのDisplay Listを再帰的にコピー(CopyNode)する。
	vector<UTRef<DispNode> >::iterator pDN;
	for( pDN = nodeChildren.begin();
		pDN != nodeChildren.end(); pDN++ ){
		newNode->SetChildren( (*pDN)->CopyNode(suffix) );
	}

	// コピー先のノードを返す
	return newNode;
}

//bool DispNode::GetRecPos(string strNode, Affinef *posOut)
//{
//	if( strNodeName == strNode ){
//		// 入力と名称が一致したなら
//		posOut = &pos;			// 位置・姿勢の格納場所を出力
//		return true;
//	} else {
//		// 入力と名称が一致しないとき
//		// 子ノードを探し、一致したらtrue、一致しないならfalse
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
	// Display Listの作成
	UTRef<TSts> stsTarg = new TSts;
	stsTarg->push_back( GetTNode() );
	nDisplayList = DrawList( stsTarg );
}

void DispDLNode::DispDL()
{
	glCallList( nDisplayList );	// 登録されているDisplayListを表示
}

void DispFootNode::Animation()
{
	static	Affinef	rotAf(Rad( 45 ), 'x');
	// とりあえずX軸方向45度の回転運動

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
