/* Hough 変換のプログラム Hough.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"

#define PI  3.141592653589  /* 円周率 */
#define MAX_THETA   360  /* θ軸のサイズ（0.5[deg]=１画素）*/
#define MAX_RHO     720  /* ρ軸のサイズ                 */

void draw_a_straight_line( int x1, int y1, int x2, int y2 )
/* image2[y][x] 上の２点(x1,y1)，(x2,y2)間の直線上の配列要素 */
/* の値をインクリメントする． */
{
    double distance, step, t;   /* t : 内分点発生用制御変数 */
    int x, y;  /* 描く点の座標 */

    distance = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ); /* 2点間の距離 */
    step = 1.0 / ( 1.5 * distance );  /* 発生させる内分点の総数 */
    t = 0.0;
    while ( t < 1.0 ){
        x = (int)( t * x2 + ( 1.0 - t ) * x1 );
        y = (int)( t * y2 + ( 1.0 - t ) * y1 );
        if ( x >= 0 && x < x_size2 && y >= 0 && y < y_size2
            && image2[y][x] < MAX_BRIGHTNESS ){
            image2[y][x] ++;
        }
        t = t + step;
    }
}

int _calc_rho( double px, double py, double r_mg, int theta )
/* ρを計算するルーチン（draw_a_curveで使われている） */
{
    return (int)(r_mg * ( px * cos( theta * PI / MAX_THETA ) 
               + py * sin( theta * PI / MAX_THETA ) ) + MAX_RHO / 2.0);
}

void draw_a_curve( double px, double py )
/* ρ＝ｘ・cosθ＋y・sinθの曲線を，配列image2[rho][theta]に描く．   */
{
    double rho_max;  /* ρ軸方向の最大値[画素] */
    double rho_c;    /* ρ方向の係数 */
    int theta;       /* θの単位は[deg]  */

    rho_max = 1.0 / 1.5 * x_size1 + 1.0 / 1.5 * y_size1;
    rho_c = MAX_RHO / 2.0 / rho_max;
        /* データから座標への変換係数 */
    for ( theta = 0; theta < MAX_THETA; theta ++ ){
        draw_a_straight_line( theta, _calc_rho( px, py, rho_c, theta),
            theta + 1, _calc_rho( px, py, rho_c, theta + 1) );
    }
}

void Hough_transform( )
/* 原画像 image1[y][x] を Hough 変換し，結果をθρ平面 */
/* image2[rho][theta] に代入する．ただしθρ平面の横軸・縦軸の */
/* 大きさは MAX_THETA, MAX_RHO であらかじめ決められている．*/
/* また，MAX_BRIGHTNESS(=255)以上の累積は考慮しない． */
{
    int x, y;

    /* θρ平面の初期化 */
    x_size2 = MAX_THETA;    y_size2 = MAX_RHO;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = 0;
    /* 変換処理の実行 */
    printf("Hough 変換を実行中です．\n");
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            if ( image1[y][x] == MAX_BRIGHTNESS ){
                draw_a_curve( (double)x, (double)y );
            }
        }
    }
}

main( )
{
    load_image_data( ); /* 画像を読み込んで image1 へ */
    Hough_transform( ); /* Hough 変換して結果を image2 へ */
    save_image_data( ); /* image2 を保存する */
    return 0;
}
