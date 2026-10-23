
#include "NBalanceCurrencyDlg.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../../common/Char/CharManager/CharManager.h"
#include "../Graphic/Graphic.h"
#include "../Graphic/Graphic_Capsule.h"
#include "../../GlobalImgFile/GlobalImgFile.h"
#include "../../common/CommonHeader.h"
#include "../../Client.h"
#include "../NPremiumStatus/NPremiumStatus.h"
#include "../../GlobalImgFile/GlobalImgFile.h"

#include "../../ninterface/NBuffStatus/NBuffstatus.h"

#include "../CommonLogic/MsgType-Person.h"

#include "../common/Event/event.h"


extern cltCommonLogic* pclClient;


CNBalanceCurrencyDlg::CNBalanceCurrencyDlg()
{
	cltClient* pclclient = (cltClient*)pclClient;
	ZeroMemory(m_BuffKindArray, sizeof(m_BuffKindArray));


	m_BuffKindArray[0]	= BUF_ATTACKUP;						// 전투력 증가
	m_BuffKindArray[1]  = BUF_DEFENSEUP;					// 방어력 증가.
	//m_BuffKindArray[2]  = BUF_SPEEDUP;						// 스피드가 증가
	m_BuffKindArray[2]	= BUF_MAKEEXPUP;					// 제조 경험치 증가.
	m_BuffKindArray[3]  = BUF_NOFIRSTATTACK;				// 선공 방지 효과.
	m_BuffKindArray[4]  = BUF_SUMMONATACUP;					// 소환수 강화. 
	m_BuffKindArray[5]  = BUF_LOVEPOWER;					// 사랑 버프		
	m_BuffKindArray[6]  = BUF_FRIENDSHIP;					// 우정 버프
	m_BuffKindArray[7]  = BUF_HINAMOCHI;					// 레인보우 케익크 효과.
	m_BuffKindArray[8]	= BUF_SUCCESS_MANUFACTURE;			// 아이템 제조 100% 성공.
	m_BuffKindArray[9]	= BUF_FOURLEAF;						// 네잎클로버 - 아이템 드랍률 2배.
	m_BuffKindArray[10] = BUF_PREMIUMEXPUP;					// 경험치 150% 획득.
	m_BuffKindArray[11] = BUF_FARMINGNMININGFOREVER;		// 채광용 농경용 아이템. 

	m_SetBuffNum = 0;
	m_bRouletteButtunOnly = false;
}

CNBalanceCurrencyDlg::~CNBalanceCurrencyDlg()
{
	
}

void CNBalanceCurrencyDlg::SetControl()
{
	cltClient* pclclient = (cltClient*)pclClient;
	if( pclclient == NULL )			return;

	NTCHARString128 kbuf(GetTxtFromMgr(8454));
	kbuf.Replace("#money#", SI64ToString(GetBuffCost()));
	m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_EDIT_EXPLAIN)->SetText(kbuf);
	m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_EDIT_IMFORMATION)->SetText(GetTxtFromMgr(8549));

	m_bRouletteAction   = false;	
	m_bRouletteComplete = false;
	m_dwTickCount	  = 0;	
	m_PrevTickCount	  = 0; 
	m_DelayTickCount  = 0;
	
	//[진성] 를렛안의 모든 버프를 사용 후 버프룰렛을 사용하면 클라가 죽는거 수정. => 2008-9-2 
	if( 0 >= m_SetBuffNum ) return;
	if( pclClient->IsWhereServiceArea(ConstServiceArea_Korea) )
	{
		if( BUFF_BALANCECURRENCYLIMIT_NUM > m_SetBuffNum ) 
		{
			cltClient*  pclclient = (cltClient *)pclClient;
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(GetTxtFromMgr(5614), GetTxtFromMgr(8825));	
			DeleteDialog();
			return;
		}
	}

	// 버프 이미지 인덱스 번호.
	m_CurBuffNum  = rand()%m_SetBuffNum;

	m_PrevBuffNum = m_CurBuffNum - 1;  
	if(0 > m_PrevBuffNum)
		m_PrevBuffNum = m_SetBuffNum - 1;
  
	m_NextBuffNum = m_CurBuffNum + 1;
	if(m_SetBuffNum <= m_NextBuffNum)
		m_NextBuffNum = 0;		

	m_pBuffImage[BUFF_BALANCECURRENCY_PREV] =	m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_IMAGESTATIC_1);
	m_pBuffImage[BUFF_BALANCECURRENCY_CUR]  =	m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_IMAGESTATIC_2);
	m_pBuffImage[BUFF_BALANCECURRENCY_NEXT] =	m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_IMAGESTATIC_3);

	m_pBuffImage[BUFF_BALANCECURRENCY_PREV]->SetFileName("gimg/BufImage.SPR");
	m_pBuffImage[BUFF_BALANCECURRENCY_PREV]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_PrevBuffNum]) );

	m_pBuffImage[BUFF_BALANCECURRENCY_CUR]->SetFileName("gimg/BufImage.SPR");
	m_pBuffImage[BUFF_BALANCECURRENCY_CUR]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_CurBuffNum]) );

	m_pBuffImage[BUFF_BALANCECURRENCY_NEXT]->SetFileName("gimg/BufImage.SPR");
	m_pBuffImage[BUFF_BALANCECURRENCY_NEXT]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_NextBuffNum]) );		
}

