#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client.h"
#include "Char/CharManager/CharManager.h"

#include "Msg/MsgType-Quest.h"
#include "Msg/MsgType-DailyQuest.h"
#include "Msg/MsgType-DailyQuest2.h"
#include "Msg/MsgType-DailyQuest3.h"

#include "NQuestInfoDlg_Parent.h"
#include "NQuestInfoDlg_GoingQuest.h"

#include "../../ninterface/nnewquestdlg/NNewQuestDlg.h"

#include "../common/Event/event.h"
#include "../client/PartyQuestMgr/PartyQuestMgr_Client.h"

cltDailyQuestInfo::cltDailyQuestInfo( cltPDailyQuestInfo* pclDailyQuestInfo1 )
{
	Init();

	if ( NULL == pclDailyQuestInfo1 )
	{
		return;
	}

	m_siDailyQuestType		= DAILY_QUESTTYPE_1;
	m_siCondition			= pclDailyQuestInfo1->siMonsterKind;
	m_siNowAmount			= pclDailyQuestInfo1->siKillMonsterNum;
	m_siNeedAmount			= pclDailyQuestInfo1->siMonsterNum;
	m_siLeftTime			= pclDailyQuestInfo1->siLeftTime;
	m_bHidden				= false;

	m_siRewardExp			= pclDailyQuestInfo1->siRewardExp;
	m_gmRewardMoney			= pclDailyQuestInfo1->siRewardMoney;
	m_siRewardItemUnique	= pclDailyQuestInfo1->siRewardItemUnique;
	m_siRewardItemAmount	= pclDailyQuestInfo1->siRewardItemNum;
	
}

cltDailyQuestInfo::cltDailyQuestInfo( cltPDailyQuest2Info* pclDailyQuestInfo2 )
{
	Init();

	if ( NULL == pclDailyQuestInfo2 )
	{
		return;
	}

	m_siDailyQuestType		= DAILY_QUESTTYPE_2;
	m_siCondition			= pclDailyQuestInfo2->siMonsterKind;
	m_siNowAmount			= pclDailyQuestInfo2->siKillMonsterNum;
	m_siNeedAmount			= pclDailyQuestInfo2->siMonsterNum;
	m_siLeftTime			= pclDailyQuestInfo2->siLeftTime;
	m_bHidden				= pclDailyQuestInfo2->bHiddenQuest;

	m_siRewardExp			= pclDailyQuestInfo2->siRewardExp;
	m_gmRewardMoney			= pclDailyQuestInfo2->siRewardMoney;
	m_siRewardItemUnique	= pclDailyQuestInfo2->siRewardItemUnique;
	m_siRewardItemAmount	= pclDailyQuestInfo2->siRewardItemNum;

}

cltDailyQuestInfo::cltDailyQuestInfo( cltPDailyQuest3Info* pclDailyQuestInfo3 )
{
	Init();

	if ( NULL == pclDailyQuestInfo3 )
	{
		return;
	}

	m_siDailyQuestType		= DAILY_QUESTTYPE_3;
	m_siCondition			= pclDailyQuestInfo3->siItemUnique;
	m_siNowAmount			= pclDailyQuestInfo3->siMadeItemNum;
	m_siNeedAmount			= pclDailyQuestInfo3->siItemNum;
	m_siLeftTime			= pclDailyQuestInfo3->siLeftTime;
	m_bHidden				= pclDailyQuestInfo3->bHiddenQuest;

	m_siRewardExp			= pclDailyQuestInfo3->siRewardExp;
	m_gmRewardMoney			= 0;
	m_siRewardItemUnique	= pclDailyQuestInfo3->siRewardItemUnique;
	m_siRewardItemAmount	= pclDailyQuestInfo3->siRewardItemNum;

}

bool cltDailyQuestInfo::GetDailyQuestInfo( OUT TCHAR* pszBuffer, IN SI32 siBufferSize, OUT SI32* psiGimgIndex, OUT SI32* psiFont )
{
	if ( (NULL == pszBuffer) || (0 >= siBufferSize) || (NULL == psiGimgIndex) || (NULL == psiFont) )
	{
		return false;
	}

	TCHAR*	pszInfoText		= NULL;
	TCHAR*	pszCondition	= NULL;
	SI32	siNpcKind		= 0;

	// 데일리 퀘스트 타입에 따른 텍스트
	switch ( m_siDailyQuestType )
	{
		case DAILY_QUESTTYPE_1:
			{
				pszInfoText		= GetTxtFromMgr(3085);
				pszCondition	= (TCHAR*)pclClient->pclKindInfoSet->GetName( m_siCondition );

				siNpcKind		= pclClient->GetUniqueFromHash( TEXT("KIND_DAILYQUEST"));
			}
			break;

		case DAILY_QUESTTYPE_2:
			{
				pszInfoText		= GetTxtFromMgr(8096);
				pszCondition	= (TCHAR*)pclClient->pclKindInfoSet->GetName( m_siCondition );

				siNpcKind		= pclClient->GetUniqueFromHash( TEXT("KIND_DAILYQUEST2"));
			}
			break;

		case DAILY_QUESTTYPE_3:
			{
				if ( true == m_bHidden )
				{
					pszInfoText	= GetTxtFromMgr(8453);
				}
				else
				{
					pszInfoText = GetTxtFromMgr(8217);
				}
				pszCondition	= (TCHAR*)pclClient->pclItemManager->GetName( m_siCondition );

				siNpcKind		= pclClient->GetUniqueFromHash( TEXT("KIND_DAILYQUEST3"));
			}
			break;

	}

	if ( (NULL == pszInfoText) || (NULL == pszCondition) || (0 >= siNpcKind) )
	{
		return false;
	}

	// 이미지 폰트 가져오기
	pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siNpcKind, psiGimgIndex, psiFont );

	// 실질적인 텍스트 조합
	
	if ( DAILY_QUESTTYPE_3 == m_siDailyQuestType &&  true == m_bHidden )
	{
		StringCchPrintf( pszBuffer, siBufferSize, pszInfoText, pszCondition, m_siNowAmount, m_siNeedAmount);
	}
	else
	{
		StringCchPrintf( pszBuffer, siBufferSize, pszInfoText, pszCondition, m_siNowAmount, m_siNeedAmount, m_siLeftTime );
	}
	return true;

}

