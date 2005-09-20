//#include <qhull_a.h>
/*#if defined(__cplusplus)
extern "C"
{
#endif
#include <stdio.h>
#include <stdlib.h>
#include <qhull.h>
#include <mem.h>
#include <qset.h>
#include <geom.h>
#include <merge.h>
#include <poly.h>
#include <io.h>
#include <stat.h>
//char qh_version[] = "qad 2000/8/1";  /* used for error messages 
#if defined(__cplusplus)
}
#endif*/
#include "calcPenalty.h"
#include <GL/glut.h>
#include <set>




void TCoords::Print(std::ostream& os) const{
	os << "{";
	for(const_iterator it=begin(); it!=end(); ++it){
		if (it != begin()) os << ",";
		os << posture * *it;
	}
	os << "}";
}

void TFace::Print(std::ostream& os) const{
	os << "(";
	for(const_iterator it=begin(); it!=end(); ++it){
		if (it != begin()) os << ",";
		os << *it;
	}
	os << ")";
}

void TPolytope::Print(std::ostream& os) const{
	for(const_iterator it = begin(); it != end(); ++it){
		os << "{";
		for(TFace::const_iterator itf = it->begin(); itf != it->end(); ++itf){
			os << vertices.GCoord(*itf);
		}
		os << "}";
	}
	os << std::endl;
}

void TMidPoint::Print(std::ostream& os) const {
	if (vtx[0] >= 0){
		if (vtx[1] >= 0){
			os.precision(2);
			os << ratio << "*" << (*face)[vtx[0]] << coords->posture * (*coords)[(*face)[vtx[0]]] << " + ";
			os << (1-ratio) << "*" << (*face)[vtx[1]] << coords->posture * (*coords)[(*face)[vtx[1]]];
		}else{
			os << ratio << "*" << (*face)[vtx[0]] << coords->posture * (*coords)[(*face)[vtx[0]]];
		}
	}else{
		os << "NON";
	}
}

//----------------------------------------------------------------------------
//	TCalcPenalty

/*bool TCalcPenalty::QhullSwitch(TPolytope* tope1, TPolytope* tope2) {
	Affined posture1 = tope1->vertices.posture;
	Affined posture2 = tope2->vertices.posture;

	for (TPolytope::iterator it = tope1->begin(); it != tope1->end(); it++) {
		Vec3d point = tope1->vertices.GCoord((*it)[0]);
		Vec3d normal = it->GNormal(posture1);
		
		if ( normal * (interior - point) >= 0)
			return false;
	}
	for (TPolytope::iterator it = tope2->begin(); it != tope2->end(); it++) {
		Vec3d point = tope2->vertices.GCoord((*it)[0]);
		Vec3d normal = it->GNormal(posture2);
		
		if ( normal * (interior - point) >= 0)
			return false;
	}
	return true;
}*/


void TCalcPenalty::Preprocessor(TPolytope* tope1, TPolytope* tope2) {
	char* argv[] = {"qhull", "H", "Fp", "QJ"};
	int i=0;
	int numHalfs = tope1->size() + tope2->size();
//	facetT* facet;
//	coordT feasible[3];
	coordT *halfspace;
	coordT *inputPt;
	Affined posture1 = tope1->vertices.posture;
	Affined posture2 = tope2->vertices.posture;

	qh_init_A(stdin, stdout, stderr, 4, argv);
	qh_initflags (qh qhull_command);

	/// normalとoffsetの入力
	halfspace = (coordT*)calloc(numHalfs * 4, sizeof(coordT));
	for (TPolytope::iterator it = tope1->begin(); it != tope1->end(); it++) {
		Vec3d point = tope1->vertices.GCoord((*it)[0]);
		Vec3d normal = it->GNormal(posture1);
		
		halfspace[i*4+0] = normal.X();
		halfspace[i*4+1] = normal.Y();
		halfspace[i*4+2] = normal.Z();
		halfspace[i*4+3] = -normal * point;	/// offset
		i++;
	}
	for (TPolytope::iterator it = tope2->begin(); it != tope2->end(); it++) {
		Vec3d point = tope2->vertices.GCoord((*it)[0]);
		Vec3d normal = it->GNormal(posture2);
		
		halfspace[i*4+0] = normal.X();
		halfspace[i*4+1] = normal.Y();
		halfspace[i*4+2] = normal.Z();
		halfspace[i*4+3] = -normal * point;	/// offset
		i++;
	}

	/// interiorの入力
	qh feasible_point = (coordT*)malloc(3*sizeof(coordT));
	qh feasible_point[0] = interior.X();
	qh feasible_point[1] = interior.Y();
	qh feasible_point[2] = interior.Z();
	inputPt = qh_sethalfspace_all(4, numHalfs, halfspace, qh feasible_point);
	qh_init_B(inputPt, numHalfs, 3, True);
	qh_qhull();
	qh_check_output();
//	qh_produce_output();
	TCalcPenalty::OutputQhalf();
	/// 頂点の取り出し
//	FORALLfacet_(facet_list) QhalfOutput(qh fout, qh PRINTout[i], facet, !qh_ALL);
//	FORALLfacets QhalfOutput(qh fout, qh PRINTout[i], facet, !qh_ALL);

/*	void qh_printvertexlist (FILE *fp, char* string, facetT *facetlist, 
                         setT *facets, boolT printall) {
  vertexT *vertex, **vertexp;
  setT *vertices;
  

  vertices= qh_facetvertices (facetlist, facets, printall);
  fputs (string, fp);
  FOREACHvertex_(vertices)
    qh_printvertex(fp, vertex);
  qh_settempfree (&vertices);*/

/*	FORALLfacet_(qh facet_list){
		FOREACHvertex_(facet->vertices){
			Vec3d coord;

			coord.X() = vertex->point[0];
			coord.Y() = vertex->point[1];
			coord.Z() = vertex->point[2];
			vertexVct.push_back(coord);
		}
	}*/
    if (qh VERIFYoutput && !qh STOPpoint && !qh STOPcone)
      qh_check_points ();
	free(halfspace);
    qh NOerrexit= True;
//	free(qh feasible_point);
	/// qhullのメモリ開放
	qh_freeqhull(!qh_ALL);
	int curlong, totlong;
//	extern "C" {
	qh_memfreeshort(&curlong, &totlong);
//	}
}
	
