#ifndef TFRAME_H
#define TFRAME_H
#include <Base/utilities.h>
#include <Base/Affine.h>
#include <algorithm>
#include <vector>

namespace LVRM{

class TFrame;
class TFrames:public std::vector< UTRef<TFrame> >{
};

class TShape;
class TFrame:public UTRefCount{
protected:
	TFrame* parent;
	TFrames children;
	Affinef afParent;
	UTRef<TShape> shape;
public:
	TFrame():parent(NULL){}
	TFrame* Parent(){ return parent; }
	void AddChild(TFrame* f){
		f->DetachSelf();
		children.push_back(f);
		f->parent = this;
	}
	void DelChild(const UTRef<TFrame>& f){
		TFrames::iterator it = std::find(children.begin(), children.end(), f);
		if (it != children.end()){
			f->parent = NULL;
			children.erase(it);
		}
	}
	void DetachSelf(){
		if (parent) parent->DelChild(this);
	}
	Affinef& AfParent(){ return afParent; }
	void AfParent(const Affinef& af){ afParent = af; }
	Affinef AfWorld(){ return parent ? parent->AfWorld() * afParent : afParent; }
	UTRef<TShape>& Shape(){ return shape; }
	virtual TShape* CalcProxyPos(Vec3f& newPrx, Vec3f& normal, const Vec3f& prx, const Vec3f& dev);
	void Draw();
};


}
#endif
