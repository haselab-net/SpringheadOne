/* 組織的ディザ法のプログラム dither.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"
#define BLOCK_SIZE 4  /* ブロックの横(=縦)画素数 */
#define NEW_LEVEL 16  /* 擬似階調数(= BLOCK_SIZE の２乗) */

void make_dither_image( )
/* 原画像 image1[y][x] のディザ画像を作り image2[y][x] に代入  */
{
    double width;         /* 16段階画像の階調値の単位幅 */
    int new_gray;         /* 新しい階調（16階調での値） */
    int x_block, y_block;  /* 横・縦のブロック数 */
    int x, y, i, j, m, n;  /* 制御変数 */
    int dither_matrix [4][4] = {    /* Bayer 型ディザ行列 */
        { 0,  8,  2, 10},
        {12,  4, 14,  6},
        { 3, 11,  1,  9},
        {15,  7, 13,  5}
    };

    /* 横，縦の画素数がBLOCK_SIZEの倍数であるかのチェック */
    if ( x_size1 % BLOCK_SIZE != 0 || y_size1 % BLOCK_SIZE != 0 ){
        printf("原画像の横・縦の画素数が不適切です．\n");
        exit(1);
    }
    /* 16階調の画像を作る */
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
    /* ディザ画像を作る */
    printf("ディザ画像を作ります．\n");
    x_block = x_size1 / BLOCK_SIZE;  /* 横のブロック数 */
    y_block = y_size1 / BLOCK_SIZE;  /* 縦のブロック数 */
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
    load_image_data( );   /* 画像データを image1 に読み込む */
    make_dither_image( ); /* image1 をディザ表示して image2 へ */
    save_image_data( );   /* image2 を保存する */
    return 0;
}
