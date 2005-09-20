MIDI�p���C�u���� (MidiLib) �̎g����
===================================


����̂̎g����

1. �܂��A�|�C���^��p�ӂ��āA

2. C++�W����new�R�}���h�ŁA�I�u�W�F�N�g�𐶐����A�|�C���^�֓����B

3. Open�֐��ŁAMIDI�f�o�C�X�̏���������B

4. MIDI�f�o�C�X�𑀍삷��B

( 5. Close�֐��ŁAMIDI�f�o�C�X�̌㏈��������B)

6. C++�W����delete�R�}���h�ŁA�I�u�W�F�N�g�̔j��������B
   ��L 5.��Close�֐��́A���C�u�����̃f�X�g���N�^�Ɋ܂܂�Ă���̂ŁA
   Close�֐��́A�ʓr�Ăяo���Ȃ��Ă�OK�B

�ڂ����́A�e�T���v�����Q�ƁB

���K�v�ȃt�@�C���́AMidiLib.h (�w�b�_�t�@�C��) �ƁAMidiLib.cpp (�R�[�h)
�݂̂ł��B�A�v�����ŁAMidiLib.h���C���N���[�h���Ă��������B
��Windows�W�����C�u�����Awinmm.lib���g���܂��̂ŁA���̃v���O�����֑g�ݍ���
�ꍇ�Awinmm.lib�������N����悤�ɐݒ肵�Ă��������B


���֐��̐���

�e�֐��ɋ��ʎ����Ƃ��āA
  �߂�l��bool�^�ŁA����I���̏ꍇ�Atrue, �G���[�I���̏ꍇ�Afalse
  �G���[�̏ꍇ�ADispString�֐��Őݒ肳�ꂽ�ꏊ�ɁA�G���[�������
  �\������B

MIDI�ł́A1�̃C���^�[�t�F�C�X�ŁA1�`16�܂ł̊y����w��ł��܂����A
�e�y����p�[�g�ƌ�������A�g���b�N�ƌ�������A�`�����l���ƌ�������
���Ă��܂����A��̓������̂��w���Ă���悤�ł��B
MidiLib�ł́A�g���b�N(track)�œ��ꂵ�Ă��܂��B

�EOpen( const char *pszDeviceName ) �������� Open()

  MIDI�f�o�C�X�̏����B
  MIDI�}�b�p�[��p����ꍇ�AOpen�֐��̈����͂Ȃ��B

�EClose()

  MIDI�f�o�C�X�̌㏈���B

�ENoteOn( int track, int scale, int vel )

  track�Ŏw�肳�ꂽ�g���b�N�ɁAscale�Ŏw�肳�ꂽ�����̌��Ղ��A
  vel�Ŏw�肳�ꂽ�����ŁA�Ō�����B(���Ȃ킿�����o��B)
  ���̊֐����ĂԂƁA�Y�����Ղ͉������܂܂ɂȂ�B

�ENoteOff( int track, int scale )

  track�Ŏw�肳�ꂽ�g���b�N�ŁAscale�Ŏw�肳�ꂽ�����̌��Ղ���A
  �Ō����~�߂�B(���Ȃ킿�����~�܂�B)
  ���̊֐����ĂԂ��Ƃɂ���āANoteOn�ŉ��������Ղ�������Ȃ���Ԃ�
  �߂�B

�ETrackProgramChange( int track, int instrument )

  track�Ŏw�肳�ꂽ�g���b�N�̉��F��instrument�Ŏw�肵���ԍ��̊y��
  �Ɋ��蓖�Ă�B
  �������A���̊֐��ł́A128�ʂ�̂�����GM���F�������蓖�Ăł��Ȃ��̂ŁA
  �o���N�؂�ւ���p�������F�̊��蓖�ẮAGS_Reset(��q)���Ăяo������A
  TrackProgramBankChange(��q)��p���邱�ƁB
  PC��������(������"Microsoft GS Wavetable SW Synth")�ł́A
  �o���N0�̉��F�����肦�Ă���B

  ���F�̏ڍׂ́A���L�A
  �uPC��������(������"Microsoft GS Wavetable SW Synth")�̏ڍׁv
  ���Q�ƁB

�ETrackProgramBankChange( int track, int instrument, int bank )

  track�Ŏw�肳�ꂽ�g���b�N�̉��F��bank�w��𔺂�instrument�̊y���
  ���蓖�Ă�B
  PC��������(������"Microsoft GS Wavetable SW Synth")�ł́A
  GS_Reset(��q)���󂯕t������łȂ��ƁA�o���N�؂�ւ����L���ɂȂ�Ȃ��̂ŁA
  GS_Reset��O�����ČĂяo���Ă������ƁB

  ���F�̏ڍׂ́A���L�A
  �uPC��������(������"Microsoft GS Wavetable SW Synth")�̏ڍׁv
  ���Q�ƁB

�ETrackPanChange( int track, int pan )

  track�Ŏw�肳�ꂽ�g���b�N�̃p��(���E�̂ǂ̈ʒu�Ɋy����ʂ����邩)
  ���w�肷��B
  �l�͈͂� -64�`+63�ŁA
  �ł��� => -64
  ����   =>   0
  �ł��E => +63
  �̈ʒu�ɒ�ʂ���B
  �p���̎w�肪�����ꍇ�A�����ɒ�ʂ���B

�ETrackVolumeChange( int track, int volume )

  track�Ŏw�肳�ꂽ�g���b�N�̃g���b�N�Ƃ��Ẵ{�����[����ݒ肷��B
  ���ۂ̉��̑傫���́A���̃{�����[����NoteOn�ł�vel�l�̗����ɂ����
  ���܂�B

�ETrackPitchBendSensitivity( int track, int maxpitchwidth )

  track�Ŏw�肳�ꂽ�g���b�N�̉����ω�(�s�b�`�x���h)�̍ő�ω��ʂ����߂�
  maxpitchwidth�́A0�`12�Ŏw�肳��A�P�ʂ͔����ɂȂ�B
  ���������āA�ő�l12�� 12���� = 1�I�N�^�[�u �ƂȂ�B

�ETrackPitchBend( int track, int pitch )

  track�Ŏw�肳�ꂽ�g���b�N�̉�����ω�������B
  TrackPitchBendSensitivity�ł̎w��l���ő�ω��ʂƂ��āA
  pitch = 0�`8191�ŁA������特�̍������֍ő�ω��ʂ܂ŁA�������ω����A
  pitch = 0�`-8192�ŁA������特�̒Ⴂ���֍ő�ω��ʂ܂ŁA�������ω�����B
  pitch = 0�Ŋ��(�����̕ω��Ȃ�)

�EGM_System_On()

  ������GM���[�h�ɐݒ肷��B
  PC��������(������"Microsoft GS Wavetable SW Synth")�ł́A
  �ł́A�����l��GM���[�h�Ȃ̂ŁA�قƂ�ǎg��Ȃ��B

�EGS_Reset() ( �܂��� GS_Reset( int devid ) )

  ������GS���[�h�ɐݒ肷��B
  devid�͉����ɐݒ肳��Ă���f�o�C�XID�����A���ʂ̓f�t�H���g�ŗǂ��͂�
  �Ȃ̂ŁA�����͂���Ȃ��͂��B

  GS���[�h�ɂ���ƁB
  PC��������(������"Microsoft GS Wavetable SW Synth")�ł́A
  1. �o���N�؂�ւ��ɂ��A226���F���g����B
  2. �C�ӂ̃g���b�N���h�������[�h�ɂ�����A�ʏ�y�탂�[�h�ɂł���
     (GM�ł́A�g���b�N10�̂݃h�������[�h�A���g���b�N�͒ʏ�y�탂�[�h�ɌŒ�)
  3. �h�����Z�b�g��2�Z�b�g�g�����Ƃ��ł���B

�EGS_TrackDrumModeSet( int track, int mode )
   ( �܂��� GS_TrackDrumModeSet( int track, int mode, int devid ) )

  track�Ŏw�肳�ꂽ�g���b�N���A
    �ʏ�y��(�����y��)���[�h mode = 0
    �h�����Z�b�g1            mode = 1
    �h�����Z�b�g2            mode = 2
  �ɕύX�ł���B
  devid�͉����ɐݒ肳��Ă���f�o�C�XID�����A���ʂ̓f�t�H���g�ŗǂ��͂�
  �Ȃ̂ŁA����devid�͒ʏ킢��Ȃ��͂��B

  �h�����Z�b�g���w�肵���p�[�g���ŁA�h�����̎�ނ�ProgramChage�ŕύX����ƁA
  �����h�����Z�b�g�ɏ������鑼�p�[�g�ł��A�h�����̎�ނ��ς���Ă��܂��B
  �f�t�H���g�ł́A�g���b�N10���h�����Z�b�g1�ɏ������Ă��āA
  ���̃p�[�g�͂��ׂĒʏ�y��(�����y��)���[�h�B


���O���[�o���֐�

�Eint	OnmeiConv( char chOnmei, int iOctave, int iSharpFlat )
�������m�[�g�ԍ��֕ϊ�
����:
	char chOnmei
		����
		C, D, E, F, G, A, B�Ŏw��̂���	(�A���t�@�x�b�g���p�啶���̂�)
		����ȊO�͂��ׂ�C����
	int iOctave;
		�I�N�^�[�u�ԍ� -1, 0�`9�Ŏw��B
		����ȊO�͂��ׂ� 4 ����
	int iSharpFlat
		�V���[�v��t���b�g���w�肷��B
		�V���[�v�̏ꍇ 1, �t���b�g�̏ꍇ -1
		���w�� 0 �ŁA�����Ȃ�
�߂�l:
	�m�[�g�ԍ� ( 0�`127 )


���֗���MIDI���m�F�c�[��

SoundChecker
http://members.at.infoseek.co.jp/tomo_kusaba/sound.htm

DrumChecker
http://members.at.infoseek.co.jp/tomo_kusaba/drum.htm

( ��҂̃y�[�W http://members.at.infoseek.co.jp/tomo_kusaba/ )


���Q�l����

PC��������(������"Microsoft GS Wavetable SW Synth")�̏ڍ�
http://www.wiztext.net/~sakura_news/4neria/mgs.html

MIDI�̊�{���t�@�����X ( �����ŗL�����ȊO )
http://www.kishi.net/rws/music.html

MIDI�̏㋉(?)���t�@�����X ( �G�N�X�N���[�V�u�Ƃ� )
http://www2s.biglobe.ne.jp/~puri-q/midilec.html


================================
2003.11.05	tmiura �쐬
2003.11.06	tmiura �f�B���N�g���\����ύX
2003.11.06	tmiura �p���̐ݒ�l��ύX
2003.11.06	tmiura �s�b�`�x���h�̋@�\��ǉ�
2003.11.19	tmiura �������m�[�g�ԍ��֕ϊ�����֐���ǉ�

