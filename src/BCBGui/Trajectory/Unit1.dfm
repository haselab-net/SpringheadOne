object Form1: TForm1
  Left = 2
  Top = 108
  Width = 1014
  Height = 650
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
  object SHBox1: TSHBox
    Left = 0
    Top = 24
    Width = 425
    Height = 409
    Scene = SHScene1
    CameraName = 'cam1'
  end
  object btStep: TButton
    Left = 0
    Top = 0
    Width = 57
    Height = 25
    Caption = 'Step'
    TabOrder = 1
    OnClick = btStepClick
  end
  object btRun: TButton
    Left = 64
    Top = 0
    Width = 57
    Height = 25
    Caption = 'Run'
    TabOrder = 2
    OnClick = btRunClick
  end
  object SHPathGraph1: TSHPathGraph
    Left = 432
    Top = 96
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
    JointId = 1
  end
  object btLoad: TButton
    Left = 128
    Top = 0
    Width = 57
    Height = 25
    Caption = 'Load'
    TabOrder = 4
    OnClick = btLoadClick
  end
  object btSave: TButton
    Left = 192
    Top = 0
    Width = 57
    Height = 25
    Caption = 'Save'
    TabOrder = 5
    OnClick = btSaveClick
  end
  object SHPathGraph2: TSHPathGraph
    Left = 432
    Top = 192
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
    JointId = 2
  end
  object SHPathGraph3: TSHPathGraph
    Left = 432
    Top = 288
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
    JointId = 4
  end
  object SHPathGraph4: TSHPathGraph
    Left = 432
    Top = 384
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
    JointId = 5
  end
  object SHPathGraph5: TSHPathGraph
    Left = 432
    Top = 0
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
  end
  object SHPathGraph6: TSHPathGraph
    Left = 432
    Top = 480
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
    JointId = 6
  end
  object SHPathGraph7: TSHPathGraph
    Left = 720
    Top = 0
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
    JointId = 8
  end
  object SHPathGraph8: TSHPathGraph
    Left = 720
    Top = 96
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
    JointId = 9
  end
  object SHPathGraph9: TSHPathGraph
    Left = 720
    Top = 192
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
    JointId = 10
  end
  object SHPathGraph10: TSHPathGraph
    Left = 720
    Top = 288
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
    JointId = 12
  end
  object SHPathGraph11: TSHPathGraph
    Left = 720
    Top = 384
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
    JointId = 13
  end
  object SHPathGraph12: TSHPathGraph
    Left = 720
    Top = 480
    Width = 281
    Height = 89
    DoubleBuffered = True
    Color = clWhite
    CurRectColor = clBlue
    BarColor = clMaroon
    JointId = 14
  end
  object SHBox2: TSHBox
    Left = 0
    Top = 440
    Width = 201
    Height = 177
    Scene = SHScene1
    CameraName = 'cam2'
  end
  object SHScene1: TSHScene
    Scene = SHScene1
    FileName = 'SHScene1.x'
    Left = 328
  end
  object Timer1: TTimer
    Interval = 10
    OnTimer = Timer1Timer
    Left = 16
    Top = 224
  end
end
