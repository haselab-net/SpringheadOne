/* GA �ɂ���̈撊�o�̃v���O���� ext_face.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"myGA.h" 
#include"mypgm.h"

/* �摜�}�b�`���O�֌W�̒萔�E�ϐ� */
#define MIN_RATE      1.0   /* �z��ŏ��g��{�� */
#define MAX_RATE      3.0   /* �z��ő�g��{�� */
#define ANGLE_RANGE  60.0   /* �z���]�p�x -30�`30[deg] */
#define MIN_MATCHING_RATE 0.9 /* �z��ŏ��}�b�`���O�� */
#define MAX_GENERATION    300 /* �ő吢�㐔           */
#define MIN_RANGE   50  /* ������K���������Ȃ������͖��� */
#define PI  3.141592653589  /* �~���� */

void set_optimizing_task( )
/* ���摜���̊�摜�̒��o���C�^����ꂽ�ۑ�Ƃ��܂� */
{
    /* �e���v���[�g�摜 = image1[y][x]              */
    /* ����܂ތ��摜   = image2[y][x]              */
    /* �ʒu (x,y), �g��{�� rate, ��]�p�x angle    */
    /* �e8�r�b�g�Ƃ���                              */
    gene_size = 32;
}

void trans_from_genotype_to_parameters( int number, int *x, int *y, 
                                       double *rate, double *angle )
/* �̔ԍ� No.number �̈�`�q�^���p�����[�^�ɕϊ����� */
/* (x,y):�e���v���[�g�̒��S�Crate:�g��{���Cangle�F��]�p�x */
{
    int i; /* ���[�v�ϐ� */
    static unsigned char code[MAX_GENE_SIZE];

    /* �O���C�R�[�h����o�C�i���R�[�h�ւ̕ϊ� */
    for ( i = 0; i < gene_size; i ++ )
        code[i] = genotype[number][i];
    for ( i = 0; i < 7; i ++ ){
        if ( code[i] == 1 ) 
            code[i+1] = (unsigned char)(1 - code[i+1]);
        if ( code[i+8] == 1 ) 
            code[i+9] = (unsigned char)(1 - code[i+9]);
        if ( code[i+16] == 1 ) 
            code[i+17] = (unsigned char)(1 - code[i+17]);
        if ( code[i+24] == 1 ) 
            code[i+25] = (unsigned char)(1 - code[i+25]);
    }
    /* ��`�q�^���琮�� (x, y) �ւ̕ϊ� */
    *x = 0;   *y = 0;   *rate = 0.0;   *angle = 0.0;
    for ( i = 0; i < 8; i ++ ){
        *x = *x * 2 + code[i];    *y = *y * 2 + code[i+8];
        *rate = *rate * 2.0 + code[i+16];    
        *angle = *angle * 2.0 + code[i+24];
    }
    /* �R�[�h������ۂ̒l�ւ̕ϊ� */
    *x = (int)( *x / 255.0 * x_size2 );
    *y = (int)( *y / 255.0 * y_size2 );
    *rate = ( MAX_RATE - MIN_RATE ) * *rate / 255.0 + MIN_RATE;
    *angle = ANGLE_RANGE * *angle / 255.0 - ANGLE_RANGE / 2.0;
    if ( *angle < 0 ) *angle = 360.0 + *angle;
}

double fitness_value( int number, int flag )
/* set_optimizing_task( )�Ő�������Ă���� No.number  */
/* �̓K���x��Ԃ��i flag = 0 : �ʏ�C= 1 : �ŏI�摜�쐬�j*/
{
    int x, y;            /* ���W�p�ϐ� */
    int xp,yp,ix2,iy2;   /* ��Ɨp�ϐ� */
    int x_half, y_half;  /* �摜�T�C�Y�̔���   */
    double x1,y1,x2,y2;  /* ��ԕϊ��p�ϐ�     */
    double rate, angle;  /* �g��{���C��]�p�x */
    double sum;          /* �K���x�p��ƕϐ�   */
    double min_gray, max_gray; /* �̈�̍ŏ��E�ő�K���l */
    double _sin, _cos;   /* sin, cos �p�ϐ� */

    /* ��`�q�^���� (x,y) �� */
    trans_from_genotype_to_parameters( number, &xp, &yp, &rate, &angle );
    /* �K���x�̌v�Z */
    x_half = x_size1 / 2;    y_half = y_size1 / 2;
    sum = 0.0;    min_gray = MAX_BRIGHTNESS;    max_gray = 0;
    _sin = sin( angle * PI / 180.0 );  
    _cos = cos( angle * PI / 180.0 );
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            /* �e���v���[�g�̊e�_(x1,y1)�ɑ΂����ԕϊ� */
            x1 = x - x_half;    y1 = y - y_half;
            x1 = x1 * rate;     y1 = y1 * rate;
            x2 = _cos * x1 - _sin * y1;
            y2 = _sin * x1 + _cos * y1;
            x2 = x2 + xp;    y2 = y2 + yp;
            ix2 = (int)x2;  iy2 = (int)y2;  /* �摜��̑Ή��_ */
            /* �Ή��_�ł̊K���l�̍��������߂� */
            if ( 0<=ix2 && ix2<x_size2 && 0<=iy2 && iy2<y_size2 ){
                sum = sum + abs( image2[iy2][ix2] - image1[y][x] );
                if ( image2[iy2][ix2] > max_gray ) 
                    max_gray = image2[iy2][ix2];
                if ( image2[iy2][ix2] < min_gray )
                    min_gray = image2[iy2][ix2];
                if ( flag == 1 )
                    image2[iy2][ix2] = (unsigned char)(
                        MAX_BRIGHTNESS - image2[iy2][ix2]);
            } else sum = sum + MAX_BRIGHTNESS;
        }
    }
    /* ���܂蕽�R�ȗ̈�͌��Ƃ��Ȃ����߂̏��� */
    if ( max_gray - min_gray < MIN_RANGE ) sum = 0.1;
    else sum = 1.0 - sum / ( x_size1 * y_size1 ) / MAX_BRIGHTNESS;
    return sum;
}

