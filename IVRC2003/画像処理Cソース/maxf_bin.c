/* GA �ɂ��֐��œK���i�o�C�i���R�[�h�j maxf_bin.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"myGA.h" 

/* �T���֐��̂��߂̃p�����[�^ */
#define PNUM  5   /* �T���֐��̃p�����[�^�����i�ύX�j�@*/
int fparam[PNUM]; /* �T���֐��̕ϐ��i�����Ō��߂���j  */
#define SOLUTION_FITNESS 0.999 /* �K���x�̖ڕW�l�i1.0�ł���) */
#define MAX_GENERATION    2000 /* �z��ő吢�㐔�i�ύX�j  */

void set_optimizing_task( )
/* �����œK�������`���� */
{
    /* �֐��̍ő�l�T�����̗��            */
    /* �W�r�b�g�̕ϐ�PNUM�ō\������Ă���  */
    int i; /* ����ϐ� */

    gene_size = 8 * PNUM;
    for ( i = 0; i < PNUM; i ++ )
        fparam[i] = 80 + random_int( 80 );
}

void trans_from_genotype_to_parameters( int number, int *p )
/* �̔ԍ� No.number �̈�`�q�^���p�����[�^�ɕϊ����� */
{
    int i, j; /* ���[�v�ϐ� */

    /* ��`�q�^���琮���ւ̕ϊ� */
    for ( i = 0; i < PNUM; i ++ ){
        *(p + i)= 0;
        for ( j = 0; j < 8; j ++ )
            *(p + i) = *(p + i) * 2 + genotype[number][j+8*i];
    }
}

double fitness_value( int number )
/* set_optimizing_task( )�Ő�������Ă���� No.number */
/* �̓K���x��Ԃ��i���̏ꍇ�̓e�X�g�֐��j               */
{
    int i, p[PNUM];  /* ����ϐ��C�p�����[�^�p�ϐ� */
    double f;   /* �֐��l */

    /* ��`�q�^���� (x,y) �� */
    trans_from_genotype_to_parameters( number, p );
    /* �K���x�̌v�Z */
    f = 1.0;
    for ( i = 0; i < PNUM; i ++ )
        f = f - abs( p[i] - fparam[i] ) / (120.0 * PNUM);
    if ( f < 0.0 ) f = 0.0; else if ( f > 1.0 ) f = 1.0;
    return f;
}

void calculate_fitness( )
/* �S���̓K���x�����߂� */
{
    int i; /* ���[�v�ϐ� */
    
    for ( i = 0; i < pop_size; i ++ )
        fitness[i] = fitness_value(i);
}

void display_best_individual( int generation )
/* �ŗǌ̂̈�`�q�^�C�K���x��\������ */
{
    int i; /* ����ϐ� */

    printf("No.%d�F",generation);
    for ( i= 0; i < gene_size; i ++ ){
        printf("%d",elite_genotype[i]);
        if ( (i+1) % 8 == 0 ) printf(" ");
    }
    printf("--> %f\n", elite_fitness);
}

void generation_iteration( )
/* ��������s�� */
{
    int i, generation; /* ���[�v�ϐ��C���㐔 */
    int p[PNUM];    /* �ŗǌ̂̃p�����[�^ */

    generation = 1;
    calculate_fitness( );    /* �K���x�̌v�Z   */
    find_and_set_best_individual( ); /* �G���[�g�̌��o */
    do{
        generation ++;
        /* ���[���b�g���[���Ɋ�Â����� */
        selection_using_roulette_rule( );
        /* ���F�̂̌��� */
        execute_crossover( );
        /* ��`�q�̓ˑR�ψ� */
        execute_mutation( );
        /* �K���x�̌v�Z */
        calculate_fitness( );
        /* �G���[�g�ۑ� */
        if ( elite_flag == 1 ) elitist_strategy();
        find_and_set_best_individual(); /* �G���[�g�̌��o */
        /* �ŗǌ̂̕\�� */
        display_best_individual( generation );
    } while ( elite_fitness < SOLUTION_FITNESS && 
              generation < MAX_GENERATION );
    trans_from_genotype_to_parameters( elite_number, p );
    printf("�ŏI�I�ȉ��F");
    for ( i=0; i<PNUM; i++ ) printf("%d ",p[i]);
    printf("\n�^�̍œK���F");
    for ( i=0; i<PNUM; i++ ) printf("%d ",fparam[i]);
    printf("\n");
}

main( )
{
    initialize_random_number( );  /* �����̏�����       */
    set_optimizing_task( );       /* ���̐ݒ�         */
    initialize_parameters( );     /* GA�p�����[�^�̐ݒ� */
    initialize_genes( );          /* ��`�q�^�̏�����   */
    generation_iteration( );      /* ������           */
    return 0;
}