void TCalcPenalty::CalcCommonArea(){
	char* argv[] = {"qhull", "Qt", "i"};
//	facetT* facet;
//	vertexT* vertex;
	coordT* convex;

	qh_init_A(stdin, stdout, stderr, 3, argv);
	qh_initflags (qh qhull_command);

	/// qhullへの入力
//	convex = new coordT[vertexVct.size() * 3];
	convex = (coordT*)malloc(vertexVct.size() * 3 * sizeof(coordT));
	for(int i=0; i<vertexVct.size(); i++){
		convex[i*3+0] = vertexVct[i].X();
		convex[i*3+1] = vertexVct[i].Y();
		convex[i*3+2] = vertexVct[i].Z();
	}

	qh_init_B(convex, vertexVct.size(), 3, False);
	qh_qhull();
	qh_check_output();
	qh_triangulate();  /* requires option 'Q11' if want to add points */ 
//	qh_produce_output();

/*	FORALLfacets{	///要注意
		TFace face;
		face.normal.X() = facet->normal[0];
		face.normal.Y() = facet->normal[1];
		face.normal.Z() = facet->normal[2];
		FOREACHvertex_(facet->vertices){
			face.push_back( qh_pointid(vertex->point) );
		}
		commonArea.push_back(face);
	}*/
	facetT* facet;
	FORALLfacets {
		vertexT *vertex, **vertexp;
		setT *vertices;
		TFace face;

		face.normal.X() = facet->normal[0];
		face.normal.Y() = facet->normal[1];
		face.normal.Z() = facet->normal[2];
		vertices= qh_facet3vertex (facet);
		FOREACHvertex_(vertices) {
			int id = qh_pointid(vertex->point);
			std::cout << id <<" ";
			face.push_back( qh_pointid(vertex->point) );
		}
		std::cout << std::endl;
//		assert(face.size() != 3);
		commonArea.push_back(face);
		qh_settempfree(&vertices);
	}
    if (qh VERIFYoutput && !qh STOPpoint && !qh STOPcone)
      qh_check_points ();
//	free(convex);
	qh NOerrexit= True;
	qh_freeqhull(!qh_ALL);
	int curlong, totlong;
	qh_memfreeshort(&curlong, &totlong);
}
void TCalcPenalty::Draw(){
	//	力の表示
	if(force.norm() > 0){
		Vec3d p = (force ^ torque).unit() * (torque.norm() / force.norm());
		glBegin(GL_LINES);
		glVertex3dv(p+force);
		glVertex3dv(p-force);
		glEnd();
	}
}
void TCalcPenalty::CalcPenalty(TPolytope* tope1, TPolytope* tope2){
	force = Vec3d();
	torque = Vec3d();

	Preprocessor(tope1, tope2);
	CalcCommonArea();
	for(TFaces::iterator it=commonArea.begin(); it!=commonArea.end(); it++)
		CalcTriPenalty(it);
}
/*void TCalcPenaltySub::CalcPenalty(){
	type=NONE;
	force = Vec3d();
	torque = Vec3d();
	area = 0.0f;
	if (poly1.state == TCollideInterface::FACE && poly2.state == TCollideInterface::FACE){
		for(TFaces::iterator it1 = poly1.begin(); it1 != poly1.end(); ++it1){
			for(TFaces::iterator it2 = poly2.begin(); it2 != poly2.end(); ++it2){
				//	共通部分を求める
				T2DIsects commonArea;
				FindCommonArea(commonArea, poly1.vertices, *it1, poly2.vertices, *it2, normal);
				if (commonArea.size() > 2){	//	共通部分が面の時，
					//	3角形に分割してペナルティを求める
					for(int i=1; i< commonArea.size()-1; i++){
						T2DIsects tri;
						tri.push_back(commonArea[0]);
						tri.push_back(commonArea[i]);
						tri.push_back(commonArea[i+1]);
						CalcTriPenalty(tri, it1, normal);
					}
				}
			}
		}
		type = FACE;
	}else if ((poly1.state == TCollideInterface::EDGE && poly2.state == TCollideInterface::FACE)
		||(poly1.state == TCollideInterface::FACE && poly2.state == TCollideInterface::EDGE)){
		for(TFaces::iterator it1 = poly1.begin(); it1 != poly1.end(); ++it1){
			for(TFaces::iterator it2 = poly2.begin(); it2 != poly2.end(); ++it2){
				//	共通部分を求める
				T2DIsects commonArea;
				if (poly1.state == TCollideInterface::EDGE){
					FindCommonLine(commonArea, poly1.vertices, *it1, poly2.vertices, *it2, normal);
					for(int i=0; i<commonArea.size(); i+=2){
						CalcEdgePenalty(commonArea[i], commonArea[i+1], normal, 1);
					}
				}else{
					FindCommonLine(commonArea, poly2.vertices, *it2, poly1.vertices, *it1, -normal);
					for(int i=0; i<(int)commonArea.size()-1; i++){
						CalcEdgePenalty(commonArea[i], commonArea[i+1], -normal, -1);
					}
				}
			}
		}
		type = EDGE;
	}else{
		CalcPointPenalty(poly1.point, poly2.point, normal);
		type = VERTEX;
	}
//	if(force.norm() > 0){
//		Vec3d p = (force ^ torque).unit() * (torque.norm() / force.norm());
//		std::cout << "pos:" << p << " f:" << force;
//		std::cout << " tq:" << torque << std::endl;
//	}
}*/
void TCalcPenalty::CalcTriPenalty(TFace* face){
	double depth[3];
	Vec3d coord[3];
	Vec3d f, t;
	int d, s;

	if (normal * face->normal == 0) {
		s = 0;
		d = 0;
	}else if(normal * face->normal > 0) {
		s = 1;
		d = 1;
	}else {
		s = 1;
		d = -1;
	}
	for(int i=0; i<3; i++){
		depth[i] = (vertexVct[(*face)[i]] - interior) * normal.unit();
		coord[i] = vertexVct[(*face)[i]] - depth[i] * normal.unit();
		depth[i] *= d;
	}
	double wholeArea = s * ((coord[2] - coord[0]) ^ (coord[1] - coord[0])).norm() / 4;
	CalcForceTorque(f, t, coord, depth);
	force += f;
	torque += t;
	area += wholeArea;
}
void TCalcPenalty::OutputQhalf() {
  int tempsize= qh_setsize ((setT*)qhmem.tempstack);

  OutputIntersections(qh fout, qh PRINTout[0], qh facet_list, NULL, !qh_ALL);
  qh_allstatistics();
  if (qh_setsize ((setT*)qhmem.tempstack) != tempsize) {
    fprintf (qh ferr, "qhull internal error (qh_produce_output): temporary sets not empty (%d)\n",
	     qh_setsize ((setT*)qhmem.tempstack));
    qh_errexit (qh_ERRqhull, NULL, NULL);
  }
}

