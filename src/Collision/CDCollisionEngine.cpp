#include "Collision.h"
#pragma hdrstop
#include <Graphics/GRMaterial.h>

namespace Spr{;
	
SGOBJECTIMP(CDCollisionEngine, SGBehaviorEngine);
CDCollisionEngine::CDCollisionEngine():nFrameRecords(0), nFramePairRecords(0), nConvexPairRecords(0){
}

int CDCollisionEngine::ReserveFrameRecord(){
	nFrameRecords ++;
	for(Frames::iterator it = frames.begin(); it != frames.end(); ++it){
		(*it)->records.resize(nFrameRecords);
	}
	return nFrameRecords-1;
}
int CDCollisionEngine::ReserveConvexPairRecord(){
	nConvexPairRecords ++;
	for(TConvexIt it = ConvexPairBegin(); it != ConvexPairEnd(); ++it){
		CDConvexPairWithRecord* cp = *it;
		cp->records.resize(nConvexPairRecords);
	}
	return nConvexPairRecords-1;
}
int CDCollisionEngine::ReserveFramePairRecord(){
	nFramePairRecords ++;
	for(CDFramePairWithRecords::iterator itf = pairs.begin(); itf != pairs.end(); ++itf){
		if (*itf) (*itf)->records.resize(nFramePairRecords);
	}
	return nFramePairRecords-1;
}
void CDCollisionEngine::AddFrame(SGFrame* frame, CDUserRecord* rec, int pos){
	CDFrame* cf=NULL;
	for(Frames::iterator it = frames.begin(); it != frames.end(); ++it){
		if ((*it)->frame == frame){
			cf = *it;
		}
	}
	if (!cf){
		frames.push_back(new CDFrame);
		cf = frames.back();
		cf->frame = frame;
		cf->records.resize(nFrameRecords);
		frameMap[frame] = frames.size()-1;
	}
	if (pos>=0) cf->records[pos] = rec;
}
int CDCollisionEngine::AddListener(CDCollisionListener* l){
	listeners.push_back(l);
	for(CDFramePairWithRecords::iterator it = pairs.begin(); it != pairs.end(); ++it){
		if (*it) (*it)->isActive.resize(listeners.size());
	}
	return (int)listeners.size()-1;
}

bool CDCollisionEngine::AddInactive(int f1, int f2, int pos){
	if (f1<0 || f1>(int)NFrame() || f2<0 || f2>(int)NFrame()) return false;
	if (f1 < pairs.height() && f2 < pairs.width()){
		//	���łɏ������ς݂ŁC�t���[���y�A������ꍇ�D
		if (f1 > f2) std::swap(f1, f2);
		pairs.item(f1, f2)->isActive[pos] = false;
		pairs.item(f1, f2)->isActive.Update();
	}
	//	���������Ȃ̂ŁC�t���[���y�A���Ȃ��̂ŁC�L�^�������Ă���
	inactiveList.push_back(ActivePair());
	inactiveList.back().frame[0] = f1;
	inactiveList.back().frame[1] = f2;
	inactiveList.back().pos = pos;
	return true;
}
bool CDCollisionEngine::AddActive(int f1, int f2, int pos){
	if (f1<0 || f1>(int)NFrame() || f2<0 || f2>(int)NFrame()) return false;
	if (f1 < pairs.height() && f2 < pairs.width()){
		//	���łɏ������ς݂ŁC�t���[���y�A������ꍇ�D
		if (f1 > f2) std::swap(f1, f2);
		assert(pos < (int) pairs.item(f1, f2)->isActive.size());
		pairs.item(f1, f2)->isActive[pos] = true;
		pairs.item(f1, f2)->isActive.Update();
	}
	activeList.push_back(ActivePair());
	activeList.back().frame[0] = f1;
	activeList.back().frame[1] = f2;
	activeList.back().pos = pos;
	return true;
}
int CDCollisionEngine::ClearActive(int pos){
	int num = 0;
	for(TActivePairs::iterator it=activeList.begin(); it != activeList.end(); ){
		if (it->pos == pos){
			it = activeList.erase(it);
			num ++;
		}else{
			++it;
		}
	}
	return num;
}
int CDCollisionEngine::ClearInactive(int pos){
	int num = 0;
	for(TActivePairs::iterator it=inactiveList.begin(); it != inactiveList.end(); ){
		if (it->pos == pos){
			it = inactiveList.erase(it);
		}else{
			++it;
			num ++;
		}
	}
	return num;
}


/**	Frame�̒[�̈ʒu�������C�\�[�g��������	*/
struct CDFrameEdge{
	float edge;				///<	�[�̈ʒu(�O���[�o���n)
	int frame;				///<	���� frame�̈ʒu
	bool bMin;				///<	���[�Ȃ�true
	bool operator < (const CDFrameEdge& s) const { return edge < s.edge; }
};

void CDCollisionEngine::Clear(SGScene* s){
	for(CDFramePairWithRecords::iterator it = pairs.begin(); it != pairs.end(); ++it){
		if(*it) (*it)->Clear();
	}
	pairs.clear();
	frames.clear();
	frameMap.clear();
	nFrameRecords = -1;
	nConvexPairRecords = -1;
}
void CDCollisionEngine::Init(){

	//	�f�t�H���g�Ŕ�������邩�ݒ肷��D
	//	activeList������ꍇ�̓f�t�H���g�Ŕ��肵�Ȃ��D
	//	�Ȃ��ꍇ�́C�f�t�H���g�Ŕ��肷��
	std::vector<bool> defaults;
	defaults.resize(listeners.size(), true);
	for(unsigned i=0; i<activeList.size(); ++i){
		defaults[activeList[i].pos] = false;
	}
	//	pairs�̃T�C�Y��Ώۃt���[���̐��ɂ��킹�čL����
	unsigned oldSz = pairs.height();
	pairs.resize(frames.size(), frames.size());
	//	�L���������Ƀt���[����ݒ肷��D
	for(unsigned i=0; i<frames.size(); ++i){
		for(unsigned j=i+1; j<frames.size(); ++j){
			UTRef<CDFramePairWithRecord>& pair = pairs.item(i,j);
			if (i>=oldSz || j>=oldSz){
				assert(!pair);
				pair = new CDFramePairWithRecord;
				pair->Set(frames[i], frames[j]);
			}
			//	���O�t���O�̃T�C�Y�����X�i�̐��ɂ��킹��
			unsigned isActiveSize = pair->isActive.size();
			pair->isActive.resize(listeners.size());
			assert(defaults.size() == pair->isActive.size());
			for(unsigned i=isActiveSize; i<pair->isActive.size(); ++i){
				pair->isActive[i] = defaults[i];
			}
			//	���[�U���R�[�h�̐������킹��
			pair->records.resize(nFramePairRecords);
		}
	}
	//	����Ώۂ̃t���[���̃y�A�ɔ���t���O��ݒ�
	for(unsigned i=0; i<activeList.size(); ++i){
		ActivePair& a = activeList[i];
		if (a.frame[0] > a.frame[1]) std::swap(a.frame[0] , a.frame[1]);
		pairs.item(a.frame[0], a.frame[1])->isActive[a.pos] = true;
	}
	//	���菜�O���X�g�̃t���[���ɏ��O�t���O��ݒ�
	for(unsigned i=0; i<inactiveList.size(); ++i){
		ActivePair& ia = inactiveList[i];
		if (ia.frame[0] > ia.frame[1]) std::swap(ia.frame[0] , ia.frame[1]);
		pairs.item(ia.frame[0], ia.frame[1])->isActive[ia.pos] = false;
	}
	//	���O�t���O�̐ݒ�
	for(unsigned int i=0; i<frames.size(); ++i){
		for(unsigned int j=i+1; j<frames.size(); ++j){
			UTRef<CDFramePairWithRecord>& pair = pairs.item(i,j);
			pair->isActive.Update();
		}
	}

/*
	DSTR << "CDCollision::Init:" << std::endl;
	for(unsigned i=0; i<frames.size(); ++i){
		for(unsigned j=i+1; j<frames.size(); ++j){
			CDFramePairWithRecord& pair = *pairs.item(i,j);
			DSTR << pair.frame[0]->frame->GetName() << " - " << pair.frame[1]->frame->GetName() << std::endl;
			for(int k=0; k<pair.isActive.size(); ++k){
				DSTR << " " << pair.isActive[k];
			}
			DSTR << std::endl;
		}
	}
	DSTR << "-------------------------------------------" << std::endl;
	for(unsigned i=0; i<frames.size(); ++i){
		for(unsigned j=0; j<frames.size(); ++j){
			DSTR << (pairs.item(i, j) ? "o" : "_");
		}
		DSTR << std::endl;
	}
	DSTR << "-------------------------------------------" << std::endl;
*/

	//	�ʌ`��̂��Ƃ̃��[�U���R�[�h�̐���ݒ�
	TConvexIt itEnd = ConvexPairEnd();
	for(TConvexIt it = ConvexPairBegin(); it != ConvexPairEnd(); ++it){
		CDConvexPairWithRecord* cp = *it;
		cp->records.resize(nConvexPairRecords);
	}
	for(Frames::iterator it = frames.begin(); it != frames.end(); ++it){
		(*it)->records.resize(nFrameRecords);
	}
}
void CDCollisionEngine::Step(SGScene* scene){
	listeners.BeforeAll(scene);
#if 1
	//	�����̒[����ׂ����X�g�����C�\�[�g����D
	Vec3f dir(1,0,0);
	typedef std::vector<CDFrameEdge> Edges;
	Edges edges;
	edges.resize(frames.size()*2);
	Edges::iterator eit = edges.begin();
	for(unsigned int i = 0; i < frames.size(); ++i){
		frames[i]->frame->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		if (!_finite(eit[0].edge) || !_finite(eit[1].edge)){
			DSTR << frames[i]->frame->GetName() << " min:" << eit[0].edge << " max:" << eit[1].edge << std::endl;
			frames[i]->frame->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		}
		eit[0].frame = i; eit[0].bMin = true;
		eit[1].frame = i; eit[1].bMin = false;
		eit += 2;
	}
	if (edges.size() > 200){
		DSTR << "strange edges" << std::endl;
		DSTR << (DWORD)&*edges.begin() << (DWORD)&*edges.end();
	}
	std::sort(edges.begin(), edges.end());
	//	�[���猩�Ă����āC�ڐG�̉\��������m�[�h�̔��������D
	typedef std::set<int> FrameSet;
	FrameSet cur;							//	���݂�Solid�̃Z�b�g
	for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
		if (it->bMin){						//	���[��������C���X�g���̕��̂Ɣ���
//			DSTR << "Col " << it->solid << " - ";
			for(FrameSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
				int f1 = it->frame;
				int f2 = *itf;
				if (f1 > f2) std::swap(f1, f2);
				UTRef<CDFramePairWithRecord>& pair =  pairs[f1*frames.size() + f2];
				//	�t���[���ƃt���[���̏Փ˔���
				timerGjk.Start();
				if (pair->IsActive() && pair->Detect(scene->GetCount())){	//	�ċA�I�ɏՓ˔���(GJK���g�p)
					timerGjk.Stop();
					listeners.Before(scene, pair);
					for(CDIntersections::iterator it = pair->intersections.begin(); it != pair->intersections.end(); ++it){
						timerQhull.Start();
						analyzer.FindIntersection(*it->convexPair, it->geometryPair->posture);	//	�ڐG�`��̉��
						timerQhull.Stop();
						timerNormal.Start();
						analyzer.CalcNormal(*(CDConvexPairWithNormal*)it->convexPair, it->geometryPair->posture);		//	�@���x�N�g���̌v�Z
						timerNormal.Stop();
						listeners.Analyzed(scene, pair, it->geometryPair, (CDConvexPairWithRecord*) it->convexPair, &analyzer);
					}
					listeners.After(scene, pair);
				}
				timerGjk.Stop();
//				DSTR << " " << *itf;
			}
//			DSTR << std::endl;
			cur.insert(it->frame);
		}else{
			cur.erase(it->frame);			//	�I�[�Ȃ̂œ��Y�t���[�����폜�D
		}
	}
#else
	for(int i=0; i<pairs.height(); ++i){
		for(int j=i+1; j<pairs.width(); ++j){
			CDFramePairWithRecord& pair =  pairs.item(i, j);
			//	�t���[���ƃt���[���̏Փ˔���
			timerGjk.Start();
			if (pair.IsActive() && pair.Detect(count)){	//	�ċA�I�ɏՓ˔���(GJK���g�p)
				timerGjk.Stop();
				listeners.Before(scene, &pair);
				for(CDIntersections::iterator it = pair.intersections.begin(); it != pair.intersections.end(); ++it){
					//	DSTR << pair.frame[0]->frame->GetName() << "-" << pair.frame[1]->frame->GetName() << "  ";
					timerQhull.Start();
					analyzer.FindIntersection(*it->convexPair, it->geometryPair->posture);	//	�ڐG�`��̉��
					timerQhull.Stop();
					timerNormal.Start();
					analyzer.CalcNormal(*(CDConvexPairWithNormal*)it->convexPair, it->geometryPair->posture);		//	�@���x�N�g���̌v�Z
					timerNormal.Stop();
					listeners.Analyzed(scene, &pair, it->geometryPair, (CDConvexPairWithRecord*) it->convexPair, &analyzer);
				}
				listeners.After(scene, &pair);
			}
			timerGjk.Stop();
		}
	}
#endif
	listeners.AfterAll(scene);
}


