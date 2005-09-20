/* GA による顔領域抽出のプログラム ext_face.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"myGA.h" 
#include"mypgm.h"

/* 画像マッチング関係の定数・変数 */
#define MIN_RATE      1.0   /* 想定最小拡大倍率 */
#define MAX_RATE      3.0   /* 想定最大拡大倍率 */
#define ANGLE_RANGE  60.0   /* 想定回転角度 -30～30[deg] */
#define MIN_MATCHING_RATE 0.9 /* 想定最小マッチング率 */
#define MAX_GENERATION    300 /* 最大世代数           */
#define MIN_RANGE   50  /* これより階調差が少ない部分は無視 */
#define PI  3.141592653589  /* 円周率 */

void set_optimizing_task( )
/* 原画像中の顔画像の抽出を，与えられた課題とします */
{
    /* テンプレート画像 = image1[y][x]              */
    /* 顔を含む原画像   = image2[y][x]              */
    /* 位置 (x,y), 拡大倍率 rate, 回転角度 angle    */
    /* 各8ビットとする                              */
    gene_size = 32;
}

void trans_from_genotype_to_parameters( int number, int *x, int *y, 
                                       double *rate, double *angle )
/* 個体番号 No.number の遺伝子型をパラメータに変換する */
/* (x,y):テンプレートの中心，rate:拡大倍率，angle：回転角度 */
{
    int i; /* ループ変数 */
    static unsigned char code[MAX_GENE_SIZE];

    /* グレイコードからバイナリコードへの変換 */
    for ( i = 0; i < gene_size; i ++ )
        code[i] = genotype[number][i];
    for ( i = 0; i < 7; i ++ ){
        if ( code[i] == 1 ) 
            code[i+1] = (unsigned char)(1 - code[i+1]);
        if ( code[i+8] == 1 ) 
            code[i+9] = (unsigned char)(1 - code[i+9]);
        if ( code[i+16] == 1 ) 
            code[i+17] = (unsigned char)(1 - code[i+17]);
        if ( code[i+24] == 1 ) 
            code[i+25] = (unsigned char)(1 - code[i+25]);
    }
    /* 遺伝子型から整数 (x, y) への変換 */
    *x = 0;   *y = 0;   *rate = 0.0;   *angle = 0.0;
    for ( i = 0; i < 8; i ++ ){
        *x = *x * 2 + code[i];    *y = *y * 2 + code[i+8];
        *rate = *rate * 2.0 + code[i+16];    
        *angle = *angle * 2.0 + code[i+24];
    }
    /* コードから実際の値への変換 */
    *x = (int)( *x / 255.0 * x_size2 );
    *y = (int)( *y / 255.0 * y_size2 );
    *rate = ( MAX_RATE - MIN_RATE ) * *rate / 255.0 + MIN_RATE;
    *angle = ANGLE_RANGE * *angle / 255.0 - ANGLE_RANGE / 2.0;
    if ( *angle < 0 ) *angle = 360.0 + *angle;
}

double fitness_value( int number, int flag )
/* set_optimizing_task( )で説明されている個体 No.number  */
/* の適応度を返す（ flag = 0 : 通常，= 1 : 最終画像作成）*/
{
    int x, y;            /* 座標用変数 */
    int xp,yp,ix2,iy2;   /* 作業用変数 */
    int x_half, y_half;  /* 画像サイズの半分   */
    double x1,y1,x2,y2;  /* 空間変換用変数     */
    double rate, angle;  /* 拡大倍率，回転角度 */
    double sum;          /* 適応度用作業変数   */
    double min_gray, max_gray; /* 領域の最小・最大階調値 */
    double _sin, _cos;   /* sin, cos 用変数 */

    /* 遺伝子型から (x,y) へ */
    trans_from_genotype_to_parameters( number, &xp, &yp, &rate, &angle );
    /* 適応度の計算 */
    x_half = x_size1 / 2;    y_half = y_size1 / 2;
    sum = 0.0;    min_gray = MAX_BRIGHTNESS;    max_gray = 0;
    _sin = sin( angle * PI / 180.0 );  
    _cos = cos( angle * PI / 180.0 );
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            /* テンプレートの各点(x1,y1)に対する空間変換 */
            x1 = x - x_half;    y1 = y - y_half;
            x1 = x1 * rate;     y1 = y1 * rate;
            x2 = _cos * x1 - _sin * y1;
            y2 = _sin * x1 + _cos * y1;
            x2 = x2 + xp;    y2 = y2 + yp;
            ix2 = (int)x2;  iy2 = (int)y2;  /* 画像上の対応点 */
            /* 対応点での階調値の差分を求める */
            if ( 0<=ix2 && ix2<x_size2 && 0<=iy2 && iy2<y_size2 ){
                sum = sum + abs( image2[iy2][ix2] - image1[y][x] );
                if ( image2[iy2][ix2] > max_gray ) 
                    max_gray = image2[iy2][ix2];
                if ( image2[iy2][ix2] < min_gray )
                    min_gray = image2[iy2][ix2];
                if ( flag == 1 )
                    image2[iy2][ix2] = (unsigned char)(
                        MAX_BRIGHTNESS - image2[iy2][ix2]);
            } else sum = sum + MAX_BRIGHTNESS;
        }
    }
    /* あまり平坦な領域は候補としないための処理 */
    if ( max_gray - min_gray < MIN_RANGE ) sum = 0.1;
    else sum = 1.0 - sum / ( x_size1 * y_size1 ) / MAX_BRIGHTNESS;
    return sum;
}

