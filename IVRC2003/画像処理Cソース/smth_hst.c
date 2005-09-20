/* ヒストグラム平滑化のプログラム smth_hst.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"
#define FINAL_LEVEL 64  /* 最終的な表現階調数 */

void smooth_histgram( )
/* image1 のヒストグラムを平滑化して image2 へ */
{
    long int hist1[GRAYLEVEL], hist2[GRAYLEVEL];
    int trans_table[GRAYLEVEL]; /* 濃度変換表 */
    int i, x, y; /* ループ変数 */
    long int target_value; /* 変換後の頻度の目標値 */          
    int gray; /* 階調用作業変数 */
    double gray_step; /* 表現階調間隔 */

    printf("ヒストグラムの平滑化を行います．\n");
    /* 原画像のヒストグラムを求める */
    for ( i = 0; i < GRAYLEVEL; i ++ ) hist1[i] = 0;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            hist1[ image1[x][y] ] ++;
    /* ヒストグラム変換表を作る */
    target_value = (int)( x_size1 * y_size1 / (double)FINAL_LEVEL );
    for ( i = 0; i < FINAL_LEVEL; i ++ ) hist2[i] = 0;
    gray = 0;
    for ( i = 0; i < GRAYLEVEL; i ++ ){
        if ( abs( target_value - hist2[gray] ) <
             abs( target_value - ( hist2[gray] + hist1[i] ) ) ){
            gray ++;
            if ( gray >= FINAL_LEVEL ) gray = FINAL_LEVEL - 1;
        }
        trans_table[i] = gray;
        hist2[gray] = hist2[gray] + hist1[i];
    }
    /* ヒストグラムを平滑化した画像を image2 に代入 */
    x_size2 = x_size1;    y_size2 = y_size1;
    gray_step = (double)GRAYLEVEL / FINAL_LEVEL;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = (unsigned char)
                ( trans_table[ image1[y][x] ] * gray_step );
}

main( )
{
    load_image_data( ); /* 原画像を image1 に読み込む */
    smooth_histgram( ); /* ヒストグラムを平滑化して image2 へ */
    save_image_data( ); /* image2 を保存する */
    return 0;
}
