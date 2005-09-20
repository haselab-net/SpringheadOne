#include <cyg/hal/sh_regs.h>
#include <cyg/hal/hal_io.h>

void TMUStart(int);
void TMUStop(int);

cyg_uint8 tstr_str[3] = {CYGARC_REG_TSTR_STR0,
				         CYGARC_REG_TSTR_STR1,
				         CYGARC_REG_TSTR_STR2 };

void TMUStart(int num){
	register cyg_uint8 _tstr_;
	HAL_READ_UINT8(CYGARC_REG_TSTR, _tstr_);
	_tstr_ |= tstr_str[num];
	HAL_WRITE_UINT8(CYGARC_REG_TSTR, _tstr_);
}

void TMUStop(int num){
	register cyg_uint8 _tstr_;
	HAL_READ_UINT8(CYGARC_REG_TSTR, _tstr_);
	_tstr_ &= ~tstr_str[num];
	HAL_WRITE_UINT8(CYGARC_REG_TSTR, _tstr_);
}

