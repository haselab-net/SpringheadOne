// SLCamera.cpp: SLCamera クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SLCamera.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

SLCamera::SLCamera(){
	lCamera = NULL;
	bStep   = false;
	bCalib  = false;
}

SLCamera::~SLCamera()
{
	CloseSLCamera();
}

bool SLCamera::InitSLC(){
	lCamera = OpenSLC("SLC-C01-00023",NULL,NULL);//ラベリングカメラ使いはじめ
	if (lCamera == NULL){
		//std::cout << " LabelingCamera Not Existing !" << std::endl;
		return false;
	}
	//else std::cout << " Succeed in getting LabelingCamera " << std::endl; //add osada
	labelNum = LABEL_NUM;
	lastLabelNum = 0;

	return true;
}

void SLCamera::Init(){
	bStep = true;
}

void SLCamera::CloseSLCamera(){
	if(lCamera) CloseSLC(lCamera);
}

bool SLCamera::Draw(Spr::GRRender* render){
	if(!lCamera) return false;
	
	render->SetModelMatrix(Spr::Affinef());
	render->SetLineWidth(10);
	for(int i = 0; i < MaxMarkerNum; i++){
		int m = MaxMarkerNum - 1;
		if(m == 0) m = 1;

		Spr::Vec4f color = Spr::Vec4f(((float)i / m), 1 - ((float)i / m), 1, 1);	// 色分け
		if(marker[i].match == -1) color = Spr::Vec4f(1, 1, 1, 1);

		Spr::Vec3f pos = marker[i].pos;
		render->SetMaterial(Spr::GRMaterialData(color));
		render->DrawDirect(Spr::GRRender::POINTS, &pos, &pos+1);
	}
	return true;
}

void SLCamera::Calib(SLCOBJECT label){
	// ４定点による"カメラ座標系→Springhead座標系"の変換行列の計算
	// ４定点：(1,0,0)(0,1,0)(0,0,1)(-1,0,0)
	Vec4f c[4], p[4];
	PTM::TMatrixRow<4,4,float> c44, p44;

	c[0] = Vec4f( 1.0f, 0.0f, 0.0f, 1.0f);
	c[1] = Vec4f( 0.0f, 1.0f, 0.0f, 1.0f);
	c[2] = Vec4f( 0.0f, 0.0f, 1.0f, 1.0f);
	c[3] = Vec4f(-1.0f, 0.0f, 0.0f, 1.0f);

	for(int i = 0; i < 4; ++i){
		p[i].x = label.dX;
		p[i].y = label.dY;
		p[i].z = label.dX;
		p[i].w = 1.0f;
		c44.col(i) = c[i];
		p44.col(i) = p[i];
	}
	T = p44 * c44.inv();
	bCalib = true;
}

Spr::Vec3f SLCamera::Convert(SLCOBJECT label){
	Spr::Vec3f lpos;
	if(bCalib){
		lpos.x = label.dX;
		lpos.y = label.dY;
		lpos.z = label.dZ;
		return T * lpos;
	}
	// ラベリングカメラの座標変換込み(手書き)
	lpos.x = -label.dY;
	lpos.y = -label.dX + 100;
	lpos.z =  label.dZ - 50;
	lpos  /= 100;
	return lpos;
}

void SLCamera::SaveLabelPos(){
	for(int i = 0; i < markerNum; ++i) tmpLabelPos[i] = Convert(labelPos[i]);
}

void SLCamera::SaveDistance(){
	for(int i = 0; i < markerNum; ++i){
		for(int j = 0; j < MaxMarkerNum; ++j){
			if(i == 0) marker[j].dist.clear();
			marker[j].dist.push_back((tmpLabelPos[i] - marker[j].pos).norm());
		}
	}
}

void SLCamera::IncorrectRecognitionCheck(){
	float d;
	for(int i = 0; i < markerNum - 1; ++i){
		d  = (tmpLabelPos[i] - tmpLabelPos[i+1]).norm();	// 隣のラベルとの距離
		if(d < 0.001f || tmpLabelPos[i].norm() > 10){		// 隣のラベルと近すぎる・位置がおかしい
//			DSTR << tmpLabelPos[i] << std::endl;
//			DSTR << tmpLabelPos[i].norm() << std::endl;
//			DSTR << d << std::endl;
			for(int j = i; j < markerNum - 1; ++j){
				tmpLabelPos[j] = tmpLabelPos[j+1];
			}
			markerNum--;
		}
	}
}

