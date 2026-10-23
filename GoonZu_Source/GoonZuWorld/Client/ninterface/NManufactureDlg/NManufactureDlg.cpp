
#include "NManufactureDlg.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "Char/CharCommon/Char-Common.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/Msg/MsgType-Personalshop.h"
#include "../../Client/Client.h"
#include "../../Client/Music/Music.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../npersonalshopsearchdlg/NPersonalShopSearchDlg.h"
#include "../../CommonLogic/Msg/MsgType-Quest.h"


CNManufactureDlg::CNManufactureDlg()
{	
	Initialize();
}

CNManufactureDlg::~CNManufactureDlg()
{
	Destroy();
}


void CNManufactureDlg::Initialize()
{

	m_bSelf		 = false;  
	m_bShopStarted = false;

	m_bShopDataChanged = false;

	memset(	m_szShopBanner, 0 , sizeof(m_szShopBanner));
	m_clShopData.Init();

	m_siShopperCharUnique = 0;		// MINI HOMEPAGE를 띄우기 위한 
	m_siShopperPersonID	  = 0;
	memset(m_szAccountID, 0, sizeof(m_szAccountID));
	memset(m_szCharName,  0, sizeof(m_szCharName));
	
	m_bShow = true;

	m_siImageType = -1;
	m_siImageIndex = -1;

}

void CNManufactureDlg::Destroy()
{

}

void CNManufactureDlg::Create()
{
	if ( !IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NManufacture/DLG_NManufacture.ddf"));
		file.CreatePopupDialog( this, NMANUFACTURE_DLG , TEXT("dialog_NManufacture"), StaticNManufactureDlgProc );

	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_1 , this ), NMANUFACTURE_STATIC_1,   "statictext_NONAME1"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_2 , this ), NMANUFACTURE_STATIC_2,   "statictext_NONAME2"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_3 , this ), NMANUFACTURE_STATIC_3,   "statictext_NONAME3"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_4 , this ), NMANUFACTURE_STATIC_4,   "statictext_NONAME4"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_5 , this ), NMANUFACTURE_STATIC_5,   "statictext_NONAME5"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_6 , this ), NMANUFACTURE_STATIC_6,   "statictext_NONAME6"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_7 , this ), NMANUFACTURE_STATIC_7,   "statictext_NONAME7"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_8 , this ), NMANUFACTURE_STATIC_8,   "statictext_NONAME8"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_9 , this ), NMANUFACTURE_STATIC_9,   "statictext_NONAME9"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_10 , this ), NMANUFACTURE_STATIC_10,   "statictext_NONAME10"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_11 , this ), NMANUFACTURE_STATIC_11,   "statictext_NONAME11"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_12 , this ), NMANUFACTURE_STATIC_12,   "statictext_NONAME12"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_13 , this ), NMANUFACTURE_STATIC_13,   "statictext_NONAME13"); 	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_14 , this ), NMANUFACTURE_STATIC_14,   "statictext_NONAME14"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURE_STATIC_15 , this ), NMANUFACTURE_STATIC_15,   "statictext_NONAME15"); 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_EDIT_EXPLAIN , this ), NMANUFACTURE_EDIT_EXPLAIN,   "editbox_NONAME1");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_EDIT_UPLOADEDNUM , this ), NMANUFACTURE_EDIT_UPLOADEDNUM,   "editbox_NONAME2");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_EDIT_UPLOADCOMMISSION , this ), NMANUFACTURE_EDIT_UPLOADCOMMISSION,   "editbox_NONAME3");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_EDIT_SKILLLEVEL , this ), NMANUFACTURE_EDIT_SKILLLEVEL,   "editbox_NONAME4");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_EDIT_DEX , this ), NMANUFACTURE_EDIT_DEX,   "editbox_NONAME5");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_EDIT_NUM , this ), NMANUFACTURE_EDIT_NUM,   "editbox_NONAME6");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_EDIT_COMMISSION , this ), NMANUFACTURE_EDIT_COMMISSION,   "editbox_NONAME7");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURE_EDIT_DANGER , this ), NMANUFACTURE_EDIT_DANGER,   "editbox_NONAME8");


		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURE_BUTTON_SERCH, this ), NMANUFACTURE_BUTTON_SERCH,   "button_item_search"); 	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURE_BUTTON_CANCEL, this ), NMANUFACTURE_BUTTON_CANCEL,   "button_NONAME2"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURE_BUTTON_UPLOAD, this ), NMANUFACTURE_BUTTON_UPLOAD,   "button_NONAME6"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURE_BUTTON_START, this ), NMANUFACTURE_BUTTON_START,   "button_NONAME3"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURE_BUTTON_END, this ), NMANUFACTURE_BUTTON_END,   "button_NONAME4"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURE_BUTTON_CLOSE, this ), NMANUFACTURE_BUTTON_CLOSE,   "button_NONAME5"); 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NMANUFACTURE_COMBO_MAKEITEM, this ), NMANUFACTURE_COMBO_MAKEITEM,   "combobox_NONAME1"); 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, NMANUFACTURE_OUTLINE_1, this ), NMANUFACTURE_OUTLINE_1, "outline_NONAME1");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NMANUFACTURE_IMAGESTATIC_1, this ), NMANUFACTURE_IMAGESTATIC_1, "imagestatic_NONAME1");

		MovePos(250,105);

		SetMakeableItemList();

		pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENMAKEREQUEST);

		TCHAR* pszText = NULL;
		if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
		{
			pszText = GetTxtFromMgr( 10016 );
			m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_9)->SetText( pszText );
		}
		else
		{
			pszText = GetTxtFromMgr( 3100 );
			m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_9)->SetText( pszText );
		}

	}
}

