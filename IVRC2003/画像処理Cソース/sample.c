/* 1����w�肵�����܂ł̘a����ʂɕ\������v���O���� sample.c */

/* �K�v�ȃw�b�_�t�@�C���̃C���N���[�h */
#include<stdio.h>   /* ��{���o�� */
#include<stdlib.h>  /* ��{���C�u���� */

/* �萔�̐錾�Ȃǁi�}�W�b�N�i���o�[�Ȃǁj */
#define  MIN_NUM  1         /* �萔�̐錾 */

/* ���ϐ��̐錾�i�ǂ�����ł��Q�Ƃł���j */
int   dummy1, dummy2;   /* �����^�̕ϐ��̐錾 */
float dummy3;           /* �����^�̕ϐ��̐錾 */
char  dummy4;           /* ������^�ϐ��̐錾 */

int calculate_sum( int num )
/* num �܂ł̘a�����߂�֐� */
{
    int  i;       /* for ���[�v�̐���ϐ� */
    int  sum;    /* �a�������鐮���^�ϐ� */ 

    if ( 100 < num || num <= MIN_NUM ){
        printf("���͂��ꂽ�������s�K�؂ł��D�I�����܂��D\n");
        exit(1);
    } else printf("�a���v�Z���܂��D\n");
    sum = 0;
    for ( i = MIN_NUM; i <= num; i++ )
        sum = sum + i;
    return sum;
}

main( )
{
    int  max_num, summation;

    printf("1 ����w�肵�������܂ł̘a�����߂�\n");
    printf("�����i2�ȏ�100�ȉ��j = ");
    scanf("%d", &max_num );
    summation = calculate_sum( max_num );
    printf("�a�� %d �ł��D\n", summation );
    return 0;
}
