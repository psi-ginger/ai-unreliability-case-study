
#include "NWarnDurabilityDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../common/Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

CNWarnDurabilityDlg::CNWarnDurabilityDlg( void )
{

}

CNWarnDurabilityDlg::~CNWarnDurabilityDlg( void )
{

}

void CNWarnDurabilityDlg::Init( )
{
		
}

void CNWarnDurabilityDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{ 
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NWarnDurabilityDlg/DLG_WarnDurability.ddf" ) );
	file.CreatePopupDialog( this, NWARNDURABILITY_DLG, TEXT( "dialog_warndurability" ), StaticCallBackDialogNWarnDurabilityDlg );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	WARNDURABILITY_DLG_IMAGESTATIC_WEAPON,	this),	WARNDURABILITY_DLG_IMAGESTATIC_WEAPON,	TEXT("imagestatic_weapon") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	WARNDURABILITY_DLG_IMAGESTATIC_SHOES,	this),	WARNDURABILITY_DLG_IMAGESTATIC_SHOES,	TEXT("imagestatic_shoes") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	WARNDURABILITY_DLG_IMAGESTATIC_BELT,	this),	WARNDURABILITY_DLG_IMAGESTATIC_BELT,	TEXT("imagestatic_belt") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	WARNDURABILITY_DLG_IMAGESTATIC_RING,	this),	WARNDURABILITY_DLG_IMAGESTATIC_RING,	TEXT("imagestatic_ring") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	WARNDURABILITY_DLG_IMAGESTATIC_NECK,	this),	WARNDURABILITY_DLG_IMAGESTATIC_NECK,	TEXT("imagestatic_neck") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	WARNDURABILITY_DLG_IMAGESTATIC_HELMET,	this),	WARNDURABILITY_DLG_IMAGESTATIC_HELMET,	TEXT("imagestatic_helmet") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	WARNDURABILITY_DLG_IMAGESTATIC_ARMOUR,	this),	WARNDURABILITY_DLG_IMAGESTATIC_ARMOUR,	TEXT("imagestatic_armour") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	WARNDURABILITY_DLG_IMAGESTATIC_CHAR,	this),	WARNDURABILITY_DLG_IMAGESTATIC_CHAR,	TEXT("imagestatic_char") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	WARNDURABILITY_DLG_IMAGESTATIC_MANTLE,	this),	WARNDURABILITY_DLG_IMAGESTATIC_MANTLE,	TEXT("imagestatic_mantle") );

	// 유저의 입력을 없애기 위해 크기를 0으로 변경
	SI16 siCalcX = 0;
	SI16 siCalcY = 0;

	if ( g_bFullScreen )
	{
		siCalcX = ((cltClient*)pclClient)->siClientScreenXsize - 80 - GetWidth() - 20;
		siCalcY = ((cltClient*)pclClient)->siClientScreenYsize - 80 - GetHeight() - 50;
	}
	else
	{
		siCalcX = ((cltClient*)pclClient)->siClientScreenXsize - 80 - GetWidth();
		siCalcY = ((cltClient*)pclClient)->siClientScreenYsize - 80 - GetHeight() - 20;
	}
		
	SetDialogPos( siCalcX, siCalcY, 0, 0 );

	// 모든 컨트롤러는 일단 꺼둔다
	SetAllControllersShowMode( false );

	// 액션 타이머 On
	SetActionTimer( 200 );
	SetTimerOnOff( true );

}

void CALLBACK CNWarnDurabilityDlg::StaticCallBackDialogNWarnDurabilityDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNWarnDurabilityDlg* pThis = (CNWarnDurabilityDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNWarnDurabilityDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNWarnDurabilityDlg::CallBackDialogNWarnDurabilityDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	return;
}

void CNWarnDurabilityDlg::Action()
{
	WarnDurabilityForAction();
}

void CNWarnDurabilityDlg::SetAllControllersShowMode( bool bShowMode )
{
	SI32 siControllersID[] = {
		WARNDURABILITY_DLG_IMAGESTATIC_MANTLE,
		WARNDURABILITY_DLG_IMAGESTATIC_CHAR,
		WARNDURABILITY_DLG_IMAGESTATIC_ARMOUR,
		WARNDURABILITY_DLG_IMAGESTATIC_HELMET,
		WARNDURABILITY_DLG_IMAGESTATIC_NECK,
		WARNDURABILITY_DLG_IMAGESTATIC_RING,
		WARNDURABILITY_DLG_IMAGESTATIC_BELT,
		WARNDURABILITY_DLG_IMAGESTATIC_SHOES,
		WARNDURABILITY_DLG_IMAGESTATIC_WEAPON,
	};
	SI32 siControllerCount = sizeof(siControllersID)/sizeof(siControllersID[0]);

	CImageStatic* pclImageStatic = NULL;

	for ( SI32 siCount=0; siCount<siControllerCount; siCount++ )
	{
		pclImageStatic = m_InterfaceFactory.GetImageStatic( siControllersID[siCount] );
		if ( NULL == pclImageStatic )
		{
			continue;
		}

		pclImageStatic->Show( bShowMode );
	}
}

