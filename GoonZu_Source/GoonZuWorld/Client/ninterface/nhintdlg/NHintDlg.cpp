

//---------------------------------
// 2004/9/18 성준엽
//---------------------------------

#include "CommonLogic.h"
#include "Client.h"
#include "Resource.h"
#include <mbstring.h>

#include "NHintDlg.h"

//#include "../../lib/WebHTML/WebHTML.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/interface/Button.h"
#include "../../InterfaceMgr/interface/List.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"
extern cltCommonLogic* pclClient;


stListBoxItemData HintListBoxItemData;

NHintDlgExplain::NHintDlgExplain()
{
	//m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;	
	//m_hHitBitmap = NULL;
	//m_hInst = NULL ;
	//m_hWnd = NULL ;

	//m_pNHintStatic1 = NULL; 
	m_pNHintStatic2 = NULL;
	m_pNHintStatic3 = NULL;
	m_pNHintCombo = NULL;
	m_pNHintList = NULL;
	m_pNHintButton = NULL;
	m_pNHintEdit=NULL;
}

NHintDlgExplain::~NHintDlgExplain()
{
	//SAFE_DELETE(m_pNHintStatic1);
	SAFE_DELETE(m_pNHintStatic2);
	SAFE_DELETE(m_pNHintStatic3);
	SAFE_DELETE(m_pNHintCombo);
	SAFE_DELETE(m_pNHintList);
	SAFE_DELETE(m_pNHintButton);
	SAFE_DELETE(m_pNHintEdit);

}

void NHintDlgExplain::Create()
{
	//m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_HINT), hwnd, HintDlgExplainStaticProc);
	if(IsCreate())
	{
		DeleteDialog();
	}
	else
	{
		CInterfaceFile file;

		file.LoadFile( TEXT("NInterface/Data/NHint/DLG_NHint.ddf"));
		file.CreatePopupDialog( this, NHINT_DLG,  TEXT("dialog_NHint"), NHintDlgExplainStaticProc);		

		
	
		//m_pNHintStatic1 = new CStatic(this); 
		m_pNHintEdit= new CEdit(this);
		m_pNHintStatic2 = new CStatic(this);
		m_pNHintStatic3 = new CStatic(this);
		m_pNHintCombo = new CComboBox(this);
		m_pNHintList = new CList(this);
		m_pNHintButton = new CButton(this);
		
	
		file.CreateControl(m_pNHintEdit, NHINT_EDIT,  TEXT("editbox_NHint"));
		file.CreateControl(m_pNHintStatic3, NHINT_STATIC3,  TEXT("statictext_NHint3"));	
		//file.CreateControl(m_pNHintStatic1, NHINT_STATIC1, TEXT("statictext_NHint1"));
		file.CreateControl(m_pNHintStatic2, NHINT_STATIC2,  TEXT("statictext_NHint2"));
		file.CreateControl(m_pNHintList,NHINT_LIST,  TEXT("listbox_NHint"));
		file.CreateControl(m_pNHintButton, NHINT_BUTTON,  TEXT("button_NHint"));
		file.CreateControl(m_pNHintCombo, NHINT_COMBO,  TEXT("combobox_NHint"));

		m_pNHintStatic3->SetFileName( TEXT("NInterface/Image/Image_256x286_00_000.SPR"));
		m_pNHintStatic3->SetFontIndex(0);
		m_pNHintStatic2->SetFontSize(12,0);
		TCHAR* pText = GetTxtFromMgr(1858);
		
		m_pNHintStatic2->SetFontKind(pText);
		m_pNHintStatic2->SetFontWeight( FW_BOLD );

		pText = GetTxtFromMgr(3439);
		m_pNHintList->SetColumn( 0, 240 ,pText);
		m_pNHintList->SetBorder(true);
		m_pNHintList->SetBKColorUse(true);
		m_pNHintList->Refresh();		
		ReadListFile();

		if ( pclClient->siServiceArea == ConstServiceArea_English
			|| pclClient->siServiceArea == ConstServiceArea_NHNChina
			|| pclClient->siServiceArea == ConstServiceArea_EUROPE
		)
		{
            m_pNHintEdit->SetMaxEditTextLength( 1024 );
		}

		
		//Set(m_szListType[0]);
	}

}



void NHintDlgExplain::Show(TCHAR * szFileName)
{
	if(!IsCreate())
	{
		Create();
	}

	TCHAR buffer[256] =  TEXT("") ;
	StringCchCopy(buffer, 256, szFileName);
	ShowList(buffer, 256);	
	Set(buffer);

}

