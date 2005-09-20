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

#define R_DAC_V0  (*(volatile unsigned short *)0xB0000000)		/* DAコンバータ0設定レジスタ */
#define R_DAC_V1  (*(volatile unsigned short *)0xB0000002)		/* DAコンバータ1設定レジスタ */
#define R_DAC_V2  (*(volatile unsigned short *)0xB0000004)		/* DAコンバータ2設定レジスタ */
#define R_DAC_V3  (*(volatile unsigned short *)0xB0000006)		/* DAコンバータ3設定レジスタ */
#define R_DAC_V4  (*(volatile unsigned short *)0xB0000008)		/* DAコンバータ4設定レジスタ */
#define R_DAC_V5  (*(volatile unsigned short *)0xB000000A)		/* DAコンバータ5設定レジスタ */
#define R_DAC_V6  (*(volatile unsigned short *)0xB000000C)		/* DAコンバータ6設定レジスタ */
#define R_DAC_V7  (*(volatile unsigned short *)0xB000000E)		/* DAコンバータ7設定レジスタ */

#define R_DAC_L0  (*(volatile unsigned short *)0xB0000010)		/* DAコンバータ0リミット設定レジスタ */
#define R_DAC_L1  (*(volatile unsigned short *)0xB0000012)		/* DAコンバータ1リミット設定レジスタ */
#define R_DAC_L2  (*(volatile unsigned short *)0xB0000014)		/* DAコンバータ2リミット設定レジスタ */
#define R_DAC_L3  (*(volatile unsigned short *)0xB0000016)		/* DAコンバータ3リミット設定レジスタ */
#define R_DAC_L4  (*(volatile unsigned short *)0xB0000018)		/* DAコンバータ4リミット設定レジスタ */
#define R_DAC_L5  (*(volatile unsigned short *)0xB000001A)		/* DAコンバータ5リミット設定レジスタ */
#define R_DAC_L6  (*(volatile unsigned short *)0xB000001C)		/* DAコンバータ6リミット設定レジスタ */
#define R_DAC_L7  (*(volatile unsigned short *)0xB000001E)		/* DAコンバータ7リミット設定レジスタ */

#ifdef _KATSU_TEST
//#define R_CTL     (*(volatile struct st_ctl  *)0x10000020)
#define R_CTL     (*(volatile union uni_ctl  *)0xB0000020)
#else
#define R_CTL     (*(volatile unsigned short *)0xB0000020)		/* コントロールレジスタ */
#endif //_KATSU_TEST

#define R_CNT_CLR (*(volatile unsigned short *)0xB0000022)		/* エンコーダカウンタクリア */
#define R_CNT_PHS (*(volatile unsigned short *)0xB0000024)		/* エンコーダ相順設定 */
#define R_CNT_HLD (*(volatile unsigned short *)0xB0000026)		/* エンコーダカウンタホールド */
#define R_RST     (*(volatile unsigned short *)0xB0000028)		/* リセットレジスタ */

#define R_CNT_V0  (*(volatile unsigned short *)0xB0000100)		/* エンコーダ0カウント値 */
#define R_CNT_V1  (*(volatile unsigned short *)0xB0000102)		/* エンコーダ1カウント値 */
#define R_CNT_V2  (*(volatile unsigned short *)0xB0000104)		/* エンコーダ2カウント値 */
#define R_CNT_V3  (*(volatile unsigned short *)0xB0000106)		/* エンコーダ3カウント値 */
#define R_CNT_V4  (*(volatile unsigned short *)0xB0000108)		/* エンコーダ4カウント値 */
#define R_CNT_V5  (*(volatile unsigned short *)0xB000010A)		/* エンコーダ5カウント値 */
#define R_CNT_V6  (*(volatile unsigned short *)0xB000010C)		/* エンコーダ6カウント値 */
#define R_CNT_V7  (*(volatile unsigned short *)0xB000010E)		/* エンコーダ7カウント値 */

#ifdef _KATSU_TEST
//#define R_STS     (*(volatile struct st_sts  *)0x10000110)
#define R_STS     (*(volatile union uni_sts  *)0xB0000110)
#else
#define R_STS     (*(volatile unsigned short *)0xB0000110)		/* ステータスレジスタ */
#endif //_KATSU_TEST

#define R_FIFO_R  (*(volatile unsigned short *)0xB0000180)		/* FIFOリードポート */
#define R_FIFO_W  (*(volatile unsigned short *)0xB0000182)		/* FIFOライトポート */

#define R_ADC    (*(volatile unsigned short *)0xB4000000)		/* ADコンバータリードポート */

#endif //_REGDEFS_H_