bool cltDailyQuestInfo::GetDailyQuestRewardText( OUT TCHAR* pszBuffer, IN SI32 siBufferSize )
{
	if ( (NULL == pszBuffer) || (0 >= siBufferSize) )
	{
		return false;
	}


	TCHAR szBuf[256] = { '\0', };

	// 경험치
	if ( m_siRewardExp )
	{
		TCHAR* pText= GetTxtFromMgr(3090);
		StringCchPrintf(szBuf, 256, pText, m_siRewardExp);
		StringCchCat(pszBuffer, siBufferSize, szBuf);
	}

	/* 돈을 보상으로 주는것은 없음
	// 돈
	if ( m_gmRewardMoney )
	{
		TCHAR* pText= GetTxtFromMgr(3091);
		StringCchPrintf(szBuf, 256, pText, m_gmRewardMoney);
		StringCchCat(pszBuffer, siBufferSize, szBuf);
	}
	*/

	// 아이템
	if ( m_siRewardItemUnique )
	{
		TCHAR* pitemname	= (TCHAR*)pclClient->pclItemManager->GetName( m_siRewardItemUnique );
		TCHAR* pText		= GetTxtFromMgr(3092);

		StringCchPrintf( szBuf, 256, pText, pitemname, m_siRewardItemAmount );
		StringCchCat( pszBuffer, siBufferSize, szBuf );
	}

	return true;

}

void cltDailyQuestInfo::DailyQuestCancel()
{
	if ( 0 >= m_siCondition )
	{
		return;
	}

	switch ( m_siDailyQuestType )
	{
		case DAILY_QUESTTYPE_1:
			{
				cltGameMsgRequest_DailyQuestEnd clDailyQuestEnd;
				cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUESTEND, sizeof( clDailyQuestEnd ), (BYTE*)&clDailyQuestEnd );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			break;

		case DAILY_QUESTTYPE_2:
			{
				cltGameMsgRequest_DailyQuest2End clDailyQuest2End;
				cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST2END, sizeof( clDailyQuest2End ), (BYTE*)&clDailyQuest2End );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			break;

		case DAILY_QUESTTYPE_3:
			{
				cltGameMsgRequest_DailyQuest3End clDailyQuest3End;
				cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST3END, sizeof( clDailyQuest3End ), (BYTE*)&clDailyQuest3End );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			break;
	}

	pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_GIVEUP")), 0, 0 );

}

//======================================================================================
// 경계선
//======================================================================================

CNQuestInfoDlg_GoingQuest::CNQuestInfoDlg_GoingQuest() : CDialogController()
{
	// 퀘스트 타입은 0부터 시작이라 0으로 초기화 하면 안됨
	memset( m_siGoingQuestList, -1, sizeof(m_siGoingQuestList) );
}

CNQuestInfoDlg_GoingQuest::~CNQuestInfoDlg_GoingQuest()
{
	SetTimerOnOff( false );
}

void CNQuestInfoDlg_GoingQuest::Create(CControllerMgr* pParent)
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	//-- 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NQuestInfoDlg/DLG_NQuestInfo_GoingQuest.ddf" ) );
	file.CreateChildDialog( this, NQUESTINFODLG_GOINGQUEST, TEXT( "dialog_goingquest" ), StaticQuestInfoDlg_GoingQuestProc, pParent );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,		NQUESTINFO_GOINGQUEST_LISTBOX_QUESTLIST,	this ), NQUESTINFO_GOINGQUEST_LISTBOX_QUESTLIST,	TEXT( "listbox_questlist" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NQUESTINFO_GOINGQUEST_BUTTON_CONTINUE,		this ), NQUESTINFO_GOINGQUEST_BUTTON_CONTINUE,		TEXT( "button_continue" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NQUESTINFO_GOINGQUEST_BUTTON_CANCEL,		this ), NQUESTINFO_GOINGQUEST_BUTTON_CANCEL,		TEXT( "button_cancel" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	NQUESTINFO_GOINGQUEST_OUTLINE_QUESTLIST,	this ), NQUESTINFO_GOINGQUEST_OUTLINE_QUESTLIST,	TEXT( "outline_questlist" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NQUESTINFO_GOINGQUEST_STATIC_EXPLAIN,		this ), NQUESTINFO_GOINGQUEST_STATIC_EXPLAIN,		TEXT( "statictext_explain" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NQUESTINFO_GOINGQUEST_EDIT_EXPLAIN,			this ), NQUESTINFO_GOINGQUEST_EDIT_EXPLAIN,			TEXT( "editbox_explain" ) );

	//-- 컨트롤 설정

	// 리스트 설정
	CList* pclList = m_InterfaceFactory.GetList(NQUESTINFO_GOINGQUEST_LISTBOX_QUESTLIST);
	if ( NULL == pclList )
	{
		DeleteDialog();
		return;
	}

	pclList->SetColumn( 0, 40, NULL );
	pclList->SetColumn( 1, pclList->GetWidth()-40 , NULL );
	//pclList->SetLineHeight( 50 );
	pclList->SetBorder( true );
	pclList->SetBKColorUse( true );
	pclList->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_WORDBREAK ); // 자동 줄바꿈 추가. (텍스트 \r 적용안되서..)
	pclList->GetScroll()->Enable( true );
	pclList->Refresh();

	// 에디트 설정
	CEdit* pclEdit = m_InterfaceFactory.GetEdit( NQUESTINFO_GOINGQUEST_EDIT_EXPLAIN );
	if ( NULL == pclEdit )
	{
		DeleteDialog();
		return;
	}

	pclEdit->SetChangeTextDrawMode( true );
		

	//-- 기본 초기 설정
	SetTimerOnOff( true );
	SetActionTimer( 1000 );
}

