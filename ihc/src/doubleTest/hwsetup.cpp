#include <cyg/hal/sh_regs.h>
#include <cyg/hal/hal_io.h>
#include "TMUCtrl.h"


#ifdef __cplusplus
extern "C" {
#endif
extern void HardwareSetup();
#ifdef __cplusplus
}
#endif

void HardwareSetup()
{
	//////////////////////////////////////////////////////////////////////////
	//SCIF èâä˙âªèàóù
	// 115200bps, 8data, 1stop, no parity
	// (on internal 33MHz P clock)

	HAL_WRITE_UINT16(CYGARC_REG_SCIF_SCSCR2, 0x0000);
	HAL_WRITE_UINT16(CYGARC_REG_SCIF_SCFCR2, 0x0006);
	HAL_WRITE_UINT16(CYGARC_REG_SCIF_SCSMR2, 0x0000);
	HAL_WRITE_UINT8 (CYGARC_REG_SCIF_SCBRR2, 0x08  );

	HAL_WRITE_UINT16(CYGARC_REG_TCR1,  0x0003    );
	HAL_WRITE_UINT32(CYGARC_REG_TCNT1, 0x00000001);

//	register cyg_uint8  _tstr_;
	register cyg_uint16 _tcr1_;
/*
	HAL_READ_UINT8(CYGARC_REG_TSTR, _tstr_);
	_tstr_ |= CYGARC_REG_TSTR_STR1;
	HAL_WRITE_UINT8(CYGARC_REG_TSTR, _tstr_);
	*/
	TMUStart(1);

	while(1){
		HAL_READ_UINT16(CYGARC_REG_TCR1, _tcr1_);
		if(_tcr1_ & CYGARC_REG_TCR_UNF)
			break;
	}

	HAL_WRITE_UINT16(CYGARC_REG_TCR1,  0x0000);
	/*
	_tstr_ &= ~CYGARC_REG_TSTR_STR1;
	HAL_WRITE_UINT8(CYGARC_REG_TSTR, _tstr_);
	*/
	TMUStop(1);

	HAL_WRITE_UINT16(CYGARC_REG_SCIF_SCFCR2, 0x0080);
	HAL_WRITE_UINT16(CYGARC_REG_SCIF_SCFSR2, 0x0060);
	HAL_WRITE_UINT16(CYGARC_REG_SCIF_SCSCR2, 0x0020);

}