void calculate_fitness( )
/* �S���̓K���x�����߂� */
{
    int i; /* ���[�v�ϐ� */
    
    for ( i = 0; i < pop_size; i ++ )
        fitness[i] = fitness_value( i, 0 );
}

void display_elitest( int generation )
/* �ŗǌ̂̈�`�q�^�C�K���x����ʂɕ\������ */
{
    int i; /* ���[�v�ϐ� */

    printf("No.%d�F",generation);
    for ( i= 0; i < gene_size; i ++ ){
        printf("%d",elite_genotype[i]);
        if ( (i+1) % 8 == 0 ) printf(" ");
    }
    printf("--> %f\n", elite_fitness);
}

void generation_iteration()
/* ��������s���֐� */
{
    int generation; /* ���㐔 */
    int x, y;       /* �ŗǌ̂̍��W */
    double rate, angle, f; /* ��ƕϐ� */

    generation = 1;
    calculate_fitness( );    /* �K���x�̌v�Z   */
    find_and_set_best_individual( ); /* �G���[�g�̌��o */
    do{
        generation ++;
        selection_using_roulette_rule( ); /* ���[���b�g���[�� */
        execute_crossover( ); /* ���F�̂̌��� */
        execute_mutation( ); /* ��`�q�̓ˑR�ψ� */
        calculate_fitness( ); /* �K���x�̌v�Z */
        if ( elite_flag == 1 ) elitist_strategy( ); /* �G���[�g�ۑ� */
        find_and_set_best_individual(); /* �G���[�g�̌��o */
        display_elitest( generation ); /* �ŗǌ̂̕\�� */
    } while ( elite_fitness < MIN_MATCHING_RATE && 
              generation < MAX_GENERATION );
    trans_from_genotype_to_parameters( elite_number, &x, &y, &rate, &angle );
    printf("�ŏI�I�ȉ��F(x, y) = (%d, %d), rate = %f, angle = %f\n",
        x,y,rate,angle);
    /* ���o���ʂ̉摜����� */
    f = fitness_value( elite_number, 1 ); /* �ŗǌ̗̂̈�𔽓] */
}

void move_image1_to_image2( )
/* �摜 image1 �� image2 �ֈړ����� */
{
    int x, y; /* ����ϐ� */

    for ( y=0; y<y_size1; y++ )
        for ( x=0; x<x_size1; x++ )
            image2[y][x] = image1[y][x];
    x_size2 = x_size1;    y_size2 = y_size1;
}

main( )
{
    initialize_random_number( );  /* �����̏�����       */
    set_optimizing_task( );       /* ���̐ݒ�         */
    initialize_parameters( );     /* GA�p�����[�^�̐ݒ� */
    initialize_genes( );          /* ��`�q�^�̏�����   */
    printf("=== ��摜���܂ތ��摜��ǂݍ��݂܂� ===\n");
    load_image_data( );           /* ���摜�̓ǂݍ���   */
    move_image1_to_image2( );
    printf("=== ��̃e���v���[�g�摜��ǂݍ��݂܂� ===\n");
    load_image_data( );           /* �e���v���[�g�ǂݍ��� */
    generation_iteration( );      /* ������ */
    printf("=== ��̒��o���ʂ̉摜��ۑ����܂� ===\n");
    save_image_data( );           /* ���ʂ̕ۑ� */
    return 0;
}
