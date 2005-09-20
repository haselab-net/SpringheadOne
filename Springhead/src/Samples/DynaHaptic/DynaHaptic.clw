; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDynaHapticView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "dynahaptic.h"
LastPage=0

ClassCount=6
Class1=CDynaHapticApp
Class2=CAboutDlg
Class3=CDynaHapticDoc
Class4=CDynaHapticView
Class5=CHisConfig
Class6=CMainFrame

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_HIS_CONFIG

[CLS:CDynaHapticApp]
Type=0
BaseClass=CWinApp
HeaderFile=DynaHaptic.h
ImplementationFile=DynaHaptic.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=DynaHaptic.cpp
ImplementationFile=DynaHaptic.cpp
LastObject=CAboutDlg

[CLS:CDynaHapticDoc]
Type=0
BaseClass=CDocument
HeaderFile=DynaHapticDoc.h
ImplementationFile=DynaHapticDoc.cpp

[CLS:CDynaHapticView]
Type=0
BaseClass=CView
HeaderFile=DynaHapticView.h
ImplementationFile=DynaHapticView.cpp
Filter=C
LastObject=CDynaHapticView

[CLS:CHisConfig]
Type=0
BaseClass=CDialog
HeaderFile=HisConfig.h
ImplementationFile=HisConfig.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_HIS_CONFIG]
Type=1
Class=CHisConfig
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_HIS_LIST,edit,1350635716
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_PosScale,edit,1350631552
Control8=IDC_ForceScale,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_TimeStep,edit,1350631552
Control11=IDC_STATIC,static,1342308352

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=RENDER_D3D
Command14=WGL_RENDER
Command15=ID_HIS_CONFIG
Command16=ID_APP_ABOUT
Command17=ID_DEMO_1
CommandCount=17

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

