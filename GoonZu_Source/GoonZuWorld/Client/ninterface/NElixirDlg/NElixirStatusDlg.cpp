#include "NElixirStatusDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

NElixirStatusDlg::NElixirStatusDlg()
{
}

NElixirStatusDlg::~NElixirStatusDlg()
{
	Destroy();
}

void NElixirStatusDlg::Init()
{
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_STRENGTH	)->SetChangeTextDrawMode(true);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_HAND		)->SetChangeTextDrawMode(true);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_DEX		)->SetChangeTextDrawMode(true);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_MAGIC	)->SetChangeTextDrawMode(true);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_WISDOM	)->SetChangeTextDrawMode(true);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_LIFE		)->SetChangeTextDrawMode(true);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_POINT	)->SetChangeTextDrawMode(true);

	m_InterfaceFactory.GetStatic(NELIXIR_STATUS_STATICTEXT_POINT	)->SetTextPrintOption(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_InterfaceFactory.GetStatic(NELIXIR_STATUS_STATICTEXT_POINT	)->Refresh();
	/*m_InterfaceFactory.GetStatic(NELIXIR_STATUS_STATICTEXT_END_TIME )->SetTextPrintOption(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_InterfaceFactory.GetStatic(NELIXIR_STATUS_STATICTEXT_END_TIME	)->Refresh();*/
 
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_STRENGTH			)->SetTextPrintOption(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_HAND				)->SetTextPrintOption(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_DEX				)->SetTextPrintOption(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_MAGIC			)->SetTextPrintOption(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_WISDOM			)->SetTextPrintOption(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_LIFE				)->SetTextPrintOption(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_InterfaceFactory.GetEdit(NELIXIR_STATUS_EDIT_POINT			)->SetTextPrintOption(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
 
	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if( NULL == pclChar )	return;

	SetElixirInfo( pclChar->pcltElixir );
}

void NElixirStatusDlg::Destroy()
{


}

void NElixirStatusDlg::Create()
{
	if( IsCreate() )
	{
        return;
	}

	CInterfaceFile file;

    file.LoadFile("Ninterface/Data/NElixirDlg/NElixirStatusDlg.ddf");

	file.CreatePopupDialog( this, NELIXIR_STATUS_DLG, "dialog_elixir" , StatiNElixirStatusDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_STRENGTH_PLUS,	this), 	NELIXIR_STATUS_BUTTON_STRENGTH_PLUS,	TEXT("button_strength_plus")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_HAND_PLUS,		this), 	NELIXIR_STATUS_BUTTON_HAND_PLUS,		TEXT("button_hand_plus")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_DEX_PLUS,			this), 	NELIXIR_STATUS_BUTTON_DEX_PLUS,			TEXT("button_dex_plus")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_MAGIC_PLUS,		this), 	NELIXIR_STATUS_BUTTON_MAGIC_PLUS,		TEXT("button_magic_plus")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_WISDOM_PLUS,		this), 	NELIXIR_STATUS_BUTTON_WISDOM_PLUS,		TEXT("button_wisdom_plus")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_LIFE_PLUS,		this), 	NELIXIR_STATUS_BUTTON_LIFE_PLUS,		TEXT("button_life_plus")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_STRENGTH_MINUS,	this), 	NELIXIR_STATUS_BUTTON_STRENGTH_MINUS,	TEXT("button_strength_minus")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_HAND_MINUS,		this), 	NELIXIR_STATUS_BUTTON_HAND_MINUS,		TEXT("button_hand_minus")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_DEX_MINUS,		this), 	NELIXIR_STATUS_BUTTON_DEX_MINUS,		TEXT("button_dex_minus")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_MAGIC_MINUS,		this), 	NELIXIR_STATUS_BUTTON_MAGIC_MINUS,		TEXT("button_magic_minus")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_WISDOM_MINUS,		this), 	NELIXIR_STATUS_BUTTON_WISDOM_MINUS,		TEXT("button_wisdom_minus")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NELIXIR_STATUS_BUTTON_LIFE_MINUS,		this), 	NELIXIR_STATUS_BUTTON_LIFE_MINUS,		TEXT("button_life_minus")		);
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_STATUS_STATICTEXT_STRENGTH,		this),  NELIXIR_STATUS_STATICTEXT_STRENGTH,		TEXT("statictext_strength")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_STATUS_STATICTEXT_HAND,			this),  NELIXIR_STATUS_STATICTEXT_HAND,			TEXT("statictext_hand")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_STATUS_STATICTEXT_DEX,			this),  NELIXIR_STATUS_STATICTEXT_DEX,			TEXT("statictext_dex")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_STATUS_STATICTEXT_MAGIC,		this),  NELIXIR_STATUS_STATICTEXT_MAGIC,		TEXT("statictext_magic")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_STATUS_STATICTEXT_WISDOM,		this),  NELIXIR_STATUS_STATICTEXT_WISDOM,		TEXT("statictext_wisdom")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_STATUS_STATICTEXT_LIFE,			this),  NELIXIR_STATUS_STATICTEXT_LIFE,			TEXT("statictext_life")			);

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NELIXIR_STATUS_EDIT_STRENGTH,			this),  NELIXIR_STATUS_EDIT_STRENGTH,			TEXT("editbox_strength")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NELIXIR_STATUS_EDIT_HAND,				this),  NELIXIR_STATUS_EDIT_HAND,				TEXT("editbox_hand")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NELIXIR_STATUS_EDIT_DEX,				this),  NELIXIR_STATUS_EDIT_DEX,				TEXT("editbox_dex")				);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NELIXIR_STATUS_EDIT_MAGIC,				this),  NELIXIR_STATUS_EDIT_MAGIC,				TEXT("editbox_magic")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NELIXIR_STATUS_EDIT_WISDOM,				this),  NELIXIR_STATUS_EDIT_WISDOM,				TEXT("editbox_wisdom")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NELIXIR_STATUS_EDIT_LIFE,				this),  NELIXIR_STATUS_EDIT_LIFE,				TEXT("editbox_life")			);

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,		NELIXIR_STATUS_STATICTEXT_POINT,		this),  NELIXIR_STATUS_STATICTEXT_POINT,		TEXT("statictext_bonus")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NELIXIR_STATUS_EDIT_POINT,				this),  NELIXIR_STATUS_EDIT_POINT,				TEXT("editbox_bonus")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, NELIXIR_STATUS_IMAGESTATIC_BACK,		this),  NELIXIR_STATUS_IMAGESTATIC_BACK,		TEXT("imagestatic_bonus")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NELIXIR_STATUS_BUTTON_APPLY,			this),  NELIXIR_STATUS_BUTTON_APPLY,			TEXT("button_statusapply")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NELIXIR_STATUS_BUTTON_CANCEL,			this),  NELIXIR_STATUS_BUTTON_CANCEL,			TEXT("button_statusclear")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,		NELIXIR_STATUS_STATICTEXT_END_TIME,		this),  NELIXIR_STATUS_STATICTEXT_END_TIME,		TEXT("statictext_end_time")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_STATUS_STATICTEXT_TIME,			this),  NELIXIR_STATUS_STATICTEXT_TIME,			TEXT("statictext_time")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,  	NELIXIR_STATUS_OUTLINE1,				this),  NELIXIR_STATUS_OUTLINE1,				TEXT("outline_NONAME1"));


	// 창 생성 위치를 캐릭터 정보창을 기준으로 설정하는거여~.
	cltClient * pclclient = (cltClient*) pclClient;
	if( pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ] ) 
	{
		if( pclclient->GetScreenXSize()/2 < pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]->GetX() )
		{
			SetX( pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]->GetX() - GetWidth() );
			SetY( pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]->GetY() );
		}
		else
		{
			SetX( pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]->GetX() + pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]->GetWidth() );
			SetY( pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]->GetY() );
		}
	}
	

	Init();
}

