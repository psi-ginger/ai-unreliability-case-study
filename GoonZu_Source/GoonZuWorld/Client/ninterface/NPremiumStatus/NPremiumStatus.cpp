
#include "NPremiumStatus.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../../common/Char/CharManager/CharManager.h"
#include "../../Server/ServerEvent/ServerEvent_Manager.h"
#include "../../../common/Event/event.h"

extern cltCommonLogic* pclClient;


CNPremiumStatus::CNPremiumStatus()
{
		Initialize();
}

CNPremiumStatus::~CNPremiumStatus()
{
		Destroy();
}

void CNPremiumStatus::Initialize()
{
	

	SI16 i = 0;

	for( i = 0; i < NPREMINUMSTATUS_MAX_PREMINUMSTATUS ; i++ )
	{
			m_StatusStatic[i] = NULL;
	}

	m_PremiumShow = false;
	
}
void CNPremiumStatus::Destroy()
{
	if( m_PremiumStatusImg.pImage) GP.FreeSpr( m_PremiumStatusImg );

	SI16 i = 0;

	for( i = 0; i < NPREMINUMSTATUS_MAX_PREMINUMSTATUS ; i++ )
	{
		SAFE_DELETE(m_StatusStatic[i]);
	}
}

void CNPremiumStatus::Create()
{
	
	cltClient *pclclient = (cltClient *)pclClient;

	GP.LoadSprFromFile( TEXT("Gimg/PREMIUM_STATUS.SPR"), &m_PremiumStatusImg ); 

	m_siImageNum = m_PremiumStatusImg.GetImageNum();
	m_siImageXSize = m_PremiumStatusImg.GetXSize();
	m_siImageYSize = m_PremiumStatusImg.GetYSize();

	CDialogController::Create( NPREMIUMSTATUS_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticNPremiumProc, NULL, NULL , 0, false, 0,
		MAP_TILE_XSIZE + 642 + (g_SoundOnOffDlg.m_bMaxWindowMode ? 52 : 0) , 40,  m_siImageXSize , m_siImageYSize * m_siImageNum, false, false, false, false );


	// 프리미엄 스태터스 상태 수만큼의 이미지스태틱 객체 생성
	if( m_siImageNum > 0 )
	{
		for( SI16 i = 0; i < m_siImageNum; i ++ )
		{

			m_StatusStatic[i] = new CImageStatic( this );

			if( m_StatusStatic[i] == NULL ) continue;

			m_StatusStatic[i]->Create( NPREMIUMSTATUS_IMAGESTATIC_PREMIUM_0 , TEXT("GIMG/PREMIUM_STATUS.SPR"), i ,false, 0, 0, 0 + i * m_siImageYSize,
			m_siImageXSize , m_siImageYSize, m_siImageXSize , m_siImageYSize );
			
			m_StatusStatic[i]->Show(false);
		}

		// 각 이미지에 맞는 툴팁 설정
		if(m_StatusStatic[0])	m_StatusStatic[0]->SetUseToolTip( GetTxtFromMgr( 7093 ));
		if(m_StatusStatic[1])	m_StatusStatic[1]->SetUseToolTip( GetTxtFromMgr( 7094 ));
		if(m_StatusStatic[2])	m_StatusStatic[2]->SetUseToolTip( GetTxtFromMgr( 7095 ));
		if(m_StatusStatic[3])	m_StatusStatic[3]->SetUseToolTip( GetTxtFromMgr( 7096 ));
		if(m_StatusStatic[4])	m_StatusStatic[4]->SetUseToolTip( GetTxtFromMgr( 7097 ));
		if(m_StatusStatic[5])	m_StatusStatic[5]->SetUseToolTip( GetTxtFromMgr( 7098 ));
		if(m_StatusStatic[6])	m_StatusStatic[6]->SetUseToolTip( GetTxtFromMgr( 7099 ));
		if(m_StatusStatic[7])	m_StatusStatic[7]->SetUseToolTip( GetTxtFromMgr( 7100 ));
		if(m_StatusStatic[8])	m_StatusStatic[8]->SetUseToolTip( GetTxtFromMgr( 7101 ));
		if(m_StatusStatic[9])	m_StatusStatic[9]->SetUseToolTip( GetTxtFromMgr( 7102 ));
		if(m_StatusStatic[10])	m_StatusStatic[10]->SetUseToolTip( GetTxtFromMgr( 10049 ));
		if(m_StatusStatic[11])	m_StatusStatic[11]->SetUseToolTip( GetTxtFromMgr( 10050 ));
		//if(m_StatusStatic[12])	m_StatusStatic[12]->SetUseToolTip( GetTxtFromMgr( 0 )); 아래에서 설정한다
		//if(m_StatusStatic[13])	m_StatusStatic[13]->SetUseToolTip( GetTxtFromMgr( 0 )); 툴팁 없음
		//if(m_StatusStatic[14])	m_StatusStatic[14]->SetUseToolTip( GetTxtFromMgr( 0 )); 아래에서 설정한다
		//if(m_StatusStatic[15])	m_StatusStatic[15]->SetUseToolTip( GetTxtFromMgr( 0 )); 아래에서 설정한다
	}

	SetTopMostWindow( true );
	SetActionTimer( 3000 );
	SetTimerOnOff( true );
	
	
	


}

