#ifndef CDCOLLISIONENGINE_H
#define CDCOLLISIONENGINE_H

#pragma once
#include <SceneGraph/SGBehaviorEngine.h>
#include <Collision/CDFramePair.h>
#include <Collision/CDDetectorImp.h>
#include <WinBasis/WBPreciseTimer.h>

namespace Spr{;


///	�Փˎ��̃��X�i�[(�R�[���o�b�N)�N���X�D
class CDCollisionListener{
public:
	///	���ׂĂ̑O�Ɉ�x�Ăяo������R�[���o�b�N�D
	virtual void BeforeAll(SGScene* scene){}

	///	�Փ˔����C��͑O�ɌĂяo�����R�[���o�b�N�D�ڐG���Ă���t���[���΂̐������Ăяo�����D
	virtual void Before(SGScene* scene, CDFramePairWithRecord* fr){}
	///	��͒���C�������̖ʂⒸ�_���L���ȂƂ��ɁC�ڐG���Ă���ʌ`��΂̐������Ăяo�����R�[���o�b�N�D
	virtual void Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){}
	///	��͌�ɌĂяo�����R�[���o�b�N�D�ڐG���Ă���t���[���΂̐������Ăяo�����D
	virtual void After(SGScene* scene, CDFramePairWithRecord* fr){}

	///	���ׂĂ̌�Ɉ�x�Ăяo������R�[���o�b�N�D
	virtual void AfterAll(SGScene* scene){}
};
///	���X�i��vector
class CDCollisionListeners:public std::vector<CDCollisionListener*>{
public:
	///	�Փ˔���O�Ɉ�x�Ăяo������R�[���o�b�N�D
	void BeforeAll(SGScene* scene){
		for(unsigned  i=0; i<size(); ++i){
			at(i)->BeforeAll(scene);
		}
	}
	///	�Փ˔����C��͑O�ɌĂяo�����R�[���o�b�N�D�ڐG���Ă���t���[���΂̐������Ăяo�����D
	void Before(SGScene* scene, CDFramePairWithRecord* fr){
		for(unsigned  i=0; i<size(); ++i){
			if (fr->IsActive(i)) begin()[i]->Before(scene, fr);
		}
	}
	///	��͒���C�������̖ʂⒸ�_���L���ȂƂ��ɁC�ڐG���Ă���ʌ`��΂̐������Ăяo�����R�[���o�b�N�D
	void Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){
		for(unsigned  i=0; i<size(); ++i){
			if (fr->IsActive(i)) begin()[i]->Analyzed(scene, fr, geom, conv, analyzer);
		}
	}
	///	��͌�ɌĂяo�����R�[���o�b�N�D�ڐG���Ă���t���[���΂̐������Ăяo�����D
	void After(SGScene* scene, CDFramePairWithRecord* fr){
		for(unsigned  i=0; i<size(); ++i){
			if (fr->IsActive(i)) begin()[i]->After(scene, fr);
		}
	}
	void AfterAll(SGScene* scene){
		for(unsigned  i=0; i<size(); ++i){
			at(i)->AfterAll(scene);
		}
	}
};

/**	�Փ˔���G���W���D
	�Փ˂��Ă���y�A�������C�Փ˔�����s���C�R�[���o�b�N���Ăяo���D	*/
class CDCollisionEngine: public SGBehaviorEngine
{
protected:
	///	�A�N�e�B�u�^��A�N�e�B�u�ȃ��X�i�̎w��
	struct ActivePair{
		int pos;		///<	���X�i�̔ԍ�
		int frame[2];	///<	�t���[���̃y�A
	};
	typedef std::vector<ActivePair> TActivePairs;
	TActivePairs activeList;	///<	�Փ˔�����s���i���X�i���Ăяo���j�y�A�̃��X�g(�f�t�H���g�ł͍s��Ȃ�)
	TActivePairs inactiveList;	///<	�Փ˔�����s��Ȃ��i���X�i���Ăт��Ȃ��j�y�A�̃��X�g(�f�t�H���g�ł͍s��)
	std::vector<bool> defaults;	///<	�f�t�H���g�ŏՓ˔�����s���i���X�i���Ăяo���j���ǂ����̃t���O
	
