/* Hilditch �̍א����̃v���O���� thinning.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"
#define GRAY 128

int func_nc8( int *b )
/* ���ډ�f�̘A���x�i�S�A���j�����߂�֐� */
{
    int n_odd[4] = { 1, 3, 5, 7 }; /* ��̋ߖT�ԍ� */
    int i, j, sum, d[10]; /* ����ϐ��C�v�Z�r����������ϐ� */
    
    for ( i = 0; i <= 9; i ++ ){
        j = i;    if (i==9) j = 1;
        if ( abs( *( b + j ) ) == 1 ) d[i] = 1;
        else d[i] = 0;
    }
    sum = 0;
    for ( i = 0; i < 4; i ++ ){
        j = n_odd[i];
        sum = sum + d[j] - d[j] * d[j+1] * d[j+2];
    }
    return( sum );
}

void thinning( )
/* �Q�l�摜�̍א�������(image2[y][x] ���ŏ��͍�Ɨ̈�Ƃ��Ďg��)*/
/* �i  = MAX_BRIGHTNESS --> 1, GRAY --> -1, 0 --> 0 �Ƃ݂Ȃ��j  */
/* ���摜image1[y][x] ===> ������̉摜image2[y][x]             */
{
    int offset[9][2] = {{0,0},{1,0},{1,-1},{0,-1},{-1,-1},
           {-1,0},{-1,1},{0,1},{1,1} }; /* �ߖT��f�ւ̃I�t�Z�b�g�l */
    int n_odd[4] = { 1, 3, 5, 7 };      /* �ߖT�ԍ��i��j         */
    int px, py;                         /* �ߖT��f��(x,y)��΍��W  */
    int b[9];                           /* �������܂ނX�ߖT�̒l     */
    int condition[6];               /* �����P�`�U�F=1�̂Ƃ��g�����h */
    int counter;                    /* ���x���ɕω�����������f�̐� */
    int i, x, y, copy, sum;         /* ����ϐ��Ȃ�                 */
    int path;                       /* �摜�̑����񐔂�\���ϐ�     */

    printf("�Q�l�摜�̍א����������s���܂��D\n");
    /* ��Ɣz��̏����� */
    x_size2 = x_size1;    y_size2 = y_size1;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = image1[y][x];
    /* �����J�n */
    path = 1;
    do {
        printf("path : %d\n", path);
        counter = 0;
        for ( y = 0; y < y_size1; y ++ ){
            for ( x = 0; x < x_size1; x ++ ){
                /* (x,y)���܂ނX�ߖT�Ƀf�[�^�������� */
                for ( i = 0; i < 9; i ++ ){
                    b[i] = 0;
                    px = x + offset[i][0];    py = y + offset[i][1];
                    if ( px >= 0 && px < x_size1 && 
                         py >= 0 && py < y_size1 )
                         if ( image2[py][px] == MAX_BRIGHTNESS )
                             b[i] = 1; 
                         else if ( image2[py][px] == GRAY ) b[i] = -1;
                }
                for ( i = 0; i < 6; i ++ ) condition[i] = 0;
                
                /* �����P�F�}�`��f�ł��� */
                if ( b[0] == 1 ) condition[0] = 1;
                
                /* �����Q�F���E�_�ł��� */
                sum = 0;
                for ( i = 0; i < 4; i ++ )
                    sum = sum + 1 - abs( b[ n_odd[i] ] );
                if ( sum >= 1 ) condition[1] = 1;
                
                /* �����R�F�[�_���������Ȃ� */
                sum = 0;
                for ( i = 1; i <= 8; i ++ )
                    sum = sum + abs( b[i] );
                if ( sum >= 2 ) condition[2] = 1;
                
                /* �����S�F�Ǘ��_��ۑ����� */
                sum = 0;
                for ( i = 1; i <= 8; i ++ )
                    if ( b[i] == 1 ) sum++;
                if ( sum >= 1 ) condition[3] = 1;
                
                /* �����T�F�A������ۑ����� */
                if ( func_nc8( b ) == 1 ) condition[4] = 1;
                
                /* �����U�F�����Q�̐����̕Б��������폜���� */
                sum = 0;
                for ( i = 1; i <= 8; i ++ )
                    if ( b[i] != -1 ) sum ++;
                    else {
                        copy = b[i];    
                        b[i] = 0;
                        if ( func_nc8( b ) == 1 ) sum ++;
                        b[i] = copy;
                    }
                if ( sum == 8 ) condition[5] = 1;
                
                /* �ŏI���� */
                if ( condition[0] && condition[1] && condition[2] &&
                     condition[3] && condition[4] && condition[5] ){
                    image2[y][x] = GRAY; /* = -1 ��\���Ă��� */
                    counter ++;
                }
            } /* end of x */
        } /* end of y */
        if ( counter != 0 ){
            for ( y=0; y<y_size1; y++)
                for ( x=0; x<x_size1; x++)
                    if ( image2[y][x] == GRAY ) image2[y][x] = 0;
        }
        path ++;
    } while ( counter != 0 );
}

main( )
{
    load_image_data( ); /* �摜��ǂݍ���� image1 �� */
    thinning( );        /* Hilditch �̍א������s�� */
    save_image_data( ); /* image2 ��ۑ����� */
    return 0;
}
