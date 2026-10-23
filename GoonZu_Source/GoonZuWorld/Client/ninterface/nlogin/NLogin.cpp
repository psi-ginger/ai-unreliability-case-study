#include "../Client/NInterface/NLogin/NLogin.h"

#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/CheckBox.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;

extern SI16 g_siChannelingCode;
extern bool g_bHanAuth;
extern bool g_bWebLoginService;
extern bool g_bUSA_Auth;
//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
extern bool g_bPorteAuth;
#endif

// WEB LOGIN SERVICE
extern char g_szWeb_id[ 1024 ];
extern char g_szWeb_pw[ 1024 ];
extern bool g_bWebLoginService;


CNLoginDlg::CNLoginDlg() : CDialogController()
{
	memset( &m_stLoginData, 0, sizeof( stLoginData ) );
	memset( m_strErrorMsg, 0, LOGIN_ERROR_MSG_LENGTH );

	m_bErrorExist = false;
	m_si32Status = LOGIN_READY;

	m_bFlag = false;

	/*cyj 회원종류 선택안함
	m_pRegistSiteInfoList = new stRegistSiteInfo[ MAX_REGISTSITE_NUM ];
	memset( m_pRegistSiteInfoList, 0, sizeof( stRegistSiteInfo ) * MAX_REGISTSITE_NUM );
	*/

	m_bIdCheck = false;

	FILE *fpId = NULL ;

	fpId = _tfopen(TEXT("./Config/CheckLog.dat"), TEXT("rt"));
	if( fpId != NULL )
	{
		SI32 TempCheck;

		_ftscanf(fpId, TEXT("%d"), &TempCheck);

		if( TempCheck == 1 )
			m_bIdCheck = true;
		else if( TempCheck == 0 )
			m_bIdCheck = false;

		fclose(fpId);
	}
	
	/*cyj 회원종류 선택안함
	FILE *fp = NULL ;

	if ( pclClient->siServiceArea == ConstServiceArea_Korea )
	{
		fp = _tfopen( TEXT("./RegistSite/RegistSite.dat"), TEXT("rt") );
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )
	{
		fp = _tfopen( TEXT("./RegistSite/RegistSiteC.dat"), TEXT("rt") );
	}
	else
	{
		fp = _tfopen( TEXT("./RegistSite/RegistSiteE.dat"), TEXT("rt") );
	}

	if ( fp != NULL )
	{
		SI32 Count = 0;
		while(!feof( fp ))
		{
			_ftscanfnf( fp, TEXT("%s %d %d"), m_pRegistSiteInfoList[ Count ].m_strSiteName, &m_pRegistSiteInfoList[ Count ].m_siIndex, &m_pRegistSiteInfoList[ Count ].m_siDefaultSite );
			++Count;
		}

		fclose( fp );
	}
	else
	{
		// RegistSite.dat 파일이 없는 경우는 그냥 군주가 디폴트임
		TCHAR* pText = GetTxtFromMgr(599);
		strcpy( m_pRegistSiteInfoList[ 0 ].m_strSiteName, pText );
		m_pRegistSiteInfoList[ 0 ].m_siIndex = 1;
		m_pRegistSiteInfoList[ 0 ].m_siDefaultSite = 1;
	}

	m_siCurrentSelectedSiteIndex = 0;
	*/

	Initialize();

	m_pEdit[0] = NULL;
	m_pEdit[1] = NULL;

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;

	m_pStatic = NULL;
	
	m_pCheckBox = NULL;

	m_pStaticID = NULL;
	m_pStaticPW = NULL;
}

CNLoginDlg::~CNLoginDlg()
{
	/*cyj
	if ( m_pRegistSiteInfoList )
	{
		delete [] m_pRegistSiteInfoList;
		m_pRegistSiteInfoList = NULL;
	}
	*/

	SAFE_DELETE(m_pEdit[0]);
	SAFE_DELETE(m_pEdit[1]);

	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[2]);

	SAFE_DELETE( m_pStatic );
	
	SAFE_DELETE(m_pCheckBox);

	SAFE_DELETE(m_pStaticID);
	SAFE_DELETE(m_pStaticPW);
}

