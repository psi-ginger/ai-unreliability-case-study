#include "CommonHeader.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"

#include "../NLib/NTCHARString.h"

#include "../../Client/InterfaceMgr/InterfaceMgr.h"

#include "../Client/ninterface/NInstantDungeonDlg/InstantDungeonRunInfoDlg.h"
#include "../Client/MarkManager/MarkManager.h"

#include "list.h"

extern cltCommonLogic* pclClient;


InstanstDungeonRunInfoDlg::InstanstDungeonRunInfoDlg()
{
	if ( false == m_Spr[DRAW_MODE_GUILDMAP_TIME].LoadTSprFromFile("gimg/DungeonTime.SPR") )
	{
		MsgBox("파일열기 실패", "DungeonTime.SPR");
	}
	if ( false == m_Spr[DRAW_MODE_GUILDMAP_SCORE].LoadTSprFromFile("gimg/Score.SPR") )
	{
		MsgBox("파일열기 실패", "Score.SPR");
	}
	if ( false == m_Spr[DRAW_MODE_GUILDMAP_BEST_MARK].LoadTSprFromFile("gimg/BestScore.SPR") )
	{
		MsgBox("파일열기 실패", "BestScore.SPR");
	}
	if ( false == m_Spr[DRAW_MODE_PARTYMAP_BOARD].LoadTSprFromFile("gimg/skullboard.SPR") )
	{
		MsgBox("파일열기 실패", "skullboard.SPR");
	}
	if ( false == m_Spr[DRAW_MODE_PARTYMAP_TIME].LoadTSprFromFile("gimg/piratetimenum.SPR") )
	{
		MsgBox("파일열기 실패", "piratetimenum.SPR");
	}
	if ( false == m_Spr[DRAW_MODE_PARTYMAP_KILLCOUNT].LoadTSprFromFile("gimg/piratekillunm.SPR") )
	{
		MsgBox("파일열기 실패", "piratekillunm.SPR");
	}

	m_dwTime			= (30 * 60 * 1000);
	m_dwStartTick		= 0;

	m_siMapType			= 0;
	m_siBestScore		= 0;
	m_siScore			= 0;
}

InstanstDungeonRunInfoDlg::~InstanstDungeonRunInfoDlg()
{

}

void InstanstDungeonRunInfoDlg::Destroy()
{

}

void InstanstDungeonRunInfoDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/NInstantDungeonDlg/InstantDungeonRunInfoDlg.ddf"));

	SI32 controlindex = file.GetControlIndex( TEXT("dialog_DungeonRunInfo") );
	file.m_control[ controlindex ].dialog.bClosable = false;

	file.CreatePopupDialog( this, NINSTANST_DUNGEON_RUN_INFO_DLG, TEXT("dialog_DungeonRunInfo"), StaticInstanstDungeonRunInfoDlgProc);


	// 이것은 보여주는 창인데 요것땜시 배경이 클릭이 안되면 이상하니까.
	SetHeight(0);
	SetWidth(0);

	SetX(375);
	SetY(60);
}


void CALLBACK InstanstDungeonRunInfoDlg::StaticInstanstDungeonRunInfoDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	InstanstDungeonRunInfoDlg *pThis = (InstanstDungeonRunInfoDlg*) pControllerMgr;
	pThis->NInstanstDungeonRunInfoDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK InstanstDungeonRunInfoDlg::NInstanstDungeonRunInfoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
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


void InstanstDungeonRunInfoDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	if(false == RetValue)
	{
		return;
	}

	switch( MsgIndex )
	{
		default:	break;
	}
}

void InstanstDungeonRunInfoDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	switch (m_siMapType) 
	{
		case INSTANCEMAP_TYPE_GUILD:
			{
				DrawSpr(600 + ScreenRX, 60  + ScreenRY,  0,				DRAW_MODE_GUILDMAP_TIME);

				DrawSpr(630 + ScreenRX, 190 + ScreenRY,  m_siScore,		DRAW_MODE_GUILDMAP_SCORE);
				DrawSpr(440 + ScreenRX, 189 + ScreenRY,  0,				DRAW_MODE_GUILDMAP_GUILD_MARK);

				DrawSpr(630 + ScreenRX, 150 + ScreenRY,  m_siBestScore,	DRAW_MODE_GUILDMAP_SCORE);
				DrawSpr(440 + ScreenRX, 149 + ScreenRY,  0,				DRAW_MODE_GUILDMAP_BEST_MARK);
			}
			break;

		case INSTANCEMAP_TYPE_PARTY:
			{
				DrawSpr( (470 + ScreenRX), (60	+ ScreenRY), 0, DRAW_MODE_PARTYMAP_BOARD);
				DrawSpr( (550 + ScreenRX), (115	+ ScreenRY), 0, DRAW_MODE_PARTYMAP_TIME);
				DrawSpr( (535 + ScreenRX), (80 + ScreenRY), m_siScore, DRAW_MODE_PARTYMAP_KILLCOUNT );
			}
			break;

	}
}

