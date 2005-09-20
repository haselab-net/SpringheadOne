; CLW ファイルは MFC ClassWizard の情報を含んでいます。

[General Info]
Version=1
LastClass=CTrgSet
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "vcsample.h"

ClassCount=5
Class1=CVcsampleApp
Class2=CVcsampleDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_VCSAMPLE_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_DATAIN
Resource4=IDD_ABOUTBOX
Class4=CDataIn
Class5=CTrgSet
Resource5=IDD_TRGSET

[CLS:CVcsampleApp]
Type=0
HeaderFile=vcsample.h
ImplementationFile=vcsample.cpp
Filter=N

[CLS:CVcsampleDlg]
Type=0
HeaderFile=vcsampleDlg.h
ImplementationFile=vcsampleDlg.cpp
Filter=D
LastObject=CVcsampleDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=vcsampleDlg.h
ImplementationFile=vcsampleDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VCSAMPLE_DIALOG]
Type=1
Class=CVcsampleDlg
ControlCount=24
Control1=IDC_LOWDIR,button,1342242816
Control2=IDC_HIGHDIR,button,1342242816
Control3=IDC_LOWWRITE,button,1342242816
Control4=IDC_HIGHWRITE,button,1342242816
Control5=IDC_LOWREAD,button,1342242816
Control6=IDC_HIGHREAD,button,1342242816
Control7=IDC_WORDWRITE,button,1342242816
Control8=IDC_WORDREAD,button,1342242816
Control9=IDC_CLOCKOUT,button,1342242816
Control10=IDC_CLOCKSTOP,button,1342242816
Control11=IDC_STATIC,button,1342177287
Control12=IDC_DACOUT1,button,1342242816
Control13=IDC_DACOUT2,button,1342242816
Control14=IDC_SINGLESAMPLE,button,1342242816
Control15=IDC_DIGITALTRG,button,1342242816
Control16=IDC_ANALOGTRG,button,1342242816
Control17=IDC_GETSTATUS,button,1342242816
Control18=IDC_GETDATAS,button,1342242816
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,button,1342177287
Control21=IDC_DEVOPEN,button,1342242816
Control22=IDC_STATIC,static,1342308352
Control23=IDC_IDNUM,combobox,1344340226
Control24=IDC_DEVCLOSE,button,1342242816

[DLG:IDD_DATAIN]
Type=1
Class=CDataIn
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_DATA,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_MESSAGE1,static,1342308352

[DLG:IDD_TRGSET]
Type=1
Class=CTrgSet
ControlCount=16
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_MESSAGE2,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_ENDCH,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_BUFSIZE,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_THLEVEL,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_TRGCH,edit,1350631552
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_RADIO1,button,1342308361
Control16=IDC_RADIO2,button,1342242825

[CLS:CDataIn]
Type=0
HeaderFile=DataIn.h
ImplementationFile=DataIn.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_MESSAGE1
VirtualFilter=dWC

[CLS:CTrgSet]
Type=0
HeaderFile=TrgSet.h
ImplementationFile=TrgSet.cpp
BaseClass=CDialog
Filter=D
LastObject=CTrgSet
VirtualFilter=dWC

