�ȒP�Ȑ�����

���������ȒP�ɂł����A�ΐ�̏������v���O����(VHInteraction)�̐��������܂��B
�����p�����ɂ͐\����Ȃ��ł����A���Ԃ̓s����ڂ��������ł��Ȃ��Ȃ��Ă��܂��܂����B
�R�����g�������Ȃ��A���Â炢�\�[�X�R�[�h�ł��߂�Ȃ����B


Creature\CRActionPlanner.cpp
    CRActionPlanner�N���X�F
        VH�̗\���s�����f��(�^���\�����ڐG���聨�s���I�������쌈��)
        
        Step()�F
            �^���\���J�n���Ɏ��̉^���̋ߎ����������v�Z(CRPuppet::LocusGenerator)
            �\������PredictionAction()��
        PredictionAction()�F
            �����ԉ^���̏�ԕۑ���MovementPrediction()����ԓǂݍ��݁��s���ύX���邩����(ChooseTargetAction())
        MovementPrediction()�F
            �\���^���̏�ԓǂݍ��݁�PredictionStep()��ContactTest()����ԕۑ�
        PredictionStep()�F
            �\���^����scene�̍X�V
            VH�̓��B�^�������User�̎��^��(user->SetExpectedPos())�ɕK�v�ȗ͂�������
            // �\�������͉������̂ōl�������K�v����
        ContactTest()�F
            VH��User�̍��̓��m�̐ڐG����(CRPuppet::HumanContactInfo)
            �ڐG���(ContactInfo�N���X)��ۑ�
            ContactInfo�N���X�F
                VH�̍��́AUser�̍��́A�ڐG�ʒu�A�ڐG�����A�ڐG�^�C�v
        ChooseTargetAction()�F
            �ڐG��񂩂玟�̖ڕW�s���I������ѓ��쌈��(������)
            VH�̖ڕW�s������ѓ���́A���r����ё̂̌v�R�����ւ̓��B�^���w��(CRPuppet::ReachingMovement)
            // ���݂͍U���ɑ΂��Ă̖h��s���̂݊���
            �h�䂨��щ���̖ڕW�ʒu�́A���ꂼ��GetPointToGuard()�EGetPointToAvoid()�Ōv�Z

Creature\CRPuppet.cpp
    CRPuppet�N���X�F
        CRHuman�̔h���N���X
        PD����ɂ��O�͂ɂĉ^������
        �Œ�ӏ��F�����̈ʒu�E�p��
        
        SetSpringForce()�F
            �ʒu�p������̊O�͌v�Z
        Step()�F
            SetSpringForce()����ѓ��B�^������
        Attack()�F
            �U������̓��B�^������
            // ���������͊�{�I�Ƀ����_��
        GuardTest()�F
            �V�~�����[�^�ɂ��\�������ł̖h�䓮��̎����Ɏg�p
        IsAimed()�F
            CRActionPlanner�ɂĉ^���\�����J�n���邩�̔��f�ޗ�
        
        PositionSpring�N���X�F
                ���̂̈ʒu����̃o�l(���́A����ʒu�A�ڕW�ʒu�A�ڕW���x�A������́A...)
                AddSpringForce()�F
                    PD����ɂ��K�v�ȗ͂̌v�Z
        PostureSpring�N���X�F
                ���̂̎p������̃o�l(���́A�ڕW�p��(�N�H�[�^�j�I��)�A�ڕW�p���x�A������g���N�A...)
                AddSpringTorque()�F
                    PD����ɂ��K�v�ȃg���N�̌v�Z
        ReachingMovement�N���X�F
                ���B�^������(�ŏI�ڕW�ʒu�A�ŏI�ڕW���x�A�ڕW���ԁA��������)
                // ��{��PositionSpring�Ɠ��l
                Step()�F
                    �ڕW���ԓ��͎��X�e�b�v�܂ł̉��ڕW����PD����
                    �ڕW���Ԃ��I��������A�������Ԃ��I���܂ōŏI�ڕW�ʒu�Ɉʒu����
                // ���B�^�������͂�����d���ō�����̂ŁA�������ł�
        LocusGenerator�N���X�F
                ���̂̉ߋ��̋O�Ղ���A����̋O�����ŏ����@�ɂ�葽�����ߎ�
                // �ߋ��T���v�����⑽�����̎����E�d�݂͎����i�K
        HumanContactInfo�N���X�F
                CRHuman���m�̍��̂̐ڐG����
                ���̓��m�̐ڐG����AHuman�Ƒ��荄�̂̐ڐG����A�ڐG�͂̌v�Z�Ȃ�

Creature\CRUser.cpp
    CRUser�N���X�F
        CRPuppet�̔h���N���X
        �Œ�ӏ��F�����̈ʒu�E�p��
        SPIDAR�ɂ�鐧��ΏہF�����
        
        SetSpidarPos()�F
            SPIDAR�ɂČv�����ꂽ�ʒu��񂩂�A�����̖ڕW������
        SetExpectedPos()�F
            CRActionPlanner�ɂĉ^���\�����̗����̖ڕW���ALocusGenerator���猈��
        SetSpidarForce()�F
            User���̂ɐ������ڐG�͂��


 �� ����B�e��f���ł̌��h����ǂ�����ׂɁA�e��p�����[�^��֐����e�𑽏����������Ă���܂��B
    ���̂��ߘ_���̓��e�ƕK��������v���Ȃ��������������L��Ǝv���܂��B
