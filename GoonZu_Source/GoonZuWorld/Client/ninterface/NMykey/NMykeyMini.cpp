#include "NMykey.h"
#include "NMykeyMini.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../common/Quest/Quest.h"

CNMykeyMiniDlg::CNMykeyMiniDlg()
{
	ZeroMemory( m_siDialogKind,		sizeof(m_siDialogKind) );
	
	memset(	m_siDialogKind, -1, sizeof(m_siDialogKind) );
}

CNMykeyMiniDlg::~CNMykeyMiniDlg()
{
	Destroy();
}

void CNMykeyMiniDlg::Init()
{

}

void CNMykeyMiniDlg::Destroy()
{


}

void CNMykeyMiniDlg::Create()
{
	if( IsCreate() )
	{
        return;
	}

	CInterfaceFile file;

    file.LoadFile("Ninterface/Data/NMyKey/NMyKeyMini.ddf");

	SI32 controlindex = file.GetControlIndex( TEXT("dialog_mykeymini") );
	file.m_control[ controlindex ].dialog.bClosable = false;

	file.CreatePopupDialog( this , NMYKEYMINI, "dialog_mykeymini" , StaticNMykeyMiniDlgProc );

	//if( g_bFullScreen)	MovePos( 80, 40 );
	//else		 			MovePos( 45, 20 );

	if( g_bFullScreen)	MovePos( 459, 705 );
	else		 		MovePos( 424, 685 );

	TCHAR szControlName[128] = "";
	for(SI32 Count = 0; Count < MYKEY_NUM; ++Count)
	{
		StringCchPrintf(szControlName, sizeof(szControlName),  "button_alt%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NMYKEYMINI_BUTTON_ALT_NO0 + Count,	this), NMYKEYMINI_BUTTON_ALT_NO0 + Count, szControlName );
	}
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		 NMYKEYMINI_BUTTON_BIG_WINDOW, this), NMYKEYMINI_BUTTON_BIG_WINDOW,  "button_big_window");


	LoadFile();

	SetActionTimer( 1000 );
	SetTimerOnOff( true );

	Show( false );	
}

void CNMykeyMiniDlg::Action()
{
	if( pclClient->IsCountrySwitch( Switch_Mykey ) )
	{
		if ( pclClient->GetGameStatus() == GS_GAME )	
		{
			// 설정이 안되있으면.
			cltClient * pclclient = (cltClient*) pclClient;
			if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bMykey == false )
			{
				Show( false );	
			}
			else
			{
				if( false == ((CNMykeyDlg*)pclclient->m_pDialog[NMYKEY])->IsShow() )
				{
					if( false == IsShow() )
						Show( true );
		
				}
			}

			// 퀘스트 관련
			cltCharClient* pclchar = pclclient->pclCM->GetCharClient( 1 );
			if ( (pclchar) && (pclchar->pclCI->clQuestInfo.IsQuestGoing(QUEST_CLEAR_COND_SETMYKEY)) )
			{
				for (SI32 siIndex=0; siIndex<MYKEY_NUM; siIndex++)
				{
					if ( 0 <= m_siDialogKind[siIndex] )
					{
						pclchar->DoQuestCheckAndAction( QUEST_CLEAR_COND_SETMYKEY );
						break;
					}
				}
			}

		}
	}
}

void CALLBACK CNMykeyMiniDlg::StaticNMykeyMiniDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMykeyMiniDlg *pThis = (CNMykeyMiniDlg *)pControllerMgr;
	pThis->NMykeyMiniProc( nEvent, nControlID, pControl );
}
void CALLBACK CNMykeyMiniDlg::NMykeyMiniProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case NMYKEYMINI_BUTTON_ALT_NO0:
			{
				KeyAction( ID_SHORT_ALT_1 );
			}
			break;
		case NMYKEYMINI_BUTTON_ALT_NO1:
			{
				KeyAction( ID_SHORT_ALT_2 );
			}
			break;
		case NMYKEYMINI_BUTTON_ALT_NO2:
			{
				KeyAction( ID_SHORT_ALT_3 );
			}
			break;
		case NMYKEYMINI_BUTTON_ALT_NO3:
			{
				KeyAction( ID_SHORT_ALT_4 );
			}
			break;
		case NMYKEYMINI_BUTTON_ALT_NO4:
			{
				KeyAction( ID_SHORT_ALT_5 );
			}
			break;
		case NMYKEYMINI_BUTTON_BIG_WINDOW:
			{
			  	if( NULL == pclclient->m_pDialog[NMYKEY] )
					pclclient->CreateInterface( NMYKEY );
				else
					((CNMykeyDlg*)pclclient->m_pDialog[NMYKEY])->Show( true );

				Show( false );
			}
			break;
		default:
			{
			}
			break;
	}
}



void CNMykeyMiniDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{


}

void CNMykeyMiniDlg::SetDialogKind(SI32* _psiDialogKind)
{


}

void CNMykeyMiniDlg::KeyAction( SI32 _KeyKind )
{
	cltClient * pclclient = (cltClient*) pclClient;

	if( pclclient->m_pDialog[NMYKEY] )
	{
		((CNMykeyDlg*)pclclient->m_pDialog[NMYKEY])->KeyAction(_KeyKind);
	}
	else
	{
		pclclient->CreateInterface( NMYKEY );
		((CNMykeyDlg*)pclclient->m_pDialog[NMYKEY])->KeyAction(_KeyKind);
	}
}

void CNMykeyMiniDlg::LoadFile()
{
	cltClient * pclclient = (cltClient*) pclClient;

	FILE* fp = NULL;
	fp = _tfopen(TEXT("Config/Mykey.dat"), TEXT("rb"));

	if( NULL == fp )	// 파일이 없으면 생성.
	{
		return;
	}
	else				// 있으면 읽자.
	{
		fread( m_siDialogKind, sizeof(m_siDialogKind), 1, fp );
		fclose( fp );
	}

	// 설정한 컨트롤 인덱스를 이용해 텍스트번호를 얻어 설정한 키 정보로 표시. 
	SI32 TextNum = 0;
	for( SI32 Count = 0; Count < MYKEY_NUM; ++Count ) 
	{
		TextNum = ((CNMykeyDlg*)pclclient->m_pDialog[NMYKEY])->FindTextNum( m_siDialogKind[Count] );
		if(-1 == TextNum) 
		{
			m_InterfaceFactory.GetButton(NMYKEYMINI_BUTTON_ALT_NO0 + Count)->Enable( false );
			m_InterfaceFactory.GetButton(NMYKEYMINI_BUTTON_ALT_NO0 + Count)->SetUseToolTip( false );
		}
		else
		{
			m_InterfaceFactory.GetButton(NMYKEYMINI_BUTTON_ALT_NO0 + Count)->Enable( true );
			m_InterfaceFactory.GetButton(NMYKEYMINI_BUTTON_ALT_NO0 + Count)->SetUseToolTip( GetTxtFromMgr( TextNum ) );
		}
	}
}


