#include "NMiningDlg.h"
#include "../../Client/client.h"
#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/interface/Button.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../CommonLogic/Msg/MsgType-Agriculture.h"
#include "../../CommonLogic/Msg/MsgType-Mining.h"

#include "../common/Agriculture/mining/MiningMgr.h"
#include "../common/Char/CharServer/Char-Server.h"
//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"
#include "Order/order.h"

#include "../../Music/Music.h"
#include "../NLib/NUpdateTimer.h"

extern cltCommonLogic* pclClient;

CNMiningDlg::CNMiningDlg()
{
	m_pExplainStatic = NULL;	
	m_pStatic = NULL;
	m_pStartButton = NULL;
	m_pStatusEdit = NULL;
	m_pImageStatic = NULL;
	m_pminieralinfoStatic = NULL;
	m_pOutline = NULL;

	m_siMiningItemUnique = 0;
	m_siSuccessPercent = 0;
	m_siDelayTime = 0;

	m_siEffcectCounter = 0;

	m_siMinePosX = 0;
	m_siMinePosY = 0;

	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		m_bStopMining = TRUE;
	}
	else
	{
		m_bStopMining = FALSE;
	}

	m_uiLeaseLeftTime = 0;

	m_siNowFontNum = 0;

	GP.LoadSprFromFile( TEXT("GImg\\MakeItemDrawPart1.Spr"),	&m_SprMineGage );
	memset(m_strMineName, 0, sizeof(m_strMineName));
	memset(m_siMineNum, 0, sizeof(m_siMineNum));
}

CNMiningDlg::~CNMiningDlg()
{
	SetTimerOnOff( false );

	if ( m_pExplainStatic )
	{
		delete m_pExplainStatic;
		m_pExplainStatic = NULL;
	}
	
	if ( m_pStatic )
	{
		delete m_pStatic;
		m_pStatic = NULL;
	}

	if( m_pStartButton )
	{
		delete m_pStartButton;
		m_pStartButton = NULL;
	}

	if( m_pStatusEdit )
	{
		delete m_pStatusEdit;
		m_pStatusEdit = NULL;
	}

	if( m_pImageStatic )
	{
		delete m_pImageStatic;
		m_pImageStatic = NULL;
	}

	if( m_pOutline )
	{
		delete m_pOutline;
		m_pOutline = NULL;
	}

	if ( m_SprMineGage.pImage )
	{
		GP.FreeSpr( m_SprMineGage );
	}

	if (m_pminieralinfoStatic)
	{
		delete m_pminieralinfoStatic;
		m_pminieralinfoStatic = NULL;
	}
}

void CNMiningDlg::Init()
{
	memset(m_strMineName, 0, sizeof(m_strMineName));
	memset(m_siMineNum, 0, sizeof(m_siMineNum));
	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		m_pStatusEdit->Clear();
	}
}

