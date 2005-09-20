/* 遺伝的アルゴリズム用のヘッダファイル myGA.h */

    /* 定数宣言 */
#define MAX_POP_SIZE   200   /* 最大個体数 */
#define MAX_GENE_SIZE   50   /* 想定する最大染色体長 */
    /* 大域変数 */
    /* 個体，新個体の遺伝子型 */
unsigned char genotype[MAX_POP_SIZE][MAX_GENE_SIZE]; 
unsigned char new_genotype[MAX_POP_SIZE][MAX_GENE_SIZE];
    /* 個体，新個体の適応度（通常は 0 ～ 1 の実数） */
double fitness[MAX_POP_SIZE];
double new_fitness[MAX_POP_SIZE];
    /* エリートの遺伝子型と適応度 */
unsigned char elite_genotype[MAX_GENE_SIZE];
double elite_fitness;
int elite_number; /* エリートの個体番号 */
    /* GAのパラメータ */
int pop_size;    /* 個体総数 */
int gene_size;   /* 染色体長 */
double crossover_rate;  /* 交叉率 [0, 1]     */
double mutation_rate;   /* 突然変異率 [0, 1] */
int elite_flag; /* エリート保存戦略あり(1)／なし(0) */
int crs_type;    /* 交叉方法（1:1点，2:2点，3:一様）  */

/* 関数のプロトタイプ宣言 */
void initialize_random_number( ); /* 乱数を初期化する */
int random_int( int n );          /* 0～n-1までの整数乱数を返す */
double random_double( double n ); /* 0.0～nまでの実数乱数を返す */
    /* 2つの unsigned char の値を交換する */
void swap_unsigned_char( unsigned char *n1, unsigned char *n2 ); 
void swap_int( int *n1, int *n2 ); /* 2つの int の値を交換する */
void intialize_parameters( );     /* パラメータを初期化する */
void initialize_genes( );         /* 染色体を初期化する */
void copy_new_to_old( );          /* 配列をコピーする（新→旧）*/
void one_point_crossover( int n1, int n2 ); /* １点交叉を行う */
void two_point_crossover( int n1, int n2 ); /* ２点交叉を行う */
void uniform_crossover( int n1, int n2 ); /* 一様交叉を行う */
void selection_using_roulette_rule( ); /* ルーレット選択を行う */
void execute_crossover( ); /* 交叉の種類を指定して実行する */
void execute_mutation( ); /* 突然変異を実行する */
void find_and_set_best_individual( ); /* 現在の最良個体をみつける */
void elitist_strategy( ); /* エリート保存戦略を実行する */

/* 以下は関数の本体 */

void initialize_random_number( )
/* 乱数の初期化 */
{
    int seed; /* シード */

    printf("乱数の系列を初期化します．\n");
    printf("乱数のシード（任意の正の整数）:");
    scanf("%d",&seed);
    if ( seed < 1 ) seed = 1;
    srand( seed );
}

int random_int( int n )
/* 0 以上 n - 1 以下の整数の乱数を返す */
{
    return (int)( (double)rand( ) / ( (double)RAND_MAX + 1.0 ) * n );
}

double random_double( double n )
/* 0.0 から n まで（ n より小さい）の実数の乱数を返す */
{
    return (double)rand( ) / ( (double)RAND_MAX + 1.0 ) * n;
}

void swap_unsigned_char( unsigned char *n1, unsigned char *n2 )
/* unsigned char n1 と n2 を交換する */
{
    unsigned char n;
    n = *n1;    *n1 = *n2;    *n2 = n;
}

void swap_int( int *n1, int *n2 )
/* int n1 と n2 を交換する */
{
    int n;
    n = *n1;    *n1 = *n2;    *n2 = n;
}

