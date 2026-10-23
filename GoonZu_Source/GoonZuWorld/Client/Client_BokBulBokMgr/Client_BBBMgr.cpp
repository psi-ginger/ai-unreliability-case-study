#include "Client_BBBMgr.h"

#include "../../NLib/NUtil.h"

#include "Char/CharManager/CharManager.h"
#include "../CommonLogic/CommonLogic.h"
#include "../Lib/dxutil.h"
#include "../../Client/Client.h"

#include "../InterfaceMgr/InterfaceFile.h"
#include "../InterfaceMgr/InterfaceMgr.h"
#include "../InterfaceMgr/DialogController.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../ninterface/nrareitemdlg/NRareItemDlg.h"
#include "../NInterface/NCommonMsgDlg/NCommonMsgDlg.h"
#include "../ninterface/NBokBulBokEventDlg/NBokBulBokEventReserveListDlg.h"
#include "../ninterface/NBokBulBokEventDlg/NBokBulBokEventDlg.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/Msg/MsgType-BokBulBok.h"







CClient_BBBMgr::CClient_BBBMgr()
{
	m_bWaitListResult = false;
	m_dwStageStartTime = 0;
	m_bStageStartFlag = false;

	m_siStageStartDelay = 6000;
	m_siStageLoseDelay = 5000;
	m_siStageWinDelay = 5000;
	m_siTotalStageWinDelay = 5000;
	m_siStagePlayDelay = 15000;
	m_b3secDelay = false;

	m_bGMShowMonsterType = false;

	if ( pclClient->IsCountrySwitch(Switch_BokBulBok) == true )
	{
		_Load_BBBGameInfo();
	}
}

CClient_BBBMgr::~CClient_BBBMgr()
{
}


void CClient_BBBMgr::Action()
{
	cltClient* pClient = (cltClient*)pclClient;
	cltCharClient* pclChar = pClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		_Clear_BBBStatus();
		return;
	}
	else
	{
		if ( true == m_bStageStartFlag )
		{
			if ( PLAY_MAP_INDEX == pclChar->GetMapIndex() )
			{
				cltMapCommon* pclmap = pClient->pclMapManager->GetMapPointer( PLAY_MAP_INDEX );
				if ( NULL != pclmap )
				{
					SI32 siColl = pclmap->CalcCollInfo( pclChar->GetX(), pclChar->GetY());
					if( 0 == (siColl & BLOCK_EVENT2 || siColl & BLOCK_EVENT3 || siColl & BLOCK_EVENT5 || siColl & BLOCK_EVENT6) )		// 2, 3, 5, 6번 이벤트 타일을 밟고 있지 않을때
					{
						_Clear_BBBStatus();   
					}
				}
				else						// 이벤트 맵 자체가 없다.
				{
					_Clear_BBBStatus();
				}
			}
			else							// 유저가 이벤트 맵에 없다.
			{
				_Clear_BBBStatus();
			}
		}
	}

	// 스테이지 시작할때 5초 카운트 -> 라운드 -> 플레이타임(15초) 카운트
	if ( true == pclChar->bBBBEvent_5SecCountDownFlag )
	{
		if ( (pclChar->siBBBEvent_5SecCountDown + m_siStageStartDelay) < pClient->CurrentClock )
		{
			// 5초 카운트 종료
			pclChar->bBBBEvent_5SecCountDownFlag = false;
			pclChar->siBBBEvent_5SecCountDown = 0;

			// 플레이타임 카운트 + 라운드 표시 시작
			pClient->bTimeCount = true;
			pClient->siTimeSet	=	m_siStagePlayDelay;				// 15초..초단위로 넣습니다~
			pClient->dwWriteSetTime	=	pclClient->CurrentClock;	// 시간을 저장한 시간
		}
	}

	if ( true == pclChar->bBBBEvent_StageFailFlag )
	{
		if ( (pclChar->siBBBEvent_StageFailCountDown + m_siStageLoseDelay) < pClient->CurrentClock )
		{
			pclChar->bBBBEvent_StageFailFlag = false;
			pclChar->siBBBEvent_StageFailCountDown = 0;

			m_b3secDelay = true;
			pclChar->siBBBEvent_TotalStageClearCountDown = pClient->CurrentClock + 1000;
		}
	}

	if ( true == m_b3secDelay )
	{
		if ( pclChar->siBBBEvent_TotalStageClearCountDown < pClient->CurrentClock )
		{
			m_b3secDelay = false;
			m_bStageStartFlag = false;
			pclChar->bBBBEvent_TotalStageClearFlag = true;
			pclChar->siBBBEvent_TotalStageClearCountDown = pClient->CurrentClock;
		}
	}

	if ( true == pclChar->bBBBEvent_TotalStageClearFlag )
	{
		if ( pclChar->siBBBEvent_TotalStageClearCountDown + m_siStageLoseDelay < pClient->CurrentClock )
		{
			pclChar->bBBBEvent_TotalStageClearFlag = false;
			pclChar->siBBBEvent_TotalStageClearCountDown = 0;
		}
	}

	if ( true == pclChar->bBBBEvent_StageClearFlag )
	{
		if ( (pclChar->siBBBEvent_StageClearCountDown + m_siStageWinDelay) < pClient->CurrentClock )
		{
			pclChar->bBBBEvent_StageClearFlag = false;
			pclChar->siBBBEvent_StageClearCountDown = 0;
		}
	}
}

