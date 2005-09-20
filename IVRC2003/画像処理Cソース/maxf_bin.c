/* GA による関数最適化（バイナリコード） maxf_bin.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"myGA.h" 

/* 探索関数のためのパラメータ */
#define PNUM  5   /* 探索関数のパラメータ総数（変更可）　*/
int fparam[PNUM]; /* 探索関数の変数（乱数で決められる）  */
#define SOLUTION_FITNESS 0.999 /* 適応度の目標値（1.0でも可) */
#define MAX_GENERATION    2000 /* 想定最大世代数（変更可）  */

void set_optimizing_task( )
/* 扱う最適化問題を定義する */
{
    /* 関数の最大値探索問題の例題            */
    /* ８ビットの変数PNUM個で構成されている  */
    int i; /* 制御変数 */

    gene_size = 8 * PNUM;
    for ( i = 0; i < PNUM; i ++ )
        fparam[i] = 80 + random_int( 80 );
}

void trans_from_genotype_to_parameters( int number, int *p )
/* 個体番号 No.number の遺伝子型をパラメータに変換する */
{
    int i, j; /* ループ変数 */

    /* 遺伝子型から整数への変換 */
    for ( i = 0; i < PNUM; i ++ ){
        *(p + i)= 0;
        for ( j = 0; j < 8; j ++ )
            *(p + i) = *(p + i) * 2 + genotype[number][j+8*i];
    }
}

double fitness_value( int number )
/* set_optimizing_task( )で説明されている個体 No.number */
/* の適応度を返す（この場合はテスト関数）               */
{
    int i, p[PNUM];  /* 制御変数，パラメータ用変数 */
    double f;   /* 関数値 */

    /* 遺伝子型から (x,y) へ */
    trans_from_genotype_to_parameters( number, p );
    /* 適応度の計算 */
    f = 1.0;
    for ( i = 0; i < PNUM; i ++ )
        f = f - abs( p[i] - fparam[i] ) / (120.0 * PNUM);
    if ( f < 0.0 ) f = 0.0; else if ( f > 1.0 ) f = 1.0;
    return f;
}

void calculate_fitness( )
/* 全員の適応度を求める */
{
    int i; /* ループ変数 */
    
    for ( i = 0; i < pop_size; i ++ )
        fitness[i] = fitness_value(i);
}

void display_best_individual( int generation )
/* 最良個体の遺伝子型，適応度を表示する */
{
    int i; /* 制御変数 */

    printf("No.%d：",generation);
    for ( i= 0; i < gene_size; i ++ ){
        printf("%d",elite_genotype[i]);
        if ( (i+1) % 8 == 0 ) printf(" ");
    }
    printf("--> %f\n", elite_fitness);
}

void generation_iteration( )
/* 世代交代を行う */
{
    int i, generation; /* ループ変数，世代数 */
    int p[PNUM];    /* 最良個体のパラメータ */

    generation = 1;
    calculate_fitness( );    /* 適応度の計算   */
    find_and_set_best_individual( ); /* エリートの検出 */
    do{
        generation ++;
        /* ルーレットルールに基づく淘汰 */
        selection_using_roulette_rule( );
        /* 染色体の交叉 */
        execute_crossover( );
        /* 遺伝子の突然変異 */
        execute_mutation( );
        /* 適応度の計算 */
        calculate_fitness( );
        /* エリート保存 */
        if ( elite_flag == 1 ) elitist_strategy();
        find_and_set_best_individual(); /* エリートの検出 */
        /* 最良個体の表示 */
        display_best_individual( generation );
    } while ( elite_fitness < SOLUTION_FITNESS && 
              generation < MAX_GENERATION );
    trans_from_genotype_to_parameters( elite_number, p );
    printf("最終的な解：");
    for ( i=0; i<PNUM; i++ ) printf("%d ",p[i]);
    printf("\n真の最適解：");
    for ( i=0; i<PNUM; i++ ) printf("%d ",fparam[i]);
    printf("\n");
}

main( )
{
    initialize_random_number( );  /* 乱数の初期化       */
    set_optimizing_task( );       /* 問題の設定         */
    initialize_parameters( );     /* GAパラメータの設定 */
    initialize_genes( );          /* 遺伝子型の初期化   */
    generation_iteration( );      /* 世代交代           */
    return 0;
}