void CALLBACK CNManufactureDlg::StaticNManufactureDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNManufactureDlg *pThis = (CNManufactureDlg*) pControllerMgr;
	pThis->NManufactureDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNManufactureDlg::NManufactureDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NMANUFACTURE_BUTTON_CLOSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;

	case NMANUFACTURE_BUTTON_SERCH:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltClient *pclclient = (cltClient *)pclClient;
					pclclient->CreateInterface( NPERSONALSHOPSEARCH_DLG );
					
					if(pclclient->m_pDialog[NPERSONALSHOPSEARCH_DLG])
					{
						(( CNPersonalShopSearchDlg * )pclclient->m_pDialog[NPERSONALSHOPSEARCH_DLG])->NPersonalShopSearchDlgProc(EVENT_LISTBOX_SELECTION,NPERSONALSHOPSEARCH_LISTBOX_SHOP_SHOWCASE,NULL);

					}					
				}
				break;
			}
		}
		break;

	case NMANUFACTURE_BUTTON_UPLOAD :
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(m_bSelf )
					{
						if( AddShopMakeItem() == true )
						{
                            m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_START)->Enable(true);							
							m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_CANCEL)->Enable(true);

							// 중간에 정보 세팅
  							TCHAR strBuffer[256] = {'\0',};

							StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_clItem[0].siItemNum );														
							m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADEDNUM)->SetText(strBuffer);
 
							StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_siPrice[0] );
							m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADCOMMISSION)->SetText(strBuffer);
						}
					}
					else
					{
						MakeShopItem();
					}
				}
			}
			break;
		}
		break;

	case NMANUFACTURE_BUTTON_CANCEL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 자신의 상점이 시작하지 않았을때만 취소를 할수 있다.
					if(m_bSelf==TRUE && m_bShopStarted==FALSE)
					{
						if(DropShopItem()==true)
						{
							m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_START)->Enable(false);							
							m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADEDNUM)->SetText(TEXT(""));							
							m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADCOMMISSION)->SetText(TEXT(""));
						}
					}
				}
				break;
			}
		}
		break;

	case NMANUFACTURE_BUTTON_START :
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					StartPersonalShop();
				}
			}
			break;
		}
		break;
	case NMANUFACTURE_BUTTON_END :
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					StopPersonalShop();
				}
			}
            break;
		}
		break;
	}
}


void CNManufactureDlg::Show( SI32 siCharUnique, SI32 siPersonID, SI16 ShopMode, TCHAR *Name, TCHAR *AccountID, BOOL bShopStarted )
{

	if(m_bShow == false)	
		CControllerMgr::Show( true );
	m_bShow = true;

	if(AccountID && _tcslen(AccountID))	StringCchCopy(m_szAccountID, MAX_PLAYER_NAME, AccountID);
	if(Name&& _tcslen(Name))				StringCchCopy(m_szCharName,  MAX_PLAYER_NAME, Name);

	m_siShopperCharUnique = siCharUnique;
	m_siShopperPersonID   = siPersonID;
	m_bShopStarted		  = bShopStarted;


	SetMakeableItemList();


	BOOL bSelf = (siCharUnique == pclClient->pclCM->CR[1]->GetCharUnique());

	ShopInit(bSelf);

}