void CNMiningDlg::Create()
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NMining/DLG_NMining.ddf"));
	file.CreatePopupDialog( this, NMINING_DLG, TEXT("dialog_NMining"), StaticNMiningDlgProc );	
	
	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		m_pExplainStatic = new CStatic( this );
		m_pStatic = new CStatic( this );
		m_pStartButton = new CButton( this );
		m_pStatusEdit = new CEdit( this );
		m_pImageStatic = new CImageStatic( this );
		m_pminieralinfoStatic = new CStatic( this );
		m_pOutline = new COutLine( this );

		m_pExplainStatic->SetBorder( true );
		m_pExplainStatic->SetBKColorUse( true );

		file.CreateControl( m_pExplainStatic, NMINING_DIALOG_EXPLAIN_STATIC, TEXT("statictext_NMiningExplain") );
		file.CreateControl( m_pminieralinfoStatic, NMINING_DIALOG_MINIERALINFO_STATIC, TEXT("statictext_NONAME1"));
		file.CreateControl( m_pStatic, NMINING_DIALOG_STATIC, TEXT("statictext_NMiningStatic") );
		file.CreateControl( m_pStartButton, NMINING_DIALOG_START_BUTTON, TEXT("button_NMiningStart") );
		file.CreateControl( m_pStatusEdit, NMINING_DIALOG_STATUS_EDIT, TEXT("editbox_status") );
		file.CreateControl( m_pImageStatic, NMINING_DIALOG_IMAGESTATIC, TEXT("imagestatic_NONAME1") );
		file.CreateControl( m_pOutline, NMINING_DIALOG_OUTLINE, TEXT("outline_NONAME1"));

		if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
		{
			m_pImageStatic->SetFileName(TEXT("GImg/ItemImg01.SPR"));
		}	
	}
	else
	{
		m_pExplainStatic = new CStatic( this );
		m_pStatic = new CStatic( this );
		m_pStartButton = new CButton( this );

		m_pExplainStatic->SetBorder( true );
		m_pExplainStatic->SetBKColorUse( true );

		m_pStatic->SetBorder( true );
		m_pStatic->SetBKColorUse( true );

		file.CreateControl( m_pExplainStatic, NMINING_DIALOG_EXPLAIN_STATIC, TEXT("statictext_NMiningExplain") );
		file.CreateControl( m_pStatic, NMINING_DIALOG_STATIC, TEXT("statictext_NMiningStatic") );
		file.CreateControl( m_pStartButton, NMINING_DIALOG_START_BUTTON, TEXT("button_NMiningStart") );

		m_pStatic->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	}

	m_pStatic->Refresh();
	
	MovePos( 400, 150 );

	SetTimerOnOff( true );
	SetActionTimer( 1000 );	

	kUpdateTimer_DrawImage.Init( 0.1 * 1000 );
	kUpdateTimer_DrawImage.SetActive( true, GetTickCount() );

	Show( false );
	
	return;
}

void CALLBACK CNMiningDlg::StaticNMiningDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMiningDlg *pThis = (CNMiningDlg*) pControllerMgr;
	pThis->NMiningDlgProc( nEvent, nControlID, pControl );

	return;
}

