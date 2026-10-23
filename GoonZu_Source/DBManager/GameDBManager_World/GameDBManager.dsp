# Microsoft Developer Studio Project File - Name="GameDBManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=GameDBManager - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameDBManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameDBManager.mak" CFG="GameDBManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameDBManager - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "GameDBManager - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/DBManager/GameDBManager", HHBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameDBManager - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map:"C:\GameDBManager\GameDBManager.map" /debug /machine:I386 /out:"C:\GameDBManager\GameDBManager.exe" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GameDBManager - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"C:\GameDBManager\GameDBManager.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GameDBManager - Win32 Release"
# Name "GameDBManager - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DBGameProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\DBGameProtocol2.cpp
# End Source File
# Begin Source File

SOURCE=.\DBLogProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\DBLogProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsg2News.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgAssistFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgBank.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgCityhall.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgFeast.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgFriend.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgGeneralMeeting.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgGuilty.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgHealth.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgHorse.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgHorseMarket.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgHouse.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgHouseHorse.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgHouseShipyard.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgHunt.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgImportantParameta.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgIPGClient.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgItemMall.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgLand.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgLetter.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgMarket.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgMinister.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgPerson.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgPersonItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgPort.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgPostOffice.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgRank.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgRealEstate.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgShip.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgShipMarket.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgStock.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgSummon.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgSummonHeroMarket.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgSummonMarket.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgTax.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMsgVillage.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DBGameProtocol.h
# End Source File
# Begin Source File

SOURCE=.\DBGameProtocol2.h
# End Source File
# Begin Source File

SOURCE=.\DBLogMsg.h
# End Source File
# Begin Source File

SOURCE=.\DBLogProcess.h
# End Source File
# Begin Source File

SOURCE=.\DBLogProtocol.h
# End Source File
# Begin Source File

SOURCE=.\GameDB.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "ITZNetLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\itzbase.h
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\ITZNetLib\ITZEncrypt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\ITZNetLib\ITZEncrypt.h
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

SOURCE=..\..\NetworkLib\itznetlib\ITZNetIOCP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZNetIOCP.h
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

SOURCE=..\..\NetworkLib\itznetlib\ITZWinApp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NetworkLib\itznetlib\ITZWinApp.h
# End Source File
# End Group
# Begin Group "DB Msg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\DBMsg-Bank.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Cityhall.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-DailyQuest.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Fishing.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-ForeignMerchantNPC.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Friend.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-HorseMarket.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-House.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Hunt.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-IPGClient.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Item.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-ItemMall.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Market.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Person.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-PersonalShop.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Port.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-PostOffice.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-RealEstateMarket.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Ship.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Stock.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Structure.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Summon.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-SummonMarket.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-System.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Vote.h"
# End Source File
# Begin Source File

SOURCE=".\DBMsg-Web.h"
# End Source File
# Begin Source File

SOURCE=.\DBMsg.h
# End Source File
# End Group
# Begin Group "RichODBC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\RichODBC\ODBC\RichODBC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\RichODBC\ODBC\RichODBC.h
# End Source File
# End Group
# Begin Group "Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BeTheRich\common\Item\ItemCommon\cltItem.h
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\common\Item\ItemCommon\ItemCommon.h
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\common\Item\ItemCommon\ItemUnit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\common\Item\ItemCommon\ItemUnit.h
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\common\item\itemcommon\npcitem.h
# End Source File
# End Group
# Begin Group "Lib"

# PROP Default_Filter ""
# Begin Group "EtcTool"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BeTheRich\Lib\EtcTool\EtcTool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\Lib\EtcTool\EtcTool.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\BeTheRich\Lib\NUtil\NDate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\Lib\NUtil\NDate.h
# End Source File
# End Group
# Begin Group "AgentProcess"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AgentProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\AgentProcess.h
# End Source File
# Begin Source File

SOURCE=..\..\AdminSystem\MasterServer\MasterMsg.h
# End Source File
# End Group
# Begin Group "Minister"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BeTheRich\Server\Minister\MinisterBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\Server\Minister\MinisterBase.h
# End Source File
# End Group
# Begin Group "DBGameLogProtocol"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DBGameLogMsg.h
# End Source File
# Begin Source File

SOURCE=.\DBGameLogProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\DBGameLogProtocol.h
# End Source File
# End Group
# Begin Group "GeneralMeeting"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\BeTheRich\CommonLogic\CityHall\GeneralMeeting.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BeTheRich\CommonLogic\CityHall\GeneralMeeting.h
# End Source File
# End Group
# Begin Group "IPGClient"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bxpgincli.tlb
# End Source File
# Begin Source File

SOURCE=.\bxpgincli.tlh
# End Source File
# End Group
# End Target
# End Project
