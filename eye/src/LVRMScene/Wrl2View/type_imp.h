// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "type.h"
namespace Spr {


////////////////////////////////////////////////////////////////////////////////
//	Node types

class TTypeTransform : public TTypeBase {
 public:
	TTypeTransform(const char *name) : TTypeBase(name) {}
	void Draw(TNode *pn);
	void Init(TNode *pn);
	void EnumNamedNode(TNode *pn);
	void EnumTypeNode(TNode *pn);
};

class TTypeShape : public TTypeBase {
 public:
	TTypeShape(const char *name) : TTypeBase(name) {}
	void Draw(TNode *pn);
};

class TTypeBox : public TTypeBase {
 public:
	TTypeBox(const char *name) : TTypeBase(name) {}
	void Draw(TNode *pn);
};

class TTypeSphere : public TTypeBase {
 public:
	TTypeSphere(const char *name) : TTypeBase(name) {}
	void Draw(TNode *pn);
};

class TTypeCylinder : public TTypeBase {
 public:
	TTypeCylinder(const char *name) : TTypeBase(name) {}
	void Draw(TNode *pn);
};

class TTypeCone : public TTypeBase {
 public:
	TTypeCone(const char *name) : TTypeBase(name) {}
	void Draw(TNode *pn);
};

class TTypeIndexedFaceSet : public TTypeBase {
 public:
	TTypeIndexedFaceSet(const char *name) : TTypeBase(name) {}
	void Draw(TNode *pn);
};

class TTypeElevationGrid : public TTypeBase {
 public:
	TTypeElevationGrid(const char *name) : TTypeBase(name) {}
	void Draw(TNode *pn);
};

class TTypeTextureTransform : public TTypeBase {
 public:
	TTypeTextureTransform(const char *name) : TTypeBase(name) {}
	void Draw(TNode *pn);
};

class TTypeMaterial : public TTypeBase {
 public:
	TTypeMaterial(const char *name) : TTypeBase(name) {}
	void Draw(TNode *pn);
};

class TTypePointLight : public TTypeBase {
 public:
	TTypePointLight(const char *name) : TTypeBase(name) {}
	void Draw(TNode *pn);
};


class TTypeBackground : public TTypeBase {
 public:
	TTypeBackground(const char *name) : TTypeBase(name) {}
	void Init(TNode *pn);
};

class TTypeViewpoint : public TTypeBase {
 public:
	TTypeViewpoint(const char *name) : TTypeBase(name) {}
	void Init(TNode *pn);
};

class TTypeInline : public TTypeBase {
 public:
	TTypeInline(const char *name) : TTypeBase(name) {}
	void Init(TNode *pn);
};


class TTypePixelTexture : public TTypeBase {
 public:
	TTypePixelTexture(const char *name) : TTypeBase(name) {}
	void Init(TNode *pn);
	void Draw(TNode *pn);
};

class TTypeImageTexture : public TTypeBase {
 public:
	TTypeImageTexture(const char *name) : TTypeBase(name) {}
	void Init(TNode *pn);
	void Draw(TNode *pn);
};
}	//	namespace Spr
