// 변신룰렛
#include "NCharRoulette.h"

#include "../Graphic/Graphic.h"
#include "../Graphic/Graphic_Capsule.h"

#include "../../Client.h"

#include "../../CommonLogic/CommonLogic.h"
#include "../CommonLogic/MsgType-Person.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../GlobalImgFile/GlobalImgFile.h"

#include "../../../common/CommonHeader.h"
#include "../../../common/Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;


CCharBufSurfaceObj::CCharBufSurfaceObj()
 : m_lpSurface(NULL),
   m_siWidth(0),
   m_siHeight(0)   
{
}

CCharBufSurfaceObj::~CCharBufSurfaceObj()
{
	if( m_lpSurface )
	{
		m_lpSurface->Release();
		m_lpSurface = NULL;
	}
}

void CCharBufSurfaceObj::Create( SI16 SurfaceWidth, SI16 SurfaceHeight )
{
	if( g_graphics.CreateSurface16( &m_lpSurface, SurfaceWidth, SurfaceHeight, TRUE, RGB16_TRANS ) == FALSE )
	{
		MsgBox( TEXT("CreateOffScreen Error"), TEXT("CControllerSurfaceObj::Create") );
		return;
	}

	m_siWidth = SurfaceWidth;
	m_siHeight = SurfaceHeight;
}

CNCharRouletteDlg::CNCharRouletteDlg()
: m_siCharBufCount(0),
  m_pCharBufSurfaceObj(NULL),
  m_ImgCharBufBuffer(NULL),
  m_bCreateImgBuffer(false),
  m_siResultCharKind(0),
  m_siSpeed(0),
  m_siStartClock(0),
  m_siCurIndex(0),
  m_siTotalCount(0),
  m_siCurCount(0),
  m_siStartY(0),
  m_siNeedPrice(0),
  b_bUseTicketItem(false)
{
	ZeroMemory(m_siCharKind, sizeof(m_siCharKind));
}

CNCharRouletteDlg::~CNCharRouletteDlg()
{
	Destroy();

	if(m_pCharBufSurfaceObj)
	{
		delete m_pCharBufSurfaceObj;
		m_pCharBufSurfaceObj = NULL;
	}

	if(m_ImgCharBufBuffer)
	{
		delete[] m_ImgCharBufBuffer;
		m_ImgCharBufBuffer = NULL;
	}
}

