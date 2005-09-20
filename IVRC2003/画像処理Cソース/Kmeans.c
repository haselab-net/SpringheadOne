/* K-���ϖ@�ɂ��_�̃N���X�^�����O�̃v���O���� Kmeans.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"

#define PI  3.141592653589  /* �~���� */
#define MAX_PNTS   3000     /* �z�肷��ő�\���_��    */
#define MAX_CLSTR  3000     /* �z�肷��ő�N���X�^��  */
#define DARK_PIXEL  128     /* �Â���f�̊K���l */

int points[MAX_PNTS][2];        /* �_�̍��W                */
int num_pnts;                   /* �_�̑���                */
int point_clstr[MAX_PNTS];      /* �_��������N���X�^�ԍ�  */

int center_clstr[MAX_CLSTR][2]; /* �N���X�^�̑�\�_�̍��W  */
int num_clstr;                  /* �N���X�^�̑���          */

void draw_a_circle( int x, int y, int r )
/* image2[y][x] �ɒ��S (x,y) ���a r �̉~��`���D�S���ōő�   */
/* 360 �̓_��`�����C�傫���~�̏ꍇ�͓_���ɂȂ邱�Ƃ�����D*/
{
    int theta;     /* �p�x [deg] �̐���ϐ�     */
    int xp, yp;    /* �`�悷��_ (xp,yp) �̍��W */

    for ( theta = 0; theta < 360; theta ++ ){
        xp = (int)( x + r * cos( theta * PI / 180.0 ) );
        yp = (int)( y + r * sin( theta * PI / 180.0 ) );
        if ( xp >= 0 && xp < x_size2 && yp >= 0 && yp < y_size2 )
            image2[ yp ][ xp ] = DARK_PIXEL;  /* �Â���f */
    }
}

double calc_distance( int x1, int y1, int x2, int y2 )
/* (x1,y1),(x2,y2) �Ԃ̋�����Ԃ��֐� */
{
    return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

void draw_a_straight_line( int x1, int y1, int x2, int y2 )
/* image2[y][x] ���(x1,y1)�C(x2,y2)�Ԃ̒����𔭐�������D */
{
    double distance, step, t;   /* t: �����_�����p����ϐ� */
    int x, y;  /* �`���_�̍��W */

    distance = calc_distance( x1, y1, x2, y2 ); /* 2�_�Ԃ̋��� */
    step = 1.0 / ( 1.5 * distance );  /* ��������������_�̑��� */
    t = 0.0;
    while ( t < 1.0 ){
        x = (int)( t * x2 + ( 1.0 - t ) * x1 );
        y = (int)( t * y2 + ( 1.0 - t ) * y1 );
        if ( x >= 0 && x < x_size2 && y >= 0 && y < y_size2 )
            image2[y][x] = DARK_PIXEL;  /* �Â��_ */
        t = t + step;
    }
}

int random_int( int n )
/* 0 �ȏ� n - 1 �ȉ��̐����̗�����Ԃ� */
{
    return (int)( (double)rand( ) / ( (double)RAND_MAX + 1.0 ) * n );
}

void obtain_points_data( )
/* image1[y][x] ���̔���f�̃f�[�^�� points[ ][ ]�ɑ�� */
{
    int x, y;   /* ���[�v�ϐ� */

    num_pnts = 0;
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            if ( image1[y][x] == MAX_BRIGHTNESS ){
                if ( num_pnts < MAX_PNTS - 2 ){
                    num_pnts ++;
                    points[num_pnts - 1][0] = x;
                    points[num_pnts - 1][1] = y;
                }
            }
        }
    }
    printf("�S���� %d �_�̃f�[�^���擾���܂����D\n", num_pnts);
    if ( num_pnts < 5 ){
        printf("�_�񂪏��ȉ߂��܂��D�摜����蒼���ĉ������D\n");
        exit( 1 );
    }
}

