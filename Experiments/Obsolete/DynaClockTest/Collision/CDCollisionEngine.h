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
	///	�Փ˔����C��͑O�ɌĂяo�����R�[���o�b�N�D�ڐG���Ă���t���[���΂̐������Ăяo�����D
	virtual void Before(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr){}
	///	��͒���C�������̖ʂⒸ�_���L���ȂƂ��ɁC�ڐG���Ă���ʌ`��΂̐������Ăяo�����R�[���o�b�N�D
	virtual void Analyzed(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){}
	///	��͌�ɌĂяo�����R�[���o�b�N�D�ڐG���Ă���t���[���΂̐������Ăяo�����D
	virtual void After(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr){}
};
///	���X�i��vector
class CDCollisionListeners:public std::vector<CDCollisionListener*>{
public:
	///	�Փ˔����C��͑O�ɌĂяo�����R�[���o�b�N�D�ڐG���Ă���t���[���΂̐������Ăяo�����D
	virtual void Before(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr);
	///	��͒���C�������̖ʂⒸ�_���L���ȂƂ��ɁC�ڐG���Ă���ʌ`��΂̐������Ăяo�����R�[���o�b�N�D
	virtual void Analyzed(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	///	��͌�ɌĂяo�����R�[���o�b�N�D�ڐG���Ă���t���[���΂̐������Ăяo�����D
	virtual void After(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fr);
};

/**	�Փ˔���G���W���D
	�Փ˂��Ă���y�A�������C�Փ˔�����s���C�R�[���o�b�N���Ăяo���D	*/
class CDCollisionEngine: public SGBehaviorEngine
{
protected:
	///	�A�N�e�B�u�^��A�N�e�B�u�ȃ��X�i�̎w��
	struct ActivePair{
		int pos;
		int frame[2];
	};
	typedef std::vector<ActivePair> ActiveList;
	ActiveList activeList;		///<	�Փ˔�����s���y�A�̃��X�g(�f�t�H���g�ł͍s��Ȃ�)
	ActiveList inactiveList;	///<	�Փ˔�����s��Ȃ��y�A�̃��X�g(�f�t�H���g�ł͍s��)
	std::vector<bool> defaults;	///<	�f�t�H���g�Ŕ�����s�����ǂ���
	
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
	WBPreciseTimer timerGjk, timerQhull, timerNormal;
	///
	SGOBJECTDEF(CDCollisionEngine);
	///
	CDCollisionEngine();

	///@name	BehaviorEngine���I�[�o�[���C�h
	//{
	///
	int GetPriority() const { return SGBP_FORCEGENERATOR; }
	///	���Ԃ� dt �i�߂�
	virtual void Step(SGScene* s, float dt, unsigned int count);
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
	int NListener(){ return listeners.size(); }

	///	�t���[���̔ԍ���Ԃ��D
	int GetFrameID(SGFrame* fr){ return frameMap[fr]; }
	///	�t���[���΂�Ԃ��D
	CDFramePairWithRecord& GetFramePair(int f1, int f2){
		if (f1 > f2) std::swap(f1, f2);
		return pairs.item(f1, f2);
	}
	///	�t���[���΂�Ԃ��D
	CDFramePairWithRecord& GetFramePair(SGFrame* f1, SGFrame*f2){ return GetFramePair(GetFrameID(f1), GetFrameID(f2)); }
	///	�t���[��ID����t���[����Ԃ��D
	SGFrame* GetFrame(int id){ return frames[id]->frame; }
	///	�t���[��ID�ƃ��R�[�h�̈ʒu(ReserveFrameRecord()�̕Ԃ�l)���烌�R�[�h��Ԃ��D
	CDUserRecord* GetFrameRecord(int id, int pos){ return frames[id]->records[pos]; }
	//}
	
	///@name	�o�^�C�ǉ��n�̃��\�b�h
	//{
	///	����Ώۃt���[����ǉ��D���ׂĂ̔���Ώۃt���[���͓��K�w�ɂȂ���΂Ȃ�Ȃ��D
	void AddFrame(SGFrame* frame, CDUserRecord* rec=NULL, int pos=-1);
	///	�t���[���̐�
	int	NFrame(){ return frames.size(); }
	///	���X�i�[�̓o�^
	int AddListener(CDCollisionListener* l){
		listeners.push_back(l);
		return listeners.size()-1;
	}
	///	�Փ˔�������Ȃ��y�A��o�^�i�f�t�H���g�ł͏Փ˔��肷��j
	bool AddInactive(int f1, int f2, int pos);
	///	�Փ˔�������Ȃ��y�A��o�^�i�f�t�H���g�ł͏Փ˔��肷��j
	bool AddInactive(SGFrame* f1, SGFrame* f2, int pos){ return AddInactive(GetFrameID(f1), GetFrameID(f2), pos); }
	///	�Փ˔��������y�A��o�^�ipos �̃��X�i�ɂ��ẮC�f�t�H���g�ł͔��肵�Ȃ��Ȃ�D�j
	bool AddActive(int f1, int f2, int pos);
	///	�Փ˔��������y�A��o�^�ipos �̃��X�i�ɂ��ẮC�f�t�H���g�ł͔��肵�Ȃ��Ȃ�D�j
	bool AddActive(SGFrame* f1, SGFrame* f2, int pos){ return AddActive(GetFrameID(f1), GetFrameID(f2), pos); }
	///	�t���[��(CDFrame) ���Ƃ̃��[�U���R�[�h(records) �̏ꏊ���D
	int ReserveFrameRecord();
	///	�t���[���̃y�A(CDFramePairWithRecord) ���Ƃ̃��[�U���R�[�h(records) �̏ꏊ���D
	int ReserveFramePairRecord();
	///	�ʌ`��̃y�A(CDConvexPairWithRecord) ���Ƃ̃��[�U���R�[�h�̏ꏊ���D
	int ReserveConvexPairRecord();
	//}
};

}

#endif

