/* �摜�̃x�N�g���ʎq���̃v���O���� VQ.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"

/* �e��萔�̐錾 */
#define MAX_BLOCK  128       /* ���E�c�����̍ő�u���b�N��    */
#define MIN_BLOCK   10       /* ���E�c�����̍ŏ��u���b�N��    */
#define MAX_CLUSTERS 8192    /* ��\�x�N�g���̍ő�{��        */
#define BLOCK_SIZE   4       /* �z��u���b�N�T�C�Y 4 x 4 ��f */
#define DIMENSION   16       /* �x�N�g���̎����i�� 4 x 4 �j   */
#define MIN_DIFFERENCE  5.0
                  /* ��\�x�N�g���̈ړ��`�F�b�N�̍ۂ̂������l */

/* ���ϐ��̐錾 */
unsigned char vector[MAX_BLOCK][MAX_BLOCK][DIMENSION];
                    /* �e�u���b�N�̃x�N�g�� */
unsigned char main_vector[MAX_CLUSTERS][DIMENSION];
                    /* ��\�x�N�g�� */
int cluster[MAX_BLOCK][MAX_BLOCK];
                    /* �e�u���b�N�̏����N���X�^�ԍ� */
int x_block, y_block;  /* �������u���b�N���C�c�����u���b�N�� */

int random_int( int n )
/* 0 �ȏ� n - 1 �ȉ��̐����̗�����Ԃ� */
{
    return (int)( (double)rand( ) / ( (double)RAND_MAX + 1.0 ) * n );
}

void obtain_vector_data( )
/* image1[y][x] �� 4 x 4 ��f�̃u���b�N�ɕ������ăx�N�g���� */
{
    int x, y, xs, ys, i, j;   /* ����ϐ� */

    if ( ( x_size1 % BLOCK_SIZE != 0 ) || 
         ( y_size1 % BLOCK_SIZE != 0 ) ){
        printf("��f���� %d �̔{���ł͂���܂���D\n",BLOCK_SIZE);
        exit( 1 );
    }
    x_block = x_size1 / BLOCK_SIZE;    
    y_block = y_size1 / BLOCK_SIZE;
    for ( y = 0; y < y_block; y ++ ){
        for ( x = 0; x < x_block; x ++ ){
            xs = BLOCK_SIZE * x;    
            ys = BLOCK_SIZE * y;
            for ( i = 0; i < BLOCK_SIZE; i ++ ){
                for ( j = 0; j < BLOCK_SIZE; j ++ ){
                    vector[y][x][i*BLOCK_SIZE+j] =
                        image1[ys + i][xs + j];
                }
            }
        }
    }
}

void initialize_random_number( )
/* �����̏����� */
{
    unsigned int seed; /* �����̃V�[�h */

    /* �����̏����� */
    printf("�����̌n������������܂��D\n");
    printf("�����̃V�[�h�i�C�ӂ̐��̐����j:");
    scanf("%d",&seed);
    if ( seed < 1 ) seed = 1;
    srand( seed );
}

double calc_distance( int y, int x, int n )
/* [y][x]�u���b�N�̃x�N�g����No.n ��\�x�N�g���Ԃ̋����v�Z */
{
    int i;      /* ����ϐ� */
    double sum; /* ��ƕϐ� */

    sum = 0;
    for ( i = 0; i < DIMENSION; i ++ )
        sum = sum + ( vector[y][x][i] - main_vector[n][i] ) *
                    ( vector[y][x][i] - main_vector[n][i] );
    return( sqrt( sum ) );
}

