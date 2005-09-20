/* １次元 FFT のパッケージ FFT1.h */
#define FFT_MAX  1024
#define PI  3.141592653589  /* 円周率 */
int calc_power_of_two( int number );
void make_initial_data( double *re_part, double *im_part, 
                        int num_of_data, int power );
void FFT1( double *re_part, double *im_part, int num_of_data, 
           int flag );

int calc_power_of_two( int number )
/* number が２の何乗であるか調べて返す */
/* 例）8 --> 3, 16 --> 4, 32 --> 5,... */
{
    int power;

    power = 0;
    while ( number != 1 ){
        power ++;
        number = number >> 1; 
            /* 右方向に１ビットシフト（２で割るのと同じ） */
    }
    return power;
}

void make_initial_data( double *re_part, double *im_part, 
                        int num_of_data, int power )
/* データを並び替えて時間の間引きによる FFT の初期データを作る */
/* double *re_part, *im_part; 元データ(実数部，虚数部)の先頭アドレス */
/* int num_of_data;           データ総数                             */
/* int power;                 num_of_data は２の power 乗            */
{
    int i, j;              /* 制御変数                               */
    int ptr, offset;       /* 元データの要素番号を決めるための変数   */
    int new_ptr;           /* 計算後のデータを代入する配列の要素番号 */
    int dft;               /* DFT点数 ( i のループで半分ずつになる） */
    double re_buf[FFT_MAX], im_buf[FFT_MAX]; /* 計算結果用配列 */

    dft = num_of_data;
    for ( i = 1; i < power; i ++ ){
        /* i は回数を制御する変数（計算には用いていない） */
        new_ptr = 0;  offset = 0;
        while( new_ptr < num_of_data ){
            ptr = 0;
            while( ptr < dft ){
                re_buf[new_ptr] = *( re_part + offset + ptr );
                im_buf[new_ptr] = *( im_part + offset + ptr );
                new_ptr ++;
                ptr = ptr + 2;
                if ( ptr == dft ) ptr = 1;
            }
            offset = offset + dft;
        }
        /* 計算結果を元のデータ配列にコピーする */
        for ( j = 0; j < num_of_data; j ++ ){
            *( re_part + j ) = re_buf[j];
            *( im_part + j ) = im_buf[j];
        }
        dft = dft / 2;
    }
}

void FFT1( double *re_part, double *im_part, int num_of_data, int flag )
/* データの FFT (flag = 1), 逆 FFT (flag = -1) を行う関数            */
/* double *re_part, *im_part; 元データ(実数部，虚数部)の先頭アドレス */
/* int num_of_data, flag;     データ総数，FFT・逆FFT を決めるフラグ  */
{
    int i, j, k, power;                         /* 制御変数，べき乗 */
    double unit_angle, step_angle;              /* 角度用の変数     */
    int dft, half, num_of_dft;   /* DFT点数，その半数，DFT 実行回数 */
    int num_out, num_in1, num_in2;       /* DFTの入出力信号線の番号 */
    double re_buf, im_buf, angle; /* 実数部，虚数部，角度用作業変数 */
                  /* 次は計算結果を一時的に保存するための作業用配列 */
    static double re_part_new[FFT_MAX], im_part_new[FFT_MAX];

    /* 逆FFT ( flg = -1 ) のとき各データを num_of_data で割り */
    /* 複素共役をとる                                         */
    if ( flag == -1){
        for ( i = 0; i < num_of_data; i ++ ){
            *( re_part + i ) =   *( re_part + i ) / num_of_data;
            *( im_part + i ) = - *( im_part + i ) / num_of_data;
        }
    }
    /* num_of_data が２の何乗(power)であるか調べる */
    power = calc_power_of_two( num_of_data );
    /* 初期データのための元データの順番の入れ替え */
    make_initial_data( re_part, im_part, num_of_data, power );
    /* 2点DFT, 4点DFT, 8点DFT, ... の順に実行する */
    unit_angle = 2.0 * PI / num_of_data;
    dft = 2;   /* = DFT の点数の初期値，２倍してゆく */
    for ( i = 0; i < power; i ++ ){
        /* DFT 点の DFT を行う                      */
        /* i=0 -> 2点, i=1 -> 4点, i=2 -> 8点, .... */
        num_of_dft = num_of_data / dft;   /* DFT の実行回数 */
        step_angle = unit_angle *  num_of_dft;
        half = dft / 2;
        for ( j = 0; j < num_of_dft; j ++ ){
            angle = 0.0;
            for ( k = 0; k < dft; k ++ ){
                /* No.num_in1 と No.num_in2 から No.num_out を出力 */
                /* （係数が１の方の信号を No.num_in1 としている）  */
                num_out = j * dft + k;
                if ( k < half ){
                    num_in1 = num_out;
                    num_in2 = num_in1 + half;
                } else
                {
                    num_in2 = num_out;
                    num_in1 = num_out - half;
                }
                /* 実数部(re_)・虚数部(im_)に分けて計算 */
                re_buf = *( re_part + num_in2 );
                im_buf = *( im_part + num_in2 );
                re_part_new[num_out] = *( re_part + num_in1 ) 
                    + re_buf * cos(angle) + im_buf * sin(angle);
                im_part_new[num_out] = *( im_part + num_in1 ) 
                    + im_buf * cos(angle) - re_buf * sin(angle);
                /* 角度を更新 */
                angle = angle + step_angle;
            }
        }
        /* 計算後のデータを元の配列に戻す */
        for ( j = 0; j < num_of_data; j ++ ){
            *( re_part + j ) = re_part_new[j];
            *( im_part + j ) = im_part_new[j];
        }
        dft = dft * 2; /* DFT の点数を２倍に更新 */
    }
    /* 逆FFT ( flg = -1 ) のとき各データの複素共役をとる */
    if ( flag == -1 ){
        for ( j = 0; j < num_of_data; j ++ ){
            *( im_part + j ) = - *( im_part + j );
        }
    }
}
