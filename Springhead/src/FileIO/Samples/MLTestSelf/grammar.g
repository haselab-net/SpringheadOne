/*	���̃t�@�C���͈�ʃp�[�T(GenParser)�̂��߂̕��@��`�t�@�C���ł��D
	
�E�@�\�ɂ���
	��ʃp�[�T�́C���̃t�@�C����(���@��`�t�@�C��)�����[�h���āC�p�[�T
	�𐶐����܂��D�������ꂽ�p�[�T���C�^�[�Q�b�g�t�@�C�������[�h���܂��D
	���[�h����ƁC��ʕ����m�[�h�c���[(DOM���ǂ�)����������܂��D
	���[�U�͈�ʕ����m�[�h�c���[��ړI�̃f�[�^�\���ɕϊ����Ďg�p����
	���ƂɂȂ�܂��D
	
�E�p�[�T�̍\���ɂ���
	�p�[�T�̓t�@�C����ǂ݂Ȃ���C�A�N�V�������s���C��ʕ����m�[�h�𐶐�
	���Ă����܂��D�p�[�T�ɂ̓f�[�^�X�^�b�N������܂��D
	�܂��C�����̒ǉ��Ȃǂ̑���ΏۂƂȂ�C�u���݂̃m�[�h�v�Ƃ�����������܂��D

�E���@�ɂ���
	BNF�L�@�ŕ��@���L�q����̂���{�ł��D
	START ���J�n�L���ł�(START�͗\���)�D
	�I�[�L���ɂ́C���̂��̂�����܂��D
	  ""�ň͂܂ꂽ������:	�������ǂݏo���܂��D
	  IDSTR	:				ID�������ǂݏo���܂��D[A-Za-z_][A-Za-z_]*
	  NUMBER:				���l��ǂݏo���܂��D
	���K���@�I�ȏ��������ł��܂��D
	  A*	:				A��0��ȏ�̌J��Ԃ�
	  A+	:				A��1��ȏ�̌J��Ԃ�

	�܂��C�ǂݏo�����Ƃ��̓����BNF�̒��Ŏw�肵�܂��D�X�^�b�N(dataStack)
	�ɑ΂��鑀��ƁC��ʕ����m�[�h(DOM���ǂ�)�ɑ΂��鑀�삪�ł��܂��D
	  ''�ň͂܂ꂽ������:	��������X�^�b�N�ɐς݂܂��D
	  $������	:			�ϐ����X�^�b�N�ɐς݂܂��D�����񂪕ϐ����ɂȂ�܂��D
	  IDSTR	:				�ǂݏo������������X�^�b�N�ɐς݂܂��D
	  NUMBER:				�ǂݏo�������l���X�^�b�N�ɐς݂܂��D
	  {		:				�m�[�h���쐬���܂��D�쐬�����m�[�h�����݂̃m�[�h��
	  						�Ȃ�܂��D
	  }		:				�쐬�����m�[�h����āC�X�^�b�N�ɐς݂܂��D
	  						���݂̃m�[�h�͍쐬�O�̏�Ԃɖ߂�܂��D
	  =		:				�X�^�b�N����C2��(A,B)�����o���āC������B��
	  						�lA���Ō�ɍ쐬�����m�[�h�ɐݒ肵�܂��D
	  						= �͉E�ӂ̂��Ƃŏ�������܂��D
	  +=	:				�X�^�b�N����C2��(A,B)�����o���āC������B��
	  						�lA���Ō�ɍ쐬�����m�[�h�ɒǉ����܂��D
	  						+= �͉E�ӂ̂��Ƃŏ�������܂��D
	  [��]	:				�����v�Z�������ʂ��X�^�b�N�ɐς݂܂��D
	  						���ł́C������Ɛ��l���������Ƃ��ł��܂��D
	  						������̑����Z�͘A���ɂȂ�܂��D
	  UNSET					�X�^�b�N����1��(A)�����o���āC������A���N���A���܂��D

	�R�����g�ƃz���C�g�X�y�[�X
		�R�����g�ƃz���C�g�X�y�[�X�� COMMENT, WHITE �Ɏw�肵�܂��D
		
	�e���v���[�g
		�e���v���[�g�́C���@�𑝂₷���@�ł��D
		�e���v���[�g��`�̗�F
		TEMPLATE	TNewType:	//	TNewType �̓e���v���[�g��
				"TNewType"/ 'children' += {
					'type'='TNewType' ('name'=IDSTR|) "{" "}" } /
				SELECT('def')(  )
				;
		�e���v���[�g�g�p�̗�F
		template	:	"template" 'def'+={
					$tplType = IDSTR /
					'TNewType'=$tplType
					CREATE_INSTANCE('TNewType' 'data')	}/;	
					//	TNewType �Ƃ����e���v���[�g���̃e���v���[�g��
					//	data �ɒǉ�����D
					//	CREATE_INSTANCE ����Ƃ��̌��݂̃m�[�h�̑����́C
					//	�e���v���[�g���̕������u��������D
					//	�Ⴆ�΁C�e���v���[�g���� TNewType �́C
					//	IDSTR �ɒu����������D
	
	�֐����t�@�����X
	CREATE_INSTANCE(tmpl, node)
		tmpl �̃C���X�^���X���쐬���Cnode�ɉ����܂��D

	UNTIL(from=0, delta=1, end)
		from ���� delta �Â� end �܂ŌJ��Ԃ��܂��D
	
	


*/


COMMENT		:	"/*" "*/" | "//" "\n" | "#" "\n";
WHITE		:	" " "\t" "\n";

START		:	{ sentence* }/;
sentence	:	'sentence' += { sentenceId ":"/ select ";" }/;
sentenceId	:	'name' = IDSTR | "TEMPLATE" 'name' = IDSTR 'template'='true';
select		:	'series'+={series} selcont*;
selcont		:	"|"/ 'series'+={series};
series		:	('repeat'+={repeat})*;
repeat		:	'phrase'={phrase}	(	"*" 'repNum'='zeroMore'
								|	"+" 'repNum'='oneMore'
								|	'repNum'='once'	);
phrase		:	"/" 'op'='fix'
			|	'left'={left} "="	'right'={right} 'op'='assign'
			|	'left'={left} "+="	'right'={right} 'op'='append'
			|						'right'={right} 'op'='none';

arg			:	'arg' += (SQSTR | NUMBER);
left		:	"[" 'left'={form} "]" 'type'='form'
			|	'value'=SQSTR 'type'='sqstr'
			|	"$" 'value'=IDSTR 'type'='var'
			|	'value'=IDSTR "(" arg* ")" 'type'='func'
			|	'value'=IDSTR 'type'='id';


right		:	"("/ 'sentence'={select} ")"/ 'type'='paren'/
			|	"{"	'value'={select} "}" 'type'='node'
			|	'value'=DQSTR 'type'='dqstr'
			|	'value'=NUMBER 'type'='number'
			|	left;

form		:	'left'={exp} (ePlus | eMinus )*;
ePlus		:	"+" 'right' += {exp 'op'='plus'};
eMinus		:	"-" 'right' += {exp 'op'='minus'};
exp			:	'left'={term} (tMul | tDiv)*;
tMul		:	"*" 'right' += {term 'op'='mul'};
tDiv		:	"/" 'right' += {term 'op'='div'};
term		:	"(" 'left' = {form} ")" 'type'='form'
			|	'value'=IDSTR 'type'='id'
			|	'value'=SQSTR 'type'='sqstr'
			|	'value'=NUMBER 'type'='number'
			|	"$" 'value'=IDSTR 'type'='var';
