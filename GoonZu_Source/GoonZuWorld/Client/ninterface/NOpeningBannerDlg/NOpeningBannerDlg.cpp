#include "NOpeningBannerDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../NLib/NDataLoader.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../Client/Client.h"

extern SI16 g_siChannelingCode;

CNOpeningBannerDlg::CNOpeningBannerDlg()
{
	for ( SI32 i=0; i<MAX_BANNER_NUMBER; i++)
	{
		StringCchCopy( m_stBanner[i].szFileName, MAX_PATH, TEXT("") );
		StringCchCopy( m_stBanner[i].szUrl, MAX_URL_LENGTH, TEXT("") );

		m_pButtonPage[i] = NULL;

	}
	m_siBannerCount = 0;
	m_siSelectedIndex = 0;

	m_pImageBanner	= NULL;
	m_pButtonExit	= NULL;
}

CNOpeningBannerDlg::~CNOpeningBannerDlg()
{
	SetTimerOnOff( false );

	SAFE_DELETE( m_pImageBanner );
	SAFE_DELETE( m_pButtonExit );
	for ( SI16 i=0 ; i<MAX_BANNER_NUMBER; i++ )
	{
		SAFE_DELETE( m_pButtonPage[i] );
	}
}

void CNOpeningBannerDlg::Create()
{
	LoadBannerText();

	if ( m_siBannerCount == 0 )
	{
		return;
	}

	NEWCIMAGESTATIC ( m_pImageBanner );
	NEWCBUTTON		( m_pButtonExit );

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NOpeningBannerDlg/DLG_NOpeningBanner.ddf"));
	file.CreatePopupDialog( this, NOPENINGBANNER_DLG , TEXT("dialog_openingbanner"), StaticCallBackDialogNOpeningBanner );

	file.CreateControl( m_pImageBanner, NOPENINGBANNER_DLG_BANNER_IMAGE, TEXT("imagestatic_banner") );
	file.CreateControl( m_pButtonExit, NOPENINGBANNER_DLG_EXIT_BUTTON, TEXT("button_exit") );

	SI16 siStartXpos = 540 + ( 5 - m_siBannerCount ) * 30;
	for ( SI16 i=0 ; i<m_siBannerCount ; i++ )
	{
		m_pButtonPage[ i ] = new CButton( this );
		m_pButtonPage[ i ]->Create( NOPENINGBANNER_DLG_PAGE1 + i, NULL, TEXT("NInterface/Button/Button_30x30_01_000.SPR"), i*4, false, 0+i*4 , siStartXpos+i*30, 520, 30, 30, 30, 30 );
	}

	// 처음에 랜덤으로 하나 로드
	m_siSelectedIndex = rand() % m_siBannerCount;
	SetBannerImage( m_siSelectedIndex );


	// 화면 중앙에 위치 시킨다.
	cltClient* pclclient = (cltClient*)pclClient;
	SI16 xPos = ( pclclient->GetScreenXSize() - GetWidth() ) / 2 + 40;
	SI16 yPos = ( pclclient->GetScreenYSize() - GetHeight() ) / 2 + 20;
	MovePos( xPos, yPos );

	SetTopMostWindow( true );

	SetTimerOnOff( true );
	SetActionTimer(200);

	Show( true );
}

void CALLBACK CNOpeningBannerDlg::StaticCallBackDialogNOpeningBanner( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNOpeningBannerDlg *pThis = (CNOpeningBannerDlg*)pControllerMgr;
	pThis->CallBackDialogNOpeningBanner( nEvent, nControlID, pControl );
}

void CALLBACK CNOpeningBannerDlg::CallBackDialogNOpeningBanner( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		DeleteDialog();
		break;
	case NOPENINGBANNER_DLG_EXIT_BUTTON:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			DeleteDialog();
		}
		break;
	case NOPENINGBANNER_DLG_PAGE1:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siSelectedIndex = 0;
			SetBannerImage( m_siSelectedIndex );
		}
		break;
	case NOPENINGBANNER_DLG_PAGE2:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siSelectedIndex = 1;
			SetBannerImage( m_siSelectedIndex );
		}
		break;
	case NOPENINGBANNER_DLG_PAGE3:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siSelectedIndex = 2;
			SetBannerImage( m_siSelectedIndex );
		}
		break;
	case NOPENINGBANNER_DLG_PAGE4:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siSelectedIndex = 3;
			SetBannerImage( m_siSelectedIndex );
		}
		break;
	case NOPENINGBANNER_DLG_PAGE5:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siSelectedIndex = 4;
			SetBannerImage( m_siSelectedIndex );
		}
		break;
	case NOPENINGBANNER_DLG_BANNER_IMAGE:
		if ( EVENT_IMAGESTATIC_LBUTTONUP == nEvent )
		{
			if ( m_siSelectedIndex > -1 )
                ShellExecute( NULL, NULL, m_stBanner[ m_siSelectedIndex ].szUrl, NULL, NULL, SW_SHOWNORMAL );
		}
		break;
	}
}

void CNOpeningBannerDlg::Action()
{
	if(IsShow())
	{
		SetTopMostWindow( true );
	}
}

void CNOpeningBannerDlg::LoadBannerText()
{
	BOOL bLoadFile = FALSE;
	NDataLoader	dataloader;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/OpeningBanner.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/OpeningBanner.txt") );
	}

	if (!bLoadFile)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("Opening Banner Load Error"), TEXT("Error"), MB_OK|MB_TOPMOST);
		return;
	}

	TCHAR szUrl[MAX_URL_LENGTH] = TEXT("");
	TCHAR szFileName[MAX_PATH] = TEXT("");
	SI32 siIsNetMarble = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szFileName,		MAX_PATH,
		NDATA_MBSTRING,	szUrl,			MAX_URL_LENGTH,
		NDATA_INT32,	&siIsNetMarble,	4,
			0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo );

	m_siBannerCount = 0;

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())	
		{
			if ( szFileName[ 0 ] != NULL )
			{
				if ( pclClient->siServiceArea == ConstServiceArea_Japan )
				{
					if ( g_siChannelingCode == CHANNELINGCODE_NETMARBLEJP && siIsNetMarble == 1 )
					{
						StringCchCopy( m_stBanner[m_siBannerCount].szFileName, MAX_PATH, szFileName);
						StringCchCopy( m_stBanner[m_siBannerCount].szUrl, MAX_URL_LENGTH, szUrl);
						m_siBannerCount++;
					}
					else if ( g_siChannelingCode <= 0 && siIsNetMarble == 0 )
					{
						StringCchCopy( m_stBanner[m_siBannerCount].szFileName, MAX_PATH, szFileName);
						StringCchCopy( m_stBanner[m_siBannerCount].szUrl, MAX_URL_LENGTH, szUrl);
						m_siBannerCount++;
					}
				}
				else
				{
					StringCchCopy( m_stBanner[m_siBannerCount].szFileName, MAX_PATH, szFileName);
					StringCchCopy( m_stBanner[m_siBannerCount].szUrl, MAX_URL_LENGTH, szUrl);
					m_siBannerCount++;
				}
			}
		}
		
		if ( m_siSelectedIndex >= MAX_BANNER_NUMBER ) break;
	}
}
void CNOpeningBannerDlg::SetBannerImage( SI16 index )
{
	m_pImageBanner->SetFileName( m_stBanner[index].szFileName );
}
