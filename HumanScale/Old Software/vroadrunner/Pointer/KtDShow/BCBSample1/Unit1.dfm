object Form1: TForm1
  Left = 192
  Top = 130
  Width = 696
  Height = 480
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 12
  object OpenDialog1: TOpenDialog
    Left = 296
    Top = 224
  end
  object MainMenu1: TMainMenu
    Left = 336
    Top = 224
    object OpenO1: TMenuItem
      Caption = 'Open(&O)'
      OnClick = OpenO1Click
    end
    object Stop1: TMenuItem
      Caption = 'Stop(&S)'
      OnClick = Stop1Click
    end
  end
end
