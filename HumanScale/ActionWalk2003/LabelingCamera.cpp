#include "LabelingCamera.h"

//using namespace LVRM;
using namespace std;

SLCamera::SLCamera(){
	lCamera = NULL;
}

bool SLCamera::InitSLC(){
	lCamera = OpenSLC("SLC-C01-00006",NULL,NULL);//ラベリングカメラ使いはじめ
	if (lCamera == NULL){
		std::cout << " LabelingCamera Not Existing !" << std::endl;
		return false;
	}
	labelNum = 2;
	alpha = 0.1f;
	lastLabelNum = 0;
	eyeDirection = Vec2f(0.0f,1.0f);
	eyeDir = Vec2f(0.0f,1.0f);
	return true;
}

void SLCamera::CloseSLCamera(){
	CloseSLC(lCamera);
}

bool SLCamera::Step(){ 
	if(lCamera){
		if(GetSLCData(lCamera,labelPos,&labelNum)){ ////ラベリングカメラから値を取得
			for(int i = 0; i < labelNum; i++){
				std::cout << "Number of Label " << labelNum << std::endl;
				std::cout << "Label No " << i << " ========== " << labelPos[i].dX << "," << labelPos[i].dY << "," << labelPos[i].dZ << "," << "Label = "<< labelPos[i].nLabel << " Status = " << labelPos[i].nStatus << std::endl;
			}
			eyeDirection = EyeDirection();
			labelNum = 2;//labelNum は常に2にしてないとダメなようだ.
		}else {
			std ::cout << GetSLCErrorCode() << std::endl;//何でラベルが見れないのかを知るため.
			labelNum = 2;
			return false;
		}
		//SetSLCDebugItem(lCamera,NULL,NULL);//いるかどうか分からない.後でテスト<-いらないみたいだ
		return true;
	}
	return false;
}

int SLCamera::GetLabelPos(Vec2f *labelPosition){
	if(lCamera != NULL){
		for(int i = 0; i < labelNum; i++){
			labelPosition[i] = Vec2f(labelPos[i].dX,labelPos[i].dY);
		}
	}
	return labelNum;
}

int SLCamera::GetLabelPos(Vec3f *labelPosition){
	if(lCamera != NULL){
		for(int i = 0; i < labelNum; i++){
			labelPosition[i] = Vec3f(labelPos[i].dX,labelPos[i].dY,labelPos[i].dZ);
		}
	}
	return labelNum;
}

Vec2f SLCamera::EyeDirection(){
	if (labelNum != 2){ ///ラベリングカメラで2点取れない場合ユーザは，まだターンテーブルに乗っていない，又はスクリーンに正対しているとみなす．
		eyeDir = Vec2f(0.0f,0.1f);
	}
	else if (lastLabelNum != 2){   ///ラベリングカメラで初めて2点取れた場合，又は復帰した場合．#ユーザを横側から入らせないこと．
		leftEye = Vec2f(labelPos[1].dX,labelPos[1].dY); //ラベリングカメラのつけ方次第でleftとrightが反対になるかも
		rightEye = Vec2f(labelPos[0].dX,labelPos[0].dY);
		eyeDir = -Matrix2f(Rad(90)) * (leftEye - rightEye);
		if(eyeDir.norm() != 0){
			eyeDir = eyeDir / eyeDir.norm();//正規化＃する必要ないかも
		}
		else{
			eyeDir = Vec2f(0.0f,0.1f);
		}
	}
	else {
		for(int i = 0; i < 2; i++){    ///前回取得した点との比較
			trackingEye[i] = Vec2f(labelPos[i].dX,labelPos[i].dY);
			dLeftEye[i] = leftEye - trackingEye[i];
			dRightEye[i] = rightEye - trackingEye[i];
		}  
		if( dLeftEye[0].norm() + dRightEye[1].norm() < dLeftEye[1].norm() + dRightEye[0].norm() ){
			leftEye = trackingEye[0];  ///前回との差の比較 ラベルの0番が左目の方,1番が右目の方に近いと判断．
			rightEye = trackingEye[1];
		}
		else{
			leftEye = trackingEye[1]; ///前回との差の比較 ラベルの1番が左目の方,0番が右目の方に近いと判断．
			rightEye = trackingEye[0];
		}
		eyeDir = - Matrix2f(Rad(90)) * (leftEye - rightEye);
		if(eyeDir.norm() != 0){
			eyeDir = eyeDir / eyeDir.norm();//正規化＃する必要ないかも
		}
		else{
			eyeDir = Vec2f(0.0f,0.1f);
		}
	}
	direction = alpha * eyeDir + (1 - alpha) * direction;//ターンテーブルと同じようにフィルタをかける．
	lastLabelNum = labelNum; 
	return direction;
}