void CNManufactureDlg::ShopInit(BOOL bSelf)
{

	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_EXPLAIN)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADEDNUM)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADCOMMISSION)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_SKILLLEVEL)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_DEX)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_NUM)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_COMMISSION)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_DANGER)->SetText(TEXT(""));

	//m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADCOMMISSION)->Enable(false);        
	//m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADEDNUM)->Enable(false);
    m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_CANCEL)->Enable(false);

	// DialogBox 수정 
	TCHAR buffer[256];

	TCHAR* pText = GetTxtFromMgr(2194);
	StringCchPrintf(buffer, 256, pText, m_szCharName);	
	this->SetTitle( buffer );

	m_bSelf		 = bSelf;

	// 배너정보를 만든다.
	cltShopBannerData clShopBanner;
	clShopBanner.SetData(&m_clShopData);
	clShopBanner.MakeBannerText(pclClient->pclItemManager, m_szShopBanner);

	// 정보들 초기화


	// 자기자신의 상점 설정 
	if(bSelf == TRUE )
	{

		cltClient *pclclient = (cltClient*)pclClient;
					
      	// Edit 텍스트 개수 제한!!
		m_szShopBanner[ MAX_PERSONAL_SHOP_TEXT_SIZE - 1] = '\0';
		
		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_EXPLAIN)->SetText(m_szShopBanner);

		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_DEX)->SetReadOnly(false);
		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_SKILLLEVEL)->SetReadOnly(false);

		// 상점이 이미 시작이 되었으면 
		if(m_bShopStarted)
		{
			m_InterfaceFactory.GetComboBox(NMANUFACTURE_COMBO_MAKEITEM)->Enable(false);

			m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_CANCEL)->Enable(false);			
			m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_UPLOAD)->Enable(false);
			
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_DEX)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_NUM)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_COMMISSION)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_SKILLLEVEL)->SetReadOnly(true);

			m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_START)->Enable(false);
			m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_END)->Enable(true);
			
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_DEX)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_NUM)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_COMMISSION)->SetReadOnly(true);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_SKILLLEVEL)->SetReadOnly(true);

			TCHAR strBuffer[256] = {'\0',};

			StringCchPrintf(strBuffer,256,TEXT(" %d"), m_clShopData.m_siHandPoint );            
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_DEX)->SetText( strBuffer );

			StringCchPrintf(strBuffer,256,TEXT(" %d"), m_clShopData.m_clItem[0].siItemNum );
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_NUM)->SetText( strBuffer );

			StringCchPrintf(strBuffer,256,TEXT(" %d"), m_clShopData.m_siPrice[0] );
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_COMMISSION)->SetText( strBuffer );

			StringCchPrintf(strBuffer,256,TEXT(" %d"), m_clShopData.m_siSkillLevel );
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_SKILLLEVEL)->SetText( strBuffer );

			SetItemImageData( m_clShopData.m_clItem[0].siUnique);

			m_InterfaceFactory.GetComboBox(NMANUFACTURE_COMBO_MAKEITEM)->Show(false);

			m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_7)->Show(false);

			RefreshShowData();

		}
		else
		{
			m_InterfaceFactory.GetComboBox(NMANUFACTURE_COMBO_MAKEITEM)->Enable(true);

			m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_CANCEL)->Enable(true);
			m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_START)->Enable(true);
			m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_UPLOAD)->Enable(true);

			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_DEX)->SetReadOnly(false);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_NUM)->SetReadOnly(false);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_COMMISSION)->SetReadOnly(false);
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_SKILLLEVEL)->SetReadOnly(false);
			
			m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_START)->Enable(false);
			m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_END)->Enable(false);
		}
		
	}
	else		// bSelf == false . 자신의 상점이 아니다.
	{
		cltClient *pclclient = (cltClient*)pclClient;

		SI32 id = cltCharCommon::CharUniqueInfo[m_siShopperCharUnique];
		if(id == 0)
		{
			return ;
		}

		cltCharClient* pclchar = (cltCharClient*)pclclient->pclCM->CR[id];
			
		// Edit 텍스트 개수 제한!!
		m_szShopBanner[ MAX_PERSONAL_SHOP_TEXT_SIZE - 1] = '\0';
		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_EXPLAIN)->SetText(m_szShopBanner);

		m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_CANCEL)->Show(false);
		
		m_InterfaceFactory.GetComboBox(NMANUFACTURE_COMBO_MAKEITEM)->Show(false);
		
        

		pText = GetTxtFromMgr(2208);
		m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_UPLOAD)->SetText(pText);
		m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_UPLOAD)->Enable(true);
		
		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_NUM)->SetText(TEXT(""));
	//	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_NUM)->Show(false);
	//	m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_14)->Show(false);
		
		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_DEX)->SetReadOnly(true);
		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_SKILLLEVEL)->SetReadOnly(true);



		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_COMMISSION)->Show(false);
		m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_15)->Show(false);

		m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_START)->Enable(false);
		m_InterfaceFactory.GetButton(NMANUFACTURE_BUTTON_END)->Enable(false);
		
		
		m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_7)->Show(false);
