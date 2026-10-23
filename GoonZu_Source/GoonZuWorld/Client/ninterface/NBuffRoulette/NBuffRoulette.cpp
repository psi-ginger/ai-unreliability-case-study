
#include "NBuffRoulette.h"
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


CNBuffRoulette::CNBuffRoulette()
{
	cltClient* pclclient = (cltClient*)pclClient;
	ZeroMemory(m_BuffKindArray, sizeof(m_BuffKindArray));

	if(pclClient->pclEventTimeManager->InEventTime("ChristmasEvent", &pclclient->sTime) )
	{
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

		m_BuffKindArray[12] = ITEMUNIQUE(13002);		// 크리스마스 양말. - 버프공간이지만 그냥 쓰자. - 버프맥스 안넘어가게 주의 

		m_sItemImageiFont = 0;
		m_siGimgIndex = 0;

		SI16 Ref = pclClient->pclItemManager->FindItemRefFromUnique( m_BuffKindArray[12] );

		// m_sItemImageiFont  설정.
		if(Ref >= 0)
		{
			pclClient->pclItemManager->GetItemGImageFileIndexAndFont(m_BuffKindArray[12], &m_siGimgIndex, &m_sItemImageiFont );
		}
	}
	else
	{
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
	}

	m_SetBuffNum = 0;
	m_bRouletteButtunOnly = false;
}

CNBuffRoulette::~CNBuffRoulette()
{
	
}

void CNBuffRoulette::SetControl()
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
		if( BUFF_LIMIT_NUM > m_SetBuffNum ) 
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

	m_pBuffImage[BUFF_PREV] =	m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_IMAGESTATIC_1);
	m_pBuffImage[BUFF_CUR]  =	m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_IMAGESTATIC_2);
	m_pBuffImage[BUFF_NEXT] =	m_InterfaceFactory.GetImageStatic(BUFF_ROULETTE_IMAGESTATIC_3);

	//KHY - 1113 - 크리스마스 이벤트 - 버프와 함께, 아이템이 들어간다.
	if(pclClient->pclEventTimeManager->InEventTime("ChristmasEvent", &pclclient->sTime) )
	{
		// 12번은 아이템이다. - 크리스 마스 양말.	
		if(m_SetBuffKindArray[m_PrevBuffNum] != m_BuffKindArray[12])
		{
			m_pBuffImage[BUFF_PREV]->SetFileName("gimg/BufImage.SPR");
			m_pBuffImage[BUFF_PREV]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_PrevBuffNum]) );
		}
		else
		{
			m_pBuffImage[BUFF_PREV]->SetItemSprType( m_siGimgIndex );
			m_pBuffImage[BUFF_PREV]->SetFontIndex(m_sItemImageiFont);
		}

		if(m_SetBuffKindArray[m_CurBuffNum] != m_BuffKindArray[12])
		{
			m_pBuffImage[BUFF_CUR]->SetFileName("gimg/BufImage.SPR");
			m_pBuffImage[BUFF_CUR]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_CurBuffNum]) );
		}
		else
		{
			m_pBuffImage[BUFF_CUR]->SetItemSprType( m_siGimgIndex );
			m_pBuffImage[BUFF_CUR]->SetFontIndex(m_sItemImageiFont);
		}

		if(m_SetBuffKindArray[m_NextBuffNum] != m_BuffKindArray[12])
		{
			m_pBuffImage[BUFF_NEXT]->SetFileName("gimg/BufImage.SPR");
			m_pBuffImage[BUFF_NEXT]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_NextBuffNum]) );
		}
		else
		{
			m_pBuffImage[BUFF_NEXT]->SetItemSprType( m_siGimgIndex );
			m_pBuffImage[BUFF_NEXT]->SetFontIndex(m_sItemImageiFont);
		}
	}
	else
	{
		m_pBuffImage[BUFF_PREV]->SetFileName("gimg/BufImage.SPR");
		m_pBuffImage[BUFF_PREV]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_PrevBuffNum]) );

		m_pBuffImage[BUFF_CUR]->SetFileName("gimg/BufImage.SPR");
		m_pBuffImage[BUFF_CUR]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_CurBuffNum]) );

		m_pBuffImage[BUFF_NEXT]->SetFileName("gimg/BufImage.SPR");
		m_pBuffImage[BUFF_NEXT]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_NextBuffNum]) );		
	}		
}

