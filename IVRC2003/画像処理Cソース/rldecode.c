/* ランレングス復号化のプログラム rldecode.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

FILE *fp;                  /* データを入力するファイルポインタ */
int byte_index, bit_index; /* データ入力用バイト，ビット位置   */
unsigned char byte_data;   /* 読み込んだ１バイトのデータ用変数 */

unsigned char get_one_bit( )
/* ファイルから１ビットのデータを取り出す */
{
    unsigned char work;  /* 作業変数 */

    if ( bit_index == 9 ){
        byte_data = (unsigned char)fgetc( fp ); /* 1バイト読み込み */
        byte_index ++;    bit_index = 1;
    }
    work = (unsigned char)( byte_data << ( bit_index - 1 ) );
    work = (unsigned char)( work >> 7 );
    bit_index++;
    return work;
}

int get_runlength_data( )
/* １つのランレングスデータを読み込んで返す */
{
    unsigned char bit_data; /* 1ビット用変数 */
    int length; /* ラン長 */
    int i, n;   /* 制御・作業用変数 */

    /* ランレングスの長さを求める */
    length = 1;
    do{
        bit_data = get_one_bit( );  length ++;
    } while ( bit_data != 0 );
    /* ランレングスデータを求める */
    n = 0;
    for ( i = 1; i <= length; i ++ ){
        bit_data = get_one_bit( );
        n = n * 2 + bit_data;
    }
    n = n + 1;  /* = ランレングス */
    return n;
}

void load_and_decode_runlength_data( )
/* ランレングスデータをロードしてデコード化して画像に直す */
{
    int y, x, i;                  /* 座標用変数・制御変数 */
    char file_name[MAX_FILENAME]; /* 出力ファイル名       */
    int runlength;                /* ランレングス         */
    int white_flag;               /* = 1 :白, = else :黒  */

    printf("ランレングスデータをロードして画像に直します．\n");
    printf("入力ファイル名 (*.wyl) : ");
    scanf("%s", file_name);
    fp = fopen( file_name, "rb" );
    if ( NULL == fp ){
        printf("その名前のファイルは存在しません．\n");
        exit(1);
    }
    /* 画像の横幅，縦幅の出力 */
    fscanf( fp, "%d %d\n", &x_size2, &y_size2 );
    printf("画像の横画素数 = %d, 縦画素数 = %d\n", x_size2, y_size2);
    /* ランレングス復号化 */
    for ( y = 0; y < y_size2; y ++ ){
        byte_index = 0;  bit_index = 9;  /* 変数の初期化 */
        /* 最初の1ビットは最左の画素の黒／白フラグ */
        white_flag = get_one_bit( );
        /* 2ビット目以降の処理 */
        x = -1; /* x座標の初期化 */
        while ( x < x_size2 - 1){
            /* データを読み込んでラン長に変換 */
            runlength = get_runlength_data( );
            /* 画像（白／黒画素列）に直す */
            for ( i = 0; i < runlength; i ++ ){
                x ++;  
                if ( white_flag )
                    image2[y][x] = MAX_BRIGHTNESS; /* 白 */
                else image2[y][x] = 0; /* 黒 */
            }
            white_flag = ! white_flag; /* 白フラグの反転 */
        }
    }
    fclose( fp );
}

main( )
{
    load_and_decode_runlength_data( );  /* 読み込みとデコード */
    save_image_data( );                 /* 復号化画像の保存   */
    return 0;
}
