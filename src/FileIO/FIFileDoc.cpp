#include "FileIO.h"
#pragma hdrstop

namespace Spr{;

void FIProtoSaver::Save(std::ostream& os, FIString fileType){
	typedef FITypeDescDb::ProtoDescs Descs;
	const Descs& descs = FITypeDescDb::GetProtoDescs();
	for(Descs::const_iterator it=descs.begin(); it!=descs.end(); ++it){
		if (fileType.length()==0 || it->fileType.compare(fileType)==0){
			SaveImp(os, *it);
		}
	}
}

}
