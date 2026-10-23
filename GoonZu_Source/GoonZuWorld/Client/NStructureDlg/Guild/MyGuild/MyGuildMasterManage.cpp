

#include "../../../Client.h"
#include "../../../InterfaceMgr/InterfaceFile.h"
#include "../../../InterfaceMgr/InterfaceMgr.h"
#include "MyGuildMasterManage.h"

#include "../../../../CommonLogic/Guild/Guild.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNMyGuildMasterManage::CNMyGuildMasterManage()
{
	Initialize();

	m_siSelectListIndex = -1 ;
	m_siSelectCharPersonID = 0 ;
	m_siSelectMemberIndex = -1 ;
}	

CNMyGuildMasterManage::~CNMyGuildMasterManage()
{
	Destroy();
}

void CNMyGuildMasterManage::Initialize()
{

}

void CNMyGuildMasterManage::Destroy()
{

}

void CNMyGuildMasterManage::Create( CControllerMgr *pParent )
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/Guild/DLG_MyGuildMasterManage.ddf"));
	
		file.CreateChildDialog( this, NMYGUILDMASTERMANAGE_DLG, TEXT("dialog_MyGuildMasterManage"),StaticMyGuildMasterManageDlgProc , pParent);

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST   ,NMYGUILDMASTERMANAGE_LIST1 , this)  , NMYGUILDMASTERMANAGE_LIST1 , TEXT("listbox_NONAME1"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDMASTERMANAGE_BUTTON_1 , this)  , NMYGUILDMASTERMANAGE_BUTTON_1 , TEXT("button_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDMASTERMANAGE_BUTTON_2 , this)  , NMYGUILDMASTERMANAGE_BUTTON_2 , TEXT("button_NONAME2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDMASTERMANAGE_BUTTON_3 , this)  , NMYGUILDMASTERMANAGE_BUTTON_3 , TEXT("button_NONAME3"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDMASTERMANAGE_BUTTON_5 , this)  , NMYGUILDMASTERMANAGE_BUTTON_5 , TEXT("button_NONAME5"));

		// 리스트 
		CList *pList = m_InterfaceFactory.GetList(NMYGUILDMASTERMANAGE_LIST1);

		pList->SetBorder(true);
		pList->SetBKColorUse(true);
 
		TCHAR * pText = GetTxtFromMgr(5248);
		pList->SetColumn( 0, 80, pText  );

		pText = GetTxtFromMgr(5271);

		pList->SetColumn( 1, 80, pText );

		pText = GetTxtFromMgr(6598);

		pList->SetColumn( 2, 80, pText );


		pText = GetTxtFromMgr(6501);


		pList->SetColumn( 3, 80, pText );

		pText = GetTxtFromMgr(6509);
		pList->SetColumn( 4, 30, pText );

		pList->Refresh();

		if( pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.GetRankType() != GUILDRANKTYPE_MASTER )
		{
			m_InterfaceFactory.GetButton(NMYGUILDMASTERMANAGE_BUTTON_3)->Enable(false);
			m_InterfaceFactory.GetButton(NMYGUILDMASTERMANAGE_BUTTON_5)->Enable(false);
		}
		RefreshManager();
	}
}

