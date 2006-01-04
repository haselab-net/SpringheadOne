#ifndef GLSPHERE_H
#define GLSPHERE_H

#include <Graphics/GRVisual.h>
#include <Graphics/GRSphere.h>

namespace Spr{;

class GLSphere:public GRVisual{
public:
	SGOBJECTDEF(GLSphere);
	int list;
	bool bOpaque;
	GLSphere():list(0), bOpaque(true){}
	void Set(GRSphere* gm, SGScene* s);
	virtual void Render(SGFrame* n, GRRender* r);
};

}
#endif
