VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "TUSB-ADAPIO VB Sample"
   ClientHeight    =   5355
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6390
   LinkTopic       =   "Form1"
   ScaleHeight     =   5355
   ScaleWidth      =   6390
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.CommandButton GetDatas 
      Caption         =   "Adc_GetDatas"
      Height          =   495
      Left            =   4080
      TabIndex        =   21
      ToolTipText     =   "�A����荞�ݍς݂̃f�[�^����荞�݂܂�"
      Top             =   4080
      Width           =   1935
   End
   Begin VB.CommandButton GetStatus 
      Caption         =   "Adc_GetStatus"
      Height          =   495
      Left            =   4080
      TabIndex        =   20
      ToolTipText     =   "�A����荞�݃X�e�[�^�X���擾�@���܂�"
      Top             =   3480
      Width           =   1935
   End
   Begin VB.CommandButton AnalogTrg 
      Caption         =   "Adc_Analog_Trg"
      Height          =   495
      Left            =   4080
      TabIndex        =   19
      ToolTipText     =   "�O���f�B�W�^�����͂ƃA�i���O���͒l�Ńg���K�A�T���v�����O���܂��B"
      Top             =   2880
      Width           =   1935
   End
   Begin VB.CommandButton DigitalTrg 
      Caption         =   "Adc_DigitalTrg"
      Height          =   495
      Left            =   4080
      TabIndex        =   18
      ToolTipText     =   "�O���f�B�W�^���g���K�ŃT���v�����O���܂�"
      Top             =   2280
      Width           =   1935
   End
   Begin VB.CommandButton SingleSample 
      Caption         =   "Adc_Single_Sample"
      Height          =   495
      Left            =   4080
      TabIndex        =   17
      ToolTipText     =   "1�`�����l���̃f�[�^��ϊ����܂�"
      Top             =   1680
      Width           =   1935
   End
   Begin VB.CommandButton DacOut1 
      Caption         =   "Dac_Out1"
      Height          =   495
      Left            =   4080
      TabIndex        =   15
      ToolTipText     =   "DA�R���o�[�^�Ƀf�[�^(ch1)�Ƀf�[�^���o�͂��܂�"
      Top             =   480
      Width           =   975
   End
   Begin VB.CommandButton ClkStop 
      Caption         =   "Pio_Clock_Stop"
      Height          =   495
      Left            =   2040
      TabIndex        =   13
      ToolTipText     =   "�N���b�N���~���܂�"
      Top             =   4080
      Width           =   1575
   End
   Begin VB.CommandButton ClkOut 
      Caption         =   "Pio_Clock_Out"
      Height          =   495
      Left            =   360
      TabIndex        =   12
      ToolTipText     =   "�N���b�N���o�͂��܂�"
      Top             =   4080
      Width           =   1575
   End
   Begin VB.CommandButton WordRead 
      Caption         =   "Pio_WordRead"
      Height          =   495
      Left            =   360
      TabIndex        =   11
      ToolTipText     =   "���[�h�ǂݍ��݂����܂�"
      Top             =   3360
      Width           =   3255
   End
   Begin VB.CommandButton WordWrite 
      Caption         =   "Pio_WordWrite"
      Height          =   495
      Left            =   360
      TabIndex        =   10
      ToolTipText     =   "���[�h�������݂����܂�"
      Top             =   2640
      Width           =   3255
   End
   Begin VB.CommandButton HighWrite 
      Caption         =   "Pio_HighByteWrite"
      Height          =   495
      Left            =   2040
      TabIndex        =   9
      ToolTipText     =   "��ʃo�C�g�̏o�̓f�[�^��ݒ肵�܂�"
      Top             =   1200
      Width           =   1575
   End
   Begin VB.CommandButton LowWrite 
      Caption         =   "Pio_LowByteWrite"
      Height          =   495
      Left            =   360
      TabIndex        =   8
      ToolTipText     =   "���ʃo�C�g�̏o�̓f�[�^��ݒ肵�܂�"
      Top             =   1200
      Width           =   1575
   End
   Begin VB.CommandButton HighRead 
      Caption         =   "Pio_HighByteRead"
      Height          =   495
      Left            =   2040
      TabIndex        =   7
      ToolTipText     =   "��ʃo�C�g�̃f�[�^��ǂݍ��݂܂�"
      Top             =   1920
      Width           =   1575
   End
   Begin VB.CommandButton LowRead 
      Caption         =   "Pio_LowByteRead"
      Height          =   495
      Left            =   360
      TabIndex        =   6
      ToolTipText     =   "���ʃo�C�g�̃f�[�^��ǂݍ��݂܂�"
      Top             =   1920
      Width           =   1575
   End
   Begin VB.CommandButton HighDir 
      Caption         =   "Pio_HighDirection"
      Height          =   495
      Left            =   2040
      TabIndex        =   5
      ToolTipText     =   "��ʃo�C�g�̓��o�͂����肵�܂�"
      Top             =   480
      Width           =   1575
   End
   Begin VB.CommandButton LowDir 
      Caption         =   "Pio_LowDirection"
      Height          =   495
      Left            =   360
      TabIndex        =   4
      ToolTipText     =   "���ʃo�C�g�̓��o�͂����肵�܂�"
      Top             =   480
      Width           =   1575
   End
   Begin VB.ComboBox IDNUM 
      Height          =   300
      ItemData        =   "Form1.frx":0000
      Left            =   2160
      List            =   "Form1.frx":0034
      TabIndex        =   3
      Text            =   "Combo1"
      Top             =   4920
      Width           =   750
   End
   Begin VB.CommandButton DEVCLOSE 
      Caption         =   "Dev Close"
      Height          =   375
      Left            =   4560
      TabIndex        =   2
      ToolTipText     =   "�f�o�C�X���N���[�Y���܂�"
      Top             =   4920
      Width           =   1455
   End
   Begin VB.CommandButton DEVOPEN 
      Caption         =   "Dev Open"
      Height          =   375
      Left            =   240
      TabIndex        =   0
      ToolTipText     =   "�f�o�C�X���I�[�v�����܂�"
      Top             =   4920
      Width           =   1455
   End
   Begin VB.Frame Frame1 
      Caption         =   "PIO"
      Height          =   4575
      Left            =   240
      TabIndex        =   14
      Top             =   120
      Width           =   3495
   End
   Begin VB.Frame Frame2 
      Caption         =   "DAC"
      Height          =   975
      Left            =   3960
      TabIndex        =   16
      Top             =   120
      Width           =   2175
      Begin VB.CommandButton DacOut2 
         Caption         =   "Dac_Out2"
         Height          =   495
         Left            =   1080
         TabIndex        =   23
         ToolTipText     =   "DA�R���o�[�^�Ƀf�[�^(ch2)�Ƀf�[�^���o�͂��܂�"
         Top             =   360
         Width           =   975
      End
   End
   Begin VB.Frame Frame3 
      Caption         =   "ADC"
      Height          =   3375
      Left            =   3960
      TabIndex        =   22
      Top             =   1320
      Width           =   2175
   End
   Begin VB.Label Label1 
      Alignment       =   1  '�E����
      Caption         =   "ID"
      Height          =   255
      Left            =   1800
      TabIndex        =   1
      Top             =   5040
      Width           =   255
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim SampleDatNum As Integer '***�A����荞�݂Őݒ肵���f�[�^���̕ۑ�

