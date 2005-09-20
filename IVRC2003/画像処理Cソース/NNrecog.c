/* �_�o��H�Ԃ��g���ĔF�����s���v���O���� NNrecog.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"myNN.h"  /* for get_data( ) */

void calc_hidden_and_output_units( )
/* ���݂̓��͂ɑ΂��Ē��ԑw�C�o�͑w�̃��j�b�g�̏�Ԃ����߂� */
/* ���̃��[�`���̑O�� input_unit[ ] �Ɍ��݂̓��͂�������  */
{
    int i,j;       /* ���[�v�ϐ� */
    double sum;    /* �d�ݕt�o�̘͂a */

    /* �B��w�̊e���j�b�g����ԑJ�ڂ����� */
    for ( i = 0; i < num_hidden - 1; i ++ ){
        sum = 0.0;
        for ( j = 0; j < num_input; j ++ )
            sum = sum + input_unit[j] * wght_in_hid[j][i];
        hidden_unit[i] = 1.0 / ( 1.0 + exp( - sum ) );
    }
    input_unit[num_input - 1] = 1.0; /* �������l�ݒ胆�j�b�g */
    /* �o�͑w�̊e���j�b�g����ԑJ�ڂ����� */
    for ( i = 0; i < num_output; i ++ ){
        sum = 0.0;
        for ( j = 0; j < num_hidden - 1; j ++ )
            sum = sum + hidden_unit[j] * wght_hid_out[j][i];
        output_unit[i] = 1.0 / ( 1.0 + exp( - sum ) );
    }
    hidden_unit[num_hidden - 1] = 1.0; /* �������l�ݒ胆�j�b�g */
}

void load_NN_data( )
/* �_�o��H�Ԃ̌����׏d�̃f�[�^���t�@�C������ǂݍ��ށD */
{
    char file_name[256]; /* ���̓t�@�C�����������镶���� */
    FILE *fp;            /* �t�@�C���|�C���^ */
    float f;             /* ��ƕϐ� */
    int i, j;            /* ����ϐ� */

    /* ���̓t�@�C���̃I�[�v�� */
    printf("�_�o��H�Ԃ̃f�[�^��ǂݍ��݂܂��D\n");
    printf("���̓t�@�C���� (*.nn) : "); /* �g���q = .nn �Ƃ��Ă��� */
    scanf("%s", file_name);
    fp = fopen( file_name, "r" );
    if ( NULL == fp ){
        printf("���̖��O�̃t�@�C���͑��݂��܂���D\n");
        exit(1);
    }
    fscanf(fp,"%d %d %d",&num_input,&num_hidden,&num_output);
    for ( i = 0; i < num_input; i ++ )
        for ( j = 0; j < num_hidden; j ++ ){
            fscanf( fp, "%f", &f );    wght_in_hid[i][j] = f;
        }
    for ( i = 0; i < num_hidden; i ++ )
        for ( j = 0; j < num_output; j ++ ){
            fscanf( fp, "%f", &f );    wght_hid_out[i][j] = f;
        }
    fclose(fp);
    printf("�����׏d�̃f�[�^��ǂݍ��݂܂����D\n\n");
}

void calculation( )
/* ���m�M���̃f�[�^���t�@�C������ǂݍ��݂��ʂ����߂ĕ\������ */
{
    char file_name[256]; /* ���̓t�@�C�����������镶���� */
    int num;             /* �f�[�^��ǂݍ��ނƂ��̍�ƕϐ� */
    FILE *fp;            /* �t�@�C���|�C���^ */
    int i, j;            /* ���[�v�ϐ� */
    int num_data;        /* ���m�M���̑��� */

    /* ���m�M���t�@�C���̃I�[�v�� */
    printf("���m�M���t�@�C����ǂݍ��݂܂��D\n");
    printf("���m�t�@�C���� (*.dat) : "); /* �g���q = .dat �Ƃ��Ă��� */
    scanf("%s", file_name);
    fp = fopen( file_name, "r" );
    if ( NULL == fp ){
        printf("���̖��O�̃t�@�C���͑��݂��܂���D\n");
        exit(1);
    }
    (void)getchar( ); /* ���ꂪ�Ȃ��ƍŏ��f�[�^�������ĕ\������� */
    /* ���́C���ԁC�o�̓��j�b�g���C���m�M���̑��� */
    fscanf(fp,"%d %d %d %d",
        &num_input,&num_hidden,&num_output,&num_data);
    printf( "���̓��j�b�g�� + �������l�ݒ胆�j�b�g = %d\n",
        num_input);
    printf( "���ԃ��j�b�g�� + �������l�ݒ胆�j�b�g = %d\n",
        num_hidden );
    printf( "�o�̓��j�b�g�� = %d\n", num_output );
    printf( "���m�M���̑��� = %d\n\n", num_data );
    /* ���m�M���̓ǂݍ��� */
    printf("�[���Q�l������Ă��Ȃ��o�͂� * �ŕ\���Ă��܂��D\n\n");
    for ( i = 0; i < num_data; i ++ ){
        printf("���m�M�� No.%d : ",i+1);
        for ( j = 0; j < num_input - 1; j ++ ){
            num = get_data( fp );
            printf("%d",num);
            if ( num == 0 ) input_unit[j] = 0.1;
            else input_unit[j] = 0.9;
        }
        input_unit[num_input - 1] = 1.0; /* �������l�ݒ胆�j�b�g */
        /* ���ԁC�o�̓��j�b�g�̏�ԑJ�� */
        calc_hidden_and_output_units( );
        printf(" �� ");
        for ( j = 0; j < num_output; j ++ ){
            if ( output_unit[j] >= 0.9 ) printf("1");
            else if ( output_unit[j] <= 0.1 ) printf("0");
            else printf("*");  
        }
        printf("   push return key");    
        (void)getchar( ); /* ���^�[����ǂނ����̂��� */
    }
    fclose(fp);
    printf("\n�S�Ă̖��m�M���ɑ΂��鏈�����I�����܂����D\n");
}

main( )
{
    load_NN_data( );   /* �����׏d�f�[�^�̓ǂݍ���     */
    calculation( );    /* ���m�M����ǂݍ���Ő����o�� */
    return 0;
}