#define SEMICOLON	10
void InstanstDungeonRunInfoDlg::DrawSpr( SI32 _ScreenX, SI32 _ScreenY, SI32 _Figure, SI32 _SprType )
{
	switch(_SprType)
	{
		case DRAW_MODE_GUILDMAP_TIME:	
			{
				DWORD dwTickCnt = GetTickCount();

				if(0 >= m_dwTime || 3600000 < m_dwTime)
				{
					m_dwTime = 0;
				}
				else if(dwTickCnt - m_dwStartTick > 1000)
				{
					m_dwTime	  -= dwTickCnt - m_dwStartTick;
					m_dwStartTick  = dwTickCnt;
				}

				SI32 Minute =   m_dwTime / 60000;
				SI32 Second = ( m_dwTime % 60000 ) / 1000;
				
				SI16 Num = 0;
				int  i   = 0;
				
				// 초.
				Num     = Second % 10;
				Second /= 10;
				GP.PutSpr(&m_Spr[_SprType], _ScreenX, _ScreenY, Num);

				Num     = Second % 10;
				Second /= 10;
				GP.PutSpr(&m_Spr[_SprType], _ScreenX - 36, _ScreenY, Num);
				
				// :
				SI16 Space = 72;
				GP.PutSpr(&m_Spr[_SprType], _ScreenX - (Space), _ScreenY, SEMICOLON); ++i;

				// 분.
				Num     = Minute%10;
				Minute /= 10;
				GP.PutSpr(&m_Spr[_SprType], _ScreenX - Space - 38, _ScreenY, Num);

				Num     = Minute%10;
				Minute /= 10;
				GP.PutSpr(&m_Spr[_SprType], _ScreenX - Space - 38 - 36, _ScreenY, Num);
			}
			break;

			
		case DRAW_MODE_GUILDMAP_BEST_MARK:	
			{
				//GP.PutSpr(&m_Spr[SCORE], _ScreenX + 25, _ScreenY, SEMICOLON);
				GP.PutSpr(&m_Spr[_SprType], _ScreenX, _ScreenY, 0);
			}
			break;

		case DRAW_MODE_GUILDMAP_GUILD_MARK:	
			{
				SI32 GuildMark = pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siMarkIndex;
				SI32  guildicon  = -1 ;
				TSpr* guildspr	= NULL;
				
				cltClient* pclclient = (cltClient*)pclClient;
				pclclient->m_pMarkManager->GetMarkSprAndFontIndex( GuildMark, &guildspr, &guildicon );
				
				if(-1 == guildicon)
				{
					break;
				}
								
				//GP.PutSpr(&m_Spr[SCORE], _ScreenX + 25, _ScreenY, SEMICOLON);
				GP.PutSpr(guildspr, _ScreenX, _ScreenY, guildicon);
			}
			break;

		case DRAW_MODE_GUILDMAP_SCORE:
			{
				// 점수 표시.
				if(0 == _Figure)
				{
					GP.PutSpr(&m_Spr[_SprType], _ScreenX, _ScreenY, 0);
				}
				else
				{
					SI16 RemainNum = 0;

					for(int i = 0; 0 != _Figure; ++i)
					{
						RemainNum  = _Figure % 10;
						_Figure   /= 10;
						GP.PutSpr(&m_Spr[_SprType], _ScreenX - (i * 24), _ScreenY, RemainNum);
					}
				}
			}
			break;
		
		case DRAW_MODE_PARTYMAP_BOARD:
			{
				GP.PutSpr(&m_Spr[_SprType], _ScreenX, _ScreenY, 0);
			}
			break;

		case DRAW_MODE_PARTYMAP_TIME:
			{
				DWORD dwTickCnt		= GetTickCount();


				if(0 >= m_dwTime || (DWORD)((INSTANCEMAP_PIRATESHIP_MAX_TIME) + ((INSTANCEMAP_PIRATESHIP_BONUSTIME) * MAX_INSTANCE_BONUS_MONSTER_NUMBER)) < m_dwTime)
				{
					m_dwTime = 0;
				}
				else if(dwTickCnt - m_dwStartTick > 1000)
				{
					m_dwTime	  -= dwTickCnt - m_dwStartTick;
					m_dwStartTick  = dwTickCnt;
				}

				SI32 Minute =   m_dwTime / 60000;
				SI32 Second = ( m_dwTime % 60000 ) / 1000;

				SI16 Num	= 0;
				SI16 Space	= 30;
				int  i		= 0;

				// 초.
				Num     = Second % 10;
				Second /= 10;
				GP.PutSpr(&m_Spr[_SprType], _ScreenX, _ScreenY, Num);

				Num     = Second % 10;
				Second /= 10;
				GP.PutSpr(&m_Spr[_SprType], _ScreenX - 14, _ScreenY, Num);

				// 분.
				Num     = Minute%10;
				Minute /= 10;
				GP.PutSpr(&m_Spr[_SprType], _ScreenX - Space - 20, _ScreenY, Num);

				Num     = Minute%10;
				Minute /= 10;
				GP.PutSpr(&m_Spr[_SprType], _ScreenX - Space - 20 - 14, _ScreenY, Num);


			}
			break;

		case DRAW_MODE_PARTYMAP_KILLCOUNT:
			{
				// 점수 표시.
				if(0 == _Figure)
				{
					GP.PutSpr(&m_Spr[_SprType], _ScreenX, _ScreenY, 0);
				}
				else
				{
					SI16 RemainNum = 0;

					for(int i = 0; 0 != _Figure; ++i)
					{
						RemainNum  = _Figure % 10;
						_Figure   /= 10;
						GP.PutSpr(&m_Spr[_SprType], _ScreenX - (i * 21), _ScreenY, RemainNum);
					}
				}

			}
			break;
	}
}

