; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=VHCheckDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "virtualhuman.h"
LastPage=0

ClassCount=6
Class1=CMainFrame
Class2=VHCheckDialog
Class3=CVirtualHumanApp
Class4=CAboutDlg
Class5=CVirtualHumanDoc
Class6=CVirtualHumanView

ResourceCount=3
Resource1=IDD_VIEW_ZMP
Resource2=IDD_ABOUTBOX
Resource3=IDR_MAINFRAME

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=ID_ZMP_DRAW

[CLS:VHCheckDialog]
Type=0
BaseClass=CDialog
HeaderFile=VHCheckDialog.h
ImplementationFile=VHCheckDialog.cpp
LastObject=IDC_BALANCE

[CLS:CVirtualHumanApp]
Type=0
BaseClass=CWinApp
HeaderFile=VirtualHuman.h
ImplementationFile=VirtualHuman.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=VirtualHuman.cpp
ImplementationFile=VirtualHuman.cpp
LastObject=ID_ZMP_DRAW

[CLS:CVirtualHumanDoc]
Type=0
BaseClass=CDocument
HeaderFile=VirtualHumanDoc.h
ImplementationFile=VirtualHumanDoc.cpp

[CLS:CVirtualHumanView]
Type=0
BaseClass=CView
HeaderFile=VirtualHumanView.h
ImplementationFile=VirtualHumanView.cpp
LastObject=CVirtualHumanView
Filter=C
VirtualFilter=VWC

[DLG:IDD_VIEW_ZMP]
Type=1
Class=VHCheckDialog
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308353
Control4=IDC_VIEWER,static,1342181383

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
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
Command13=ID_ZMP_DRAW
Command14=ID_APP_ABOUT
CommandCount=14

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

