
#include "../../../Client.h"
#include "../../../InterfaceMgr/InterfaceFile.h"
#include "../../../InterfaceMgr/InterfaceMgr.h"

#include "../../../../common/Char/CharManager/CharManager.h"

#include "MyGuildBase.h"
#include "MyGuild.h"
#include "MyGuildMaster.h"

#include "MyGuildMasterManage.h"
#include "MyGuildMasterStatus.h"
#include "MyGuildMasterHuntMap.h"
#include "MyGuildMasterMark.h"

#include "../../../../CommonLogic/Guild/Guild.h"

#include "../../../../CommonLogic/MsgType-Guild.h"

#include "../CommonLogic/MsgType-System.h"

extern cltCommonLogic* pclClient;

CNMyGuildBase::CNMyGuildBase()
{
	Initialize();
}	

CNMyGuildBase::~CNMyGuildBase()
{
	Destroy();
}

void CNMyGuildBase::Initialize()
{
	MyGuildTab = NULL;
	MyGuildMasterTab = NULL;
	MasterManageTab = NULL;
	MasterStatusTab = NULL;
	MasterMarkTab = NULL;
}

void CNMyGuildBase::Destroy()
{
	SAFE_DELETE(MyGuildTab);
	SAFE_DELETE(MyGuildMasterTab);
	SAFE_DELETE(MasterManageTab);
	SAFE_DELETE(MasterStatusTab);
	SAFE_DELETE(MasterMarkTab);
}

void CNMyGuildBase::Create()
{

	cltClient *pclclient = (cltClient *)pclClient;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/Guild/DLG_MyGuildBase.ddf"));
		file.CreatePopupDialog( this, NMYGUILDBASE_DLG, TEXT("dialog_MyGuildBase"), StaticNMyGuildBaseDlgProc);		

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDBASE_BUTTON_MYGUILD , this)  , NMYGUILDBASE_BUTTON_MYGUILD , TEXT("button_MyGuild"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDBASE_BUTTON_MYGUILD_MASTER, this)  , NMYGUILDBASE_BUTTON_MYGUILD_MASTER, TEXT("button_MyGuildMaster"));
		if( pclClient->IsCountrySwitch( Switch_Guild_GuildMaster_Change ) ){
			file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDBASE_BUTTON_MYGUILD_CHANGEGUILDMASTER, this)  , NMYGUILDBASE_BUTTON_MYGUILD_CHANGEGUILDMASTER, TEXT("button_ghangeguildmaster"));
		}
		
		MyGuildTab = new	CNMyGuild;
		MyGuildMasterTab = new	CNMyGuildMaster;
		MasterManageTab = new CNMyGuildMasterManage;
		MasterStatusTab = new CNMyGuildStatusStatus;
		MasterMarkTab = new CNMyGuildMasterMark;

		MyGuildTab->Create(this);
		MyGuildMasterTab->Create(this);
		MasterManageTab->Create(this);
		MasterStatusTab->Create(this);
		MasterMarkTab->Create(this);

		MyGuildMasterTab->Hide();
		MyGuildTab->Show();
		MasterManageTab->Hide();
		MasterStatusTab->Hide();
		MasterMarkTab->Hide();
		if( pclClient->IsCountrySwitch( Switch_Guild_GuildMaster_Change ) )
		{
			m_InterfaceFactory.GetButton(NMYGUILDBASE_BUTTON_MYGUILD_CHANGEGUILDMASTER)->Show(true);
		}
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNMyGuildBase::StaticNMyGuildBaseDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMyGuildBase *pThis = (CNMyGuildBase*) pControllerMgr;
	pThis->NMyGuildBaseDlgProc( nEvent, nControlID, pControl );
}


void CALLBACK CNMyGuildBase::NMyGuildBaseDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NMYGUILDBASE_BUTTON_MYGUILD_MASTER:
		{
			if ( pclclient->pclGuildManager->clCommonUnit.siUnique == pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex )
			{
				if ( pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siType == GUILDRANKTYPE_MASTER ||
					 pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siType == GUILDRANKTYPE_SECONDMASTER )
				{
					if(MyGuildMasterTab->IsShow())
						break;
					if(MyGuildTab->IsShow())
						MyGuildTab->Hide();

					MyGuildMasterTab->Show();
					MasterManageTab->Show();
					MasterStatusTab->Hide();
					MasterMarkTab->Hide();
					if( pclClient->IsCountrySwitch( Switch_Guild_GuildMaster_Change ) )
					{
						m_InterfaceFactory.GetButton(NMYGUILDBASE_BUTTON_MYGUILD_CHANGEGUILDMASTER)->Show(false);
					}
				}
			}
		}
		break;

	case NMYGUILDBASE_BUTTON_MYGUILD:
		{	
			if(MyGuildTab->IsShow())
				break;
			if(MyGuildMasterTab->IsShow())
			{
				MyGuildMasterTab->Hide();
				MasterManageTab->Hide();
				MasterStatusTab->Hide();
				MasterMarkTab->Hide();
				if( pclClient->IsCountrySwitch( Switch_Guild_GuildMaster_Change ) )
				{
					m_InterfaceFactory.GetButton(NMYGUILDBASE_BUTTON_MYGUILD_CHANGEGUILDMASTER)->Show(true);
				}
			}
			MyGuildTab->Show();	
			
		}
		break;
	case NMYGUILDBASE_BUTTON_MYGUILD_CHANGEGUILDMASTER:
		{
			// 길드 마스터 위임 시스템
			if( pclClient->IsCountrySwitch( Switch_Guild_GuildMaster_Change ) ){
				TCHAR buffer[512] = TEXT("") ;
				TCHAR* pTitle = GetTxtFromMgr(5614);
				TCHAR* pText = GetTxtFromMgr(8691);
				StringCchPrintf(buffer, 512, pText);

				

				stMsgBox MsgBox;
				MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 7 );
				pclclient->SetMsgBox( &MsgBox, NULL, 0 );
			}
		}break;

	}
	
}


