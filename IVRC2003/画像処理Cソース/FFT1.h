/* �P���� FFT �̃p�b�P�[�W FFT1.h */
#define FFT_MAX  1024
#define PI  3.141592653589  /* �~���� */
int calc_power_of_two( int number );
void make_initial_data( double *re_part, double *im_part, 
                        int num_of_data, int power );
void FFT1( double *re_part, double *im_part, int num_of_data, 
           int flag );

int calc_power_of_two( int number )
/* number ���Q�̉���ł��邩���ׂĕԂ� */
/* ��j8 --> 3, 16 --> 4, 32 --> 5,... */
{
    int power;

    power = 0;
    while ( number != 1 ){
        power ++;
        number = number >> 1; 
            /* �E�����ɂP�r�b�g�V�t�g�i�Q�Ŋ���̂Ɠ����j */
    }
    return power;
}

void make_initial_data( double *re_part, double *im_part, 
                        int num_of_data, int power )
/* �f�[�^����ёւ��Ď��Ԃ̊Ԉ����ɂ�� FFT �̏����f�[�^����� */
/* double *re_part, *im_part; ���f�[�^(�������C������)�̐擪�A�h���X */
/* int num_of_data;           �f�[�^����                             */
/* int power;                 num_of_data �͂Q�� power ��            */
{
    int i, j;              /* ����ϐ�                               */
    int ptr, offset;       /* ���f�[�^�̗v�f�ԍ������߂邽�߂̕ϐ�   */
    int new_ptr;           /* �v�Z��̃f�[�^��������z��̗v�f�ԍ� */
    int dft;               /* DFT�_�� ( i �̃��[�v�Ŕ������ɂȂ�j */
    double re_buf[FFT_MAX], im_buf[FFT_MAX]; /* �v�Z���ʗp�z�� */

    dft = num_of_data;
    for ( i = 1; i < power; i ++ ){
        /* i �͉񐔂𐧌䂷��ϐ��i�v�Z�ɂ͗p���Ă��Ȃ��j */
        new_ptr = 0;  offset = 0;
        while( new_ptr < num_of_data ){
            ptr = 0;
            while( ptr < dft ){
                re_buf[new_ptr] = *( re_part + offset + ptr );
                im_buf[new_ptr] = *( im_part + offset + ptr );
                new_ptr ++;
                ptr = ptr + 2;
                if ( ptr == dft ) ptr = 1;
            }
            offset = offset + dft;
        }
        /* �v�Z���ʂ����̃f�[�^�z��ɃR�s�[���� */
        for ( j = 0; j < num_of_data; j ++ ){
            *( re_part + j ) = re_buf[j];
            *( im_part + j ) = im_buf[j];
        }
        dft = dft / 2;
    }
}

void FFT1( double *re_part, double *im_part, int num_of_data, int flag )
/* �f�[�^�� FFT (flag = 1), �t FFT (flag = -1) ���s���֐�            */
/* double *re_part, *im_part; ���f�[�^(�������C������)�̐擪�A�h���X */
/* int num_of_data, flag;     �f�[�^�����CFFT�E�tFFT �����߂�t���O  */
{
    int i, j, k, power;                         /* ����ϐ��C�ׂ��� */
    double unit_angle, step_angle;              /* �p�x�p�̕ϐ�     */
    int dft, half, num_of_dft;   /* DFT�_���C���̔����CDFT ���s�� */
    int num_out, num_in1, num_in2;       /* DFT�̓��o�͐M�����̔ԍ� */
    double re_buf, im_buf, angle; /* �������C�������C�p�x�p��ƕϐ� */
                  /* ���͌v�Z���ʂ��ꎞ�I�ɕۑ����邽�߂̍�Ɨp�z�� */
    static double re_part_new[FFT_MAX], im_part_new[FFT_MAX];

    /* �tFFT ( flg = -1 ) �̂Ƃ��e�f�[�^�� num_of_data �Ŋ��� */
    /* ���f�������Ƃ�                                         */
    if ( flag == -1){
        for ( i = 0; i < num_of_data; i ++ ){
            *( re_part + i ) =   *( re_part + i ) / num_of_data;
            *( im_part + i ) = - *( im_part + i ) / num_of_data;
        }
    }
    /* num_of_data ���Q�̉���(power)�ł��邩���ׂ� */
    power = calc_power_of_two( num_of_data );
    /* �����f�[�^�̂��߂̌��f�[�^�̏��Ԃ̓���ւ� */
    make_initial_data( re_part, im_part, num_of_data, power );
    /* 2�_DFT, 4�_DFT, 8�_DFT, ... �̏��Ɏ��s���� */
    unit_angle = 2.0 * PI / num_of_data;
    dft = 2;   /* = DFT �̓_���̏����l�C�Q�{���Ă䂭 */
    for ( i = 0; i < power; i ++ ){
        /* DFT �_�� DFT ���s��                      */
        /* i=0 -> 2�_, i=1 -> 4�_, i=2 -> 8�_, .... */
        num_of_dft = num_of_data / dft;   /* DFT �̎��s�� */
        step_angle = unit_angle *  num_of_dft;
        half = dft / 2;
        for ( j = 0; j < num_of_dft; j ++ ){
            angle = 0.0;
            for ( k = 0; k < dft; k ++ ){
                /* No.num_in1 �� No.num_in2 ���� No.num_out ���o�� */
                /* �i�W�����P�̕��̐M���� No.num_in1 �Ƃ��Ă���j  */
                num_out = j * dft + k;
                if ( k < half ){
                    num_in1 = num_out;
                    num_in2 = num_in1 + half;
                } else
                {
                    num_in2 = num_out;
                    num_in1 = num_out - half;
                }
                /* ������(re_)�E������(im_)�ɕ����Čv�Z */
                re_buf = *( re_part + num_in2 );
                im_buf = *( im_part + num_in2 );
                re_part_new[num_out] = *( re_part + num_in1 ) 
                    + re_buf * cos(angle) + im_buf * sin(angle);
                im_part_new[num_out] = *( im_part + num_in1 ) 
                    + im_buf * cos(angle) - re_buf * sin(angle);
                /* �p�x���X�V */
                angle = angle + step_angle;
            }
        }
        /* �v�Z��̃f�[�^�����̔z��ɖ߂� */
        for ( j = 0; j < num_of_data; j ++ ){
            *( re_part + j ) = re_part_new[j];
            *( im_part + j ) = im_part_new[j];
        }
        dft = dft * 2; /* DFT �̓_�����Q�{�ɍX�V */
    }
    /* �tFFT ( flg = -1 ) �̂Ƃ��e�f�[�^�̕��f�������Ƃ� */
    if ( flag == -1 ){
        for ( j = 0; j < num_of_data; j ++ ){
            *( im_part + j ) = - *( im_part + j );
        }
    }
}
