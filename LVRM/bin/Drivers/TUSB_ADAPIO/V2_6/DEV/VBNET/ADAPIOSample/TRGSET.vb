Public Class TRGSET
    Inherits System.Windows.Forms.Form

#Region " Windows フォーム デザイナで生成されたコード "

    Public Sub New()
        MyBase.New()

        ' この呼び出しは Windows フォーム デザイナで必要です。
        InitializeComponent()

        ' InitializeComponent() 呼び出しの後に初期化を追加します。

    End Sub

    ' Form は dispose をオーバーライドしてコンポーネント一覧を消去します。
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    ' Windows フォーム デザイナで必要です。
    Private components As System.ComponentModel.IContainer

    ' メモ : 以下のプロシージャは、Windows フォーム デザイナで必要です。
    ' Windows フォーム デザイナを使って変更してください。  
    ' コード エディタは使用しないでください。
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents OKBtn As System.Windows.Forms.Button
    Friend WithEvents CancelBtn As System.Windows.Forms.Button
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents Label6 As System.Windows.Forms.Label
    Friend WithEvents Label7 As System.Windows.Forms.Label
    Friend WithEvents Label8 As System.Windows.Forms.Label
    Friend WithEvents RadioButton1 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton2 As System.Windows.Forms.RadioButton
    Friend WithEvents EndCh As System.Windows.Forms.TextBox
    Friend WithEvents BuffSize As System.Windows.Forms.TextBox
    Friend WithEvents ThLevel As System.Windows.Forms.TextBox
    Friend WithEvents TrgCh As System.Windows.Forms.TextBox
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.OKBtn = New System.Windows.Forms.Button()
        Me.CancelBtn = New System.Windows.Forms.Button()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.EndCh = New System.Windows.Forms.TextBox()
        Me.BuffSize = New System.Windows.Forms.TextBox()
        Me.ThLevel = New System.Windows.Forms.TextBox()
        Me.TrgCh = New System.Windows.Forms.TextBox()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.Label8 = New System.Windows.Forms.Label()
        Me.RadioButton1 = New System.Windows.Forms.RadioButton()
        Me.RadioButton2 = New System.Windows.Forms.RadioButton()
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(16, 24)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(208, 16)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "トリガ条件を設定してください"
        '
        'OKBtn
        '
        Me.OKBtn.Location = New System.Drawing.Point(312, 16)
        Me.OKBtn.Name = "OKBtn"
        Me.OKBtn.Size = New System.Drawing.Size(72, 24)
        Me.OKBtn.TabIndex = 1
        Me.OKBtn.Text = "OK"
        '
        'CancelBtn
        '
        Me.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.CancelBtn.Location = New System.Drawing.Point(312, 56)
        Me.CancelBtn.Name = "CancelBtn"
        Me.CancelBtn.Size = New System.Drawing.Size(72, 24)
        Me.CancelBtn.TabIndex = 2
        Me.CancelBtn.Text = "Cancel"
        '
        'Label2
        '
        Me.Label2.Location = New System.Drawing.Point(16, 64)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(144, 16)
        Me.Label2.TabIndex = 3
        Me.Label2.Text = "取り込みチャンネル　0～"
        '
        'Label3
        '
        Me.Label3.Location = New System.Drawing.Point(16, 96)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(144, 16)
        Me.Label3.TabIndex = 4
        Me.Label3.Text = "バッファサイズ(MAX512)"
        '
        'Label4
        '
        Me.Label4.Location = New System.Drawing.Point(16, 128)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(144, 16)
        Me.Label4.TabIndex = 5
        Me.Label4.Text = "閾値(1～3FE)"
        '
        'Label5
        '
        Me.Label5.Location = New System.Drawing.Point(16, 160)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(144, 16)
        Me.Label5.TabIndex = 6
        Me.Label5.Text = "閾値判定チャンネル"
        '
        'EndCh
        '
        Me.EndCh.Location = New System.Drawing.Point(168, 56)
        Me.EndCh.Name = "EndCh"
        Me.EndCh.Size = New System.Drawing.Size(40, 19)
        Me.EndCh.TabIndex = 7
        Me.EndCh.Text = "0"
        '
        'BuffSize
        '
        Me.BuffSize.Location = New System.Drawing.Point(168, 88)
        Me.BuffSize.Name = "BuffSize"
        Me.BuffSize.Size = New System.Drawing.Size(64, 19)
        Me.BuffSize.TabIndex = 8
        Me.BuffSize.Text = "100"
        '
        'ThLevel
        '
        Me.ThLevel.Location = New System.Drawing.Point(168, 120)
        Me.ThLevel.Name = "ThLevel"
        Me.ThLevel.Size = New System.Drawing.Size(64, 19)
        Me.ThLevel.TabIndex = 9
        Me.ThLevel.Text = "1ff"
        '
        'TrgCh
        '
        Me.TrgCh.Location = New System.Drawing.Point(168, 152)
        Me.TrgCh.Name = "TrgCh"
        Me.TrgCh.Size = New System.Drawing.Size(40, 19)
        Me.TrgCh.TabIndex = 10
        Me.TrgCh.Text = "0"
        '
        'Label6
        '
        Me.Label6.Location = New System.Drawing.Point(216, 64)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(32, 16)
        Me.Label6.TabIndex = 11
        Me.Label6.Text = "ch"
        '
        'Label7
        '
        Me.Label7.Location = New System.Drawing.Point(216, 160)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(32, 16)
        Me.Label7.TabIndex = 12
        Me.Label7.Text = "ch"
        '
        'Label8
        '
        Me.Label8.Location = New System.Drawing.Point(240, 128)
        Me.Label8.Name = "Label8"
        Me.Label8.Size = New System.Drawing.Size(48, 16)
        Me.Label8.TabIndex = 13
        Me.Label8.Text = "HEX"
        '
        'RadioButton1
        '
        Me.RadioButton1.Checked = True
        Me.RadioButton1.Location = New System.Drawing.Point(80, 184)
        Me.RadioButton1.Name = "RadioButton1"
        Me.RadioButton1.Size = New System.Drawing.Size(80, 24)
        Me.RadioButton1.TabIndex = 14
        Me.RadioButton1.TabStop = True
        Me.RadioButton1.Text = "立上がり"
        '
        'RadioButton2
        '
        Me.RadioButton2.Location = New System.Drawing.Point(176, 184)
        Me.RadioButton2.Name = "RadioButton2"
        Me.RadioButton2.Size = New System.Drawing.Size(80, 24)
        Me.RadioButton2.TabIndex = 15
        Me.RadioButton2.Text = "立下り"
        '
        'TRGSET
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 12)
        Me.ClientSize = New System.Drawing.Size(400, 222)
        Me.Controls.AddRange(New System.Windows.Forms.Control() {Me.RadioButton2, Me.RadioButton1, Me.Label8, Me.Label7, Me.Label6, Me.TrgCh, Me.ThLevel, Me.BuffSize, Me.EndCh, Me.Label5, Me.Label4, Me.Label3, Me.Label2, Me.CancelBtn, Me.OKBtn, Me.Label1})
        Me.Name = "TRGSET"
        Me.Text = "取り込み条件選択"
        Me.ResumeLayout(False)

    End Sub

