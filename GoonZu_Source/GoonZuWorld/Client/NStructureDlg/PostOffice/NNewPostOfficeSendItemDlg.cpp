#include "NPostOfficeDlg.h"

#include "../../InterfaceMgr/Interface/ListView.h"
#include "../StrBaseDlg/StrInventory.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-PostOffice.h"
#include "../../AbuseFilter/AbuseFilter.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"
#include "..\..\Common\JWLib\IsCorrectID\IsCorrectID.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "gachamanager\GachaManager.h"
#include "../../Server/gachamanager/GachaManager.h"

#include "NStructureDlg/PostOffice/NPostOfficeGroupListDlg.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;



NNewPostOfficeSendItemDlg::NNewPostOfficeSendItemDlg()
{
	m_pMyInventory = new NStrInventory;
}

NNewPostOfficeSendItemDlg::~NNewPostOfficeSendItemDlg()
{
	NDelete( m_pMyInventory );	

	//[진성] 단체 메시지 보내기 창. => 2008-8-4  
	cltClient* pclclient = (cltClient*)pclClient;
	if( pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG] )
		pclclient->DestroyInterface(pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG]);
}

void NNewPostOfficeSendItemDlg::InitDlg()
{
	SetControlMap( STATIC_PLAYERNAME,		TEXT("statictext_NONAME1") );
	SetControlMap( EDIT_PLAYERNAME,			TEXT("editbox_playername") );
	SetControlMap( EDIT_ITEMNAME,			TEXT("editbox_itemname") );
	SetControlMap( EDIT_AMOUNT,				TEXT("editbox_amount") );
	SetControlMap( EDIT_RECEIVERNAME,		TEXT("editbox_receivername") );
	SetControlMap( EDIT_MSG,				TEXT("editbox_msg") );
	SetControlMap( LISTVIEW_MYINVENTORY,	TEXT("listview_myinventory") );
	SetControlMap( BUTTON_SENDITEM,			TEXT("button_senditem") );
	SetControlMap( BUTTON_SENDMSG,			TEXT("button_sendmsg") );

	

	//KHY - 0626 - 메세지  127바이트로 제한.
	((CEdit*)m_pControlMap[ EDIT_MSG ])->SetMaxEditTextLength(127);

	//KHY - 0629 - ID  20바이트로 제한.
	((CEdit*)m_pControlMap[ EDIT_PLAYERNAME ])->SetMaxEditTextLength(20);
	
	//[진성] 수신자 정보 관련 컨트롤 숨김. => 2008-8-4
	((CStatic*)m_pControlMap[ STATIC_PLAYERNAME ])->Show(false);
	((CEdit*)m_pControlMap[ EDIT_PLAYERNAME ])->Show(false);
	
	// MyInventory
	m_pMyInventory->CreateInventory( MAX_ITEM_PER_PERSON, (CListView*)m_pControlMap[ LISTVIEW_MYINVENTORY ] );

	m_pMyInventory->m_pListView->SetBKColorUse( true );
	m_pMyInventory->m_pListView->SetBorder( true );

	m_pMyInventory->LoadMyInventory();

	//[김영훈 추가:2007.11.21 - 아이템 겟수 입력시 숫자 이외의 다른 값은 입력되지 않도록 수정]
	((CEdit*)m_pControlMap[ EDIT_AMOUNT ])->SetNumberMode( true );
}

void NNewPostOfficeSendItemDlg::Destroy()
{
	

}
void NNewPostOfficeSendItemDlg::Show()
{
	CControllerMgr::Show( true );

	//[진성] 단체 메시지 보내기 창. => 2008-8-4
	cltClient* pclclient = (cltClient*)pclClient;
	pclclient->CreateInterface( NPOSTOFFICE_GROUP_LIST_DLG );
	pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG]->SetX( GetParentDialog()->GetX() + GetParentDialog()->GetWidth() );
	pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG]->SetY( GetParentDialog()->GetY() );
}

void NNewPostOfficeSendItemDlg::Hide()
{
	CControllerMgr::Show( false );

	//[진성] 단체 메시지 보내기 창. => 2008-8-4
	cltClient* pclclient = (cltClient*)pclClient;
	pclclient->DestroyInterface(pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG]);
}

void NNewPostOfficeSendItemDlg::Action()
{
	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;

	TCHAR buffer[ 256 ];
	TCHAR msg[ 127 ];
	TCHAR playername[ 128 ];

	if( m_pMyInventory->IsMyInventoryChanged() ) {
		m_pMyInventory->LoadMyInventory();
	}

	MStrCpy( playername, GetEditText( EDIT_PLAYERNAME ), 32 );
//	MStrCpy( msg, GetEditText( EDIT_MSG ), 256 );
	MStrCpy( msg, GetEditText( EDIT_MSG ),127 );
	

	int len = _tcslen( msg );
	//[GOONZU-589 김영훈 수정 : 최대 입력 표시 글자를 127에서 126으로 수정]
	StringCchPrintf( buffer, 256, TEXT("To [%s] (%d/126)bytes"), playername, len );
	SetEditText( EDIT_RECEIVERNAME, buffer );

}

void NNewPostOfficeSendItemDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;

	TCHAR buffer[ 256 ];

	CASE_CONTROL( LISTVIEW_MYINVENTORY )
	{
		switch( nEvent )
		{
			case EVENT_LISTVIEW_LBUTTONDOWN:
			{
				m_pMyInventory->SelectAction();

				StringCchPrintf( buffer, 256, TEXT("%d"), m_pMyInventory->m_clSelectedItem.GetItemNum() );				
				SetEditText( EDIT_AMOUNT, buffer );
				SetEditText( EDIT_ITEMNAME, (TCHAR*)pclClient->pclItemManager->GetName( m_pMyInventory->m_clSelectedItem.siUnique ) );
			}
			break;
		}
	}

	else CASE_CONTROL( BUTTON_SENDITEM )
	{
		TCHAR buf[ 256 ];

		TCHAR itemname[ 64 ];

		MStrCpy( itemname, GetEditText( EDIT_ITEMNAME ), 64 );

		if( m_pMyInventory->m_siSelectedIndex < 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1726);
			SetMainExplain( pText );
			return;
		}

		SI32 siUserCount = 0;
		if ( pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG] )
		{
			siUserCount = ((NPostOfficeGroupListDlg*)pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG])->GetListCountNum();
		}

		if ( 0 >= siUserCount )
		{
			TCHAR* pText = GetTxtFromMgr(1725);
			SetMainExplain( pText );
			return;
		}

		SI32 amount = _tstoi( GetEditText( EDIT_AMOUNT ) );
		if ( amount < 1 || 
			 amount > m_pMyInventory->m_clSelectedItem.siItemNum )
		{
			TCHAR* pText = GetTxtFromMgr(1727);
			SetMainExplain( pText );
			return;
		}				

		//KHY - 0614 - 가차 아이템은 같은 계정간 캐릭터에 한하여 우편으로 주고 받을 수 있다. 수수료가 없다.
		bool isGacha = false; 
		for(SI16 i =0; i <GACHA_MAX_RARELIST  ; i++)
		{
			if(pclClient->pclGachaManager->m_GachaRareList[i] == m_pMyInventory->m_clSelectedItem.siUnique )
			{
				isGacha = true; 
				break;
			}
		}

		if(isGacha == true)
		{
			TCHAR* pText = GetTxtFromMgr(8681);

			TCHAR num[128]= TEXT(" ") ;
			StringCchPrintf(num, 128, TEXT("%d"),amount);
			Nsprintf(buf, pText, TEXT("item"), itemname, TEXT("num"), num , NULL);		
		}
		else
		{
			float feerate = (float)(pPostOfficeManager->clClientStrInfo.clPostOfficeStrInfo.clItemFee.GetFeeRate() / 10.0);
			
			TCHAR* pText = GetTxtFromMgr(8682);

			TCHAR num[128]= TEXT(" ") ;
			TCHAR feerate1[128]= TEXT(" ") ;
			StringCchPrintf(num, 128, TEXT("%d"),amount);
			StringCchPrintf(feerate1, 128, TEXT("%.1f"),feerate);
			Nsprintf(buf,pText, TEXT("item"), itemname, TEXT("num"), num, TEXT("feerate"), feerate1, NULL);
		}

		TCHAR* pText = GetTxtFromMgr(1729);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		
		MsgBox.Set( pParent, pText, buf, MSGBOX_TYPE_YESNO, 10 );
		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( amount );
		memcpy( TempBuffer, &amount, sizeof( amount ) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
	}

	else CASE_CONTROL( BUTTON_SENDMSG )
	{
		TCHAR szmsg[ 128 ];

		MStrCpy( szmsg, GetEditText( EDIT_MSG ), 128 );
		
#ifdef _IAF_EXPORT
		if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
		{
			TCHAR ResultBuf[1024] = "";
			if( 1 == IAF::Filtering( szmsg, ResultBuf, sizeof(ResultBuf) ) )
			{
				TCHAR* pTitle = GetTxtFromMgr(1853);
				TCHAR* pText = GetTxtFromMgr(1854);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return;
			}
			
		}
#else	
		//cyj 역창메세지에도 욕설필터 적용
		if (pclclient->m_pCAbuseFilter->CheckAbuse( szmsg ) == false)
		{
			TCHAR* pTitle = GetTxtFromMgr(1853);
			TCHAR* pText = GetTxtFromMgr(1854);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}
#endif
		
		SI32 siUserCount = 0;
		if ( pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG] )
		{
			siUserCount = ((NPostOfficeGroupListDlg*)pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG])->GetListCountNum();
		}

		if ( 0 >= siUserCount )
		{
			TCHAR* pText = GetTxtFromMgr(1725);
			SetMainExplain( pText );
			return;
		}

		// 총 금액 화폐단위 콤마 표시
		TCHAR	szPrice[64]	= { '\0', };
		GMONEY	siPrice		= 500 * siUserCount;
		g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));

		// 출력 메시지
		NTCHARString512 strNotice( GetTxtFromMgr(8683) );
		strNotice.Replace( TEXT("#totalmoney#"), szPrice );

		TCHAR* pText = GetTxtFromMgr(1731);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, strNotice, MSGBOX_TYPE_YESNO, 11 );
		BYTE TempBuffer[ 256 ];
		SI16 Size = sizeof( szmsg );
		memcpy( TempBuffer, szmsg, sizeof( szmsg ) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
	}
}

void NNewPostOfficeSendItemDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 10:	// item 보내기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 amount = *((SI32*)(pData));
				
				cltClient* pclclient = (cltClient*)pclClient;
				if( pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG] )
					((NPostOfficeGroupListDlg*)pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG])->SendItem( m_pMyInventory->m_siSelectedIndex,
																											m_pMyInventory->m_clSelectedItem.siUnique, 
																											amount);
			}
		}
		break;
	case 11:	// 메세지 보내기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				
				TCHAR szmsg[128];
				memcpy( szmsg, pData, sizeof(szmsg) );

				cltClient* pclclient = (cltClient*)pclClient;
				if( pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG] )
					((NPostOfficeGroupListDlg*)pclclient->m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG])->SendMessage( szmsg );
			}
		}
		break;
	}
}
void NNewPostOfficeSendItemDlg :: WriteName(TCHAR *pName)//답장하기 [2007.08.17 손성웅]
{
	SetEditText( EDIT_PLAYERNAME, pName );
}
        