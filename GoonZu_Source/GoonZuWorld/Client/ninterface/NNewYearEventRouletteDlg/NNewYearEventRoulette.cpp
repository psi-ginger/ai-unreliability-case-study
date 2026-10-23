// 신년운세 룰렛

#include "NNewYearEventRoulette.h"

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


CNewYearEventSurfaceObj::CNewYearEventSurfaceObj()
: m_lpSurface(NULL),
  m_siWidth(0),
  m_siHeight(0)   
{
}

CNewYearEventSurfaceObj::~CNewYearEventSurfaceObj()
{
	if( m_lpSurface )
	{
		m_lpSurface->Release();
		m_lpSurface = NULL;
	}
}

void CNewYearEventSurfaceObj::Create( SI16 SurfaceWidth, SI16 SurfaceHeight )
{
	if( g_graphics.CreateSurface16( &m_lpSurface, SurfaceWidth, SurfaceHeight, TRUE, RGB16_TRANS ) == FALSE )
	{
		MsgBox( TEXT("CreateOffScreen Error"), TEXT("CControllerSurfaceObj::Create") );
		return;
	}

	m_siWidth = SurfaceWidth;
	m_siHeight = SurfaceHeight;
}

CNNewYearEventRoulette::CNNewYearEventRoulette( void )
: m_siDivinationCount(0),
  m_pDivinationSurfaceObj(NULL),
  m_ImgDivinationBuffer(NULL),
  m_bCreateImgBuffer(false),
  m_siResultDivination(0),
  m_siSpeed(0),
  m_siStartClock(0),
  m_siCurIndex(0),
  m_siTotalCount(0),
  m_siCurCount(0),
  m_siStartY(0),
  m_siNeedPrice(0)
{
	ZeroMemory(m_siDivinationList, sizeof(m_siDivinationList));
}


