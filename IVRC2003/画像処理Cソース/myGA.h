/* ��`�I�A���S���Y���p�̃w�b�_�t�@�C�� myGA.h */

    /* �萔�錾 */
#define MAX_POP_SIZE   200   /* �ő�̐� */
#define MAX_GENE_SIZE   50   /* �z�肷��ő���F�̒� */
    /* ���ϐ� */
    /* �́C�V�̂̈�`�q�^ */
unsigned char genotype[MAX_POP_SIZE][MAX_GENE_SIZE]; 
unsigned char new_genotype[MAX_POP_SIZE][MAX_GENE_SIZE];
    /* �́C�V�̂̓K���x�i�ʏ�� 0 �` 1 �̎����j */
double fitness[MAX_POP_SIZE];
double new_fitness[MAX_POP_SIZE];
    /* �G���[�g�̈�`�q�^�ƓK���x */
unsigned char elite_genotype[MAX_GENE_SIZE];
double elite_fitness;
int elite_number; /* �G���[�g�̌̔ԍ� */
    /* GA�̃p�����[�^ */
int pop_size;    /* �̑��� */
int gene_size;   /* ���F�̒� */
double crossover_rate;  /* ������ [0, 1]     */
double mutation_rate;   /* �ˑR�ψٗ� [0, 1] */
int elite_flag; /* �G���[�g�ۑ��헪����(1)�^�Ȃ�(0) */
int crs_type;    /* �������@�i1:1�_�C2:2�_�C3:��l�j  */

/* �֐��̃v���g�^�C�v�錾 */
void initialize_random_number( ); /* ���������������� */
int random_int( int n );          /* 0�`n-1�܂ł̐���������Ԃ� */
double random_double( double n ); /* 0.0�`n�܂ł̎���������Ԃ� */
    /* 2�� unsigned char �̒l���������� */
void swap_unsigned_char( unsigned char *n1, unsigned char *n2 ); 
void swap_int( int *n1, int *n2 ); /* 2�� int �̒l���������� */
void intialize_parameters( );     /* �p�����[�^������������ */
void initialize_genes( );         /* ���F�̂����������� */
void copy_new_to_old( );          /* �z����R�s�[����i�V�����j*/
void one_point_crossover( int n1, int n2 ); /* �P�_�������s�� */
void two_point_crossover( int n1, int n2 ); /* �Q�_�������s�� */
void uniform_crossover( int n1, int n2 ); /* ��l�������s�� */
void selection_using_roulette_rule( ); /* ���[���b�g�I�����s�� */
void execute_crossover( ); /* �����̎�ނ��w�肵�Ď��s���� */
void execute_mutation( ); /* �ˑR�ψق����s���� */
void find_and_set_best_individual( ); /* ���݂̍ŗǌ̂��݂��� */
void elitist_strategy( ); /* �G���[�g�ۑ��헪�����s���� */

/* �ȉ��͊֐��̖{�� */

void initialize_random_number( )
/* �����̏����� */
{
    int seed; /* �V�[�h */

    printf("�����̌n������������܂��D\n");
    printf("�����̃V�[�h�i�C�ӂ̐��̐����j:");
    scanf("%d",&seed);
    if ( seed < 1 ) seed = 1;
    srand( seed );
}

int random_int( int n )
/* 0 �ȏ� n - 1 �ȉ��̐����̗�����Ԃ� */
{
    return (int)( (double)rand( ) / ( (double)RAND_MAX + 1.0 ) * n );
}

double random_double( double n )
/* 0.0 ���� n �܂Łi n ��菬�����j�̎����̗�����Ԃ� */
{
    return (double)rand( ) / ( (double)RAND_MAX + 1.0 ) * n;
}

void swap_unsigned_char( unsigned char *n1, unsigned char *n2 )
/* unsigned char n1 �� n2 ���������� */
{
    unsigned char n;
    n = *n1;    *n1 = *n2;    *n2 = n;
}

void swap_int( int *n1, int *n2 )
/* int n1 �� n2 ���������� */
{
    int n;
    n = *n1;    *n1 = *n2;    *n2 = n;
}

