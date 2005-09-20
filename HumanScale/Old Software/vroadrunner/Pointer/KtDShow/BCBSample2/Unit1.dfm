object Form1: TForm1
  Left = 305
  Top = 155
  Width = 696
  Height = 480
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -15
  Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  PixelsPerInch = 120
  TextHeight = 15
  object Image1: TImage
    Left = 0
    Top = 0
    Width = 688
    Height = 424
    Align = alClient
  end
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
    object CaptureC1: TMenuItem
      Caption = 'Capture(&C)'
      OnClick = CaptureC1Click
    end
    object SaveF1: TMenuItem
      Caption = 'Save(&F)'
      OnClick = SaveF1Click
    end
  end
  object SavePictureDialog1: TSavePictureDialog
    DefaultExt = 'bmp'
    Filter = #12499#12483#12488#12510#12483#12503' (*.bmp)|*.bmp'
    Left = 480
    Top = 216
  end
end