void clustering_using_Kmeans( )
/* K-���ϖ@�ɂ��N���X�^�����O���s�� */
{
    int K_number; /* K �̒l�i��������N���X�^�̑����j*/
    int i, j, x, y, xs, ys; /* ����ϐ��Ȃ� */
    double min_distance, distance; /* �����Ɋւ���ϐ� */
    int min_num; /* �ł��߂��N���X�^�̔ԍ� */
    int finish; /* �����I���Ɋւ���t���O */
    int counter; /* ������ */
    unsigned char init_main_vector[MAX_CLUSTERS][DIMENSION];
                            /* ��\�x�N�g�� */
    double sum[DIMENSION], sum_number, difference;
                            /* ��Ɨp�ϐ� */
    int original_data_size, coded_data_size; /* �f�[�^�ʂɊւ���ϐ� */
    double compression_rate; /* ���k�� */

    printf("***** �x�N�g���ʎq���@ *****\n");
    do {
        printf("��\�x�N�g���̖{��(8�`%d):", MAX_CLUSTERS );
        scanf("%d",&K_number);
    } while ( K_number < 8 || K_number > MAX_CLUSTERS );
    /* ����\�x�N�g���̐ݒ� */
    for ( i = 0; i < K_number; i ++ ){
        x = random_int( x_block );
        y = random_int( y_block );
        for ( j = 0; j < DIMENSION; j ++ ){
            main_vector[i][j] = vector[y][x][j];
        }
    }
    /* K-���ϖ@�ɂ���ăx�N�g���̃N���X�^�����O���s�� */
    finish = 0;    counter = 0;
    while ( ! finish ){
        counter ++;  
        printf("%d ��ڂ̃N���X�^�����O���ł��D\n", counter );
        /* �N���X�^�̏�����\�_�ւ̑�� */
        for ( i = 0; i < K_number; i ++ ){
            for ( j = 0; j < DIMENSION; j ++ )
                init_main_vector[i][j] = main_vector[i][j];
        }
        /* �e�u���b�N�̏����N���X�^�ԍ��̏����� */
        for ( y = 0; y < y_block; y ++ )
            for ( x = 0; x < x_block; x ++ )
                cluster[y][x] = - 1;
        /* �e�_�̏����N���X�^�����肷�� */
        for ( y = 0; y < y_block; y ++ ){
            for ( x = 0; x < x_block; x ++ ){
                /* �e�N���X�^��\�_�Ƃ̋��������߂� */
                min_distance = 255 * 255 * DIMENSION;
                for ( i = 0; i < K_number; i ++ ){
                    distance = calc_distance( y, x, i );
                    if ( distance < min_distance){
                        min_distance = distance;
                        min_num = i;
                    }
                }
                /* [y][x]�u���b�N���N���X�^ No.min_num �֑������� */
                cluster[y][x] = min_num;
            }
        }
        /* �e�N���X�^�̑�\�_���C������ */
        for ( i = 0; i < K_number; i ++ ){
            for ( j = 0; j < DIMENSION; j ++ ){
                sum[j] = 0.0;
            }
            sum_number = 0.0;
            for ( y = 0; y < y_block; y ++ ){
                for ( x = 0; x < x_block; x ++ ){
                    if ( cluster[y][x] == i ){
                        sum_number = sum_number + 1.0;
                        for ( j = 0; j < DIMENSION; j ++ )
                            sum[j] = sum[j] + vector[y][x][j];
                    }
                }
            }
            for ( j = 0; j < DIMENSION; j ++ ){
                main_vector[i][j] = (unsigned char)
                    ( sum[j] / sum_number );
            }
        }
        /* �N���X�^���S���ړ��������ǂ����̃`�F�b�N */
        finish = 1;
        for ( i = 0; i < K_number; i ++ ){
            difference = 0.0;    
            for ( j = 0; j < DIMENSION; j ++ ){
                difference = difference + abs(
                    init_main_vector[i][j] - main_vector[i][j] );
            }
            if ( difference > MIN_DIFFERENCE ) finish = 0;
        }
        if ( finish == 1 ){  /* �����I�� */ 
            /* �e�u���b�N���\�x�N�g���ɒu�������� */
            x_size2 = x_size1;   y_size2 = y_size1;
            for ( y = 0; y < y_block; y ++ ){
                for ( x = 0; x < x_block; x ++ ){
                    xs = BLOCK_SIZE * x;    
                    ys = BLOCK_SIZE * y;
                    for ( i = 0; i < BLOCK_SIZE; i ++ ){
                        for ( j = 0; j < BLOCK_SIZE; j ++ ){
                            image2[ ys + i][ xs + j] =
                                main_vector[ cluster[y][x] ]
                                           [i*BLOCK_SIZE+j];
                        }
                    }
                }
            }
        }
    }
    printf("�x�N�g���ʎq�����I�����܂����D\n");
    /* �f�[�^�ʁE���k���֌W�̌v�Z�ƕ\�� */
    original_data_size = x_size1 * y_size1 * 8;
    printf("���̊K���摜�̃f�[�^�� = %d x %d x 8 = %d bits\n",
        x_size1,y_size1,original_data_size);
    i = K_number - 1;    j = 0;
    do{
        i = i / 2;   j ++;
    } while ( i > 0 );  /* j : ��\�x�N�g���̎w��ɕK�v�ȃr�b�g�� */
    coded_data_size = BLOCK_SIZE * BLOCK_SIZE * K_number * 8
                            /* ��\�x�N�g�� */
        + j * x_block * y_block; /* ���b�N�A�b�v�e�[�u�� */
    printf("���k�f�[�^�i�摜�����j�̃f�[�^�� = %d bits\n",
        coded_data_size);
    compression_rate = (double)coded_data_size
        / (double)original_data_size * 100.0;
    printf("���k���F%f [��]\n",compression_rate);
}

main( )
{
    load_image_data( );           /* �摜�t�@�C�� --> image1  */
    initialize_random_number( );
    obtain_vector_data( );        /* image1���̓_���x�N�g���� */
    clustering_using_Kmeans( );   /* K-���ϖ@�ŃN���X�^�����O */
    save_image_data( );           /* image2 --> �t�@�C���o��  */
    return 0;
}
