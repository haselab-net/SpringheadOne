Public Class Form1
    Inherits System.Windows.Forms.Form

#Region " Windows �t�H�[�� �f�U�C�i�Ő������ꂽ�R�[�h "

    Public Sub New()
        MyBase.New()

        ' ���̌Ăяo���� Windows �t�H�[�� �f�U�C�i�ŕK�v�ł��B
        InitializeComponent()

        ' InitializeComponent() �Ăяo���̌�ɏ�������ǉ����܂��B

    End Sub

    ' Form �� dispose ���I�[�o�[���C�h���ăR���|�[�l���g�ꗗ���������܂��B
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    ' Windows �t�H�[�� �f�U�C�i�ŕK�v�ł��B
    Private components As System.ComponentModel.IContainer

    ' ���� : �ȉ��̃v���V�[�W���́AWindows �t�H�[�� �f�U�C�i�ŕK�v�ł��B
    ' Windows �t�H�[�� �f�U�C�i���g���ĕύX���Ă��������B  
    ' �R�[�h �G�f�B�^�͎g�p���Ȃ��ł��������B
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents GroupBox2 As System.Windows.Forms.GroupBox
    Friend WithEvents GroupBox3 As System.Windows.Forms.GroupBox
    Friend WithEvents DevOpen As System.Windows.Forms.Button
    Friend WithEvents IDNUM As System.Windows.Forms.ComboBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents DevClose As System.Windows.Forms.Button
    Friend WithEvents PioClockStop As System.Windows.Forms.Button
    Friend WithEvents PioClockOut As System.Windows.Forms.Button
    Friend WithEvents PioWordRead As System.Windows.Forms.Button
    Friend WithEvents PioWordWrite As System.Windows.Forms.Button
    Friend WithEvents PioHighByteRead As System.Windows.Forms.Button
    Friend WithEvents PioLowByteRead As System.Windows.Forms.Button
    Friend WithEvents PioHighByteWrite As System.Windows.Forms.Button
    Friend WithEvents PioLowByteWrite As System.Windows.Forms.Button
    Friend WithEvents PioHighDirection As System.Windows.Forms.Button
    Friend WithEvents PioLowDirection As System.Windows.Forms.Button
    Friend WithEvents DacOut2 As System.Windows.Forms.Button
    Friend WithEvents DacOut1 As System.Windows.Forms.Button
    Friend WithEvents AdcGetData As System.Windows.Forms.Button
    Friend WithEvents AdcAnalogTrg As System.Windows.Forms.Button
    Friend WithEvents AdcSingleSample As System.Windows.Forms.Button
    Friend WithEvents AdcDigitalTrg As System.Windows.Forms.Button
    Friend WithEvents AdcGetStatus As System.Windows.Forms.Button
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.PioClockStop = New System.Windows.Forms.Button()
        Me.PioClockOut = New System.Windows.Forms.Button()
        Me.PioWordRead = New System.Windows.Forms.Button()
        Me.PioWordWrite = New System.Windows.Forms.Button()
        Me.PioHighByteRead = New System.Windows.Forms.Button()
        Me.PioLowByteRead = New System.Windows.Forms.Button()
        Me.PioHighByteWrite = New System.Windows.Forms.Button()
        Me.PioLowByteWrite = New System.Windows.Forms.Button()
        Me.PioHighDirection = New System.Windows.Forms.Button()
        Me.PioLowDirection = New System.Windows.Forms.Button()
        Me.GroupBox2 = New System.Windows.Forms.GroupBox()
        Me.DacOut2 = New System.Windows.Forms.Button()
        Me.DacOut1 = New System.Windows.Forms.Button()
        Me.GroupBox3 = New System.Windows.Forms.GroupBox()
        Me.AdcGetData = New System.Windows.Forms.Button()
        Me.AdcGetStatus = New System.Windows.Forms.Button()
        Me.AdcAnalogTrg = New System.Windows.Forms.Button()
        Me.AdcDigitalTrg = New System.Windows.Forms.Button()
        Me.AdcSingleSample = New System.Windows.Forms.Button()
        Me.DevOpen = New System.Windows.Forms.Button()
        Me.IDNUM = New System.Windows.Forms.ComboBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.DevClose = New System.Windows.Forms.Button()
        Me.GroupBox1.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.GroupBox3.SuspendLayout()
        Me.SuspendLayout()
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.AddRange(New System.Windows.Forms.Control() {Me.PioClockStop, Me.PioClockOut, Me.PioWordRead, Me.PioWordWrite, Me.PioHighByteRead, Me.PioLowByteRead, Me.PioHighByteWrite, Me.PioLowByteWrite, Me.PioHighDirection, Me.PioLowDirection})
        Me.GroupBox1.Location = New System.Drawing.Point(8, 8)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(248, 304)
        Me.GroupBox1.TabIndex = 0
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "PIO"
        '
        'PioClockStop
        '
        Me.PioClockStop.Location = New System.Drawing.Point(128, 264)
        Me.PioClockStop.Name = "PioClockStop"
        Me.PioClockStop.Size = New System.Drawing.Size(112, 32)
        Me.PioClockStop.TabIndex = 9
        Me.PioClockStop.Text = "Pio_ClockStop"
        '
        'PioClockOut
        '
        Me.PioClockOut.Location = New System.Drawing.Point(8, 264)
        Me.PioClockOut.Name = "PioClockOut"
        Me.PioClockOut.Size = New System.Drawing.Size(112, 32)
        Me.PioClockOut.TabIndex = 8
        Me.PioClockOut.Text = "Pio_ClockOut"
        '
        'PioWordRead
        '
        Me.PioWordRead.Location = New System.Drawing.Point(8, 216)
        Me.PioWordRead.Name = "PioWordRead"
        Me.PioWordRead.Size = New System.Drawing.Size(232, 32)
        Me.PioWordRead.TabIndex = 7
        Me.PioWordRead.Text = "Pio_WordRead"
        '
        'PioWordWrite
        '
        Me.PioWordWrite.Location = New System.Drawing.Point(8, 168)
        Me.PioWordWrite.Name = "PioWordWrite"
        Me.PioWordWrite.Size = New System.Drawing.Size(232, 32)
        Me.PioWordWrite.TabIndex = 6
        Me.PioWordWrite.Text = "Pio_WordWrite"
        '
        'PioHighByteRead
        '
        Me.PioHighByteRead.Location = New System.Drawing.Point(128, 120)
        Me.PioHighByteRead.Name = "PioHighByteRead"
        Me.PioHighByteRead.Size = New System.Drawing.Size(112, 32)
        Me.PioHighByteRead.TabIndex = 5
        Me.PioHighByteRead.Text = "Pio_HighByteRead"
        '
        'PioLowByteRead
        '
        Me.PioLowByteRead.Location = New System.Drawing.Point(8, 120)
        Me.PioLowByteRead.Name = "PioLowByteRead"
        Me.PioLowByteRead.Size = New System.Drawing.Size(112, 32)
        Me.PioLowByteRead.TabIndex = 4
        Me.PioLowByteRead.Text = "Pio_LowByteRead"
        '
        'PioHighByteWrite
        '
        Me.PioHighByteWrite.Location = New System.Drawing.Point(128, 72)
        Me.PioHighByteWrite.Name = "PioHighByteWrite"
        Me.PioHighByteWrite.Size = New System.Drawing.Size(112, 32)
        Me.PioHighByteWrite.TabIndex = 3
        Me.PioHighByteWrite.Text = "Pio_HighByteWrite"
        '
        'PioLowByteWrite
        '
        Me.PioLowByteWrite.Location = New System.Drawing.Point(8, 72)
        Me.PioLowByteWrite.Name = "PioLowByteWrite"
        Me.PioLowByteWrite.Size = New System.Drawing.Size(112, 32)
        Me.PioLowByteWrite.TabIndex = 2
        Me.PioLowByteWrite.Text = "Pio_LowByteWrite"
        '
        'PioHighDirection
        '
        Me.PioHighDirection.Location = New System.Drawing.Point(128, 24)
        Me.PioHighDirection.Name = "PioHighDirection"
        Me.PioHighDirection.Size = New System.Drawing.Size(112, 32)
        Me.PioHighDirection.TabIndex = 1
        Me.PioHighDirection.Text = "Pio_HighDirection"
        '
        'PioLowDirection
        '
        Me.PioLowDirection.Location = New System.Drawing.Point(8, 24)
        Me.PioLowDirection.Name = "PioLowDirection"
        Me.PioLowDirection.Size = New System.Drawing.Size(112, 32)
        Me.PioLowDirection.TabIndex = 0
        Me.PioLowDirection.Text = "Pio_LowDirection"
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.AddRange(New System.Windows.Forms.Control() {Me.DacOut2, Me.DacOut1})
        Me.GroupBox2.Location = New System.Drawing.Point(264, 8)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(144, 64)
        Me.GroupBox2.TabIndex = 1
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "DAC"
        '
        'DacOut2
        '
        Me.DacOut2.Location = New System.Drawing.Point(72, 24)
        Me.DacOut2.Name = "DacOut2"
        Me.DacOut2.Size = New System.Drawing.Size(64, 32)
        Me.DacOut2.TabIndex = 1
        Me.DacOut2.Text = "Dac_Out2"
        '
        'DacOut1
        '
        Me.DacOut1.Location = New System.Drawing.Point(8, 24)
        Me.DacOut1.Name = "DacOut1"
        Me.DacOut1.Size = New System.Drawing.Size(64, 32)
        Me.DacOut1.TabIndex = 0
        Me.DacOut1.Text = "Dac_Out1"
        '
        'GroupBox3
        '
        Me.GroupBox3.Controls.AddRange(New System.Windows.Forms.Control() {Me.AdcGetData, Me.AdcGetStatus, Me.AdcAnalogTrg, Me.AdcDigitalTrg, Me.AdcSingleSample})
        Me.GroupBox3.Location = New System.Drawing.Point(264, 88)
        Me.GroupBox3.Name = "GroupBox3"
        Me.GroupBox3.Size = New System.Drawing.Size(144, 224)
        Me.GroupBox3.TabIndex = 2
        Me.GroupBox3.TabStop = False
        Me.GroupBox3.Text = "ADC"
        '
        'AdcGetData
        '
        Me.AdcGetData.Location = New System.Drawing.Point(8, 184)
        Me.AdcGetData.Name = "AdcGetData"
        Me.AdcGetData.Size = New System.Drawing.Size(128, 32)
        Me.AdcGetData.TabIndex = 4
        Me.AdcGetData.Text = "Adc_GetData"
        '
        'AdcGetStatus
        '
        Me.AdcGetStatus.Location = New System.Drawing.Point(8, 144)
        Me.AdcGetStatus.Name = "AdcGetStatus"
        Me.AdcGetStatus.Size = New System.Drawing.Size(128, 32)
        Me.AdcGetStatus.TabIndex = 3
        Me.AdcGetStatus.Text = "Adc_GetStatus"
        '
        'AdcAnalogTrg
        '
        Me.AdcAnalogTrg.Location = New System.Drawing.Point(8, 104)
        Me.AdcAnalogTrg.Name = "AdcAnalogTrg"
        Me.AdcAnalogTrg.Size = New System.Drawing.Size(128, 32)
        Me.AdcAnalogTrg.TabIndex = 2
        Me.AdcAnalogTrg.Text = "Adc_AnalogTrg"
        '
        'AdcDigitalTrg
        '
        Me.AdcDigitalTrg.Location = New System.Drawing.Point(8, 64)
        Me.AdcDigitalTrg.Name = "AdcDigitalTrg"
        Me.AdcDigitalTrg.Size = New System.Drawing.Size(128, 32)
        Me.AdcDigitalTrg.TabIndex = 1
        Me.AdcDigitalTrg.Text = "Adc_DigitalTrg"
        '
        'AdcSingleSample
        '
        Me.AdcSingleSample.Location = New System.Drawing.Point(8, 24)
        Me.AdcSingleSample.Name = "AdcSingleSample"
        Me.AdcSingleSample.Size = New System.Drawing.Size(128, 32)
        Me.AdcSingleSample.TabIndex = 0
        Me.AdcSingleSample.Text = "Adc_SingleSample"
        '
        'DevOpen
        '
        Me.DevOpen.Location = New System.Drawing.Point(16, 320)
        Me.DevOpen.Name = "DevOpen"
        Me.DevOpen.Size = New System.Drawing.Size(96, 24)
        Me.DevOpen.TabIndex = 3
        Me.DevOpen.Text = "Dev_Open"
        '
        'IDNUM
        '
        Me.IDNUM.Items.AddRange(New Object() {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"})
        Me.IDNUM.Location = New System.Drawing.Point(184, 320)
        Me.IDNUM.Name = "IDNUM"
        Me.IDNUM.Size = New System.Drawing.Size(48, 20)
        Me.IDNUM.TabIndex = 4
        Me.IDNUM.Text = "ComboBox1"
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(128, 328)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(48, 24)
        Me.Label1.TabIndex = 5
        Me.Label1.Text = "ID"
        '
        'DevClose
        '
        Me.DevClose.Location = New System.Drawing.Point(304, 320)
        Me.DevClose.Name = "DevClose"
        Me.DevClose.Size = New System.Drawing.Size(96, 24)
        Me.DevClose.TabIndex = 6
        Me.DevClose.Text = "Dev_Close"
        '
        'Form1
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 12)
        Me.ClientSize = New System.Drawing.Size(424, 350)
        Me.Controls.AddRange(New System.Windows.Forms.Control() {Me.DevClose, Me.Label1, Me.IDNUM, Me.DevOpen, Me.GroupBox3, Me.GroupBox2, Me.GroupBox1})
        Me.Name = "Form1"
        Me.Text = "TUSB-ADAPIO VB Sample"
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox2.ResumeLayout(False)
        Me.GroupBox3.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub

#End Region
    Dim SampleDatNum As Short '***�A����荞�݂Őݒ肵���f�[�^���̕ۑ�
    Dim TrgSetDlg As New TRGSET()

    Private Sub DevOpen_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles DevOpen.Click
        '***�f�o�C�X�̃I�[�v��***
        If (Adapio_Device_Open(IDNUM.SelectedIndex) = 0) Then
            Function_Enable() '***�e�@�\�{�^����L���ɂ���
        Else
            MsgBox("�I�[�v���ł��܂���ł���") '***�I�[�v���Ɏ��s�����ꍇ
        End If
    End Sub

    Private Sub DevClose_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles DevClose.Click
        '***�f�o�C�X�̃N���[�Y***
        Adapio_Device_Close(IDNUM.SelectedIndex) '***�f�o�C�X�N���[�Y
        Function_Disable() '***�{�^���̗L�������؂�ւ�
    End Sub
    Private Sub Function_Disable()
        '***�I�[�v����ID�I���{�^���ȊO�̊e�{�^���@�\�𖳌��ɂ���***
        DEVCLOSE.Enabled = False '***�N���[�Y�{�^������
        DEVOPEN.Enabled = True '***�I�[�v���{�^���L��
        IDNUM.Enabled = True '***ID�I���{�^���L��
        '***�ȉ��̃{�^�����ׂĖ���
        PioLowDirection.Enabled = False
        PioHighDirection.Enabled = False
        PioLowByteRead.Enabled = False
        PioHighByteRead.Enabled = False
        PioLowByteWrite.Enabled = False
        PioHighByteWrite.Enabled = False
        PioWordRead.Enabled = False
        PioWordWrite.Enabled = False
        PioClockOut.Enabled = False
        PioClockStop.Enabled = False
        DacOut1.Enabled = False
        DacOut2.Enabled = False
        AdcSingleSample.Enabled = False
        adcDigitalTrg.Enabled = False
        AdcAnalogTrg.Enabled = False
        adcGetStatus.Enabled = False
        AdcGetData.Enabled = False
    End Sub
    Private Sub Function_Enable()
        '***�I�[�v����ID�I���{�^���݂̂𖳌��ɂ���B***
        DEVOPEN.Enabled = False '***�I�[�v���{�^������
        DEVCLOSE.Enabled = True '***�N���[�Y�{�^���L��
        IDNUM.Enabled = False '***ID�I���{�^������
        '***�ȉ��̃{�^�����ׂėL��
        PioLowDirection.Enabled = True
        PioHighDirection.Enabled = True
        PioLowByteRead.Enabled = True
        PioHighByteRead.Enabled = True
        PioLowByteWrite.Enabled = True
        PioHighByteWrite.Enabled = True
        PioWordRead.Enabled = True
        PioWordWrite.Enabled = True
        PioClockOut.Enabled = True
        PioClockStop.Enabled = True
        DacOut1.Enabled = True
        DacOut2.Enabled = True
        AdcSingleSample.Enabled = True
        AdcDigitalTrg.Enabled = True
        AdcAnalogTrg.Enabled = True
        AdcGetStatus.Enabled = True
        AdcGetData.Enabled = True
    End Sub

    Private Sub PioLowDirection_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PioLowDirection.Click
        '***���ʃo�C�g�̓��o�͕����ݒ�
        Dim Ret As String
        Dim Data As Byte

        Ret = InputBox("���ʃo�C�g�̓��o�͒l����͂��Ă�������(HEX)", "�o�͕����ݒ�", "0")
        If Ret.Length > 0 Then  '***OK�ŕԂ��Ă��Ă��邩
            If (IsNumeric("&h" & Ret)) Then
                Data = CByte("&h" & Ret)
                If (Data >= 0 And Data <= &HFFS) Then '*** �f�[�^�͈̔͂ɂ��邩
                    If Adapio_Pio_LowDirection(IDNUM.SelectedIndex, Data) <> 0 Then '***DLL�֐��̎��s
                        MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                        DevClose_Click(DevClose, New System.EventArgs())
                    End If
                Else
                    MsgBox("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
                End If
            End If
        End If
    End Sub

    Private Sub PioHighDirection_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PioHighDirection.Click
        '***��ʃo�C�g�̓��o�͕����ݒ�
        Dim Ret As String
        Dim Data As Byte

        Ret = InputBox("��ʃo�C�g�̓��o�͒l����͂��Ă�������(HEX)", "�o�͕����ݒ�", "0")
        If Ret.Length > 0 Then  '***OK�ŕԂ��Ă��Ă��邩
            If (IsNumeric("&h" & Ret)) Then
                Data = CByte("&h" & Ret)
                If (Data >= 0 And Data <= &HFFS) Then '*** �f�[�^�͈̔͂ɂ��邩
                    If Adapio_Pio_HighDirection(IDNUM.SelectedIndex, Data) <> 0 Then '***DLL�֐��̎��s
                        MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                        DevClose_Click(DevClose, New System.EventArgs())
                    End If
                Else
                    MsgBox("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
                End If
            End If
        End If
    End Sub

    Private Sub PioLowByteWrite_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PioLowByteWrite.Click
        '***���ʃo�C�g�̏o��
        Dim Ret As String
        Dim Data As Byte

        Ret = InputBox("���ʃo�C�g�̏o�͒l����͂��Ă�������(HEX)", "���ʏo��", "0")
        If Ret.Length > 0 Then  '***OK�ŕԂ��Ă��Ă��邩
            If (IsNumeric("&h" & Ret)) Then
                Data = CByte("&h" & Ret)
                If (Data >= 0 And Data <= &HFFS) Then '*** �f�[�^�͈̔͂ɂ��邩
                    If Adapio_Pio_LowByteWrite(IDNUM.SelectedIndex, Data) <> 0 Then '***DLL�֐��̎��s
                        MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                        DevClose_Click(DevClose, New System.EventArgs())
                    End If
                Else
                    MsgBox("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
                End If
            End If
        End If
    End Sub

    Private Sub PioHighByteWrite_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PioHighByteWrite.Click
        '***��ʃo�C�g�̏o��
        Dim Ret As String
        Dim Data As Byte

        Ret = InputBox("��ʃo�C�g�̏o�͒l����͂��Ă�������(HEX)", "��ʏo��", "0")
        If Ret.Length > 0 Then  '***OK�ŕԂ��Ă��Ă��邩
            If (IsNumeric("&h" & Ret)) Then
                Data = CByte("&h" & Ret)
                If (Data >= 0 And Data <= &HFFS) Then '*** �f�[�^�͈̔͂ɂ��邩
                    If Adapio_Pio_HighByteWrite(IDNUM.SelectedIndex, Data) <> 0 Then '***DLL�֐��̎��s
                        MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                        DevClose_Click(DevClose, New System.EventArgs())
                    End If
                Else
                    MsgBox("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
                End If
            End If
        End If
    End Sub

    Private Sub PioLowByteRead_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PioLowByteRead.Click
        '***���ʃo�C�g�̓��̓f�[�^�̎擾
        Dim dat As Byte
        If Adapio_Pio_LowByteRead(IDNUM.SelectedIndex, dat) <> 0 Then '***DLL�֐��̎��s
            MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
            DevClose_Click(DevClose, New System.EventArgs())
        Else
            MsgBox("���ʃo�C�g���͒l�� " & Hex(dat) & "(HEX) �ł�")
        End If
    End Sub

    Private Sub PioHighByteRead_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PioHighByteRead.Click
        '***��ʃo�C�g�̓��̓f�[�^�̎擾
        Dim dat As Byte
        If Adapio_Pio_HighByteRead(IDNUM.SelectedIndex, dat) <> 0 Then '***DLL�֐��̎��s
            MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
            DevClose_Click(DevClose, New System.EventArgs())
        Else
            MsgBox("��ʃo�C�g���͒l�� " & Hex(dat) & "(HEX) �ł�")
        End If
    End Sub

    Private Sub PioWordWrite_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PioWordWrite.Click
        '***���[�h�o��
        Dim Ret As String
        Dim Data As Integer

        Ret = InputBox("�o�͒l����͂��Ă�������(HEX)", "���[�h�o��", "0")
        If Ret.Length > 0 Then  '***OK�ŕԂ��Ă��Ă��邩
            If (IsNumeric("&h" & Ret)) Then
                Data = CInt("&h" & Ret)
                If (Data >= 0 And Data <= &HFFFF) Then '*** �f�[�^�͈̔͂ɂ��邩
                    If Adapio_Pio_WordWrite(IDNUM.SelectedIndex, Data) <> 0 Then '***DLL�֐��̎��s
                        MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                        DevClose_Click(DevClose, New System.EventArgs())
                    End If
                Else
                    MsgBox("�f�[�^�͈̔͂�0�`FFFF(HEX)�ł��B")
                End If
            End If
        End If
    End Sub

    Private Sub PioWordRead_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PioWordRead.Click
        '***���̓��[�h�f�[�^�̎擾
        Dim dat As Integer
        If Adapio_Pio_WordRead(IDNUM.SelectedIndex, dat) <> 0 Then '***DLL�֐��̎��s
            MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
            DevClose_Click(DevClose, New System.EventArgs())
        Else
            MsgBox("���[�h���͒l�� " & Hex(dat) & "(HEX) �ł�")
        End If
    End Sub

    Private Sub PioClockOut_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PioClockOut.Click
        '***���[�h�o��
        Dim Ret As String
        Dim Data As Integer

        Ret = InputBox("�N���b�N�̎�������͂��Ă�������(HEX)�ʕb", "�N���b�N�o��", "0")
        If Ret.Length > 0 Then  '***OK�ŕԂ��Ă��Ă��邩
            If (IsNumeric("&h" & Ret)) Then
                Data = CInt("&h" & Ret)
                If (Data >= 0 And Data <= &HFFFF) Then '*** �f�[�^�͈̔͂ɂ��邩
                    If Adapio_Pio_Clock_Out(IDNUM.SelectedIndex, Data) <> 0 Then '***DLL�֐��̎��s
                        MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                        DevClose_Click(DevClose, New System.EventArgs())
                    End If
                Else
                    MsgBox("�f�[�^�͈̔͂�0�`FFFF(HEX)�ł��B")
                End If
            End If
        End If
    End Sub

    Private Sub PioClockStop_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PioClockStop.Click
        If Adapio_Pio_Clock_Stop(IDNUM.SelectedIndex) <> 0 Then '***DLL�֐��̎��s
            MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
            DevClose_Click(DevClose, New System.EventArgs())
        End If
    End Sub

    Private Sub DacOut1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles DacOut1.Click
        '***DAC1�̏o��
        Dim Ret As String
        Dim Data As Byte

        Ret = InputBox("DAC1�̏o�͒l����͂��Ă�������(HEX)", "DAC1�o��", "0")
        If Ret.Length > 0 Then  '***OK�ŕԂ��Ă��Ă��邩
            If (IsNumeric("&h" & Ret)) Then
                Data = CByte("&h" & Ret)
                If (Data >= 0 And Data <= &HFFS) Then '*** �f�[�^�͈̔͂ɂ��邩
                    If Adapio_Dac_Out(IDNUM.SelectedIndex, 0, Data) <> 0 Then '***DLL�֐��̎��s
                        MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                        DevClose_Click(DevClose, New System.EventArgs())
                    End If
                Else
                    MsgBox("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
                End If
            End If
        End If
    End Sub

    Private Sub DacOut2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles DacOut2.Click
        '***DAC2�̏o��
        Dim Ret As String
        Dim Data As Byte

        Ret = InputBox("DAC2�̏o�͒l����͂��Ă�������(HEX)", "DAC2�o��", "0")
        If Ret.Length > 0 Then  '***OK�ŕԂ��Ă��Ă��邩
            If (IsNumeric("&h" & Ret)) Then
                Data = CByte("&h" & Ret)
                If (Data >= 0 And Data <= &HFFS) Then '*** �f�[�^�͈̔͂ɂ��邩
                    If Adapio_Dac_Out(IDNUM.SelectedIndex, 1, Data) <> 0 Then '***DLL�֐��̎��s
                        MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������݂Ɏ��s
                        DevClose_Click(DevClose, New System.EventArgs())
                    End If
                Else
                    MsgBox("�f�[�^�͈̔͂�0�`FF(HEX)�ł��B")
                End If
            End If
        End If
    End Sub

    Private Sub AdcSingleSample_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AdcSingleSample.Click
        '***��ʃo�C�g�̓��̓f�[�^�̎擾
        Dim dat(6) As Short '***�f�[�^�z��
        Dim ch As Short '***�`�����l��
        Dim TmpStr As String '***������
        Dim strCR As String '***���s������

        strCR = Chr(13) & Chr(10)
        For ch = 0 To 5
            If Adapio_Adc_SingleSample(IDNUM.SelectedIndex, ch, dat(ch)) <> 0 Then '***DLL�֐��̎��s
                MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
                DevClose_Click(DevClose, New System.EventArgs())
                Exit Sub
            End If
        Next
        TmpStr = "ch1�̃f�[�^ " & CStr(dat(0)) & strCR
        TmpStr = TmpStr & "ch2�̃f�[�^ " & CStr(dat(1)) & strCR
        TmpStr = TmpStr & "ch3�̃f�[�^ " & CStr(dat(2)) & strCR
        TmpStr = TmpStr & "ch4�̃f�[�^ " & CStr(dat(3)) & strCR
        TmpStr = TmpStr & "ch5�̃f�[�^ " & CStr(dat(4)) & strCR
        TmpStr = TmpStr & "ch6�̃f�[�^ " & CStr(dat(5))
        MsgBox(TmpStr)
    End Sub

    Private Sub AdcDigitalTrg_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AdcDigitalTrg.Click
        '***���͗p�_�C�A���O�̌Ăяo��
        TrgSetDlg.ThLevel.Visible = False '***���͂Ɏg�p���Ȃ����ڂ͔�\���ɂ���
        TrgSetDlg.TrgCh.Visible = False
        TrgSetDlg.RadioButton1.Visible = False
        TrgSetDlg.RadioButton2.Visible = False
        If (TrgSetDlg.ShowDialog() = DialogResult.OK) Then  '***OK�{�^���������ꂽ��
            If Adapio_Adc_AnalogTrg(IDNUM.SelectedIndex, TrgSetDlg.mEndCh, TrgSetDlg.mBufSize, TrgSetDlg.mThLevel, TrgSetDlg.mTrgCh, TrgSetDlg.mUpDown) <> 0 Then '***DLL�֐��̎��s
                MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
                DevClose_Click(DevClose, New System.EventArgs())
            Else
                SampleDatNum = TrgSetDlg.mBufSize
            End If
        End If
    End Sub

    Private Sub AdcAnalogTrg_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AdcAnalogTrg.Click
        '***�A�i���O�g���K����̐ݒ�ƊJ�n
        TrgSetDlg.ThLevel.Visible = True '***���͂Ɏg�p���鍀�ڂ�\��
        TrgSetDlg.TrgCh.Visible = True
        TrgSetDlg.RadioButton1.Visible = True
        TrgSetDlg.RadioButton2.Visible = True
        If (TrgSetDlg.ShowDialog() = DialogResult.OK) Then  '***OK�{�^���������ꂽ��
            If Adapio_Adc_AnalogTrg(IDNUM.SelectedIndex, TrgSetDlg.mEndCh, TrgSetDlg.mBufSize, TrgSetDlg.mThLevel, TrgSetDlg.mTrgCh, TrgSetDlg.mUpDown) <> 0 Then '***DLL�֐��̎��s
                MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
                DevClose_Click(DevClose, New System.EventArgs())
            Else
                SampleDatNum = TrgSetDlg.mBufSize
            End If
        End If
    End Sub

    Private Sub AdcGetStatus_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AdcGetStatus.Click
        '***AD��荞�݃X�e�[�^�X�̓ǂݍ���
        Dim Running As Short
        Dim SampleNum As Short
        If Adapio_Adc_GetStatus(IDNUM.SelectedIndex, Running, SampleNum) <> 0 Then '***DLL�֐��̎��s
            MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
            DevClose_Click(DevClose, New System.EventArgs())
        Else
            If (Running = 1) Then '***���쒆�ł����
                MsgBox("���쒆   ��荞�ݍς݃f�[�^��(" & CStr(SampleNum) & ")")
            Else
                MsgBox("��荞�݊���   ��荞�ݍς݃f�[�^��(" & CStr(SampleNum) & ")")
            End If
        End If
    End Sub

    Private Sub AdcGetData_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AdcGetData.Click
        '***�T���v�����O�ς݃f�[�^�̓ǂݍ���
        Dim i As Integer
        Dim dat(512) As Short
        Dim wfile As Scripting.TextStream
        Dim fso As New Scripting.FileSystemObject()

        If Adapio_Adc_GetDatas(IDNUM.SelectedIndex, dat(0), SampleDatNum) <> 0 Then '***DLL�֐��̎��s
            MsgBox("DLL�֐��̎��s�Ɏ��s���܂����B�f�o�C�X���N���[�Y���܂��B") '***�������ݎ��s
            DevClose_Click(DevClose, New System.EventArgs())
        Else
            wfile = fso.CreateTextFile("Sample.dat", True) '***�ۑ��t�@�C���̃I�[�v��

            For i = 0 To (SampleDatNum - 1)
                wfile.WriteLine(CStr(dat(i))) '***�ۑ��t�@�C���ւ̏�������
            Next
            wfile.Close()
            MsgBox("��荞�݃f�[�^��sample.dat�ɕۑ�����܂���")
        End If
    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        '***�t�H�[���̕\���J�n���̏���������***
        Function_Disable() '***
        IDNUM.SelectedIndex = 0 '***�����I��ID��0�ɂ���
    End Sub

    Private Sub Form1_Closed(ByVal sender As Object, ByVal e As System.EventArgs) Handles MyBase.Closed
        '***�A�v���P�[�V�����I�����̏���
        If DevClose.Enabled Then
            Adapio_Device_Close(IDNUM.Items.Count) '***�f�o�C�X�N���[�Y
        End If
    End Sub
End Class