void CNCharRouletteDlg::Destroy()
{
	// 룰렛이 동작중이라면, 취소한다.
	if( m_siResultCharKind > 0 )
	{
		Init();

		if(pclClient)
		{
			cltGameMsgRequest_Char_Roulette clCharRoulette( CHAR_ROULETTE_END, 0,false);
			cltMsg clMsg(GAMEMSG_REQUEST_CHAR_ROULETTE, sizeof(clCharRoulette), (BYTE*)&clCharRoulette);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}
}

void CNCharRouletteDlg::Init()
{
	m_siCharBufCount	= 0;
	m_siResultCharKind	= 0;
	m_siSpeed			= 0;
	m_siCurIndex		= 0;
	m_siTotalCount		= 0;
	m_siCurCount		= 0;
	m_siNeedPrice		= 0;
	m_siStartY			= 0;
	b_bUseTicketItem = false;
	
	ZeroMemory(m_siCharKind, sizeof(m_siCharKind));

	m_siCharBufCount	= 3;
	m_siCharKind[0]		= pclClient->GetUniqueFromHash("KIND_RACCOON");
	m_siCharKind[1]		= pclClient->GetUniqueFromHash("KIND_RABBIT");
	m_siCharKind[2]		= pclClient->GetUniqueFromHash("KIND_MUSHROOM");
}

void CNCharRouletteDlg::Create()
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);

	if ( IsCreate() )
	{
		Destroy();
		DeleteDialog();
		return;
	}
	else
	{
		CInterfaceFile file;

		file.LoadFile( TEXT( "NInterface/Data/NCharRoulette/NCharRoulette.ddf" ) );
		file.CreatePopupDialog( this, NCHARBUFF_ROULETTE_DLG, TEXT( "dialog_char_roulette" ), StaticNCharfRouletteProc );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,			BUFF_ROULETTE_EDIT_EXPLAIN,		this ), BUFF_ROULETTE_EDIT_EXPLAIN,			TEXT( "editbox_explain" ) );	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,			BUFF_ROULETTE_EDIT_IMFORMATION,	this ), BUFF_ROULETTE_EDIT_IMFORMATION,		TEXT( "editbox_information" ) );	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,			BUFF_ROULETTE_BUTTON_CLOSE,		this ), BUFF_ROULETTE_BUTTON_CLOSE,			TEXT( "button_close" ) );	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,			BUFF_ROULETTE_BUTTON_START,		this ), BUFF_ROULETTE_BUTTON_START,			TEXT( "button_roulette_start" ) );	

		m_pCharBufSurfaceObj = new CCharBufSurfaceObj();
		if(m_pCharBufSurfaceObj)		m_pCharBufSurfaceObj->Create(60, 60 * 4);

		m_ImgCharBufBuffer = new RGB16[ 40 * 40 * 4 * 4 ];

		SetActionTimer( 50 );    
		SetTimerOnOff( true );

		CEdit* pclEdit_Info = m_InterfaceFactory.GetEdit(BUFF_ROULETTE_EDIT_EXPLAIN);
		if(pclEdit_Info)
		{
			NTCHARString256 kText(GetTxtFromMgr(8870));
			m_siNeedPrice = 0;

			// 사용요금 계산
			SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

			// [춘기] 일본의 경우 룰렛돌리는 금액을 다르게 설정.
			if(pclClient->IsWhereServiceArea(ConstServiceArea_Japan))
			{
				// 사용요금 계산
				if		(npcrate < 100)		{		m_siNeedPrice = 15000;		}
				else if	(npcrate < 300)		{		m_siNeedPrice = 30000;		}
				else if	(npcrate < 700)		{		m_siNeedPrice = 50000;		}
				else if	(npcrate < 1000)	{		m_siNeedPrice = 100000;	}
				else if	(npcrate < 1450)	{		m_siNeedPrice = 180000;	}
				else if	(npcrate < 1750)	{		m_siNeedPrice = 290000;	}
				else if	(npcrate < 2050)	{		m_siNeedPrice = 420000;	}
				else if	(npcrate < 2350)	{		m_siNeedPrice = 580000;	}
				else if	(npcrate < 3000)	{		m_siNeedPrice = 780000;	}

				// KHY - 1021 - NPCRate 값 확대.
				else if	(npcrate < 4000)	{		m_siNeedPrice = 1000000;	}
				else if	(npcrate < 5500)	{		m_siNeedPrice = 1300000;	}
				else if	(npcrate < 8500)	{		m_siNeedPrice = 1800000;	}
				else if	(npcrate < 14500)	{		m_siNeedPrice = 2500000;	}
				else if	(npcrate < 25500)	{		m_siNeedPrice = 4000000;	}
				else if	(npcrate < 33500)	{		m_siNeedPrice = 5800000;	}
				else						{		m_siNeedPrice = 8500000;	}
			}
			else
			{
				// 사용요금 계산
				if		(npcrate < 100)		{		m_siNeedPrice = 150000;		}
				else if	(npcrate < 300)		{		m_siNeedPrice = 300000;		}
				else if	(npcrate < 700)		{		m_siNeedPrice = 500000;		}
				else if	(npcrate < 1000)	{		m_siNeedPrice = 1000000;	}
				else if	(npcrate < 1450)	{		m_siNeedPrice = 1800000;	}
				else if	(npcrate < 1750)	{		m_siNeedPrice = 2900000;	}
				else if	(npcrate < 2050)	{		m_siNeedPrice = 4200000;	}
				else if	(npcrate < 2350)	{		m_siNeedPrice = 5800000;	}
				else if	(npcrate < 3000)	{		m_siNeedPrice = 7800000;	}

				// KHY - 1021 - NPCRate 값 확대.
				else if	(npcrate < 4000)	{		m_siNeedPrice = 10000000;	}
				else if	(npcrate < 5500)	{		m_siNeedPrice = 13000000;	}
				else if	(npcrate < 8500)	{		m_siNeedPrice = 18000000;	}
				else if	(npcrate < 14500)	{		m_siNeedPrice = 25000000;	}
				else if	(npcrate < 25500)	{		m_siNeedPrice = 40000000;	}
				else if	(npcrate < 33500)	{		m_siNeedPrice = 58000000;	}
				else						{		m_siNeedPrice = 85000000;	}
			}

			// 아무 마을이나 가입되어 있으면, 정상가격
			if( pclchar->pclCI->clBI.siHomeVillage > 0 )
			{
				// 룰렛을 돌리고 있는 마을과 가입된 마을이 같다면, 0.7배 가격
				if( pclchar->GetMapIndex() == pclchar->pclCI->clBI.siHomeVillage)
				{
					m_siNeedPrice *= 0.7;
				}
			}
			else
			{
				// 마을에 가입조차 되어있지 않으면 1.5배 가격
				m_siNeedPrice *= 1.5;
			}
			
			TCHAR szBuf[64] = { '\0', };
			g_JYLibFunction.SetNumUnit(m_siNeedPrice, szBuf, sizeof(szBuf));
			kText.Replace("#transmoney#", szBuf);
			pclEdit_Info->SetText( kText );
		}

		Init();

		// 초기화면 그리기
		MakeBufRouletteImage();
		GP.LoadSprFromFile("NINTERFACE/IMAGE/Artifact_Slot_Image_40x40_005.SPR", &m_SprSelect);
	}


}