void CNBuffRoulette::Destroy()
{
	// 버프룰렛이 멈추면 해당 버프를 날림.
	cltGameMsgRequest_Buff_Roulette Roulette(false, -1, 0);
	cltMsg clMsg(GAMEMSG_REQUEST_BUFF_ROULETTE, sizeof(cltGameMsgRequest_Buff_Roulette), (BYTE*)&Roulette);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CNBuffRoulette::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NBuffRoulette/NBuffRoulette.ddf" ) );
	file.CreatePopupDialog( this, NBUFF_ROULETTE_DLG, TEXT( "dialog_buff_roulette" ), StaticNBuffRouletteProc );
	
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

void CALLBACK CNBuffRoulette::StaticNBuffRouletteProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNBuffRoulette *pThis = (CNBuffRoulette *)pControllerMgr;
	pThis->NBuffRouletteDlgProc(nEvent,nControlID,pControl);
	return;
}

void CALLBACK CNBuffRoulette::NBuffRouletteDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
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
				cltGameMsgRequest_Buff_Roulette Roulette(false, -1, 0);
				cltMsg clMsg(GAMEMSG_REQUEST_BUFF_ROULETTE, sizeof(cltGameMsgRequest_Buff_Roulette), (BYTE*)&Roulette);
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
					if( BUFF_LIMIT_NUM > m_SetBuffNum ) 
					{						
						pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(GetTxtFromMgr(5614), GetTxtFromMgr(8825));	
						DeleteDialog();
						return;
					}
				}

				// 크리스마스 아이템을 받을 공간을 확보한다.
				cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
				if(pclchar->IsBlankInv( 1, INVMODE_ALL ) == false )
				{
					NTCHARString128	kTitle(GetTxtFromMgr(7660));
					NTCHARString128 kText(GetTxtFromMgr(8886));

					kText.Replace( "#count#", SI32ToString((SI32)1) );
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
					pclClient->pclCM->CR[1]->bBuffRouletteSwitch = false ;
					return;
				}

				// 룰렛을 여러번 돌리지 못하게.
				if(true == pclClient->pclCM->CR[1]->bBuffRouletteSwitch)
					return;

				// 아직 룰렛이 돌고 있다. - 골치아프다 걍 만들자.
				if(true == m_bRouletteButtunOnly)
					return;

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

					//KHY - 1113 - 크리스마스 이벤트 - 버프와 함께, 아이템이 들어간다.
					if(pclClient->pclEventTimeManager->InEventTime("ChristmasEvent", &pclclient->sTime) )
					{
						SI32 TextNum = 0;
						SI08 HomeVillage = pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage;
						SI08 CurVillage  = pclClient->pclCM->CR[1]->GetCurrentVillageUnique();

						if(HomeVillage == CurVillage)
						{
							TextNum = 8928;
							NTCHARString256 kbuf(GetTxtFromMgr(TextNum));

							kbuf.Replace("#money#", SI64ToString(GetBuffCost()+ ((GetBuffCost()*3)/7)));
							kbuf.Replace("#dmoney#", SI64ToString(GetBuffCost()));

							stMsgBox MsgBox;
							MsgBox.Set( this, GetTxtFromMgr(8457), kbuf, MSGBOX_TYPE_YESNO, 0 );
							pclclient->SetMsgBox( &MsgBox, NULL, 0 );

						}
						else
						{
							TextNum = 8927;
							NTCHARString256 kbuf(GetTxtFromMgr(TextNum));

							kbuf.Replace("#money#", SI64ToString(GetBuffCost()));

							stMsgBox MsgBox;
							MsgBox.Set( this, GetTxtFromMgr(8457), kbuf, MSGBOX_TYPE_YESNO, 0 );
							pclclient->SetMsgBox( &MsgBox, NULL, 0 );

						}
					}
					else
					{
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

void CNBuffRoulette::Action()
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
			cltGameMsgRequest_Buff_Roulette Roulette(false, m_CompleteBuffKind, -Money);
			cltMsg clMsg(GAMEMSG_REQUEST_BUFF_ROULETTE, sizeof(cltGameMsgRequest_Buff_Roulette), (BYTE*)&Roulette);
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
				if(pclClient->pclEventTimeManager->InEventTime("ChristmasEvent", &pclclient->sTime) )
				{
					if(m_CompleteBuffKind == m_BuffKindArray[12])
					{
						const TCHAR* szPersonName = pclclient->pclCM->CR[1]->GetName();
						const TCHAR* stritemName = pclClient->pclItemManager->GetName(m_BuffKindArray[12]);

						NTCHARString128 strText( GetTxtFromMgr(7684) );
						strText.Replace( TEXT("#user#"), szPersonName );
						strText.Replace( TEXT("#item#"), stritemName );

						m_InterfaceFactory.GetEdit(BUFF_ROULETTE_EDIT_IMFORMATION)->SetText(strText);
					}
					else
					{
						const TCHAR* pBuffToolTip = ((CNBuffstatus*)pclclient->m_pDialog[ NBUFFSTATUS_DLG ])->m_BuffStatic[pclclient->pclGImg->GetBufImageFontIndex(m_CompleteBuffKind)]->GetToolTipMsg();
						if(NULL != pBuffToolTip)
						{
							m_InterfaceFactory.GetEdit(BUFF_ROULETTE_EDIT_IMFORMATION)->SetText((TCHAR*)pBuffToolTip);
						}
					}
				}
				else
				{
					const TCHAR* pBuffToolTip = ((CNBuffstatus*)pclclient->m_pDialog[ NBUFFSTATUS_DLG ])->m_BuffStatic[pclclient->pclGImg->GetBufImageFontIndex(m_CompleteBuffKind)]->GetToolTipMsg();
					if(NULL != pBuffToolTip)
					{
						m_InterfaceFactory.GetEdit(BUFF_ROULETTE_EDIT_IMFORMATION)->SetText((TCHAR*)pBuffToolTip);
					}
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

void CNBuffRoulette::StartRoulette(void)
{
	m_bRouletteAction   = true;
	m_bRouletteComplete = false;

	SetBuff();
}

void CNBuffRoulette::ChangeBuff(void)
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
	//m_pBuffImage[BUFF_PREV]->SetFontIndex( m_SetBuffKindArray[m_PrevBuffNum] - 1);
	//m_pBuffImage[BUFF_CUR]->SetFontIndex(  m_SetBuffKindArray[m_CurBuffNum]  - 1 );
	//m_pBuffImage[BUFF_NEXT]->SetFontIndex( m_SetBuffKindArray[m_NextBuffNum] - 1 );

	if(pclClient->pclEventTimeManager->InEventTime("ChristmasEvent", &pclclient->sTime) )
	{
		if(m_SetBuffKindArray[m_PrevBuffNum] != m_BuffKindArray[12])
		{
			m_pBuffImage[BUFF_PREV]->SetFileName("gimg/BufImage.SPR");
			m_pBuffImage[BUFF_PREV]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_PrevBuffNum]) );
		}
		else
		{
			m_pBuffImage[BUFF_PREV]->SetItemSprType( m_siGimgIndex );
			m_pBuffImage[BUFF_PREV]->SetFontIndex(m_sItemImageiFont);
		}

		if(m_SetBuffKindArray[m_CurBuffNum] != m_BuffKindArray[12])
		{
			m_pBuffImage[BUFF_CUR]->SetFileName("gimg/BufImage.SPR");
			m_pBuffImage[BUFF_CUR]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_CurBuffNum]) );
		}
		else
		{
			m_pBuffImage[BUFF_CUR]->SetItemSprType( m_siGimgIndex );
			m_pBuffImage[BUFF_CUR]->SetFontIndex(m_sItemImageiFont);		
		}

		if(m_SetBuffKindArray[m_NextBuffNum] != m_BuffKindArray[12])
		{
			m_pBuffImage[BUFF_NEXT]->SetFileName("gimg/BufImage.SPR");
			m_pBuffImage[BUFF_NEXT]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_NextBuffNum]) );
		}
		else
		{
			m_pBuffImage[BUFF_NEXT]->SetItemSprType( m_siGimgIndex );
			m_pBuffImage[BUFF_NEXT]->SetFontIndex(m_sItemImageiFont);		
		}
	}
	else
	{
		m_pBuffImage[BUFF_PREV]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_PrevBuffNum]) );
		m_pBuffImage[BUFF_CUR]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_CurBuffNum]) );
		m_pBuffImage[BUFF_NEXT]->SetFontIndex( pclclient->pclGImg->GetBufImageFontIndex(m_SetBuffKindArray[m_NextBuffNum]) );
	}

	pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_STATUE_BUFF")), 0, 0);
}

