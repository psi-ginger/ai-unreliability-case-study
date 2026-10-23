/* ==========================================================================
	클래스 :		CNFriendAddAcceptDlg

	작성일 :		05/03/05
	작성자 :		정용래

	목  적 :		CNFriendAddAcceptDlg 클래스는  전체 화면 모드 실행시 NInterface에
					따라 게임 화면 안에서 친구 추가 허락 다이얼로그를 생성한다.

	용  법 :		

	변경사항 :		없음
	
	차후작업 :		cpp 참고

   ========================================================================*/

//#if (defined(__LEON_START_ALL) || defined(__LEON_NFRIENDDLG__)) && defined(_DEBUG)

#include "NFriendAddAcceptDlg.h"

#include "../../Client/client.h"
#include "../../CommonLogic/Msg/MsgType-Friend.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../common/SMLib/LibList/LibListMgr.h"
#include "../../common/SMLib/LibList/LibList.h"
#include "../../common/SMLib/LibList/Obj.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/interface/Edit.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

#define MAX_NACCEPTLISTOBJ_BUFFER_SIZE		128

#define NFRIENDACCEPTDLG_TIMER_VALUE			10000
extern SI32 gsiResolution;

CNFriendAddAcceptDlg::CNFriendAddAcceptDlg()
{
	m_bShow = false;

	m_pAcceptListMgr = new CLibListMgr();
	m_pAcceptListMgr->CreateMaxEmptyObj( MAX_FRIEND_NUMBER, MAX_NACCEPTLISTOBJ_BUFFER_SIZE );

	m_pFriendAddExplainEdit = NULL; 
	m_pFriendAddAcceptButton = NULL;
	m_pFriendAddRefuseButton = NULL;
}

CNFriendAddAcceptDlg::~CNFriendAddAcceptDlg()
{
	if ( m_pAcceptListMgr )
	{
		delete m_pAcceptListMgr;
		m_pAcceptListMgr = NULL;
	}

	if(m_pFriendAddExplainEdit)
	{
		delete m_pFriendAddExplainEdit; 
	}
	if(m_pFriendAddAcceptButton)
	{
		delete m_pFriendAddAcceptButton;
		m_pFriendAddAcceptButton = NULL;
	}

	if(m_pFriendAddRefuseButton)
	{
		delete m_pFriendAddRefuseButton;
		m_pFriendAddRefuseButton = NULL;
	}

	SetTimerOnOff( false );
}

void CNFriendAddAcceptDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;
	TCHAR* pText= GetTxtFromMgr(3354);

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/DLG_NFriendAddAccept.ddf"));
	file.CreatePopupDialog(this, NFRIENDACCEPT_DLG, TEXT("dialog_friendaddaccept"), StatiCNFriendAddAcceptDlgProc );

	if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		MovePos( 720, 66 );			
	else
		MovePos( 554, 66 );

	m_pFriendAddExplainEdit = new CEdit( this );             
	m_pFriendAddAcceptButton = new CButton( this );
	m_pFriendAddRefuseButton = new CButton( this );

	SetTitle(GetTxtFromMgr(4427));//친구등록


	file.CreateControl(m_pFriendAddExplainEdit, NFRIENDACCEPTDLG_DIALOG_FRIENDADDEXPLAINEDIT, TEXT("editbox_friendaddexplain"));
	file.CreateControl(m_pFriendAddAcceptButton, NFRIENDACCEPTDLG_DIALOG_FRIENDADDACCEPTBUTTON, TEXT("button_friendaddaccept"));
	file.CreateControl(m_pFriendAddRefuseButton, NFRIENDACCEPTDLG_DIALOG_FRIENDADDREFUSEBUTTON, TEXT("button_friendaddrefuse"));

	m_siPersonID = 0;
	m_siCharID = 0;

	SetTimerOnOff( true );
	SetActionTimer( NFRIENDACCEPTDLG_TIMER_VALUE );

	Hide();

	return;
}