void initialize_parameters( )
/* gene_size 以外のパラメータを設定する */
{
    float work; /* 作業変数 */

    printf("***** ＧＡのパラメータの設定 *****\n");
    do {
        printf("個体総数 (10～%d) : ",MAX_POP_SIZE);
        scanf("%d",&pop_size);
    } while ( pop_size < 10 || pop_size > MAX_POP_SIZE );
    do {
        printf("交叉方法 (1:1点，2:2点，3:一様） : ");    
        scanf("%d",&crs_type);    
    } while ( crs_type != 1 && crs_type != 2 && crs_type != 3 );
    do {
        printf("交叉率 (0.0～1.0) : ");    
        scanf("%f",&work);    crossover_rate = work;
    } while ( crossover_rate < 0.0 || crossover_rate > 1.0 );
    do {
        printf("突然変異率 (0.0～1.0) : ");    
        scanf("%f",&work);    mutation_rate = work;
    } while ( mutation_rate < 0.0 || mutation_rate > 1.0 );
    printf("エリート保存戦略を行う(1)/行わない(0) : ");    
    scanf("%d",&elite_flag); 
    if ( elite_flag != 1 ) elite_flag = 0;
}

void initialize_genes( )
/* 染色体を初期化する */
{
    int i, j;        /* ループ変数 */

    for ( i = 0; i < pop_size; i ++ ){
        for ( j = 0; j < gene_size; j ++ ){
            if ( random_double(1.0) < 0.5 ) genotype[i][j] = 0;
            else genotype[i][j] = 1;
        }
    }
}

void copy_new_to_old( )
/* new_genotype => genotype, new_fitness => fitness */
/* 配列をコピーする                                 */
{
    int i, j; /* 制御変数 */

    for ( i = 0; i < pop_size; i ++ ){
        for ( j = 0; j < gene_size; j++ )
            genotype[i][j] = new_genotype[i][j];
        fitness[i] = new_fitness[i];
    }
}

void one_point_crossover( int n1, int n2 )
/* 個体 No.n1 と個体 No.n2 の染色体を１点交叉させ，元と置き換える */
{
    int crs_pnt;        /* 交叉位置（両端は設定しない）*/
    int i;              /* 制御変数 */

    crs_pnt = random_int( gene_size );
    /* 後半部を入れ替え */
    for ( i = crs_pnt + 1; i < gene_size; i ++ ){
        swap_unsigned_char( &genotype[n1][i], &genotype[n2][i] );  
    }
}

void two_point_crossover( int n1, int n2 )
/* 個体 No.n1 と個体 No.n2 の染色体を２点交叉させ，元と置き換える */
{
    int crs_pnt1, crs_pnt2;  /* 交叉位置（両端は設定しない）*/
    int i;                   /* ループ変数 */

    crs_pnt1 = random_int( gene_size );
    do {
        crs_pnt2 = random_int( gene_size );
    } while ( crs_pnt1 == crs_pnt2 );
    if ( crs_pnt1 > crs_pnt2 ) swap_int( &crs_pnt1, &crs_pnt2 );
    /* 中間部を入れ替え */
    for ( i = crs_pnt1 + 1; i < crs_pnt2; i ++ ){
        swap_unsigned_char( &genotype[n1][i], &genotype[n2][i] );
    }
}

void uniform_crossover( int n1, int n2 )
/* 個体 No.n1 と個体 No.n2 の染色体を一様交叉させ，元と置き換える */
/* 子孫は２つ作り（互いに反転パターン）それらと両親を入れ替える． */
{
    int i; /* ループ変数   */

    /* 遺伝子ごとにどちらかの親の遺伝子を選択する */
    for ( i = 0; i < gene_size; i ++ ){
        if ( random_double( 1.0 ) < 0.5 ){  
            /* 等確率で親どうしの遺伝子を入れ替えている */
            swap_unsigned_char( &genotype[n1][i], &genotype[n2][i] );
        }
    }
}

