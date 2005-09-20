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
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CommandButton GetDatas 
      Caption         =   "Adc_GetDatas"
      Height          =   495
      Left            =   4080
      TabIndex        =   21
      ToolTipText     =   "連続取り込み済みのデータを取り込みます"
      Top             =   4080
      Width           =   1935
   End
   Begin VB.CommandButton GetStatus 
      Caption         =   "Adc_GetStatus"
      Height          =   495
      Left            =   4080
      TabIndex        =   20
      ToolTipText     =   "連続取り込みステータスを取得機します"
      Top             =   3480
      Width           =   1935
   End
   Begin VB.CommandButton AnalogTrg 
      Caption         =   "Adc_Analog_Trg"
      Height          =   495
      Left            =   4080
      TabIndex        =   19
      ToolTipText     =   "外部ディジタル入力とアナログ入力値でトリガ、サンプリングします。"
      Top             =   2880
      Width           =   1935
   End
   Begin VB.CommandButton DigitalTrg 
      Caption         =   "Adc_DigitalTrg"
      Height          =   495
      Left            =   4080
      TabIndex        =   18
      ToolTipText     =   "外部ディジタルトリガでサンプリングします"
      Top             =   2280
      Width           =   1935
   End
   Begin VB.CommandButton SingleSample 
      Caption         =   "Adc_Single_Sample"
      Height          =   495
      Left            =   4080
      TabIndex        =   17
      ToolTipText     =   "1チャンネルのデータを変換します"
      Top             =   1680
      Width           =   1935
   End
   Begin VB.CommandButton DacOut1 
      Caption         =   "Dac_Out1"
      Height          =   495
      Left            =   4080
      TabIndex        =   15
      ToolTipText     =   "DAコンバータにデータ(ch1)にデータを出力します"
      Top             =   480
      Width           =   975
   End
   Begin VB.CommandButton ClkStop 
      Caption         =   "Pio_Clock_Stop"
      Height          =   495
      Left            =   2040
      TabIndex        =   13
      ToolTipText     =   "クロックを停止します"
      Top             =   4080
      Width           =   1575
   End
   Begin VB.CommandButton ClkOut 
      Caption         =   "Pio_Clock_Out"
      Height          =   495
      Left            =   360
      TabIndex        =   12
      ToolTipText     =   "クロックを出力します"
      Top             =   4080
      Width           =   1575
   End
   Begin VB.CommandButton WordRead 
      Caption         =   "Pio_WordRead"
      Height          =   495
      Left            =   360
      TabIndex        =   11
      ToolTipText     =   "ワード読み込みをします"
      Top             =   3360
      Width           =   3255
   End
   Begin VB.CommandButton WordWrite 
      Caption         =   "Pio_WordWrite"
      Height          =   495
      Left            =   360
      TabIndex        =   10
      ToolTipText     =   "ワード書きこみをします"
      Top             =   2640
      Width           =   3255
   End
   Begin VB.CommandButton HighWrite 
      Caption         =   "Pio_HighByteWrite"
      Height          =   495
      Left            =   2040
      TabIndex        =   9
      ToolTipText     =   "上位バイトの出力データを設定します"
      Top             =   1200
      Width           =   1575
   End
   Begin VB.CommandButton LowWrite 
      Caption         =   "Pio_LowByteWrite"
      Height          =   495
      Left            =   360
      TabIndex        =   8
      ToolTipText     =   "下位バイトの出力データを設定します"
      Top             =   1200
      Width           =   1575
   End
   Begin VB.CommandButton HighRead 
      Caption         =   "Pio_HighByteRead"
      Height          =   495
      Left            =   2040
      TabIndex        =   7
      ToolTipText     =   "上位バイトのデータを読み込みます"
      Top             =   1920
      Width           =   1575
   End
   Begin VB.CommandButton LowRead 
      Caption         =   "Pio_LowByteRead"
      Height          =   495
      Left            =   360
      TabIndex        =   6
      ToolTipText     =   "下位バイトのデータを読み込みます"
      Top             =   1920
      Width           =   1575
   End
   Begin VB.CommandButton HighDir 
      Caption         =   "Pio_HighDirection"
      Height          =   495
      Left            =   2040
      TabIndex        =   5
      ToolTipText     =   "上位バイトの入出力を決定します"
      Top             =   480
      Width           =   1575
   End
   Begin VB.CommandButton LowDir 
      Caption         =   "Pio_LowDirection"
      Height          =   495
      Left            =   360
      TabIndex        =   4
      ToolTipText     =   "下位バイトの入出力を決定します"
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
      ToolTipText     =   "デバイスをクローズします"
      Top             =   4920
      Width           =   1455
   End
   Begin VB.CommandButton DEVOPEN 
      Caption         =   "Dev Open"
      Height          =   375
      Left            =   240
      TabIndex        =   0
      ToolTipText     =   "デバイスをオープンします"
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
         ToolTipText     =   "DAコンバータにデータ(ch2)にデータを出力します"
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
      Alignment       =   1  '右揃え
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
Dim SampleDatNum As Integer '***連続取り込みで設定したデータ数の保存

