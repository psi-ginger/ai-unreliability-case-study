#include "NShowGetItemDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client/Client.h"
#include "../../Client/GlobalImgFile/GlobalImgFile.h"
#include "../../common/Item/ItemClient/Item-managerClient.h"
#include "../../CommonLogic/CommonLogic.h"

extern cltCommonLogic* pclClient;

const SI16 NOTIFYGETITME_SHOW_FRAME = 10;		// 약 10초간 보여줌
const SI32 MAX_GET_ITEMCOUNT		= 100000;		// 최대 9999개 ( 넘으면 숫자 보이지 않음.. )

CNShowGetItemDlg::CNShowGetItemDlg()	
{
	//김영훈 수정 2007.12.10 [20391] 아이템 획득 UI개선
	m_pImageStaticBackGround	= NULL;
	m_pImageStaticItemImage		= NULL;
	m_pStatic_itemName			= NULL;
	m_pStatic_itemUnits			= NULL;
	m_pStatic_itemPrice			= NULL;

	m_siCurrentFrame			= 0;
	m_siImageType				= 0;			
	m_siImageIndex				= 0;			

	m_siDialogWidth				= 0;
	m_siItemWidth				= 0;

}
CNShowGetItemDlg::~CNShowGetItemDlg()
{
	SAFE_DELETE( m_pImageStaticBackGround )
	SAFE_DELETE( m_pImageStaticItemImage )
	SAFE_DELETE( m_pStatic_itemName )
	SAFE_DELETE( m_pStatic_itemUnits )
	SAFE_DELETE( m_pStatic_itemPrice )

}
void CNShowGetItemDlg::Create()
{
	m_pImageStaticBackGround	= new CImageStatic( this );
	m_pImageStaticItemImage		= new CImageStatic( this );
	m_pStatic_itemName			= new CStatic( this );
	m_pStatic_itemUnits			= new CStatic( this );
	m_pStatic_itemPrice			= new CStatic( this );


	CInterfaceFile file;
	
	file.LoadFile( TEXT( "NInterface/Data/NShowGetItemDlg/DLG_NShowGetItem.ddf" ) );
	file.CreatePopupDialog( this, NSHOWGETITEM_DLG, TEXT("dialog_showgetitem"), StaticCallBackDialogNShowGetItem, false );

	file.CreateControl( m_pImageStaticItemImage,	NSHOWGETITEM_IMAGESTATIC_ITEMIMAGE,		TEXT( "imagestatic_itemimage" ) );
	file.CreateControl( m_pImageStaticBackGround,	NSHOWGETITEM_IMAGESTATIC_BACKGROUND,	TEXT( "imagestatic_background" ) );

	file.CreateControl( m_pStatic_itemName,			NSHOWGETITEM_STATIC_ITEMNAME,			TEXT( "statictext_itemname" ) );
	file.CreateControl( m_pStatic_itemUnits,		NSHOWGETITEM_STATIC_ITEMUNITS,			TEXT( "statictext_itemunits" ) );
	file.CreateControl( m_pStatic_itemPrice,		NSHOWGETITEM_STATIC_ITEMPRICE,			TEXT( "statictext_itemprice" ) );
	
	m_pStatic_itemName->SetFontColor( RGB(255, 255, 255) );
	m_pStatic_itemUnits->SetFontColor( RGB(255, 255, 255) );
	m_pStatic_itemPrice->SetFontColor( RGB(255, 255, 255) );
	
	if( g_bFullScreen )
        MovePos( ((cltClient*)pclClient)->GetScreenXSize() - 705, 40 );
	else
		MovePos( ((cltClient*)pclClient)->GetScreenXSize() - 655, 40 );
	
	//SetDrawOption( SPR_PUTSPRALPHAT );
	//SetAlphaValue( 7 );
	SetAlphaMode( 7 );

	//다이얼로그와 아이템 출력 컨트롤 크기와 위치 얻어오기
	m_siDialogWidth	= GetWidth();
	m_siItemWidth	= m_pStatic_itemName->GetWidth();

	Show( false );

	SetTimerOnOff( true );

}
void CALLBACK CNShowGetItemDlg::StaticCallBackDialogNShowGetItem( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNShowGetItemDlg *pThis = (CNShowGetItemDlg*)pControllerMgr;
	pThis->CallBackDialogNShowGetItem( nEvent, nControlID, pControl );

}
void CALLBACK CNShowGetItemDlg::CallBackDialogNShowGetItem( UINT nEvent, int nControlID, CControllerObj* pControl )
{
}

