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

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;



NPostOfficeSendItemDlg::NPostOfficeSendItemDlg()
{
	m_pMyInventory = new NStrInventory;
}

NPostOfficeSendItemDlg::~NPostOfficeSendItemDlg()
{
	NDelete( m_pMyInventory );	
}

void NPostOfficeSendItemDlg::InitDlg()
{
	SetControlMap( EDIT_PLAYERNAME, TEXT("editbox_playername") );
	SetControlMap( EDIT_ITEMNAME, TEXT("editbox_itemname") );
	SetControlMap( EDIT_AMOUNT, TEXT("editbox_amount") );
	SetControlMap( EDIT_RECEIVERNAME, TEXT("editbox_receivername") );
	SetControlMap( EDIT_MSG, TEXT("editbox_msg") );

	SetControlMap( LISTVIEW_MYINVENTORY, TEXT("listview_myinventory") );

	SetControlMap( BUTTON_SENDITEM, TEXT("button_senditem") );
	SetControlMap( BUTTON_SENDMSG, TEXT("button_sendmsg") );

	//KHY - 0626 - 메세지  127바이트로 제한.
	((CEdit*)m_pControlMap[ EDIT_MSG ])->SetMaxEditTextLength(127);

	//KHY - 0629 - ID  20바이트로 제한.
	((CEdit*)m_pControlMap[ EDIT_PLAYERNAME ])->SetMaxEditTextLength(20);
	
	// MyInventory
	m_pMyInventory->CreateInventory( MAX_ITEM_PER_PERSON, (CListView*)m_pControlMap[ LISTVIEW_MYINVENTORY ] );

	m_pMyInventory->m_pListView->SetBKColorUse( true );
	m_pMyInventory->m_pListView->SetBorder( true );

	m_pMyInventory->LoadMyInventory();

	//[김영훈 추가:2007.11.21 - 아이템 겟수 입력시 숫자 이외의 다른 값은 입력되지 않도록 수정]
	((CEdit*)m_pControlMap[ EDIT_AMOUNT ])->SetNumberMode( true );



}

