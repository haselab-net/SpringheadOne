/* �摜���Ȉ�3�����\������v���O���� draw3D.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"

void draw_a_straight_line( int x1, int y1, int x2, int y2, int brightness )
/* image2[y][x] ���(x1,y1)�C(x2,y2)�Ԃ̒�����`�悷��D */
/* ���̖��邳�� brightness (0�`255�j�Ŏw�肷��D         */
{
    double distance, step, t;   /* t:�����_�����p����ϐ� */
    int x, y;  /* �`���_�̍��W */
    
    distance = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ); /* 2�_�Ԃ̋��� */
    step = 1.0 / ( 1.5 * distance );  /* ��������������_�̑��� */
    t = 0.0;
    while ( t < 1.0 ){
        x = (int)( t * x2 + ( 1.0 - t ) * x1 );
        y = (int)( t * y2 + ( 1.0 - t ) * y1 );
        if ( 0 <= x && x < x_size2 && 0 <= y && y < y_size2 )
            image2[y][x] = (unsigned char)brightness; 
        t = t + step;
    }
}

void draw_3D_graph( )
{
    int y, x;           /* ���[�v�ϐ� */
    int plot_step;      /* ���̕`��Ԋu [��f] 2�`10 ���x    */
    float plot_ratio;  /* �K�������̔{�� 0.0�`1.0(=255��f) */
    int plot_height;  /* �K�������̍��� = 256.0 * plot_ratio */

    x_size2 = x_size1 + y_size1 / 2;   /* �O���t�̑z�艡���i��f�j*/
    y_size2 = y_size1 + 256;           /* �O���t�̑z��c���i��f�j*/
    if ( x_size2 > MAX_IMAGESIZE || y_size2 > MAX_IMAGESIZE ){
        printf("�摜���z��ő�T�C�Y�𒴂��Ă��܂��D\n");
        exit(1);
    } else {
        /* �`��p�����[�^( plot_step, plot_ratio )�̎w�� */
        printf("���̊Ԋu�i��f�C2�`10���x�j�F");
        scanf("%d",&plot_step);
        printf("�K�������̔{��(0�ȏ�1�ȉ��j�F");
        scanf("%f",&plot_ratio);
        plot_height = (int)( 256.0 * plot_ratio );
        y_size2 = y_size1 + plot_height;
        /* �`��G���A( image2[y][x] )�̏����� */
        for ( y = 0; y < y_size2; y ++ )
            for ( x = 0; x < x_size2; x ++ )
                image2[y][x] = 0;
        /* �O���t�̕\�� */
        printf("�摜�������ł��D���΂炭���҂��������D\n");
        for ( y = 0; y < y_size1; y ++ ){
            if ( y % plot_step == 0 ){
                /* y �� plot_step �Ŋ���؂��Ƃ������`�� */
                /* �n�߂ɋȐ����������ŕ`���ĉB�ʏ������� */
                for ( x = 0; x < x_size1; x ++ ){
                    draw_a_straight_line( x + y / 2, plot_height + y,
                        x + y / 2, 
                   plot_height + y - (int)(image1[y][x] * plot_ratio), 0 );
                }
                /* �Ȑ������𔒂ŕ`�� */
                for ( x = 0; x < x_size1 - 1; x ++ ){
                    draw_a_straight_line( x + y / 2, 
                    plot_height + y - (int)(image1[y][x] * plot_ratio),
                    x + y / 2 + 1, 
                    plot_height + y - (int)(image1[y][x+1] * plot_ratio), 
                    MAX_BRIGHTNESS );
                }
            }
        }
    }
}

main( )
{
    load_image_data( );    /* ���摜�̓ǂݍ��� */
    draw_3D_graph( );      /* �R�����O���t�摜�̐��� */
    save_image_data( );    /* �摜�̏o�� */
    return 0;
}
