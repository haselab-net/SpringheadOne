#include "ImpD3D.h"
#pragma hdrstop
#include "D3Import.h"

namespace Spr{;

//-------------------------------------------------------------------
// D3Import
//-------------------------------------------------------------------
SGOBJECTIMP(D3Import, SGFrame);


class D3ImportLoader : public FIObjectLoader<D3Import>{
	bool LoadData(FILoadScene* ctx, D3Import* im){			
		//インポートするファイル名を取得
		char* filename = 0;
		ctx->docs.Top()->GetWholeData(filename);
		im->filename = filename;

		WBPath xFilePath;
		xFilePath.Path(ctx->fileName);
		xFilePath.Path(xFilePath.FullPath());
		WBPath imPath;
		imPath.Path(filename);
        FIString oldCwd = imPath.GetCwd();
		imPath.SetCwd(xFilePath.Drive() + xFilePath.Dir());
		UTString full = imPath.FullPath();		
		//	該当するファイルを読み込んでＤＯＭに変換
		D3FileDoc fileDoc;
		fileDoc.Load(full);
		//	DOMをロード
		bool shouldPop = ctx->PushContainer();
		UTString oldFileName = ctx->fileName;
		ctx->fileName = full;
		for(unsigned i=0; i<fileDoc.GetRoots().size(); ++i){
			ctx->docs.Push(fileDoc.GetRoots()[i]);
			ctx->LoadRecursive();
			ctx->docs.Pop();
		}
		if (shouldPop) ctx->PopContainer();
		ctx->fileName = oldFileName;
        imPath.SetCwd(oldCwd);
		return true;
	}
};
class D3ImportSaver:public FIObjectSaver<D3Import, true, false>{
public:
	virtual void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, D3Import* im){
		doc->SetWholeData(im->filename.c_str());
	}
};

DEF_REGISTER_BOTH(D3Import);

}