void initialize_parameters( )
/* gene_size �ȊO�̃p�����[�^��ݒ肷�� */
{
    float work; /* ��ƕϐ� */

    printf("***** �f�`�̃p�����[�^�̐ݒ� *****\n");
    do {
        printf("�̑��� (10�`%d) : ",MAX_POP_SIZE);
        scanf("%d",&pop_size);
    } while ( pop_size < 10 || pop_size > MAX_POP_SIZE );
    do {
        printf("�������@ (1:1�_�C2:2�_�C3:��l�j : ");    
        scanf("%d",&crs_type);    
    } while ( crs_type != 1 && crs_type != 2 && crs_type != 3 );
    do {
        printf("������ (0.0�`1.0) : ");    
        scanf("%f",&work);    crossover_rate = work;
    } while ( crossover_rate < 0.0 || crossover_rate > 1.0 );
    do {
        printf("�ˑR�ψٗ� (0.0�`1.0) : ");    
        scanf("%f",&work);    mutation_rate = work;
    } while ( mutation_rate < 0.0 || mutation_rate > 1.0 );
    printf("�G���[�g�ۑ��헪���s��(1)/�s��Ȃ�(0) : ");    
    scanf("%d",&elite_flag); 
    if ( elite_flag != 1 ) elite_flag = 0;
}

void initialize_genes( )
/* ���F�̂����������� */
{
    int i, j;        /* ���[�v�ϐ� */

    for ( i = 0; i < pop_size; i ++ ){
        for ( j = 0; j < gene_size; j ++ ){
            if ( random_double(1.0) < 0.5 ) genotype[i][j] = 0;
            else genotype[i][j] = 1;
        }
    }
}

void copy_new_to_old( )
/* new_genotype => genotype, new_fitness => fitness */
/* �z����R�s�[����                                 */
{
    int i, j; /* ����ϐ� */

    for ( i = 0; i < pop_size; i ++ ){
        for ( j = 0; j < gene_size; j++ )
            genotype[i][j] = new_genotype[i][j];
        fitness[i] = new_fitness[i];
    }
}

void one_point_crossover( int n1, int n2 )
/* �� No.n1 �ƌ� No.n2 �̐��F�̂��P�_���������C���ƒu�������� */
{
    int crs_pnt;        /* �����ʒu�i���[�͐ݒ肵�Ȃ��j*/
    int i;              /* ����ϐ� */

    crs_pnt = random_int( gene_size );
    /* �㔼�������ւ� */
    for ( i = crs_pnt + 1; i < gene_size; i ++ ){
        swap_unsigned_char( &genotype[n1][i], &genotype[n2][i] );  
    }
}

void two_point_crossover( int n1, int n2 )
/* �� No.n1 �ƌ� No.n2 �̐��F�̂��Q�_���������C���ƒu�������� */
{
    int crs_pnt1, crs_pnt2;  /* �����ʒu�i���[�͐ݒ肵�Ȃ��j*/
    int i;                   /* ���[�v�ϐ� */

    crs_pnt1 = random_int( gene_size );
    do {
        crs_pnt2 = random_int( gene_size );
    } while ( crs_pnt1 == crs_pnt2 );
    if ( crs_pnt1 > crs_pnt2 ) swap_int( &crs_pnt1, &crs_pnt2 );
    /* ���ԕ������ւ� */
    for ( i = crs_pnt1 + 1; i < crs_pnt2; i ++ ){
        swap_unsigned_char( &genotype[n1][i], &genotype[n2][i] );
    }
}

void uniform_crossover( int n1, int n2 )
/* �� No.n1 �ƌ� No.n2 �̐��F�̂���l���������C���ƒu�������� */
/* �q���͂Q���i�݂��ɔ��]�p�^�[���j�����Ɨ��e�����ւ���D */
{
    int i; /* ���[�v�ϐ�   */

    /* ��`�q���Ƃɂǂ��炩�̐e�̈�`�q��I������ */
    for ( i = 0; i < gene_size; i ++ ){
        if ( random_double( 1.0 ) < 0.5 ){  
            /* ���m���Őe�ǂ����̈�`�q�����ւ��Ă��� */
            swap_unsigned_char( &genotype[n1][i], &genotype[n2][i] );
        }
    }
}

