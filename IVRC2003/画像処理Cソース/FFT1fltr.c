/* 1次元FFTによるフィルタリングのプログラム FFT1fltr.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"
#include"FFT1.h"  /* 1次元FFT・逆FFTのヘッダ */

double  data[FFT_MAX]; /* 元データの実数部 */
double jdata[FFT_MAX]; /* 元データの虚数部 */
int num_of_data;       /* データの要素数   */

void display_data( )
/* データを表示する */
{
    int i;    /* 制御変数 */

    for ( i = 0; i < num_of_data; i ++ ){
        printf("No.%3d: 実数部 =%f, 虚数部 =%f\n",i,data[i],jdata[i]);
    }
    printf("何かキーを押して下さい : ");
    (void)getchar( );    printf("\n");
}

void make_test_data( )
/* テスト用のデータを作る（例：台形状の波形）  */
{
    int i, min, max; /* 作業変数 */ 
    
    /* 要素 No.min ～ No.max の実数部だけ 100.0 */
    num_of_data = 16;   min = 4;   max = 12;
    for ( i = 0; i < num_of_data; i ++ ){
        if ( min <= i && i < max ) data[i] = 100.0;
        else data[i] = 0.0;
        jdata[i] = 0.0;
    }
}

void filtering_frequency( )
/* 周波数領域でのフィルタリング */
{
    int i, max; /* 作業変数 */

    /* 例）low pass フィルタ（低域通過フィルタ） */
    max = num_of_data / 4;
    for ( i = 0; i < num_of_data; i ++ ){
        if ( max <= i && i <= ( num_of_data - max ) ){
             data[i] = 0.0;
            jdata[i] = 0.0;
        }
    }
}

main( )
{
    /* テストデータを作る */
    make_test_data( );
    printf("テストデータを作りました．\n");
    display_data( );

    /* 高速フーリエ変換 */
    FFT1( data, jdata, num_of_data, 1 );
    printf("FFT が終わりました．\n");
    display_data( );

    /* 周波数領域のフィルタリング */
    filtering_frequency( );
    printf("フィルタリング(低域通過)が終わりました．\n");
    display_data( );

    /* 逆高速フーリエ変換 */
    FFT1( data, jdata, num_of_data, -1 );
    printf("逆 FFT が終わりました．\n");
    display_data( );

    return 0;
}