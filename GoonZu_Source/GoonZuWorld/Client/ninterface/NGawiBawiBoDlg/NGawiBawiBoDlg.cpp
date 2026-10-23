#include "NGawiBawiBoDlg.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "Char/CharCommon/Char-Common.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../Client/Client.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../GawiBawiBoMgr/GawiBawiBoMgr_Client.h"
#include "../../CommonLogic/MsgType-GawiBawiBo.h"

extern cltCommonLogic* pclClient;

NGawiBawiBoDlg::NGawiBawiBoDlg()
{	
	m_pSprGawiBawiBo	= NULL;
	m_pSprResult		= NULL;
	m_pImgStcBalloon	= NULL;
	m_pImgStcCount		= NULL;

	for(SI16 i = 0; i < m_sMaxSelectButtonNum; i++)
	{
		m_pSelectButton[i]	= NULL;
		m_pStcAbuseNPC[i]	= NULL;
	}

	for(SI16 i = 0; i < m_sMaxStatus; i++)
		m_pStcStatusIcon[i] = NULL;

	m_siSelecteIndex			= GAWIBAWIBO_NONE;
	m_siStatus					= GAWIBAWIBO_STATUS_NONE;
	m_pStcResult				= NULL;
	m_pStcCumulativeMoney		= NULL;
	m_pCharDraw					= NULL;
	m_siAniDirection			= CCD_SOUTH_EAST;

	m_siAniTypePlayer			= WT2U;
	m_siAniTypeNPC				= ANITYPE_WAIT0;

	ZeroMemory(&m_lnPositionPlayer, sizeof(POINT));
	ZeroMemory(&m_lnPositionNPC, sizeof(POINT));
}

NGawiBawiBoDlg::~NGawiBawiBoDlg()
{
	cltClient *pClient = (cltClient*)pclClient;
	if(pclClient != NULL)
		pClient->GetGawiBawiBoMgr()->SetStatus(CGawiBawiBoMgr_Client::GAWIBAWIBO_WAIT_PLAYER);

	SAFE_DELETE(m_pCharDraw);
	
}


