/* 神経回路網のヘッダ myNN.h */

/* 神経回路網に関する定数宣言 */
#define MAX_INPUT  100       /* 入力ユニット数の最大値 */
#define MAX_HIDDEN  50       /* 中間ユニット数の最大値 */
#define MAX_OUTPUT  20       /* 出力ユニット数の最大値 */
#define MAX_WEIGHT  5.0      /* 結合荷重の初期値の範囲 */
                         /* - MAX_WEIGHT ～ MAX_WEIGHT */
#define MAX_LEARN  50        /* 教師信号数の最大値     */

/* 神経回路網に関する大域変数の宣言 */
    /* 結合荷重（入力層 → 中間層） */
double wght_in_hid[MAX_INPUT][MAX_HIDDEN];
    /* 結合荷重（中間層 → 出力層） */
double wght_hid_out[MAX_HIDDEN][MAX_OUTPUT];
    /* 教師信号を保存するための配列 */
double learn_input_data[MAX_LEARN][MAX_INPUT];
double learn_output_data[MAX_LEARN][MAX_OUTPUT];
    /* 入力，中間，出力ユニット数 */
int num_input, num_hidden, num_output;
    /* 教師信号数 */
int num_learn;
    /* ユニットの状態 */
double input_unit[MAX_INPUT];
double hidden_unit[MAX_HIDDEN];
double output_unit[MAX_OUTPUT];

/* 関数のプロトタイプ宣言 */
double random_double( double n );
void initialize_weights( );
int get_data( FILE *fp );
void load_learn_data( );

/* 以下は関数の本体 */

double random_double( double n )
/* 0.0 から n まで（ n より小さい）の実数の乱数を返す */
{
    return (double)rand( ) / ( (double)RAND_MAX + 1.0 ) * n;
}

void initialize_weights( )
{
    int seed, i, j; /* 作業変数 */

    /* 乱数の初期化 */
    printf("乱数の系列を初期化します．\n");
    printf("乱数のシード（任意の正の整数）:");
    scanf("%d",&seed);
    if ( seed < 1 ) seed = 1;
    srand( seed );
    for ( i = 0; i < num_input; i ++ ){
        for ( j = 0; j < num_hidden; j ++ ){
            wght_in_hid[i][j] = 2.0 * MAX_WEIGHT 
                * random_double( 1.0 ) - MAX_WEIGHT;
        }
    }
    for ( i = 0; i < num_hidden; i ++ ){
        for ( j = 0; j < num_output; j ++ ){
            wght_hid_out[i][j] = 2.0 * MAX_WEIGHT 
                * random_double( 1.0 ) - MAX_WEIGHT;
        }
    }
}

int get_data( FILE *fp )
/* ファイルから１文字(０または１)読み込む */
{
    char ch;

    do {
        ch = (char)fgetc( fp );
    } while ( ch != '0' && ch != '1' );
    if ( ch == '0' ) return 0;
    else return 1;
}

void load_learn_data( )
/* 神経回路網の教師信号のデータをファイルから読み込む */
{
    char file_name[256];  /* 入力ファイル名 */
    int buffer;   /* データを読み込むときの作業変数 */
    FILE *fp;  /* ファイルポインタ */
    int i, j;  /* ループ変数 */

    /* 教師信号ファイルのオープン */
    printf("教師信号ファイルを読み込みます．\n");
    printf("教師ファイル名 (*.lrn) : ");
    scanf("%s", file_name);
    fp = fopen( file_name, "r" );
    if ( NULL == fp ){
        printf("その名前のファイルは存在しません．\n");
        exit(1);
    }
    /* num_input, num_hidden, num_output, num_learn */
    fscanf( fp, "%d %d %d %d", &num_input, &num_hidden,
            &num_output, &num_learn );
    printf( "入力ユニット数 + しきい値設定ユニット = %d\n",
             num_input);
    printf( "中間ユニット数 + しきい値設定ユニット = %d\n",
             num_hidden );
    printf( "出力ユニット数 = %d\n", num_output );
    printf( "教師信号数     = %d\n", num_learn );
    if ( num_input  > MAX_INPUT   || 
         num_hidden > MAX_HIDDEN  ||
         num_output > MAX_OUTPUT ) {
        printf("ユニット数が不適切です．\n");
        exit(1);
    }
    /* 教師信号の読み込み */
    for ( i = 0; i < num_learn; i ++ ){
        printf("教師信号 No.%d : ",i+1);
        for ( j = 0; j < num_input - 1; j ++ ){
            /* No.num_input - 1 はしきい値設定ユニット */
            buffer = get_data( fp );
            printf("%d", buffer);
            if ( buffer == 1 ) learn_input_data[i][j] = 0.9;
            else learn_input_data[i][j] = 0.1;
        }
        printf(" → ");
        for ( j = 0; j < num_output; j ++ ){
            buffer = get_data( fp );
            printf("%d",buffer);
            learn_output_data[i][j] = (double)buffer;
        }
        printf("\n");
    }
    printf("データは正しく読み込まれました．\n");
    fclose(fp);
    initialize_weights( );    /* 結合荷重の初期化 */
} 
