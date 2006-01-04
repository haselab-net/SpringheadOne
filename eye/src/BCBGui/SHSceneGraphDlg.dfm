object SHSceneGraphDlg: TSHSceneGraphDlg
  Left = 27
  Top = 107
  Width = 409
  Height = 537
  BorderStyle = bsSizeToolWin
  Caption = 'SceneGraph'
  Color = clBtnFace
  DockSite = True
  DragKind = dkDock
  DragMode = dmAutomatic
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 12
  object Splitter2: TSplitter
    Left = 0
    Top = 256
    Width = 401
    Height = 5
    Cursor = crVSplit
    Align = alBottom
  end
  object Tree: TSHTreeView
    Left = 0
    Top = 0
    Width = 401
    Height = 256
    Align = alClient
    BorderStyle = bsSizeable
    Images = ImageListForTree
    PopupMenu = TreeMenu
    OnKeyDown = TreeKeyDown
    OnChange = TreeChange
  end
  object ParentPanel: TPanel
    Left = 0
    Top = 261
    Width = 401
    Height = 249
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object PHMaterialPanel: TPanel
      Left = 0
      Top = 0
      Width = 401
      Height = 249
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 3
      object Label1: TLabel
        Left = 177
        Top = 42
        Width = 6
        Height = 11
        Caption = '0'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label2: TLabel
        Left = 218
        Top = 42
        Width = 20
        Height = 11
        Caption = '0.01'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label3: TLabel
        Left = 269
        Top = 42
        Width = 14
        Height = 11
        Caption = '1.0'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label4: TLabel
        Left = 315
        Top = 42
        Width = 18
        Height = 11
        Caption = '100'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label5: TLabel
        Left = 357
        Top = 42
        Width = 30
        Height = 11
        Caption = '10000'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 177
        Top = 81
        Width = 6
        Height = 11
        Caption = '0'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label7: TLabel
        Left = 218
        Top = 81
        Width = 20
        Height = 11
        Caption = '0.01'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label8: TLabel
        Left = 269
        Top = 81
        Width = 14
        Height = 11
        Caption = '1.0'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label9: TLabel
        Left = 315
        Top = 81
        Width = 18
        Height = 11
        Caption = '100'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label10: TLabel
        Left = 357
        Top = 81
        Width = 30
        Height = 11
        Caption = '10000'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label13: TLabel
        Left = 269
        Top = 120
        Width = 14
        Height = 11
        Caption = '1.0'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label11: TLabel
        Left = 177
        Top = 120
        Width = 6
        Height = 11
        Caption = '0'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label12: TLabel
        Left = 218
        Top = 120
        Width = 20
        Height = 11
        Caption = '0.01'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label14: TLabel
        Left = 315
        Top = 120
        Width = 18
        Height = 11
        Caption = '100'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label15: TLabel
        Left = 357
        Top = 120
        Width = 30
        Height = 11
        Caption = '10000'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label16: TLabel
        Left = 269
        Top = 159
        Width = 14
        Height = 11
        Caption = '1.0'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label17: TLabel
        Left = 177
        Top = 159
        Width = 6
        Height = 11
        Caption = '0'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label18: TLabel
        Left = 218
        Top = 159
        Width = 20
        Height = 11
        Caption = '0.01'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label19: TLabel
        Left = 315
        Top = 159
        Width = 18
        Height = 11
        Caption = '100'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label20: TLabel
        Left = 357
        Top = 159
        Width = 30
        Height = 11
        Caption = '10000'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label21: TLabel
        Left = 269
        Top = 198
        Width = 14
        Height = 11
        Caption = '0.6'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label22: TLabel
        Left = 177
        Top = 198
        Width = 6
        Height = 11
        Caption = '0'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label23: TLabel
        Left = 216
        Top = 198
        Width = 26
        Height = 11
        Caption = '0.006'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label24: TLabel
        Left = 318
        Top = 198
        Width = 12
        Height = 11
        Caption = '60'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label25: TLabel
        Left = 360
        Top = 198
        Width = 24
        Height = 11
        Caption = '6000'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label26: TLabel
        Left = 269
        Top = 237
        Width = 14
        Height = 11
        Caption = '0.3'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label27: TLabel
        Left = 177
        Top = 237
        Width = 6
        Height = 11
        Caption = '0'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label28: TLabel
        Left = 216
        Top = 237
        Width = 26
        Height = 11
        Caption = '0.003'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label29: TLabel
        Left = 318
        Top = 237
        Width = 12
        Height = 11
        Caption = '30'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object Label30: TLabel
        Left = 360
        Top = 237
        Width = 24
        Height = 11
        Caption = '3000'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
      end
      object TrackBarPHMaterialDynamicFriction: TTrackBar
        Left = 168
        Top = 209
        Width = 215
        Height = 28
        LineSize = 0
        Max = 1000
        Orientation = trHorizontal
        Frequency = 250
        Position = 1
        SelEnd = 0
        SelStart = 0
        TabOrder = 11
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = TrackBarPHMaterialChange
      end
      object LabeledEditPHMaterialReflexSpring: TLabeledEdit
        Left = 24
        Top = 16
        Width = 135
        Height = 20
        EditLabel.Width = 65
        EditLabel.Height = 12
        EditLabel.Caption = 'ReflexSpring'
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 0
        OnChange = LabeledEditPHMaterialChange
      end
      object TrackBarPHMaterialReflexSpring: TTrackBar
        Left = 168
        Top = 14
        Width = 215
        Height = 28
        LineSize = 0
        Max = 1000
        Orientation = trHorizontal
        Frequency = 250
        Position = 1
        SelEnd = 0
        SelStart = 0
        TabOrder = 1
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = TrackBarPHMaterialChange
      end
      object TrackBarPHMaterialReflexDamper: TTrackBar
        Left = 168
        Top = 53
        Width = 215
        Height = 28
        LineSize = 0
        Max = 1000
        Orientation = trHorizontal
        Frequency = 250
        Position = 1
        SelEnd = 0
        SelStart = 0
        TabOrder = 3
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = TrackBarPHMaterialChange
      end
      object LabeledEditPHMaterialReflexDamper: TLabeledEdit
        Left = 24
        Top = 55
        Width = 135
        Height = 20
        EditLabel.Width = 72
        EditLabel.Height = 12
        EditLabel.Caption = 'ReflexDamper'
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 2
        OnChange = LabeledEditPHMaterialChange
      end
      object LabeledEditPHMaterialFrictionSpring: TLabeledEdit
        Left = 24
        Top = 94
        Width = 135
        Height = 20
        EditLabel.Width = 71
        EditLabel.Height = 12
        EditLabel.Caption = 'FrictionSpring'
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 4
        OnChange = LabeledEditPHMaterialChange
      end
      object TrackBarPHMaterialFrictionSpring: TTrackBar
        Left = 168
        Top = 92
        Width = 215
        Height = 28
        LineSize = 0
        Max = 1000
        Orientation = trHorizontal
        Frequency = 250
        Position = 1
        SelEnd = 0
        SelStart = 0
        TabOrder = 5
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = TrackBarPHMaterialChange
      end
      object TrackBarPHMaterialFrictionDamper: TTrackBar
        Left = 168
        Top = 131
        Width = 215
        Height = 28
        LineSize = 0
        Max = 1000
        Orientation = trHorizontal
        Frequency = 250
        Position = 1
        SelEnd = 0
        SelStart = 0
        TabOrder = 7
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = TrackBarPHMaterialChange
      end
      object LabeledEditPHMaterialFrictionDamper: TLabeledEdit
        Left = 24
        Top = 133
        Width = 135
        Height = 20
        EditLabel.Width = 78
        EditLabel.Height = 12
        EditLabel.Caption = 'FrictionDamper'
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 6
        OnChange = LabeledEditPHMaterialChange
      end
      object LabeledEditPHMaterialStaticFriction: TLabeledEdit
        Left = 24
        Top = 172
        Width = 135
        Height = 20
        EditLabel.Width = 69
        EditLabel.Height = 12
        EditLabel.Caption = 'StaticFriction'
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 8
        OnChange = LabeledEditPHMaterialChange
      end
      object TrackBarPHMaterialStaticFriction: TTrackBar
        Left = 168
        Top = 170
        Width = 215
        Height = 28
        LineSize = 0
        Max = 1000
        Orientation = trHorizontal
        Frequency = 250
        Position = 1
        SelEnd = 0
        SelStart = 0
        TabOrder = 9
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = TrackBarPHMaterialChange
      end
      object LabeledEditPHMaterialDynamicFriction: TLabeledEdit
        Left = 24
        Top = 211
        Width = 135
        Height = 20
        EditLabel.Width = 83
        EditLabel.Height = 12
        EditLabel.Caption = 'DynamicFriction'
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 10
        OnChange = LabeledEditPHMaterialChange
      end
    end
    object LightPanel: TPanel
      Left = 0
      Top = 0
      Width = 401
      Height = 249
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      object LabelLightAmbient: TLabel
        Left = 44
        Top = 93
        Width = 40
        Height = 12
        Caption = 'ambient'
      end
      object LabelLightSpecular: TLabel
        Left = 41
        Top = 64
        Width = 43
        Height = 12
        Caption = 'specular'
      end
      object LabelLightDiffuse: TLabel
        Left = 49
        Top = 38
        Width = 35
        Height = 12
        Caption = 'diffuse'
      end
      object LabelLightType: TLabel
        Left = 27
        Top = 12
        Width = 54
        Height = 12
        Caption = 'Light Type'
      end
      object LabelLightAttenuation: TLabel
        Left = 37
        Top = 177
        Width = 89
        Height = 12
        Caption = 'Attenuation(0,1,2)'
        Color = clBtnFace
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentColor = False
        ParentFont = False
      end
      object LabeledEditLightRange: TLabeledEdit
        Left = 88
        Top = 115
        Width = 57
        Height = 20
        EditLabel.Width = 32
        EditLabel.Height = 12
        EditLabel.Caption = 'Range'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 9
        OnChange = LabeledEditLightRangeChange
        OnMouseDown = LabeledEditRotMouseDown
      end
      object LabeledEditLightFalloff: TLabeledEdit
        Left = 88
        Top = 143
        Width = 57
        Height = 20
        EditLabel.Width = 33
        EditLabel.Height = 12
        EditLabel.Caption = 'Falloff'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 10
        OnChange = LabeledEditLightFalloffChange
        OnMouseDown = LabeledEditRotMouseDown
      end
      object LabeledEditLightTheta: TLabeledEdit
        Left = 88
        Top = 200
        Width = 57
        Height = 20
        EditLabel.Width = 29
        EditLabel.Height = 12
        EditLabel.Caption = 'Theta'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 12
        OnChange = LabeledEditLightThetaChange
        OnMouseDown = LabeledEditRotMouseDown
      end
      object StringGridLightAmbient: TStringGrid
        Left = 88
        Top = 87
        Width = 167
        Height = 24
        ColCount = 4
        DefaultColWidth = 40
        DefaultRowHeight = 20
        FixedCols = 0
        RowCount = 1
        FixedRows = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing]
        TabOrder = 7
        OnDrawCell = StringGridDrawCell
        OnSetEditText = StringGridLightAmbientSetEditText
      end
      object StringGridLightSpecular: TStringGrid
        Left = 88
        Top = 59
        Width = 167
        Height = 24
        ColCount = 4
        DefaultColWidth = 40
        DefaultRowHeight = 20
        FixedCols = 0
        RowCount = 1
        FixedRows = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing]
        TabOrder = 5
        OnDrawCell = StringGridDrawCell
        OnSetEditText = StringGridLightSpecularSetEditText
      end
      object StringGridLightDiffuse: TStringGrid
        Left = 88
        Top = 32
        Width = 167
        Height = 24
        ColCount = 4
        DefaultColWidth = 40
        DefaultRowHeight = 20
        FixedCols = 0
        RowCount = 1
        FixedRows = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing]
        TabOrder = 3
        OnDrawCell = StringGridDrawCell
        OnSetEditText = StringGridLightDiffuseSetEditText
      end
      object RadioButtonLightPoint: TRadioButton
        Left = 93
        Top = 10
        Width = 49
        Height = 17
        Caption = 'Point'
        TabOrder = 0
        OnClick = RadioButtonLightPointClick
      end
      object RadioButtonLightSpot: TRadioButton
        Left = 144
        Top = 10
        Width = 41
        Height = 17
        Caption = 'Spot'
        TabOrder = 1
        OnClick = RadioButtonLightSpotClick
      end
      object RadioButtonLightDirectional: TRadioButton
        Left = 195
        Top = 10
        Width = 73
        Height = 17
        Caption = 'Directional'
        TabOrder = 2
        OnClick = RadioButtonLightDirectionalClick
      end
      object LabeledEditLightPhi: TLabeledEdit
        Left = 198
        Top = 200
        Width = 57
        Height = 20
        EditLabel.Width = 16
        EditLabel.Height = 12
        EditLabel.Caption = 'Phi'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 13
        OnChange = LabeledEditLightPhiChange
        OnMouseDown = LabeledEditRotMouseDown
      end
      object StringGridLightAttenuation: TStringGrid
        Left = 135
        Top = 171
        Width = 126
        Height = 24
        ColCount = 3
        DefaultColWidth = 40
        DefaultRowHeight = 20
        FixedCols = 0
        RowCount = 1
        FixedRows = 0
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing]
        ParentFont = False
        ScrollBars = ssNone
        TabOrder = 11
        OnDrawCell = StringGridDrawCell
        OnSetEditText = StringGridLightAttenuationSetEditText
      end
      object PanelLightSpecular: TPanel
        Left = 264
        Top = 59
        Width = 41
        Height = 24
        TabOrder = 6
        OnClick = PanelLightSpecularClick
      end
      object PanelLightDiffuse: TPanel
        Left = 264
        Top = 32
        Width = 41
        Height = 24
        TabOrder = 4
        OnClick = PanelLightDiffuseClick
      end
      object PanelLightAmbient: TPanel
        Left = 264
        Top = 87
        Width = 41
        Height = 24
        TabOrder = 8
        OnClick = PanelLightAmbientClick
      end
    end
    object FramePanel: TPanel
      Left = 0
      Top = 0
      Width = 401
      Height = 249
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object BevelFrameViewCtrl: TBevel
        Left = 8
        Top = 144
        Width = 121
        Height = 41
      end
      object LabelFrameState: TLabel
        Left = 8
        Top = 204
        Width = 185
        Height = 24
        AutoSize = False
        Caption = 'Ready'
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelFrameRot: TLabel
        Left = 8
        Top = 85
        Width = 34
        Height = 12
        Caption = 'Rotate'
      end
      object LabelFrameTrans: TLabel
        Left = 8
        Top = 5
        Width = 52
        Height = 12
        Caption = 'Transform'
      end
      object ImageFrameViewTrans: TImage
        Left = 16
        Top = 152
        Width = 25
        Height = 25
        Hint = 'ViewTransform'
        ParentShowHint = False
        Picture.Data = {
          07544269746D6170F6060000424DF60600000000000036000000280000001700
          0000180000000100180000000000C0060000C40E0000C40E0000000000000000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          800000800000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0FF0040FF0000FF0000800000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0FF0040FF0000FF0000FF0000FF0000800000C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FF8000FF8000FF8000FF0000FF0000FF00
          00FF8000800000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FFFF00FFFF00FFFF00FFFF00
          FF8000FF0000FF0000FFFF00FFFF00FFFF00C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0FFFF00FF8000FF0000800000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0FFFF00FF8000FF0000800000C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0800000C0C0C0C0C0C0C0C0C0C0C0C0FFFF00FF8000FF00008000
          00C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FFFF00C0C0C0C0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0FF0040800000C0C0C0C0C0C0C0C0C0C0C0C0FFFF00
          FF8000FF0000800000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FFFF00800000C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0FF0040FF0000800000C0C0C0C0C0C0C0
          C0C0C0C0C0FFFF00FF8000FF0000800000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          FFFF00FF8000800000C0C0C0C0C0C0000000C0C0C0FF0040FF0000FF00008000
          00800000800000800000800000800000FF0000FF000080000080000080000080
          0000800000800000FFFF00FF8000FF0000800000C0C0C0000000FF8000FF0000
          FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF00
          00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF000080000000
          0000FFFF00FF8000FF0000FF0000FF0000FF8000FF8000FF8000FF8000FF8000
          FF8000FF0000FF0000FF8000FF8000FF8000FF8000FF8000FF8000FF0000FF00
          00FF0000FF0040000000C0C0C0FFFF00FF8000FF0000800000FFFF00FFFF00FF
          FF00FFFF00FFFF00FF8000FF0000800000FFFF00FFFF00FFFF00FFFF00FFFF00
          FFFF00FF8000FF0000FF0040C0C0C0000000C0C0C0C0C0C0FFFF00FF80008000
          00C0C0C0C0C0C0C0C0C0C0C0C0FFFF00FF8000FF0000800000C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0FFFF00FF8000FF0040C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0FFFF00800000C0C0C0C0C0C0C0C0C0C0C0C0FFFF00FF8000FF00008000
          00C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FFFF00FF8000C0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0FFFF00C0C0C0C0C0C0C0C0C0C0C0C0FFFF00
          FF8000FF0000800000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FFFF00C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0FFFF00FF8000FF0000800000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0FFFF00FF8000FF0000800000C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0FFFF00800000800000800000FF0000FF00008000
          00800000800000800000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FFFF00FF8000FF0000
          FF0000FF0000FF0000FF0000FF0040C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0FFFF00FF8000FF0000FF0000FF0000FF0040C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0FFFF00FF8000FF0000FF0040C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FFFF00FF8000C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C000
          0000}
        ShowHint = True
        OnMouseDown = ImageViewMouseDown
        OnMouseMove = ImageViewMouseMove
        OnMouseUp = ImageViewMouseUp
      end
      object ImageFrameViewZoom: TImage
        Left = 96
        Top = 152
        Width = 25
        Height = 25
        Hint = 'ViewZoom'
        ParentShowHint = False
        Picture.Data = {
          07544269746D6170F6060000424DF60600000000000036000000280000001700
          0000180000000100180000000000C0060000C40E0000C40E0000000000000000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0408080004080
          004080000080000080C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0408080408080004080004080000080C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0408080004080004080004080000080C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C04080804080800040800040
          80000080C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          408080004080004080000080C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0408080004080004080000080C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FF0000FF0000FF0000FF0000FF0000C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FF0000FF0000FFFF00FFFF00FFFF
          00FFFF00FFFF00FF0000FF0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FF0000FFFF00FFFF00
          FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF0000C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FF0000FF
          FF00FFFF00FFFF00FFFF00FFFF00000000FFFF00FFFF00FFFF00FFFF00FFFF00
          FF0000C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0FF0000FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00000000FFFF00FFFF00FF
          FF00FFFF00FFFF00FFFF00FF0000C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0FF0000FFFF00FFFF00FFFF00FFFF00FFFF00FFFF000000
          00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF0000C0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0FF0000FFFF00FFFF00FFFF00FFFF00FFFF00
          FFFF00FFFF00000000FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF00
          00C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0FF0000FFFF00FFFF00FF
          FF00FFFF00FFFF00FFFF00FFFF00000000FFFF00FFFF00FFFF00FFFF00FFFF00
          FFFF00FFFF00FF0000C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0FF00
          00FFFF00FFFF0000000000000000000000000000000000000000000000000000
          0000000000000000FFFF00FFFF00FF0000C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0FF0000FFFF00FFFFFFFFFFFFFFFF00FFFF00FFFF00FFFF000000
          00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF0000C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0FF0000FFFF00FFFF00FFFFFFFFFFFFFFFF00
          FFFF00FFFF00000000FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF00
          00C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FF0000FFFF00FF
          FFFFFFFFFFFFFF00FFFF00FFFF00000000FFFF00FFFF00FFFF00FFFF00FFFF00
          FFFF00FF0000C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0FF0000FFFF00FFFF00FFFFFFFFFFFFFFFF00FFFF00000000FFFF00FFFF00FF
          FF00FFFF00FFFF00FFFF00FF0000C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0FF0000FFFF00FFFF00FFFFFFFFFF00FFFF000000
          00FFFF00FFFF00FFFF00FFFF00FFFF00FF0000C0C0C0C0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FF0000FFFF00FFFF00
          FFFF00FFFFFFFFFF00FFFF00FFFF00FFFF00FFFF00FF0000C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0FF0000FF0000FFFF00FFFF00FFFF00FFFF00FFFF00FF0000FF0000C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FF0000FF0000FF0000FF0000FF0000C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C000
          0000}
        ShowHint = True
        OnMouseDown = ImageViewMouseDown
        OnMouseMove = ImageViewMouseMove
        OnMouseUp = ImageViewMouseUp
      end
      object LabelFrameViewCtrl: TLabel
        Left = 32
        Top = 136
        Width = 70
        Height = 12
        Caption = ' View Control'
      end
      object ImageFrameViewRotate: TImage
        Left = 56
        Top = 152
        Width = 25
        Height = 25
        Hint = 'ViewRotate'
        ParentShowHint = False
        Picture.Data = {
          07544269746D6170F6060000424DF60600000000000036000000280000001700
          0000180000000100180000000000C0060000C40E0000C40E0000000000000000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0800000800000800000
          800000800000800000800000800000800000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          000000000000000000C0C0C0C0C0C0000000C0C0C0C0C0C0000000FFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFF000000C0C0C0C0C0C0000000FFFFFF000000
          C0C0C0C0C0C0C0C0C0FFFF80FF8000FF0000FF0000FF0000FF0000FF0000FF00
          00FF0000FF0000FF0000FF0040800000C0C0C0C0C0C0FFFFFF000000C0C0C000
          0000FFFFFF000000C0C0C0C0C0C0C0C0C0FFFF80FF8000FF0000FF0000FF0000
          FF0000FF0000FF0000FF0000FF0000FF0000FF0040800000C0C0C0C0C0C0FFFF
          FF000000C0C0C0000000C0C0C0FFFFFF000000C0C0C0C0C0C0FFFF80FF8000FF
          8000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0040800000
          C0C0C0FFFFFF000000C0C0C0C0C0C0000000C0C0C0C0C0C0FFFFFF0000000000
          00000000FFFF80FF8000FF8000FF8000FF8000FF8000FF8000FF0000FF0000FF
          0040800000000000000000000000C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0FFFFFFFFFFFFC0C0C0C0C0C0FFFF80FFFF80FFFF80FFFF80FFFF80FFFF
          80FF0000FF0000FF0000C0C0C0FFFFFFFFFFFFFFFFFFC0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0FFFF80
          FFFF80FFFF80FFFF80FFFF80FFFF80C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C000
          0000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C000
          0000}
        ShowHint = True
        OnMouseDown = ImageViewMouseDown
        OnMouseMove = ImageViewMouseMove
        OnMouseUp = ImageViewMouseUp
      end
      object LabelFrameAffine: TLabel
        Left = 168
        Top = 37
        Width = 31
        Height = 12
        Caption = 'Affine'
      end
      object LabelFrameScale: TLabel
        Left = 168
        Top = 88
        Width = 28
        Height = 12
        Caption = 'Scale'
      end
      object BevelFrameInit: TBevel
        Left = 176
        Top = 192
        Width = 217
        Height = 41
      end
      object LabelFrameInit: TLabel
        Left = 172
        Top = 206
        Width = 42
        Height = 12
        Caption = 'Initialize'
      end
      object ButtonFrameRotX: TButton
        Left = 8
        Top = 100
        Width = 41
        Height = 25
        Caption = 'RotX'
        DragCursor = crSizeAll
        TabOrder = 6
        OnMouseDown = ButtonMouseDown
        OnMouseMove = ButtonMouseMove
        OnMouseUp = ButtonMouseUp
      end
      object ButtonFrameXY: TButton
        Left = 8
        Top = 44
        Width = 41
        Height = 25
        Caption = 'XY'
        TabOrder = 3
        OnMouseDown = ButtonMouseDown
        OnMouseMove = ButtonMouseMove
        OnMouseUp = ButtonMouseUp
      end
      object ButtonFrameY: TButton
        Left = 49
        Top = 19
        Width = 41
        Height = 25
        Caption = 'Y'
        TabOrder = 1
        OnMouseDown = ButtonMouseDown
        OnMouseMove = ButtonMouseMove
        OnMouseUp = ButtonMouseUp
      end
      object ButtonFrameZ: TButton
        Left = 90
        Top = 19
        Width = 41
        Height = 25
        Caption = 'Z'
        TabOrder = 2
        OnMouseDown = ButtonMouseDown
        OnMouseMove = ButtonMouseMove
        OnMouseUp = ButtonMouseUp
      end
      object ButtonFrameYZ: TButton
        Left = 49
        Top = 44
        Width = 41
        Height = 25
        Caption = 'YZ'
        TabOrder = 4
        OnMouseDown = ButtonMouseDown
        OnMouseMove = ButtonMouseMove
        OnMouseUp = ButtonMouseUp
      end
      object ButtonFrameZX: TButton
        Left = 90
        Top = 44
        Width = 41
        Height = 25
        Caption = 'ZX'
        TabOrder = 5
        OnMouseDown = ButtonMouseDown
        OnMouseMove = ButtonMouseMove
        OnMouseUp = ButtonMouseUp
      end
      object ButtonFrameRotY: TButton
        Left = 49
        Top = 100
        Width = 41
        Height = 25
        Caption = 'RotY'
        TabOrder = 7
        OnMouseDown = ButtonMouseDown
        OnMouseMove = ButtonMouseMove
        OnMouseUp = ButtonMouseUp
      end
      object ButtonFrameRotZ: TButton
        Left = 90
        Top = 100
        Width = 41
        Height = 25
        Caption = 'RotZ'
        TabOrder = 8
        OnMouseDown = ButtonMouseDown
        OnMouseMove = ButtonMouseMove
        OnMouseUp = ButtonMouseUp
      end
      object StringGridFrameAffine: TStringGrid
        Left = 208
        Top = 11
        Width = 167
        Height = 66
        ColCount = 4
        DefaultColWidth = 40
        DefaultRowHeight = 20
        FixedCols = 0
        RowCount = 3
        FixedRows = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing]
        TabOrder = 9
        OnDrawCell = StringGridDrawCell
        OnSetEditText = StringGridFrameAffineSetEditText
      end
      object ButtonFrameX: TButton
        Left = 8
        Top = 19
        Width = 41
        Height = 25
        Caption = 'X'
        TabOrder = 0
        OnMouseDown = ButtonMouseDown
        OnMouseMove = ButtonMouseMove
        OnMouseUp = ButtonMouseUp
      end
      object LabeledEditFrameRotX: TLabeledEdit
        Left = 224
        Top = 112
        Width = 105
        Height = 20
        EditLabel.Width = 54
        EditLabel.Height = 12
        EditLabel.Caption = 'Rotation X'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 11
        OnChange = LabeledEditRotChange
        OnEnter = LabeledEditRotEnter
        OnExit = LabeledEditRotExit
        OnMouseDown = LabeledEditRotMouseDown
      end
      object LabeledEditFrameRotY: TLabeledEdit
        Left = 224
        Top = 136
        Width = 105
        Height = 20
        EditLabel.Width = 54
        EditLabel.Height = 12
        EditLabel.Caption = 'Rotation Y'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 12
        OnChange = LabeledEditRotChange
        OnEnter = LabeledEditRotEnter
        OnExit = LabeledEditRotExit
        OnMouseDown = LabeledEditRotMouseDown
      end
      object LabeledEditFrameRotZ: TLabeledEdit
        Left = 224
        Top = 160
        Width = 105
        Height = 20
        EditLabel.Width = 54
        EditLabel.Height = 12
        EditLabel.Caption = 'Rotation Z'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 13
        OnChange = LabeledEditRotChange
        OnEnter = LabeledEditRotEnter
        OnExit = LabeledEditRotExit
        OnMouseDown = LabeledEditRotMouseDown
      end
      object StringGridFrameScale: TStringGrid
        Left = 208
        Top = 80
        Width = 167
        Height = 24
        ColCount = 3
        DefaultColWidth = 40
        DefaultRowHeight = 20
        FixedCols = 0
        RowCount = 1
        FixedRows = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing]
        TabOrder = 10
        OnDrawCell = StringGridDrawCell
        OnSetEditText = StringGridFrameScaleSetEditText
      end
      object ButtonFrameInitRot: TButton
        Left = 336
        Top = 112
        Width = 57
        Height = 65
        Caption = 'Init Rot'
        TabOrder = 14
        OnClick = ButtonFrameInitRotClick
      end
      object ButtonFrameMeshCtrlPos: TButton
        Left = 224
        Top = 200
        Width = 50
        Height = 25
        Caption = 'Position'
        TabOrder = 15
        OnClick = ButtonMeshCtrlClick
      end
      object ButtonFrameMeshCtrlRot: TButton
        Left = 280
        Top = 200
        Width = 50
        Height = 25
        Caption = 'Rotation'
        TabOrder = 16
        OnClick = ButtonMeshCtrlClick
      end
      object ButtonFrameMeshCtrlScale: TButton
        Left = 336
        Top = 200
        Width = 50
        Height = 25
        Caption = 'Scale'
        TabOrder = 17
        OnClick = ButtonMeshCtrlClick
      end
      object CheckBoxFrameMeshCheck: TCheckBox
        Left = 233
        Top = 227
        Width = 97
        Height = 17
        Caption = 'Mesh'#12395#21453#26144
        TabOrder = 18
      end
    end
    object SolidPanel: TPanel
      Left = 0
      Top = 0
      Width = 401
      Height = 249
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 2
      object LabelSolidVelocity: TLabel
        Left = 186
        Top = 94
        Width = 42
        Height = 12
        Caption = 'Velocity'
      end
      object LabelSolidAngVel1: TLabel
        Left = 186
        Top = 160
        Width = 39
        Height = 12
        Alignment = taCenter
        Caption = 'Angular'
        Transparent = True
        WordWrap = True
      end
      object LabelSolidAngVel2: TLabel
        Left = 181
        Top = 172
        Width = 49
        Height = 17
        Alignment = taCenter
        AutoSize = False
        Caption = 'Velocity'
        Transparent = True
        WordWrap = True
      end
      object LabelSolidInertia: TLabel
        Left = 6
        Top = 103
        Width = 32
        Height = 12
        Caption = 'Inertia'
      end
      object LabelSolidCenter: TLabel
        Left = 6
        Top = 174
        Width = 34
        Height = 12
        Caption = 'Center'
      end
      object ButtonSolidSet: TButton
        Left = 230
        Top = 208
        Width = 167
        Height = 30
        Caption = 'Set Velocity && AngularVelocity'
        TabOrder = 6
        OnClick = ButtonSolidSetClick
      end
      object StringGridSolidVelocity: TStringGrid
        Left = 230
        Top = 72
        Width = 167
        Height = 53
        ColCount = 4
        DefaultColWidth = 40
        RowCount = 2
        FixedRows = 0
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
        ParentFont = False
        ScrollBars = ssNone
        TabOrder = 4
        OnDrawCell = StringGridDrawCell
        OnSelectCell = StringGridSolidVelocitySelectCell
      end
      object StringGridSolidAngularVelocity: TStringGrid
        Left = 230
        Top = 144
        Width = 167
        Height = 53
        ColCount = 4
        DefaultColWidth = 40
        RowCount = 2
        FixedRows = 0
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
        ParentFont = False
        ScrollBars = ssNone
        TabOrder = 5
        OnDrawCell = StringGridDrawCell
        OnSelectCell = StringGridSolidVelocitySelectCell
      end
      object StringGridSolidCenter: TStringGrid
        Left = 46
        Top = 168
        Width = 126
        Height = 25
        ColCount = 3
        DefaultColWidth = 40
        FixedCols = 0
        RowCount = 1
        FixedRows = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
        ScrollBars = ssNone
        TabOrder = 2
        OnDrawCell = StringGridDrawCell
        OnSetEditText = StringGridSolidCenterSetEditText
      end
      object StringGridSolidInertia: TStringGrid
        Left = 46
        Top = 72
        Width = 126
        Height = 78
        ColCount = 3
        DefaultColWidth = 40
        FixedCols = 0
        RowCount = 3
        FixedRows = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
        ScrollBars = ssNone
        TabOrder = 1
        OnDrawCell = StringGridDrawCell
        OnSetEditText = StringGridSolidInertiaSetEditText
      end
      object ButtonSolidToFrame: TButton
        Left = 56
        Top = 208
        Width = 113
        Height = 30
        Caption = 'Jump to Frame'
        TabOrder = 3
        OnClick = ButtonSolidToFrameClick
      end
      object GroupBoxSolidMass: TGroupBox
        Left = 12
        Top = 2
        Width = 385
        Height = 62
        Caption = '  Mass  '
        Color = clBtnFace
        ParentColor = False
        TabOrder = 0
        object TrackBarSolidMass: TTrackBar
          Left = 168
          Top = 34
          Width = 215
          Height = 25
          Max = 1000
          Orientation = trHorizontal
          Frequency = 1
          Position = 0
          SelEnd = 0
          SelStart = 0
          TabOrder = 3
          TickMarks = tmBottomRight
          TickStyle = tsNone
          OnChange = TrackBarSolidMassChange
        end
        object LabeledEditSolidMassMax: TLabeledEdit
          Left = 302
          Top = 10
          Width = 73
          Height = 20
          EditLabel.Width = 21
          EditLabel.Height = 12
          EditLabel.Caption = 'Max'
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 2
          OnChange = LabeledEditSolidMassMaxChange
        end
        object LabeledEditSolidMassMin: TLabeledEdit
          Left = 197
          Top = 10
          Width = 73
          Height = 20
          EditLabel.Width = 18
          EditLabel.Height = 12
          EditLabel.Caption = 'Min'
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 1
          OnChange = LabeledEditSolidMassMinChange
        end
        object LabeledEditSolidMass: TLabeledEdit
          Left = 14
          Top = 34
          Width = 129
          Height = 20
          EditLabel.Width = 29
          EditLabel.Height = 12
          EditLabel.Caption = 'Value'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 0
          OnChange = LabeledEditSolidMassChange
        end
      end
    end
  end
  object TreeMenu: TPopupMenu
    Left = 8
    Top = 8
    object Insert: TMenuItem
      Caption = 'Insert(&I)'
      OnClick = InsertClick
    end
    object Delete: TMenuItem
      Caption = 'Delete(&D)'
      OnClick = DeleteClick
    end
    object Import: TMenuItem
      Caption = 'Import(&M)'
      OnClick = ImportClick
    end
    object Export: TMenuItem
      Caption = 'Export(&E)'
      OnClick = ExportClick
    end
    object AddLight: TMenuItem
      Caption = 'Add Light(&L)'
      OnClick = AddLightClick
    end
    object Component: TMenuItem
      Caption = 'Component(&C)'
      OnClick = ComponentClick
    end
  end
  object OpenDlg: TOpenDialog
    Options = [ofAllowMultiSelect, ofEnableSizing]
    Title = 'Select Importing Files'
    Left = 40
    Top = 8
  end
  object SaveDlg: TSaveDialog
    Left = 72
    Top = 8
  end
  object ImageListForTree: TImageList
    Left = 104
    Top = 8
    Bitmap = {
      494C010101000400040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008400000084000000840000008400000084000000840000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF000000000000000000FFFF
      8400FF840000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      42008400000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF000000000000000000FFFF
      8400FF840000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      42008400000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFF8400FF840000FF840000FF840000FF840000FF000000FF0042008400
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      000000000000FFFF8400FFFF8400FFFF8400FFFF8400FF000000FF0000000000
      0000FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFF000000000000FFFF000000000000
      FFFF000000000000FFFF000000000000FFFF000000000000F81F000000000000
      8001000000000000600500000000000060050000000000008003000000000000
      D813000000000000FFFF000000000000FFFF000000000000FFFF000000000000
      FFFF000000000000FFFF00000000000000000000000000000000000000000000
      000000000000}
  end
  object ColorDialogLight: TColorDialog
    Ctl3D = True
    Left = 136
    Top = 8
  end
  object TimerSolidObserver: TTimer
    Interval = 500
    OnTimer = TimerSolidObserverTimer
    Left = 168
    Top = 8
  end
end