void CClient_BBBMgr::Warp_GameStage(SI32 siPersonID)
{
	// 맵으로 워프
	//m_pClient_BBBMgr->Warp_GameStage(siCharPersonID); // 이건 가져다 쓸때;;
}

// 맵은 모두 사용중이고 예약리스트 등록할때 뜨는 메세지.
void CClient_BBBMgr::Pop_WaitListDlg()
{
	//리스트에 동록하겠냐는 대화상자 팝업.
	cltClient* pClient = (cltClient*)pclClient;

	if (pClient->m_pDialog[NCOMMONMSG_DLG] == NULL)
	{
		pClient->CreateInterface( NCOMMONMSG_DLG );
	}

	((CNCommonMsgDlg*)pClient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_BBB_WAIT_QUESTION);
	//((CNCommonMsgDlg*)pClient->m_pDialog[NCOMMONMSG_DLG])->SetShowTimeSec(10);	// 대답 제한 시간 10초.
}

// 복불복 예약자리스트가 다 찼을때 뜨는 메세지
void CClient_BBBMgr::Pop_EntryFailDlg()
{
	cltClient* pClient = (cltClient*)pclClient;

	TCHAR*	pTitle	= GetTxtFromMgr(40065); // 복불복 게임
	TCHAR*	pBody	= GetTxtFromMgr(40076); // 조금만 더 빨리 오시지~ 한발 늦으셨네요. 잠시 뒤에 신청해 주세요.
	pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);

	//pClient->m_pDialog[NOTIFYMSG_DLG]->DeleteDialog();

}

// 대기자 리스트를 서버로 부터 얻어왔고, 이 이름들을 대화상자에 채워서 대화상자를 출력함
void CClient_BBBMgr::Pop_DetailWaitListDlg(TCHAR** ppWaitList, bool bResult, GMONEY siTotalMoney )
{
	if ( NULL == ppWaitList ) return;
	cltClient* pClient = (cltClient*)pclClient;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	// 대기자 리스트 현황을 보여주는 대화상자를 출력
	if (pClient->m_pDialog[NBOKBULBOK_RESERVELIST_DLG] == NULL)
	{
		pClient->CreateInterface( NBOKBULBOK_RESERVELIST_DLG );

		// 최초 대기자 리스트 창이 뜰때만 실패 메세지창 띄워줌. 
		// 이후에 이창 떠있을때 대기자 신청 버튼누름에 대해 실패 결과가 서버에서 돌아와도 메세지창을 띄우진 않는다. 
		// 대신 성공에 대한 메세지창은 띄움.
		if ( false == bResult ) // 등록 실패 시 
		{
			// 실패 상황 알려주는 대화상자 출력
			cltClient* pClient = (cltClient*)pclClient;

			TCHAR*	pTitle	= GetTxtFromMgr(40065); // 복불복 게임
			TCHAR*	pBody	= GetTxtFromMgr(40029); // 대기자 명단에 등록 실패
			pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);
		}
	}

	((CBokBulBokEventReserveListDlg*)pClient->m_pDialog[NBOKBULBOK_RESERVELIST_DLG])->SetListData( ppWaitList );
	//((CNCommonMsgDlg*)pClient->m_pDialog[NCOMMONMSG_DLG])->SetShowTimeSec();	// 대답 제한 시간 10초.


	// 대기자 등록 요청에 대한 결과를 알려주는 대화상자 출력 (대기자 등록 성공/실패에 대한)
	m_bWaitListResult = bResult;		// 처리 결과를 클라이언트쪽 BBB매니저에 저장해두고,

	// 처리 결과에 따른 대화상자를 출력
	if ( true == bResult )
	{
		// 대기자 등록에 성공했다는 메세지창
		cltClient* pClient = (cltClient*)pclClient;

		TCHAR*	pTitle	= GetTxtFromMgr(40065); // 복불복 게임
		TCHAR*	pBody	= GetTxtFromMgr(40074); // 대기자 등록되셨습니다.조금만 기다리면 도전하실 수 있어요.
		pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);
	}
	else
	{
		// 대기자 등록 신청 했지만 리스트가 풀 되는 바람에 등록 못하고 리스트만 받은 상황.
	}
}

