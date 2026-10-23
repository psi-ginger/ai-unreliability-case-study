#include "../Client/ninterface/NArtifactDlg/NArtifactMixtureDlg.h"
#include "../../Client/NInterface/NNewInventory/NNewInventory.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../CommonLogic/MsgType-Item.h"
#include "..\..\Common\Item\ItemCommon\cltitem.h"
#include "..\Client\Client.h"

NArtifactMixtureDlg::NArtifactMixtureDlg()
{
	//ZeroMemory( m_InventoryPos,			 sizeof( m_InventoryPos ) );
	memset(m_InventoryPos, NO_SLOT, sizeof( m_InventoryPos ) );
	ZeroMemory( m_clArtifactItem,		 sizeof( m_clArtifactItem ) );
	ZeroMemory( m_bCrystalSlotImageList, sizeof( m_bCrystalSlotImageList ) );

	m_Spr[0] = pclClient->GetGlobalSpr( GIMG_ARTIFACT_SLOT_STATUS );
	m_Spr[1] = pclClient->GetGlobalSpr( GIMG_ARTIFACT_GAUGE_BACK  );
	m_Spr[2] = pclClient->GetGlobalSpr( GIMG_ARTIFACT_GAUGE		  );
	m_Spr[3] = pclClient->GetGlobalSpr( GIMG_ARTIFACT_SLOT_EFFECT );

	if(NULL == m_Spr[0] || NULL == m_Spr[1] || NULL == m_Spr[2] || NULL == m_Spr[3])
	{
		MsgBox("이미지 얻기 실패", "NArtifactMixtureDlg");
	}

	m_CurrentSlot   = -1;
	m_siGaugeCount  =  0;
	m_siEffectCount =  0;
	m_bGaugeAction  = false;
	m_bEffectAction = false;
}

NArtifactMixtureDlg::~NArtifactMixtureDlg()
{
	
}

void NArtifactMixtureDlg::Clear( void )
{
	DelAllListViewItem();
	m_CurrentSlot   = -1;
	m_siGaugeCount  =  0;
	m_siEffectCount =  0;
	m_bGaugeAction  = false;
	m_bEffectAction = false;

	CButton* pButton = NULL;
	pButton = m_InterfaceFactory.GetButton(ARTIFACT_MIXTURE_BUTTON_MIXTURE);
	pButton->Enable(true);
}

void NArtifactMixtureDlg::Create(  CControllerMgr *pParent  )
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NArtifactDlg/NArtifactMixtureDlg.ddf" ) );
	file.CreateChildDialog( this, ARTIFACT_MIXTURE_DLG, TEXT( "dialog_mixture" ), StaticNArtifactMixtureDlgProc, pParent );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,   ARTIFACT_MIXTURE_BUTTON_MIXTURE,				    		this ), ARTIFACT_MIXTURE_BUTTON_MIXTURE,							TEXT( "button_mixture" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_MIXTURE_LISTVIEW_EQUIP_REGISTER_ITEM,			this ), ARTIFACT_MIXTURE_LISTVIEW_EQUIP_REGISTER_ITEM,				TEXT( "listview_equip_register_Item" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT1,			this ), ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT1,			TEXT( "listview_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT2,			this ), ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT2,			TEXT( "listview_NONAME2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT3,			this ), ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT3,			TEXT( "listview_NONAME3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT4,			this ), ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT4,			TEXT( "listview_NONAME4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT5,			this ), ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT5,			TEXT( "listview_NONAME5" ) );	
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	  ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT1,				this ), ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT1,				TEXT( "editbox_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,     ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT2,				this ), ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT2,				TEXT( "editbox_NONAME2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,     ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT3,				this ), ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT3,				TEXT( "editbox_NONAME3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,     ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT4,				this ), ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT4,				TEXT( "editbox_NONAME4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,     ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT5,				this ), ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT5,				TEXT( "editbox_NONAME5" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,     ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT6,				this ), ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT6,				TEXT( "editbox_NONAME6" ) );

	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	  ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT1,			this ), ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT1,				TEXT( "statictext_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT2,			this ), ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT2,				TEXT( "statictext_NONAME2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT3,			this ), ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT3,				TEXT( "statictext_NONAME3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT4,			this ), ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT4,				TEXT( "statictext_NONAME4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT5,			this ), ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT5,				TEXT( "statictext_NONAME5" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT6,			this ), ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT6,				TEXT( "statictext_NONAME6" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT7,			this ), ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT7,				TEXT( "statictext_NONAME7" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT8,			this ), ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT8,				TEXT( "statictext_NONAME8" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_MIXTURE_OUTLINE_LINE1,				   		 	this ), ARTIFACT_MIXTURE_OUTLINE_LINE1,								TEXT( "outline_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_MIXTURE_OUTLINE_LINE2,				   		 	this ), ARTIFACT_MIXTURE_OUTLINE_LINE2,								TEXT( "outline_NONAME2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_MIXTURE_OUTLINE_LINE3,				   		 	this ), ARTIFACT_MIXTURE_OUTLINE_LINE3,								TEXT( "outline_NONAME3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_MIXTURE_OUTLINE_LINE4,				   		 	this ), ARTIFACT_MIXTURE_OUTLINE_LINE4,								TEXT( "outline_NONAME4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_MIXTURE_OUTLINE_LINE5,				   		 	this ), ARTIFACT_MIXTURE_OUTLINE_LINE5,								TEXT( "outline_NONAME5" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_MIXTURE_OUTLINE_LINE6,				   		 	this ), ARTIFACT_MIXTURE_OUTLINE_LINE6,								TEXT( "outline_NONAME6" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_MIXTURE_OUTLINE_LINE7,				   		 	this ), ARTIFACT_MIXTURE_OUTLINE_LINE7,								TEXT( "outline_NONAME7" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_MIXTURE_OUTLINE_LINE8,				   		 	this ), ARTIFACT_MIXTURE_OUTLINE_LINE8,								TEXT( "outline_NONAME8" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_MIXTURE_OUTLINE_LINE9,				   		 	this ), ARTIFACT_MIXTURE_OUTLINE_LINE9,								TEXT( "outline_NONAME9" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_MIXTURE_OUTLINE_LINE10,				   		 	this ), ARTIFACT_MIXTURE_OUTLINE_LINE10,							TEXT( "outline_NONAME10" ) );	
	
	CStatic* pStatic = NULL;
	pStatic = m_InterfaceFactory.GetStatic(ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT7);
	pStatic->SetTextPrintOption(DT_CENTER);
	pStatic->Refresh();
	pStatic = m_InterfaceFactory.GetStatic(ARTIFACT_MIXTURE_STATIC_CRYSTAL_REGISTER_SLOT8);
	pStatic->SetTextPrintOption(DT_CENTER);
	pStatic->Refresh();

	SetActionTimer( 10 );    
	SetTimerOnOff( true );
}

