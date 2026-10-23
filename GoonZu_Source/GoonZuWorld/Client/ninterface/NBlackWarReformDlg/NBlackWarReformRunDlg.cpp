#include "../Client/ninterface/NBlackWarReformDlg/NBlackWarReformRunDlg.h"


extern cltCommonLogic* pclClient;

#define	RANKING_COUNT	5

CNBlackWarReformRun::CNBlackWarReformRun()
{
	if(false == m_Spr.LoadTSprFromFile("NInterface/image/BlackRank.SPR"))
	{
		MsgBox("파일열기 실패", "BlackRank.SPR");
	}
	
}
CNBlackWarReformRun::~CNBlackWarReformRun()
{

}

void CNBlackWarReformRun::Create()
{
	if ( IsCreate() )
		return;

	m_WindowKind = BIG_SIZE;
	m_bDelay     = false;
	
	// [추가 : 황진성 2008. 1. 23 => 인스턴스 맵에서는 흑의 군단 진행 상태창이 뜨지 않게.]
	SI32 MapIndex  = pclClient->pclCM->CR[1]->GetMapIndex();
	if(true == pclClient->pclMapManager->IsInstanceMap(MapIndex))
	{
		return;
	}

	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NBlackWarReformDlg/NBlackWarReformRunDlg.ddf" ) );

	SI32 controlindex = file.GetControlIndex( TEXT("dialog_blackWarReform") );
	file.m_control[ controlindex ].dialog.bClosable = false;

	file.CreatePopupDialog( this, NBLACK_WAR_RUN_DLG, TEXT( "dialog_blackWarReform" ), StaticBlackWarReformRunDlgProc );

	TCHAR Buffer[32] = "";
	for( int count = 0; count < RANKING_COUNT; ++count )
	{
		ZeroMemory( Buffer, sizeof( Buffer ) );
		StringCchPrintf( Buffer, sizeof(Buffer), "%s%d", "statictext_char_no", count+1 );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  NBLACK_WAR_RUN_STATIC_CHAR_NO1 + count, this ),	NBLACK_WAR_RUN_STATIC_CHAR_NO1 + count, TEXT( Buffer ) );	

		ZeroMemory( Buffer, sizeof( Buffer ) );
		StringCchPrintf( Buffer, sizeof(Buffer), "%s%d", "statictext_village_no", count+1 );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  NBLACK_WAR_RUN_STATIC_VILLAGE_NO1 + count, this ),	NBLACK_WAR_RUN_STATIC_VILLAGE_NO1 + count, TEXT( Buffer ) );	
	}

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  NBLACK_WAR_RUN_STATIC_MY_CHAR_SCORE,	 this ), NBLACK_WAR_RUN_STATIC_MY_CHAR_SCORE,	 TEXT( "statictext_my_char_score" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  NBLACK_WAR_RUN_STATIC_MY_VILLAGE_SCORE, this ), NBLACK_WAR_RUN_STATIC_MY_VILLAGE_SCORE, TEXT( "statictext_my_village_score" ) );	

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  NBLACK_WAR_RUN_BUTTON_SMALL,		this ), NBLACK_WAR_RUN_BUTTON_SMALL,	   TEXT( "button_small" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  NBLACK_WAR_RUN_BUTTON_SMALL_CLICK, this ), NBLACK_WAR_RUN_BUTTON_SMALL_CLICK, TEXT( "button_Small_Click_Button" ) );	

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,  NBLACK_WAR_RUN_EDIT_TOTAL_STATUS, this ), NBLACK_WAR_RUN_EDIT_TOTAL_STATUS, TEXT( "editbox_Total_Status" ) );	

			
	CStatic* pStatic = NULL;
	for( int count = 0; count < RANKING_COUNT; ++count )
	{
		pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_CHAR_NO1 + count );
		pStatic->SetFontColor( RGB( 0xff, 0xff, 0xff ) );

		pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_VILLAGE_NO1 + count );
		pStatic->SetFontColor( RGB( 0xff, 0xff, 0xff ) );
	}

	pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_MY_CHAR_SCORE );
	pStatic->SetFontColor( RGB( 0xff,0xff,0xff ) );

	pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_MY_VILLAGE_SCORE );
	pStatic->SetFontColor( RGB( 0xff,0xff,0xff ) );
	
	CEdit* Edit = NULL;
	Edit = m_InterfaceFactory.GetEdit( NBLACK_WAR_RUN_EDIT_TOTAL_STATUS );
	Edit->SetFontColor( RGB( 0xcc,0xcc,0xcc ) );

	CButton* pButton = m_InterfaceFactory.GetButton( NBLACK_WAR_RUN_BUTTON_SMALL );
	pButton->Show(false);		
	// 이것은 보여주는 창인데 요것땜시 배경이 클릭이 안되면 이상하니까.
	SetHeight(50);
	SetAlphaMode(7);

	SetActionTimer( 10000 );    
	SetTimerOnOff( true );

	WindowSmallSize();
}
void CNBlackWarReformRun::Destroy()
{
	
}
void CNBlackWarReformRun::SetRunStatus(TCHAR* pWarMsg, TCHAR** WarPlayersMsg, TCHAR** WarVillageMsg)
{
	TCHAR WarPlayers[MAX_WARRANK_PERSON_PER_MAP][MAX_WAR_MSG_SIZE];
	TCHAR WarVillage[MAX_WARRANK_PERSON_PER_MAP][MAX_WAR_MSG_SIZE];
	ZeroMemory( WarPlayers, sizeof( WarPlayers ) );
	ZeroMemory( WarVillage, sizeof( WarVillage ) );
     
	memcpy( WarPlayers, WarPlayersMsg,  MAX_WAR_MSG_SIZE * MAX_WARRANK_PERSON_PER_MAP * sizeof( TCHAR ) );
	memcpy( WarVillage, WarVillageMsg,  MAX_WAR_MSG_SIZE * MAX_WARRANK_PERSON_PER_MAP * sizeof( TCHAR ) );

	CStatic* pStatic = NULL;
	for( int count = 0; count < RANKING_COUNT; ++count )
	{
		pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_CHAR_NO1 + count );
		pStatic->SetText( WarPlayers[count] ); 
        
		pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_VILLAGE_NO1 + count );
		pStatic->SetText( WarVillage[count] ); 
	}
	  
	pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_MY_CHAR_SCORE );
	pStatic->SetText( SI32ToString( pclClient->pclCM->CR[1]->siBlackWarEnemyKillScore ) ); 
		   
	pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_MY_VILLAGE_SCORE );
	pStatic->SetText( SI32ToString( pclClient->pclCM->CR[1]->siBlackWarEnemyKillVillageScore ) ); 

	m_InterfaceFactory.GetEdit( NBLACK_WAR_RUN_EDIT_TOTAL_STATUS )->SetText(pWarMsg);	
}


