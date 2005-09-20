/* �q�X�g�O�����������̃v���O���� smth_hst.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"
#define FINAL_LEVEL 64  /* �ŏI�I�ȕ\���K���� */

void smooth_histgram( )
/* image1 �̃q�X�g�O�����𕽊������� image2 �� */
{
    long int hist1[GRAYLEVEL], hist2[GRAYLEVEL];
    int trans_table[GRAYLEVEL]; /* �Z�x�ϊ��\ */
    int i, x, y; /* ���[�v�ϐ� */
    long int target_value; /* �ϊ���̕p�x�̖ڕW�l */          
    int gray; /* �K���p��ƕϐ� */
    double gray_step; /* �\���K���Ԋu */

    printf("�q�X�g�O�����̕��������s���܂��D\n");
    /* ���摜�̃q�X�g�O���������߂� */
    for ( i = 0; i < GRAYLEVEL; i ++ ) hist1[i] = 0;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            hist1[ image1[x][y] ] ++;
    /* �q�X�g�O�����ϊ��\����� */
    target_value = (int)( x_size1 * y_size1 / (double)FINAL_LEVEL );
    for ( i = 0; i < FINAL_LEVEL; i ++ ) hist2[i] = 0;
    gray = 0;
    for ( i = 0; i < GRAYLEVEL; i ++ ){
        if ( abs( target_value - hist2[gray] ) <
             abs( target_value - ( hist2[gray] + hist1[i] ) ) ){
            gray ++;
            if ( gray >= FINAL_LEVEL ) gray = FINAL_LEVEL - 1;
        }
        trans_table[i] = gray;
        hist2[gray] = hist2[gray] + hist1[i];
    }
    /* �q�X�g�O�����𕽊��������摜�� image2 �ɑ�� */
    x_size2 = x_size1;    y_size2 = y_size1;
    gray_step = (double)GRAYLEVEL / FINAL_LEVEL;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = (unsigned char)
                ( trans_table[ image1[y][x] ] * gray_step );
}

main( )
{
    load_image_data( ); /* ���摜�� image1 �ɓǂݍ��� */
    smooth_histgram( ); /* �q�X�g�O�����𕽊������� image2 �� */
    save_image_data( ); /* image2 ��ۑ����� */
    return 0;
}
