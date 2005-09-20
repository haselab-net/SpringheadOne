/* Hough �ϊ��̃v���O���� Hough.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"

#define PI  3.141592653589  /* �~���� */
#define MAX_THETA   360  /* �Ǝ��̃T�C�Y�i0.5[deg]=�P��f�j*/
#define MAX_RHO     720  /* �ώ��̃T�C�Y                 */

void draw_a_straight_line( int x1, int y1, int x2, int y2 )
/* image2[y][x] ��̂Q�_(x1,y1)�C(x2,y2)�Ԃ̒�����̔z��v�f */
/* �̒l���C���N�������g����D */
{
    double distance, step, t;   /* t : �����_�����p����ϐ� */
    int x, y;  /* �`���_�̍��W */

    distance = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ); /* 2�_�Ԃ̋��� */
    step = 1.0 / ( 1.5 * distance );  /* ��������������_�̑��� */
    t = 0.0;
    while ( t < 1.0 ){
        x = (int)( t * x2 + ( 1.0 - t ) * x1 );
        y = (int)( t * y2 + ( 1.0 - t ) * y1 );
        if ( x >= 0 && x < x_size2 && y >= 0 && y < y_size2
            && image2[y][x] < MAX_BRIGHTNESS ){
            image2[y][x] ++;
        }
        t = t + step;
    }
}

int _calc_rho( double px, double py, double r_mg, int theta )
/* �ς��v�Z���郋�[�`���idraw_a_curve�Ŏg���Ă���j */
{
    return (int)(r_mg * ( px * cos( theta * PI / MAX_THETA ) 
               + py * sin( theta * PI / MAX_THETA ) ) + MAX_RHO / 2.0);
}

void draw_a_curve( double px, double py )
/* �ρ����Ecos�Ɓ{y�Esin�Ƃ̋Ȑ����C�z��image2[rho][theta]�ɕ`���D   */
{
    double rho_max;  /* �ώ������̍ő�l[��f] */
    double rho_c;    /* �ϕ����̌W�� */
    int theta;       /* �Ƃ̒P�ʂ�[deg]  */

    rho_max = 1.0 / 1.5 * x_size1 + 1.0 / 1.5 * y_size1;
    rho_c = MAX_RHO / 2.0 / rho_max;
        /* �f�[�^������W�ւ̕ϊ��W�� */
    for ( theta = 0; theta < MAX_THETA; theta ++ ){
        draw_a_straight_line( theta, _calc_rho( px, py, rho_c, theta),
            theta + 1, _calc_rho( px, py, rho_c, theta + 1) );
    }
}

void Hough_transform( )
/* ���摜 image1[y][x] �� Hough �ϊ����C���ʂ��ƃϕ��� */
/* image2[rho][theta] �ɑ������D�������ƃϕ��ʂ̉����E�c���� */
/* �傫���� MAX_THETA, MAX_RHO �ł��炩���ߌ��߂��Ă���D*/
/* �܂��CMAX_BRIGHTNESS(=255)�ȏ�̗ݐς͍l�����Ȃ��D */
{
    int x, y;

    /* �ƃϕ��ʂ̏����� */
    x_size2 = MAX_THETA;    y_size2 = MAX_RHO;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = 0;
    /* �ϊ������̎��s */
    printf("Hough �ϊ������s���ł��D\n");
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            if ( image1[y][x] == MAX_BRIGHTNESS ){
                draw_a_curve( (double)x, (double)y );
            }
        }
    }
}

main( )
{
    load_image_data( ); /* �摜��ǂݍ���� image1 �� */
    Hough_transform( ); /* Hough �ϊ����Č��ʂ� image2 �� */
    save_image_data( ); /* image2 ��ۑ����� */
    return 0;
}
