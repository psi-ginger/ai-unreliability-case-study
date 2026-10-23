#include "../Client/ninterface/NBlackWarReformDlg/NBlackWarReformRewardDlg.h"
#include "../Client/ninterface/NBlackWarReformDlg/NBlackWarReformRunDlg.h"


extern cltCommonLogic* pclClient;


CNBlackWarReformReward::CNBlackWarReformReward()
{
	if(false == m_Spr.LoadTSprFromFile("NInterface/image/BlackResult.SPR"))
	{
		MsgBox("파일열기 실패", "BlackResult.SPR");
	}
}
CNBlackWarReformReward::~CNBlackWarReformReward()
{

}



void CNBlackWarReformReward::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NBlackWarReformDlg/NBlackWarReformRewardDlg.ddf" ) );

	SI32 controlindex = file.GetControlIndex( TEXT("dialog_black_war_reform_reward") );
	file.m_control[ controlindex ].dialog.bClosable = false;

	file.CreatePopupDialog( this, NBLACK_WAR_REWARD_DLG, TEXT( "dialog_black_war_reform_reward" ), StaticBlackWarReformRewardDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  NBLACK_WAR_REWARD_STATIC_MY_SCORE,		  this ),	NBLACK_WAR_REWARD_STATIC_MY_SCORE,		 TEXT( "statictext_my_score" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  NBLACK_WAR_REWARD_STATIC_VILLAGE_SCORE,  this ),	NBLACK_WAR_REWARD_STATIC_VILLAGE_SCORE,  TEXT( "statictext_village_score" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  NBLACK_WAR_REWARD_STATIC_MY_REWARD,	  this ),	NBLACK_WAR_REWARD_STATIC_MY_REWARD,		 TEXT( "statictext_my_reward" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  NBLACK_WAR_REWARD_STATIC_VILLAGE_REWARD, this ),	NBLACK_WAR_REWARD_STATIC_VILLAGE_REWARD, TEXT( "statictext_village_reward" ) );	

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,  NBLACK_WAR_REWARD_EDIT_MY_SCORE,			 this ),	NBLACK_WAR_REWARD_EDIT_MY_SCORE,		 TEXT( "editbox_my_score" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,  NBLACK_WAR_REWARD_EDIT_VILLAGE_SCORE,	  this ),	NBLACK_WAR_REWARD_EDIT_VILLAGE_SCORE,    TEXT( "editbox_village_score" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,  NBLACK_WAR_REWARD_EDIT_MY_REWARD,		  this ),	NBLACK_WAR_REWARD_EDIT_MY_REWARD,		 TEXT( "editbox_my_reward" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,  NBLACK_WAR_REWARD_EDIT_VILLAGE_REWARD,	  this ),	NBLACK_WAR_REWARD_EDIT_VILLAGE_REWARD,   TEXT( "editbox_village_reward" ) );	

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  NBLACK_WAR_REWARD_BUTTON_CLOSE,			   this ),	NBLACK_WAR_REWARD_BUTTON_CLOSE,			 TEXT( "button_close" ) );	
	 
	SetX(355);
	SetY(67);

	m_bWinLose	  = LOSE;
	m_bMyRewardOK = false;
}
void CNBlackWarReformReward::Destroy()
{

}
void CNBlackWarReformReward::SetMyReward(cltGameMsgResponse_Black_War_Reform_Char_Result* _MyResult)
{
	m_bMyRewardOK = true;			// 내가 보상을 받았음.

	if(true == _MyResult->bWin)
	{
		m_bWinLose = WIN;
	}
	else
	{
		m_bWinLose = LOSE;
	}


			
	TCHAR pRankText    = NULL;
	TCHAR szbuffer[32] = "";

	if(0 < _MyResult->siRank)
	{
		StringCchPrintf(szbuffer, sizeof(szbuffer), "%d%s (%d %s)",  _MyResult->siCharScore,
																	  GetTxtFromMgr(4791),
																	 _MyResult->siRank,
																	  GetTxtFromMgr(8812)				);
	}
	else
	{
		StringCchPrintf(szbuffer, sizeof(szbuffer), "%d%s, (%s)",  _MyResult->siCharScore,
																	GetTxtFromMgr(4791),
																	GetTxtFromMgr(610)				);
	}
	CEdit* pEdit = NULL;
	pEdit = m_InterfaceFactory.GetEdit( NBLACK_WAR_REWARD_EDIT_MY_SCORE );
	pEdit->SetText( szbuffer );

	NTCHARString256	kBuf1(GetTxtFromMgr(7901));
	kBuf1.Replace("#RewardMoney#", SI64ToString(_MyResult->siCharRewardMoney));
	pEdit = m_InterfaceFactory.GetEdit( NBLACK_WAR_REWARD_EDIT_MY_REWARD );
	pEdit->SetText( kBuf1 );

	NTCHARString256	kBuf2(GetTxtFromMgr(7903));
	pEdit = m_InterfaceFactory.GetEdit( NBLACK_WAR_REWARD_EDIT_VILLAGE_REWARD );
	pEdit->SetText( kBuf2 );

	cltClient* pclclient = (cltClient *)pclClient;
	CNBlackWarReformRun* pBlackWarResult = (CNBlackWarReformRun*)pclclient->m_pDialog[ NBLACK_WAR_RUN_DLG ];	
	if(NULL != pBlackWarResult)
	{
		pBlackWarResult->WindowSmallSize();	
	}
}