void CClient_BBBMgr::Pop_WaitListCancelResult()
{
	cltClient* pClient = (cltClient*)pclClient;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	// 대기 리스트에서 지워졌다는걸 알려주는 대화상자 출력
	TCHAR*	pTitle	= GetTxtFromMgr(40065); // 복불복 게임
	TCHAR*	pBody	= GetTxtFromMgr(40112); // 대기자 명단에서 삭제 되었습니다.
	pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);
}

void CClient_BBBMgr::RefreshWaitList(TCHAR** ppWaitList)
{
	if ( NULL == ppWaitList ) return;
	cltClient* pClient = (cltClient*)pclClient;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	// 대기자 리스트 현황을 보여주는 대화상자를 출력
	if (pClient->m_pDialog[NBOKBULBOK_RESERVELIST_DLG] == NULL)
	{
		pClient->CreateInterface( NBOKBULBOK_RESERVELIST_DLG );
	}

	((CBokBulBokEventReserveListDlg*)pClient->m_pDialog[NBOKBULBOK_RESERVELIST_DLG])->SetListData( ppWaitList );
}

void CClient_BBBMgr::StageStart(SI32 siPersonID, SI16 siCurrentStage, DWORD dwStartTime)
{
	cltClient* pClient = (cltClient*)pclClient;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	// 5초 카운트 부분.
	m_siCurrentStage = siCurrentStage;
	pclChar->siBBBEvent_5SecCountDown = pClient->CurrentClock;
	pclChar->bBBBEvent_5SecCountDownFlag = true;

	// 라운드는 스테이지 시작 5초 카운트 시작될때 같이 시작
	pClient->siRoundCount = m_siCurrentStage -1;				// 몇판째인지도 넣어줌.

	m_bStageStartFlag = true;
}

void CClient_BBBMgr::StageFail(SI32 siPersonID, DWORD dwEndTime)
{
	cltClient* pClient = (cltClient*)pclClient;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	// Lose
	pclChar->siBBBEvent_TotalStageClearResultImageIndex = 4;

	pclChar->siBBBEvent_StageFailCountDown = pClient->CurrentClock;
	pclChar->bBBBEvent_StageFailFlag = true;


	// 라운드 표시 지워줌
	pClient->siRoundCount = -1;

	// 스테이지 제한시간도 지워줌
	pClient->bTimeCount = false;
	pClient->siTimeSet	=	0;								// 30초..초단위로 넣습니다~
	pClient->dwWriteSetTime	=	0;	// 시간을 저장한 시간
}

void CClient_BBBMgr::Pop_WarpMapQuestionDlg()
{
	cltClient* pClient = (cltClient*)pclClient;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	// 빈맵 생겼는데 지금 워프 할래요? 라는 대화상자 출력
	if (pClient->m_pDialog[NCOMMONMSG_DLG] == NULL)
	{
		pClient->CreateInterface( NCOMMONMSG_DLG );
	}

	((CNCommonMsgDlg*)pClient->m_pDialog[NCOMMONMSG_DLG])->Set( COMMONDLG_BBB_WARPMAPQUESTION );
	((CNCommonMsgDlg*)pClient->m_pDialog[NCOMMONMSG_DLG])->SetShowTimeSec(10);	// 대답 제한 시간 10초.
}

void CClient_BBBMgr::Pop_NotEnoughMoneyDlg()
{
	cltClient* pClient = (cltClient*)pclClient;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	// 가진 돈이 부족하다고 대화상자 띄움
	TCHAR*	pTitle	= GetTxtFromMgr(1260); // 소지금 부족
	TCHAR*	pBody	= GetTxtFromMgr(1261); // 소지금을 확인해 주십시오.
	pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);


	// 가진 돈이 부족하다고 대화상자 띄움
	//if (pClient->m_pDialog[NCOMMONMSG_DLG] == NULL)
	//{
	//	pClient->CreateInterface( NCOMMONMSG_DLG );
	//}

	//((CNCommonMsgDlg*)pClient->m_pDialog[NCOMMONMSG_DLG])->Set( COMMONDLG_BBB_NOTENOUGHMONEY );
	//((CNCommonMsgDlg*)pClient->m_pDialog[NCOMMONMSG_DLG])->SetShowTimeSec(10);	// 대답 제한 시간 10초.
}

