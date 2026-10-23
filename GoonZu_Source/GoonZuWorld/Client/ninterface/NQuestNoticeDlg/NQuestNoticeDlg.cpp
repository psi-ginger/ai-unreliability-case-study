#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client.h"
#include "Char/CharManager/CharManager.h"
#include "../common/SystemNPC/SystemNPC.h"
#include "Msg/MsgType-ItemMall.h"

#include "NQuestNoticeDlg.h"

CNQuestNoticeDlg::CNQuestNoticeDlg() : CDialogController()
{
	m_siMapIndex	= 0;
	m_siMapNameNum	= 0;
}

CNQuestNoticeDlg::~CNQuestNoticeDlg()
{

}

void CNQuestNoticeDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	//-- 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NQuestNoticeDlg/DLG_NQuestNotice.ddf" ) );
	file.CreatePopupDialog( this, NQUESTNOTICE_DLG, TEXT( "dialog_questnotice" ), StaticQuestNoticeDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,	NQUESTNOTICE_IMAGESTATIC_FACE,	this ), NQUESTNOTICE_IMAGESTATIC_FACE,	TEXT( "imagestatic_face" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,		NQUESTNOTICE_OUTLINE_FACE,		this ), NQUESTNOTICE_OUTLINE_FACE,		TEXT( "outline_face" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,			NQUESTNOTICE_BUTTON_GOTO,		this ), NQUESTNOTICE_BUTTON_GOTO,		TEXT( "button_goto" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,			NQUESTNOTICE_EDITBOX_TITLE,		this ), NQUESTNOTICE_EDITBOX_TITLE,		TEXT( "editbox_title" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,			NQUESTNOTICE_EDITBOX_EXPLAIN,	this ), NQUESTNOTICE_EDITBOX_EXPLAIN,	TEXT( "editbox_explain" ) );


	
	Hide();
}

void CALLBACK CNQuestNoticeDlg::StaticQuestNoticeDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNQuestNoticeDlg *pThis = (CNQuestNoticeDlg*) pControllerMgr;
	pThis->NQuestNoticeDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNQuestNoticeDlg::NQuestNoticeDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch( nControlID )
	{
		case NQUESTNOTICE_BUTTON_GOTO:
			{
				CheckWarp();
			}
			break;

		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;

	}

}

void CNQuestNoticeDlg::Show()
{
	CControllerMgr::Show( true );
}

void CNQuestNoticeDlg::Hide()
{
	CControllerMgr::Show( false );
}

void CNQuestNoticeDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	if ( pData == NULL || DataLen < 1 )
	{
		return;
	}

	if ( false == RetValue )
	{
		return;
	}


	switch ( MsgIndex )
	{
		case 1:
			{
				SI32 siMapIndex = *((SI32*)pData);

				cltGameMsgRequest_HuntMapWarp pclInfo( siMapIndex, 0, false );
				cltMsg clMsg( GAMEMSG_REQUEST_HUNTMAPWARP, sizeof(pclInfo), (BYTE*)&pclInfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			break;
	}


}

void CNQuestNoticeDlg::CheckNotice()
{
	// 기본적으로 초기화
	m_siMapIndex = 0;

	cltSystemNPC clSystemNPCList[ MAX_SYSTEM_NPC_NUMBER ];
	if ( false == pclClient->pclSystemNPCManager->GetSystemNPCListForShowNotice(clSystemNPCList, MAX_SYSTEM_NPC_NUMBER) )
	{
		return;
	}

	cltSystemNPC	clNoticeSystemNPC;


	for ( SI32 siIndex=0; siIndex<MAX_SYSTEM_NPC_NUMBER; ++siIndex )
	{
		cltSystemNPC* pclSystemNPC = &clSystemNPCList[siIndex];
		if ( (NULL == pclSystemNPC) || (0 >= pclSystemNPC->m_siShowNotice) )
		{
			break;
		}
		
		CheckCondition( pclSystemNPC, &clNoticeSystemNPC );
	}

	if ( 0 < clNoticeSystemNPC.m_siShowNotice )
	{
		DisplayNoticeDlg( &clNoticeSystemNPC );
	}
	else
	{
		DeleteDialog();
	}
}

void CNQuestNoticeDlg::CheckCondition( IN cltSystemNPC* pclConditionNPC, OUT cltSystemNPC* pclResultNPC )
{
	if ( (NULL == pclConditionNPC) || (NULL == pclResultNPC) )
	{
		return;
	}

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( NULL == pclchar )
	{
		return;
	}

	// 해당퀘스트가 이미 진행중이거나 클리어 했다면 바로 리턴
	SI32 siQuestType = pclConditionNPC->m_siQuestType;
	if ( true == pclchar->pclCI->clQuestInfo.IsQuestClearOrGoing(siQuestType) )
	{
		return;
	}
	
	cltQuestInfo* pclQuestInfo = &pclClient->pclQuestManager->clQuestInfo[siQuestType][0];
	if ( NULL == pclQuestInfo )
	{
		return;
	}

	SI32 siNowLevel		= pclchar->pclCI->clIP.GetLevel();
	SI32 siNeedLevel	= pclQuestInfo->clInitConditionInfo.siLevel;
	SI16 siClearCond	= pclQuestInfo->clQuestClearConditionInfo.siFlag;
	SI32 siQuestUinque	= pclQuestInfo->clQuestClearConditionInfo.siPara1;
	bool bLevelCheck	= false;
	bool bCondCheck		= true;	// 초기값을 true로 한 이유는 코딩양을 줄이기 위함

	// 레벨과 선행퀘스트 조건이 각가 다르게 적용될수 있으므로 분리해서 검사
	if ( siNeedLevel <= siNowLevel )
	{
		bLevelCheck = true;
	}

	if ( QUEST_CLEAR_COND_CLEARQUEST == siClearCond )
	{
		if ( true == pclchar->pclCI->clQuestInfo.IsClear(pclClient->pclQuestManager, siQuestUinque) )
		{
			bCondCheck = true;
		}
		else
		{	
			bCondCheck = false;
		}
	}

	// 두조건을 만족시킬때
	if ( (true == bLevelCheck) && (true == bCondCheck) )
	{
		if ( 0 >= pclResultNPC->m_siShowNotice )
		{
			pclResultNPC->Set( pclConditionNPC );
		}
		else if( pclConditionNPC->m_siShowNotice < pclResultNPC->m_siShowNotice )
		{
			pclResultNPC->Set( pclConditionNPC );
		}
	}

}

void CNQuestNoticeDlg::DisplayNoticeDlg( cltSystemNPC* pclNoticeNPCInfo )
{
	if ( NULL == pclNoticeNPCInfo )
	{
		return;
	}

	CImageStatic*	pclFaceImage	= m_InterfaceFactory.GetImageStatic( NQUESTNOTICE_IMAGESTATIC_FACE );
	CEdit*			pclTitleEdit	= m_InterfaceFactory.GetEdit( NQUESTNOTICE_EDITBOX_TITLE );
	CEdit*			pclExplainEdit	= m_InterfaceFactory.GetEdit( NQUESTNOTICE_EDITBOX_EXPLAIN );

	if ( (NULL == pclFaceImage) || (NULL == pclTitleEdit) || (NULL == pclExplainEdit) )
	{
		return;
	}

	TCHAR	szFileName[512] = { '\0', };
	SI32	siFont = 0;

	if ( false == pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclNoticeNPCInfo->m_siKind, szFileName, 512, &siFont) )
	{
		return;
	}
	
	pclFaceImage->SetFileName( szFileName );
	pclFaceImage->SetFontIndex( siFont );

	pclTitleEdit->SetText( GetTxtFromMgr(pclNoticeNPCInfo->m_siNoticeExplainNum) );
	pclExplainEdit->SetText( GetTxtFromMgr(pclNoticeNPCInfo->m_siNoticeLocationNum) );

	Show();

	m_siMapIndex	= pclNoticeNPCInfo->m_siMapIndex;
	m_siMapNameNum	= pclNoticeNPCInfo->m_siLocationNum;

}

