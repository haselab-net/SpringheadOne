/* ヒストグラムの線形変換を行うプログラム linear.c */
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include"mypgm.h"

void linear_transformation( )
/* 画素の階調分布に対して線形変換を行う．             */
/* 原画像 image1[y][x] ===> 変換後の画像 image2[y][x] */
{
    int min, max; /* 階調値の最小値，最大値 */
    int x, y;     /* 制御変数 */

    /* 階調値の最小値，最大値を求める */
    min = INT_MAX;  max = INT_MIN;
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            if ( image1[y][x] < min ) min = image1[y][x];
            if ( image1[y][x] > max ) max = image1[y][x];
        }
    }
    printf("階調値の最小値 = %d，最大値 = %d\n", min, max);
    /* 階調を線形変換して image2[y][x] に代入 */
    x_size2 = x_size1;    y_size2 = y_size1;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            image2[y][x] = (unsigned char)( ( image1[y][x] - min )
                           * MAX_BRIGHTNESS / (double)(max - min) );
}

main( )
{
    load_image_data( );       /* 原画像を image1 に読み込む */
    linear_transformation( ); /* ヒストグラムの線形変換を行う */
    save_image_data( );       /* 処理画像 image2 を保存する */
    return 0;
}