void CALLBACK CNFriendAddAcceptDlg::StatiCNFriendAddAcceptDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNFriendAddAcceptDlg *pThis = (CNFriendAddAcceptDlg*) pControllerMgr;
	pThis->NFriendAddAcceptDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNFriendAddAcceptDlg::NFriendAddAcceptDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;
	case NFRIENDACCEPTDLG_DIALOG_FRIENDADDACCEPTBUTTON:
		{
			if ( m_siPersonID < 1 || m_strCharName == NULL || _tcslen( m_strCharName ) < 1 )
			{
		
			}
			else
			{
				cltGameMsgRequest_FriendAddAccept clFriendAddAccept( m_siGroupID, m_siPersonID, m_siCharID, m_strCharName );
				cltMsg clMsg( GAMEMSG_REQUEST_FRIENDADDACCEPT, sizeof( clFriendAddAccept ), (BYTE*)&clFriendAddAccept );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}

			Hide();
		}
		break;
	case NFRIENDACCEPTDLG_DIALOG_FRIENDADDREFUSEBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_siPersonID < 1 || m_strCharName == NULL || _tcslen( m_strCharName ) < 1 )
					{

					}
					else
					{
						cltGameMsgRequest_FriendAddAccept clFriendAddReject( m_siGroupID, m_siPersonID, m_siCharID, m_strCharName );
						cltMsg clMsg( GAMEMSG_REQUEST_REJECTFRIENDADD, sizeof( clFriendAddReject ), (BYTE*)&clFriendAddReject );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}

					Hide();
				}
				break;
			}
		}
		break;
	}

	return ;
};

bool CNFriendAddAcceptDlg::IsShow()
{
	return m_bShow;
}

void CNFriendAddAcceptDlg::Show()
{
	m_bShow = true;

	CDialogController::Show( true );

	return;
}

void CNFriendAddAcceptDlg::Hide()
{
	m_siPersonID = 0;
	m_siCharID = 0;
	memset( m_strCharName, 0, MAX_PLAYER_NAME );
	
	m_bShow = false;

	CDialogController::Show( false );

	return;
}

void CNFriendAddAcceptDlg::PushAccpetList( SI08 GroupID, SI32 PersonID, SI32 CharID, TCHAR *CharName )
{	
	cltClient *pclclient = (cltClient *)pclClient;
	
	if( pclclient->bWatchModeSwitch )
	{
		return;
	}
		
	if ( CharName == NULL || _tcslen( CharName ) < 1 || PersonID < 1 || CharID < 1 )
	{
		return;
	}

	if ( m_siPersonID == PersonID )
	{
		return;
	}

	CObj *pObj = m_pAcceptListMgr->GetExistList()->GetFirst();

	// 리스트에 있는 지 확인하기
	while ( pObj != NULL )
	{
		stNAcceptObj *pAcceptObj = (stNAcceptObj *)pObj->GetDataPtr();
		if ( pAcceptObj != NULL )
		{
			if ( pAcceptObj->siPersonID == PersonID )
			{
				return;
			}
		}

		pObj = pObj->GetNext();
	}


	stNAcceptObj sAcceptObj;
	
	sAcceptObj.Set( GroupID, PersonID, CharID, CharName );


	pObj = m_pAcceptListMgr->PopFrontToEmptyListAndPushBackToExistList();
	
	if ( pObj == NULL )
	{
		return;
	}

	pObj->SetData( (BYTE*)&sAcceptObj, sizeof( stNAcceptObj ) );
	
	return;
}

void CNFriendAddAcceptDlg::PopAcceptList()
{
	if ( IsShow() )
	{
		return;
	}

	CObj *pObj = m_pAcceptListMgr->GetExistList()->GetFirst();

	if ( pObj == NULL )
	{
		return;
	}

	stNAcceptObj *pAcceptObj = (stNAcceptObj *)pObj->GetDataPtr();

	if ( pAcceptObj == NULL )
	{
		return;
	}

	TCHAR Buffer[256 ];

	//TCHAR* pTitle = GetTxtFromMgr(6295);//친구추가요청이 왔을때 타이틀에 '친구 추가' 라는 텍스트 추가.
	TCHAR* pText= GetTxtFromMgr(3355);
	//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
	StringCchPrintf( Buffer, 256, pText, pAcceptObj->strCharName );
	


	m_siGroupID = pAcceptObj->siGroupID;
	m_siPersonID = pAcceptObj->siPersonID;
	m_siCharID = pAcceptObj->siCharID;
	StringCchCopy( m_strCharName, MAX_PLAYER_NAME, pAcceptObj->strCharName );

	m_pFriendAddExplainEdit->SetText( Buffer );

	Show();

	m_pAcceptListMgr->GetExistList()->Pop( pObj );
	pObj->Initialize();
	m_pAcceptListMgr->PushBackToEmptyList( pObj );

	return;
}

// WM_TIMER 메세지 처리 루틴
void CNFriendAddAcceptDlg::Action()
{
	// 보이지 않는 경우라면, 버퍼에서 꺼내온다.
	if ( ! IsShow() )
	{				
		PopAcceptList();
	}
}
//#endif	//#if defined(__LEON_NFRIENDDLG__) && defined(_DEBUG)