void CNQuestNoticeDlg::CheckWarp( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharCommon* pclchar = (cltCharCommon*)pclClient->pclCM->CR[1];
	if ( NULL == pclchar )
	{
		return;
	}

	if ( false == pclchar->CanWarpFrom() )
	{
		return;
	}

	// 혹시 모르니 멥 인덱스 검사
	if ( (0 >= m_siMapIndex) || (MAX_MAP_NUMBER <= m_siMapIndex) )
	{
		return;
	}

	SI32 siTitleNum	= 0;
	SI32 siTextNum	= 0;
	
	// 공짜 워프 검사
	if ( pclchar->CanFreeWarp(m_siMapIndex) )
	{
		siTitleNum	= (3189);
		siTextNum	= (3760);
	}
	// 워프티켓검사
	else if ( 0 < pclchar->pclCI->clCharItem.GetItemNum(ITEMUNIQUE( 24000 )) )
	{
		siTitleNum	= (3326);
		siTextNum	= (3332);
	}
	// 안되면 리턴
	else
	{
		TCHAR* pszTitle	= GetTxtFromMgr(3326);
		TCHAR* pszText	= GetTxtFromMgr(3756);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pszTitle, pszText );
		return;
	}

	SetRetMsgBox( siTitleNum, siTextNum );
}

void CNQuestNoticeDlg::SetRetMsgBox( SI32 siTitleNum, SI32 siTextNum )
{
	if ( (0 >= siTitleNum) || (0 >= siTextNum) )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	// 텍스트 메니져에서 지역 이름을 뽑아오고
	TCHAR* pszMapName = GetTxtFromMgr( m_siMapNameNum );
	if ( NULL == pszMapName )
	{
		// 만약 안되면 MapIndex로 지역이름을 뽑아온다
		pszMapName = pclClient->pclMapManager->GetMapName( m_siMapIndex );
		if ( NULL == pszMapName )
		{
			return;
		}
	}

	TCHAR*	pTitle			= GetTxtFromMgr(siTitleNum);
	TCHAR*	pText			= GetTxtFromMgr(siTextNum);
	TCHAR	szBuffer[256]	= { '\0', };

	StringCchPrintf( szBuffer, 256, pText, pszMapName);

	stMsgBox MsgBox;
	MsgBox.Set( this, pTitle, szBuffer, MSGBOX_TYPE_YESNO, 1 );

	// 혹시 보내놓고 맵 인덱스가 바뀔수있으니 멥 인덱스를 여기에서 인자로 보내준다
	BYTE TempBuffer[ 128 ];
	SI16 Size = 0;
	memcpy( &TempBuffer[Size], &m_siMapIndex, sizeof(m_siMapIndex) );	Size += sizeof(m_siMapIndex);

	pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

}

void CNQuestNoticeDlg::NoticeEvent( SI32 siNpcKind, SI32 siMapIndex )// 이벤트시 해당 위치와 엔피시 얼굴을 나타내기 위해서
{
	// 먼저 퀘스트 이동 버튼을 보이지 않도록 한다
	m_InterfaceFactory.GetButton( NQUESTNOTICE_BUTTON_GOTO )->Show( false );
	CImageStatic*	pclFaceImage	= m_InterfaceFactory.GetImageStatic( NQUESTNOTICE_IMAGESTATIC_FACE );
	CEdit*	pcNPCEditTitle		= m_InterfaceFactory.GetEdit( NQUESTNOTICE_EDITBOX_TITLE );
	CEdit*	pcNPCEditExplain	= m_InterfaceFactory.GetEdit( NQUESTNOTICE_EDITBOX_EXPLAIN );
	
	// 얼굴을 보여주고
	TCHAR	szFileName[512] = { '\0', };
	SI32	siFont = 0;

	if ( false == pclClient->pclKindInfoSet->GetNPCImageFileAndFont(siNpcKind, szFileName, 512, &siFont) )
	{
		return;
	}
	pclFaceImage->SetFileName( szFileName );
	pclFaceImage->SetFontIndex( siFont );

	// 텍스트 내용만 세팅해주면 끝
	TCHAR* NpcEdit = GetTxtFromMgr( 8867 );

	NTCHARString256	strText( GetTxtFromMgr(8868) );
	strText.Replace( TEXT("#map#"),		pclClient->pclMapManager->GetMapName( siMapIndex ) );

	pcNPCEditTitle->SetText( NpcEdit );
	pcNPCEditExplain->SetText( strText );
	Show();

}

