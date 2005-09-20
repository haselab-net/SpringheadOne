object Form1: TForm1
  Left = 347
  Top = 141
  Width = 628
  Height = 347
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 12
  object SglPanel2: TSglPanel
    Left = 312
    Top = 8
    Width = 300
    Height = 300
    glDriver = 'opengl32.dll'
    OnGLInit = SglPanel2GLInit
    OnGLPaint = SglPanel2GLPaint
  end
  object SglPanel1: TSglPanel
    Left = 8
    Top = 8
    Width = 300
    Height = 300
    glDriver = 'opengl32.dll'
    OnGLInit = SglPanel1GLInit
    OnGLPaint = SglPanel1GLPaint
  end
  object Timer1: TTimer
    Interval = 10
    OnTimer = Timer1Timer
    Left = 8
    Top = 8
  end
end
