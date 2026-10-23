#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>

#include "NMonsterCreateDlg.h"

#include "../../Client.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "..\..\..\CommonLogic\MagicMgr\MagicMgr.h"
#include "..\..\..\CommonLogic\MagicMgr\MagicAttribute\MagicAttribute.h"
#include "../../../CommonLogic/Msg/MsgType-NPC.h"

#include "../../Lib/WebHTML/WebManager.h"

extern cltCommonLogic* pclClient;

CNMonsterCreateDlg::CNMonsterCreateDlg()
{

}

CNMonsterCreateDlg::~CNMonsterCreateDlg()
{

}

void CNMonsterCreateDlg::Init()
{

}

void CNMonsterCreateDlg::Create()
{
	if (IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT ( "NInterface/Data/NBalanceTestGMDlg/DLG_NMosterCreateDlg.ddf" ) );
	file.CreatePopupDialog( this, NMONSTERCREATE_DLG, TEXT("dialog_MonsterCreate"), StaticCallBackDialogCNMonsterCreateProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONNAME, this ), NMONSTERCREATE_DLG_STATIC_MONNAME, TEXT("statictext_MonName") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONMAGIC, this ), NMONSTERCREATE_DLG_STATIC_MONMAGIC, TEXT("statictext_MonMagic") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONATTACKTYPE, this ), NMONSTERCREATE_DLG_STATIC_MONATTACKTYPE, TEXT("statictext_MonAttackType") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONSKILL, this ), NMONSTERCREATE_DLG_STATIC_MONSKILL, TEXT("statictext_MonSkill") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONLEVEL, this ), NMONSTERCREATE_DLG_STATIC_MONLEVEL, TEXT("statictext_MonLevel") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONCREATENUM, this ), NMONSTERCREATE_DLG_STATIC_MONCREATENUM, TEXT("statictext_MonCreateNum") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONAGGRO, this ), NMONSTERCREATE_DLG_STATIC_MONAGGRO, TEXT("statictext_MonAggro") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONAC, this ), NMONSTERCREATE_DLG_STATIC_MONAC, TEXT("statictext_MonAC") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONMINDMG, this ), NMONSTERCREATE_DLG_STATIC_MONMINDMG, TEXT("statictext_MonMinDmg") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONSTR, this ), NMONSTERCREATE_DLG_STATIC_MONSTR, TEXT("statictext_MonStr") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONMAXDMG, this ), NMONSTERCREATE_DLG_STATIC_MONMAXDMG, TEXT("statictext_MonMaxDmg") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONDEX, this ), NMONSTERCREATE_DLG_STATIC_MONDEX, TEXT("statictext_MonDex") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONCRIRATE, this ), NMONSTERCREATE_DLG_STATIC_MONCRIRATE, TEXT("statictext_MonCriRate") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONVIT, this ), NMONSTERCREATE_DLG_STATIC_MONVIT, TEXT("statictext_MonVit") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONATTINTER, this ), NMONSTERCREATE_DLG_STATIC_MONATTINTER, TEXT("statictext_MonAttInter") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONINTL, this ), NMONSTERCREATE_DLG_STATIC_MONINTL, TEXT("statictext_MonIntl") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONFIRE, this ), NMONSTERCREATE_DLG_STATIC_MONFIRE, TEXT("statictext_MonFire") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONWATER, this ), NMONSTERCREATE_DLG_STATIC_MONWATER, TEXT("statictext_MonWater") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONHND, this ), NMONSTERCREATE_DLG_STATIC_MONHND, TEXT("statictext_MonHnd") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONLAND, this ), NMONSTERCREATE_DLG_STATIC_MONLAND, TEXT("statictext_MonLand") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONLUK, this ), NMONSTERCREATE_DLG_STATIC_MONLUK, TEXT("statictext_MonLuk") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONWIND, this ), NMONSTERCREATE_DLG_STATIC_MONWIND, TEXT("statictext_MonWind") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONWIS, this ), NMONSTERCREATE_DLG_STATIC_MONWIS, TEXT("statictext_MonWis") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMONSTERCREATE_DLG_STATIC_MONNATURE, this ), NMONSTERCREATE_DLG_STATIC_MONNATURE, TEXT("statictext_MonNature") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONMAGIC, this ), NMONSTERCREATE_DLG_EDITBOX_MONMAGIC, TEXT("editbox_MonMagic") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONATTACKTYPE, this ), NMONSTERCREATE_DLG_EDITBOX_MONATTACKTYPE, TEXT("editbox_MonAttackType") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONSKILL, this ), NMONSTERCREATE_DLG_EDITBOX_MONSKILL, TEXT("editbox_MonSkill") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONLEVEL, this ), NMONSTERCREATE_DLG_EDITBOX_MONLEVEL, TEXT("editbox_MonLevel") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONCREATENUM, this ), NMONSTERCREATE_DLG_EDITBOX_MONCREATENUM, TEXT("editbox_MonCreateNum") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONAGGRO, this ), NMONSTERCREATE_DLG_EDITBOX_MONAGGRO, TEXT("editbox_MonAggro") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONAC, this ), NMONSTERCREATE_DLG_EDITBOX_MONAC, TEXT("editbox_MonAC") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONMINDMG, this ), NMONSTERCREATE_DLG_EDITBOX_MONMINDMG, TEXT("editbox_MinDmg") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONSTR, this ), NMONSTERCREATE_DLG_EDITBOX_MONSTR, TEXT("editbox_MonStr") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONMAXDMG, this ), NMONSTERCREATE_DLG_EDITBOX_MONMAXDMG, TEXT("editbox_MonMaxDmg") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONDEX, this ), NMONSTERCREATE_DLG_EDITBOX_MONDEX, TEXT("editbox_MonDex") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONCRIRATE, this ), NMONSTERCREATE_DLG_EDITBOX_MONCRIRATE, TEXT("editbox_MonCriRate") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONVIT, this ), NMONSTERCREATE_DLG_EDITBOX_MONVIT, TEXT("editbox_MonVit") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONATTINTER, this ), NMONSTERCREATE_DLG_EDITBOX_MONATTINTER, TEXT("editbox_MonAttInter") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONINTL, this ), NMONSTERCREATE_DLG_EDITBOX_MONINTL, TEXT("editbox_MonIntl") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONFIRE, this ), NMONSTERCREATE_DLG_EDITBOX_MONFIRE, TEXT("editbox_MonFire") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONWATER, this ), NMONSTERCREATE_DLG_EDITBOX_MONWATER, TEXT("editbox_MonWater") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONHND, this ), NMONSTERCREATE_DLG_EDITBOX_MONHND, TEXT("editbox_MonHnd") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONLAND, this ), NMONSTERCREATE_DLG_EDITBOX_MONLAND, TEXT("editbox_MonLand") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONLUK, this ), NMONSTERCREATE_DLG_EDITBOX_MONLUK, TEXT("editbox_MonLuk") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONWIND, this ), NMONSTERCREATE_DLG_EDITBOX_MONWIND, TEXT("editbox_MonWind") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONWIS, this ), NMONSTERCREATE_DLG_EDITBOX_MONWIS, TEXT("editbox_MonWis") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMONSTERCREATE_DLG_EDITBOX_MONNATURE, this ), NMONSTERCREATE_DLG_EDITBOX_MONNATURE, TEXT("editbox_MonNature") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NMONSTERCREATE_DLG_COMBOBOX_MONNAME, this ), NMONSTERCREATE_DLG_COMBOBOX_MONNAME, TEXT("combobox_MonName") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMONSTERCREATE_DLG_BUTTON_MONCREATE, this ), NMONSTERCREATE_DLG_BUTTON_MONCREATE, TEXT("button_Create") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMONSTERCREATE_DLG_BUTTON_CLOSE, this ), NMONSTERCREATE_DLG_BUTTON_CLOSE, TEXT("button_Close") );

	AddCombo();

	if(m_InterfaceFactory.GetComboBox(NMONSTERCREATE_DLG_COMBOBOX_MONNAME)->GetItemNum() > 0 )
		m_InterfaceFactory.GetComboBox(NMONSTERCREATE_DLG_COMBOBOX_MONNAME)->SetCursel( 0 );

	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLEVEL)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONCREATENUM)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONAGGRO)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONAC)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONMINDMG)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONMAXDMG)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONSTR)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONDEX)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONVIT)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONINTL)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONHND)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLUK)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWIS)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONCRIRATE)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONATTINTER)->SetNumberMode(true);
}

