#ifndef	_DISPNODE_H_
#define	_DISPNODE_H_

#include <Base/Utilities.h>
#include <Base/VRBase.h>
#include <GL/glut.h>
#include <SceneGraph/libWrl2view/libWrl2view.h>

#define	_debug_disp_

///////////////////////////////////////////////////////////////
// Class Define

class DispNode : public UTRefCount
{
	string				strNodeName;
	bool				bCopyNode;
	UTRef<TNode>		nTarg;
	vector<UTRef<DispNode> >	nodeChildren;

	public:
		Affinef				pos;
		DispNode(string nodeID);
		DispNode(string nNameTarg, UTRef<DispNode> orgNode );
#ifdef _debug_disp_
		~DispNode(){
			std::cout << "Delete Node " << GetNodeName() << std::endl;
		}
#endif
		UTRef<TNode>	GetTNode() { return nTarg; };
		void		SetChildren(UTRef<DispNode> inNode){
						nodeChildren.push_back( inNode );
					}
		void		Draw();
		void		MakeDL();
		string			GetNodeName() { return strNodeName; }
		bool		IsCopy(){ return bCopyNode; }
		bool		DeleteCopy();
		UTRef<DispNode>	CopyNode( int suffix );
		virtual UTRef<DispNode> CreateNode( string nNameTarg )
			{ return ( new DispNode( nNameTarg, this ) ); }
//		bool			GetRecPos( string strNode, Affinef *posOut );
		virtual void	MakeDLBase() { }
		virtual void	DispDL(){ }
		virtual void	Animation(){ }
};

class DispDLNode : public DispNode
{
	int		nDisplayList;

	public:
		DispDLNode(string nodeID) : DispNode(nodeID) { }
		DispDLNode(string nNameTarg, UTRef<DispDLNode> orgNode ) : DispNode(nNameTarg, &*orgNode)
			{ nDisplayList = orgNode->GetDL(); }
		int				GetDL() { return nDisplayList; }
		virtual UTRef<DispNode> CreateNode( string nNameTarg )
			{ return ( new DispDLNode( nNameTarg, this ) ); }
		virtual void	MakeDLBase();
		virtual void	DispDL();
};

class DispFootNode : public DispDLNode
{
	public:
		DispFootNode(string nodeID) : DispDLNode(nodeID) { }
		DispFootNode(string nNameTarg, UTRef<DispFootNode> orgNode ) : DispDLNode(nNameTarg, &*orgNode) { }
		virtual UTRef<DispNode> CreateNode( string nNameTarg )
			{ return ( new DispFootNode( nNameTarg, this ) ); }
		virtual void	Animation();
};

///////////////////////////////////////////////////////////////
// Global Funcs extern

extern void	MakeDLs( vector<UTRef<DispNode> > root );
extern void	DrawDLs( vector<UTRef<DispNode> > root );
extern bool	GetRecPoss( vector<UTRef<DispNode> > root, string strNode, Affinef *posOut );
extern map<string,UTRef<DispNode> >& GetNodeMap( void );

#endif // _DISPNODE_H_