void CNBalanceCurrencyDlg::Destroy()
{
	// 버프룰렛이 멈추면 해당 버프를 날림.
	cltGameMsgRequest_BalanceCurrency_Buff Roulette(false, -1, 0);
	cltMsg clMsg(GAMEMSG_REQUEST_BALANCECURRENCY_BUFF, sizeof(cltGameMsgRequest_BalanceCurrency_Buff), (BYTE*)&Roulette);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CNBalanceCurrencyDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NBuffRoulette/NBuffRoulette.ddf" ) );
	file.CreatePopupDialog( this, NBALANCECURRENCY_DLG, TEXT( "dialog_buff_roulette" ), StatiCNBalanceCurrencyDlgProc );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,			BUFF_ROULETTE_EDIT_EXPLAIN,		this ), BUFF_ROULETTE_EDIT_EXPLAIN,			TEXT( "editbox_explain" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,			BUFF_ROULETTE_EDIT_IMFORMATION,	this ), BUFF_ROULETTE_EDIT_IMFORMATION,		TEXT( "editbox_information" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,			BUFF_ROULETTE_BUTTON_CLOSE,		this ), BUFF_ROULETTE_BUTTON_CLOSE,			TEXT( "button_close" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,			BUFF_ROULETTE_BUTTON_START,		this ), BUFF_ROULETTE_BUTTON_START,			TEXT( "button_roulette_start" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,	BUFF_ROULETTE_IMAGESTATIC_1,	this ), BUFF_ROULETTE_IMAGESTATIC_1,		TEXT( "imagestatic_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,	BUFF_ROULETTE_IMAGESTATIC_2,	this ), BUFF_ROULETTE_IMAGESTATIC_2,		TEXT( "imagestatic_NONAME2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,	BUFF_ROULETTE_IMAGESTATIC_3,	this ), BUFF_ROULETTE_IMAGESTATIC_3,		TEXT( "imagestatic_NONAME3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,	BUFF_ROULETTE_IMAGESTATIC_4,	this ), BUFF_ROULETTE_IMAGESTATIC_4,		TEXT( "imagestatic_NONAME4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,	BUFF_ROULETTE_IMAGESTATIC_5,	this ), BUFF_ROULETTE_IMAGESTATIC_5,		TEXT( "imagestatic_NONAME5" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,	BUFF_ROULETTE_IMAGESTATIC_6,	this ), BUFF_ROULETTE_IMAGESTATIC_6,		TEXT( "imagestatic_NONAME6" ) );	
	
	// 관련 정보 세팅.
	SetBuff();
	SetControl();
	
	SetActionTimer( 1 );    
	SetTimerOnOff( true );
}

