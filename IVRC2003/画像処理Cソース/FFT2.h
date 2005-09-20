/* �Q���� FFT �̃p�b�P�[�W FFT2.h           */
/* ���Ŋ֐� FFT1 ��p���Ă���D             */
/* FFT1.h �̌�ɃC���N���[�h���ėp���邱�ƁD*/
/* ���ϐ� */
double  data[FFT_MAX][FFT_MAX];   /* ���f�[�^�̎����� */
double jdata[FFT_MAX][FFT_MAX];   /* ���f�[�^�̋����� */
int num_of_data;          /* �f�[�^�̗v�f�ԍ��̍ő�l */
/* �֐��̃v���g�^�C�v */
void FFT2( int flag );

void FFT2( int flag )
/* ���f�[�^�� data, jdata, num_of_data �ɑ�����Ă���g�� */
/* flag = 1 : �Q���� FFT, flag = -1 : �Q�����t FFT        */
{
    int i, j; /* ���[�v�ϐ� */
    static double re[FFT_MAX], im[FFT_MAX]; /* ��ƕϐ� */

    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
            re[j] =  data[i][j];
            im[j] = jdata[i][j];
        }
        FFT1( re, im, num_of_data, flag );
        for ( j = 0; j < num_of_data; j ++ ){
             data[i][j] = re[j];
            jdata[i][j] = im[j];
        }
    }
    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
            re[j] =  data[j][i];
            im[j] = jdata[j][i];
        }
        FFT1( re, im, num_of_data, flag );
        for ( j = 0; j < num_of_data; j ++ ){
             data[j][i] = re[j];
            jdata[j][i] = im[j];
        }
    }
}