void selection_using_roulette_rule( )
/* ���[���b�g���[���Ɋ�Â��Ď�����̌̂����߂�  */
/* �f�[�^�� new_genotype, new_fitness �ɑ������� */
{
    int i, j, num; /* ����ϐ��C�I�����ꂽ�̔ԍ� */
    double sum, rand_real;  /* ��Ɨp�ϐ� */
    static double roulette_table[MAX_POP_SIZE]; /* ���[���b�g */

    /* ���[���b�g�̐��� */
        /*�i�P�������߂Ă��獇�v�Ŋ����Ĕ䗦���o���j */
    sum = 0.0;
    for ( i = 0; i < pop_size; i ++ ) 
        sum = sum + fitness[i];
    for ( i = 0; i < pop_size; i ++ ){
        roulette_table[i] = fitness[i] / sum;
    }
        /* �e�̂̋��E�̔䗦�� roulette_table �� */
    sum = 0.0;
    for ( i = 0; i < pop_size; i ++ ){
        sum = sum + roulette_table[i];
        roulette_table[i] = sum;
    }
    /* �I�𑀍�i���[���b�g�� pop_size �񂾂��񂷁j*/
    for ( i = 0; i < pop_size; i ++ ){
        rand_real = random_double( 1.0 );  /* [0.0, 1.0) */
        for ( num = 0; num < pop_size; num ++ ){
            if ( roulette_table[num] > rand_real ) break;
            /* No.num ���I�����ꂽ */
        }
        /* �V���� No.i �̂� No.num �̃f�[�^���� */
        for ( j = 0; j < gene_size; j ++ )
            new_genotype[i][j] = genotype[num][j];
        new_fitness[i] = fitness[num];
    }
    copy_new_to_old( ); /* new ���畁�ʂ� genotype, fitnes �ɖ߂� */
}

void execute_crossover( )
/* �Q�������_���ɑg�ݍ��킹�Č��������� */
/* ���ϐ� crs_type =1:�P�_�����C=2:�Q�_�����C=3�F��l���� */
{
    int i;  /* ���[�v�ϐ� */
    int num1, num2;  /* �̔ԍ� */
    int num_of_pair;   /* �y�A�̑��� */
    static number[MAX_POP_SIZE]; /* ����ւ��̂��߂̔z�� */

    for ( i = 0; i < pop_size; i ++ ) number[i] = i; /* ������ */
    /* �����_���ɕ��בւ� */
    for ( i = 0; i < pop_size * 10; i ++ ){
        num1 = random_int( pop_size );
        num2 = random_int( pop_size );
        swap_int( &number[ num1 ], &number[ num2 ] );
    }
    num_of_pair = pop_size / 2;
    for ( i = 0; i < num_of_pair; i ++ ){
        num1 = number[ 2 * i ];    /* �e�P */
        num2 = number[ 2 * i + 1]; /* �e�Q */
        /* ���������邩�ǂ������肷�� */
        if ( random_double( 1.0 ) <= crossover_rate ){
            if ( crs_type == 1 ) one_point_crossover( num1, num2 );
            else if ( crs_type == 2 ) two_point_crossover( num1, num2 );
            else uniform_crossover( num1, num2 );
        }
    }
}

void execute_mutation( )
/* �S�̂̑S��`�q�̓ˑR�ψ� */
{
    int i, j; /* ���[�v�ϐ� */

    for ( i = 0; i < pop_size; i ++ ){
        for ( j = 0; j < gene_size; j ++ ){
            if ( random_double( 1.0 ) <= mutation_rate )
                genotype[i][j] = (unsigned char)(1 - genotype[i][j]);
        }
    }
}

void find_and_set_best_individual( )
/* ���݂̍ŗǌ̂����o���C����̑��ϐ��ɑ������     */
/* ���ϐ��Felite_number, elite_fitness, elite_genpyte */
{
    int i; /* ���[�v�ϐ� */
    double best_fitness; /* �ő�K���x */

    best_fitness = 0.0;
    for ( i = 0; i < pop_size; i ++ )
        if ( fitness[i] > best_fitness ){
            elite_number = i;
            best_fitness = fitness[i];
        }
    for ( i = 0; i < gene_size; i ++ )
        elite_genotype[i] = genotype[elite_number][i];
    elite_fitness = fitness[elite_number];
}
    
void elitist_strategy( )
/* �G���[�g�ۑ����s�� */
{
    int worst_number;     /* �ň��̔ԍ� */
    int i;             /* ���[�v�ϐ�     */
    double worst_fitness; /* �ň��K���x   */

    /* �ň��̂̌��o */
    worst_fitness = 1.0;    worst_number = 0;
    for ( i = 0; i < pop_size; i ++ ){
        if ( fitness[i] < worst_fitness ){
            worst_number = i;
            worst_fitness = fitness[i];
        }
    }
    /* �O��̃G���[�g�ƍň��̂̓���ւ� */
    for ( i = 0; i < gene_size; i ++ )
        genotype[worst_number][i] = elite_genotype[i];
    fitness[worst_number] = elite_fitness;
}