void CNWarnDurabilityDlg::WarnDurabilityForAction( void )
{
	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	SI32 siCompareValue[PERSONITEM_INV0];
	ZeroMemory( siCompareValue, sizeof(siCompareValue) );

	stWarnController sWarnController[] = {
		{PERSONITEM_HELMET,		WARNDURABILITY_DLG_IMAGESTATIC_HELMET,	&siCompareValue[PERSONITEM_HELMET]}
		,{PERSONITEM_ARMOUR,	WARNDURABILITY_DLG_IMAGESTATIC_ARMOUR,	&siCompareValue[PERSONITEM_ARMOUR]}
		,{PERSONITEM_WEAPON1,	WARNDURABILITY_DLG_IMAGESTATIC_WEAPON,	&siCompareValue[PERSONITEM_WEAPON1]}
		,{PERSONITEM_NECK,		WARNDURABILITY_DLG_IMAGESTATIC_NECK,	&siCompareValue[PERSONITEM_NECK]}
		,{PERSONITEM_RING1,		WARNDURABILITY_DLG_IMAGESTATIC_RING,	&siCompareValue[PERSONITEM_RING1]}
		,{PERSONITEM_RING2,		WARNDURABILITY_DLG_IMAGESTATIC_RING,	&siCompareValue[PERSONITEM_RING1]}
		,{PERSONITEM_BELT,		WARNDURABILITY_DLG_IMAGESTATIC_BELT,	&siCompareValue[PERSONITEM_BELT]}
		,{PERSONITEM_SHOES,		WARNDURABILITY_DLG_IMAGESTATIC_SHOES,	&siCompareValue[PERSONITEM_SHOES]}
		,{PERSONITEM_WEAPON2,	WARNDURABILITY_DLG_IMAGESTATIC_WEAPON,	&siCompareValue[PERSONITEM_WEAPON1]}
		,{PERSONITEM_MANTLE,	WARNDURABILITY_DLG_IMAGESTATIC_MANTLE,	&siCompareValue[PERSONITEM_MANTLE]}
		,{PERSONITEM_HAT,		WARNDURABILITY_DLG_IMAGESTATIC_HELMET,	&siCompareValue[PERSONITEM_HELMET]}
		,{PERSONITEM_DRESS,		WARNDURABILITY_DLG_IMAGESTATIC_ARMOUR,	&siCompareValue[PERSONITEM_ARMOUR]}
	};

	SI32 siWarnControllerCount	= sizeof(sWarnController) / sizeof(sWarnController[0]);

	SI32 siForeverDurValue		= (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
	SI32 siForeverDurValue2		= (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

	SI32		siIndex			= 0;

	cltItem*	pclGetItem		= NULL;
	SI32		siGetDurability	= 0;
	SI32		siInventoryPos	= 0;
	SI32		siControllerID	= 0;

	SI32		siShowColor		= WARNDURABILITY_SHOW_COLOR_NONE;

	for( siIndex=0; siIndex<siWarnControllerCount; siIndex++ )
	{
		siInventoryPos = sWarnController[siIndex].m_siInventoryPos;

		// 무기는 현재 사용하는 무기만 내구도를 표시해준다
		if ( (PERSONITEM_WEAPON1 == siInventoryPos) || (PERSONITEM_WEAPON2 == siInventoryPos) )
		{
			if ( pclChar->ActiveWeapon != siInventoryPos )
			{
				continue;
			}
		}

		// 장비장착창에 장비를 가져오고
		pclGetItem = &pclChar->pclCI->clCharItem.clItem[siInventoryPos];
		if ( NULL == pclGetItem )
		{
			continue;
		}

		if ( 0 >= pclGetItem->siUnique )
		{
			continue;
		}

		siGetDurability = pclGetItem->GetDurability();
		// 무한내구도의 장비일경우 표시하지 않는다
		if ( (siForeverDurValue == siGetDurability) || (siForeverDurValue2 == siGetDurability) )
		{
			continue;
		}

		// 각 구간별 색값을 설정한다
		if ( 201 <= siGetDurability )
		{
			siShowColor = WARNDURABILITY_SHOW_COLOR_NONE;
		}
		else if ( (200 >= siGetDurability) && (101 <= siGetDurability) )
		{
			siShowColor = WARNDURABILITY_SHOW_COLOR_01;
		}
		else if ( (100 >= siGetDurability) && (51 <= siGetDurability) )
		{
			siShowColor = WARNDURABILITY_SHOW_COLOR_02;
		}
		else
		{
			siShowColor = WARNDURABILITY_SHOW_COLOR_03;
		}
		
		// 현재 설정되어 있는 칼라값보다 클때만 넣어준다
		if ( *(sWarnController[siIndex].m_psiCompareValue) < siShowColor )
		{
			*(sWarnController[siIndex].m_psiCompareValue) = siShowColor;
		}
	}

	CImageStatic*	pclImageStatic	= NULL;
	bool			bShowMode		= false;
	SI32			siColorValue	= 0;

	// 내구도 경고 표시해야될 장비가 하나라도 있다면 캐릭터도 같이 표시한다
	bool			bShowChar		= false;

	// 각종 상황들때문에 For문을 한번 더 돌게 한다 장비가 변경되엇을때나 내구도가 변경되었을때 표시가 잘못되는 것을 방지하기 위해서 이다
	for( siIndex=0; siIndex<siWarnControllerCount; siIndex++ )
	{
		pclImageStatic = m_InterfaceFactory.GetImageStatic( sWarnController[siIndex].m_siControllerID );
		if ( NULL == pclImageStatic )
		{
			continue;
		}

		siColorValue	= *(sWarnController[siIndex].m_psiCompareValue);
		bShowMode		= pclImageStatic->IsShow();

		if ( WARNDURABILITY_SHOW_COLOR_NONE == siColorValue )
		{
			if ( bShowMode )
			{
				pclImageStatic->Show( false );
			}
		}
		else
		{
			if ( false == bShowMode )
			{
				pclImageStatic->Show( true );
			}

			pclImageStatic->SetFontIndex( (siColorValue-1) );

			bShowChar = true;
		}
	}
	
	pclImageStatic = m_InterfaceFactory.GetImageStatic( WARNDURABILITY_DLG_IMAGESTATIC_CHAR );
	if ( pclImageStatic )
	{
		pclImageStatic->Show( bShowChar );
	}

}