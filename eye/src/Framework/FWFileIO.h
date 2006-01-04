#ifndef FWFILEIO_H
#define FWFILEIO_H

namespace Spr{;

class SGScene;
class SGObject;
///	
class FWFileIO{
public:
	virtual bool Load(const char* fn, SGScene* scene, SGObject* obj=0){ return false; }
	virtual bool Save(const char* fn, SGScene* scene, SGObject* obj=0){ return false; }
};

}


#endif