void CALLBACK CNMyGuildMasterManage::StaticMyGuildMasterManageDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMyGuildMasterManage *pThis = (CNMyGuildMasterManage*) pControllerMgr;
	pThis->NMyGuildMasterManageDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNMyGuildMasterManage::NMyGuildMasterManageDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;
	cltGuildManager *pGuildManager = ((cltGuildManager *)pclClient->pclGuildManager);

	switch(nControlID)
	{
	case NMYGUILDMASTERMANAGE_BUTTON_1: // 가입창
		{
			pclclient->CreateInterface(NBEGUILDMEMBER_DLG);
		}
		break;
	case NMYGUILDMASTERMANAGE_BUTTON_2: // 퇴출시키기
		{
			if(m_siSelectMemberIndex < 0 || m_siSelectMemberIndex >= MAX_GUILD_MEMBER_NUM)			return;

			TCHAR buffer[256] = "" ;
			TCHAR * pName = pGuildManager->clCommonUnit.clMember[m_siSelectMemberIndex].szCharName;
			if ( pName == NULL )
				break ;

			if ( m_siSelectCharPersonID <= 0 )
				break;

			TCHAR * pTitle = GetTxtFromMgr(6527);
			TCHAR * pText = GetTxtFromMgr(6553);
			sprintf(buffer,pText,pName);

			stMsgBox MsgBox;
			// 현재 dialog가 탭 창 같은 Child Dialog일때
			CControllerMgr *pParent = NULL;
			pParent = this->GetParentDialog();
			if(pParent == NULL ) return;
			MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 1 );

			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( m_siSelectCharPersonID );
			memcpy( TempBuffer, &m_siSelectCharPersonID, sizeof( m_siSelectCharPersonID ) );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		}
		break;
	case NMYGUILDMASTERMANAGE_BUTTON_3: // 부길드장으로 승격/부길드장에서 일반 회원으로 강등
		{
			if(m_siSelectMemberIndex < 0 || m_siSelectMemberIndex >= MAX_GUILD_MEMBER_NUM)			return;

			if ( pGuildManager->clCommonUnit.clMember[m_siSelectMemberIndex].siType == GUILDRANKTYPE_SECONDMASTER )
			{
				TCHAR buffer[256] = "" ;
				TCHAR * pName = pGuildManager->clCommonUnit.clMember[m_siSelectMemberIndex].szCharName;
				if ( pName == NULL )
					break ;

				if ( m_siSelectCharPersonID <= 0 )
					break;

				TCHAR * pTitle = GetTxtFromMgr(6547);
				TCHAR * pText = GetTxtFromMgr(6555);
				StringCchPrintf(buffer, 256, pText, pName);

				stMsgBox MsgBox;
				// 현재 dialog가 탭 창 같은 Child Dialog일때
				CControllerMgr *pParent = NULL;
				pParent = this->GetParentDialog();
				if(pParent == NULL ) return;
				MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 2 );

				BYTE TempBuffer[ 128 ];
				SI16 Size = sizeof( m_siSelectCharPersonID );
				memcpy( TempBuffer, &m_siSelectCharPersonID, sizeof( m_siSelectCharPersonID ) );
				pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
			}
			else if ( pGuildManager->clCommonUnit.clMember[m_siSelectMemberIndex].siType == GUILDRANKTYPE_MEMBER )
			{
				TCHAR buffer[256] = "" ;
				TCHAR * pName = pGuildManager->clCommonUnit.clMember[m_siSelectMemberIndex].szCharName;
				if ( pName == NULL )
					break ;

				if ( m_siSelectCharPersonID <= 0 )
					break;

				if ( pGuildManager->clCommonUnit.IsSecondMaster() == true )
				{
					TCHAR * pTitle = GetTxtFromMgr(6541);
					TCHAR * pText = GetTxtFromMgr(6635);
					pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
					break;
				}

				TCHAR * pTitle = GetTxtFromMgr(6547);
				TCHAR * pText = GetTxtFromMgr(6554);
				sprintf(buffer,pText,pName);

				stMsgBox MsgBox;
				// 현재 dialog가 탭 창 같은 Child Dialog일때
				CControllerMgr *pParent = NULL;
				pParent = this->GetParentDialog();
				if(pParent == NULL ) return;
				MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 3 );

				BYTE TempBuffer[ 128 ];
				SI16 Size = sizeof( m_siSelectCharPersonID );
				memcpy( TempBuffer, &m_siSelectCharPersonID, sizeof( m_siSelectCharPersonID ) );
				pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
			}
		}
		break;
	case NMYGUILDMASTERMANAGE_BUTTON_5: // 길드 마스터 권한 위임
		{
			if(m_siSelectMemberIndex < 0 || m_siSelectMemberIndex >= MAX_GUILD_MEMBER_NUM)			return;

			if ( pGuildManager->clCommonUnit.clMember[m_siSelectMemberIndex].siType == GUILDRANKTYPE_MASTER )
				break;

			TCHAR buffer[256] = "" ;
			TCHAR* pName = pGuildManager->clCommonUnit.clMember[m_siSelectMemberIndex].szCharName;
			if ( pName == NULL )
				break ;

			if ( m_siSelectCharPersonID <= 0 )
				break;

			TCHAR * pTitle = GetTxtFromMgr(6547);
			TCHAR * pText = GetTxtFromMgr(6650);
			StringCchPrintf(buffer, 256, pText, pName);

			stMsgBox MsgBox;
			// 현재 dialog가 탭 창 같은 Child Dialog일때
			CControllerMgr *pParent = NULL;
			pParent = this->GetParentDialog();
			if(pParent == NULL ) return;
			MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 6 );

			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( m_siSelectCharPersonID );
			memcpy( TempBuffer, &m_siSelectCharPersonID, sizeof( m_siSelectCharPersonID ) );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		}
		break;
	case NMYGUILDMASTERMANAGE_LIST1:
		{
			switch(nEvent)
			{
			case EVENT_LISTBOX_SELECTION:
				{
					CList *pList = m_InterfaceFactory.GetList(NMYGUILDMASTERMANAGE_LIST1) ;

					SI16 siRow = -1, siCol = -1;
					pList->GetSelectedIndex(&siRow,&siCol);
					if( siRow == -1 || siCol == -1 )
						break;

					m_siSelectListIndex = siRow ;
					stListBoxItemData* pBox = pList->GetItemData(m_siSelectListIndex);

					for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
					{
						if ( !strcmp(pBox->strText[0],pGuildManager->clCommonUnit.clMember[i].szCharName) )
						{
							m_siSelectCharPersonID = pGuildManager->clCommonUnit.clMember[i].siPersonID;
							if ( m_siSelectCharPersonID <= 0 )
								break;

							if ( pGuildManager->clCommonUnit.clMember[i].siType == GUILDRANKTYPE_MASTER )
							{
								m_InterfaceFactory.GetButton(NMYGUILDMASTERMANAGE_BUTTON_2)->Enable(false);
								TCHAR* pText = GetTxtFromMgr(6512);
								m_InterfaceFactory.GetButton(NMYGUILDMASTERMANAGE_BUTTON_3)->SetText(pText);
								m_InterfaceFactory.GetButton(NMYGUILDMASTERMANAGE_BUTTON_3)->Enable(false);
							}
							else if ( pGuildManager->clCommonUnit.clMember[i].siType == GUILDRANKTYPE_SECONDMASTER )
							{
								TCHAR* pText = GetTxtFromMgr(6552);
								m_InterfaceFactory.GetButton(NMYGUILDMASTERMANAGE_BUTTON_3)->SetText(pText);
								//[(LUM71)김영훈 수정:2007.11.27 - 길드 오피스가 자신을 강퇴할 수 없게 강퇴버튼 false로수정]
								m_InterfaceFactory.GetButton(NMYGUILDMASTERMANAGE_BUTTON_2)->Enable(false);
								m_InterfaceFactory.GetButton(NMYGUILDMASTERMANAGE_BUTTON_3)->Enable(true);
							}
							else
							{
								TCHAR* pText = GetTxtFromMgr(6512);
								m_InterfaceFactory.GetButton(NMYGUILDMASTERMANAGE_BUTTON_3)->SetText(pText);
								m_InterfaceFactory.GetButton(NMYGUILDMASTERMANAGE_BUTTON_2)->Enable(true);
								m_InterfaceFactory.GetButton(NMYGUILDMASTERMANAGE_BUTTON_3)->Enable(true);
							}

							m_siSelectMemberIndex = i ;

							break;
						}
					}
				}
				break;
			}
		}
		break;
	}
}


