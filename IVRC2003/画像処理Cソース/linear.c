/* �q�X�g�O�����̐��`�ϊ����s���v���O���� linear.c */
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include"mypgm.h"

void linear_transformation( )
/* ��f�̊K�����z�ɑ΂��Đ��`�ϊ����s���D             */
/* ���摜 image1[y][x] ===> �ϊ���̉摜 image2[y][x] */
{
    int min, max; /* �K���l�̍ŏ��l�C�ő�l */
    int x, y;     /* ����ϐ� */

    /* �K���l�̍ŏ��l�C�ő�l�����߂� */
    min = INT_MAX;  max = INT_MIN;
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            if ( image1[y][x] < min ) min = image1[y][x];
            if ( image1[y][x] > max ) max = image1[y][x];
        }
    }
    printf("�K���l�̍ŏ��l = %d�C�ő�l = %d\n", min, max);
    /* �K������`�ϊ����� image2[y][x] �ɑ�� */
    x_size2 = x_size1;    y_size2 = y_size1;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            image2[y][x] = (unsigned char)( ( image1[y][x] - min )
                           * MAX_BRIGHTNESS / (double)(max - min) );
}

main( )
{
    load_image_data( );       /* ���摜�� image1 �ɓǂݍ��� */
    linear_transformation( ); /* �q�X�g�O�����̐��`�ϊ����s�� */
    save_image_data( );       /* �����摜 image2 ��ۑ����� */
    return 0;
}
