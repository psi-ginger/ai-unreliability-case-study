#include "NPromoter_InfoDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"

#include "../../Promoter/NHNChinaPromoter.h"

#include "../../../CommonLogic/Msg/MsgType-NPC.h"
#include "../../CommonLogic/MsgType-Person.h"

NPromoter_InfoDlg::NPromoter_InfoDlg()
{
	ZeroMemory( m_promoterCharid,	sizeof(m_promoterCharid) );
	
	ZeroMemory( m_GiveItemUnique,	sizeof(m_GiveItemUnique) );
}

NPromoter_InfoDlg::~NPromoter_InfoDlg()
{
	
}

void NPromoter_InfoDlg::Init( CInterfaceFile* _pfile )
{
	_pfile->CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,			NPROMOTER_INFO_BUTTON_WHISPER,					this ), NPROMOTER_INFO_BUTTON_WHISPER,					"button_whisper"				);
	_pfile->CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,			NPROMOTER_INFO_BUTTON_PRIVATE_PERSON_CHANGE,	this ), NPROMOTER_INFO_BUTTON_PRIVATE_PERSON_CHANGE,	"button_private_person_change"	);
	_pfile->CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,			NPROMOTER_INFO_LIST_PROMOTER,					this ), NPROMOTER_INFO_LIST_PROMOTER,					"listbox_promoter"				);
	_pfile->CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,			NPROMOTER_INFO_LIST_GIVE_ITEM,					this ), NPROMOTER_INFO_LIST_GIVE_ITEM,					"listbox_give_item"				);
	_pfile->CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,			NPROMOTER_INFO_STATIC_CELLULAR_AUTH,			this ), NPROMOTER_INFO_STATIC_CELLULAR_AUTH,			"statictext_cellular_phone"		);
	_pfile->CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,			NPROMOTER_INFO_STATIC_GIVE_ITEM,				this ), NPROMOTER_INFO_STATIC_GIVE_ITEM,				"statictext_give_item"			);  
	_pfile->CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,		NPROMOTER_INFO_IMAGESTATIC_PORTRAIT,			this ), NPROMOTER_INFO_IMAGESTATIC_PORTRAIT,			"imagestatic_potrait"			);
	_pfile->CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,			NPROMOTER_INFO_EDIT_CELLULAR_AUTH,				this ), NPROMOTER_INFO_EDIT_CELLULAR_AUTH,				"editbox_cellular_phone_auth"	);
	_pfile->CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,			NPROMOTER_INFO_EXPLAIN,							this ), NPROMOTER_INFO_EXPLAIN,							"editbox_explain"				); 
	_pfile->CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,		NPROMOTER_INFO_IMAGESTATIC_GIVE_ITEM,			this ), NPROMOTER_INFO_IMAGESTATIC_GIVE_ITEM,			"imagestatic_give_item"			);
	

	CList* pPromoterList = m_InterfaceFactory.GetList( NPROMOTER_INFO_LIST_PROMOTER );
	CList* pGiveItemList = m_InterfaceFactory.GetList( NPROMOTER_INFO_LIST_GIVE_ITEM );

	pPromoterList->Clear();
	pPromoterList->SetBorder(true);
	pPromoterList->SetBKColorUse(true);
	pPromoterList->SetColumn( 0,  110, GetTxtFromMgr( 4855 ) );
	pPromoterList->SetColumn( 1,  50, GetTxtFromMgr( 1506 ) );	  
	pPromoterList->Refresh();

	pGiveItemList->Clear();
	pGiveItemList->SetBorder(true); 
	pGiveItemList->SetBKColorUse(true);
	pGiveItemList->SetColumn( 0,  60, GetTxtFromMgr( 1506 ) );
	pGiveItemList->SetColumn( 1, 170, GetTxtFromMgr( 1928 ) );	
	pGiveItemList->Refresh();

	m_InterfaceFactory.GetEdit(NPROMOTER_INFO_EXPLAIN)->SetText(GetTxtFromMgr(6241));
	
	// NPC 초상 설정   
	SetPotrait();
	
	// 프로모터 관련 정보 요청.
	RequestPromoter();
}


void NPromoter_InfoDlg::Create()
{
	if( IsCreate() )	return;
	
	m_bChildCreate = false;

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/NPromoterDlg/NPromoter_InfoDlg.ddf"));
	file.CreatePopupDialog( this, NPROMOTER_INFO, TEXT("NPromoter_InfoDlg"), StatiNPromoter_InfoDlgProc, false );
 
	Init( &file );

	SetX( 150 ); 
	SetY( 300 );
}

