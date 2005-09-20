/* �֊s���ǐՂ̃v���O���� contour.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

#define MAX_CNTR  5000      /* �֊s���̑z��ő�\����f�� */
#define GRAY 128

int chain_code[MAX_CNTR];   /* �֊s���̔z�� */
int Freeman[8][2] = {   /* Freeman �̃`�F�[���R�[�h�̃I�t�Z�b�g�l */
        { 1, 0}, { 1, -1}, { 0, -1}, {-1, -1},
        {-1, 0}, {-1,  1}, { 0,  1}, { 1,  1} };

int obtain_contour( int x_start, int y_start )
/* �J�n�_(x_start, y_start)����֊s���ǐՂ����s���ăf�[�^���擾���� */
/* ���ϐ� unsigned char chain_code[MAX_CNTR] �ɑ�����C�֊s */
/* ���Ɋ܂܂��_�̑����inum�j��Ԃ��Dimage1[y][x]��Ώۂɂ���D*/
{
    int  x,  y;             /* �֊s����̌��݂̒��ډ�f�̍��W */
    int xs, ys;             /* ���ډ�f�̎��͂̒T���_�̍��W   */
    int code, num;          /* �֊s�_�̃`�F�[���R�[�h, ����   */
    int i, counter, detect; /* ����ϐ��Ȃ�                   */

    /* �Ǘ��_�̃`�F�b�N */
    counter = 0;
    for ( i = 0; i<8; i++ ){
        xs = x_start + Freeman[i][0];
        ys = y_start + Freeman[i][1];
        if ( xs >= 0 && xs <= x_size1 && ys >= 0 && ys <= y_size1
            && image1[ys][xs] == MAX_BRIGHTNESS ) counter ++;
    }
    if ( counter == 0 ) num = 1;  /* (x_start,y_start)�͌Ǘ��_ */
    else {
        /* �T���J�n */
        num = -1;   x = x_start;    y = y_start;    code = 0;
        do {
            detect = 0; /* ���̓_���݂����Ƃ��P�ɂ��� */
            /* �����T�������̌��� */
            code = code - 3;   if ( code < 0 ) code = code + 8;
            do{
                xs = x + Freeman[code][0];
                ys = y + Freeman[code][1];
                if ( xs >= 0 && xs <= x_size1 && ys >= 0 &&
                     ys <= y_size1 && 
                     image1[ys][xs] == MAX_BRIGHTNESS ){
                    detect = 1;  /* ���̓_�����o */
                    num ++ ;  
                    if ( num > MAX_CNTR ){
                        printf("�֊s���̉�f�� > %d\n",MAX_CNTR);
                        exit(1);
                    }
                    chain_code[ num ] = code;
                    x = xs;  y = ys;  
                }
                code ++;  if ( code > 7 ) code = 0;
            } while ( detect == 0 );
        } while ( x != x_start || y != y_start ); /* �J�n�_�̌��o�܂� */
        num = num + 2;  /* chain_code[ ]�̓Y�����Ƃ̂���̏C�� */
    }
    return( num );  /* �J�n�_������Ԃ� */
}

void remove_areas( )
/* �Q�l�摜�̗֊s���ǐՂ̊֐� obtain_contour( ) ��p���āC*/
/* ���͒����w�肵���l���������ȗ̈����菜���D         */
{
    int threshold;              /* ���͒��ɑ΂��邵�����l */
    int num, x,y,xs, ys, i;
    int fill_value;

    printf("�֊s���ǐՂ�p�����̈�̏������s���܂��D\n");
    printf("�����Ώۗ̈�̍ő���͒� = ");
    scanf("%d", &threshold);
    /* ������摜�̏����� */
    x_size2 = x_size1;    y_size2 = y_size1;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            image2[y][x] = 0;
    /* �����J�n */
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            if ( image1[y][x] == MAX_BRIGHTNESS ){  /* = �J�n�_ */
                num = obtain_contour( x, y );  /* �֊s���ǐ� */
                if ( num > threshold ) 
                    fill_value = MAX_BRIGHTNESS;
                else fill_value = GRAY;
                xs = x;  ys = y;
                image1[ys][xs] = 0;
                image2[ys][xs] = (unsigned char)fill_value;
                if ( num > 1 ){
                    for ( i = 0; i < num - 1; i ++ ){
                        xs = xs + Freeman[ chain_code[i] ][0];
                        ys = ys + Freeman[ chain_code[i] ][1];
                        image1[ys][xs] = 0;
                        image2[ys][xs] = (unsigned char)fill_value;
                    }
                }
            }
        }
    }
    /* �㏈�� */
    for ( y = 0; y < y_size2; y ++ ){
        for ( x = 0; x < x_size2; x ++ ){
            if ( image2[y][x] == GRAY ){
                image2[y][x] = 0;
                for ( i = 0; i < 8; i ++ ){
                    xs = x + Freeman[i][0];
                    ys = y + Freeman[i][1];
                    if ( xs >= 0 && xs <= x_size2 && 
                         ys >= 0 && ys <= y_size2 &&
                         image2[ys][xs] == MAX_BRIGHTNESS ) 
                        image2[y][x] = MAX_BRIGHTNESS;
                }
            }
        }
    }
}

main( )
{
    load_image_data( ); /* �摜��ǂݍ���� image1 �� */
    remove_areas( ); /* �֊s���ǐՂ��ď��̈���������Cimage2 �� */
    save_image_data( ); /* image2 ��ۑ����� */
    return 0;
}