void CNLoginDlg::Create()
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/DLG_NLogin.ddf"));
	file.CreatePopupDialog( this, NLOGIN_DLG, TEXT("dialog_Login"), StaticCallBackDialogNLoginDlg,false );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_Korea:
		{
			if (!g_SoundOnOffDlg.m_bMaxWindowMode)		MovePos( 48, -8 ); // 윈도
			else										MovePos( 80, 0 + 40 );  // 풀스크린
		}
		break;
	case ConstServiceArea_Japan:
	case ConstServiceArea_China:
	case ConstServiceArea_USA:
	case ConstServiceArea_English:
	case ConstServiceArea_EUROPE:
	case ConstServiceArea_NHNChina:
		{
			if (!g_SoundOnOffDlg.m_bMaxWindowMode)		MovePos( 261 + 48, -8 ); // 윈도
			else										MovePos( 261 + 80, 0 + 40 );  // 풀스크린
		}
		break;
	case ConstServiceArea_Taiwan:
		{
			if (!g_SoundOnOffDlg.m_bMaxWindowMode)		MovePos( 400, 270 ); // 윈도
			else										MovePos( 440, 300 );  // 풀스크린
		}
		break;
	default:
		{
			if (!g_SoundOnOffDlg.m_bMaxWindowMode)		MovePos( 400, 270 ); // 윈도
			else										MovePos( 440, 300 );  // 풀스크린
		}
		break;
	}

	m_pEdit[0] = new CEdit( this );
	m_pEdit[1] = new CEdit( this );

	m_pButton[0] = new CButton( this );
	m_pButton[1] = new CButton( this );
	m_pButton[2] = new CButton( this );

	NEWCSTATIC( m_pStatic );
	
	NEWCCHECKBOX(m_pCheckBox);

	m_pStaticID	= new CStatic( this );
	m_pStaticPW	= new CStatic( this );

	file.CreateControl( m_pEdit[0], NLOGIN_DIALOG_EDIT1, TEXT("editbox_NONAME1"));	// 아이디
	file.CreateControl( m_pEdit[1], NLOGIN_DIALOG_EDIT2, TEXT("editbox_NONAME2"));	// 패스워드

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_Japan:
	case ConstServiceArea_China:
	case ConstServiceArea_Korea:
	case ConstServiceArea_USA:
	case ConstServiceArea_English:
	case ConstServiceArea_EUROPE:
	case ConstServiceArea_NHNChina:
		{
			m_pEdit[1]->SetFontColor( RGB( 24, 42, 89 ) );
			m_pEdit[0]->SetFontColor( RGB( 24, 42, 89 ) );
		}
		break; 
	case ConstServiceArea_Taiwan:
		{
			m_pEdit[1]->SetFontColor( RGB( 255, 255, 255 ) );
			m_pEdit[0]->SetFontColor( RGB( 255, 255, 255 ) );
		}
		break;
	default:
		{
			m_pEdit[1]->SetFontColor( RGB( 255, 255, 255 ) );
			m_pEdit[0]->SetFontColor( RGB( 255, 255, 255 ) );
		}
		break;
	}
		
	m_pEdit[0]->SetTabKeyPerssNotifyUse(TRUE);
	m_pEdit[0]->SetMaxEditTextLength(MAX_PLAYER_NAME);
	m_pEdit[0]->SetAutoScroll(TRUE);
	m_pEdit[0]->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	m_pEdit[0]->Refresh();

	m_pEdit[1]->SetPasswordMode(true);
	m_pEdit[1]->SetTabKeyPerssNotifyUse(TRUE);
	m_pEdit[1]->SetMaxEditTextLength(MAX_PLAYER_NAME);
	m_pEdit[1]->SetAutoScroll(TRUE);
	m_pEdit[1]->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	m_pEdit[1]->Refresh();

	file.CreateControl( m_pButton[0], NLOGIN_DIALOG_BUTTON1, TEXT("button_NONAME1"));
	file.CreateControl( m_pButton[1], NLOGIN_DIALOG_BUTTON2, TEXT("button_NONAME2"));
	file.CreateControl( m_pButton[2], NLOGIN_DIALOG_BUTTON3, TEXT("button_NONAME3"));

	
	m_pButton[0]->SetFontColor( RGB( 255, 255, 255 ) );
	m_pButton[1]->SetFontColor( RGB( 255, 255, 255 ) );
	m_pButton[2]->SetFontColor( RGB( 255, 255, 255 ) );

	if( (pclClient->siServiceArea == ConstServiceArea_Taiwan))
		// ||(pclClient->siServiceArea == ConstServiceArea_NHNChina))
	{
		TCHAR *ButtonName = NULL;
		ButtonName = GetTxtFromMgr( 4101 );
		if ( ButtonName )
		{
			m_pButton[0]->SetText( ButtonName );
			m_pButton[0]->Refresh();
		}
		ButtonName = GetTxtFromMgr( 4419 );
		if ( ButtonName )
		{
			m_pButton[1]->SetText( ButtonName );
			m_pButton[1]->Refresh();
		}
		ButtonName = GetTxtFromMgr( 5247 );
		if ( ButtonName )
		{
			m_pButton[2]->SetText( ButtonName );
			m_pButton[2]->Refresh();
		}
	}

	file.CreateControl( m_pStatic, NLOGIN_DIALOG_STATIC1, TEXT("statictext_NONAME1"));
	

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Japan:
	case ConstServiceArea_Korea:
	case ConstServiceArea_English:
	case ConstServiceArea_USA:
	case ConstServiceArea_EUROPE:
	case ConstServiceArea_NHNChina:
		m_pStatic->SetFontColor( RGB( 24, 42, 89 ) );
		break;
	case ConstServiceArea_Taiwan:
		m_pStatic->SetFontColor( RGB( 255, 255, 255 ) );
		break;

	default:
		m_pStatic->SetFontColor( RGB( 255, 255, 255 ) );
		break;
	}

	m_pStatic->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	m_pStatic->Refresh();
	
	TCHAR* pID;
	pID = GetTxtFromMgr(5245);
	TCHAR* pPass;
	pPass = GetTxtFromMgr(5246);
	TCHAR* pSaveID;
	pSaveID = GetTxtFromMgr(5252);

	file.CreateControl( m_pStaticID, NLOGIN_DIALOG_STATIC1, TEXT("statictext_ID"));
	m_pStaticID->SetFontColor( RGB( 24, 42, 89 ) );
	m_pStaticID->SetFontWeight(FW_BOLD);
	m_pStaticID->SetTextPrintOption( DT_RIGHT | DT_VCENTER | DT_SINGLELINE );

	m_pStaticID->SetText( pID, m_pStaticID->GetTextPrintOption());
	m_pStaticID->Refresh();

	file.CreateControl( m_pStaticPW, NLOGIN_DIALOG_STATIC1, TEXT("statictext_PW"));
	m_pStaticPW->SetFontColor( RGB( 24, 42, 89 ) );
	m_pStaticPW->SetFontWeight(FW_BOLD);
	m_pStaticPW->SetTextPrintOption( DT_RIGHT | DT_VCENTER | DT_SINGLELINE );
	m_pStaticPW->SetText( pPass, m_pStaticPW->GetTextPrintOption());
	m_pStaticPW->Refresh();

	file.CreateControl( m_pCheckBox, NLOGIN_DIALOG_CHECKBOX, TEXT("checkbox_NONAME1"));
	if( pclClient->siServiceArea == ConstServiceArea_Japan
		|| pclClient->siServiceArea == ConstServiceArea_China
		|| pclClient->siServiceArea == ConstServiceArea_Korea
		|| pclClient->siServiceArea == ConstServiceArea_USA
		|| pclClient->siServiceArea == ConstServiceArea_English
		|| pclClient->siServiceArea == ConstServiceArea_NHNChina 
		|| pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
		m_pCheckBox->SetText( pSaveID );

	if( m_bIdCheck )
		m_pCheckBox->SetCheck( m_bIdCheck );


	if( g_siChannelingCode > 0 || g_bHanAuth || g_bWebLoginService)
	{

		m_pEdit[0]->Enable(FALSE);
		m_pEdit[1]->Enable(FALSE);
		m_pButton[0]->Enable(FALSE);
		m_pButton[1]->Enable(TRUE);
		m_pButton[2]->Enable(FALSE);
	}
//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
	if(g_bPorteAuth )
	{
		m_pEdit[0]->Enable(FALSE);
		m_pEdit[1]->Enable(FALSE);
		m_pButton[0]->Enable(FALSE);
		m_pButton[1]->Enable(TRUE);
		m_pButton[2]->Enable(FALSE);
	}
#endif
	//KHY - 0109 - 중국 홈피 막음 - 임시
	if(pclClient->siServiceArea == ConstServiceArea_NHNChina)
		m_pButton[2]->Show(false);

	Hide();
}