//		m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_8)->Show(false);
//		m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_9)->Show(false);
		//m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_10)->Show(false);
		m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_11)->Show(false);


		// 중간에 정보 세팅
		TCHAR strBuffer[256] = {'\0',};

		StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_clItem[0].siItemNum );
		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADEDNUM)->SetText(strBuffer);

		StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_siPrice[0] );
		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADCOMMISSION)->SetText(strBuffer);
			
		StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_siHandPoint );
		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_DEX)->SetText(strBuffer);

		StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_siSkillLevel );
		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_SKILLLEVEL)->SetText(strBuffer);


		SI64 atb = pclClient->pclItemManager->GetItemAtb(m_clShopData.m_clItem[0].siUnique);
		if(atb & ITEMATB_OBJECT)
		{
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_NUM)->SetText(TEXT("1"));
			m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_NUM)->SetReadOnly(true);            			
		}

		SetItemImageData( m_clShopData.m_clItem[0].siUnique);

	}

}


void CNManufactureDlg::Hide()
{
	if(m_bShow) CControllerMgr::Show(false);
	{
		m_bShow = false;
	}



}
void CNManufactureDlg::Action()
{
	



}

void CNManufactureDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( MsgIndex )
	{
	case 0:	// 개인 상점 시작
		{
			if ( RetValue )
			{
				SI32 PersonID = *((SI32*)pData);

				TCHAR szText[MAX_PERSONAL_SHOP_TEXT_SIZE]=TEXT("");
				TCHAR *pTmpChar = NULL;
				pTmpChar = m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_EXPLAIN)->GetText();
				
				if( pTmpChar == NULL )
					return ;

				_tcscpy( szText, pTmpChar );

				if(_tcslen(szText) >= MAX_PERSONAL_SHOP_TEXT_SIZE ) return;

				// 모드는 제조의뢰로 고정
				m_clShopData.m_siShopMode   = PSHOP_MODE_MAKEITEM;

				cltGameMsgRequest_StartPersonalShop clshopdata( m_siShopperCharUnique,  &m_clShopData);
				cltMsg clMsgDt( GAMEMSG_REQUEST_STARTPERSONALSHOP, sizeof( clshopdata ), (BYTE*)&clshopdata );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsgDt);

				DeleteDialog();
			}
		}
		break;
	}
}


