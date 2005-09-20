/* pgm ファイルを読み書きするためのヘッダファイル mypgm.h */

/* 定数宣言 */
#define MAX_IMAGESIZE  1024 /* 想定する縦・横の最大画素数 */
#define MAX_BRIGHTNESS  255 /* 想定する最大階調値 */
#define GRAYLEVEL       256 /* 想定する階調数(=最大階調値+1) */
#define MAX_FILENAME    256 /* 想定するファイル名の最大長 */
#define MAX_BUFFERSIZE  256 /* 利用するバッファ最大長 */

/* 大域変数の宣言 */
/* 画像用配列１，画像用配列２ */
unsigned char image1[MAX_IMAGESIZE][MAX_IMAGESIZE],
              image2[MAX_IMAGESIZE][MAX_IMAGESIZE];
int x_size1, y_size1, /* image1 の横画素数，縦画素数 */
    x_size2, y_size2; /* image2 の横画素数，縦画素数 */

/* 関数のプロトタイプ宣言 */
void load_image_data( ); /* 画像読み込み用関数 */
void save_image_data( ); /* 画像書き込み用関数 */


/* 以下は関数の本体 */

void load_image_data( )
/* pgm 画像，横画素数，縦画素数のデータをファイルから読み込み，*/
/* image1[ ][ ]，x_size1，y_size1 にそれぞれ代入する．         */
{
    char file_name[MAX_FILENAME]; /* ファイル名用の文字配列 */
    char buffer[MAX_BUFFERSIZE];  /* データ読み込み用作業変数 */
    FILE *fp; /* ファイルポインタ */
    int max_gray; /* 最大階調値 */
    int x, y; /* ループ変数 */

    /* 入力ファイルのオープン */
    printf("-----------------------------------------------------\n");
    printf("  モノクロ階調画像入力ルーチン\n");
    printf("-----------------------------------------------------\n");
    printf("ファイル形式は pgm, バイナリ形式とします．\n");
    printf("入力ファイル名 (*.pgm) : ");
    scanf("%s",file_name);
    fp = fopen( file_name, "rb" );
    if ( NULL == fp ){
        printf("その名前のファイルは存在しません．\n");
        exit(1);
    }
    /* ファイルタイプ(=P5)の確認 */
    fgets( buffer, MAX_BUFFERSIZE, fp );
    if ( buffer[0] != 'P' || buffer[1] != '5' ){
        printf("ファイルのフォーマットが P5 とは異なります．\n");
        exit(1);
    }
    /* x_size1, y_size1 の代入（#から始まるコメントは読み飛ばす） */
    x_size1 = 0;
    y_size1 = 0;
    while ( x_size1 == 0 || y_size1 == 0 ){
        fgets( buffer, MAX_BUFFERSIZE, fp );
        if ( buffer[0] != '#' ){
            sscanf( buffer, "%d %d", &x_size1, &y_size1 );
        }
    }
    /* max_gray の代入（#から始まるコメントは読み飛ばす） */
    max_gray = 0;
    while ( max_gray == 0 ){
        fgets( buffer, MAX_BUFFERSIZE, fp );
        if ( buffer[0] != '#' ){
            sscanf( buffer, "%d", &max_gray );
        }
    }
    /* パラメータの画面への表示 */
    printf("横の画素数 = %d, 縦の画素数 = %d\n", x_size1, y_size1 );
    printf("最大階調値 = %d\n",max_gray);
    if ( x_size1 > MAX_IMAGESIZE || y_size1 > MAX_IMAGESIZE ){
        printf("想定値 %d x %d を超えています．\n", 
            MAX_IMAGESIZE, MAX_IMAGESIZE);
        printf("もう少し小さな画像を使って下さい．\n");
        exit(1);
    }
    if ( max_gray != MAX_BRIGHTNESS ){
        printf("最大階調値が不適切です．\n");
        exit(1);
    }
    /* 画像データを読み込んで画像用配列に代入する */
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            image1[y][x] = (unsigned char)fgetc( fp );
        }
    }
    printf("データは正しく読み込まれました．\n");
    printf("-----------------------------------------------------\n");
    fclose(fp);
}

void save_image_data()
/* image2[ ][ ], x_size2, y_size2 のデータを，それぞれ pgm 画像，*/
/* 横画素数，縦画素数としてファイルに保存する．                  */
{
    char file_name[MAX_FILENAME]; /* ファイル名用の文字配列 */
    FILE *fp; /* ファイルポインタ */
    int x, y; /* ループ変数 */

    /* 出力ファイルのオープン */
    printf("-----------------------------------------------------\n");
    printf("  モノクロ階調画像（pgm形式）出力ルーチン\n");
    printf("-----------------------------------------------------\n");
    printf("出力ファイル名 (*.pgm) : ");
    scanf("%s",file_name);
    fp = fopen(file_name, "wb");
    /* ファイル識別子 "P5" を先頭に出力する */
    fputs( "P5\n", fp );
    /* # で始まるコメント行（省略可能） */
    fputs( "# Created by Image Processing\n", fp );
    /* 画像の横幅，縦幅の出力 */
    fprintf( fp, "%d %d\n", x_size2, y_size2 );
    /* 最大階調値の出力 */
    fprintf( fp, "%d\n", MAX_BRIGHTNESS );
    /* 画像データの出力 */
    for ( y = 0; y < y_size2; y ++ ){
        for ( x = 0; x < x_size2; x ++ ){
            fputc( image2[y][x], fp );
        }
    }
    printf("データは正しく出力されました．\n");
    printf("-----------------------------------------------------\n");
    fclose(fp);
}