void CNBlackWarReformReward::SetVillageReward(cltGameMsgResponse_Black_War_Reform_Village_Result* _VillageResult)
{
	if(true == _VillageResult->bWin)
	{
		m_bWinLose = WIN;
	}
	else
	{
		m_bWinLose = LOSE;
	}

	CEdit* pEdit = NULL;

	if( false == m_bMyRewardOK )
	{
		NTCHARString256	kBuf(GetTxtFromMgr(7904));
		pEdit = m_InterfaceFactory.GetEdit( NBLACK_WAR_REWARD_EDIT_MY_REWARD );
		pEdit->SetText( kBuf );
	}


	TCHAR szbuffer[32] = "";

	if(0 < _VillageResult->siVillageRank)
	{
		StringCchPrintf(szbuffer, sizeof(szbuffer), "%d%s (%d %s)",		_VillageResult->siVillageScore,
																		GetTxtFromMgr(4791),
																		_VillageResult->siVillageRank,
																		GetTxtFromMgr(8812)				);
	}
	else
	{
		StringCchPrintf(szbuffer, sizeof(szbuffer), "%d%s, (%s)",		_VillageResult->siVillageScore,
																		GetTxtFromMgr(4791),
																		GetTxtFromMgr(610)				);
	}
	
	pEdit = m_InterfaceFactory.GetEdit( NBLACK_WAR_REWARD_EDIT_VILLAGE_SCORE );
	//[진성] 흑의 군단 개편 점수 출력시 마을 점수 출력 수정. => 2008-9-5
	//pEdit->SetText(SI32ToString( _VillageResult->siVillageScore) );
	pEdit->SetText( szbuffer );
	

	NTCHARString256	kBuf(GetTxtFromMgr(7902));
	kBuf.Replace("#Rank#",		  SI64ToString(_VillageResult->siVillageRank));
	kBuf.Replace("#RewardMoney#", SI64ToString(_VillageResult->siVillageRewardMoney));
	
	pEdit = m_InterfaceFactory.GetEdit( NBLACK_WAR_REWARD_EDIT_VILLAGE_REWARD );
	pEdit->SetText( kBuf );

	cltClient* pclclient = (cltClient *)pclClient;
	CNBlackWarReformRun* pBlackWarResult = (CNBlackWarReformRun*)pclclient->m_pDialog[ NBLACK_WAR_RUN_DLG ];	
	if(NULL != pBlackWarResult)
	{
		pBlackWarResult->WindowSmallSize();	
	}
}

void CNBlackWarReformReward::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{ 
	GP.PutSpr(&m_Spr, GetX() + ScreenRX +10 , GetY() + ScreenRY + 10, m_bWinLose);
}

void CALLBACK CNBlackWarReformReward::StaticBlackWarReformRewardDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNBlackWarReformReward *pThis = (CNBlackWarReformReward*) pControllerMgr;
	pThis->BlackWarReformRewardDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK CNBlackWarReformReward::BlackWarReformRewardDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch ( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;	

		case NBLACK_WAR_REWARD_BUTTON_CLOSE:
			{
				DeleteDialog();
			}	
			break;
	}
}