void CALLBACK NArtifactMixtureDlg::StaticNArtifactMixtureDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NArtifactMixtureDlg *pThis = (NArtifactMixtureDlg*) pControllerMgr;
	pThis->NArtifactMixtureDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NArtifactMixtureDlg::NArtifactMixtureDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	if(m_bGaugeAction || m_bEffectAction)
		return;

	cltClient*  pclclient     = (cltClient *)pclClient;
	void*	    pvoidControl  = m_InterfaceFactory.GetControl( nControlID );
	bool		bSlot		  = false;
	bool		bDelSlot	  = false;

	switch ( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;	
		case ARTIFACT_MIXTURE_BUTTON_MIXTURE:
			{
				bool bPos = false;

				// 결정체 슬롯에 단하나의 결정체라도 있어야 한다. 결정체는 1부터다.
				for(SI16 Count = 1; Count <TOTAL_SLOT_COUNT ;Count++)
				{
					if(m_InventoryPos[Count] > 0)
						bPos = true;
				}

				// 장비가 아바타 아이템이라면.
				if( (cltItemManagerCommon::SLOTITEMTYPE_AVATAR == pclClient->pclItemManager->UsedSlotItem( m_clArtifactItem[EQUIP_SLOT].siUnique  ) )
					&& bPos == true)
				{
					TCHAR* pTitle = GetTxtFromMgr(30295);
					TCHAR* pText  = GetTxtFromMgr(30294);

					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 1 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
				else if(bPos == true)
				{
					if(false == IsMixture())
						return; 

					m_bGaugeAction = true;
					//SendServerMsg( GAMEMSG_REQUEST_ARTIFACT_MIXTURE );
				}
				else
					return; 
			}
			break; 
		case ARTIFACT_MIXTURE_LISTVIEW_EQUIP_REGISTER_ITEM:
			{
				m_CurrentSlot = EQUIP_SLOT;

				switch( nEvent )
				{
					case EVENT_LISTVIEW_RBUTTONDOWN:
						{
							DelAllListViewItem();
						}
						break;
				}
			}
			break;
		case ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT1:
			{
				m_CurrentSlot = CRYSTAL_SLOT1;

				switch( nEvent )
				{
				case EVENT_LISTVIEW_RBUTTONDOWN:
					{
						if( TRUE == m_bCrystalSlotImageList[CRYSTAL_SLOT1] )
							return;	

						DelListViewItem((CListView*)pvoidControl, m_CurrentSlot);
					}
					break;
				}
			}
			break;
		case ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT2:
			{
				m_CurrentSlot = CRYSTAL_SLOT2;

				switch( nEvent )
				{
				case EVENT_LISTVIEW_RBUTTONDOWN:
					{
						if( TRUE == m_bCrystalSlotImageList[CRYSTAL_SLOT2] )
							return;	

						DelListViewItem((CListView*)pvoidControl, m_CurrentSlot);
					}
					break;
				}
			}
			break;
		case ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT3:
			{
				m_CurrentSlot = CRYSTAL_SLOT3;

				switch( nEvent )
				{
				case EVENT_LISTVIEW_RBUTTONDOWN:
					{
						if( TRUE == m_bCrystalSlotImageList[CRYSTAL_SLOT3] )
							return;	

						DelListViewItem((CListView*)pvoidControl, m_CurrentSlot);
					}
					break;
				}
			}
			break;
		case ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT4:
			{
				m_CurrentSlot = CRYSTAL_SLOT4;

				switch( nEvent )
				{
				case EVENT_LISTVIEW_RBUTTONDOWN:
					{
						if( TRUE == m_bCrystalSlotImageList[CRYSTAL_SLOT4] )
							return;	

						DelListViewItem((CListView*)pvoidControl, m_CurrentSlot);
					}
					break;
				}
			}
			break;
		case ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT5:
			{
				m_CurrentSlot = CRYSTAL_SLOT5;

				switch( nEvent )
				{
				case EVENT_LISTVIEW_RBUTTONDOWN:
					{
						if( TRUE == m_bCrystalSlotImageList[CRYSTAL_SLOT5] )
							return;	

						DelListViewItem((CListView*)pvoidControl, m_CurrentSlot);
					}
					break;
				}
			}
			break;
	}
}


void NArtifactMixtureDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 1:  // 아바타 결정체의 경우 한번 물어본다.
		{
			if(RetValue)
			{
				if(false == IsMixture())
					return; 

				m_bGaugeAction = true;
			}
		}
		break;
	}
}



void NArtifactMixtureDlg::Action(void)
{
	// 게이지 효과 종료.
	if(GAGE_NUM <= m_siGaugeCount)
	{
		m_bGaugeAction = false;
		m_siGaugeCount = 0;

		m_bEffectAction = true;
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_ATIFACT")), 0, 0);
	}
	// 이펙트 효과 종료.
	if(EFFECT_NUM <= m_siEffectCount)
	{
		m_bEffectAction = false;
		m_siEffectCount = 0;

		SendServerMsg( GAMEMSG_REQUEST_ARTIFACT_MIXTURE ); 

		if( 0 < m_clMixtureSuccessItem.siUnique)
		{ 
			MixtureSuccess(&m_clMixtureSuccessItem);  
		}
	}  
}

void NArtifactMixtureDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	// 등록된 장비가 없으면 그릴것도 없다.
	if( 0 >= m_clArtifactItem[EQUIP_SLOT].siUnique )  
		return;

	//게이지 효과 그리기
	if(true == m_bGaugeAction)
	{
		COutLine* pLineOut = m_InterfaceFactory.GetOutLine( ARTIFACT_MIXTURE_OUTLINE_LINE4);

		GP.PutSprAlpha(m_Spr[1],  GetX() + pLineOut->GetX() +  ScreenRX, GetY() + pLineOut->GetY() + ScreenRY, 8, 0, 0 );

		// 게이지 앞,뒤 이미지.
		GP.PutSpr(m_Spr[2], GetX() + pLineOut->GetX() +		  ScreenRX, GetY() + pLineOut->GetY() + ScreenRY, 0);
		GP.PutSpr(m_Spr[2], GetX() + pLineOut->GetX() + 306 + ScreenRX, GetY() + pLineOut->GetY() + ScreenRY, 2);

		for(SI32 Count = 0; Count < m_siGaugeCount; ++Count)  
		{
			GP.PutSpr(m_Spr[2], GetX() + pLineOut->GetX() + ScreenRX + 2 + (Count * 2), GetY() + pLineOut->GetY() + ScreenRY, 1 );
		}       
		m_siGaugeCount++;
	}
	
	CListView* pListView = NULL;
	SI32		X_Add = 0;
	SI32		Y_Add = 0;        
	SI32		Alpha = 0;
	for( SI32 CrystalCount = 0; CrystalCount < MAX_CRYSTAL_NUM; ++CrystalCount)
	{	
		pListView = m_InterfaceFactory.GetListView( ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT1 + CrystalCount );

		X_Add = GetX() + ScreenRX + pListView->GetX();
		Y_Add = GetY() + ScreenRY + pListView->GetY();
            
		   	
		if( 0 < m_clArtifactItem[EQUIP_SLOT].cltItemCrytal[CrystalCount].siCrystalType )
		{
			//KHY - 0805 - 아트펙스 수정.
			//GP.PutSprAlpha(m_Spr[0], X_Add, Y_Add, 10, 2 );
			// 이펙트 효과 그리기..
			if((true == m_bEffectAction) && (m_InventoryPos[CrystalCount+1] >0))
			{
				GP.PutSprScreen(m_Spr[3], X_Add, Y_Add, m_siEffectCount );
			}

		}   
		else if( 0			 >= m_clArtifactItem[EQUIP_SLOT].cltItemCrytal[CrystalCount].siCrystalType &&
			CrystalCount <  m_clArtifactItem[EQUIP_SLOT].siUseCrystalNum )
		{ 
			// 조합할 결정체가 들어갔다면 반투명으로 하자.
			//KHY - 0805 - 아트펙스 수정. - 반투명 없음.
			if( 0 < m_clArtifactItem[CrystalCount+1].siUnique )
			{
				Alpha = 0;    
			}
			else
			{
				Alpha = 16;  
			}
			GP.PutSprAlpha(m_Spr[0], X_Add, Y_Add, Alpha, 1  );

			//SI16 cCount = 0;

			// 첫번째 장비슬롯의 경우만 예외로 두고, 다른 슬롯의 경우는 계산한다.
			//if(CrystalCount == 0 )
			//	cCount = 1;
			//else
			//	cCount = CrystalCount;

			//if( 0 < m_clArtifactItem[cCount].siUnique )
			if( 0 < m_clArtifactItem[CrystalCount+1].siUnique )
			{
				// 이펙트 효과 그리기..
				if(true == m_bEffectAction)
				{
					GP.PutSprScreen(m_Spr[3], X_Add, Y_Add, m_siEffectCount );
				}
			}
		} 
		else 
		{
			GP.PutSprAlpha(m_Spr[0], X_Add, Y_Add, 13, 0 );
		}   
	}  
	
	// 이펙트 효과 카운트.
	if(true == m_bEffectAction)
	{
		static DWORD StartTickCount = GetTickCount();
			   DWORD EndTickCount	= GetTickCount();
		
		if(50 < EndTickCount - StartTickCount) 
		{ 
			StartTickCount = EndTickCount; 

			m_siEffectCount++;  
		}
	}
}