void CClient_BBBMgr::StageClear()
{
	cltClient* pClient = (cltClient*)pclClient;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	pclChar->siBBBEvent_StageClearCountDown = pClient->CurrentClock;
	pclChar->bBBBEvent_StageClearFlag = true;

	// 라운드 표시 지워줌
	pClient->siRoundCount = -1;

	// 스테이지 제한시간도 지워줌
	pClient->bTimeCount = false;
	pClient->siTimeSet	=	0;		// 30초..초단위로 넣습니다~
	pClient->dwWriteSetTime	=	0;	// 시간을 저장한 시간
}

void CClient_BBBMgr::TotalStageClear()
{
	cltClient* pClient = (cltClient*)pclClient;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if( NULL == pclChar)
	{
		return;
	}

	pclChar->siBBBEvent_TotalStageClearCountDown = pClient->CurrentClock;
	pclChar->bBBBEvent_TotalStageClearFlag = true;

	// 라운드 표시 지워줌
	pClient->siRoundCount = -1;

	// 스테이지 제한시간도 지워줌
	pClient->bTimeCount = false;
	pClient->siTimeSet	=	0;		// 30초..초단위로 넣습니다~
	pClient->dwWriteSetTime	=	0;	// 시간을 저장한 시간	
}

void CClient_BBBMgr::_Load_BBBGameInfo()
{
	//////////////////////////////////////////////////////////////////////////
	// 파일 로드
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/BBBEvent/BBBEvent_BBBGameInfo.txt") );
	}
	else
	{
		if ( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
		{
			bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/BBBEvent/BBBEvent_BBBGameInfo.txt"));
		}

		else
		{
			bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/BBBEvent/BBBEvent_BBBGameInfo.txt") );
		}	
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile BBBEvent_BBBGameInfo.txt"),TEXT("Error"));
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 읽어들인 파일 내용으로 배열 초기화
	SI32 siStartDelay = 0;				// 스테이지 시작시 카운트 딜레이 (몇부터 카운트 될지? 현재 5초부터 카운트. 3초로 바꾸려면 파일에서 3으로)
	SI32 siLoseDelay = 0;				// Lose 표시 딜레이
	SI32 siWinDelay = 0;				// Win 표시 딜레이
	SI32 siPlayDelay = 0;				// 스테이지 플레이 시간
	//SI32 siTotalWinDelay = 0;			// 토탈 스테이지 완료 후 WIN 딜레이
	SI32 siDropRate = 0;				// 선물몹이 아이템 드롭할 확률	(%)

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siStartDelay,		4,
			NDATA_INT32,	&siLoseDelay,		4,
			NDATA_INT32,	&siWinDelay,		4,
			NDATA_INT32,	&siPlayDelay,		4,
			NDATA_INT32,	&siDropRate,		4,
			//NDATA_INT32,	&siTotalWinDelay,	4,
			0,				0,					0
	};	
	dataloader.RegDataType( pDataTypeInfo );


	//////////////////////////////////////////////////////////////////////////
	SI32 siArrayindex = 0;
	while( !dataloader.IsEndOfData() )
	{
		if ( dataloader.ReadData(true) )
		{
			if ( siArrayindex >= 1 )	break;

			m_siStageStartDelay = (siStartDelay + 1) * 1000;		// start 표시를 위한 1초를 더해준다.
			m_siStageLoseDelay = siLoseDelay * 1000;
			m_siStageWinDelay = siWinDelay * 1000;
			//m_siTotalStageWinDelay = siTotalWinDelay;
			m_siStagePlayDelay = siPlayDelay;				// 클라이언트엔 1000을 곱하지 않고 그냥 초 단위로 사용 (15초면 15)
			//m_siItemDropRate = siDropRate * 100;					// 클라이언트엔 아이템 드롭율이 필요없으므로 읽어서 그냥 버린다.

			++siArrayindex;
		}
	}
}

void CClient_BBBMgr::Notice_10ClearNotice(SI32 siRewardMoney, TCHAR* pszWinnerName )
{
	cltClient* pClient = (cltClient*)pclClient;
	cltCharClient* pCharClient = (cltCharClient*)pClient->pclCM->GetCharClient( 1 );
	if ( NULL == pCharClient ) return;

	NTCHARString256 strText = GetTxtFromMgr(40068); //축하합니다. #name#님이 복불복 게임에서 모든 스테이지를 클리어하여 #money#골드의 상금을 받았습니다.
	NTCHARString256 strMoney;
	g_JYLibFunction.SetNumUnit( siRewardMoney, strMoney, 256 );

	strText.Replace( TEXT("#money#"), strMoney );

	NTCHARString256 strName = pszWinnerName;
	strText.Replace( TEXT("#name#"), strName );

	TCHAR* pszTitle = GetTxtFromMgr(865);

	if( pClient->m_pDialog[NRAREITEM_DLG ] == NULL )
	{
		pClient->CreateInterface( NRAREITEM_DLG );
	}
	((CNRareItemDlg*)pClient->m_pDialog[NRAREITEM_DLG ])->Set( NULL, (TCHAR*)strName, pszTitle, (TCHAR*)strText );
}

