/* 画像を簡易3次元表示するプログラム draw3D.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"

void draw_a_straight_line( int x1, int y1, int x2, int y2, int brightness )
/* image2[y][x] 上に(x1,y1)，(x2,y2)間の直線を描画する． */
/* 線の明るさは brightness (0～255）で指定する．         */
{
    double distance, step, t;   /* t:内分点発生用制御変数 */
    int x, y;  /* 描く点の座標 */
    
    distance = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ); /* 2点間の距離 */
    step = 1.0 / ( 1.5 * distance );  /* 発生させる内分点の総数 */
    t = 0.0;
    while ( t < 1.0 ){
        x = (int)( t * x2 + ( 1.0 - t ) * x1 );
        y = (int)( t * y2 + ( 1.0 - t ) * y1 );
        if ( 0 <= x && x < x_size2 && 0 <= y && y < y_size2 )
            image2[y][x] = (unsigned char)brightness; 
        t = t + step;
    }
}

void draw_3D_graph( )
{
    int y, x;           /* ループ変数 */
    int plot_step;      /* 線の描画間隔 [画素] 2～10 程度    */
    float plot_ratio;  /* 階調方向の倍率 0.0～1.0(=255画素) */
    int plot_height;  /* 階調方向の高さ = 256.0 * plot_ratio */

    x_size2 = x_size1 + y_size1 / 2;   /* グラフの想定横幅（画素）*/
    y_size2 = y_size1 + 256;           /* グラフの想定縦幅（画素）*/
    if ( x_size2 > MAX_IMAGESIZE || y_size2 > MAX_IMAGESIZE ){
        printf("画像が想定最大サイズを超えています．\n");
        exit(1);
    } else {
        /* 描画パラメータ( plot_step, plot_ratio )の指定 */
        printf("線の間隔（画素，2～10程度）：");
        scanf("%d",&plot_step);
        printf("階調方向の倍率(0以上1以下）：");
        scanf("%f",&plot_ratio);
        plot_height = (int)( 256.0 * plot_ratio );
        y_size2 = y_size1 + plot_height;
        /* 描画エリア( image2[y][x] )の初期化 */
        for ( y = 0; y < y_size2; y ++ )
            for ( x = 0; x < x_size2; x ++ )
                image2[y][x] = 0;
        /* グラフの表示 */
        printf("画像生成中です．しばらくお待ち下さい．\n");
        for ( y = 0; y < y_size1; y ++ ){
            if ( y % plot_step == 0 ){
                /* y が plot_step で割り切れるときだけ描く */
                /* 始めに曲線部分を黒で描いて隠面消去する */
                for ( x = 0; x < x_size1; x ++ ){
                    draw_a_straight_line( x + y / 2, plot_height + y,
                        x + y / 2, 
                   plot_height + y - (int)(image1[y][x] * plot_ratio), 0 );
                }
                /* 曲線部分を白で描く */
                for ( x = 0; x < x_size1 - 1; x ++ ){
                    draw_a_straight_line( x + y / 2, 
                    plot_height + y - (int)(image1[y][x] * plot_ratio),
                    x + y / 2 + 1, 
                    plot_height + y - (int)(image1[y][x+1] * plot_ratio), 
                    MAX_BRIGHTNESS );
                }
            }
        }
    }
}

main( )
{
    load_image_data( );    /* 原画像の読み込み */
    draw_3D_graph( );      /* ３次元グラフ画像の生成 */
    save_image_data( );    /* 画像の出力 */
    return 0;
}
