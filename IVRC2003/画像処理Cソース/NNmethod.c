/* NN 法による点のクラスタリングのプログラム NNmethod.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"

#define PI  3.141592653589  /* 円周率 */
#define MAX_PNTS   3000     /* 想定する最大構成点数    */
#define MAX_CLSTR  3000     /* 想定する最大クラスタ数  */
#define DARK_PIXEL  128     /* 暗い画素の階調値 */

int points[MAX_PNTS][2];        /* 点の座標                */
int num_pnts;                   /* 点の総数                */
int point_clstr[MAX_PNTS];      /* 点が属するクラスタ番号  */

int center_clstr[MAX_CLSTR][2]; /* クラスタの代表点の座標  */
int num_clstr;                  /* クラスタの総数          */

void draw_a_circle( int x, int y, int r )
/* image2[y][x] に中心 (x,y) 半径 r の円を描く．全周で最大   */
/* 360 個の点を描くが，大きい円の場合は点線になることもある．*/
{
    int theta;     /* 角度 [deg] の制御変数     */
    int xp, yp;    /* 描画する点 (xp,yp) の座標 */

    for ( theta = 0; theta < 360; theta ++ ){
        xp = (int)( x + r * cos( theta * PI / 180.0 ) );
        yp = (int)( y + r * sin( theta * PI / 180.0 ) );
        if ( xp >= 0 && xp < x_size2 && yp >= 0 && yp < y_size2 )
            image2[ yp ][ xp ] = DARK_PIXEL;  /* 暗い画素 */
    }
}

double calc_distance( int x1, int y1, int x2, int y2 )
/* (x1,y1),(x2,y2) 間の距離を返す関数 */
{
    return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

void draw_a_straight_line( int x1, int y1, int x2, int y2 )
/* image2[y][x] 上に(x1,y1)，(x2,y2)間の直線を発生させる． */
{
    double distance, step, t;   /* t: 内分点発生用制御変数 */
    int x, y;  /* 描く点の座標 */

    distance = calc_distance( x1, y1, x2, y2 ); /* 2点間の距離 */
    step = 1.0 / ( 1.5 * distance );  /* 発生させる内分点の総数 */
    t = 0.0;
    while ( t < 1.0 ){
        x = (int)( t * x2 + ( 1.0 - t ) * x1 );
        y = (int)( t * y2 + ( 1.0 - t ) * y1 );
        if ( x >= 0 && x < x_size2 && y >= 0 && y < y_size2 )
            image2[y][x] = DARK_PIXEL;  /* 暗い点 */
        t = t + step;
    }
}

int random_int( int n )
/* 0 以上 n - 1 以下の整数の乱数を返す */
{
    return (int)( (double)rand( ) / ( (double)RAND_MAX + 1.0 ) * n );
}

void obtain_points_data( )
/* image1[y][x] 中の白画素のデータを points[ ][ ]に代入 */
{
    int x, y;   /* ループ変数 */

    num_pnts = 0;
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            if ( image1[y][x] == MAX_BRIGHTNESS ){
                if ( num_pnts < MAX_PNTS - 2 ){
                    num_pnts ++;
                    points[num_pnts - 1][0] = x;
                    points[num_pnts - 1][1] = y;
                }
            }
        }
    }
    printf("全部で %d 点のデータを取得しました．\n", num_pnts);
    if ( num_pnts < 5 ){
        printf("点列が少な過ぎます．画像を作り直して下さい．\n");
        exit( 1 );
    }
}

void shuffle_points_data( )
/* 取得した点データをランダムに入れ替える． */
{
    int seed;              /* 乱数のシード                 */
    int i, j, num1, num2;  /* 制御変数，入れ替える番号1，2 */
    int temp[2];           /* 入れ替えに用いる作業変数     */
    int max;    /* 入れ替え回数 */

    /* 乱数の初期化 */
    printf("乱数の系列を初期化します．\n");
    printf("乱数のシード（任意の正の整数）:");
    scanf("%d",&seed);
    if ( seed < 1 ) seed = 1;
    srand( seed );

    /* 点の総数の２倍の回数の入れ替え処理 */
    max = num_pnts * 2;
    for ( i = 0; i < max; i ++ ){
        num1 = random_int( num_pnts );
        do { 
            num2 = random_int( num_pnts );
        } while ( num2 == num1 );
        /* No.num1 と No.num2 の点を入れ替えます */
        for ( j = 0; j < 2; j ++ ){
            temp[j]         = points[num1][j];
            points[num1][j] = points[num2][j];
            points[num2][j] = temp[j];
        }
    }
}

