VERSION 5.00
Begin VB.Form TRGSET 
   BorderStyle     =   3  '固定ﾀﾞｲｱﾛｸﾞ
   Caption         =   "取り込み条件選択"
   ClientHeight    =   3300
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   6030
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3300
   ScaleWidth      =   6030
   ShowInTaskbar   =   0   'False
   Begin VB.OptionButton Option2 
      Caption         =   "立下り"
      Height          =   255
      Left            =   2760
      TabIndex        =   15
      Top             =   2880
      Width           =   1935
   End
   Begin VB.OptionButton Option1 
      Caption         =   "立上がり"
      Height          =   255
      Left            =   720
      TabIndex        =   14
      Top             =   2880
      Width           =   1695
   End
   Begin VB.TextBox TrgCh 
      Height          =   375
      Left            =   2520
      TabIndex        =   12
      Top             =   2280
      Width           =   615
   End
   Begin VB.TextBox ThLevel 
      Height          =   375
      Left            =   2520
      TabIndex        =   9
      Top             =   1800
      Width           =   1095
   End
   Begin VB.TextBox BufSize 
      Height          =   375
      Left            =   2520
      TabIndex        =   7
      Text            =   "Text2"
      Top             =   1320
      Width           =   1095
   End
   Begin VB.TextBox EndCh 
      Height          =   375
      Left            =   2520
      TabIndex        =   4
      Text            =   "Text1"
      Top             =   840
      Width           =   615
   End
   Begin VB.CommandButton CancelButton 
      Caption         =   "ｷｬﾝｾﾙ"
      Height          =   375
      Left            =   4680
      TabIndex        =   1
      Top             =   600
      Width           =   1215
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Height          =   375
      Left            =   4680
      TabIndex        =   0
      Top             =   120
      Width           =   1215
   End
   Begin VB.Label Label7 
      Caption         =   "ch"
      Height          =   255
      Left            =   3240
      TabIndex        =   13
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label6 
      Caption         =   "閾値判定チャンネル"
      Height          =   255
      Left            =   360
      TabIndex        =   11
      Top             =   2400
      Width           =   1935
   End
   Begin VB.Label Label3 
      Caption         =   "HEX"
      Height          =   255
      Left            =   3840
      TabIndex        =   10
      Top             =   1920
      Width           =   615
   End
   Begin VB.Label Label5 
      Caption         =   "閾値(1～3FE)"
      Height          =   255
      Left            =   360
      TabIndex        =   8
      Top             =   1920
      Width           =   1935
   End
   Begin VB.Label Label4 
      Caption         =   "バッファサイズ(MAX512)"
      Height          =   255
      Left            =   360
      TabIndex        =   6
      Top             =   1440
      Width           =   2055
   End
   Begin VB.Label Label2 
      Caption         =   "ch"
      Height          =   255
      Left            =   3240
      TabIndex        =   5
      Top             =   960
      Width           =   495
   End
   Begin VB.Label Label1 
      Caption         =   "取り込みチャンネル　0～"
      Height          =   255
      Left            =   360
      TabIndex        =   3
      Top             =   960
      Width           =   2055
   End
   Begin VB.Label Message 
      Caption         =   "Label1"
      Height          =   375
      Left            =   360
      TabIndex        =   2
      Top             =   240
      Width           =   3855
   End
End
Attribute VB_Name = "TRGSET"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public mThLevel As Integer
Public mEndCh As Integer
Public mBufSize As Integer
Public mTrgCh As Integer
Public mUpDown As Integer
Public ret As Boolean

Option Explicit

Private Sub CancelButton_Click()
    ret = False
    Hide
End Sub

Private Sub OKButton_Click()
    If (ThLevel.Visible) Then '***閾値の設定がVisibleならば
        If (HexTextCheck = False) Then '***閾値にHEX以外の値が書きこまれていないかチェックする
            Exit Sub
        End If
        mThLevel = Val("&H0" + ThLevel.Text + "&")
        If (mThLevel < 1 Or mThLevel > &H3FE&) Then '***閾値の値のチェック
            Exit Sub
        End If
    End If
    
    If (EndCh.Visible) Then '**** endchの設定がVisibleならば
        mEndCh = Val(EndCh.Text) '***EndChの値のチェックをする
        If (mEndCh < 0 Or mEndCh > 3) Then
            MsgBox ("終了チャンネルは0～3としてください")
            Exit Sub
        End If
    End If
    
    If (BufSize.Visible) Then '***bufSizeの設定がVisibleならば
        mBufSize = Val(BufSize.Text) '***BufSizeの値のチェックをする
        If (mBufSize < 1 Or mBufSize > 512) Then
            MsgBox ("バッファサイズは1～512としてください")
            Exit Sub
        End If
    End If
    
    If (TrgCh.Visible) Then '***trgChの設定がVisibleならば
        mTrgCh = Val(TrgCh.Text) '***TrgChの値のチェックをする
        If (mTrgCh < 0 Or mTrgCh > 3) Then
            MsgBox ("トリガ入力チャンネルは0～3としてください")
            Exit Sub
        End If
    End If
    
    If (Option1.Visible) Then '***Optionの設定がVisibleならば
        If (Option1.Value = True) Then
            mUpDown = 0 '***立下りは0
        Else
            mUpDown = 1 '***立ち上がりは1
      End If
    End If
    
    ret = True
    Hide
End Sub
Private Function HexTextCheck() As Boolean
'*** Text1 に16進文字以外の文字が含まれていないかチェックする
    Dim i
    HexTextCheck = True
    For i = 1 To Len(ThLevel.Text)
      Select Case Mid(ThLevel.Text, i, 1)
      Case "0"
      Case "1"
      Case "2"
      Case "3"
      Case "4"
      Case "5"
      Case "6"
      Case "7"
      Case "8"
      Case "9"
      Case "A"
      Case "B"
      Case "C"
      Case "D"
      Case "E"
      Case "F"
      Case "a"
      Case "b"
      Case "c"
      Case "d"
      Case "e"
      Case "f"
      Case Else
        MsgBox ("閾値に入力できるのは16進値のみです")
        HexTextCheck = False   '***16進以外の文字があった
        Exit Function
      End Select
      HexTextCheck = True      '***16進以外の文字はなかった
    Next
End Function

