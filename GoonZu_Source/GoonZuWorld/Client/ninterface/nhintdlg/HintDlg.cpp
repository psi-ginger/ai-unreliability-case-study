

//---------------------------------
// 2004/9/18 성준엽
//---------------------------------

#include "CommonLogic.h"
#include "Client.h"
#include "Resource.h"

#include "HintDlg.h"

//#include "../../lib/WebHTML/WebHTML.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/interface/Button.h"
#include "../../InterfaceMgr/interface/List.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"
extern cltCommonLogic* pclClient;

NHintDlgExplain::NHintDlgExplain()
{
	//m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;	
	//m_hHitBitmap = NULL;
	//m_hInst = NULL ;
	//m_hWnd = NULL ;

	m_pNHintStatic1 = NULL; 
	m_pNHintStatic2 = NULL;
	m_pNHintCombo = NULL;
	m_pNHintList = NULL;
	m_pNHintButton = NULL;
}

NHintDlgExplain::~NHintDlgExplain()
{
	SAFE_DELETE(m_pNHintStatic1);
	SAFE_DELETE(m_pNHintStatic2);
	SAFE_DELETE(m_pNHintCombo);
	SAFE_DELETE(m_pNHintList);
	SAFE_DELETE(m_pNHintButton);

}

void NHintDlgExplain::Create()
{
	//m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_HINT), hwnd, HintDlgExplainStaticProc);

	ReadListFile();
	if(!IsCreate())
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NHint/DLG_NHint.ddf");
		file.CreatePopupDialog( this, NHINT_DLG, "dialog_NHint", NHintDlgExplainStaticProc);		

	
		m_pNHintStatic1 = new CStatic(this); 
		m_pNHintStatic2 = new CStatic(this);
		m_pNHintCombo = new CComboBox(this);
		m_pNHintList = new CList(this);
		m_pNHintButton = new CButton(this);
	
		file.CreateControl(m_pNHintStatic1, NHINT_STATIC1, "statictext_NHint1");
		file.CreateControl(m_pNHintStatic2, NHINT_STATIC2, "statictext_NHint2");
		file.CreateControl(m_pNHintCombo, NHINT_EDIT, "combobox_NHint");
		file.CreateControl(m_pNHintList,NHINT_LIST, "listbox_NHint");
		file.CreateControl(m_pNHintButton, NHINT_BUTTON, "button_NHint");
	}

	//ShowWindow(m_hDlg, SW_HIDE);
}

void NHintDlgExplain::Show(char * szFileName)
{
	/*
	if(!IsWindow(m_hDlg))
	{
		if(m_DlgRectInitSwitch == TRUE)
		{
			MoveWindow(m_hDlg, 
				m_DlgRect.left,	 m_DlgRect.top, 
				m_DlgRect.right - m_DlgRect.left, 
				m_DlgRect.bottom - m_DlgRect.top, FALSE);
		}
	}

	char buffer[256] = "" ;
	strcpy(buffer,szFileName);

	ShowList(buffer);
	Set(buffer);

	ShowWindow(m_hDlg, SW_SHOW);
	*/
}

void CALLBACK NHintDlgExplain::NHintDlgExplainStaticProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{	
	NHintDlgExplain *pThis = (NHintDlgExplain*) pControllerMgr;
	pThis->NHintDlgExplainProc( nEvent, nControlID, pControl );
}

void CALLBACK NHintDlgExplain::NHintDlgExplainProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	/*
	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			for ( SI32 i = 0 ; i < MAX_HINTCOMBO_TYPE ; i ++ )
			{
				strcpy(m_szComboType[i],"");
				strcpy(m_szBmpName[i],"");
			}
		}
		break;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_LIST_HINT_SUBJECT:
				{
					switch(HIWORD(wParam))
					{
					case LBN_SELCHANGE:
						{
							SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_HINT_SUBJECT);
							if ( index < 0 ) break ;

							SI32 selectinx = SendDlgItemMessage(hDlg, IDC_LIST_HINT_SUBJECT, LB_GETITEMDATA, index, 0);

							SetWindowText(m_hDlg,m_szSubject[selectinx]);
							Set(m_szListType[selectinx]);
						}
						break;
					}
				}
				break;
			case IDC_COMBO_HINT_TYPE:
				{
					switch(HIWORD(wParam))
					{
					case CBN_SELCHANGE:
						{
							char buffer[256] = "";
							ComboGetString(hDlg, IDC_COMBO_HINT_TYPE, (LPARAM)buffer);

							SI32 type = GetExplainType(buffer);
							if ( type < 0 ) break ;

							ShowExplainHint(hDlg, type);
						}
						break;
					}
				}
				break;
			case IDCANCEL:
				{
					ShowWindow(m_hDlg, SW_HIDE);
				}
				break;
			}
			break;
		}
		break;
	case WM_CLOSE:
		{
			ShowWindow(m_hDlg, SW_HIDE);
		}
		break;
	case WM_DESTROY:
		{
			if ( m_hHitBitmap )
			{
				DeleteObject(m_hHitBitmap);
				m_hHitBitmap = NULL ;
			}

			GetWindowRect(m_hDlg, &m_DlgRect);
			m_DlgRectInitSwitch = TRUE ;

			m_hDlg = NULL ;
		}
		break;
	}
*/
	return  ;
}

SI32 NHintDlgExplain::GetExplainType(char* pExplainType)
{
	/*
	if ( pExplainType == NULL || !strcmp(pExplainType,"") ) return -1 ;

	for(SI32 i = 0; i < MAX_HINTCOMBO_TYPE; ++i)
	{
		if ( !strcmp(m_szComboType[i],"") ) continue ;
		if ( !strcmp(m_szComboType[i], pExplainType) )
		{
			return i;
		}
	}
*/
	return -1 ;
	
}

