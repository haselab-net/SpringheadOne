#ifndef CDDETECTOR_H
#define CDDETECTOR_H
#include "CDConvex.h"
#include <SceneGraph/SGFrame.h>
#include <Base/Combination.h>
#include "CDQuickHull3D.h"

namespace Spr {;

template <class T, class ITI, class ITJ, class GetContainer>
class UTComposedIterator{
public:
	typedef UTComposedIterator<T, ITI, ITJ, GetContainer> self_type;
protected:
	ITI iti, iEnd;
	ITJ itj;
public:
	self_type(){}
	T operator->(){ return (T&)*itj; }
	T operator*(){ return (T&)*itj; }
	self_type& operator ++(int){ Next(); return *this; }
	self_type operator ++(){ self_type rv(*this); Next(); return rv; }
	bool operator ==(const self_type& i) const{ return i.iti == iti && i.itj == itj; }
	bool operator !=(const self_type& i) const {return !(i==*this);}
	void Init(ITI b, ITI e, ITJ j){
		while (b!=e && !GetContainer()(b)) ++b;
		iti = b;
		iEnd = e;
		itj = j;
	}
	void InitAsBegin(ITI b, ITI e){
		Init(b, e, NULL);
		if (b!=e) itj = GetContainer()(iti)->begin();
		else return;
		while (itj==GetContainer()(iti)->end()){
			do{
				++iti;
				if (iti == iEnd){
					itj=NULL;
					break;
				}
			} while(!GetContainer()(iti));
			itj=GetContainer()(iti)->begin();
		}
	}
	void InitAsEnd(ITI b, ITI e){
		Init(e, e, 0);
	}
	void Next(){
		assert(itj != NULL);
		++itj;
		if (itj == GetContainer()(iti)->end()){
			do{
				++iti;
			}while(iti != iEnd && (!GetContainer()(iti) || !GetContainer()(iti)->size()));
			if (iti != iEnd){
				itj = GetContainer()(iti)->begin();
			}else{
				itj = NULL;
			}
		}
	}
};


///	�ʌ`��̃y�A
class CDConvexPair:public UTRefCount{
public:
	virtual ~CDConvexPair(){}
	UTRef<CDConvex> convex[2];
	Vec3d closestPoint[2]; // ���[�J�����W�n
	Vec3d commonPoint;     // �O���[�o�����W�n
	unsigned int lastContactCount;
	enum State{
		NEW,
		CONTINUE,
	} state;
	void Set(CDConvex* c1, CDConvex* c2){
		convex[0] = c1;
		convex[1] = c2;
	}
	CDConvexPair():lastContactCount(-2){}
};
///	�ʌ`��y�A�̑S�g�ݍ��킹
class CDConvexPairs:public UTCombination< UTRef<CDConvexPair> >{
public:
};
class CDFramePair;
///	�W�I���g���̃y�A
class CDGeometryPair:public UTRefCount{
public:
	UTRef<CDGeometry> geometry[2];	///<	�W�I���g��
	UTRef<SGFrame> frame[2];		///<	�W�I���g���������t���[��
	Affinef posture[2];				///<	�t���[���̊�t���[������̎p��(���[�N�G���A)
	CDConvexPairs convexPairs;		///<	2�̃W�I���g�����\������ʌ`��̂��ׂẴy�A
	void Set(CDFramePair* fp, CDGeometry* g1, SGFrame* f1, CDGeometry* g2, SGFrame* f2);
	virtual ~CDGeometryPair(){}
};
///	�W�I���g���̃y�A�̑S�g�ݍ��킹
class CDGeometryPairs:public UTCombination< UTRef<CDGeometryPair> >{
public:
};

///	�Փ˔��茋��
class CDIntersection{
public:
	CDConvexPair* convexPair;
	CDGeometryPair* geometryPair;
	CDIntersection(CDConvexPair* cp, CDGeometryPair* gp): convexPair(cp), geometryPair(gp){}
};
///	�Փ˔��茋�ʂ̔z��
	class CDIntersections:public std::vector<CDIntersection>{
};

	
///	���[�U���R�[�h�̊�{�N���X
typedef SGObject CDUserRecord;

///	���[�U���R�[�h�̃R���e�i�^
class CDUserRecords:public std::vector< UTRef<CDUserRecord> >{
public:
};
///	�Փ˔���Ώۃt���[���D	�t���[���ƃ��[�U���R�[�h������
class CDFrame:public UTRefCount{
public:
	CDFrame(){}
	CDFrame(SGFrame* f): frame(f){}
	CDUserRecords records;
	UTRef<SGFrame> frame;
};
/**	�Փ˔���Ώۂ̃t���[���̃y�A.
�t���[���͓����K�w�łȂ���΂Ȃ�Ȃ�(�e�₻��Posture�͍l�����Ȃ�)�D

	�t���[���̏��L�֌W
	SGFrame
	 +-�qSGFrame
	 +-CDMesh
		    +-CDPolyhedron
	�t���[��(SGFrame)�́C�����̃W�I���g��(CDGeometries)�Ǝq�t���[�������D
	�W�I���g��(CDGeometry) �̒��ɂ́C�����̃W�I���g����������(CDMesh)
	�ƒP���ȓʌ`��(CDPolyhedron)������D
	CDMesh �͕�����Convex�����D
*/
class CDFramePair{	
public:
	///	���X�i���A�N�e�B�u���ǂ����̃t���O�D�Ō�̗v�f���S�̂� or ��\���D
	class CDIsActive: public std::vector<bool>{
		bool bOr;
	public:
		void Update(){
			bOr = false;
			for(unsigned i=0; i<size(); ++i){
				bOr |= at(i);
			}
		}
		bool GetOr(){ return bOr; }
	};
	struct GetConv{
		CDConvexPairs* operator()(CDGeometryPairs::iterator it){ return *it ? &(*it)->convexPairs : NULL; }
	};
	///	�ʌ`��̃y�A��񋓂��邽�߂̃C�^���[�^
	struct CDConvexPairIt: UTComposedIterator<CDConvexPair*, CDGeometryPairs::iterator, CDConvexPairs::iterator, GetConv>{
		friend class CDCollisionEngine;
		CDConvexPairIt(){}
		CDConvexPairIt(CDGeometryPairs::iterator b, CDGeometryPairs::iterator e, CDConvexPairs::iterator c){
			Init(b, e, c);
		}
	};

