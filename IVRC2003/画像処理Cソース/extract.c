/* マスク画像で移動物体を抽出するプログラム extract.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

void copy_image1_to_image2( )
/* image1 の画像データを image2 へコピーする */
{
    int x, y;  /* ループ変数 */

    x_size2 = x_size1;  y_size2 = y_size1;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            image2[y][x] = image1[y][x];
}

void extract_image( )
{
    int x, y;  /* ループ変数 */

    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            if ( image1[y][x] == 0 ) image2[y][x] = 0;
}

main( )
{
    printf("*** 原画像のデータを読み込みます．\n");
    load_image_data( );        /* 画像1 --> image1      */
    copy_image1_to_image2( );  /* 画像1 --> image2      */
    printf("*** マスク画像のデータを読み込みます．\n");
    load_image_data( );        /* 画像2 --> image1      */
    extract_image( );          /* 領域抽出して image2 へ */
    printf("*** 抽出画像のデータを保存します．\n");
    save_image_data( );        /* image2 --> 出力      */
    return 0;
}