void TCalcPenalty::OutputIntersections(FILE *fp, int format, facetT *facetlist, setT *facets, boolT printall) {
//  int numfacets;
  facetT *facet;
//  setT *vertices;
//  coordT *center;
//  realT outerplane, innerplane;

  qh old_randomdist= qh RANDOMdist;
  qh RANDOMdist= False;
//  qh_printbegin (fp, format, facetlist, facets, printall);
  FORALLfacet_(facetlist)
     TCalcPenalty::OutputIntersection (fp, format, facet, printall);
//  FOREACHfacet_(facets) 
//     qh_printafacet (fp, format, facet, printall);
//  qh_printend (fp, format, facetlist, facets, printall);
  qh RANDOMdist= qh old_randomdist;
}

void TCalcPenalty::OutputIntersection(FILE *fp, int format, facetT *facet, boolT printall) {
//  realT color[4], offset, dist, outerplane, innerplane;
  boolT zerodiv;
  coordT *point, *normp, *coordp, *feasiblep;
  int k;
//  vertexT *vertex, **vertexp;
//  facetT *neighbor, **neighborp;

  switch (format) {
  case qh_PRINTpointintersect:
    if (!qh feasible_point) {
      fprintf (fp, "qhull input error (qh_printafacet): option 'Fp' needs qh feasible_point\n");
      qh_errexit( qh_ERRinput, NULL, NULL);
    }
    if (facet->offset > 0)
      goto LABELprintinfinite;
    point= coordp= (coordT*)qh_memalloc (qh normal_size);
    normp= facet->normal;
    feasiblep= qh feasible_point;
    if (facet->offset < -qh MINdenom) {
      for (k= qh hull_dim; k--; )
        *(coordp++)= (*(normp++) / - facet->offset) + *(feasiblep++);
    }else {
      for (k= qh hull_dim; k--; ) {
        *(coordp++)= qh_divzero (*(normp++), facet->offset, qh MINdenom_1,
				 &zerodiv) + *(feasiblep++);
        if (zerodiv) {
          qh_memfree (point, qh normal_size);
          goto LABELprintinfinite;
        }
      }
    }
    TCalcPenalty::OutputCoord(fp, NULL, point);
    qh_memfree (point, qh normal_size);
    break;
  LABELprintinfinite:
    for (k= qh hull_dim; k--; )
      fprintf (fp, qh_REAL_1, qh_INFINITE);
    fprintf (fp, "\n");   
    break;
  }
}
void TCalcPenalty::OutputCoord(FILE *fp, char *string, pointT* point) {
	realT r;
	Vec3d coord;
//	int id= qh_pointid( point);

	if(!point) return;
	r = *point++;
	coord.X() = r;
	r = *point++;
	coord.Y() = r;
	r = *point++;
	coord.Z() = r;

	TCalcPenalty::vertexVct.push_back(coord);
}


/*	TCalcPenaltySub::Id2Coord(qh hull_dim, point, id);
}
void TCalcPenaltySub::Id2Coord(int dim, pointT* point, int id) {
	realT r; 
	Vec3d coord;

	if(!point) return;
	r = *point++;
	coord.X() = r;
	r = *point++;
	coord.Y() = r;
	r = *point++;
	coord.Z() = r;

	TCalcPenalty::vertexVct.push_back(coord);
}*/