bool NArtifactMixtureDlg::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	if(m_bGaugeAction || m_bEffectAction)
		return true;

	cltClient* pclclient = (cltClient*)pclClient;

	if(pclClickAndDrop == NULL)										return false;
	if(pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false)	return false;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;

	void*	pvoidControl  = m_InterfaceFactory.GetControl( pclClickAndDrop->m_siDstControlID );

	// 이미 슬롯에 올린 인벤의 아이템이 인가?
	if( true == SameCrystalInvenItem(pclClickAndDrop->m_siPara1) )
	{
		NotifyMsg(cltItemManagerCommon::ALREADY_USED_INVEN_ITEM);
		return true;
	}

	switch ( pclClickAndDrop->m_siDstControlID )
	{
		case ARTIFACT_MIXTURE_LISTVIEW_EQUIP_REGISTER_ITEM:
			{
				// 조합이 가능한 아이템인지 검사.
				if( false == CanMixure( pclClickAndDrop_Item->GetItem()->siUnique ) )
				{
					return false;
				}

				DelAllListViewItem();

				m_CurrentSlot				 = EQUIP_SLOT;
				m_InventoryPos[ EQUIP_SLOT ] = 	pclClickAndDrop->m_siPara1;

				if( false == InsertEquip( (CListView*)pvoidControl, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot				 = NO_SLOT;
					m_InventoryPos[ EQUIP_SLOT ] = 	NO_SLOT;
				}
			}
			break;
		case ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT1:
			{
				m_CurrentSlot					= CRYSTAL_SLOT1;
				m_InventoryPos[ CRYSTAL_SLOT1 ] = pclClickAndDrop->m_siPara1;

				if( false == InsertCrystal( (CListView*)pvoidControl, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot				    = NO_SLOT;
					m_InventoryPos[ CRYSTAL_SLOT1 ] = 	NO_SLOT;
				}
			}
			break;
		case ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT2:
			{
				m_CurrentSlot					= CRYSTAL_SLOT2;
				m_InventoryPos[ CRYSTAL_SLOT2 ] = pclClickAndDrop->m_siPara1;

				if( false == InsertCrystal( (CListView*)pvoidControl, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot					= NO_SLOT;
					m_InventoryPos[ CRYSTAL_SLOT2 ] = 	NO_SLOT;
				}
			}
			break;
		case ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT3:
			{
				m_CurrentSlot					= CRYSTAL_SLOT3;
				m_InventoryPos[ CRYSTAL_SLOT3 ] = pclClickAndDrop->m_siPara1;

				if( false == InsertCrystal( (CListView*)pvoidControl, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot					= NO_SLOT;
					m_InventoryPos[ CRYSTAL_SLOT3 ] = 	NO_SLOT;
				}
			}
			break;
		case ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT4:
			{
				m_CurrentSlot					= CRYSTAL_SLOT4;
				m_InventoryPos[ CRYSTAL_SLOT4 ] = pclClickAndDrop->m_siPara1;

				if( false == InsertCrystal( (CListView*)pvoidControl, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot					= NO_SLOT;
					m_InventoryPos[ CRYSTAL_SLOT4 ] = 	NO_SLOT;
				}
			}
			break;
		case ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT5:
			{
				m_CurrentSlot					= CRYSTAL_SLOT5;
				m_InventoryPos[ CRYSTAL_SLOT5 ] = pclClickAndDrop->m_siPara1;

				if( false == InsertCrystal( (CListView*)pvoidControl, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot					= NO_SLOT;
					m_InventoryPos[ CRYSTAL_SLOT5 ] = 	NO_SLOT;
				}
			}
			break;
	}

	return true;
}

void NArtifactMixtureDlg::SendServerMsg( SI32 _MsgType )
{
	SI32	MsgSize	= 0;
	void*	pclInfo = NULL;
	
	switch( _MsgType )
	{
	case GAMEMSG_REQUEST_ARTIFACT_MIXTURE:
		{
			// 조합이 가능한 아이템인지 검사.
			if( false == CanMixure( m_clArtifactItem[EQUIP_SLOT].siUnique ) )   
				return;

			// 사용가능한 슬롯이 비어있는 개수가 0개면 리턴.
			//if( 0 == m_clArtifactItem[EQUIP_SLOT].EmptyCrystalSlotCount() )
				//return;

			pclInfo = new cltGameMsgRequest_Artifact_Mixture;
			
			cltGameMsgRequest_Artifact_Mixture* pclMixtureInfo = (cltGameMsgRequest_Artifact_Mixture*)pclInfo;

			MsgSize = sizeof( cltGameMsgRequest_Artifact_Mixture );

			pclMixtureInfo->m_siEquipItemPos = m_InventoryPos[ EQUIP_SLOT ];					// 장비의 인벤 위치.

			// 장비 아이템 복사.
			pclMixtureInfo->m_clEquipItem = m_clArtifactItem[ EQUIP_SLOT ];									// 조합되는 아이템의 위치.(장비류가 되겠지..)

			CListView*  _pListView = NULL;
			SI32		InvIndex   = 0;

			//KHY - 20090526 - 아바타 결정체. - 아바타는 누적 시키기 때문에 이미 있는건 보내면 안된다.			
			if( cltItemManagerCommon::SLOTITEMTYPE_AVATAR == pclClient->pclItemManager->UsedSlotItem( m_clArtifactItem[EQUIP_SLOT].siUnique  ) )
			{
				for(SI32 Count = 0; Count < pclMixtureInfo->m_clEquipItem.siUseCrystalNum ; ++Count)
				{
					// 이미 조합된 결정체는 제외하고 결합되지 않은 결정체만 보낸다. - 결정체 아이템의 등록여부는, 인벤 위치가 있는지 여부로.
					if( m_clArtifactItem[Count+1].siUnique > 0 && m_InventoryPos[ Count + 1 ] > 0 )
					{
						pclMixtureInfo->m_siCrystalItemPos[ Count ]  = m_InventoryPos[ Count + 1 ];		// 결정체의 인벤 위치.
						pclMixtureInfo->m_clCrystalItem[ Count ]	 = m_clArtifactItem[ Count +1 ];	// 결정체 아이템.
						pclMixtureInfo->m_clCrystalItem[ Count ].siItemNum	 = 1;	// 무조건 1개만 없앤다.
					}
				}
			}
			else
			{
				for(SI32 Count = 0; Count < pclMixtureInfo->m_clEquipItem.siUseCrystalNum ; ++Count)
				{
					// 이미 조합된 결정체는 제외하고 결합되지 않은 결정체만 보낸다.
					//if( 0 >= pclMixtureInfo->m_clEquipItem.cltItemCrytal[Count].GetCrystalType() )
					{
						pclMixtureInfo->m_siCrystalItemPos[ Count ]  = m_InventoryPos[ Count + 1 ];		// 결정체의 인벤 위치.
						pclMixtureInfo->m_clCrystalItem[ Count ]	 = m_clArtifactItem[ Count +1 ];	// 결정체 아이템.
						pclMixtureInfo->m_clCrystalItem[ Count ].siItemNum	 = 1;	// 무조건 1개만 없앤다.
					}
				}
			}

			// 결합이 가능한 정보들인지 최종 검사.
			SI32 siResult = pclClient->pclItemManager->CanCrystalMixtureData( &pclMixtureInfo->m_clEquipItem, pclMixtureInfo->m_clCrystalItem ,pclMixtureInfo->m_siEquipItemPos);
			switch( siResult )
			{
				case cltItemManagerCommon::SAME_CRYSTAL:			NotifyMsg(cltItemManagerCommon::SAME_CRYSTAL);		return;
				case cltItemManagerCommon::NO_ITEM_UNIQUE:			NotifyMsg(cltItemManagerCommon::NO_ITEM_UNIQUE);	return;
				case cltItemManagerCommon::NO_SLOT_ITEM:			NotifyMsg(cltItemManagerCommon::NO_SLOT_ITEM);		return;
				case cltItemManagerCommon::NO_USED_SLOT:			NotifyMsg(cltItemManagerCommon::NO_USED_SLOT);		return;
				case cltItemManagerCommon::NO_CRYSTAL:				NotifyMsg(cltItemManagerCommon::NO_CRYSTAL);		return;
				case cltItemManagerCommon::ALL_EMPTY_CRYSTAL:		NotifyMsg(cltItemManagerCommon::ALL_EMPTY_CRYSTAL);	return;
				case cltItemManagerCommon::ALREADY_USE_SLOT:		NotifyMsg(cltItemManagerCommon::ALREADY_USE_SLOT);	return;
				case cltItemManagerCommon::LIMIT_CRYSTAL_ABILITY:	NotifyMsg(cltItemManagerCommon::LIMIT_CRYSTAL_ABILITY);	return;
			}
			CButton* pButton = NULL;
			pButton = m_InterfaceFactory.GetButton(ARTIFACT_MIXTURE_BUTTON_MIXTURE);
			pButton->Enable(false);
		}
		break;
	}

	cltMsg clMsg( _MsgType, MsgSize, (BYTE*)pclInfo );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

	DeleteBuffer( _MsgType, pclInfo );
}






bool NArtifactMixtureDlg::InsertEquip( CListView* _pListView, cltItem* _cltItem )
{
	// 슬롯 범위 조사.
	if( false == CanInsertEquip() )
	{
		return false;
	}

	SI32 GlobalImageIndex	= -1;
	SI32 GlobalImageFont	= -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;

	m_clArtifactItem[ m_CurrentSlot ].Set( _cltItem );			

	// 장비슬롯에 아이템 넣기.
	if( m_clArtifactItem[ m_CurrentSlot ].siUnique > 0 )
	{
		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( _cltItem->siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( _cltItem->siUnique );

			if (ItemName)
			{
				pclClient->pclItemManager->GetExplain( _cltItem, ToolTipMsg );

				ListViewItemData.Init();
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	_cltItem->siItemNum, ItemName, ToolTipMsg );

				_pListView->DeleteAllItems();
				_pListView->InsertItem( &ListViewItemData );   

				EquipExplain( _cltItem );
			}
		}   
	}

	//장비에 관한 슬롯 정보를 세팅한다.
	InsertEquipSetCrystal();

	return true;
}

void NArtifactMixtureDlg::InsertEquipSetCrystal( void )
{
	ZeroMemory( m_bCrystalSlotImageList, sizeof( m_bCrystalSlotImageList ) );

	CEdit*		pEdit				= NULL;
	CListView*	pListView			= NULL;
	SI32		GlobalImageIndex	= -1;
	SI32		GlobalImageFont		= -1;

	cltItem				CrystalcltItem;
	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;
	SI32				siCrystalSlot = -1;
	for( SI32 Count = 0; Count < m_clArtifactItem[ m_CurrentSlot ].siUseCrystalNum; ++Count )
	{
		// 슬롯에 결정체가 있으면....
		if( 0 < m_clArtifactItem[ m_CurrentSlot ].cltItemCrytal[ Count ].siCrystalType )
			m_bCrystalSlotImageList[CRYSTAL_SLOT1 + Count] = true;
		
		pListView = m_InterfaceFactory.GetListView( ARTIFACT_MIXTURE_LISTVIEW_CRYSTAL_REGISTER_SLOT1 + Count );
		if(NULL == pListView)
			continue;

		// 리스트 뷰를 채워준다.
		CrystalcltItem.siUnique  = m_clArtifactItem[ m_CurrentSlot ].cltItemCrytal[ Count ].siCrystalType;
		if(0 < CrystalcltItem.siUnique )
		{
			CrystalcltItem.siItemNum = 1;
		}
		//CrystalcltItem.clItemCrystal.siDurability		 = m_clArtifactItem[ EQUIP_SLOT ].cltItemCrytal[ Count ].GetDurability();
		CrystalcltItem.clItemCrystal.siMaxDurability	 = m_clArtifactItem[ EQUIP_SLOT ].cltItemCrytal[ Count ].GetMaxDurability();
		CrystalcltItem.clItemCrystal.uiCrystalAbility	 = m_clArtifactItem[ EQUIP_SLOT ].cltItemCrytal[ Count ].uiCrystalAbility;
		CrystalcltItem.clItemCrystal.uiCrystalAbilityNum = m_clArtifactItem[ EQUIP_SLOT ].cltItemCrytal[ Count ].uiCrystalAbilityNum;
		

		// 결정체가 들어갈 슬롯에 결정체 그리기.
		siCrystalSlot = CRYSTAL_SLOT1 + Count;
		
		m_clArtifactItem[ siCrystalSlot ].Set( &CrystalcltItem );			

		if( m_clArtifactItem[ siCrystalSlot ].siUnique > 0 )
		{
			if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clArtifactItem[ siCrystalSlot ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clArtifactItem[ siCrystalSlot ].siUnique );

				if(ItemName)
				{
					pclClient->pclItemManager->GetExplain( &m_clArtifactItem[ siCrystalSlot ], ToolTipMsg );

					ListViewItemData.Init();
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	m_clArtifactItem[ siCrystalSlot ].siItemNum, ItemName, ToolTipMsg );

					pListView->DeleteAllItems();
					pListView->InsertItem( &ListViewItemData );   

					pEdit = m_InterfaceFactory.GetEdit(ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT1 + Count);
					
					// 결정체 아이템 정보 쓰기. 
					CrystalExplain( pEdit, &m_clArtifactItem[ EQUIP_SLOT ], Count);  
				}
			}
		}
	}
}


void NArtifactMixtureDlg::EquipExplain( cltItem* pEquip )
{
	CEdit*	pEdit		= m_InterfaceFactory.GetEdit( ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT6 );	
	TCHAR szbuffer[512] = "";

	SI32 MixtureSlotNum = 0;
	for(SI32 Count = 0; Count < pEquip->GetUseCrystalNum(); ++Count)
	{
		if(0 < pEquip->cltItemCrytal[Count].GetCrystalType())
			++MixtureSlotNum;
	}

	SI32	ref			  = pclClient->pclItemManager->FindItemRefFromUnique(pEquip->siUnique);	
	SI32	siLevel		  = pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel;
	SI32    temCrystalNum = 0;
	if((siLevel >= 1)&&(siLevel <= 29))
	{
		temCrystalNum = 2;
	}
	else if((siLevel >= 30)&&(siLevel <= 59))
	{
		temCrystalNum = 3;
	}
	else if((siLevel >= 60)&&(siLevel <= 89))
	{
		temCrystalNum = 4;
	}
	else if(siLevel >= 90)
	{
		temCrystalNum = 5;
	}

	//[진성] 아바타 결정체. 사용가능한 최대 슬롯의 수가 3개이다.
	if( cltItemManagerCommon::SLOTITEMTYPE_AVATAR == pclClient->pclItemManager->UsedSlotItem( pEquip->siUnique ) )
		temCrystalNum = 3;


	//TCHAR* ItemName = (TCHAR*)pclClient->pclItemManager->GetName( pEquip->siUnique );
	//
	//StringCchPrintfA( szbuffer, sizeof( szbuffer ), GetTxtFromMgr(8378), ItemName,
	//																	 //MixtureSlotNum,
	//																	 pEquip->GetUseCrystalNum(),
	//																	 temCrystalNum, 
	//																	 MixtureSlotNum  );
	//pEdit->SetText( szbuffer );  


	NTCHARString256 strText = GetTxtFromMgr(8378);

	NTCHARString256 strName = (TCHAR*)pclClient->pclItemManager->GetName( pEquip->siUnique );	// 결정체 이름
	strText.Replace(TEXT("#ItemName#"), strName );

	SI16 strCurrentSlot = pEquip->GetUseCrystalNum();		// 현재 슬롯 수 
	strText.Replace(TEXT("#CurrentSlotNum#"), SI16ToString(strCurrentSlot) );

	SI32 strTotalSlot = temCrystalNum;						// 토탈 슬롯 수
	strText.Replace(TEXT("#TotalSlotNum#"), SI32ToString(strTotalSlot) );

	SI32 strMixureCrystalNum = MixtureSlotNum;				// 조합된 결정체
	strText.Replace(TEXT("#MitureCrystalNum#"), SI32ToString(strMixureCrystalNum) );

	pEdit->SetText(strText);
}

void NArtifactMixtureDlg::CrystalExplain( CEdit* _pExplainEdit, cltItem* pEquipItem, SI16 _siSlot )
{
	TCHAR szbuffer[512]				= "";
	TCHAR buf[64]					= "";
	TCHAR CrystalReturnString[50]	= "";
	
	((cltItemManagerClient*)pclClient->pclItemManager)->GetCrystalAbilityTypeName(	buf, 
																					pEquipItem->cltItemCrytal[_siSlot].GetCrystalAblilty(), 
																					pEquipItem->cltItemCrytal[_siSlot].GetCrystalAbilityNum(), 
																					CrystalReturnString );

	BOOL bAvatarItem = ((cltItemManagerClient*)pclClient->pclItemManager)->IsAbatarCrystalItem(  pEquipItem->cltItemCrytal[_siSlot].GetCrystalAblilty() )	;

	if( bAvatarItem== TRUE)
	{
		StringCchPrintfA( szbuffer, sizeof( szbuffer ), "%s", 	CrystalReturnString);
	}
	else
	{
		StringCchPrintfA( szbuffer, sizeof( szbuffer ), "%s\r\n %d / %d", 	CrystalReturnString,
																		pEquipItem->cltItemCrytal[_siSlot].GetDurability() / 300,
																		pEquipItem->cltItemCrytal[_siSlot].GetMaxDurability() / 300);
	}

	_pExplainEdit->SetText( szbuffer );
}


void NArtifactMixtureDlg::EquipCrystalExplain( CEdit* _pExplainEdit, cltItem* pEquipCrystal, SI16 _siSlot )
{
	TCHAR szbuffer[512]				= "";
	TCHAR buf[64]					= "";
	TCHAR CrystalReturnString[50]	= "";


	if(pEquipCrystal->clItemCrystal.uiCrystalAbility > 0)
	{	
		//KHY - 20090526 - 아바타 결정체. - 내구력 표시가 없다.
		if(pclClient->pclItemManager->IsAbatarCrystalItem(pEquipCrystal->clItemCrystal.GetCrystalAblilty()) == true)
		{
			((cltItemManagerClient*)pclClient->pclItemManager)->GetCrystalAbilityTypeName(	buf, 
																							pEquipCrystal->clItemCrystal.GetCrystalAblilty(), 
																							pEquipCrystal->clItemCrystal.GetCrystalAbilityNum(), 
																							CrystalReturnString );		

			StringCchPrintfA( szbuffer, sizeof( szbuffer ), "%s", 	CrystalReturnString); 

		}
		else
		{
			((cltItemManagerClient*)pclClient->pclItemManager)->GetCrystalAbilityTypeName(	buf, 
																							pEquipCrystal->clItemCrystal.GetCrystalAblilty(), 
																							pEquipCrystal->clItemCrystal.GetCrystalAbilityNum(), 
																							CrystalReturnString );
			
			
			StringCchPrintfA( szbuffer, sizeof( szbuffer ), "%s\r\n %d / %d", 	CrystalReturnString,
																				pEquipCrystal->clItemCrystal.GetMaxDurability() / 300,
																				pEquipCrystal->clItemCrystal.GetMaxDurability() / 300); 
		}
	}
	else
	{
		// 아직 능력이 없는 결정체는 이름을 출력하고 만다.
		TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( pEquipCrystal->siUnique);

		StringCchPrintfA( szbuffer, sizeof( szbuffer ), "%s\r\n", 	ItemName); 
	}	

	_pExplainEdit->SetText( szbuffer );
}

bool NArtifactMixtureDlg::InsertCrystal( CListView* _pListView, cltItem* _cltItem )
{
	//ATB로 결정체 걸러내기
	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(_cltItem->siUnique);
	if(ref <= 0)
		return false;

	if(pclClient->pclItemManager->pclItemInfo[ref]->siSetItemIndex > 0) 
		return false ;

	// 분리이나 제거용 결정체가 아니어야한다.
	if(pclClient->pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_CRYSTAL_SLOT_CREATE) ||
	   pclClient->pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_CRYSTAL_REMOVE)			)
	{
	   NotifyMsg(cltItemManagerCommon::NO_CRYSTAL);
	   return false;
	}

	// 장비가 없으면 슬롯에 결정체도 못넣는다.
	if( 0 >= m_clArtifactItem[ EQUIP_SLOT ].siUnique )
	{
		NotifyMsg(cltItemManagerCommon::NO_ITEM_UNIQUE);		
		return false;
	}

	// 슬롯 범위 조사.
	if( false == CanInsertCrystal( &m_clArtifactItem[ EQUIP_SLOT ] ) )
	{
		return false;
	}
	
	if( false == IsCristal( _cltItem->siUnique ) )
	{
		return false;
	}

	
	//KHY - 20090526 - 아바타 결정체. - 아바타 결정체 조합여부에 관해서 처리하자.
	if( cltItemManagerCommon::SLOTITEMTYPE_AVATAR == pclClient->pclItemManager->UsedSlotItem( m_clArtifactItem[ EQUIP_SLOT ].siUnique ) )  // equip 시키려는 아이템이 아바타 아이템이다.
	{
		bool bAvatar = false;
		
		if(pclClient->pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_CRYSTAL_AVATAR))
		{
			if(m_CurrentSlot >0
)
			{
				// 아바타 결정체는 같은 능력의 결정체가 다른 슬롯에 있으면 안된다.
				if((m_clArtifactItem[ EQUIP_SLOT ].cltItemCrytal[m_CurrentSlot-1].uiCrystalAbility ==  _cltItem->GetCrystalAbility()) // 능력이 같던가
				||(m_clArtifactItem[ m_CurrentSlot ].siUnique == 0)) // 비어 있던가
				{
					// 아이템의 타입과 결정체의 능력에 따라 최대값이 다르다.
					SI32 maxAvatarCrystalAbility = pclClient->pclItemManager->GetMaxAbatarCrystalAbilityNum(m_clArtifactItem[ EQUIP_SLOT ].GetType( pclClient->pclItemManager) , _cltItem->GetCrystalAbility());

					// 최대값이 없다면, 조합불가한 아이템이다.
					if(maxAvatarCrystalAbility >0 )
					{
						UI08 lastCrystalAblitynum = 	m_clArtifactItem[ EQUIP_SLOT ].cltItemCrytal[m_CurrentSlot-1].uiCrystalAbilityNum;
						UI08 newCrystalAblitynum = 	_cltItem->GetCrystalAbilityNum();

						if((lastCrystalAblitynum + newCrystalAblitynum) <= maxAvatarCrystalAbility)
							bAvatar = true;						
					}
				}
			}
		}

		if(bAvatar == false)
			return false;

	}

	
	//// 이미 장비 아이템에서 사용중인 결정체가 있다. 유니크는 조합에 영향을 주지 않는다.
	//if( true == SameEquipCrystalItem(_cltItem) )
	//{
	//	NotifyMsg(cltItemManagerCommon::SAME_CRYSTAL);
	//	return false; 
	//}

	// 이미 장비 아이템에서 같은 능력을로 사용중인 결정체가 있다.
	//KHY - 0805 - 아트펙스 수정.
	/*if(true == SameEquipCrystalAbility(_cltItem))
	{
		NotifyMsg(cltItemManagerCommon::SAME_CRYSTAL);
		return false;
	}
	*/
	/*
	if(false == IsAbilityType(&m_clArtifactItem[ EQUIP_SLOT ], _cltItem->GetCrystalAbility()))
	{
		NotifyMsg(cltItemManagerCommon::LIMIT_CRYSTAL_ABILITY);
		return false;
	}
	*/

	if(false == Islevel( m_clArtifactItem[ EQUIP_SLOT ].siUnique,  _cltItem->siUnique))
	{
		NotifyMsg(cltItemManagerCommon::LIMIT_LEVEL);
		return false;
	}

	
	CEdit*		pEdit				= NULL;
	SI32 GlobalImageIndex	= -1;
	SI32 GlobalImageFont	= -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;

	m_clArtifactItem[ m_CurrentSlot ].Set( _cltItem );			
	
	if( m_clArtifactItem[ m_CurrentSlot ].siUnique > 0 )
	{
		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clArtifactItem[ m_CurrentSlot ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clArtifactItem[ m_CurrentSlot ].siUnique );

			if (ItemName)
			{
				pclClient->pclItemManager->GetExplain( &m_clArtifactItem[ m_CurrentSlot ], ToolTipMsg );

				ListViewItemData.Init();
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	m_clArtifactItem[ m_CurrentSlot ].siItemNum, ItemName, ToolTipMsg );
			
				_pListView->DeleteAllItems();
				_pListView->InsertItem( &ListViewItemData );   

				pEdit = m_InterfaceFactory.GetEdit(ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT1 + m_CurrentSlot -1);

				// 결정체 아이템 정보 쓰기. 
				EquipCrystalExplain( pEdit, &m_clArtifactItem[ m_CurrentSlot ], m_CurrentSlot -1);  				
			}
		}
	}

	return true;
}

