object MainForm: TMainForm
  Left = 328
  Top = 198
  BorderStyle = bsSingle
  Caption = 'vRoadRunner - IVRC2002'
  ClientHeight = 240
  ClientWidth = 320
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -15
  Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnMouseUp = FormMouseUp
  OnResize = FormResize
  PixelsPerInch = 120
  TextHeight = 15
  object StatusBar1: TStatusBar
    Left = 0
    Top = 219
    Width = 320
    Height = 21
    Panels = <
      item
        Width = 145
      end
      item
        Width = 160
      end
      item
        Width = 80
      end>
    SimplePanel = False
  end
  object MainMenu1: TMainMenu
    Left = 8
    Top = 8
    object O1: TMenuItem
      Caption = #25805#20316'(&O)'
      object OpenO1: TMenuItem
        Action = Start
      end
      object CaptureC1: TMenuItem
        Action = Capture
        ShortCut = 113
      end
      object StopS1: TMenuItem
        Action = Stop
        ShortCut = 27
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object C1: TMenuItem
        Action = CameraProperty
      end
      object V1: TMenuItem
        Action = OutputProperty
      end
      object P1: TMenuItem
        Action = ToggleShowPreview
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object ExitE1: TMenuItem
        Caption = #32066#20102'(&E)'
        ShortCut = 32883
        OnClick = ExitE1Click
      end
    end
    object S1: TMenuItem
      Caption = #35373#23450'(&S)'
      object RGBR1: TMenuItem
        Caption = 'RGB'#38334#20516'(&R)'
        object U1: TMenuItem
          Action = RMinUp
        end
        object D1: TMenuItem
          Action = RMinDown
        end
        object GMaxJ1: TMenuItem
          Action = GMaxUp
        end
        object GMaxC1: TMenuItem
          Action = GMaxDown
        end
        object BMaxM1: TMenuItem
          Action = BMaxUp
        end
        object BMaxV1: TMenuItem
          Action = BMaxDown
        end
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object CalibrateMatrix1: TMenuItem
        Action = CalibrateMatrix
      end
      object P2: TMenuItem
        Action = TogglePositionTransform
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object M1: TMenuItem
        Action = ToggleMoveMouse
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object S2: TMenuItem
        Action = ShowSettingForm
      end
    end
  end
  object ActionList1: TActionList
    Left = 48
    Top = 8
    object Start: TAction
      Caption = #38283#22987'(&P)'
      ShortCut = 112
      OnExecute = StartExecute
    end
    object Stop: TAction
      Caption = #20572#27490'(&S)'
      OnExecute = StopExecute
    end
    object Capture: TAction
      Caption = #12461#12515#12503#12481#12515'(&C)'
      OnExecute = CaptureExecute
    end
    object RMinUp: TAction
      Caption = 'RMin'#22679'(&U)'
      ShortCut = 38
      OnExecute = RMinUpExecute
    end
    object RMinDown: TAction
      Caption = 'RMin'#28187'(&D)'
      ShortCut = 40
      OnExecute = RMinDownExecute
    end
    object GMaxUp: TAction
      Caption = 'GMax'#22679'(&J)'
      ShortCut = 8230
      OnExecute = GMaxUpExecute
    end
    object GMaxDown: TAction
      Caption = 'GMax'#28187'(&C)'
      ShortCut = 8232
      OnExecute = GMaxDownExecute
    end
    object BMaxUp: TAction
      Caption = 'BMax'#22679'(&M)'
      ShortCut = 16422
      OnExecute = BMaxUpExecute
    end
    object BMaxDown: TAction
      Caption = 'BMax'#28187'(&V)'
      ShortCut = 16424
      OnExecute = BMaxDownExecute
    end
    object CameraProperty: TAction
      Caption = #12459#12513#12521#12398#12503#12525#12497#12486#12451'(&C)'
      OnExecute = CameraPropertyExecute
    end
    object OutputProperty: TAction
      Caption = #20986#21147#12398#12503#12525#12497#12486#12451'(&V)'
      OnExecute = OutputPropertyExecute
    end
    object ToggleMoveMouse: TAction
      Caption = #12510#12454#12473#36899#21205'(&M)'
      ShortCut = 115
      OnExecute = ToggleMoveMouseExecute
    end
    object ToggleShowPreview: TAction
      Caption = #12503#12524#12499#12517#12540'(&P)'
      ShortCut = 116
      OnExecute = ToggleShowPreviewExecute
    end
    object LoadTransformMatrix: TAction
      Caption = 'LoadTransformMatrix'
      OnExecute = LoadTransformMatrixExecute
    end
    object TogglePositionTransform: TAction
      Caption = #24231#27161#22793#25563'(&P)'
      ShortCut = 114
      OnExecute = TogglePositionTransformExecute
    end
    object CalibrateMatrix: TAction
      Caption = #38914#28857#24231#27161#25351#23450'(&A)'
      OnExecute = CalibrateMatrixExecute
    end
    object ShowSettingForm: TAction
      Caption = #35373#23450'(&S)'
      OnExecute = ShowSettingFormExecute
    end
  end
  object UdpSocket1: TUdpSocket
    Left = 88
    Top = 8
  end
end