GMONEY CNBuffRoulette::GetBuffCost(void)
{
	SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

	GMONEY	siPrice = 0;

	// 수정 - 군주S의 경우에는 npcrate에 상관없이 같은가격으로 통일한다.
	if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
	{
		siPrice = 1000000;
	}
	else
	{
		if (npcrate < 100)		 siPrice =  150000;
		else if (npcrate < 300)  siPrice =	300000;
		else if (npcrate < 700)  siPrice =  500000;
		else if (npcrate < 1000) siPrice = 1000000;
		else if (npcrate < 1450) siPrice = 1800000;
		else if (npcrate < 1750) siPrice = 2900000;
		else if (npcrate < 2050) siPrice = 4200000;
		else if (npcrate < 2350) siPrice = 5800000;
		else if (npcrate < 3000) siPrice = 7800000;
		// KHY - 1021 - NPCRate 값 확대.
		else if (npcrate < 4000) siPrice = 10000000;
		else if (npcrate < 5500) siPrice = 13000000;
		else if (npcrate < 8500) siPrice = 18000000;
		else if (npcrate < 14500) siPrice = 25000000;
		else if (npcrate < 25500) siPrice = 40000000;
		else if (npcrate < 35500) siPrice = 58000000;
		else
			siPrice = 85000000;
	}

	// 마을을 검사해서 내의 마을 이면 30% 활인된 가격으로 버프룰렛을 사용한다.
	SI08 HomeVillage = pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage;
	SI08 CurVillage  = pclClient->pclCM->CR[1]->GetCurrentVillageUnique();
	if(HomeVillage == CurVillage)
	{
		siPrice = siPrice -((siPrice * 30)/100);
	}

	return siPrice;
}

void CNBuffRoulette::SetBuff(void)
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
		if( BUFF_LIMIT_NUM > m_SetBuffNum ) 
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
void CNBuffRoulette::SetCompleteBuff(bool _Action, SI32 _CompleteBuffKind)
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

bool CNBuffRoulette::GetRouletteAction(void)
{
	return m_bRouletteAction;
}

void CNBuffRoulette::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	
}

void CNBuffRoulette::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
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
  
				cltGameMsgRequest_Buff_Roulette Roulette(true);
				cltMsg clMsg(GAMEMSG_REQUEST_BUFF_ROULETTE, sizeof(cltGameMsgRequest_Buff_Roulette), (BYTE*)&Roulette);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}