void CALLBACK NHintDlgExplain::NHintDlgExplainStaticProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{	
	NHintDlgExplain *pThis = (NHintDlgExplain*) pControllerMgr;
	pThis->NHintDlgExplainProc( nEvent, nControlID, pControl );
}

void CALLBACK NHintDlgExplain::NHintDlgExplainProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	
	/*
	for ( SI32 i = 0 ; i < MAX_HINTCOMBO_TYPE ; i ++ )
	{
		strcpy(m_szComboType[i], TEXT(""));
		strcpy(m_szBmpName[i], TEXT(""));
	}
	*/
	
	switch (nControlID  )
	{	

	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;        

	case NHINT_COMBO:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{								
					TCHAR buffer[256] =  TEXT("");
					SI32 index= m_pNHintCombo->GetSelectedIndex();
					if(index<0)
						break;
					StringCchCopy(buffer, 256, m_pNHintCombo->GetText(index));				
						
//#ifdef _DEBUG
				
					m_pNHintStatic2->SetText(buffer,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
//#endif	
					SI32 kind=1;
					SI32 type = GetExplainType(buffer);
					if ( type < 0 ) break ;
					
					ShowExplainHint(type);
				}
				break;
			}
		}
		break;


	case NHINT_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					
					SI32 selectinx = m_pNHintList->GetSelectedIndex();
					if(selectinx < 0 ) break;
					
					//m_pNHintStatic1->SetText(m_szSubject[selectinx],DT_WORDBREAK);
					m_pNHintCombo->Clear();
					Set(m_szListType[selectinx]);

//#ifdef _DEBUG
					m_pNHintStatic2->SetText(m_szSubject[selectinx],DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//#endif	
					
					

				}
				break;
			}
		}
		break;


	case NHINT_BUTTON:  //닫기 버튼
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;	
	}
	
	return  ;
}

SI32 NHintDlgExplain::GetExplainType(TCHAR* pExplainType)
{
	
	if ( pExplainType == NULL || !_tcscmp(pExplainType, TEXT("")) ) return -1 ;

	for(SI32 i = 0; i < MAX_HINTCOMBO_TYPE; ++i)
	{
		if ( !_tcscmp(m_szComboType[i], TEXT("")) ) continue ;
		if ( !_tcscmp(m_szComboType[i], pExplainType) )
		{
			return i;
		}
	}

	return -1 ;
	
}

void NHintDlgExplain::ShowExplainHint(SI32 siHintType)
{
	
	SI32 FindImageIndex =_tstoi(m_szBmpName[siHintType]);
	
//#ifdef _DEBUG
	{
	TCHAR* pText = GetHelpFromMgr(FindImageIndex);
	if(pText)
	//m_pNHintStatic2->SetText(pText,DT_WORDBREAK);
	m_pNHintEdit->SetText(pText);
	}
//#else

//	m_pNHintStatic2->SetFileName(TEXT("StatusHint\\Image_256x286_00_000.SPR"));
//	m_pNHintStatic2->SetFontIndex(FindImageIndex);
//#endif	
	
}