void CDCollisionEngine::RayCheck(Vec3f from, Vec3f dir){
/*	float depth = 1e20;
	for(Frames::iterator it = frames.begin(); it != frames.end() ++it){
		CDFrame& frame = **it;
		float d = frame->RayCheck(from, dir);
		if (d < depth){
			depth = d;

		}
	}
*/
}



class CDCollisionEngineState:public SGBehaviorState{
public:
	SGOBJECTDEF(CDCollisionEngineState);
	struct TConvexPairState{
		CDConvexPair::State state;		///<	�ڐG���
		Vec3d normal;					///<	�Փ˂̖@��(0����1��) (Global)
		double depth;					///<	�Փ˂̐[���F�ŋߖT�_�����߂邽�߂ɁC2���̂𓮂��������D
		int polyCurPos[2];				///<	���ʑ̂�GJK��Support�̒T���J�n���_
		unsigned lastContactCount;		///<	�Ō�ɐڐG��������
	};
	typedef std::vector<TConvexPairState> TConvexPairStates;
	TConvexPairStates convexPairs;	
	struct TFramePairState{
		unsigned lastContactCount;		///<	�t���[�����Ō�ɐڐG��������
	};
	typedef std::vector<TFramePairState> TFramePairStates;
	TFramePairStates framePairs;
};
SGOBJECTIMP(CDCollisionEngineState, SGBehaviorState);

