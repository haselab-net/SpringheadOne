/* 誤差拡散法のプログラム err_dfs3.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"
#include"myutil.h"

void make_error_difusion_image( )
/* 原画像 image1[y][x] の誤差拡散画像を作り image2[y][x] に代入  */
{
    int x, y;  /* ループ変数 */
    double err1, err2, err3;  /* 階調誤差 */
    double buf[2][MAX_IMAGESIZE]; /* 誤差保存用バッファ */
    double modified_gray;

    /* 誤差拡散画像を作る */
    printf("誤差拡散画像を作ります．\n");
    x_size2 = x_size1;    y_size2 = y_size1;
        /* ２ライン分のバッファのクリア */
    for ( y = 0; y < 2; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            buf[y][x] = 0.0;
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            /* 近傍が外に出るときは単純２値化する */
            if ( x < 2 || x > x_size1 - 3 || y == y_size1 - 1 ){
                if ( image1[y][x] <= MAX_BRIGHTNESS / 2) 
                    image2[y][x] = 0;
                else image2[y][x] = MAX_BRIGHTNESS;
            } else {
                modified_gray = image1[y][x] + buf[0][x];
                err1 = modified_gray - 0.0;
                err2 = modified_gray - MAX_BRIGHTNESS;
                if ( err1 * err1 < err2 * err2 ){
                    image2[y][x] = 0;    
                    err3 = err1;
                } else {
                    image2[y][x] = MAX_BRIGHTNESS;  
                    err3 = err2;
                }
                /* 近傍に応じて次の式を変える */
                buf[0][x+1] = buf[0][x+1] + err3 * 2.0 / 10.0;
                buf[0][x+2] = buf[0][x+2] + err3 * 1.0 / 10.0;
                buf[1][x+2] = buf[1][x+2] + err3 * 1.0 / 10.0;
                buf[1][x+1] = buf[1][x+1] + err3 * 2.0 / 10.0;
                buf[1][x]   = buf[1][x]   + err3 * 2.0 / 10.0;
                buf[1][x-1] = buf[1][x-1] + err3 * 1.0 / 10.0;
                buf[1][x-2] = buf[1][x-2] + err3 * 1.0 / 10.0;
            }
        }
        for ( x = 0; x < x_size1; x ++ ){
            buf[0][x] = buf[1][x];
            buf[1][x] = 0.0;
        }
    }
}

main( )
{
    load_image_data( );         /* 画像データを image1 に読み込む */
    make_error_difusion_image( ); /* 誤差拡散画像を作成する*/
    save_image_data( );           /* image2 を保存する */
    return 0;
}
