/* �摜�̔Z�x�q�X�g�O�����̉摜�����v���O���� histgram.c */
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include"mypgm.h"
#define IMAGESIZE 256   /* = GRAYLEVEL */

void make_histogram_image( )
/* ���摜 image1 �̃q�X�g�O���������摜�Ƃ���image2 �ɑ�� */
{
    long int histogram[GRAYLEVEL]; /* �q�X�g�O�����p�z�� */
    long int max_frequency; /* �p�x�̍ő�l */
    int i, j, x, y; /* ����ϐ� */
    int data; /* �p�x��\���O���t�̒��� */

    /* ���摜�̃q�X�g�O���������߂� */
    for ( i = 0; i < GRAYLEVEL; i ++ ) 
        histogram[ i ] = 0;
    printf("���C���摜�̃q�X�g�O���������߂Ă��܂��D\n");
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            histogram[ image1[x][y] ] ++;
    /* �p�x�̍ő�l�����߂� */
    max_frequency = INT_MIN;
    for ( i = 0; i < GRAYLEVEL; i ++ )
        if ( histogram[i] > max_frequency ) 
            max_frequency = histogram[i];
    printf("�p�x�̍ő�l = %d\n", max_frequency);
    /* �q�X�g�O�����摜�� image2 �ɑ�� */
    x_size2 = IMAGESIZE;    y_size2 = IMAGESIZE;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = 0;
    for ( i = 0; i < GRAYLEVEL; i ++ ){
        data = (int)( MAX_BRIGHTNESS / 
            (double)max_frequency * (double)histogram[i] );
        for ( j = 0; j < data; j ++ )
            image2[IMAGESIZE-1-j][i] = MAX_BRIGHTNESS;
    }
}

main( )
{
    load_image_data( );     /* ���摜�� image1 �ɓǂݍ��� */
    make_histogram_image( ); /* �q�X�g�O�����̉摜�� image2 �� */
    save_image_data( );     /* image2 ��ۑ����� */
    return 0;
}