void CNMyGuildMasterManage::Hide()
{
	CControllerMgr::Show(false);

}

void CNMyGuildMasterManage::Show()
{
	CControllerMgr::Show(true);

}

void CNMyGuildMasterManage::RefreshManager()
{
	cltGuildManager *pGuildManager = ((cltGuildManager *)pclClient->pclGuildManager);
	stListBoxItemData itemdata;
	CList *pList = m_InterfaceFactory.GetList(NMYGUILDMASTERMANAGE_LIST1);

	TCHAR buffer[256] = "" ;

	// 리스트뷰를 모두 지운다. 
	pList->Clear();

	for( SI32 i = 0; i < MAX_GUILD_MEMBER_NUM; i++ )
	{
		itemdata.Init();

		if( pGuildManager->clCommonUnit.clMember[i].siPersonID != 0 )
		{
			itemdata.Set( 0, (TCHAR*)pGuildManager->clCommonUnit.clMember[i].szCharName );
			if ( pGuildManager->clCommonUnit.clMember[i].siType == GUILDRANKTYPE_MASTER )
			{
				TCHAR * pText = GetTxtFromMgr(6540);
				itemdata.Set( 1, pText );
			}
			else if ( pGuildManager->clCommonUnit.clMember[i].siType == GUILDRANKTYPE_SECONDMASTER )
			{
				TCHAR * pText = GetTxtFromMgr(6541);
				itemdata.Set( 1, pText );
			}
			else
			{
				TCHAR * pText = GetTxtFromMgr(6542);
				itemdata.Set( 1, pText );
			}

			StringCchPrintf(buffer, 256, TEXT("%d"), pGuildManager->clCommonUnit.clMember[i].siGP );
			itemdata.Set(2,buffer);

			StringCchPrintf(buffer, 256, TEXT("%I64d"), pGuildManager->clCommonUnit.clMember[i].siMoney );
			itemdata.Set(3,buffer);

			pList->AddItem( &itemdata );
		}
	}

	pList->Refresh();
}
