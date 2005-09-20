/* ステレオマッチングのプログラム stereo.c */
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include"mypgm.h"

void copy_image1_to_image2( )
/* image1 を image2 へコピーする */
{
    int x, y; /* ループ変数 */

    x_size2 = x_size1;    y_size2 = y_size1;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            image2[y][x] = image1[y][x];
}

void determine_corresponding_point( int xL, int yL, int *xR, int *yR )
/* 左画像の(xL,yL)の右画像上の対応点(*xR,*yR)を決定する */
{
    /* テンプレートに関する変数 */
    static unsigned int tmp_image[100][100]; /* 最大100x100画素 */
    int x_tmp    =   5;         /* x 方向サイズ [-x_tmp, x_tmp] */
    int y_tmp    =   3;         /* y 方向サイズ [-y_tmp, y_tmp] */
    int x_offset = 128;        /* x 方向探索範囲 [-x_offset, 0] */
      /* 左画像を基準にすると視差は負なので正方向には動かさない */
    int out_flag;  /* テンプレートの一部が外に出るとき1, else 0 */
    int threshold_for_gray = 20;    /* テンプレート内の階調差が */
      /* この値以下の場合は信頼性が低いので対応点検出しない     */
    int y_offset =  1;  /* y 方向探索範囲 [-y_offset, y_offset] */
      /* 左右カメラの上下のぶれを考慮しないときは y_offset = 0  */
    int i, j, m, n, xp, yp, xs, ys;     /* 制御変数，作業用変数 */
    int num_of_temp = 0;              /* テンプレート中の画素数 */
    double sum, min_error;            /* 誤差の和，誤差の最小値 */
    double error_ratio;               /* 誤差率（０以上１以下） */
    double min_gray, max_gray;/* テンプレート内の最小，最大階調 */
      /*  (xL,yL) を中心とするテンプレート tmp_image の切り出し */

    num_of_temp = 0;                  /* テンプレート中の画素数 */
    min_gray = 255.0;     /* テンプレート内の最小階調値の初期化 */
    max_gray = 0.0;       /* テンプレート内の最大階調値の初期化 */
    out_flag = 0;     /* テンプレートは画像内にあるとして初期化 */
    for ( i = - y_tmp; i <= y_tmp; i ++ ){
        for ( j = - x_tmp; j <= x_tmp; j ++ ){
            xp = xL + j;     yp = yL + i;
            if ( xp<0 || xp>x_size1 || yp<0 || yp>y_size1 )
                out_flag = 1;     /* 画像外 */
            else {
                num_of_temp ++ ;
                tmp_image[i+y_tmp][j+x_tmp] = image1[yp][xp];
                if ( image1[yp][xp] < min_gray )
                        min_gray = image1[yp][xp];
                else if ( image1[yp][xp] > max_gray )
                        max_gray = image1[yp][xp];
            }
        }
    }
    if ( ( max_gray - min_gray ) <= threshold_for_gray ||
    out_flag == 1 ){
        *xR = xL - 1;    *yR = yL;
        /* テンプレート内の階調変化があまりない場合，または */
        /* テンプレートの一部が画像の外に出てしまうときは */
        /* 視差を１（遠方）に設定する */
    } else {
        /* 探索点の決定 */
        min_error = 10000.0; /* 最小誤差の初期化 */
        for ( m = - y_offset; m <= y_offset; m ++ ){
            for ( n = - x_offset; n < 0; n ++ ){
                xs = xL + n;     ys = yL + m;
                /* (xs,ys)を対応点としたときの誤差の計算 */
                if ( xs>=0 && xs<x_size1 && ys>=0 && ys<y_size1 ){
                    sum = 0.0;  /* 誤差の総和の初期化 */
                    /* マッチングを行う */
                    for ( i = - y_tmp; i <= y_tmp; i ++ ){
                        for ( j = - x_tmp; j <= x_tmp; j ++ ){
                            xp = xs + j;    yp = ys + i;
                            if ( xp>=0 && xp<x_size1 && yp>=0 &&
                            yp<y_size1 ){
                                sum = sum + 
                                    abs( tmp_image[i+y_tmp][j+x_tmp] 
                                    - image2[yp][xp] );
                           /* 誤差＝各画素の階調値の差分の絶対値 */
                            }
                        }
                    }
                    error_ratio = sum / num_of_temp;   /* 誤差率 */ 
                    if ( error_ratio <= min_error ){
                      /* 誤差が等しければ n が 0 に近いほど優先 */
                        min_error = error_ratio;    
                        *xR = xs;   *yR = ys;
                    }
                }
            }
        }
    }
}

void calc_range_data( int xL, int yL, int xR, int yR, double d, double f, 
                     double p_to_mm, double *xp, double *yp, double *zp )
