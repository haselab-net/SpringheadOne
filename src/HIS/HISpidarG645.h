	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
	#define PX	(0.390f/2)		//	x�����̕ӂ̒���/2
	#define PY	(0.395f/2)		//	y�����̕ӂ̒���/2
	#define PZ	(0.405f/2)		//	z�����̕ӂ̒���/2
/*	Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
*/
	#define GX	(0.065f/2)		//	x�����̕ӂ̒���/2
	#define GY	(0.065f/2)		//	y�����̕ӂ̒���/2
	Vec3f motorPos[8][2] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f( PX, PY,-PZ), Vec3f( GX,  0, 0)},
		{Vec3f(-PX, PY, PZ), Vec3f(  0, GY, 0)},
		{Vec3f( PX,-PY, PZ), Vec3f(  0,-GY, 0)},
		{Vec3f(-PX,-PY,-PZ), Vec3f(-GX,  0, 0)},
		{Vec3f(-PX, PY,-PZ), Vec3f(-GX,  0, 0)},
		{Vec3f( PX, PY, PZ), Vec3f(  0, GY, 0)},
		{Vec3f(-PX,-PY, PZ), Vec3f(  0,-GY, 0)},
		{Vec3f( PX,-PY,-PZ), Vec3f( GX,  0, 0)}
	};

	for(int i=4; i<8; ++i){
		spidar.motor[i].lengthPerPulse *= -1;
	}

	spidar.Init(devMan, 8, motorPos, 0.3f, 2.924062107079e-5f, 1.0f, 20.0f);