void NPostOfficeSendItemDlg::Action()
{
	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;

	TCHAR buffer[ 256 ];
//	TCHAR msg[ 256 ];
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

void NPostOfficeSendItemDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
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

				SetEditText( EDIT_ITEMNAME, 
					(TCHAR*)pclClient->pclItemManager->GetName( m_pMyInventory->m_clSelectedItem.siUnique ) );

			}
			break;
		}
	}

	else CASE_CONTROL( BUTTON_SENDITEM )
	{

		TCHAR buf[ 256 ];

		TCHAR personName[ MAX_PLAYER_NAME ];
		TCHAR itemname[ 64 ];

		MStrCpy( personName, GetEditText( EDIT_PLAYERNAME ), MAX_PLAYER_NAME );
		MStrCpy( itemname, GetEditText( EDIT_ITEMNAME ), 64 );

		if( personName[ 0 ] == NULL )
		{
			TCHAR* pText = GetTxtFromMgr(1725);
			SetMainExplain( pText );
			return;
		}

		//if ( !IsCorrectID(personName) )
		//{
		//	TCHAR* pText = GetTxtFromMgr(2039);
		//	SetMainExplain( pText );
		//	return;
		//}

		if( m_pMyInventory->m_siSelectedIndex < 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1726);
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
			TCHAR* pText = GetTxtFromMgr(20084);

			TCHAR num[128]= TEXT(" ") ;
			StringCchPrintf(num, 128, TEXT("%d"),amount);
			Nsprintf(buf,pText,TEXT("user"),personName,TEXT("item"), itemname   ,TEXT("num"), num ,NULL);		
		}
		else
		{
			float feerate = (float)(pPostOfficeManager->clClientStrInfo.clPostOfficeStrInfo.clItemFee.GetFeeRate() / 10.0);
			
			TCHAR* pText = GetTxtFromMgr(1728);

			TCHAR num[128]= TEXT(" ") ;
			TCHAR feerate1[128]= TEXT(" ") ;
			StringCchPrintf(num, 128, TEXT("%d"),amount);
			StringCchPrintf(feerate1, 128, TEXT("%.1f"),feerate);
			Nsprintf(buf,pText,TEXT("user"),personName,TEXT("item"), itemname   ,TEXT("num"), num   ,TEXT("feerate"),  feerate1  ,NULL);
		}

		//sprintf( buf, pText, personName, itemname, amount, feerate);

		TCHAR* pText = GetTxtFromMgr(1729);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buf, MSGBOX_TYPE_YESNO, 0 );
		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( personName ) + sizeof( amount );
		memcpy( TempBuffer, personName, sizeof( personName ) );
		memcpy( &TempBuffer[ sizeof(personName) ], &amount, sizeof( amount ) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

		//if( MessageBox(NULL, buf, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) return;

		//cltGameMsgRequest_PostOfficeSendItem sendMsg( 
		//	 personName, 
		//	 m_pMyInventory->m_siSelectedIndex,
		//	 m_pMyInventory->m_clSelectedItem.siUnique, 
		//	 amount );

		//cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SENDITEM, 
		//		sizeof(cltGameMsgRequest_PostOfficeSendItem), (BYTE*)&sendMsg);
		//
		//pclClient->SendMsgToServer( (BYTE*)&clMsg );

	}

	else CASE_CONTROL( BUTTON_SENDMSG )
	{
		TCHAR buf[ 256 ];
		TCHAR personName[ 32 ];
		TCHAR szmsg[ 128 ];

		MStrCpy( personName, GetEditText( EDIT_PLAYERNAME ), MAX_PLAYER_NAME );
		MStrCpy( szmsg, GetEditText( EDIT_MSG ), 128 );
		
		if( personName[ 0 ] == NULL )
		{
			TCHAR* pText = GetTxtFromMgr(1725);
			SetMainExplain( pText );
			return;
		}

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
		TCHAR* pText = GetTxtFromMgr(1730);
		StringCchPrintf( buf, 256, 
			pText,
			personName, 
			pPostOfficeManager->clClientStrInfo.clPostOfficeStrInfo.siMsgFee );

		pText = GetTxtFromMgr(1731);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buf, MSGBOX_TYPE_YESNO, 1 );
		BYTE TempBuffer[ 256 ];
		SI16 Size = sizeof( personName ) + sizeof( szmsg );
		memcpy( TempBuffer, personName, sizeof( personName ) );
		memcpy( &TempBuffer[ sizeof(personName) ], szmsg, sizeof( szmsg ) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

		//if( MessageBox(NULL, buf, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) return;;

		//cltGameMsgRequest_PostOfficeSendMsg sendMsg( personName, szmsg );
		//
		//cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SENDMSG, 
		//		sizeof(cltGameMsgRequest_PostOfficeSendMsg), (BYTE*)&sendMsg);
		//
		//pclClient->SendMsgToServer( (BYTE	*)&clMsg );
	}
}

void NPostOfficeSendItemDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// item 보내기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				TCHAR personName[20];
				memcpy( personName, pData, sizeof(personName));
				SI32 amount = *((SI32*)(pData + sizeof(personName) ));

				cltGameMsgRequest_PostOfficeSendItem sendMsg( 
					 personName, 
					 m_pMyInventory->m_siSelectedIndex,
					 m_pMyInventory->m_clSelectedItem.siUnique, 
					 amount );

				cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SENDITEM, 
						sizeof(cltGameMsgRequest_PostOfficeSendItem), (BYTE*)&sendMsg);
				
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
		}
		break;
	case 1:	// 메세지 보내기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				TCHAR personName[32];
				TCHAR szmsg[128];
				memcpy( personName, pData, sizeof(personName));
				memcpy( szmsg, pData + sizeof(personName), sizeof(szmsg) );

				cltGameMsgRequest_PostOfficeSendMsg sendMsg( personName, szmsg );
				
				cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SENDMSG, 
						sizeof(cltGameMsgRequest_PostOfficeSendMsg), (BYTE*)&sendMsg);
				
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
		}
		break;
	}
}
void NPostOfficeSendItemDlg :: WriteName(TCHAR *pName)//답장하기 [2007.08.17 손성웅]
{
	SetEditText( EDIT_PLAYERNAME, pName );
}
        