; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMFCD3DDynaView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mfcd3ddyna.h"
LastPage=0

ClassCount=5
Class1=CMainFrame
Class2=CMFCD3DDynaApp
Class3=CAboutDlg
Class4=CMFCD3DDynaDoc
Class5=CMFCD3DDynaView

ResourceCount=2
Resource1=IDR_MAINFRAME
Resource2=IDD_ABOUTBOX

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CMFCD3DDynaApp]
Type=0
BaseClass=CWinApp
HeaderFile=MFCD3DDyna.h
ImplementationFile=MFCD3DDyna.cpp
Filter=N
VirtualFilter=AC
LastObject=CMFCD3DDynaApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=MFCD3DDyna.cpp
ImplementationFile=MFCD3DDyna.cpp
LastObject=CAboutDlg

[CLS:CMFCD3DDynaDoc]
Type=0
BaseClass=CDocument
HeaderFile=MFCD3DDynaDoc.h
ImplementationFile=MFCD3DDynaDoc.cpp
Filter=N
VirtualFilter=DC
LastObject=ID_EDIT_CUT

[CLS:CMFCD3DDynaView]
Type=0
BaseClass=CView
HeaderFile=MFCD3DDynaView.h
ImplementationFile=MFCD3DDynaView.cpp
Filter=C
VirtualFilter=VWC
LastObject=ID_FILE_INSERT

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

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
Class=CMFCD3DDynaView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_INSERT
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_VIEW_TOOLBAR
Command13=ID_VIEW_STATUS_BAR
Command14=ID_RENDER_D3D
Command15=ID_RENDER_GL
Command16=ID_APP_ABOUT
CommandCount=16

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

