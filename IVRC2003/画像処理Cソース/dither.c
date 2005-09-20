/* �g�D�I�f�B�U�@�̃v���O���� dither.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"
#define BLOCK_SIZE 4  /* �u���b�N�̉�(=�c)��f�� */
#define NEW_LEVEL 16  /* �[���K����(= BLOCK_SIZE �̂Q��) */

void make_dither_image( )
/* ���摜 image1[y][x] �̃f�B�U�摜����� image2[y][x] �ɑ��  */
{
    double width;         /* 16�i�K�摜�̊K���l�̒P�ʕ� */
    int new_gray;         /* �V�����K���i16�K���ł̒l�j */
    int x_block, y_block;  /* ���E�c�̃u���b�N�� */
    int x, y, i, j, m, n;  /* ����ϐ� */
    int dither_matrix [4][4] = {    /* Bayer �^�f�B�U�s�� */
        { 0,  8,  2, 10},
        {12,  4, 14,  6},
        { 3, 11,  1,  9},
        {15,  7, 13,  5}
    };

    /* ���C�c�̉�f����BLOCK_SIZE�̔{���ł��邩�̃`�F�b�N */
    if ( x_size1 % BLOCK_SIZE != 0 || y_size1 % BLOCK_SIZE != 0 ){
        printf("���摜�̉��E�c�̉�f�����s�K�؂ł��D\n");
        exit(1);
    }
    /* 16�K���̉摜����� */
    width = MAX_BRIGHTNESS / (double)NEW_LEVEL;
    x_size2 = x_size1;   y_size2 = y_size1;
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            new_gray = (int)( image1[y][x] / width );
            if ( new_gray > NEW_LEVEL - 1 ) 
                new_gray = NEW_LEVEL - 1;
            image2[y][x] = (unsigned char)new_gray;
        }
    }
    /* �f�B�U�摜����� */
    printf("�f�B�U�摜�����܂��D\n");
    x_block = x_size1 / BLOCK_SIZE;  /* ���̃u���b�N�� */
    y_block = y_size1 / BLOCK_SIZE;  /* �c�̃u���b�N�� */
    for ( i = 0; i < y_block; i ++ ){
        for ( j = 0; j < x_block; j ++ ){
            x = BLOCK_SIZE * j;    y = BLOCK_SIZE * i;
            for ( m = 0; m < BLOCK_SIZE; m ++ ){
                for ( n = 0; n < BLOCK_SIZE; n ++ ){
                    if ( image2[y + m][x + n] <= dither_matrix[m][n] )
                         image2[y + m][x + n] = 0;
                    else image2[y + m][x + n] = MAX_BRIGHTNESS;
                }
            }
        }
    }
}

main( )
{
    load_image_data( );   /* �摜�f�[�^�� image1 �ɓǂݍ��� */
    make_dither_image( ); /* image1 ���f�B�U�\������ image2 �� */
    save_image_data( );   /* image2 ��ۑ����� */
    return 0;
}