void shuffle_points_data( )
/* �擾�����_�f�[�^�������_���ɓ���ւ���D */
{
    int seed;              /* �����̃V�[�h                 */
    int i, j, num1, num2;  /* ����ϐ��C����ւ���ԍ�1�C2 */
    int temp[2];           /* ����ւ��ɗp�����ƕϐ�     */
    int max;    /* ����ւ��� */

    /* �����̏����� */
    printf("�����̌n������������܂��D\n");
    printf("�����̃V�[�h�i�C�ӂ̐��̐����j:");
    scanf("%d",&seed);
    if ( seed < 1 ) seed = 1;
    srand( seed );

    /* �_�̑����̂Q�{�̉񐔂̓���ւ����� */
    max = num_pnts * 2;
    for ( i = 0; i < max; i ++ ){
        num1 = random_int( num_pnts );
        do { 
            num2 = random_int( num_pnts );
        } while ( num2 == num1 );
        /* No.num1 �� No.num2 �̓_�����ւ��܂� */
        for ( j = 0; j < 2; j ++ ){
            temp[j]         = points[num1][j];
            points[num1][j] = points[num2][j];
            points[num2][j] = temp[j];
        }
    }
}

void clustering_using_Kmeans( )
/* K-���ϖ@�ɂ��N���X�^�����O���s�� */
{
    int K_number;       /* K �̒l�i��������N���X�^�̑����j */
    int y, x, i, j;                 /* ����ϐ��Ȃ�         */
    double min_distance, distance;  /* �����Ɋւ���ϐ�     */
    int min_num;        /* �_�ɑ΂��čł��߂��N���X�^�̔ԍ� */
    double sum_x, sum_y, sum_number;  /* �N���X�^�̍�ƕϐ� */
    int init_cntr_clstr[MAX_PNTS][2]; /* �N���X�^�̏����ʒu */
    int radius_clstr[MAX_PNTS];   /* �N���X�^�̈Ӗ���̔��a */
    int finish;                   /* �����I���Ɋւ���t���O */
    int counter;                  /* ������               */

    printf("K-���ϖ@�ɂ���ē_����N���X�^�����O���܂��D\n");
    printf("K�i�N���X�^�����j�̒l����͂��ĉ������D\n");
    do {
        printf("�i 1 �ȏ� %d �ȉ� �j: ", num_pnts / 2);
        scanf("%d",&K_number);
    } while ( K_number < 1 || K_number > ( num_pnts / 2.0 ) );
    printf("�f�[�^�𕪗ނ��ăN���X�^���`�����C���ʂ�\n");
    printf("�摜�Ƃ��č쐬���܂��D\n");
    /* �ԍ��̓����_�����ɂ���Ă���̂ŁCNo.0�`K_number-1 */
    /* �̓_����ŏ��� K_number �̃N���X�^���S�ɐݒ肷�� */
    num_clstr = K_number;
    for ( i = 0; i < num_clstr; i ++ )
        for ( j = 0; j < 2; j ++ )
            center_clstr[i][j] = points[i][j];
    finish = 0;    counter = 0;
    while ( ! finish ){
        counter ++;  
        printf("%d ��ڂ̃N���X�^�����O���ł��D\n", counter );
        /* �N���X�^�̏�����\�_�ւ̑�� */
        for ( i = 0; i < num_clstr; i ++ ){
            for ( j = 0; j < 2; j ++ )
                init_cntr_clstr[i][j] = center_clstr[i][j];
            radius_clstr[i] = 10; /* defalt �l */
        }
        /* �e�_�̏����N���X�^�ԍ��̏����� */
        for ( i = 0; i < num_pnts; i ++ )
            point_clstr[i] = - 1;
        /* �e�_�̏����N���X�^�����肷�� */
        for ( i = 0; i < num_pnts; i ++ ){
            /* �e�N���X�^��\�_�Ƃ̋��������߂� */
            min_distance = 10000.0;
            for ( j = 0; j < num_clstr; j ++ ){
                distance = calc_distance( points[i][0], points[i][1],
                           center_clstr[j][0], center_clstr[j][1] );
                if ( distance < min_distance){
                    min_distance = distance;
                    min_num = j;
                }
            }
            /* No.i �̓_���N���X�^ No.min_num �֑������� */
            point_clstr[i] = min_num;
        }
        /* �e�N���X�^�̑�\�_���C������ */
        for ( i = 0; i < num_clstr; i ++ ){
            sum_x = 0.0;    sum_y = 0.0;
            sum_number = 0.0;
            for ( j = 0; j < num_pnts; j ++ ){
                if ( point_clstr[j] == i ){
                    sum_number = sum_number + 1.0;
                    sum_x = sum_x + points[j][0];
                    sum_y = sum_y + points[j][1];
                }
            }
            center_clstr[i][0] = (int)( sum_x / sum_number );
            center_clstr[i][1] = (int)( sum_y / sum_number );
        }
        /* �e�N���X�^�̈Ӗ���̔��a�����߂� */
        for ( i = 0; i < num_clstr; i ++ ){
            for ( j = 0; j < num_pnts; j ++ ){
                if ( point_clstr[j] == i ){
                    distance = calc_distance( 
                        center_clstr[i][0], center_clstr[i][1],
                        points[j][0], points[j][1] );
                    if ( distance > radius_clstr[i] )
                        radius_clstr[i] = (int)distance;
                }
            }
        }
        /* �N���X�^���S���ړ��������ǂ����̃`�F�b�N */
        finish = 1;
        for ( i = 0; i < num_clstr; i ++ ){
            distance = calc_distance( 
                init_cntr_clstr[i][0], init_cntr_clstr[i][1],
            center_clstr[i][0],    center_clstr[i][1] );
            if ( distance > 1.0 ) finish = 0;  /* ���e�덷 = 1.0 */
        }
        if ( finish == 1 ){  /* �����I�� */ 
            /* image2[y][x] �̏����� */
            x_size2 = x_size1;  y_size2 = y_size1;
            for ( y = 0; y < y_size2; y ++ )
                for ( x = 0; x < x_size2; x ++ )
                    image2[y][x] = image1[y][x];
            /* �N���X�^���S�Ɗe�_�f�[�^�܂ł̐�����`��   */
            for ( i = 0; i < num_pnts; i ++ ){
                draw_a_straight_line( points[i][0], points[i][1],
                    center_clstr[ point_clstr[i] ][0],
                    center_clstr[ point_clstr[i] ][1] );
            }
            /* image2[y][x] �ɃN���X�^���S�Ɖ~��`��      */
            /* �~�̔��a�����̃N���X�^�ɏ�������ł������_ */
            /* �܂ł̋���(=radius_clstr[ ]) + 5           */
            for ( i = 0; i < num_clstr; i ++ ){
                printf("No.%d �̃N���X�^�̔��a = %d\n", 
                    i + 1, radius_clstr[i] );
                draw_a_circle( center_clstr[i][0],
                    center_clstr[i][1], 5 );
                draw_a_circle( center_clstr[i][0],
                    center_clstr[i][1], radius_clstr[i] + 5 );
            }
            /* image2[y][x] �Ɍ��̃f�[�^�_���ĕ`�悷�� */
            for ( i = 0; i < num_pnts; i ++ )
                image2[ points[i][1] ][ points[i][0] ] =
                    MAX_BRIGHTNESS;
        }
    }
    printf("K-���ϖ@�ɂ��N���X�^�����O���I�����܂����D\n");
}

main( )
{
    load_image_data( );           /* �摜�t�@�C�� --> image1  */
    obtain_points_data( );        /* image1���̓_���f�[�^��   */
    shuffle_points_data( );       /* �f�[�^�����̃����_����   */
    clustering_using_Kmeans( );   /* K-���ϖ@�ŃN���X�^�����O */
                                  /* ����image2�֕`�悷��     */
    save_image_data( );           /* image2 --> �t�@�C���o��  */
    return 0;
}