void InstanstDungeonRunInfoDlg::Init(cltGameMsgResponse_cInstantsDungeonRunInfo* _RunInfo)
{
	switch ( _RunInfo->siMapType ) 
	{
		case INSTANCEMAP_TYPE_GUILD:
			{
				// 인던 종료 시 정보를 보려줌.
				if( _RunInfo->siMode == DUNGEONRUN_END )
				{	
					cltClient* pclclient = (cltClient *)pclClient;

					TCHAR*  pTitle = GetTxtFromMgr(5614);

					NTCHARString256	kBuf(GetTxtFromMgr(7844));
					kBuf.Replace("#Score#", SI32ToString(_RunInfo->siScore));

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kBuf);

					return;
				}

				if( _RunInfo->siMode == DUNGEONRUN_START )
				{	
					m_dwStartTick	= GetTickCount();
					m_dwTime		= (30 * 60 * 1000) - _RunInfo->dwTime;
					m_siMapType		= _RunInfo->siMapType;
				}

				m_siScore		= _RunInfo->siScore;
				m_siBestScore	= _RunInfo->siBestScore;
		
			}
			break;

		case INSTANCEMAP_TYPE_PARTY:
			{
				switch (_RunInfo->siMode)
				{
					case DUNGEONRUN_START:
						{
							m_dwStartTick	= GetTickCount();
							m_dwTime		= (INSTANCEMAP_PIRATESHIP_MAX_TIME) - _RunInfo->dwTime;
							m_siMapType		= _RunInfo->siMapType;

							m_siBestScore	= 0; // 파티맵에서는 BestScore가 보너스 시간이다
						}
						break;

					case DUNGEONRUN_UPDATE:
						{
							// 보너스 시간
							if ( m_siBestScore < _RunInfo->siBestScore )
							{
								m_dwTime		+= ( (INSTANCEMAP_PIRATESHIP_BONUSTIME) * (_RunInfo->siBestScore-m_siBestScore) );
								m_siBestScore	= _RunInfo->siBestScore;
							}
						}
						break;
				}
				
				// 현재 몬스터 카운터
				m_siScore = _RunInfo->siScore;

			}
			break;
	}

}