bool CNCharRouletteDlg::Start()
{
	if(m_siCharBufCount < 0)		return false;
	if(m_siResultCharKind < 0)		return false;

	SI32 siResultIndex = 0;

	for(SI32 i=0; i<m_siCharBufCount; i++)
	{
		if(m_siCharKind[i] == m_siResultCharKind)
		{
			siResultIndex = i;
			break;
		}
	}

	m_siSpeed		= 20;
	m_siCurIndex	= 0;
	m_siTotalCount	= (m_siCharBufCount * 2) + siResultIndex;
	m_siCurCount	= 1;
	m_siStartY		= 0;

    return true;
}

bool CNCharRouletteDlg::End()
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
	if(pclchar == NULL)									return false;

	cltClient*  pclclient = (cltClient *)pclClient;

	if( m_siResultCharKind > 0 )
	{
		bool bSuccess = true;
		bool bHaveTicket = false;

		// 소지금 확인
		// PCK : 룰렛을 돌릴 갯수만큼 촛대가 있으면 먼저 촛대부터 사용한다.
		if(b_bUseTicketItem)
		{
			// 실제 촛대가 있는지, 있다면 100개 이상 가지고 있는지 체크

			for(SI32 i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
			{
				if (pclchar->pclCI->clCharItem.clItem[i].siUnique == ITEMUNIQUE(13049)
					&& pclchar->pclCI->clCharItem.clItem[i].siItemNum >= 100)
				{	
					bHaveTicket = true;
					break;
				}					
			}

			if(bHaveTicket == false)
			{
				Init();
				return true;
			}					
		}
		else
		{
			if( m_siNeedPrice <= 0 || pclchar->pclCI->clIP.GetMoney() < m_siNeedPrice )
			{
				pclClient->pclCM->CR[1]->bBuffRouletteSwitch = false ;
				bSuccess = false;

				NTCHARString128	kTitle(GetTxtFromMgr(8869));
				NTCHARString128 kText(GetTxtFromMgr(5870));

				cltClient*  pclclient = (cltClient *)pclClient;
				pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(kTitle, kText);

			}		
		}

		if( pclchar->IsBlankInv( 1, INVMODE_ALL ) == false )
		{
			NTCHARString128	kTitle(GetTxtFromMgr(8869));
			NTCHARString128 kText(GetTxtFromMgr(8886));

			kText.Replace( "#count#", SI32ToString((SI32)1) );
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
			pclClient->pclCM->CR[1]->bBuffRouletteSwitch = false ;

			bSuccess = false;
		}

		
		if( bSuccess )
		{
			cltGameMsgRequest_Char_Roulette clCharRoulette( CHAR_ROULETTE_ADDBUF, m_siResultCharKind ,b_bUseTicketItem);
			cltMsg clMsg(GAMEMSG_REQUEST_CHAR_ROULETTE, sizeof(clCharRoulette), (BYTE*)&clCharRoulette);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

			NTCHARString128	kText(GetTxtFromMgr(8881));
			if(m_siResultCharKind < 10000)
			{
				NTCHARString64 kName;
				kName = pclClient->pclKindInfoSet->GetName(m_siResultCharKind);
				kText.Replace("#monname#", kName);

				CEdit* pclEdit_Info = m_InterfaceFactory.GetEdit(BUFF_ROULETTE_EDIT_IMFORMATION);
				if(pclEdit_Info)
				{
					pclEdit_Info->SetText( kText );
				}
			}
		}

		Init();
	}

	return true;
}