void selection_using_roulette_rule( )
/* ルーレットルールに基づいて次世代の個体を決める  */
/* データは new_genotype, new_fitness に代入される */
{
    int i, j, num; /* 制御変数，選択された個体番号 */
    double sum, rand_real;  /* 作業用変数 */
    static double roulette_table[MAX_POP_SIZE]; /* ルーレット */

    /* ルーレットの生成 */
        /*（１周分求めてから合計で割って比率を出す） */
    sum = 0.0;
    for ( i = 0; i < pop_size; i ++ ) 
        sum = sum + fitness[i];
    for ( i = 0; i < pop_size; i ++ ){
        roulette_table[i] = fitness[i] / sum;
    }
        /* 各個体の境界の比率を roulette_table へ */
    sum = 0.0;
    for ( i = 0; i < pop_size; i ++ ){
        sum = sum + roulette_table[i];
        roulette_table[i] = sum;
    }
    /* 選択操作（ルーレットを pop_size 回だけ回す）*/
    for ( i = 0; i < pop_size; i ++ ){
        rand_real = random_double( 1.0 );  /* [0.0, 1.0) */
        for ( num = 0; num < pop_size; num ++ ){
            if ( roulette_table[num] > rand_real ) break;
            /* No.num が選択された */
        }
        /* 新しい No.i 個体に No.num のデータを代入 */
        for ( j = 0; j < gene_size; j ++ )
            new_genotype[i][j] = genotype[num][j];
        new_fitness[i] = fitness[num];
    }
    copy_new_to_old( ); /* new から普通の genotype, fitnes に戻す */
}

void execute_crossover( )
/* ２つずつランダムに組み合わせて交叉させる */
/* 大域変数 crs_type =1:１点交叉，=2:２点交叉，=3：一様交叉 */
{
    int i;  /* ループ変数 */
    int num1, num2;  /* 個体番号 */
    int num_of_pair;   /* ペアの総数 */
    static number[MAX_POP_SIZE]; /* 入れ替えのための配列 */

    for ( i = 0; i < pop_size; i ++ ) number[i] = i; /* 初期化 */
    /* ランダムに並べ替え */
    for ( i = 0; i < pop_size * 10; i ++ ){
        num1 = random_int( pop_size );
        num2 = random_int( pop_size );
        swap_int( &number[ num1 ], &number[ num2 ] );
    }
    num_of_pair = pop_size / 2;
    for ( i = 0; i < num_of_pair; i ++ ){
        num1 = number[ 2 * i ];    /* 親１ */
        num2 = number[ 2 * i + 1]; /* 親２ */
        /* 交叉させるかどうか決定する */
        if ( random_double( 1.0 ) <= crossover_rate ){
            if ( crs_type == 1 ) one_point_crossover( num1, num2 );
            else if ( crs_type == 2 ) two_point_crossover( num1, num2 );
            else uniform_crossover( num1, num2 );
        }
    }
}

void execute_mutation( )
/* 全個体の全遺伝子の突然変異 */
{
    int i, j; /* ループ変数 */

    for ( i = 0; i < pop_size; i ++ ){
        for ( j = 0; j < gene_size; j ++ ){
            if ( random_double( 1.0 ) <= mutation_rate )
                genotype[i][j] = (unsigned char)(1 - genotype[i][j]);
        }
    }
}

void find_and_set_best_individual( )
/* 現在の最良個体を検出し，所定の大域変数に代入する     */
/* 大域変数：elite_number, elite_fitness, elite_genpyte */
{
    int i; /* ループ変数 */
    double best_fitness; /* 最大適応度 */

    best_fitness = 0.0;
    for ( i = 0; i < pop_size; i ++ )
        if ( fitness[i] > best_fitness ){
            elite_number = i;
            best_fitness = fitness[i];
        }
    for ( i = 0; i < gene_size; i ++ )
        elite_genotype[i] = genotype[elite_number][i];
    elite_fitness = fitness[elite_number];
}
    
void elitist_strategy( )
/* エリート保存を行う */
{
    int worst_number;     /* 最悪個体番号 */
    int i;             /* ループ変数     */
    double worst_fitness; /* 最悪適応度   */

    /* 最悪個体の検出 */
    worst_fitness = 1.0;    worst_number = 0;
    for ( i = 0; i < pop_size; i ++ ){
        if ( fitness[i] < worst_fitness ){
            worst_number = i;
            worst_fitness = fitness[i];
        }
    }
    /* 前回のエリートと最悪個体の入れ替え */
    for ( i = 0; i < gene_size; i ++ )
        genotype[worst_number][i] = elite_genotype[i];
    fitness[worst_number] = elite_fitness;
}
