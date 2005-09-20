/* 8近傍膨張処理のプログラム dilate.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

void dilatation( )
/* ２値画像の８近傍膨張処理                             */
/* 原画像 image1[y][x] ===> 変換後の画像 image2[y][x]   */
{
    int repetition;     /* 膨張処理の回数 */
    int counter;        /* 近傍中の図形画素の数 */
    int p_x, p_y;       /* 近傍の(x,y)座標 */
    int i, x, y, m, n;  /* ループ変数 */

    x_size2 = x_size1;    y_size2 = y_size1;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = image1[y][x];
    printf("２値画像の８近傍膨張処理を行います．\n");
    printf("膨張処理を行う回数 = ");
    scanf("%d", &repetition);
    for ( i = 0; i < repetition; i ++ ){
        for ( y = 0; y < y_size1; y ++ ){
            for ( x = 0; x < x_size1; x ++ ){
                if ( image1[y][x] == 0 ){
                    counter = 0;
                    for ( m = -1; m < 2; m ++ ){
                        for ( n = -1; n < 2; n ++ ){
                            p_x = x + n;
                            p_y = y + m;
                            if ( p_x > 0 && p_x < x_size1 &&
                                 p_y > 0 && p_y < y_size1 &&
                                 image1[p_y][p_x] == MAX_BRIGHTNESS )
                                counter ++;
                        }
                    }
                    if ( counter > 0 ) 
                        image2[y][x] = MAX_BRIGHTNESS;
                }
            }
        }
        for ( y = 0; y < y_size2; y ++ )
            for ( x = 0; x < x_size2; x ++ )
                image1[y][x] = image2[y][x];
    }
}

main( )
{
    load_image_data( ); /* 画像を読み込んで image1 へ */
    dilatation( );     /* 8近傍膨脹処理して image2 へ */
    save_image_data( ); /* image2 を保存する */
    return 0;
}