void NHintDlgExplain::Set(TCHAR * szFileName )
{
	if ( !_tcscmp(szFileName, TEXT("NULL")) )
		return ;

	TCHAR buffer[1024] =  TEXT("") ;
	TCHAR path[256] =  TEXT("") ;

	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea : 
		StringCchPrintf(path, 256,  TEXT("hintdlg\\NHintDlg\\%sK.txt"),szFileName);
		break;
	case ConstServiceArea_English : 
		StringCchPrintf(path, 256,  TEXT("hintdlg\\NHintDlg\\%sE.txt"),szFileName);
		break;
	case ConstServiceArea_China : 
		StringCchPrintf(path, 256,  TEXT("hintdlg\\NHintDlg\\%sC.txt"),szFileName);
		break;
	case ConstServiceArea_Japan : 
		StringCchPrintf(path, 256,  TEXT("hintdlg\\NHintDlg\\%sJ.txt"),szFileName);
		break;
	case ConstServiceArea_Taiwan : 
		StringCchPrintf(path, 256,  TEXT("hintdlg\\NHintDlg\\%sT.txt"),szFileName);
		break;
	case ConstServiceArea_USA : 
		StringCchPrintf(path, 256,  TEXT("hintdlg\\NHintDlg\\%sU.txt"),szFileName);
		break;
	case ConstServiceArea_NHNChina : 
		StringCchPrintf(path, 256,  TEXT("hintdlg\\NHintDlg\\%sC.txt"),szFileName);
		break;
	case ConstServiceArea_EUROPE : 
		StringCchPrintf(path, 256,  TEXT("hintdlg\\NHintDlg\\%sG.txt"),szFileName);
		break;

	}

	FILE* fp = _tfopen(path, TEXT("rt"));
	if(fp == NULL)
	{
		MsgBox(TEXT("fileError"), TEXT("not NHintDlg"));
		return ;
	}


	// 첫번째 줄에서 주제를 얻어온다.
	_fgetts(buffer, 1024, fp);
	SI32 len = _tcslen(buffer);
	buffer[len-1] = '\0';
	//m_pNHintStatic1->SetText(buffer,DT_WORDBREAK);
	

	// 5줄은 무시한다.
	for(SI32 i = 0;i < 5;i++)
		_fgetts(buffer, 1024, fp);

	for ( i = 0 ; i < MAX_HINTCOMBO_TYPE ; i ++ )
	{
		StringCchCopy(m_szComboType[i], 256, TEXT(""));
		StringCchCopy(m_szBmpName[i], 128, TEXT(""));
	}

	i = 0 ;
	while( _fgetts(buffer, 1024, fp) != NULL)
	{
		if ( i >= MAX_HINTCOMBO_TYPE ) break ;

		TCHAR * szcode;
		TCHAR * szname;

		szcode = _tcstok(buffer, TEXT("\t"));
		szname = _tcstok(NULL, TEXT("\r\n"));

		if ( !szcode || !szname ) break ;

		//ComboAddString(m_hDlg, IDC_COMBO_HINT_TYPE, (LPARAM)szcode);

		StringCchCopy(m_szComboType[i], 256, szcode);
		StringCchCopy(m_szBmpName[i], 128, szname);
		i ++ ;
	}
	

    
	for(SI32 i=0; i<MAX_HINTCOMBO_TYPE; i++)
	{
		if ( !_tcscmp(m_szComboType[i], TEXT("")) )
			continue;
				
		m_pNHintCombo->GetItemData(NHINT_COMBO );

		stComboBoxItemData tmpComboBoxItemData;
		StringCchCopy(tmpComboBoxItemData.strText, MAX_COMBOBOXITEM_TEXT_LEN, m_szComboType[i]);
		tmpComboBoxItemData.siParam=i;

		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( m_szComboType[i] );
		m_pNHintCombo->AddItem( &tmpComboBoxItemData );

	}	
	
	m_pNHintCombo->SetCursel(0);
	ShowExplainHint(0);
	

	fclose(fp);
	
}