void CALLBACK CNMonsterCreateDlg::StaticCallBackDialogCNMonsterCreateProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNMonsterCreateDlg *pThis = (CNMonsterCreateDlg * )pControllerMgr;
	if( pThis )
	{
		pThis->CallBackDialogCNMonsterCreateProc( nEvent, nControlID, pControl);
	}
}

void CALLBACK CNMonsterCreateDlg::CallBackDialogCNMonsterCreateProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NMONSTERCREATE_DLG_COMBOBOX_MONNAME:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					m_siSelectedIndex = m_InterfaceFactory.GetComboBox(NMONSTERCREATE_DLG_COMBOBOX_MONNAME)->GetSelectedIndex();

					if ( m_siSelectedIndex > -1 )
					{
						TCHAR strMonsterName[ 128 ] = TEXT("");
						StringCchCopy(strMonsterName, 128, m_InterfaceFactory.GetComboBox(NMONSTERCREATE_DLG_COMBOBOX_MONNAME)->GetText(m_siSelectedIndex) );

						GetMonsterInfo( strMonsterName );
					}
				}
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_BUTTON_CLOSE:
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

	case NMONSTERCREATE_DLG_BUTTON_MONCREATE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetMonsterInfo();
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONLEVEL:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLEVEL)->GetText();

					SI32 siInput = atoi(szInput);

					if( siInput > 255 ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), 255);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLEVEL)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONCREATENUM:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONCREATENUM)->GetText();

					SI32 siInput = atoi(szInput);

					if( siInput > 200 ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), 200);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONCREATENUM)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONAGGRO:
		{			
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONAGGRO)->GetText();

					SI32 siInput = atoi(szInput);

					if( siInput > 100 ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), 100);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONAGGRO)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONAC:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONAC)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONAC)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONMINDMG:
		{			
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{			
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONMINDMG)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONMINDMG)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONMAXDMG:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONMAXDMG)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONMAXDMG)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONSTR:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONSTR)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONSTR)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONDEX:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONDEX)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONDEX)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONVIT:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONVIT)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONVIT)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONINTL:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONINTL)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONINTL)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONHND:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONHND)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONHND)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONLUK:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLUK)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLUK)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONWIS:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWIS)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWIS)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONCRIRATE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONCRIRATE)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONCRIRATE)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONATTINTER:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONATTINTER)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 10000이상의 값이 들어오면 10000으로 고정시킨다.
					if( siInput > MAX_INPUT ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_INPUT);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONATTINTER)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONFIRE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONFIRE)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 3000이상의 값이 들어오면 3000으로 고정시킨다.
					if( siInput > 3000 ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), 3000);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONFIRE)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONWATER:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWATER)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 3000이상의 값이 들어오면 3000으로 고정시킨다.
					if( siInput > 3000 ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), 3000);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWATER)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONLAND:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLAND)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 3000이상의 값이 들어오면 3000으로 고정시킨다.
					if( siInput > 3000 ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), 3000);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLAND)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONWIND:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWIND)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 3000이상의 값이 들어오면 3000으로 고정시킨다.
					if( siInput > 3000 ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), 3000);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWIND)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NMONSTERCREATE_DLG_EDITBOX_MONNATURE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONNATURE)->GetText();

					SI32 siInput = atoi(szInput);

					// 수치에 3000이상의 값이 들어오면 3000으로 고정시킨다.
					if( siInput > 3000 ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), 3000);

						m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONNATURE)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;
	}
}

