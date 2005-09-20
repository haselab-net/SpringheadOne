/* 線形平滑化のプログラム smooth.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

void spacial_filtering( )
/* 画像の空間フィルタリングを行う．　　                 */
/* 原画像 image1[y][x] ===> 変換後の画像 image2[y][x]   */
{
    /* 適用するフィルタの定義（3x3の8近傍線形平滑化) */
    int weight[3][3] = {
        { 1,  1,  1 },
        { 1,  2,  1 },
        { 1,  1,  1 }  };
    double div_const = 10.0; /* <== 最後に割る値 */
    double new_value;        /* 処理後の階調値 */
    int x, y, i, j;          /* 制御変数 */

    /* image2[y][x] の初期化 */
    x_size2 = x_size1;  y_size2 = y_size1;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = image1[y][x];
    /* フィルタリングを行う */
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
    load_image_data( );   /* 画像を読み込んで image1 へ */
    spacial_filtering( ); /* 空間フィルタリングして image2 へ */
    save_image_data( );   /* image2 を保存する */
    return 0;
}

