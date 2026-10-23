/* ==========================================================================
	클래스 :		CNPersonalInfoDlg

	작성일 :		05/03/30
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		
					1. webpage를 다루는 컨트롤 부분 작업 요망!

   ========================================================================*/

#include "./NPersonalInfo.h"

//#if defined(_DEBUG) && defined(_LEON_NPERSONALINFO_)

#include "../../Common/CommonHeader.h"
#include "..\..\Client\client.h"

// Basic Include =====================================================
#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../nautohunting/NAutoHunting.h"
#include "../../InterfaceMgr/InterfaceMgr.h"


extern cltCommonLogic* pclClient;
// ==================================================================

// Changed Include ==================================================
#include "NPersonalInfo.h"
#include "../../Lib/JYLibFunction.h"
#include "Char\CharManager\CharManager.h"
#include "../../CommonLogic/MsgType-System.h"
extern CJYLibFunction g_JYLibFunction;
extern cltCommonLogic* pclClient;
// ==================================================================

#define		APRILFOOL_IMAGE_NUM		76
#define		APRILFOOL_IMAGE_NUM_J	47

CNPersonalInfoDlg::CNPersonalInfoDlg()
{
	m_bShow = false;

	m_pImageStatic_profile	 = NULL; 
	m_pEdit_intro	 = NULL; 
	m_pStatic_name	 = NULL; 
	m_pImageStatic_gomzi	 = NULL; 
	m_pEdit_name	 = NULL; 
	m_pStatic_teacher	 = NULL; 
	m_pEdit_teacher	 = NULL; 
	m_pStatic_famelevel	 = NULL; 
	m_pEdit_famelevel	 = NULL; 
	m_pStatic_country	= NULL;
	m_pEdit_country		= NULL;
	m_pImageStatic_country	= NULL;
	m_pStatic_rank	 = NULL; 
	m_pEdit_rank	 = NULL; 
	m_pStatic_greeting	 = NULL; 
	m_pEdit_greeting	 = NULL; 

	m_pStatic_NameLevel	= NULL;
	m_pEdit_NameLevel = NULL;
	m_pStatic_GuildName = NULL;
	m_pEdit_GuildName = NULL;
	m_pStatic_VillageName = NULL;
	m_pEdit_VillageName = NULL;

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;

	m_pEdit[0] = NULL;
	m_pEdit[1] = NULL;
	m_pEdit[2] = NULL;

	m_pButton_Report = NULL;

	memset(m_szAccountID,	0, sizeof(m_szAccountID));
	memset(m_szCharName,	0, sizeof(m_szCharName));
}

CNPersonalInfoDlg::~CNPersonalInfoDlg()
{
	if( m_pImageStatic_profile )  delete m_pImageStatic_profile;
	if( m_pEdit_intro )  delete m_pEdit_intro;
	if( m_pStatic_name )  delete m_pStatic_name;
	if( m_pImageStatic_gomzi )  delete m_pImageStatic_gomzi;
	if( m_pEdit_name )  delete m_pEdit_name;
	if( m_pStatic_teacher )  delete m_pStatic_teacher;
	if( m_pEdit_teacher )  delete m_pEdit_teacher;
	if( m_pStatic_famelevel )  delete m_pStatic_famelevel;
	if( m_pEdit_famelevel )  delete m_pEdit_famelevel;

	SAFE_DELETE( m_pStatic_country );
	SAFE_DELETE( m_pEdit_country );
	SAFE_DELETE( m_pImageStatic_country );

	if( m_pStatic_rank )  delete m_pStatic_rank;
	if( m_pEdit_rank )  delete m_pEdit_rank;
	if( m_pStatic_greeting )  delete m_pStatic_greeting;
	if( m_pEdit_greeting )  delete m_pEdit_greeting;

	SAFE_DELETE( m_pStatic_NameLevel );
	SAFE_DELETE( m_pEdit_NameLevel );
	SAFE_DELETE( m_pStatic_GuildName );
	SAFE_DELETE( m_pEdit_GuildName );
	SAFE_DELETE( m_pStatic_VillageName );
	SAFE_DELETE( m_pEdit_VillageName );

	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);

	SAFE_DELETE(m_pEdit[0]);
	SAFE_DELETE(m_pEdit[1]);
	SAFE_DELETE(m_pEdit[2]);

	SAFE_DELETE(m_pButton_Report);
	////---------------------------------
	//// HTML 을 해제한다.
	////---------------------------------
	////==================================
	//HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_GOMZI_70 );
	//if( hwnd )
	//{
	//	DoPageAction(hwnd, WEBPAGE_STOP);
	//	UnEmbedBrowserObject(hwnd);
	//}			
	//hwnd = GetDlgItem( hDlg, IDC_STATIC_PROFILE );
	//if( hwnd )
	//{
	//	DoPageAction(hwnd, WEBPAGE_STOP);
	//	UnEmbedBrowserObject(hwnd);
	//}			
	//hwnd = GetDlgItem( hDlg, IDC_STATIC_INTRO );
	//if( hwnd )
	//{
	//	DoPageAction(hwnd, WEBPAGE_STOP);
	//	UnEmbedBrowserObject(hwnd);
	//}			
}

