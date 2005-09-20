/* �_�o��H�Ԃ̃w�b�_ myNN.h */

/* �_�o��H�ԂɊւ���萔�錾 */
#define MAX_INPUT  100       /* ���̓��j�b�g���̍ő�l */
#define MAX_HIDDEN  50       /* ���ԃ��j�b�g���̍ő�l */
#define MAX_OUTPUT  20       /* �o�̓��j�b�g���̍ő�l */
#define MAX_WEIGHT  5.0      /* �����׏d�̏����l�͈̔� */
                         /* - MAX_WEIGHT �` MAX_WEIGHT */
#define MAX_LEARN  50        /* ���t�M�����̍ő�l     */

/* �_�o��H�ԂɊւ�����ϐ��̐錾 */
    /* �����׏d�i���͑w �� ���ԑw�j */
double wght_in_hid[MAX_INPUT][MAX_HIDDEN];
    /* �����׏d�i���ԑw �� �o�͑w�j */
double wght_hid_out[MAX_HIDDEN][MAX_OUTPUT];
    /* ���t�M����ۑ����邽�߂̔z�� */
double learn_input_data[MAX_LEARN][MAX_INPUT];
double learn_output_data[MAX_LEARN][MAX_OUTPUT];
    /* ���́C���ԁC�o�̓��j�b�g�� */
int num_input, num_hidden, num_output;
    /* ���t�M���� */
int num_learn;
    /* ���j�b�g�̏�� */
double input_unit[MAX_INPUT];
double hidden_unit[MAX_HIDDEN];
double output_unit[MAX_OUTPUT];

/* �֐��̃v���g�^�C�v�錾 */
double random_double( double n );
void initialize_weights( );
int get_data( FILE *fp );
void load_learn_data( );

/* �ȉ��͊֐��̖{�� */

double random_double( double n )
/* 0.0 ���� n �܂Łi n ��菬�����j�̎����̗�����Ԃ� */
{
    return (double)rand( ) / ( (double)RAND_MAX + 1.0 ) * n;
}

void initialize_weights( )
{
    int seed, i, j; /* ��ƕϐ� */

    /* �����̏����� */
    printf("�����̌n������������܂��D\n");
    printf("�����̃V�[�h�i�C�ӂ̐��̐����j:");
    scanf("%d",&seed);
    if ( seed < 1 ) seed = 1;
    srand( seed );
    for ( i = 0; i < num_input; i ++ ){
        for ( j = 0; j < num_hidden; j ++ ){
            wght_in_hid[i][j] = 2.0 * MAX_WEIGHT 
                * random_double( 1.0 ) - MAX_WEIGHT;
        }
    }
    for ( i = 0; i < num_hidden; i ++ ){
        for ( j = 0; j < num_output; j ++ ){
            wght_hid_out[i][j] = 2.0 * MAX_WEIGHT 
                * random_double( 1.0 ) - MAX_WEIGHT;
        }
    }
}

int get_data( FILE *fp )
/* �t�@�C������P����(�O�܂��͂P)�ǂݍ��� */
{
    char ch;

    do {
        ch = (char)fgetc( fp );
    } while ( ch != '0' && ch != '1' );
    if ( ch == '0' ) return 0;
    else return 1;
}

void load_learn_data( )
/* �_�o��H�Ԃ̋��t�M���̃f�[�^���t�@�C������ǂݍ��� */
{
    char file_name[256];  /* ���̓t�@�C���� */
    int buffer;   /* �f�[�^��ǂݍ��ނƂ��̍�ƕϐ� */
    FILE *fp;  /* �t�@�C���|�C���^ */
    int i, j;  /* ���[�v�ϐ� */

    /* ���t�M���t�@�C���̃I�[�v�� */
    printf("���t�M���t�@�C����ǂݍ��݂܂��D\n");
    printf("���t�t�@�C���� (*.lrn) : ");
    scanf("%s", file_name);
    fp = fopen( file_name, "r" );
    if ( NULL == fp ){
        printf("���̖��O�̃t�@�C���͑��݂��܂���D\n");
        exit(1);
    }
    /* num_input, num_hidden, num_output, num_learn */
    fscanf( fp, "%d %d %d %d", &num_input, &num_hidden,
            &num_output, &num_learn );
    printf( "���̓��j�b�g�� + �������l�ݒ胆�j�b�g = %d\n",
             num_input);
    printf( "���ԃ��j�b�g�� + �������l�ݒ胆�j�b�g = %d\n",
             num_hidden );
    printf( "�o�̓��j�b�g�� = %d\n", num_output );
    printf( "���t�M����     = %d\n", num_learn );
    if ( num_input  > MAX_INPUT   || 
         num_hidden > MAX_HIDDEN  ||
         num_output > MAX_OUTPUT ) {
        printf("���j�b�g�����s�K�؂ł��D\n");
        exit(1);
    }
    /* ���t�M���̓ǂݍ��� */
    for ( i = 0; i < num_learn; i ++ ){
        printf("���t�M�� No.%d : ",i+1);
        for ( j = 0; j < num_input - 1; j ++ ){
            /* No.num_input - 1 �͂������l�ݒ胆�j�b�g */
            buffer = get_data( fp );
            printf("%d", buffer);
            if ( buffer == 1 ) learn_input_data[i][j] = 0.9;
            else learn_input_data[i][j] = 0.1;
        }
        printf(" �� ");
        for ( j = 0; j < num_output; j ++ ){
            buffer = get_data( fp );
            printf("%d",buffer);
            learn_output_data[i][j] = (double)buffer;
        }
        printf("\n");
    }
    printf("�f�[�^�͐������ǂݍ��܂�܂����D\n");
    fclose(fp);
    initialize_weights( );    /* �����׏d�̏����� */
} 