void CALLBACK CNMiningDlg::NMiningDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;
	switch( nControlID)
	{			
	case COMMON_DIALOG_CLOSEBUTTON:	
		{
			if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
			{
				m_bStopMining = TRUE;
				pclclient->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
				pclclient->SetSiSelectedOrder(ORDER_STOP);

				pclclient->bStopDoingSwitch = FALSE;
			}
			else
			{
			}
		}
		break;
	case NMINING_DIALOG_START_BUTTON:
		{
			if ( nEvent == EVENT_BUTTON_CLICKED )
			{	
				SI16 VillageUnique = pclClient->pclVillageManager->FindNearVillage( pclClient->pclCM->CR[1]->GetX(), pclClient->pclCM->CR[1]->GetY() );
				SI32 FieldObjectUnique = pclClient->pclCM->CR[1]->pclMap->GetFieldObject_Unique( m_siMinePosX, m_siMinePosY );
				
				if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
				{
					if( m_bStopMining == TRUE )
					{
						m_pImageStatic->Show(TRUE);
						m_bStopMining = FALSE;
						cltGameMsgRequest_MiningStart clGameMsgRequest_MiningStart( VillageUnique, FieldObjectUnique );
						cltMsg clMsg(GAMEMSG_REQUEST_MININGSTART, sizeof(clGameMsgRequest_MiningStart), (BYTE*)&clGameMsgRequest_MiningStart);
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
					}
					else
					{
						m_bStopMining = TRUE;
						pclclient->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
						pclclient->SetSiSelectedOrder(ORDER_STOP);
						TCHAR* pText = GetTxtFromMgr(3386);
						m_pStartButton->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
						m_siMiningItemUnique = 0;
						m_siSuccessPercent = 0;
						m_siDelayTime = 0;
						Init();
						Show(true);
					}
				}
				else
				{
					if( VillageUnique > 0 )
					{
						cltGameMsgRequest_MiningStart clGameMsgRequest_MiningStart( VillageUnique, FieldObjectUnique );
						cltMsg clMsg(GAMEMSG_REQUEST_MININGSTART, sizeof(clGameMsgRequest_MiningStart), (BYTE*)&clGameMsgRequest_MiningStart);
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
		}
		break;
	}

	return;
}

void CNMiningDlg::Action()
{
	if ( pclClient->pclCM->CR[1]->GetCurrentOrder() == ORDER_MINE && IsShow() == false && pclClient->pclCM->CR[1]->GetSubOrder() == CHARWORKSTEP_ACTION )
	{	
		Show( true );
		if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
		{
			m_pImageStatic->Show(false);
		}
	}
	else if ( pclClient->pclCM->CR[1]->GetCurrentOrder() != ORDER_MINE )
	{
		TCHAR* pText = GetTxtFromMgr(3386);
		m_pStartButton->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		m_siMiningItemUnique = 0;
		m_siSuccessPercent = 0;
		m_siDelayTime = 0;
		m_uiLeaseLeftTime = 0;
		if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
		{
			Init();
		}
		Show( false );
	}

	if ( IsShow() )
	{	
		++m_siEffcectCounter;
		
		if ( m_siEffcectCounter > 7 )
		{
			m_siEffcectCounter = 0;
			pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINING_NOW")), 0, 0 );
		}

		SetMiningExplain();
	}

	return;
}

void CNMiningDlg::SetMiningItem( SI16 ItemUnique, SI16 SuccePercent, SI32 DelayTime )
{
	TCHAR* pText = GetTxtFromMgr(3387);
	m_pStartButton->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	m_siMiningItemUnique = (SI32)ItemUnique;
	m_siSuccessPercent = SuccePercent;
	if ( DelayTime > 300000 )
	{
		m_siDelayTime = 300000;
	}
	else
	{
		m_siDelayTime = DelayTime;
	}
	
	cltItem clItem;
	clItem.siItemNum = 1;
	clItem.siUnique = ItemUnique;

	SetMiningExplain();

	TCHAR Buffer[ MAX_PATH ] = TEXT("");
	pText = GetTxtFromMgr(3223);
	StringCchPrintf( Buffer, MAX_PATH, pText, pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_MINE, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0] ) );

	SetTitle( Buffer );

	return;
}

void CNMiningDlg::SetMiningExplain()
{
	TCHAR Temp[ 256 ] = TEXT("");
	TCHAR Buffer[256 ] = TEXT("");

	if ( m_siMiningItemUnique > 0 )
	{
		TCHAR *ProductItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_siMiningItemUnique );

		if ( ProductItemName )
		{
			if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
			{
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(2446);
				StringCchPrintf( Temp, 256, pText, ProductItemName, ( m_siDelayTime / 1000 ) / 60, ( m_siDelayTime / 1000 ) % 60 );
				StringCchCat( Buffer, 256, Temp );
			}
		}

		if ( m_siSuccessPercent > 0 )
		{
			TCHAR* pText = GetTxtFromMgr(3221);
			StringCchPrintf( Temp, 256, pText, m_siSuccessPercent );
			StringCchCat( Buffer, 256, Temp );
		}
	}

	if (  pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[1]->ActiveWeapon ].siUnique > 0 )
	{
		TCHAR *MachineName = (TCHAR*)pclClient->pclItemManager->GetName( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[1]->ActiveWeapon ].siUnique );

		if ( MachineName )
		{
			StringCchPrintf( Temp, 256, TEXT("[%s]\r\n"), MachineName);
			StringCchCat( Buffer, 256, Temp );
		}	
	}

