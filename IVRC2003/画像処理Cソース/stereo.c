/* �X�e���I�}�b�`���O�̃v���O���� stereo.c */
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include"mypgm.h"

void copy_image1_to_image2( )
/* image1 �� image2 �փR�s�[���� */
{
    int x, y; /* ���[�v�ϐ� */

    x_size2 = x_size1;    y_size2 = y_size1;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            image2[y][x] = image1[y][x];
}

void determine_corresponding_point( int xL, int yL, int *xR, int *yR )
/* ���摜��(xL,yL)�̉E�摜��̑Ή��_(*xR,*yR)�����肷�� */
{
    /* �e���v���[�g�Ɋւ���ϐ� */
    static unsigned int tmp_image[100][100]; /* �ő�100x100��f */
    int x_tmp    =   5;         /* x �����T�C�Y [-x_tmp, x_tmp] */
    int y_tmp    =   3;         /* y �����T�C�Y [-y_tmp, y_tmp] */
    int x_offset = 128;        /* x �����T���͈� [-x_offset, 0] */
      /* ���摜����ɂ���Ǝ����͕��Ȃ̂Ő������ɂ͓������Ȃ� */
    int out_flag;  /* �e���v���[�g�̈ꕔ���O�ɏo��Ƃ�1, else 0 */
    int threshold_for_gray = 20;    /* �e���v���[�g���̊K������ */
      /* ���̒l�ȉ��̏ꍇ�͐M�������Ⴂ�̂őΉ��_���o���Ȃ�     */
    int y_offset =  1;  /* y �����T���͈� [-y_offset, y_offset] */
      /* ���E�J�����̏㉺�̂Ԃ���l�����Ȃ��Ƃ��� y_offset = 0  */
    int i, j, m, n, xp, yp, xs, ys;     /* ����ϐ��C��Ɨp�ϐ� */
    int num_of_temp = 0;              /* �e���v���[�g���̉�f�� */
    double sum, min_error;            /* �덷�̘a�C�덷�̍ŏ��l */
    double error_ratio;               /* �덷���i�O�ȏ�P�ȉ��j */
    double min_gray, max_gray;/* �e���v���[�g���̍ŏ��C�ő�K�� */
      /*  (xL,yL) �𒆐S�Ƃ���e���v���[�g tmp_image �̐؂�o�� */

    num_of_temp = 0;                  /* �e���v���[�g���̉�f�� */
    min_gray = 255.0;     /* �e���v���[�g���̍ŏ��K���l�̏����� */
    max_gray = 0.0;       /* �e���v���[�g���̍ő�K���l�̏����� */
    out_flag = 0;     /* �e���v���[�g�͉摜���ɂ���Ƃ��ď����� */
    for ( i = - y_tmp; i <= y_tmp; i ++ ){
        for ( j = - x_tmp; j <= x_tmp; j ++ ){
            xp = xL + j;     yp = yL + i;
            if ( xp<0 || xp>x_size1 || yp<0 || yp>y_size1 )
                out_flag = 1;     /* �摜�O */
            else {
                num_of_temp ++ ;
                tmp_image[i+y_tmp][j+x_tmp] = image1[yp][xp];
                if ( image1[yp][xp] < min_gray )
                        min_gray = image1[yp][xp];
                else if ( image1[yp][xp] > max_gray )
                        max_gray = image1[yp][xp];
            }
        }
    }
    if ( ( max_gray - min_gray ) <= threshold_for_gray ||
    out_flag == 1 ){
        *xR = xL - 1;    *yR = yL;
        /* �e���v���[�g���̊K���ω������܂�Ȃ��ꍇ�C�܂��� */
        /* �e���v���[�g�̈ꕔ���摜�̊O�ɏo�Ă��܂��Ƃ��� */
        /* �������P�i�����j�ɐݒ肷�� */
    } else {
        /* �T���_�̌��� */
        min_error = 10000.0; /* �ŏ��덷�̏����� */
        for ( m = - y_offset; m <= y_offset; m ++ ){
            for ( n = - x_offset; n < 0; n ++ ){
                xs = xL + n;     ys = yL + m;
                /* (xs,ys)��Ή��_�Ƃ����Ƃ��̌덷�̌v�Z */
                if ( xs>=0 && xs<x_size1 && ys>=0 && ys<y_size1 ){
                    sum = 0.0;  /* �덷�̑��a�̏����� */
                    /* �}�b�`���O���s�� */
                    for ( i = - y_tmp; i <= y_tmp; i ++ ){
                        for ( j = - x_tmp; j <= x_tmp; j ++ ){
                            xp = xs + j;    yp = ys + i;
                            if ( xp>=0 && xp<x_size1 && yp>=0 &&
                            yp<y_size1 ){
                                sum = sum + 
                                    abs( tmp_image[i+y_tmp][j+x_tmp] 
                                    - image2[yp][xp] );
                           /* �덷���e��f�̊K���l�̍����̐�Βl */
                            }
                        }
                    }
                    error_ratio = sum / num_of_temp;   /* �덷�� */ 
                    if ( error_ratio <= min_error ){
                      /* �덷����������� n �� 0 �ɋ߂��قǗD�� */
                        min_error = error_ratio;    
                        *xR = xs;   *yR = ys;
                    }
                }
            }
        }
    }
}

