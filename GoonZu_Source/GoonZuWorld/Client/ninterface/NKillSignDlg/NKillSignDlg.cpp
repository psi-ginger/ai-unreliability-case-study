#include "NKillSignDlg.h"

#include "../../Client.h"
#include "../Client/Gametip/GameTip.h"

extern cltCommonLogic* pclClient;

CNKillSignDlg::CNKillSignDlg() : CDialogController()
{
	for(int i = 0; i < SIGN_END; i++)
	{
		for(int j = 0; j < m_siMaxLine; j++)
		{
			m_pstaticKillSign[i][j]		=	NULL;
		}
	}

	for(int i = 0; i < m_siMaxLine; i++)
	{
		ZeroMemory(m_szUserName[SIGN_KILL][i], MAX_PLAYER_NAME);
		ZeroMemory(m_szUserName[SIGN_DEATH][i], MAX_PLAYER_NAME);
	}

	m_siEndValue	=	0;
}

CNKillSignDlg::~CNKillSignDlg()
{
	for(int i = 0; i < SIGN_END; i++)
	{
		for(int j = 0; j < m_siMaxLine; j++)
			SAFE_DELETE(m_pstaticKillSign[i][j]);
	}
	
	if(m_sprKillSignImg.pImage)
		GP.FreeSpr(m_sprKillSignImg);
}

void CNKillSignDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}
  

	SI16 siStaticWitdh	= 1000;
	SI16 siStaticHeight	= 100;

	SI16 siDialogXpos	= (pclclient->GetScreenXSize() - siStaticWitdh + MAP_TILE_XSIZE) + 300;    
	SI16 siDialogYpos	= 300;

	// 다이얼로그 생성
	CDialogController::Create( NGUILDWARKILLSIGN_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNKillSign, NULL, NULL, 0, false, 0, siDialogXpos, siDialogYpos, 0, 0, false, false, false );
	SetDrawOption( SPR_PUTSPRALPHAT );


	SI16 siFontSize		= 15;
	SI16 siTipFontSize	= 14;
	SI32 siOption	= DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	GP.LoadSprFromFile(TEXT("GIMG\\KillSign.SPR"),&m_sprKillSignImg);

	if( pclClient->IsWhereServiceArea( ConstServiceArea_Korea ) )
		siTipFontSize = 13;


	for(int i = 0; i < SIGN_END; i++)
	{
		for(int j = 0; j < m_siMaxLine; j++)
		{
			NEWCSTATIC(m_pstaticKillSign[i][j]);
			
			switch(i)
			{   
			case SIGN_KILL:
				{
					m_pstaticKillSign[i][j]->Create( 0, NULL, NULL, 0, false, 50, 0, 0, siStaticWitdh - 350, siStaticHeight + (j * 60), siStaticWitdh - 350, siStaticHeight + (j * 60));
					m_pstaticKillSign[i][j]->SetFontSize( siFontSize );
					m_pstaticKillSign[i][j]->SetTextPrintOption( siOption );
					m_pstaticKillSign[i][j]->SetFontWeight(FW_BOLD );
					//m_pstaticKillSign[i][j]->SetFontColor( RGB(255, 75, 0) );
					m_pstaticKillSign[i][j]->SetFontColor(0xff);
					
				}
				break;

			case SIGN_DEATH:
				{
					m_pstaticKillSign[i][j]->Create( 0, NULL, NULL, 0, false, 50, 0, 0, siStaticWitdh, siStaticHeight + (j * 60), siStaticWitdh, siStaticHeight + (j * 60));
					m_pstaticKillSign[i][j]->SetFontSize( siFontSize );
					m_pstaticKillSign[i][j]->SetTextPrintOption( siOption );
					m_pstaticKillSign[i][j]->SetFontWeight(FW_BOLD );
					m_pstaticKillSign[i][j]->SetFontColor(0xff);
				}
				break;
			}
		}
	}

	SetTopMostWindow( true );
	SetActionTimer( 1	 );    
	SetTimerOnOff( true );
}

void CNKillSignDlg::Action(void)
{
	if(CMAPNAME_VARIABLE_ENDTIME >= m_siEndValue)
		m_siEndValue++;	
	else
	{
		m_siEndValue = 0;
		UpdateLine();
	}

	for(int i = 0; i < m_siMaxLine; i++)
	{
		m_pstaticKillSign[SIGN_KILL][i]->SetText(m_szUserName[SIGN_KILL][i]);
		m_pstaticKillSign[SIGN_DEATH][i]->SetText(m_szUserName[SIGN_DEATH][i]);
	}
}



void CNKillSignDlg::DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY )
{
	for(int i = 0; i < m_siMaxLine; i++)
	{
		if(strcmp(m_szUserName[SIGN_KILL][i], TEXT("")))
			GP.PutSpr(	&m_sprKillSignImg, ScreenRX + GetX() + 400, ScreenRY + GetY() + 30 + (i * 30), 0);
	}
}

void CALLBACK CNKillSignDlg::StaticCallBackDialogNKillSign( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNKillSignDlg *pThis = (CNKillSignDlg*)pControllerMgr;
	pThis->CallBackDialogNKillSign( nEvent, nControlID, pControl );

	return;
}

void CALLBACK CNKillSignDlg::CallBackDialogNKillSign( UINT nEvent, int nControlID, CControllerObj* pControl )
{

	return;
}