void CNQuestNoticeDlg::NoticeEvent( SI32 siNpcKind, const TCHAR* pszTitle, const TCHAR* pszExplain ) // 텍스트 내용을 강제로 채워주는 함수
{
	// 먼저 퀘스트 이동 버튼을 보이지 않도록 한다
	m_InterfaceFactory.GetButton( NQUESTNOTICE_BUTTON_GOTO )->Show( false );
	CImageStatic*	pclFaceImage	= m_InterfaceFactory.GetImageStatic( NQUESTNOTICE_IMAGESTATIC_FACE );
	CEdit*	pcNPCEditTitle		= m_InterfaceFactory.GetEdit( NQUESTNOTICE_EDITBOX_TITLE );
	CEdit*	pcNPCEditExplain	= m_InterfaceFactory.GetEdit( NQUESTNOTICE_EDITBOX_EXPLAIN );

	// 얼굴을 보여주고
//	TCHAR	szFileName[512] = { '\0', };
//	SI32	siFont = 0;

//	if ( false == pclClient->pclKindInfoSet->GetNPCImageFileAndFont(siNpcKind, szFileName, 512, &siFont) )
//	{
//		return;
//	}
//	pclFaceImage->SetFileName( szFileName );
//	pclFaceImage->SetFontIndex( siFont );
	
	pclFaceImage->SetFileName("Gimg/hcandy.SPR");
	pclFaceImage->SetFontIndex( 0 );

	// 텍스트 내용만 세팅해주면 끝
	pcNPCEditTitle->SetText( (TCHAR*)pszTitle );
	pcNPCEditExplain->SetText( (TCHAR*)pszExplain );
	Show();

}
void CNQuestNoticeDlg::NoticeEvent2( SI32 siNpcKind, const TCHAR* pszTitle, const TCHAR* pszExplain )// 이벤트시 해당 위치와 엔피시 얼굴을 나타내기 위해서
{
	// 먼저 퀘스트 이동 버튼을 보이지 않도록 한다
	m_InterfaceFactory.GetButton( NQUESTNOTICE_BUTTON_GOTO )->Show( false );
	CImageStatic*	pclFaceImage	= m_InterfaceFactory.GetImageStatic( NQUESTNOTICE_IMAGESTATIC_FACE );
	CEdit*	pcNPCEditTitle		= m_InterfaceFactory.GetEdit( NQUESTNOTICE_EDITBOX_TITLE );
	CEdit*	pcNPCEditExplain	= m_InterfaceFactory.GetEdit( NQUESTNOTICE_EDITBOX_EXPLAIN );

	// 얼굴을 보여주고
	TCHAR	szFileName[512] = { '\0', };
	SI32	siFont = 0;

	if ( false == pclClient->pclKindInfoSet->GetNPCImageFileAndFont(siNpcKind, szFileName, 512, &siFont) )
	{
		return;
	}
	pclFaceImage->SetFileName( szFileName );
	pclFaceImage->SetFontIndex( siFont );

	pcNPCEditTitle->SetText( (TCHAR*)pszTitle );
	pcNPCEditExplain->SetText( (TCHAR*)pszExplain );
	Show();

}