void NElixirStatusDlg::Action()
{

}

void CALLBACK NElixirStatusDlg::StatiNElixirStatusDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NElixirStatusDlg *pThis = (NElixirStatusDlg *)pControllerMgr;
	pThis->NElixirStatusDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK NElixirStatusDlg::NElixirStatusDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;

		case NELIXIR_STATUS_BUTTON_STRENGTH_PLUS:	
		case NELIXIR_STATUS_BUTTON_HAND_PLUS:	
		case NELIXIR_STATUS_BUTTON_DEX_PLUS:
		case NELIXIR_STATUS_BUTTON_MAGIC_PLUS:
		case NELIXIR_STATUS_BUTTON_WISDOM_PLUS:
		case NELIXIR_STATUS_BUTTON_LIFE_PLUS:	
			{	
				if(GetKeyState(VK_SHIFT) & 0x8000)
				{
					Calculation( nControlID, 10 );
				}
				else
				{
					Calculation( nControlID, 1 );
				}
			}
			break;
		case NELIXIR_STATUS_BUTTON_STRENGTH_MINUS:	
		case NELIXIR_STATUS_BUTTON_HAND_MINUS:	
		case NELIXIR_STATUS_BUTTON_DEX_MINUS:	
		case NELIXIR_STATUS_BUTTON_MAGIC_MINUS:	
		case NELIXIR_STATUS_BUTTON_WISDOM_MINUS:	
		case NELIXIR_STATUS_BUTTON_LIFE_MINUS:	
			{
				if(GetKeyState(VK_SHIFT) & 0x8000)
				{
					Calculation( nControlID, -10 );
				}
				else
				{
					Calculation( nControlID, -1 ); 
				}
			}
			break;
		case NELIXIR_STATUS_BUTTON_APPLY:	
			{ 
				if( 0 >= m_cltPrevElixir.GetPoint() ) return; 
				if( m_cltPrevElixir.GetPoint() == m_cltElixir.GetPoint() ) return;

				stMsgBox MsgBox;
				MsgBox.Set( this, GetTxtFromMgr(6438), GetTxtFromMgr(6437), MSGBOX_TYPE_OKCANCEL, 0 );
				pclclient->SetMsgBox( &MsgBox, NULL, 0 );					
			} 
			break;
		case NELIXIR_STATUS_BUTTON_CANCEL:	
			{
				cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
				if( NULL == pclChar )	return;

				SetElixirInfo( pclChar->pcltElixir );
			}
			break;
		default:
			{

			}
			break;
	}
}