void CNShowGetItemDlg::Action()
{
	if ( IsShow() )
	{
		if ( ++m_siCurrentFrame == NOTIFYGETITME_SHOW_FRAME )
		{
			Show( false );
			m_siCurrentFrame = 0;
		}
	}
}

void CNShowGetItemDlg::SetItemData( cltItem* pclItem, TCHAR* ptszItemName, GMONEY gmPrice )
{
	const SI16	MaxBufferSize			= 128;
	SI32		siItemUnique			= pclItem->siUnique;
	SI32		siItemCount				= pclItem->siItemNum;
	TCHAR		buffer[MaxBufferSize]	= TEXT("");
	TCHAR*		ptszTextMgr				= NULL;
	SIZE		sizeString				= {0, };
	
	//아이템 이미지
	if(siItemUnique)
	{
		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siItemUnique, &m_siImageType, &m_siImageIndex ) )
		{}
		else return;
	}

	m_pImageStaticItemImage->SetItemSprType(m_siImageType);
	m_pImageStaticItemImage->SetFontIndex(m_siImageIndex);
	
	//아이템 이름
	ZeroMemory( buffer, MaxBufferSize );
	StringCchPrintf( buffer, MaxBufferSize, TEXT("[%s]"), ptszItemName );
	
	m_pStatic_itemName->SetText( buffer, DT_LEFT | DT_VCENTER | DT_SINGLELINE, &sizeString );

	//아이템 갯수
	ZeroMemory( buffer, MaxBufferSize );
	ptszTextMgr = GetTxtFromMgr(717);
	StringCchPrintf( buffer, MaxBufferSize, ptszTextMgr, siItemCount );

	m_pStatic_itemUnits->SetText( buffer, DT_LEFT | DT_VCENTER | DT_SINGLELINE );

	//아이템 가격
	ZeroMemory( buffer, MaxBufferSize );
	ptszTextMgr = GetTxtFromMgr(611);
	g_JYLibFunction.SetNumUnit(gmPrice, buffer, MaxBufferSize, ptszTextMgr );

	m_pStatic_itemPrice->SetText( buffer, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	
	//문자 길이에 따른 다이얼로그 창 가변 변환
	if ( sizeString.cx > MAX_LIMIT_WIDTH ) 
	{
		//좌/우 쉬프트 연산으로 4로 나눈 후 나머지 값을 뺀 연산과 같은 결과가 된다.
		SI16 siCalcWidth = (( ( sizeString.cx - m_siItemWidth ) >> 2 ) << 2) + ADD_TO_WIDTH;
		//Width를 바꾸는 작업도 부하가 걸리는 작업이기 때문에 길이가 같다면 길이를 다시 설정하지 않는다.
		if ( GetWidth() != (m_siDialogWidth+siCalcWidth) )
		{
			SetWidth( m_siDialogWidth + siCalcWidth );
			m_pStatic_itemName->SetSurfaceWidth( m_siItemWidth + siCalcWidth + ADD_TO_WIDTH );
			m_pStatic_itemName->SetWidth( m_siItemWidth + siCalcWidth );
		}
	}
	else if ( GetWidth() != m_siDialogWidth ) //기본(최초)크기라면 길이를 변경하지 않는다.
	{
		SetWidth( m_siDialogWidth );
		m_pStatic_itemName->SetSurfaceWidth( m_siItemWidth + ADD_TO_WIDTH );
		m_pStatic_itemName->SetWidth( m_siItemWidth );
	}

	Show( true );

	return; 
	
}
