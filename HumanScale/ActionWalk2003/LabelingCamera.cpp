#include "LabelingCamera.h"

//using namespace LVRM;
using namespace std;

SLCamera::SLCamera(){
	lCamera = NULL;
}

bool SLCamera::InitSLC(){
	lCamera = OpenSLC("SLC-C01-00006",NULL,NULL);//���x�����O�J�����g���͂���
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
		if(GetSLCData(lCamera,labelPos,&labelNum)){ ////���x�����O�J��������l���擾
			for(int i = 0; i < labelNum; i++){
				std::cout << "Number of Label " << labelNum << std::endl;
				std::cout << "Label No " << i << " ========== " << labelPos[i].dX << "," << labelPos[i].dY << "," << labelPos[i].dZ << "," << "Label = "<< labelPos[i].nLabel << " Status = " << labelPos[i].nStatus << std::endl;
			}
			eyeDirection = EyeDirection();
			labelNum = 2;//labelNum �͏��2�ɂ��ĂȂ��ƃ_���Ȃ悤��.
		}else {
			std ::cout << GetSLCErrorCode() << std::endl;//���Ń��x��������Ȃ��̂���m�邽��.
			labelNum = 2;
			return false;
		}
		//SetSLCDebugItem(lCamera,NULL,NULL);//���邩�ǂ���������Ȃ�.��Ńe�X�g<-����Ȃ��݂�����
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
	if (labelNum != 2){ ///���x�����O�J������2�_���Ȃ��ꍇ���[�U�́C�܂��^�[���e�[�u���ɏ���Ă��Ȃ��C���̓X�N���[���ɐ��΂��Ă���Ƃ݂Ȃ��D
		eyeDir = Vec2f(0.0f,0.1f);
	}
	else if (lastLabelNum != 2){   ///���x�����O�J�����ŏ��߂�2�_��ꂽ�ꍇ�C���͕��A�����ꍇ�D#���[�U������������点�Ȃ����ƁD
		leftEye = Vec2f(labelPos[1].dX,labelPos[1].dY); //���x�����O�J�����̂��������left��right�����΂ɂȂ邩��
		rightEye = Vec2f(labelPos[0].dX,labelPos[0].dY);
		eyeDir = -Matrix2f(Rad(90)) * (leftEye - rightEye);
		if(eyeDir.norm() != 0){
			eyeDir = eyeDir / eyeDir.norm();//���K��������K�v�Ȃ�����
		}
		else{
			eyeDir = Vec2f(0.0f,0.1f);
		}
	}
	else {
		for(int i = 0; i < 2; i++){    ///�O��擾�����_�Ƃ̔�r
			trackingEye[i] = Vec2f(labelPos[i].dX,labelPos[i].dY);
			dLeftEye[i] = leftEye - trackingEye[i];
			dRightEye[i] = rightEye - trackingEye[i];
		}  
		if( dLeftEye[0].norm() + dRightEye[1].norm() < dLeftEye[1].norm() + dRightEye[0].norm() ){
			leftEye = trackingEye[0];  ///�O��Ƃ̍��̔�r ���x����0�Ԃ����ڂ̕�,1�Ԃ��E�ڂ̕��ɋ߂��Ɣ��f�D
			rightEye = trackingEye[1];
		}
		else{
			leftEye = trackingEye[1]; ///�O��Ƃ̍��̔�r ���x����1�Ԃ����ڂ̕�,0�Ԃ��E�ڂ̕��ɋ߂��Ɣ��f�D
			rightEye = trackingEye[0];
		}
		eyeDir = - Matrix2f(Rad(90)) * (leftEye - rightEye);
		if(eyeDir.norm() != 0){
			eyeDir = eyeDir / eyeDir.norm();//���K��������K�v�Ȃ�����
		}
		else{
			eyeDir = Vec2f(0.0f,0.1f);
		}
	}
	direction = alpha * eyeDir + (1 - alpha) * direction;//�^�[���e�[�u���Ɠ����悤�Ƀt�B���^��������D
	lastLabelNum = labelNum; 
	return direction;
}
