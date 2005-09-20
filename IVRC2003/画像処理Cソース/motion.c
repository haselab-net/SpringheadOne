/* ���x�x�N�g�������߂�v���O���� motion.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
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

void draw_vector( int x1, int y1, int vx, int vy )
/* image2[y][x] ��(x1,y1)�̑��x�x�N�g��(vx,vy)��`�悷��D */
{
    double distance, step, t;   /* t:�����_�����p����ϐ� */
    int x2, y2;  /* �I�_ */
    int x, y;    /* �`���_�̍��W */
    
    x2 = x1 + vx;    y2 = y1 + vy;
    distance = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
    step = 1.0 / ( 1.5 * distance );
    t = 0.0;
    while ( t < 1.0 ){
        x = (int)( t * x2 + ( 1.0 - t ) * x1 );
        y = (int)( t * y2 + ( 1.0 - t ) * y1 );
        if ( x >= 0 && x < x_size2 && y >= 0 && y < y_size2 )
            image2[y][x] = (unsigned char)MAX_BRIGHTNESS; 
        t = t + step;
    }
}

void calc_motion_vector( int x, int y, int *vx, int *vy )
/* image1�̓_(x,y)�̑��x�x�N�g��(*vx, *vy)�����肷��D*/
{
    /* �e���v���[�g�Ɋւ���ϐ� */
    static unsigned int tmp_image[100][100]; /* �ő�100x100��f */
    int x_tmp =  4;    /* x �����T�C�Y [-x_tmp, x_tmp] */
    int y_tmp =  4;    /* y �����T�C�Y [-y_tmp, y_tmp] */
    int x_offset = 30; /* x �����T���͈� [-x_offset, x_offset] */
    int y_offset = 30; /* y �����T���͈� [-y_offset, y_offset] */
    int threshold_for_gray = 30;    /* �e���v���[�g���̊K������ */
      /* ���̒l�ȉ��̏ꍇ�͐M�������Ⴂ�̂őΉ��_���o���Ȃ�     */

    int i, j, m, n, xp, yp, xs, ys;     /* ����ϐ��C��Ɨp�ϐ� */
    int num_of_temp;  /* �e���v���[�g���̉�f���i�摜�O�𖳎��j */
    double sum, min_error;            /* �덷�̘a�C�덷�̍ŏ��l */
    double error_ratio;               /* �덷���i�O�ȏ�P�ȉ��j */
    double min_gray, max_gray;  /* �e���v���[�g���ŏ��C�ő�K�� */

    /* �e���v���[�g�̈ꕔ���摜�O�ɏo��_�͑��x�O�ɂ��� */
    if ( x - x_tmp < 0 || x + x_tmp >= x_size1 ||
        y - y_tmp < 0 || y - y_tmp >= y_size1 ){
        *vx = 0;    *vy = 0;
    } else {
        /*  (x,y) �𒆐S�Ƃ���e���v���[�g tmp_image �̐؂�o�� */
        num_of_temp = ( 2 * x_tmp + 1 ) * ( 2 * y_tmp + 1);
        min_gray = 255;    max_gray = 0;   /* �ŏ��C�ő�K�� */
        for ( i = - y_tmp; i <= y_tmp; i ++ ){
            for ( j = - x_tmp; j <= x_tmp; j ++ ){
                xp = x + j;     yp = y + i;
                tmp_image[i+y_tmp][j+x_tmp] = image1[yp][xp];
                if ( image1[yp][xp] < min_gray )
                    min_gray = image1[yp][xp];
                else if ( image1[yp][xp] > max_gray )
                    max_gray = image1[yp][xp];
            }
        }
        if ( ( max_gray - min_gray ) <= threshold_for_gray ){
        *vx = 0;    *vy = 0;
      /* �e���v���[�g���̊K���ω������܂�Ȃ��ꍇ�͑��x=0 �ɂ��� */
        }
        else
        {
            /* �T���_�̌��� */
            min_error = 10000.0; /* �ŏ��덷�̏����� */
            for ( m = - y_offset; m <= y_offset; m ++ ){
                for ( n = - x_offset; n < x_offset; n ++ ){
                    xs = x + n;     ys = y + m;
                    /* (xs,ys)��Ή��_�Ƃ����Ƃ��̌덷�̌v�Z */
                    if ( xs - x_tmp >= 0 && xs + x_tmp < x_size1 &&
                         ys - y_tmp >= 0 && ys + y_tmp < y_size1 ){
                        sum = 0.0;  /* �덷�̑��a�̏����� */
                        /* �}�b�`���O���s�� */
                        for ( i = - y_tmp; i <= y_tmp; i ++ ){
                            for ( j = - x_tmp; j <= x_tmp; j ++ ){
                                xp = xs + j;    yp = ys + i;
                                sum = sum + 
                                    abs( tmp_image[i+y_tmp][j+x_tmp] 
                                    - image2[yp][xp] );
                           /* �덷���e��f�̊K���l�̍����̐�Βl */
                            }
                        }
                        error_ratio = sum / num_of_temp; /* �덷�� */ 
                        if ( error_ratio <= min_error ){
                            min_error = error_ratio;    
                            *vx = xs - x;   *vy = ys - y;
                        }
                    }
                }
            }
        }
    }
}

void block_matching( )
/* image1�F�摜1�Cimage2�F�摜2 �̊Ԃ̃u���b�N�}�b�`���O���s���D */
/* ���ʂ̑��x��̉摜�� vector[y][x] �ɑ����Cimage2 �ɃR�s�[   */
/* ���Ă���ۑ�����D                                            */
{
    /* x �����Ey�������x�x�N�g�� */
    int x_vctr[MAX_IMAGESIZE][MAX_IMAGESIZE];
    int y_vctr[MAX_IMAGESIZE][MAX_IMAGESIZE];
    int x, y; /* ����ϐ� */
    int vx, vy, step; /* ���x�x�N�g���C�x�N�g�������߂�_�̊Ԋu */

    do {
        printf("���x�x�N�g�����v�Z�����f�Ԃ̋���(2�`20) : ");
        scanf("%d", &step);
    } while ( step < 2 || step > 20 );
    /* ���x��̉摜�̏����� */
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ ){
            x_vctr[y][x] = 0;    y_vctr[y][x] = 0;
        }
    /* �����J�n */
    printf("�摜1 ����摜2 �ւ̑��x�x�N�g�������߂Ă��܂��D\n");
    for ( y = 0; y < y_size1; y ++ ){
        printf("line %d\n",y);
        if ( ( y % step ) == 0 ){
            for ( x = 0; x < x_size1; x ++ ){
                if ( ( x % step ) == 0 ){
                    calc_motion_vector( x, y, &vx, &vy );
                    x_vctr[y][x] = vx;    y_vctr[y][x] = vy;
                }
            }
        }
    }
    /* �v�����ʂ��摜�ɂ��� */
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = 0;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            if ( ( y % step ) == 0 && ( x % step ) == 0 )
                draw_vector( x, y, x_vctr[y][x], y_vctr[y][x] );
}

main( )
{
    printf("===== �����x�N�g���̌��o =====\n");
    printf("frame No.2 ��ǂݍ��݂܂��D\n");
    load_image_data( );         /* frame No.2 --> image1 */
    copy_image1_to_image2( );   /* image1 --> image2     */
    printf("frame No.1 ��ǂݍ��݂܂��D\n");
    load_image_data( );         /* frame No.1 --> image1 */
    block_matching( );          /* �����x�N�g�������߁C  */
                                /* ���ʂ� image2 �ɋL�^  */
    save_image_data( );         /* image2 --> �ۑ�       */
    return 0;
}
