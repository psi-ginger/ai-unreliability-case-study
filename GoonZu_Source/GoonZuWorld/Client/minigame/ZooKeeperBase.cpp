#include "../Client/Client.h"
#include "../CommonLogic/CommonLogic.h"
#include "../Client/Interface/MiniGameDlg/MiniGameDlg.h"
#include "../Client/MiniGame/MiniGameManager.h"
#include "../Client/MiniGame/ZooKeeperLogic.h"
#include "../Client/MiniGame/ZooKeeperDraw.h"

#include "../../CommonLogic/MsgType-Item.h"
#include "../../CommonLogic/MsgType.h"
#include "../Client/Music/music.h"
extern cltCommonLogic* pclClient;

CZooKeeperBase::CZooKeeperBase() : CMiniGameManager()
{
	m_ZooKeeperLogic = new CZooKeeper;
	m_ZooDraw		 = new CZooDraw;

	LoadSpr();
	m_ZooKeeperLogic->Init();
	GAME_STATE = INIT_BLOCK;
}

CZooKeeperBase::~CZooKeeperBase()
{
	if(NULL != m_ZooKeeperLogic)
	{
		delete m_ZooKeeperLogic;
		m_ZooKeeperLogic = NULL;
	}
	if(NULL != m_ZooDraw)
	{
		delete m_ZooDraw;
		m_ZooDraw = NULL;
	}

//	cltClient* pclclient = (cltClient*)pclClient;
//	pclclient->pclMusic->pclSoundManager->BackGroundSoundStop();
//	SI32 mapindex = pclclient->pclMapManager->pclCurrentMap->siMapIndex;
//	pclclient->PlayBackgroundMusic(mapindex);
}

bool CZooKeeperBase::LoadSpr()
{
	if(!m_ZooDraw->LoadImage())
		return false;

	return true;
}

void CZooKeeperBase::FreeSpr()
{

}
bool CZooKeeperBase::Init()
{
	m_ZooKeeperLogic->SetBlockTable();
	GAME_STATE = INIT_BLOCK;

	return true;
}

void CZooKeeperBase::KeyAction(SI32 x, SI32 y, SI32 type)
{
	if( MOUSEINPUT_BLOCK == GAME_STATE )
	{

		if(m_ZooKeeperLogic->m_bClickState == FALSE)
		{
        //    pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_CLICK")), 0, 0);
			PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_CLICK")) );
			if(m_ZooKeeperLogic->SetMouseXY(x, y))
				GAME_STATE = SPECIAL_BLOCK;
		}
		else if(m_ZooKeeperLogic->m_bClickState == TRUE)
		{
			if(m_ZooKeeperLogic->SwapBlockCheck(x,y))
				GAME_STATE = INPUTCHECK_BLOCK;
		}
	}
}

void CZooKeeperBase::Action()
{
// 배경음 막자
/*	cltClient* pclclient = (cltClient*)pclClient;

	pclclient->pclMusic->pclSoundManager->BackGroundSoundStop();

	if(m_ZooKeeperLogic->GetGameTime() > 60)
		pclclient->pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/MiniGame/MonsterCorp/MonsterBack1.ogg"), true, false );
	//	PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_BACK1")));
	else if(m_ZooKeeperLogic->GetGameTime() > 20)
		pclclient->pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/MiniGame/MonsterCorp/MonsterBack2.ogg"), true, false );
	else if(m_ZooKeeperLogic->GetGameTime() > 0)
		pclclient->pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/MiniGame/MonsterCorp/MonsterBack3.ogg"), true, false );
*/
	if(INPUTCHECK_BLOCK == GAME_STATE || FALSE_BLOCK == GAME_STATE)
		m_ZooKeeperLogic->MovBlocks2();
	else
		m_ZooKeeperLogic->MovBlocks();

	switch(GAME_STATE)
	{
	case INIT_BLOCK:
		{
			RunInitBlock();
		}
		break;
	case CHECK_BLOCK:
		{
			RunCheckBlock();
		}
		break;

	case MOUSEINPUT_BLOCK:
		{

		}
		break;

	case INPUTCHECK_BLOCK:
		{
			RunInputCheckBlock();
		}
		break;

	case SPECIAL_BLOCK:
		{
			RunSpecialBlock();
		}
		break;
	case FALSE_BLOCK:
		{
			RunFalseBlock();
		}
		break;

	case DOWN_BLOCK:
		{
			RunDownBlock();
		}
		break;
	case EFFECT_BLOCK:
		{
			RunEffectBlock();
		}
		break;
	case GAME_OVER:
		{
			GameOver();
		}
		break;
	}

	if(m_ZooKeeperLogic->GetBlockCombo() == 2)
	{
		PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_SUCCESS1")));
	}
	else if(m_ZooKeeperLogic->GetBlockCombo() == 3)
	{
		PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_SUCCESS2")));
	}
	else if(m_ZooKeeperLogic->GetBlockCombo() >= 4)
	{
		PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_SUCCESS3")) );
	}
	

}