void CALLBACK CNQuestInfoDlg_GoingQuest::StaticQuestInfoDlg_GoingQuestProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNQuestInfoDlg_GoingQuest *pThis = (CNQuestInfoDlg_GoingQuest*) pControllerMgr;
	pThis->NQuestInfoDlg_GoingQuestProc( nEvent, nControlID, pControl );
}

void CALLBACK CNQuestInfoDlg_GoingQuest::NQuestInfoDlg_GoingQuestProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{ 
	switch ( nControlID )
	{
		case NQUESTINFO_GOINGQUEST_LISTBOX_QUESTLIST:
			{
				if ( EVENT_LISTBOX_SELECTION == nEvent )
				{
					SI32 siSelectedIndex = ((CList*)m_InterfaceFactory.GetList(NQUESTINFO_GOINGQUEST_LISTBOX_QUESTLIST))->GetSelectedIndex();

					SelectQuestInfo( siSelectedIndex );
				}
			}
			break;

		case NQUESTINFO_GOINGQUEST_BUTTON_CONTINUE:
			{
				SI32 siSelectedIndex = ((CList*)m_InterfaceFactory.GetList(NQUESTINFO_GOINGQUEST_LISTBOX_QUESTLIST))->GetSelectedIndex();

				QuestContinue( siSelectedIndex );
			}
			break;

		case NQUESTINFO_GOINGQUEST_BUTTON_CANCEL:
			{
				SI32 siSelectedIndex = ((CList*)m_InterfaceFactory.GetList(NQUESTINFO_GOINGQUEST_LISTBOX_QUESTLIST))->GetSelectedIndex();

				ShowQuestCancelMsgBox( siSelectedIndex );
			}
			break;
	}

}

void CNQuestInfoDlg_GoingQuest::Action()
{
	static clock_t PreClock = 0;

	// Hide상태일때는 처리하지 않는다
	if ( false == IsShow() )
	{
		return;
	}

	// 시간제한이 있는 퀘스트(현재는 군주데이 미션만 해당)의 경우 시간을 갱신시킨다 (서버와 싱크가 맞진 않지만 분단위이기 때문에 몇초정도의 오차는 무시)
	{
		cltClient* pClient = (cltClient*)pclClient;
		SI32 siTimeReduce = 0;
		siTimeReduce = (pClient->CurrentClock - PreClock) / 1000;		// pClient->CurrentClock의 값은 밀리초 (초로 계산하기 위해선 1000을 나누고 분으로 하기 위해선 나눈 값을 60으로 다시 나눠야 한다)
		
		if( 0 != PreClock )
		{
			pClient->pclQuestManager->UpdateRemainTime( siTimeReduce );
		}

		PreClock = pClient->CurrentClock;
	}

	LoadGoingQuestList();
}

void CNQuestInfoDlg_GoingQuest::Show()
{
	CControllerMgr::Show( true );

}

void CNQuestInfoDlg_GoingQuest::Hide()
{
	CControllerMgr::Show( false );
}

void CNQuestInfoDlg_GoingQuest::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
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
		case QUEST_INFO_REGMSG_GOING_QUESTCANCEL:
			{

				SI32 siQuestType = *((SI32*)pData);

				if ( DAILY_QUESTTYPE_1 == siQuestType )
				{
					cltDailyQuestInfo clDailyQuestInfo( &pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo );
					clDailyQuestInfo.DailyQuestCancel();
				}
				else if ( DAILY_QUESTTYPE_2 == siQuestType )
				{
					cltDailyQuestInfo clDailyQuestInfo( &pclClient->pclCM->CR[1]->pclCI->clDailyQuest2Info );
					clDailyQuestInfo.DailyQuestCancel();
				}
				else if ( DAILY_QUESTTYPE_3 == siQuestType )
				{
					cltDailyQuestInfo clDailyQuestInfo( &pclClient->pclCM->CR[1]->pclCI->clDailyQuest3Info );
					clDailyQuestInfo.DailyQuestCancel();
				}
				else if ( (0 <= siQuestType) && (MAX_QUEST_TYPE > siQuestType) )
				{
					QuestCancel( siQuestType );
				}

			}
			break;

	}
}

