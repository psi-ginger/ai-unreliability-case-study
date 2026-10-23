//
//#include "NMyFamilyInfoDlg.h"
//
//#include "../../InterfaceMgr/InterfaceMgr.h"
//#include "../../InterfaceMgr/InterfaceFile.h"
//#include "../../Client.h"
//
//#include "../../common/Char/CharManager/CharManager.h"
//#include "../../common/Family/Marriage.h"
//
//#include "../../CommonLogic/MsgType.h"
//
//extern cltCommonLogic* pclClient;
//
//CNMyFamilyInfoDlg::CNMyFamilyInfoDlg( void )
//{
//
//}
//
//CNMyFamilyInfoDlg::~CNMyFamilyInfoDlg( void )
//{
//
//}
//
//void CNMyFamilyInfoDlg::Init( )
//{
//		
//}
//
//void CNMyFamilyInfoDlg::Create()
//{
//	//-- 이미 생성되었다면 리턴
//	if ( IsCreate() == true )
//	{
//		DeleteDialog();
//		return;
//	}
//
//	CInterfaceFile file;
//	file.LoadFile( TEXT( "NInterface/Data/NMyFamilyInfoDlg/DLG_NMyFamilyInfoDlg.ddf " ) );
//	file.CreatePopupDialog( this, NMYFAMILYINFO_DLG_WITH_CHILDREN, TEXT( "dialog_familyinfo" ), StaticCallBackDialogNMyFamilyInfoDlg );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	MYFAMILYINFO_DLG_STATIC_ME,		this),	MYFAMILYINFO_DLG_STATIC_ME,		TEXT("statictext_NONAME1") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	MYFAMILYINFO_DLG_STATIC_MATE,	this),	MYFAMILYINFO_DLG_STATIC_MATE,	TEXT("statictext_NONAME2") );
//
//
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_ME,		this),	MYFAMILYINFO_DLG_BUTTON_ME,		TEXT("button_me") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_MATE,	this),	MYFAMILYINFO_DLG_BUTTON_MATE,	TEXT("button_mate") );
//
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st,	TEXT("button_me") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_2nd,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_2nd,	TEXT("button_mate") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_3rd,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_3rd,	TEXT("button_me") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_4th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_4th,	TEXT("button_mate") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_5th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_5th,	TEXT("button_me") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_6th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_6th,	TEXT("button_mate") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_7th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_7th,	TEXT("button_me") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_8th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_8th,	TEXT("button_mate") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_9th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_9th,	TEXT("button_me") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_10th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_10th,	TEXT("button_mate") );
//
//	DisplayFamilyInfo();
//
//	SetActionTimer( 1000 );
//	SetTimerOnOff( true );
//	
//}
//
//void CALLBACK CNMyFamilyInfoDlg::StaticCallBackDialogNMyFamilyInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
//{
//	CNMyFamilyInfoDlg* pThis = (CNMyFamilyInfoDlg*)pControllerMgr;
//	if ( pThis )
//	{
//		pThis->CallBackDialogNMyFamilyInfoDlg( nEvent, nControlID, pControl );
//	}
//}
//
//void CALLBACK CNMyFamilyInfoDlg::CallBackDialogNMyFamilyInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
//{
//	switch ( nControlID )
//	{
//
//	/*case FAMILYINFO_DLG_BUTTON_MATE:
//		{
//			OnButtonClickMate();
//		}
//		break;
//
//	case COMMON_DIALOG_CLOSEBUTTON:
//		{
//			DeleteDialog();
//		}
//		break;*/
//
//	}
//}
//
//void CNMyFamilyInfoDlg::Action()
//{
//	/*CheckMateConnect();*/
//}
//void CNMyFamilyInfoDlg::DisplayFamilyInfo( void )
//{
//	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
//	if ( NULL == pclChar )
//	{
//		return;
//	}
//
//	if ( FALSE == pclChar->IsMarried() )
//	{
//		return;
//	}
//
//	CButton* pclMyButton = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_ME );		if ( NULL == pclMyButton )		return;
//	CButton* pclMateButton = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_MATE );	if ( NULL == pclMateButton )	return;
//
//	CButton* pclButton_Children_1st = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st );	if ( NULL == pclButton_Children_1st )	return;
//	CButton* pclButton_Children_2nd = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_2nd );	if ( NULL == pclButton_Children_2nd )	return;
//	CButton* pclButton_Children_3rd = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_3rd );	if ( NULL == pclButton_Children_3rd )	return;
//	CButton* pclButton_Children_4th = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_4th );	if ( NULL == pclButton_Children_4th )	return;
//	CButton* pclButton_Children_5th = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_5th );	if ( NULL == pclButton_Children_5th )	return;
//	CButton* pclButton_Children_6th = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_6th );	if ( NULL == pclButton_Children_6th )	return;
//	CButton* pclButton_Children_7th = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_7th );	if ( NULL == pclButton_Children_7th )	return;
//	CButton* pclButton_Children_8th = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_8th );	if ( NULL == pclButton_Children_8th )	return;
//	CButton* pclButton_Children_9th = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_9th );	if ( NULL == pclButton_Children_9th )	return;
//	CButton* pclButton_Children_10th = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_10th );if ( NULL == pclButton_Children_10th )	return;
//	// 내이름 설정
//	NTCHARString32 strMyName = (TCHAR*)pclChar->pclCI->clBI.GetName();	pclMyButton->SetText( strMyName );
//	// 배우자 이름 설정
//	NTCHARString32 strMateName = pclChar->pclMarriage->GetMateName();	pclMateButton->SetText( strMateName );
//
//	// 자식들 이름 설정
//	// 자식등록을 만든다음에 기능 추가.
//}
//
//void CNMyFamilyInfoDlg::CheckMateConnect( void )
//{
//	/*cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
//	if ( NULL == pclChar )
//	{
//		return;
//	}
//
//	if ( FALSE == pclChar->IsMarried() )
//	{
//		return;
//	}
//
//	CButton* pclMateButton = m_InterfaceFactory.GetButton( FAMILYINFO_DLG_BUTTON_MATE );
//	if ( NULL == pclMateButton )
//	{
//		return;
//	}
//
//	if ( MARRIAGE_MATE_CONNECT_MODE_CONNECT <= pclChar->pclMarriage->GetConnectMode() )
//	{
//		pclMateButton->Enable( true );
//	}
//	else
//	{
//		pclMateButton->Enable( false );
//	}*/
//}
//
//void CNMyFamilyInfoDlg::OnButtonClickMate()
//{
//	/*cltClient* pclclient = (cltClient*)pclClient;
//	if ( NULL == pclclient )
//	{
//		return;
//	}
//
//	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
//	if ( NULL == pclChar )
//	{
//		return;
//	}
//
//	if ( FALSE == pclChar->IsMarried() )
//	{
//		return;
//	}
//
//	if ( MARRIAGE_MATE_CONNECT_MODE_NONE == pclChar->pclMarriage->GetConnectMode() )
//	{
//		return;
//	}
//
//	if ( false == pclChar->CanWarpFrom() )
//	{
//		return;
//	}
//
//	TCHAR*	pTitle		= GetTxtFromMgr( 3326 );
//
//	TCHAR	szText[256]	= TEXT("");
//	TCHAR*	pText		= GetTxtFromMgr(5185);
//
//	StringCchPrintf( szText, 256, pText, pclChar->pclMarriage->GetMateName() );
//
//	stMsgBox MsgBox;
//	MsgBox.Set( this, pTitle, szText, MSGBOX_TYPE_YESNO, 1 );
//	pclclient->SetMsgBox( &MsgBox, NULL, 0 );*/
//
//}
//
//void CNMyFamilyInfoDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
//{
//	/*switch( MsgIndex )
//	{
//	case 1:	
//		{
//			if ( true == RetValue )
//			{
//				cltMsg clMsg( GAMEMSG_REQUEST_MARRIAGEMATE_WARP, 0 );
//				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
//			}
//		}
//	}*/
//
//}