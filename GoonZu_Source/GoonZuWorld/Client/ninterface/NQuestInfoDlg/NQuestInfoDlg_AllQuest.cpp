#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client.h"
#include "Char/CharManager/CharManager.h"
#include "../common/SystemNPC/SystemNPC.h"

#include "NQuestInfoDlg_AllQuest.h"


CNQuestInfoDlg_AllQuest::CNQuestInfoDlg_AllQuest() : CDialogController()
{
	for ( SI32 siIndex=0; siIndex<Const_Max_Category_Button; ++siIndex )
	{
		m_pclButton[ siIndex ] = NULL;
	}

	m_pclList = NULL;

}

CNQuestInfoDlg_AllQuest::~CNQuestInfoDlg_AllQuest()
{
	for ( SI32 siIndex=0; siIndex<Const_Max_Category_Button; ++siIndex )
	{
		SAFE_DELETE( m_pclButton[siIndex] );
	}

	SAFE_DELETE( m_pclList );
}

void CNQuestInfoDlg_AllQuest::Create(CControllerMgr* pParent)
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	//-- 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NQuestInfoDlg/DLG_NQuestInfo_AllQuest.ddf" ) );
	file.CreateChildDialog( this, NQUESTINFODLG_ALLQUEST, TEXT( "dialog_allquest" ), StaticQuestInfoDlg_AllQuestProc, pParent );


	//-- 메세지멥 연결
	NEWCLIST( m_pclList );			file.CreateControl( m_pclList,		NQUESTINFO_ALLQUEST_LISTBOX_LIST,		TEXT("listbox_list") );
	NEWCBUTTON( m_pclButton[0] );	file.CreateControl( m_pclButton[0], NQUESTINFO_ALLQUEST_BUTTON_BEGINNER,	TEXT("button_beginner") );
	NEWCBUTTON( m_pclButton[1] );	file.CreateControl( m_pclButton[1], NQUESTINFO_ALLQUEST_BUTTON_MANUFACTURE,	TEXT("button_manufacture") );
	NEWCBUTTON( m_pclButton[2] );	file.CreateControl( m_pclButton[2], NQUESTINFO_ALLQUEST_BUTTON_WORLD,		TEXT("button_world") );


	//-- 컨트롤 설정
	SI16 siHeader0 = 80;
	SI16 siHeader1 = (m_pclList->GetWidth() - siHeader0) / 2;
	SI16 siHeader2 = siHeader1;
	

	m_pclList->SetColumn( 0, siHeader0, GetTxtFromMgr(6087) );
	m_pclList->SetColumn( 1, siHeader1, GetTxtFromMgr(647) );
	m_pclList->SetColumn( 2, siHeader2, GetTxtFromMgr(1611) );
	m_pclList->SetBorder( true );
	m_pclList->SetBKColorUse( true );
	m_pclList->SetTextPrintOption( DT_CENTER | DT_VCENTER );
	m_pclList->Refresh();

	//-- 기본 초기 설정
	// 버튼의 기본 위치를 기억한다
	for ( SI32 siIndex=0; siIndex<Const_Max_Category_Button; ++siIndex )
	{
		m_siButtonPosY[ siIndex ] = m_pclButton[ siIndex ]->GetY();
	}

	SelectCategory( NQUESTINFO_ALLQUEST_BUTTON_BEGINNER );

}

void CALLBACK CNQuestInfoDlg_AllQuest::StaticQuestInfoDlg_AllQuestProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNQuestInfoDlg_AllQuest *pThis = (CNQuestInfoDlg_AllQuest*) pControllerMgr;
	pThis->NQuestInfoDlg_AllQuestProc( nEvent, nControlID, pControl );
}

void CALLBACK CNQuestInfoDlg_AllQuest::NQuestInfoDlg_AllQuestProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch (nControlID)
	{
		case NQUESTINFO_ALLQUEST_BUTTON_BEGINNER:
		case NQUESTINFO_ALLQUEST_BUTTON_MANUFACTURE:
		case NQUESTINFO_ALLQUEST_BUTTON_WORLD:
			{
				SelectCategory( nControlID );
			}
			break;

	}
}

void CNQuestInfoDlg_AllQuest::Action()
{
	// Hide상태일때는 처리하지 않는다
	if ( false == IsShow() )
	{
		return;
	}

}

void CNQuestInfoDlg_AllQuest::Show()
{
	CControllerMgr::Show( true );

}

void CNQuestInfoDlg_AllQuest::Hide()
{
	CControllerMgr::Show( false );
}