void CNMyGuildBase::SelectMasterSubTab(SI32 TabDialogID)
{
	if( MyGuildMasterTab->IsShow() == false ) return;

	switch( TabDialogID )
	{
	case NMYGUILDMASTERMANAGE_DLG:
		{					
			MasterManageTab->Show();
			MasterStatusTab->Hide();
			MasterMarkTab->Hide();
		}
		break;
	case NMYGUILDMASTERSTATUS_DLG:
		{
			MasterManageTab->Hide();
			MasterStatusTab->Show();
			MasterMarkTab->Hide();
		}
		break;
	case NMYGUILDMASTERMARK_DLG:
		{
			MasterManageTab->Hide();
			MasterStatusTab->Hide();
			MasterMarkTab->Show();
		}
		break;
	}

}

void CNMyGuildBase::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{

	switch( MsgIndex )
	{
	case 0:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				GMONEY money = *((GMONEY*)pData);

				cltGameMsgRequest_Guild_GiveGuildMoney clinfo(money);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GIVEGUILDMONEY, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);	

				//if(pclCM->CR[1]->pclCI->clBI.clGuildRank.IsGuildMember())
				
				NTCHARString128	kBuf( GetTxtFromMgr(7933) );
				kBuf.Replace("#name#",  pclClient->pclCM->CR[1]->pclCI->clBI.clPerson.szName);
				kBuf.Replace("#money#", SI32ToString(money));
				
				((cltClient*)pclClient)->SendChatMessage(CHATMODE_GUILD, kBuf);
				
			}
		}
		break;
	case 1:	// 길드에서 퇴출시키기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 personid = *((SI32*)pData);

				cltGameMsgRequest_Guild_ConfirmJoinGuild clinfo(FALSE,personid);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_CONFIRMJOINGUILD, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);					
			}
		}
		break;
	case 2:	// 부길마에서 강등
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 personid = *((SI32*)pData);

				cltGameMsgRequest_Guild_SetSecondMaster clinfo(personid,false);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_SETSECONDMASTER, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);					
			}
		}
		break;
	case 3:	// 부길마로 임명
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 personid = *((SI32*)pData);

				cltGameMsgRequest_Guild_SetSecondMaster clinfo(personid,true);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_SETSECONDMASTER, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);					
			}
		}
		break;
	case 4: // 길드 이름 변경
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return ;
			}

			if ( RetValue )
			{
				TCHAR GuildName[13] = "";
				memcpy(GuildName,(TCHAR*)(pData),DataLen);

				cltGameMsgRequest_Guild_ChangeName clinfo(GuildName);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_CHANGENAME, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);					
			}
		}
		break;
	case 5: // 길드 마크 변경
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return ;
			}
			if( RetValue )
			{
				SI32 markIndex = *((SI32*)pData);

				cltGameMsgRequest_Guild_SetMark clinfo(markIndex);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_SETMARK, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 6: // 길드 마스터 권한 위임
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 personid = *((SI32*)pData);

				cltGameMsgRequest_Guild_ChangeMaster clinfo(personid);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_CHANGEMASTER, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);					
			}
		}
		break;
	case 7: // 길드 마스터 미접으로 위임
		{
			if ( RetValue )
			{
				if( pclClient->IsCountrySwitch( Switch_Guild_GuildMaster_Change ) ){
					if (pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siType == GUILDRANKTYPE_MASTER){
						cltClient *pclclient = (cltClient *)pclClient;
						//길드장은 불가능하다는 메세지
						TCHAR* pTitle = GetTxtFromMgr(404);// 임명실패
						TCHAR* pText  = GetTxtFromMgr(8689);//
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
						return;
					}
					cltGameMsgRequest_Guild_NotConnectMaster_ChangeMaster clinfo(MAX_NOTCONNECTMASTER);
					cltMsg clMsg(GAMEMSG_REQUEST_GUILD_NOTCONNECT_CHANGEMASTER, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);					
				}
				
				
			}
		}
		break;
	}
}

void CNMyGuildBase::RefreshMyGuildMemberList( cltGuildInfo guildinfo )
{
	MyGuildTab->RefreshList(guildinfo);
	MasterManageTab->RefreshManager();
	MasterStatusTab->RefreshStatus();
	MasterMarkTab->RefreshWindow();
}