void CNQuestInfoDlg_GoingQuest::LoadGoingQuestList()
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( NULL == pclchar )
	{
		return;
	}

	CList* pclList = m_InterfaceFactory.GetList( NQUESTINFO_GOINGQUEST_LISTBOX_QUESTLIST );
	if ( NULL == pclList )
	{
		return;
	}

	CEdit* pclEdit = m_InterfaceFactory.GetEdit( NQUESTINFO_GOINGQUEST_EDIT_EXPLAIN );
	if ( NULL == pclEdit )
	{
		return;
	}

	// 리스트의 줄수가 변하지 않았다면 리플레쉬 돼기 전의 선택상태로 되돌림.
	SI32 siOldListNum		= pclList->GetListNum();
	SI32 siOldSelectedItem	= pclList->GetSelectedIndex();
	SI32 siOldScrollPos		= pclList->GetScrollTrackPos();

	SI32 siQuestCount		= 0;

	// 퀘스트 타입은 0부터 시작이라 0으로 초기화 하면 안됨
	memset( m_siGoingQuestList, -1, sizeof(m_siGoingQuestList) );

	pclList->Clear();

	
	// 요술램프 퀘스트는 내용이 없어도 항상 리스트박스에 존재하도록 한다.
	cltPersonQuestUnitInfo* pclunit = &pclchar->pclCI->clQuestInfo.clUnit[ QUEST_TYPE_SPECIAL ];
	AddQuestInfo(QUEST_TYPE_SPECIAL, pclunit);
	m_siGoingQuestList[ siQuestCount++ ] = QUEST_TYPE_SPECIAL;

	// 해피빈 퀘스트는 내용이 없어도 항상 리스트 박스에 존재하도록 한다	
	if( pclClient->IsCountrySwitch( Switch_HappyBeanQuest ) )
	{
		cltPersonQuestUnitInfo* pclunit = &pclchar->pclCI->clQuestInfo.clUnit[ QUEST_TYPE_HAPPYBEAN ];
		AddQuestInfo(QUEST_TYPE_HAPPYBEAN, pclunit);
		m_siGoingQuestList[ siQuestCount++ ] = QUEST_TYPE_HAPPYBEAN;
	}

	// 데일리 퀘스트 1,2,3
	cltDailyQuestInfo clDailyQuestInfo[ 3 ] = {&pclchar->pclCI->clDailyQuestInfo, &pclchar->pclCI->clDailyQuest2Info, &pclchar->pclCI->clDailyQuest3Info};
	for ( SI32 siDailyIndex=0; siDailyIndex<3; ++siDailyIndex )
	{
		if ( true == AddDailyQuestInfo(&clDailyQuestInfo[siDailyIndex]) )
		{
			m_siGoingQuestList[ siQuestCount++ ] = clDailyQuestInfo[ siDailyIndex ].m_siDailyQuestType;
		}
	}

	if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true)
	{
		TCHAR* szInfo = pclclient->m_pPartyQuestMgr->m_szQuestInfoForDlg;

		if(szInfo != NULL)
		{
			if(AddPartyQuestInfo(pclclient->m_pPartyQuestMgr->m_szQuestInfoForDlg) == true)
				m_siGoingQuestList[ siQuestCount++ ] = PARTY_QUESTTYPE;
		}
	}

	for ( SI32 siQuestType=0; siQuestType<MAX_QUEST_TYPE; ++siQuestType )
	{
		// 요술램프, 해피빈 퀘스트는 위에서 처리했으므로 여기에서는 하지 않는다
		if ( (QUEST_TYPE_SPECIAL == siQuestType) || (QUEST_TYPE_HAPPYBEAN == siQuestType) )
		{
			continue;
		}
		
		// 이 타입의 걸린 퀘스트가 없다면
		cltPersonQuestUnitInfo* pclunit = &pclchar->pclCI->clQuestInfo.clUnit[ siQuestType ];
		if ( (NULL == pclunit) || (0 >= pclunit->siCurQuestUnique) )
		{
			continue;
		}

		if ( true == AddQuestInfo(siQuestType, pclunit) )
		{
			m_siGoingQuestList[ siQuestCount++ ] = siQuestType;
		}
	}

	// 리스트의 갯수가 변하지 않았다면 리스트를 다시 쓰기 전 설정으로 되돌린다
	if ( pclList->GetListNum() == siOldListNum )
	{
		pclList->SetSelectIndex( siOldSelectedItem );
		pclList->SetScrollTrackPos( siOldScrollPos );
	}
	else
	{
		pclEdit->ClearText();
	}

	pclList->Refresh();
	
}

bool CNQuestInfoDlg_GoingQuest::AddDailyQuestInfo( cltDailyQuestInfo* pclDailyQuestInfo )
{
	if ( NULL == pclDailyQuestInfo )
	{
		return false;
	}

	if ( 0 >= pclDailyQuestInfo->m_siCondition )
	{
		return false;
	}

	TCHAR szInfo[ Const_Max_Buf_Size ]	= { '\0', };
	SI32 siGimg							= 0;
	SI32 siFont							= 0;

	pclDailyQuestInfo->GetDailyQuestInfo( szInfo, sizeof(szInfo), &siGimg, &siFont );

	// 히든 퀘스트 일때 텍스트 색깔이 다르다
	if ( true == pclDailyQuestInfo->m_bHidden )
	{
		SetQuestListBoxItemData( siGimg, siFont, szInfo, RGB(255, 0, 0) );
	}
	else
	{
		SetQuestListBoxItemData( siGimg, siFont, szInfo );
	}

	return true;

}


