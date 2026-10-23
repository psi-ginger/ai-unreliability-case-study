#include "../Client/NInterface/NMakeChar/NMakeChar.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"


#include "..\..\Common\JWLib\IsCorrectID\IsCorrectID.h"
#include "../Client/NInterface/NSelectChar/NSelectChar.h"
#include "AbuseFilter/AbuseFilter.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;

CMakeCharInfo makecharinfo[] =
{
		SELECT_CHAR1, KIND_HERO3, 0, 20,
		SELECT_CHAR2, KIND_HEROINE3, 20, 20,
		SELECT_CHAR3, KIND_HERO4, 40, 20,
		SELECT_CHAR4, KIND_HEROINE4, 60, 20,
};

#define CHAR_EFFECT_LOOP_FRAME_START 0
#define CHAR_EFFECT_LOOP_FRAME_END 9
#define CHAR_EFFECT_FLASH_FRAME_START 10
#define CHAR_EFFECT_FLASH_FRAME_END 12
#define CHAR_EFFECT_FLASH_FRAME_NUM	3


CNMakeCharDlg::CNMakeCharDlg() : CDialogController()
{
	m_siChar_flag	= 0;

	m_dwStratClock	= 0;
	m_dwLastClock	= 0;

	m_pEditName = NULL;

	m_pButton_Char[0] = NULL;
	m_pButton_Char[1] = NULL;
	m_pButton_Char[2] = NULL;
	m_pButton_Char[3] = NULL;

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;

	for(SI32 i=0;i<5;++i)
	{		
		m_pStatic[i] = NULL;
	}

	m_bShowCharMakeWindow = false;
}

CNMakeCharDlg::~CNMakeCharDlg()
{
	//	SAFE_DELETE(m_pImageStatic);
	SAFE_DELETE(m_pEditName);

	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pStatic[3]);
	SAFE_DELETE(m_pStatic[4]);

	SAFE_DELETE(m_pButton_Char[0]);
	SAFE_DELETE(m_pButton_Char[1]);
	SAFE_DELETE(m_pButton_Char[2]);
	SAFE_DELETE(m_pButton_Char[3]);

	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[2]);

	SAFE_DELETE(m_pStaticCharname);
}