void clustering_using_NNmethod( )
/* NN(Nearest Neighbour 法によるクラスタリングを行う */
{
    int threshold;  /* 同一のクラスタ内に含める距離の最大値 */
    int y, x, i, j;                 /* 制御変数など         */
    double min_distance, distance;  /* 距離に関する変数     */
    int min_num;                  /* 最も近いクラスタの番号 */
    char str[10];                 /* 作業用文字列           */

    do {         /* threshold を変えながら何度か試す */
        printf("NN法によって点列をクラスタリングします．\n");
        printf("しきい値（クラスタの半径）: ");
        scanf("%d",&threshold);
        printf("データを分類してクラスタを形成し，結果を\n");
        printf("画像として作成しています．\n");
        /* image2[y][x] の初期化 */
        x_size2 = x_size1;  y_size2 = y_size1;
        for ( y = 0; y < y_size2; y ++ )
            for ( x = 0; x < x_size2; x ++ )
                image2[y][x] = image1[y][x];
        /* No.0 の点を No.0 のクラスタ代表点に設定する */
        num_clstr = 0;           /* クラスタの総数 - 1 */
        for ( i = 0; i < 2; i ++ )
            center_clstr[num_clstr][i] = points[0][i];
            draw_a_circle( points[0][0], points[0][1], threshold );
            draw_a_circle( points[0][0], points[0][1], 5 );
            point_clstr[0] = 0;
            num_clstr ++;
        /* No.1 から No.num_pnts-1 までの点を分類する */
        for ( i = 1; i < num_pnts; i ++ ){
            /* 最も近いクラスタ:No.min_numと距離:min_distance */
            min_distance = 10000.0;
            for ( j = 0; j < num_clstr; j ++ ){
                distance = calc_distance( points[i][0], points[i][1],
                        center_clstr[j][0], center_clstr[j][1] );
                if ( distance < min_distance ){
                    min_distance = distance;
                    min_num = j;
                }
            }
            if ( min_distance < threshold ){ 
                /* No.i の点をクラスタ No.min_num へ属させる */
                point_clstr[i] = min_num;
            } 
            else
            {   /* No.i の点は新しいクラスタに所属させる */
            for ( j = 0; j < 2; j ++ )
                center_clstr[num_clstr][j] = points[i][j];
                draw_a_circle( points[i][0], points[i][1], threshold );
                draw_a_circle( points[i][0], points[i][1], 5 );
                point_clstr[i] = num_clstr;
                num_clstr ++;
            }
        }
        /* クラスタ中心と各点データまでの線分を描く   */
        for ( i = 0; i < num_pnts; i ++ ){
            draw_a_straight_line( points[i][0], points[i][1],
                center_clstr[ point_clstr[i] ][0],
                center_clstr[ point_clstr[i] ][1] );
        }
        /* image2[y][x] に元のデータ点を再描画する */
        for ( i = 0; i < num_pnts; i ++ )
            image2[ points[i][1] ][ points[i][0] ] = MAX_BRIGHTNESS;

        printf("%d 個のクラスタが生成されました．\n", num_clstr);
        printf("もう１回やり直しますか ( y / n ) : ");
        scanf("%s",str);
    } while ( str[0] != 'n' && str[0] != 'N' );
}

main( )
{
    load_image_data( );             /* 画像ファイル --> image1 */
    obtain_points_data( );          /* image1中の点をデータ化  */
    shuffle_points_data( );         /* データ順序のランダム化  */
    clustering_using_NNmethod( );   /* NN法でクラスタリングし，*/
                                    /* 結果をimage2へ描画する  */
    save_image_data( );             /* image2 --> ファイル出力 */
    return 0;
}