bool CNQuestInfoDlg_GoingQuest::AddPartyQuestInfo(TCHAR* szInfo)
{
	if(szInfo == NULL || strlen(szInfo) <= 0)
		return false;
	else
	{
		cltClient* pClient = (cltClient*) pclClient;

		SI32 siGlobalImageIndex = 0;
		SI32 siFontIndex		= 0;
		SI32 siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_PARTYQUESTNPC"));

		pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siNpcKind, &siGlobalImageIndex, &siFontIndex );

		if(siNpcKind <= 0)
			return false;

		// 파티퀘스트 정보를 퀘스트 정보창에 등록
		SetQuestListBoxItemData( siGlobalImageIndex, siFontIndex, szInfo, RGB(255, 0, 0) );	
	}

	return true;
}


bool CNQuestInfoDlg_GoingQuest::AddQuestInfo( SI32 siQuestType, cltPersonQuestUnitInfo* pclPQuestInfo )
{
	if ( (QUEST_TYPE_SPECIAL > siQuestType) || (MAX_QUEST_TYPE <= siQuestType) )
	{
		return false;
	}

	if ( NULL == pclPQuestInfo )
	{
		return false;
	}

	if ( (QUEST_STEP_CLEAR == pclPQuestInfo->siCurQuestStep) && ((QUEST_TYPE_SPECIAL != siQuestType) && (QUEST_TYPE_HAPPYBEAN != siQuestType)) )
	{
		return false;
	}

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( NULL == pclchar )
	{
		return false;
	}

	TCHAR	szBuffer[ Const_Max_Buf_Size ]		= { '\0', };
	TCHAR	szQuestType[Const_Max_Buf_Size]		= { '\0', };
	TCHAR	szQuestContent[Const_Max_Buf_Size]	= { '\0', };

	SI32	siGlobalImageIndex					= 0;
	SI32	siFontIndex							= 0;

	if ( false == pclClient->pclQuestManager->GetCurStatusText( 1, siQuestType, pclPQuestInfo->siCurQuestUnique, szQuestContent, Const_Max_Buf_Size ) 
		&& ((QUEST_TYPE_SPECIAL != siQuestType) && (QUEST_TYPE_HAPPYBEAN != siQuestType))
	)
	{
		return false;
	}

	// 문장중에 불필요한 단어는 삭제한다.
	for ( SI32 siIndex=0; siIndex<lstrlen(szQuestContent); ++siIndex )
	{
		if( (szQuestContent[siIndex] == '\r') || (szQuestContent[siIndex] == '\n') )
		{
			szQuestContent[siIndex] = ' ';
		}
	}

	switch ( siQuestType )
	{
		case QUEST_TYPE_SPECIAL:	// 요술램프
			{
				TCHAR* pText = GetTxtFromMgr(3086);
				StringCchPrintf( szQuestType, Const_Max_Buf_Size, TEXT("[ %s ]"), pText );

				pclClient->pclItemManager->GetItemGImageFileIndexAndFont( ITEMUNIQUE(13025), &siGlobalImageIndex, &siFontIndex );
			}
			break;

		case QUEST_TYPE_NAMING:		// 명칭여신
			{
				TCHAR* pText = GetTxtFromMgr(10087);
				StringCchPrintf( szQuestType, Const_Max_Buf_Size, TEXT("[ %s ]"), pText );

				SI32 siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_TITLENPC"));
				pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siNpcKind, &siGlobalImageIndex, &siFontIndex );
			}
			break;

		case QUEST_TYPE_HAPPYBEAN:	// 해피빈
			{
				TCHAR* pText = GetTxtFromMgr(7144);
				StringCchPrintf( szQuestType, Const_Max_Buf_Size, TEXT("[ %s ]"), pText );
				
				SI32 siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_HAPPYBEAN_NPC1"));
				pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siNpcKind, &siGlobalImageIndex, &siFontIndex );
			}
			break;
		case QUEST_TYPE_FINDTREASURE:	// 보물찾기 퀘스트 
			{
				SI32 siNpcKind	= pclClient->GetUniqueFromHash(TEXT("KIND_MERIEL"));
				SI32 siNowNum	= pclClient->pclCM->CR[1]->pclCI->clQuestInfo.GetClearQuestNum( pclClient->pclQuestManager, siQuestType ) + 1;
				SI32 siNeedNum	= pclClient->pclQuestManager->GetQuestNum( siQuestType );
				TCHAR* pszName	= (TCHAR*)pclClient->pclKindInfoSet->GetName(siNpcKind);

				if ( (siNpcKind) && (pszName) )
				{
					StringCchPrintf( szQuestType, Const_Max_Buf_Size, TEXT("[%s]  (%d / %d)"), pszName, siNowNum, siNeedNum );

					pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siNpcKind, &siGlobalImageIndex, &siFontIndex );
				}
			}
			break;

		case QUEST_TYPE_MISSION1:
			{
				TCHAR* pText = "";
				SI32 siNpcKind = 0;
				siGlobalImageIndex = -1;
				siFontIndex = 0;

				if( pclPQuestInfo->siCurQuestUnique == MISSIONQUEST_UNIQUE_HOLLOWEEN )
				{
					pText = GetTxtFromMgr(8874);
					siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_HALLOWEENPC"));
				}
				else if( pclPQuestInfo->siCurQuestUnique == MISSIONQUEST_UNIQUE_REMOVE_UNLUCK )
				{
					pText = GetTxtFromMgr(40258);  // "신년이벤트 미션"
					siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_FORTUNE")); // 포춘쿠키용 npc- KIND_QUESTNPC
				}

				else if( pclPQuestInfo->siCurQuestUnique == MISSIONQUEST_UNIQUE_GOONZUDAY )
				{
					pText = GetTxtFromMgr(30534); // "군주데이 미션"
					siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_GOONZUDAYMISSION")); 
				}

				StringCchPrintf( szQuestType, Const_Max_Buf_Size, TEXT("[ %s ]"), pText );    

				pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siNpcKind, &siGlobalImageIndex, &siFontIndex );

				//TCHAR* pText = "";

				//if( pclPQuestInfo->siCurQuestUnique == MISSIONQUEST_UNIQUE_HOLLOWEEN)
				//	pText = GetTxtFromMgr(8874); //40258

				//StringCchPrintf( szQuestType, Const_Max_Buf_Size, TEXT("[ %s ]"), pText );

				//SI32 siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_HALLOWEENPC"));
				//pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siNpcKind, &siGlobalImageIndex, &siFontIndex );
				//siGlobalImageIndex = -1;
				//siFontIndex = 0;
			}
			break;
		/*case QUEST_TYPE_LEVELUP_100:
		case QUEST_TYPE_LEVELUP_101:
		case QUEST_TYPE_LEVELUP_102:
		case QUEST_TYPE_LEVELUP_103:
		case QUEST_TYPE_LEVELUP_104:
		case QUEST_TYPE_LEVELUP_105:
		case QUEST_TYPE_LEVELUP_106:
		case QUEST_TYPE_LEVELUP_107:
		case QUEST_TYPE_LEVELUP_108:
		case QUEST_TYPE_LEVELUP_109:
		case QUEST_TYPE_LEVELUP_110:
		case QUEST_TYPE_LEVELUP_111:
		case QUEST_TYPE_LEVELUP_112:
		case QUEST_TYPE_LEVELUP_113:
		case QUEST_TYPE_LEVELUP_114:
		case QUEST_TYPE_LEVELUP_115:
		case QUEST_TYPE_LEVELUP_116:
		case QUEST_TYPE_LEVELUP_117:
		case QUEST_TYPE_LEVELUP_118:
		case QUEST_TYPE_LEVELUP_119:
		case QUEST_TYPE_LEVELUP_120:
			{
				TCHAR* pText = GetTxtFromMgr(3086);
				StringCchPrintf( szQuestType, Const_Max_Buf_Size, TEXT("[ %s ]"), pText );

				pclClient->pclItemManager->GetItemGImageFileIndexAndFont( ITEMUNIQUE(13025), &siGlobalImageIndex, &siFontIndex );
			}
			break;*/

		default:
			{
				SI32 siNpcKind	= pclClient->pclQuestManager->GetNPCKindFromQuestType( siQuestType );
				SI32 siNowNum	= pclClient->pclCM->CR[1]->pclCI->clQuestInfo.GetClearQuestNum( pclClient->pclQuestManager, siQuestType ) + 1;
				SI32 siNeedNum	= pclClient->pclQuestManager->GetQuestNum( siQuestType );
				TCHAR* pszName	= (TCHAR*)pclClient->pclKindInfoSet->GetName(siNpcKind);

				if ( (siNpcKind) && (pszName) )
				{
					StringCchPrintf( szQuestType, Const_Max_Buf_Size, TEXT("[%s]  (%d / %d)"), pszName, siNowNum, siNeedNum );
					
					pclClient->pclKindInfoSet->GetNPCGlobalImageIndexAndFont( siNpcKind, &siGlobalImageIndex, &siFontIndex );
				}
			}
			break;
			
	}
	
	StringCchPrintf( szBuffer, Const_Max_Buf_Size, TEXT(" %s \r %s"), szQuestType, szQuestContent );

	SetQuestListBoxItemData( siGlobalImageIndex, siFontIndex, szBuffer );

	return true;
}

