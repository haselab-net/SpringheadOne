#pragma once
#ifndef FIFILEDOC_H
#define FIFILEDOC_H
#include "FIDocNode.h"

namespace Spr {;

/**	TypeDesc����PROTO(directX�Ȃ�template)�����N���X	*/
class FIProtoSaver{
public:
	virtual ~FIProtoSaver(){}
	virtual void Save(std::ostream& os){Save(os, "");}
	virtual void Save(std::ostream& os, FIString fileType);
	virtual void SaveImp(std::ostream& os, const FITypeDescDb::ProtoDesc& proto)=0;
};

/**	�h�L�������g�ƃt�@�C���̕ϊ��̂��߂̃R���e�L�X�g�D	*/
class SPR_DLL FIFileDoc:public UTRefCount{
protected:
	///	�t�@�C����
	UTString fileName;
	///	�h�L�������g�m�[�h�̃X�^�b�N
	FIDocNodes docs;
public:
	///	�t�@�C������h�L�������g�I�u�W�F�N�g���쐬
	virtual bool Load(UTString filename)=0;
	///	�h�L�������g�I�u�W�F�N�g�ɕۑ�
	virtual bool Save(UTString filename, FIDocNodeBase* doc)=0;
	///	�h�L�������g�̃��[�g�m�[�h
	virtual FIDocNodes& GetRoots(){ return docs; }
	///	�t�@�C����
	UTString GetFileName(){ return fileName; }
};



}//

#endif
