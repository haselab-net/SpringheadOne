/* ���`�������̃v���O���� smooth.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

void spacial_filtering( )
/* �摜�̋�ԃt�B���^�����O���s���D�@�@                 */
/* ���摜 image1[y][x] ===> �ϊ���̉摜 image2[y][x]   */
{
    /* �K�p����t�B���^�̒�`�i3x3��8�ߖT���`������) */
    int weight[3][3] = {
        { 1,  1,  1 },
        { 1,  2,  1 },
        { 1,  1,  1 }  };
    double div_const = 10.0; /* <== �Ō�Ɋ���l */
    double new_value;        /* ������̊K���l */
    int x, y, i, j;          /* ����ϐ� */

    /* image2[y][x] �̏����� */
    x_size2 = x_size1;  y_size2 = y_size1;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = image1[y][x];
    /* �t�B���^�����O���s�� */
    for ( y = 1; y < y_size1 - 1; y ++ )
        for ( x = 1; x < x_size1 - 1; x ++ ){
            new_value = 0.0;
            for ( i = - 1; i < 2; i ++  )
                for ( j = -1; j < 2; j ++ )
                    new_value = new_value + 
                        weight[i + 1][j + 1] * image1[y + i][x + j];
            image2[y][x] = (unsigned char)(new_value / div_const);
        }
}

main( )
{
    load_image_data( );   /* �摜��ǂݍ���� image1 �� */
    spacial_filtering( ); /* ��ԃt�B���^�����O���� image2 �� */
    save_image_data( );   /* image2 ��ۑ����� */
    return 0;
}

