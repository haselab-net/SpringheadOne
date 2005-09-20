/* 2次元FFTによるフィルタリングのプログラム FFT2fltr.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"
#include"FFT1.h"
#include"FFT2.h"

void make_original_data( )
/* image1[y][x] のデータを data[y][x], jdata[y][x] に代入する */
{
    int i, j; /* 制御変数 */

    if ( x_size1 != y_size1 ){
        printf("縦と横の画素数が異なっているので終了します．\n");
        exit(-1);
    } else {
        printf("\n読み込んだ画像を元データに直します．\n");
        num_of_data = x_size1;
        for ( i = 0; i < num_of_data; i ++ ){
            for ( j = 0; j < num_of_data; j ++ ){
                 data[i][j] = (double)image1[i][j];
                jdata[i][j] = 0.0;
            }
        }
    }
}

void filtering_frequency( )
/* 周波数領域に対するフィルタリングを行う */
/* 例：低域通過フィルタ                   */
{
    int i, j, max;

    printf("\nFFT後の係数に対するフィルタリングを行います．\n");
    max = num_of_data / 8;
    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
            if ( ( max < i ) && ( i < ( num_of_data - max ) ) ||
                 ( max < j ) && ( j < ( num_of_data - max ) ) ){
                 data[i][j] = 0.0;
                jdata[i][j] = 0.0;
            }
        }
    }
}

void make_output_image( )
/* データ data[y][x], jdata[y][x] を image2[y][x] に直す */
{
    int x, y;

    printf("\n逆FFT変換後の画像を作成します．\n");
    x_size2 = num_of_data;    y_size2 = num_of_data;
    for ( y = 0; y < y_size2; y ++ ){
        for ( x = 0; x < x_size2; x ++ ){
            if ( data[y][x] < 0 ) data[y][x] = 0;
            if ( data[y][x] > MAX_BRIGHTNESS ) 
                     data[y][x] = MAX_BRIGHTNESS;
            image2[y][x] = (unsigned char)data[y][x];
        }
    }
}

main( )
{
    load_image_data( );     /* 原画像の読み込み       */
    make_original_data( );  /* 元のデータを作成する   */
    FFT2( 1 );              /* ２次元 FFT の実行      */
    filtering_frequency( ); /* 周波数領域でのフィルタリング */
    FFT2( -1 );             /* ２次元 逆FFT の実行    */
    make_output_image( );   /* 変換データを画像に直す */
    save_image_data( );     /* 出力画像を保存する     */
    return 0;
}
