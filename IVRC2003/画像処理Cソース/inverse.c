/* �摜�𔽓]������v���O���� inverse.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

void make_inverse_image( )
/* ���摜 image1 �𔽓]������ image2 �ɑ������ */
{
    int x, y;    /* ����ϐ� */

    printf("���]�摜���쐬���܂��D\n");
    x_size2 = x_size1;  /* ����f�� */
    y_size2 = y_size1;  /* �c��f�� */
    for ( y = 0; y < y_size2; y ++ ){
        for ( x = 0; x < x_size2; x ++ ){
            image2[y][x] = (unsigned char)
                           (MAX_BRIGHTNESS - image1[y][x]);
        }
    }
}

main( )
{
    load_image_data( );    /* �摜�f�[�^�� image1 �ɓǂݍ��� */
    make_inverse_image( ); /* image1 �𔽓]���� image2 �� */
    save_image_data( );    /* image2 ��ۑ����� */
    return 0;
}
