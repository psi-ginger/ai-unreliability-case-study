
#pragma once

#include "../../../InterfaceMgr/DialogController.h"

class CNMyGuild;
class CNMyGuildMaster;
class CNMyGuildMasterManage;
class CNMyGuildStatusStatus;
class CNMyGuildMasterHuntMap;
class CNMyGuildMasterMark;

class CNMyGuildBase : public CDialogController
{
public:

	CNMyGuildBase();
	~CNMyGuildBase();

	void	Initialize();
	void	Destroy();

	void	Create( );

	static void CALLBACK StaticNMyGuildBaseDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NMyGuildBaseDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl);

	void	SelectMasterSubTab(SI32 TabDialogID);

	void	RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	InterfaceFactory m_InterfaceFactory;

	CNMyGuild	 *MyGuildTab;
	CNMyGuildMaster *MyGuildMasterTab;
	CNMyGuildMasterManage *MasterManageTab;
	CNMyGuildStatusStatus *MasterStatusTab;
	CNMyGuildMasterMark	*MasterMarkTab;


	void RefreshMyGuildMemberList( cltGuildInfo guildinfo );
	
private:


};