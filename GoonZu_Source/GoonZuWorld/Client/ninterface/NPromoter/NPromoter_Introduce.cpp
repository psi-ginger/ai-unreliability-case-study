#include "NPromoter_Introduce.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../common/Quest/Quest.h"

#include "Msg/MsgType-ItemMall.h"

NPromoter_Introduce::NPromoter_Introduce()
{
}

NPromoter_Introduce::~NPromoter_Introduce()
{
}

void NPromoter_Introduce::Create()
{
	if( IsCreate() )	return;


	CInterfaceFile file;

    file.LoadFile("Ninterface/Data/NPromoterDlg/NPromoter_Introduce.ddf");
	file.CreatePopupDialog( this , NPROMOTER_INTRODUCE, "dialog_introduce" , StaticNPromoter_IntroduceProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NPROMOTER_INTRODUCE_BUTTON_WARP,			this), NPROMOTER_INTRODUCE_BUTTON_WARP,				"button_warp");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NPROMOTER_INTRODUCE_BUTTON_WHISPERER,		this), NPROMOTER_INTRODUCE_BUTTON_WHISPERER,		"button_whisperer");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NPROMOTER_INTRODUCE_EDIT_EXPLAIN,			this), NPROMOTER_INTRODUCE_EDIT_EXPLAIN,			"editbox_explain");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,NPROMOTER_INTRODUCE_IMAGESTATIC_PORTRAIT,	this), NPROMOTER_INTRODUCE_IMAGESTATIC_PORTRAIT,	"imagestatic_portrait");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NPROMOTER_INTRODUCE_STATIC_NAME,			this), NPROMOTER_INTRODUCE_STATIC_NAME,				"statictext_name");

	m_InterfaceFactory.GetStatic( NPROMOTER_INTRODUCE_STATIC_NAME )->SetTextPrintOption( DT_CENTER | DT_VCENTER );
}

void CALLBACK NPromoter_Introduce::StaticNPromoter_IntroduceProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NPromoter_Introduce *pThis = (NPromoter_Introduce *)pControllerMgr;
	pThis->NPromoter_IntroduceProc( nEvent, nControlID, pControl );
}

void CALLBACK NPromoter_Introduce::NPromoter_IntroduceProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		
		case NPROMOTER_INTRODUCE_BUTTON_WARP:
			{
				if( _tcslen(m_WhispererCharName) )
				{
					cltGameMsgRequest_PromoterWarp clInfo( m_WhispererCharName );
					cltMsg clMsg( GAMEMSG_REQUEST_PROMOTER_WARP, sizeof(clInfo), (BYTE*)&clInfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
			}
			break;
			
		case NPROMOTER_INTRODUCE_BUTTON_WHISPERER:
			{
				if( _tcslen( m_WhispererCharName ) )
					pclclient->RequestSecretChatByCharName( m_WhispererCharName );
			}
			break;

		default:
			{
			}
			break;
	}
}


void NPromoter_Introduce::SetRecommendIntroduce( TCHAR* _pszRecommendName, SI32 _siKind, bool _siIsConnect )
{
	m_InterfaceFactory.GetStatic(NPROMOTER_INTRODUCE_STATIC_NAME)->SetText( _pszRecommendName );
	
	NTCHARString256 szExplain( GetTxtFromMgr(30328) );
	szExplain.Replace( TEXT("#recommendee#"), pclClient->pclCM->CR[1]->GetName() );
	szExplain.Replace( TEXT("#recommend#"),   _pszRecommendName  );

	m_InterfaceFactory.GetEdit(NPROMOTER_INTRODUCE_EDIT_EXPLAIN)->SetText( szExplain );

	if( _pszRecommendName )
		StringCchCopy( m_WhispererCharName, sizeof(m_WhispererCharName), _pszRecommendName );

	
	TCHAR	szDummy[256]	= "";
	SI32	siPortraitFont	= 0;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont( _siKind, szDummy, sizeof(szDummy), &siPortraitFont);
	m_InterfaceFactory.GetImageStatic(NPROMOTER_INTRODUCE_IMAGESTATIC_PORTRAIT)->SetFontIndex( siPortraitFont );
}

void NPromoter_Introduce::SetRecommendeeIntroduce( TCHAR* _pszRecommendeeName, SI32 _siKind, bool _siIsConnect )
{
	m_InterfaceFactory.GetButton(NPROMOTER_INTRODUCE_BUTTON_WARP)->Show( false );
	m_InterfaceFactory.GetButton(NPROMOTER_INTRODUCE_BUTTON_WHISPERER)->Enable( _siIsConnect );

	m_InterfaceFactory.GetStatic(NPROMOTER_INTRODUCE_STATIC_NAME)->SetText( _pszRecommendeeName );

	NTCHARString256 szExplain( GetTxtFromMgr(30327) );
	szExplain.Replace( TEXT("#recommendee#"), _pszRecommendeeName );
	
	m_InterfaceFactory.GetEdit(NPROMOTER_INTRODUCE_EDIT_EXPLAIN)->SetText( szExplain );

	if( _pszRecommendeeName )
		StringCchCopy( m_WhispererCharName, sizeof(m_WhispererCharName), _pszRecommendeeName );

	TCHAR	szDummy[256]	= "";
	SI32	siPortraitFont	= 0;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont( _siKind, szDummy, sizeof(szDummy), &siPortraitFont);
	m_InterfaceFactory.GetImageStatic(NPROMOTER_INTRODUCE_IMAGESTATIC_PORTRAIT)->SetFontIndex( siPortraitFont );
}
