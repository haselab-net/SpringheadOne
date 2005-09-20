/* 2����FFT�ɂ��t�B���^�����O�̃v���O���� FFT2fltr.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"
#include"FFT1.h"
#include"FFT2.h"

void make_original_data( )
/* image1[y][x] �̃f�[�^�� data[y][x], jdata[y][x] �ɑ������ */
{
    int i, j; /* ����ϐ� */

    if ( x_size1 != y_size1 ){
        printf("�c�Ɖ��̉�f�����قȂ��Ă���̂ŏI�����܂��D\n");
        exit(-1);
    } else {
        printf("\n�ǂݍ��񂾉摜�����f�[�^�ɒ����܂��D\n");
        num_of_data = x_size1;
        for ( i = 0; i < num_of_data; i ++ ){
            for ( j = 0; j < num_of_data; j ++ ){
                 data[i][j] = (double)image1[i][j];
                jdata[i][j] = 0.0;
            }
        }
    }
}

void filtering_frequency( )
/* ���g���̈�ɑ΂���t�B���^�����O���s�� */
/* ��F���ʉ߃t�B���^                   */
{
    int i, j, max;

    printf("\nFFT��̌W���ɑ΂���t�B���^�����O���s���܂��D\n");
    max = num_of_data / 8;
    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
            if ( ( max < i ) && ( i < ( num_of_data - max ) ) ||
                 ( max < j ) && ( j < ( num_of_data - max ) ) ){
                 data[i][j] = 0.0;
                jdata[i][j] = 0.0;
            }
        }
    }
}

void make_output_image( )
/* �f�[�^ data[y][x], jdata[y][x] �� image2[y][x] �ɒ��� */
{
    int x, y;

    printf("\n�tFFT�ϊ���̉摜���쐬���܂��D\n");
    x_size2 = num_of_data;    y_size2 = num_of_data;
    for ( y = 0; y < y_size2; y ++ ){
        for ( x = 0; x < x_size2; x ++ ){
            if ( data[y][x] < 0 ) data[y][x] = 0;
            if ( data[y][x] > MAX_BRIGHTNESS ) 
                     data[y][x] = MAX_BRIGHTNESS;
            image2[y][x] = (unsigned char)data[y][x];
        }
    }
}

main( )
{
    load_image_data( );     /* ���摜�̓ǂݍ���       */
    make_original_data( );  /* ���̃f�[�^���쐬����   */
    FFT2( 1 );              /* �Q���� FFT �̎��s      */
    filtering_frequency( ); /* ���g���̈�ł̃t�B���^�����O */
    FFT2( -1 );             /* �Q���� �tFFT �̎��s    */
    make_output_image( );   /* �ϊ��f�[�^���摜�ɒ��� */
    save_image_data( );     /* �o�͉摜��ۑ�����     */
    return 0;
}