void NGawiBawiBoDlg::Create()
{
	if (IsCreate() == false)
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NGawi-Bawi-Bo/nGawi-Bawi-Bo.ddf"));
		file.CreatePopupDialog( this, NGAWIBAWIBO_DLG , TEXT("dialog_GAWI-BAWI-BO"), StaticNGawiBawiBoDlgProc );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NGAWIBAWIBO_STATIC_RESULT, this ),			NGAWIBAWIBO_STATIC_RESULT,		"statictext_RESULT"); 

		//	가위바위보 선택버튼
		file.CreateControl(m_InterfaceFactory.CreateUserInterface(BUTTON, NGAWIBAWIBO_BUTTON_GAWI,	this ),	NGAWIBAWIBO_BUTTON_GAWI,	"button_GAWI"); 	
		file.CreateControl(m_InterfaceFactory.CreateUserInterface(BUTTON, NGAWIBAWIBO_BUTTON_BAWI,	this ), NGAWIBAWIBO_BUTTON_BAWI,	"button_BAWI"); 	
		file.CreateControl(m_InterfaceFactory.CreateUserInterface(BUTTON, NGAWIBAWIBO_BUTTON_BO,	this ),	NGAWIBAWIBO_BUTTON_BO,		"button_BO"); 	

		file.CreateControl(m_InterfaceFactory.CreateUserInterface(STATIC, NGAWIBAWIBO_BUTTON_ABUSE_GAWI,	this ),	NGAWIBAWIBO_BUTTON_ABUSE_GAWI,"button_ABUSE_GAWI"); 	
		file.CreateControl(m_InterfaceFactory.CreateUserInterface(STATIC, NGAWIBAWIBO_BUTTON_ABUSE_BAWI,	this ),	NGAWIBAWIBO_BUTTON_ABUSE_BAWI,"button_ABUSE_BAWI"); 	
		file.CreateControl(m_InterfaceFactory.CreateUserInterface(STATIC, NGAWIBAWIBO_BUTTON_ABUSE_BO,		this ),	NGAWIBAWIBO_BUTTON_ABUSE_BO,"button_ABUSE_BO"); 	

		
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, NGAWIBAWIBO_IMAGESTATIC_COUNT,	this ), NGAWIBAWIBO_IMAGESTATIC_COUNT,	"imagestatic_count");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, NGAWIBAWIBO_IMAGESTATIC_BALLOON,	this ), NGAWIBAWIBO_IMAGESTATIC_BALLOON,	"imagestatic_balloon");
		
		// 결과
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NGAWIBAWIBO_STATIC_MYHAND , this ),			NGAWIBAWIBO_STATIC_MYHAND,		"statictext_MYHAND"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NGAWIBAWIBO_STATIC_MONSTERHAND , this ),	NGAWIBAWIBO_STATIC_MONSTERHAND,	"statictext_MONSTERHAND"); 

		// VS
		file.CreateControl(m_InterfaceFactory.CreateUserInterface(STATIC, NGAWIBAWIBO_STATIC_VS,	this ),	NGAWIBAWIBO_STATIC_VS,		"statictext_VS"); 	

		// 캐릭터출력 뷰어
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, NGAWIBAWIBO_IMAGESTATIC_PLAYER_VIEWER,	this ), NGAWIBAWIBO_IMAGESTATIC_PLAYER_VIEWER,	"imagestatic_PLAYER_VIEWER");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, NGAWIBAWIBO_IMAGESTATIC_MONSTER_VIEWER, this ), NGAWIBAWIBO_IMAGESTATIC_MONSTER_VIEWER, "imagestatic_MOSTER_VIEWER");

        // 누적금액
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NGAWIBAWIBO_STATIC_CUMULATIVEMONEY, this ),	NGAWIBAWIBO_STATIC_CUMULATIVEMONEY,		"statictext_CUMULATIVEMONEY"); 

		m_pSelectButton[GAWIBAWIBO_GAWI]	= m_InterfaceFactory.GetButton(NGAWIBAWIBO_BUTTON_GAWI);
		m_pSelectButton[GAWIBAWIBO_BAWI]	= m_InterfaceFactory.GetButton(NGAWIBAWIBO_BUTTON_BAWI);
		m_pSelectButton[GAWIBAWIBO_BO]		= m_InterfaceFactory.GetButton(NGAWIBAWIBO_BUTTON_BO);

		m_pStcAbuseNPC[GAWIBAWIBO_GAWI]	= m_InterfaceFactory.GetButton(NGAWIBAWIBO_BUTTON_ABUSE_GAWI);
		m_pStcAbuseNPC[GAWIBAWIBO_BAWI]	= m_InterfaceFactory.GetButton(NGAWIBAWIBO_BUTTON_ABUSE_BAWI);
		m_pStcAbuseNPC[GAWIBAWIBO_BO]		= m_InterfaceFactory.GetButton(NGAWIBAWIBO_BUTTON_ABUSE_BO);

		for(SI16 i = 1; i < m_sMaxSelectButtonNum; i++)
			m_pStcAbuseNPC[i]->Show(false);

		
		m_pStcStatusIcon[GAWIBAWIBO_PLAYER]	= m_InterfaceFactory.GetStatic(NGAWIBAWIBO_STATIC_MYHAND);
		m_pStcStatusIcon[GAWIBAWIBO_NPC]	= m_InterfaceFactory.GetStatic(NGAWIBAWIBO_STATIC_MONSTERHAND);

		m_pStcResult = m_InterfaceFactory.GetStatic(NGAWIBAWIBO_STATIC_RESULT);
		m_pStcResult->Show(false);

		m_pImgStcBalloon = m_InterfaceFactory.GetImageStatic(NGAWIBAWIBO_IMAGESTATIC_BALLOON);
		m_pImgStcBalloon->Show(false);

		m_pImgStcCount = m_InterfaceFactory.GetImageStatic(NGAWIBAWIBO_IMAGESTATIC_COUNT);
		m_pImgStcCount->Show(false);

		m_pStcCumulativeMoney = m_InterfaceFactory.GetStatic(NGAWIBAWIBO_STATIC_CUMULATIVEMONEY);
		
		SetActionTimer(300);
		SetTimerOnOff( true );  

		m_pSprGawiBawiBo	= pclClient->GetGlobalSpr(GIMG_GAWIBAWIBO);
		m_pSprResult		= pclClient->GetGlobalSpr(GIMG_GUILDWAR_NOTICE);

		SetCumulativeMoneyText(0);

		m_pCharDraw = new CCharacterDrawing(pclClient->GameMode);

		SetPlayerDraw();
		SetNPCDraw();
		m_lnPositionPlayer.x	= GetX() + m_InterfaceFactory.GetStatic(NGAWIBAWIBO_IMAGESTATIC_PLAYER_VIEWER)->GetX(); 
		m_lnPositionPlayer.y	= GetY() + m_InterfaceFactory.GetStatic(NGAWIBAWIBO_IMAGESTATIC_PLAYER_VIEWER)->GetY();
		m_lnPositionNPC.x		= GetX() + m_InterfaceFactory.GetStatic(NGAWIBAWIBO_IMAGESTATIC_MONSTER_VIEWER)->GetX();
		m_lnPositionNPC.y		= GetY() + m_InterfaceFactory.GetStatic(NGAWIBAWIBO_IMAGESTATIC_MONSTER_VIEWER)->GetY();
	}
	else
        DeleteDialog();
}