/*void SLCamera::SearchNeighborhood(){
	for(int j = 0; j < MaxMarkerNum; j++){
		for(int i = 0; i < markerNum; ++i) neighborhood[i][j] = i;	// 初期化
		// バブルソート
		for(int i = 0; i < markerNum - 1; ++i){
			for(int k = markerNum - 1; k > i; --k){
				if(dist[neighborhood[j][k]][j] < dist[neighborhood[j][k-1]][j]){
					int tmp = neighborhood[j][k];
					neighborhood[j][k] = neighborhood[j][k-1];
					neighborhood[j][k-1] = tmp;
				}
			}
		}
	}
}*/

float SLCamera::CalcTotalCost(int l, float cost, float min){
	float tmp;
	if(l == markerNum){					// 再帰終了
		if(cost <= min) return cost;	// 最小コストを返す
		else			return min;	
	}

	for(int i = 0; i < MaxMarkerNum; i++){
		if(marker[i].match == -1){					// 今までに選ばれていないマーカー
			if(cost + marker[i].dist[l] <= min){	// 最小コストを更新する可能性有り
				marker[i].match = l;				// マーカー[i]をラベル[l]と対応付ける
				tmp = CalcTotalCost(l + 1, cost + marker[i].dist[l], min);

				if(tmp <= min){		// 今までの最小コスト更新
					min = tmp;
					match[l] = i;	// ラベル[l]はマーカー[i]と結ばれました
				}
			}
			marker[i].match = -1;
		}
	}
	return min;
}

void SLCamera::Matching(){
	for(int i = 0; i < MaxMarkerNum; ++i) marker[i].match = -1;
	CalcTotalCost(0, 0, 10000);
	for(int i = 0; i < markerNum; ++i){
		marker[match[i]].match = i;	// マーカーとラベルのマッチング
		marker[match[i]].state = labelPos[i].nStatus;
		marker[match[i]].pos = tmpLabelPos[i];
	}
}

void SLCamera::SetInitMarkerPos(Spr::Vec3f* soPos){
	for(int i = 0; i < MaxMarkerNum; ++i){
		marker[i].pos = soPos[i];
	}
}

/*void SLCamera::CaseOfErasure(){
	int match[MaxMarkerNum];
	int label;
	for(int i = 0; i < MaxMarkerNum; i++) match[MaxMarkerNum] = -1;
	for(int i = 0; i < MaxMarkerNum; i++){
		label = neighborhood[0][i];
		if(match[label] == -1){	// マッチングしていない

		}
	}
}*/

void SLCamera::SetUserPos(CRUser* user){
	user->positionSprings[2].SetTarget(marker[0].pos, marker[0].vel, false);
	user->positionSprings[3].SetTarget(marker[1].pos, marker[1].vel, false);
	user->positionSprings[4].SetTarget(marker[2].pos, marker[2].vel, false);
}

bool SLCamera::Step(Spr::SGScene* scene){ 
	if(lCamera){
		markerNum = 0;
		if(GetSLCData(lCamera, labelPos, &labelNum)){ ////ラベリングカメラから値を取得
			if(bStep){
				markerNum = labelNum;			// 取得したラベルの数
				SaveLabelPos();					// ラベル位置を仮保存
				IncorrectRecognitionCheck();	// ラベルを重複して認識していないかチェック
				SaveDistance();					// ラベルとマーカーの距離を計算

				Matching();						// マーカー追従
				
				Spr::GRRender* render;
				scene->GetRenderers().Find(render);
				Draw(render);

				labelNum = LABEL_NUM;	// labelNum は常に2にしてないとダメなようだ.
			}
		}
		else{
			//std ::cout << GetSLCErrorCode() << std::endl;	// 何でラベルが見れないのかを知るため.
			labelNum = LABEL_NUM;
			return false;
		}
		//SetSLCDebugItem(lCamera,NULL,NULL);	// いるかどうか分からない.後でテスト<-いらないみたいだ
		return true;
	}
	//else DSTR << GetSLCErrorCode() << std::endl;
	return false;
}

}