void CZooKeeperBase::RunInitBlock()
{
	SI32 BlockValue = 0;
	SI32 x = 0, y = 0;

	m_ZooKeeperLogic->m_MoveY +=4;
	if(m_ZooKeeperLogic->m_MoveY > TABLE_Y)
	{
		PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_DOWN")) );
		m_ZooKeeperLogic->m_MoveY = TABLE_Y;
		GAME_STATE = CHECK_BLOCK;
	}
}

void CZooKeeperBase::RunCheckBlock()
{
	if(TRUE == m_ZooKeeperLogic->HorizonalCheck())
		m_ZooKeeperLogic->SetHorizonalCheck(TRUE);
	if(TRUE == m_ZooKeeperLogic->VerticalCheck())
		m_ZooKeeperLogic->SetVerticalCheck(TRUE);

	if(TRUE == m_ZooKeeperLogic->GetHorizonalCheck() || TRUE == m_ZooKeeperLogic->GetVerticalCheck())
	{
		m_ZooKeeperLogic->ResetTable();
		if(m_ZooDraw->m_AniBlock.AniProcEnd())
		{
			m_ZooDraw->m_AniBlock.ResetFrame();
			m_ZooKeeperLogic->SetHorizonalCheck(FALSE);
			m_ZooKeeperLogic->SetVerticalCheck(FALSE);
			GAME_STATE = EFFECT_BLOCK; 
		}
	}
	else
	{
		m_ZooKeeperLogic->SetBlockCombo(1); // 블럭 콤보 1으로
		GAME_STATE = MOUSEINPUT_BLOCK;
	}

}

void CZooKeeperBase::RunDownBlock()
{
	m_ZooKeeperLogic->BlockDown();

	PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_DOWN")) );
	if( m_ZooKeeperLogic->IsMovStopAllBlock() )
	{
		m_ZooKeeperLogic->m_Combo++;
        m_ZooKeeperLogic->ResetXYTable();
		GAME_STATE = CHECK_BLOCK;
	}
}
void CZooKeeperBase::RunTrueBlock()
{
	GAME_STATE = CHECK_BLOCK;
}

void CZooKeeperBase::RunInputCheckBlock()
{
	// 마우스 입력을 받는다
	if( m_ZooKeeperLogic->IsMovStopAllBlock() )
	{
		m_ZooKeeperLogic->SwapBlockValue(&m_ZooKeeperLogic->m_FirstClickBlockValue, &m_ZooKeeperLogic->m_SecondClickBlockValue);

		if(TRUE == m_ZooKeeperLogic->HorizonalCheck())
			m_ZooKeeperLogic->SetHorizonalCheck(TRUE);
		if(TRUE == m_ZooKeeperLogic->VerticalCheck())
			m_ZooKeeperLogic->SetVerticalCheck(TRUE);

		if(TRUE == m_ZooKeeperLogic->GetHorizonalCheck() || TRUE == m_ZooKeeperLogic->GetVerticalCheck())
		{
			PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_MOVE")) );
			GAME_STATE = CHECK_BLOCK;
		}
		else
		{
			PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_FAIL")) );
			m_ZooKeeperLogic->SwapBlockValue(&m_ZooKeeperLogic->m_FirstClickBlockValue, &m_ZooKeeperLogic->m_SecondClickBlockValue);
			m_ZooKeeperLogic->InputMoveBlock2();
			m_ZooKeeperLogic->m_BlockTableXY[m_ZooKeeperLogic->m_FirstClickBlockY ][m_ZooKeeperLogic->m_FirstClickBlockX].m_bSelectImage = FALSE;
			GAME_STATE = FALSE_BLOCK;
		}
	}
}
void CZooKeeperBase::RunFalseBlock()
{
	if( m_ZooKeeperLogic->IsMovStopAllBlock() )
	{
		m_ZooKeeperLogic->PenaltyData();
		GAME_STATE = MOUSEINPUT_BLOCK;
	}
}