void CNPersonalInfoDlg::Create()
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NPersonalInfoDlg/DLG_NPersonalInfo.ddf"));

		file.CreatePopupDialog( this, NPERSONMALINFO_DLG, TEXT("dialog_personal"),StaticNPersonalInfoDlgProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_personal") );

		if(siTmp < 0 )
			siTmpWidth = DLG_BIG_POS_X;
		else
			siTmpWidth = file.m_control[ siTmp  ].width;

		if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
		{
			this->MovePos(DLG_BIG_POS_X - siTmpWidth, DLG_BIG_POS_Y);
		}
		else
		{
			this->MovePos(DLG_SMALL_POS_X - siTmpWidth, DLG_SMALL_POS_Y);
		}

		//m_pImageStatic_profile	 = new 	CImageStatic	( this )	;
		//m_pEdit_intro	 = new 	CEdit	( this )	;
		m_pStatic_name	 = new 	CStatic	( this )	;
		//m_pImageStatic_gomzi	 = new 	CImageStatic	( this )	;
		m_pEdit_name	 = new 	CEdit	( this )	;
		m_pStatic_teacher	 = new 	CStatic	( this )	;
		m_pEdit_teacher	 = new 	CEdit	( this )	;
		m_pStatic_famelevel	 = new 	CStatic	( this )	;
		m_pEdit_famelevel	 = new 	CEdit	( this )	;

		m_pStatic_country		= new	CStatic	( this )	;
		m_pEdit_country			= new	CEdit	( this )	;
		m_pImageStatic_country	= new	CImageStatic( this );

		m_pStatic_rank	 = new 	CStatic	( this )	;
		m_pEdit_rank	 = new 	CEdit	( this )	;
		m_pStatic_greeting	 = new 	CStatic	( this )	;
		m_pEdit_greeting	 = new 	CEdit	( this )	;

		m_pStatic_NameLevel = new CStatic (this);
		m_pEdit_NameLevel = new CEdit (this);
		m_pStatic_GuildName = new CStatic (this);
		m_pEdit_GuildName = new CEdit (this);
		m_pStatic_VillageName = new CStatic (this);
		m_pEdit_VillageName = new CEdit (this);

		NEWCSTATIC(m_pStatic[0]);
		NEWCSTATIC(m_pStatic[1]);
		NEWCSTATIC(m_pStatic[2]);

		NEWCEDIT(m_pEdit[0]);
		NEWCEDIT(m_pEdit[1]);
		NEWCEDIT(m_pEdit[2]);
		if ( pclClient->IsCountrySwitch(Switch_AutoHuntingReport) == true )
		{
			NEWCBUTTON(m_pButton_Report);
		}

		//file.CreateControl( m_pImageStatic_profile, CNPERSONMALINFO_DLG_IMAGESTATIC_PROFILE, TEXT("imagestatic_profile") );
		//file.CreateControl( m_pEdit_intro, CNPERSONMALINFO_DLG_EDITBOX_INTRO, TEXT("editbox_intro") );
		file.CreateControl( m_pStatic_name, CNPERSONMALINFO_DLG_STATICTEXT_NAME, TEXT("statictext_name") );
		//file.CreateControl( m_pImageStatic_gomzi, CNPERSONMALINFO_DLG_IMAGESTATIC_GOMZI, TEXT("imagestatic_gomzi") );
		file.CreateControl( m_pEdit_name, CNPERSONMALINFO_DLG_EDITBOX_NAME, TEXT("editbox_name") );
		file.CreateControl( m_pStatic_teacher, CNPERSONMALINFO_DLG_STATICTEXT_TEACHER, TEXT("statictext_teacher") );
		file.CreateControl( m_pEdit_teacher, CNPERSONMALINFO_DLG_EDITBOX_TEACHER, TEXT("editbox_teacher") );
		file.CreateControl( m_pStatic_famelevel, CNPERSONMALINFO_DLG_STATICTEXT_FAMELEVEL, TEXT("statictext_famelevel") );
		file.CreateControl( m_pEdit_famelevel, CNPERSONMALINFO_DLG_EDITBOX_FAMELEVEL, TEXT("editbox_famelevel") );
//KHY - 0608 -NHN 국가 삭제.
		if ( pclClient->siServiceArea != ConstServiceArea_Korea )
		{
			file.CreateControl( m_pStatic_country , CNPERSONMALINFO_DLG_STATICTEXT_COUNTRY, TEXT( "statictext_country") );
			file.CreateControl( m_pEdit_country , CNPERSONMALINFO_DLG_EDITBOX_COUNTRY, TEXT( "editbox_country") );
		}
		file.CreateControl( m_pImageStatic_country , CNPERSONMALINFO_DLG_IMAGESTATIC_COUNTRY, TEXT( "imagestatic_country") );
		file.CreateControl( m_pStatic_rank, CNPERSONMALINFO_DLG_STATICTEXT_RANK, TEXT("statictext_rank") );
		file.CreateControl( m_pEdit_rank, CNPERSONMALINFO_DLG_EDITBOX_RANK, TEXT("editbox_rank") );
		file.CreateControl( m_pStatic_greeting, CNPERSONMALINFO_DLG_STATICTEXT_GREETING, TEXT("statictext_greeting") );
		file.CreateControl( m_pEdit_greeting, CNPERSONMALINFO_DLG_EDITBOX_GREETING, TEXT("editbox_greeting") );


		file.CreateControl( m_pStatic[0], CNPERSONMALINFO_DLG_STATIC1, TEXT("statictext_NONAME1"));
		file.CreateControl( m_pStatic[1], CNPERSONMALINFO_DLG_STATIC2, TEXT("statictext_NONAME2"));
		file.CreateControl( m_pStatic[2], CNPERSONMALINFO_DLG_STATIC3, TEXT("statictext_NONAME3"));

		file.CreateControl( m_pEdit[0], CNPERSONMALINFO_DLG_EDIT1, TEXT("editbox_NONAME1"));
		file.CreateControl( m_pEdit[1], CNPERSONMALINFO_DLG_EDIT2, TEXT("editbox_NONAME2"));
		file.CreateControl( m_pEdit[2], CNPERSONMALINFO_DLG_EDIT3, TEXT("editbox_NONAME3"));
		if ( pclClient->IsCountrySwitch(Switch_AutoHuntingReport) == true )
		{
			file.CreateControl( m_pButton_Report, CNPERSONMALINFO_DLG_BUTTON_REPORT, TEXT("button_report"));
		}

		file.CreateControl( m_pStatic_NameLevel,   CNPERSONMALINFO_DLG_STATICTEXT_NAMELEVEL,   TEXT("statictext_NameLevel"));
		file.CreateControl( m_pEdit_NameLevel,	   CNPERSONMALINFO_DLG_EDITBOX_NAMELEVEL,	   TEXT("editbox_NameLevel"));
		file.CreateControl( m_pStatic_GuildName,   CNPERSONMALINFO_DLG_STATICTEXT_GUILDNAME,   TEXT("statictext_guildname"));
		file.CreateControl( m_pEdit_GuildName,     CNPERSONMALINFO_DLG_EDITBOX_GUILDNAME,      TEXT("editbox_guildname"));
		file.CreateControl( m_pStatic_VillageName, CNPERSONMALINFO_DLG_STATICTEXT_VILLAGENAME, TEXT("statictext_VillageName"));
		file.CreateControl( m_pEdit_VillageName,   CNPERSONMALINFO_DLG_EDITBOX_VILLAGENAME,    TEXT("editbox_VillageName"));
	}
	else
	{
		DeleteDialog();
	}
}

