# Microsoft Developer Studio Project File - Name="GoonZuMC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GoonZuMC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GoonZuMC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GoonZuMC.mak" CFG="GoonZuMC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GoonZuMC - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GoonZuMC - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/AdminSystem/GoonZuMC", ERCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GoonZuMC - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "GoonZuMC - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__WS32__" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"C:\GoonZuMC\GoonZuMC.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GoonZuMC - Win32 Release"
# Name "GoonZuMC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FrontServerListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GameServerListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GoonZuMC.cpp
# End Source File
# Begin Source File

SOURCE=.\GoonZuMC.rc
# End Source File
# Begin Source File

SOURCE=.\GoonZuMCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MasterProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\MyRichEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\Lib\NUtil\NDataTypes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\Lib\NUtil\NDate.cpp
# End Source File
# Begin Source File

SOURCE=..\MasterServer\ServerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsData.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FrontServerListDlg.h
# End Source File
# Begin Source File

SOURCE=.\GameServerListDlg.h
# End Source File
# Begin Source File

SOURCE=.\GoonZuMC.h
# End Source File
# Begin Source File

SOURCE=.\GoonZuMCDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=..\MasterServer\MasterMsg.h
# End Source File
# Begin Source File

SOURCE=.\MasterProcess.h
# End Source File
# Begin Source File

SOURCE=.\MyRichEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\Lib\NUtil\NDataTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\Lib\NUtil\NDate.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\MasterServer\ServerInfo.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsData.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsDlg.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsGraph.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\GoonZuMC.ico
# End Source File
# Begin Source File

SOURCE=.\res\GoonZuMC.rc2
# End Source File
# End Group
# Begin Group "ITZNetLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\itzbase.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZCAQueue.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZCell.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZCell.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZEncrypt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZEncrypt.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZList.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZListen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZListen.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZMap.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZNetIOCP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZNetIOCP.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZNetOverlapped.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZNetOverlapped.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZNetwork.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZNetworkThread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZNetworkThread.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZObject.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZPerson.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZPerson.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZQList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZQList.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZQueue.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZRecvBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZRecvBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZSendBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZSendBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZServer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZServer.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZSession.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZSession.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZSessionFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZSessionManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZSessionManager.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZWorld.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZWorld.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
