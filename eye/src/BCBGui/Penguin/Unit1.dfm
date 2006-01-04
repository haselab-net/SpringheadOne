object Form1: TForm1
  Left = 328
  Top = 143
  Width = 696
  Height = 508
  Caption = 'Penguin on Springhead'
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
    Top = 0
    Width = 688
    Height = 441
    Scene = Scene
    CameraName = 'NewCamera'
    Align = alTop
  end
  object btLeft: TButton
    Left = 320
    Top = 456
    Width = 20
    Height = 25
    Caption = '<'
    TabOrder = 1
    OnClick = btLeftClick
  end
  object btStop: TButton
    Left = 344
    Top = 456
    Width = 20
    Height = 25
    Caption = '-'
    TabOrder = 2
    OnClick = btStopClick
  end
  object btRight: TButton
    Left = 368
    Top = 456
    Width = 20
    Height = 25
    Caption = '>'
    TabOrder = 3
    OnClick = btRightClick
  end
  object Button1: TButton
    Left = 112
    Top = 448
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 4
    OnClick = Button1Click
  end
  object Scene: TSHScene
    Scene = Scene
    FileName = 'SHScene1.x'
    Left = 8
    Top = 448
  end
  object Timer1: TTimer
    Interval = 30
    OnTimer = Timer1Timer
    Left = 72
    Top = 448
  end
  object Penguin: TSHFrame
    Scene = Scene
    PostureDummy.ExDummy.X = 1
    PostureDummy.EyDummy.Y = 1
    PostureDummy.EzDummy.Z = 1
    Left = 40
    Top = 448
  end
end
