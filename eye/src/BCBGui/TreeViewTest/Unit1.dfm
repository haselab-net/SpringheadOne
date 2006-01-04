object Form1: TForm1
  Left = 409
  Top = 158
  Width = 604
  Height = 464
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 12
  object btnLoad: TButton
    Left = 16
    Top = 191
    Width = 33
    Height = 25
    Caption = 'Load'
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = btnLoadClick
  end
  object SHBox1: TSHBox
    Left = 56
    Top = 8
    Width = 537
    Height = 417
    Scene = SHScene1
    CameraName = 'Cam1'
  end
  object btnStep: TButton
    Left = 16
    Top = 114
    Width = 33
    Height = 25
    Caption = 'Step'
    TabOrder = 2
    OnClick = btnStepClick
  end
  object btnRun: TButton
    Left = 16
    Top = 89
    Width = 33
    Height = 25
    Caption = 'Run'
    TabOrder = 3
    OnClick = btnRunClick
  end
  object btnReset: TButton
    Left = 16
    Top = 147
    Width = 33
    Height = 25
    Caption = 'Reset'
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = btnResetClick
  end
  object btnTree: TButton
    Left = 16
    Top = 56
    Width = 33
    Height = 25
    Caption = 'Tree'
    TabOrder = 1
    OnClick = btnTreeClick
  end
  object SHScene1: TSHScene
    Scene = SHScene1
    FileName = 'SHScene1.x'
    Left = 16
    Top = 16
  end
  object Timer1: TTimer
    Interval = 5
    OnTimer = Timer1Timer
    Left = 16
    Top = 362
  end
  object OpenDialog1: TOpenDialog
    Left = 16
    Top = 394
  end
end