void NElixirStatusDlg::Calculation( SI32 _siStatusType, SI32 _siNum )
{
	SI32 siType = 0;
	switch( _siStatusType )
	{
		case NELIXIR_STATUS_BUTTON_STRENGTH_MINUS:	
		case NELIXIR_STATUS_BUTTON_STRENGTH_PLUS:	siType = cltElixir::TYPE_STR;	break;
		case NELIXIR_STATUS_BUTTON_HAND_MINUS:		
		case NELIXIR_STATUS_BUTTON_HAND_PLUS:		siType = cltElixir::TYPE_HND;	break;
		case NELIXIR_STATUS_BUTTON_DEX_MINUS:		
		case NELIXIR_STATUS_BUTTON_DEX_PLUS:		siType = cltElixir::TYPE_DEX;	break;
		case NELIXIR_STATUS_BUTTON_MAGIC_MINUS:		
		case NELIXIR_STATUS_BUTTON_MAGIC_PLUS:		siType = cltElixir::TYPE_MAG;	break;
		case NELIXIR_STATUS_BUTTON_WISDOM_MINUS:	
		case NELIXIR_STATUS_BUTTON_WISDOM_PLUS:		siType = cltElixir::TYPE_WIS;	break;
		case NELIXIR_STATUS_BUTTON_LIFE_MINUS:		
		case NELIXIR_STATUS_BUTTON_LIFE_PLUS:		siType = cltElixir::TYPE_VIT;	break;
		
		
		default: return;
	}

    m_cltElixir.Calculation( siType, _siNum, &m_cltPrevElixir );	

	SetElixirInfo();
}

