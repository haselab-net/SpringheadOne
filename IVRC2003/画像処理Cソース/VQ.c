/* 画像のベクトル量子化のプログラム VQ.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"

/* 各種定数の宣言 */
#define MAX_BLOCK  128       /* 横・縦方向の最大ブロック数    */
#define MIN_BLOCK   10       /* 横・縦方向の最小ブロック数    */
#define MAX_CLUSTERS 8192    /* 代表ベクトルの最大本数        */
#define BLOCK_SIZE   4       /* 想定ブロックサイズ 4 x 4 画素 */
#define DIMENSION   16       /* ベクトルの次元（＝ 4 x 4 ）   */
#define MIN_DIFFERENCE  5.0
                  /* 代表ベクトルの移動チェックの際のしきい値 */

/* 大域変数の宣言 */
unsigned char vector[MAX_BLOCK][MAX_BLOCK][DIMENSION];
                    /* 各ブロックのベクトル */
unsigned char main_vector[MAX_CLUSTERS][DIMENSION];
                    /* 代表ベクトル */
int cluster[MAX_BLOCK][MAX_BLOCK];
                    /* 各ブロックの所属クラスタ番号 */
int x_block, y_block;  /* 横方向ブロック数，縦方向ブロック数 */

int random_int( int n )
/* 0 以上 n - 1 以下の整数の乱数を返す */
{
    return (int)( (double)rand( ) / ( (double)RAND_MAX + 1.0 ) * n );
}

void obtain_vector_data( )
/* image1[y][x] を 4 x 4 画素のブロックに分割してベクトル化 */
{
    int x, y, xs, ys, i, j;   /* 制御変数 */

    if ( ( x_size1 % BLOCK_SIZE != 0 ) || 
         ( y_size1 % BLOCK_SIZE != 0 ) ){
        printf("画素数が %d の倍数ではありません．\n",BLOCK_SIZE);
        exit( 1 );
    }
    x_block = x_size1 / BLOCK_SIZE;    
    y_block = y_size1 / BLOCK_SIZE;
    for ( y = 0; y < y_block; y ++ ){
        for ( x = 0; x < x_block; x ++ ){
            xs = BLOCK_SIZE * x;    
            ys = BLOCK_SIZE * y;
            for ( i = 0; i < BLOCK_SIZE; i ++ ){
                for ( j = 0; j < BLOCK_SIZE; j ++ ){
                    vector[y][x][i*BLOCK_SIZE+j] =
                        image1[ys + i][xs + j];
                }
            }
        }
    }
}

void initialize_random_number( )
/* 乱数の初期化 */
{
    unsigned int seed; /* 乱数のシード */

    /* 乱数の初期化 */
    printf("乱数の系列を初期化します．\n");
    printf("乱数のシード（任意の正の整数）:");
    scanf("%d",&seed);
    if ( seed < 1 ) seed = 1;
    srand( seed );
}

double calc_distance( int y, int x, int n )
/* [y][x]ブロックのベクトルとNo.n 代表ベクトル間の距離計算 */
{
    int i;      /* 制御変数 */
    double sum; /* 作業変数 */

    sum = 0;
    for ( i = 0; i < DIMENSION; i ++ )
        sum = sum + ( vector[y][x][i] - main_vector[n][i] ) *
                    ( vector[y][x][i] - main_vector[n][i] );
    return( sqrt( sum ) );
}