void NPromoter_InfoDlg::Create( CControllerMgr *pParent )
{
	if( IsCreate() )	return;

	m_bChildCreate = true;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NPromoterDlg/NPromoter_InfoDlg.ddf"));

	SI32 controlindex = file.GetControlIndex( TEXT("NPromoter_InfoDlg") ); 
	file.m_control[ controlindex ].dialog.bClosable = false;

	file.CreateChildDialog( this, NPROMOTER_INFO, TEXT("NPromoter_InfoDlg"), StatiNPromoter_InfoDlgProc, pParent);

	Init( &file );

	m_InterfaceFactory.GetImageStatic(NPROMOTER_INFO_IMAGESTATIC_PORTRAIT)->Show( false );
	m_InterfaceFactory.GetEdit(NPROMOTER_INFO_EXPLAIN)->Show( false ); 

	SetFileName(""); 
	SetY( GetY() + 30 );   
}

void NPromoter_InfoDlg::Action()
{

}

void CALLBACK NPromoter_InfoDlg::StatiNPromoter_InfoDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NPromoter_InfoDlg *pThis = (NPromoter_InfoDlg *)pControllerMgr;
	pThis->NPromoter_InfoDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NPromoter_InfoDlg::NPromoter_InfoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case NPROMOTER_INFO_LIST_GIVE_ITEM:
			{
				switch( nEvent )
				{
				case EVENT_LISTBOX_SELECTION:
					{
						SI32 siSelectIndex = m_InterfaceFactory.GetList(NPROMOTER_INFO_LIST_GIVE_ITEM)->GetSelectedIndex();

						if( siSelectIndex < 0)
							break;

						if( 0 >= m_GiveItemUnique[siSelectIndex] ) break;

						pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_GiveItemUnique[siSelectIndex], &m_siImageType, &m_siImageIndex );
					}
					break;
				}
			}
			break;
		case NPROMOTER_INFO_BUTTON_WHISPER:
			{
				if( _tcslen(m_promoterCharid) )
					pclclient->RequestSecretChatByCharName(m_promoterCharid);
			}
			break;
		case NPROMOTER_INFO_BUTTON_PRIVATE_PERSON_CHANGE:
			{
				ShellExecute( NULL, NULL, TEXT("http://www.newhljz.cn/"), NULL, NULL, SW_SHOWNORMAL ); 
			}
			break;
	}
}


void NPromoter_InfoDlg::SetPotrait( void )
{
	TCHAR* pText = GetTxtFromMgr(3395);
	SI32   siNCPKind = pclClient->pclKindInfoSet->FindKindFromName( pText );

	if(pclClient->pclKindInfoSet->pclKI[ siNCPKind ])
	{
		CImageStatic* pImageStatic = m_InterfaceFactory.GetImageStatic( NPROMOTER_INFO_IMAGESTATIC_PORTRAIT );

		TCHAR		  strFileName[ 128 ] = TEXT("");
		SI16		  tmpSelectedIndex   = pclClient->pclKindInfoSet->FindKindFromName( pText );
		TCHAR		  szPortrait[256]    = "";
		SI32		  siPortraitFont     = 0;

		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(tmpSelectedIndex, szPortrait, sizeof(szPortrait), &siPortraitFont);

		pImageStatic->SetFileName(szPortrait);
		pImageStatic->SetFontIndex( siPortraitFont );
		pImageStatic->Refresh();
	}
}