/*void TCalcPenaltySub::CalcTriPenalty(const T2DIsects& tri, TFace* face1, const Vec3d& normal){
	//	face1 から face2 に向かうベクトルがnormal
	//	World座標系での３頂点の位置と侵入量を求める．
	double depth[3];
	Vec3d coord[3];
	for(int i=0; i<3; i++){
		Vec3d oppFaceBase = (*tri[i].mp[1].coords).posture * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[0]];
		Vec3d oppFace1 = (*tri[i].mp[1].coords).posture * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[1]];
		Vec3d oppFace2 = (*tri[i].mp[1].coords).posture * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[2]];
		Vec3d oppFaceNormal = ((oppFace1-oppFaceBase) ^ (oppFace2-oppFaceBase)).unit();
		
		coord[i] = (*tri[i].mp[0].coords).posture * tri[i].mp[0].Coord();
		Vec3d oppPos = coord[i] - (coord[i]-oppFaceBase)*oppFaceNormal*oppFaceNormal;
		depth[i] = (coord[i] - oppPos) * normal;
		if (face1 != tri[i].mp[0].face) depth[i] *= -1;
	}
	coord[1] -= (coord[1] - coord[0]) * normal * normal;
	coord[2] -= (coord[2] - coord[0]) * normal * normal;
	for(int i=0; i<3; i++){
//		std::cout << "P" << i << ":" << coord[i] << "depth:" << depth[i] << std::endl;
	}
	
	//	侵入量が負の頂点の数で場合わけ
	int minusId[3], pulsId[3];
	int nMinus = 0, nPlus = 0;
	for(int i=0; i<3; i++){
		if (depth[i] < 0){
			minusId[nMinus] = i;
			nMinus++;
		}else{
			pulsId[nPlus] = i;
			nPlus++;
		}
	}
			
	Vec3d f, t;
	double wholeArea = ((coord[2] - coord[0]) ^ (coord[1] - coord[0])).norm()/8;
	if (nMinus == 3){
		//	全部負なら力は出ない
	}else if(nMinus == 2){
		//	正の部分だけを積分
		std::swap(coord[0], coord[pulsId[0]]);
		std::swap(depth[0], depth[pulsId[0]]);
		double ratio1 = depth[0] / (depth[0] - depth[1]);
		double ratio2 = depth[0] / (depth[0] - depth[2]);
		CalcForceTorque(f, t, ratio1, ratio2, coord, depth, normal);
		force += f; torque += t;
		area += wholeArea*ratio1*ratio2;
	}else if(nMinus == 1){
		//	まず全体を積分
		CalcForceTorque(f, t, 1, 1, coord, depth, normal);
		force += f; torque += t;
		
		//	負の部分だけを積分して引く
		std::swap(coord[0], coord[minusId[0]]);
		std::swap(depth[0], depth[minusId[0]]);
		double ratio1 = depth[0] / (depth[0] - depth[1]);
		double ratio2 = depth[0] / (depth[0] - depth[2]);
		Vec3d f2, t2;
		CalcForceTorque(f2, t2, ratio1, ratio2, coord, depth, normal);
		force -= f2; torque -= t2;
		area += wholeArea*(1-ratio1*ratio2);
	}else{
		//	全体を積分
		CalcForceTorque(f, t, 1, 1, coord, depth, normal);
		force += f; torque += t;
		area += wholeArea;
	}
}*/	
/*void TCalcPenaltySub::CalcEdgePenalty(const T2DIsect& p1, const T2DIsect& p2, const Vec3d& normal, double sign){
	Vec3d wp1 = p1.mp[0].coords->posture * p1.mp[0].Coord();
	Vec3d wp2 = p2.mp[0].coords->posture * p2.mp[0].Coord();
	Vec3d opp = p1.mp[1].coords->posture * (*p1.mp[1].coords)[(*p1.mp[1].face)[0]];
	Vec3d a = wp2 - wp1;
	double pen[2];
	pen[0]= (wp1 - opp)*normal;
	pen[1]= (wp2 - opp)*normal;
	double from = 0, to = 1;
	if (pen[0] <= 0 && pen[1] <= 0){
		return;
	}else if (pen[1] < 0){
		to = (pen[0] / pen[0] - pen[1]);
	}else if (pen[0] < 0){
		from = (pen[1] / pen[1] - pen[0]);
	}else{
		double c = (wp1 - opp)*normal;
		double f = (wp2 - opp)*normal - c;
		double s = to - from;
		double ss = to*to - from*from;
		double sss = to*to*to - from*from*from;
		force += sign * (c*s+0.5f*f*ss) * a.norm() * normal;
		torque += sign * ((wp1^force) + (a^normal) * a.norm() * (0.5f*c*ss+(1.0f/3.0f)*f*sss));
		area += a.norm()*s/2;
	}
}
void TCalcPenaltySub::CalcPointPenalty(Vec3d p1, Vec3d p2, const Vec3d& normal){
	force += p1-p2;
	torque += 0.5f*(p1+p2) ^ (p1-p2);
	area += 1.0f;
}*/
void TCalcPenalty::CalcForceTorque(Vec3d& f, Vec3d& t, Vec3d* coord, double* depth){
	Vec3d p = coord[0];
	Vec3d a = (coord[1] - coord[0]);
	double fa = (depth[1] - depth[0]);
	Vec3d b = (coord[2] - coord[0]);
	double fb = (depth[2] - depth[0]);
	double c = depth[0];
	if ((a^b) * normal < 0){
		std::swap(a, b);
		std::swap(fa, fb);
	}
//	std::cout << "p:" << p << " a:" << a << " b:" << b;
//	std::cout << " c" << c <<  "fa:" << fa << " fb:" << fb << std::endl;
	f = (a^b) * ( (1.0f/6.0f)*fa + (1.0f/6.0f)*fb + 0.5f*c );
	t = (p^f) + (a^(a^b))*( (1.0f/12.0f)*fa + (1.0f/24.0f)*fb + (1.0f/6.0f)*c )
		+ (b^(a^b))*( (1.0f/24.0f)*fa + (1.0f/12.0f)*fb + (1.0f/6.0f)*c );
}
/*//	Line ---------------------------------------------------------------------
struct CompForFindCommonLine{
	bool operator () (const T2DIsect& a, const T2DIsect& b) const{
		return a.mp[0].ratio > a.mp[1].ratio;
	}
};*/
/*void TCalcPenaltySub::FindCommonLine(T2DIsects& commonLine, const TCoords& vtxs1, TFace& edge1, const TCoords& vtxs2, TFace& face2, const Vec3d& n){
	assert(edge1.size() == 2);
	Vec3d p1 = vtxs1.posture * vtxs1[edge1[0]];
	Vec3d p2 = vtxs1.posture * vtxs1[edge1[1]];
	bool bInside1 = true, bInside2 = true;
	std::set<T2DIsect, CompForFindCommonLine> iss;
	for(int j=0; j<face2.size(); ++j){
		int j2 = (j+1) % face2.size();
		Vec3d q1 = vtxs2.posture * vtxs2[face2[j]];
		Vec3d q2 = vtxs2.posture * vtxs2[face2[j2]];
		//	交点を探す
		double r1, r2;
		if (FindIntersection(r1, p1, p2, r2, q1, q2, n)){
			T2DIsect is(TMidPoint(&vtxs1, &edge1, 0, 1, r1), TMidPoint(&vtxs2, &face2, j, j2, r2));
			iss.insert(is);
		}
		if (!InsideCheck(p1, q1, q2, n)) bInside1 = false;
		if (!InsideCheck(p2, q1, q2, n)) bInside2 = false;
	}
	//	中にある頂点を列挙
	if (bInside1) commonLine.push_back(T2DIsect(TMidPoint(&vtxs1, &edge1, 0, -1, 1), TMidPoint(&vtxs2, &face2)));
	for(std::set<T2DIsect, CompForFindCommonLine>::iterator it = iss.begin(); it != iss.end(); ++it){
		commonLine.push_back(*it);
	}
	if (bInside2) commonLine.push_back(T2DIsect(TMidPoint(&vtxs1, &edge1, 1, -1, 1), TMidPoint(&vtxs2, &face2)));
	if (commonLine.size()){
//		std::cout << "common line:" << std::endl;
//		std::cout << commonLine;
	}
}*/
	