void NArtifactMixtureDlg::DelListViewItem( CListView* _pListView, SI08 m_CurrentSlot )
{
	if( false == CanDel() )
		return;

	m_clArtifactItem[ m_CurrentSlot ].Init();
	_pListView->DeleteAllItems();

	m_bCrystalSlotImageList[ m_CurrentSlot ] = false;
	m_InventoryPos[ m_CurrentSlot ]			 = NO_SLOT;			
}

void NArtifactMixtureDlg::DelAllListViewItem( void )
{
	ZeroMemory( m_clArtifactItem, sizeof( m_clArtifactItem ) ); 
	//ZeroMemory( m_InventoryPos,	  sizeof( m_InventoryPos ) );
	memset(m_InventoryPos, NO_SLOT, sizeof( m_InventoryPos ) );
	
	CEdit*		pEdit	  = NULL;
	CListView*	pListView = NULL;
	
	for( SI32 SlotCount = 0; SlotCount < TOTAL_SLOT_COUNT; ++ SlotCount )
	{
		pListView  = m_InterfaceFactory.GetListView( ARTIFACT_MIXTURE_LISTVIEW_EQUIP_REGISTER_ITEM + SlotCount );
		pListView->DeleteAllItems();

		pEdit	   = m_InterfaceFactory.GetEdit(ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT1 + SlotCount );	
		pEdit->SetText("");
	}	
}