void CALLBACK NGawiBawiBoDlg::StaticNGawiBawiBoDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NGawiBawiBoDlg *pThis = (NGawiBawiBoDlg*) pControllerMgr;
	pThis->NGawiBawiBoDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NGawiBawiBoDlg::NGawiBawiBoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pClient = (cltClient*)pclClient;

	if(pClient == NULL)
		return;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			//	닫기 시도시 게임도중 포기로 간주한다.
			TCHAR* pTitle	= GetTxtFromMgr(40050);
			NTCHARString256	pBody(GetTxtFromMgr(40051));

			//누적금액
			GMONEY siCumulativeMoney = pClient->GetGawiBawiBoMgr()->GetCumulativeMoney();

			NTCHARString256 strMoney;
			g_JYLibFunction.SetNumUnit(siCumulativeMoney, strMoney, 256);

			pBody.Replace("#money#", strMoney);

			stMsgBox MsgBox;
			MsgBox.Set((CControllerMgr*)pClient->m_pDialog[NGAWIBAWIBO_DLG], pTitle, pBody, MSGBOX_TYPE_YESNO, 1);
			pClient->SetMsgBox(&MsgBox, NULL, 0);
		}
		break;

	case NGAWIBAWIBO_BUTTON_GAWI:
		{
			if(SelectedBtn(GAWIBAWIBO_GAWI) == true)
			{
				pClient->GetGawiBawiBoMgr()->NextSteps();
				pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MENU_CLICK")), 0, 0);
			}
		}
		break;

	case NGAWIBAWIBO_BUTTON_BAWI:
		{
			if(SelectedBtn(GAWIBAWIBO_BAWI) == true)
			{
				pClient->GetGawiBawiBoMgr()->NextSteps();
				pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MENU_CLICK")), 0, 0);
			}
		}
		break;

	case NGAWIBAWIBO_BUTTON_BO:
		{
			if(SelectedBtn(GAWIBAWIBO_BO) == true)
			{
				pClient->GetGawiBawiBoMgr()->NextSteps();
				pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MENU_CLICK")), 0, 0);
			}
		}
		break;
	}
}

