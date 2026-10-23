#include "../Client/ninterface/NArtifactDlg/NArtifactBaseDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"

NArtifactBaseDlg::NArtifactBaseDlg() 
{
	
}

NArtifactBaseDlg::~NArtifactBaseDlg()
{

}
void NArtifactBaseDlg::Clear( void )
{
	m_NArtifactMixtureDlg.Clear();
	m_NArtifactSlotCreateDlg.Clear();
	m_NArtifactRestoreDlg.Clear();
	m_NArtifactRemoveDlg.Clear();
}


void NArtifactBaseDlg::Create( void )
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NArtifactDlg/NArtifactBaseDlg.ddf" ) );
	file.CreatePopupDialog( this, ARTIFACT_BASE_DLG, TEXT( "dialog_Base" ), StaticNArtifactBaseDlgProc );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  ARTIFACT_BASE_IMAGESTATIC_PORTRAIT,	this ), ARTIFACT_BASE_IMAGESTATIC_PORTRAIT,	TEXT( "imagestatic_portrait" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  ARTIFACT_BASE_STATIC_EXPLAIN,			this ), ARTIFACT_BASE_STATIC_EXPLAIN,		TEXT( "statictext_explain" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	 ARTIFACT_BASE_BUTTON_MIXTURE,			this ), ARTIFACT_BASE_BUTTON_MIXTURE,		TEXT( "button_tab1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	 ARTIFACT_BASE_BUTTON_RESTORE,			this ), ARTIFACT_BASE_BUTTON_RESTORE,		TEXT( "button_tab2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  ARTIFACT_BASE_BUTTON_SLOT_CREATE,		this ), ARTIFACT_BASE_BUTTON_SLOT_CREATE,	TEXT( "button_tab3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  ARTIFACT_BASE_BUTTON_REMOVE,			this ), ARTIFACT_BASE_BUTTON_REMOVE,		TEXT( "button_tab4" ) );	
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, ARTIFACT_BASE_OUTLINE1,				this ), ARTIFACT_BASE_OUTLINE1,				TEXT( "outline_NONAME1" ) );	
	
	if( false == m_NArtifactMixtureDlg.IsCreate() )
	{
		m_NArtifactMixtureDlg.Create( this );
		m_NArtifactMixtureDlg.Show( true );
	}
	
	if( false == m_NArtifactSlotCreateDlg.IsCreate() )
	{
		m_NArtifactSlotCreateDlg.Create( this );
		m_NArtifactSlotCreateDlg.Show( false );
	}
	if( false == m_NArtifactRestoreDlg.IsCreate() )
	{
		m_NArtifactRestoreDlg.Create( this );
		m_NArtifactRestoreDlg.Show( false );
	}

	if( false == m_NArtifactRemoveDlg.IsCreate() )
	{
		m_NArtifactRemoveDlg.Create( this );
		m_NArtifactRemoveDlg.Show( false );
	}

	SetPotrait();
	SetX(100);       
	SetY(140);

	ButtonSetting( MIX );
	SetExplain(MIX);
	 

	SetActionTimer( 100 );    
	SetTimerOnOff( true );
}


void CALLBACK NArtifactBaseDlg::StaticNArtifactBaseDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NArtifactBaseDlg *pThis = (NArtifactBaseDlg*) pControllerMgr;
	pThis->NArtifactBaseDlgProc( nEvent, nControlID, pControl );
}


void CALLBACK NArtifactBaseDlg::NArtifactBaseDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	if(true == IsGage())
		return;	

	switch ( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case ARTIFACT_BASE_BUTTON_MIXTURE:
			{	
				Clear();
				
				m_NArtifactSlotCreateDlg.Show( false );
				m_NArtifactRestoreDlg.Show( false );
				m_NArtifactRemoveDlg.Show( false );

				m_NArtifactMixtureDlg.Show( true );
				
				ButtonSetting( MIX );
				SetExplain(MIX);
			}
			break;
		case ARTIFACT_BASE_BUTTON_RESTORE:
			{
				Clear();

				m_NArtifactSlotCreateDlg.Show( false );
				m_NArtifactMixtureDlg.Show( false );
				m_NArtifactRemoveDlg.Show( false );

				m_NArtifactRestoreDlg.Show( true );

				ButtonSetting( RESTORE );
				SetExplain(RESTORE);
			}
			break;	
		case ARTIFACT_BASE_BUTTON_SLOT_CREATE:
			{
				Clear();

				m_NArtifactMixtureDlg.Show( false );
				m_NArtifactRestoreDlg.Show( false );
				m_NArtifactRemoveDlg.Show( false );

				m_NArtifactSlotCreateDlg.Show( true );

				ButtonSetting( CREATE );
				SetExplain(CREATE);
			}
			break;
		case ARTIFACT_BASE_BUTTON_REMOVE:
			{
				Clear();

				m_NArtifactSlotCreateDlg.Show( false );
				m_NArtifactMixtureDlg.Show( false );
				m_NArtifactRestoreDlg.Show( false );

				m_NArtifactRemoveDlg.Show( true );

				ButtonSetting( REMOVE );
				SetExplain(REMOVE);
			}
			break;
	}
}

