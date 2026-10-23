#include "NFriendChangeGroupNameDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "../../common/Char/CharManager/CharManager.h"
#include "../../CommonLogic/Msg/MsgType-Friend.h"

extern cltCommonLogic* pclClient;

enum
{
	NFRIENDCHANGEGROUPNAME_FIRST = 1000,
	NFRIENDCHANGEGROUPNAME_EDIT_GROUP1NAME,
	NFRIENDCHANGEGROUPNAME_EDIT_GROUP2NAME,
	NFRIENDCHANGEGROUPNAME_EDIT_GROUP3NAME,
	NFRIENDCHANGEGROUPNAME_EDIT_GROUP4NAME,
	NFRIENDCHANGEGROUPNAME_BUTTON_OK,
	NFRIENDCHANGEGROUPNAME_BUTTON_CANCEL,
};

CNFriendChangeGroupNameDlg::CNFriendChangeGroupNameDlg()
{
	Initialize();
}

CNFriendChangeGroupNameDlg::~CNFriendChangeGroupNameDlg()
{
	Destroy();
}

void CNFriendChangeGroupNameDlg::Initialize()
{

}

void CNFriendChangeGroupNameDlg::Destroy()
{

}

void CNFriendChangeGroupNameDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/DLG_NFriendChangeGroupName.ddf"));
	file.CreatePopupDialog( this, NFRIENDCHANGEGROUPNAME_DLG, TEXT("dialog_friendchangegroupname"), StaticFriendChangeGroupNameDlgProc );
		
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT ,NFRIENDCHANGEGROUPNAME_EDIT_GROUP1NAME, this) ,NFRIENDCHANGEGROUPNAME_EDIT_GROUP1NAME, TEXT("editbox_friendgroup1name"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT ,NFRIENDCHANGEGROUPNAME_EDIT_GROUP2NAME, this) ,NFRIENDCHANGEGROUPNAME_EDIT_GROUP2NAME, TEXT("editbox_friendgroup2name"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT ,NFRIENDCHANGEGROUPNAME_EDIT_GROUP3NAME, this) ,NFRIENDCHANGEGROUPNAME_EDIT_GROUP3NAME, TEXT("editbox_friendgroup3name"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT ,NFRIENDCHANGEGROUPNAME_EDIT_GROUP4NAME, this) ,NFRIENDCHANGEGROUPNAME_EDIT_GROUP4NAME, TEXT("editbox_friendgroup4name"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NFRIENDCHANGEGROUPNAME_BUTTON_OK, this)  ,		NFRIENDCHANGEGROUPNAME_BUTTON_OK , TEXT("button_friendok"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NFRIENDCHANGEGROUPNAME_BUTTON_CANCEL, this)  , NFRIENDCHANGEGROUPNAME_BUTTON_CANCEL , TEXT("button_friendcancel"));

	CEdit* pEdit1 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP1NAME);
	CEdit* pEdit2 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP2NAME);
	CEdit* pEdit3 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP3NAME);
	CEdit* pEdit4 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP4NAME);

	if ( pEdit1 && pEdit2 && pEdit3 && pEdit4 )
	{
		SI32 maxTextLength = MAX_FRIEND_GROUPNAME;
		if ( pclClient->siServiceArea == ConstServiceArea_China )
		{
			// 중국은 12자 까지만 입력가능
			maxTextLength = 13;
		}
		if (	pclClient->siServiceArea == ConstServiceArea_USA		 
			 ||	pclClient->siServiceArea == ConstServiceArea_English 
			 ||	pclClient->siServiceArea == ConstServiceArea_NHNChina
			 ||	pclClient->siServiceArea == ConstServiceArea_EUROPE
		)
		{
			// usa은 11자 까지만 입력가능
			maxTextLength = 11;
		}
		pEdit1->SetMaxEditTextLength( maxTextLength );
		pEdit2->SetMaxEditTextLength( maxTextLength );
		pEdit3->SetMaxEditTextLength( maxTextLength );
		pEdit4->SetMaxEditTextLength( maxTextLength );
	}


}

void CALLBACK CNFriendChangeGroupNameDlg::StaticFriendChangeGroupNameDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNFriendChangeGroupNameDlg *pThis = (CNFriendChangeGroupNameDlg*) pControllerMgr;
	pThis->NFriendChangeGroupNameDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK CNFriendChangeGroupNameDlg::NFriendChangeGroupNameDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
	case NFRIENDCHANGEGROUPNAME_BUTTON_CANCEL:
		{
			DeleteDialog();
		}
		break;
	case NFRIENDCHANGEGROUPNAME_BUTTON_OK:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				cltClient* pclclient = (cltClient*)pclClient;

				CEdit* pEdit1 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP1NAME);
				CEdit* pEdit2 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP2NAME);
				CEdit* pEdit3 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP3NAME);
				CEdit* pEdit4 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP4NAME);

				TCHAR* pText1 = pEdit1->GetText();
				TCHAR* pText2 = pEdit2->GetText();
				TCHAR* pText3 = pEdit3->GetText();
				TCHAR* pText4 = pEdit4->GetText();