void CALLBACK CNPersonalInfoDlg::StaticNPersonalInfoDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNPersonalInfoDlg *pThis = (CNPersonalInfoDlg*) pControllerMgr;
	pThis->NPersonalInfoMsgDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNPersonalInfoDlg::NPersonalInfoMsgDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case CNPERSONMALINFO_DLG_BUTTON_REPORT:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
                ReportUser();
		}
	}
}

void CNPersonalInfoDlg::set(TCHAR * szPersonalName,TCHAR * szFatherName , SI32 ChangeNum ,
						   SI16 Level,SI16 FameLevel , SI32 Country, bool ShowCountry, 
						   /*TCHAR * szSchoolName,*/ TCHAR * szAccountID,
						   SI32 wealthrank,cltMoney money, TCHAR* greeting, SI32 PVPWinNumber, SI32 PVPLoseNumber, TCHAR* PVPWinHighLevelUserName,
						   SI32 siNameLevel, TCHAR* szGuildName, SI32 siVillageUnique)
{
	cltClient* pclclient = (cltClient*)pclClient;

	TCHAR strTitle[256] = TEXT("");
	TCHAR strBuf[256] = TEXT("");
	TCHAR buf[256] = TEXT("");

	memset( strTitle, 0, sizeof(strTitle) );
	TCHAR* pText = GetTxtFromMgr(3396);
	StringCchPrintf(strTitle, sizeof(strTitle), pText,szPersonalName);
	this->SetTitle( strTitle ); 

	memset(strBuf,0,sizeof(strBuf));
	pText = GetTxtFromMgr(3397);
	StringCchPrintf(strBuf, sizeof(strBuf), pText,szPersonalName,Level);
	m_pEdit_name->SetText( strBuf );
	StringCchCopy( m_szCharName, MAX_PLAYER_NAME, szPersonalName );

	memset(strBuf,0,sizeof(strBuf));
	pText = GetTxtFromMgr(3398);
	StringCchPrintf(strBuf, sizeof(strBuf), pText,szFatherName,ChangeNum);
	m_pEdit_teacher->SetText( strBuf );

	memset(strBuf,0,sizeof(strBuf));
	StringCchPrintf(strBuf, sizeof(strBuf), TEXT("%d"),FameLevel);
	m_pEdit_famelevel->SetText( strBuf );

//KHY - 0608 -NHN 국가 삭제.
	if ( pclClient->siServiceArea != ConstServiceArea_Korea )
	{
		if ( ShowCountry == TRUE )
		{
			if ( pclClient->siServiceArea == ConstServiceArea_English )
			{
				memset(strBuf,0,sizeof(strBuf));
				StringCchCopy(strBuf, sizeof(strBuf), pclClient->pclCountryManager->GetCountryFromUnique( Country ) );
				m_pEdit_country->SetText( strBuf );
				m_pImageStatic_country->SetFileName( TEXT("GImg/National_flag.spr") );
				
				if( pclClient->pclEventTimeManager->InEventTime("AprilFoolEvent", &pclclient->sTime) )
				{
					m_pImageStatic_country->SetFontIndex( APRILFOOL_IMAGE_NUM );
				}	
				else
				{
					m_pImageStatic_country->SetFontIndex( Country );
				}
			}
			else if ( pclClient->siServiceArea == ConstServiceArea_NHNChina )
			{
				memset(strBuf,0,sizeof(strBuf));
				StringCchCopy(strBuf, sizeof(strBuf), pclClient->pclCountryManager->GetCountryFromUnique( Country ) );
				m_pEdit_country->SetText( strBuf );
				m_pImageStatic_country->SetFileName( TEXT("GImg/National_flag.spr") );
				
				if( pclClient->pclEventTimeManager->InEventTime("AprilFoolEvent", &pclclient->sTime) )
				{
					m_pImageStatic_country->SetFontIndex( APRILFOOL_IMAGE_NUM );
				}
				else
				{
					m_pImageStatic_country->SetFontIndex( Country );
				}
				
			}
			else if ( pclClient->siServiceArea == ConstServiceArea_EUROPE )
			{
				memset(strBuf,0,sizeof(strBuf));
				StringCchCopy(strBuf, sizeof(strBuf), pclClient->pclCountryManager->GetCountryFromUnique( Country ) );
				m_pEdit_country->SetText( strBuf );
				m_pImageStatic_country->SetFileName( TEXT("GImg/National_flag.spr") );
				
				if( pclClient->pclEventTimeManager->InEventTime("AprilFoolEvent", &pclclient->sTime) )
				{
					m_pImageStatic_country->SetFontIndex( APRILFOOL_IMAGE_NUM );
				}
				else
				{
					m_pImageStatic_country->SetFontIndex( Country );
				}
			}
			else if ( pclClient->siServiceArea == ConstServiceArea_Japan )
			{
				memset(strBuf,0,sizeof(strBuf));
				StringCchCopy(strBuf, sizeof(strBuf), pclClient->pclCountryManagerJapan->GetCountryFromUnique( Country ) );
				m_pEdit_country->SetText( strBuf );
				m_pImageStatic_country->SetFileName( TEXT("GImg/JapanCity_flag.spr") );
				
				if( pclClient->pclEventTimeManager->InEventTime("AprilFoolEvent", &pclclient->sTime) )
				{
					m_pImageStatic_country->SetFontIndex( APRILFOOL_IMAGE_NUM_J );
				}
				else
				{
					m_pImageStatic_country->SetFontIndex( Country );
				}
			}
			m_pImageStatic_country->Show( true );
		}
		else
		{
			m_pEdit_country->SetText( TEXT("") );
			m_pImageStatic_country->Show( false );
		}
		memset(strBuf, 0, sizeof(strBuf));
	}

	// 인사말
	m_pEdit_greeting->SetText(greeting);

	memset(strBuf,0,sizeof(strBuf));
	if ( wealthrank < 0 ) wealthrank = 0 ;
	if ( money.itMoney < 0 ) money.itMoney = 0 ;
	pText = GetTxtFromMgr(4837);
	g_JYLibFunction.SetNumUnit(money.itMoney,buf, sizeof(buf),pText);

	//cyj "%d위(%s)" 를 텍스트 메니저로 뺌
	pText = GetTxtFromMgr(5733);
	StringCchPrintf(strBuf, sizeof(strBuf), pText,wealthrank,buf);
	m_pEdit_rank->SetText( strBuf );

	// ACCOUNT 를 설정한다.
	if(_tcslen(m_szAccountID)>0 && _tcscmp(m_szAccountID, szAccountID) == 0)
	{
		//hWnd = GetDlgItem(m_hPersoanlInfoMsgDlg, IDC_STATIC_INTRO);
		//if(hWnd)	DoPageAction(hWnd, WEBPAGE_REFRESH);

		// leon todo - webpage 재작업 필요!!
		if(m_pEdit_intro)
			m_pEdit_intro->SetText( TEXT("") );
	}
	else if(_tcslen(szAccountID)>=2)
	{
		// ID를 복사한다.
		StringCchCopy(m_szAccountID, MAX_PLAYER_NAME, szAccountID);

		// GOMZI 70%
		TCHAR GomziURL[MAX_PATH];

		if( szAccountID[ 1 ] == '_' && ( szAccountID[ 0 ] == 's' || szAccountID[ 0 ] == 'S' ) ) {
			// sbs

			StringCchPrintf(GomziURL, MAX_PATH, TEXT("http://goonzucz.sbs.co.kr/mh/image/darak/s_mydk/%c/%c/%s/%s_70.html"), szAccountID[2], szAccountID[3], &szAccountID[2], &szAccountID[2] );

		} else {
			// goonzu

			StringCchPrintf(GomziURL, MAX_PATH, TEXT("http://ccomzi.ndoors.com/mh/image/darak/mydk/%c/%c/%s/%s_70.html"), szAccountID[0], szAccountID[1], szAccountID, szAccountID);
		}
		
		//pclClient->GetCZIntroUrl( GomziURL, szAccountID );
		//
		//hWnd = GetDlgItem(m_hPersoanlInfoMsgDlg, IDC_STATIC_INTRO);
		//if(hWnd)	DisplayHTMLPage(hWnd, GomziURL);
		// leon todo - webpage 재작업 필요!!
		if(m_pEdit_intro)
			m_pEdit_intro->SetText( TEXT("") );
	}

	// PVP-
	StringCchPrintf(strBuf, 256, TEXT("%d"), PVPWinNumber );
	m_pEdit[0]->SetText( strBuf );

	StringCchPrintf(strBuf, 256, TEXT("%d"), PVPLoseNumber );
	m_pEdit[1]->SetText( strBuf );

	m_pEdit[2]->SetText( PVPWinHighLevelUserName );
	
	// pck : 명칭 레벨, 길드 이름, 마을 이름 추가 (08.11.05)
	StringCchPrintf(strBuf, 256, TEXT("%d"), siNameLevel);
	m_pEdit_NameLevel->SetText(strBuf);

	m_pEdit_GuildName->SetText(szGuildName);

	TCHAR* szVillageName = pclClient->pclVillageManager->GetName(siVillageUnique);
	m_pEdit_VillageName->SetText(szVillageName);

	Show();
	return;
}