bool NArtifactMixtureDlg::CanInsertEquip( void )
{
	if( 0 > m_CurrentSlot || m_CurrentSlot > EQUIP_SLOT )
		return false;

	//추가 되면 결정체 슬롯에 관련 슬롯이 보이게 세팅하는부분을 추가하자.


	return true;
}

bool NArtifactMixtureDlg::CanInsertCrystal( cltItem* _cltItem )
{
	if( CRYSTAL_SLOT1 > m_CurrentSlot || m_CurrentSlot >= TOTAL_SLOT_COUNT )
		return false;

	if( m_CurrentSlot > _cltItem->siUseCrystalNum )
		return false;

	//KHY - 0805 - 아트펙스 수정.
	//if( 0 < _cltItem->cltItemCrytal[ m_CurrentSlot-1 ].GetCrystalType() )
	//	return false;

	return true;
}

bool NArtifactMixtureDlg::CanDel( void )
{
	if( 0 > m_CurrentSlot || m_CurrentSlot >= TOTAL_SLOT_COUNT )
		return false;

	if( 0 >= m_clArtifactItem[ m_CurrentSlot ].siUnique 	|| 0 >= m_clArtifactItem[ m_CurrentSlot ].siItemNum )
		return false;

	return true;
}

bool NArtifactMixtureDlg::CanMixure( SI32 _ItemUnique )
{
	SI32 siSlotType = pclClient->pclItemManager->UsedSlotItem( _ItemUnique );

	if ( siSlotType )
	{
		return true;		
	}
	else if (cltItemManagerCommon::SLOTITEMTYPE_DORMANCY == siSlotType )
	{
		return false;		
	}
	else
	{
		return false;
	}
}

