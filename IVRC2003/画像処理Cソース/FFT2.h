/* ２次元 FFT のパッケージ FFT2.h           */
/* 中で関数 FFT1 を用いている．             */
/* FFT1.h の後にインクルードして用いること．*/
/* 大域変数 */
double  data[FFT_MAX][FFT_MAX];   /* 元データの実数部 */
double jdata[FFT_MAX][FFT_MAX];   /* 元データの虚数部 */
int num_of_data;          /* データの要素番号の最大値 */
/* 関数のプロトタイプ */
void FFT2( int flag );

void FFT2( int flag )
/* 元データを data, jdata, num_of_data に代入してから使う */
/* flag = 1 : ２次元 FFT, flag = -1 : ２次元逆 FFT        */
{
    int i, j; /* ループ変数 */
    static double re[FFT_MAX], im[FFT_MAX]; /* 作業変数 */

    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
            re[j] =  data[i][j];
            im[j] = jdata[i][j];
        }
        FFT1( re, im, num_of_data, flag );
        for ( j = 0; j < num_of_data; j ++ ){
             data[i][j] = re[j];
            jdata[i][j] = im[j];
        }
    }
    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
            re[j] =  data[j][i];
            im[j] = jdata[j][i];
        }
        FFT1( re, im, num_of_data, flag );
        for ( j = 0; j < num_of_data; j ++ ){
             data[j][i] = re[j];
            jdata[j][i] = im[j];
        }
    }
}