void CNMonsterCreateDlg::GetMonsterInfo(TCHAR* szText)
{
	//============================================
	// 얻어온 Monster이름으로부터 몬스터 정보를 출력한다.
	//============================================
	m_siMonsterKind = pclClient->pclKindInfoSet->FindKindFromName( szText );
	
	cltKindInfo* pclki = NULL;
	pclki = pclClient->pclKindInfoSet->pclKI[m_siMonsterKind];

	SI32 siMagicKind = pclki->siMagic;
	SI32 siAttackType = pclki->clDefaultWeaponInfo.GetAttackType();
	SI32 siSkill = pclki->siSkillLevel;
	SI32 m_siLevel = 1;
	SI32 m_siMonCreateNum = 1;
	SI32 m_siAggro = pclki->siAggressive;
	SI32 m_siMonAC = pclki->clDefaultWeaponInfo.GetDefaultAC();
	SI32 m_siMinDmg = pclki->clDefaultWeaponInfo.GetDefaultMinDamage();
	SI32 m_siMaxDmg = pclki->clDefaultWeaponInfo.GetDefaultMaxDamage();
	SI32 m_siMonStr = pclki->clBA.GetStr();
	SI32 m_siMonDex = pclki->clBA.GetDex();
	SI32 m_siMonVit = pclki->clBA.GetVit();
	SI32 m_siMonIntl = pclki->clBA.GetMag();
	SI32 m_siMonHnd = pclki->clBA.GetHnd();
	SI32 m_siMonLuk = pclki->clBA.GetLuk();
	SI32 m_siMonWis = pclki->clBA.GetWis();
	SI32 m_siCriRate = pclki->siCriticalRate;
	SI32 m_siAttInter = pclki->dwAttackInterval;
	SI32 m_siFire = pclki->pclCharElemental.GetFire();
	SI32 m_siWater = pclki->pclCharElemental.GetWater();
	SI32 m_siLand = pclki->pclCharElemental.GetLand();
	SI32 m_siWind = pclki->pclCharElemental.GetWind();
	SI32 m_siNature = pclki->pclCharElemental.GetNature();

	stMagicAttributeData *pMagicAttributeData	= pclClient->pclMagicManager->GetMagicAttribute( siMagicKind );

	TCHAR szBuffer[ 128 ] = TEXT("");
	TCHAR szAttackTypeName [ 128 ] = TEXT("");

	for(SI32 i = 0;i < MAX_ATTACKTYPE_NUMBER;i++)
	{
		if(pclClient->pclAttackTypeInfoManager->pclAttackTypeInfo[i])
		{
			if(pclClient->pclAttackTypeInfoManager->pclAttackTypeInfo[i]->siAttackType == siAttackType)
			{
				StringCchCopy( szAttackTypeName, 128, pclClient->pclAttackTypeInfoManager->pclAttackTypeInfo[i]->szAttackTypeCode );
			}
		}
	}
		
	StringCchCopy( szBuffer, 128, pMagicAttributeData->strMagicKindName );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONMAGIC)->SetText(szBuffer);

	StringCchCopy( szBuffer, 128, szAttackTypeName);
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONATTACKTYPE)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , siSkill );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONSKILL)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siLevel );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLEVEL)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siMonCreateNum );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONCREATENUM)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siAggro );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONAGGRO)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siMonAC );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONAC)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siMinDmg );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONMINDMG)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siMonStr );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONSTR)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siMaxDmg );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONMAXDMG)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siMonDex );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONDEX)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siCriRate );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONCRIRATE)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siMonVit );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONVIT)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siAttInter );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONATTINTER)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siMonIntl );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONINTL)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siFire );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONFIRE)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siWater );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWATER)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siMonHnd );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONHND)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siLand );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLAND)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siMonLuk );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLUK)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siWind );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWIND)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siMonWis );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWIS)->SetText(szBuffer);

	StringCchPrintf(szBuffer, 128 ,TEXT("%d") , m_siNature );
	m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONNATURE)->SetText(szBuffer);
}