bool NArtifactMixtureDlg::IsCristal( SI32 _siItemUnique )
{
	SI32 ref	= pclClient->pclItemManager->FindItemRefFromUnique( _siItemUnique );

	SI32 siType = pclClient->pclItemManager->pclItemInfo[ ref ]->siType; 
	
	if( ITEMTYPE_CRYSTAL == siType )
		return true;

	return false;
}

void NArtifactMixtureDlg::MixtureSuccess( cltItem* _cltItem )
{
	ZeroMemory( m_clArtifactItem,				sizeof( m_clArtifactItem ) ); 
	//ZeroMemory( &m_InventoryPos[CRYSTAL_SLOT1],	sizeof( m_InventoryPos[CRYSTAL_SLOT1] ) * MAX_CRYSTAL_NUM );
	memset(&m_InventoryPos[CRYSTAL_SLOT1],	NO_SLOT, sizeof( m_InventoryPos[CRYSTAL_SLOT1] ) * MAX_CRYSTAL_NUM );

	CListView*	pListView = NULL;
	CEdit*		pEdit	  = NULL;
	for( SI32 SlotCount = 0; SlotCount < TOTAL_SLOT_COUNT; ++SlotCount )
	{
		pListView  = m_InterfaceFactory.GetListView( ARTIFACT_MIXTURE_LISTVIEW_EQUIP_REGISTER_ITEM + SlotCount );
		if( NULL == pListView )
		{
			--SlotCount;
			continue;
		}
		pListView->DeleteAllItems();

		pEdit = m_InterfaceFactory.GetEdit(ARTIFACT_MIXTURE_EDIT_CRYSTAL_REGISTER_SLOT1 + SlotCount );	
		pEdit->SetText("");
	}

	
	m_CurrentSlot = 0;
	pListView  = m_InterfaceFactory.GetListView( ARTIFACT_MIXTURE_LISTVIEW_EQUIP_REGISTER_ITEM );
	InsertEquip( pListView, _cltItem );
	
	CButton* pButton = NULL;
	pButton = m_InterfaceFactory.GetButton(ARTIFACT_MIXTURE_BUTTON_MIXTURE);
	pButton->Enable(true);
}

void NArtifactMixtureDlg::DeleteBuffer( SI32 _ClassType, void* _ClassData )
{
	switch( _ClassType )
	{
		case GAMEMSG_REQUEST_ARTIFACT_MIXTURE:	delete (cltGameMsgRequest_Artifact_Mixture*)_ClassData; break;	
	}

}

bool NArtifactMixtureDlg::SameCrystalInvenItem( SI32 _InvenPos)
{
	for(SI32 Count = CRYSTAL_SLOT1; Count < TOTAL_SLOT_COUNT; ++Count)
	{
			if(_InvenPos == m_InventoryPos[Count] )
				return true;
	}
	
	return false;
}

