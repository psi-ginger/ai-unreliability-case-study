#include "NHuntDlg.h"

#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "Msg/MsgType-Village.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NHuntChangeMapDlg::NHuntChangeMapDlg()
{
	m_lpSmallMapSurface		=	NULL;
	m_lpSmallMapWorkSurface	=	NULL;
}

NHuntChangeMapDlg::~NHuntChangeMapDlg()
{
	if(Spr.pImage) GP.FreeSpr( Spr );

	if(m_lpSmallMapWorkSurface)
		m_lpSmallMapWorkSurface->Release();

	m_lpSmallMapWorkSurface = NULL;

	SAFE_RELEASE(m_lpSmallMapSurface);
}

void NHuntChangeMapDlg::InitDlg()
{
	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);

	if( pHuntManager == NULL ) return;

	SetControlMap( IMAGESTATIC_MAP, TEXT("imagestatic_map") );
	SetControlMap( BUTTON_PREV, TEXT("button_prev") );
	SetControlMap( BUTTON_SETUP, TEXT("button_setup") );
	SetControlMap( BUTTON_NEXT, TEXT("button_next") );
	SetControlMap( EDITBOX_COST, TEXT("editbox_cost") );
	SetControlMap( EDITBOX_HUNTTYPE, TEXT("editbox_hunttype") );
	SetControlMap( EDITBOX_NEEDLEVEL, TEXT("editbox_needlevel") );

#ifdef _SAFE_MEMORY
	pHuntManager->m_siHuntMapUnique.ZeroMem();
#else
	ZeroMemory( pHuntManager->m_siHuntMapUnique, sizeof( pHuntManager->m_siHuntMapUnique ) );
#endif
	pHuntManager->m_siHuntMapIndex = 0;
	pHuntManager->m_siHuntMapNumber = 0;			


	cltMapTypeInfo *pMapTypeInfo = NULL;

	//_LEON_HUNT
	for(SI32 i=0; i<MAX_HUNT_MAP_NUMBER-1; i++)
	{
		//SI32 mapunique = pclClient->pclMapManager->GetMapUnique(HUNT_MAP_START + i);
		pMapTypeInfo = NULL;
		pMapTypeInfo = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[ i ];

		if( pMapTypeInfo == NULL ) continue;
		if( pMapTypeInfo->siAdaptHuntType != HUNTTYPE_ALL ) continue;
		SI32 mapunique = pMapTypeInfo->siMapUnique;
		pHuntManager->m_siHuntMapUnique[ pHuntManager->m_siHuntMapNumber ] = mapunique;

		pHuntManager->m_siHuntMapNumber++;
	}
	

	// 수렵장 맵 변경 막음 [영진]

	/*
	((CButton*)m_pControlMap[ BUTTON_PREV ])->Enable(false);
	((CButton*)m_pControlMap[ BUTTON_NEXT ])->Enable(false);
	((CButton*)m_pControlMap[ BUTTON_SETUP ])->Enable(false);
	*/


	//cltMapTypeInfo *pMapTypeInfo = NULL;

	//SI32 villagetype = pclClient->pclVillageManager->GetVillateType( pHuntManager->siVillageUnique );

	//for( int i = 0; i < MAX_MAPTYPE_NUMBER; ++i ) {

	//	pMapTypeInfo = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[ i ];

	//	//pMapTypeInfo->

	//	if( pMapTypeInfo == NULL ) continue;

	//	if( villagetype != 0 ) {
	//		if( villagetype % 2 == 1 ) {

	//			if( pMapTypeInfo->siAdaptVillageType == 0 ) { // 마을용이 아님

						//pHuntManager->m_siHuntMapUnique[ pHuntManager->m_siHuntMapNumber ] = pMapTypeInfo->siMapUnique;
						//pHuntManager->m_siHuntMapNumber++;
	//				}
	//		}
	//	}

	//}

	ShowHuntMap();
}

void NHuntChangeMapDlg::Action()
{
	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);
}

void NHuntChangeMapDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);
	cltClient *pclclient = (cltClient*)pclClient;

	CASE_CONTROL( BUTTON_PREV )
	{
		if( pHuntManager->m_siHuntMapIndex > 0 ) pHuntManager->m_siHuntMapIndex--;

		ShowHuntMap();
	}
	else CASE_CONTROL( BUTTON_NEXT )
	{
		if( pHuntManager->m_siHuntMapIndex < pHuntManager->m_siHuntMapNumber - 1 ) pHuntManager->m_siHuntMapIndex++;

		ShowHuntMap();

	}
	else CASE_CONTROL( BUTTON_SETUP )
	{

		SI32 mapunique = pHuntManager->m_siHuntMapUnique[ pHuntManager->m_siHuntMapIndex ];

		if( mapunique == 0 ) return;				


		if( pclClient->pclMapManager->pclMapArray[ pHuntManager->siVillageUnique ] == NULL ) return;

		if( pclClient->pclMapManager->pclMapArray[ pHuntManager->siVillageUnique ]->siMapUnique == mapunique ) {
			TCHAR* pText = GetTxtFromMgr(1534);
			SetMainExplain(pText);
			return;
		}

		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(5529);
		StringCchPrintf(buffer, 256, pText );

		pText = GetTxtFromMgr(5530);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 10 );
		BYTE TmpChar[128];
		memset( TmpChar, 0, sizeof(TmpChar));
		memcpy( TmpChar, &mapunique, sizeof(mapunique));
		SI16 Size = sizeof(mapunique);
		pclclient->SetMsgBox( &MsgBox, TmpChar, Size);

	}

}

void NHuntChangeMapDlg::ShowHuntMap()
{
	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);
	CImageStatic *pImageMap = (CImageStatic *)m_pControlMap[ IMAGESTATIC_MAP ];

	if( pHuntManager->m_siHuntMapNumber == 0 ) return;

	SI32 mapunique = pHuntManager->m_siHuntMapUnique[ pHuntManager->m_siHuntMapIndex ];
	if( mapunique == 0 ) return;

	TCHAR *pmaptype = pclClient->pclMapManager->pclMapTypeManager->GetMapTypeNameFromUnique( mapunique );
	if( pmaptype == NULL )return;

	TCHAR strFileName[ 128 ] = TEXT("");
	StringCchPrintf( strFileName, 128, TEXT("map\\%s\\%s.spr"), pmaptype, pmaptype );
	//sprintf( strFileName, TEXT("map/%s/%s.spr"), pmaptype, pmaptype );
	if( Spr.pImage )
	{
		GP.FreeSpr( Spr );
	}
	
	if( GP.LoadSprFromFile( strFileName, &Spr ) == FALSE )
		return;

	SAFE_RELEASE( m_lpSmallMapSurface );
	SAFE_RELEASE( m_lpSmallMapWorkSurface );


	//pImageMap->SetFileName( strFileName );
	if( m_lpSmallMapWorkSurface == NULL )
	{
		if( g_graphics.CreateSurface16(&m_lpSmallMapWorkSurface,  Spr.GetXSize(), Spr.GetYSize() ) == FALSE )
		{
			MessageBox(NULL, TEXT("wkcn"), TEXT("Error"), MB_OK|MB_TOPMOST);
		}
	}

	if( m_lpSmallMapSurface == NULL )
	{
		if( g_graphics.CreateSurface16(&m_lpSmallMapSurface,  Spr.GetXSize(), Spr.GetYSize() ) == FALSE )
		{
			MessageBox(NULL, TEXT("wkcn"), TEXT("Error"), MB_OK|MB_TOPMOST);
		}
	}

	// 클리핑 영역을 설정한다. 
	SI32 x1, y1, x2, y2;
	GP.GetClipArea( x1, y1, x2, y2 );

	GP.SetClipArea( 0, 0, Spr.GetXSize()-1, Spr.GetYSize()-1 );

	if(GP.LockSurface( m_lpSmallMapSurface ) == TRUE )
	{
		GP.PutSpr(&Spr, 0, 0, 0 );
		GP.UnlockSurface( m_lpSmallMapSurface );
	}

	GP.SetClipArea( x1, y1, x2, y2 );

	if( Spr.pImage )
	{
		GP.FreeSpr( Spr );
		Spr.pImage = NULL;
	}

	RECT rect;
	rect.left = 0;
	rect.right = FIXED_WIDTH;
	rect.top = 0;
	rect.bottom = FIXED_HEIGHT;

	g_graphics.ClearSurface(m_lpSmallMapWorkSurface, COLOR_BLACK );
	m_lpSmallMapWorkSurface->Blt(&rect, m_lpSmallMapSurface, NULL, DDBLT_WAIT, NULL );

	// RefreshPos();

	//Spr.PutSprT( xDlg, yDlg, 0 );

	TCHAR buffer[256 ];

	cltMapTypeInfo *pMapTypeInfo;

	for( int i = 0; i < MAX_MAPTYPE_NUMBER; ++i ) {

		pMapTypeInfo = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[ i ];

		if( pMapTypeInfo == NULL ) continue;

		if( pMapTypeInfo->siMapUnique == mapunique ) {

			TCHAR* pText = GetTxtFromMgr(611);

			g_JYLibFunction.SetNumUnit( pMapTypeInfo->siAdaptNeedVillageMoney, buffer, 256, pText);

			SetEditText( EDITBOX_COST, buffer );

			_itot( pMapTypeInfo->siAdaptNeedVillageLevel, buffer, 10 );

			SetEditText( EDITBOX_NEEDLEVEL, buffer );

			switch( pMapTypeInfo->siAdaptHuntType )
			{
			case HUNTTYPE_ALL:
				{
					TCHAR* pText = GetTxtFromMgr(5512);
					StringCchCopy( buffer, 256, pText );
				}
				break;
			}

			SetEditText( EDITBOX_HUNTTYPE, buffer );
			break;
		}
	}


}