Private Sub AnalogTrg_Click()
'***�A�i���O�g���K����̐ݒ�ƊJ�n
    TRGSET.Message.Caption = "�g���K������ݒ肵�Ă�������"
    TRGSET.EndCh.Text = "0" '***�_�C�A���O�ɕ\�����鏉���l��ݒ肷��
    TRGSET.BufSize.Text = "1"
    TRGSET.ThLevel.Text = "1"
    TRGSET.TrgCh.Text = "0"
    TRGSET.Option1.Value = True
    TRGSET.Show 1 '***���͗p�_�C�A���O�̌Ăяo��
    If (TRGSET.ret) Then  '***OK�{�^���������ꂽ��
        If Adapio_Adc_AnalogTrg(IDNUM.ListIndex, TRGSET.mEndCh, TRGSET.mBufSize, TRGSET.mThLevel, TRGSET.mTrgCh, TRGSET.mUpDown) <> 0 Then '***DLL�֐��̎��s
            MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
            DEVCLOSE_Click
        Else
            SampleDatNum = TRGSET.mBufSize
        End If
    End If
    Unload TRGSET
End Sub

Private Sub ClkOut_Click()
'***�N���b�N�̏o��
    DATAIN.Text1.Text = 0                   '***�����l��0
    DATAIN.MsgText.Caption = "�N���b�N�̎�������͂��Ă�������(HEX)"
    DATAIN.Show 1                           '***���[�_���_�C�A���O�\��
    If DATAIN.ret Then                      '***OK�ŕԂ��Ă��Ă��邩
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFFFF&) Then '*** �f�[�^�͈̔͂ɂ��邩
            If Adapio_Pio_Clock_Out(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL�֐��̎��s
                MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("�f�[�^�͈̔͂�0�`FFFF(HEX)�ł��B")
        End If
    End If
    Unload DATAIN
End Sub
'***�N���b�N�o�͂̒�~
Private Sub ClkStop_Click()
    If Adapio_Pio_Clock_Stop(IDNUM.ListIndex) <> 0 Then '***DLL�֐��̎��s
        MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
        DEVCLOSE_Click
    End If
End Sub

Private Sub DacOut1_Click()
'***DAC 1ch �̏o�̓f�[�^�̐ݒ�
    DATAIN.Text1.Text = 0                   '***�����l��0
    DATAIN.MsgText.Caption = "DAC(ch1)�ւ̏o�͒l����͂��Ă�������(HEX)"
    DATAIN.Show 1                           '***���[�_���_�C�A���O�\��
    If DATAIN.ret Then                      '***OK�ŕԂ��Ă��Ă��邩
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** �f�[�^�͈̔͂ɂ��邩
            If Adapio_Dac_Out(IDNUM.ListIndex, 0, DATAIN.dat) <> 0 Then '***DLL�֐��̎��s
                MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub DacOut2_Click()
'***DAC 2ch �̏o�̓f�[�^�̐ݒ�
    DATAIN.Text1.Text = 0                   '***�����l��0
    DATAIN.MsgText.Caption = "DAC(ch2)�ւ̏o�͒l����͂��Ă�������(HEX)"
    DATAIN.Show 1                           '***���[�_���_�C�A���O�\��
    If DATAIN.ret Then                      '***OK�ŕԂ��Ă��Ă��邩
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** �f�[�^�͈̔͂ɂ��邩
            If Adapio_Dac_Out(IDNUM.ListIndex, 1, DATAIN.dat) <> 0 Then '***DLL�֐��̎��s
                MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub DEVCLOSE_Click()
'***�f�o�C�X�̃N���[�Y***
    Adapio_Device_Close IDNUM.ListIndex '***�f�o�C�X�N���[�Y
    Function_Disable                    '***�{�^���̗L�������؂�ւ�
End Sub

Private Sub DEVOPEN_Click()
'***�f�o�C�X�̃I�[�v��***
    If (Adapio_Device_Open(IDNUM.ListIndex) = 0) Then
        Function_Enable                       '***�e�@�\�{�^����L���ɂ���
    Else
        MsgBox ("�I�[�v���ł��܂���ł���")     '***�I�[�v���Ɏ��s�����ꍇ
    End If
End Sub

Private Sub DigitalTrg_Click()
'***�f�B�W�^���g���K����̐ݒ�ƊJ�n
    TRGSET.Message.Caption = "�g���K������ݒ肵�Ă�������"
    TRGSET.EndCh.Text = "0" '***�_�C�A���O�ɕ\�����鏉���l��ݒ肷��
    TRGSET.BufSize.Text = "1"
    TRGSET.ThLevel.Visible = False '***���͂Ɏg�p���Ȃ����ڂ͔�\���ɂ���
    TRGSET.TrgCh.Visible = False
    TRGSET.Option1.Visible = False
    TRGSET.Option2.Visible = False
    TRGSET.Show 1 '***���͗p�_�C�A���O�̌Ăяo��
    If (TRGSET.ret) Then  '***OK�{�^���������ꂽ��
        If Adapio_Adc_DigitalTrg(IDNUM.ListIndex, TRGSET.mEndCh, TRGSET.mBufSize) <> 0 Then '***DLL�֐��̎��s
            MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
            DEVCLOSE_Click
        Else
            SampleDatNum = TRGSET.mBufSize
        End If
    End If
    Unload TRGSET
End Sub

Private Sub Form_Load()
'***�t�H�[���̕\���J�n���̏���������***
    Function_Disable            '***
    IDNUM.ListIndex = 0         '***�����I��ID��0�ɂ���
End Sub

Private Sub Function_Disable()
'***�I�[�v����ID�I���{�^���ȊO�̊e�{�^���@�\�𖳌��ɂ���***
    DEVCLOSE.Enabled = False    '***�N���[�Y�{�^������
    DEVOPEN.Enabled = True      '***�I�[�v���{�^���L��
    IDNUM.Enabled = True        '***ID�I���{�^���L��
    '***�ȉ��̃{�^�����ׂĖ���
    LowDir.Enabled = False
    HighDir.Enabled = False
    LowRead.Enabled = False
    HighRead.Enabled = False
    LowWrite.Enabled = False
    HighWrite.Enabled = False
    WordRead.Enabled = False
    WordWrite.Enabled = False
    ClkOut.Enabled = False
    ClkStop.Enabled = False
    DacOut1.Enabled = False
    DacOut2.Enabled = False
    SingleSample.Enabled = False
    DigitalTrg.Enabled = False
    AnalogTrg.Enabled = False
    GetStatus.Enabled = False
    GetDatas.Enabled = False
End Sub
Private Sub Function_Enable()
'***�I�[�v����ID�I���{�^���݂̂𖳌��ɂ���B***
    DEVOPEN.Enabled = False     '***�I�[�v���{�^������
    DEVCLOSE.Enabled = True     '***�N���[�Y�{�^���L��
    IDNUM.Enabled = False       '***ID�I���{�^������
    '***�ȉ��̃{�^�����ׂėL��
    LowDir.Enabled = True
    HighDir.Enabled = True
    LowRead.Enabled = True
    HighRead.Enabled = True
    LowWrite.Enabled = True
    HighWrite.Enabled = True
    WordRead.Enabled = True
    WordWrite.Enabled = True
    ClkOut.Enabled = True
    ClkStop.Enabled = True
    DacOut1.Enabled = True
    DacOut2.Enabled = True
    SingleSample.Enabled = True
    DigitalTrg.Enabled = True
    AnalogTrg.Enabled = True
    GetStatus.Enabled = True
    GetDatas.Enabled = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
'***�A�v���P�[�V�����I�����̏���
    If DEVCLOSE.Enabled Then
        Adapio_Device_Close IDNUM.ListCount '***�f�o�C�X�N���[�Y
    End If
End Sub

Private Sub GetDatas_Click()
'***�T���v�����O�ς݃f�[�^�̓ǂݍ���
    Dim dat(512) As Integer
    If Adapio_Adc_GetDatas(IDNUM.ListIndex, dat(0), SampleDatNum) <> 0 Then '***DLL�֐��̎��s
        MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
        DEVCLOSE_Click
    Else
        Open "Sample.dat" For Output As #1 '***�ۑ��t�@�C���̃I�[�v��
        For i = 0 To (SampleDatNum - 1)
            Print #1, (CStr(dat(i)) + ",") '***�ۑ��t�@�C���ւ̏�������
        Next
        Close #1 '***�t�@�C���̃N���[�Y
        MsgBox ("��荞�݃f�[�^��sample.dat�ɕۑ�����܂���")
    End If
End Sub

Private Sub GetStatus_Click()
'***AD��荞�݃X�e�[�^�X�̓ǂݍ���
    Dim Running As Integer
    Dim SampleNum As Integer
    If Adapio_Adc_GetStatus(IDNUM.ListIndex, Running, SampleNum) <> 0 Then '***DLL�֐��̎��s
        MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
        DEVCLOSE_Click
    Else
        If (Running = 1) Then  '***���쒆�ł����
            MsgBox ("���쒆   ��荞�ݍς݃f�[�^��(" + CStr(SampleNum) + ")")
        Else
            MsgBox ("��荞�݊���   ��荞�ݍς݃f�[�^��(" + CStr(SampleNum) + ")")
       End If
    End If
End Sub

Private Sub HighDir_Click()
'***��ʃo�C�g�̓��o�͕����ݒ�
    DATAIN.Text1.Text = 0                   '***�����l��0
    DATAIN.MsgText.Caption = "��ʃo�C�g�̓��o�͒l����͂��Ă�������(HEX)"
    DATAIN.Show 1                           '***���[�_���_�C�A���O�\��
    If DATAIN.ret Then                      '***OK�ŕԂ��Ă��Ă��邩
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** �f�[�^�͈̔͂ɂ��邩
            If Adapio_Pio_HighDirection(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL�֐��̎��s
                MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub HighRead_Click()
'***��ʃo�C�g�̓��̓f�[�^�̎擾
    Dim dat As Byte
    If Adapio_Pio_HighByteRead(IDNUM.ListIndex, dat) <> 0 Then '***DLL�֐��̎��s
        MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
        DEVCLOSE_Click
    Else
        MsgBox ("��ʃo�C�g���͒l�� " + Hex(dat) + "(HEX) �ł�")
    End If
End Sub

Private Sub HighWrite_Click()
'***��ʃo�C�g�̏o�̓f�[�^��������
    DATAIN.Text1.Text = 0                   '***�����l��0
    DATAIN.MsgText.Caption = "��ʃo�C�g�̏o�͒l����͂��Ă�������(HEX)"
    DATAIN.Show 1                           '***���[�_���_�C�A���O�\��
    If DATAIN.ret Then                      '***OK�ŕԂ��Ă��Ă��邩
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** �f�[�^�͈̔͂ɂ��邩
            If Adapio_Pio_HighByteWrite(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL�֐��̎��s
                MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub LowDir_Click()
'***���ʃo�C�g�̓��o�͕����ݒ�
    DATAIN.Text1.Text = 0                   '***�����l��0
    DATAIN.MsgText.Caption = "���ʃo�C�g�̓��o�͒l����͂��Ă�������(HEX)"
    DATAIN.Show 1                           '***���[�_���_�C�A���O�\��
    If DATAIN.ret Then                      '***OK�ŕԂ��Ă��Ă��邩
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** �f�[�^�͈̔͂ɂ��邩
            If Adapio_Pio_LowDirection(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL�֐��̎��s
                MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub LowRead_Click()
'***���ʃo�C�g�̓��̓f�[�^�̎擾
    Dim dat As Byte
    If Adapio_Pio_LowByteRead(IDNUM.ListIndex, dat) <> 0 Then '***DLL�֐��̎��s
        MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
        DEVCLOSE_Click
    Else
        MsgBox ("���ʃo�C�g���͒l�� " + Hex(dat) + "(HEX) �ł�")
    End If
End Sub

Private Sub LowWrite_Click()
'***���ʃo�C�g�̏o�̓f�[�^��������
    DATAIN.Text1.Text = 0                   '***�����l��0
    DATAIN.MsgText.Caption = "���ʃo�C�g�̏o�͒l����͂��Ă�������(HEX)"
    DATAIN.Show 1                           '***���[�_���_�C�A���O�\��
    If DATAIN.ret Then                      '***OK�ŕԂ��Ă��Ă��邩
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** �f�[�^�͈̔͂ɂ��邩
            If Adapio_Pio_LowByteWrite(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL�֐��̎��s
                MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub SingleSample_Click()
'***��ʃo�C�g�̓��̓f�[�^�̎擾
    Dim dat(6) As Integer '***�f�[�^�z��
    Dim ch              '***�`�����l��
    Dim str As String '***������
    Dim strCR As String '***���s������
    strCR = Chr(13) + Chr(10)
    For ch = 0 To 5
        If Adapio_Adc_SingleSample(IDNUM.ListIndex, ch, dat(ch)) <> 0 Then '***DLL�֐��̎��s
            MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
            DEVCLOSE_Click
            Exit Sub
        End If
    Next
    str = "ch1�̃f�[�^ " + CStr(dat(0)) + strCR
    str = str + "ch2�̃f�[�^ " + CStr(dat(1)) + strCR
    str = str + "ch3�̃f�[�^ " + CStr(dat(2)) + strCR
    str = str + "ch4�̃f�[�^ " + CStr(dat(3)) + strCR
    str = str + "ch5�̃f�[�^ " + CStr(dat(4)) + strCR
    str = str + "ch6�̃f�[�^ " + CStr(dat(5))
    MsgBox (str)
End Sub

Private Sub WordRead_Click()
'***���̓��[�h�f�[�^�̎擾
    Dim dat As Long
    If Adapio_Pio_WordRead(IDNUM.ListIndex, dat) <> 0 Then '***DLL�֐��̎��s
        MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
        DEVCLOSE_Click
    Else
        MsgBox ("���[�h���͒l�� " + Hex(dat) + "(HEX) �ł�")
    End If
End Sub

Private Sub WordWrite_Click()
'***���[�h�f�[�^��������
    DATAIN.Text1.Text = 0                   '***�����l��0
    DATAIN.MsgText.Caption = "�o�͒l����͂��Ă�������(HEX)"
    DATAIN.Show 1                           '***���[�_���_�C�A���O�\��
    If DATAIN.ret Then                      '***OK�ŕԂ��Ă��Ă��邩
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFFFF&) Then '*** �f�[�^�͈̔͂ɂ��邩
            If Adapio_Pio_WordWrite(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL�֐��̎��s
                MsgBox ("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("�f�[�^�͈̔͂�0�`FFFF(HEX)�ł��B")
        End If
    End If
    Unload DATAIN
End Sub
