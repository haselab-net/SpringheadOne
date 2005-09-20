/* Hough �t�ϊ����s���Ē����𒊏o����v���O���� invHough.c */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mypgm.h"

#define PI  3.141592653589 /* �~���� */
#define MAX_THETA   360    /* �Ǝ��̃T�C�Y�i�P[deg]=�P��f�j*/
#define MAX_RHO     720    /* �ώ��̃T�C�Y                  */

void swap_int( int *n1, int *n2 )
/* int n1 �� int n2 �����ւ��� */
{
    int n;  /* ��ƕϐ� */

    n = *n1;    *n1 = *n2;    *n2 = n;
}

void extract_line( int xs, int ys )
/* �ƃϕ��ʏ�iimage1��j�̓_(xs,ys)���摜��̒����ɋt�ϊ����C   */
/* ���ʂ� image2[y][x] �ɕ`��                                    */
{
    double theta, rho, rho_max, _sin, _cos;  /* ��ƕϐ� */
    int x, y;  /* ����ϐ� */

    theta = 180.0 * xs / MAX_THETA;  /* [deg] */
    _sin = sin( theta / 180.0 * PI );
    _cos = cos( theta / 180.0 * PI );
    rho_max = 1.0 / 1.5 * x_size2 + 1.0 / 1.5 * y_size2;
    rho = ( ys - MAX_RHO / 2.0 ) * rho_max / MAX_RHO * 2.0; 
    /*  rho = x _cos + y _sin  --->  y = rho/_sin - x _cos/_sin  */
    if ( 45.0 <= theta && theta <= 135.0 ){ /* ���l�̒P�ʂ�[deg] */
        for ( x = 0; x < x_size2; x ++ ){
            y = (int)( ( rho - x * _cos ) / _sin );
            if ( 0 <= y && y < y_size2 )
                image2[y][x] = MAX_BRIGHTNESS; 
        }
    } else {
        for ( y = 0; y < y_size2; y ++ ){
            x = (int)( ( rho - y * _sin ) / _cos );
            if ( 0 <= x && x < x_size2 )
                image2[y][x] = MAX_BRIGHTNESS;
        }
    }
}

void inverse_Hough_transform( )
/* image1 �� �ƃϕ��ʂƂ݂Ȃ��C�������l����̓_���摜��Ԃ� */
/* �t�ϊ����ĉ摜���ɒ�����`���D���ʂ� image2 �ɑ������D   */
{
    int threshold,counter;  /* �������l�C�����̃J�E���^ */
    int x,y;  /* ����ϐ� */
    char str[10];  /* ��ƕϐ� */

    printf("�ǂݍ��񂾃ƃϕ��ʂ���C�摜��̒������Č����܂��D\n");
    do {
        printf("�ƃϕ��ʏ�̗ݐςɑ΂��邵�����l (0�`255�̒l�j= ");
        scanf("%d",&threshold);
        counter = 0;
        for ( y = 0; y < MAX_RHO;  y ++ )
            for ( x = 0; x < MAX_THETA; x ++ )
                if ( image1[y][x] >= threshold ) counter ++;
        printf("�S���� %d �{�̒��������o����܂����D\n", counter);
        printf("���̒l�� OK ? ( y / n ) :");
        scanf("%s",str);
    } while ( str[0] != 'y' && str[0] != 'Y' );
    /* �t�ϊ������̎��s */
    for ( y = 0; y < MAX_RHO; y ++ )
        for ( x = 0; x < MAX_THETA; x ++ )
            if ( image1[y][x] >= threshold )
                extract_line( x, y );
}

void move_image1_to_image2( )
/* ���摜�f�[�^��image2[y][x]�ɑޔ����� */
/* ���̍ہC�K���𗎂Ƃ� (=80)          */
{
    int x, y;  /* ����ϐ� */

    x_size2 = x_size1;    y_size2 = y_size1;
    for ( y = 0; y < y_size1; y ++ )
        for ( x = 0; x < x_size1; x ++ ){
            image2[y][x] = image1[y][x];
            if ( image2[y][x] == MAX_BRIGHTNESS )
                image2[y][x] = 80;
        }
}

void check_data( )
{
    if ( x_size1 != MAX_THETA || y_size1 != MAX_RHO ){
        printf("����͑ΏۂƂ���ƃϕ��ʂ̃f�[�^�ƈقȂ�܂��D\n");
        exit(1);
    }
}

main( )
{
    printf("�܂��C���̉摜�f�[�^��ǂݍ��݂܂��D\n");
    load_image_data( );  /* ���摜�� image1 �� */
    move_image1_to_image2( );  /* image1 �� image2 �փR�s�[ */
    printf("���������ƃϕ��ʂ̃f�[�^��ǂݍ��݂܂��D\n");
    load_image_data( );  /* �ƃϕ��ʂ̃f�[�^�� image1 �� */
    check_data( );  /* �f�[�^�̃`�F�b�N */
    inverse_Hough_transform( );  /* Hough�t�ϊ����Ē������o */
    save_image_data( );  /* image2 ��ۑ� */
    return 0;
}