void NHuntChangeMapDlg::VDirectDrawToBackScreen(LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY)
{
	if( ! IsShow() ) return;

	if( m_lpSmallMapWorkSurface == NULL ) return;

	CImageStatic *m_pImageStatic = (CImageStatic *)m_pControlMap[ IMAGESTATIC_MAP ];

	SI32 XPos = GetParentDialog()->GetX() + GetX() + m_pImageStatic->GetX() + ScreenRX;
	SI32 YPos = GetParentDialog()->GetY() + GetY() + m_pImageStatic->GetY() + ScreenRY;

	SI32 TrimXPos = 0;
	SI32 TrimYPos = 0;

	if ( XPos < 0 )
	{
		TrimXPos = -XPos;
		XPos = 0;
	}

	if ( YPos < 0 )
	{
		TrimYPos = -YPos;
		YPos = 0;
	}

	SI32 TrimXSize = 0;
	SI32 TrimYSize = 0;

	//if( XPos + FIXED_WIDTH > GP.GetScreenXsize() || FIXED_WIDTH > m_siCurrentMapWidth )
	//{
	//	TrimXSize = max ( XPos + FIXED_WIDTH - GP.GetScreenXsize(), FIXED_WIDTH - m_siCurrentMapWidth );
	//}

	//if( YPos + FIXED_HEIGHT > GP.GetScreenYsize() || FIXED_HEIGHT > m_siCurrentMapHeight )
	//{
	//	TrimYSize = max( YPos + FIXED_HEIGHT - GP.GetScreenYsize(), FIXED_HEIGHT - m_siCurrentMapHeight );
	//}
	
	RECT SrcRect;
	SetRect( &SrcRect, 0, 0, FIXED_WIDTH, FIXED_HEIGHT );
	
	lpBackScreen->BltFast( XPos, YPos, m_lpSmallMapWorkSurface, &SrcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT );

	SI32 clipx1, clipx2, clipy1, clipy2;
	GP.GetClipArea(clipx1, clipy1, clipx2, clipy2 );
	GP.SetClipArea( XPos, YPos, XPos + FIXED_WIDTH - 1, YPos + FIXED_HEIGHT - 1 );

	if(GP.LockSurface( lpBackScreen ) == TRUE )
	{
		NHuntDlg* parentDlg = NULL;
		SI16 xDlg = 0, yDlg = 0;
		SI16 width = 0, height = 0;
		parentDlg = (NHuntDlg*)GetParentDialog();
		if(parentDlg == NULL ) return;

		xDlg = parentDlg->GetX();
		yDlg = parentDlg->GetY();

		width = GetWidth();
		height = GetHeight();

		GP.Box(0, 0, FIXED_WIDTH, FIXED_HEIGHT, 40);

		GP.UnlockSurface( lpBackScreen );
	}

	GP.SetClipArea(clipx1, clipy1, clipx2, clipy2 );

}

void NHuntChangeMapDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);

	switch( MsgIndex )
	{
	case 10:	// 수렵장 지도 변경
		{
			if( pData == NULL || DataLen < 1 ) return;

			if ( RetValue )
			{
				SI32 mapunique = -1;
				mapunique = *((SI32*)pData);

				cltGameMsgRequest_SetVillageMap sendMsg( mapunique, cltGameMsgRequest_SetVillageMap::MAPTYPE_HUNT );

				cltMsg clMsg( GAMEMSG_REQUEST_SETVILLAGEMAP, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
			}
		}
		break;
	}		
}
