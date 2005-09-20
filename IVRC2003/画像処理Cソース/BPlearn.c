/* 誤差逆伝播(BP)法のプログラム BPlearn.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"myNN.h" /* 神経回路網のヘッダファイル */

#define E  0.3             /* =ε（学習係数）  */
#define LEARNING_PERIOD  5
    /* 何回ごとに教師信号を変えるかを指定 */

void calc_hidden_and_output_units( )
/* 現在の入力に対して中間層，出力層のユニットの状態を求める */
/* このルーチンの前に input_unit[ ] に現在の入力を代入する  */
{
    int i, j;      /* ループ変数 */
    double sum;    /* 重み付出力の和 */

    /* 隠れ層の各ユニットを状態遷移させる */
    input_unit[num_input - 1] = 1.0; /* しきい値設定ユニット */
    for ( i = 0; i < num_hidden - 1; i ++ ){
        sum = 0.0;
        for ( j = 0; j < num_input; j ++ )
            sum = sum + input_unit[j] * wght_in_hid[j][i];
        hidden_unit[i] = 1.0 / ( 1.0 + exp( - sum ) );
    }
    /* 出力層の各ユニットを状態遷移させる */
    hidden_unit[num_hidden - 1] = 1.0; /* しきい値設定ユニット */
    for ( i = 0; i < num_output; i ++ ){
        sum = 0.0;
        for ( j = 0; j < num_hidden; j ++ )
            sum = sum + hidden_unit[j] * wght_hid_out[j][i];
        output_unit[i] = 1.0 / ( 1.0 + exp( - sum ) );
    }
}

double calc_error_of_NN( )
/* 現在の全教師データに対する回路網の出力２乗誤差の平均値を返す */
{
    int i, j;          /* 制御変数 */
    double err, sum;   /* ビットごとの誤差，その２乗和 */

    sum = 0.0;
    /* 各教師信号との誤差を求める */
    for ( i = 0; i < num_learn; i ++ ){
        /* 入力信号を入力する */
        for ( j = 0; j < num_input - 1; j ++ ){
            if ( learn_input_data[i][j] < 0.5 )
                input_unit[j] = 0.1;
            else input_unit[j] = 0.9;
        }
        input_unit[num_input - 1] = 1.0; /* しきい値設定ユニット */
        /* 隠れ層，出力層のユニットを状態遷移させる */
        calc_hidden_and_output_units( );
        /* 出力誤差を求める */
        for ( j = 0; j < num_output; j ++ ){
            err = learn_output_data[i][j] - output_unit[j];
            sum = sum + err * err;  /* 誤差の２乗を累積 */
        }
    }
    sum = sum / (double)num_learn; /* 誤差をデータ数で平均化 */
    return( sum );
}