void CALLBACK CNBalanceCurrencyDlg::StatiCNBalanceCurrencyDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNBalanceCurrencyDlg *pThis = (CNBalanceCurrencyDlg *)pControllerMgr;
	pThis->NBalanceCurrencyDlgProc(nEvent,nControlID,pControl);
	return;
}

void CALLBACK CNBalanceCurrencyDlg::NBalanceCurrencyDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch ( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				pclClient->pclCM->CR[1]->bBuffRouletteSwitch = false ;
			}
		case BUFF_ROULETTE_BUTTON_CLOSE:
			{
				// 버프룰렛이 멈추면 해당 버프를 날림.
				cltGameMsgRequest_BalanceCurrency_Buff Roulette(false, -1, 0);
				cltMsg clMsg(GAMEMSG_REQUEST_BALANCECURRENCY_BUFF, sizeof(cltGameMsgRequest_BalanceCurrency_Buff), (BYTE*)&Roulette);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				
				DeleteDialog();
			}
			break;	
		case BUFF_ROULETTE_BUTTON_START:
			{
				cltClient*  pclclient = (cltClient *)pclClient;
				//[진성] 를렛안의 모든 버프를 사용 후 버프룰렛을 사용하면 클라가 죽는거 수정. => 2008-9-2 
				if( 0 >= m_SetBuffNum ) return;
				if( pclClient->IsWhereServiceArea(ConstServiceArea_Korea) )
				{
					if( BUFF_BALANCECURRENCYLIMIT_NUM > m_SetBuffNum ) 
					{						
						pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(GetTxtFromMgr(5614), GetTxtFromMgr(8825));	
						DeleteDialog();
						return;
					}
				}

				// 룰렛을 여러번 돌리지 못하게.
				if(true == pclClient->pclCM->CR[1]->bBuffRouletteSwitch)
					return;

				// 아직 룰렛이 돌고 있다. - 골치아프다 걍 만들자.
				if(true == m_bRouletteButtunOnly)
					return;

				if(pclclient->m_pclBalanceCurrencyMgr->GetCanUseMoney() < pclclient->m_pclBalanceCurrencyMgr->GetBuffCost())
				{
					TCHAR *pTitle = GetTxtFromMgr(5614);
					TCHAR *pText = GetTxtFromMgr(10336);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

					DeleteDialog();

					return;
				}

				// 비용 확인.
				if(0 < GetBuffCost())
				{
					// 소지금 부족.
					if(GetBuffCost() > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney())
					{
						CHAR*	pTitle	= GetTxtFromMgr(5614);
						TCHAR*	pText	= GetTxtFromMgr(5870);

						cltClient*  pclclient = (cltClient *)pclClient;
						pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pText);
						return;
					}

					SI32 TextNum = 0;

					if(-1 < pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.FindBufGetTypeSlot(BUF_GET_TYPE_BUFROULETTE))
					{
						TextNum = 8456;
					}
					else
					{
						TextNum = 8455;
					}


					NTCHARString256 kbuf(GetTxtFromMgr(TextNum));
					kbuf.Replace("#money#", SI64ToString(GetBuffCost()));

					stMsgBox MsgBox;
					MsgBox.Set( this, GetTxtFromMgr(8457), kbuf, MSGBOX_TYPE_YESNO, 0 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
				// 비용을 얻지 못해 리턴.
				else
				{
					return;
				}
			}
			break;	
	}
}

