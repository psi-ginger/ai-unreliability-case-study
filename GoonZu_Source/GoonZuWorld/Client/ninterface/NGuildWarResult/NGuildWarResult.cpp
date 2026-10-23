#include "NGuildWarResult.h"

#include "Char\CharManager\CharManager.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"
#include "../CommonLogic/Guild/GuildHuntMapWarStatMgr.h"


extern cltCommonLogic* pclClient;

CNGuildWarResult::CNGuildWarResult()
{	
	Init();
}

CNGuildWarResult::~CNGuildWarResult()
{
	Destroy();
}


void CNGuildWarResult::Init()
{
	m_siInputCount	=	0;
	m_siActionCount	=	0;
}

void CNGuildWarResult::Destroy()
{

}

void CNGuildWarResult::Create()
{
	if ( IsCreate() )
		return;


	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NGuildWarResult/NGuildWarResult.ddf"));
	file.CreatePopupDialog( this, NGUILDWARRESULT_DLG , TEXT("dialog_guildwarresult"), StaticGuildWarResultDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NGUILDWARRESULT_NONAME , this ), NGUILDWARRESULT_NONAME,   "statictext_guildwarresult_nona");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NGUILDWARRESULT_INFO , this ), NGUILDWARRESULT_INFO,   "statictext_resultinfo");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NGUILDWARRESULT_HOMETEAM , this ), NGUILDWARRESULT_INFO,   "statictext_hometeam");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NGUILDWARRESULT_AWAYTEAM , this ), NGUILDWARRESULT_INFO,   "statictext_awayteam");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST, NGUILDWARRESULT_LIST , this ), NGUILDWARRESULT_LIST,   "listbox_resultlist");

	SetTitle(GetTxtFromMgr(10327));		//	길드전 종료 타이틀 바

	
	CStatic* pStatic = m_InterfaceFactory.GetStatic( NGUILDWARRESULT_NONAME );		//	'전적 정보' 라는 텍스트 찍어줌

	if(pStatic != NULL)
		pStatic->SetText(GetTxtFromMgr(10335));

	
	CStatic* pStaticHometeam = m_InterfaceFactory.GetStatic( NGUILDWARRESULT_HOMETEAM );	//	홈팀 길드명

	if(pStaticHometeam != NULL)
		pStaticHometeam->SetText(pclClient->m_pclGuildHuntMapWarStatMgr->GetGuildName(0));

	CStatic* pStaticAwayteam = m_InterfaceFactory.GetStatic( NGUILDWARRESULT_AWAYTEAM );	//	어웨이팀 길드명

	if(pStaticAwayteam != NULL)
		pStaticAwayteam->SetText(pclClient->m_pclGuildHuntMapWarStatMgr->GetGuildName(1));

	
    CList* pList = m_InterfaceFactory.GetList(NGUILDWARRESULT_LIST);
	pList->Clear();

	if (pList != NULL)
	{
		SI16 siNameWidth		=	130;
		SI16 siKillWidth		=	50;
		SI16 siDieWidth			=	50;

		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr(1556);		//	 이름
		pList->SetColumn(0, siNameWidth, pText);

		pText = GetTxtFromMgr( 10314 );			//	 킬
		pList->SetColumn(1, siKillWidth, pText);

		pText = GetTxtFromMgr( 10315 );			//	 다이
		pList->SetColumn(2, siDieWidth, pText);

		pText = GetTxtFromMgr(1556);			//	 이름
		pList->SetColumn(3, siNameWidth, pText);

		pText = GetTxtFromMgr( 10314 );			//	 킬
		pList->SetColumn(4, siKillWidth, pText);

		pText = GetTxtFromMgr( 10315 );			//	 다이
		pList->SetColumn(5, siDieWidth,	pText);

		pList->Refresh();
	}


	SetActionTimer( 1000 );
	SetTimerOnOff( true );
}