#ifdef _DEBUG	
		
		if (  pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_NECK ].siUnique > 0 )
		{
			TCHAR *NecklaceName = (TCHAR*)pclClient->pclItemManager->GetName( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_NECK].siUnique );
			if ( NecklaceName )
			{
				StringCchPrintf( Temp, 256, TEXT("[%s]\r\n"), NecklaceName );
				StringCchCat( Buffer, 256, Temp );	
			}
		}
		
#endif
	
		if ( m_uiLeaseLeftTime > 0 && m_bStopMining == FALSE )
		{
			TCHAR* pText = GetTxtFromMgr(6497);
			TCHAR leftTime[MAX_PATH] = TEXT("");
			StringCchPrintf( leftTime, MAX_PATH, pText, m_uiLeaseLeftTime / 60 , m_uiLeaseLeftTime % 60 );
			StringCchCat( Buffer, MAX_PATH, leftTime );
		}


	m_pExplainStatic->SetText( Buffer, DT_LEFT | DT_VCENTER | DT_VCENTER );

	return;
}

void CNMiningDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	// PCK : 군주S는 기존처럼 랜덤으로 광물표기되게 수정 09.02.23
	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
		{
			SI16 MiningKindList[MAX_MININGDATA_NUM];

			SI16 siMiningItemUnique = 0;

			SI16 siCounter = pclClient->pclMiningMgr->GetMineItemUnique(MiningKindList);

			SI32 GlobalImageIndex = -1;
			SI32 GlobalImageFont = -1;

			if(siCounter > 0)
			{
				if ( kUpdateTimer_DrawImage.IsTimed(GetTickCount()) == true )
				{
					m_siNowFontNum = (m_siNowFontNum+1) % siCounter;
				}

				siMiningItemUnique = MiningKindList[ m_siNowFontNum ];

				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siMiningItemUnique, &GlobalImageIndex,  &GlobalImageFont ) )
				{
					cltClient *pclclient = (cltClient *)pclClient;
					m_pImageStatic->SetFontIndex(GlobalImageFont);
				}
			}
		}
		else
		{
			if(m_siMiningItemUnique)
			{
				SI32 GlobalImageIndex = -1;
				SI32 GlobalImageFont = -1;

				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_siMiningItemUnique, &GlobalImageIndex,  &GlobalImageFont ) )
				{
					cltClient *pclclient = (cltClient *)pclClient;
					TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( GlobalImageIndex );

					if ( pSpr )
					{
						GP.PutSpr(pSpr, ScreenRX + GetX() + 20, ScreenRY + GetY() + 57, GlobalImageFont );
					}
				}
			}
		}
	}
	else
	{
		if ( m_siMiningItemUnique )
		{
			SI32 GlobalImageIndex = -1;
			SI32 GlobalImageFont = -1;

			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_siMiningItemUnique, &GlobalImageIndex,  &GlobalImageFont ) )
			{
				cltClient *pclclient = (cltClient *)pclClient;

				TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( GlobalImageIndex );

				if ( pSpr )
				{
					GP.PutSpr(pSpr, ScreenRX + GetX() + 6, ScreenRY + GetY() + 44, GlobalImageFont );
				}
			}
		}
		else
		{
			SI32 GlobalImageIndex = -1;
			SI32 GlobalImageFont = -1;

			//강제로 석탄을 보여주던것을 상황에 맞게 곡괭이로 변경 [2007.07.25 손성웅]
			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( 10100, &GlobalImageIndex,  &GlobalImageFont ) )
			{
				cltClient *pclclient = (cltClient *)pclClient;

				TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( GlobalImageIndex );

				if ( pSpr )
				{
					GP.PutSpr(pSpr, ScreenRX + GetX() + 6, ScreenRY + GetY() + 44, GlobalImageFont );
				}
			}
		}
	}
}

void CNMiningDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if( RetValue )
			{
				if ( pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() < *((SI32*)pData) )
				{
					TCHAR* pTitle = GetTxtFromMgr(1260);
					TCHAR* pText = GetTxtFromMgr(1261);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, pTitle );
					return;
				}

                // 광산에서 멀리 떨어졌을시에 메세지 출력하고 리턴
				if ( pclClient->pclCM->CR[1]->GetCurrentOrder() != ORDER_MINE )
				{
					TCHAR* pTitle = GetTxtFromMgr(1260);
					TCHAR* pText = GetTxtFromMgr(6164);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
					return;
				}

				SI32 FieldObjectUnique = pclClient->pclCM->CR[1]->pclMap->GetFieldObject_Unique( m_siMinePosX, m_siMinePosY );

				cltGameMsgRequest_MiningPayRentFee clinfo( *((SI32*)pData), FieldObjectUnique );
				cltMsg clMsg(GAMEMSG_REQUEST_MININGPAYRENTFEE, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 1:
		{
			if ( RetValue )
			{
				m_bStopMining = TRUE;
				pclclient->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
				pclclient->SetSiSelectedOrder(ORDER_STOP);
			}

			pclclient->bStopDoingSwitch = FALSE;
		}
		break;
	}
}

void CNMiningDlg::SetMinePos(SI32 PosX, SI32 PosY)
{
	m_siMinePosX = PosX;
	m_siMinePosY = PosY;
	
	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
	}
	else
	{
		m_bStopMining = FALSE;
	}
}

void CNMiningDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		DrawMiningDelay( ScreenRX, ScreenRY );
	}
}

void CNMiningDlg::SetMiningInfo(TCHAR* strMineName, SI16 nMineNum)
{
	TCHAR Buffer[ 1024 ] = TEXT("");
	TCHAR TempBuffer[ 64 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(8401);

	for(SI16 i=0;i<MAX_MINE_NUMBER;i++)
	{
		if ( _tcscmp(m_strMineName[i], TEXT("")) == 0 )
		{
			StringCchCopy( m_strMineName[i], MAX_MINE_NAME_LENGTH, strMineName );
			m_siMineNum[i] = m_siMineNum[i] + nMineNum;
			break;
		}
		else if ( _tcscmp(m_strMineName[i], strMineName) == 0 )
		{
			m_siMineNum[i] += nMineNum;
			break;
		}
	}

	for ( SI16 siCount=0; siCount<MAX_MINE_NUMBER; siCount++ )
	{
		if ( _tcscmp(m_strMineName[siCount], TEXT("")) == 0 )
		{
			break;
		}
		else
		{
			StringCchPrintf( TempBuffer, 64, pText, m_strMineName[siCount], m_siMineNum[siCount]);
			StringCchCat(Buffer, 1024, TempBuffer );
		}
	}

	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		m_pStatusEdit->SetBorder(true);
		m_pStatusEdit->SetBKColorUse(true);
		m_pStatusEdit->SetText(Buffer, DT_WORDBREAK);
	}
	return;
}

BOOL CNMiningDlg::ReturnAutoMining()
{
	return m_bStopMining;
}

void CNMiningDlg::DrawMiningDelay(SI32 rx, SI32 ry)
{
	SI32 x = 93 + GetX() + rx;
	SI32 y = 26 + GetY() + ry;
	SI32 siWidth = 218;
	SI32 siHeight = 20;

	// 총 채광시간을 참조하여 게이지가 올라가는 비율을 구한다.
	REAL32	f32MineRate		= 0.0f;

	f32MineRate = ( pclClient->CurrentClock - m_dwMiningStartClock ) / (m_siDelayTime * 1.0f);

	SI32 siDrawWidth = (SI32)(siWidth * f32MineRate);

	for ( SI32 siCount=0; siCount<siDrawWidth; siCount+=2  )
	{
		if ( (DWORD)(pclClient->CurrentClock - m_dwMiningStartClock) >= (DWORD)m_siDelayTime)
		{
			break;
		}
		GP.PutSpr( &m_SprMineGage, (x+siCount), y );
	}	
}