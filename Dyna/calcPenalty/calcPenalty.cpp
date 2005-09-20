#include "calcPenalty.h"
#include <glut.h>

void TPolygons::Draw(){
	for(TFaces::iterator it = faces.begin(); it != faces.end(); ++it){
		glBegin(GL_LINES);
		for(int v=0; v != it->size(); ++v){
			TFace& face=*it;
			glVertex3fv(vtxs.af * vtxs[face[v]]);
			glVertex3fv(vtxs.af * vtxs[face[(v+1)%face.size()]]);
		}
		glEnd();
	}
}
void TMidPoint::Print(std::ostream& os) const {
	if (vtx[0] >= 0){
		if (vtx[1] >= 0){
			os << ratio << "*" << coords->id << vtx[0] << (*coords)[vtx[0]] << " + ";
			os << (1-ratio) << "*" << coords->id << vtx[1] << (*coords)[vtx[1]];
		}else{
			os << ratio << "*" << coords->id << vtx[0] << (*coords)[vtx[0]];
		}
	}else{
		os << "NON";
	}
}

//----------------------------------------------------------------------------
//	TCalcPenalty

void TCalcPenalty::Draw(){
	//	�|���S���̕\��
	polys[0].Draw();
	polys[1].Draw();

		//	�͂̕\��
		if(force.norm() > 0){
			Vec3f p = (force ^ torque).unit() * (torque.norm() / force.norm());
			glBegin(GL_LINES);
			glVertex3fv(p+force);
			glVertex3fv(p-force);
			glEnd();
		}
	}
	void TCalcPenalty::CalcPenalty(){
		for(TFaces::iterator it1 = polys[0].faces.begin(); it1 != polys[0].faces.end(); ++it1){
			for(TFaces::iterator it2 = polys[1].faces.begin(); it2 != polys[1].faces.end(); ++it2){
				//	���ʕ��������߂�
				T2DIsects commonArea;
				FindCommonArea(commonArea, polys[0].vtxs, *it1, polys[1].vtxs, *it2, normal);
				if (commonArea.size() > 2){	//	���ʕ������ʂ̎��C
					//	3�p�`�ɕ������ăy�i���e�B�����߂�
					for(int i=1; i< commonArea.size()-1; i++){
						T2DIsects tri;
						tri.push_back(commonArea[0]);
						tri.push_back(commonArea[i]);
						tri.push_back(commonArea[i+1]);
						CalcTriPenalty(force, torque, tri, it1, normal);
					}
				}else if (commonArea.size() == 2){	//	���ʕ������ӂ̎��C
				}else if (commonArea.size() == 1){	//	���ʕ�����1�_�̎�
				}
			}
		}
		if(force.norm() > 0){
			Vec3f p = (force ^ torque).unit() * (torque.norm() / force.norm());
			std::cout << "pos:" << p << " f:" << force;
			std::cout << " tq:" << torque << std::endl;
		}
	}
	void TCalcPenalty::CalcTriPenalty(Vec3f& force, Vec3f& torque, const T2DIsects& tri, TFace* face1, const Vec3f& normal){
		//	face1 ���� face2 �Ɍ������x�N�g����normal
		//	World���W�n�ł̂R���_�̈ʒu�ƐN���ʂ����߂�D
		float depth[3];
		Vec3f coord[3];
		for(int i=0; i<3; i++){
			Vec3f oppFaceBase = (*tri[i].mp[1].coords).af * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[0]];
			Vec3f oppFace1 = (*tri[i].mp[1].coords).af * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[1]];
			Vec3f oppFace2 = (*tri[i].mp[1].coords).af * (*tri[i].mp[1].coords)[(*tri[i].mp[1].face)[2]];
			Vec3f oppFaceNormal = ((oppFace1-oppFaceBase) ^ (oppFace2-oppFaceBase)).unit();

			coord[i] = (*tri[i].mp[0].coords).af * tri[i].mp[0].Coord();
			Vec3f oppPos = coord[i] - (coord[i]-oppFaceBase)*oppFaceNormal*oppFaceNormal;
			depth[i] = (coord[i] - oppPos) * normal;
			if (face1 != tri[i].mp[0].face) depth[i] *= -1;
		}
		coord[1] -= (coord[1] - coord[0]) * normal * normal;
		coord[2] -= (coord[2] - coord[0]) * normal * normal;
		for(int i=0; i<3; i++){
			std::cout << "P" << i << ":" << coord[i] << "depth:" << depth[i] << std::endl;
		}

		//	�N���ʂ����̒��_�̐��ŏꍇ�킯
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
		if (nMinus == 3){
			//	�S�����Ȃ�͂͏o�Ȃ�
		}else if(nMinus == 2){
			//	���̕���������ϕ�
			std::swap(coord[0], coord[pulsId[0]]);
			std::swap(depth[0], depth[pulsId[0]]);
			float ratio1 = depth[0] / (depth[0] - depth[1]);
			float ratio2 = depth[0] / (depth[0] - depth[2]);
			CalcForceTorque(f, t, ratio1, ratio2, coord, depth, normal);
			force += f; torque += t;
		}else if(nMinus == 1){
			//	�܂��S�̂�ϕ�
			CalcForceTorque(f, t, 1, 1, coord, depth, normal);
			force += f; torque += t;

			//	���̕���������ϕ����Ĉ���
			std::swap(coord[0], coord[minusId[0]]);
			std::swap(depth[0], depth[minusId[0]]);
			float ratio1 = depth[0] / (depth[0] - depth[1]);
			float ratio2 = depth[0] / (depth[0] - depth[2]);
			Vec3f f2, t2;
			CalcForceTorque(f2, t2, ratio1, ratio2, coord, depth, normal);
			force -= f2; torque -= t2;
		}else{
			//	�S�̂�ϕ�
			CalcForceTorque(f, t, 1, 1, coord, depth, normal);
			force += f; torque += t;
		}
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
		std::cout << "p:" << p << " a:" << a << " b:" << b;
		std::cout << " c" << c <<  "fa:" << fa << " fb:" << fb << std::endl;
		f = (a^b) * ( (1.0f/6.0f)*fa + (1.0f/6.0f)*fb + 0.5f*c );
		t = (p^f) + (a^(a^b))*( (1.0f/12.0f)*fa + (1.0f/24.0f)*fb + (1.0f/6.0f)*c )
					+ (b^(a^b))*( (1.0f/24.0f)*fa + (1.0f/12.0f)*fb + (1.0f/6.0f)*c );
	}

	
	void TCalcPenalty::FindCommonArea(T2DIsects& commonArea, TCoords& vtxs1, TFace& face1, TCoords& vtxs2, TFace& face2, const Vec3f& n){
		T2DIsects iss;
		//	���ɂ��钸�_���1
		for(int i=0; i<face1.size(); ++i){
			int i2 = (i+1) % face1.size();
			Vec3f p1 = vtxs1.af * vtxs1[face1[i]];
			Vec3f p2 = vtxs1.af * vtxs1[face1[i2]];
			bool bInside= true;
			for(int j=0; j<face2.size(); ++j){
				int j2 = (j+1) % face2.size();
				Vec3f q1 = vtxs2.af * vtxs2[face2[j]];
				Vec3f q2 = vtxs2.af * vtxs2[face2[j2]];
				//	���ɂ��钸�_���`�F�b�N
				if (!InsideCheck(p1, q1, q2, n)) bInside = false;
			}
			//	���ɂ��钸�_���
			if (bInside){
				iss.push_back(T2DIsect(TMidPoint(&vtxs1, &face1, i, -1, 1), TMidPoint(&vtxs2, &face2)));
			}
		}
		//	���ɂ��钸�_���2
		for(int j=0; j<face2.size(); ++j){
			int j2 = (j+1) % face2.size();
			Vec3f q1 = vtxs2.af * vtxs2[face2[j]];
			Vec3f q2 = vtxs2.af * vtxs2[face2[j2]];
			bool bInside= true;
			for(int i=0; i<face1.size(); ++i){
				int i2 = (i+1) % face1.size();
				Vec3f p1 = vtxs1.af * vtxs1[face1[i]];
				Vec3f p2 = vtxs1.af * vtxs1[face1[i2]];
				//	���ɂ��钸�_���`�F�b�N
				if (!InsideCheck(q1, p1, p2, n)) bInside = false;
			}
			//	���ɂ��钸�_���
			if (bInside){
				iss.push_back(T2DIsect(TMidPoint(&vtxs2, &face2, j, -1, 1), TMidPoint(&vtxs1, &face1)));
			}
		}
		//	��_�̗�
		for(int i=0; i<face1.size(); ++i){
			int i2 = (i+1) % face1.size();
			Vec3f p1 = vtxs1.af * vtxs1[face1[i]];
			Vec3f p2 = vtxs1.af * vtxs1[face1[i2]];
			bool bInside= true;
			for(int j=0; j<face2.size(); ++j){
				int j2 = (j+1) % face2.size();
				Vec3f q1 = vtxs2.af * vtxs2[face2[j]];
				Vec3f q2 = vtxs2.af * vtxs2[face2[j2]];
				float r1, r2;
				//	��_��T��
				if (FindIntersection(r1, p1, p2, r2, q1, q2, n)){
					T2DIsect is(TMidPoint(&vtxs1, &face1, i, i2, r1), TMidPoint(&vtxs2, &face2, j, j2, r2));
					iss.push_back(is);
				}
			}
		}
		std::cout << "Find Intersections:" << std::endl;
		std::cout << iss;
		//-----------------------------------------------------------
		//	����������_�C���_�����Ԃɕ��ׂ�
		commonArea.push_back(iss.back());
		iss.back().bUsed = true;
		for(int i=0; i<iss.size(); ++i){
			//	���̓_��T�����Ēǉ�����D
			if (commonArea.back().mp[1]){	//	�Ō�̓_����_�̏ꍇ
				T2DIsects::iterator it;
				//	2�ڂ̕ӂƓ����ӂ�����_���C�ǂ��炩�̒��_��T���D
				for(it = iss.begin(); it != iss.end(); ++it){
					if (it->bUsed) continue;
					if (it->mp[1]){	//	��_�̏ꍇ
						if (commonArea.back().mp[1] == it->mp[0]){
							commonArea.push_back(*it);
							it->bUsed = true;
							break;
						}else if (commonArea.back().mp[1] == it->mp[1]){
							commonArea.push_back(T2DIsect(it->mp[1], it->mp[0]));
							it->bUsed = true;
							break;
						}
					}else{	//	���_�̏ꍇ
						if (it->mp[0].face != commonArea.back().mp[1].face) continue;
						if (it->mp[0].vtx[0] == commonArea.back().mp[1].vtx[0]){
							commonArea.push_back(*it);
							it->bUsed = true;
						}
						if (it->mp[0].vtx[0] == commonArea.back().mp[1].vtx[1]){
							commonArea.push_back(*it);
							it->bUsed = true;
						}
					}
				}
			}else{	//	���ݓ_�����_�̏ꍇ
				TFace& curFace = *commonArea.back().mp[0].face;
				int curVtx = commonArea.back().mp[0].vtx[0];
				int prevVtx=0, nextVtx=0;
				for(int i=0; i<curFace.size(); ++i){
					if (curFace[i] == curVtx){
						prevVtx = (curVtx-1+curFace.size()) % curFace.size();
						nextVtx = (curVtx+1) % curFace.size();
						break;
					}
				}
				//	���_���܂ޕӂ̌�_���C�ׂ̒��_��T���D
				for(T2DIsects::iterator it = iss.begin(); it != iss.end(); ++it){
					if (it->bUsed) continue;
					if (it->mp[1]){	//	��_�̏ꍇ
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
					}else{	//	���_�̏ꍇ
						if (it->mp[0].face != &curFace) continue;
						if (it->mp[0].vtx[0] == nextVtx || it->mp[0].vtx[0] == prevVtx){
							commonArea.push_back(*it);
							it->bUsed = true;
						}
					}
				}
			}
		}
		std::cout << "common area:" << std::endl;
		std::cout << commonArea;
	}

	bool TCalcPenalty::InsideCheck(const Vec3f& p, const Vec3f& q1, const Vec3f& q2, const Vec3f& n){
		//	���_��ʂ�n��@�ʂɁC���_�𗎂Ƃ��D
		Vec3f pn = p - (p*n)*n;
		Vec3f q1n = q1 - (q1*n)*n;
		Vec3f q2n = q2 - (q2*n)*n;
		if (((p-q1) ^ (q2-q1)) * n >= 0) return true;
		return false;
	}
	bool TCalcPenalty::FindIntersection(float& r1, const Vec3f& p1, const Vec3f&	q1,
		float& r2, const Vec3f& p2, const Vec3f& q2, const Vec3f& n){
		//	���_��ʂ�n��@�ʂɁC���_�𗎂Ƃ��D
		Vec3f p1n = p1 - (p1*n)*n;
		Vec3f p2n = p2 - (p2*n)*n;
		Vec3f q1n = q1 - (q1*n)*n;
		Vec3f q2n = q2 - (q2*n)*n;
		
		Vec3f n1 = (q1-p1) ^ n;
		float lp2 = n1 * (p2n - p1n);
		float lq2 = n1 * (q2n - p1n);
		r2 = lq2 / (lq2 - lp2);
		
		Vec3f n2 = (q2-p2) ^ n;
		float lp1 = n2 * (p1n - p2n);
		float lq1 = n2 * (q1n - p2n);
		r1 = lq1 / (lq1 - lp1);

		if (0<r1 && r1<1 && 0<r2 && r2<1) return true;
		return false;
	}