void CZooKeeperBase::RunSpecialBlock()
{
	PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_SPECIAL")) );

	if(m_ZooDraw->m_AniBlock.AniProcEnd())
	{
		m_ZooDraw->m_AniBlock.ResetFrame();
		//	m_ZooKeeperLogic->SetBlockCombo(0);
		GAME_STATE = EFFECT_BLOCK;
	}
}

void CZooKeeperBase::RunEffectBlock()
{
	if(m_ZooDraw->m_AniEffectBlock.AniProcEnd())
	{
		m_ZooDraw->m_AniEffectBlock.ResetFrame();
		GAME_STATE = DOWN_BLOCK; 
	}
}

void CZooKeeperBase::GameOver()
{
	static float Time=0.0f, Last=0;
	static SI32  Count = 2;

	LARGE_INTEGER m_ticksPerSecond;

	if(!QueryPerformanceFrequency(&m_ticksPerSecond))
	{
		return;
	}

	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	Time = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

	if(Time - Last >= 1)
	{
		Last = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

		Count--;
		if(Count < 0)
		{	
			SI32 score = 0 ;
			if(m_ZooKeeperLogic->CheckScore())
			{
				score = m_ZooKeeperLogic->m_TrueGameScore;
				cltGameMsgRequest_MonsterCorp_Score clinfo(score);
				cltMsg clMsg(GAMEMSG_REQUEST_MONSTERCORP_SCORE,sizeof(clinfo),(BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			else
			{
				cltGameMsgRequest_MonsterCorp_Score clinfo(0);
				cltMsg clMsg(GAMEMSG_REQUEST_MONSTERCORP_SCORE,sizeof(clinfo),(BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}


			Count = 2;
			cltClient *pclclient = (cltClient*)pclClient;
			pclclient->m_pDialog[ MONSTERCORPORATION_DLG ]->DeleteDialog();

			return;
		}
	}
/*	static float Time=0.0f, Last=0;
	static SI32  Count = 2;

	LARGE_INTEGER m_ticksPerSecond;

	if(!QueryPerformanceFrequency(&m_ticksPerSecond))
	{
		return;
	}

	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	Time = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

	if(Time - Last >= 1)
	{
		Last = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

		Count--;
		if(Count < 0)
		{	
			SI32 score = 0 ;
			if(m_ZooKeeperLogic->CheckScore())
			{
				score = m_ZooKeeperLogic->m_TrueGameScore;
			}

			cltGameMsgRequest_MonsterCorp_Score clinfo(score);
			cltMsg clMsg(GAMEMSG_REQUEST_MONSTERCORP_SCORE,sizeof(clinfo),(BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

			Count = 2;
			cltClient *pclclient = (cltClient*)pclClient;
			pclclient->m_pDialog[ MONSTERCORPORATION_DLG ]->DeleteDialog();
		}
	}*/
}
SI32 CZooKeeperBase::GetResult()
{
	if(m_ZooKeeperLogic->CheckScore())
		return m_ZooKeeperLogic->GetTrueGameScore();
	else
		return 0;
}

void CZooKeeperBase::PlaySound(cltCommonLogic* pclClient, int Type)
{
//#ifdef _DEBUG
	pclClient->PushEffect(Type, 0, 0);
//#endif
}
/*
void CZooKeeperBase::BackSound()
{
    cltClient* pclclient = (cltClient*)pclClient;

	pclclient->pclMusic->pclSoundManager->BackGroundSoundStop();

	if(m_ZooKeeperLogic->GetGameTime() > 60)
		pclclient->pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/MiniGame/MonsterCorp/MonsterBack1.ogg"), true, false );
	//	PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_BACK1")));
	else if(m_ZooKeeperLogic->GetGameTime() > 20)
		PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_BACK2")));
	else if(m_ZooKeeperLogic->GetGameTime() > 0)
		PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_BACK3")));

}*/