/* 8�ߖT���k�����̃v���O���� erode.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"

void erosion( )
/* �Q�l�摜�̂W�ߖT���k����                             */
/* ���摜 image1[y][x] ===> �ϊ���̉摜 image2[y][x]   */
{
    int repetition;     /* ���k�����̉� */
    int counter;        /* �ߖT���̔w�i��f�̐� */
    int p_x, p_y;       /* �ߖT��(x,y)���W */
    int i, x, y, m, n;  /* ����ϐ� */

    x_size2 = x_size1;
    y_size2 = y_size1;
    for ( y = 0; y < y_size2; y ++ )
        for ( x = 0; x < x_size2; x ++ )
            image2[y][x] = image1[y][x];
    printf("�Q�l�摜�̂W�ߖT���k�������s���܂��D\n");
    printf("���k�������s���� = ");
    scanf("%d", &repetition);
    for ( i = 0; i < repetition; i ++ ){
        for ( y = 0; y < y_size1; y ++ ){
            for ( x = 0; x < x_size1; x ++ ){
                if ( image1[y][x] == MAX_BRIGHTNESS ){
                    counter = 0;
                    for ( m = -1; m < 2; m ++ ){
                        for ( n = -1; n < 2; n ++ ){
                            p_x = x + n;
                            p_y = y + m;
                            if ( p_x > 0 && p_x < x_size1 &&
                                 p_y > 0 && p_y < y_size1 &&
                                 image1[p_y][p_x] == 0 )
                                     counter ++;
                        }
                    }
                    if ( counter > 0 ) image2[y][x] = 0;
                }
            }
        }
        for ( y = 0; y < y_size2; y ++ )
            for ( x = 0; x < x_size2; x ++ )
                image1[y][x] = image2[y][x];
    }
}

main( )
{
    load_image_data( ); /* �摜��ǂݍ���� image1 �� */
    erosion( );        /* 8�ߖT���k�������� image2 �� */
    save_image_data( ); /* image2 ��ۑ����� */
    return 0;
}