void CALLBACK CNLoginDlg::StaticCallBackDialogNLoginDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNLoginDlg *pThis = (CNLoginDlg *)pControllerMgr;
	pThis->CallBackDialogNLoginDlg( nEvent, nControlID, pControl );
	
}

void CALLBACK CNLoginDlg::CallBackDialogNLoginDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	 TCHAR* pText = GetTxtFromMgr(3378);
	switch ( nControlID )
	{
	case NLOGIN_DIALOG_EDIT1:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_TAB:
				{
					SetFocus( m_pEdit[1] );
				}
				break;
			case EVENT_EDITBOX_STRING:
				{
					TCHAR TempID[ LOGIN_ERROR_MSG_LENGTH ] = TEXT("");
					TCHAR TempPassword[ LOGIN_ERROR_MSG_LENGTH ] = TEXT("");
					
					if ( _tcslen( m_pEdit[0]->GetText() ) < 1 || _tcslen( m_pEdit[1]->GetText() ) < 1 )
					{
						m_pEdit[0]->SetText(TEXT(""));
						m_pEdit[1]->SetText(TEXT(""));
                       
						Show( pText);

						m_si32Status = LOGIN_READY;
					}
					else
					{
						SetLoginData( m_pEdit[0]->GetText(), m_pEdit[1]->GetText() );

						m_pEdit[0]->SetText( TEXT("") );
						m_pEdit[1]->SetText( TEXT("") );

						m_si32Status = LOGIN_OK;
					}
				}
				break;
			}
			
		}
		break;

	case NLOGIN_DIALOG_EDIT2:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_TAB:
				{
					SetFocus( m_pEdit[0] );
				}
				break;
			case EVENT_EDITBOX_STRING:
				{
					TCHAR TempID[ LOGIN_ERROR_MSG_LENGTH ] =TEXT("");
					TCHAR TempPassword[ LOGIN_ERROR_MSG_LENGTH ] = TEXT("");

					if ( _tcslen( m_pEdit[0]->GetText() ) < 1 || _tcslen( m_pEdit[1]->GetText() ) < 1 )
					{
						m_pEdit[0]->SetText( TEXT("") );
						m_pEdit[1]->SetText( TEXT("") );
						Show( pText );

						m_si32Status = LOGIN_READY;
					}
					else
					{
						SetLoginData( m_pEdit[0]->GetText(), m_pEdit[1]->GetText() );

						m_pEdit[0]->SetText( TEXT("") );
						m_pEdit[1]->SetText( TEXT("") );

						m_si32Status = LOGIN_OK;
					}

				}
				break;
			}
			
		}
		break;

	case NLOGIN_DIALOG_BUTTON1:
		{
			switch( nEvent ) 
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( _tcslen( m_pEdit[0]->GetText() ) < 1 || _tcslen( m_pEdit[1]->GetText() ) < 1 )
					{
						m_pEdit[0]->SetText( TEXT("") );
						m_pEdit[1]->SetText( TEXT("") );
						Show( pText );

						m_si32Status = LOGIN_READY;
					}
					else
					{
						SetLoginData( m_pEdit[0]->GetText(), m_pEdit[1]->GetText() );

						m_pEdit[0]->SetText( TEXT("") );
						m_pEdit[1]->SetText( TEXT("") );

						m_si32Status = LOGIN_OK;
					}

				}
				break;
			}
		}
		break;
	
	case NLOGIN_DIALOG_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_si32Status = LOGIN_CANCEL;
					Hide();
				}
				break;
			}
		}
		break;

	case NLOGIN_DIALOG_BUTTON3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_si32Status = LOGIN_ID_REGIST;
					switch( pclClient->siServiceArea )
					{
					case ConstServiceArea_Japan:
						ShellExecute( NULL, NULL, TEXT("http://www.kunshu.jp"), NULL, NULL, SW_SHOWNORMAL );
						break;
					case ConstServiceArea_English:
						{
							// PCK : 글로벌의 경우 NHN회원과 NDI회원에 따라서 보여지는 링크페이지가 다르다. (09.03.30)			
							NTCHARString32	kID(pclclient->szID);

							if( kID.Find("@nh") >= 0 )		// NHN 회원이면
							{
								ShellExecute( NULL, NULL, TEXT("http://luminary.ijji.com/"), NULL, NULL, SW_SHOWNORMAL );
							}
							else							// NDI 회원이면
							{
								ShellExecute( NULL, NULL, TEXT("http://luminary.ndoorsgames.com/center/default.asp"), NULL, NULL, SW_SHOWNORMAL );
							}
						}
						break;
					// kkm-
					case ConstServiceArea_China:
						ShellExecute( NULL, NULL, TEXT("www.hljz.net"), NULL, NULL, SW_SHOWNORMAL );
						break;
					// -kkm
					//KHY - 0612 - 군주S 홈피 추가. 디폴트 삭제.
					case ConstServiceArea_Korea:
						ShellExecute( NULL, NULL, TEXT("http://goonzus.ndoors.com/"), NULL, NULL, SW_SHOWNORMAL ); 
						break;
					case ConstServiceArea_Taiwan:
						ShellExecute( NULL, NULL, TEXT("http://goonzu.omg.com.tw/"), NULL, NULL, SW_SHOWNORMAL );
						break;
					case ConstServiceArea_USA:
						ShellExecute( NULL, NULL, TEXT("http://luminary.ijji.com/"), NULL, NULL, SW_SHOWNORMAL );
						break;
					case ConstServiceArea_NHNChina:
						ShellExecute( NULL, NULL, TEXT("http://goonzu.omg.com.tw/"), NULL, NULL, SW_SHOWNORMAL );
						break;
					case ConstServiceArea_EUROPE:
						ShellExecute( NULL, NULL, TEXT("https://login.aeriagames.com/index.php?q=user/register"), NULL, NULL, SW_SHOWNORMAL );
						break;

					default:
						//ShellExecute( NULL, NULL, TEXT("http://www.goonzu.com"), NULL, NULL, SW_SHOWNORMAL );
						break;
					}
				}
				break;
			}
		}
		break;

	case NLOGIN_DIALOG_CHECKBOX:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					m_bIdCheck = m_pCheckBox->IsCheck();

					FILE *fp;

					fp = _tfopen(TEXT("./Config/CheckLog.dat"), TEXT("wt"));

					if( fp == NULL )
						return;

					if( m_bIdCheck )
						_ftprintf(fp, TEXT("%d"), 1 );
					else
						_ftprintf(fp, TEXT("%d"), 0 );

					fclose(fp);
				}
				break;
			}
		}
		break;
	
	}
	

}

