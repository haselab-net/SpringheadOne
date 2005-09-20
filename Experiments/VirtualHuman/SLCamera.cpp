// SLCamera.cpp: SLCamera クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "virtualhuman.h"
#include "SLCamera.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SLCamera::SLCamera(){
	lCamera = NULL;
}

SLCamera::~SLCamera()
{
	CloseSLCamera();
}

bool SLCamera::InitSLC(){
	lCamera = OpenSLC("SLC-C01-00023",NULL,NULL);//ラベリングカメラ使いはじめ
	if (lCamera == NULL){
		//std::cout << " LabelingCamera Not Existing !" << std::endl;
		DSTR << " LabelingCamera Not Existing !" << (DWORD&)lCamera << std::endl;
		return false;
	}
	//else std::cout << " Succeed in getting LabelingCamera " << std::endl; //add osada
	else DSTR << " Succeed in getting LabelingCamera " << std::endl; //add osada
	labelNum = LABEL_NUM;
	lastLabelNum = 0;

	return true;
}

void SLCamera::CloseSLCamera(){
	if(lCamera) CloseSLC(lCamera);
}

extern Spr::GRMaterialData Mat(Spr::Vec4f c);

/*bool SLCamera::Draw(Spr::GRRender* render){
	if(!lCamera) return false;

	render->SetLineWidth(10);
	for(int i = 0; i < maxLabelNum; i++){
		if(marker[i].match == -1) render->SetMaterial(Mat(Spr::Vec4f(1,0,0,1)));
		else if(maxLabelNum == 1) render->SetMaterial(Mat(Spr::Vec4f(0,0,1,1)));
		else render->SetMaterial(Mat(Spr::Vec4f(0,((float)i)/(maxLabelNum-1),((float)(maxLabelNum-1-i))/(maxLabelNum-1),1)));
		Spr::Vec3f pos = marker[i].pos;
		render->DrawDirect(Spr::GRRender::POINTS, &pos, &pos+1);
	}
	return true;
}*/

bool SLCamera::Draw(Spr::GRRender* render){
	if(!lCamera) return false;

	render->SetLineWidth(10);
	for(int i = 0; i < markerNum; i++){
		render->SetMaterial(Mat(Spr::Vec4f(0,1,0,1)));
		Spr::Vec3f pos = marker[i].pos;
		render->DrawDirect(Spr::GRRender::POINTS, &pos, &pos+1);
	}
	return true;
}

Spr::Vec3f SLCamera::Convert(SLCOBJECT label){
	Spr::Vec3f lpos;
	lpos.x = label.dX;
	lpos.y = label.dY;
	lpos.z = label.dZ;
	lpos  /= 100;
	return lpos;
}

void SLCamera::SetMarkerPos(){
	Spr::Vec3f oldPos;
	for(int i = 0; i < markerNum; i++){
		marker[i].pos = Convert(labelPos[i]);
	}
}


/*void SLCamera::Matching(int lnum){
	Spr::Vec3f lpos;

	for(int j = 0; j < maxLabelNum; j++) marker[j].match = -1;	
	for(int i = 0; i < lnum; i++){
		lpos = Convert(labelPos[i]);
		for(int j = 0; j < maxLabelNum; j++){
			marker[j].dist[i] = (lpos - marker[j].pos).norm();
		}
	}
	// マーカーが新たに追加された場合
	if(maxLabelNum < lnum){
		for(int i = 0; i < lnum; i++){
			marker[i].match = i;
		}
		return;
	}
	// マーカー追従
	minCost = 10000;
	CalcTotalCost(lnum, 0, 0);
	for(int i = 0; i < lnum; i++){
		marker[minIndex[i]].match = i;
	}
	return;
}*/

/*float SLCamera::CalcTotalCost(int lnum, int l, float cost){
	float tcost, mcost = 10000;
	if(l == lnum) return cost;

	for(int j = 0; j < maxLabelNum; j++){
		if(marker[j].match == -1){
			marker[j].match = l;
			tcost = CalcTotalCost(lnum, l+1, cost + marker[j].dist[l]);

			if(tcost <= minCost){
				mcost = tcost;
				minCost = mcost;
				minIndex[l] = j;
			}
			marker[j].match = -1;
		}
	}
	return mcost;
}*/

/*void SLCamera::SetPosition(float dt){
	Spr::Vec3f oldPos;
	for(int j = 0; j < maxLabelNum; j++){
		if(minIndex[j] != -1){
			if(marker[j].state > 1){
				oldPos = marker[j].pos;
				marker[j].pos = Convert(labelPos[minIndex[j]]);
				marker[j].vel = (marker[j].pos - oldPos) / dt;
			}
			else{
				marker[j].pos = Convert(labelPos[minIndex[j]]);
				marker[j].state++;
			}
		}
		else marker[j].state = 0;
	}
}*/

bool SLCamera::Step(float dt){ 
	if(lCamera){
		markerNum = 0;
		if(GetSLCData(lCamera, labelPos, &labelNum)){ ////ラベリングカメラから値を取得
			//------　ラベリングカメラ座標系を変換(仮)--------//
			for(int i = 0; i < labelNum; i++){
				double tmp;
				tmp = labelPos[i].dX;
				labelPos[i].dX = -labelPos[i].dY;
				labelPos[i].dY = -tmp + 100;
				labelPos[i].dZ =  labelPos[i].dZ - 110;
			}
			markerNum = labelNum;
			SetMarkerPos();
			
			//Matching(labelNum);
			//SetPosition(dt);
			//if(maxLabelNum < labelNum) maxLabelNum = labelNum;

			labelNum = LABEL_NUM;	// labelNum は常に2にしてないとダメなようだ.
		}

		else {
			//std ::cout << GetSLCErrorCode() << std::endl;	// 何でラベルが見れないのかを知るため.
			DSTR << GetSLCErrorCode() << std::endl;	// 何でラベルが見れないのかを知るため.
			labelNum = LABEL_NUM;
			return false;
		}

		//SetSLCDebugItem(lCamera,NULL,NULL);	// いるかどうか分からない.後でテスト<-いらないみたいだ
		return true;
	}
	else DSTR << GetSLCErrorCode() << std::endl;

	return false;
}
