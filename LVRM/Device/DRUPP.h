#ifndef UPP_H
#define UPP_H
#include "../Base/Debug.h"
#include <vector>

namespace LVRM {

/**	UPP(Universal Pulse Processor)�̃h���C�o�[.
	CRatorUppCard, CPCCardDriver���g��	*/
class DRUpp{
public:
	class UppFunc
	{
	public:
		///	UPP�̖��ߌ�
		enum {	FRS = 0x00, INS = 0x10, UDS = 0x20, GTS = 0x30,
			FRC = 0x40, INC = 0x50, PWC = 0x60, OSC = 0x70,
			FFC = 0x80, TPC = 0x98, GTC = 0xA0, CTO = 0xB0,
			SIT = 0xC0, SOT = 0xD0, SPO = 0xE0, NOP = 0xF0};
		///	UPP�̖��ߌ�̃I�v�V����
		enum {};
		///	�[�q�̃G�b�W���o
		enum {};
		enum {IDLEN = 20};
		///	���߂����ʂ��邽�߂�ID�A����̖��߂𖽗ߕ\����폜���邽�߂ɗp����
		char id[IDLEN+1];
			
		///	UPP����
		int cmd, ar, br, ioa, iob, ioc, iod;
	
		/**	�R���X�g���N�^.
		@param aCmd		����
		@param anAr		A���W�X�^
		@param anBr		B���W�X�^
		@param anIoa	�[�qA
		@param anIob	�[�qB
		@param anIoc	�[�qC
		@param anIod	�[�qD
		@param anId		���ʕ�����(�ȗ���)	*/
		UppFunc(int aCmd = NOP, int anAr = 0, int anBr = 0,
			int anIoa = 0, int anIob = 0, int anIoc = 0, int anIod = 0, char* anId = "");
		bool operator ==(const UppFunc& f) const;
		bool operator <(const UppFunc& f) const;
		bool operator >(const UppFunc& f) const;
		bool operator !=(const UppFunc& f) const;
	};
protected:
	///	UPP�������W�X�^�̃A�h���X
	enum {PORT1 = 0x3, PORT2 = 0x2, UDR_BASE=0x40};
	///	�������ς݂Ȃ� true.
	bool bInitalized;
	///	FNR�̕\

	static int FNR[17];
	///	UPP�̖��ߕ\��UPP�ɐݒ肷��
	void SetFunc();
	///	UPP�̖��ߕ\

	std::vector<UppFunc> func;
public:
	/// 
	DRUpp();
	/// 
	virtual bool Init();
	///	UPP�̃��W�X�^�̃A�h���X a �ɒl v ����������
	virtual void Out(unsigned a, unsigned v)=0;
	///	UPP�̃��W�X�^�̃A�h���X a �̒l��ǂݏo��
	virtual int In(unsigned a)=0;
	///	UPP�̎��s��Ԃ̎擾
	bool	Run();
	/**	UPP�̏�Ԃ�ݒ肷��.
	@param b �ݒ肵�������. true:���s��� false:��~���	*/
	void	Run(bool b);
	///	���ߕ\�ɖ��߂�ǉ�����
	void	Add(const UppFunc& f);
	///	���ߕ\���疽�߂��폜����
	void	Del(char* id);
	///	���ߕ\���疽�߂��폜����
	void	Del(int id);
	
	/**	UPP�̒[�q���g�p���郆�j�b�g�̐ݒ�.
	1bit�ڂ��|�[�g0�ɁA16bit�ڂ��|�[�g0�ɑΉ�����B
    \begin{verbatim}
	UPP�̒[�q�ƃr�b�g�̊��蓖��
	0xABCD
	dir  MSB 0x   A    B    C    D LSB
	port       FEDC BA98 7654 3210
	\end{verbatim}
	0: PIO���g�p	1: UPP���g�p	*/
	void	PortOwner(unsigned owner = 0xFFFF);
	/**	UPP�̒[�q�̓��o�͂̐ݒ�.
	1bit�ڂ��|�[�g0�ɁA16bit�ڂ��|�[�g0�ɑΉ�����B
    \begin{verbatim}
	UPP�̒[�q�ƃr�b�g�̊��蓖��
	0xABCD
	dir  MSB 0x   A    B    C    D LSB
	port       FEDC BA98 7654 3210
	\end{verbatim}
	0: ����	1: �o��	*/
	void	PortDir(unsigned dir = 0x0000);
	///	PIO�̒l�����(PortOwner��PIO�ɂȂ��Ă���[�q�̂ݎg�p�ł���)
	unsigned short PioRead();
	///	PIO�֒l���o��(PortOwner��PIO�ɂȂ��Ă���[�q�̂ݎg�p�ł���)
	void	PioWrite(unsigned short d);
	/**	UPP Data Register �̒l��ݒ�.
	@param	no	UDR�̔ԍ�(0-23)
	@param	v	�l	*/
	void	UdrWrite(unsigned no,unsigned short v);
	/**	UPP Data Register �̒l�̓ǂݏo��.
	@param	no	UDR�̔ԍ�(0-23)
	@return		�l	*/
	unsigned short	UdrRead(unsigned no);
};
}	//	namespace LVRM
#endif