void CNBalanceCurrencyDlg::Action()
{
	if(true == m_bRouletteAction)
	{
		if(340 < m_dwTickCount && false == m_bRouletteComplete)
		{  
			m_bRouletteComplete = true;
			
			GMONEY Money = GetBuffCost();
			if(0 >= Money)
			{
				return;
			}

			// 버프룰렛이 멈추면 해당 버프를 날림.
			cltGameMsgRequest_BalanceCurrency_Buff Roulette(false, m_CompleteBuffKind, -Money);
			cltMsg clMsg(GAMEMSG_REQUEST_BALANCECURRENCY_BUFF, sizeof(cltGameMsgRequest_BalanceCurrency_Buff), (BYTE*)&Roulette);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}

		// 룰렛 종료. (노가다 타이밍 ㅋㅋ)
		if(m_CompleteBuffKind == m_SetBuffKindArray[m_CurBuffNum] && true == m_bRouletteComplete)
		{
			m_bRouletteAction   = false;
			m_dwTickCount	    = 0;	
			m_PrevTickCount	    = 0;   
			m_DelayTickCount    = 0;
			
			// 버프 툴팁 설명.
			cltClient *pclclient = (cltClient *)pclClient;
			if(NULL != pclclient->m_pDialog[ NBUFFSTATUS_DLG ])
			{
				const TCHAR* pBuffToolTip = ((CNBuffstatus*)pclclient->m_pDialog[ NBUFFSTATUS_DLG ])->m_BuffStatic[pclclient->pclGImg->GetBufImageFontIndex(m_CompleteBuffKind)]->GetToolTipMsg();
				
				if(NULL != pBuffToolTip)
				{
					m_InterfaceFactory.GetEdit(BUFF_ROULETTE_EDIT_IMFORMATION)->SetText((TCHAR*)pBuffToolTip);
				}

				// 블렛 멈출때까지는 버튼 안되도록.
				// pclClient->pclCM->CR[1]->bBuffRouletteSwitch  =false;
				m_bRouletteButtunOnly = false;
			}
			  
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GACHA_ROLLOVER_BOX")), 0, 0);

			return;
		}

		// 초당 조금씩 느려지게..
		if((GetTickCount() - m_DelayTickCount) > 300 && false == m_bRouletteComplete)
		{
			m_DelayTickCount = GetTickCount();

			m_dwTickCount += rand() % 30;   
		}

		//룰렛 돌리기.
		if((GetTickCount() - m_PrevTickCount) > m_dwTickCount)
		{
			m_bRouletteButtunOnly = true;

			m_PrevTickCount = GetTickCount();

			ChangeBuff();	
		}

		
		if(100 > m_dwTickCount)
			m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_EDIT_IMFORMATION)->SetText(GetTxtFromMgr(8550));
		else if(200 > m_dwTickCount)
			m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_EDIT_IMFORMATION)->SetText(GetTxtFromMgr(8551));
		else if(300 > m_dwTickCount)
			m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_EDIT_IMFORMATION)->SetText(GetTxtFromMgr(8552));
		else if(360 > m_dwTickCount)
			m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_EDIT_IMFORMATION)->SetText(GetTxtFromMgr(8553));
	} 
}

void CNBalanceCurrencyDlg::StartRoulette(void)
{
	m_bRouletteAction   = true;
	m_bRouletteComplete = false;

	SetBuff();
}

void CNBalanceCurrencyDlg::ChangeBuff(void)
{
	cltClient* pclclient = (cltClient*)pclClient;
	if( pclclient == NULL )			return;

	--m_CurBuffNum;
	--m_PrevBuffNum;
	--m_NextBuffNum;

	if(0 > m_CurBuffNum)
	{	
		m_CurBuffNum = m_SetBuffNum-1;
	}

	if(0 > m_PrevBuffNum)
	{	
		m_PrevBuffNum = m_SetBuffNum-1;	
	}  

	if(0 > m_NextBuffNum)
	{	
		m_NextBuffNum = m_SetBuffNum-1;
	}

	m_pBuffImage[BUFF_BALANCECURRENCY_PREV]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_PrevBuffNum]) );
	m_pBuffImage[BUFF_BALANCECURRENCY_CUR]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_CurBuffNum]) );
	m_pBuffImage[BUFF_BALANCECURRENCY_NEXT]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_NextBuffNum]) );

	pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_STATUE_BUFF")), 0, 0);
}

