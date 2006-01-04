#ifndef D3FILEDOC_H
#define D3FILEDOC_H

//	DirectX�̃t�@�C�����[�_���g�p���邩�ǂ���
#define USE_DXFILE


#include <FileIO/FIDocNode.h>
#include <FileIO/FIDocScene.h>
#include <WinBasis/WBUtility.h>
#include <WinDx/D3D/WXD3D.h>

#ifdef USE_DXFILE
 #include <dxfile.h>
#else
 #include <FileIO/MetaLoader/FIGenParser.h>
#endif

namespace Spr {;

class D3ProtoSaver:public FIProtoSaver{
public:
	virtual void SaveImp(std::ostream& os, const FITypeDescDb::ProtoDesc& proto);
};

void RegisterD3DLoader(FILoadScene* l);
class SPR_DLL D3DocNode;
/**	�h�L�������g�ƃt�@�C���̕ϊ��̂��߂̃R���e�L�X�g�D	*/
class SPR_DLL D3FileDoc:public FIFileDoc{
protected:
	WXINTF(DirectXFileEnumObject) enumObject;
	WXINTF(DirectXFileSaveObject) saveObject;
	std::map<WBGuid, UTString> typeNameMap;
	std::map<UTString, WBGuid, UTStringLess> typeGuidMap;
	std::vector<WBGuid> typeGuids;
#ifndef USE_DXFILE
	UTRef<FIGenParser> fileIo;
#endif
public:
	D3FileDoc();
	void Init();
	void RegisterFromTypeDB();
	void RegisterType(UTString name, const GUID* guid);
	UTString GetTypeName(const GUID* g){ return typeNameMap[*g]; }
	GUID GetTypeGuid(UTString n){ return typeGuidMap[n]; }
	void Print(std::ostream& os) const;

	///	�t�@�C������h�L�������g�I�u�W�F�N�g���쐬
	virtual bool Load(UTString filename);
	///	�h�L�������g�I�u�W�F�N�g�ɕۑ�
	virtual bool Save(UTString filename, FIDocNodeBase* doc);
protected:
	///	���[�h�̍ċA����
	void LoadRecurse();
	///	�Z�[�u�̍ċA����
	void SaveRecurse();
	///	DirectX�ɂȂ��V�K�e���v���[�g�̓o�^
	void RegisterTemplates();
};


}	//	Spr
#endif
