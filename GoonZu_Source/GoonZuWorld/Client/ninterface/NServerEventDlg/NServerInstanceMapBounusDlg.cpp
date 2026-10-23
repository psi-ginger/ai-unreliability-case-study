#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client.h"

#include "NServerInstanceMapBounusDlg.h"

NServerInstanceMapBounusDlg::NServerInstanceMapBounusDlg()
{
	if(false == m_Spr[FIGURE].LoadTSprFromFile("NInterface/image/ServerEvent_Bonus_Figure.SPR"))
	{
		MsgBox("파일열기 실패", "ServerEvent_Bonus_Figure.SPR");
	}
	
	if(false == m_Spr[ROLL_PAPER].LoadTSprFromFile("NInterface/image/ServerEvent_Roll_Paper.SPR"))
	{
		MsgBox("파일열기 실패", "ServerEvent_Bonus_Figure.SPR");
	}

	ZeroMemory(m_siData, sizeof(m_siData));

	m_dwOpenTickCount = 0;	// GetTickCount();
	m_RollPaperSize   = 0;

	m_siViewType = VIEW_TYPE_FIGURE;
}


NServerInstanceMapBounusDlg::~NServerInstanceMapBounusDlg()
{
	
}


void NServerInstanceMapBounusDlg::Create()
{
	
	if( true == IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NServerEvent/ServerEventBonus.ddf" ) );
	
	file.CreatePopupDialog( this, SERVER_EVENT_INSTANCE_MAP_BONUS, TEXT( "dialog_server_event_bonus" ), StaticServerInstanceMapBounusDlgProc );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, SERVER_EVENT_BONUS_IMAGE_1,		this ), SERVER_EVENT_BONUS_IMAGE_1,		 TEXT("imagestatic_NONAME1"));	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, SERVER_EVENT_BONUS_IMAGE_2,		this ), SERVER_EVENT_BONUS_IMAGE_2,		 TEXT("imagestatic_NONAME2"));	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, SERVER_EVENT_BONUS_IMAGE_3,		this ), SERVER_EVENT_BONUS_IMAGE_3,		 TEXT("imagestatic_NONAME3"));	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, SERVER_EVENT_BONUS_IMAGE_4,		this ), SERVER_EVENT_BONUS_IMAGE_4,		 TEXT("imagestatic_NONAME4"));	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		 SERVER_EVENT_BONUS_EDIT_EXPLAIN,	this ), SERVER_EVENT_BONUS_EDIT_EXPLAIN, TEXT("editbox_explain"));	

	//m_InterfaceFactory.GetEdit(SERVER_EVENT_BONUS_EDIT_EXPLAIN)->SetText(GetTxtFromMgr(8520));
	m_InterfaceFactory.GetEdit(SERVER_EVENT_BONUS_EDIT_EXPLAIN)->Show(false);
	
	SetWidth(10);  
	SetHeight(20);  

	SetX(340);      
	SetY(40);  

	SetActionTimer( 1000 );
	SetTimerOnOff( true );

	SetTopMostWindow( true );

#ifdef _DEBUG
	SI16 Data[4] = {1,2,3,4};
	//SetIstanceMapStepCount(Data);  
	//SetDailyBestServerNotice(); 
#endif
}

void CALLBACK NServerInstanceMapBounusDlg::StaticServerInstanceMapBounusDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NServerInstanceMapBounusDlg *pThis = (NServerInstanceMapBounusDlg*) pControllerMgr;
	pThis->NServerInstanceMapBounusDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NServerInstanceMapBounusDlg::NServerInstanceMapBounusDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
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

void NServerInstanceMapBounusDlg::Action(void)
{
	// 초 후에 창을 닫는다.
	if(pclClient->siServiceArea == ConstServiceArea_English
		|| pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		if ( (m_dwOpenTickCount > 0) && (GetTickCount() - m_dwOpenTickCount > 10000) ) 
		{
			DeleteDialog();
		}
	}
	else
	{
		if ( (m_dwOpenTickCount > 0) && (GetTickCount() - m_dwOpenTickCount > 4000) ) 
		{
			DeleteDialog();
		}
	}
}

void NServerInstanceMapBounusDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{ 
	SI32 Count = 0; 
	// 두루마리 그리기.
	SI32 x     = GetX() + ScreenRX + 78; 
	SI32 y	   = GetY() + ScreenRY + 125;    
	SI32 Space = 6;    
	GP.PutSpr(&m_Spr[ROLL_PAPER], x, y, 0);
	x += 33;
	for(Count = 0; Count < m_RollPaperSize; ++Count)
	{
		GP.PutSprAlpha(&m_Spr[ROLL_PAPER], x+(Count * Space), y, 5, 1);
	}
	GP.PutSpr(&m_Spr[ROLL_PAPER], x+(Count * Space), y, 2); 

	if(50 > m_RollPaperSize)
	{
		++m_RollPaperSize;  
	}
}

void NServerInstanceMapBounusDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if(VIEW_TYPE_FIGURE == m_siViewType)
	{
		// 횟수 그리기.
		CImageStatic* pImageStatic = NULL;  
		SI32 Count = 0; 
		for(Count = 0; Count < DATA_NUM; ++Count)
		{
			pImageStatic = m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_1 + Count);

			if(10 > m_siData[Count])
			{ 
				GP.PutSpr(&m_Spr[FIGURE], GetX() + pImageStatic->GetX() + ScreenRX + 43, GetY() + pImageStatic->GetY() + ScreenRY + 39, m_siData[Count]);
			} 
			else if(10 <= m_siData[Count]) 
			{    
				GP.PutSpr(&m_Spr[FIGURE], GetX() + pImageStatic->GetX() + ScreenRX + 22, GetY() + pImageStatic->GetY() + ScreenRY + 39, m_siData[Count]/10);
				GP.PutSpr(&m_Spr[FIGURE], GetX() + pImageStatic->GetX() + ScreenRX + 62, GetY() + pImageStatic->GetY() + ScreenRY + 39, m_siData[Count]%10);
			}    
		}
	}
	
	if(50 == m_RollPaperSize)	  
	{  
		CEdit* pEdit = (CEdit*)m_InterfaceFactory.GetEdit(SERVER_EVENT_BONUS_EDIT_EXPLAIN);
		if ( pEdit )
		{
			pEdit->Show( true );
		}
		m_RollPaperSize		= 51;
		m_dwOpenTickCount	= GetTickCount();
	}
}

void NServerInstanceMapBounusDlg::SetIstanceMapStepCount(SI16* _pData)
{
	memcpy(m_siData, _pData, sizeof(m_siData));	

	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_1)->Show(true);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_2)->Show(true);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_3)->Show(true);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_4)->Show(true);

	m_InterfaceFactory.GetEdit(SERVER_EVENT_BONUS_EDIT_EXPLAIN)->SetText(GetTxtFromMgr(8520));

	pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BGM_GAME_START")), 0, 0);
	
	m_siViewType = VIEW_TYPE_FIGURE;
}

void NServerInstanceMapBounusDlg::SetDailyBestServerNotice(void)
{
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_1)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_2)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_3)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_4)->Show(false);
	
	m_InterfaceFactory.GetEdit(SERVER_EVENT_BONUS_EDIT_EXPLAIN)->SetText(GetTxtFromMgr(8489));
  
	pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BATTLEWIN")), 0, 0);
 
	m_siViewType = VIEW_TYPE_DAILY_RANK_1;
}

void NServerInstanceMapBounusDlg::SetGateState( bool bGateMode )
{
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_1)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_2)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_3)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_4)->Show(false);


	CEdit* pEdit = (CEdit*)m_InterfaceFactory.GetEdit( SERVER_EVENT_BONUS_EDIT_EXPLAIN );
	if ( pEdit )
	{
		pEdit->SetFontSize( 35 );
		pEdit->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		
		// 게이트가 열렸다
		if ( true == bGateMode )
		{
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BATTLEWIN")), 0, 0);
			pEdit->SetText( GetTxtFromMgr(8517) );
		}
		// 게이트가 닫혔다
		else
		{
			pEdit->SetText( GetTxtFromMgr(8518) );
		}
	}

	m_siViewType = VIEW_TYPE_OPEN_THE_GATE;

}

void NServerInstanceMapBounusDlg::SetCharMessage( TCHAR* pszMessage )
{
	if ( NULL == pszMessage )
	{
		return;
	}

	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_1)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_2)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_3)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_4)->Show(false);

	CEdit* pEdit = (CEdit*)m_InterfaceFactory.GetEdit( SERVER_EVENT_BONUS_EDIT_EXPLAIN );
	if ( pEdit )
	{
		pEdit->SetFontSize( 20 );
		pEdit->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		pEdit->SetText( pszMessage );
	}
	
	m_siViewType = VIEW_TYPE_MESSAGE;
}

void NServerInstanceMapBounusDlg::SetMessage( SI32 _siTextNum )
{
	TCHAR* pText = GetTxtFromMgr( _siTextNum );
	if ( NULL == pText )	return;

	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_1)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_2)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_3)->Show(false);
	m_InterfaceFactory.GetImageStatic(SERVER_EVENT_BONUS_IMAGE_4)->Show(false);

	CEdit* pEdit = (CEdit*)m_InterfaceFactory.GetEdit( SERVER_EVENT_BONUS_EDIT_EXPLAIN );
	if ( pEdit )
	{
		if(pclClient->IsWhereServiceArea(ConstServiceArea_Japan))
		{
			pEdit->SetFontSize( 10 );
			pEdit->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_WORDBREAK);
			pEdit->SetText( pText );
		}
		else
		{
			pEdit->SetFontSize( 15 );
			pEdit->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_WORDBREAK);
			pEdit->SetText( pText );
		}
	}

	m_siViewType = VIEW_TYPE_MESSAGE;
}