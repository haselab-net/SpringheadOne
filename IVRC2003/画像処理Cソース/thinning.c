/* Hilditch の細線化のプログラム thinning.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"
#define GRAY 128

int func_nc8( int *b )
/* 注目画素の連結度（４連結）を求める関数 */
{
    int n_odd[4] = { 1, 3, 5, 7 }; /* 奇数の近傍番号 */
    int i, j, sum, d[10]; /* 制御変数，計算途中を代入する変数 */
    
    for ( i = 0; i <= 9; i ++ ){
        j = i;    if (i==9) j = 1;
        if ( abs( *( b + j ) ) == 1 ) d[i] = 1;
        else d[i] = 0;
    }
    sum = 0;
    for ( i = 0; i < 4; i ++ ){
        j = n_odd[i];
        sum = sum + d[j] - d[j] * d[j+1] * d[j+2];
    }
    return( sum );
}

void thinning( )
/* ２値画像の細線化処理(image2[y][x] を最初は作業領域として使う)*/
/* （  = MAX_BRIGHTNESS --> 1, GRAY --> -1, 0 --> 0 とみなす）  */
/* 原画像image1[y][x] ===> 処理後の画像image2[y][x]             */
{
    int offset[9][2] = {{0,0},{1,0},{1,-1},{0,-1},{-1,-1},
           {-1,0},{-1,1},{0,1},{1,1} }; /* 近傍画素へのオフセット値 */
    int n_odd[4] = { 1, 3, 5, 7 };      /* 近傍番号（奇数）         */
    int px, py;                         /* 近傍画素の(x,y)絶対座標  */
    int b[9];                           /* 自分を含む９近傍の値     */
    int condition[6];               /* 条件１～６：=1のとき“成立” */
    int counter;                    /* ラベルに変化が生じた画素の数 */
    int i, x, y, copy, sum;         /* 制御変数など                 */
    int path;                       /* 画像の走査回数を表す変数     */

    printf("２値画像の細線化処理を行います．\n");
    /* 作業配列の初期化 */
    x_size2 = x_size1;    y_size2 = y_size1;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = image1[y][x];
    /* 処理開始 */
    path = 1;
    do {
        printf("path : %d\n", path);
        counter = 0;
        for ( y = 0; y < y_size1; y ++ ){
            for ( x = 0; x < x_size1; x ++ ){
                /* (x,y)を含む９近傍にデータを代入する */
                for ( i = 0; i < 9; i ++ ){
                    b[i] = 0;
                    px = x + offset[i][0];    py = y + offset[i][1];
                    if ( px >= 0 && px < x_size1 && 
                         py >= 0 && py < y_size1 )
                         if ( image2[py][px] == MAX_BRIGHTNESS )
                             b[i] = 1; 
                         else if ( image2[py][px] == GRAY ) b[i] = -1;
                }
                for ( i = 0; i < 6; i ++ ) condition[i] = 0;
                
                /* 条件１：図形画素である */
                if ( b[0] == 1 ) condition[0] = 1;
                
                /* 条件２：境界点である */
                sum = 0;
                for ( i = 0; i < 4; i ++ )
                    sum = sum + 1 - abs( b[ n_odd[i] ] );
                if ( sum >= 1 ) condition[1] = 1;
                
                /* 条件３：端点を除去しない */
                sum = 0;
                for ( i = 1; i <= 8; i ++ )
                    sum = sum + abs( b[i] );
                if ( sum >= 2 ) condition[2] = 1;
                
                /* 条件４：孤立点を保存する */
                sum = 0;
                for ( i = 1; i <= 8; i ++ )
                    if ( b[i] == 1 ) sum++;
                if ( sum >= 1 ) condition[3] = 1;
                
                /* 条件５：連結性を保存する */
                if ( func_nc8( b ) == 1 ) condition[4] = 1;
                
                /* 条件６：線幅２の線分の片側だけを削除する */
                sum = 0;
                for ( i = 1; i <= 8; i ++ )
                    if ( b[i] != -1 ) sum ++;
                    else {
                        copy = b[i];    
                        b[i] = 0;
                        if ( func_nc8( b ) == 1 ) sum ++;
                        b[i] = copy;
                    }
                if ( sum == 8 ) condition[5] = 1;
                
                /* 最終判定 */
                if ( condition[0] && condition[1] && condition[2] &&
                     condition[3] && condition[4] && condition[5] ){
                    image2[y][x] = GRAY; /* = -1 を表している */
                    counter ++;
                }
            } /* end of x */
        } /* end of y */
        if ( counter != 0 ){
            for ( y=0; y<y_size1; y++)
                for ( x=0; x<x_size1; x++)
                    if ( image2[y][x] == GRAY ) image2[y][x] = 0;
        }
        path ++;
    } while ( counter != 0 );
}

main( )
{
    load_image_data( ); /* 画像を読み込んで image1 へ */
    thinning( );        /* Hilditch の細線化を行う */
    save_image_data( ); /* image2 を保存する */
    return 0;
}