void CNMonsterCreateDlg::AddCombo()
{
	m_InterfaceFactory.GetComboBox(NMONSTERCREATE_DLG_COMBOBOX_MONNAME)->Clear();

	cltKindInfo* pclki = NULL;

	SI32 sortindex = 0 ;

	for ( SI32 j = 1 ; j < MAX_KIND_NUMBER ; j ++ )
	{
		for ( SI32 i = 10; i < MAX_KIND_NUMBER ; ++i )
		{
			pclki = pclClient->pclKindInfoSet->pclKI[ i ];

			if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
				continue;

			switch (pclClient->siServiceArea)
			{
			case ConstServiceArea_Korea :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
					continue;
				break;
			case ConstServiceArea_China :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
					continue;
				break;
			case ConstServiceArea_Japan :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
					continue;
				break;
			case ConstServiceArea_English :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
					continue;
				break;
			case ConstServiceArea_Taiwan :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
					continue;
				break;
			case ConstServiceArea_USA :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
					continue;
				break;
			case ConstServiceArea_NHNChina :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_NHNCHINA ) == false)
					continue;
				break;
			case ConstServiceArea_EUROPE :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_EUROPE ) == false)
					continue;
				break;

			}

			if ( pclki->siSortLevel <= 0 || pclki->siSortLevel >= MAX_SORT_LEVEL ) continue ;

			if ( pclki->siSortLevel == j )
			{
				TCHAR* pname = (TCHAR*)pclki->GetName();

				stComboBoxItemData ComboBoxItemData;
				ComboBoxItemData.Init();
				ComboBoxItemData.Set( pname );

				m_InterfaceFactory.GetComboBox(NMONSTERCREATE_DLG_COMBOBOX_MONNAME)->AddItem( &ComboBoxItemData );

				break ;
			}
		}
	}
}