#End Region
    Public mThLevel As Short
    Public mEndCh As Short
    Public mBufSize As Short
    Public mTrgCh As Short
    Public mUpDown As Short

    Private Sub OKBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OKBtn.Click
        If (ThLevel.Visible) Then '***閾値の設定がVisibleならば
            If (IsNumeric("&h" & ThLevel.Text) = False) Then  '***閾値にHEX以外の値が書きこまれていないかチェックする
                Exit Sub
            End If
            mThLevel = Val("&H0" & ThLevel.Text & "&")
            If (mThLevel < 1 Or mThLevel > &H3FE) Then '***閾値の値のチェック
                Exit Sub
            End If
        End If

        If (EndCh.Visible) Then '**** endchの設定がVisibleならば
            mEndCh = Val(EndCh.Text) '***EndChの値のチェックをする
            If (mEndCh < 0 Or mEndCh > 3) Then
                MsgBox("終了チャンネルは0～3としてください")
                Exit Sub
            End If
        End If

        If (BuffSize.Visible) Then '***bufSizeの設定がVisibleならば
            mBufSize = Val(BuffSize.Text) '***BufSizeの値のチェックをする
            If (mBufSize < 1 Or mBufSize > 512) Then
                MsgBox("バッファサイズは1～512としてください")
                Exit Sub
            End If
        End If

        If (TrgCh.Visible) Then '***trgChの設定がVisibleならば
            mTrgCh = Val(TrgCh.Text) '***TrgChの値のチェックをする
            If (mTrgCh < 0 Or mTrgCh > 3) Then
                MsgBox("トリガ入力チャンネルは0～3としてください")
                Exit Sub
            End If
        End If

        If (RadioButton1.Visible) Then '***Optionの設定がVisibleならば
            If (RadioButton1.Checked = True) Then
                mUpDown = 0 '***立上りは0
            Else
                mUpDown = 1 '***立下がりは1
            End If
        End If

        Me.DialogResult = DialogResult.OK
        Hide()
    End Sub
End Class