bool NArtifactMixtureDlg::SameEquipCrystalItem( cltItem* _cltItem )
{
	// 아래 능력은 중복이 가능하다.
	switch(_cltItem->GetCrystalAbility())
	{
		case CRYSTALABILITY_EFFECT_PHYSICAL_ATTACK:
		case CRYSTALABILITY_EFFECT_MAGICAL_ATTACK:
		case CRYSTALABILITY_ATTRIBUTE_FIREATTACK:	
		case CRYSTALABILITY_ATTRIBUTE_WATERATTACK:
		case CRYSTALABILITY_ATTRIBUTE_WINDATTACK:	
		case CRYSTALABILITY_ATTRIBUTE_EARTHATTACK:		
		case CRYSTALABILITY_EFFECT_PHYSICAL_MIN_ATTACK:	
		case CRYSTALABILITY_EFFECT_PHYSICAL_MAX_ATTACK:	
		case CRYSTALABILITY_EFFECT_MAGICAL_MIN_ATTACK:	
		case CRYSTALABILITY_EFFECT_MAGICAL_MAX_ATTACK:		return false;
	}
	
	
	for(SI32 Count = CRYSTAL_SLOT1; Count < TOTAL_SLOT_COUNT; ++Count)
	{
		//if( TRUE == m_bCrystalSlotImageList[Count] )
		{
			if( _cltItem->siUnique == m_clArtifactItem[Count].siUnique )
                return true;
		}
	}

	return false;
}
bool NArtifactMixtureDlg::SameEquipCrystalAbility( cltItem* _cltItem )
{
	// 아래 능력은 중복이 가능하다.
	switch(_cltItem->GetCrystalAbility())
	{
		case CRYSTALABILITY_EFFECT_PHYSICAL_ATTACK:
		case CRYSTALABILITY_EFFECT_MAGICAL_ATTACK:
		case CRYSTALABILITY_ATTRIBUTE_FIREATTACK:	
		case CRYSTALABILITY_ATTRIBUTE_WATERATTACK:
		case CRYSTALABILITY_ATTRIBUTE_WINDATTACK:	
		case CRYSTALABILITY_ATTRIBUTE_EARTHATTACK:		
		case CRYSTALABILITY_EFFECT_PHYSICAL_MIN_ATTACK:	 
		case CRYSTALABILITY_EFFECT_PHYSICAL_MAX_ATTACK:	
		case CRYSTALABILITY_EFFECT_MAGICAL_MIN_ATTACK:	
		case CRYSTALABILITY_EFFECT_MAGICAL_MAX_ATTACK:		return false;
	}

	SI16	UserSlotNum = m_clArtifactItem[EQUIP_SLOT].GetUseCrystalNum();
	
	for(SI32 Count = 0; Count <  UserSlotNum; ++Count)
	{
		if(m_clArtifactItem[EQUIP_SLOT].cltItemCrytal[Count].GetCrystalType())
		{
			if(_cltItem->GetCrystalAbility() == m_clArtifactItem[EQUIP_SLOT].cltItemCrytal[Count].GetCrystalAblilty())
			{
				return true;
			}
		}
		if(m_clArtifactItem[CRYSTAL_SLOT1+Count].siUnique)
		{
			if(_cltItem->GetCrystalAbility() == m_clArtifactItem[CRYSTAL_SLOT1+Count].GetCrystalAbility())
			{
				return true;
			}
		}
	}

	return false;
}

bool NArtifactMixtureDlg::Islevel(SI32 EquipItemUnique, SI32 CristalItemUnique)
{
	//[진성] 아바타 결정체. 의복은 착용레벨과 무관하다.
	if(cltItemManagerCommon::SLOTITEMTYPE_AVATAR == pclClient->pclItemManager->UsedSlotItem( EquipItemUnique ) )
		return true;

	SI32 ref = 0;
	ref = pclClient->pclItemManager->FindItemRefFromUnique( EquipItemUnique );
	if( ref <= 0 ) 
		return false;

	// 아이템의 장착 가능 레벨.
	SI32 siEquipItemLevel = pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel; 
	
	ref = pclClient->pclItemManager->FindItemRefFromUnique( CristalItemUnique );
	if( ref <= 0 ) 
		return false;

	// 아이템의 장착 레벨.
	SI32 siCrystalItemLevel = pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel; 
	
	if(siEquipItemLevel >= siCrystalItemLevel)
	{
		return true;
	}

	return false;
}

bool NArtifactMixtureDlg::IsAbilityType(cltItem* pclEquipItem, SI32 CristalItemAbilytype)
{
	enum{ ATTACK_EQUIP, DEFENCE_EQUIP };
	
	BOOL Crystal_TYPE = -1; 
	switch(CristalItemAbilytype)
	{
		// 방어구
		case CRYSTALABILITY_STATE_STR:
		case CRYSTALABILITY_STATE_DEX:			
		case CRYSTALABILITY_STATE_MAG:			
		case CRYSTALABILITY_STATE_VIT:			
		case CRYSTALABILITY_STATE_LUK:			
		case CRYSTALABILITY_STATE_HND:	
		case CRYSTALABILITY_STATE_WIS:
		case CRYSTALABILITY_ATTRIBUTE_FIREDEFENCE:
		case CRYSTALABILITY_ATTRIBUTE_WATERDEFENCE:
		case CRYSTALABILITY_ATTRIBUTE_WINDDEFENCE:
		case CRYSTALABILITY_ATTRIBUTE_EARTHDEFENCE:	
		case CRYSTALABILITY_EFFECT_PHYSICAL_HITRATE:
		case CRYSTALABILITY_EFFECT_MAGICAL_HITRATE:
		case CRYSTALABILITY_EFFECT_PHYSICAL_DODGERATE:
		case CRYSTALABILITY_EFFECT_MAGICAL_DODGERATE:	
		case CRYSTALABILITY_EFFECT_PHYSICAL_DEFENSIVERATE:
		case CRYSTALABILITY_EFFECT_MAGICAL_DEFENSIVERATE:	
		case CRYSTALABILITY_EFFECT_HITPOINT:
		case CRYSTALABILITY_EFFECT_MANAPOINT:
		case CRYSTALABILITY_EFFECT_WEIGHT:	
		case CRYSTALABILITY_SKILL_SUMMON:	
		case CRYSTALABILITY_SKILL_FARM:
		case CRYSTALABILITY_SKILL_MINE:
		case CRYSTALABILITY_SKILL_FISH:
		case CRYSTALABILITY_SKILL_MAKEFOOD:
		case CRYSTALABILITY_SKILL_CAPTURE:
		case CRYSTALABILITY_SKILL_HORSETRAINING:
		case CRYSTALABILITY_SKILL_MAKEIRON1:
		case CRYSTALABILITY_SKILL_MAKETOOL1:	
		case CRYSTALABILITY_SKILL_MAKEMED:
		case CRYSTALABILITY_SKILL_MAKESWORD1:
		case CRYSTALABILITY_SKILL_MAKESPEAR1:
		case CRYSTALABILITY_SKILL_MAKEAXE1:
		case CRYSTALABILITY_SKILL_MAKESTAFF1:	
		case CRYSTALABILITY_SKILL_MAKEBOW1:	
		case CRYSTALABILITY_SKILL_MAKEGUN1:	
		case CRYSTALABILITY_SKILL_MAKEARMOUR1:
		case CRYSTALABILITY_SKILL_MAKEHELMET1:
		case CRYSTALABILITY_SKILL_MAKESHOES1:	
		case CRYSTALABILITY_SKILL_MAKEBELT1:
		case CRYSTALABILITY_SKILL_ARTIFACT:			
		case CRYSTALABILITY_SKILL_USESWORD1:
		case CRYSTALABILITY_SKILL_USESPEAR1:	
		case CRYSTALABILITY_SKILL_USEAXE1:	
		case CRYSTALABILITY_SKILL_USESTAFF1:	
		case CRYSTALABILITY_SKILL_USEBOW1:	
		case CRYSTALABILITY_SKILL_USEGUN1:					Crystal_TYPE = DEFENCE_EQUIP; break;
	
		//무기
		case CRYSTALABILITY_EFFECT_PHYSICAL_ATTACK:
		case CRYSTALABILITY_EFFECT_MAGICAL_ATTACK:
		case CRYSTALABILITY_ATTRIBUTE_FIREATTACK:	
		case CRYSTALABILITY_ATTRIBUTE_WATERATTACK:
		case CRYSTALABILITY_ATTRIBUTE_WINDATTACK:	
		case CRYSTALABILITY_ATTRIBUTE_EARTHATTACK:	
		case CRYSTALABILITY_EFFECT_PHYSICAL_MIN_ATTACK:	
		case CRYSTALABILITY_EFFECT_PHYSICAL_MAX_ATTACK:	
		case CRYSTALABILITY_EFFECT_MAGICAL_MIN_ATTACK:	
		case CRYSTALABILITY_EFFECT_MAGICAL_MAX_ATTACK:		Crystal_TYPE = ATTACK_EQUIP; break;
	}

	BOOL Equip_TYPE = -1;
	if( pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_WEAPON) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_BOW	  ) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_SWORD ) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_GUN	  ) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_SPEAR ) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_STAFF ) ||
		pclClient->pclItemManager->pclItemTypeInfo[pclEquipItem->GetType(pclClient->pclItemManager)]->IsAtb(ITEMATB_AXE	  )		)
	{
		Equip_TYPE = ATTACK_EQUIP;
	}
	else
	{
		Equip_TYPE = DEFENCE_EQUIP;
	}

	// 조합할 아이템에 결정체의 능력이 적용되면.
	if(Equip_TYPE == Crystal_TYPE)
		return true;
	
	// 무속성은 무조건 가능.
	else if(-1 == Crystal_TYPE)
		return true;
	
	else
		return false;

}



