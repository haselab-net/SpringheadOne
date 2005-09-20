/* 神経回路網を使って認識を行うプログラム NNrecog.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"myNN.h"  /* for get_data( ) */

void calc_hidden_and_output_units( )
/* 現在の入力に対して中間層，出力層のユニットの状態を求める */
/* このルーチンの前に input_unit[ ] に現在の入力を代入する  */
{
    int i,j;       /* ループ変数 */
    double sum;    /* 重み付出力の和 */

    /* 隠れ層の各ユニットを状態遷移させる */
    for ( i = 0; i < num_hidden - 1; i ++ ){
        sum = 0.0;
        for ( j = 0; j < num_input; j ++ )
            sum = sum + input_unit[j] * wght_in_hid[j][i];
        hidden_unit[i] = 1.0 / ( 1.0 + exp( - sum ) );
    }
    input_unit[num_input - 1] = 1.0; /* しきい値設定ユニット */
    /* 出力層の各ユニットを状態遷移させる */
    for ( i = 0; i < num_output; i ++ ){
        sum = 0.0;
        for ( j = 0; j < num_hidden - 1; j ++ )
            sum = sum + hidden_unit[j] * wght_hid_out[j][i];
        output_unit[i] = 1.0 / ( 1.0 + exp( - sum ) );
    }
    hidden_unit[num_hidden - 1] = 1.0; /* しきい値設定ユニット */
}

void load_NN_data( )
/* 神経回路網の結合荷重のデータをファイルから読み込む． */
{
    char file_name[256]; /* 入力ファイル名を代入する文字列 */
    FILE *fp;            /* ファイルポインタ */
    float f;             /* 作業変数 */
    int i, j;            /* 制御変数 */

    /* 入力ファイルのオープン */
    printf("神経回路網のデータを読み込みます．\n");
    printf("入力ファイル名 (*.nn) : "); /* 拡張子 = .nn としている */
    scanf("%s", file_name);
    fp = fopen( file_name, "r" );
    if ( NULL == fp ){
        printf("その名前のファイルは存在しません．\n");
        exit(1);
    }
    fscanf(fp,"%d %d %d",&num_input,&num_hidden,&num_output);
    for ( i = 0; i < num_input; i ++ )
        for ( j = 0; j < num_hidden; j ++ ){
            fscanf( fp, "%f", &f );    wght_in_hid[i][j] = f;
        }
    for ( i = 0; i < num_hidden; i ++ )
        for ( j = 0; j < num_output; j ++ ){
            fscanf( fp, "%f", &f );    wght_hid_out[i][j] = f;
        }
    fclose(fp);
    printf("結合荷重のデータを読み込みました．\n\n");
}

void calculation( )
/* 未知信号のデータをファイルから読み込みつつ結果を求めて表示する */
{
    char file_name[256]; /* 入力ファイル名を代入する文字列 */
    int num;             /* データを読み込むときの作業変数 */
    FILE *fp;            /* ファイルポインタ */
    int i, j;            /* ループ変数 */
    int num_data;        /* 未知信号の総数 */

    /* 未知信号ファイルのオープン */
    printf("未知信号ファイルを読み込みます．\n");
    printf("未知ファイル名 (*.dat) : "); /* 拡張子 = .dat としている */
    scanf("%s", file_name);
    fp = fopen( file_name, "r" );
    if ( NULL == fp ){
        printf("その名前のファイルは存在しません．\n");
        exit(1);
    }
    (void)getchar( ); /* これがないと最初データが続けて表示される */
    /* 入力，中間，出力ユニット数，未知信号の総数 */
    fscanf(fp,"%d %d %d %d",
        &num_input,&num_hidden,&num_output,&num_data);
    printf( "入力ユニット数 + しきい値設定ユニット = %d\n",
        num_input);
    printf( "中間ユニット数 + しきい値設定ユニット = %d\n",
        num_hidden );
    printf( "出力ユニット数 = %d\n", num_output );
    printf( "未知信号の総数 = %d\n\n", num_data );
    /* 未知信号の読み込み */
    printf("充分２値化されていない出力は * で表しています．\n\n");
    for ( i = 0; i < num_data; i ++ ){
        printf("未知信号 No.%d : ",i+1);
        for ( j = 0; j < num_input - 1; j ++ ){
            num = get_data( fp );
            printf("%d",num);
            if ( num == 0 ) input_unit[j] = 0.1;
            else input_unit[j] = 0.9;
        }
        input_unit[num_input - 1] = 1.0; /* しきい値設定ユニット */
        /* 中間，出力ユニットの状態遷移 */
        calc_hidden_and_output_units( );
        printf(" → ");
        for ( j = 0; j < num_output; j ++ ){
            if ( output_unit[j] >= 0.9 ) printf("1");
            else if ( output_unit[j] <= 0.1 ) printf("0");
            else printf("*");  
        }
        printf("   push return key");    
        (void)getchar( ); /* リターンを読むだけのもの */
    }
    fclose(fp);
    printf("\n全ての未知信号に対する処理を終了しました．\n");
}

main( )
{
    load_NN_data( );   /* 結合荷重データの読み込み     */
    calculation( );    /* 未知信号を読み込んで随時出力 */
    return 0;
}