void CNManufactureDlg::SetMakeableItemList()
{
	stComboBoxItemData tmpComboBoxItemData;
	// 콤보 박스를 지운다.
	CComboBox * pComboBox =	m_InterfaceFactory.GetComboBox(NMANUFACTURE_COMBO_MAKEITEM);		
	pComboBox->Clear();

	SI32 m_siMakeableItemNum = 0;
	SI32 *m_pMakeableItemList = NULL;
	SI32 id = 1;
	// 만들수 있는 아이템 종류를 보여준다. 
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
	if(pclitemmanager->FindMakeableItem(NULL, 
		&pclClient->pclCM->CR[id]->pclCI->clCharItem,
		NULL,
		MAX_INVITEM_PER_PERSON,
		&pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[PERSONITEM_INV0],//인벤에 있는 아이템읋 토대로 만들수 있는 아이템을 리턴
		&m_siMakeableItemNum,
		&m_pMakeableItemList,
		true) == true)
	{
		TCHAR buffer[256];
		for(SI32 i = 0; i < m_siMakeableItemNum; ++i)
		{
			TCHAR* pText = GetTxtFromMgr(2246);
			StringCchPrintf(buffer, 256, pText, pclitemmanager->GetName(m_pMakeableItemList[i*2]), m_pMakeableItemList[i*2 + 1]);
			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( buffer );
			pComboBox->AddItem( &tmpComboBoxItemData );
			//SendMessage( hCombobox, CB_ADDSTRING, 0, (LPARAM)buffer );
		}

	}

	if(m_pMakeableItemList)
	{
		delete []m_pMakeableItemList;
		m_pMakeableItemList = NULL;
	}
}



