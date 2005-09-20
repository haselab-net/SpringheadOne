/* Sobel �������������t�B���^ sobel.c */
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include"mypgm.h"

void spacial_filtering( )
/* �摜�̋�ԃt�B���^�����O���s���D                     */
/* Sobel�t�B���^�i�������������j                        */
/* ���摜 image1[y][x] ===> �ϊ���̉摜 image2[y][x]   */
{
    /* �K�p����t�B���^�̒�`�iSobel������������) */
    int weight[3][3] = {
        { -1,  0,  1 },
        { -2,  0,  2 },
        { -1,  0,  1 }  };
    double div_const = 1.0; /* <== �Ō�Ɋ���l */
    double pixel_value; /* �v�Z�l */
    double min, max; /* �v�Z�l�̍ŏ��l�C�ő�l */
    int x, y, i, j;  /* ���[�v�ϐ� */

    /* �t�B���^�����O��̊K���l�̍ŏ��l�C�ő�l�����߂� */
    printf("���摜�̃t�B���^�����O�����Ă��܂��D\n");
    min = (double)INT_MAX;
    max = (double)INT_MIN;
    for ( y = 1; y < y_size1 - 1; y ++ ){
        for ( x = 1; x < x_size1 - 1; x ++ ){
            pixel_value = 0.0;
            for ( i = - 1; i < 2; i ++  )
                for ( j = -1; j < 2; j ++ )
                    pixel_value = pixel_value + 
                        weight[i + 1][j + 1] * image1[y + i][x + j];
            pixel_value = pixel_value / div_const;
            if ( pixel_value < min ) min = pixel_value;
            if ( pixel_value > max ) max = pixel_value;
        }
    }
    if ( (int)(max - min) == 0 ) exit(1);
    /* image2[y][x] �̏�����(�O�s�̉�f�̂��߁j*/
    x_size2 = x_size1;  y_size2 = y_size1;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = 0;
    /* ������̒l����`�ϊ����Ă��� image2 �ɑ�� */
    for ( y = 1; y < y_size1 - 1; y ++ ){
        for ( x = 1; x < x_size1 - 1; x ++ ){
            pixel_value = 0.0;
            for ( i = - 1; i < 2; i ++  )
                for ( j = -1; j < 2; j ++ )
                    pixel_value = pixel_value + 
                        weight[i + 1][j + 1] * image1[y + i][x + j];
            pixel_value = pixel_value / div_const;
            pixel_value = MAX_BRIGHTNESS /
                         ( max - min ) * ( pixel_value - min );
            image2[y][x] = (unsigned char)pixel_value;
        }
    }
}

main( )
{
    load_image_data( );   /* �摜��ǂݍ���� image1 �� */ 
    spacial_filtering( ); /* ��ԃt�B���^�����O���� image2 �� */
    save_image_data( );   /* image2 ��ۑ����� */
    return 0;
}
