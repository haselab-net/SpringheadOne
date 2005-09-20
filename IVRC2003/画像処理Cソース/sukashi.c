/* 電子透かしのプログラム sukashi.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"FFT1.h"
#include"FFT2.h"
#include"mypgm.h"

#define RATIO  0.1;  /* 混入させる画像に掛ける係数（＝ε） */

double  data[FFT_MAX][FFT_MAX], data2[FFT_MAX][FFT_MAX];
                /* 元データの実数部 */
double jdata[FFT_MAX][FFT_MAX],jdata2[FFT_MAX][FFT_MAX];
                /* 元データの虚数部 */
int num_of_data;          /* データの要素番号の最大値 */

void make_data( )
/* image1[y][x] のデータを data[y][x], jdata[y][x] に代入する */
{
    int i, j; /* ループ変数 */

    num_of_data = x_size1;
    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
             data[i][j] = (double)image1[i][j];
            jdata[i][j] = 0.0;
        }
    }
}

void make_output_image( )
/* データ data[y][x], jdata[y][x] を image2[y][x] に直す */
{
    int x, y; /* ループ変数 */

    x_size2 = num_of_data;    y_size2 = num_of_data;
    for ( y = 0; y < y_size2; y ++ ){
        for ( x = 0; x < x_size2; x ++ ){
            if ( data[y][x] < 0.0 ) image2[y][x] = 0;
            else if ( data[y][x] > 255.0 ) image2[y][x] = 255;
            else image2[y][x] = (unsigned char)data[y][x];
        }
    }
}

void copy_image_data( )
/* image1[y][x] ==> image2[y][x] へデータコピー */
{
    int i, j; /* ループ変数 */

    for ( i = 0; i < y_size1; i ++ ){
        for ( j = 0; j < x_size1; j ++ ){
            image2[i][j] = image1[i][j];
        }
    }
    y_size2 = y_size1;  x_size2 = x_size1;
}

void copy_FFT_data( )
/* data, jdata ==> data2, jdata2 のデータコピー */
{
    int i, j; /* ループ変数 */

    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
             data2[j][i] =  data[j][i];
            jdata2[j][i] = jdata[j][i];
        }
    }
}

void coding( )
/* 基準画像と混入する画像を指定して透かし入りの画像を保存する */
{
    int i, j; /* ループ変数 */

    printf("\n★　混入する画像を読み込みます．\n");
    load_image_data( );
    copy_image_data( );
    printf("\n★　基準画像を読み込みます．\n");
    load_image_data( );
    make_data( );
    FFT2( data, jdata, num_of_data, 1 );
    for ( i = 0; i < y_size1; i ++ ){
        for ( j = 0; j < x_size1; j ++ ){
            data[i][j] = data[i][j] + image2[i][j] / RATIO;
        }
    }
    FFT2( data, jdata, num_of_data, -1 ); /* ２次元 逆FFT の実行 */
    make_output_image( );            /* 周波数データを画像に直す */
    printf("\n★　透かし入りの画像を保存します．\n");
    save_image_data( );                        /* 画像を保存する */
}

void decoding( )
/* 透かし入りの画像と基準画像を指定して抽出された画像を保存する */
{
    int i, j; /* ループ変数 */
    double d; /* 作業変数 */

    printf("\n★　基準画像を読み込みます．\n");
    load_image_data( );
    make_data( );
    FFT2( data, jdata, num_of_data, 1 );
    copy_FFT_data( );
    printf("\n★　透かし入り画像を読み込みます．\n");
    load_image_data( );
    make_data( );
    FFT2( data, jdata, num_of_data, 1 );
    x_size2 = x_size1;  y_size2 = y_size1;
    for ( i = 0; i < y_size2; i ++ ){
        for ( j = 0; j < x_size2; j ++ ){
            d = ( data[i][j] - data2[i][j] ) * RATIO;
            if ( d < 0.0 ) image2[i][j] = 0;
            else if ( d > MAX_BRIGHTNESS ) 
                image2[i][j] = MAX_BRIGHTNESS;
            else image2[i][j] = (unsigned char)d;
        }
    } 
    printf("\n★　抽出された画像を保存します．\n");
    save_image_data( );     /* 画像を保存する */
}

main( )
{
    int mode;

    printf("*** 電子透かしのプログラム ***\n");
    do {
        printf("透かしを入れる(1) ／ 透かしを取る(-1) : ");
        scanf("%d",&mode); 
    } while ( mode != 1 && mode != -1 );
    if ( mode == 1 ) coding( );
    else decoding( );
    return 0;
}