void CNMakeCharDlg::Create()
{
	if( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/DLG_NMakeChar.ddf"));
		file.CreatePopupDialog( this, NMAKECHAR_DLG, TEXT("dialog_MakeNewChar"), StaticCallBackDialogNMakeCharDlg,false );

		cltClient *pclclient = (cltClient *)pclClient;

		switch( pclClient->siServiceArea )
		{
		case ConstServiceArea_China:
		case ConstServiceArea_Japan:
		case ConstServiceArea_Korea:
		case ConstServiceArea_Taiwan:
		case ConstServiceArea_USA:
		case ConstServiceArea_English:
		case ConstServiceArea_NHNChina:
		case ConstServiceArea_EUROPE:
			{
				if ( !g_bFullScreen )		MovePos( 50 + 48, -8 );
				else						MovePos( 50 + 80, 0 + 40);
			}
			break;

		default:
			{
				if( g_bFullScreen)			MovePos( 80 , 40 );
				else						MovePos( 55 , 30 );
			}
			break;
		}

		m_pEditName = new CEdit( this );

		m_pButton[0] 		= new CButton( this );
		m_pButton[1] 		= new CButton( this );
		m_pButton[2] 		= new CButton( this );

		m_pButton_Char[0] 	= new CButton( this );
		m_pButton_Char[1] 	= new CButton( this );
		m_pButton_Char[2] 	= new CButton( this );		
		m_pButton_Char[3]	= new CButton( this );

		NEWCIMAGESTATIC(m_pStatic[0]);
		NEWCIMAGESTATIC(m_pStatic[1]);
		NEWCIMAGESTATIC(m_pStatic[2]);
		NEWCIMAGESTATIC(m_pStatic[3]);
		NEWCIMAGESTATIC(m_pStatic[4]);

		m_pStaticCharname	= new CStatic( this );

		file.CreateControl( m_pEditName, NMAKECHAR_DIALOG_EDIT, TEXT("editbox_Name"));  

		file.CreateControl( m_pButton[0], NMAKECHAR_DIALOG_BUTTON1, TEXT("button_Make"));  		
		file.CreateControl( m_pButton[2], NMAKECHAR_DIALOG_BUTTON3, TEXT("button_Back"));

		switch( pclClient->siServiceArea )
		{
		case ConstServiceArea_China:
		case ConstServiceArea_Japan:
		case ConstServiceArea_Korea:
		case ConstServiceArea_Taiwan:
		case ConstServiceArea_USA:
		case ConstServiceArea_English:
		case ConstServiceArea_NHNChina:
		case ConstServiceArea_EUROPE:
			{
				file.CreateControl( m_pButton_Char[0], NMAKECHAR_DIALOG_BUTTON_CHAR1, TEXT("button_easthero"));  		
				file.CreateControl( m_pButton_Char[1], NMAKECHAR_DIALOG_BUTTON_CHAR2, TEXT("button_eastheroine"));  		
				file.CreateControl( m_pButton_Char[2], NMAKECHAR_DIALOG_BUTTON_CHAR3, TEXT("button_westhero"));  		
				file.CreateControl( m_pButton_Char[3], NMAKECHAR_DIALOG_BUTTON_CHAR4, TEXT("button_westheroine"));  		
			}
			break;

		default:
			{
				file.CreateControl( m_pButton[1], NMAKECHAR_DIALOG_BUTTON2, TEXT("button_Cancel"));
				file.CreateControl( m_pStatic[0], NMAKECHAR_DIALOG_STATIC1, TEXT("imagestatic_NONAME1"));
				file.CreateControl( m_pStatic[1], NMAKECHAR_DIALOG_STATIC2, TEXT("imagestatic_NONAME2"));
				file.CreateControl( m_pStatic[2], NMAKECHAR_DIALOG_STATIC3, TEXT("imagestatic_NONAME3"));
				file.CreateControl( m_pStatic[3], NMAKECHAR_DIALOG_STATIC4, TEXT("imagestatic_NONAME4"));
				file.CreateControl( m_pStatic[4], NMAKECHAR_DIALOG_STATIC5, TEXT("imagestatic_NONAME5"));
			}
			break;
		}

		file.CreateControl( m_pStaticCharname, NMAKECHAR_DIALOG_BUTTON3, TEXT("statictext_Charname"));
		
		m_siChar_flag = SELECT_CHAR1;
		
		// ĳ���� �̸��� �ִ� 16�� ������ �Է��Ҽ��ְԼ���. (PCK - 07.11.08)
		// �ɸ��� �̸� 8�� ���� ���� �ֵ��� ���� - by LEEKH 2008-10-30 
		// ������ NULL�� ���ؼ� �����ϳ��� �� �Ҵ��Ѵ�.
		m_pEditName->SetMaxEditTextLength( 8 * 2 + 1 );

		switch( pclClient->siServiceArea )
		{
		case ConstServiceArea_China:
		case ConstServiceArea_Japan:
		case ConstServiceArea_Korea:
		case ConstServiceArea_Taiwan:
		case ConstServiceArea_USA:
		case ConstServiceArea_English:
		case ConstServiceArea_NHNChina:
		case ConstServiceArea_EUROPE:
			{
				m_pStaticCharname->SetFontHeight(15);
				m_pStaticCharname->SetFontColor(RGB(24, 42, 89));
				m_pStaticCharname->SetFontWeight(FW_BOLD);
				m_pStaticCharname->SetTextPrintOption(DT_RIGHT);
				TCHAR* pInputName;
				pInputName = GetTxtFromMgr(5248);
				m_pStaticCharname->SetText(pInputName, m_pStaticCharname->GetTextPrintOption());
			}
			break;

		default:
			{
				ShowCharMakeWindow( false );
			}
			break;
		}

		SetActionTimer(1000);
		SetTimerOnOff(true);

		// �⺻ �̹��� ����
		SelectSlot(-1);
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNMakeCharDlg::StaticCallBackDialogNMakeCharDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNMakeCharDlg *pThis = (CNMakeCharDlg *)pControllerMgr;
	pThis->CallBackDialogNMakeCharDlg( nEvent, nControlID, pControl );

}

void CALLBACK CNMakeCharDlg::CallBackDialogNMakeCharDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;


	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NMAKECHAR_DIALOG_BUTTON1: // �����
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{	
					if(pclClient->IsCountrySwitch(Switch_Sound))
					{
						pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_CLICK_BUTTON")), 0, 0);
					}
					
					m_pEditName->GetText();

					if ( m_pEditName == NULL || _tcslen( m_pEditName->GetText() ) < 1 )
					{
						return;
					}


					size_t siMaxNameLength = 0;
					if ( pclClient->siServiceArea == ConstServiceArea_China )
					{
						siMaxNameLength = 6 * 2;
					}
					else
					{
						siMaxNameLength = 8 * 2;
					}

					if ( _tcslen( m_pEditName->GetText() ) > siMaxNameLength )
					{
						if(pclClient->IsCountrySwitch(Switch_Sound))
						{
							pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_OPEN_WARNING")), 0, 0);
						}
						
						if (pclclient->m_pDialog[ NOTIFYMSG_DLG ] != NULL)
						{
							TCHAR *pTitle = GetTxtFromMgr(836);
							TCHAR *pText = GetTxtFromMgr(5748);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
						}
						return;
					}

					//cyj ĳ���͸���� �弳���� ����
					TCHAR buf[128];
					StringCchPrintf(buf, 128, m_pEditName->GetText());
#ifdef _IAF_EXPORT
					if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
					{
						TCHAR ResultBuf[1024] = "";
						IAF::Filtering( buf, ResultBuf, sizeof(ResultBuf), IAF::FILTER_CHARACTER );
						memcpy( buf, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
					}
#else
					pclclient->m_pCAbuseFilter->CheckAbuse(buf);
#endif

					if ( ! IsCorrectID( buf ) )
					{
						TCHAR* pTitle = GetTxtFromMgr(1116);
						TCHAR* pText = GetTxtFromMgr(1115);

						stMsgBox MsgBox;              //by taeyang

						MsgBox.Set( this, pTitle ,pText,  MSGBOX_TYPE_OK, 1 );			

						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof(SI32);					
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
						return;
					}

					TCHAR* pText = GetTxtFromMgr(1117);
					TCHAR* pTitle = GetTxtFromMgr(1118);

					stMsgBox MsgBox;              //by taeyang

					MsgBox.Set( this, pTitle ,pText,  MSGBOX_TYPE_YESNO, 0 );			

					BYTE TempBuffer[ 128 ];
					SI16 Size = sizeof(SI32);					
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
					
				}
				break;
			}
		}
		break;
	case NMAKECHAR_DIALOG_BUTTON2: // ���
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(pclClient->IsCountrySwitch(Switch_Sound))
					{
						pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_CLICK_BUTTON")), 0, 0);
					}
					
					if( pclClient->siServiceArea != ConstServiceArea_Japan		&&
						pclClient->siServiceArea != ConstServiceArea_Korea		&&
						pclClient->siServiceArea != ConstServiceArea_Taiwan		&&
						pclClient->siServiceArea != ConstServiceArea_USA		&&
						pclClient->siServiceArea != ConstServiceArea_English	&&
						pclClient->siServiceArea != ConstServiceArea_NHNChina	&&
						pclClient->siServiceArea != ConstServiceArea_EUROPE
					)
						ShowCharMakeWindow(false);
				}
				break;
			}
		}
		break;
	case NMAKECHAR_DIALOG_BUTTON3: // �ݱ�
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(pclClient->IsCountrySwitch(Switch_Sound))
					{
						pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_CLICK_BUTTON")), 0, 0);
					}
					
					if( pclclient->m_pDialog[ NSELECTCHAR_DLG ] && ((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->IsShow() == false )
					{
						((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->Show();
					}

					DeleteDialog();

				}
				break;
			}
		}
		break;

	case NMAKECHAR_DIALOG_BUTTON_CHAR1:
	case NMAKECHAR_DIALOG_BUTTON_CHAR2:
	case NMAKECHAR_DIALOG_BUTTON_CHAR3:
	case NMAKECHAR_DIALOG_BUTTON_CHAR4:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 siIndex = nControlID - NMAKECHAR_DIALOG_BUTTON_CHAR1;

					SelectSlot(siIndex);

					switch( nControlID )
					{
					case NMAKECHAR_DIALOG_BUTTON_CHAR1:
						{
							if(pclClient->IsCountrySwitch(Switch_Sound))
							{
								pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HERO")), 0, 0);
							}
							m_siChar_flag = SELECT_CHAR1;
						}
						break;
					case NMAKECHAR_DIALOG_BUTTON_CHAR2:
						{
							if(pclClient->IsCountrySwitch(Switch_Sound))
							{
								pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HEROINE")), 0, 0);
							}
							m_siChar_flag = SELECT_CHAR2;
						}
						break;
					case NMAKECHAR_DIALOG_BUTTON_CHAR3:
						{
							if(pclClient->IsCountrySwitch(Switch_Sound))
							{
								pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HERO2")), 0, 0);
							}
							m_siChar_flag = SELECT_CHAR3;
						}
						break;
					case NMAKECHAR_DIALOG_BUTTON_CHAR4:
						{
							if(pclClient->IsCountrySwitch(Switch_Sound))
							{
								pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HEROINE2")), 0, 0);
							}
							m_siChar_flag = SELECT_CHAR4;
						}
						break;
					}
				}
				break;
			}
		}
		break;

	case NMAKECHAR_DIALOG_STATIC1:
	case NMAKECHAR_DIALOG_STATIC2:
	case NMAKECHAR_DIALOG_STATIC3:
	case NMAKECHAR_DIALOG_STATIC4:
		{
			if(m_bShowCharMakeWindow) break; // ĳ���� ���� â�� �������� �ٷ� ����

			SI32 siIndex = nControlID - NMAKECHAR_DIALOG_STATIC1;
			SI32 siCharIndex = 0;

			switch(siIndex)
			{
			case 0:		siCharIndex = SELECT_CHAR1;		break;
			case 1:		siCharIndex = SELECT_CHAR2;		break;
			case 2:		siCharIndex = SELECT_CHAR3;		break;
			case 3:		siCharIndex = SELECT_CHAR4;		break;
			}

			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					if( pclClient->siServiceArea != ConstServiceArea_Japan
						&& pclClient->siServiceArea != ConstServiceArea_Korea
						&& pclClient->siServiceArea != ConstServiceArea_Taiwan
						&& pclClient->siServiceArea != ConstServiceArea_USA
						&& pclClient->siServiceArea != ConstServiceArea_English
						&& pclClient->siServiceArea != ConstServiceArea_NHNChina
						&& pclClient->siServiceArea != ConstServiceArea_EUROPE
					)
					{
						if(m_siChar_flag == siCharIndex)
							ShowCharMakeWindow(true, siCharIndex);
					}

					m_siChar_flag = siCharIndex;
					SelectSlot(siIndex);
				}
				break;
			case EVENT_IMAGESTATIC_DBCLICKED:
				{
					if( pclClient->siServiceArea != ConstServiceArea_Japan
						&& pclClient->siServiceArea != ConstServiceArea_Korea
						&& pclClient->siServiceArea != ConstServiceArea_Taiwan
						&& pclClient->siServiceArea != ConstServiceArea_USA
						&& pclClient->siServiceArea != ConstServiceArea_English
						&& pclClient->siServiceArea != ConstServiceArea_NHNChina
						&& pclClient->siServiceArea != ConstServiceArea_EUROPE
					)
						ShowCharMakeWindow(true, siCharIndex);

					m_siChar_flag = siCharIndex;
					SelectSlot(siIndex);
				}
				break;
			case EVENT_IMAGESTATIC_MOUSEMOVE:
				{

				}
				break;
			}
		}
		break;
	}
}

void CNMakeCharDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	SI32 i;

	if( pclClient->siServiceArea != ConstServiceArea_Japan
		&& pclClient->siServiceArea != ConstServiceArea_China
		&& pclClient->siServiceArea != ConstServiceArea_Korea
		&& pclClient->siServiceArea != ConstServiceArea_Taiwan
		&& pclClient->siServiceArea != ConstServiceArea_USA
		&& pclClient->siServiceArea != ConstServiceArea_English 
		&& pclClient->siServiceArea != ConstServiceArea_NHNChina
		&& pclClient->siServiceArea != ConstServiceArea_EUROPE
	)
	{
		m_dwStratClock	= pclClient->CurrentClock;

		if(  m_dwStratClock - m_dwLastClock > 100 )
		{
			for(i=0; i<4; i++)
			{
				if( i == m_siSelectedSlot )
				{
					SI32 siFontIndex = m_pStatic[i]->GetFontIndex();
					siFontIndex++;
					if(siFontIndex > 12)						siFontIndex = 0;
					if(siFontIndex >= 0 && siFontIndex <= 10)	
						siFontIndex %= 10;

					m_pStatic[i]->SetFontIndex(siFontIndex);
				}
				else
				{
					if(m_pStatic[i]->IsMouseOver())
					{
						SI32 siFontIndex = m_pStatic[i]->GetFontIndex();
						siFontIndex++;
						if(siFontIndex >= 3)		siFontIndex = 3;
						m_pStatic[i]->SetFontIndex(siFontIndex);
					}
					else
					{
						m_pStatic[i]->SetFontIndex(0);
					}
				}
			}
			m_dwLastClock = m_dwStratClock;
		}
	}

	return;
}


void CNMakeCharDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	cltClient * pclclient = (cltClient*)pclClient;
	if(pclclient == NULL)												return;

	if(m_siChar_flag < SELECT_CHAR1 || m_siChar_flag > SELECT_CHAR4)	return;

	SI32 i;
	HDC hdc;

	if( pclClient->siServiceArea == ConstServiceArea_Japan
		|| pclClient->siServiceArea == ConstServiceArea_China
		|| pclClient->siServiceArea == ConstServiceArea_Korea
		|| pclClient->siServiceArea == ConstServiceArea_Taiwan
		|| pclClient->siServiceArea == ConstServiceArea_USA
		|| pclClient->siServiceArea == ConstServiceArea_English 
		|| pclClient->siServiceArea == ConstServiceArea_NHNChina
		|| pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		if(pclclient->lpBackScreen->GetDC(&hdc) != DD_OK) return;

		HFONT hFont;
		if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
			hFont = CreateFont( 18, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("����") );
		}
		else {
			hFont = CreateFont( 18, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("����") );
		}
		
		HFONT hOldFont = NULL;
		if ( hFont != NULL )
		{
			hOldFont = (HFONT)SelectObject( hdc, hFont );
		}

		TCHAR Country[256];
		TCHAR* pEastern_Boy;
		TCHAR* pEastern_Girl;
		TCHAR* pWestern_Boy;
		TCHAR* pWestern_Girl;

		if ( pclclient->IsWhereServiceArea( ConstServiceArea_China ) )
		{
			pEastern_Boy = GetTxtFromMgr(6457);
			pEastern_Girl = GetTxtFromMgr(6458);
			pWestern_Boy = GetTxtFromMgr(6459);
			pWestern_Girl = GetTxtFromMgr(6460);
		}
		else
		{
			pEastern_Boy = GetTxtFromMgr(2530);
			pEastern_Girl = GetTxtFromMgr(2531);
			pWestern_Boy = GetTxtFromMgr(7500);
			pWestern_Girl = GetTxtFromMgr(7501);
		}
	    
		for(i=0; i<4; i++)
		{
			switch(i)
			{
			case 0:		StringCchPrintf(Country, 256, pEastern_Boy);		break;
			case 1:		StringCchPrintf(Country, 256, pEastern_Girl);		break;
			case 2:		StringCchPrintf(Country, 256, pWestern_Boy);		break;
			case 3:		StringCchPrintf(Country, 256, pWestern_Girl);		break;
			}
			RECT rect;
			rect.left	= GetX() + 4 + (i * 232) + ScreenRX;
			rect.top	= GetY() + 503 + ScreenRY;
			rect.right	= rect.left + 220;
			rect.bottom	= rect.top	+ 35;

			SetBkMode(hdc, TRANSPARENT);

			if(i == m_siSelectedSlot)
				SetTextColor(hdc, RGB(76, 58, 128));
			else
				SetTextColor(hdc, RGB(26, 58, 128));

			// �ܰ��� �׸���
			RECT temprc;
			CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, Country, lstrlen(Country), &temprc, DT_CENTER | DT_VCENTER );
			CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, Country, lstrlen(Country), &temprc, DT_CENTER | DT_VCENTER );
			CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, Country, lstrlen(Country), &temprc, DT_CENTER | DT_VCENTER );
			CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, Country, lstrlen(Country), &temprc, DT_CENTER | DT_VCENTER );

			CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, Country, lstrlen(Country), &temprc, DT_CENTER | DT_VCENTER );
			CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, Country, lstrlen(Country), &temprc, DT_CENTER | DT_VCENTER );
			CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, Country, lstrlen(Country), &temprc, DT_CENTER | DT_VCENTER );
			CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, Country, lstrlen(Country), &temprc, DT_CENTER | DT_VCENTER );

			SetTextColor(hdc, RGB(255, 255, 255));
			DrawText( hdc, Country, lstrlen(Country), &rect, DT_CENTER | DT_VCENTER );
		}

		if(hOldFont)
		{
			SelectObject(hdc,hOldFont);
		}

		pclclient->lpBackScreen->ReleaseDC(hdc);
	}
}