//	상태 아이콘 설정
void NGawiBawiBoDlg::SetStatusIcon()
{
	switch(m_siStatus)
	{
	case GAWIBAWIBO_STATUS_NONE:
		m_pStcResult->Show(false);
		break;

	case GAWIBAWIBO_STATUS_WAIT_SELECT:
		{
			m_pStcResult->Show(false);

			//	상태 아이콘을 대기 아이콘으로.
			if(m_pStcStatusIcon[GAWIBAWIBO_PLAYER]->GetFontIndex() == GAWIBAWIBO_PLAYER_WAIT_1)
			{
				SetIcon(GAWIBAWIBO_PLAYER, GAWIBAWIBO_PLAYER_WAIT_2);
				SetIcon(GAWIBAWIBO_NPC, GAWIBAWIBO_NPC_WAIT_2);
			}
			else
			{
				SetIcon(GAWIBAWIBO_PLAYER, GAWIBAWIBO_PLAYER_WAIT_1);
				SetIcon(GAWIBAWIBO_NPC, GAWIBAWIBO_NPC_WAIT_1);
			}
		}
		break;

	case GAWIBAWIBO_STATUS_SELECT_COMPLETE:
		{
			m_pStcResult->Show(false);

			//	선택이 완료 되었다면~
			cltClient *pClient	= (cltClient*)pclClient;
			SI08 siResult		= pClient->GetGawiBawiBoMgr()->GetResult();

			//	선택 완료
			switch(siResult)
			{
			case GAWIBAWIBO_RESULT_ABUSE_WIN:	//	NPC부정행위로
				{
					for(SI16 i = 1; i < m_sMaxSelectButtonNum; i++)
						m_pStcAbuseNPC[i]->Show(true);

					m_pStcStatusIcon[GAWIBAWIBO_NPC]->Show(false);

					pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_SUCCESS")), 0, 0);
				}
				break;

			case GAWIBAWIBO_RESULT_WIN:	//	이긴경우
				{	
					for(SI16 i = 1; i < m_sMaxSelectButtonNum; i++)
						m_pStcAbuseNPC[i]->Show(false);

					m_pStcStatusIcon[GAWIBAWIBO_NPC]->Show(true);

					switch(m_siSelecteIndex)
					{
					case GAWIBAWIBO_GAWI:
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_GAWI);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_BO);
						pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_SUCCESS")), 0, 0);
						break;

					case GAWIBAWIBO_BAWI:
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_BAWI);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_GAWI);
						pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_SUCCESS")), 0, 0);
						break;

					case GAWIBAWIBO_BO:
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_BO);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_BAWI);
						pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_SUCCESS")), 0, 0);
						break;

					default:
						assert(0);
						break;
					}
				}
				break;

			case GAWIBAWIBO_RESULT_DRAW:	// 비긴경우
				{
					for(SI16 i = 1; i < m_sMaxSelectButtonNum; i++)
						m_pStcAbuseNPC[i]->Show(false);

					m_pStcStatusIcon[GAWIBAWIBO_NPC]->Show(true);

					switch(m_siSelecteIndex)
					{
					case GAWIBAWIBO_GAWI:
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_GAWI);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_GAWI);
						pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_OK")), 0, 0);
						break;

					case GAWIBAWIBO_BAWI:
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_BAWI);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_BAWI);
						pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_OK")), 0, 0);
						break;

					case GAWIBAWIBO_BO:
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_BO);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_BO);
						pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_OK")), 0, 0);
						break;

					default:
						assert(0);
						break;
					}
				}
				break;

			case GAWIBAWIBO_RESULT_LOSE:	// 진 경우
				{
					for(SI16 i = 1; i < m_sMaxSelectButtonNum; i++)
						m_pStcAbuseNPC[i]->Show(false);

					m_pStcStatusIcon[GAWIBAWIBO_NPC]->Show(true);

					switch(m_siSelecteIndex)
					{
					case GAWIBAWIBO_GAWI:
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_GAWI);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_BAWI);
						pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_GIVEUP")), 0, 0);
						break;

					case GAWIBAWIBO_BAWI:
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_BAWI);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_BO);
						pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_GIVEUP")), 0, 0);
						break;

					case GAWIBAWIBO_BO:
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_BO);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_GAWI);
						pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_GIVEUP")), 0, 0);
						break;

					default:
						assert(0);
						break;
					}
				}
				break;
			default:
				{
					assert(0);
				}
				break;
			}
		}
		break;

	case GAWIBAWIBO_STATUS_RESULT:
		{
			cltClient	*pClient = (cltClient*)pclClient;
			SI08		siResult = pClient->GetGawiBawiBoMgr()->GetResult();
			m_pStcResult->Show(true);

			for(SI16 i = 1; i < m_sMaxSelectButtonNum; i++)
				m_pStcAbuseNPC[i]->Show(false);

			m_pStcStatusIcon[GAWIBAWIBO_NPC]->Show(true);

			// 가위바위보 버튼 비활성
			m_pSelectButton[GAWIBAWIBO_GAWI]->SetFocus(false);
			m_pSelectButton[GAWIBAWIBO_BAWI]->SetFocus(false);
			m_pSelectButton[GAWIBAWIBO_BO]->SetFocus(false);

			switch(siResult)
			{
			case GAWIBAWIBO_RESULT_ABUSE_WIN:	//	NPC부정행위로
				{

				}
				break;

			case GAWIBAWIBO_RESULT_WIN:
				{
					m_pStcResult->SetFontIndex(1);

					if(m_pStcStatusIcon[GAWIBAWIBO_PLAYER]->GetFontIndex() == GAWIBAWIBO_PLAYER_WIN_2)
					{
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_WIN_1);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_LOSE_1);
					}
					else
					{
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_WIN_2);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_LOSE_2);
					}
				}
				break;

			case GAWIBAWIBO_RESULT_DRAW:
				{
					m_pStcResult->SetFontIndex(3);
				}
				break;

			case GAWIBAWIBO_RESULT_LOSE:
				{
					m_pStcResult->SetFontIndex(2);

					if(m_pStcStatusIcon[GAWIBAWIBO_PLAYER]->GetFontIndex() == GAWIBAWIBO_PLAYER_LOSE_2)
					{
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_LOSE_1);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_WIN_1);
					}
					else
					{
						SetIcon(GAWIBAWIBO_PLAYER,	GAWIBAWIBO_PLAYER_LOSE_2);
						SetIcon(GAWIBAWIBO_NPC,		GAWIBAWIBO_NPC_WIN_2);
					}
				}
				break;

			default:
				assert(0);
				break;
			}
		}
		break;
	}
}