void CNPersonalInfoDlg::Show()
{
	m_bShow = true;
	CControllerMgr::Show( true );
}

void CNPersonalInfoDlg::Hide()
{
	m_bShow = false;
	CControllerMgr::Show( false );
}

bool CNPersonalInfoDlg::IsShow()
{
	return m_bShow;
}
void CNPersonalInfoDlg::ReportUser()
{
	if ( pclClient->IsCountrySwitch(Switch_AutoHuntingReport) == false )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	cltCharClient* pclchar = (cltCharClient*)pclclient->pclCM->CR[1];

	if ( pclchar->pclCI->clIP.GetLevel() < REPORT_MINLEVEL )
	{
		TCHAR* pTitle = GetTxtFromMgr(2037);
		TCHAR* pText = GetTxtFromMgr(2038);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return;
	}

	TCHAR name[MAX_PLAYER_NAME] = TEXT("");
	TCHAR buffer[256] = TEXT("");
	TCHAR buffer2[256] = TEXT("");

	StringCchCopy( name, MAX_PLAYER_NAME, m_pEdit_name->GetText() );
	if ( _tcslen(name) <= 0 || !_tcscmp(name,TEXT("")) )
	{
		TCHAR* pTitle = GetTxtFromMgr(567);
		TCHAR* pText = GetTxtFromMgr(2039);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return;
	}

	TCHAR* pText = GetTxtFromMgr(2040);
	StringCchPrintf(buffer, 256, pText, m_szCharName);
	pText = GetTxtFromMgr(2041);
	TCHAR min[128]= TEXT(" ") ;
	StringCchPrintf(min, 128, TEXT("%d"),NOSUSPICTION_PRISON_TIME);
	Nsprintf(buffer2,pText,TEXT("user"),  m_szCharName  ,TEXT("min"), min   ,NULL);

	stMsgBox MsgBox;
	MsgBox.Set( this, buffer, buffer2, MSGBOX_TYPE_YESNO, 0 );

	BYTE TempBuffer[ 128 ];
	StringCchCopy( (TCHAR*)TempBuffer, MAX_PLAYER_NAME, m_szCharName );
	pclclient->SetMsgBox( &MsgBox, TempBuffer, MAX_PLAYER_NAME );
}

void CNPersonalInfoDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{
				cltGameMsgRequest_AutoHunting clinfo((TCHAR*)pData);
				cltMsg clMsg(GAMEMSG_REQUEST_AUTOHUNTING, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			else
				return;
		}
		break;
	}
}

//#endif