void clustering_using_Kmeans( )
/* K-平均法によるクラスタリングを行う */
{
    int K_number; /* K の値（生成するクラスタの総数）*/
    int i, j, x, y, xs, ys; /* 制御変数など */
    double min_distance, distance; /* 距離に関する変数 */
    int min_num; /* 最も近いクラスタの番号 */
    int finish; /* 処理終了に関するフラグ */
    int counter; /* 処理回数 */
    unsigned char init_main_vector[MAX_CLUSTERS][DIMENSION];
                            /* 代表ベクトル */
    double sum[DIMENSION], sum_number, difference;
                            /* 作業用変数 */
    int original_data_size, coded_data_size; /* データ量に関する変数 */
    double compression_rate; /* 圧縮率 */

    printf("***** ベクトル量子化法 *****\n");
    do {
        printf("代表ベクトルの本数(8～%d):", MAX_CLUSTERS );
        scanf("%d",&K_number);
    } while ( K_number < 8 || K_number > MAX_CLUSTERS );
    /* 仮代表ベクトルの設定 */
    for ( i = 0; i < K_number; i ++ ){
        x = random_int( x_block );
        y = random_int( y_block );
        for ( j = 0; j < DIMENSION; j ++ ){
            main_vector[i][j] = vector[y][x][j];
        }
    }
    /* K-平均法によってベクトルのクラスタリングを行う */
    finish = 0;    counter = 0;
    while ( ! finish ){
        counter ++;  
        printf("%d 回目のクラスタリング中です．\n", counter );
        /* クラスタの初期代表点への代入 */
        for ( i = 0; i < K_number; i ++ ){
            for ( j = 0; j < DIMENSION; j ++ )
                init_main_vector[i][j] = main_vector[i][j];
        }
        /* 各ブロックの所属クラスタ番号の初期化 */
        for ( y = 0; y < y_block; y ++ )
            for ( x = 0; x < x_block; x ++ )
                cluster[y][x] = - 1;
        /* 各点の所属クラスタを決定する */
        for ( y = 0; y < y_block; y ++ ){
            for ( x = 0; x < x_block; x ++ ){
                /* 各クラスタ代表点との距離を求める */
                min_distance = 255 * 255 * DIMENSION;
                for ( i = 0; i < K_number; i ++ ){
                    distance = calc_distance( y, x, i );
                    if ( distance < min_distance){
                        min_distance = distance;
                        min_num = i;
                    }
                }
                /* [y][x]ブロックをクラスタ No.min_num へ属させる */
                cluster[y][x] = min_num;
            }
        }
        /* 各クラスタの代表点を修正する */
        for ( i = 0; i < K_number; i ++ ){
            for ( j = 0; j < DIMENSION; j ++ ){
                sum[j] = 0.0;
            }
            sum_number = 0.0;
            for ( y = 0; y < y_block; y ++ ){
                for ( x = 0; x < x_block; x ++ ){
                    if ( cluster[y][x] == i ){
                        sum_number = sum_number + 1.0;
                        for ( j = 0; j < DIMENSION; j ++ )
                            sum[j] = sum[j] + vector[y][x][j];
                    }
                }
            }
            for ( j = 0; j < DIMENSION; j ++ ){
                main_vector[i][j] = (unsigned char)
                    ( sum[j] / sum_number );
            }
        }
        /* クラスタ中心が移動したかどうかのチェック */
        finish = 1;
        for ( i = 0; i < K_number; i ++ ){
            difference = 0.0;    
            for ( j = 0; j < DIMENSION; j ++ ){
                difference = difference + abs(
                    init_main_vector[i][j] - main_vector[i][j] );
            }
            if ( difference > MIN_DIFFERENCE ) finish = 0;
        }
        if ( finish == 1 ){  /* 処理終了 */ 
            /* 各ブロックを代表ベクトルに置き換える */
            x_size2 = x_size1;   y_size2 = y_size1;
            for ( y = 0; y < y_block; y ++ ){
                for ( x = 0; x < x_block; x ++ ){
                    xs = BLOCK_SIZE * x;    
                    ys = BLOCK_SIZE * y;
                    for ( i = 0; i < BLOCK_SIZE; i ++ ){
                        for ( j = 0; j < BLOCK_SIZE; j ++ ){
                            image2[ ys + i][ xs + j] =
                                main_vector[ cluster[y][x] ]
                                           [i*BLOCK_SIZE+j];
                        }
                    }
                }
            }
        }
    }
    printf("ベクトル量子化が終了しました．\n");
    /* データ量・圧縮率関係の計算と表示 */
    original_data_size = x_size1 * y_size1 * 8;
    printf("元の階調画像のデータ量 = %d x %d x 8 = %d bits\n",
        x_size1,y_size1,original_data_size);
    i = K_number - 1;    j = 0;
    do{
        i = i / 2;   j ++;
    } while ( i > 0 );  /* j : 代表ベクトルの指定に必要なビット数 */
    coded_data_size = BLOCK_SIZE * BLOCK_SIZE * K_number * 8
                            /* 代表ベクトル */
        + j * x_block * y_block; /* ルックアップテーブル */
    printf("圧縮データ（画像部分）のデータ量 = %d bits\n",
        coded_data_size);
    compression_rate = (double)coded_data_size
        / (double)original_data_size * 100.0;
    printf("圧縮率：%f [％]\n",compression_rate);
}

main( )
{
    load_image_data( );           /* 画像ファイル --> image1  */
    initialize_random_number( );
    obtain_vector_data( );        /* image1中の点をベクトル化 */
    clustering_using_Kmeans( );   /* K-平均法でクラスタリング */
    save_image_data( );           /* image2 --> ファイル出力  */
    return 0;
}
