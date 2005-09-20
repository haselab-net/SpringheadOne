/* �덷�t�`�d(BP)�@�̃v���O���� BPlearn.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"myNN.h" /* �_�o��H�Ԃ̃w�b�_�t�@�C�� */

#define E  0.3             /* =�Ái�w�K�W���j  */
#define LEARNING_PERIOD  5
    /* ���񂲂Ƃɋ��t�M����ς��邩���w�� */

void calc_hidden_and_output_units( )
/* ���݂̓��͂ɑ΂��Ē��ԑw�C�o�͑w�̃��j�b�g�̏�Ԃ����߂� */
/* ���̃��[�`���̑O�� input_unit[ ] �Ɍ��݂̓��͂�������  */
{
    int i, j;      /* ���[�v�ϐ� */
    double sum;    /* �d�ݕt�o�̘͂a */

    /* �B��w�̊e���j�b�g����ԑJ�ڂ����� */
    input_unit[num_input - 1] = 1.0; /* �������l�ݒ胆�j�b�g */
    for ( i = 0; i < num_hidden - 1; i ++ ){
        sum = 0.0;
        for ( j = 0; j < num_input; j ++ )
            sum = sum + input_unit[j] * wght_in_hid[j][i];
        hidden_unit[i] = 1.0 / ( 1.0 + exp( - sum ) );
    }
    /* �o�͑w�̊e���j�b�g����ԑJ�ڂ����� */
    hidden_unit[num_hidden - 1] = 1.0; /* �������l�ݒ胆�j�b�g */
    for ( i = 0; i < num_output; i ++ ){
        sum = 0.0;
        for ( j = 0; j < num_hidden; j ++ )
            sum = sum + hidden_unit[j] * wght_hid_out[j][i];
        output_unit[i] = 1.0 / ( 1.0 + exp( - sum ) );
    }
}

double calc_error_of_NN( )
/* ���݂̑S���t�f�[�^�ɑ΂����H�Ԃ̏o�͂Q��덷�̕��ϒl��Ԃ� */
{
    int i, j;          /* ����ϐ� */
    double err, sum;   /* �r�b�g���Ƃ̌덷�C���̂Q��a */

    sum = 0.0;
    /* �e���t�M���Ƃ̌덷�����߂� */
    for ( i = 0; i < num_learn; i ++ ){
        /* ���͐M������͂��� */
        for ( j = 0; j < num_input - 1; j ++ ){
            if ( learn_input_data[i][j] < 0.5 )
                input_unit[j] = 0.1;
            else input_unit[j] = 0.9;
        }
        input_unit[num_input - 1] = 1.0; /* �������l�ݒ胆�j�b�g */
        /* �B��w�C�o�͑w�̃��j�b�g����ԑJ�ڂ����� */
        calc_hidden_and_output_units( );
        /* �o�͌덷�����߂� */
        for ( j = 0; j < num_output; j ++ ){
            err = learn_output_data[i][j] - output_unit[j];
            sum = sum + err * err;  /* �덷�̂Q���ݐ� */
        }
    }
    sum = sum / (double)num_learn; /* �덷���f�[�^���ŕ��ω� */
    return( sum );
}

