/* 1����FFT�ɂ��t�B���^�����O�̃v���O���� FFT1fltr.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"
#include"FFT1.h"  /* 1����FFT�E�tFFT�̃w�b�_ */

double  data[FFT_MAX]; /* ���f�[�^�̎����� */
double jdata[FFT_MAX]; /* ���f�[�^�̋����� */
int num_of_data;       /* �f�[�^�̗v�f��   */

void display_data( )
/* �f�[�^��\������ */
{
    int i;    /* ����ϐ� */

    for ( i = 0; i < num_of_data; i ++ ){
        printf("No.%3d: ������ =%f, ������ =%f\n",i,data[i],jdata[i]);
    }
    printf("�����L�[�������ĉ����� : ");
    (void)getchar( );    printf("\n");
}

void make_test_data( )
/* �e�X�g�p�̃f�[�^�����i��F��`��̔g�`�j  */
{
    int i, min, max; /* ��ƕϐ� */ 
    
    /* �v�f No.min �` No.max �̎��������� 100.0 */
    num_of_data = 16;   min = 4;   max = 12;
    for ( i = 0; i < num_of_data; i ++ ){
        if ( min <= i && i < max ) data[i] = 100.0;
        else data[i] = 0.0;
        jdata[i] = 0.0;
    }
}

void filtering_frequency( )
/* ���g���̈�ł̃t�B���^�����O */
{
    int i, max; /* ��ƕϐ� */

    /* ��jlow pass �t�B���^�i���ʉ߃t�B���^�j */
    max = num_of_data / 4;
    for ( i = 0; i < num_of_data; i ++ ){
        if ( max <= i && i <= ( num_of_data - max ) ){
             data[i] = 0.0;
            jdata[i] = 0.0;
        }
    }
}

main( )
{
    /* �e�X�g�f�[�^����� */
    make_test_data( );
    printf("�e�X�g�f�[�^�����܂����D\n");
    display_data( );

    /* �����t�[���G�ϊ� */
    FFT1( data, jdata, num_of_data, 1 );
    printf("FFT ���I���܂����D\n");
    display_data( );

    /* ���g���̈�̃t�B���^�����O */
    filtering_frequency( );
    printf("�t�B���^�����O(���ʉ�)���I���܂����D\n");
    display_data( );

    /* �t�����t�[���G�ϊ� */
    FFT1( data, jdata, num_of_data, -1 );
    printf("�t FFT ���I���܂����D\n");
    display_data( );

    return 0;
}