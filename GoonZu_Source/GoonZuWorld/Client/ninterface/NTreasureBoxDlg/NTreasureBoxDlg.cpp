#include "NTreasureBoxDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "Char/CharManager/CharManager.h"
#include "Char/CharCommon/Char-Common.h"



extern cltCommonLogic* pclClient;

CNTreasureBoxDlg::CNTreasureBoxDlg()
{
	GP.LoadSprFromFile( TEXT("gimg/Question.SPR"), &m_sprQuestion);

	Init();
}

CNTreasureBoxDlg::~CNTreasureBoxDlg()
{
	if ( m_sprQuestion.pImage )
	{
		GP.FreeSpr( m_sprQuestion );
	}
	Destroy();
}

void CNTreasureBoxDlg::Init()
{
	m_pushTime		= 0;
	m_pBuffer		= NULL;
	
	m_siBoxPos		= 0;					// 포지션
	m_bIsDrawing	= false;

	m_siFontIndex	= 0;
	m_bSend			= false;

	m_clItem.Init();

}

void CNTreasureBoxDlg::Destroy()
{

}

void CNTreasureBoxDlg::Create()
{
	cltClient* pclclient = (cltClient*)pclClient;

	if ( !IsCreate() )
	{
		CInterfaceFile	file;

		file.LoadFile(TEXT("NInterface/Data/NTreasureBox/DLG_NTreasureBox.ddf"));
		file.CreatePopupDialog( this, NTREASUREBOX_DLG, TEXT("dialog_treasurebox"),StaticTreasureBoxDlgProc );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC ,NTREASURERBOX_IMAGE_BOXIMAGE, this) ,NTREASURERBOX_IMAGE_BOXIMAGE, TEXT("imagestatic_box"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC , NTREASURERBOX_STATIC_STATUS, this)  , NTREASURERBOX_STATIC_STATUS , TEXT("static_status"));

		CDialogController::SetTimerOnOff( true );
		CDialogController::SetActionTimer(100);

		Show( true );
	}
}

void CALLBACK CNTreasureBoxDlg::StaticTreasureBoxDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNTreasureBoxDlg *pThis = (CNTreasureBoxDlg*) pControllerMgr;
	pThis->NTreasureBoxDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNTreasureBoxDlg::NTreasureBoxDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	default:
		break;
	}
}

void CNTreasureBoxDlg::Action()
{
	if( m_bIsDrawing && !m_bSend  &&  ( TABS(pclClient->CurrentClock - m_pushTime ) > 3000 ) )
	{
		if ( m_clItem.siUnique == pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[m_siBoxPos].siUnique )
		{
			m_pBuffer->Push( INV_USE_ITEM, m_siBoxPos, &m_clItem);
			m_bSend = true;
		}
		else
		{
			StopDraw();
		}
	}

	if ( m_bIsDrawing )
	{
		m_siFontIndex = (++m_siFontIndex) % TB_VARIABLE_MAXFRAMENUM;
	}
/*
	if ( m_bIsDrawing )
	{
		m_siFontIndex++;
		if ( m_siFontIndex == 2 )	m_siFontIndex = 0;

		(m_InterfaceFactory.GetImageStatic(NTREASURERBOX_IMAGE_BOXIMAGE))->SetFontIndex(m_siFontIndex);
	}
*/
}

void CNTreasureBoxDlg::SetTreasureBox( SI32 BoxPos, cltItem* Item, CItemInventoryBuffer* Buffer )
{
	if ( (NULL == Item) || (NULL == Buffer) )
	{
		return;
	}

	Init();

	m_siBoxPos		= BoxPos;
	m_pBuffer		= Buffer;
	m_clItem.Set(Item);
	
	m_bIsDrawing	= true;
	m_pushTime		= pclClient->CurrentClock;

	// 다이얼로그에 뿌려질 텍스트
	NTCHARString32 kText( GetTxtFromMgr(6632) );
	(m_InterfaceFactory.GetStatic(NTREASURERBOX_STATIC_STATUS))->SetText( kText );

	// 다이얼로그 제목
	NTCHARString128 kTitle( GetTxtFromMgr(8397) );
	TCHAR szItemName[128] = {'\0',};
	
	if ( m_clItem.GetFullName( pclClient->pclItemManager, szItemName, 128 ) == true )
	{
		kTitle.Replace( TEXT("#itemname#"), szItemName );
	}
	else
	{
		kTitle.Replace( TEXT("[#itemname#]"), TEXT("") );
		kTitle.Replace( TEXT(" "), TEXT("") );
	}

	SetTitle( kTitle );

	// 아이템 이미지 구하기
	CImageStatic *pImageStatic = m_InterfaceFactory.GetImageStatic( NTREASURERBOX_IMAGE_BOXIMAGE );
	if ( NULL == pImageStatic )
	{
		return;
	}

	SI32 siItemUnique = m_clItem.siUnique;
	if ( siItemUnique <= 0 )
	{
		return;
	}

	SI32 siImageType	= 0;
	SI32 siImageIndex	= 0;

	if ( true == pclClient->pclItemManager->GetItemGImageFileIndexAndFont(siItemUnique, &siImageType, &siImageIndex) )
	{
		pImageStatic->SetItemSprType( siImageType );
		pImageStatic->SetFontIndex( siImageIndex );
	}

}