Private Sub AnalogTrg_Click()
'***アナログトリガ動作の設定と開始
    TRGSET.Message.Caption = "トリガ条件を設定してください"
    TRGSET.EndCh.Text = "0" '***ダイアログに表示する初期値を設定する
    TRGSET.BufSize.Text = "1"
    TRGSET.ThLevel.Text = "1"
    TRGSET.TrgCh.Text = "0"
    TRGSET.Option1.Value = True
    TRGSET.Show 1 '***入力用ダイアログの呼び出し
    If (TRGSET.ret) Then  '***OKボタンが押されたか
        If Adapio_Adc_AnalogTrg(IDNUM.ListIndex, TRGSET.mEndCh, TRGSET.mBufSize, TRGSET.mThLevel, TRGSET.mTrgCh, TRGSET.mUpDown) <> 0 Then '***DLL関数の実行
            MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみ失敗
            DEVCLOSE_Click
        Else
            SampleDatNum = TRGSET.mBufSize
        End If
    End If
    Unload TRGSET
End Sub

Private Sub ClkOut_Click()
'***クロックの出力
    DATAIN.Text1.Text = 0                   '***初期値は0
    DATAIN.MsgText.Caption = "クロックの周期を入力してください(HEX)"
    DATAIN.Show 1                           '***モーダルダイアログ表示
    If DATAIN.ret Then                      '***OKで返ってきているか
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFFFF&) Then '*** データの範囲にあるか
            If Adapio_Pio_Clock_Out(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL関数の実行
                MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみに失敗
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("データの範囲は0～FFFF(HEX)です。")
        End If
    End If
    Unload DATAIN
End Sub
'***クロック出力の停止
Private Sub ClkStop_Click()
    If Adapio_Pio_Clock_Stop(IDNUM.ListIndex) <> 0 Then '***DLL関数の実行
        MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみに失敗
        DEVCLOSE_Click
    End If
End Sub

Private Sub DacOut1_Click()
'***DAC 1ch の出力データの設定
    DATAIN.Text1.Text = 0                   '***初期値は0
    DATAIN.MsgText.Caption = "DAC(ch1)への出力値を入力してください(HEX)"
    DATAIN.Show 1                           '***モーダルダイアログ表示
    If DATAIN.ret Then                      '***OKで返ってきているか
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** データの範囲にあるか
            If Adapio_Dac_Out(IDNUM.ListIndex, 0, DATAIN.dat) <> 0 Then '***DLL関数の実行
                MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみに失敗
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("データの範囲は0～FF(HEX)です。")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub DacOut2_Click()
'***DAC 2ch の出力データの設定
    DATAIN.Text1.Text = 0                   '***初期値は0
    DATAIN.MsgText.Caption = "DAC(ch2)への出力値を入力してください(HEX)"
    DATAIN.Show 1                           '***モーダルダイアログ表示
    If DATAIN.ret Then                      '***OKで返ってきているか
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** データの範囲にあるか
            If Adapio_Dac_Out(IDNUM.ListIndex, 1, DATAIN.dat) <> 0 Then '***DLL関数の実行
                MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみに失敗
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("データの範囲は0～FF(HEX)です。")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub DEVCLOSE_Click()
'***デバイスのクローズ***
    Adapio_Device_Close IDNUM.ListIndex '***デバイスクローズ
    Function_Disable                    '***ボタンの有効無効切り替え
End Sub

Private Sub DEVOPEN_Click()
'***デバイスのオープン***
    If (Adapio_Device_Open(IDNUM.ListIndex) = 0) Then
        Function_Enable                       '***各機能ボタンを有効にする
    Else
        MsgBox ("オープンできませんでした")     '***オープンに失敗した場合
    End If
End Sub

Private Sub DigitalTrg_Click()
'***ディジタルトリガ動作の設定と開始
    TRGSET.Message.Caption = "トリガ条件を設定してください"
    TRGSET.EndCh.Text = "0" '***ダイアログに表示する初期値を設定する
    TRGSET.BufSize.Text = "1"
    TRGSET.ThLevel.Visible = False '***入力に使用しない項目は非表示にする
    TRGSET.TrgCh.Visible = False
    TRGSET.Option1.Visible = False
    TRGSET.Option2.Visible = False
    TRGSET.Show 1 '***入力用ダイアログの呼び出し
    If (TRGSET.ret) Then  '***OKボタンが押されたか
        If Adapio_Adc_DigitalTrg(IDNUM.ListIndex, TRGSET.mEndCh, TRGSET.mBufSize) <> 0 Then '***DLL関数の実行
            MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみ失敗
            DEVCLOSE_Click
        Else
            SampleDatNum = TRGSET.mBufSize
        End If
    End If
    Unload TRGSET
End Sub

Private Sub Form_Load()
'***フォームの表示開始時の初期化処理***
    Function_Disable            '***
    IDNUM.ListIndex = 0         '***初期選択IDを0にする
End Sub

Private Sub Function_Disable()
'***オープンとID選択ボタン以外の各ボタン機能を無効にする***
    DEVCLOSE.Enabled = False    '***クローズボタン無効
    DEVOPEN.Enabled = True      '***オープンボタン有効
    IDNUM.Enabled = True        '***ID選択ボタン有効
    '***以下のボタンすべて無効
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
'***オープンとID選択ボタンのみを無効にする。***
    DEVOPEN.Enabled = False     '***オープンボタン無効
    DEVCLOSE.Enabled = True     '***クローズボタン有効
    IDNUM.Enabled = False       '***ID選択ボタン無効
    '***以下のボタンすべて有効
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
'***アプリケーション終了時の処理
    If DEVCLOSE.Enabled Then
        Adapio_Device_Close IDNUM.ListCount '***デバイスクローズ
    End If
End Sub

Private Sub GetDatas_Click()
'***サンプリング済みデータの読み込み
    Dim dat(512) As Integer
    If Adapio_Adc_GetDatas(IDNUM.ListIndex, dat(0), SampleDatNum) <> 0 Then '***DLL関数の実行
        MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみ失敗
        DEVCLOSE_Click
    Else
        Open "Sample.dat" For Output As #1 '***保存ファイルのオープン
        For i = 0 To (SampleDatNum - 1)
            Print #1, (CStr(dat(i)) + ",") '***保存ファイルへの書きこみ
        Next
        Close #1 '***ファイルのクローズ
        MsgBox ("取り込みデータはsample.datに保存されました")
    End If
End Sub

Private Sub GetStatus_Click()
'***AD取り込みステータスの読み込み
    Dim Running As Integer
    Dim SampleNum As Integer
    If Adapio_Adc_GetStatus(IDNUM.ListIndex, Running, SampleNum) <> 0 Then '***DLL関数の実行
        MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみ失敗
        DEVCLOSE_Click
    Else
        If (Running = 1) Then  '***動作中であれば
            MsgBox ("動作中   取り込み済みデータ数(" + CStr(SampleNum) + ")")
        Else
            MsgBox ("取り込み完了   取り込み済みデータ数(" + CStr(SampleNum) + ")")
       End If
    End If
End Sub

Private Sub HighDir_Click()
'***上位バイトの入出力方向設定
    DATAIN.Text1.Text = 0                   '***初期値は0
    DATAIN.MsgText.Caption = "上位バイトの入出力値を入力してください(HEX)"
    DATAIN.Show 1                           '***モーダルダイアログ表示
    If DATAIN.ret Then                      '***OKで返ってきているか
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** データの範囲にあるか
            If Adapio_Pio_HighDirection(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL関数の実行
                MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみに失敗
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("データの範囲は0～FF(HEX)です。")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub HighRead_Click()
'***上位バイトの入力データの取得
    Dim dat As Byte
    If Adapio_Pio_HighByteRead(IDNUM.ListIndex, dat) <> 0 Then '***DLL関数の実行
        MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみ失敗
        DEVCLOSE_Click
    Else
        MsgBox ("上位バイト入力値は " + Hex(dat) + "(HEX) です")
    End If
End Sub

Private Sub HighWrite_Click()
'***上位バイトの出力データ書きこみ
    DATAIN.Text1.Text = 0                   '***初期値は0
    DATAIN.MsgText.Caption = "上位バイトの出力値を入力してください(HEX)"
    DATAIN.Show 1                           '***モーダルダイアログ表示
    If DATAIN.ret Then                      '***OKで返ってきているか
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** データの範囲にあるか
            If Adapio_Pio_HighByteWrite(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL関数の実行
                MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみに失敗
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("データの範囲は0～FF(HEX)です。")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub LowDir_Click()
'***下位バイトの入出力方向設定
    DATAIN.Text1.Text = 0                   '***初期値は0
    DATAIN.MsgText.Caption = "下位バイトの入出力値を入力してください(HEX)"
    DATAIN.Show 1                           '***モーダルダイアログ表示
    If DATAIN.ret Then                      '***OKで返ってきているか
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** データの範囲にあるか
            If Adapio_Pio_LowDirection(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL関数の実行
                MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみに失敗
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("データの範囲は0～FF(HEX)です。")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub LowRead_Click()
'***下位バイトの入力データの取得
    Dim dat As Byte
    If Adapio_Pio_LowByteRead(IDNUM.ListIndex, dat) <> 0 Then '***DLL関数の実行
        MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみ失敗
        DEVCLOSE_Click
    Else
        MsgBox ("下位バイト入力値は " + Hex(dat) + "(HEX) です")
    End If
End Sub

Private Sub LowWrite_Click()
'***下位バイトの出力データ書きこみ
    DATAIN.Text1.Text = 0                   '***初期値は0
    DATAIN.MsgText.Caption = "下位バイトの出力値を入力してください(HEX)"
    DATAIN.Show 1                           '***モーダルダイアログ表示
    If DATAIN.ret Then                      '***OKで返ってきているか
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFF) Then '*** データの範囲にあるか
            If Adapio_Pio_LowByteWrite(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL関数の実行
                MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみに失敗
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("データの範囲は0～FF(HEX)です。")
        End If
    End If
    Unload DATAIN
End Sub

Private Sub SingleSample_Click()
'***上位バイトの入力データの取得
    Dim dat(6) As Integer '***データ配列
    Dim ch              '***チャンネル
    Dim str As String '***文字列
    Dim strCR As String '***改行文字列
    strCR = Chr(13) + Chr(10)
    For ch = 0 To 5
        If Adapio_Adc_SingleSample(IDNUM.ListIndex, ch, dat(ch)) <> 0 Then '***DLL関数の実行
            MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみ失敗
            DEVCLOSE_Click
            Exit Sub
        End If
    Next
    str = "ch1のデータ " + CStr(dat(0)) + strCR
    str = str + "ch2のデータ " + CStr(dat(1)) + strCR
    str = str + "ch3のデータ " + CStr(dat(2)) + strCR
    str = str + "ch4のデータ " + CStr(dat(3)) + strCR
    str = str + "ch5のデータ " + CStr(dat(4)) + strCR
    str = str + "ch6のデータ " + CStr(dat(5))
    MsgBox (str)
End Sub

Private Sub WordRead_Click()
'***入力ワードデータの取得
    Dim dat As Long
    If Adapio_Pio_WordRead(IDNUM.ListIndex, dat) <> 0 Then '***DLL関数の実行
        MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみ失敗
        DEVCLOSE_Click
    Else
        MsgBox ("ワード入力値は " + Hex(dat) + "(HEX) です")
    End If
End Sub

Private Sub WordWrite_Click()
'***ワードデータ書きこみ
    DATAIN.Text1.Text = 0                   '***初期値は0
    DATAIN.MsgText.Caption = "出力値を入力してください(HEX)"
    DATAIN.Show 1                           '***モーダルダイアログ表示
    If DATAIN.ret Then                      '***OKで返ってきているか
        If (DATAIN.dat >= 0 And DATAIN.dat <= &HFFFF&) Then '*** データの範囲にあるか
            If Adapio_Pio_WordWrite(IDNUM.ListIndex, DATAIN.dat) <> 0 Then '***DLL関数の実行
                MsgBox ("DLL関数の実行に失敗しました。デバイスをクローズします。") '***書きこみに失敗
                DEVCLOSE_Click
            End If
        Else
            MsgBox ("データの範囲は0～FFFF(HEX)です。")
        End If
    End If
    Unload DATAIN
End Sub
