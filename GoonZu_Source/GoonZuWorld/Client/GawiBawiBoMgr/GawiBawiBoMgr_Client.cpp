#include "GawiBawiBoMgr_Client.h"

#include "Char/CharManager/CharManager.h"
#include "../CommonLogic/CommonLogic.h"
#include "../Lib/dxutil.h"
#include "../Client/Client.h"

#include "../InterfaceMgr/InterfaceFile.h"
#include "../InterfaceMgr/InterfaceMgr.h"
#include "../../common/CommonHeader.h"
#include "../ninterface/NGawiBawiBoDlg/NGawiBawiBoDlg.h"
#include "../../CommonLogic/MsgType-GawiBawiBo.h"


extern cltCommonLogic* pclClient;

CGawiBawiBoMgr_Client::CGawiBawiBoMgr_Client()
{
	m_siStatus			= GAWIBAWIBO_NONE;	
	m_siResult			= GAWIBAWIBO_RESULT_NONE;
	m_siCumulativeMoney	= 0;
	m_siGawiBawiBoCount	= 0;

	ZeroMemory(&m_Timer, sizeof(clock_t));
	ZeroMemory(m_siWInPercentage, sizeof(m_siWInPercentage));
}

CGawiBawiBoMgr_Client::~CGawiBawiBoMgr_Client()
{
	
}


bool	CGawiBawiBoMgr_Client::Initialize()			//	매니저 초기화
{
	//	매니저 초기화 작업.. 파일로드 등등
	bool bResult = false;

	//	매니저 초기화 작업.. 파일로드 등등
	bResult = LoadGawiBawiBoFiles();

	InitTimer();

	return bResult;
}

bool CGawiBawiBoMgr_Client::LoadGawiBawiBoFiles()
{
	NDataLoader	dataloader;

	bool bResult = false;

	if( pclClient->IsWhereServiceArea(ConstSwitch_TextEncryption))
		bResult = dataloader.LoadDataFromCryptFile(TEXT("Data/SRPInfo.txt"));
	else
		bResult = dataloader.LoadDataFromFile(TEXT("Data/SRPInfo.txt"));

	if (bResult == false)
		return false;

	TCHAR	szName[MAX_PATH] = TEXT("");
	SI32	siValue = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szName,		MAX_PATH,
		NDATA_INT32,	&siValue,	4,
		0,				0,			0
	};

	dataloader.RegDataType(pDataTypeInfo);

	SI16 siCount = 1;

	while(dataloader.IsEndOfData() != true) 
	{
		if (dataloader.ReadData() == true)	
		{
			if(siCount <= 4)		//	데이터파일의 3번 인덱스까지는 승률 데이터
				m_siWInPercentage[siCount] = siValue;
			else				//	데이터파일의 3번인덱스 부터는 누적포인트 데이터 (누적포인트는 누적카운트가 기획상 더 늘어날수 있으므로 벡터로)
				m_vecStraightWins.push_back(siValue);				

			siCount++;
		}
		else
			return true;
	}
	return false;	
}


bool CGawiBawiBoMgr_Client::StartGame()		//	가위바위보 매니저 작동
{
	cltClient *pClient = (cltClient*)pclClient;

	if(pClient != NULL)
		pClient->CreateInterface(NGAWIBAWIBO_DLG);
	else
		return false;


	//	플레이어 기다리는중 상태였다면 플레이어가 입장 했으므로 플레이 가능한지 금액 체크
	if(GetStatus() == GAWIBAWIBO_WAIT_PLAYER)
		SetStatus(GAWIBAWIBO_DO_CHECK_MONEY);

	return true;
}

