�
 TMAINFORM 0�
  TPF0	TMainFormMainFormLeft&Top� Width!Height�CaptionGLEnumColor	clBtnFaceFont.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style OldCreateOrder	PositionpoDefaultPosOnlyOnCreate
FormCreatePixelsPerInchx
TextHeight TStringGridGridLeft TopWidthHeight� AlignalClientColCount	FixedCols RowCount	FixedRows OptionsgoFixedVertLinegoFixedHorzLine
goVertLine
goHorzLinegoColSizinggoRowSelectgoThumbTracking TabOrder 
RowHeights   TPanelPanel1Left Top WidthHeightAlignalTop
BevelOuterbvNoneTabOrder 	TGroupBox	GroupBox1Left(Top Width� HeightAlignalRightCaptionDriver search pathsTabOrder  TMemo
DriverListLeft
TopWidth� Height� Lines.Strings
DriverList TabOrder    TPanel	TestPanelLeft Top Width1HeightAlignalLeft
BevelOuterbvNoneCaption	TestPanelColorclBlackTabOrder  TPanelPanel2Left1Top Width�HeightAlignalClient
BevelOuterbvNoneCaptionPanel2TabOrder TLabelLabel2Left Top Width�HeightIAlignalTopAutoSizeCaption�This program goes through the GL drivers on your system (OPENGL32.DLL,  3dfxopengl.dll, 3dfxquake.dll, ...). For each driver, it enumerates all pixel formats. You can save the list to a text-separate file ready for importing into Excel.WordWrap	  TLabelLabel1Left TopIWidth�HeightPAlignalTopAutoSizeCaption�Warning for 3dfx users: if the list to the right includes any 3dfx drivers, then when you get the details the screen will change mode frequently (up to 20 times) over several seconds. If you are scared about damage to your monitor, DO NOT RUN IT!ColorclSilverFont.CharsetDEFAULT_CHARSET
Font.ColorclRedFont.Height�	Font.NameMS Sans Serif
Font.Style ParentColor
ParentFontWordWrap	  TLabelLabel3Left Top� Width�HeightHAlignalTopAutoSizeCaption�(It runs without problem on my machine, although the screen goes blank after the 3dfx logo for a couple of seconds). By Lucian Wischik. www.wischik.com/luWordWrap	  TPanelPanel3Left Top� Width�Height)AlignalBottom
BevelOuterbvNoneTabOrder  TButtonbtnSaveLeft� TopWidth� HeightCaptionSave list to file...TabOrder OnClickbtnSaveClick  TButtonRefreshLeftTopWidth� HeightCaptionGet all the details!TabOrderOnClickRefreshClick     TSaveDialog
SaveDialog
DefaultExttxtFilterText files (*.txt)|*.txtOptionsofOverwritePromptofHideReadOnlyofPathMustExist Left�Top  TSglDriverListSglDriverListSearch.Strings<project>\opengl*.dll<project>\3dfx*gl*.dll<windows>\opengl*.dll<system>\opengl*.dll Left Top   