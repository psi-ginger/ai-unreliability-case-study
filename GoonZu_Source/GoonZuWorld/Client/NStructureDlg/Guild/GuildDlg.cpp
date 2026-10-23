#include "GuildDlg.h"

#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"
#include "../../Client/abusefilter/AbuseFilter.h"

#include "MsgType-Guild.h"

#include "../../common/Char/CharManager/CharManager.h"

extern cltCommonLogic *pclClient;


NGuildDlg::NGuildDlg()
{

}

NGuildDlg::~NGuildDlg()
{

}

void NGuildDlg::Create()
{

	CreateDlg( NGUILDSTR_DLG, pclClient->pclGuildManager, 3 );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pText = GetTxtFromMgr(6594);
	AddUserTabDialog( 0, pText, 70, new NGuildListDlg, TEXT("NInterface/Data/NStructureDlg/DLG_GuildStrList.ddf"), TEXT("dialog_guildstrlist") );
	pText = GetTxtFromMgr(5255);
	AddUserTabDialog( 1, pText, 70, new NGuildUserDlg, TEXT("NInterface/Data/NStructureDlg/DLG_GuildStrUser.ddf"), TEXT("dialog_guildstruser") );
	pText = GetTxtFromMgr(6595);
	AddUserTabDialog( 2, pText, 70, new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );
	if( pclClient->IsCountrySwitch(Switch_EnableGuildWar) )
	{
		pText = GetTxtFromMgr(6509);
		AddUserTabDialog( 3, pText, 40, new NGuildWarListDlg, TEXT("NInterface/Data/NStructureDlg/DLG_GuildWarList.ddf"), TEXT("dialog_guildwarlist"));
	}

	// 행수 메뉴
	SetTabInfo( 2, 1 );
	pText = GetTxtFromMgr(3509);
	AddChiefTabDialog( 0, pText, 60, new NGuildChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_GuildStrChief.ddf"), TEXT("dialog_guildstrchief") );
	pText = GetTxtFromMgr(3443);
	AddChiefTabDialog( 1, pText, 60, new NStrPayTaxDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 90, new NStrRepairDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );

	SetTabInfo( 2, 2 );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 60, new NStrResignChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );
	pText = GetTxtFromMgr(6539);
	AddChiefTabDialog( 4, pText, 150, new NGuildManageDlg, TEXT("NInterface/Data/NStructureDlg/DLG_GuildManage.ddf"), TEXT("dialog_guildmanage") );


	if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
	{
		if ( Const_Beginner_Village == pclClient->pclGuildManager->siVillageUnique )
		{
			// 버튼 비활성화
			DisableTabButton( 0, true );
			DisableTabButton( 1, true );
			DisableTabButton( 2, true );
			DisableTabButton( 3, true );
		}
	}

	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );

	RefreshAllList();

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENGUILD);
}

void NGuildDlg::GreetingMsg()
{
	TCHAR* pText = GetTxtFromMgr(6596);
	SetMainExplain(pText);
}

void NGuildDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:	// 길드 개설 신청하기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 VillageUnique = *((SI32*)pData);
				TCHAR GuildName[13] = "";
				memcpy(GuildName,(TCHAR*)(pData + 4),DataLen - 4 );
					
				
#ifdef _IAF_EXPORT
				if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
				{
					TCHAR ResultBuf[1024] = "";
					if( 1 == IAF::Filtering( GuildName, ResultBuf, sizeof(ResultBuf) , IAF::FILTER_CHARACTER ) )
					{                    
						TCHAR * pText = GetTxtFromMgr(1854);
						TCHAR * pTitle = GetTxtFromMgr(6527);

						if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
						{
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle , pText);						
						}
						return;
					}	
				}
#else
				if(pclclient->m_pCAbuseFilter->CheckAbuseWord( GuildName ))
				{                    
					TCHAR * pText = GetTxtFromMgr(1854);
					TCHAR * pTitle = GetTxtFromMgr(6527);

					if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
					{
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle , pText);						
					}
					return;
				}	
				else if(TrimChar(GuildName, ' ', DataLen-4) == 0)
				{
					TCHAR * pText = GetTxtFromMgr(15552);
					TCHAR * pTitle = GetTxtFromMgr(6527);

					if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle , pText);						

					return; 
				}

#endif
				cltGameMsgRequest_Guild_CreateGuild CreateGuild(VillageUnique,GuildName);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_CREATEGUILD, sizeof(CreateGuild), (BYTE*)&CreateGuild);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 1:	// 길드 개설 허가하기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 guildunique = *((SI32*)pData);

				cltGameMsgRequest_Guild_ConfirmCreateGuild clinfo(guildunique,true);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_CONFIRMCREATEGUILD, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 2:	// 길드 개설 취소하기 
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 guildunique = *((SI32*)pData);

				cltGameMsgRequest_Guild_ConfirmCreateGuild clinfo(guildunique,false);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_CONFIRMCREATEGUILD, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 3:	// 길드 가입 신청하기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 guildunique = *((SI32*)pData);

				cltGameMsgRequest_Guild_JoinGuild clinfo(guildunique);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_JOINGUILD, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 4:	// 길드 탈퇴하기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 guildunique = *((SI32*)pData);

				cltGameMsgRequest_Guild_LeaveGuild clinfo(guildunique);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_LEAVEGUILD, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 5: // 길드 대기 취소 - 길드 삭제
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 guildunique = *((SI32*)pData);
				SI32 villageunique = *((SI32*)(pData + sizeof(SI32) ));

				cltGameMsgRequest_Guild_DestroyGuild clinfo(guildunique,villageunique);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_DESTROYGUILD, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 6: // 길드 본거지 변경
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 guildunique = *((SI32*)pData);
				SI32 villageunique = *((SI32*)(pData + sizeof(SI32) ));

				cltGameMsgRequest_Guild_ChangeVillage clinfo(guildunique,villageunique);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_CHANGEVILLAGE, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}

void NGuildDlg::RefreshAllList()
{
	((NGuildListDlg*)m_pUserChildDlg[0])->ShowGuildUnitList();
	((NGuildManageDlg*)m_pChiefChildDlg[4])->RefreshList();
}



//	넘어온 문자열이 공백문자갯수만큼 count 리턴
SI32 NGuildDlg::TrimChar( TCHAR *InTextData,TCHAR ch, SI32 siStrLen )
{
	SI32 siCount = 0;

	for ( int i = 0; i < siStrLen; i++ )
	{
		if ( InTextData[ i ] == ch )
            continue;
		else
			siCount++;
	}
	return siCount;
}