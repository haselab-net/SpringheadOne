/* 速度ベクトルを求めるプログラム motion.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
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

void draw_vector( int x1, int y1, int vx, int vy )
/* image2[y][x] に(x1,y1)の速度ベクトル(vx,vy)を描画する． */
{
    double distance, step, t;   /* t:内分点発生用制御変数 */
    int x2, y2;  /* 終点 */
    int x, y;    /* 描く点の座標 */
    
    x2 = x1 + vx;    y2 = y1 + vy;
    distance = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
    step = 1.0 / ( 1.5 * distance );
    t = 0.0;
    while ( t < 1.0 ){
        x = (int)( t * x2 + ( 1.0 - t ) * x1 );
        y = (int)( t * y2 + ( 1.0 - t ) * y1 );
        if ( x >= 0 && x < x_size2 && y >= 0 && y < y_size2 )
            image2[y][x] = (unsigned char)MAX_BRIGHTNESS; 
        t = t + step;
    }
}

void calc_motion_vector( int x, int y, int *vx, int *vy )
/* image1の点(x,y)の速度ベクトル(*vx, *vy)を決定する．*/
{
    /* テンプレートに関する変数 */
    static unsigned int tmp_image[100][100]; /* 最大100x100画素 */
    int x_tmp =  4;    /* x 方向サイズ [-x_tmp, x_tmp] */
    int y_tmp =  4;    /* y 方向サイズ [-y_tmp, y_tmp] */
    int x_offset = 30; /* x 方向探索範囲 [-x_offset, x_offset] */
    int y_offset = 30; /* y 方向探索範囲 [-y_offset, y_offset] */
    int threshold_for_gray = 30;    /* テンプレート内の階調差が */
      /* この値以下の場合は信頼性が低いので対応点検出しない     */

    int i, j, m, n, xp, yp, xs, ys;     /* 制御変数，作業用変数 */
    int num_of_temp;  /* テンプレート中の画素数（画像外を無視） */
    double sum, min_error;            /* 誤差の和，誤差の最小値 */
    double error_ratio;               /* 誤差率（０以上１以下） */
    double min_gray, max_gray;  /* テンプレート内最小，最大階調 */

    /* テンプレートの一部が画像外に出る点は速度０にする */
    if ( x - x_tmp < 0 || x + x_tmp >= x_size1 ||
        y - y_tmp < 0 || y - y_tmp >= y_size1 ){
        *vx = 0;    *vy = 0;
    } else {
        /*  (x,y) を中心とするテンプレート tmp_image の切り出し */
        num_of_temp = ( 2 * x_tmp + 1 ) * ( 2 * y_tmp + 1);
        min_gray = 255;    max_gray = 0;   /* 最小，最大階調 */
        for ( i = - y_tmp; i <= y_tmp; i ++ ){
            for ( j = - x_tmp; j <= x_tmp; j ++ ){
                xp = x + j;     yp = y + i;
                tmp_image[i+y_tmp][j+x_tmp] = image1[yp][xp];
                if ( image1[yp][xp] < min_gray )
                    min_gray = image1[yp][xp];
                else if ( image1[yp][xp] > max_gray )
                    max_gray = image1[yp][xp];
            }
        }
        if ( ( max_gray - min_gray ) <= threshold_for_gray ){
        *vx = 0;    *vy = 0;
      /* テンプレート内の階調変化があまりない場合は速度=0 にする */
        }
        else
        {
            /* 探索点の決定 */
            min_error = 10000.0; /* 最小誤差の初期化 */
            for ( m = - y_offset; m <= y_offset; m ++ ){
                for ( n = - x_offset; n < x_offset; n ++ ){
                    xs = x + n;     ys = y + m;
                    /* (xs,ys)を対応点としたときの誤差の計算 */
                    if ( xs - x_tmp >= 0 && xs + x_tmp < x_size1 &&
                         ys - y_tmp >= 0 && ys + y_tmp < y_size1 ){
                        sum = 0.0;  /* 誤差の総和の初期化 */
                        /* マッチングを行う */
                        for ( i = - y_tmp; i <= y_tmp; i ++ ){
                            for ( j = - x_tmp; j <= x_tmp; j ++ ){
                                xp = xs + j;    yp = ys + i;
                                sum = sum + 
                                    abs( tmp_image[i+y_tmp][j+x_tmp] 
                                    - image2[yp][xp] );
                           /* 誤差＝各画素の階調値の差分の絶対値 */
                            }
                        }
                        error_ratio = sum / num_of_temp; /* 誤差率 */ 
                        if ( error_ratio <= min_error ){
                            min_error = error_ratio;    
                            *vx = xs - x;   *vy = ys - y;
                        }
                    }
                }
            }
        }
    }
}

void block_matching( )
/* image1：画像1，image2：画像2 の間のブロックマッチングを行う． */
/* 結果の速度場の画像を vector[y][x] に代入後，image2 にコピー   */
/* してから保存する．                                            */
{
    /* x 方向・y方向速度ベクトル */
    int x_vctr[MAX_IMAGESIZE][MAX_IMAGESIZE];
    int y_vctr[MAX_IMAGESIZE][MAX_IMAGESIZE];
    int x, y; /* 制御変数 */
    int vx, vy, step; /* 速度ベクトル，ベクトルを求める点の間隔 */

    do {
        printf("速度ベクトルを計算する画素間の距離(2～20) : ");
        scanf("%d", &step);
    } while ( step < 2 || step > 20 );
    /* 速度場の画像の初期化 */
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ ){
            x_vctr[y][x] = 0;    y_vctr[y][x] = 0;
        }
    /* 処理開始 */
    printf("画像1 から画像2 への速度ベクトルを求めています．\n");
    for ( y = 0; y < y_size1; y ++ ){
        printf("line %d\n",y);
        if ( ( y % step ) == 0 ){
            for ( x = 0; x < x_size1; x ++ ){
                if ( ( x % step ) == 0 ){
                    calc_motion_vector( x, y, &vx, &vy );
                    x_vctr[y][x] = vx;    y_vctr[y][x] = vy;
                }
            }
        }
    }
    /* 計測結果を画像にする */
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = 0;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            if ( ( y % step ) == 0 && ( x % step ) == 0 )
                draw_vector( x, y, x_vctr[y][x], y_vctr[y][x] );
}

main( )
{
    printf("===== 動きベクトルの検出 =====\n");
    printf("frame No.2 を読み込みます．\n");
    load_image_data( );         /* frame No.2 --> image1 */
    copy_image1_to_image2( );   /* image1 --> image2     */
    printf("frame No.1 を読み込みます．\n");
    load_image_data( );         /* frame No.1 --> image1 */
    block_matching( );          /* 動きベクトルを求め，  */
                                /* 結果を image2 に記録  */
    save_image_data( );         /* image2 --> 保存       */
    return 0;
}
