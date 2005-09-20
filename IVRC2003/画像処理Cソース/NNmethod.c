/* NN �@�ɂ��_�̃N���X�^�����O�̃v���O���� NNmethod.c */
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

void clustering_using_NNmethod( )
/* NN(Nearest Neighbour �@�ɂ��N���X�^�����O���s�� */
{
    int threshold;  /* ����̃N���X�^���Ɋ܂߂鋗���̍ő�l */
    int y, x, i, j;                 /* ����ϐ��Ȃ�         */
    double min_distance, distance;  /* �����Ɋւ���ϐ�     */
    int min_num;                  /* �ł��߂��N���X�^�̔ԍ� */
    char str[10];                 /* ��Ɨp������           */

    do {         /* threshold ��ς��Ȃ��牽�x������ */
        printf("NN�@�ɂ���ē_����N���X�^�����O���܂��D\n");
        printf("�������l�i�N���X�^�̔��a�j: ");
        scanf("%d",&threshold);
        printf("�f�[�^�𕪗ނ��ăN���X�^���`�����C���ʂ�\n");
        printf("�摜�Ƃ��č쐬���Ă��܂��D\n");
        /* image2[y][x] �̏����� */
        x_size2 = x_size1;  y_size2 = y_size1;
        for ( y = 0; y < y_size2; y ++ )
            for ( x = 0; x < x_size2; x ++ )
                image2[y][x] = image1[y][x];
        /* No.0 �̓_�� No.0 �̃N���X�^��\�_�ɐݒ肷�� */
        num_clstr = 0;           /* �N���X�^�̑��� - 1 */
        for ( i = 0; i < 2; i ++ )
            center_clstr[num_clstr][i] = points[0][i];
            draw_a_circle( points[0][0], points[0][1], threshold );
            draw_a_circle( points[0][0], points[0][1], 5 );
            point_clstr[0] = 0;
            num_clstr ++;
        /* No.1 ���� No.num_pnts-1 �܂ł̓_�𕪗ނ��� */
        for ( i = 1; i < num_pnts; i ++ ){
            /* �ł��߂��N���X�^:No.min_num�Ƌ���:min_distance */
            min_distance = 10000.0;
            for ( j = 0; j < num_clstr; j ++ ){
                distance = calc_distance( points[i][0], points[i][1],
                        center_clstr[j][0], center_clstr[j][1] );
                if ( distance < min_distance ){
                    min_distance = distance;
                    min_num = j;
                }
            }
            if ( min_distance < threshold ){ 
                /* No.i �̓_���N���X�^ No.min_num �֑������� */
                point_clstr[i] = min_num;
            } 
            else
            {   /* No.i �̓_�͐V�����N���X�^�ɏ��������� */
            for ( j = 0; j < 2; j ++ )
                center_clstr[num_clstr][j] = points[i][j];
                draw_a_circle( points[i][0], points[i][1], threshold );
                draw_a_circle( points[i][0], points[i][1], 5 );
                point_clstr[i] = num_clstr;
                num_clstr ++;
            }
        }
        /* �N���X�^���S�Ɗe�_�f�[�^�܂ł̐�����`��   */
        for ( i = 0; i < num_pnts; i ++ ){
            draw_a_straight_line( points[i][0], points[i][1],
                center_clstr[ point_clstr[i] ][0],
                center_clstr[ point_clstr[i] ][1] );
        }
        /* image2[y][x] �Ɍ��̃f�[�^�_���ĕ`�悷�� */
        for ( i = 0; i < num_pnts; i ++ )
            image2[ points[i][1] ][ points[i][0] ] = MAX_BRIGHTNESS;

        printf("%d �̃N���X�^����������܂����D\n", num_clstr);
        printf("�����P���蒼���܂��� ( y / n ) : ");
        scanf("%s",str);
    } while ( str[0] != 'n' && str[0] != 'N' );
}

main( )
{
    load_image_data( );             /* �摜�t�@�C�� --> image1 */
    obtain_points_data( );          /* image1���̓_���f�[�^��  */
    shuffle_points_data( );         /* �f�[�^�����̃����_����  */
    clustering_using_NNmethod( );   /* NN�@�ŃN���X�^�����O���C*/
                                    /* ���ʂ�image2�֕`�悷��  */
    save_image_data( );             /* image2 --> �t�@�C���o�� */
    return 0;
}
