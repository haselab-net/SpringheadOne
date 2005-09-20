VERSION 5.00
Begin VB.Form DATAIN 
   BorderStyle     =   3  '固定ﾀﾞｲｱﾛｸﾞ
   Caption         =   "データ入力"
   ClientHeight    =   1290
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   6030
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1290
   ScaleWidth      =   6030
   ShowInTaskbar   =   0   'False
   Begin VB.TextBox Text1 
      Height          =   270
      Left            =   360
      TabIndex        =   3
      Text            =   "Text1"
      Top             =   720
      Width           =   2895
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
   Begin VB.Label Label1 
      Caption         =   "(HEX)"
      Height          =   255
      Left            =   3480
      TabIndex        =   4
      Top             =   840
      Width           =   615
   End
   Begin VB.Label MsgText 
      Caption         =   "Msg"
      Height          =   255
      Left            =   360
      TabIndex        =   2
      Top             =   240
      Width           =   3975
   End
End
Attribute VB_Name = "DATAIN"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public dat As Long
Public ret As Boolean

Option Explicit

Private Sub CancelButton_Click()
    ret = False
    Hide
End Sub

Private Sub OKButton_Click()
    If (HexTextCheck) Then
        dat = Val("&H0" + Text1.Text + "&")  '***16進文字列データを数値に変換
        ret = True
        Hide                                '***ウィンドゥを消す
    End If
End Sub

Private Function HexTextCheck() As Boolean
'*** Text1 に16進文字以外の文字が含まれていないかチェックする
    Dim i
    For i = 1 To Len(Text1.Text)
      Select Case Mid(Text1.Text, i, 1)
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
        MsgBox ("入力できるのは16進値のみです")
        HexTextCheck = False   '***16進以外の文字があった
        Exit Function
      End Select
      HexTextCheck = True      '***16進以外の文字はなかった
    Next
End Function