void NHintDlgExplain::ShowExplainHint(HWND hDlg, SI32 siHintType)
{
	/*
	char filename[MAX_PATH] = "" ;
	strcpy( filename, m_szBmpName[siHintType] );

	if(m_hHitBitmap != NULL)
	{
		DeleteObject(m_hHitBitmap);
		m_hHitBitmap = NULL;
	}

	m_hHitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

	if ( m_hHitBitmap != NULL )
	{
		SendDlgItemMessage( hDlg, IDC_STATIC_EXPLAIN_BMP, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHitBitmap);
	}	
	*/
}

void NHintDlgExplain::Set(char * szFileName )
{
	/*
	if ( !strcmp(szFileName,"NULL") )
		return ;

	char buffer[1024] = "" ;
	char path[256] = "" ;

	sprintf(path,"hintdlg\\%s.txt",szFileName);

	FILE* fp = fopen(path, "rt");
	if(fp == NULL)
	{
		MsgBox("fileError", "not HintDlg");
		return ;
	}


	// 첫번째 줄에서 주제를 얻어온다.
	fgets(buffer, 1024, fp);
	SI32 len = strlen(buffer);
	buffer[len-1] = '\0';
	SetWindowText(GetDlgItem(m_hDlg,IDC_STATIC_SUBJECT),buffer);

	ComboDelete(m_hDlg,IDC_COMBO_HINT_TYPE);

	// 5줄은 무시한다.
	for(SI32 i = 0;i < 5;i++)
		fgets(buffer, 1024, fp);

	for ( i = 0 ; i < MAX_HINTCOMBO_TYPE ; i ++ )
	{
		strcpy(m_szComboType[i],"");
		strcpy(m_szBmpName[i],"");
	}

	i = 0 ;
	while( fgets(buffer, 1024, fp) != NULL)
	{
		if ( i >= MAX_HINTCOMBO_TYPE ) break ;

		char * szcode;
		char * szname;

		szcode = strtok(buffer, "\t");
		szname = strtok(NULL,"\r\n");

		if ( !szcode || !szname ) break ;

		ComboAddString(m_hDlg, IDC_COMBO_HINT_TYPE, (LPARAM)szcode);

		strcpy(m_szComboType[i],szcode);
		strcpy(m_szBmpName[i],szname);
		i ++ ;
	}
	SendDlgItemMessage( m_hDlg, IDC_COMBO_HINT_TYPE, CB_SETCURSEL, 0, 0 );
	ShowExplainHint(m_hDlg, 0);

	fclose(fp);
	*/
}

void NHintDlgExplain::ReadListFile()
{
	/*
	char buffer[1024] = "" ;

	FILE* fp = fopen("hintdlg\\filelist.txt", "rt");
	if(fp == NULL)
	{
		MsgBox("fileError", "not HintDlgList");
		return ;
	}

	// 6줄은 무시한다.
	for(SI32 i = 0;i < 6;i++)
		fgets(buffer, 1024, fp);

	for ( i = 0 ; i < MAX_HINTCOMBO_TYPE ; i ++ )
	{
		strcpy(m_szListType[i],"");
	}

	ListDelete(m_hDlg, IDC_LIST_HINT_SUBJECT);
	i = 0 ;
	while( fgets(buffer, 1024, fp) != NULL)
	{
		if ( i >= MAX_HINTCOMBO_TYPE ) break ;

		char * szcode;
		char * szname;
		char * szshow;

		szcode = strtok(buffer, "\t");
		szname = strtok(NULL,"\t");
		szshow = strtok(NULL,"\t\n");

		if ( !szcode || !szname || !szshow ) break ;

		bool bshow = DATA_TO_BOOL(atoi(szshow)) ;
		if ( bshow == false ) continue ;

		strcpy(m_szSubject[i],szcode);
		strcpy(m_szListType[i],szname);

		SI32 index = ListAddString(m_hDlg, IDC_LIST_HINT_SUBJECT, (LPARAM)m_szSubject[i]);
		ListSetItemData(m_hDlg, IDC_LIST_HINT_SUBJECT, index, i);

		i ++ ;
	}

	fclose(fp);
	*/
}

void NHintDlgExplain::ShowList(char * szFileName)
{
	/*
	SendMessage(GetDlgItem(m_hDlg,IDC_LIST_HINT_SUBJECT), LB_SETCURSEL, (WPARAM)-1, 0 );

	if ( !strcmp(szFileName,"NULL") )
	{
		//SetWindowText(m_hDlg,"게임 도움말");
		SI32 index = (SI32)SendDlgItemMessage(m_hDlg, IDC_LIST_HINT_SUBJECT, LB_GETITEMDATA, 0, 0);
		strcpy(szFileName,m_szListType[index]);
		return ;
	}

	for ( SI32 i = 0 ; i < MAX_HINTLIST_TYPE ; i ++ )
	{
		if ( !strcmp(m_szListType[i],szFileName) )
		{
			char buffer[256] = "" ;
			for ( SI32 j = 0 ; j < MAX_HINTLIST_TYPE ; j ++ )
			{
				//SendDlgItemMessage(m_hDlg, IDC_LIST_HINT_SUBJECT, LB_GETTEXT, j, (LPARAM)buffer);

				if ( !strcmp(m_szSubject[i],buffer) )
				{
					//SendMessage(GetDlgItem(m_hDlg,IDC_LIST_HINT_SUBJECT), LB_SETCURSEL, j, 0 );
					break;
				}
			}
			//SetWindowText(m_hDlg,m_szSubject[i]);
			break;
		}
	}
	*/
}


