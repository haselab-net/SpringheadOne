object Form1: TForm1
  Left = 192
  Top = 130
  BorderStyle = bsSingle
  Caption = 'Form1'
  ClientHeight = 424
  ClientWidth = 688
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -15
  Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 120
  TextHeight = 15
  object OpenDialog1: TOpenDialog
    Left = 608
    Top = 400
  end
  object MainMenu1: TMainMenu
    Left = 560
    Top = 384
    object OpenO1: TMenuItem
      Caption = 'Open(&O)'
      OnClick = OpenO1Click
    end
    object StopS1: TMenuItem
      Caption = 'Stop(&S)'
      OnClick = StopS1Click
    end
  end
end
