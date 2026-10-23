; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CStatisticsDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "goonzumc.h"
LastPage=0

ClassCount=6
Class1=CFrontServerListDlg
Class2=CGameServerListDlg
Class3=CGoonZuMCApp
Class4=CGoonZuMCDlg
Class5=CLoginDlg
Class6=CStatisticsDlg

ResourceCount=5
Resource1=IDD_DIALOG_LOGIN
Resource2=IDD_DIALOG_STATISTICS
Resource3=IDD_DIALOG_GAMESERVERLIST
Resource4=IDD_GOONZUMC_DIALOG
Resource5=IDD_DIALOG_FRONTSERVERLIST

[CLS:CFrontServerListDlg]
Type=0
BaseClass=CDialog
HeaderFile=FrontServerListDlg.h
ImplementationFile=FrontServerListDlg.cpp
LastObject=CFrontServerListDlg
Filter=D
VirtualFilter=dWC

[CLS:CGameServerListDlg]
Type=0
BaseClass=CDialog
HeaderFile=GameServerListDlg.h
ImplementationFile=GameServerListDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CGameServerListDlg

[CLS:CGoonZuMCApp]
Type=0
BaseClass=CWinApp
HeaderFile=GoonZuMC.h
ImplementationFile=GoonZuMC.cpp
Filter=N
VirtualFilter=AC
LastObject=CGoonZuMCApp

[CLS:CGoonZuMCDlg]
Type=0
BaseClass=CDialog
HeaderFile=GoonZuMCDlg.h
ImplementationFile=GoonZuMCDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CGoonZuMCDlg

[CLS:CLoginDlg]
Type=0
BaseClass=CDialog
HeaderFile=LoginDlg.h
ImplementationFile=LoginDlg.cpp
LastObject=CLoginDlg

[CLS:CStatisticsDlg]
Type=0
BaseClass=CDialog
HeaderFile=StatisticsDlg.h
ImplementationFile=StatisticsDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CStatisticsDlg

[DLG:IDD_DIALOG_FRONTSERVERLIST]
Type=1
Class=CFrontServerListDlg
ControlCount=5
Control1=IDC_STATIC,button,1342177287
Control2=IDC_LIST_FRONTSERVER,SysListView32,1350664197
Control3=IDC_STATIC,static,1342312960
Control4=IDC_BUTTON_FRONTON,button,1476460544
Control5=IDC_BUTTON_FRONTOFF,button,1476460544

[DLG:IDD_DIALOG_GAMESERVERLIST]
Type=1
Class=CGameServerListDlg
ControlCount=24
Control1=IDC_LIST_GAMESERVER,SysListView32,1350631425
Control2=IDC_EDIT_GAMESERVERINFO,edit,1342244996
Control3=IDC_EDIT_DBMANAGERINFO,edit,1342244996
Control4=IDC_EDIT_GAMESERVERTITLE,edit,1342244992
Control5=IDC_EDIT_DBMANAGERTITLE,edit,1342244992
Control6=IDC_STATIC,button,1342177287
Control7=IDC_BUTTON1,button,1476460544
Control8=IDC_BUTTON2,button,1476460544
Control9=IDC_BUTTON3,button,1476460544
Control10=IDC_BUTTON4,button,1476460544
Control11=IDC_STATIC,static,1342312960
Control12=IDC_STATIC,button,1342177287
Control13=IDC_BUTTON_ALARMON,button,1342242816
Control14=IDC_BUTTON_ALARMOFF,button,1342242816
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,button,1342177287
Control17=IDC_BUTTON_ALARMON2,button,1476460544
Control18=IDC_BUTTON_ALARMOFF2,button,1476460544
Control19=IDC_BUTTON_ALARMOFF3,button,1476460544
Control20=IDC_EDIT2,edit,1350631552
Control21=IDC_STATIC,static,1342308864
Control22=IDC_STATIC,static,1342312960
Control23=IDC_EDIT_CMD,edit,1350631552
Control24=IDC_BUTTON_CMD,button,1342242816

[DLG:IDD_GOONZUMC_DIALOG]
Type=1
Class=CGoonZuMCDlg
ControlCount=8
Control1=IDC_TAB_MAIN,SysTabControl32,1342177280
Control2=IDC_EDIT_TOTALUSER,edit,1350633600
Control3=IDC_STATIC,static,1342312961
Control4=IDC_STATIC_RICH,static,1342308352
Control5=IDC_STATIC,static,1342312961
Control6=IDC_EDIT_NEWCHAR,edit,1350633600
Control7=IDC_STATIC,static,1342312961
Control8=IDC_EDIT_PCUSER,edit,1350633600

[DLG:IDD_DIALOG_LOGIN]
Type=1
Class=CLoginDlg
ControlCount=10
Control1=IDC_EDIT_ID,edit,1350631552
Control2=IDC_EDIT_PW,edit,1350631584
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,static,1342308864
Control5=IDC_BUTTON_LOGIN,button,1342242816
Control6=IDC_COMBO_IP,combobox,1344340226
Control7=IDC_STATIC,static,1342308864
Control8=IDC_EDIT_STATUS,edit,1342244992
Control9=IDC_STATIC,button,1342177287
Control10=IDC_BUTTON_EXIT,button,1342242816

[DLG:IDD_DIALOG_STATISTICS]
Type=1
Class=CStatisticsDlg
ControlCount=31
Control1=IDC_STATIC,button,1342177287
Control2=IDC_COMBO_YEAR1,combobox,1344339971
Control3=IDC_BUTTON_REQUEST1,button,1342242816
Control4=IDC_COMBO_MONTH1,combobox,1344339971
Control5=IDC_COMBO_DAY1,combobox,1344339971
Control6=IDC_BUTTON_CLEAR1,button,1342242816
Control7=IDC_STATIC,static,1342312960
Control8=IDC_COMBO_YEAR2,combobox,1344339971
Control9=IDC_BUTTON_REQUEST2,button,1342242816
Control10=IDC_COMBO_MONTH2,combobox,1344339971
Control11=IDC_COMBO_DAY2,combobox,1344339971
Control12=IDC_BUTTON_CLEAR2,button,1342242816
Control13=IDC_STATIC,static,1342312960
Control14=IDC_COMBO_YEAR3,combobox,1344339971
Control15=IDC_BUTTON_REQUEST3,button,1342242816
Control16=IDC_COMBO_MONTH3,combobox,1344339971
Control17=IDC_COMBO_DAY3,combobox,1344339971
Control18=IDC_BUTTON_CLEAR3,button,1342242816
Control19=IDC_STATIC,static,1342312960
Control20=IDC_COMBO_YEAR4,combobox,1344339971
Control21=IDC_BUTTON_REQUEST4,button,1342242816
Control22=IDC_COMBO_MONTH4,combobox,1344339971
Control23=IDC_COMBO_DAY4,combobox,1344339971
Control24=IDC_BUTTON_CLEAR4,button,1342242816
Control25=IDC_STATIC,static,1342312960
Control26=IDC_LIST_USERNUM,SysListView32,1350631429
Control27=IDC_STATIC,button,1342177287
Control28=IDC_BUTTON_1,button,1342242816
Control29=IDC_BUTTON_2,button,1342242816
Control30=IDC_BUTTON_3,button,1342242816
Control31=IDC_BUTTON_4,button,1342242816