//	Area ---------------------------------------------------------------------
/*void TCalcPenalty::FindCommonArea(T2DIsects& commonArea, const TCoords& vtxs1, TFace& face1, const TCoords& vtxs2, TFace& face2, const Vec3d& n){
//	std::cout << "F1:" << face1 << std::endl;
//	std::cout << "F2:" << face2 << std::endl;

	T2DIsects iss;
	//	中にある頂点を列挙1
	for(int i=0; i<face1.size(); ++i){
		int i2 = (i+1) % face1.size();
		Vec3d p1 = vtxs1.posture * vtxs1[face1[i]];
		Vec3d p2 = vtxs1.posture * vtxs1[face1[i2]];
		bool bInside= true;
		for(int j=0; j<face2.size(); ++j){
			int j2 = (j+1) % face2.size();
			Vec3d q1 = vtxs2.posture * vtxs2[face2[j]];
			Vec3d q2 = vtxs2.posture * vtxs2[face2[j2]];
			//	中にある頂点かチェック
			if (!InsideCheck(p1, q1, q2, n)) bInside = false;
		}
		//	中にある頂点を列挙
		if (bInside){
			iss.push_back(T2DIsect(TMidPoint(&vtxs1, &face1, i, -1, 1), TMidPoint(&vtxs2, &face2)));
		}
	}
	//	中にある頂点を列挙2
	for(int j=0; j<face2.size(); ++j){
		int j2 = (j+1) % face2.size();
		Vec3d q1 = vtxs2.posture * vtxs2[face2[j]];
		Vec3d q2 = vtxs2.posture * vtxs2[face2[j2]];
		bool bInside= true;
		for(int i=0; i<face1.size(); ++i){
			int i2 = (i+1) % face1.size();
			Vec3d p1 = vtxs1.posture * vtxs1[face1[i]];
			Vec3d p2 = vtxs1.posture * vtxs1[face1[i2]];
			//	中にある頂点かチェック
			if (!InsideCheck(q1, p1, p2, -n)) bInside = false;
		}
		//	中にある頂点を列挙
		if (bInside){
			iss.push_back(T2DIsect(TMidPoint(&vtxs2, &face2, j, -1, 1), TMidPoint(&vtxs1, &face1)));
		}
	}
	//	交点の列挙
	for(int i=0; i<face1.size(); ++i){
		int i2 = (i+1) % face1.size();
		Vec3d p1 = vtxs1.posture * vtxs1[face1[i]];
		Vec3d p2 = vtxs1.posture * vtxs1[face1[i2]];
		bool bInside= true;
		for(int j=0; j<face2.size(); ++j){
			int j2 = (j+1) % face2.size();
			Vec3d q1 = vtxs2.posture * vtxs2[face2[j]];
			Vec3d q2 = vtxs2.posture * vtxs2[face2[j2]];
			double r1, r2;
			//	交点を探す
			if (FindIntersection(r1, p1, p2, r2, q1, q2, n)){
				T2DIsect is(TMidPoint(&vtxs1, &face1, i, i2, r1), TMidPoint(&vtxs2, &face2, j, j2, r2));
				iss.push_back(is);
			}
		}
	}
	if (!iss.size()) return;
//	std::cout << "Find Intersections:" << std::endl;
//	std::cout << iss;
	//-----------------------------------------------------------
	//	見つかった交点，頂点を順番に並べる
	commonArea.push_back(iss.back());
	iss.back().bUsed = true;
	for(int i=0; i<iss.size(); ++i){
		//	次の点を探して追加する．
		if (commonArea.back().mp[1]){	//	最後の点が交点の場合
			//	2つ目の辺と同じ辺を持つ交点か，どちらかの頂点を探す．
			for(T2DIsects::iterator it = iss.begin(); it != iss.end(); ++it){
				if (it->bUsed) continue;
				if (it->mp[1]){	//	交点の場合
					if (SameLine(commonArea.back().mp[1], it->mp[0])){
						commonArea.push_back(*it);
						it->bUsed = true;
						break;
					}else if (SameLine(commonArea.back().mp[1], it->mp[1])){
						commonArea.push_back(T2DIsect(it->mp[1], it->mp[0]));
						it->bUsed = true;
						break;
					}
				}else{	//	頂点の場合
					if (it->mp[0].face != commonArea.back().mp[1].face) continue;
					if (it->mp[0].vtx[0] == commonArea.back().mp[1].vtx[0]){
						commonArea.push_back(*it);
						it->bUsed = true;
					}else if (it->mp[0].vtx[0] == commonArea.back().mp[1].vtx[1]){
						commonArea.push_back(*it);
						it->bUsed = true;
					}
				}
			}
		}else{	//	現在点が頂点の場合
			TFace& curFace = *commonArea.back().mp[0].face;
			int curVtx = commonArea.back().mp[0].vtx[0];
			int nextVtx=(curVtx+1) % curFace.size();
			int prevVtx=(curVtx-1+curFace.size()) % curFace.size();
			//	頂点を含む辺の交点か，隣の頂点を探す．
			for(T2DIsects::iterator it = iss.begin(); it != iss.end(); ++it){
				if (it->bUsed) continue;
				if (it->mp[1]){	//	交点の場合
					if (&curFace == it->mp[0].face && 
						(curVtx == it->mp[0].vtx[0] || curVtx == it->mp[0].vtx[1])){
						commonArea.push_back(*it);
						it->bUsed = true;
						break;
					}else if (&curFace == it->mp[1].face && 
						(curVtx == it->mp[1].vtx[0] || curVtx == it->mp[1].vtx[1])){
						commonArea.push_back(T2DIsect(it->mp[1], it->mp[0]));
						it->bUsed = true;
						break;
					}
				}else{	//	頂点の場合
					if (it->mp[0].face == &curFace && it->mp[0].vtx[0] == nextVtx){
						commonArea.push_back(*it);
						it->bUsed = true;
						break;
					}else if (it->mp[0].face == &curFace && it->mp[0].vtx[0] == prevVtx){
						commonArea.push_back(*it);
						it->bUsed = true;
						break;
					}
				}
			}
		}
	}
	if (commonArea.size()){
//		std::cout << "common area:" << std::endl;
//		std::cout << commonArea;
	}
}*/
	
/*	bool TCalcPenalty::InsideCheck(const Vec3d& p, const Vec3d& q1, const Vec3d& q2, const Vec3d& n){
		//	原点を通るnを法面に，頂点を落とす．
		Vec3d pn = p - (p*n)*n;
		Vec3d q1n = q1 - (q1*n)*n;
		Vec3d q2n = q2 - (q2*n)*n;
		double f = ((p-q1) ^ (q2-q1)) * n;
		if (f >= 0) return true;
		return false;
	}
	bool TCalcPenalty::FindIntersection(double& r1, const Vec3d& p1, const Vec3d&	q1,
		double& r2, const Vec3d& p2, const Vec3d& q2, const Vec3d& n){
		//	原点を通るnを法面に，頂点を落とす．
		Vec3d p1n = p1 - (p1*n)*n;
		Vec3d p2n = p2 - (p2*n)*n;
		Vec3d q1n = q1 - (q1*n)*n;
		Vec3d q2n = q2 - (q2*n)*n;
		
		Vec3d n1 = (q1-p1) ^ n;
		double lp2 = n1 * (p2n - p1n);
		double lq2 = n1 * (q2n - p1n);
		if (lq2 - lp2) r2 = lq2 / (lq2 - lp2);
		else r2 = 0.5f;
		
		Vec3d n2 = (q2-p2) ^ n;
		double lp1 = n2 * (p1n - p2n);
		double lq1 = n2 * (q1n - p2n);
		if (lq1 - lp1) r1 = lq1 / (lq1 - lp1);
		else r1 = 0.5f;

		if (0<r1 && r1<1 && 0<r2 && r2<1) return true;
		return false;
	}
*/