void calc_range_data( int xL, int yL, int xR, int yR, double d, double f, 
                     double p_to_mm, double *xp, double *yp, double *zp )
/* ���摜��(xL,yL)�ƉE�摜��(xR,yR)���Ή�����Ƃ��̌v���_ */
/* (*xp, *yp, *zp)�̍��W���v�Z����Dd�F���E�����Y�Ԃ̋��� */
/* f�F�����Y���S�Ɖ摜�ʂƂ̊Ԃ̋����i�����Y�̏œ_�����j  */
{
    double disparity; /* ���� */

    xL = xL - (int)(x_size1 / 2.0);      /* x ���W�ϊ� */
    yL = - ( yL - (int)(y_size1 / 2.0)); /* y ���W�ϊ� */ 
    xR = xR - (int)(x_size1 / 2.0);      /* x ���W�ϊ� */
    yR = - ( yR - (int)(y_size1 / 2.0)); /* y ���W�ϊ� */ 
    disparity = ( xL - xR ) * p_to_mm;   /*  ���� [mm] */
    *xp = d * xL / disparity;           /* x ���W [mm] */
    *yp = d * yL / disparity;           /* y ���W [mm] */
    *zp = f * d  / disparity;           /* z ���W [mm] */
}

void stereo_matching( )
/* image1�F���摜�Cimage2�F�E�摜 �̊Ԃ̃X�e���I�}�b�`���O�� */
/* �s���D���ʂ� range[y][x] �ɑ����C�摜�ɒ�����image2 ��  */
/* �������D                                                */
{
    double d = 100.0;             /* ���E�J�����Ԃ̋��� [mm] */
    double f =  10.0;             /* �����Y�̏œ_���� [mm]   */
    double pixel_to_mm = 0.02;   /* ��f��[mm]�ɕϊ�����Ƃ� */
         /* �Ɋ|����W���D����̓J�����Ɉˑ�����̂ŁC���ʂ� */
         /* ���������Ƃ��ɏ����ς��Ă݂Ăق����D             */
    int min_range = 100;          /* �z��ŏ����� [mm]       */
    int max_range = 600;          /* �z��ő勗�� [mm]       */
    static int range[MAX_IMAGESIZE][MAX_IMAGESIZE];  /* ���� */
    int xL, yL, xR, yR;      /* ���摜�C�E�摜�̑Ή�������W */
    double xp, yp, zp;       /* �v���_��(x,y,z)��΍��W      */
    int min_dist, max_dist;  /* �v�����������̍ŏ��C�ő�l   */

    /* �����f�[�^�p�z��̏����� */
    for ( yL = 0; yL < y_size1; yL ++ )
        for ( xL = 0; xL < x_size1; xL ++ )
            range[yL][xL] = 0;
    /* �����J�n */
    printf("���摜����ɂ��ăX�e���I�}�b�`���O���s���܂��D\n");
    for ( yL = 0; yL < y_size1; yL ++ ){
        printf("line %d\n",yL);
        for ( xL = 0; xL < x_size1; xL ++ ){
            determine_corresponding_point( xL, yL, &xR, &yR );
            calc_range_data( xL, yL, xR, yR, d, f, pixel_to_mm, 
                             &xp, &yp, &zp );
            range[yL][xL] = (int)zp;
        }
    }

    /* �ȍ~�͌v�����ʂ��摜�ɂ��邽�߂̌㏈���ł���D */

    /* �v������������͈� [min_range, max_range] ���ɓ���� */
    for ( yL = 0; yL < y_size1; yL ++ ){
        for ( xL = 0; xL < x_size1; xL ++ ){
            if ( range[yL][xL] <= min_range )
                range[yL][xL] = min_range;
            if ( range[yL][xL] > max_range )
                range[yL][xL] = max_range;
        }
    }
    /* �����̍ŏ��C�ő�l�����߂� */
    min_dist = INT_MAX;    max_dist = INT_MIN;
    for ( yL = 0; yL < y_size1; yL ++ ){
        for ( xL = 0; xL < x_size1; xL ++ ){
            if ( range[yL][xL] < min_dist )
                min_dist = range[yL][xL]; 
            else if ( range[yL][xL] > max_dist )
                max_dist = range[yL][xL];
        }
    }
    /* �摜�ɒ��� */
    for ( yL = 0; yL < y_size1; yL ++ ){
        for ( xL = 0; xL < x_size1; xL ++ ){
            image2[yL][xL] = (unsigned char)(MAX_BRIGHTNESS - 
                MAX_BRIGHTNESS * ( range[yL][xL] - min_dist ) 
                / (double)( max_dist - min_dist ) );
            /* �߂����̂قǔ�����f�ɂ��� */
        }
    }
}

main( )
{
    printf("===== �X�e���I�}�b�`���O =====\n");
    printf("���E�摜��ǂݍ��݂܂��D\n");
    load_image_data( );           /* �E�摜 --> image1  */
    copy_image1_to_image2( );     /* image1 --> image2  */
    printf("�����摜��ǂݍ��݂܂��D\n");
    load_image_data( );           /* ���摜 --> image1  */
    stereo_matching( );     /* �X�e���I�}�b�`���O���s�� */
                           /* ���ʂ� image2 �ɋL�^     */
    save_image_data( );           /* image2 --> �ۑ�    */
    return 0;
}