void CNKillSignDlg::SetKillSign(TCHAR* szKillName, TCHAR* szDeathName)
{	
	//	메세지가 들어왔다면 밑으로 한칸씩~미뤄줄거에요

	//UpdateLine();


	if(strcmp(m_szUserName[SIGN_KILL][4],TEXT("")) == 0)
	{
		strncpy(m_szUserName[SIGN_KILL][4], szKillName, MAX_PLAYER_NAME);
		strncpy(m_szUserName[SIGN_DEATH][4], szDeathName, MAX_PLAYER_NAME);
	}

	else if(strcmp(m_szUserName[SIGN_KILL][3],TEXT("")) == 0)
	{
		strncpy(m_szUserName[SIGN_KILL][3], szKillName, MAX_PLAYER_NAME);
		strncpy(m_szUserName[SIGN_DEATH][3], szDeathName, MAX_PLAYER_NAME);
	}

	else if(strcmp(m_szUserName[SIGN_KILL][2],TEXT("")) == 0)
	{
		strncpy(m_szUserName[SIGN_KILL][2], szKillName, MAX_PLAYER_NAME);
		strncpy(m_szUserName[SIGN_DEATH][2], szDeathName, MAX_PLAYER_NAME);
	}

	else if(strcmp(m_szUserName[SIGN_KILL][1],TEXT("")) == 0)
	{
		strncpy(m_szUserName[SIGN_KILL][1], szKillName, MAX_PLAYER_NAME);
		strncpy(m_szUserName[SIGN_DEATH][1], szDeathName, MAX_PLAYER_NAME);
	}

	else if(strcmp(m_szUserName[SIGN_KILL][0],TEXT("")) == 0)
	{
		strncpy(m_szUserName[SIGN_KILL][0], szKillName, MAX_PLAYER_NAME);
		strncpy(m_szUserName[SIGN_DEATH][0], szDeathName, MAX_PLAYER_NAME);
	}

	else 
	{
		strncpy(m_szUserName[SIGN_KILL][0], m_szUserName[SIGN_KILL][1], MAX_PLAYER_NAME);
		strncpy(m_szUserName[SIGN_DEATH][0], m_szUserName[SIGN_DEATH][1], MAX_PLAYER_NAME);


		strncpy(m_szUserName[SIGN_KILL][1], m_szUserName[SIGN_KILL][2], MAX_PLAYER_NAME);
		strncpy(m_szUserName[SIGN_DEATH][1], m_szUserName[SIGN_DEATH][2], MAX_PLAYER_NAME);

		strncpy(m_szUserName[SIGN_KILL][2], m_szUserName[SIGN_KILL][3], MAX_PLAYER_NAME);
		strncpy(m_szUserName[SIGN_DEATH][2], m_szUserName[SIGN_DEATH][3], MAX_PLAYER_NAME);


		strncpy(m_szUserName[SIGN_KILL][3], m_szUserName[SIGN_KILL][4], MAX_PLAYER_NAME);
		strncpy(m_szUserName[SIGN_DEATH][3], m_szUserName[SIGN_DEATH][4], MAX_PLAYER_NAME);


		strncpy(m_szUserName[SIGN_KILL][4], szKillName, MAX_PLAYER_NAME);
		strncpy(m_szUserName[SIGN_DEATH][4], szDeathName, MAX_PLAYER_NAME);
	}
}

void CNKillSignDlg::UpdateLine() 
{

	ZeroMemory(m_szUserName[SIGN_KILL][0], MAX_PLAYER_NAME);
	ZeroMemory(m_szUserName[SIGN_DEATH][0], MAX_PLAYER_NAME);
    strncpy(m_szUserName[SIGN_KILL][0], m_szUserName[SIGN_KILL][1], MAX_PLAYER_NAME);
	strncpy(m_szUserName[SIGN_DEATH][0], m_szUserName[SIGN_DEATH][1], MAX_PLAYER_NAME);


	ZeroMemory(m_szUserName[SIGN_KILL][1], MAX_PLAYER_NAME);
	ZeroMemory(m_szUserName[SIGN_DEATH][1], MAX_PLAYER_NAME);
	strncpy(m_szUserName[SIGN_KILL][1], m_szUserName[SIGN_KILL][2], MAX_PLAYER_NAME);
	strncpy(m_szUserName[SIGN_DEATH][1], m_szUserName[SIGN_DEATH][2], MAX_PLAYER_NAME);

	ZeroMemory(m_szUserName[SIGN_KILL][2], MAX_PLAYER_NAME);
	ZeroMemory(m_szUserName[SIGN_DEATH][2], MAX_PLAYER_NAME);
	strncpy(m_szUserName[SIGN_KILL][2], m_szUserName[SIGN_KILL][3], MAX_PLAYER_NAME);
	strncpy(m_szUserName[SIGN_DEATH][2], m_szUserName[SIGN_DEATH][3], MAX_PLAYER_NAME);


	ZeroMemory(m_szUserName[SIGN_KILL][3], MAX_PLAYER_NAME);
	ZeroMemory(m_szUserName[SIGN_DEATH][3], MAX_PLAYER_NAME);
	strncpy(m_szUserName[SIGN_KILL][3], m_szUserName[SIGN_KILL][4], MAX_PLAYER_NAME);
	strncpy(m_szUserName[SIGN_DEATH][3], m_szUserName[SIGN_DEATH][4], MAX_PLAYER_NAME);

	ZeroMemory(m_szUserName[SIGN_KILL][4], MAX_PLAYER_NAME);
	ZeroMemory(m_szUserName[SIGN_DEATH][4], MAX_PLAYER_NAME);
}




