// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "type.h"
#include "type_imp.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <map>
#include <iterator>
#define GLUT_NO_LIB_PRAGMA
#include <GL/glut.h>

namespace LVRM {
using namespace std;


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern GLUquadricObj*	qobj;
const int		cQuadricLOD = 24;

float	fieldOfView = M_PI / 4;
float	viewpos[] = { 0, 0, 10 };
float	vieworient[] = { 0, 0, 1, 0};


////////////////////////////////////////////////////////////////////////////////
//	Utilities

struct TVertex{
	float	coord[3];
	TVertex(float f = 0) { coord[0] = coord[1] = coord[2] = f;}
	TVertex(float *fp) { coord[0] = fp[0]; coord[1] = fp[1]; coord[2] = fp[2];}
	TVertex& operator=(const TVertex &t) {
		coord[0] = t.coord[0]; coord[1] = t.coord[1]; coord[2] = t.coord[2];
		return *this;
	}
	TVertex& operator+=(const TVertex &t) {
		coord[0] += t.coord[0]; coord[1] += t.coord[1]; coord[2] += t.coord[2];
		return *this;
	}
	float inner_product(const TVertex &t) const {
		return (coord[0] * t.coord[0] + coord[1] * t.coord[1] + coord[2] * t.coord[2]);
	}
	float cosine(const TVertex &t) const {
		return (this->inner_product(t)/sqrt(this->inner_product(*this)*t.inner_product(t)));
	}
	bool operator<(const TVertex &t) const {
		return (coord[0] < t.coord[0] ? true :
				(coord[0] > t.coord[0] ? false :
				 (coord[1] < t.coord[1] ? true :
				  (coord[1] > t.coord[1] ? false :
				   (coord[2] < t.coord[2])))));
	}
};

inline int Norm(const float *v0, const float *v1, const float *v2, float *n)
{
	n[0] = (v1[1] - v0[1]) * (v2[2] - v1[2]) - (v1[2] - v0[2]) * (v2[1] - v1[1]);
	n[1] = (v1[2] - v0[2]) * (v2[0] - v1[0]) - (v1[0] - v0[0]) * (v2[2] - v1[2]);
	n[2] = (v1[0] - v0[0]) * (v2[1] - v1[1]) - (v1[1] - v0[1]) * (v2[0] - v1[0]);

	double d = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	if(d != 0.0){
		n[0] /= d;	n[1] /= d;	n[2] /= d;
	}

	if(1.0E-3 < d && d < 1.0E+9)
		return 1;
	return 0;
}


template<class T>
struct ptr_less : binary_function<T*,T*,bool> {
	bool operator()(const T* x, const T* y) const { return *x < *y; }
};

typedef multimap<TVertex*, TVertex*, ptr_less<TVertex> >	TOptMap; // Vertex-NormalVector

static void OptimizeNormals(TOptMap &optmap, float creaseCos)
{
// 	if( creaseCos < .25)	creaseCos = .25;
	TOptMap::iterator	itm, itmt, itmi;
	for(itm = optmap.begin(); itm != optmap.end(); ++itm){
		itmt = optmap.upper_bound(itm->first);
		if(itmt == itm)
			continue;
		bool		bInit = false;
		itmi = itm;	++itmi;
		for( ; itmi != itmt; ++itmi){
			if(itm->second->cosine(*itmi->second) <= creaseCos)
				continue;
			*itm->second += *itmi->second;
			if(!bInit){
				bInit = true;
				*itmi->second = *itm->second;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
//	Draw global setting nodes

void TTypeBackground::Init(TNode *pn)
{
	vector<float>	c;
	PushbackElm(pn, "skyColor", c);

	if(c.size() >= 3)
		glClearColor(c[0], c[1], c[2], 1);
}

void TTypeViewpoint::Init(TNode *pn)
{
	CopyElm(pn, "fieldOfView", &fieldOfView);
	CopyElm(pn, "orientation", vieworient);
	CopyElm(pn, "position", viewpos);
}

void YYCleanup();
extern FILE *yyin;
void TTypeInline::Init(TNode *pn)
{
	char* url;
	CopyElm(pn, "url", &url);
	if (url){
		UTRef<TSts> root = LoadWrl(url);
		if (root){
			TField	fv;
			fv.push_back(new TFieldBaseSts(root));
			pn->SetElm("children", &fv);
		}else{
		}
	}
	TTypeBase::Init(pn);
}


////////////////////////////////////////////////////////////////////////////////
// Draw base nodes

void TTypeTransform::Draw(TNode *pn)
{
	float		center[] = {0, 0, 0};
	float		translation[] = {0, 0, 0};
	float		rotation[] = {0, 0, 1, 0};
	float		scale[] = {1, 1, 1};
	float		scaleOrientation[] = {0, 0, 1, 0};

	CopyElm(pn, "center", center);
	CopyElm(pn, "translation", translation);
	CopyElm(pn, "rotation", rotation);
	CopyElm(pn, "scale", scale);
	CopyElm(pn, "scaleOrientation", scaleOrientation);

	glPushMatrix();

	if(translation[0] || translation[1] || translation[2]){
		glTranslatef(translation[0], translation[1], translation[2]);
	}
	if(center[0] || center[1] || center[2]){
		glTranslatef(center[0], center[1], center[2]);
	}
	if(rotation[3]){
		glRotatef(180/M_PI * rotation[3], rotation[0], rotation[1], rotation[2]);
	}
	if(scaleOrientation[3]){
		glRotatef(180/M_PI * scaleOrientation[3], scaleOrientation[0], scaleOrientation[1], scaleOrientation[2]);
	}
	if(scale[0] != 1 || scale[1] != 1 || scale[2] != 1){
		glScalef(scale[0], scale[1], scale[2]);
	}
	if(scaleOrientation[3]){
		glRotatef(-180/M_PI * scaleOrientation[3], scaleOrientation[0], scaleOrientation[1], scaleOrientation[2]);
	}
	if(center[0] || center[1] || center[2]){
		glTranslatef(-center[0], -center[1], -center[2]);
	}
	Traverse(&TType::Draw, pn);
	glPopMatrix();
}
void TTypeTransform::EnumNamedNode(TNode *pn){
	char* nodeID = NULL;
	if (CopyElm(pn, "nodeID", &nodeID)){
		if (listener) listener->FoundNamed(pn);
	}else{
		if (listener) listener->FoundTransformBefore(pn);
		Traverse(&TType::EnumNamedNode, pn);
		if (listener) listener->FoundTransformAfter(pn);
	}
}
void TTypeTransform::EnumTypeNode(TNode *pn){
	if (listener) listener->FoundTransformBefore(pn);
	TTypeBase::EnumTypeNode(pn);
	if (listener) listener->FoundTransformAfter(pn);
}

void TTypeShape::Draw(TNode *pn)
{
	bool		bHasTextureTransform = false;
	TSts		*pSts = 0;
	if(CopyElm(pn, "appearance", &pSts) && 
		CopyElm(*pSts->begin(), "textureTransform", &pSts)){
		bHasTextureTransform = true;
	}

	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_LIGHTING_BIT);
	if(bHasTextureTransform){
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	Traverse(&TType::Draw, pn);

	if(bHasTextureTransform){
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	glPopAttrib();
}


////////////////////////////////////////////////////////////////////////////////
//	Draw geometry nodes

void TTypeBox::Draw(TNode *pn)
{
	float		size[] = {2, 2, 2};
	
	CopyElm(pn, "size", size);

	const static GLfloat	t[][2] = { {0, 0}, {1, 0}, {1, 1}, {0, 1}, };
	const static GLfloat	n[][3] = {
		{-1, 0, 0}, {0, 1, 0}, {1, 0, 0},
		{0, -1, 0}, {0, 0, 1}, {0, 0, -1},
	};
	const static int	f[][4] = {
		{0, 1, 2, 3}, {2, 6, 7, 3}, {5, 4, 7, 6},
		{0, 4, 5, 1}, {1, 5, 6, 2}, {4, 0, 3, 7},
	};
	GLfloat v[8][3];

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size[0] / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size[1] / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size[2] / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] =  size[0] / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] =  size[1] / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] =  size[2] / 2;

	glBegin(GL_QUADS);
	for(int i = 0 ; i < sizeof(f)/sizeof(*f); ++i){
		glNormal3fv(n[i]);
		glTexCoord2fv(t[0]);	glVertex3fv(v[f[i][0]]);
		glTexCoord2fv(t[1]);	glVertex3fv(v[f[i][1]]);
		glTexCoord2fv(t[2]);	glVertex3fv(v[f[i][2]]);
		glTexCoord2fv(t[3]);	glVertex3fv(v[f[i][3]]);
	}
	glEnd();
}

void TTypeSphere::Draw(TNode *pn)
{
	float		radius = 1;

	CopyElm(pn, "radius", &radius);

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	gluSphere(qobj, radius, cQuadricLOD, cQuadricLOD/2);
	glPopMatrix();
}

void TTypeCylinder::Draw(TNode *pn)
{
	float		radius = 1;
	float		height = 2;
	int			side = 1;
	int			bottom = 1;
	int			top = 1;

	CopyElm(pn, "radius", &radius);
	CopyElm(pn, "height", &height);
	CopyElm(pn, "side", &side);
	CopyElm(pn, "bottom", &bottom);
	CopyElm(pn, "top", &top);

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, -height/2);
	if(side)
		gluCylinder(qobj, radius, radius, height, cQuadricLOD, 1);
	if(bottom){
		glRotatef(180, 1, 0, 0);
		gluDisk(qobj, 0, radius, cQuadricLOD, 1);
		glRotatef(180, 1, 0, 0);
	}
	if(top){
		glTranslatef(0, 0 ,height);
		gluDisk(qobj, 0, radius, cQuadricLOD, 1);
	}
	glPopMatrix();
}

void TTypeCone::Draw(TNode *pn)
{
	float		bottomRadius = 1;
	float		height = 2;
	int			side = 1;
	int			bottom = 1;

	CopyElm(pn, "bottomRadius", &bottomRadius);
	CopyElm(pn, "height", &height);
	CopyElm(pn, "side", &side);
	CopyElm(pn, "bottom", &bottom);

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, -height/2);
	if(side)
		gluCylinder(qobj, bottomRadius, 0, height, cQuadricLOD, 1);
	if(bottom){
		glRotatef(180, 1, 0, 0);
		gluDisk(qobj, 0, bottomRadius, cQuadricLOD, 1);
	}
	glPopMatrix();
}

/*
void TTypeIndexedFaceSet::GetTriangles(TNode* pn, vector<Vec3f>& vtx, vector<PTM::TVector<3, int> >& face){
	vector<int> v;
	vector<float> p;
	TSts* pSts = 0;
	PushbackElm(pn, "coordIndex", v);
	if(CopyElm(pn, "coord", &pSts)) PushbackElm(*pSts->begin(), "point", p);
	if(v.size() == 0){
		cerr << "IndexedFaceSet has invalid data. skipping..." << endl;
		return;					// error
	}
	if(v.back() >= 0) v.push_back(-1);
	int idBase = vtx.size();
	for(vector<float>::iterator it = p.begin(); it != p.end(); it+=3){
		float v[3];
		v[0] = it[0];
		v[1] = it[1];
		v[2] = it[2];
		vtx.push_back(v);
	}
	vector<int> oneFace;
	for(vector<int>::iterator it = v.begin(); it != v.end(); ++it){
		if (*it < 0){
			oneFace.clear();
		}else{
			oneFace.push_back(*it + idBase);
			if (oneFace.size() >= 3){
				int f[3];
				f[0] = oneFace[0];
				f[1] = oneFace.end()[-2];
				f[2] = oneFace.end()[-1];
				face.push_back(f);
			}
		}
	}
}
*/
void TTypeIndexedFaceSet::Draw(TNode *pn)
{
	vector<int>	v;
	vector<float>	p;
	vector<int>	colidx;
	vector<float>	col;
	vector<int>	texidx;
	vector<float>	tex;
	TSts		*pSts = 0;
	int			ccw = 1;
	int			solid = 1;
	float		creaseAngle = 0;
	int			colorPerVertex = 1;

	PushbackElm(pn, "coordIndex", v);
	if(CopyElm(pn, "coord", &pSts))
		PushbackElm(*pSts->begin(), "point", p);
	PushbackElm(pn, "colorIndex", colidx);
	if(CopyElm(pn, "color", &pSts))
		PushbackElm(*pSts->begin(), "color", col);
	PushbackElm(pn, "texCoordIndex", texidx);
	if(CopyElm(pn, "texCoord", &pSts) && pSts->size())
		PushbackElm(*pSts->begin(), "point", tex);

	CopyElm(pn, "ccw", &ccw);
	CopyElm(pn, "solid", &solid);
	CopyElm(pn, "creaseAngle", &creaseAngle);
	CopyElm(pn, "colorPerVertex", &colorPerVertex);

	if(v.size() == 0){
		cerr << "IndexedFaceSet has invalid data. skipping..." << endl;
		return;					// error
	}
	if(v.back() >= 0)
		v.push_back(-1);

	vector<TVertex>		n(v.size(), 0.0f);
	vector<int>::iterator		itEnd, it;
	TOptMap		optmap;

	for(itEnd = it = v.begin(); itEnd != v.end(); ++itEnd){	// set normal vector
		if(*itEnd < 0){
			float	norm[3];
			int result = Norm(&p[*it * 3], &p[*(it+1) * 3], &p[*(it+2) * 3], norm);
			if(!result && itEnd - it > 3)
				Norm(&p[*it * 3], &p[*(it+2) * 3], &p[*(it+3) * 3], norm);
			for( ; it != itEnd; ++it){
				n[it - v.begin()] = norm;
				if(creaseAngle)
					optmap.insert(TOptMap::value_type((TVertex*)&p[*it * 3], &n[it - v.begin()]));
			}
			++it;
		}
	}
	OptimizeNormals(optmap, cos(creaseAngle));

	if(!solid)
		glDisable(GL_CULL_FACE);
	else if(!ccw)
		glCullFace(GL_FRONT);

	int	cntFace = 0;
	for(itEnd = it = v.begin(); itEnd != v.end(); ++itEnd){	// draw
		if(*itEnd < 0){
			glBegin(GL_POLYGON);
			if(!colorPerVertex && col.size()){
				int		idx = colidx.size() ? colidx[cntFace] : cntFace;
				if(idx * 3 < col.size())
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &col[idx * 3]);
			}
			for( ; it != itEnd; ++it){
				if(colorPerVertex && col.size()){
					int	idx = colidx.size() ? colidx[it - v.begin()] : *it;
					if(idx * 3 < col.size())
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &col[idx * 3]);
				}
				if(tex.size()){
					int	idx = texidx.size() ? texidx[it - v.begin()] : *it;
					if(idx * 2 < tex.size())
						glTexCoord2fv(&tex[idx * 2]);
				}
				glNormal3fv((float*)&n[it - v.begin()]);
				glVertex3fv(&p[*it * 3]);
			}
			glEnd();
			++it;
			++cntFace;
		}
	}
}
/*
void TTypeElevationGrid::GetTriangles(TNode* pn, vector<Vec3f>& vtx, vector<PTM::TVector<3, int> >& face){
	vector<float>	height;
	float		creaseAngle = 0;
	int			colorPerVertex = 1;
	int			xDimension = 0;
	float		xSpacing = 1;
	int			zDimension = 0;
	float		zSpacing = 1;

	PushbackElm(pn, "height", height);
	CopyElm(pn, "xDimension", &xDimension);
	CopyElm(pn, "xSpacing", &xSpacing);
	CopyElm(pn, "zDimension", &zDimension);
	CopyElm(pn, "zSpacing", &zSpacing);
	vector<int>	v;
	vector<float>	p(xDimension * zDimension * 3, 0.0f);
	for(int z = 0; z < zDimension; ++z){
		for(int x = 0; x < xDimension; ++x){
			const int		idx = x + xDimension * z;
			p[idx*3 + 0] = x * xSpacing;
			p[idx*3 + 1] = height[idx];
			p[idx*3 + 2] = z * zSpacing;
			if(x != xDimension - 1 && z != zDimension - 1){
				v.push_back(idx);
				v.push_back(idx + xDimension);
				v.push_back(idx + xDimension + 1);
				v.push_back(-1);
				v.push_back(idx + xDimension + 1);
				v.push_back(idx + 1);
				v.push_back(idx);
				v.push_back(-1);
			}
		}
	}
	int idBase = vtx.size();
	for(int i=0; i<p.size(); i+=3){
		float idx[3];
		idx[0] = p[0];
		idx[1] = p[1];
		idx[2] = p[2];
		vtx.push_back(idx);
	}
	for(int i=0; i<v.size(); i+=4){
		int oneFace[3];
		oneFace[0] = v[i] + idBase;
		oneFace[1] = v[i+1] + idBase;
		oneFace[2] = v[i+2] + idBase;
		face.push_back(oneFace);
	}
}
*/

