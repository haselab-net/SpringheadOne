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

void TCalcPenalty::Draw(){
	//	力の表示
	if(force.norm() > 0){
		Vec3f p = (force ^ torque).unit() * (torque.norm() / force.norm());
		glBegin(GL_LINES);
		glVertex3fv(p+force);
		glVertex3fv(p-force);
		glEnd();
	}
}
void TCalcPenalty::CalcPenalty(){
	type=NONE;
	force = Vec3f();
	torque = Vec3f();
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
//		Vec3f p = (force ^ torque).unit() * (torque.norm() / force.norm());
//		std::cout << "pos:" << p << " f:" << force;
//		std::cout << " tq:" << torque << std::endl;
//	}
}
void TCalcPenalty::CalcTriPenalty(const T2DIsects& tri, TFace* face1, const Vec3f& normal){
	//	face1 から face2 に向かうベクトルがnormal
	//	World座標系での３頂点の位置と侵入量を求める．
	float depth[3];
	Vec3f coord[3];
	for(int i=0; i<3; i++){
		Vec3f oppFaceBase = (*tri[i].mp[1].coords).posture * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[0]];
		Vec3f oppFace1 = (*tri[i].mp[1].coords).posture * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[1]];
		Vec3f oppFace2 = (*tri[i].mp[1].coords).posture * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[2]];
		Vec3f oppFaceNormal = ((oppFace1-oppFaceBase) ^ (oppFace2-oppFaceBase)).unit();
		
		coord[i] = (*tri[i].mp[0].coords).posture * tri[i].mp[0].Coord();
		Vec3f oppPos = coord[i] - (coord[i]-oppFaceBase)*oppFaceNormal*oppFaceNormal;
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
			
	Vec3f f, t;
	float wholeArea = ((coord[2] - coord[0]) ^ (coord[1] - coord[0])).norm()/8;
	if (nMinus == 3){
		//	全部負なら力は出ない
	}else if(nMinus == 2){
		//	正の部分だけを積分
		std::swap(coord[0], coord[pulsId[0]]);
		std::swap(depth[0], depth[pulsId[0]]);
		float ratio1 = depth[0] / (depth[0] - depth[1]);
		float ratio2 = depth[0] / (depth[0] - depth[2]);
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
		float ratio1 = depth[0] / (depth[0] - depth[1]);
		float ratio2 = depth[0] / (depth[0] - depth[2]);
		Vec3f f2, t2;
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
void TCalcPenalty::CalcEdgePenalty(const T2DIsect& p1, const T2DIsect& p2, const Vec3f& normal, float sign){
	Vec3f wp1 = p1.mp[0].coords->posture * p1.mp[0].Coord();
	Vec3f wp2 = p2.mp[0].coords->posture * p2.mp[0].Coord();
	Vec3f opp = p1.mp[1].coords->posture * (*p1.mp[1].coords)[(*p1.mp[1].face)[0]];
	Vec3f a = wp2 - wp1;
	float pen[2];
	pen[0]= (wp1 - opp)*normal;
	pen[1]= (wp2 - opp)*normal;
	float from = 0, to = 1;
	if (pen[0] <= 0 && pen[1] <= 0){
		return;
	}else if (pen[1] < 0){
		to = (pen[0] / pen[0] - pen[1]);
	}else if (pen[0] < 0){
		from = (pen[1] / pen[1] - pen[0]);
	}else{
		float c = (wp1 - opp)*normal;
		float f = (wp2 - opp)*normal - c;
		float s = to - from;
		float ss = to*to - from*from;
		float sss = to*to*to - from*from*from;
		force += sign * (c*s+0.5f*f*ss) * a.norm() * normal;
		torque += sign * ((wp1^force) + (a^normal) * a.norm() * (0.5f*c*ss+(1.0f/3.0f)*f*sss));
		area += a.norm()*s/2;
	}
}
void TCalcPenalty::CalcPointPenalty(Vec3d p1, Vec3d p2, const Vec3f& normal){
	force += p1-p2;
	torque += 0.5f*(p1+p2) ^ (p1-p2);
	area += 1.0f;
}
void TCalcPenalty::CalcForceTorque(Vec3f& f, Vec3f& t, float ra, float rb, Vec3f* coord, float* depth, const Vec3f& normal){
	Vec3f p = coord[0];
	Vec3f a = (coord[1] - coord[0]) * ra;
	float fa = (depth[1] - depth[0]) * ra;
	Vec3f b = (coord[2] - coord[0]) * rb;
	float fb = (depth[2] - depth[0]) * rb;
	float c = depth[0];
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
void TCalcPenalty::FindCommonLine(T2DIsects& commonLine, const TCoords& vtxs1, TFace& edge1, const TCoords& vtxs2, TFace& face2, const Vec3f& n){
	assert(edge1.size() == 2);
	Vec3f p1 = vtxs1.posture * vtxs1[edge1[0]];
	Vec3f p2 = vtxs1.posture * vtxs1[edge1[1]];
	bool bInside1 = true, bInside2 = true;
	std::set<T2DIsect, CompForFindCommonLine> iss;
	for(int j=0; j<face2.size(); ++j){
		int j2 = (j+1) % face2.size();
		Vec3f q1 = vtxs2.posture * vtxs2[face2[j]];
		Vec3f q2 = vtxs2.posture * vtxs2[face2[j2]];
		//	交点を探す
		float r1, r2;
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
void TCalcPenalty::FindCommonArea(T2DIsects& commonArea, const TCoords& vtxs1, TFace& face1, const TCoords& vtxs2, TFace& face2, const Vec3f& n){
//	std::cout << "F1:" << face1 << std::endl;
//	std::cout << "F2:" << face2 << std::endl;

	T2DIsects iss;
	//	中にある頂点を列挙1
	for(int i=0; i<face1.size(); ++i){
		int i2 = (i+1) % face1.size();
		Vec3f p1 = vtxs1.posture * vtxs1[face1[i]];
		Vec3f p2 = vtxs1.posture * vtxs1[face1[i2]];
		bool bInside= true;
		for(int j=0; j<face2.size(); ++j){
			int j2 = (j+1) % face2.size();
			Vec3f q1 = vtxs2.posture * vtxs2[face2[j]];
			Vec3f q2 = vtxs2.posture * vtxs2[face2[j2]];
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
		Vec3f q1 = vtxs2.posture * vtxs2[face2[j]];
		Vec3f q2 = vtxs2.posture * vtxs2[face2[j2]];
		bool bInside= true;
		for(int i=0; i<face1.size(); ++i){
			int i2 = (i+1) % face1.size();
			Vec3f p1 = vtxs1.posture * vtxs1[face1[i]];
			Vec3f p2 = vtxs1.posture * vtxs1[face1[i2]];
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
		Vec3f p1 = vtxs1.posture * vtxs1[face1[i]];
		Vec3f p2 = vtxs1.posture * vtxs1[face1[i2]];
		bool bInside= true;
		for(int j=0; j<face2.size(); ++j){
			int j2 = (j+1) % face2.size();
			Vec3f q1 = vtxs2.posture * vtxs2[face2[j]];
			Vec3f q2 = vtxs2.posture * vtxs2[face2[j2]];
			float r1, r2;
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
	
	bool TCalcPenalty::InsideCheck(const Vec3f& p, const Vec3f& q1, const Vec3f& q2, const Vec3f& n){
		//	原点を通るnを法面に，頂点を落とす．
		Vec3f pn = p - (p*n)*n;
		Vec3f q1n = q1 - (q1*n)*n;
		Vec3f q2n = q2 - (q2*n)*n;
		float f = ((p-q1) ^ (q2-q1)) * n;
		if (f >= 0) return true;
		return false;
	}
	bool TCalcPenalty::FindIntersection(float& r1, const Vec3f& p1, const Vec3f&	q1,
		float& r2, const Vec3f& p2, const Vec3f& q2, const Vec3f& n){
		//	原点を通るnを法面に，頂点を落とす．
		Vec3f p1n = p1 - (p1*n)*n;
		Vec3f p2n = p2 - (p2*n)*n;
		Vec3f q1n = q1 - (q1*n)*n;
		Vec3f q2n = q2 - (q2*n)*n;
		
		Vec3f n1 = (q1-p1) ^ n;
		float lp2 = n1 * (p2n - p1n);
		float lq2 = n1 * (q2n - p1n);
		if (lq2 - lp2) r2 = lq2 / (lq2 - lp2);
		else r2 = 0.5f;
		
		Vec3f n2 = (q2-p2) ^ n;
		float lp1 = n2 * (p1n - p2n);
		float lq1 = n2 * (q1n - p2n);
		if (lq1 - lp1) r1 = lq1 / (lq1 - lp1);
		else r1 = 0.5f;

		if (0<r1 && r1<1 && 0<r2 && r2<1) return true;
		return false;
	}