void CDCollisionEngine::LoadState(const SGBehaviorStates& states){
	CDCollisionEngineState* state = DCAST(CDCollisionEngineState, states.GetNext());
	CDCollisionEngineState::TConvexPairStates::iterator it = state->convexPairs.begin();
	for(TConvexIt itc = ConvexPairBegin(); itc!=ConvexPairEnd(); ++itc){
		CDConvexPairWithRecord* cp = (CDConvexPairWithRecord*)&**itc;
		if (cp){
			cp->normal = it->normal;
			cp->depth = it->depth;
			cp->lastContactCount = it->lastContactCount;
			cp->state = it->state;
			for(int i=0; i<2; ++i){
				CDPolyhedron* poly = DCAST(CDPolyhedron, cp->convex[i]);
				if (poly) poly->curPos = it->polyCurPos[i];
			}
			++it;
		}
	}
	for(unsigned i=0,j=0; i<pairs.size(); ++i){
		if (pairs[i]){
			pairs[i]->lastContactCount = state->framePairs[j].lastContactCount;
			j++;
		}
	}
}
void CDCollisionEngine::SaveState(SGBehaviorStates& states) const{
	UTRef<CDCollisionEngineState> state = new CDCollisionEngineState;
	states.push_back(state);
	for(TConvexIt it = ConvexPairBegin(); it!=ConvexPairEnd(); ++it){
		CDConvexPairWithRecord* cp = (CDConvexPairWithRecord*)&**it;
		if (cp){
			CDCollisionEngineState::TConvexPairStates& cs = state->convexPairs;
			cs.push_back(CDCollisionEngineState::TConvexPairState());
			cs.back().normal = cp->normal;
			cs.back().depth = cp->depth;
			cs.back().lastContactCount = cp->lastContactCount;
			cs.back().state = cp->state;
			for(int i=0; i<2; ++i){
				CDPolyhedron* poly = DCAST(CDPolyhedron, cp->convex[i]);
				if (poly) cs.back().polyCurPos[i] = poly->curPos;
			}
		}
	}
	for(unsigned i=0; i<pairs.size(); ++i){
		if (pairs[i]){
			state->framePairs.push_back(CDCollisionEngineState::TFramePairState());
			state->framePairs.back().lastContactCount = pairs[i]->lastContactCount;
		}
	}
}