	///	�Փ˔���̈ꗗ�Ditem(i,j) (i<j) �ŃA�N�Z�X����D
	CDFramePairWithRecords pairs;
	///	�ڐG��̓G���W���D�������̖ʂⒸ�_�̏��������Ă���D
	CDContactAnalysis analyzer;
	///	�t���[�����Ƃ̃��[�U���R�[�h�̐�
	int nFrameRecords;
	///	�t���[���y�A���Ƃ̃��[�U���R�[�h�̐�
	int nFramePairRecords;
	///	�ʌ`��̃y�A���Ƃ̃��[�U���R�[�h�̐�
	int nConvexPairRecords;
	///	����Ώۃt���[���̃R���e�i�^
	typedef std::vector< UTRef<CDFrame> > Frames;
	///	�Փ˔���Ώۂ̃t���[��
	Frames frames;
	///	�Փ˂̃��X�i
	CDCollisionListeners listeners;
	///	�t���[���ƃt���[���ԍ��̑Ή��\
	std::map<SGFrame*, int> frameMap;

public:
	///	�t���[���΂̂��߂̃C�^���[�^
	typedef CDFramePairWithRecords::iterator TFrameIt;

	struct GetGeom{
		CDGeometryPairs* operator()(TFrameIt it){ return *it ? &(*it)->geometryPairs : NULL; }
	};
	///	�W�I���g���΂̂��߂̃C�^���[�^
	class TGeometryIt: public UTComposedIterator<CDGeometryPair*, TFrameIt, CDGeometryPairs::iterator, GetGeom>{
		friend class CDCollisionEngine;
		friend class TConvexIt;
	public:
		TGeometryIt(){}
		TGeometryIt(CDFramePairWithRecords::iterator b, CDFramePairWithRecords::iterator e, CDGeometryPairs::iterator g){
			Init(b, e, g);
		}
	};

	
	struct GetConv{
		CDConvexPairs* operator()(TGeometryIt it){ return &it->convexPairs; }
	};
	///	�ʌ`��΂̂��߂̃C�^���[�^
	class TConvexIt: public UTComposedIterator<CDConvexPairWithRecord*, TGeometryIt, CDConvexPairs::iterator, GetConv>{
		friend class CDCollisionEngine;
		TConvexIt(){}
		TConvexIt(TGeometryIt b, TGeometryIt e, CDConvexPairs::iterator c){
			Init(b, e, c);
		}
	};

public:
	WBPreciseTimer timerGjk, timerQhull, timerNormal;
	///
	SGOBJECTDEF(CDCollisionEngine);
	///
	CDCollisionEngine();

	///@name	BehaviorEngine���I�[�o�[���C�h
	//{
	///
	int GetPriority() const { return SGBP_COLLISIONENGINE; }
	///	���Ԃ� dt �i�߂�
	virtual void Step(SGScene* s);
	///	����Ώۂ̃t���[�����N���A����D
	virtual void Clear(SGScene* s);
	///	������(frames ���� pairs�����)
	void Init();
	//}

	//@name	���X�i�┻��Ώۂ̃t���[���Ɗ֘A�t����ꂽ�f�[�^�̃A�N�Z�X
	//@{
	///	���X�i��Ԃ��D
	CDCollisionListener* GetListener(int pos){ return listeners[pos]; }
	///	���X�i�̐�
	size_t NListener(){ return listeners.size(); }

