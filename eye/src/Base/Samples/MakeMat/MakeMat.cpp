#include <Base/Affine.h>
#include <iostream>
#include <stdlib.h>
using namespace Spr;
using namespace std;
int main(int argc, char** argv){
	if (argc != 4){
		cout << "DirectX�t�@�C���p�̉�]�s��t�B�[���h�𐶐����܂��D" << endl;
		cout << "Affine(Rad(x), 'x') * Affine(Rad(y), 'y')"
				" * Affine(Rad(z), 'z')" << endl;
		cout << "�̒l�����߂��܂��D " << endl;
		cout << "�R�}���h���C���F" << endl;
		cout << "  MakeMat x y z" << endl;
		cout << "�Ӗ��F" << endl;
		cout << "  x: x�����̉�]�p" << endl;
		cout << "  y: y�����̉�]�p" << endl;
		cout << "  z: z�����̉�]�p" << endl;
		return -1;
	}
	Vec3f r;
	r.X() = (float)atof(argv[1]);
	r.Y() = (float)atof(argv[2]);
	r.Z() = (float)atof(argv[3]);
	Affinef af;
	af = af * Affinef::Rot((float)Rad(r.X()), 'x');
	af = af * Affinef::Rot((float)Rad(r.Y()), 'y');
	af = af * Affinef::Rot((float)Rad(r.Z()), 'z');
	cout.setf(ios::showpoint);
	for(int row=0; row<4; row++){
		for(int col=0; col<4; col++){
			cout << af[row][col];
			if (row==3 && col == 3){
				cout << ";;";
			}else{
				cout << ",";
			}
		}
		cout << endl;
	}
	return 0;
}