void CNTreasureBoxDlg::ShowTreasureItem(SI32 Unique, SI32 Number)
{
	// 2010 신년이벤트(상자아이템 클릭시 운세만일땐 당첨된 아이템 박스 나타나지 않도록..)
	if ( 0 >= Unique && 0 >= Number )
	{
		DeleteDialog();
		return;
	}

	// 당첨된 아이템을 보여준다!!.

	TCHAR buffer[256];

	TCHAR* pText = GetTxtFromMgr(6633);

	StringCchPrintf( buffer, 256, pText, pclClient->pclItemManager->GetName(Unique), Number);
	(m_InterfaceFactory.GetStatic(NTREASURERBOX_STATIC_STATUS))->SetText(buffer);

    SI32 itemTypeIndex = 0;
	SI32 itemFontIndex = 0;

	pclClient->pclItemManager->GetItemGImageFileIndexAndFont( Unique, &itemTypeIndex, &itemFontIndex );

	(m_InterfaceFactory.GetImageStatic(NTREASURERBOX_IMAGE_BOXIMAGE))->SetItemSprType(itemTypeIndex);
	(m_InterfaceFactory.GetImageStatic(NTREASURERBOX_IMAGE_BOXIMAGE))->SetFontIndex(itemFontIndex);

	TCHAR* pTitle = GetTxtFromMgr(6641);
	SetTitle(pTitle);

	StopDraw();
	
}

bool CNTreasureBoxDlg::CanDraw()
{
	if ( m_bIsDrawing || m_bSend )
		return false;
	else
		return true;
}

void CNTreasureBoxDlg::SetErrorText( TCHAR* Text)
{
	(m_InterfaceFactory.GetStatic(NTREASURERBOX_STATIC_STATUS))->SetText(Text);
	//(m_InterfaceFactory.GetImageStatic(NTREASURERBOX_IMAGE_BOXIMAGE))->SetFontIndex(0);
	StopDraw();
}

void CNTreasureBoxDlg::StopDraw()
{
	m_bIsDrawing = false;
	m_bSend = false;
	
}

void CNTreasureBoxDlg::SetTitle(TCHAR* Text)
{
	CDialogController::SetTitle( Text );
}

void CNTreasureBoxDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if ( m_bIsDrawing )
	{
		DrawQuestion( ScreenRX, ScreenRY );
	}

}

void CNTreasureBoxDlg::DrawQuestion( SI32 ScreenRX, SI32 ScreenRY )
{
	SI32 siDrawFrame[TB_VARIABLE_MAXFRAMENUM] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 8, 0, 8};
	
	SI32 siDrawX = GetX() + (m_InterfaceFactory.GetImageStatic(NTREASURERBOX_IMAGE_BOXIMAGE))->GetX() + ScreenRX;
	SI32 siDrawY = GetY() + (m_InterfaceFactory.GetImageStatic(NTREASURERBOX_IMAGE_BOXIMAGE))->GetY() + ScreenRY;
	

	//GP.PutSprScreen( &m_sprQuestion, siDrawX, siDrawY, 0 );
	//GP.PutSprScreen( &m_sprQuestion, siDrawX, siDrawY, siDrawFrame[m_siFontIndex] );
	GP.PutSpr( &m_sprQuestion, siDrawX, siDrawY, siDrawFrame[m_siFontIndex] );
}