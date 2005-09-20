/* �}�X�N�摜�ňړ����̂𒊏o����v���O���� extract.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

void copy_image1_to_image2( )
/* image1 �̉摜�f�[�^�� image2 �փR�s�[���� */
{
    int x, y;  /* ���[�v�ϐ� */

    x_size2 = x_size1;  y_size2 = y_size1;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            image2[y][x] = image1[y][x];
}

void extract_image( )
{
    int x, y;  /* ���[�v�ϐ� */

    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            if ( image1[y][x] == 0 ) image2[y][x] = 0;
}

main( )
{
    printf("*** ���摜�̃f�[�^��ǂݍ��݂܂��D\n");
    load_image_data( );        /* �摜1 --> image1      */
    copy_image1_to_image2( );  /* �摜1 --> image2      */
    printf("*** �}�X�N�摜�̃f�[�^��ǂݍ��݂܂��D\n");
    load_image_data( );        /* �摜2 --> image1      */
    extract_image( );          /* �̈撊�o���� image2 �� */
    printf("*** ���o�摜�̃f�[�^��ۑ����܂��D\n");
    save_image_data( );        /* image2 --> �o��      */
    return 0;
}
