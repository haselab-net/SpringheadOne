#ifndef IHREGDEFS_H_
#define IHREGDEFS_H_

///////////////////////////////////////////////////////////////////////////////
#define R_DAC_V		((volatile unsigned short *)0xB0000000)			/* DA�R���o�[�^0�ݒ背�W�X�^ */
#define R_DAC_V0	(*(volatile unsigned short *)0xB0000000)		/* DA�R���o�[�^0�ݒ背�W�X�^ */
#define R_DAC_V1	(*(volatile unsigned short *)0xB0000002)		/* DA�R���o�[�^1�ݒ背�W�X�^ */
#define R_DAC_V2	(*(volatile unsigned short *)0xB0000004)		/* DA�R���o�[�^2�ݒ背�W�X�^ */
#define R_DAC_V3	(*(volatile unsigned short *)0xB0000006)		/* DA�R���o�[�^3�ݒ背�W�X�^ */
#define R_DAC_V4	(*(volatile unsigned short *)0xB0000008)		/* DA�R���o�[�^4�ݒ背�W�X�^ */
#define R_DAC_V5	(*(volatile unsigned short *)0xB000000A)		/* DA�R���o�[�^5�ݒ背�W�X�^ */
#define R_DAC_V6	(*(volatile unsigned short *)0xB000000C)		/* DA�R���o�[�^6�ݒ背�W�X�^ */
#define R_DAC_V7	(*(volatile unsigned short *)0xB000000E)		/* DA�R���o�[�^7�ݒ背�W�X�^ */

#define R_DAC_L		((volatile unsigned short *)0xB0000010)			/* DA�R���o�[�^0���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L0	(*(volatile unsigned short *)0xB0000010)		/* DA�R���o�[�^0���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L1	(*(volatile unsigned short *)0xB0000012)		/* DA�R���o�[�^1���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L2	(*(volatile unsigned short *)0xB0000014)		/* DA�R���o�[�^2���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L3	(*(volatile unsigned short *)0xB0000016)		/* DA�R���o�[�^3���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L4	(*(volatile unsigned short *)0xB0000018)		/* DA�R���o�[�^4���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L5	(*(volatile unsigned short *)0xB000001A)		/* DA�R���o�[�^5���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L6	(*(volatile unsigned short *)0xB000001C)		/* DA�R���o�[�^6���~�b�g�ݒ背�W�X�^ */
#define R_DAC_L7	(*(volatile unsigned short *)0xB000001E)		/* DA�R���o�[�^7���~�b�g�ݒ背�W�X�^ */

#define R_CTL		(*(volatile unsigned short *)0xB0000020)		/* �R���g���[�����W�X�^ */

#define R_CNT_CLR	(*(volatile unsigned short *)0xB0000022)		/* �G���R�[�_�J�E���^�N���A */
#define R_CNT_PHS	(*(volatile unsigned short *)0xB0000024)		/* �G���R�[�_�����ݒ� */
#define R_CNT_HLD	(*(volatile unsigned short *)0xB0000026)		/* �G���R�[�_�J�E���^�z�[���h */
#define R_RST		(*(volatile unsigned short *)0xB0000028)		/* ���Z�b�g���W�X�^ */

#define R_CNT_V		((volatile unsigned short *)0xB0000100)			/* �G���R�[�_0�J�E���g�l */
#define R_CNT_V0	(*(volatile unsigned short *)0xB0000100)		/* �G���R�[�_0�J�E���g�l */
#define R_CNT_V1	(*(volatile unsigned short *)0xB0000102)		/* �G���R�[�_1�J�E���g�l */
#define R_CNT_V2	(*(volatile unsigned short *)0xB0000104)		/* �G���R�[�_2�J�E���g�l */
#define R_CNT_V3	(*(volatile unsigned short *)0xB0000106)		/* �G���R�[�_3�J�E���g�l */
#define R_CNT_V4	(*(volatile unsigned short *)0xB0000108)		/* �G���R�[�_4�J�E���g�l */
#define R_CNT_V5	(*(volatile unsigned short *)0xB000010A)		/* �G���R�[�_5�J�E���g�l */
#define R_CNT_V6	(*(volatile unsigned short *)0xB000010C)		/* �G���R�[�_6�J�E���g�l */
#define R_CNT_V7	(*(volatile unsigned short *)0xB000010E)		/* �G���R�[�_7�J�E���g�l */

#define R_STS		(*(volatile unsigned short *)0xB0000110)		/* �X�e�[�^�X���W�X�^ */

#define R_FIFO_R	(*(volatile unsigned short *)0xB0000180)		/* FIFO���[�h�|�[�g */
#define R_FIFO_W	(*(volatile unsigned short *)0xB0000182)		/* FIFO���C�g�|�[�g */

#define R_ADC		(*(volatile unsigned short *)0xB4000000)		/* AD�R���o�[�^���[�h�|�[�g */


#endif //	IHREGDEFS_H_
