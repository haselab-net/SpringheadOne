/* Hough 逆変換を行って直線を抽出するプログラム invHough.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"

#define PI  3.141592653589 /* 円周率 */
#define MAX_THETA   360    /* θ軸のサイズ（１[deg]=１画素）*/
#define MAX_RHO     720    /* ρ軸のサイズ                  */

void swap_int( int *n1, int *n2 )
/* int n1 と int n2 を入れ替える */
{
    int n;  /* 作業変数 */

    n = *n1;    *n1 = *n2;    *n2 = n;
}

void extract_line( int xs, int ys )
/* θρ平面上（image1上）の点(xs,ys)を画像上の直線に逆変換し，   */
/* 結果を image2[y][x] に描く                                    */
{
    double theta, rho, rho_max, _sin, _cos;  /* 作業変数 */
    int x, y;  /* 制御変数 */

    theta = 180.0 * xs / MAX_THETA;  /* [deg] */
    _sin = sin( theta / 180.0 * PI );
    _cos = cos( theta / 180.0 * PI );
    rho_max = 1.0 / 1.5 * x_size2 + 1.0 / 1.5 * y_size2;
    rho = ( ys - MAX_RHO / 2.0 ) * rho_max / MAX_RHO * 2.0; 
    /*  rho = x _cos + y _sin  --->  y = rho/_sin - x _cos/_sin  */
    if ( 45.0 <= theta && theta <= 135.0 ){ /* 数値の単位は[deg] */
        for ( x = 0; x < x_size2; x ++ ){
            y = (int)( ( rho - x * _cos ) / _sin );
            if ( 0 <= y && y < y_size2 )
                image2[y][x] = MAX_BRIGHTNESS; 
        }
    } else {
        for ( y = 0; y < y_size2; y ++ ){
            x = (int)( ( rho - y * _sin ) / _cos );
            if ( 0 <= x && x < x_size2 )
                image2[y][x] = MAX_BRIGHTNESS;
        }
    }
}

void inverse_Hough_transform( )
/* image1 を θρ平面とみなし，しきい値より上の点を画像空間に */
/* 逆変換して画像中に直線を描く．結果は image2 に代入する．   */
{
    int threshold,counter;  /* しきい値，直線のカウンタ */
    int x,y;  /* 制御変数 */
    char str[10];  /* 作業変数 */

    printf("読み込んだθρ平面から，画像上の直線を再現します．\n");
    do {
        printf("θρ平面上の累積に対するしきい値 (0～255の値）= ");
        scanf("%d",&threshold);
        counter = 0;
        for ( y = 0; y < MAX_RHO;  y ++ )
            for ( x = 0; x < MAX_THETA; x ++ )
                if ( image1[y][x] >= threshold ) counter ++;
        printf("全部で %d 本の直線が検出されました．\n", counter);
        printf("この値で OK ? ( y / n ) :");
        scanf("%s",str);
    } while ( str[0] != 'y' && str[0] != 'Y' );
    /* 逆変換処理の実行 */
    for ( y = 0; y < MAX_RHO; y ++ )
        for ( x = 0; x < MAX_THETA; x ++ )
            if ( image1[y][x] >= threshold )
                extract_line( x, y );
}

void move_image1_to_image2( )
/* 原画像データをimage2[y][x]に退避する */
/* その際，階調を落とす (=80)          */
{
    int x, y;  /* 制御変数 */

    x_size2 = x_size1;    y_size2 = y_size1;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ ){
            image2[y][x] = image1[y][x];
            if ( image2[y][x] == MAX_BRIGHTNESS )
                image2[y][x] = 80;
        }
}

void check_data( )
{
    if ( x_size1 != MAX_THETA || y_size1 != MAX_RHO ){
        printf("これは対象とするθρ平面のデータと異なります．\n");
        exit(1);
    }
}

main( )
{
    printf("まず，元の画像データを読み込みます．\n");
    load_image_data( );  /* 原画像を image1 へ */
    move_image1_to_image2( );  /* image1 を image2 へコピー */
    printf("引き続きθρ平面のデータを読み込みます．\n");
    load_image_data( );  /* θρ平面のデータを image1 へ */
    check_data( );  /* データのチェック */
    inverse_Hough_transform( );  /* Hough逆変換して直線抽出 */
    save_image_data( );  /* image2 を保存 */
    return 0;
}
