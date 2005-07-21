#include "SceneGraph.h"
#pragma hdrstop
#include <Collision/CDGeometry.h>
#include <float.h>

namespace Spr{;

//----------------------------------------------------------------------------
//	SGFrame
SGOBJECTIMP(SGFrame, SGObject);
size_t SGFrame::NChildObjects(){
	return contents.size() + Children().size();
}
SGObject* SGFrame::ChildObject(size_t i){
	if (i<contents.size()) return contents[i];
	i -= contents.size();
	if (i<Children().size()) return Children()[i];
	return NULL;
}
bool SGFrame::AddChildObject(SGObject* o, SGScene* s){
    SGFrame* fr = DCAST(SGFrame, o);
    if (fr){
        fr->SetParent(this);
        return true;
    }
    if (o){
        contents.push_back(o);
        return true;
    }
    return false;
}
bool SGFrame::DelChildObject(SGObject* o, SGScene* s){
    SGFrame* fr = DCAST(SGFrame, o);
    if (fr){
        for(unsigned i=0; i<Children().size(); ++i){
            if (Children()[i] == fr){
                fr->SetParent(NULL);
                return true;
            }
        }
    }
    if (o){
        for(SGObjects::iterator it = contents.begin(); it!=contents.end(); ++it){
            if (*it == o){
                contents.erase(it);
                return true;
            }
        }
    }
    return false;
}
const UTTypeInfo** SGFrame::ChildCandidates(){
    static const UTTypeInfo* rv[]={
        NULL
    };
    return rv;
}

void SGFrame::Clear(){
	ForEachChild(&SGFrame::Clear);
	if (doc){
		doc->ClearChildren();
		doc->SetParent(NULL);//ドキュメントを親から削除
	}
	doc = NULL;
	ClearChildren();
	contents.clear();
}

void SGFrame::CalcBBox(){
	Vec3f bboxMin = Vec3f(FLT_MAX, FLT_MAX, FLT_MAX);
	Vec3f bboxMax = Vec3f(-FLT_MAX,-FLT_MAX,-FLT_MAX);
	for(SGObjects::iterator it=contents.begin(); it != contents.end(); ++it){
		CDGeometry* geom = DCAST(CDGeometry, *it);
		if(geom) geom->CalcBBox(bboxMin, bboxMax);
	}
	for(SGFrames::const_iterator it=Children().begin(); it != Children().end(); ++it){
		(*it)->CalcBBox();
		if ((*it)->bbox.GetBBoxExtent().X() >= 0){
			Vec3f bbMin = (*it)->GetPosture() * (*it)->bbox.GetBBoxMin();
			Vec3f bbMax = (*it)->GetPosture() * (*it)->bbox.GetBBoxMax();
			bboxMin.element_min(PTM::element_min(bbMin, bbMax));
			bboxMax.element_max(PTM::element_max(bbMin, bbMax));
		}
	}
	if (bboxMin.X() == FLT_MAX){
		bbox.SetBBoxMinMax(Vec3f(0,0,0), Vec3f(-1,-1,-1));
	}else{
		bbox.SetBBoxMinMax(bboxMin, bboxMax);
	}
//	DSTR << GetName() << bbox.GetBBoxMin() << bbox.GetBBoxMax() << std::endl;
}
void SGFrame::Print(std::ostream& os) const{
	int w = os.width();
	os.width(0);
	os << UTPadding(w) << "<SGFrame " << GetName() << ">" <<std::endl;
/*	os << UTPadding(w+2) << "posture=\"\n";
	os.width(w+2);
	os << posture;
	os.width(0);
	os << UTPadding(w+2) << "\">" << std::endl;
*/	os.width(w+2);
	for(SGObjects::const_iterator it=contents.begin(); it != contents.end(); ++it){
		(*it)->Print(os);
	}
	for(SGFrames::const_iterator it = children.begin(); it != children.end(); ++it){
		(*it)->Print(os);
	}
	os.width(0);
	os << UTPadding(w) << "</SGFrame>" << std::endl;
	os.width(w);
}

///	ロード後，参照の解決が終わってから呼び出す関数．
void SGFrame::Loaded(SGScene* scene)
{
	for (size_t i= 0; i<NChildObjects(); ++i)
		ChildObject(i)->Loaded(scene);
}

void SGFrame::EnumContents(SGObjects& objs){
	objs.insert(objs.end(), contents.begin(), contents.end());
	for(SGFrames::iterator it = children.begin(); it!=children.end(); ++it){
		(*it)->EnumContents(objs);
	}
}
void SGFrame::EnumContents(SGObjects& objs, SGFrames& frms){
	objs.insert(objs.end(), contents.begin(), contents.end());
	for(int i=0; i<contents.size(); ++i) frms.push_back(this);
	for(SGFrames::iterator it = children.begin(); it!=children.end(); ++it){
		(*it)->EnumContents(objs, frms);
	}
}


//----------------------------------------------------------------------------
//	FileIO
class SGFrameLoader:public FIObjectLoader<SGFrame>{
public:
	virtual bool LoadData(FILoadScene* ctx, SGFrame* fr){
		ctx->objects.Push(fr);
		return true;
	}
	virtual void Loaded(FILoadScene* ctx){
		ctx->objects.pop_back();
	}
};

class SGTransformMatrixLoader:public FIBaseLoader{
public:
	virtual UTString GetNodeType() const{
		return "FrameTransformMatrix";
	}
	virtual void Load(FILoadScene* ctx){
		Affinef af;
		ctx->docs.Top()->GetWholeData(af);
		//	左手系→右手系への変換
		af.ExZ() *= -1;
		af.EyZ() *= -1;
		af.EzX() *= -1;
		af.EzY() *= -1;
		af.PosZ() *= -1;
		SGFrame* frame = ACAST(SGFrame, ctx->objects.back());
		frame->SetPosture(af);
	}
};

class SPR_DLL SGFrameSaver:public FIObjectSaver<SGFrame>{
public:
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, SGFrame* fr){
		Affinef af = fr->GetPosture();
		//	左手系→右手系への変換
		af.ExZ() *= -1;
		af.EyZ() *= -1;
		af.EzX() *= -1;
		af.EzY() *= -1;
		af.PosZ() *= -1;
		if (af != Affinef()){	//	行列の保存
			UTRef<FIDocNodeBase> docPosture = ctx->CreateDocNode("FrameTransformMatrix");
			docPosture->SetWholeData(af);
			doc->AddChild(docPosture);
		}
	}
};

DEF_REGISTER_BOTH(SGFrame);
DEF_REGISTER_LOADER(SGTransformMatrix);

}