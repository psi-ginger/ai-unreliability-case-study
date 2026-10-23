#include "NEmoticonDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "../../common/Char/CharManager/CharManager.h"
#include "../../Client/Effect/Emoticon/EmoticonMgr.h"



extern cltCommonLogic* pclClient;

#define	LINE_COUNTNUM	6	// 1Line에 표시될 이모티콘의 갯수

CNEmoticonDlg::CNEmoticonDlg()
{
	Initialize();
}

CNEmoticonDlg::~CNEmoticonDlg()
{
	Destroy();
}

void CNEmoticonDlg::Initialize()
{

	m_siEmoticonCount = 0;
	m_siUnderMouseIndex = 0;
}

void CNEmoticonDlg::Destroy()
{

}

void CNEmoticonDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/NEmoticonDlg/DLG_NEmoticonDlg.ddf"));
	file.CreatePopupDialog( this, NEMOTICON_DLG, TEXT("dialog_emoticon"),StaticEmoticonDlgProc );

	m_siEmoticonCount = pclclient->clEmoticonInfo.GetEmoticonCount();

	if ( m_siEmoticonCount <= 0 )
	{
		DeleteDialog();
	}

	SI32 dialogWidth = 12;
	SI32 dialogHeight = 0;

	if ( m_siEmoticonCount < LINE_COUNTNUM )
	{
		dialogWidth += m_siEmoticonCount * 35;
	}
	else
	{
		dialogWidth += LINE_COUNTNUM * 35;
	}

	dialogHeight = 35 + ( m_siEmoticonCount / LINE_COUNTNUM * 35 );

	SetDialogPos( 300, 300, dialogWidth, dialogHeight );
	
	TCHAR *pTitle = GetTxtFromMgr(7114);//이모티콘 타이틀 추가.[2006.06.29 성웅]
	SetTitle(pTitle);

	for ( SI32 i=0; i < m_siEmoticonCount ; i++ )
	{
		CButton* pButton = ((CButton*)(m_InterfaceFactory.CreateUserInterface( BUTTON , NEMOTICON_BUTTON_0 + i, this )));

		pButton->Create( NEMOTICON_BUTTON_0 + i, 
								TEXT(""),
								"NInterface/Button/button_34x34_00_000.spr", 
								0, 
								false, 
								0, 
								7 + i % LINE_COUNTNUM * 35,
								20+ i / LINE_COUNTNUM * 35,
								34, 34, 34, 34 );

		TCHAR tooltip[64];
		StringCchPrintf( tooltip, 64, TEXT("%s\r\n~%s"),
							pclclient->clEmoticonInfo.GetInfo(i),
							pclclient->clEmoticonInfo.GetCode(i) );
#ifdef _DEBUG
		pButton->SetUseToolTip( tooltip ); //이모티콘 툴팁 출력 부분.
#else
		if ( pclclient->siServiceArea == ConstServiceArea_China )
		{
			pButton->SetUseToolTip( tooltip );
		}
#endif
	}

	m_pCloseButton->MoveController( dialogWidth - 20, 2 );

	CDialogController* pControl = pclclient->m_pDialog[NCHATBAR_DLG];
	this->MovePos( 	pControl->GetX() + pControl->GetWidth() , pControl->GetY() - dialogHeight + 17);

	Show( false );
}

void CNEmoticonDlg::Action()
{

}

void CALLBACK CNEmoticonDlg::StaticEmoticonDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNEmoticonDlg *pThis = (CNEmoticonDlg*) pControllerMgr;
	pThis->NEmoticonDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK CNEmoticonDlg::NEmoticonDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Show( false );
		}
		break;
	default:
		break;
	}

	for ( SI16 i = 0; i < m_siEmoticonCount; i ++ )
	{
		if ( NEMOTICON_BUTTON_0 + i == nControlID )
		{
			if ( nEvent == EVENT_BUTTON_CLICKED )
			{
				pclclient->SendEmoticon(i);
			}
		}
	}
}
void CNEmoticonDlg::DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient* pclclient = (cltClient*)pclClient;

	TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr(GIMG_EMOTICON);

	SI32 fontIndex = 0;
	SI32 Num	   = 0;
	for ( SI16 i=0; i < m_siEmoticonCount; i++  )
	{
		fontIndex = pclclient->clEmoticonInfo.GetStartFrame(i);
		Num	      = pclclient->clEmoticonInfo.GetAniStepNumber(i)-1;
		
		GP.PutSpr( pSpr, 
			ScreenRX + GetX() + 9 + i % LINE_COUNTNUM * 35, 
			ScreenRY + GetY() + 22 + i / LINE_COUNTNUM * 35,
			fontIndex + Num
			);
	}
}