void BPlearning( )
/* �덷�t�`�d(BP)�@�Ɋ�Â���H�Ԃ̊w�K���s�� */
{
    int counter, lrn_num;  /* �w�K�񐔁C���݂̋��t�M���̔ԍ� */
    int learning_time;     /* �g�[�^���̊w�K�� */
    double acceptable_error;  /* ���e�ő�덷�C�L�[���͂Ŏw�肷�� */
    double error;  /* �S���t�M���ɑ΂��镽�ςQ��덷 */
    float f;       /* ��ƕϐ� */
    int i, j, k;   /* ����ϐ� */
    /* �ȉ��͌����׏d�̏C���ʂ����߂�Ƃ��Ɏg���Ă���ϐ��ł��D */
    /* �{���̎� (8.2) �ɍ��킹�邽�߁C�ςȖ��O�ɂȂ��Ă��܂��D  */
    double delta_W, O_mi, y_i, O_k1_j;
    double d_hid[MAX_HIDDEN][MAX_OUTPUT], d_in;
    double sum;

    /* �w�K���I�������邽�߂̏����̎w�� */
    do {
        printf("���e�ő啽�ό덷( 0 �ȏ� 0.01 �ȉ��j : ");
        scanf("%f",&f);    acceptable_error = f;
    } while ( acceptable_error < 0.0 || acceptable_error > 0.01 );
    /* �w�K�J�n */
    learning_time = 1;    counter = 0;    lrn_num = 0;
    do {
        /* ���̓��j�b�g�����t�M�� No.lrn_num �Ɉ�v������ */
        for ( i = 0; i < num_input - 1; i ++ )
            input_unit[i] = learn_input_data[lrn_num][i];
        input_unit[num_input -1] = 1.0; /* �������l�ݒ胆�j�b�g */
        /* ���ԑw�Əo�͑w�̃��j�b�g�̏�Ԃ�J�ڂ����� */
        calc_hidden_and_output_units( );
        /* �o�͌덷��p���āC�e�����׏d����C������ */
        /* part 1 :  �o�͑w �� ���ԑw�̌����׏d */
        for ( j = 0; j < num_hidden; j ++ ){
            for ( i = 0; i < num_output; i ++ ){
                /* �ϐ�����{���ɍ��킹�Ă��܂� */
                O_mi = output_unit[i];  /* i �Ԗڂ̏o�� */
                y_i  = learn_output_data[lrn_num][i]; /* ���t�o�� */
                d_hid[j][i] = ( O_mi - y_i ) * O_mi * ( 1.0 - O_mi );
                O_k1_j = hidden_unit[j];
                delta_W = - E * d_hid[j][i] * O_k1_j;
                /* �����׏d�̏C�� */
                wght_hid_out[j][i] = wght_hid_out[j][i] + delta_W;
            }
        }
        /* part 2 :  ���ԑw �� ���͑w�̌����׏d */
        for ( j = 0; j < num_input; j ++ ){
            for ( i = 0; i < num_hidden; i ++ ){
                sum = 0.0;
                for ( k = 0; k < num_output; k ++ )
                    sum = sum + wght_hid_out[i][k] * d_hid[i][k];
                d_in = sum * hidden_unit[i] * ( 1.0 - hidden_unit[i] );
                delta_W = - E * d_in * input_unit[j];
                /* �����׏d�̏C�� */
                wght_in_hid[j][i] = wght_in_hid[j][i] + delta_W;
            }
        }
        error = calc_error_of_NN(); /* ���݂̉�H�Ԃ̌덷�����߂� */
        learning_time ++;    counter ++;  /* �J�E���^�̍X�V */
        printf("No.%d : error = %f\n", learning_time, error);
        /* ���Ԋu���Ƃɋ��t�M����؂�ւ��� */
        if ( counter > LEARNING_PERIOD ){  
            counter = 0;
            lrn_num ++;    if ( lrn_num > num_learn ) lrn_num = 0;
        }
    } while ( error > acceptable_error );  /* �덷�� -> �I�� */
    printf("�w�K���I�����܂����D\n");
}

void save_NN_data( )
/* �w�K��̐_�o��H�Ԃ̌����׏d���t�@�C���ɏo�͂���    */
/* �f�[�^�`�� : N1 N2 N3   -->  ���́C���ԁC�o�̓��j�b�g�� */
/* �����׏d�f�[�^�� wght_in_hid[N1][N2], wght_hid_out[N2][N3] */
/* �����Ԃɏo�͂��ďI���D�Y�����̓��������͈ȉ����Q�Ƃ̂��ƁD*/
{
    int i, j; /* ���[�v�ϐ� */
    char file_name[256];  /* �o�̓t�@�C�����p�����z�� */
    FILE *fp; /* �t�@�C���|�C���^ */  

    /* �o�̓t�@�C���̃I�[�v�� */
    printf("�w�K�����j���[�����l�b�g���[�N��ۑ����܂��D\n");
    printf("�o�̓t�@�C���� (*.nn) : ");  /* �g���q = .nn �Ƃ��Ă��� */
    scanf("%s",file_name);
    fp = fopen( file_name, "w");
    fprintf( fp, "%d %d %d\n", num_input, num_hidden, num_output );
    for ( i = 0; i < num_input; i ++ )
        for ( j = 0; j < num_hidden; j ++ )
            fprintf( fp, "%f\n", wght_in_hid[i][j] );
    for ( i = 0; i < num_hidden; i ++ )
        for ( j = 0; j < num_output; j ++ )
            fprintf( fp, "%f\n", wght_hid_out[i][j] );
    printf("�f�[�^�͐������o�͂���܂����D\n");
    fclose(fp);
}

main( )
{
    load_learn_data( ); /* ���t�f�[�^�̓ǂݍ��݂ƌ����׏d�̏����� */
    BPlearning( );      /* BP �w�K�@�ɂ�錋���׏d�̍œK��        */
    save_NN_data( );    /* �w�K��̐_�o��H�Ԃ�ۑ�               */
    return 0;
}
