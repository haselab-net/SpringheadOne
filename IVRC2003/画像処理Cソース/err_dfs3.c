/* �덷�g�U�@�̃v���O���� err_dfs3.c */
#include<stdio.h>
#include<stdlib.h>
#include"mypgm.h"
#include"myutil.h"

void make_error_difusion_image( )
/* ���摜 image1[y][x] �̌덷�g�U�摜����� image2[y][x] �ɑ��  */
{
    int x, y;  /* ���[�v�ϐ� */
    double err1, err2, err3;  /* �K���덷 */
    double buf[2][MAX_IMAGESIZE]; /* �덷�ۑ��p�o�b�t�@ */
    double modified_gray;

    /* �덷�g�U�摜����� */
    printf("�덷�g�U�摜�����܂��D\n");
    x_size2 = x_size1;    y_size2 = y_size1;
        /* �Q���C�����̃o�b�t�@�̃N���A */
    for ( y = 0; y < 2; y ++ )
        for ( x = 0; x < x_size1; x ++ )
            buf[y][x] = 0.0;
    for ( y = 0; y < y_size1; y ++ ){
        for ( x = 0; x < x_size1; x ++ ){
            /* �ߖT���O�ɏo��Ƃ��͒P���Q�l������ */
            if ( x < 2 || x > x_size1 - 3 || y == y_size1 - 1 ){
                if ( image1[y][x] <= MAX_BRIGHTNESS / 2) 
                    image2[y][x] = 0;
                else image2[y][x] = MAX_BRIGHTNESS;
            } else {
                modified_gray = image1[y][x] + buf[0][x];
                err1 = modified_gray - 0.0;
                err2 = modified_gray - MAX_BRIGHTNESS;
                if ( err1 * err1 < err2 * err2 ){
                    image2[y][x] = 0;    
                    err3 = err1;
                } else {
                    image2[y][x] = MAX_BRIGHTNESS;  
                    err3 = err2;
                }
                /* �ߖT�ɉ����Ď��̎���ς��� */
                buf[0][x+1] = buf[0][x+1] + err3 * 2.0 / 10.0;
                buf[0][x+2] = buf[0][x+2] + err3 * 1.0 / 10.0;
                buf[1][x+2] = buf[1][x+2] + err3 * 1.0 / 10.0;
                buf[1][x+1] = buf[1][x+1] + err3 * 2.0 / 10.0;
                buf[1][x]   = buf[1][x]   + err3 * 2.0 / 10.0;
                buf[1][x-1] = buf[1][x-1] + err3 * 1.0 / 10.0;
                buf[1][x-2] = buf[1][x-2] + err3 * 1.0 / 10.0;
            }
        }
        for ( x = 0; x < x_size1; x ++ ){
            buf[0][x] = buf[1][x];
            buf[1][x] = 0.0;
        }
    }
}

main( )
{
    load_image_data( );         /* �摜�f�[�^�� image1 �ɓǂݍ��� */
    make_error_difusion_image( ); /* �덷�g�U�摜���쐬����*/
    save_image_data( );           /* image2 ��ۑ����� */
    return 0;
}
