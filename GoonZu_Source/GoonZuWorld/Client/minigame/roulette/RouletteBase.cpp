#include "../Client/Client.h"
#include "../CommonLogic/CommonLogic.h"
#include "../Client/Interface/MiniGameDlg/RouletteGameDlg/RouletteGameDlg.h"
#include "../Client/MiniGame/MiniGameManager.h"
#include "../Client/MiniGame/Roulette/RouletteLogic.h"
#include "../Client/MiniGame/Roulette/RouletteDraw.h"
#include "../Client/InterfaceMgr/Interface/Button.h"

#include "../CommonLogic/MsgType-Item.h"
#include "../CommonLogic/MsgType.h"
#include "../Client/Music/music.h"

extern cltCommonLogic* pclClient;

CRouletteBase::CRouletteBase()
{
	m_pRouletteLogic = new CRouletteLogic;
	m_pRouletteDraw = new CRouletteDraw;

	m_pRouletteDraw->LoadImage();

	Init();
}

CRouletteBase::~CRouletteBase()
{
	if(NULL != m_pRouletteDraw)
	{
		delete m_pRouletteDraw;
		m_pRouletteDraw = NULL;
	}

	if(NULL != m_pRouletteLogic)
	{
		delete m_pRouletteLogic;
		m_pRouletteLogic = NULL;
	}
}

bool CRouletteBase::Init()
{
	for(SI32 i=0; i<5; ++i)
	{
		m_siRandomX[i] = -1;
		m_siRandomY[i] = -1;
		m_bSelect[i] = false;
	}

	m_dwTotalClock = 30000;
	m_dwCheckClock = 0;
	m_dwStartClock = 0;
	m_dwLastClock  = 0;
	m_dwNextClock  = 0;

	m_siDelay	   = 100;
	m_siBirdX	   = 0;

	m_bStart	   = false;
	m_siNumber	   = 0;

	GAME_STATE = INIT_ROULETTE;

	return true;
}

void CRouletteBase::Action()
{
	switch(GAME_STATE)
	{
	case INIT_ROULETTE:
		RunInitRoulette();
		break;

	case INPUT_ROULETTE:
		RunInputRoulette();
		break;

	case START_ROULETTE:
		RunStartRoulette();
		break;

	case CHECK_ROULETTE:
		RunCheckRoulette();
		break;

	case END_ROULETTE:
		RunEndRoulette();
		break;
	}
}

void CRouletteBase::KeyAction(SI32 x, SI32 y, SI32 type)
{
	if( INPUT_ROULETTE == GAME_STATE)
	{
		switch(type)
		{
		case 0:
			m_pRouletteLogic->SetMouseXY(x, y);
			break;
		case 1:

			if(0 < m_pRouletteLogic->GetBettingCount())
			{
				m_pRouletteLogic->StartRoulette();
				m_dwCheckClock = pclClient->CurrentClock;
			//	m_siNumber = m_siNumber - m_pRouletteLogic->GetBettingCount();
				GAME_STATE = START_ROULETTE;

				cltClient *pclclient = (cltClient *)pclClient;
				((CRouletteGameDlg*)pclclient->m_pDialog[ ROULETTEGAME_DLG ])->m_pButton->Enable(false);
			}
			break;
		}
	}
}

SI32 CRouletteBase::GetResult()
{
	return 1;
}

void CRouletteBase::SetResult(SI16 randomvalue[], SI16 result, SI16 number)
{
	m_pRouletteLogic->SetRandomValue(randomvalue);
//	m_siNumber = number;
}

void CRouletteBase::RunInitRoulette()
{
	Init();
	m_pRouletteLogic->Init();
	m_siNumber = m_pRouletteLogic->GetGourdNumber();

	cltClient *pclclient = (cltClient *)pclClient;
	((CRouletteGameDlg*)pclclient->m_pDialog[ ROULETTEGAME_DLG ])->m_pButton->Enable(true);

	GAME_STATE = INPUT_ROULETTE;
}

void CRouletteBase::RunInputRoulette()
{
}