	///	�Ō�ɐڐG��������
	unsigned lastContactCount;
	///	�A�N�e�B�u���ǂ����̃t���O�����DCDCollisionEngine�̃��X�i�̐������t���O������D
	CDIsActive isActive;
	///	����Ώۃt���[��
	UTRef<CDFrame> frame[2];
	///	�Փ˂����y�A��������ׂ��z��
	CDIntersections intersections;
	///	�W�I���g���̃y�A�̑S�g�ݍ��킹
	CDGeometryPairs geometryPairs;	

	///
	CDFramePair();
	///	�t���[���̃Z�b�g
	void Set(CDFrame* f1, CDFrame* f2);
	///	�S�̂��A�N�e�B�u���ǂ���
	bool IsActive(){ return isActive.GetOr(); }
	///	���X�i i ���A�N�e�B�u���ǂ���
	bool IsActive(int i){ return isActive[i]; }
	///	�N���A
	void Clear(){
		frame[0] = frame[1] = NULL;
		geometryPairs.clear();
	}
	///	���L�_��T��
	bool Detect(int count);
	///	�ڐG���Ă��邩�ǂ���
	bool IsContact(SGScene* scene);

	///	�ʌ`��΂̗�
	CDConvexPairIt ConvexPairBegin(){
		CDConvexPairIt rv;
		rv.InitAsBegin(geometryPairs.begin(), geometryPairs.end());
		return rv;
	}
	CDConvexPairIt ConvexPairEnd(){
		CDConvexPairIt rv;
		rv.InitAsEnd(geometryPairs.begin(), geometryPairs.end());
		return rv;
	}
protected:

	///	�ڐG���̃R�[���o�b�N
	virtual void Found(CDConvexPair& cp, CDGeometryPair& gp){
		intersections.push_back(CDIntersection(&cp, &gp));
	}
	/**	CDConvexPair�̔h���N���X���g�����߂̎d�g�݁D
		���̃N���X���p�����āC���̊֐����I�[�o�[���C�h���邱�ƂŁC
		CDConvexPair�̑���ɔh���N���X���g�p�ł���D	*/
	virtual CDConvexPair* CreateConvexPair(){
		return new CDConvexPair;
	}
	/**	CDGeometryPair�̔h���N���X���g�����߂̎d�g�݁D
		���̃N���X���p�����āC���̊֐����I�[�o�[���C�h���邱�ƂŁC
		CDGeometryPair�̑���ɔh���N���X���g�p�ł���D	*/
	virtual CDGeometryPair* CreateGeometryPair(){
		return new CDGeometryPair;
	}
private:
	///	�t���[�������W�I���g����񋓁D
	void EnumGeometry(SGFrames& frames, CDGeometries& geoms, SGFrame* f);
	friend class CDGeometryPair;
};
///	�@��������ConvexPair
class CDConvexPairWithNormal: public CDConvexPair{
public:
	Vec3d normal;				///<	�Փ˂̖@��(0����1��) (Global)
	Vec3d iNormal;				///<	�ϕ��ɂ��@��
	Vec3d center;				///<	2�̍ŐN���_�̒��Ԃ̓_�CCDContactAnalysis::CalcNormal ���X�V����D
	double depth;				///<	�Փ˂̐[���F�ŋߖT�_�����߂邽�߂ɁC2���̂𓮂��������D
};
///	�@��������FramePair
class CDFramePairWithNormal :public CDFramePair{
protected:
	virtual CDConvexPair* CreateConvexPair(){
		return new CDConvexPairWithNormal;
	}
};

///	���[�U���R�[�h�Ɩ@��������ConvexPair
class CDConvexPairWithRecord: public CDConvexPairWithNormal{
public:
	CDUserRecords records;
};
///	CDConvexPairWithRecord�̂��߂�CDFramePair ���R�[�h�����D
class CDFramePairWithRecord :public CDFramePairWithNormal, public UTRefCount{
public:
	CDUserRecords records;
protected:
	virtual CDConvexPair* CreateConvexPair(){
		return new CDConvexPairWithRecord;
	}
};
///	�Փ˔���̑Ώۂ̃t���[���΁D
class CDFramePairWithRecords: public UTCombination< UTRef<CDFramePairWithRecord> >{
public:
};

}
#endif