void CNGuildWarResult::Action()
{
	cltClient *pclclient = (cltClient *)pclClient;
	SI32 siMapIndex = pclclient->pclCM->CR[1]->GetMapIndex();

	if(siMapIndex != MAPINDEX_GUILDWARFIELD  && IsShow() == true)		//	현재 길드전장맵이 아니면서 길드전 결과창이 보여지고있다면
	{																	
		if(m_siActionCount >= 20)										//	20초후에 결과창을 안보여줍니다.
		{
			Show(false);
			CList* pList = m_InterfaceFactory.GetList(NGUILDWARRESULT_LIST);
			pList->Clear();

			m_siActionCount	=	0;
		}
		m_siActionCount++;
	}

}

void CNGuildWarResult::setWinLose(SI32 siWinLose)
{
	CStatic* pStaticInfo = m_InterfaceFactory.GetStatic( NGUILDWARRESULT_INFO );	//	결과 정보창 (BGP 얼마나 받나 보여줌)

	if( pStaticInfo != NULL )
	{     
		NTCHARString128 szResultInfo; 

		switch(siWinLose)
		{
		case 0:				//	승리
		case 3:
			{
				szResultInfo	=	GetTxtFromMgr(9949);			//	이긴 메세지
				szResultInfo.Replace("#point#",SI32ToString(50));	//	이긴경우 정보창
			}
			break;

		case 1:				//	무승부
		case 4:
			{ 
				szResultInfo	=	GetTxtFromMgr(10313);			//	무승부 메세지
				szResultInfo.Replace("#point#",SI32ToString(20));	//	비긴경우 정보창
			}
			break;

		case 2:				//	패배
		case 5:
			{
				szResultInfo	=	GetTxtFromMgr(10312);			//	진 메세지
				szResultInfo.Replace("#point#",SI32ToString(10));	//	진 경우 정보창
			}
			break;

		case 6:				//	잘못된 데이터
			{
				_ASSERT(0);
			}
			break;
		}

		pStaticInfo->SetText(szResultInfo);
	}
}

void CNGuildWarResult::InputData(SI32 siTeamIndex, TCHAR* pUserName, SI32 siKillPoint, SI32 siDeathPoint)
{
	CList* pList = m_InterfaceFactory.GetList(NGUILDWARRESULT_LIST);
	_ASSERT(pList != NULL);


	ZeroMemory(&m_clGulidMember, sizeof(GuildMember));
	m_clGulidMember.m_siTeamIndex = siTeamIndex;
	StringCchCopy(m_clGulidMember.m_szUserName, MAX_PLAYER_NAME, pUserName);		//	이름 설정
	m_clGulidMember.m_siKillCount	=	siKillPoint;
	m_clGulidMember.m_siDeathCount	=	siDeathPoint;

	TCHAR szBufferKillCount[256] = "";
	itoa( siKillPoint, szBufferKillCount, 10);

	TCHAR szBufferDeathCount[256] = "";
	itoa( siDeathPoint, szBufferDeathCount, 10);


	switch(siTeamIndex)
	{
	case 0:
		m_tempItemData.Set(0, pUserName, COLOR_DIALOG_BLACK);
		m_tempItemData.Set(1, szBufferKillCount, COLOR_DIALOG_BLACK);
		m_tempItemData.Set(2, szBufferDeathCount, COLOR_DIALOG_BLACK);
		break;

	case 1:
		m_tempItemData.Set(3, pUserName, COLOR_DIALOG_BLACK);
		m_tempItemData.Set(4, szBufferKillCount, COLOR_DIALOG_BLACK);
		m_tempItemData.Set(5, szBufferDeathCount, COLOR_DIALOG_BLACK);

		pList->AddItem( &m_tempItemData );
		break;
	}

	pList->Refresh();



	
	//SetList(&clGulidMember);		//	받아온 결과 레코드를 리스트에 설정한다.

}

void CNGuildWarResult::SetList()
{
	
}

void CALLBACK CNGuildWarResult::StaticGuildWarResultDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGuildWarResult *pThis = (CNGuildWarResult*) pControllerMgr;
	pThis->NGuildWarResultDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNGuildWarResult::NGuildWarResultDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}
}