void BPlearning( )
/* 誤差逆伝播(BP)法に基づく回路網の学習を行う */
{
    int counter, lrn_num;  /* 学習回数，現在の教師信号の番号 */
    int learning_time;     /* トータルの学習回数 */
    double acceptable_error;  /* 許容最大誤差，キー入力で指定する */
    double error;  /* 全教師信号に対する平均２乗誤差 */
    float f;       /* 作業変数 */
    int i, j, k;   /* 制御変数 */
    /* 以下は結合荷重の修正量を決めるときに使っている変数です． */
    /* 本文の式 (8.2) に合わせるため，変な名前になっています．  */
    double delta_W, O_mi, y_i, O_k1_j;
    double d_hid[MAX_HIDDEN][MAX_OUTPUT], d_in;
    double sum;

    /* 学習を終了させるための条件の指定 */
    do {
        printf("許容最大平均誤差( 0 以上 0.01 以下） : ");
        scanf("%f",&f);    acceptable_error = f;
    } while ( acceptable_error < 0.0 || acceptable_error > 0.01 );
    /* 学習開始 */
    learning_time = 1;    counter = 0;    lrn_num = 0;
    do {
        /* 入力ユニットを教師信号 No.lrn_num に一致させる */
        for ( i = 0; i < num_input - 1; i ++ )
            input_unit[i] = learn_input_data[lrn_num][i];
        input_unit[num_input -1] = 1.0; /* しきい値設定ユニット */
        /* 中間層と出力層のユニットの状態を遷移させる */
        calc_hidden_and_output_units( );
        /* 出力誤差を用いて，各結合荷重を微修正する */
        /* part 1 :  出力層 ← 中間層の結合荷重 */
        for ( j = 0; j < num_hidden; j ++ ){
            for ( i = 0; i < num_output; i ++ ){
                /* 変数名を本文に合わせています */
                O_mi = output_unit[i];  /* i 番目の出力 */
                y_i  = learn_output_data[lrn_num][i]; /* 教師出力 */
                d_hid[j][i] = ( O_mi - y_i ) * O_mi * ( 1.0 - O_mi );
                O_k1_j = hidden_unit[j];
                delta_W = - E * d_hid[j][i] * O_k1_j;
                /* 結合荷重の修正 */
                wght_hid_out[j][i] = wght_hid_out[j][i] + delta_W;
            }
        }
        /* part 2 :  中間層 ← 入力層の結合荷重 */
        for ( j = 0; j < num_input; j ++ ){
            for ( i = 0; i < num_hidden; i ++ ){
                sum = 0.0;
                for ( k = 0; k < num_output; k ++ )
                    sum = sum + wght_hid_out[i][k] * d_hid[i][k];
                d_in = sum * hidden_unit[i] * ( 1.0 - hidden_unit[i] );
                delta_W = - E * d_in * input_unit[j];
                /* 結合荷重の修正 */
                wght_in_hid[j][i] = wght_in_hid[j][i] + delta_W;
            }
        }
        error = calc_error_of_NN(); /* 現在の回路網の誤差を求める */
        learning_time ++;    counter ++;  /* カウンタの更新 */
        printf("No.%d : error = %f\n", learning_time, error);
        /* 一定間隔ごとに教師信号を切り替える */
        if ( counter > LEARNING_PERIOD ){  
            counter = 0;
            lrn_num ++;    if ( lrn_num > num_learn ) lrn_num = 0;
        }
    } while ( error > acceptable_error );  /* 誤差小 -> 終了 */
    printf("学習が終了しました．\n");
}

void save_NN_data( )
/* 学習後の神経回路網の結合荷重をファイルに出力する    */
/* データ形式 : N1 N2 N3   -->  入力，中間，出力ユニット数 */
/* 結合荷重データを wght_in_hid[N1][N2], wght_hid_out[N2][N3] */
/* を順番に出力して終わり．添え字の動かし方は以下を参照のこと．*/
{
    int i, j; /* ループ変数 */
    char file_name[256];  /* 出力ファイル名用文字配列 */
    FILE *fp; /* ファイルポインタ */  

    /* 出力ファイルのオープン */
    printf("学習したニューラルネットワークを保存します．\n");
    printf("出力ファイル名 (*.nn) : ");  /* 拡張子 = .nn としている */
    scanf("%s",file_name);
    fp = fopen( file_name, "w");
    fprintf( fp, "%d %d %d\n", num_input, num_hidden, num_output );
    for ( i = 0; i < num_input; i ++ )
        for ( j = 0; j < num_hidden; j ++ )
            fprintf( fp, "%f\n", wght_in_hid[i][j] );
    for ( i = 0; i < num_hidden; i ++ )
        for ( j = 0; j < num_output; j ++ )
            fprintf( fp, "%f\n", wght_hid_out[i][j] );
    printf("データは正しく出力されました．\n");
    fclose(fp);
}

main( )
{
    load_learn_data( ); /* 教師データの読み込みと結合荷重の初期化 */
    BPlearning( );      /* BP 学習法による結合荷重の最適化        */
    save_NN_data( );    /* 学習後の神経回路網を保存               */
    return 0;
}