	///	�t���[���̔ԍ���Ԃ��D
	int GetFrameID(SGFrame* fr){ return frameMap[fr]; }
	///	�t���[���΂�Ԃ��Df1 < f2 �łȂ���΂Ȃ�Ȃ��D
	CDFramePairWithRecord* GetFramePair(int f1, int f2){
		return pairs.item(f1, f2);
	}
	///	�t���[���΂�Ԃ��D(f1,f2)��(f2,f1)�ǂ���͌������D
	CDFramePairWithRecord* GetFramePair(SGFrame* f1, SGFrame*f2){ return GetFramePair(GetFrameID(f1), GetFrameID(f2)); }
	///	�t���[��ID����t���[����Ԃ��D
	SGFrame* GetFrame(int id){ return frames[id]->frame; }
	///	�t���[��ID�ƃ��R�[�h�̈ʒu(ReserveFrameRecord()�̕Ԃ�l)���烌�R�[�h��Ԃ��D
	CDUserRecord* GetFrameRecord(int id, int pos){ return frames[id]->records[pos]; }
#if defined _MSC_VER && _MSC_VER >= 1300
	///	�t���[���΂̃C�^���[�^�̎n�_
	TFrameIt FramePairBegin() const { return (TFrameIt&)pairs.begin(); }
	///	�t���[���΂̃C�^���[�^�̏I�_
	TFrameIt FramePairEnd() const { return (TFrameIt&)pairs.end(); }
#else
	///	�t���[���΂̃C�^���[�^�̎n�_
	TFrameIt FramePairBegin() const { return (TFrameIt)pairs.begin(); }
	///	�t���[���΂̃C�^���[�^�̏I�_
	TFrameIt FramePairEnd() const { return (TFrameIt)pairs.end(); }
#endif
	///	�W�I���g���΂̃C�^���[�^�̎n�_
	TGeometryIt GeometryPairBegin() const{
		TGeometryIt rv;
		rv.InitAsBegin(FramePairBegin(), FramePairEnd());
		return rv;
	}
	///	�W�I���g���΂̃C�^���[�^�̏I�_
	TGeometryIt GeometryPairEnd() const{
		TGeometryIt rv;
		rv.InitAsEnd(FramePairBegin(), FramePairEnd());
		return rv;
	}
	///	�ʌ`��΂̃C�^���[�^�̎n�_
	TConvexIt ConvexPairBegin() const {
		TConvexIt rv;
		rv.InitAsBegin(GeometryPairBegin(), GeometryPairEnd());
		return rv;
	}
	///	�ʌ`��΂̃C�^���[�^�̏I�_
	TConvexIt ConvexPairEnd() const{
		TConvexIt rv;
		rv.InitAsEnd(GeometryPairBegin(), GeometryPairEnd());
		return rv;
	}
	//}
	
	///@name	�o�^�C�ǉ��n�̃��\�b�h
	//{
	///	����Ώۃt���[����ǉ��D���ׂĂ̔���Ώۃt���[���͓��K�w�ɂȂ���΂Ȃ�Ȃ��D
	void AddFrame(SGFrame* frame, CDUserRecord* rec=NULL, int pos=-1);
	///	�t���[���̐�
	size_t NFrame(){ return frames.size(); }
	///	���X�i�[�̓o�^
	int AddListener(CDCollisionListener* l);
	///	�Փ˔�������Ȃ��y�A��o�^�i�f�t�H���g�ł͏Փ˔��肷��j
	bool AddInactive(int f1, int f2, int pos);
	///	�Փ˔�������Ȃ��y�A��o�^�i�f�t�H���g�ł͏Փ˔��肷��j
	bool AddInactive(SGFrame* f1, SGFrame* f2, int pos){ return AddInactive(GetFrameID(f1), GetFrameID(f2), pos); }
	///	pos�̃��X�i�ɂ��Ĕ��肵�Ȃ��悤�ɓo�^�����y�A(InactiveList)���N���A����
	int ClearInactive(int pos);
	///	�Փ˔��������y�A��o�^�ipos �̃��X�i�ɂ��ẮC�f�t�H���g�ł͔��肵�Ȃ��Ȃ�D�j
	bool AddActive(int f1, int f2, int pos);
	///	�Փ˔��������y�A��o�^�ipos �̃��X�i�ɂ��ẮC�f�t�H���g�ł͔��肵�Ȃ��Ȃ�D�j
	bool AddActive(SGFrame* f1, SGFrame* f2, int pos){ return AddActive(GetFrameID(f1), GetFrameID(f2), pos); }
	///	pos�̃��X�i�ɂ��Ĕ��肷��悤�ɓo�^�����y�A(activeList)���N���A����
	int ClearActive(int pos);
	///	�t���[��(CDFrame) ���Ƃ̃��[�U���R�[�h(records) �̏ꏊ���D
	int ReserveFrameRecord();
	///	�t���[���̃y�A(CDFramePairWithRecord) ���Ƃ̃��[�U���R�[�h(records) �̏ꏊ���D
	int ReserveFramePairRecord();
	///	�ʌ`��̃y�A(CDConvexPairWithRecord) ���Ƃ̃��[�U���R�[�h�̏ꏊ���D
	int ReserveConvexPairRecord();
	//}
	
	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;
};

}

#endif

