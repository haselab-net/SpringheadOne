#ifndef GLMESH_H
#define GLMESH_H

#include <Graphics/GRMesh.h>

namespace Spr{;

class GLMesh:public GRVisual{
public:
	SGOBJECTDEF(GLMesh);
	UTRef<GRMesh> gm;
	unsigned listOpaque;
	unsigned listTransparent;
	GLMesh():listOpaque(0), listTransparent(0){}
	void Set(GRMesh* gm, SGScene* s);
	virtual void Render(SGFrame* n, GRRender* r);
};

}
#endif