void NArtifactBaseDlg::Action(void)
{
	m_NArtifactMixtureDlg.Action();
	m_NArtifactSlotCreateDlg.Action();
	m_NArtifactRestoreDlg.Action();
	m_NArtifactRemoveDlg.Action();
}


void NArtifactBaseDlg::ButtonSetting( SI32 _ButtonKind )
{
	CButton* pButton = NULL;
	for( int count = ARTIFACT_BASE_BUTTON_MIXTURE; count <= ARTIFACT_BASE_BUTTON_REMOVE; ++count )
	{
		pButton = m_InterfaceFactory.GetButton( count );
		//pButton->SetFontSize(11); 
		pButton->SetFontWeight(FW_NORMAL);
		pButton->SetFontColor(RGB(120, 120, 120));           
		pButton->Refresh();      
	}

	SI32 TypeID = -1; 
	switch( _ButtonKind )  
	{
		case MIX:		TypeID = ARTIFACT_BASE_BUTTON_MIXTURE;		break;
		case CREATE:	TypeID = ARTIFACT_BASE_BUTTON_SLOT_CREATE;	break;
		case RESTORE:	TypeID = ARTIFACT_BASE_BUTTON_RESTORE;		break;
		case REMOVE:	TypeID = ARTIFACT_BASE_BUTTON_REMOVE;		break;
	}

	pButton = m_InterfaceFactory.GetButton( TypeID );
	if(-1 == TypeID)
		return; 

	//pButton->SetFontSize(13);
	pButton->SetFontWeight(FW_BOLD);
	pButton->SetFontColor(RGB(0 ,0 ,0));
	pButton->Refresh();  
}

// 초상화 그리기
void NArtifactBaseDlg::SetPotrait( void )
{
	TCHAR* pText	 = GetTxtFromMgr(8080);
	SI32   siNCPKind = pclClient->pclKindInfoSet->FindKindFromName( pText );
	
	if(pclClient->pclKindInfoSet->pclKI[ siNCPKind ])
	{
		CImageStatic* pImageStatic		 = m_InterfaceFactory.GetImageStatic( ARTIFACT_BASE_IMAGESTATIC_PORTRAIT );
		
		TCHAR		  strFileName[ 128 ] = TEXT("");
		SI16		  tmpSelectedIndex   = pclClient->pclKindInfoSet->FindKindFromName( pText );
		TCHAR		  szPortrait[256]    = "";
		SI32		  siPortraitFont     = 0;
		
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(tmpSelectedIndex, szPortrait, sizeof(szPortrait), &siPortraitFont);

		pImageStatic->SetFileName(szPortrait);
		pImageStatic->SetFontIndex( siPortraitFont );
	}
}

void NArtifactBaseDlg::SetExplain(SI32 _siType)
{
	SI32 TextNum = 0;
	switch(_siType)
	{
		case MIX:		TextNum = 8201; break;
		case CREATE:	TextNum = 8199; break;
		case RESTORE:	TextNum = 8200; break;
		case REMOVE:	TextNum = 8202; break;
	}
		
	if(0 == TextNum)
		return;
	
	m_InterfaceFactory.GetStatic(ARTIFACT_BASE_STATIC_EXPLAIN)->SetText(GetTxtFromMgr(TextNum));
}

bool NArtifactBaseDlg::IsGage(void)
{
	
	if(	m_NArtifactMixtureDlg.m_bGaugeAction	|| m_NArtifactMixtureDlg.m_bEffectAction	||
		m_NArtifactSlotCreateDlg.m_bGaugeAction || m_NArtifactSlotCreateDlg.m_bEffectAction ||
		m_NArtifactRestoreDlg.m_bGaugeAction	|| m_NArtifactRestoreDlg.m_bEffectAction	||
		m_NArtifactRemoveDlg.m_bGaugeAction		|| m_NArtifactRemoveDlg.m_bEffectAction			)
		return true;
	
	return false;
}

void NArtifactBaseDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:
		{
			if(RetValue)
			{
				m_NArtifactSlotCreateDlg.RetMsgBox( RetValue, MsgIndex, pData, DataLen );
			}
		}
		break;
	case 1:
		{
			if(RetValue)
			{
				m_NArtifactRestoreDlg.RetMsgBox( RetValue, MsgIndex, pData, DataLen );
			}
		}
		break;
	default:	
		{

		}
		break;
	}
}