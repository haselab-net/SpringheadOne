#ifndef D3FILEDOC_H
#define D3FILEDOC_H

//	DirectXのファイルローダを使用するかどうか
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
/**	ドキュメントとファイルの変換のためのコンテキスト．	*/
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

	///	ファイルからドキュメントオブジェクトを作成
	virtual bool Load(UTString filename);
	///	ドキュメントオブジェクトに保存
	virtual bool Save(UTString filename, FIDocNodeBase* doc);
protected:
	///	ロードの再帰部分
	void LoadRecurse();
	///	セーブの再帰部分
	void SaveRecurse();
	///	DirectXにない新規テンプレートの登録
	void RegisterTemplates();
};


}	//	Spr
#endif