bool CNManufactureDlg::AddShopMakeItem()
{//콤보 박스에 제조 의뢰 리스트 추가.

	TCHAR szSelText[128];
	TCHAR *pTmpChar = NULL;
	TCHAR Nbuffer[512]=TEXT(" ");

	SI16 siTmpIndex = -1;
	szSelText[0] = '\0';	

	siTmpIndex = m_InterfaceFactory.GetComboBox(NMANUFACTURE_COMBO_MAKEITEM)->GetSelectedIndex();

	if(siTmpIndex < 0 ) return false;

	pTmpChar = m_InterfaceFactory.GetComboBox(NMANUFACTURE_COMBO_MAKEITEM)->GetText( siTmpIndex );

	// 제작목록에서 설정한 물건의 설정갯수와 가격을 showcase에 설정한다. 
	//if(GetWindowText(GetDlgItem(m_hDlg, m_pComboBox_iteminfo_name), szSelText, 100))
	if(pTmpChar != NULL )
	{
		_tcscpy( szSelText, pTmpChar );
		
		//[2007.12.21 김영훈 수정 : 아이템 이름에 '-' 문자 있을때 등록 안되던 버그 수정]
		TCHAR *pTemp1 = _tcsrchr( szSelText, _T('-') );	 //뒤에서부터 '-' 문자를 검색함
		TCHAR* pText = GetTxtFromMgr(2219);
		TCHAR *pTemp2 = _tcsrchr( szSelText, _T('-') );//_tcsstr(szSelText, pText);	
		if(pTemp1 == NULL || pTemp2 == NULL) return false;

		*pTemp1 = '\0';	pTemp1++;
		*pTemp2 = '\0';

		SI32 siMaxMakeItem = _tstoi(pTemp1);
		if(siMaxMakeItem <= 0)				{ return false;   }

		SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName(szSelText);
		if(itemunique)
		{
			//----------------------------------------
			// 아이템의 기타 정보를 표시한다 
			//----------------------------------------
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
			if(ref > 0)
			{
				SI32 i = 0;
				SI32 siAmountSuccess= 0, siPriceSuccess = 0;
				SI32 siHandSuccess= 0,   siSkillSuccess = 0;
				TCHAR AmountBuffer[256]=TEXT(""), PriceBuffer[256]=TEXT("");
				TCHAR HandBuffer[256]=TEXT(""),   SkillBuffer[256]=TEXT("");
				TCHAR *pTmpChar1, *pTmpChar2, *pTmpChar3, *pTmpChar4;
				pTmpChar1 = pTmpChar2 = pTmpChar3 = pTmpChar4 = NULL;

				pTmpChar1 =	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_NUM)->GetText();
				pTmpChar2 =	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_COMMISSION)->GetText();
				pTmpChar3 =	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_DEX)->GetText();
				pTmpChar4 = m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_SKILLLEVEL)->GetText();

				if( pTmpChar1 == NULL || pTmpChar2 == NULL || pTmpChar3 == NULL || pTmpChar1 == NULL )
				{
					TCHAR* pText = GetTxtFromMgr(2220);
					DisplayMessage(FALSE, pText);
					return false;
				}

				_tcscpy( AmountBuffer, pTmpChar1 );
				_tcscpy( PriceBuffer, pTmpChar2 );
				_tcscpy( HandBuffer, pTmpChar3 );
				_tcscpy( SkillBuffer, pTmpChar4 );

				siAmountSuccess =  _tcslen( AmountBuffer );
				siPriceSuccess = _tcslen( PriceBuffer );
				siHandSuccess = _tcslen( HandBuffer );
				siSkillSuccess = _tcslen( SkillBuffer );

				if(siAmountSuccess <= 0 || siPriceSuccess <= 0 || siHandSuccess<=0 || siSkillSuccess <=0)
				{
					TCHAR* pText = GetTxtFromMgr(2220);
					DisplayMessage(FALSE, pText);
					return false;
				}

				SI32 siAmountValue = 0, siPriceValue = 0;
				SI16 siHandValue = 0,   siSkillValue = 0;

				// AMOUNT
				siAmountValue = _tstoi(AmountBuffer);
				if(siAmountValue <=0 )
				{
					TCHAR* pText = GetTxtFromMgr(1668);
					DisplayMessage(FALSE, pText);
					return false;
				}
				if(siAmountValue > siMaxMakeItem )
				{
					TCHAR buffer[256];
					TCHAR* pText = GetTxtFromMgr(2221);
					StringCchPrintf(buffer, 256, pText, siMaxMakeItem);
					DisplayMessage(FALSE, buffer);
					return false;
				}
				if(siAmountValue > MAX_ITEM_PILE_NUMBER)
				{
					TCHAR buffer[256];
					TCHAR* pText = GetTxtFromMgr(2211);
					StringCchPrintf(buffer, 256, pText, MAX_ITEM_PILE_NUMBER);
					DisplayMessage(FALSE, buffer);
					return false;
				}

				// PRICE 
				siPriceValue  = _tstoi(PriceBuffer);
				if(siPriceValue < 0)
				{
					TCHAR* pText = GetTxtFromMgr(2222);
					DisplayMessage(FALSE, pText);
					return false;
				}
				if(siPriceValue > MAX_PRICE_IN_MARKET)
				{
					TCHAR buffer[256];
					TCHAR* pText = GetTxtFromMgr(2223);
					StringCchPrintf(buffer, 256, pText, 
						MAX_PRICE_IN_MARKET);
					DisplayMessage(FALSE, buffer);
					return false;
				}

				// 손재주 
				siHandValue  = _tstoi(HandBuffer);
				if(siHandValue < 0)
				{
					TCHAR* pText = NULL;
					if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
					{
						pText = GetTxtFromMgr(9865);
					}
					else
					{
						pText = GetTxtFromMgr(2224);
					}

					DisplayMessage(FALSE, pText);
					return false;
				}
				if(siHandValue > 2000)
				{
					TCHAR buffer[256];
					TCHAR* pText = NULL;
					if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
					{
						pText = GetTxtFromMgr(9866);
					}
					else
					{
						pText = GetTxtFromMgr(2225);
					}
					
					StringCchPrintf(buffer, 256, pText, 
						2000);
					DisplayMessage(FALSE, buffer);
					return false;
				}

				// 스킬레벨 
				siSkillValue  = _tstoi(SkillBuffer);
				if(siSkillValue < 0)
				{
					TCHAR* pText = GetTxtFromMgr(2226);
					DisplayMessage(FALSE, pText);
					return false;
				}
				if(siSkillValue > MAX_SKILL_LEVEL+30)
				{
					TCHAR buffer[256];
					TCHAR* pText = GetTxtFromMgr(2227);
					StringCchPrintf(buffer, 256, pText, 
						MAX_SKILL_LEVEL+30);
					DisplayMessage(FALSE, buffer);
					return false;
				}
			
				// 1개만 설정할수있도록 한다.				
				m_clShopData.m_clItem[0].siUnique		= itemunique;
				m_clShopData.m_clItem[0].siItemNum		= (SI32)siAmountValue;
				m_clShopData.m_siPrice[0]				= (SI32)siPriceValue;
				m_clShopData.m_siSelectedInventoryPos[0]= 0;
				m_clShopData.m_siHandPoint				= siHandValue;
				m_clShopData.m_siSkillLevel				= siSkillValue;
				TCHAR itemname[128];

				if(m_clShopData.m_clItem[ 0 ].GetFullName(pclClient->pclItemManager, itemname, 128) == true)			
				{
					m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_1)->SetText(itemname);
					m_InterfaceFactory.GetStatic(NMANUFACTURE_STATIC_1)->Refresh();
					TCHAR buffer[512];
					TCHAR item[128]= TEXT(" ") ;
					TCHAR num[128]= TEXT(" ") ;
					TCHAR money[128]= TEXT(" ") ;
					TCHAR hand[128]= TEXT(" ") ;
					TCHAR skill[128]= TEXT(" ") ;							
					TCHAR* pText = NULL;
					if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
					{
						pText = GetTxtFromMgr(9863);
					}
					else
					{
						pText = GetTxtFromMgr(2228);
					}
					
					StringCchPrintf(num, 128, TEXT("%d"), (SI32)siAmountValue);
					StringCchPrintf(money, 128, TEXT("%d"),(SI32)siPriceValue);
					StringCchPrintf(hand, 128, TEXT("%d"),(SI32)siHandValue);
					StringCchPrintf(skill, 128, TEXT("%d"), (SI32)siSkillValue);
					Nsprintf(buffer,pText,TEXT("item"), itemname   ,TEXT("num"),  num  ,TEXT("money"), money   ,TEXT("hand"), hand   ,TEXT("skill"),  skill  ,NULL);
					DisplayMessage(TRUE, buffer);

					// EDIT에 배너정보를 표시한다.
					cltShopBannerData clShopBanner;
					clShopBanner.SetData(&m_clShopData);
					clShopBanner.MakeBannerText(pclClient->pclItemManager, m_szShopBanner);
					// Edit 텍스트 개수 제한!!
					m_szShopBanner[ MAX_PERSONAL_SHOP_TEXT_SIZE - 1] = '\0';
					m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_EXPLAIN)->SetText(m_szShopBanner);

					// 이미지 설정
					SetItemImageData( m_clShopData.m_clItem[0].siUnique);

				}
				return true;		
			}
		}
	}

	DisplayMessage(FALSE, TEXT(""));
	
	return false;
	
}