void CNMonsterCreateDlg::SetMonsterInfo()
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltKindInfo* pclki = NULL;
	pclki = pclClient->pclKindInfoSet->pclKI[m_siMonsterKind];

	SI32 siMagicKind = pclki->siMagic;
	SI32 siAttackType = pclki->clDefaultWeaponInfo.GetAttackType();
	SI32 siSkill = pclki->siSkillLevel;
	SI32 m_siLevel = 1;
	SI32 m_siMonCreateNum = 1;
	SI32 m_siAggro = pclki->siAggressive;
	SI32 m_siMonAC = pclki->clDefaultWeaponInfo.GetDefaultAC();
	SI32 m_siMinDmg = pclki->clDefaultWeaponInfo.GetDefaultMinDamage();
	SI32 m_siMaxDmg = pclki->clDefaultWeaponInfo.GetDefaultMaxDamage();
	SI32 m_siMonStr = pclki->clBA.GetStr();
	SI32 m_siMonDex = pclki->clBA.GetDex();
	SI32 m_siMonVit = pclki->clBA.GetVit();
	SI32 m_siMonIntl = pclki->clBA.GetMag();
	SI32 m_siMonHnd = pclki->clBA.GetHnd();
	SI32 m_siMonLuk = pclki->clBA.GetLuk();
	SI32 m_siMonWis = pclki->clBA.GetWis();
	SI32 m_siCriRate = pclki->siCriticalRate;
	SI32 m_siAttInter = pclki->dwAttackInterval;
	SI32 m_siFire = pclki->pclCharElemental.GetFire();
	SI32 m_siWater = pclki->pclCharElemental.GetWater();
	SI32 m_siLand = pclki->pclCharElemental.GetLand();
	SI32 m_siWind = pclki->pclCharElemental.GetWind();
	SI32 m_siNature = pclki->pclCharElemental.GetNature();

	TCHAR* szLevel = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLEVEL)->GetText();
	TCHAR* szMonCreateNum = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONCREATENUM)->GetText();
	TCHAR* szAggro = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONAGGRO)->GetText();
	TCHAR* szMonAC = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONAC)->GetText();
	TCHAR* szMinDmg = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONMINDMG)->GetText();
	TCHAR* szMaxDmg = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONMAXDMG)->GetText();
	TCHAR* szMonStr = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONSTR)->GetText();
	TCHAR* szMonDex = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONDEX)->GetText();
	TCHAR* szMonVit = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONVIT)->GetText();
	TCHAR* szMonIntl = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONINTL)->GetText();
	TCHAR* szMonHnd = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONHND)->GetText();
	TCHAR* szMonLuk = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLUK)->GetText();
	TCHAR* szMonWis = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWIS)->GetText();
	TCHAR* szCriRate = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONCRIRATE)->GetText();
	TCHAR* szAttInter = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONATTINTER)->GetText();
	TCHAR* szFire = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONFIRE)->GetText();
	TCHAR* szWater = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWATER)->GetText();
	TCHAR* szLand = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONLAND)->GetText();
	TCHAR* szWind = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONWIND)->GetText();
	TCHAR* szNature = m_InterfaceFactory.GetEdit(NMONSTERCREATE_DLG_EDITBOX_MONNATURE)->GetText();

	// 능력치가 아무것도 입력이 안되면 몬스터를 생성할 수 없다.
	CString	cStrLevel = szLevel;
	CString cStrMonCreateNum = szMonCreateNum;
	CString cStrAggro = szAggro;
	CString cStrMonAC = szMonAC;
	CString cStrMinDmg = szMinDmg;
	CString cStrMaxDmg = szMaxDmg;
	CString cStrMonStr = szMonStr;
	CString cStrMonDex = szMonDex;
	CString cStrMonVit = szMonVit;
	CString cStrMonIntl = szMonIntl;
	CString cStrMonHnd = szMonHnd;
	CString cStrMonLuk = szMonLuk;
	CString cStrMonWis = szMonWis;
	CString cStrCriRate = szCriRate;
	CString cStrAttInter = szAttInter;
	CString cStrFire = szFire;
	CString cStrWater = szWater;
	CString cStrLand = szLand;
	CString cStrWind = szWind;
	CString cStrNature = szNature;

	cStrLevel.Trim();
	cStrMonCreateNum.Trim();
	cStrAggro.Trim();
	cStrMonAC.Trim();
	cStrMinDmg.Trim();
	cStrMaxDmg.Trim();
	cStrMonStr.Trim();
	cStrMonDex.Trim();
	cStrMonVit.Trim();
	cStrMonIntl.Trim();
	cStrMonHnd.Trim();
	cStrMonLuk.Trim();
	cStrMonWis.Trim();
	cStrCriRate.Trim();
	cStrAttInter.Trim();
	cStrFire.Trim();
	cStrWater.Trim();
	cStrLand.Trim();
	cStrWind.Trim();
	cStrNature.Trim();

	if((_tcscmp(cStrLevel, TEXT("")) == 0) || (_tcscmp(cStrMonCreateNum, TEXT("")) == 0) || (_tcscmp(cStrMonAC, TEXT("")) == 0) || (_tcscmp(cStrMinDmg, TEXT("")) == 0)
		|| (_tcscmp(cStrMaxDmg, TEXT("")) == 0) || (_tcscmp(cStrMonStr, TEXT("")) == 0) || (_tcscmp(cStrMonDex, TEXT("")) == 0) || (_tcscmp(cStrMonVit, TEXT("")) == 0) || (_tcscmp(cStrMonIntl, TEXT("")) == 0)
		|| (_tcscmp(cStrMonHnd, TEXT("")) == 0) || (_tcscmp(cStrMonLuk, TEXT("")) == 0) || (_tcscmp(cStrMonWis, TEXT("")) == 0) 
		|| (_tcscmp(cStrFire, TEXT("")) == 0) || (_tcscmp(cStrWater, TEXT("")) == 0) || (_tcscmp(cStrLand, TEXT("")) == 0) || (_tcscmp(cStrWind, TEXT("")) == 0) || (_tcscmp(cStrNature, TEXT("")) == 0))
	{
		NTCHARString256	strTitle	= GetTxtFromMgr(512);
		NTCHARString256	strText		= GetTxtFromMgr(40199);

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		return;
	}

	SI32 siLevel = atoi(szLevel);
	SI32 siMonCreateNum = atoi(szMonCreateNum);
	SI32 siAggro = atoi(szAggro);
	SI32 siMonAC = atoi(szMonAC);
	SI32 siMinDmg = atoi(szMinDmg);
	SI32 siMaxDmg = atoi(szMaxDmg);
	SI32 siMonStr = atoi(szMonStr);
	SI32 siMonDex = atoi(szMonDex);
	SI32 siMonVit = atoi(szMonVit);
	SI32 siMonIntl = atoi(szMonIntl);
	SI32 siMonHnd = atoi(szMonHnd);
	SI32 siMonLuk = atoi(szMonLuk);
	SI32 siMonWis = atoi(szMonWis);
	SI32 siCriRate = atoi(szCriRate);
	SI32 siAttInter = atoi(szAttInter);
	SI32 siFire = atoi(szFire);
	SI32 siWater = atoi(szWater);
	SI32 siLand = atoi(szLand);
	SI32 siWind = atoi(szWind);
	SI32 siNature = atoi(szNature);

	// 1개이상의 능력치를 설정하지 않으면 몬스터 생성을 할 수 없음.
	//if(((m_siLevel == siLevel) && (m_siMonCreateNum == siMonCreateNum) && (m_siAggro == siAggro) && (m_siMonAC == siMonAC) && (m_siMinDmg == siMinDmg) && (m_siMaxDmg == siMaxDmg) && (m_siMonStr == siMonStr)
	//	&& (m_siMonDex == siMonDex) && (m_siMonVit == siMonVit) && (m_siMonIntl == siMonIntl) && (m_siMonHnd == siMonHnd) && (m_siMonLuk == siMonLuk) && (m_siMonWis == siMonWis) && (m_siCriRate == siCriRate)
	//	&& (m_siAttInter == siAttInter) && (m_siFire == siFire) && (m_siWater == siWater) && (m_siLand == siLand) && (m_siWind == siWind) && (m_siNature == siNature)))		
	//{
	//	NTCHARString256	strTitle	= GetTxtFromMgr(512);
	//	NTCHARString256	strText		= GetTxtFromMgr(40199);

	//	pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
	//	return;
	//}

	// 몬스터 생성 요청
	cltGameRequest_MonsterCreate clMonsterCreateMsg( m_siMonsterKind, siLevel, siMonCreateNum, siAggro, siMonAC, siMinDmg, siMaxDmg, siMonStr, siMonDex, siMonVit, siMonIntl, siMonHnd, siMonLuk, siMonWis, 
		                                             siCriRate, siAttInter, siFire, siWater, siLand, siWind, siNature );
	cltMsg clMsg( GAMEMSG_REQUEST_MONSTERCREATE, sizeof(clMonsterCreateMsg), (BYTE*)&clMonsterCreateMsg );
	pclclient->SendMsgToServer( (sPacketHeader*)& clMsg );
}