void CNQuestInfoDlg_GoingQuest::SetQuestListBoxItemData( SI32 siGimgIndex, SI32 siFont, TCHAR* pszText )
{
	if ( NULL == pszText )
	{
		return;
	}

	CList* pclList = m_InterfaceFactory.GetList( NQUESTINFO_GOINGQUEST_LISTBOX_QUESTLIST );
	if ( NULL == pclList )
	{
		return;
	}

	stListBoxItemData listBoxItemData;

	listBoxItemData.Init();

	listBoxItemData.SetSmallIconGImg( 0, siGimgIndex, siFont );
	listBoxItemData.Set( 1, pszText );

	pclList->AddItem( &listBoxItemData );
}

void CNQuestInfoDlg_GoingQuest::SetQuestListBoxItemData( SI32 siGimgIndex, SI32 siFont, TCHAR* pszText, COLORREF colorRef )
{
	if ( NULL == pszText )
	{
		return;
	}

	CList* pclList = m_InterfaceFactory.GetList( NQUESTINFO_GOINGQUEST_LISTBOX_QUESTLIST );
	if ( NULL == pclList )
	{
		return;
	}

	stListBoxItemData listBoxItemData;

	listBoxItemData.Init();

	listBoxItemData.SetSmallIconGImg( 0, siGimgIndex, siFont );
	listBoxItemData.Set( 1, pszText, colorRef );

	pclList->AddItem( &listBoxItemData );
}


