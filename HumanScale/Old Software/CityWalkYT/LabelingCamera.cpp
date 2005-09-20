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
	labelNum = LABEL_NUM;
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
				//std::cout << "Number of Label " << labelNum << std::endl;
				//std::cout << "Label No " << i << " ========== " << labelPos[i].dX << "," << labelPos[i].dY << "," << labelPos[i].dZ << "," << "Label = "<< labelPos[i].nLabel << " Status = " << labelPos[i].nStatus << std::endl;
			}
			eyeDirection = EyeDirection();
			labelNum = LABEL_NUM;//labelNum �͏��2�ɂ��ĂȂ��ƃ_���Ȃ悤��.
		}else {
			std ::cout << GetSLCErrorCode() << std::endl;//���Ń��x��������Ȃ��̂���m�邽��.
			labelNum = LABEL_NUM;
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
//	cout << "labeunum = " << lastLabelNum <<endl;

	if (labelNum != LABEL_NUM){ ///���x�����O�J������2�_���Ȃ��ꍇ���[�U�́C�܂��^�[���e�[�u���ɏ���Ă��Ȃ��C���̓X�N���[���ɐ��΂��Ă���Ƃ݂Ȃ��D
		eyeDir = Vec2f(0.0f,0.1f);
        eyeDirection3D = Vec3f(0.0f, 0.0f, 1.0f);
	}
	else if (lastLabelNum != LABEL_NUM){   ///���x�����O�J�����ŏ��߂�2�_��ꂽ�ꍇ�C���͕��A�����ꍇ�D#���[�U������������点�Ȃ����ƁD
		
		//���x�����O�J�����̂��������left��right�����΂ɂȂ邩��
		topEye   = Vec3f(labelPos[1].dX, labelPos[1].dY, labelPos[1].dZ);
		rightEye = Vec3f(labelPos[0].dX, labelPos[0].dY, labelPos[0].dZ);
		leftEye  = Vec3f(labelPos[2].dX, labelPos[2].dY, labelPos[2].dZ); 
		/*
		cout << "Right: " << rightEye << endl;
		cout << "TOP:   " << topEye << endl;
		cout << "Left:  " << leftEye << endl ;
		cout << endl;
		*/
		rightEye2D.X() = rightEye.X();
		rightEye2D.Y() = rightEye.Z();
		leftEye2D.X() = leftEye.X();
		leftEye2D.Y() = leftEye.Y();
		eyeDir = -Matrix2f(Rad(90)) * (leftEye2D - rightEye2D);
		if(eyeDir.norm() != 0){
			eyeDir = eyeDir / eyeDir.norm();//���K��������K�v�Ȃ�����
		}
		else{
			eyeDir = Vec2f(0.0f,0.1f);
		}
	}
	else {
		for(int i = 0; i < LABEL_NUM; i++){    ///�O��擾�����_�Ƃ̔�r
			trackingEye[i] = Vec3f(labelPos[i].dX,labelPos[i].dY,labelPos[i].dZ);
			dLeftEye[i] = leftEye - trackingEye[i];
			dRightEye[i] = rightEye - trackingEye[i];
			dtopEye[i] = topEye - trackingEye[i];
		} 
		/* �Ƃ肠�������̂ւ�͑S���J�b�g
		if( dLeftEye[0].norm() + dRightEye[1].norm() < dLeftEye[1].norm() + dRightEye[0].norm() ){
			leftEye = trackingEye[1];  ///�O��Ƃ̍��̔�r ���x����0�Ԃ����ڂ̕�,1�Ԃ��E�ڂ̕��ɋ߂��Ɣ��f�D
			rightEye = trackingEye[2];
			topEye = trackingEye[0];
		}
		else{
			leftEye = trackingEye[2]; ///�O��Ƃ̍��̔�r ���x����1�Ԃ����ڂ̕�,0�Ԃ��E�ڂ̕��ɋ߂��Ɣ��f�D
			rightEye = trackingEye[1];
			topEye = trackingEye[0];
		}
		*/
		
		// ��ԉE�ɂ�����̂��珇�Ƀ}�[�J�[���F�������悤�Ȃ̂�
		topEye = trackingEye[1];
		rightEye = trackingEye[0];
		leftEye = trackingEye[2];
		
		// �񎟌��ł̂��߂�
		rightEye2D.X() = rightEye.X();
		rightEye2D.Y() = rightEye.Y();
		leftEye2D.X() = leftEye.X();
		leftEye2D.Y() = leftEye.Y();
		/*
		cout << "[TOP]:   " << topEye << endl;
		cout << "[Right]: " << rightEye << endl;
		cout << "[Left]:  " << leftEye << endl ;
		cout << endl;
		*/

		// �O���ɂ́AD-vision�̍��W�n�ŕԂ����ƁI�I�i�\��j



		rightEyeVec = rightEye - leftEye;     // �E�ڂ̌���
		centerPoint = (leftEye + rightEye) / 2.0f;  // �E�ڂƍ��ڂ̒��S���W
		thirdEyeVec = (topEye - centerPoint);    // ��O�̖ڂւ̃x�N�g��

		// ���W�n��D-vision�ɂ��킹��
		//rightEyeVec.X() *= - 1.0f;
		tmp = rightEyeVec.Y();
		rightEyeVec.Y() =  - rightEyeVec.Z();
		rightEyeVec.Z() =  - tmp;

		//thirdEyeVec.X() *= - 1.0f;
		tmp = thirdEyeVec.Y();
		thirdEyeVec.Y() = - thirdEyeVec.Z();
		thirdEyeVec.Z() = - tmp;
		

		eyeDirection3D = thirdEyeVec % rightEyeVec;  // �����������v�Z (�O��)
		if(eyeDirection3D.norm() != 0)    // ���K��
				eyeDirection3D = eyeDirection3D / eyeDirection3D.norm();
		/*
		cout << "[Right] " << rightEyeVec << endl;
		cout << "[ Top ] " << thirdEyeVec << endl;
		*/
		//cout << "[ Dir ] " << eyeDirection3D << endl;
		/*
		cout << endl;
		*/

		eyeDir = - Matrix2f(Rad(90)) * (leftEye2D - rightEye2D);
		//eyeDir = rightEye2D - leftEye2D;
		if(eyeDir.norm() != 0){
			eyeDir = eyeDir / eyeDir.norm();//���K��������K�v�Ȃ�����
		}
		else{
			eyeDir = Vec2f(0.0f,0.1f);  // �����@�f�t�H���g�������I�I�I�i�����j
		}
	}
	// �E�ڃx�N�g��
	//cout << "[eyeDir] : " << eyeDir << endl;
	direction = alpha * eyeDir + (1 - alpha) * direction;//�^�[���e�[�u���Ɠ����悤�Ƀt�B���^��������D
	direction = direction / direction.norm();  // ���K��
	lastLabelNum = labelNum;
	

	// D-vision���W�n�֕ϊ�
	/*
	tmp = direction.Y();
	direction.Y() = - direction.Z();
	direction.Z() = - tmp;
	*/
//cout << "[Dir] :" << direction << endl; 

	// 3�����ł̎��������̌v�Z

	return direction;
}