void NHintDlgExplain::ReadListFile()
{
		
	TCHAR buffer[1024] = TEXT("") ;
	FILE* fp= NULL;

	
	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Korea : 
		fp = _tfopen(TEXT("hintdlg\\NHintDlg\\filelistK.txt"), TEXT("rt"));
		break;
	case ConstServiceArea_English : 
		fp = _tfopen(TEXT("hintdlg\\NHintDlg\\filelistE.txt"), TEXT("rt"));
		break;
	case ConstServiceArea_China : 
		fp = _tfopen(TEXT("hintdlg\\NHintDlg\\filelistC.txt"), TEXT("rt"));
		break;
	case ConstServiceArea_Japan : 
		fp = _tfopen(TEXT("hintdlg\\NHintDlg\\filelistJ.txt"), TEXT("rt"));
		break;
	case ConstServiceArea_Taiwan : 
		fp = _tfopen(TEXT("hintdlg\\NHintDlg\\filelistT.txt"), TEXT("rt"));
		break;
	case ConstServiceArea_USA : 
		fp = _tfopen(TEXT("hintdlg\\NHintDlg\\filelistU.txt"), TEXT("rt"));
		break;
	case ConstServiceArea_NHNChina : 
		fp = _tfopen(TEXT("hintdlg\\NHintDlg\\filelistC.txt"), TEXT("rt"));
		break;
	case ConstServiceArea_EUROPE : 
		fp = _tfopen(TEXT("hintdlg\\NHintDlg\\filelistG.txt"), TEXT("rt"));
		break;
	}

	if(fp == NULL)
	{
		MsgBox(TEXT("fileError"), TEXT("not HintDlgList"));
		return ;
	}

	// 6줄은 무시한다.
	for(SI32 i = 0;i < 6;i++)
		_fgetts(buffer, 1024, fp);

	for ( i = 0 ; i < MAX_HINTCOMBO_TYPE ; i ++ )
	{
		//StringCchCopy(m_szListType[i], 128, TEXT(""));
		m_szListType[i][0] = NULL;
		m_szSubject[i][0] = NULL;

	}

	//ListDelete(m_hDlg, IDC_LIST_HINT_SUBJECT);
	i = 0 ;
	while( _fgetts(buffer, 1024, fp) != NULL)
	{
		if ( i >= MAX_HINTCOMBO_TYPE ) break ;

		TCHAR * szcode;
		TCHAR * szname;
		TCHAR * szshow;

		szcode = _tcstok(buffer, TEXT("\t"));
		szname = _tcstok(NULL, TEXT("\t"));
		szshow = _tcstok(NULL, TEXT("\t\n"));

		if ( !szcode || !szname || !szshow ) break ;

		bool bshow = DATA_TO_BOOL(_tstoi(szshow)) ;
		if ( bshow == false ) continue ;

		StringCchCopy(m_szSubject[i], 128,szcode);
		StringCchCopy(m_szListType[i], 128, szname);	

		i ++ ;
	}


	fclose(fp);


		//-----------------------------------------------
    //   마을 정렬 내림차순   by tae-yang
	//----------------------------------------------

	TCHAR tmp[256]=TEXT("");
  
	for(SI32 sun_index2 = 0 ; sun_index2 < MAX_HINTCOMBO_TYPE - 1 ; sun_index2++)
		{
			if (m_szListType[sun_index2][0] == NULL) continue;

			for(SI32 sun_index3 = sun_index2 + 1; sun_index3 < MAX_HINTCOMBO_TYPE; sun_index3++)
			{
				if (m_szListType[sun_index3][0] == NULL) continue;
				
				if( _mbscoll( (BYTE *)m_szSubject[sun_index2], (unsigned char *)m_szSubject[sun_index3] ) < 0 )
				{
					StringCchCopy(tmp, 256, m_szSubject[sun_index2]);
					StringCchCopy(m_szSubject[sun_index2], 128,m_szSubject[sun_index3]);
					StringCchCopy(m_szSubject[sun_index3], 128, tmp);
                    

					StringCchCopy(tmp, 256, m_szListType[sun_index2]);
					StringCchCopy(m_szListType[sun_index2], 128, m_szListType[sun_index3]);
					StringCchCopy(m_szListType[sun_index3], 128, tmp);
				}
			}
		}



		
	for ( i = 0 ; i < MAX_HINTCOMBO_TYPE ; i ++ )
	{
		if (!_tcscmp(m_szSubject[i], TEXT("")) ) continue;
		if (m_szSubject[i][0] == NULL) continue;

		HintListBoxItemData.Init();									
		HintListBoxItemData.Set(0,m_szSubject[i]);	
		HintListBoxItemData.siParam[0] = i;	
		SI32 index = m_pNHintList->AddItem( &HintListBoxItemData );	
	}

		

	m_pNHintList->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );	
	m_pNHintList->Refresh();
}

void NHintDlgExplain::ShowList(TCHAR * szFileName, SI16 txtSize)
{
	
	//SendMessage(GetDlgItem(m_hDlg,IDC_LIST_HINT_SUBJECT), LB_SETCURSEL, (WPARAM)-1, 0 );

	TCHAR* pTitle = GetTxtFromMgr(3313);
	if ( !_tcscmp(szFileName, TEXT("NULL")) )
	{
		SetTitle(pTitle);		
		SI32 index=0;//m_pNHintList->GetSelectedIndex();
		StringCchCopy(szFileName, txtSize, m_szListType[index]);
		return ;
	}

	for ( SI32 i = 0 ; i < MAX_HINTLIST_TYPE ; i ++ )
	{
		if ( !_tcscmp(m_szListType[i],szFileName) )
		{
			TCHAR buffer[256] = TEXT("") ;
			for ( SI32 j = 0 ; j < MAX_HINTLIST_TYPE ; j ++ )
			{
				//SendDlgItemMessage(m_hDlg, IDC_LIST_HINT_SUBJECT, LB_GETTEXT, j, (LPARAM)buffer);

				if ( !_tcscmp(m_szSubject[i],buffer) )
				{
					//SendMessage(GetDlgItem(m_hDlg,IDC_LIST_HINT_SUBJECT), LB_SETCURSEL, j, 0 );
					break;
				}
			}
//			m_pNHintStatic1->SetText(m_szSubject[i],DT_WORDBREAK);	

			break;
		}	
	}
	
	
}