bool CNLoginDlg::GetExistLoginID()
{
	FILE	*fp = _tfopen( TEXT("./Log.Dat"), TEXT("rt"));

	if ( fp == NULL )
	{
		return false;
	}

	_ftscanf( fp, TEXT("%s"), m_stLoginData.m_strUserID );

	fclose( fp );

	return true;
}

void CNLoginDlg::SetLoginData( TCHAR *strID, TCHAR *strPassword )
{
	if( m_bIdCheck )
	{
		FILE	*fp = _tfopen( TEXT("./Log.Dat"), TEXT("wt"));

		if ( fp == NULL )
		{
			return;
		}

		_ftprintf( fp, TEXT("%s"), strID );

		fclose( fp );
	}

	StringCchCopy( m_stLoginData.m_strUserID, MAX_PLAYER_NAME, strID );

	// 중국 버전일 경우 패스워드 소문자로 변환.
	if ( pclClient->siServiceArea == ConstServiceArea_China )
	{
		_tcslwr( strPassword );
	}

	StringCchCopy( m_stLoginData.m_strUserPassword, MAX_PLAYER_NAME, strPassword );

	return;
}

void CNLoginDlg::GetLoginData( stLoginData *pLoginData )
{
	memcpy( pLoginData, &m_stLoginData, sizeof( stLoginData ) );
	return;
}