void CALLBACK CNCharRouletteDlg::StaticNCharfRouletteProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNCharRouletteDlg *pThis = (CNCharRouletteDlg *)pControllerMgr;
	pThis->NCharfRouletteDlgProc(nEvent, nControlID, pControl);
	return;
}

void CALLBACK CNCharRouletteDlg::NCharfRouletteDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient*  pclclient = (cltClient *)pclClient;
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
	case BUFF_ROULETTE_BUTTON_CLOSE:
		{
			pclClient->pclCM->CR[1]->bBuffRouletteSwitch = false ;
			Destroy();
			DeleteDialog();
		}
		break;	
	case BUFF_ROULETTE_BUTTON_START:
		{
			// 룰렛이 동작중이라면 명령은 취소된다.
			if( m_siResultCharKind > 0 )									return;

			SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
			//cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
			if(pclchar == NULL)												return;

			// PCK - 가차 스피릿 변신 상태에서도 룰렛 사용가능하게 함 (09.08.07)
			//if( pclchar->clTransFormInfo.bTransFormMode == true )
			//{
			//	NTCHARString128	kTitle(GetTxtFromMgr(8869));
			//	NTCHARString128 kText(GetTxtFromMgr(8872));

			//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
			//	return;
			//}

			// 망토 아이템을 받을 공간을 확보한다.
			if( pclchar->IsBlankInv( 1, INVMODE_ALL ) == false )
			{
				NTCHARString128	kTitle(GetTxtFromMgr(8869));
				NTCHARString128 kText(GetTxtFromMgr(8886));

				kText.Replace( "#count#", SI32ToString((SI32)1) );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
				pclClient->pclCM->CR[1]->bBuffRouletteSwitch = false ;
				return;
			}

			NTCHARString256 kText, kTitle;
			TCHAR szBuf[64] = { '\0', };
			m_siNeedPrice = 0;

			// [춘기] 일본의 경우 룰렛돌리는 금액을 다르게 설정.
			if(pclClient->IsWhereServiceArea(ConstServiceArea_Japan))
			{
				// 사용요금 계산
				if		(npcrate < 100)		{		m_siNeedPrice = 15000;		}
				else if	(npcrate < 300)		{		m_siNeedPrice = 30000;		}
				else if	(npcrate < 700)		{		m_siNeedPrice = 50000;		}
				else if	(npcrate < 1000)	{		m_siNeedPrice = 100000;	}
				else if	(npcrate < 1450)	{		m_siNeedPrice = 180000;	}
				else if	(npcrate < 1750)	{		m_siNeedPrice = 290000;	}
				else if	(npcrate < 2050)	{		m_siNeedPrice = 420000;	}
				else if	(npcrate < 2350)	{		m_siNeedPrice = 580000;	}
				else if	(npcrate < 3000)	{		m_siNeedPrice = 780000;	}

				// KHY - 1021 - NPCRate 값 확대.
				else if	(npcrate < 4000)	{		m_siNeedPrice = 1000000;	}
				else if	(npcrate < 5500)	{		m_siNeedPrice = 1300000;	}
				else if	(npcrate < 8500)	{		m_siNeedPrice = 1800000;	}
				else if	(npcrate < 14500)	{		m_siNeedPrice = 2500000;	}
				else if	(npcrate < 25500)	{		m_siNeedPrice = 4000000;	}
				else if	(npcrate < 33500)	{		m_siNeedPrice = 5800000;	}
				else						{		m_siNeedPrice = 8500000;	}
			}
			else
			{
				// 사용요금 계산
				if		(npcrate < 100)		{		m_siNeedPrice = 150000;		}
				else if	(npcrate < 300)		{		m_siNeedPrice = 300000;		}
				else if	(npcrate < 700)		{		m_siNeedPrice = 500000;		}
				else if	(npcrate < 1000)	{		m_siNeedPrice = 1000000;	}
				else if	(npcrate < 1450)	{		m_siNeedPrice = 1800000;	}
				else if	(npcrate < 1750)	{		m_siNeedPrice = 2900000;	}
				else if	(npcrate < 2050)	{		m_siNeedPrice = 4200000;	}
				else if	(npcrate < 2350)	{		m_siNeedPrice = 5800000;	}
				else if	(npcrate < 3000)	{		m_siNeedPrice = 7800000;	}

				// KHY - 1021 - NPCRate 값 확대.
				else if	(npcrate < 4000)	{		m_siNeedPrice = 10000000;	}
				else if	(npcrate < 5500)	{		m_siNeedPrice = 13000000;	}
				else if	(npcrate < 8500)	{		m_siNeedPrice = 18000000;	}
				else if	(npcrate < 14500)	{		m_siNeedPrice = 25000000;	}
				else if	(npcrate < 25500)	{		m_siNeedPrice = 40000000;	}
				else if	(npcrate < 33500)	{		m_siNeedPrice = 58000000;	}
				else						{		m_siNeedPrice = 85000000;	}
			}

			SI64	 NeedPrice_1 = m_siNeedPrice;
			SI64	 NeedPrice_2 = 0;
			SI64	 NeedPrice_3 = 0;
			
			// 아무 마을이나 가입되어 있으면, 정상가격
			if( pclchar->pclCI->clBI.siHomeVillage > 0 )
			{
				// 룰렛을 돌리고 있는 마을과 가입된 마을이 같다면, 0.7배 가격
				if( pclchar->GetMapIndex() == pclchar->pclCI->clBI.siHomeVillage)
				{
					NeedPrice_2 = m_siNeedPrice *= 0.7;
					m_siNeedPrice = NeedPrice_2;
				}
			}
			else
			{
				// 마을에 가입조차 되어있지 않으면 1.5배 가격
				NeedPrice_3 = m_siNeedPrice *= 1.5;
				m_siNeedPrice = NeedPrice_3;
			}


			// 메시지 타이틀 설정.
			kTitle = GetTxtFromMgr(8869);

			// PCK : 룰렛을 돌릴때 촛대 아이템이 있으면 우선 촛대부터 소모해서 돌린다. (09.08.10)
			// 실제 촛대가 있는지, 있다면 100개 이상 가지고 있는지 체크

			for(SI32 i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
			{
				if (pclchar->pclCI->clCharItem.clItem[i].siUnique == ITEMUNIQUE(13049)
					&& pclchar->pclCI->clCharItem.clItem[i].siItemNum >= 100)
				{	
					// 촛대 소모 메시지
					kText = GetTxtFromMgr(40078);

					g_JYLibFunction.SetNumUnit(100, szBuf, sizeof(szBuf));
					kText.Replace("#num#", szBuf);

					g_JYLibFunction.SetNumUnit(m_siNeedPrice, szBuf, sizeof(szBuf));
					kText.Replace("#money#", szBuf);

					b_bUseTicketItem = true;
					break;
				}
			}

			if(b_bUseTicketItem == false)
			{
				// 정상가격 설정 메시지
				kText = GetTxtFromMgr(8870);
				g_JYLibFunction.SetNumUnit(NeedPrice_1, szBuf, sizeof(szBuf));
				kText.Replace("#transmoney#", szBuf);

				// 아무 마을이나 가입되어 있으면, 정상가격
				if( pclchar->pclCI->clBI.siHomeVillage > 0 )
				{
					// 룰렛을 돌리고 있는 마을과 가입된 마을이 같다면, 0.7배 가격
					if( pclchar->GetMapIndex() == pclchar->pclCI->clBI.siHomeVillage)
					{
						kText = GetTxtFromMgr(8871);
						g_JYLibFunction.SetNumUnit(NeedPrice_1, szBuf, sizeof(szBuf));
						kText.Replace("#transmoney#", szBuf);

						g_JYLibFunction.SetNumUnit(NeedPrice_2, szBuf, sizeof(szBuf));
						kText.Replace("#distransmoney#", szBuf);
					}
				}
				else
				{
					// 마을에 가입조차 되어있지 않으면 1.5배 가격
					kText = GetTxtFromMgr(8870);
					g_JYLibFunction.SetNumUnit(NeedPrice_3, szBuf, sizeof(szBuf));
					kText.Replace("#transmoney#", szBuf);
				}


				// 소지금 확인
				if( m_siNeedPrice <= 0 || pclchar->pclCI->clIP.GetMoney() < m_siNeedPrice )
				{
					kText = GetTxtFromMgr(5870);

					cltClient*  pclclient = (cltClient *)pclClient;
					pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(kTitle, kText);
					return;
				}

			}

			stMsgBox MsgBox;
			MsgBox.Set( this, kTitle, kText, MSGBOX_TYPE_YESNO, 100 );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );

			CEdit* pclEdit_Info = m_InterfaceFactory.GetEdit(BUFF_ROULETTE_EDIT_IMFORMATION);
			if(pclEdit_Info)		pclEdit_Info->SetText("");
		}
		break;
	}
}

void CNCharRouletteDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient*  pclclient = (cltClient *)pclClient;
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);

	switch(MsgIndex)
	{
	case 100:
		{
			if(RetValue)
			{
				pclclient->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
				pclclient->SetSiSelectedOrder(ORDER_STOP);

				cltGameMsgRequest_Char_Roulette clCharRoulette( CHAR_ROULETTE_START, 0 , b_bUseTicketItem);
				cltMsg clMsg(GAMEMSG_REQUEST_CHAR_ROULETTE, sizeof(clCharRoulette), (BYTE*)&clCharRoulette);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			else if(b_bUseTicketItem) // RetValue = false 이고, b_bUseTicketItem = true 라면, 티켓이 있는 상태에서 NO를 누른 것이다.  
			{
				// 소지금 확인
				if( m_siNeedPrice <= 0 || pclchar->pclCI->clIP.GetMoney() < m_siNeedPrice )
				{
					NTCHARString256 kText, kTitle;

					kTitle = GetTxtFromMgr(8869);
					kText = GetTxtFromMgr(5870);

					cltClient*  pclclient = (cltClient *)pclClient;
					pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(kTitle, kText);
					return;
				}

				b_bUseTicketItem = false; // 돈으로 계산되어 한다.

				pclclient->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
				pclclient->SetSiSelectedOrder(ORDER_STOP);

				cltGameMsgRequest_Char_Roulette clCharRoulette( CHAR_ROULETTE_START, 0 , b_bUseTicketItem);
				cltMsg clMsg(GAMEMSG_REQUEST_CHAR_ROULETTE, sizeof(clCharRoulette), (BYTE*)&clCharRoulette);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);				
			}
		}
		break;
	}
}

void CNCharRouletteDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if( m_ImgCharBufBuffer && m_bCreateImgBuffer )
	{
		GP.PutXpr16( ScreenRX + GetX() + 33, ScreenRY + GetY() + 118, 
					40, 40 * 3,
					m_ImgCharBufBuffer,
					RGB16_TRANS );

		if( m_SprSelect.header.iXSize > 0 )
		{
			GP.PutSpr( &m_SprSelect, ScreenRX + GetX() + 33, ScreenRY + GetY() + 118 + 40, 5 );
		}
	}
}

void CNCharRouletteDlg::Action()
{
	if( m_siCharBufCount > 0 && m_siCurCount < m_siTotalCount )
	{
		m_siStartY-= m_siSpeed;
		if(m_siStartY <= -40)
		{
			m_siCurIndex++;
			m_siCurCount++;
			m_siCurIndex = m_siCurIndex % m_siCharBufCount;
			m_siStartY = 0;

//			if(m_siCurCount == m_siTotalCount - 40)			m_siSpeed = 20;
//			else if(m_siCurCount == m_siTotalCount - 15)	m_siSpeed = 10;
//			else if(m_siCurCount == m_siTotalCount - 7)		m_siSpeed = 8;
//			else if(m_siCurCount == m_siTotalCount - 3)		m_siSpeed = 5;
//			else if(m_siCurCount ==  m_siTotalCount - 1)	m_siSpeed = 2;

			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_STATUE_BUFF")), 0, 0);
		}

		MakeBufRouletteImage();
	}

	// 룰렛 종료
	if( m_siTotalCount > 0 && m_siCurCount >= m_siTotalCount )
	{
		End();
	}
}

