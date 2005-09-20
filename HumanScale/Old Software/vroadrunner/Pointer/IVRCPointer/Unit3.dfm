object SettingForm: TSettingForm
  Left = 192
  Top = 107
  BorderStyle = bsDialog
  Caption = #35373#23450
  ClientHeight = 169
  ClientWidth = 234
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 12
  object Label1: TLabel
    Left = 30
    Top = 8
    Width = 15
    Height = 15
    Caption = #24133
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 24
    Top = 32
    Width = 26
    Height = 15
    Caption = #39640#12373
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 22
    Top = 56
    Width = 30
    Height = 15
    Caption = 'Host'
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 24
    Top = 80
    Width = 27
    Height = 15
    Caption = 'Port'
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 8
    Top = 104
    Width = 58
    Height = 15
    Caption = #24453#12385#26178#38291
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
    Font.Style = []
    ParentFont = False
  end
  object EditWidth: TEdit
    Left = 72
    Top = 8
    Width = 121
    Height = 20
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = #65325#65331' '#12468#12471#12483#12463
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = 'EditWidth'
  end
  object EditHeight: TEdit
    Left = 72
    Top = 32
    Width = 121
    Height = 20
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = #65325#65331' '#12468#12471#12483#12463
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = 'EditHeight'
  end
  object EditHost: TEdit
    Left = 72
    Top = 56
    Width = 121
    Height = 20
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = #65325#65331' '#12468#12471#12483#12463
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = 'EditHost'
  end
  object EditPort: TEdit
    Left = 72
    Top = 80
    Width = 121
    Height = 20
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = #65325#65331' '#12468#12471#12483#12463
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = 'EditPort'
  end
  object Button1: TButton
    Left = 32
    Top = 136
    Width = 73
    Height = 25
    Caption = 'OK(&O)'
    ModalResult = 1
    TabOrder = 4
  end
  object Button2: TButton
    Left = 128
    Top = 136
    Width = 73
    Height = 25
    Caption = 'Cancel(&C)'
    ModalResult = 2
    TabOrder = 5
  end
  object EditSendWaitTime: TEdit
    Left = 72
    Top = 104
    Width = 121
    Height = 20
    TabOrder = 6
    Text = 'EDITWAITSENDTIME'
    OnExit = EditSendWaitTimeExit
  end
end