void CNQuestInfoDlg_AllQuest::SelectCategory( SI32 siControlID )
{
	SI32 siNowIndex = siControlID - NQUESTINFO_ALLQUEST_BUTTON_BEGINNER;
	if ((0 > siNowIndex) || (Const_Max_Category_Button <= siNowIndex) )
	{
		return;
	}
	
	ChangeList( siNowIndex );
	
	SI16 siAddY = 0;

	for ( SI32 siIndex=0; siIndex<Const_Max_Category_Button; ++siIndex )
	{
		SI16 siNowPostionY	= m_siButtonPosY[ siIndex ] + siAddY;

		m_pclButton[ siIndex ]->SetY( siNowPostionY );
		m_pclButton[ siIndex ]->SetFontIndex( 0 );

		// 현재 현재 누른 버튼과 지금 처리하는 버튼이 같은 것이라면
		if ( siIndex == siNowIndex )
		{
			m_pclButton[ siIndex ]->SetFontIndex( 12 );
			// 리스트를 이동시키고
			m_pclList->SetY( siNowPostionY + m_pclButton[siIndex]->GetHeight() );
			// 리스트의 높이 만큼을 다음 처리하는 버튼의 Y pon로 설정(+4는 사이띄기 값)
			siAddY = m_pclList->GetHeight() + 4;
		}

	}

	// 리스트의 위치가 바뀌었으므로 다시한번 위치를 계산해 준다
	CalcListHeight();

}

void CNQuestInfoDlg_AllQuest::ChangeList( SI32 siIndex )
{
	if ( (0 > siIndex) || (Const_Max_Category_Button <= siIndex) )
	{
		return;
	}
	
	SI32 siCategory = CATEGORY_NONE;
	switch ( siIndex )
	{
		case 0: siCategory = CATEGORY_BEGINNER;		break;
		case 1: siCategory = CATEGORY_MANUFACTURE;	break;
		case 2: siCategory = CATEGORY_WORLD;		break;
		default: return;
	}

	cltSystemNPC clSystemNPCList[MAX_SYSTEM_NPC_NUMBER];
	if ( false == pclClient->pclSystemNPCManager->GetSystemNPCListFormCategory(siCategory, clSystemNPCList, MAX_SYSTEM_NPC_NUMBER) )
	{
		return ;
	}

	stListBoxItemData listBoxItemDate;

	m_pclList->Clear();

	for ( SI32 siCount=0; siCount<MAX_SYSTEM_NPC_NUMBER; ++siCount )
	{
		cltSystemNPC* pclSystemNPC( &clSystemNPCList[siCount] );
		if ( 0 >= pclSystemNPC->m_siKind )
		{
			break;
		}

		TCHAR* pszNPCName		= (TCHAR*)pclClient->pclKindInfoSet->GetName( pclSystemNPC->m_siKind );
		TCHAR* pszSimpleExplain	= GetTxtFromMgr( pclSystemNPC->m_siSimpleExplainNum );
		TCHAR* pszMapName		= GetTxtFromMgr( pclSystemNPC->m_siLocationNum );

		if ( (NULL == pszNPCName) || (NULL == pszSimpleExplain) || (NULL == pszMapName) )
		{
			continue;
		}

		listBoxItemDate.Init();

		listBoxItemDate.Set(0, pszNPCName);
		listBoxItemDate.Set(1, pszSimpleExplain);
		listBoxItemDate.Set(2, pszMapName);

		m_pclList->AddItem( &listBoxItemDate );
	}
	
	CalcListHeight();
}

void CNQuestInfoDlg_AllQuest::CalcListHeight( void )
{
	SI16 siHeaderHeight	= 15;
	SI16 siRowHeight	= m_pclList->GetLineHeight();

	SI32 siNowHeight	= (m_pclList->GetListNum() * siRowHeight) + siHeaderHeight;
	SI32 siMaxHeight	= (siRowHeight * Const_Max_ListBox_Item) + siHeaderHeight;

	SI16 siListX		= m_pclList->GetX();
	SI16 siListY		= m_pclList->GetY();
	SI16 siListWidth	= m_pclList->GetWidth();

	// Max 높이보다 높다면 Max높이로 설정
	if ( siNowHeight > siMaxHeight )
	{
		m_pclList->SetControllerPos( siListX, siListY, siListWidth, siMaxHeight, siListWidth, siMaxHeight );
	}
	else
	{
		m_pclList->SetControllerPos( siListX, siListY, siListWidth, siNowHeight, siListWidth, siNowHeight );
	}

	// 높이가 변함에 따라 스크롤도 따로 변경시켜줘야 한다
	CScroll* pclScroll = m_pclList->GetScroll();
	if ( pclScroll )
	{
		pclScroll->SetScrollPos( pclScroll->GetX(), siListY, pclScroll->GetWidth(), m_pclList->GetHeight() );
	}

	m_pclList->Refresh();

}