//	캐릭터 애니메이션 설정
void NGawiBawiBoDlg::SetStatusCharacter()
{
	cltClient* pClient = (cltClient*)pclClient; 

	switch(m_siStatus)
	{
	case GAWIBAWIBO_STATUS_WAIT_SELECT:
		{
			//	플레이어 와 NPC 캐릭터 모션을 대기 모션으로
			m_siAniTypePlayer	= WT2U;
			m_siAniTypeNPC		= ANITYPE_WAIT0;
		}
		break;

	case GAWIBAWIBO_STATUS_SELECT_COMPLETE:
		{
			//	선택 완료
		}
		break;

	case GAWIBAWIBO_STATUS_RESULT:
		{
			SI08 siResult = pClient->GetGawiBawiBoMgr()->GetResult();

			switch(siResult)
			{
			case GAWIBAWIBO_RESULT_ABUSE_WIN:	//	NPC부정행위로
				{
					m_siAniTypePlayer	= SPCU;
					m_siAniTypeNPC		= ANITYPE_DYING;

					m_lnPositionPlayer.x= GetX()-80;
				}
				break;

			case GAWIBAWIBO_RESULT_WIN:
				{
					m_siAniTypePlayer	= SPCU;
					m_siAniTypeNPC		= ANITYPE_DYING;

					m_lnPositionPlayer.x= GetX()-80;
				}
				break;

			case GAWIBAWIBO_RESULT_DRAW:
				{
					m_siAniTypePlayer	= WT2U;
					m_siAniTypeNPC		= ANITYPE_WAIT0;

					m_lnPositionPlayer.x= GetX()-80;
				}
				break;

			case GAWIBAWIBO_RESULT_LOSE:
				{
					m_siAniTypePlayer	= DTHU;
					m_siAniTypeNPC		= ANITYPE_ATTACK;
					m_lnPositionPlayer.x= GetX()-120;
				}
				break;

			default:
				assert(0);
				break;
			}
		}
		break;
	}
}

void NGawiBawiBoDlg::Action()
{
	SetGiveUpEnable();
	SetStatusIcon();
	SetStatusCharacter();
}

void NGawiBawiBoDlg::SetGiveUpEnable()
{
	cltClient *pClient	= (cltClient*)pclClient;
	//	포기창은 선택 중에만 띄울수 있다.
	if(	pClient->GetGawiBawiBoMgr()->GetStatus() == CGawiBawiBoMgr_Client::GAWIBAWIBO_WAIT_SELECT ||
		pClient->GetGawiBawiBoMgr()->GetStatus() == CGawiBawiBoMgr_Client::GAWIBAWIBO_NOW_WAIT_SELECT)
		Show(COMMON_DIALOG_CLOSEBUTTON, true);
	else
		Show(COMMON_DIALOG_CLOSEBUTTON, false);
}