void NElixirStatusDlg::SetElixirInfo( cltElixir* _pcltElixir )
{
	_pcltElixir ? memcpy( &m_cltElixir,		_pcltElixir, sizeof(m_cltElixir) )		: 0;	
	_pcltElixir ? memcpy( &m_cltPrevElixir, _pcltElixir, sizeof(m_cltPrevElixir) )	: 0;	
	
	
	m_InterfaceFactory.GetEdit( NELIXIR_STATUS_EDIT_STRENGTH	)->SetText( GetStatusText( cltElixir::TYPE_STR ) );
	m_InterfaceFactory.GetEdit( NELIXIR_STATUS_EDIT_HAND		)->SetText( GetStatusText( cltElixir::TYPE_HND ) ); 
	m_InterfaceFactory.GetEdit( NELIXIR_STATUS_EDIT_DEX			)->SetText( GetStatusText( cltElixir::TYPE_DEX ) );
	m_InterfaceFactory.GetEdit( NELIXIR_STATUS_EDIT_MAGIC		)->SetText( GetStatusText( cltElixir::TYPE_MAG ) );
	m_InterfaceFactory.GetEdit( NELIXIR_STATUS_EDIT_WISDOM		)->SetText( GetStatusText( cltElixir::TYPE_WIS ) ); 
	m_InterfaceFactory.GetEdit( NELIXIR_STATUS_EDIT_LIFE		)->SetText( GetStatusText( cltElixir::TYPE_VIT ) );
	m_InterfaceFactory.GetEdit( NELIXIR_STATUS_EDIT_POINT		)->SetText( GetStatusText( cltElixir::TYPE_POINT ) ); 
 
	TCHAR szTxt[128] = "";
	m_cltPrevElixir.IsElixir() ? m_cltPrevElixir.GetEndDate()->GetDateText( szTxt ) : StringCchPrintf(szTxt, sizeof(szTxt), "%s", GetTxtFromMgr(10100) );
	m_InterfaceFactory.GetStatic( NELIXIR_STATUS_STATICTEXT_END_TIME )->SetText( szTxt );
 
	m_InterfaceFactory.GetButton( NELIXIR_STATUS_BUTTON_APPLY )->Enable( true );
}

TCHAR* NElixirStatusDlg::GetStatusText( SI32 _siStatusType )
{
	static TCHAR szBuffer[32] = {'\0',}; 
	SI16 siPrevValue;
	SI16 siCurValue;

	switch( _siStatusType )
	{
		case cltElixir::TYPE_STR:	siPrevValue = m_cltPrevElixir.GetStr();
									siCurValue	= m_cltElixir.GetStr();
									break;

		case cltElixir::TYPE_HND: 	siPrevValue = m_cltPrevElixir.GetHnd();
									siCurValue	= m_cltElixir.GetHnd();
									break;

		case cltElixir::TYPE_DEX: 	siPrevValue = m_cltPrevElixir.GetDex();
									siCurValue	= m_cltElixir.GetDex();
									break;

		case cltElixir::TYPE_MAG: 	siPrevValue = m_cltPrevElixir.GetMag();
									siCurValue	= m_cltElixir.GetMag();
									break;

		case cltElixir::TYPE_WIS:	siPrevValue = m_cltPrevElixir.GetWis();
									siCurValue	= m_cltElixir.GetWis();
									break;

		case cltElixir::TYPE_VIT: 	siPrevValue = m_cltPrevElixir.GetVit();
									siCurValue	= m_cltElixir.GetVit();
									break;

		case cltElixir::TYPE_POINT:	siPrevValue = m_cltPrevElixir.GetPoint();
									siCurValue	= m_cltElixir.GetPoint();
									break;
		
		default: return NULL;
	}

	siCurValue - siPrevValue ? StringCchPrintf( szBuffer, sizeof(szBuffer), TEXT("%d >   <C08>%d</C>"),  siPrevValue, siCurValue ) 
							 : StringCchPrintf( szBuffer, sizeof(szBuffer), TEXT("%d "),  siPrevValue );  
 
	return szBuffer;  

}


void NElixirStatusDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch(MsgIndex)
	{
		case 0:
			{
				if ( RetValue )
				{
					if( 0 >= m_cltPrevElixir.GetPoint() ) return; 
					if( m_cltPrevElixir.GetPoint() == m_cltElixir.GetPoint() ) return;

					// 서버에 엘릭서 사용 배수를 던져 주고 엘릭서 사용을 시작한다.
					cltGameMsgRequest_Elixir_Set clInfo( &m_cltElixir );
					cltMsg clMsg( GAMEMSG_REQUEST_ELIXIR_SET, sizeof(clInfo), (BYTE*)&clInfo );
					pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg); 

					m_InterfaceFactory.GetButton( NELIXIR_STATUS_BUTTON_APPLY )->Enable( false );
				}
			}
			break;
	}

}



