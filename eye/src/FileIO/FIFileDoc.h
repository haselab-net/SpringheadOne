#pragma once
#ifndef FIFILEDOC_H
#define FIFILEDOC_H
#include "FIDocNode.h"

namespace Spr {;

/**	TypeDescからPROTO(directXならtemplate)を作るクラス	*/
class FIProtoSaver{
public:
	virtual ~FIProtoSaver(){}
	virtual void Save(std::ostream& os){Save(os, "");}
	virtual void Save(std::ostream& os, FIString fileType);
	virtual void SaveImp(std::ostream& os, const FITypeDescDb::ProtoDesc& proto)=0;
};

/**	ドキュメントとファイルの変換のためのコンテキスト．	*/
class SPR_DLL FIFileDoc:public UTRefCount{
protected:
	///	ファイル名
	UTString fileName;
	///	ドキュメントノードのスタック
	FIDocNodes docs;
public:
	///	ファイルからドキュメントオブジェクトを作成
	virtual bool Load(UTString filename)=0;
	///	ドキュメントオブジェクトに保存
	virtual bool Save(UTString filename, FIDocNodeBase* doc)=0;
	///	ドキュメントのルートノード
	virtual FIDocNodes& GetRoots(){ return docs; }
	///	ファイル名
	UTString GetFileName(){ return fileName; }
};



}//

#endif