void CNQuestInfoDlg_GoingQuest::SelectQuestInfo( SI32 siSelectedIndex )
{
	if ( (0 > siSelectedIndex) || (ALL_QUEST_TYPE <= siSelectedIndex) )
	{
		return;
	}

	SI32 siSelectQuestType = m_siGoingQuestList[ siSelectedIndex ];
	if ( (0 > siSelectQuestType) || (ALL_QUEST_TYPE <= siSelectQuestType) )
	{
		return;
	}

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( NULL == pclchar )
	{
		return;
	}

	CEdit* pclEdit = m_InterfaceFactory.GetEdit( NQUESTINFO_GOINGQUEST_EDIT_EXPLAIN );
	if ( NULL == pclEdit )
	{
		return;
	}

	NTCHARString<2048> strInfo( TEXT("") );
	

	switch ( siSelectQuestType )
	{
		case QUEST_TYPE_SPECIAL:
			{
				TCHAR szBuf[Const_Max_Buf_Size] = { '\0', };

				GetMagicLampQuestInfoText( szBuf, sizeof(szBuf) );
				strInfo = szBuf;
			}
			break;

		case DAILY_QUESTTYPE_1:
			{
				TCHAR				szReward[256] = { '\0', };
				cltDailyQuestInfo	clDailyQuestInof( &pclchar->pclCI->clDailyQuestInfo );

				clDailyQuestInof.GetDailyQuestRewardText( szReward, sizeof(szReward) );
				strInfo = szReward;
			}
			break;

		case DAILY_QUESTTYPE_2:
			{
				TCHAR				szReward[256] = { '\0', };
				cltDailyQuestInfo	clDailyQuestInof( &pclchar->pclCI->clDailyQuest2Info );

				clDailyQuestInof.GetDailyQuestRewardText( szReward, sizeof(szReward) );
				strInfo = szReward;
			}
			break;
		case DAILY_QUESTTYPE_3:
			{
				TCHAR				szReward[256] = { '\0', };
				cltDailyQuestInfo	clDailyQuestInof( &pclchar->pclCI->clDailyQuest3Info );

				clDailyQuestInof.GetDailyQuestRewardText( szReward, sizeof(szReward) );
				strInfo = szReward;
			}
			break;

		case PARTY_QUESTTYPE:
			{
				cltClient* pclclient = (cltClient*)pclClient;
				TCHAR				szReward[256] = { '\0', };
				strInfo =			pclclient->m_pPartyQuestMgr->m_szQuestInfoForDlg;
			}
			break;

		default:
			{
				cltPersonQuestUnitInfo* pclunit = &pclchar->pclCI->clQuestInfo.clUnit[ siSelectQuestType ];
				if  ( NULL == pclunit )
				{
					return;
				}

				SI32 siBit = pclClient->pclQuestManager->GetBitFromUnique( siSelectQuestType, pclunit->siCurQuestUnique );
				if ( 0 <= siBit )
				{
					strInfo = TEXT("#greeting#\r\n#middle#");

					strInfo.Replace( TEXT("#greeting#"),	pclClient->pclQuestManager->clQuestTxtInfo[siSelectQuestType][siBit].szQuestGreeting );
					strInfo.Replace( TEXT("#middle#"),		pclClient->pclQuestManager->clQuestTxtInfo[siSelectQuestType][siBit].szQuestMiddle );
				}
			}
	}

	// 캐릭터 이름 넣기
	strInfo.ReplaceAll( TEXT("#charname#"), (TCHAR*)pclchar->GetName() );

	pclEdit->Clear();
	pclEdit->SetText( strInfo );

}

void CNQuestInfoDlg_GoingQuest::GetMagicLampQuestInfoText( OUT TCHAR* pszBuffer, IN SI32 siBufferSize )
{
	if ( (NULL == pszBuffer) || (0 >= siBufferSize) )
	{
		return;
	}

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( NULL == pclchar )
	{
		return;
	}

	cltQuestInfoCommon* pclspecialquest = &pclchar->pclCI->clQuestInfo.clSpecialQuestInfo;
	if ( (NULL == pclspecialquest) )
	{
		return;
	}

	// 보상목록
	TCHAR szReward[256]	= { '\0', };
	pclspecialquest->clQuestRewardInfo.GetExplainText( szReward, 256 );


	// 조합 단어
	TCHAR szWord[256] = { '\0', };
	pclchar->pclCI->clQuestInfo.GetWordAssembleText( pclClient->pclQuestManager, szWord, sizeof(szWord) );


	// 요술램프 단어 조합 완성 횟수 UI
	TCHAR	szCompleteCount[256]	= { '\0', };
	TCHAR*	pText					= GetTxtFromMgr(2833);
	StringCchPrintf( szCompleteCount, 256, pText, pclchar->pclCI->clQuestInfo.siClearWordSpecialQuest);


	TCHAR szEventCompleteCount[256] = { '\0', };


	cltClient* pclclient  = (cltClient*)pclClient;

	// 요술램프 이벤트 기간동안에만.
	if( pclClient->pclEventTimeManager->InEventTime("MagicLamp_event", &pclclient->sTime) )
	{
		// 이벤트 단어조합 완성 횟수 추가.
		
		TCHAR* pText2 = GetTxtFromMgr(30210); // 이벤트 단어 조합 완성 횟 수: %d회

		//SI32 siClearCount = pclchar->pclCI->clQuestInfo.siEventClearWordSpecialQuest;
		
		// 디비에 저장되어있는 값 7회가 넘어가지 않도록 계산 해서 클라에 보여줌. (8회시 1회로)
		SI32 siClearCount = pclchar->pclCI->clQuestInfo.siEventClearWordSpecialQuest - (((pclchar->pclCI->clQuestInfo.siEventClearWordSpecialQuest - 1) / 7) * 7); 
	
		StringCchPrintf( szEventCompleteCount, 256, pText2, siClearCount );
	}


	StringCchPrintf( pszBuffer, siBufferSize, TEXT("%s\r\n%s\r\n%s\n%s"), szWord, szCompleteCount, szReward, szEventCompleteCount );
}