void CClient_BBBMgr::Check_AlreadyReserved()
{
	// 패킷 보냄 
	cltClient* pClient = (cltClient*)pclClient;
	cltCharClient* pCharClient = pClient->pclCM->GetCharClient( 1 );
	if ( NULL == pCharClient ) return;

	cltGameMsgRequest_BBB_AlreadyReserved clinfo( 1 );
	cltMsg clMsg(GAMEMSG_REQUEST_BBB_ALREADYRESERVED, sizeof(clinfo), (BYTE*)&clinfo);
	pCharClient->SendNetMsg((sPacketHeader*)&clMsg);
}

void CClient_BBBMgr::Receive_AlreadyReserved(TCHAR** ppWaitList, bool bResult, GMONEY siTotalMoney )
{
	// 패킷 받음 
	if ( true == bResult )
	{
		if ( NULL == ppWaitList ) return;
		cltClient* pClient = (cltClient*)pclClient;

		cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
		if ( NULL == pclChar )
		{
			return;
		}

		// 대기자 리스트 현황을 보여주는 대화상자를 출력
		if (pClient->m_pDialog[NBOKBULBOK_RESERVELIST_DLG] == NULL)
		{
			pClient->CreateInterface( NBOKBULBOK_RESERVELIST_DLG );
		}

		((CBokBulBokEventReserveListDlg*)pClient->m_pDialog[NBOKBULBOK_RESERVELIST_DLG])->SetListData( ppWaitList );
	}
	else
	{
		cltClient* pClient = (cltClient*)pclClient;

		cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
		if ( NULL == pclChar )
		{
			return;
		}

		if ( NULL == pClient->m_pDialog[NBOKBULBOK_EVENT_DLG] )
		{
			pClient->CreateInterface( NBOKBULBOK_EVENT_DLG );
		}

		((NBokBulBokEventDlg*)pClient->m_pDialog[NBOKBULBOK_EVENT_DLG])->Show_FirstMessageDlg( siTotalMoney );		
	}
}

void CClient_BBBMgr::_Clear_BBBStatus()
{
	cltClient* pClient = (cltClient*)pclClient;
	cltCharClient* pCharClient = pClient->pclCM->GetCharClient( 1 );
	if ( NULL == pCharClient ) return;

	/*pCharClient->bBBBEvent_5SecCountDownFlag = false;
	pCharClient->siBBBEvent_5SecCountDown = 0;

	pCharClient->bBBBEvent_StageFailFlag = false;
	pCharClient->siBBBEvent_StageFailCountDown = 0;

	pCharClient->bBBBEvent_StageClearFlag = false;
	pCharClient->siBBBEvent_StageClearCountDown = 0;

	pCharClient->bBBBEvent_TotalStageClearFlag = false;
	pCharClient->siBBBEvent_TotalStageClearCountDown = 0;

	m_b3secDelay = false;*/

	m_bStageStartFlag = false;

	// 라운드 표시 지워줌
	pClient->siRoundCount = -1;

	// 스테이지 제한시간도 지워줌
	pClient->bTimeCount = false;
	pClient->siTimeSet	=	0;								// 30초..초단위로 넣습니다~
	pClient->dwWriteSetTime	=	0;	// 시간을 저장한 시간
}

void CClient_BBBMgr::DrawWinImage_ForMe()
{
	cltClient* pClient = (cltClient*)pclClient;
	cltCharClient* pCharClient = pClient->pclCM->GetCharClient( 1 );
	if ( NULL == pCharClient ) return;

	pCharClient->siBBBEvent_TotalStageClearResultImageIndex = 0;
	m_b3secDelay = true;
	pCharClient->siBBBEvent_TotalStageClearCountDown = pClient->CurrentClock + 1000;

	// 라운드 표시 지워줌
	pClient->siRoundCount = -1;

	// 스테이지 제한시간도 지워줌
	pClient->bTimeCount = false;
	pClient->siTimeSet	=	0;		// 30초..초단위로 넣습니다~
	pClient->dwWriteSetTime	=	0;	// 시간을 저장한 시간
}