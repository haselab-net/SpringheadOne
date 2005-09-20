/* 1から指定した数までの和を画面に表示するプログラム sample.c */

/* 必要なヘッダファイルのインクルード */
#include<stdio.h>   /* 基本入出力 */
#include<stdlib.h>  /* 基本ライブラリ */

/* 定数の宣言など（マジックナンバーなど） */
#define  MIN_NUM  1         /* 定数の宣言 */

/* 大域変数の宣言（どこからでも参照できる） */
int   dummy1, dummy2;   /* 整数型の変数の宣言 */
float dummy3;           /* 実数型の変数の宣言 */
char  dummy4;           /* 文字列型変数の宣言 */

int calculate_sum( int num )
/* num までの和を求める関数 */
{
    int  i;       /* for ループの制御変数 */
    int  sum;    /* 和を代入する整数型変数 */ 

    if ( 100 < num || num <= MIN_NUM ){
        printf("入力された数字が不適切です．終了します．\n");
        exit(1);
    } else printf("和を計算します．\n");
    sum = 0;
    for ( i = MIN_NUM; i <= num; i++ )
        sum = sum + i;
    return sum;
}

main( )
{
    int  max_num, summation;

    printf("1 から指定した整数までの和を求める\n");
    printf("整数（2以上100以下） = ");
    scanf("%d", &max_num );
    summation = calculate_sum( max_num );
    printf("和は %d です．\n", summation );
    return 0;
}
