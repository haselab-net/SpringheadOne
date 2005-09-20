/* 輪郭線追跡のプログラム contour.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

#define MAX_CNTR  5000      /* 輪郭線の想定最大構成画素数 */
#define GRAY 128

int chain_code[MAX_CNTR];   /* 輪郭線の配列 */
int Freeman[8][2] = {   /* Freeman のチェーンコードのオフセット値 */
        { 1, 0}, { 1, -1}, { 0, -1}, {-1, -1},
        {-1, 0}, {-1,  1}, { 0,  1}, { 1,  1} };

int obtain_contour( int x_start, int y_start )
/* 開始点(x_start, y_start)から輪郭線追跡を実行してデータを取得して */
/* 大域変数 unsigned char chain_code[MAX_CNTR] に代入し，輪郭 */
/* 線に含まれる点の総数（num）を返す．image1[y][x]を対象にする．*/
{
    int  x,  y;             /* 輪郭線上の現在の注目画素の座標 */
    int xs, ys;             /* 注目画素の周囲の探索点の座標   */
    int code, num;          /* 輪郭点のチェーンコード, 総数   */
    int i, counter, detect; /* 制御変数など                   */

    /* 孤立点のチェック */
    counter = 0;
    for ( i = 0; i<8; i++ ){
        xs = x_start + Freeman[i][0];
        ys = y_start + Freeman[i][1];
        if ( xs >= 0 && xs <= x_size1 && ys >= 0 && ys <= y_size1
            && image1[ys][xs] == MAX_BRIGHTNESS ) counter ++;
    }
    if ( counter == 0 ) num = 1;  /* (x_start,y_start)は孤立点 */
    else {
        /* 探索開始 */
        num = -1;   x = x_start;    y = y_start;    code = 0;
        do {
            detect = 0; /* 次の点をみつけたとき１にする */
            /* 初期探索方向の決定 */
            code = code - 3;   if ( code < 0 ) code = code + 8;
            do{
                xs = x + Freeman[code][0];
                ys = y + Freeman[code][1];
                if ( xs >= 0 && xs <= x_size1 && ys >= 0 &&
                     ys <= y_size1 && 
                     image1[ys][xs] == MAX_BRIGHTNESS ){
                    detect = 1;  /* 次の点を検出 */
                    num ++ ;  
                    if ( num > MAX_CNTR ){
                        printf("輪郭線の画素数 > %d\n",MAX_CNTR);
                        exit(1);
                    }
                    chain_code[ num ] = code;
                    x = xs;  y = ys;  
                }
                code ++;  if ( code > 7 ) code = 0;
            } while ( detect == 0 );
        } while ( x != x_start || y != y_start ); /* 開始点の検出まで */
        num = num + 2;  /* chain_code[ ]の添え字とのずれの修正 */
    }
    return( num );  /* 開始点総数を返す */
}

void remove_areas( )
/* ２値画像の輪郭線追跡の関数 obtain_contour( ) を用いて，*/
/* 周囲長が指定した値よりも小さな領域を取り除く．         */
{
    int threshold;              /* 周囲長に対するしきい値 */
    int num, x,y,xs, ys, i;
    int fill_value;

    printf("輪郭線追跡を用いた領域の除去を行います．\n");
    printf("除去対象領域の最大周囲長 = ");
    scanf("%d", &threshold);
    /* 処理後画像の初期化 */
    x_size2 = x_size1;    y_size2 = y_size1;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            image2[y][x] = 0;
    /* 処理開始 */
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            if ( image1[y][x] == MAX_BRIGHTNESS ){  /* = 開始点 */
                num = obtain_contour( x, y );  /* 輪郭線追跡 */
                if ( num > threshold ) 
                    fill_value = MAX_BRIGHTNESS;
                else fill_value = GRAY;
                xs = x;  ys = y;
                image1[ys][xs] = 0;
                image2[ys][xs] = (unsigned char)fill_value;
                if ( num > 1 ){
                    for ( i = 0; i < num - 1; i ++ ){
                        xs = xs + Freeman[ chain_code[i] ][0];
                        ys = ys + Freeman[ chain_code[i] ][1];
                        image1[ys][xs] = 0;
                        image2[ys][xs] = (unsigned char)fill_value;
                    }
                }
            }
        }
    }
    /* 後処理 */
    for ( y = 0; y < y_size2; y ++ ){
        for ( x = 0; x < x_size2; x ++ ){
            if ( image2[y][x] == GRAY ){
                image2[y][x] = 0;
                for ( i = 0; i < 8; i ++ ){
                    xs = x + Freeman[i][0];
                    ys = y + Freeman[i][1];
                    if ( xs >= 0 && xs <= x_size2 && 
                         ys >= 0 && ys <= y_size2 &&
                         image2[ys][xs] == MAX_BRIGHTNESS ) 
                        image2[y][x] = MAX_BRIGHTNESS;
                }
            }
        }
    }
}

main( )
{
    load_image_data( ); /* 画像を読み込んで image1 へ */
    remove_areas( ); /* 輪郭線追跡して小領域を除去し，image2 へ */
    save_image_data( ); /* image2 を保存する */
    return 0;
}