//----------------------------------------------------------------------------
//	TCalcPenaltySub

void TCalcPenaltySub::Draw(){
	//	力の表示
	if(force.norm() > 0){
		Vec3d p = (force ^ torque).unit() * (torque.norm() / force.norm());
		glBegin(GL_LINES);
		glVertex3dv(p+force);
		glVertex3dv(p-force);
		glEnd();
	}
}
void TCalcPenaltySub::CalcPenalty(){
	type=NONE;
	force = Vec3d();
	torque = Vec3d();
	area = 0.0f;
	if (poly1.state == TCollideInterface::FACE && poly2.state == TCollideInterface::FACE){
		for(TFaces::iterator it1 = poly1.begin(); it1 != poly1.end(); ++it1){
			for(TFaces::iterator it2 = poly2.begin(); it2 != poly2.end(); ++it2){
				//	共通部分を求める
				T2DIsects commonArea;
				FindCommonArea(commonArea, poly1.vertices, *it1, poly2.vertices, *it2, normal);
				if (commonArea.size() > 2){	//	共通部分が面の時，
					//	3角形に分割してペナルティを求める
					for(int i=1; i< commonArea.size()-1; i++){
						T2DIsects tri;
						tri.push_back(commonArea[0]);
						tri.push_back(commonArea[i]);
						tri.push_back(commonArea[i+1]);
						CalcTriPenalty(tri, it1, normal);
					}
				}
			}
		}
		type = FACE;
	}else if ((poly1.state == TCollideInterface::EDGE && poly2.state == TCollideInterface::FACE)
		||(poly1.state == TCollideInterface::FACE && poly2.state == TCollideInterface::EDGE)){
		for(TFaces::iterator it1 = poly1.begin(); it1 != poly1.end(); ++it1){
			for(TFaces::iterator it2 = poly2.begin(); it2 != poly2.end(); ++it2){
				//	共通部分を求める
				T2DIsects commonArea;
				if (poly1.state == TCollideInterface::EDGE){
					FindCommonLine(commonArea, poly1.vertices, *it1, poly2.vertices, *it2, normal);
					for(int i=0; i<commonArea.size(); i+=2){
						CalcEdgePenalty(commonArea[i], commonArea[i+1], normal, 1);
					}
				}else{
					FindCommonLine(commonArea, poly2.vertices, *it2, poly1.vertices, *it1, -normal);
					for(int i=0; i<(int)commonArea.size()-1; i++){
						CalcEdgePenalty(commonArea[i], commonArea[i+1], -normal, -1);
					}
				}
			}
		}
		type = EDGE;
	}else{
		CalcPointPenalty(poly1.point, poly2.point, normal);
		type = VERTEX;
	}
//	if(force.norm() > 0){
//		Vec3d p = (force ^ torque).unit() * (torque.norm() / force.norm());
//		std::cout << "pos:" << p << " f:" << force;
//		std::cout << " tq:" << torque << std::endl;
//	}
}
void TCalcPenaltySub::CalcTriPenalty(const T2DIsects& tri, TFace* face1, const Vec3d& normal){
	//	face1 から face2 に向かうベクトルがnormal
	//	World座標系での３頂点の位置と侵入量を求める．
	double depth[3];
	Vec3d coord[3];
	for(int i=0; i<3; i++){
		Vec3d oppFaceBase = (*tri[i].mp[1].coords).posture * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[0]];
		Vec3d oppFace1 = (*tri[i].mp[1].coords).posture * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[1]];
		Vec3d oppFace2 = (*tri[i].mp[1].coords).posture * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[2]];
		Vec3d oppFaceNormal = ((oppFace1-oppFaceBase) ^ (oppFace2-oppFaceBase)).unit();
		
		coord[i] = (*tri[i].mp[0].coords).posture * tri[i].mp[0].Coord();
		Vec3d oppPos = coord[i] - (coord[i]-oppFaceBase)*oppFaceNormal*oppFaceNormal;
		depth[i] = (coord[i] - oppPos) * normal;
		if (face1 != tri[i].mp[0].face) depth[i] *= -1;
	}
	coord[1] -= (coord[1] - coord[0]) * normal * normal;
	coord[2] -= (coord[2] - coord[0]) * normal * normal;
	for(int i=0; i<3; i++){
//		std::cout << "P" << i << ":" << coord[i] << "depth:" << depth[i] << std::endl;
	}
	
	//	侵入量が負の頂点の数で場合わけ
	int minusId[3], pulsId[3];
	int nMinus = 0, nPlus = 0;
	for(int i=0; i<3; i++){
		if (depth[i] < 0){
			minusId[nMinus] = i;
			nMinus++;
		}else{
			pulsId[nPlus] = i;
			nPlus++;
		}
	}
			
	Vec3d f, t;
	double wholeArea = ((coord[2] - coord[0]) ^ (coord[1] - coord[0])).norm()/8;
	if (nMinus == 3){
		//	全部負なら力は出ない
	}else if(nMinus == 2){
		//	正の部分だけを積分
		std::swap(coord[0], coord[pulsId[0]]);
		std::swap(depth[0], depth[pulsId[0]]);
		double ratio1 = depth[0] / (depth[0] - depth[1]);
		double ratio2 = depth[0] / (depth[0] - depth[2]);
		CalcForceTorque(f, t, ratio1, ratio2, coord, depth, normal);
		force += f; torque += t;
		area += wholeArea*ratio1*ratio2;
	}else if(nMinus == 1){
		//	まず全体を積分
		CalcForceTorque(f, t, 1, 1, coord, depth, normal);
		force += f; torque += t;
		
		//	負の部分だけを積分して引く
		std::swap(coord[0], coord[minusId[0]]);
		std::swap(depth[0], depth[minusId[0]]);
		double ratio1 = depth[0] / (depth[0] - depth[1]);
		double ratio2 = depth[0] / (depth[0] - depth[2]);
		Vec3d f2, t2;
		CalcForceTorque(f2, t2, ratio1, ratio2, coord, depth, normal);
		force -= f2; torque -= t2;
		area += wholeArea*(1-ratio1*ratio2);
	}else{
		//	全体を積分
		CalcForceTorque(f, t, 1, 1, coord, depth, normal);
		force += f; torque += t;
		area += wholeArea;
	}
}	
void TCalcPenaltySub::CalcEdgePenalty(const T2DIsect& p1, const T2DIsect& p2, const Vec3d& normal, double sign){
	Vec3d wp1 = p1.mp[0].coords->posture * p1.mp[0].Coord();
	Vec3d wp2 = p2.mp[0].coords->posture * p2.mp[0].Coord();
	Vec3d opp = p1.mp[1].coords->posture * (*p1.mp[1].coords)[(*p1.mp[1].face)[0]];
	Vec3d a = wp2 - wp1;
	double pen[2];
	pen[0]= (wp1 - opp)*normal;
	pen[1]= (wp2 - opp)*normal;
	double from = 0, to = 1;
	if (pen[0] <= 0 && pen[1] <= 0){
		return;
	}else if (pen[1] < 0){
		to = (pen[0] / pen[0] - pen[1]);
	}else if (pen[0] < 0){
		from = (pen[1] / pen[1] - pen[0]);
	}else{
		double c = (wp1 - opp)*normal;
		double f = (wp2 - opp)*normal - c;
		double s = to - from;
		double ss = to*to - from*from;
		double sss = to*to*to - from*from*from;
		force += sign * (c*s+0.5f*f*ss) * a.norm() * normal;
		torque += sign * ((wp1^force) + (a^normal) * a.norm() * (0.5f*c*ss+(1.0f/3.0f)*f*sss));
		area += a.norm()*s/2;
	}
}
void TCalcPenaltySub::CalcPointPenalty(Vec3d p1, Vec3d p2, const Vec3d& normal){
	force += p1-p2;
	torque += 0.5f*(p1+p2) ^ (p1-p2);
	area += 1.0f;
}
void TCalcPenaltySub::CalcForceTorque(Vec3d& f, Vec3d& t, double ra, double rb, Vec3d* coord, double* depth, const Vec3d& normal){
	Vec3d p = coord[0];
	Vec3d a = (coord[1] - coord[0]) * ra;
	double fa = (depth[1] - depth[0]) * ra;
	Vec3d b = (coord[2] - coord[0]) * rb;
	double fb = (depth[2] - depth[0]) * rb;
	double c = depth[0];
	if ((a^b) * normal < 0){
		std::swap(a, b);
		std::swap(fa, fb);
	}
//	std::cout << "p:" << p << " a:" << a << " b:" << b;
//	std::cout << " c" << c <<  "fa:" << fa << " fb:" << fb << std::endl;
	f = (a^b) * ( (1.0f/6.0f)*fa + (1.0f/6.0f)*fb + 0.5f*c );
	t = (p^f) + (a^(a^b))*( (1.0f/12.0f)*fa + (1.0f/24.0f)*fb + (1.0f/6.0f)*c )
		+ (b^(a^b))*( (1.0f/24.0f)*fa + (1.0f/12.0f)*fb + (1.0f/6.0f)*c );
}
//	Line ---------------------------------------------------------------------
struct CompForFindCommonLine{
	bool operator () (const T2DIsect& a, const T2DIsect& b) const{
		return a.mp[0].ratio > a.mp[1].ratio;
	}
};
void TCalcPenaltySub::FindCommonLine(T2DIsects& commonLine, const TCoords& vtxs1, TFace& edge1, const TCoords& vtxs2, TFace& face2, const Vec3d& n){
	assert(edge1.size() == 2);
	Vec3d p1 = vtxs1.posture * vtxs1[edge1[0]];
	Vec3d p2 = vtxs1.posture * vtxs1[edge1[1]];
	bool bInside1 = true, bInside2 = true;
	std::set<T2DIsect, CompForFindCommonLine> iss;
	for(int j=0; j<face2.size(); ++j){
		int j2 = (j+1) % face2.size();
		Vec3d q1 = vtxs2.posture * vtxs2[face2[j]];
		Vec3d q2 = vtxs2.posture * vtxs2[face2[j2]];
		//	交点を探す
		double r1, r2;
		if (FindIntersection(r1, p1, p2, r2, q1, q2, n)){
			T2DIsect is(TMidPoint(&vtxs1, &edge1, 0, 1, r1), TMidPoint(&vtxs2, &face2, j, j2, r2));
			iss.insert(is);
		}
		if (!InsideCheck(p1, q1, q2, n)) bInside1 = false;
		if (!InsideCheck(p2, q1, q2, n)) bInside2 = false;
	}
	//	中にある頂点を列挙
	if (bInside1) commonLine.push_back(T2DIsect(TMidPoint(&vtxs1, &edge1, 0, -1, 1), TMidPoint(&vtxs2, &face2)));
	for(std::set<T2DIsect, CompForFindCommonLine>::iterator it = iss.begin(); it != iss.end(); ++it){
		commonLine.push_back(*it);
	}
	if (bInside2) commonLine.push_back(T2DIsect(TMidPoint(&vtxs1, &edge1, 1, -1, 1), TMidPoint(&vtxs2, &face2)));
	if (commonLine.size()){
//		std::cout << "common line:" << std::endl;
//		std::cout << commonLine;
	}
}
	