/* 左画像の(xL,yL)と右画像の(xR,yR)が対応するときの計測点 */
/* (*xp, *yp, *zp)の座標を計算する．d：左右レンズ間の距離 */
/* f：レンズ中心と画像面との間の距離（レンズの焦点距離）  */
{
    double disparity; /* 視差 */

    xL = xL - (int)(x_size1 / 2.0);      /* x 座標変換 */
    yL = - ( yL - (int)(y_size1 / 2.0)); /* y 座標変換 */ 
    xR = xR - (int)(x_size1 / 2.0);      /* x 座標変換 */
    yR = - ( yR - (int)(y_size1 / 2.0)); /* y 座標変換 */ 
    disparity = ( xL - xR ) * p_to_mm;   /*  視差 [mm] */
    *xp = d * xL / disparity;           /* x 座標 [mm] */
    *yp = d * yL / disparity;           /* y 座標 [mm] */
    *zp = f * d  / disparity;           /* z 座標 [mm] */
}

void stereo_matching( )
/* image1：左画像，image2：右画像 の間のステレオマッチングを */
/* 行う．結果を range[y][x] に代入後，画像に直してimage2 に  */
/* 代入する．                                                */
{
    double d = 100.0;             /* 左右カメラ間の距離 [mm] */
    double f =  10.0;             /* レンズの焦点距離 [mm]   */
    double pixel_to_mm = 0.02;   /* 画素を[mm]に変換するとき */
         /* に掛ける係数．これはカメラに依存するので，結果が */
         /* おかしいときに少し変えてみてほしい．             */
    int min_range = 100;          /* 想定最小距離 [mm]       */
    int max_range = 600;          /* 想定最大距離 [mm]       */
    static int range[MAX_IMAGESIZE][MAX_IMAGESIZE];  /* 距離 */
    int xL, yL, xR, yR;      /* 左画像，右画像の対応する座標 */
    double xp, yp, zp;       /* 計測点の(x,y,z)絶対座標      */
    int min_dist, max_dist;  /* 計測した距離の最小，最大値   */

    /* 距離データ用配列の初期化 */
    for ( yL = 0; yL < y_size1; yL ++ )
        for ( xL = 0; xL < x_size1; xL ++ )
            range[yL][xL] = 0;
    /* 処理開始 */
    printf("左画像を基準にしてステレオマッチングを行います．\n");
    for ( yL = 0; yL < y_size1; yL ++ ){
        printf("line %d\n",yL);
        for ( xL = 0; xL < x_size1; xL ++ ){
            determine_corresponding_point( xL, yL, &xR, &yR );
            calc_range_data( xL, yL, xR, yR, d, f, pixel_to_mm, 
                             &xp, &yp, &zp );
            range[yL][xL] = (int)zp;
        }
    }

    /* 以降は計測結果を画像にするための後処理である． */

    /* 計測した距離を範囲 [min_range, max_range] 内に入れる */
    for ( yL = 0; yL < y_size1; yL ++ ){
        for ( xL = 0; xL < x_size1; xL ++ ){
            if ( range[yL][xL] <= min_range )
                range[yL][xL] = min_range;
            if ( range[yL][xL] > max_range )
                range[yL][xL] = max_range;
        }
    }
    /* 距離の最小，最大値を求める */
    min_dist = INT_MAX;    max_dist = INT_MIN;
    for ( yL = 0; yL < y_size1; yL ++ ){
        for ( xL = 0; xL < x_size1; xL ++ ){
            if ( range[yL][xL] < min_dist )
                min_dist = range[yL][xL]; 
            else if ( range[yL][xL] > max_dist )
                max_dist = range[yL][xL];
        }
    }
    /* 画像に直す */
    for ( yL = 0; yL < y_size1; yL ++ ){
        for ( xL = 0; xL < x_size1; xL ++ ){
            image2[yL][xL] = (unsigned char)(MAX_BRIGHTNESS - 
                MAX_BRIGHTNESS * ( range[yL][xL] - min_dist ) 
                / (double)( max_dist - min_dist ) );
            /* 近いものほど白い画素にする */
        }
    }
}

main( )
{
    printf("===== ステレオマッチング =====\n");
    printf("●右画像を読み込みます．\n");
    load_image_data( );           /* 右画像 --> image1  */
    copy_image1_to_image2( );     /* image1 --> image2  */
    printf("●左画像を読み込みます．\n");
    load_image_data( );           /* 左画像 --> image1  */
    stereo_matching( );     /* ステレオマッチングを行い */
                           /* 結果を image2 に記録     */
    save_image_data( );           /* image2 --> 保存    */
    return 0;
}
