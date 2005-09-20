object Form1: TForm1
  Left = 192
  Top = 130
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Form1'
  ClientHeight = 339
  ClientWidth = 550
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 12
  object Image1: TImage
    Left = 0
    Top = 0
    Width = 550
    Height = 339
    Align = alClient
  end
  object OpenDialog1: TOpenDialog
    Left = 504
    Top = 296
  end
  object MainMenu1: TMainMenu
    Left = 464
    Top = 296
    object OpenO1: TMenuItem
      Caption = 'Open(&O)'
      OnClick = OpenO1Click
    end
    object StopS1: TMenuItem
      Caption = 'Stop(&S)'
      OnClick = StopS1Click
    end
    object CaptureC1: TMenuItem
      Caption = 'Capture(&C)'
      OnClick = CaptureC1Click
    end
  end
end