void CNManufactureDlg::StartPersonalShop()
{
	cltClient *pclclient = ( cltClient * )pclClient;
	

	SI32 siTicketNum = 1;
	// 14040 : 개인상점 이용권 -> 24010 으로 변경
	if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(ITEMUNIQUE(24065)) >= siTicketNum )
	{
		TCHAR buffer[256] = TEXT("");
		TCHAR* pText = GetTxtFromMgr(6322);
		StringCchPrintf(buffer, 256, pText, siTicketNum);
		pText = GetTxtFromMgr(6323);

		stMsgBox MsgBox;
		MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
		pclclient->SetMsgBox( &MsgBox, NULL, 0 );

	}
	else	
	{
  		TCHAR buffer[256] = TEXT("") ;
		TCHAR* pText = GetTxtFromMgr(6321);
		StringCchPrintf(buffer, 256, pText, siTicketNum);
		DisplayMessage(FALSE, buffer);
	}
}


bool CNManufactureDlg::MakeShopItem()
{
	if(m_clShopData.m_clItem[ 0 ].siItemNum>0)
	{
		TCHAR AmountBuffer[256];
		SI32  siAmountSuccess	= 0;
		SI64  siAmountValue     = 0;
		TCHAR *pTmpChar = NULL;
		pTmpChar = m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_NUM)->GetText();

		if(pTmpChar == NULL )
		{
			TCHAR* pText = GetTxtFromMgr(2240);
			DisplayMessage(FALSE, pText); 

			return false;
		}

		_tcscpy( AmountBuffer, pTmpChar );

		siAmountSuccess = _tcslen( AmountBuffer );

		if(siAmountSuccess>0)	siAmountValue = _tstoi64(AmountBuffer);
		if(siAmountValue > 0)
		{
			cltClient *pclclient = ( cltClient * )pclClient;
			cltItem clitem;
			clitem.Set(&m_clShopData.m_clItem[ 0 ]);	// 다른 모든 정보는 같으나 
			clitem.siItemNum  = (SI32)siAmountValue;								// 갯수는 다르다.

			GMONEY  siPrice	    = 0;
			SI32  siHandpoint   = 0;
			SI32  siSkillLevel  = 0;
			siPrice			  = m_clShopData.m_siPrice[ 0 ];
			siHandpoint		  = m_clShopData.m_siHandPoint;
			siSkillLevel	  = m_clShopData.m_siSkillLevel;

			SI08 siType = pclClient->pclItemManager->FindMakeAniTypeFromUnique(clitem.siUnique);

			cltGameMsgRequest_MakePersonalShop clInfo(m_siShopperCharUnique, PSHOP_MODE_MAKEITEM, m_siShopperPersonID, &clitem, siPrice, siHandpoint, siSkillLevel,0,siType);
			cltMsg clMsg( GAMEMSG_REQUEST_MAKEPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo );
			pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

			DisplayMessage(TRUE, TEXT(""));
			return true;
		}
		else { 
			TCHAR* pText = GetTxtFromMgr(2240);
			DisplayMessage(FALSE, pText); }
	}
	else 
	{ 
		TCHAR* pText = GetTxtFromMgr(2241);
		DisplayMessage(FALSE, pText);	
	}
	
	return false;
}