void CRouletteBase::RunStartRoulette()
{
	if( m_pRouletteLogic->GetStart() )
	{
		if(TABS(pclClient->CurrentClock - m_dwCheckClock) < 30000)
		{
			if(TABS(pclClient->CurrentClock - m_dwCheckClock) < 5000)
			{
				m_siDelay = 50;
			}
			else if(TABS(pclClient->CurrentClock - m_dwCheckClock) >= 5000 && TABS(pclClient->CurrentClock - m_dwCheckClock) < 10000)
			{
				m_siDelay = 100;
			}
			else if(TABS(pclClient->CurrentClock - m_dwCheckClock) >= 10000 && TABS(pclClient->CurrentClock - m_dwCheckClock) < 15000)
			{
				m_siDelay = 200;
			}
			else if(TABS(pclClient->CurrentClock - m_dwCheckClock) >= 15000 && TABS(pclClient->CurrentClock - m_dwCheckClock) < 20000)
			{
				m_siDelay = 500;
			}
			else if(TABS(pclClient->CurrentClock - m_dwCheckClock) >= 20000 && TABS(pclClient->CurrentClock - m_dwCheckClock) < 30000)
			{
				m_siDelay = 1000;
			}

			m_dwStartClock = pclClient->CurrentClock;
			if(TABS(m_dwStartClock - m_dwLastClock) > m_siDelay)
			{
				m_siRandomX[0] = rand()%5;
				m_siRandomY[0] = rand()%5;

				bool loop = true;
				do 
				{
					m_siRandomX[1] = rand()%5;
					m_siRandomY[1] = rand()%5;

					if((m_siRandomX[0] != m_siRandomX[1] && m_siRandomY[0] != m_siRandomY[1]) == true)
						loop = false;
				} while( loop );

				loop = true;
				do 
				{
					m_siRandomX[2] = rand()%5;
					m_siRandomY[2] = rand()%5;

					if((m_siRandomX[0] != m_siRandomX[2] && m_siRandomY[0] != m_siRandomY[2]) == true)
					{
						if((m_siRandomX[1] != m_siRandomX[2] && m_siRandomY[1] != m_siRandomY[2]) == true)
							loop = false;
					}

				} while( loop );

				loop = true;
				do 
				{
					m_siRandomX[3] = rand()%5;
					m_siRandomY[3] = rand()%5;

					if((m_siRandomX[0] != m_siRandomX[3] && m_siRandomY[0] != m_siRandomY[3]) == true)
					{
						if((m_siRandomX[1] != m_siRandomX[3] && m_siRandomY[1] != m_siRandomY[3]) == true)
						{
							if((m_siRandomX[2] != m_siRandomX[3] && m_siRandomY[2] != m_siRandomY[3]) == true)
								loop = false;
						}
					}

				} while( loop );

				loop = true;
				do 
				{
					m_siRandomX[4] = rand()%5;
					m_siRandomY[4] = rand()%5;

					if((m_siRandomX[0] != m_siRandomX[4] && m_siRandomY[0] != m_siRandomY[4]) == true)
					{
						if((m_siRandomX[1] != m_siRandomX[4] && m_siRandomY[1] != m_siRandomY[4]) == true)
						{
							if((m_siRandomX[2] != m_siRandomX[4] && m_siRandomY[2] != m_siRandomY[4]) == true)
							{
								if((m_siRandomX[3] != m_siRandomX[4] && m_siRandomY[3] != m_siRandomY[4]) == true)
									loop = false;
							}
						}
					}

				} while( loop );


				m_dwLastClock = pclClient->CurrentClock;
			}
		}
		else if(TABS(pclClient->CurrentClock - m_dwCheckClock) >= 30000 && TABS(pclClient->CurrentClock - m_dwCheckClock) < 31000)
		{
			SI16 tempValue = m_pRouletteLogic->GetRandom(0);
			m_siRandomX[0] = (tempValue - 1) % 5;
			m_siRandomY[0] = (tempValue - 1) / 5;
			m_bSelect[0] = true;
		}
		else if(TABS(pclClient->CurrentClock - m_dwCheckClock) >= 31000 && TABS(pclClient->CurrentClock - m_dwCheckClock) < 32000)
		{
			SI16 tempValue = m_pRouletteLogic->GetRandom(1);
			m_siRandomX[1] = (tempValue - 1) % 5;
			m_siRandomY[1] = (tempValue - 1) / 5;
			m_bSelect[1] = true;
		}
		else if(TABS(pclClient->CurrentClock - m_dwCheckClock) >= 32000 && TABS(pclClient->CurrentClock - m_dwCheckClock) < 33000)
		{
			SI16 tempValue = m_pRouletteLogic->GetRandom(2);
			m_siRandomX[2] = (tempValue - 1) % 5;
			m_siRandomY[2] = (tempValue - 1) / 5;
			m_bSelect[2] = true;
		}
		else if(TABS(pclClient->CurrentClock - m_dwCheckClock) >= 33000 && TABS(pclClient->CurrentClock - m_dwCheckClock) < 34000)
		{
			SI16 tempValue = m_pRouletteLogic->GetRandom(3);
			m_siRandomX[3] = (tempValue - 1) % 5;
			m_siRandomY[3] = (tempValue - 1) / 5;
			m_bSelect[3] = true;
		}
		else if(TABS(pclClient->CurrentClock - m_dwCheckClock) >= 34000 && TABS(pclClient->CurrentClock - m_dwCheckClock) < 35000)
		{
			SI16 tempValue = m_pRouletteLogic->GetRandom(4);
			m_siRandomX[4] = (tempValue - 1) % 5;
			m_siRandomY[4] = (tempValue - 1) / 5;
			m_bSelect[4] = true;
		}
		else if(TABS(pclClient->CurrentClock - m_dwCheckClock) >= 35000 && TABS(pclClient->CurrentClock - m_dwCheckClock) < 36000)
		{
			m_pRouletteLogic->SetStart(false);
			m_dwNextClock = pclClient->CurrentClock;
			m_pRouletteLogic->CheckRoulette();
			m_siNumber = m_pRouletteLogic->GetGourdNumber() ;
			m_pRouletteLogic->SetBettingCount(0);
			GAME_STATE = CHECK_ROULETTE;
		}

	}
}

void CRouletteBase::RunCheckRoulette()
{
	if(TABS(pclClient->CurrentClock - m_dwNextClock) > 5000)
	{
		GAME_STATE = END_ROULETTE;
	}
	
}

void CRouletteBase::RunEndRoulette()
{
	m_siBirdX += 15;
	if(m_siBirdX > 1000)
	{
		m_siBirdX = 1000;
		GAME_STATE = INIT_ROULETTE;
	}
}

void CRouletteBase::PlaySound(cltCommonLogic* pclClient, int Type)
{
	pclClient->PushEffect(Type, 0, 0);
}