void CNCharRouletteDlg::MakeBufRouletteImage()
{
	SI32	siKind = 0;
	SI32	siGimgIndex = 0;
	SI32	siFont = 0;
	TSpr*	pSpr = 0;

	SI32	size = 0;
	SI32	siIndex = m_siCurIndex;

	if ( m_pCharBufSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = m_pCharBufSurfaceObj->GetSurface();
		if ( lpSurface )
		{
			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			if( GP.LockSurface( lpSurface ) )
			{
				siKind = m_siCharKind[siIndex];
				if(siKind < 1000)	
					pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siKind, &siGimgIndex, &siFont );
				else				
					pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siKind, &siGimgIndex, &siFont );

				pSpr = pclClient->GetGlobalSpr(siGimgIndex);
				if(pSpr)	GP.PutSpr(pSpr, 0, m_siStartY + 0, siFont);
				siIndex++;		siIndex = siIndex % m_siCharBufCount;

				siKind = m_siCharKind[siIndex];
				if(siKind < 1000)	pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siKind, &siGimgIndex, &siFont );
				else				pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siKind, &siGimgIndex, &siFont );
				pSpr = pclClient->GetGlobalSpr(siGimgIndex);
				if(pSpr)	GP.PutSpr(pSpr, 0, m_siStartY + 40, siFont);
				siIndex++;		siIndex = siIndex % m_siCharBufCount;

				siKind = m_siCharKind[siIndex];
				if(siKind < 1000)	pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siKind, &siGimgIndex, &siFont );
				else				pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siKind, &siGimgIndex, &siFont );
				pSpr = pclClient->GetGlobalSpr(siGimgIndex);
				if(pSpr)	GP.PutSpr(pSpr, 0, m_siStartY + 80, siFont);
				siIndex++;		siIndex = siIndex % m_siCharBufCount;

				siKind = m_siCharKind[siIndex];
				if(siKind < 1000)	pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siKind, &siGimgIndex, &siFont );
				else				
					pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siKind, &siGimgIndex, &siFont );
				pSpr = pclClient->GetGlobalSpr(siGimgIndex);
				if(pSpr)	GP.PutSpr(pSpr, 0, m_siStartY + 120, siFont);

				if(m_ImgCharBufBuffer)
				{
					g_graphics.EncodeXpr( m_ImgCharBufBuffer, 40, 40 * 3, RGB16_TRANS, &size );
					m_bCreateImgBuffer = true;
				}

				GP.UnlockSurface( lpSurface );
			}
		}
	}
}