void CNBlackWarReformRun::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	static float Size = 0.f;
	if(BIG_SIZE == m_WindowKind)
	{
		if(Size < 1.f)
		{
			Size += Size + 0.001f;   
		}
		else
		{
			Size = 1.f;   
		}

		GP.PutSprRS(&m_Spr, GetX() + ScreenRX , GetY() + ScreenRY, 0, 0, 0, Size);
	}
	else
	{
		Size = 0.f;     
	}
}

void CNBlackWarReformRun::WindowBigSize(void)
{
	m_WindowKind = BIG_SIZE;

	CStatic* pStatic = NULL;
	for( int count = 0; count < RANKING_COUNT; ++count )
	{
		pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_CHAR_NO1 + count );
		pStatic->Show(true);
		pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_VILLAGE_NO1 + count );
		pStatic->Show(true);
	}

	pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_MY_CHAR_SCORE );
	pStatic->Show(true);

	pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_MY_VILLAGE_SCORE );
	pStatic->Show(true);

	m_InterfaceFactory.GetEdit( NBLACK_WAR_RUN_EDIT_TOTAL_STATUS )->Show(true);
	
	CButton* pButton = m_InterfaceFactory.GetButton( NBLACK_WAR_RUN_BUTTON_SMALL );
	pButton->Show(false);

	pButton = m_InterfaceFactory.GetButton( NBLACK_WAR_RUN_BUTTON_SMALL_CLICK );
	pButton->Show(true);		

	SetAlphaMode(7);
	SetMoveable(true);

	SetX(380);           
	SetY(90);    

	SetWidth(400);
}

void CNBlackWarReformRun::WindowSmallSize(void)
{
	m_WindowKind = SMALL_SIZE;

	CStatic* pStatic = NULL;
	for( int count = 0; count < RANKING_COUNT; ++count )
	{
		pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_CHAR_NO1 + count );
		pStatic->Show(false);
		pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_VILLAGE_NO1 + count );
		pStatic->Show(false);
	}

	pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_MY_CHAR_SCORE );
	pStatic->Show(false);

	pStatic = m_InterfaceFactory.GetStatic( NBLACK_WAR_RUN_STATIC_MY_VILLAGE_SCORE );
	pStatic->Show(false);

	m_InterfaceFactory.GetEdit( NBLACK_WAR_RUN_EDIT_TOTAL_STATUS )->Show(false);

	CButton* pButton = m_InterfaceFactory.GetButton( NBLACK_WAR_RUN_BUTTON_SMALL );
	pButton->Show(true);

	pButton = m_InterfaceFactory.GetButton( NBLACK_WAR_RUN_BUTTON_SMALL_CLICK );
	pButton->Show(false);		

	SetAlphaMode(0);        
	SetMoveable(false);

	SetX(343);  
	SetY(90);     

	SetWidth(50);
}

void CNBlackWarReformRun::Action(void)
{
	if(true == m_bDelay)
	{
		WindowBigSize();

		SetTimerOnOff( false );
	}
	m_bDelay = true;
}

void CALLBACK CNBlackWarReformRun::StaticBlackWarReformRunDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNBlackWarReformRun *pThis = (CNBlackWarReformRun*) pControllerMgr;
	pThis->BlackWarReformRunDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK CNBlackWarReformRun::BlackWarReformRunDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch ( nControlID )
	{
		case NBLACK_WAR_RUN_BUTTON_SMALL_CLICK:
			{
				WindowSmallSize();
			}
			break;	
		case NBLACK_WAR_RUN_BUTTON_SMALL:
			{
				WindowBigSize();
			}
			break;
	}
}