bool CNManufactureDlg::DropShopItem()
{

	m_clShopData.m_clItem[0].Init();
	m_clShopData.m_siSelectedInventoryPos[0]	= 0;
	m_clShopData.m_siPrice[0]					= 0;

	cltShopTradeData	TempShopData;
	memcpy(&TempShopData, &m_clShopData, sizeof(cltShopTradeData));
	memset(&m_clShopData, 0, sizeof( cltShopTradeData ) );		// 메모리를 Clear 시킨다.

	// 고정된 값은 복사한다. 
	m_clShopData.m_siShopMode =  PSHOP_MODE_MAKEITEM;
	m_clShopData.m_siPersonID =  TempShopData.m_siPersonID; 
	StringCchCopy(m_clShopData.m_szAccountID, MAX_PLAYER_NAME, TempShopData.m_szAccountID);


	// EDIT에 배너정보를 표시한다.
	cltShopBannerData clShopBanner;
	clShopBanner.SetData(&m_clShopData);
	clShopBanner.MakeBannerText(pclClient->pclItemManager, m_szShopBanner);

	// Edit 텍스트 개수 제한!!
	m_szShopBanner[ MAX_PERSONAL_SHOP_TEXT_SIZE - 1] = '\0';

	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_EXPLAIN)->SetText(m_szShopBanner);

	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADEDNUM)->SetText(TEXT(""));
	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADCOMMISSION)->SetText(TEXT(""));


	TCHAR* pText = GetTxtFromMgr(2230);
	DisplayMessage(TRUE, pText);

	// 이미지 설정
	SetItemImageData( 0 );

	return true;
}

void CNManufactureDlg::StopPersonalShop()
{
	cltGameMsgRequest_StopPersonalShop clshopdata( m_siShopperCharUnique);
	cltMsg clMsg( GAMEMSG_REQUEST_STOPPERSONALSHOP, sizeof( clshopdata ), (BYTE*)&clshopdata );
	cltClient *pclclient = ( cltClient * )pclClient;
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

	DeleteDialog();
}

void CNManufactureDlg::RefreshShowData()
{
	// 중간에 정보 세팅
	TCHAR strBuffer[256] = {'\0',};

	StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_clItem[0].siItemNum );
	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADEDNUM)->SetText(strBuffer);

	StringCchPrintf(strBuffer,256,TEXT(" %d"),m_clShopData.m_siPrice[0] );
	m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_UPLOADCOMMISSION)->SetText(strBuffer);
}

void CNManufactureDlg::SetItemImageData(SI32 ItemUnique)
{
	if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( ItemUnique, &m_siImageType, &m_siImageIndex ) )
	{}
	else
	{
		m_siImageType = -1;
		m_siImageIndex = -1;
	}
}
void CNManufactureDlg::DisplayMessage(BOOL siSuccess, TCHAR *pBuffer)
{
	if(siSuccess == FALSE)	pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")) , 0, 0);
	if(pBuffer)
	{
		m_InterfaceFactory.GetEdit(NMANUFACTURE_EDIT_DANGER)->SetText( pBuffer );		
	}
}


void CNManufactureDlg::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
{

	if(m_siImageIndex < 0 || m_siImageType < 0)   
		return;

	cltClient *pclclient = ( cltClient * )pclClient;
	TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( m_siImageType );

	if ( pSpr )
	{

		GP.PutSpr( pSpr ,ScreenRX +  GetX() + 25,ScreenRY +  GetY() + 165, m_siImageIndex);

	}
}