void NGawiBawiBoDlg::Counting(clock_t nCount)
{
	m_pImgStcBalloon->Show(true);
	m_pImgStcCount->Show(true);

	nCount /= 1000;

	if(nCount < 0 || nCount >= 10)
		return;

    m_pImgStcCount->SetFontIndex(nCount);
}

void NGawiBawiBoDlg::EndCount()
{
	m_pImgStcBalloon->Show(false);
	m_pImgStcCount->Show(false);
	m_pImgStcCount->SetFontIndex(0);
}


//	플레이어 드로우 설정
void NGawiBawiBoDlg::SetPlayerDraw()
{
	SI32 delay = 0, instant = 0;

	SI32 tempKind = pclClient->pclCM->CR[1]->GetKind();

	if( pclClient->pclCM->CR[1]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_GOONZUMASTER")) )
		tempKind = KIND_HERO1;

	m_pCharDraw->LoadItemCodeData(tempKind, "KOR01", "KOR01",	"KOR01", "", "", 0, NULL );

	delay	= m_pCharDraw->GetDelayTime(WT2U, tempKind);
	instant	= m_pCharDraw->GetFrameNum(WT2U) - 1 ;

	m_clAniInfoPlayer.SetAniDelay(delay);
	m_clAniInfoPlayer.SetAniInstant( instant );
}

void NGawiBawiBoDlg::SetNPCDraw()
{
	SI32 delay = 0, instant = 0;

	SI32 tempKind = pclClient->pclCM->CR[1]->GetKind();

	if( pclClient->pclCM->CR[1]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_GOONZUMASTER")) )
		tempKind = KIND_HERO1;

	m_pCharDraw->LoadItemCodeData(tempKind, "KOR01", "KOR01",	"KOR01", "", "", 0, NULL );

	delay	= m_pCharDraw->GetDelayTime(WT2U, tempKind);
	instant	= m_pCharDraw->GetFrameNum(WT2U) - 1 ;

	m_clAniInfoNPC.SetAniDelay(delay);
	m_clAniInfoNPC.SetAniInstant( instant );
}

void NGawiBawiBoDlg::DrawPlayer(SI32 ScreenRX, SI32 ScreenRY)
{
	SI32	delay;
	SI32	framenumber;

	framenumber = m_pCharDraw->GetFrameNum(m_siAniTypePlayer);

	if(((cltClient*)pclClient)->GetFrameDelay() == 25)
		delay = m_clAniInfoPlayer.siAniDelay * 2;
	else
		delay = m_clAniInfoPlayer.siAniDelay;

    m_clAniInfoPlayer.IncreaseAniDelayor(1);

	if(m_clAniInfoPlayer.GetAniDelayor() >= delay)
	{
		m_clAniInfoPlayer.SetAniDelayor(0);

		if(m_siAniTypePlayer == DTHU)
		{
			if(m_clAniInfoPlayer.GetAniStep() != (framenumber - 1))
				m_clAniInfoPlayer.IncreaseAniStep(1);

			if(framenumber != 0)
				m_clAniInfoPlayer.SetAniStep(m_clAniInfoPlayer.GetAniStep() % framenumber);
		}
		else
		{
			m_clAniInfoPlayer.IncreaseAniStep(1);

			if(framenumber != 0)
				m_clAniInfoPlayer.SetAniStep(m_clAniInfoPlayer.GetAniStep() % framenumber);
		}
	}

	// 방향에 따른 출력 좌표 보정 ( 부드러운 회전을 위해)
	SI32 offsetX = 0;
	SI32 offsetY = 0;

	if( g_SoundOnOffDlg.m_bMaxWindowMode )
	{
		offsetX += 25;
		offsetY += 30;
	}

	SI32	siDrawX			= ScreenRX + GetX() - 97;
	SI32	siDrawY			= ScreenRY + GetY() + 15;

	m_pCharDraw->DrawCharacterSelectMode(m_siAniTypePlayer, (char)m_siAniDirection, m_clAniInfoPlayer.GetAniStep(), siDrawX + offsetX, siDrawY + offsetY);
}

void NGawiBawiBoDlg::DrawNPC(SI32 ScreenRX, SI32 ScreenRY)
{
	cltClient* pClient = (cltClient*)pclClient;

	cltKindInfo* pclGetKI = pClient->pclKindInfoSet->pclKI[173];
	if(NULL == pclGetKI)
		return;

	SI32	siAniRef		= pclGetKI->clCharAniManager.FindRefFromAnitype(m_siAniTypeNPC);
	SI32	siFramenumber	= pclGetKI->clCharAniManager.GetFrameNum(siAniRef);
	SI32	siDelay			= 0;

	if ( pClient->GetFrameDelay() == 25 )
	{
		siDelay = m_clAniInfoNPC.siAniDelay * 2 + 2;
	}
	else
	{
		siDelay = m_clAniInfoNPC.siAniDelay + 2;
	} 

	m_clAniInfoNPC.IncreaseAniDelayor(1);

	if ( m_clAniInfoNPC.GetAniDelayor() >= siDelay )
	{
		m_clAniInfoNPC.SetAniDelayor(0);

		m_clAniInfoNPC.IncreaseAniStep(1);

		if(siFramenumber == 0)
		{
			m_clAniInfoNPC.SetAniStep(0);
		}
		else
		{
			m_clAniInfoNPC.SetAniStep(m_clAniInfoNPC.GetAniStep() % siFramenumber );
		}
	}

	TSpr*	psprDraw		= NULL;
	SI32	siFontIndex		= 0;
	bool	bReverse		= false;
	SI32	siEffectFont	= 0;

	SI32	siRef			= pclGetKI->clCharAniManager.FindFirstAni();

	SI32	siImageXsize	= pclGetKI->clCharAniManager.GetImageXsize(siRef, pclClient->CurrentClock);
	SI32	siImageYsize	= pclGetKI->clCharAniManager.GetImageYsize(siRef, pclClient->CurrentClock);

	SI32	siRealXSize		= pclGetKI->clKIDrawInfo.siXsizePercent*siImageXsize/100;
	SI32	siRealYSize		= pclGetKI->clKIDrawInfo.siYsizePercent*siImageYsize/100;

	SI32	siXStart		= (siImageXsize - siRealXSize) / 2;
	SI32	siYStart		= siImageYsize - pclGetKI->clKIDrawInfo.siBottomIndent - siRealYSize;

	SI32	siX				= m_InterfaceFactory.GetStatic(NGAWIBAWIBO_IMAGESTATIC_MONSTER_VIEWER)->GetX() + (m_InterfaceFactory.GetStatic(NGAWIBAWIBO_IMAGESTATIC_MONSTER_VIEWER)->GetWidth() / 2 - siRealXSize / 2);
	SI32	siY				= m_InterfaceFactory.GetStatic(NGAWIBAWIBO_IMAGESTATIC_MONSTER_VIEWER)->GetY() + (m_InterfaceFactory.GetStatic(NGAWIBAWIBO_IMAGESTATIC_MONSTER_VIEWER)->GetHeight() / 2 - siRealYSize / 2);

	SI32	siDrawX			= GetX() + siX - siXStart + ScreenRX;
	SI32	siDrawY			= GetY() + siY - siYStart + ScreenRY;

	//Normal	= ANITYPE_WAIT0
	//LevelUp	= ANITYPE_ATTACK
	//Die		= ANITYPE_DYING


	pclGetKI->clCharAniManager.DecideDrawImg( m_siAniTypeNPC, (SOUTH | EAST), m_clAniInfoNPC, TABS(pclClient->CurrentClock), &psprDraw, &siFontIndex, &bReverse, &siEffectFont );

	if(psprDraw != NULL)
		GP.PutSpr( psprDraw, siDrawX, siDrawY, siFontIndex, bReverse );
}

void NGawiBawiBoDlg::DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY)
{
	DrawPlayer(ScreenRX, ScreenRY);
	DrawNPC(ScreenRX, ScreenRY);
}

void NGawiBawiBoDlg::SetStatus(SI16 siStatus)
{
	if(siStatus < GAWIBAWIBO_STATUS_WAIT_SELECT || siStatus > GAWIBAWIBO_STATUS_RESULT)
		return;
		
	m_siStatus = siStatus;
}


void NGawiBawiBoDlg::SetIcon(SI08 siPlayer, SI08 siIconIndex)
{
	if(!((siPlayer == GAWIBAWIBO_PLAYER) || (siPlayer == GAWIBAWIBO_NPC)))
		return;

	m_pStcStatusIcon[siPlayer]->SetFontIndex(siIconIndex);	
}

bool NGawiBawiBoDlg::SelectedBtn(SI16 siIndex)							//	선택한 버튼 설정
{
	bool bResult = true;

	cltClient* pClient = (cltClient*)pclClient;
	
	if(pClient->GetGawiBawiBoMgr()->GetStatus() == CGawiBawiBoMgr_Client::GAWIBAWIBO_NOW_WAIT_SELECT)
		m_siSelecteIndex = siIndex;
	else
		bResult = false;

	return bResult;
}

void NGawiBawiBoDlg::RandomSelect(SI32 siIndex)										//	랜덤으로 패를 선택
{
	cltClient* pClient = (cltClient*)pclClient;

	if(SelectedBtn(siIndex) == true)
	{
		pClient->GetGawiBawiBoMgr()->NextSteps();
		pClient->PushEffect(pClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MENU_CLICK")), 0, 0);
	}
}

void NGawiBawiBoDlg::SetCumulativeMoneyText(GMONEY siMoney)
{
	NTCHARString256	pBody(GetTxtFromMgr(10342));
	
	NTCHARString256 strMoney;
	g_JYLibFunction.SetNumUnit(siMoney,strMoney, 256);

	pBody.Replace("#money#", strMoney);

	m_pStcCumulativeMoney->SetText(pBody);
}


//	메세지 박스 처리
void NGawiBawiBoDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pClient = (cltClient*)pclClient;
	
	if(pClient == NULL)
		return;

	switch(MsgIndex)
	{
	case 0:	//	금액 체크
		{
			if(RetValue)	//	 clicked ok
			{
				//	보유 금액
				GMONEY siMyMoney	= pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney();
				
				//	 플레이 금액
				SI32 siNpcrate		= pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
				GMONEY siNeedMoney	= siNpcrate * 200;
				
				if(siMyMoney >= siNeedMoney)
				{
					//	 플레이 할 금액이 있다. 다음 스텝을 진행한다.
					pClient->GetGawiBawiBoMgr()->NextSteps();
				}
				else		
				{
					// 플레이 금액이 모자르다.
					TCHAR*	pTitle	= GetTxtFromMgr(10345);
					TCHAR*	pBody	= GetTxtFromMgr(1261);
					pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);

					pClient->GetGawiBawiBoMgr()->GiveUp();
				}
			}
			else			//	 clicked cancel
			{
				pClient->GetGawiBawiBoMgr()->GiveUp();
			}
		}
		break;

	case 1:	//	기권시
		{
			if(RetValue)	
			{
				// 기권이 가능한 상황인지 체크한다.
				if(	pClient->GetGawiBawiBoMgr()->GetStatus() == CGawiBawiBoMgr_Client::GAWIBAWIBO_WAIT_SELECT ||
					pClient->GetGawiBawiBoMgr()->GetStatus() == CGawiBawiBoMgr_Client::GAWIBAWIBO_NOW_WAIT_SELECT)
				{
					//	플레이어가 기권함. 보상받고 나간다.
					pClient->GetGawiBawiBoMgr()->GiveUp();
				}
				else
				{
					// 기권할수 없는상황에서 기권하려함.
					
				}
			}
			else
			{
				//	기권안함

			}
		}
		break;

	case 2:	//	승리시
		{
			if(RetValue)	
			{
				//	계속 진행 한다고 했으니 다시 금액체크~
				pClient->GetGawiBawiBoMgr()->ReGame();
			}
			else
			{
				//	여기서 중단. 보상받고 나간다.
				pClient->GetGawiBawiBoMgr()->GiveUp();
			}
		}
		break;

	case 3:	//	패배시
		{
			if(RetValue)	
			{
				//	종료
				pClient->GetGawiBawiBoMgr()->EndGame();
				DeleteDialog();
			}
			else
			{
				//	종료
				pClient->GetGawiBawiBoMgr()->EndGame();
				DeleteDialog();
			}
		}
		break;
	}
}