void	CGawiBawiBoMgr_Client::Action()
{
	cltClient *pClient = (cltClient*)pclClient;
	if(pClient == NULL)
		return;

	// 만약 현재 이벤트맵을 벗어났다면 기권
	if(((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG]) != NULL)
	{
		if(pClient->pclCM->CR[1]->GetMapIndex() != MAPINDEX_FESTIVAL)
		{
			if(m_siStatus >= GAWIBAWIBO_WAIT_PLAYER)
			{
				GiveUp();

				if((CControllerMgr*)pClient->m_pDialog[NMSGBOX_DLG] != NULL)
					((CControllerMgr*)pClient->m_pDialog[NMSGBOX_DLG])->DeleteDialog();
			}
		}
	}

	switch(m_siStatus)
	{
	case GAWIBAWIBO_NONE:
		SetStatus(GAWIBAWIBO_INIT);
		break;

	case GAWIBAWIBO_INIT:
		{
			if(Initialize() == true)
				NextSteps();	//	매니저 초기화 완료.
			else
				MsgBox("Error", "GawiBawiBo::Initialize is Failed");	
		}
		break;

	case GAWIBAWIBO_WAIT_PLAYER:	// 유저를 기다리는중	
		break;

	case GAWIBAWIBO_DO_CHECK_MONEY:
		{
			//	유저가 들어왔다. 게임을 할수 있는지 금액을 체크
			SI32 siNpcrate		= pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
			GMONEY siNeedMoney	= siNpcrate * 200;	//	 비용은 시세 * 10

			TCHAR* pTitle	= GetTxtFromMgr(10344);
			NTCHARString256	pBody(GetTxtFromMgr(10343));

			NTCHARString256 strMoney;
			g_JYLibFunction.SetNumUnit(siNeedMoney, strMoney, 256);

			pBody.Replace("#money#", strMoney);

			stMsgBox MsgBox;
			MsgBox.Set((CControllerMgr*)pClient->m_pDialog[NGAWIBAWIBO_DLG], pTitle, pBody, MSGBOX_TYPE_YESNO, 0);
			pClient->SetMsgBox(&MsgBox, NULL, 0);

			NextSteps();
			SetTimer(GAWIBAWIBO_MAX_TIME);
		}
		break;

	case GAWIBAWIBO_NOW_CHECKING:		//	금액 체크중이니 대기~
		{
			// 타임아웃
			if(IsTimePassed() == true)
			{
				((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->EndCount();	// 시간내에 결정을 못했으므로 기권처리
				GiveUp();
			}
			else
			{
				// 게임을 계속 할지 결정 대기 중이니 카운트
				SI32 siNpcrate		= pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
				GMONEY siNeedMoney	= siNpcrate * 200;	//	 비용은 시세 * 10

				TCHAR* pTitle	= GetTxtFromMgr(10344);
				NTCHARString256	pBody(GetTxtFromMgr(10343));

				NTCHARString256 strMoney;
				g_JYLibFunction.SetNumUnit(siNeedMoney, strMoney, 256);

				pBody.Replace("#money#", strMoney);
				pBody.Replace("#time#", SI32ToString(((GetTimePassed()+1000)/1000)));

				stMsgBox MsgBox;
				MsgBox.Set((CControllerMgr*)pClient->m_pDialog[NGAWIBAWIBO_DLG], pTitle, pBody, MSGBOX_TYPE_YESNO, 0);
				pClient->SetMsgBox(&MsgBox, NULL, 0);

				//((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->Counting(GetTimePassed()+1000);
			}
		}
		break;

	case GAWIBAWIBO_DO_CHARGEDMONEY:
		{
			((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->EndCount();

			//	게임플레이가 가능한 플레이어 이므로 금액 차감
			cltGameRequest_GawiBawiBo_ChargedMoney ChargedMoney;
			cltMsg clMsg(GAMEMSG_REQUEST_GAWIBAWIBO_CHARGED_MONEY, sizeof(cltGameRequest_GawiBawiBo_ChargedMoney), (BYTE*)&ChargedMoney);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			
			NextSteps();
		}
		break;

	case GAWIBAWIBO_NOW_CHARGINGMONEY:	// 서버에서 금액 차감중..
		break;

	case GAWIBAWIBO_WAIT_SELECT:
		{
			//	상태를 가위바위보 선택 대기 상태로
			((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->WaitForSelect();

			NextSteps();
			SetTimer(GAWIBAWIBO_MIN_TIME);
		}
		break;

	case GAWIBAWIBO_NOW_WAIT_SELECT:	//	플레이어가 패를 고를떄까지 기다리는중...		
		{
			// 시간내에 패를 고르지않는다면 랜덤으로 패를 선택하게 함.

			// time out
			if(IsTimePassed() == true)
			{
				// 랜덤으로 선택
				SI08 siRand = rand() % 3;

				((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->RandomSelect(siRand+1);	// 가위바위보 버튼 인덱스는 1~3 
				((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->EndCount();	// 시간내에 결정을 못했으므로 기권처리
			}
			else
			{
				// 패고르는 중....
				((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->Counting(GetTimePassed()+1000);
			}
		}
		break;

	case GAWIBAWIBO_SELECT:
		{
			//	플레이어가 패를 선택했다. 승률 연산을 거쳐 결과를 산출

			((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->EndCount();

			cltGameRequest_GawiBawiBo_CalculateResult CalculateResult;
			cltMsg clMsg(GAMEMSG_REQUEST_GAWIBAWIBO_CALCULATE_RESULT, sizeof(cltGameRequest_GawiBawiBo_CalculateResult), (BYTE*)&CalculateResult);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

			NextSteps();
		}
		break;

	case GAWIBAWIBO_NOW_CALCULATE_SELECT:	//	서버로부터 가위바위보 결과 기다리는중...
		break;

	case GAWIBAWIBO_CALCULATE_SELECT_COMPLETE:
		{
			//	결과 선택이 완료 됐다.
			
			// 선택한 결과를 보여준다.
			((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->SeleceComplete();
			
			SetTimer(GAWIBAWIBO_NEXTSTEP_TIME);
			NextSteps();	
		}
		break;

	case GAWIBAWIBO_SHOW_CALCULATE_SELECT:
		{
			//	설정된 시간동안 선택한 결과 보여주는 중
			if(IsTimePassed() == true)
				NextSteps();

			//	NPC의 부정행위로 인한 승리!
			if(m_siResult == GAWIBAWIBO_RESULT_ABUSE_WIN)
			{
				TCHAR*	pTitle	= GetTxtFromMgr(40055);
				TCHAR*	pBody	= GetTxtFromMgr(40056);
				pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);
			}
		}
		break;
	
	case GAWIBAWIBO_RESULT:
		{
			//	결과 산출이 완료 됐다. 결과에 따른 애니메이션을 보여준 후 연속해서 게임을 할지 체크
			//	3초 후 다음 스텝
			((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->ResultComplete();

			SetTimer(GAWIBAWIBO_MIN_TIME);
			NextSteps();
		}

	case GAWIBAWIBO_SHOW_CEREMONY:
		{
			//	결과에 따라 애니메이션 중~
			if(IsTimePassed() == true)
			{
				NextSteps();
				pClient->m_pDialog[NOTIFYMSG_DLG]->Show(false);

				// 패배한 경우에는 애니메이션을 유지
				if(m_siResult != GAWIBAWIBO_RESULT_LOSE)	
					((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->WaitForSelect();
			}
		}
		break;

	case GAWIBAWIBO_RESULT_PROCESS:
		{
			//	결과에 따라 제시
			switch(m_siResult)
			{
			case GAWIBAWIBO_RESULT_WIN:
			case GAWIBAWIBO_RESULT_ABUSE_WIN:
				{
					// 승리~
					// 만약 최고연승을 했다면 바로 축하메세지와 함께 종료
					if(GetMaximumWins() == true)
					{
						m_siCumulativeMoney = GetCalculateCumulativeMoney(m_siGawiBawiBoCount);	
						GiveUp();
					}
					else
					{
						//	다음판에 받을 누적금액을 보여줍니다.
						m_siCumulativeMoney = GetCalculateCumulativeMoney(m_siGawiBawiBoCount);	

						((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->SetCumulativeMoneyText(m_siCumulativeMoney);

						TCHAR* pTitle	= GetTxtFromMgr(40055);
						NTCHARString256	pBody(GetTxtFromMgr(40053));

						NTCHARString256 strMoney;
						g_JYLibFunction.SetNumUnit(GetCalculateCumulativeMoney(m_siGawiBawiBoCount+1), strMoney, 256);

						pBody.Replace("#money#", strMoney);

						stMsgBox MsgBox;
						MsgBox.Set((CControllerMgr*)pClient->m_pDialog[NGAWIBAWIBO_DLG], pTitle, pBody, MSGBOX_TYPE_YESNO, 2);
						pClient->SetMsgBox(&MsgBox, NULL, 0);

						SetTimer(GAWIBAWIBO_MAX_TIME);

						NextSteps();
					}
					//NextSteps();
				}
				break;

			case GAWIBAWIBO_RESULT_DRAW:
				{
					// 비긴 경우 재 시합
					DrawGame();					
				}
				break;

			case GAWIBAWIBO_RESULT_LOSE:
				{
					//	가위바위보에서 졌다.
					//((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->DeleteDialog();

					TCHAR* pTitle	= GetTxtFromMgr(40050);
					NTCHARString256	pBody(GetTxtFromMgr(40052));

					stMsgBox MsgBox;
					MsgBox.Set((CControllerMgr*)pClient->m_pDialog[NGAWIBAWIBO_DLG], pTitle, pBody, MSGBOX_TYPE_OK, 3);
					pClient->SetMsgBox(&MsgBox, NULL, 0);
				}
				break;

			default:
				assert(0);
				break;
			}
		}
		break;

	case GAWIBAWIBO_NOW_RESULT_PROCESS:
		{
			//	현재 유저가 더 게임을 할지 누적금액 받아갈지 선택중...
			
			// 타임아웃
			if(IsTimePassed() == true)
			{
				((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->EndCount();	// 시간내에 결정을 못했으므로 기권처리
				GiveUp();
			}
			else
			{
				// 게임을 계속 할지 결정 대기 중이니 카운트

				TCHAR* pTitle	= GetTxtFromMgr(40055);
				NTCHARString256	pBody(GetTxtFromMgr(40053));

				NTCHARString256 strMoney;
				g_JYLibFunction.SetNumUnit(GetCalculateCumulativeMoney(m_siGawiBawiBoCount+1), strMoney, 256);

				pBody.Replace("#money#", strMoney);
				pBody.Replace("#time#", SI32ToString(((GetTimePassed()+1000)/1000)));

				stMsgBox MsgBox;
				MsgBox.Set((CControllerMgr*)pClient->m_pDialog[NGAWIBAWIBO_DLG], pTitle, pBody, MSGBOX_TYPE_YESNO, 2);
				pClient->SetMsgBox(&MsgBox, NULL, 0);

				//((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->Counting(GetTimePassed()+1000);
			}
		}
		break;

	default:
		assert(0);
		break;
	}
}

//  [8/11/2009 종호_GAWIBAWIBO] 상태값 설정(이 상태값으로 보여줄 스텝들을 제어 합니다.)
void CGawiBawiBoMgr_Client::SetStatus(SI16 siStatus)
{
	if(siStatus < GAWIBAWIBO_NONE || siStatus >= GAWIBAWIBO_END)
	{
		assert(0);
		return;
	}

	m_siStatus = siStatus;
}

SI08 CGawiBawiBoMgr_Client::GetCount()
{
	if(m_siGawiBawiBoCount < 0 || m_siGawiBawiBoCount >= (SI08)m_vecStraightWins.size())
	{
		assert(0);
		return -1;
	}
	else
		return m_siGawiBawiBoCount;
}

// 넘어온 인자값이 연승수. 해당 연승시 받을 수있는 누적금액을  구한다.
GMONEY CGawiBawiBoMgr_Client::GetCalculateCumulativeMoney(SI08 siCount)						//	누적금액 계산
{
	GMONEY siMoneyCumulativeMoney = 0;

	//	siCount 연승을 했을시 받을 금액을 리턴
	SI32	siNpcrate	= pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
	GMONEY	siUnitMoney	= siNpcrate * 200;	//	 비용은 시세 * 10			

	////	 siIndex의 초기값을 1로 설정한 이유는 리소스상에서 1승의 index 가 1이다.
	//for(SI08 siIndex = 1; siIndex <= siCount; siIndex++)
	//{
	//	if(siIndex == 1)	// 아직 누적금액이 없는 경우.
	//		siMoneyCumulativeMoney = (siUnitMoney * m_vecStraightWins.at(siIndex)) / 100;	
	//	else
	//		siMoneyCumulativeMoney = (siMoneyCumulativeMoney * m_vecStraightWins.at(siIndex)) / 100;
	//}

	// 보상금액 산출방식 변경
	siMoneyCumulativeMoney = (siUnitMoney * m_vecStraightWins.at(siCount)) / 100;	

	return siMoneyCumulativeMoney;
}

void CGawiBawiBoMgr_Client::SetResult(SI08 siResult)		// 가위바위보 승패 결과
{
	if(siResult <= GAWIBAWIBO_RESULT_NONE || siResult >= GAWIBAWIBO_RESULT_END)
	{
		assert(0);
		return;
	}

	m_siResult = siResult;
}

void CGawiBawiBoMgr_Client::InitTimer()
{
	m_Timer = pclClient->CurrentClock;
}

void CGawiBawiBoMgr_Client::SetTimer(clock_t lnTime)
{
	m_Timer = pclClient->CurrentClock + lnTime;
}

clock_t CGawiBawiBoMgr_Client::GetTimePassed()
{
	return m_Timer - pclClient->CurrentClock;
}

bool CGawiBawiBoMgr_Client::IsTimePassed()
{
	if(m_Timer <= pclClient->CurrentClock)
		return true;
	else
		return false;
}

//	기권
void CGawiBawiBoMgr_Client::GiveUp()
{
	// 보상
	if(m_siGawiBawiBoCount <= 0)
		EndGame();
	else
	{
		cltGameRequest_GawiBawiBo_Reward Reward(m_siCumulativeMoney);
		cltMsg clMsg(GAMEMSG_REQUEST_GAWIBAWIBO_REWARD, sizeof(cltGameRequest_GawiBawiBo_Reward), (BYTE*)&Reward);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

		SetStatus(GAWIBAWIBO_WAIT_PLAYER);
	}


	cltClient* pClient = (cltClient*)pclClient;
	((NGawiBawiBoDlg*)pClient->m_pDialog[NGAWIBAWIBO_DLG])->DeleteDialog();

	if((CControllerMgr*)pClient->m_pDialog[NMSGBOX_DLG] != NULL)
		((CControllerMgr*)pClient->m_pDialog[NMSGBOX_DLG])->DeleteDialog();
}
