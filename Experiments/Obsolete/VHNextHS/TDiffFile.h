#ifndef TDIFFFILE_H
#define TDIFFFILE_H

#include <Graphics/GRRender.h>
#include <Base/Affine.h>
#include <vector>
#include "THuman.h"

///	DIFF�t�@�C���̈�s
class TDiffRecord{
public:
	std::string type;
	float time;
	typedef std::vector<Spr::Vec3f> TData;
	TData data;
	TDiffRecord():time(0.0f){}
	bool Load(std::istream& is);
	float LoadField(std::istream& is);
	void Print(std::ostream& os);
};

///	Diff�t�@�C���P�ɑΉ�����N���X
class TDiffFile {
public:
	class TRecords:public std::vector<TDiffRecord>{
	public:
		void Print(std::ostream& os);
		void Draw(Spr::GRRender* render, float time);
		bool GetRecord(TDiffRecord& pos, TDiffRecord& vel, float time);
		int curIndex;
		TRecords():curIndex(0){}
	};
	TRecords g1[2];	///<	G1L, G1R
	TRecords g2[2];	///<	G2L, G2R
	TRecords frf;	///<	FRF
	bool bLoaded;
	TDiffFile(): bLoaded(false){}
	bool IsLoaded(){ return bLoaded; }
	bool Load(std::istream& is);
	void Print(std::ostream& os);
	void Draw(Spr::GRRender* render, THuman& human, float time);
	void GetRecPos(TRecords*& rec, int &pos, int id, bool bLeft);
	///	�ʂ��ԍ��Ōv���_�̈ʒu���擾����D�S14�_
	bool GetRecord(Spr::Vec3f* pos, Spr::Vec3f* vel, bool bLeft, float time);
	///	i1��i2�̋��������߂�D
	float GetLinkLength(TRecords& r1, int i1, TRecords& r2, int i2);
	float GetLinkLength(int id1, bool bLeft1, int id2, bool bLeft2);
	float GetLinkLength(int id1, int id2);
	void SetHumanScale(THuman& human);
	bool AddSpringForce(THuman& human, float time, float dt);
};

#endif