void CNMakeCharDlg::ShowCharMakeWindow(bool bShow, SI16 fontindex )
{
	m_bShowCharMakeWindow = bShow;

	m_pEditName->Show(bShow);
	m_pButton[0]->Show(bShow);
	m_pButton[1]->Show(bShow);

	if ( pclClient->siServiceArea == ConstServiceArea_China )
	{
		m_pStatic[4]->SetFontIndex( fontindex );
	}
	m_pStatic[4]->Show(bShow);
}

void CNMakeCharDlg::Action()
{
	cltClient *pclclient = (cltClient *)pclClient;

	static SI32 siHeartBeatCount = 0;

	if (siHeartBeatCount >= 20)
	{
		siHeartBeatCount = 0;

		cltMsg clMsg(GAMEMSG_REQUEST_HEARTBEAT, 0, NULL);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
	siHeartBeatCount++;
}

void CNMakeCharDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = ( cltClient * )pclClient;
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

				cltCharListInfo clInfo;

				//-------------------------------------------
				// ���ο� ĳ������ ������ Ŭ������ �ִ´�. 
				//-------------------------------------------
				clInfo.clBI.SetIndex(pclclient->CurrentCharInfoList);

				if (m_siChar_flag >= SELECT_CHAR1 && m_siChar_flag <= SELECT_CHAR4)
				{
					clInfo.clBI.SetKind(makecharinfo[m_siChar_flag].siKind);
				}
				else
				{
					clInfo.clBI.SetKind(KIND_HERO3);
				}

				clInfo.clBI.SetName(m_pEditName->GetText());
				clInfo.clIP.SetBasicAbility(&pclclient->pclKindInfoSet->pclKI[clInfo.clBI.GetKind()]->clBA);	

				//KHY - 0331 - �ʱ� ������ �ٲ��.
				if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
				{				
					clInfo.clIP.SetLife(25);
					clInfo.clIP.SetMana(15);					
				}
				//------------------------------------------
				// ������ ���ο� ĳ������ ������ ������.
				//------------------------------------------
				cltMsg clMsg(GAMEMSG_REQUEST_MAKENEWCHAR, sizeof(cltCharListInfo),  (BYTE*)&clInfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

				m_pEditName->SetText(TEXT(""));

				//---------------------------------
				// ĳ���� ���� â�� �����ش�.
				//---------------------------------
				if(pclclient->m_pDialog[ NSELECTCHAR_DLG ] && ((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->IsShow() == false )
				{
					((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->Show();
				}

				//---------------------------------
				// ĳ���� ���� ��ȭâ�� �����Ѵ�. 
				//---------------------------------
				DeleteDialog();
			} 
		}
		break;

	case 1:
		{
			m_pEditName->SetText(TEXT(""));
			return;
		}
		break;
	}
	return;
}

void CNMakeCharDlg::SelectSlot(SI32 Slot)
{
	SI32 i;

	if(Slot >= 0)		m_siSelectedSlot = Slot;

	if( pclClient->siServiceArea == ConstServiceArea_Japan
		|| pclClient->siServiceArea == ConstServiceArea_China
		|| pclClient->siServiceArea == ConstServiceArea_Korea
		|| pclClient->siServiceArea == ConstServiceArea_Taiwan
		|| pclClient->siServiceArea == ConstServiceArea_USA
		|| pclClient->siServiceArea == ConstServiceArea_English
		|| pclClient->siServiceArea == ConstServiceArea_NHNChina
		|| pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		for(i=0; i<4; i++)
		{
			m_pButton_Char[i]->SetFileName(TEXT("Interface/CharMake/CharImg.SPR"));
			if(Slot == i)
			{
				m_pButton_Char[i]->SetFontIndex((i * 8) + 4);
			}
			else
			{
				m_pButton_Char[i]->SetFontIndex(i * 8);
			}
		}
	}
	else
	{
		m_pStatic[0]->SetFileName(TEXT("Interface/CharMake/CharImg1.SPR"));
		m_pStatic[0]->SetFontIndex(0);
		m_pStatic[1]->SetFileName(TEXT("Interface/CharMake/CharImg2.SPR"));
		m_pStatic[1]->SetFontIndex(0);
		m_pStatic[2]->SetFileName(TEXT("Interface/CharMake/CharImg3.SPR"));
		m_pStatic[2]->SetFontIndex(0);
		m_pStatic[3]->SetFileName(TEXT("Interface/CharMake/CharImg4.SPR"));
		m_pStatic[3]->SetFontIndex(0);

		switch(Slot)
		{
		case 0:		m_pStatic[Slot]->SetFileName(TEXT("Interface/CharMake/CharImg1Eff.SPR"));	break;
		case 1:		m_pStatic[Slot]->SetFileName(TEXT("Interface/CharMake/CharImg2Eff.SPR"));	break;
		case 2:		m_pStatic[Slot]->SetFileName(TEXT("Interface/CharMake/CharImg3Eff.SPR"));	break;
		case 3:		m_pStatic[Slot]->SetFileName(TEXT("Interface/CharMake/CharImg4Eff.SPR"));	break;
		}
		m_pStatic[Slot]->SetFontIndex(10);
	}
}