GMONEY CNBalanceCurrencyDlg::GetBuffCost()
{
	SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

	GMONEY	siPrice = 0;

	if		(npcrate < 100)		siPrice	=	150000;
	else if	(npcrate < 300)		siPrice	=	300000;
	else if	(npcrate < 700)		siPrice	=	500000;
	else if	(npcrate < 1000)	siPrice	=	1000000;
	else if	(npcrate < 1450)	siPrice	=	1800000;
	else if	(npcrate < 1750)	siPrice	=	2900000;
	else if	(npcrate < 2050)	siPrice	=	4200000;
	else if	(npcrate < 2350)	siPrice	=	5800000;
	else if	(npcrate < 3000)	siPrice	=	7800000;
	else if (npcrate < 4000)	siPrice	=	10000000;
	else if (npcrate < 5500)	siPrice	=	13000000;
	else if (npcrate < 8500)	siPrice	=	18000000;
	else if (npcrate < 14500)	siPrice	=	25000000;
	else if (npcrate < 25500)	siPrice	=	40000000;
	else if (npcrate < 35500)	siPrice	=	58000000;
	else						siPrice =	85000000;

	return siPrice;
}

void CNBalanceCurrencyDlg::SetBuff(void)
{
	cltClient* pclclient = (cltClient*)pclClient;

	ZeroMemory(m_SetBuffKindArray, sizeof(m_SetBuffKindArray));
	
	m_SetBuffNum = 0; 
	for(SI32 Count = 0; Count < BUF_END; ++Count)	
	{
		if(0 >= m_BuffKindArray[Count])
			break;

		// 이미 사용중인 버프는 뺀다.
		if(true == pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(m_BuffKindArray[Count]))
			continue;

		m_SetBuffKindArray[m_SetBuffNum++] = m_BuffKindArray[Count];
	}
	
	//[진성] 를렛안의 모든 버프를 사용 후 버프룰렛을 사용하면 클라가 죽는거 수정. => 2008-9-2 
	if( 0 >= m_SetBuffNum ) return;
	if( pclClient->IsWhereServiceArea(ConstServiceArea_Korea) )
	{
		if( BUFF_BALANCECURRENCYLIMIT_NUM > m_SetBuffNum ) 
		{
			cltClient*  pclclient = (cltClient *)pclClient;
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(GetTxtFromMgr(5614), GetTxtFromMgr(8825));	
			DeleteDialog();
			return;
		}
	}

	// 버프 이미지 인덱스 번호.
	m_CurBuffNum  = rand()%m_SetBuffNum;

	m_PrevBuffNum = m_CurBuffNum - 1;    
	if(0 > m_PrevBuffNum)
		m_PrevBuffNum = m_SetBuffNum - 1;

	m_NextBuffNum = m_CurBuffNum + 1;
	if(m_SetBuffNum <= m_NextBuffNum)
		m_NextBuffNum = 0;		

	m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_EDIT_IMFORMATION)->SetText( GetTxtFromMgr(8550) );
}
void CNBalanceCurrencyDlg::SetCompleteBuff(bool _Action, SI32 _CompleteBuffKind)
{
	if(true == _Action)
	{
		m_CompleteBuffKind = 0;
		StartRoulette();
	}
	else
	{
		m_CompleteBuffKind = _CompleteBuffKind;
	}
}

bool CNBalanceCurrencyDlg::GetRouletteAction(void)
{
	return m_bRouletteAction;
}

void CNBalanceCurrencyDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	
}

void CNBalanceCurrencyDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:
		{
			if(RetValue)
			{
				// 비용 확인.
				if(0 < GetBuffCost())
				{
					// 소지금 부족.
					if(GetBuffCost() > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney())
					{
						CHAR*	pTitle	= GetTxtFromMgr(5614);
						TCHAR*	pText	= GetTxtFromMgr(5870);
						
						cltClient*  pclclient = (cltClient *)pclClient;
						pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pText);
						return;
					}
				}
  
				cltGameMsgRequest_BalanceCurrency_Buff Roulette(true);
				cltMsg clMsg(GAMEMSG_REQUEST_BALANCECURRENCY_BUFF, sizeof(cltGameMsgRequest_BalanceCurrency_Buff), (BYTE*)&Roulette);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}
