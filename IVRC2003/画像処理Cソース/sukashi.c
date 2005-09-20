/* �d�q�������̃v���O���� sukashi.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"FFT1.h"
#include"FFT2.h"
#include"mypgm.h"

#define RATIO  0.1;  /* ����������摜�Ɋ|����W���i���Áj */

double  data[FFT_MAX][FFT_MAX], data2[FFT_MAX][FFT_MAX];
                /* ���f�[�^�̎����� */
double jdata[FFT_MAX][FFT_MAX],jdata2[FFT_MAX][FFT_MAX];
                /* ���f�[�^�̋����� */
int num_of_data;          /* �f�[�^�̗v�f�ԍ��̍ő�l */

void make_data( )
/* image1[y][x] �̃f�[�^�� data[y][x], jdata[y][x] �ɑ������ */
{
    int i, j; /* ���[�v�ϐ� */

    num_of_data = x_size1;
    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
             data[i][j] = (double)image1[i][j];
            jdata[i][j] = 0.0;
        }
    }
}

void make_output_image( )
/* �f�[�^ data[y][x], jdata[y][x] �� image2[y][x] �ɒ��� */
{
    int x, y; /* ���[�v�ϐ� */

    x_size2 = num_of_data;    y_size2 = num_of_data;
    for ( y = 0; y < y_size2; y ++ ){
        for ( x = 0; x < x_size2; x ++ ){
            if ( data[y][x] < 0.0 ) image2[y][x] = 0;
            else if ( data[y][x] > 255.0 ) image2[y][x] = 255;
            else image2[y][x] = (unsigned char)data[y][x];
        }
    }
}

void copy_image_data( )
/* image1[y][x] ==> image2[y][x] �փf�[�^�R�s�[ */
{
    int i, j; /* ���[�v�ϐ� */

    for ( i = 0; i < y_size1; i ++ ){
        for ( j = 0; j < x_size1; j ++ ){
            image2[i][j] = image1[i][j];
        }
    }
    y_size2 = y_size1;  x_size2 = x_size1;
}

void copy_FFT_data( )
/* data, jdata ==> data2, jdata2 �̃f�[�^�R�s�[ */
{
    int i, j; /* ���[�v�ϐ� */

    for ( i = 0; i < num_of_data; i ++ ){
        for ( j = 0; j < num_of_data; j ++ ){
             data2[j][i] =  data[j][i];
            jdata2[j][i] = jdata[j][i];
        }
    }
}

void coding( )
/* ��摜�ƍ�������摜���w�肵�ē���������̉摜��ۑ����� */
{
    int i, j; /* ���[�v�ϐ� */

    printf("\n���@��������摜��ǂݍ��݂܂��D\n");
    load_image_data( );
    copy_image_data( );
    printf("\n���@��摜��ǂݍ��݂܂��D\n");
    load_image_data( );
    make_data( );
    FFT2( data, jdata, num_of_data, 1 );
    for ( i = 0; i < y_size1; i ++ ){
        for ( j = 0; j < x_size1; j ++ ){
            data[i][j] = data[i][j] + image2[i][j] / RATIO;
        }
    }
    FFT2( data, jdata, num_of_data, -1 ); /* �Q���� �tFFT �̎��s */
    make_output_image( );            /* ���g���f�[�^���摜�ɒ��� */
    printf("\n���@����������̉摜��ۑ����܂��D\n");
    save_image_data( );                        /* �摜��ۑ����� */
}

void decoding( )
/* ����������̉摜�Ɗ�摜���w�肵�Ē��o���ꂽ�摜��ۑ����� */
{
    int i, j; /* ���[�v�ϐ� */
    double d; /* ��ƕϐ� */

    printf("\n���@��摜��ǂݍ��݂܂��D\n");
    load_image_data( );
    make_data( );
    FFT2( data, jdata, num_of_data, 1 );
    copy_FFT_data( );
    printf("\n���@����������摜��ǂݍ��݂܂��D\n");
    load_image_data( );
    make_data( );
    FFT2( data, jdata, num_of_data, 1 );
    x_size2 = x_size1;  y_size2 = y_size1;
    for ( i = 0; i < y_size2; i ++ ){
        for ( j = 0; j < x_size2; j ++ ){
            d = ( data[i][j] - data2[i][j] ) * RATIO;
            if ( d < 0.0 ) image2[i][j] = 0;
            else if ( d > MAX_BRIGHTNESS ) 
                image2[i][j] = MAX_BRIGHTNESS;
            else image2[i][j] = (unsigned char)d;
        }
    } 
    printf("\n���@���o���ꂽ�摜��ۑ����܂��D\n");
    save_image_data( );     /* �摜��ۑ����� */
}

main( )
{
    int mode;

    printf("*** �d�q�������̃v���O���� ***\n");
    do {
        printf("������������(1) �^ �����������(-1) : ");
        scanf("%d",&mode); 
    } while ( mode != 1 && mode != -1 );
    if ( mode == 1 ) coding( );
    else decoding( );
    return 0;
}