void CALLBACK CNPremiumStatus::StaticNPremiumProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{	
	CNPremiumStatus *pThis = (CNPremiumStatus *)pControllerMgr;
	pThis->NPremiumDlgProc( nEvent, nControlID, pControl );
	return;
	
}

void CNPremiumStatus::NPremiumDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	
}

void CNPremiumStatus::Action()
{
	cltCharCommon* pclChar = pclClient->pclCM->CR[1];

	cltClient * pclclient  = (cltClient*)pclClient;

	SI32 yIndet = 0;

	// 0
	if( m_StatusStatic[0] )
	{
		if( pclclient->bBonusTimeSwitch )
		{
			m_StatusStatic[0]->Show( true );
			m_StatusStatic[0]->MoveControl( 0 , yIndet );		
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[0]->Show( false );
		}
	}
	
	// 1
	if( m_StatusStatic[1] )
	{
		if( pclclient->bVillageWarSwitch )
		{
			m_StatusStatic[1]->Show( true );
			m_StatusStatic[1]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[1]->Show( false );	
		}
	}

	// 2
	if( m_StatusStatic[2] )
	{
		if( pclclient->pclBlackArmyManager->bWarSwitch )
		{
			m_StatusStatic[2]->Show( true );
			m_StatusStatic[2]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[2]->Show( false );	
		}
	}

	// 3	
	if( m_StatusStatic[3] )
	{
		if( pclChar->pclCI->clBI.bCombatService || pclChar->pclCI->clBI.bPremiumService )
		{
			m_StatusStatic[3]->Show( true );
			m_StatusStatic[3]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;

		}
		else
		{
			m_StatusStatic[3]->Show(false);
		}
	}

	// 4
	if( m_StatusStatic[4] )
	{
		if(  pclChar->pclCI->clBI.bMakeItemService )
		{
			m_StatusStatic[4]->Show( true );
			m_StatusStatic[4]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[4]->Show( false );		
		}
	}

	// 5
	if( m_StatusStatic[5] )
	{
		if( pclclient->bBonus2TimeSwitch )
		{
			m_StatusStatic[5]->Show( true );
			m_StatusStatic[5]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[5]->Show( false );		
		}
	}

	// 6
	if( m_StatusStatic[6] )
	{
		if( pclChar->pclCI->clBI.bSummonStaminaService )
		{
			m_StatusStatic[6]->Show( true );
			m_StatusStatic[6]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[6]->Show( false );		
		}
	}

	// 7
	if( m_StatusStatic[7] )
	{
		if( pclclient->bHuntingEventSwitch )
		{
			m_StatusStatic[7]->Show( true );
			m_StatusStatic[7]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[7]->Show( false );		
		}
	}

	// 8
	if( m_StatusStatic[8] )
	{
		if(pclclient->siMofuMofuEvent_VilliageUnique > 0 &&  pclclient->siMofuMofuEvent_VilliageUnique  == pclChar->pclCI->clBI.siHomeVillage )
		{
			m_StatusStatic[8]->Show( true );
			m_StatusStatic[8]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[8]->Show( false );
		}
	}

	// [9] PC방 프리미엄 글자로된 마크
	if( m_StatusStatic[9] )
	{
		if( pclChar->pclCI->clBI.bPCRoom )
		{
			m_StatusStatic[9]->Show( true );
			m_StatusStatic[9]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[9]->Show( false );		
		}
	}
	// 10
	if( m_StatusStatic[10] )
	{
		if ( pclclient->uiTiredState == cltCharCommon::TIRED_STATUS_LITTLE_TIRED )
		{
			m_StatusStatic[10]->Show( true );
			m_StatusStatic[10]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[10]->Show( false );		
		}
	}
	// 11
	if( m_StatusStatic[11] )
	{
		if ( pclclient->uiTiredState == cltCharCommon::TIRED_STATUS_MUCH_TIRED )
		{
			m_StatusStatic[11]->Show( true );
			m_StatusStatic[11]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[11]->Show( false );		
		}
	}

	if( m_StatusStatic[12] )
	{
		if(pclClient->IsCountrySwitch(Switch_Server_Event))
		{
			// 버프가 있을수 있는상태고(군주데이로 변경되면서 의미가 바뀜)  내 서버의 순위가 1위이면 버프를 적용한다는 표시를 그린다.
			if( pclclient->pclServerEventManager->GetBonusBufType() && pclclient->pclServerEventManager->IsMyServerRankBest() )
			{
				m_StatusStatic[12]->SetUseToolTip(GetTxtFromMgr(8724));
				m_StatusStatic[12]->Show( true );
				m_StatusStatic[12]->MoveControl( 0 , yIndet );	
				m_PremiumShow = true;
				yIndet += m_siImageYSize;
			}
			/*else if( pclclient->pclServerEventManager->IsMyServerRankBest2() )
			{
				m_StatusStatic[12]->SetUseToolTip(GetTxtFromMgr(8723));
				m_StatusStatic[12]->Show( true );
				m_StatusStatic[12]->MoveControl( 0 , yIndet );	
				m_PremiumShow = true;
				yIndet += m_siImageYSize;
			}*/
			else
			{
				m_StatusStatic[12]->Show( false );		
			}
		}
	}

	// 군주 Day(군주데이)
	if( m_StatusStatic[13] )
	{
		if ( pclclient->pclEventTimeManager->InEventTime("GoonzuDay_Week", &pclclient->sTime) )
		{
			m_StatusStatic[13]->Show( true );
			m_StatusStatic[13]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[13]->Show( false );		
		}
	}

	// PVP 무한대전 리그타임
	if( m_StatusStatic[14] )
	{
		// 신청시간/경기시간때 나온다
		if ( (pclclient->pclEventTimeManager->InEventTime("PVP_LeagueReserve", &pclclient->sTime))
			|| (pclclient->pclEventTimeManager->InEventTime("PVP_LeagueMatch", &pclclient->sTime))
			)
		{
			SetToolTip( 14, 0, 0 );
			m_StatusStatic[14]->Show( true );
			m_StatusStatic[14]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[14]->Show( false );		
		}
	}

	// PVP 무한대전 보너스타임
	if( m_StatusStatic[15] )
	{
		if ( (true == pclclient->pclEventTimeManager->InEventTime("PVP_League_1Grade_Bonus", &pclclient->sTime))
			|| (true == pclclient->pclEventTimeManager->InEventTime("PVP_League_1Grade_Bonus_2", &pclclient->sTime))
			)
		{
			SetToolTip( 15, 0, 0 );
			m_StatusStatic[15]->Show( true );
			m_StatusStatic[15]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
		}
		else
		{
			m_StatusStatic[15]->Show( false );		
		}
	}
	// 발렌타인데이 이벤트 // 커플일때
	if ( m_StatusStatic[16] )
	{
		if(pclChar->pclValentineWar != NULL)
		{
			if(pclChar->pclValentineWar->GetCouple() == VALENTINEWAR_COUPLE )
			{
				m_StatusStatic[17]->Show( false );
				m_StatusStatic[16]->Show( true );
				m_StatusStatic[16]->MoveControl( 0 , yIndet );	
				m_PremiumShow = true;
				yIndet += m_siImageYSize;
			}
			else
			{
				if(pclChar->pclValentineWar->GetCouple() == VALENTINEWAR_NONE )
				{
					m_StatusStatic[16]->Show( false );
					m_StatusStatic[17]->Show( false );
				}
			}
		}
		
	}
	// 발렌타인데이 이벤트 // 솔로일때
	if ( m_StatusStatic[17] )
	{
		if(pclChar->pclValentineWar != NULL)
		{
			if(pclChar->pclValentineWar->GetCouple() == VALENTINEWAR_SOLO )
			{
				m_StatusStatic[16]->Show( false );
				m_StatusStatic[17]->Show( true );
				m_StatusStatic[17]->MoveControl( 0 , yIndet );	
				m_PremiumShow = true;
				yIndet += m_siImageYSize;
			}
			else
			{
				if(pclChar->pclValentineWar->GetCouple() == VALENTINEWAR_NONE )
				{
					m_StatusStatic[16]->Show( false );
					m_StatusStatic[17]->Show( false );
				}
			}
		}		
	}
	
	// [춘기] 크리스마스 이벤트 - 엔펠리스 앞에 크리스마스 트리가 세워졌을때
	if ( m_StatusStatic[18] )
	{
		if(pclClient->siWorldTreeID[0] > 0)
		{		
			m_StatusStatic[18]->Show( true );
			m_StatusStatic[18]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
			m_StatusStatic[18]->SetUseToolTip( GetTxtFromMgr( 20094 ));
		}
		else if(pclClient->siWorldTreeID[1] > 0)
		{		
			m_StatusStatic[18]->Show( true );
			m_StatusStatic[18]->MoveControl( 0 , yIndet );	
			m_PremiumShow = true;
			yIndet += m_siImageYSize;
			m_StatusStatic[18]->SetUseToolTip( GetTxtFromMgr( 20095 ));
		}
		else
		{
			m_StatusStatic[18]->Show( false );
		}
	}

	SetDialogPos( GetX(), GetY(), GetWidth(), yIndet );

}