CNNewYearEventRoulette::~CNNewYearEventRoulette( void )
{
	Destroy();

	if(m_pDivinationSurfaceObj)
	{
		delete m_pDivinationSurfaceObj;
		m_pDivinationSurfaceObj = NULL;
	}

	if(m_ImgDivinationBuffer)
	{
		delete[] m_ImgDivinationBuffer;
		m_ImgDivinationBuffer = NULL;
	}
}
void CNNewYearEventRoulette::Destroy()
{
	// 룰렛이 동작중이라면, 취소한다.
	if( m_siResultDivination > 0 )
	{
		Init();

		if(pclClient)
		{
			cltGameMsgRequest_Divination_Roulette clDivinationRoulette( DIVINATION_ROULETTE_END, 0, 0 );
			cltMsg clMsg(GAMEMSG_REQUEST_DIVINATION_ROULETTE, sizeof(clDivinationRoulette), (BYTE*)&clDivinationRoulette);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}
}

void CNNewYearEventRoulette::Init( )
{
	m_siDivinationCount	= 0;
	m_siResultDivination= 0;
	m_siSpeed			= 0;
	m_siCurIndex		= 0;
	m_siTotalCount		= 0;
	m_siCurCount		= 0;
	m_siNeedPrice		= 0;
	m_siStartY			= 0;
	ZeroMemory(m_siDivinationList, sizeof(m_siDivinationList));

	m_siDivinationCount	= 3;
	m_siDivinationList[0]		= pclClient->GetUniqueFromHash("KIND_RACCOON");
	m_siDivinationList[1]		= pclClient->GetUniqueFromHash("KIND_RABBIT");
	m_siDivinationList[2]		= pclClient->GetUniqueFromHash("KIND_MUSHROOM");		
}

void CNNewYearEventRoulette::Create()
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);

	if ( IsCreate() == true )
	{
		Destroy();
		DeleteDialog();
		return;
	}
	else
	{
		CInterfaceFile file;
		file.LoadFile( TEXT( "NInterface/Data/NNewYearEventRouletteDlg/DLG_NNewYearEventRoulette.ddf" ) );
		file.CreatePopupDialog( this, NEWYEAR_ROULETTE_DLG, TEXT( "dialog_NewYear_Roulette" ), StaticCallBackDialogNNewYearEventRoulette );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NEWYEAR_ROULETTE_DLG_BUTTON_ROULETTE_START, this), NEWYEAR_ROULETTE_DLG_BUTTON_ROULETTE_START, TEXT("button_roulette_start") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NEWYEAR_ROULETTE_DLG_BUTTON_CLOSE, this),          NEWYEAR_ROULETTE_DLG_BUTTON_CLOSE, TEXT("button_close") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,   NEWYEAR_ROULETTE_DLG_EDITBOX_EXPLAIN, this),       NEWYEAR_ROULETTE_DLG_EDITBOX_EXPLAIN, TEXT("editbox_explain") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,   NEWYEAR_ROULETTE_DLG_EDITBOX_INFORMATION, this),   NEWYEAR_ROULETTE_DLG_EDITBOX_INFORMATION, TEXT("editbox_information") );

		m_pDivinationSurfaceObj = new CNewYearEventSurfaceObj();
		if(m_pDivinationSurfaceObj)		m_pDivinationSurfaceObj->Create(60, 60 * 4);

		m_ImgDivinationBuffer = new RGB16[ 40 * 40 * 4 * 4 ];

		SetActionTimer( 50 );    
		SetTimerOnOff( true );

		CEdit* pclEdit_Info = m_InterfaceFactory.GetEdit(NEWYEAR_ROULETTE_DLG_EDITBOX_EXPLAIN);
		if(pclEdit_Info)
		{
			NTCHARString256 kText(GetTxtFromMgr(9675));
			m_siNeedPrice = 0;

			// 사용요금 계산
			SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

			if		(npcrate < 100)		{		m_siNeedPrice = 500000;		}
			else if	(npcrate < 300)		{		m_siNeedPrice = 1000000;	}
			else if	(npcrate < 700)		{		m_siNeedPrice = 1500000;	}
			else if	(npcrate < 1000)	{		m_siNeedPrice = 2000000;	}
			else if (npcrate < 1450)    {		m_siNeedPrice = 2500000;	}
			else if (npcrate < 1750)	{		m_siNeedPrice = 3000000;	}
			else if	(npcrate < 2050)	{		m_siNeedPrice = 3500000;	}
			else if (npcrate < 2350)	{		m_siNeedPrice = 4000000;	}
			else if	(npcrate < 3000)	{		m_siNeedPrice = 4500000;	}

			// KHY - 1021 - NPCRate 값 확대.
			else if	(npcrate < 4000)	{		m_siNeedPrice = 5000000;	}
			else if	(npcrate < 5500)	{		m_siNeedPrice = 7500000;	}
			else if	(npcrate < 8500)	{		m_siNeedPrice = 12000000;	}
			else if	(npcrate < 14500)	{		m_siNeedPrice = 20000000;	}
			else if	(npcrate < 25500)	{		m_siNeedPrice = 38000000;	}
			else if	(npcrate < 33500)	{		m_siNeedPrice = 50000000;	}
			else						{		m_siNeedPrice = 70000000;	}

			//// 아무 마을이나 가입되어 있으면, 정상가격
			//if( pclchar->pclCI->clBI.siHomeVillage > 0 )
			//{
			//	// 룰렛을 돌리고 있는 마을과 가입된 마을이 같다면, 0.7배 가격
			//	if( pclchar->GetMapIndex() == pclchar->pclCI->clBI.siHomeVillage)
			//	{
			//		m_siNeedPrice *= 0.7;
			//	}
			//}
			//else
			//{
			//	// 마을에 가입조차 되어있지 않으면 1.5배 가격
			//	m_siNeedPrice *= 1.5;
			//}

			TCHAR szBuf[64] = { '\0', };
			g_JYLibFunction.SetNumUnit(m_siNeedPrice, szBuf, sizeof(szBuf));
			kText.Replace("#newyearmoney#", szBuf);
			pclEdit_Info->SetText( kText );
		}

		Init();

		// 초기화면 그리기
		MakeDivinationImage();
		GP.LoadSprFromFile("NINTERFACE/IMAGE/Artifact_Slot_Image_40x40_005.SPR", &m_SprSelect);
	}
}

