/* 画像間の差分画像を求めるプログラム make_dif.c */
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

void make_difference_image( )
{
    int x, y;        /* ループ変数   */
    int difference;  /* 差分の絶対値 */

    printf("frame1 と frame2 の画素ごとの階調値の差分の\n");
    printf("絶対値を求めて image2[y][x] に代入します．\n");
    for ( y = 0; y < y_size2; y ++ ){
        for ( x = 0; x < x_size2; x ++ ){
            difference = abs( image2[y][x] - image1[y][x] );
            image2[y][x] = (unsigned char)difference;
        }
    }
}

void make_mask_image( )
{
    int x, y;      /* ループ変数 */
    int max_dif;   /* 許容できる最大の差分値 */

    printf("差分画像からマスク画像を作ります．\n");
    do {
        printf("差分の許容最大階調値 (1～100) : ");
        scanf("%d", &max_dif);
    } while ( max_dif < 1 || max_dif > 100 );
    /* マスク画像の生成 */
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            if ( image2[y][x] > max_dif ) 
                image2[y][x] = MAX_BRIGHTNESS;
            else image2[y][x] = 0;
}

main( )
{
    printf("*** frame No.1 のデータを読み込みます．\n");
    load_image_data( );        /* frame1 データ --> image1  */
    copy_image1_to_image2( );  /* frame1 データ --> image2  */
    printf("*** frame No.2 のデータを読み込みます．\n");
    load_image_data( );        /* frame2 データ --> image1  */
    make_difference_image( );  /* 差分を求めて image2 へ    */
    printf("*** 差分画像のデータを保存します．\n");
    save_image_data( );        /* 差分画像 --> 出力         */
    make_mask_image( );        /* 差分マスク画像 --> image2 */
    printf("*** 差分画像から作ったマスク画像を保存します．\n");
    save_image_data( );        /* マスク画像 --> 出力       */
    return 0;
}
