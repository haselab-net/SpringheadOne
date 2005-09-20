///////////////////////////////////////////////////////////////////////////////
//
// regdefs.h
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _REGDEFS_H_
#define _REGDEFS_H_

//#define _KATSU_TEST

#ifdef _KATSU_TEST
// 
//struct st_ctl {
union uni_ctl{
	unsigned short WORD;
	struct {
		unsigned short :5;
		unsigned short IRL0_CLS:1;
		unsigned short :4;
		unsigned short W_FIFO_START:1;
		unsigned short R_FIFO_CLS:1;
		unsigned short ALL_ENC_CLS:1;
		unsigned short ALL_ENC_HOLD:1;
		unsigned short DA_CLS:1;
		unsigned short DA_SET:1;
	} BIT;
};
//};

// 
//struct st_sts{
union uni_sts{
	unsigned short WORD;
	struct {
		unsigned short :5;
		unsigned short IRL0_STS:1;
		unsigned short W_FIFO_BUZY:1;
		unsigned short R_FIFO_ENPTY:1;
		unsigned short ROT_SW:4;
		unsigned short :1;
		unsigned short EXT_SW2;
		unsigned short EXT_SW1;
		unsigned short EXT_SW0;
	} BIT;
};
//};

#endif //_KATSU_TEST

///////////////////////////////////////////////////////////////////////////////

#define R_DAC_V0  (*(volatile unsigned short *)0xB0000000)		/* DA�R���o�[�^0�ݒ背�W�X�^ */
#define R_DAC_V1  (*(volatile unsigned short *)0xB0000002)		/* DA�R���o�[�^1�ݒ背�W�X�^ */
#define R_DAC_V2  (*(volatile unsigned short *)0xB0000004)		/* DA�R���o�[�^2�ݒ背�W�X�^ */
#define R_DAC_V3  (*(volatile unsigned short *)0xB0000006)		/* DA�R���o�[�^3�ݒ背�W�X�^ */
#define R_DAC_V4  (*(volatile unsigned short *)0xB0000008)		/* DA�R���o�[�^4�ݒ背�W�X�^ */
#define R_DAC_V5  (*(volatile unsigned short *)0xB000000A)		/* DA�R���o�[�^5�ݒ背�W�X�^ */
#define R_DAC_V6  (*(volatile unsigned short *)0xB000000C)		/* DA�R���o�[�^6�ݒ背�W�X�^ */
#define R_DAC_V7  (*(volatile unsigned short *)0xB000000E)		/* DA�R���o�[�^7�ݒ背�W�X�^ */

#define R_DAC_L0  (*(volatile unsigned short *)0xB0000010)		/* DA�R���o�[�^0���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L1  (*(volatile unsigned short *)0xB0000012)		/* DA�R���o�[�^1���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L2  (*(volatile unsigned short *)0xB0000014)		/* DA�R���o�[�^2���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L3  (*(volatile unsigned short *)0xB0000016)		/* DA�R���o�[�^3���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L4  (*(volatile unsigned short *)0xB0000018)		/* DA�R���o�[�^4���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L5  (*(volatile unsigned short *)0xB000001A)		/* DA�R���o�[�^5���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L6  (*(volatile unsigned short *)0xB000001C)		/* DA�R���o�[�^6���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L7  (*(volatile unsigned short *)0xB000001E)		/* DA�R���o�[�^7���~�b�g�ݒ背�W�X�^ */

#ifdef _KATSU_TEST
//#define R_CTL     (*(volatile struct st_ctl  *)0x10000020)
#define R_CTL     (*(volatile union uni_ctl  *)0xB0000020)
#else
#define R_CTL     (*(volatile unsigned short *)0xB0000020)		/* �R���g���[�����W�X�^ */
#endif //_KATSU_TEST

#define R_CNT_CLR (*(volatile unsigned short *)0xB0000022)		/* �G���R�[�_�J�E���^�N���A */
#define R_CNT_PHS (*(volatile unsigned short *)0xB0000024)		/* �G���R�[�_�����ݒ� */
#define R_CNT_HLD (*(volatile unsigned short *)0xB0000026)		/* �G���R�[�_�J�E���^�z�[���h */
#define R_RST     (*(volatile unsigned short *)0xB0000028)		/* ���Z�b�g���W�X�^ */

#define R_CNT_V0  (*(volatile unsigned short *)0xB0000100)		/* �G���R�[�_0�J�E���g�l */
#define R_CNT_V1  (*(volatile unsigned short *)0xB0000102)		/* �G���R�[�_1�J�E���g�l */
#define R_CNT_V2  (*(volatile unsigned short *)0xB0000104)		/* �G���R�[�_2�J�E���g�l */
#define R_CNT_V3  (*(volatile unsigned short *)0xB0000106)		/* �G���R�[�_3�J�E���g�l */
#define R_CNT_V4  (*(volatile unsigned short *)0xB0000108)		/* �G���R�[�_4�J�E���g�l */
#define R_CNT_V5  (*(volatile unsigned short *)0xB000010A)		/* �G���R�[�_5�J�E���g�l */
#define R_CNT_V6  (*(volatile unsigned short *)0xB000010C)		/* �G���R�[�_6�J�E���g�l */
#define R_CNT_V7  (*(volatile unsigned short *)0xB000010E)		/* �G���R�[�_7�J�E���g�l */

#ifdef _KATSU_TEST
//#define R_STS     (*(volatile struct st_sts  *)0x10000110)
#define R_STS     (*(volatile union uni_sts  *)0xB0000110)
#else
#define R_STS     (*(volatile unsigned short *)0xB0000110)		/* �X�e�[�^�X���W�X�^ */
#endif //_KATSU_TEST

#define R_FIFO_R  (*(volatile unsigned short *)0xB0000180)		/* FIFO���[�h�|�[�g */
#define R_FIFO_W  (*(volatile unsigned short *)0xB0000182)		/* FIFO���C�g�|�[�g */

#define R_ADC    (*(volatile unsigned short *)0xB4000000)		/* AD�R���o�[�^���[�h�|�[�g */

#endif //_REGDEFS_H_
