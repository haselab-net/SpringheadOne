/* 画像の濃度ヒストグラムの画像を作るプログラム histgram.c */
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include"mypgm.h"
#define IMAGESIZE 256   /* = GRAYLEVEL */

void make_histogram_image( )
/* 原画像 image1 のヒストグラムを作り画像としてimage2 に代入 */
{
    long int histogram[GRAYLEVEL]; /* ヒストグラム用配列 */
    long int max_frequency; /* 頻度の最大値 */
    int i, j, x, y; /* 制御変数 */
    int data; /* 頻度を表すグラフの長さ */

    /* 原画像のヒストグラムを求める */
    for ( i = 0; i < GRAYLEVEL; i ++ ) 
        histogram[ i ] = 0;
    printf("今，原画像のヒストグラムを求めています．\n");
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            histogram[ image1[x][y] ] ++;
    /* 頻度の最大値を求める */
    max_frequency = INT_MIN;
    for ( i = 0; i < GRAYLEVEL; i ++ )
        if ( histogram[i] > max_frequency ) 
            max_frequency = histogram[i];
    printf("頻度の最大値 = %d\n", max_frequency);
    /* ヒストグラム画像を image2 に代入 */
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
    load_image_data( );     /* 原画像を image1 に読み込む */
    make_histogram_image( ); /* ヒストグラムの画像を image2 へ */
    save_image_data( );     /* image2 を保存する */
    return 0;
}
