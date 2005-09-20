/***********************************************************************/
/*                                                                     */
/*  FILE        :qp_prog01.h                                           */
/*  DATE        :2002/01/29                                            */
/*  DESCRIPTION :Quadratic Programming (Active Set Method)             */
/*                                                                     */
/*  Katsuhito AKAHANE                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef _QP_PROG01_H_
#define _QP_PROG01_H_

//
// f(x) = (1/2)x'Qx - c'x    ->minimize
// minX <= x <= maxX
//


// ���ӁI�I�I
// �g���O�ɁA�����̐ݒ��Y��Ȃ����ƁI�I�I
#define N 4					//�񎟌v��̎�����

extern float gVecX[N];		//out:�œK��

extern float gMatQ[N][N];	//in:�񎟌v��̂Q���̌W��
extern float gVecC[N];		//in:�񎟌v��̂P���̌W��

//
// �񎟌v��@�̏����������֐�
// �{�b�N�X�����̐ݒ� (minTin <= X <= maxTin)
extern void InitQuadpr(float minTin, float maxTin);

//
// �񎟌v��@���C���֐�
// ���̊֐����Ăяo���O��
// InitQuadpr() ���Ăяo��
// gMatQ[N][N]
// gVecC[N]
// �̐ݒ�����邱��
// �܂��AgMatQ,gVecC �́A�v�Z�̑O��ŕύX����邱�Ƃ͂Ȃ��B
extern int quadpr();// �����T���_��O��̉��ɂ���
extern int quadpr2();// �����T���_�𖳏����œK���̋ߖT�ɂ���

#endif //_QP_PROG01_H_