bool CNPremiumStatus::IsPremiumShow()
{
	return m_PremiumShow;
}

void CNPremiumStatus::SetToolTip(SI32 _siStatusIndex, SI32 _siToolTipTextNum, SI32 _siPara1)
{
	cltClient* pclclient = (cltClient*) pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	switch(_siStatusIndex)
	{
		case 12:
		{
			TCHAR szValue[8] = "";
			itoa(_siPara1, szValue, 10);
			NTCHARString128 kbuf(GetTxtFromMgr(_siToolTipTextNum));
			kbuf.Replace("#time#", "");
			kbuf.Replace("#bonus#", szValue );
			m_StatusStatic[12]->SetUseToolTip(kbuf);
		}
		break;

		case 14:
			{
				cltEventTime* pclPVPLeagueReserve = pclClient->pclEventTimeManager->GetEvent("PVP_LeagueReserve");
				if ( NULL == pclPVPLeagueReserve )
				{
					return;
				}

				cltEventTime* pclPVPLeagueMatch = pclClient->pclEventTimeManager->GetEvent("PVP_LeagueMatch");
				if ( NULL == pclPVPLeagueMatch )
				{
					return;
				}

				SI32 siStartHour	= pclPVPLeagueReserve->m_StartTime.wHour;
				SI32 siStartMinute	= pclPVPLeagueReserve->m_StartTime.wMinute;
				SI32 siEndHour		= pclPVPLeagueMatch->m_EndTime.wHour;
				SI32 siEndMinute	= pclPVPLeagueMatch->m_EndTime.wMinute;

				NTCHARString256 strText			= GetTxtFromMgr(9599);
				TCHAR			szStartTime[64]	= { '\0', };
				TCHAR			szEndTime[64]	= { '\0', };

				StringCchPrintf( szStartTime, sizeof(szStartTime), TEXT("%02d:%02d"), siStartHour, siStartMinute );
				StringCchPrintf( szEndTime, sizeof(szEndTime), TEXT("%02d:%02d"), siEndHour, siEndMinute );

				strText.Replace( TEXT("#starttime#"), szStartTime );
				strText.Replace( TEXT("#endtime#"), szEndTime );

				m_StatusStatic[14]->SetUseToolTip(strText);
			}
			break;

		case 15:
		{
			cltEventTime* pcl1Grade = NULL;
			cltEventTime* pcl2Grade = NULL;
			cltEventTime* pcl3Grade = NULL;

			if ( pclclient->pclEventTimeManager->InEventTime("PVP_League_1Grade_Bonus", &pclclient->sTime) )
			{
				pcl1Grade = pclClient->pclEventTimeManager->GetEvent("PVP_League_1Grade_Bonus");
				pcl2Grade = pclClient->pclEventTimeManager->GetEvent("PVP_League_2Grade_Bonus");
				pcl3Grade = pclClient->pclEventTimeManager->GetEvent("PVP_League_3Grade_Bonus");
			}
			else if ( pclclient->pclEventTimeManager->InEventTime("PVP_League_1Grade_Bonus_2", &pclclient->sTime) )
			{
				pcl1Grade = pclClient->pclEventTimeManager->GetEvent("PVP_League_1Grade_Bonus_2");
				pcl2Grade = pclClient->pclEventTimeManager->GetEvent("PVP_League_2Grade_Bonus_2");
				pcl3Grade = pclClient->pclEventTimeManager->GetEvent("PVP_League_3Grade_Bonus_2");
			}

			if ( (NULL == pcl1Grade) || (NULL == pcl2Grade) || (NULL == pcl3Grade) )
			{
				return;
			}

			NTCHARString256 strText = GetTxtFromMgr(9600);				
			strText.Replace( TEXT("#grade1#"), SI32ToString(pcl1Grade->m_siBonusAmount) );
			strText.Replace( TEXT("#grade2#"), SI32ToString(pcl2Grade->m_siBonusAmount) );
			strText.Replace( TEXT("#grade3#"), SI32ToString(pcl3Grade->m_siBonusAmount) );

			m_StatusStatic[15]->SetUseToolTip(strText);
		}
		break;
	}
}