void NArtifactMixtureDlg::NotifyMsg( SI32 _MsgType )
{
	cltClient *pclclient = (cltClient*)pclClient;
	TCHAR* pTitle = NULL;
	TCHAR* pText  = NULL;

	if ( NULL == pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
	{
		 return;
	}

	pTitle = GetTxtFromMgr(836);

	switch( _MsgType )		
	{
		case cltItemManagerCommon::ALREADY_USED_INVEN_ITEM: pText = GetTxtFromMgr(8090); break;
		case cltItemManagerCommon::SAME_CRYSTAL:			pText = GetTxtFromMgr(8089); break;
		case cltItemManagerCommon::NO_ITEM_UNIQUE:			pText = GetTxtFromMgr(8084); break;
		case cltItemManagerCommon::NO_SLOT_ITEM:			pText = GetTxtFromMgr(8082); break;
		case cltItemManagerCommon::NO_USED_SLOT:			pText = GetTxtFromMgr(8086); break;
		case cltItemManagerCommon::NO_CRYSTAL:				pText = GetTxtFromMgr(8082); break;
		case cltItemManagerCommon::ALL_EMPTY_CRYSTAL:		pText = GetTxtFromMgr(8082); break;
		case cltItemManagerCommon::ALREADY_USE_SLOT:		pText = GetTxtFromMgr(8088); break;
		case cltItemManagerCommon::LIMIT_LEVEL:				pText = GetTxtFromMgr(8357); break;
		case cltItemManagerCommon::LIMIT_CRYSTAL_ABILITY:	pText = GetTxtFromMgr(8358); break;
	}
	
	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
}

bool NArtifactMixtureDlg::IsMixture(void)
{
	if(0 >= m_clArtifactItem[EQUIP_SLOT].siUnique)
        NotifyMsg(cltItemManagerCommon::NO_ITEM_UNIQUE);

	// 조합이 가능한 아이템인지 검사.
	if( false == CanMixure( m_clArtifactItem[EQUIP_SLOT].siUnique ) )   
	{
		return false;
	}

	// 사용가능한 슬롯이 비어있는 개수가 0개면 리턴.
	//KHY - 0805 - 아트펙스 수정.
	/*( 0 == m_clArtifactItem[EQUIP_SLOT].EmptyCrystalSlotCount() )
		return false;
	*/
	
	cltGameMsgRequest_Artifact_Mixture	pclInfo;
	
	cltGameMsgRequest_Artifact_Mixture* pclMixtureInfo = &pclInfo;

	pclMixtureInfo->m_siEquipItemPos = m_InventoryPos[ EQUIP_SLOT ];					// 장비의 인벤 위치.

	// 장비 아이템 복사.
	pclMixtureInfo->m_clEquipItem = m_clArtifactItem[ EQUIP_SLOT ];									// 조합되는 아이템의 위치.(장비류가 되겠지..)

	CListView*  _pListView = NULL;
	SI32		InvIndex   = 0;
	SI16		CrystalItemPosVal = -1; // 슬롯에 단 하나라도, 실제로 아이템이 등록되어 있어야 한다.

	for(SI32 Count = 0; Count < pclMixtureInfo->m_clEquipItem.siUseCrystalNum ; ++Count)
	{
		// 이미 조합된 결정체는 제외하고 결합되지 않은 결정체만 보낸다.
		//if( 0 >= pclMixtureInfo->m_clEquipItem.cltItemCrytal[Count].GetCrystalType() )
		{
			pclMixtureInfo->m_siCrystalItemPos[ Count ]  = m_InventoryPos[ Count + 1 ];		// 결정체의 인벤 위치.
			pclMixtureInfo->m_clCrystalItem[ Count ]	 = m_clArtifactItem[ Count +1 ];	// 결정체 아이템.

			pclMixtureInfo->m_clCrystalItem[ Count ].siItemNum	 = 1;	// 무조건 1개만 설정하여 없앤다.
			
			// 슬롯의 아이템 유무 체크
			if(CrystalItemPosVal < pclMixtureInfo->m_siCrystalItemPos[ Count ])
				CrystalItemPosVal = pclMixtureInfo->m_siCrystalItemPos[ Count ];
		}
	}

	// 슬롯에 단 하나라도, 실제로 아이템이 등록되어 있어야 한다.
	if(CrystalItemPosVal <= 0)
	{
		NotifyMsg(cltItemManagerCommon::NO_ITEM_UNIQUE);
		return false;
	}

	// 결합이 가능한 정보들인지 최종 검사.
	SI32 siResult = pclClient->pclItemManager->CanCrystalMixtureData( &pclMixtureInfo->m_clEquipItem, pclMixtureInfo->m_clCrystalItem ,pclMixtureInfo->m_siEquipItemPos);
	switch( siResult )
	{
		case cltItemManagerCommon::SAME_CRYSTAL:			NotifyMsg(cltItemManagerCommon::SAME_CRYSTAL);			return false;
		case cltItemManagerCommon::NO_ITEM_UNIQUE:			NotifyMsg(cltItemManagerCommon::NO_ITEM_UNIQUE);		return false;
		case cltItemManagerCommon::NO_SLOT_ITEM:			NotifyMsg(cltItemManagerCommon::NO_SLOT_ITEM);			return false;
		case cltItemManagerCommon::NO_USED_SLOT:			NotifyMsg(cltItemManagerCommon::NO_USED_SLOT);			return false;
		case cltItemManagerCommon::NO_CRYSTAL:				NotifyMsg(cltItemManagerCommon::NO_CRYSTAL);			return false;
		case cltItemManagerCommon::ALL_EMPTY_CRYSTAL:		NotifyMsg(cltItemManagerCommon::ALL_EMPTY_CRYSTAL);		return false;
		case cltItemManagerCommon::ALREADY_USE_SLOT:		NotifyMsg(cltItemManagerCommon::ALREADY_USE_SLOT);		return false;
		case cltItemManagerCommon::LIMIT_CRYSTAL_ABILITY:	NotifyMsg(cltItemManagerCommon::LIMIT_CRYSTAL_ABILITY);	return false; 
	}
	CButton* pButton = NULL;
	pButton = m_InterfaceFactory.GetButton(ARTIFACT_MIXTURE_BUTTON_MIXTURE);
	pButton->Enable(false);

	
	return true;
}