#ifdef _IAF_EXPORT
				if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
				{
					TCHAR ResultBuf[1024] = "";
					IAF::Filtering( pText1, ResultBuf, sizeof(ResultBuf), IAF::FILTER_CHARACTER );
					memcpy( pText1, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
					
					ZeroMemory(ResultBuf, sizeof(ResultBuf));
					IAF::Filtering( pText2, ResultBuf, sizeof(ResultBuf), IAF::FILTER_CHARACTER );
					memcpy( pText2, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
					
					ZeroMemory(ResultBuf, sizeof(ResultBuf));
					IAF::Filtering( pText3, ResultBuf, sizeof(ResultBuf), IAF::FILTER_CHARACTER );
					memcpy( pText3, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
					
					ZeroMemory(ResultBuf, sizeof(ResultBuf));
					IAF::Filtering( pText4, ResultBuf, sizeof(ResultBuf), IAF::FILTER_CHARACTER );
					memcpy( pText4, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
				}
#else
				// 욕설 필터링 적용
				pclclient->m_pCAbuseFilter->CheckAbuse(pText1);
				pclclient->m_pCAbuseFilter->CheckAbuse(pText2);
				pclclient->m_pCAbuseFilter->CheckAbuse(pText3);
				pclclient->m_pCAbuseFilter->CheckAbuse(pText4);
#endif			
				size_t textSize1 = _tcslen( pText1 );
				size_t textSize2 = _tcslen( pText2 );
				size_t textSize3 = _tcslen( pText3 );
				size_t textSize4 = _tcslen( pText4 );

				size_t maxTextLength = MAX_FRIEND_GROUPNAME;
				if ( pclClient->siServiceArea == ConstServiceArea_China )
				{
					// 중국은 12자 까지만 입력가능
					maxTextLength = 13;
				}

				if ( (textSize1 >= maxTextLength || textSize1 < 1) ||
					 (textSize2 >= maxTextLength || textSize2 < 1) ||
					 (textSize3 >= maxTextLength || textSize3 < 1) ||
					 (textSize4 >= maxTextLength || textSize4 < 1) )
				{
					TCHAR* pErrText = GetTxtFromMgr(6782);

					TCHAR buffer[256];
					StringCchPrintf( buffer, 256, pErrText, (maxTextLength-1)/2 , maxTextLength-1 );
					pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( TEXT(""), buffer );

					SetGroupName();

					return;
				}

				cltGameMsgRequest_FriendChangeGroupName clinfo; 
				MStrCpy(clinfo.szGroupName[FRIEND_GROUP_1], pText1, MAX_FRIEND_GROUPNAME);
				MStrCpy(clinfo.szGroupName[FRIEND_GROUP_2], pText2, MAX_FRIEND_GROUPNAME);
				MStrCpy(clinfo.szGroupName[FRIEND_GROUP_3], pText3, MAX_FRIEND_GROUPNAME);
				MStrCpy(clinfo.szGroupName[FRIEND_GROUP_4], pText4, MAX_FRIEND_GROUPNAME);

				cltMsg clMsg(GAMEMSG_REQUEST_FRIENDCHANGEGROUPNAME, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

				DeleteDialog();
			}
		}
	default:
		break;
	}
}

void CNFriendChangeGroupNameDlg::SetGroupName()
{
	CEdit* pEdit1 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP1NAME);
	CEdit* pEdit2 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP2NAME);
	CEdit* pEdit3 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP3NAME);
	CEdit* pEdit4 = m_InterfaceFactory.GetEdit(NFRIENDCHANGEGROUPNAME_EDIT_GROUP4NAME);

	if(pclClient->pclCM->CR[1]->pclFriendInfo == NULL)		return;

	if(strlen(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_1]) > 0)
		pEdit1->SetText(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_1]);
	else
	{
		TCHAR *GroupName = NULL;
		GroupName = GetTxtFromMgr( 6868 );
		if ( GroupName )
		{
			pEdit1->SetText( GroupName );
		}
	}

	if(strlen(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_2]) > 0)
		pEdit2->SetText(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_2]);
	else
	{
		TCHAR *GroupName = NULL;
		GroupName = GetTxtFromMgr( 6869 );
		if ( GroupName )
		{
			pEdit2->SetText( GroupName );
		}
	}

	if(strlen(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_3]) > 0)
		pEdit3->SetText(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_3]);
	else
	{
		TCHAR *GroupName = NULL;
		GroupName = GetTxtFromMgr( 6870 );
		if ( GroupName )
		{
			pEdit3->SetText( GroupName );
		}
	}

	if(strlen(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_4]) > 0)
		pEdit4->SetText(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_4]);
	else
	{
		TCHAR *GroupName = NULL;
		GroupName = GetTxtFromMgr( 6871 );
		if ( GroupName )
		{
			pEdit4->SetText( GroupName );
		}
	}

}

