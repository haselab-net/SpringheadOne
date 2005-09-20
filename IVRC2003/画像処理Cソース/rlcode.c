/* ���������O�X���������s���v���O���� rlcode.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

unsigned char line_buffer[5000]; /* 1���C�����̃f�[�^����p */
int index;          /* line_buffer[ ] �̌��݂̗v�f�ԍ���\�� */
FILE *fp;          /* �f�[�^���o�͂���ۂ̃t�@�C���|�C���^  */

void Wyle_coding( int n )
/* ���� n �̃��������O�X���������߂� line_buffer[ ] �ɑ�� */
{
    int n_length;     /* n - 1 �̂Q�i����   */
    int i, n_1, work; /* ����E��ƕϐ�     */

    /* �����ɑ΂��镄���� n_length �����߂� */
    n_1 = n - 1;
    if ( n_1 <= 3 ) n_length = 2;
    else {
        work = n_1;  n_length = 1;
        do { 
            n_length ++;    work = work / 2;
        } while ( work > 1 );
    }
    /* Wyle ������������ */
    if ( n_length > 2 ){
        for ( i = 0; i < n_length - 2; i ++ ){
            line_buffer[index] = 1;    index ++;
        }
    }
    line_buffer[index] = 0;
    /* n - 1 �̃o�C�i�������������� */
    index = index + n_length;
    do {
        line_buffer[index] = (unsigned char)(n_1 % 2);
                                   /* 2�Ŋ������Ƃ��̗]�� */
        n_1 = n_1 / 2;    index --;
    } while ( n_1 > 1 );
    if ( n < 3 ) line_buffer[index] = 0;
    else line_buffer[index] = 1;
    index = index + n_length;
}

void save_line_buffer( )
/* line_buffer[ ] �̓��e���t�@�C���ɏo�͂��� */
{
    unsigned char byte_data; /* 1�o�C�g���̃f�[�^�p */
    int byte_index;     /* line_buffer[ ] ���v�f�ԍ� */
    int byte_size,remain_bit_size;  /* �����̒����p */
    int i, j;  /* ����ϐ� */

    /* �������͑S���� index �r�b�g */
    byte_size = index / 8;        
    remain_bit_size = index % 8;  /* �[���̃r�b�g�� */
    byte_index = -1;
    for ( i = 0; i < byte_size; i ++ ){
        byte_data = 0;
        for ( j = 1; j <= 8; j ++ ){
            byte_index ++;
            byte_data = (unsigned char)(byte_data * 2
                + line_buffer[byte_index]);
        }
        fputc( byte_data, fp );  /* �t�@�C���o�� */
    }
    /* �ȉ��͒[���̃r�b�g������Ƃ��̏��� */
    if ( remain_bit_size != 0 ){
        byte_data = 0;
        for ( j = 1; j <= remain_bit_size; j ++ ){
            byte_index ++;
            byte_data = (unsigned char)(byte_data * 2
                + line_buffer[byte_index]);
        }
        byte_data = (unsigned char)( byte_data <<
            ( 8 - remain_bit_size ) );
        fputc( byte_data, fp );  /* �t�@�C���o�� */
    }
}

void code_and_save_runlength_data( )
/* �摜�̃��������O�X���������s���ăf�[�^��ۑ����� */
{
    int y, x; /* ����ϐ� */
    long int coded_data_size;     /* ���k�f�[�^��   */
    char file_name[MAX_FILENAME]; /* �o�̓t�@�C���� */
    int run_length;               /* ���������O�X   */

    printf("���摜�����������O�X���������ăf�[�^��ۑ����܂��D\n");
    printf("�o�̓t�@�C���� (*.wyl) : ");
    scanf("%s",file_name);    fp = fopen( file_name, "wb");
    /* �摜�̉����C�c���̏o�� */
    fprintf( fp, "%d %d\n", x_size1, y_size1 );
    /* ���������O�X������ */
    coded_data_size = 0;
    for ( y = 0; y < y_size1; y ++ ){
        /* �ō��̉�f�̊K���l���P�r�b�g�ŕ\������ */
        if ( image1[y][0] == 0 ) line_buffer[0] = 0;
        else line_buffer[0] = 1;
        index = 1; /* ���ɑ�����ׂ� line_buffer[ ] �̗v�f�ԍ� */
                  /* �ł���C�֐� Wyle_coding ���ōX�V�����  */
        /* ���P���C�����̕����� */
        x = 0;  run_length = 1;  /* �ϐ��̏����� */
        while ( x < x_size1 ){
                /* �ŉE��f�łȂ���f�ɑ΂��鏈�� */
            if ( x != x_size1 - 1 ){
                if ( image1[y][x] == image1[y][x+1] ){
                    run_length ++;
                } else {
                    Wyle_coding( run_length );
                    run_length = 1;
                }
            } else {
                /* �ŉE��f�̂Ƃ��̏��� */
                if ( image1[y][x] == image1[y][x-1] ){
                    Wyle_coding( run_length );
                } else {
                    Wyle_coding( 1 ); 
                }
            }
            x ++;
        } /* �ŉE�ɗ�����I�� */
        /* line_buffer[ ] �̃f�[�^���o�͂��� */
        save_line_buffer( );
        /* ���k�f�[�^�ʂ̍X�V */
        coded_data_size = coded_data_size + index;
    }
    fclose( fp );
    /* �f�[�^�ʁE���k���֌W�̌v�Z�ƕ\�� */
    printf("���̂Q�l�摜�̃f�[�^�� = %d x %d = %d bits\n",
        x_size1,y_size1,x_size1 * y_size1);
    printf("���k�f�[�^�i�摜�����j�̃f�[�^�� = %d bits\n",
        coded_data_size);
    printf("���k���F%f[��]\n", 100.0 * coded_data_size / 
        (x_size1 * y_size1) );
}

main( )
{
    load_image_data( );              /* ���摜�̓ǂݍ��� */
    code_and_save_runlength_data( ); /* �R�[�h���ƕۑ�   */
    return 0;
}
