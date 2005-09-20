/* ���������O�X�������̃v���O���� rldecode.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

FILE *fp;                  /* �f�[�^����͂���t�@�C���|�C���^ */
int byte_index, bit_index; /* �f�[�^���͗p�o�C�g�C�r�b�g�ʒu   */
unsigned char byte_data;   /* �ǂݍ��񂾂P�o�C�g�̃f�[�^�p�ϐ� */

unsigned char get_one_bit( )
/* �t�@�C������P�r�b�g�̃f�[�^�����o�� */
{
    unsigned char work;  /* ��ƕϐ� */

    if ( bit_index == 9 ){
        byte_data = (unsigned char)fgetc( fp ); /* 1�o�C�g�ǂݍ��� */
        byte_index ++;    bit_index = 1;
    }
    work = (unsigned char)( byte_data << ( bit_index - 1 ) );
    work = (unsigned char)( work >> 7 );
    bit_index++;
    return work;
}

int get_runlength_data( )
/* �P�̃��������O�X�f�[�^��ǂݍ���ŕԂ� */
{
    unsigned char bit_data; /* 1�r�b�g�p�ϐ� */
    int length; /* ������ */
    int i, n;   /* ����E��Ɨp�ϐ� */

    /* ���������O�X�̒��������߂� */
    length = 1;
    do{
        bit_data = get_one_bit( );  length ++;
    } while ( bit_data != 0 );
    /* ���������O�X�f�[�^�����߂� */
    n = 0;
    for ( i = 1; i <= length; i ++ ){
        bit_data = get_one_bit( );
        n = n * 2 + bit_data;
    }
    n = n + 1;  /* = ���������O�X */
    return n;
}

void load_and_decode_runlength_data( )
/* ���������O�X�f�[�^�����[�h���ăf�R�[�h�����ĉ摜�ɒ��� */
{
    int y, x, i;                  /* ���W�p�ϐ��E����ϐ� */
    char file_name[MAX_FILENAME]; /* �o�̓t�@�C����       */
    int runlength;                /* ���������O�X         */
    int white_flag;               /* = 1 :��, = else :��  */

    printf("���������O�X�f�[�^�����[�h���ĉ摜�ɒ����܂��D\n");
    printf("���̓t�@�C���� (*.wyl) : ");
    scanf("%s", file_name);
    fp = fopen( file_name, "rb" );
    if ( NULL == fp ){
        printf("���̖��O�̃t�@�C���͑��݂��܂���D\n");
        exit(1);
    }
    /* �摜�̉����C�c���̏o�� */
    fscanf( fp, "%d %d\n", &x_size2, &y_size2 );
    printf("�摜�̉���f�� = %d, �c��f�� = %d\n", x_size2, y_size2);
    /* ���������O�X������ */
    for ( y = 0; y < y_size2; y ++ ){
        byte_index = 0;  bit_index = 9;  /* �ϐ��̏����� */
        /* �ŏ���1�r�b�g�͍ō��̉�f�̍��^���t���O */
        white_flag = get_one_bit( );
        /* 2�r�b�g�ڈȍ~�̏��� */
        x = -1; /* x���W�̏����� */
        while ( x < x_size2 - 1){
            /* �f�[�^��ǂݍ���Ń������ɕϊ� */
            runlength = get_runlength_data( );
            /* �摜�i���^����f��j�ɒ��� */
            for ( i = 0; i < runlength; i ++ ){
                x ++;  
                if ( white_flag )
                    image2[y][x] = MAX_BRIGHTNESS; /* �� */
                else image2[y][x] = 0; /* �� */
            }
            white_flag = ! white_flag; /* ���t���O�̔��] */
        }
    }
    fclose( fp );
}

main( )
{
    load_and_decode_runlength_data( );  /* �ǂݍ��݂ƃf�R�[�h */
    save_image_data( );                 /* �������摜�̕ۑ�   */
    return 0;
}