bool CNNewYearEventRoulette::Start()
{
	if(m_siDivinationCount < 0)		return false;
	if(m_siResultDivination < 0)	return false;

	SI32 siResultIndex = 0;

	for(SI32 i=0; i<m_siDivinationCount; i++)
	{
		if(m_siDivinationList[i] == m_siResultDivination)
		{
			siResultIndex = i;
			break;
		}
	}

	m_siSpeed		= 20;
	m_siCurIndex	= 0;
	m_siTotalCount	= m_siDivinationCount  + siResultIndex;
	m_siCurCount	= 1;
	m_siStartY		= 0;

	return true;
}

bool CNNewYearEventRoulette::End()
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
	if(pclchar == NULL)									return false;

	cltClient*  pclclient = (cltClient *)pclClient;

	if( m_siResultDivination > 0 )
	{
		bool bSuccess = true;
		// 소지금 확인
		if( m_siNeedPrice <= 0 || pclchar->pclCI->clIP.GetMoney() < m_siNeedPrice )
		{
			pclClient->pclCM->CR[1]->bBuffRouletteSwitch = false ;
			NTCHARString128	kTitle(GetTxtFromMgr(9738));
			NTCHARString128 kText(GetTxtFromMgr(5870));

			cltClient*  pclclient = (cltClient *)pclClient;
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(kTitle, kText);

			bSuccess = false;
		}

		if( pclchar->IsBlankInv( 1, INVMODE_ALL ) == false )
		{
			NTCHARString128	kTitle(GetTxtFromMgr(9738));
			NTCHARString128 kText(GetTxtFromMgr(8886));

			kText.Replace( "#count#", SI32ToString((SI32)1) );
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
			pclClient->pclCM->CR[1]->bBuffRouletteSwitch = false ;

			bSuccess = false;
		}


		if( bSuccess )
		{
			cltGameMsgRequest_Divination_Roulette clDivinationRoulette( DIVINATION_ROULETTE_ADDBUF, m_siResultDivination, m_siBufKind_Type );
			cltMsg clMsg(GAMEMSG_REQUEST_DIVINATION_ROULETTE, sizeof(clDivinationRoulette), (BYTE*)&clDivinationRoulette);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

			// 캐릭터 변신 버프이면
			if(m_siBufKind_Type == BUFKIND_CHAR)
			{
				NTCHARString128	kText(GetTxtFromMgr(9677));
				CEdit* pclEdit_Info = m_InterfaceFactory.GetEdit(NEWYEAR_ROULETTE_DLG_EDITBOX_INFORMATION);
				if(pclEdit_Info)
				{
					pclEdit_Info->SetText( kText );
				}
			}
			// 아이템 지급이면
			else if(m_siBufKind_Type == BUFKIND_ITEM)
			{
				NTCHARString128	kText(GetTxtFromMgr(9678));
				NTCHARString64 kName;
				kName = pclClient->pclItemManager->GetName(m_siResultDivination);
				kText.Replace("#name#", kName);

				CEdit* pclEdit_Info = m_InterfaceFactory.GetEdit(NEWYEAR_ROULETTE_DLG_EDITBOX_INFORMATION);
				if(pclEdit_Info)
				{
					pclEdit_Info->SetText( kText );
				}
			}
			// 버프이면
			else if(m_siBufKind_Type == BUFKIND_BUF)
			{
				NTCHARString128	kText(GetTxtFromMgr(9676));
				CEdit* pclEdit_Info = m_InterfaceFactory.GetEdit(NEWYEAR_ROULETTE_DLG_EDITBOX_INFORMATION);
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

void CALLBACK CNNewYearEventRoulette::StaticCallBackDialogNNewYearEventRoulette( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNNewYearEventRoulette* pThis = (CNNewYearEventRoulette*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNNewYearEventRoulette( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNNewYearEventRoulette::CallBackDialogNNewYearEventRoulette( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient*  pclclient = (cltClient *)pclClient;
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
	case NEWYEAR_ROULETTE_DLG_BUTTON_CLOSE:
		{
			pclClient->pclCM->CR[1]->bBuffRouletteSwitch = false ;
			Destroy();
			DeleteDialog();
		}
		break;	
	case NEWYEAR_ROULETTE_DLG_BUTTON_ROULETTE_START:
		{
			// 룰렛이 동작중이라면 명령은 취소된다.
			if( m_siResultDivination > 0 )									return;

			SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
			//cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
			if(pclchar == NULL)												return;

			if( pclchar->clTransFormInfo.bTransFormMode == true )
			{
				NTCHARString128	kTitle(GetTxtFromMgr(8662));
				NTCHARString128 kText(GetTxtFromMgr(8872));

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
				return;
			}

			// 아이템을 받을 공간을 확보한다.
			if( pclchar->IsBlankInv( 1, INVMODE_ALL ) == false )
			{
				NTCHARString128	kTitle(GetTxtFromMgr(9738));
				NTCHARString128 kText(GetTxtFromMgr(8886));

				kText.Replace( "#count#", SI32ToString((SI32)1) );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
				pclClient->pclCM->CR[1]->bBuffRouletteSwitch = false ;
				return;
			}

			NTCHARString256 kText, kTitle;
			TCHAR szBuf[64] = { '\0', };
			m_siNeedPrice = 0;

			// 사용요금 계산
			if		(npcrate < 100)		{		m_siNeedPrice = 500000;		}
			else if	(npcrate < 300)		{		m_siNeedPrice = 1000000;	}
			else if	(npcrate < 700)		{		m_siNeedPrice = 1500000;	}
			else if	(npcrate < 1000)	{		m_siNeedPrice = 2000000;	}
			else if (npcrate < 1450)    {		m_siNeedPrice = 2500000;	}
			else if (npcrate < 1750)	{		m_siNeedPrice = 3000000;	}
			else if	(npcrate < 2050)	{		m_siNeedPrice = 3500000;	}
			else if (npcrate < 2350)	{		m_siNeedPrice = 4000000;	}
			else if	(npcrate < 3000)	{		m_siNeedPrice = 4500000;	}

			// KHY - 1021 - NPCRate 값 확대.
			else if	(npcrate < 4000)	{		m_siNeedPrice = 5000000;	}
			else if	(npcrate < 5500)	{		m_siNeedPrice = 7500000;	}
			else if	(npcrate < 8500)	{		m_siNeedPrice = 12000000;	}
			else if	(npcrate < 14500)	{		m_siNeedPrice = 20000000;	}
			else if	(npcrate < 25500)	{		m_siNeedPrice = 38000000;	}
			else if	(npcrate < 33500)	{		m_siNeedPrice = 50000000;	}
			else						{		m_siNeedPrice = 70000000;	}

			// 정상가격 설정 메시지
			kText = GetTxtFromMgr(9675);
			g_JYLibFunction.SetNumUnit(m_siNeedPrice, szBuf, sizeof(szBuf));
			kText.Replace("#newyearmoney#", szBuf);

			kTitle = GetTxtFromMgr(9738);

			//// 아무 마을이나 가입되어 있으면, 정상가격
			//if( pclchar->pclCI->clBI.siHomeVillage > 0 )
			//{
			//	// 룰렛을 돌리고 있는 마을과 가입된 마을이 같다면, 0.7배 가격
			//	if( pclchar->GetMapIndex() == pclchar->pclCI->clBI.siHomeVillage)
			//	{
			//		kText = GetTxtFromMgr(8871);
			//		g_JYLibFunction.SetNumUnit(m_siNeedPrice, szBuf, sizeof(szBuf));
			//		kText.Replace("#transmoney#", szBuf);
			//		m_siNeedPrice *= 0.7;
			//		g_JYLibFunction.SetNumUnit(m_siNeedPrice, szBuf, sizeof(szBuf));
			//		kText.Replace("#distransmoney#", szBuf);
			//	}
			//}
			//else
			//{
			//	// 마을에 가입조차 되어있지 않으면 1.5배 가격
			//	m_siNeedPrice *= 1.5;
			//	kText = GetTxtFromMgr(8870);
			//	g_JYLibFunction.SetNumUnit(m_siNeedPrice, szBuf, sizeof(szBuf));
			//	kText.Replace("#transmoney#", szBuf);
			//}

			// 소지금 확인
			if( m_siNeedPrice <= 0 || pclchar->pclCI->clIP.GetMoney() < m_siNeedPrice )
			{
				kText = GetTxtFromMgr(5870);

				cltClient*  pclclient = (cltClient *)pclClient;
				pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(kTitle, kText);
				return;
			}

			stMsgBox MsgBox;
			MsgBox.Set( this, kTitle, kText, MSGBOX_TYPE_YESNO, 100 );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );

			CEdit* pclEdit_Info = m_InterfaceFactory.GetEdit(NEWYEAR_ROULETTE_DLG_EDITBOX_INFORMATION);
			if(pclEdit_Info)		pclEdit_Info->SetText("");
		}
		break;
	}
}

void CNNewYearEventRoulette::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient*  pclclient = (cltClient *)pclClient;

	switch(MsgIndex)
	{
	case 100:
		{
			if(RetValue)
			{
				pclclient->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
				pclclient->SetSiSelectedOrder(ORDER_STOP);

				cltGameMsgRequest_Divination_Roulette clDivinationRoulette( DIVINATION_ROULETTE_START, 0, 0 );
				cltMsg clMsg(GAMEMSG_REQUEST_DIVINATION_ROULETTE, sizeof(clDivinationRoulette), (BYTE*)&clDivinationRoulette);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}

void CNNewYearEventRoulette::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if( m_ImgDivinationBuffer && m_bCreateImgBuffer )
	{
		GP.PutXpr16( ScreenRX + GetX() + 33, ScreenRY + GetY() + 118, 
			40, 40 * 3,
			m_ImgDivinationBuffer,
			RGB16_TRANS );

		if( m_SprSelect.header.iXSize > 0 )
		{
			GP.PutSpr( &m_SprSelect, ScreenRX + GetX() + 33, ScreenRY + GetY() + 118 + 40, 5 );
		}
	}
}

void CNNewYearEventRoulette::Action()
{
	if( m_siDivinationCount > 0 && m_siCurCount < m_siTotalCount )
	{
		m_siStartY-= m_siSpeed;
		if(m_siStartY <= -40)
		{
			m_siCurIndex++;
			m_siCurCount++;
			m_siCurIndex = m_siCurIndex % m_siDivinationCount;
			m_siStartY = 0;

			//			if(m_siCurCount == m_siTotalCount - 40)			m_siSpeed = 20;
			//			else if(m_siCurCount == m_siTotalCount - 15)	m_siSpeed = 10;
			//			else if(m_siCurCount == m_siTotalCount - 7)		m_siSpeed = 8;
			//			else if(m_siCurCount == m_siTotalCount - 3)		m_siSpeed = 5;
			//			else if(m_siCurCount ==  m_siTotalCount - 1)	m_siSpeed = 2;

			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_STATUE_BUFF")), 0, 0);
		}

		MakeDivinationImage();
	}

	// 룰렛 종료
	if( m_siTotalCount > 0 && m_siCurCount >= m_siTotalCount )
	{
		End();
	}
}

void CNNewYearEventRoulette::MakeDivinationImage()
{
	SI32	siKind = 0;
	SI32	siGimgIndex = 0;
	SI32	siFont = 0;
	SI32	siFontindex = 34;
	TSpr*	pSpr = 0;
	TSpr	BufSpr = 0;

	GP.LoadSprFromFile(TEXT("gimg\\BufImage.SPR"), &BufSpr);

	SI32	size = 0;
	SI32	siIndex = m_siCurIndex;

	if ( m_pDivinationSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = m_pDivinationSurfaceObj->GetSurface();
		if ( lpSurface )
		{
			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			if( GP.LockSurface( lpSurface ) )
			{
				if(&BufSpr)	GP.PutSpr(&BufSpr, 0, m_siStartY + 0, siFontindex);
				siIndex++;		siIndex = siIndex % m_siDivinationCount;

				if(&BufSpr)	GP.PutSpr(&BufSpr, 0, m_siStartY + 40, siFontindex);
				siIndex++;		siIndex = siIndex % m_siDivinationCount;

				if(&BufSpr)	GP.PutSpr(&BufSpr, 0, m_siStartY + 80, siFontindex);
				siIndex++;		siIndex = siIndex % m_siDivinationCount;

				if(&BufSpr)	GP.PutSpr(&BufSpr, 0, m_siStartY + 120, siFontindex);

				if(m_ImgDivinationBuffer)
				{
					g_graphics.EncodeXpr( m_ImgDivinationBuffer, 40, 40 * 3, RGB16_TRANS, &size );
					m_bCreateImgBuffer = true;
				}

				GP.UnlockSurface( lpSurface );
			}
		}
	}
}