void TTypeElevationGrid::Draw(TNode *pn){
	vector<float>	height;
	vector<float>	col;
	vector<float>	tex;
	TSts		*pSts = 0;
	int			ccw = 1;
	int			solid = 1;
	float		creaseAngle = 0;
	int			colorPerVertex = 1;
	int			xDimension = 0;
	float		xSpacing = 1;
	int			zDimension = 0;
	float		zSpacing = 1;

	PushbackElm(pn, "height", height);
 	if(CopyElm(pn, "color", &pSts))
		PushbackElm(*pSts->begin(), "color", col);
 	if(CopyElm(pn, "texCoord", &pSts))
		PushbackElm(*pSts->begin(), "point", tex);

	CopyElm(pn, "ccw", &ccw);
	CopyElm(pn, "solid", &solid);
	CopyElm(pn, "creaseAngle", &creaseAngle);
	CopyElm(pn, "colorPerVertex", &colorPerVertex);
	CopyElm(pn, "xDimension", &xDimension);
	CopyElm(pn, "xSpacing", &xSpacing);
	CopyElm(pn, "zDimension", &zDimension);
	CopyElm(pn, "zSpacing", &zSpacing);

	// make IndexedFaceSet-compatible data
	tex.resize(xDimension * zDimension * 2, 0.0f);
	vector<int>	v;
	vector<float>	p(xDimension * zDimension * 3, 0.0f);
	for(int z = 0; z < zDimension; ++z){
		for(int x = 0; x < xDimension; ++x){
			const int		idx = x + xDimension * z;
			tex[idx*2 + 0] = 1.0/xDimension * x;
			tex[idx*2 + 1] = 1.0/zDimension * z;
			p[idx*3 + 0] = x * xSpacing;
			p[idx*3 + 1] = height[idx];
			p[idx*3 + 2] = z * zSpacing;
			if(x != xDimension - 1 && z != zDimension - 1){
				v.push_back(idx);
				v.push_back(idx + xDimension);
				v.push_back(idx + xDimension + 1);
				v.push_back(-1);
				v.push_back(idx + xDimension + 1);
				v.push_back(idx + 1);
				v.push_back(idx);
				v.push_back(-1);
			}
		}
	}

// following lines are copyed from TTypeIndexedFaceSet::Draw()
	vector<TVertex>		n(v.size(), 0.0f);
	vector<int>::iterator		itEnd, it;
	TOptMap		optmap;

	for(itEnd = it = v.begin(); itEnd != v.end(); ++itEnd){	// set normal vector
		if(*itEnd < 0){
			float	norm[3];
			Norm(&p[*it * 3], &p[*(it+1) * 3], &p[*(it+2) * 3], norm);
			for( ; it != itEnd; ++it){
				n[it - v.begin()] = norm;
				if(creaseAngle)
					optmap.insert(TOptMap::value_type((TVertex*)&p[*it * 3], &n[it - v.begin()]));
			}
			++it;
		}
	}
	OptimizeNormals(optmap, cos(creaseAngle));

	if(!solid)
		glDisable(GL_CULL_FACE);
	else if(!ccw)
		glCullFace(GL_FRONT);

	glDisable(GL_LIGHTING);
	glBegin(GL_TRIANGLES);
	int triCount=0;
	for(itEnd = it = v.begin(); itEnd != v.end(); ++itEnd){	// draw
		if(*itEnd < 0){
			if (!colorPerVertex){
				glColor3fv(&col[(triCount/2)*3]);
			}
			for( ; it != itEnd; ++it){
				if (colorPerVertex){
					glColor3fv(&col[*it * 3]);
				}
				glTexCoord2fv(&tex[*it * 2]);
				glNormal3fv((float*)&n[it - v.begin()]);
				glVertex3fv(&p[*it * 3]);
			}
			triCount++;
			++it;
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}
////////////////////////////////////////////////////////////////////////////////
//	Draw apperance nodes

void TTypeTextureTransform::Draw(TNode *pn)
{
	float		center[] = {0, 0, 0};
	float		translation[] = {0, 0, 0};
	float		rotation[] = {0, 0, 1, 0};
	float		scale[] = {1, 1, 1};

	CopyElm(pn, "center", center);
	CopyElm(pn, "translation", translation);
	CopyElm(pn, "rotation", rotation);
	CopyElm(pn, "scale", scale);

	glMatrixMode(GL_TEXTURE);
	if(center[0] || center[1] || center[2]){
		glTranslatef(-center[0], -center[1], -center[2]);
	}
	if(scale[0] != 1 || scale[1] != 1 || scale[2] != 1){
		glScalef(scale[0], scale[1], scale[2]);
	}
	if(rotation[3]){
		glRotatef(180/M_PI * rotation[3], rotation[0], rotation[1], rotation[2]);
	}
	if(center[0] || center[1] || center[2]){
		glTranslatef(center[0], center[1], center[2]);
	}
	if(translation[0] || translation[1] || translation[2]){
		glTranslatef(translation[0], translation[1], translation[2]);
	}
	glMatrixMode(GL_MODELVIEW);
}

void TTypeMaterial::Draw(TNode *pn)
{
	float		diffuseColor[] = {.8, .8, .8, 1};
	float		emissiveColor[] = {0, 0, 0, 1};
	float		specularColor[] = {0, 0, 0, 1};
	float		ambientIntensity = .2;
	float		shininess = .2;
	float		transparency = 0;

	CopyElm(pn, "diffuseColor", diffuseColor);
	CopyElm(pn, "emissiveColor", emissiveColor);
	CopyElm(pn, "specularColor", specularColor);
	CopyElm(pn, "ambientIntensity", &ambientIntensity);
	CopyElm(pn, "shininess", &shininess);
	CopyElm(pn, "transparency", &transparency);

	diffuseColor[3] = emissiveColor[3] = specularColor[3] = 1 - transparency;
	float		ambientColor[] = {diffuseColor[0] * ambientIntensity,
								  diffuseColor[1] * ambientIntensity,
								  diffuseColor[2] * ambientIntensity, diffuseColor[3]};

	if(transparency){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissiveColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientColor);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * shininess);
}

}//	namespace LVRM
