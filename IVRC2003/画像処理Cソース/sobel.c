/* Sobel 水平方向微分フィルタ sobel.c */
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include"mypgm.h"

void spacial_filtering( )
/* 画像の空間フィルタリングを行う．                     */
/* Sobelフィルタ（水平方向微分）                        */
/* 原画像 image1[y][x] ===> 変換後の画像 image2[y][x]   */
{
    /* 適用するフィルタの定義（Sobel水平方向微分) */
    int weight[3][3] = {
        { -1,  0,  1 },
        { -2,  0,  2 },
        { -1,  0,  1 }  };
    double div_const = 1.0; /* <== 最後に割る値 */
    double pixel_value; /* 計算値 */
    double min, max; /* 計算値の最小値，最大値 */
    int x, y, i, j;  /* ループ変数 */

    /* フィルタリング後の階調値の最小値，最大値を求める */
    printf("原画像のフィルタリングをしています．\n");
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
    /* image2[y][x] の初期化(外郭の画素のため）*/
    x_size2 = x_size1;  y_size2 = y_size1;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = 0;
    /* 処理後の値を線形変換してから image2 に代入 */
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
    load_image_data( );   /* 画像を読み込んで image1 へ */ 
    spacial_filtering( ); /* 空間フィルタリングして image2 へ */
    save_image_data( );   /* image2 を保存する */
    return 0;
}