int	CNLoginDlg::GetStatus()
{
	int si32Status = m_si32Status;
	if ( m_si32Status == LOGIN_OK )
	{
		m_si32Status = LOGIN_READY;
	}

	return si32Status;
}

void CNLoginDlg::SmallShow()
{
	return;
}

void CNLoginDlg::BigShow()
{
	return;
}

void CNLoginDlg::Show( TCHAR *strErrorMsg )
{
#ifdef USE_HANAUTH		// 한게임 인증시엔 창이 보이지 않는다.
	return;
#endif

	//KHY - 0204 -  중국 PurpleAuth 인증 모듈 - PorteAuth사용시 ID/PW창이 보이지 않는다.
#ifdef USE_PORTEAUTH
	return;
#endif

	cltClient* pclclient = (cltClient*)pclClient;

	if( g_bWebLoginService == true )	return;		// 웹 로그인 서비스 사용시 창이 보이지 않는다.
	if( g_siChannelingCode > 0)			return;		//  일본  - 채널링 서비스를 하는 곳은 모두 웹 로그인이다.
	if( g_bUSA_Auth == true )			return;		//  [진성] USA를 글로벌 이관작업으로 추가. 야는 웹 로그인이거덩. 


	// 유럽과 일본 한국 아닐때만 체크한다
		if ( false == pclclient->IsWhereServiceArea(ConstServiceArea_EUROPE | ConstServiceArea_Japan | ConstServiceArea_Korea) )
	{
		// 글로벌 자동 로그인
		if( _tcslen(pclclient->szID) > 0 && _tcslen(pclclient->szPassword) > 0 )		return;
	}

	CControllerMgr::Show( true );

	m_bFlag = true;

	if ( strErrorMsg == NULL )
	{
		if ( m_bErrorExist )
		{
			BigShow();
		}
		else
		{
			SmallShow();
		}
	}
	else
	{
		memset( m_strErrorMsg, 0, LOGIN_ERROR_MSG_LENGTH );
		m_bErrorExist = true;
		StringCchCopy( m_strErrorMsg, LOGIN_ERROR_MSG_LENGTH, strErrorMsg );
		m_pStatic->SetText( strErrorMsg );
		SetFocus( m_pEdit[1] );
	
		BigShow();
	}

	if( m_bIdCheck )
	{
		if ( GetExistLoginID() )
		{
			m_pEdit[0]->SetText(m_stLoginData.m_strUserID); // 유저 아이디
			SetFocus( m_pEdit[1] );
		}
	}
	else
	{
		// 포커스를 아이디로 바꿔준다
		SetFocus( m_pEdit[0] );
	}

	return;
}

void CNLoginDlg::Hide()
{
	CControllerMgr::Show( false );
	
	return;
}
void CNLoginDlg::ShowMiniHomePage()
{	
}