void NPromoter_InfoDlg::RequestPromoter( void )
{
	// 프로모터 정보 요청.
	cltGameRequest_Promoter_Info_Total_Get clinfo;
	cltMsg clMsg( GAMEMSG_REQUEST_PROMOTER_INFO_TOTAL_GET, sizeof(clinfo), (BYTE*)&clinfo );
	((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg);
	
	
	// 프로모터로 인해 얻을 수 있는 아이템의 정보 요청.
	cltGameRequest_PromoterNPC_Give_Item_Get clinfo2;
	cltMsg clMsg2( GAMEMSG_REQUEST_PROMOTERNPC_GIVE_ITEM_GET, sizeof(clinfo2), (BYTE*)&clinfo2 );
	((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg2);
}

void NPromoter_InfoDlg::SetGiveItem( SI32* _pItemUnique, SI32* _pItemNum )
{
	
	memcpy(m_GiveItemUnique, _pItemUnique, sizeof(m_GiveItemUnique));

	CList* pList = m_InterfaceFactory.GetList(NPROMOTER_INFO_LIST_GIVE_ITEM);
	stListBoxItemData Itemdata;
	TCHAR			  szItemInfo[64]	= "";
	TCHAR			  szItemNum[4]		= "";
	TCHAR			  szLevel[4]		= "";
	
	SI32 siInsertItemNum = 0;
	for( SI32 siCount = 0; siCount < Const_Max_RecommandReward_Item; ++siCount )
	{
		if( 0 >= _pItemUnique[siCount] ) continue;

		m_GiveItemUnique[siInsertItemNum++] = _pItemUnique[siCount];

		_itot( siCount, szLevel, 10 );
		Itemdata.Set( 0, szLevel );		
        
		_itot( _pItemNum[siCount], szItemNum, 10 );
		StringCchPrintf(szItemInfo, sizeof(szItemInfo), "%s %s%s",	pclClient->pclItemManager->GetName( _pItemUnique[siCount] ),
																	szItemNum, 												
																	GetTxtFromMgr(2219) );
		Itemdata.Set( 1, szItemInfo );		
		
		pList->AddItem( &Itemdata );
	}

	pList->Refresh(); 

	m_InterfaceFactory.GetList(NPROMOTER_INFO_LIST_GIVE_ITEM)->SetSelectedIndex(0);
	SI32 siSelectIndex = m_InterfaceFactory.GetList(NPROMOTER_INFO_LIST_GIVE_ITEM)->GetSelectedIndex();

	if( 0 < m_GiveItemUnique[siSelectIndex] )  
	{
		pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_GiveItemUnique[siSelectIndex], &m_siImageType, &m_siImageIndex );
	}
}

void NPromoter_InfoDlg::SetPromoterInfo( SI16 _siKind, SI16 _siLevel, bool _bAuth, TCHAR* _pszPromoterCharName )
{
	cltClient * pclclient = (cltClient*) pclClient;

	if( 0 >= strlen( _pszPromoterCharName ) ) 
	{
		m_InterfaceFactory.GetButton(NPROMOTER_INFO_BUTTON_WHISPER)->Enable( false );
		return;
	}
	memcpy(m_promoterCharid, _pszPromoterCharName, sizeof(m_promoterCharid) );

	CList* pList = m_InterfaceFactory.GetList(NPROMOTER_INFO_LIST_PROMOTER);
	stListBoxItemData Itemdata;
	
	TCHAR			  szLevel[4] = "";
	_itot(_siLevel, szLevel, 10);

	Itemdata.Set( 0, _pszPromoterCharName );		
	Itemdata.Set( 1, szLevel, COLORREF(0xffffffff) );

	pList->AddItem( &Itemdata );
	pList->Refresh();

	// 인증상태
	CEdit* pEdit = m_InterfaceFactory.GetEdit(NPROMOTER_INFO_EDIT_CELLULAR_AUTH);
	if( _bAuth )
		pEdit->SetText( GetTxtFromMgr(30282) );	
	else
		pEdit->SetText( GetTxtFromMgr(30283) );
}

void NPromoter_InfoDlg::Hide()
{
	CControllerMgr::Show(false);
}

void NPromoter_InfoDlg::Show()
{
	CControllerMgr::Show(true);

}

void NPromoter_InfoDlg::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
{
	if ( ! IsShow() )
		return;

	if(m_siImageIndex < 0)   
		return;

	cltClient *pclclient = ( cltClient * )pclClient;
	TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( m_siImageType );

	if ( pSpr )
	{
		CImageStatic* pImageStatic = m_InterfaceFactory.GetImageStatic(NPROMOTER_INFO_IMAGESTATIC_GIVE_ITEM);
		
		// 자식창이라면 부모로부터 이미지를 그릴 위치를 얻어온다.
		if( m_bChildCreate )
		{
			GP.PutSpr( pSpr, GetParentDialog()->GetX() + GetX() + ScreenRX + pImageStatic->GetX() + 10,  
							 GetParentDialog()->GetY() + GetY() + ScreenRY + pImageStatic->GetY(), 
							 m_siImageIndex );
		}
		else
		{
			GP.PutSpr( pSpr, GetX() + ScreenRX + pImageStatic->GetX() + 10,  
							 GetY() + ScreenRY + pImageStatic->GetY(), 
							 m_siImageIndex );
		}
	}
}