void CNQuestInfoDlg_GoingQuest::QuestContinue( SI32 siSelectedIndex )
{
	if ( (0 > siSelectedIndex) || (ALL_QUEST_TYPE <= siSelectedIndex) )
	{
		return;
	}

	SI32 siQuestType = m_siGoingQuestList[ siSelectedIndex ];
	if ( (0 > siQuestType) || (ALL_QUEST_TYPE <= siQuestType) )
	{
		return;
	}


	// 이 퀘스트는 퀘스트창이 나올필요없다
	switch ( siQuestType )
	{
		case QUEST_TYPE_SPECIAL:
		case QUEST_TYPE_NAMING:
		case QUEST_TYPE_HAPPYBEAN:
		case QUEST_TYPE_DAILYQUEST2:
		case QUEST_TYPE_DAILYQUEST3:
		case QUEST_TYPE_MISSION1:
		case DAILY_QUESTTYPE_1:
		case DAILY_QUESTTYPE_2:
		case DAILY_QUESTTYPE_3:
		case QUEST_TYPE_FINDTREASURE:
		case PARTY_QUESTTYPE:
			{
				return;
			}
			break;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	// 열려 있다면 닫는다
	if ( pclclient->m_pDialog[NNEWQUEST_DLG] )
	{
		((CNNewQuestDlg*)pclclient->m_pDialog[NNEWQUEST_DLG])->Show( false );
		((CNNewQuestDlg*)pclclient->m_pDialog[NNEWQUEST_DLG])->DeleteDialog();
		return;
	}

	pclclient->CreateInterface( NNEWQUEST_DLG );

	CNNewQuestDlg* pclQuestDlg = (CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ];
	if ( NULL == pclQuestDlg )
	{
		return;
	}

	SI32 siNpcKind = pclclient->pclQuestManager->GetNPCKindFromQuestType( siQuestType );
	if ( 0 >= siNpcKind )
	{
		return;
	}

	pclclient->pclQuestManager->bNewQuseOpened	= TRUE;
	pclQuestDlg->m_siButtonType					= 0;
	pclQuestDlg->SetNewQuest( siNpcKind );
	pclQuestDlg->SetOpenedQuestDlg( true );
	
}

void CNQuestInfoDlg_GoingQuest::ShowQuestCancelMsgBox( SI32 siSelectedIndex )
{
	if ( (0 > siSelectedIndex) || (ALL_QUEST_TYPE <= siSelectedIndex) )
	{
		return;
	}

	SI32 siQuestType = m_siGoingQuestList[ siSelectedIndex ];
	if ( (0 > siQuestType) || (ALL_QUEST_TYPE <= siQuestType) )
	{
		return;
	}

	if(siQuestType == PARTY_QUESTTYPE)
		return;

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	NTCHARString64	strTitle( GetTxtFromMgr(6398) );
	NTCHARString512	strText( GetTxtFromMgr(6399) );

	stMsgBox retMsgBox;
	retMsgBox.Set( this, strTitle, strText, MSGBOX_TYPE_YESNO, QUEST_INFO_REGMSG_GOING_QUESTCANCEL );					

	BYTE bTempBuffer[128]	= {0, };
	SI16 siSize				= sizeof(siQuestType);

	memcpy( bTempBuffer, &siQuestType, sizeof(siQuestType) );

	pclclient->SetMsgBox( &retMsgBox, bTempBuffer, siSize );

}

void CNQuestInfoDlg_GoingQuest::QuestCancel( SI32 siQuestType )
{
	if ( (0 > siQuestType) || (MAX_QUEST_TYPE <= siQuestType) )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltPersonQuestUnitInfo* pclunit = &pclClient->pclCM->CR[1]->pclCI->clQuestInfo.clUnit[siQuestType];
	if ( NULL == pclunit )
	{
		return;
	}

	SI16 siQuestUnique = pclunit->siCurQuestUnique;
	if ( 0 >= siQuestUnique )
	{
		return;
	}

	// 퀘스트 정보를 삭제한다. 
	cltGameMsgRequest_CancelQuest clInfo( siQuestType , siQuestUnique );
	cltMsg clMsg(GAMEMSG_REQUEST_CANCELQUEST, sizeof(clInfo), (BYTE*)&clInfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

	// 퀘스트 취소시 열려있는 퀘스트 다이얼로그 닫기
	if( pclclient->m_pDialog[ NNEWQUEST_DLG ] )
	{
		((CNNewQuestDlg *)pclclient->m_pDialog[ NNEWQUEST_DLG ])->Show( false );
		((CNNewQuestDlg *)pclclient->m_pDialog[ NNEWQUEST_DLG ])->DeleteDialog();
	}

}