static int count;
void RenderCDMesh(Affinef af, SGFrame* fr, GRRender* render){
	for(int i=0; i<fr->contents.size(); ++i){
		CDMesh* mesh = DCAST(CDMesh, fr->contents[i]);
		if (mesh){
			for(int i=0; i<mesh->conveces.size(); ++i){
				CDPolyhedron* poly = DCAST(CDPolyhedron, mesh->conveces[i]);
				if (poly){
					Vec4f color[] = {
						Vec4f(1,0,0,1),
						Vec4f(1,1,0,1),
						Vec4f(0,1,0,1),
						Vec4f(0,1,1,1),
						Vec4f(0,0,1,1),
						Vec4f(1,0,1,1),
					};
					render->SetMaterial(GRMaterialData(color[count%6], 2));
					count ++;
					render->SetModelMatrix(af);
					for(int i=0; i<poly->faces.size(); ++i){
						Vec3f vtx[3];
						for(int j=0; j<3; ++j) vtx[j] = poly->base[poly->faces[i].vtxs[j]];
						render->DrawDirect(GRRender::TRIANGLES, vtx, vtx+3);
					}
				}
			}
		}
	}
	for(int i=0; i<fr->Children().size(); ++i){
		RenderCDMesh(af * fr->Children()[i]->GetPosture(), fr->Children()[i], render);
	}
}
void CDCollisionEngine::Render(GRRender* render, SGScene* scene){
	if (!render->bDrawDebug) return;
	if ((render->drawState & GRRender::DRAW_OPAQUE) == 0) return;
	count = 0;
	for(int i=0; i<frames.size(); ++i){	
		RenderCDMesh(frames[i]->frame->GetWorldPosture(), frames[i]->frame, render);
	}
}

}