void calculate_fitness( )
/* 全員の適応度を求める */
{
    int i; /* ループ変数 */
    
    for ( i = 0; i < pop_size; i ++ )
        fitness[i] = fitness_value( i, 0 );
}

void display_elitest( int generation )
/* 最良個体の遺伝子型，適応度を画面に表示する */
{
    int i; /* ループ変数 */

    printf("No.%d：",generation);
    for ( i= 0; i < gene_size; i ++ ){
        printf("%d",elite_genotype[i]);
        if ( (i+1) % 8 == 0 ) printf(" ");
    }
    printf("--> %f\n", elite_fitness);
}

void generation_iteration()
/* 世代交代を行う関数 */
{
    int generation; /* 世代数 */
    int x, y;       /* 最良個体の座標 */
    double rate, angle, f; /* 作業変数 */

    generation = 1;
    calculate_fitness( );    /* 適応度の計算   */
    find_and_set_best_individual( ); /* エリートの検出 */
    do{
        generation ++;
        selection_using_roulette_rule( ); /* ルーレットルール */
        execute_crossover( ); /* 染色体の交叉 */
        execute_mutation( ); /* 遺伝子の突然変異 */
        calculate_fitness( ); /* 適応度の計算 */
        if ( elite_flag == 1 ) elitist_strategy( ); /* エリート保存 */
        find_and_set_best_individual(); /* エリートの検出 */
        display_elitest( generation ); /* 最良個体の表示 */
    } while ( elite_fitness < MIN_MATCHING_RATE && 
              generation < MAX_GENERATION );
    trans_from_genotype_to_parameters( elite_number, &x, &y, &rate, &angle );
    printf("最終的な解：(x, y) = (%d, %d), rate = %f, angle = %f\n",
        x,y,rate,angle);
    /* 抽出結果の画像を作る */
    f = fitness_value( elite_number, 1 ); /* 最良個体の領域を反転 */
}

void move_image1_to_image2( )
/* 画像 image1 を image2 へ移動する */
{
    int x, y; /* 制御変数 */

    for ( y=0; y<y_size1; y++ )
        for ( x=0; x<x_size1; x++ )
            image2[y][x] = image1[y][x];
    x_size2 = x_size1;    y_size2 = y_size1;
}

main( )
{
    initialize_random_number( );  /* 乱数の初期化       */
    set_optimizing_task( );       /* 問題の設定         */
    initialize_parameters( );     /* GAパラメータの設定 */
    initialize_genes( );          /* 遺伝子型の初期化   */
    printf("=== 顔画像を含む原画像を読み込みます ===\n");
    load_image_data( );           /* 原画像の読み込み   */
    move_image1_to_image2( );
    printf("=== 顔のテンプレート画像を読み込みます ===\n");
    load_image_data( );           /* テンプレート読み込み */
    generation_iteration( );      /* 世代交代 */
    printf("=== 顔の抽出結果の画像を保存します ===\n");
    save_image_data( );           /* 結果の保存 */
    return 0;
}