//	Area ---------------------------------------------------------------------
void TCalcPenaltySub::FindCommonArea(T2DIsects& commonArea, const TCoords& vtxs1, TFace& face1, const TCoords& vtxs2, TFace& face2, const Vec3d& n){
//	std::cout << "F1:" << face1 << std::endl;
//	std::cout << "F2:" << face2 << std::endl;

	T2DIsects iss;
	//	中にある頂点を列挙1
	for(int i=0; i<face1.size(); ++i){
		int i2 = (i+1) % face1.size();
		Vec3d p1 = vtxs1.posture * vtxs1[face1[i]];
		Vec3d p2 = vtxs1.posture * vtxs1[face1[i2]];
		bool bInside= true;
		for(int j=0; j<face2.size(); ++j){
			int j2 = (j+1) % face2.size();
			Vec3d q1 = vtxs2.posture * vtxs2[face2[j]];
			Vec3d q2 = vtxs2.posture * vtxs2[face2[j2]];
			//	中にある頂点かチェック
			if (!InsideCheck(p1, q1, q2, n)) bInside = false;
		}
		//	中にある頂点を列挙
		if (bInside){
			iss.push_back(T2DIsect(TMidPoint(&vtxs1, &face1, i, -1, 1), TMidPoint(&vtxs2, &face2)));
		}
	}
	//	中にある頂点を列挙2
	for(int j=0; j<face2.size(); ++j){
		int j2 = (j+1) % face2.size();
		Vec3d q1 = vtxs2.posture * vtxs2[face2[j]];
		Vec3d q2 = vtxs2.posture * vtxs2[face2[j2]];
		bool bInside= true;
		for(int i=0; i<face1.size(); ++i){
			int i2 = (i+1) % face1.size();
			Vec3d p1 = vtxs1.posture * vtxs1[face1[i]];
			Vec3d p2 = vtxs1.posture * vtxs1[face1[i2]];
			//	中にある頂点かチェック
			if (!InsideCheck(q1, p1, p2, -n)) bInside = false;
		}
		//	中にある頂点を列挙
		if (bInside){
			iss.push_back(T2DIsect(TMidPoint(&vtxs2, &face2, j, -1, 1), TMidPoint(&vtxs1, &face1)));
		}
	}
	//	交点の列挙
	for(int i=0; i<face1.size(); ++i){
		int i2 = (i+1) % face1.size();
		Vec3d p1 = vtxs1.posture * vtxs1[face1[i]];
		Vec3d p2 = vtxs1.posture * vtxs1[face1[i2]];
		bool bInside= true;
		for(int j=0; j<face2.size(); ++j){
			int j2 = (j+1) % face2.size();
			Vec3d q1 = vtxs2.posture * vtxs2[face2[j]];
			Vec3d q2 = vtxs2.posture * vtxs2[face2[j2]];
			double r1, r2;
			//	交点を探す
			if (FindIntersection(r1, p1, p2, r2, q1, q2, n)){
				T2DIsect is(TMidPoint(&vtxs1, &face1, i, i2, r1), TMidPoint(&vtxs2, &face2, j, j2, r2));
				iss.push_back(is);
			}
		}
	}
	if (!iss.size()) return;
//	std::cout << "Find Intersections:" << std::endl;
//	std::cout << iss;
	//-----------------------------------------------------------
	//	見つかった交点，頂点を順番に並べる
	commonArea.push_back(iss.back());
	iss.back().bUsed = true;
	for(int i=0; i<iss.size(); ++i){
		//	次の点を探して追加する．
		if (commonArea.back().mp[1]){	//	最後の点が交点の場合
			//	2つ目の辺と同じ辺を持つ交点か，どちらかの頂点を探す．
			for(T2DIsects::iterator it = iss.begin(); it != iss.end(); ++it){
				if (it->bUsed) continue;
				if (it->mp[1]){	//	交点の場合
					if (SameLine(commonArea.back().mp[1], it->mp[0])){
						commonArea.push_back(*it);
						it->bUsed = true;
						break;
					}else if (SameLine(commonArea.back().mp[1], it->mp[1])){
						commonArea.push_back(T2DIsect(it->mp[1], it->mp[0]));
						it->bUsed = true;
						break;
					}
				}else{	//	頂点の場合
					if (it->mp[0].face != commonArea.back().mp[1].face) continue;
					if (it->mp[0].vtx[0] == commonArea.back().mp[1].vtx[0]){
						commonArea.push_back(*it);
						it->bUsed = true;
					}else if (it->mp[0].vtx[0] == commonArea.back().mp[1].vtx[1]){
						commonArea.push_back(*it);
						it->bUsed = true;
					}
				}
			}
		}else{	//	現在点が頂点の場合
			TFace& curFace = *commonArea.back().mp[0].face;
			int curVtx = commonArea.back().mp[0].vtx[0];
			int nextVtx=(curVtx+1) % curFace.size();
			int prevVtx=(curVtx-1+curFace.size()) % curFace.size();
			//	頂点を含む辺の交点か，隣の頂点を探す．
			for(T2DIsects::iterator it = iss.begin(); it != iss.end(); ++it){
				if (it->bUsed) continue;
				if (it->mp[1]){	//	交点の場合
					if (&curFace == it->mp[0].face && 
						(curVtx == it->mp[0].vtx[0] || curVtx == it->mp[0].vtx[1])){
						commonArea.push_back(*it);
						it->bUsed = true;
						break;
					}else if (&curFace == it->mp[1].face && 
						(curVtx == it->mp[1].vtx[0] || curVtx == it->mp[1].vtx[1])){
						commonArea.push_back(T2DIsect(it->mp[1], it->mp[0]));
						it->bUsed = true;
						break;
					}
				}else{	//	頂点の場合
					if (it->mp[0].face == &curFace && it->mp[0].vtx[0] == nextVtx){
						commonArea.push_back(*it);
						it->bUsed = true;
						break;
					}else if (it->mp[0].face == &curFace && it->mp[0].vtx[0] == prevVtx){
						commonArea.push_back(*it);
						it->bUsed = true;
						break;
					}
				}
			}
		}
	}
	if (commonArea.size()){
//		std::cout << "common area:" << std::endl;
//		std::cout << commonArea;
	}
}
	
	bool TCalcPenaltySub::InsideCheck(const Vec3d& p, const Vec3d& q1, const Vec3d& q2, const Vec3d& n){
		//	原点を通るnを法面に，頂点を落とす．
		Vec3d pn = p - (p*n)*n;
		Vec3d q1n = q1 - (q1*n)*n;
		Vec3d q2n = q2 - (q2*n)*n;
		double f = ((p-q1) ^ (q2-q1)) * n;
		if (f >= 0) return true;
		return false;
	}
	bool TCalcPenaltySub::FindIntersection(double& r1, const Vec3d& p1, const Vec3d&	q1,
		double& r2, const Vec3d& p2, const Vec3d& q2, const Vec3d& n){
		//	原点を通るnを法面に，頂点を落とす．
		Vec3d p1n = p1 - (p1*n)*n;
		Vec3d p2n = p2 - (p2*n)*n;
		Vec3d q1n = q1 - (q1*n)*n;
		Vec3d q2n = q2 - (q2*n)*n;
		
		Vec3d n1 = (q1-p1) ^ n;
		double lp2 = n1 * (p2n - p1n);
		double lq2 = n1 * (q2n - p1n);
		if (lq2 - lp2) r2 = lq2 / (lq2 - lp2);
		else r2 = 0.5f;
		
		Vec3d n2 = (q2-p2) ^ n;
		double lp1 = n2 * (p1n - p2n);
		double lq1 = n2 * (q1n - p2n);
		if (lq1 - lp1) r1 = lq1 / (lq1 - lp1);
		else r1 = 0.5f;

		if (0<r1 && r1<1 && 0<r2 && r2<1) return true;
		return false;
	}
