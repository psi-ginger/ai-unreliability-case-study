#include "NMiniMapDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../ninterface/npartydlg/NPartyDlg.h"

#include "../../Common/Char/CharCommon/Char-Common.h"
#include "Char\CharManager\CharManager.h"
#include "../../Client/client.h"

#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../Client/ninterface/NMainMapDlg/NMainMapDlg.h"

#include "../../CommonLogic/MsgType-System.h"

#define MIN_MAP_RATE				10
#define INCREASE_MAP_RATE			5

#include <zmouse.h>

extern cltCommonLogic* pclClient;

extern SI32		gsiResolution;

CNMiniMapDlg::CNMiniMapDlg() : CDialogController()
{
	SI32		i;

	m_pImageStatic			=	NULL;

	m_pButtonMini			=	NULL;

	m_pButtonDetail			=	NULL;

	m_pButtonMainmap		=	NULL;

	m_pButtonGoRaccoonWorld	=	NULL;

	m_siMiniMapWidth		=	206;
	m_siMiniMapHeight		=	148;

	m_siCurrentMapWidth		=	0;
	m_siCurrentMapHeight	=	0;


	m_siStandardPosX		=	0;
	m_siStandardPosY		=	0;

	memset( m_strSmallMapFileName, 0, FILE_NAME_SIZE );
	m_strSmallMapFileName[ FILE_NAME_SIZE - 1 ] = NULL;

	m_lpSmallMapSurface		=	NULL;
	m_lpSmallMapWorkSurface	=	NULL;

	m_bSet					=	false;
	m_bFollowMovePos		=	true;

	m_siDrawStartXPos		=	0;
	m_siDrawStartYPos		=	0;

	m_fScale = 1;

	m_bDraw = false;
	m_bMini = false;
	m_bEnlarge = FALSE;

	colorArray[0] = 40;
	colorArray[1] = 57;  
	colorArray[2] = 20;
	colorArray[3] = 33;
	colorArray[4] = 101;
	colorArray[5] = 120; 

	point[0].x = MAPTYPE_STRUCTURE_POINT_X_SLOT0;
	point[0].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT0;
	point[1].x = MAPTYPE_STRUCTURE_POINT_X_SLOT1;
	point[1].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT1;
	point[2].x = MAPTYPE_STRUCTURE_POINT_X_SLOT2;
	point[2].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT2;
	point[3].x = MAPTYPE_STRUCTURE_POINT_X_SLOT3;
	point[3].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT3;
	point[4].x = MAPTYPE_STRUCTURE_POINT_X_SLOT4;
	point[4].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT4;
	point[5].x = MAPTYPE_STRUCTURE_POINT_X_SLOT5;
	point[5].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT5;
	point[6].x = MAPTYPE_STRUCTURE_POINT_X_SLOT6;
	point[6].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT6;
	point[7].x = MAPTYPE_STRUCTURE_POINT_X_SLOT7;
	point[7].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT7;
	point[8].x = MAPTYPE_STRUCTURE_POINT_X_SLOT8;
	point[8].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT8;
	point[9].x = MAPTYPE_STRUCTURE_POINT_X_SLOT9;
	point[9].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT9;

	memset(m_MinimapNPC,0,sizeof(stMinimapNPC) * MAX_MINIMAP_NPC );
	
	memset(m_OldStructureInfo, 0, sizeof(SI32) * MAX_CONSTRUCTION_NUM );

	m_OldMinimapNPCTable = 0;
	m_MinimapNPCTable = 0;
	

	m_ClickStartPoint.x = 0;
	m_ClickStartPoint.y = 0;
	m_DragEndPoint.x = 0;
	m_DragEndPoint.y = 0;
	m_MovedPoint.x = 0;
	m_MovedPoint.y = 0;

	for(i=0; i<MAX_MONSTERHOME_NUMBER; i++)
		m_MonsterHome[i].Init();
	
	SetMonsterHome();
}

CNMiniMapDlg::~CNMiniMapDlg()
{
	
	if(m_MiniPlayerSpr.pImage)
	{
		GP.FreeSpr(m_MiniPlayerSpr);
	}
	
	if(m_MiniStructureSpr.pImage)
	{
		GP.FreeSpr(m_MiniStructureSpr);
	}

	if(m_MiniNPCSpr.pImage)
	{
		GP.FreeSpr(m_MiniNPCSpr);
	}
	
	if(m_ButtonSpr.pImage)
	{
		GP.FreeSpr(m_ButtonSpr);
	}

	SAFE_DELETE( m_pButtonMini );
	
	SAFE_DELETE( m_pButtonDetail );
	SAFE_DELETE( m_pButtonMainmap );
	
	SAFE_DELETE( m_pImageStatic );

	SAFE_RELEASE( m_lpSmallMapSurface );
	SAFE_RELEASE( m_lpSmallMapWorkSurface );

	SAFE_DELETE( m_pButtonGoRaccoonWorld );	
}

void CNMiniMapDlg::Create()
{
	if(IsCreate()) return;

	cltClient *pclclient = (cltClient *)pclClient;
	
	m_bMini = false;
	m_bEnlarge = FALSE;
	
	CInterfaceFile file;

	file.LoadFile("NInterface/Data/NMinimapDlg/DLG_MiniMap.ddf");
	file.CreatePopupDialog( this, NMINIMAP_DLG, "dialog_minimap", StaticCallBackDialogNMiniMap, true );
	
	NEWCBUTTON( m_pButtonMini);
	file.CreateControl(m_pButtonMini,NMINIMAP_DIALOG_BUTTON_MINI, "button_mini" );
	m_pButtonMini->Show(FALSE);

	NEWCBUTTON( m_pButtonDetail);	
	file.CreateControl(m_pButtonDetail,NMINIMAP_DIALOG_BUTTON_DETAIL,"button_detail");

	NEWCBUTTON( m_pButtonMainmap);
	file.CreateControl(m_pButtonMainmap,NMINIMAP_DIALOG_BUTTON_MAINMAP, "imagestatic_mainmap");

	//라쿤월드 이동 버튼 
	NEWCBUTTON( m_pButtonGoRaccoonWorld);// 라쿤월드 이동 버튼 
	file.CreateControl(m_pButtonGoRaccoonWorld,NMINIMAP_DIALOG_BUTTON_RACCOON_GO_WORLD,"button_raccoon");

	NEWCIMAGESTATIC( m_pImageStatic );
	file.CreateControl( m_pImageStatic, NMINIMAP_DIALOG_IMAGESTATIC, "imagestatic_NONAME1" );

	if(pclClient->IsCountrySwitch( Switch_Go_RaccoonWorld ))
	{
		if( pclClient->IsWhereServiceArea( ConstServiceArea_Korea ) )
		{
			cltCharClient *pclCharMe = pclClient->pclCM->GetCharClient(1)	;
			if(pclCharMe != NULL )
			{
				if( pclCharMe->pclCI->clBI.m_uiRealAge	>=	18 )	// 한국은 18세 이상만 버튼이 보인다 
				{
					m_pButtonGoRaccoonWorld->Show( TRUE );
					m_pButtonGoRaccoonWorld->Enable(TRUE)	;
				}
				else
				{
					m_pButtonGoRaccoonWorld->Show( FALSE );
					m_pButtonGoRaccoonWorld->Enable(FALSE)	;
				}
			}
		}
		else
		{
			m_pButtonGoRaccoonWorld->Show( TRUE );
			m_pButtonGoRaccoonWorld->Enable(TRUE)	;
		}
	}

	m_siMiniMapWidth		=	m_pImageStatic->GetWidth();
	m_siMiniMapHeight		=	m_pImageStatic->GetHeight();


	GP.LoadSprFromFile(TEXT("ninterface/image/IMAGE_20X22_00_000.SPR"), &m_MiniPlayerSpr);


	GP.LoadSprFromFile(TEXT("ninterface/Image/IMAGE_46X43_00_000.SPR"),&m_MiniStructureSpr);

	
	GP.LoadSprFromFile(TEXT("ninterface/image/IMAGE_20X24_01_000.SPR"), &m_MiniNPCSpr);

	GP.LoadSprFromFile(TEXT("ninterface/button/BUTTON_24X18_00_000.SPR"), &m_ButtonSpr);

	if(GetX() == DEFAULT_DIALOGPOSX && GetY() == DEFAULT_DIALOGPOSY)  // 기본위치일경우에 우측 상단으로 옮겨준다
		MovePos( pclclient->GetScreenXSize() - GetWidth(), 40 );


	SetTimerOnOff( true );
	SetActionTimer(2000);

	Hide();
	return;
}

void CNMiniMapDlg::Show()
{

	CControllerMgr::Show(TRUE);

}

void CNMiniMapDlg::Hide()
{
	
	CControllerMgr::Show(FALSE);

}

void CNMiniMapDlg::Minimi(BOOL bMini)
{

	cltClient *pclclient = (cltClient*)pclClient;
	pclclient->bSmallMapSwitch = !bMini;

	if( bMini )
	{	
		m_bDraw = FALSE;

		SetMoveable(FALSE);
		SetDialogPos( pclclient->GetScreenXSize() - 44, 40, 44, 40 );
		SetFileName(NULL);
		SetTitle(NULL);
		
		m_pImageStatic->Show(FALSE);
        m_pButtonMini->Show(TRUE);
		m_pCloseButton->Show(FALSE);
		m_pButtonDetail->Show(FALSE);
		m_pButtonMainmap->Show(FALSE);
		m_pButtonGoRaccoonWorld->Show( FALSE );
	}
	else
	{
		m_bDraw = TRUE;

		SetMoveable(TRUE);
		ReSizeDialog( pclclient->GetScreenXSize() - 212, 40, 212, 169 );

		//[진성] 미니맵 크기 변경시 창의 색변경이 이상해서 수정. => 2008-5-7
		//SetFileName("ninterface/interface/interface_212x169_00_000.spr");
		SetFileName("ninterface/interface/INTERFACE_20X20_00_109.SPR");
		
		m_pImageStatic->Show(TRUE);
		m_pButtonMini->Show(FALSE);		
		m_pCloseButton->Show(TRUE);
		m_pButtonDetail->Show(TRUE);
		m_pButtonMainmap->Show(TRUE);

		if(pclClient->IsCountrySwitch( Switch_Go_RaccoonWorld ))
		{
			if( pclClient->IsWhereServiceArea( ConstServiceArea_Korea ) )
			{
				cltCharClient *pclCharMe = pclClient->pclCM->GetCharClient(1)	;
				if(pclCharMe != NULL )
				{
					if( pclCharMe->pclCI->clBI.m_uiRealAge	>=	18 )	// 한국은 18세 이상만 버튼이 보인다 
					{
						m_pButtonGoRaccoonWorld->Show( TRUE );
						m_pButtonGoRaccoonWorld->Enable(TRUE)	;
					}
					else
					{
						m_pButtonGoRaccoonWorld->Show( FALSE );
						m_pButtonGoRaccoonWorld->Enable(FALSE)	;
					}
				}
			}
			else
			{
				m_pButtonGoRaccoonWorld->Show( TRUE );
				m_pButtonGoRaccoonWorld->Enable(TRUE)	;
			}
		}



		m_pImageStatic->SetWidth(208);
		m_pImageStatic->SetHeight(150);
		m_siMiniMapWidth = m_pImageStatic->GetWidth();
		m_siMiniMapHeight = m_pImageStatic->GetHeight();


		m_pButtonDetail->MoveControl( 3, GetHeight() - (m_pButtonDetail->GetHeight() + 3)  );
		m_pButtonMainmap->MoveControl( 32, GetHeight() - (m_pButtonMainmap->GetHeight() + 3) );	// 위치
		if(pclClient->IsCountrySwitch( Switch_Go_RaccoonWorld ))
		{
			if(pclClient->IsCountrySwitch( Switch_Go_RaccoonWorld ))
			{
				if( pclClient->IsWhereServiceArea( ConstServiceArea_Korea ) )
				{
					cltCharClient *pclCharMe = pclClient->pclCM->GetCharClient(1)	;
					if(pclCharMe	!= NULL )
					{
						if( pclCharMe->pclCI->clBI.m_uiRealAge	>=	18 )	// 한국은 18세 이상만 버튼이 보인다 
						{
							m_pButtonGoRaccoonWorld->MoveControl( 61, GetHeight() - (m_pButtonGoRaccoonWorld->GetHeight() + 3)  );
						}
					}
				}
				else
				{
					m_pButtonGoRaccoonWorld->MoveControl( 61, GetHeight() - (m_pButtonGoRaccoonWorld->GetHeight() + 3)  );
				}
			}
		}
		
	}
	
	m_bEnlarge = FALSE;
	m_bMini = bMini;
}

void CNMiniMapDlg::Reduce()
{
	if( m_bEnlarge)
	{
		Minimi(FALSE);		
	}
	else
	{
		Minimi(TRUE);
	}

}

void CNMiniMapDlg::EnLarge()
{
	cltClient *pclclient = (cltClient*)pclClient;

	m_bDraw = TRUE;
	m_bEnlarge = TRUE;

 	SetMoveable(TRUE);
	ReSizeDialog( pclclient->GetScreenXSize() - 636, 40, 636, 507 );

	SetFileName("ninterface/interface/INTERFACE_20X20_00_109.SPR");

 	m_pImageStatic->Show(TRUE);	
	m_pButtonMini->Show(FALSE);	
	m_pCloseButton->Show(TRUE); 
	m_pButtonDetail->Show(TRUE); 
	m_pButtonMainmap->Show(TRUE);
	if(pclClient->IsCountrySwitch( Switch_Go_RaccoonWorld ))
	{
		if( pclClient->IsWhereServiceArea( ConstServiceArea_Korea ) )
		{
			cltCharClient *pclCharMe = pclClient->pclCM->GetCharClient(1)	;
			if(pclCharMe != NULL )
			{
				if( pclCharMe->pclCI->clBI.m_uiRealAge	>=	18 )	// 한국은 18세 이상만 버튼이 보인다 
				{
					m_pButtonGoRaccoonWorld->Show( TRUE );
					m_pButtonGoRaccoonWorld->Enable(TRUE)	;
				}
				else
				{
					m_pButtonGoRaccoonWorld->Show( FALSE );
					m_pButtonGoRaccoonWorld->Enable(FALSE)	;
				}
			}
		}
		else
		{
			m_pButtonGoRaccoonWorld->Show( TRUE );
			m_pButtonGoRaccoonWorld->Enable(TRUE)	;
		}
	}




	m_pImageStatic->SetWidth(632);
	m_pImageStatic->SetHeight(487);
	m_siMiniMapWidth = m_pImageStatic->GetWidth();
	m_siMiniMapHeight = m_pImageStatic->GetHeight();


	m_pButtonDetail->MoveControl(   3, GetHeight() - (m_pButtonDetail->GetHeight() + 3)  );
	m_pButtonMainmap->MoveControl(	32, GetHeight() - (m_pButtonMainmap->GetHeight() + 3) );
	if(pclClient->IsCountrySwitch( Switch_Go_RaccoonWorld ))
	{
		if( pclClient->IsWhereServiceArea( ConstServiceArea_Korea ) )
		{
			cltCharClient *pclCharMe = pclClient->pclCM->GetCharClient(1)	;
			if(pclCharMe	!= NULL )
			{
				if( pclCharMe->pclCI->clBI.m_uiRealAge	>=	18 )	// 한국은 18세 이상만 버튼이 보인다 
				{
					m_pButtonGoRaccoonWorld->MoveControl( 61, GetHeight() - (m_pButtonGoRaccoonWorld->GetHeight() + 3)  );
				}
			}
		}
		else
		{
			m_pButtonGoRaccoonWorld->MoveControl( 61, GetHeight() - (m_pButtonGoRaccoonWorld->GetHeight() + 3)  );
		}
	}
}

void CALLBACK CNMiniMapDlg::StaticCallBackDialogNMiniMap( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNMiniMapDlg *pThis = (CNMiniMapDlg*)pControllerMgr;
	pThis->CallBackDialogNMiniMap( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CNMiniMapDlg::CallBackDialogNMiniMap( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Minimi(TRUE);
		}
		break;
	case NMINIMAP_DIALOG_BUTTON_MINI:
		{
			
            Minimi(FALSE);
		}
		break;
	case NMINIMAP_DIALOG_BUTTON_DETAIL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_bEnlarge == FALSE)
					{
						EnLarge();
					}
					else
					{
						Reduce();
					}
				
				}
				break;
			}
		}
		break;
	case NMINIMAP_DIALOG_BUTTON_MAINMAP:	// 메인맵 버튼
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltClient* pClient = (cltClient*)pclClient;

					if( pClient->m_pDialog[ NMAINMAP_DLG ] )
					{
						if ( true == ((CMainMapDlg*)pClient->m_pDialog[ NMAINMAP_DLG ])->IsShow() )
						{
							//((CMainMapDlg*)pclclient->m_pDialog[ NMAINMAP_DLG ])->Show( false );
							((CMainMapDlg*)pClient->m_pDialog[ NMAINMAP_DLG ])->DeleteDialog();
						}
						else
						{
							((CMainMapDlg*)pClient->m_pDialog[ NMAINMAP_DLG ])->Show( true );
						}
					}
					else
					{
						pClient->CreateInterface( NMAINMAP_DLG );
						((CMainMapDlg*)pClient->m_pDialog[ NMAINMAP_DLG ])->Show( true );
					}
				}
				break;
			}
		}
		break;

	case NMINIMAP_DIALOG_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					/*
					cltMapClient *pMap = (cltMapClient * )pclClient->pclMapManager->pclCurrentMap;
					if ( pMap == NULL )
					{
						return;
					}

					SI16 PreSmallMapRete = m_siCurrentSmallMapRate;

					SI32 ZDelta = 1;
					m_siCurrentSmallMapRate = max( m_siOrgSmallMapRate, m_siCurrentSmallMapRate + ZDelta * INCREASE_MAP_RATE );

					if ( PreSmallMapRete == m_siCurrentSmallMapRate )
					{
						return;
					}

					SI32 RealMapXSize = pMap->MH.MapXsize * MAP_TILE_XSIZE_HALF + pMap->MH.MapYsize * MAP_TILE_XSIZE_HALF;
					SI32 RealMapYSize = pMap->MH.MapXsize * MAP_TILE_YSIZE_HALF + pMap->MH.MapYsize * MAP_TILE_YSIZE_HALF;

					if (  m_siMiniMapWidth > RealMapXSize/m_siCurrentSmallMapRate || m_siMiniMapHeight > RealMapYSize/m_siCurrentSmallMapRate )
					{
						m_siCurrentSmallMapRate = PreSmallMapRete;
						return;
					}

					m_siCurrentMapWidth = RealMapXSize/m_siCurrentSmallMapRate;
					m_siCurrentMapHeight = RealMapYSize/m_siCurrentSmallMapRate;

					ReadyRateMap( pMap );
					*/
				}
				break;
			}
		}
		break;

	case NMINIMAP_DIALOG_BUTTON3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
				}
				break;
			}
		}
		break;

	case NMINIMAP_DIALOG_BUTTON4:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
				}
				break;
			}
		}
		break;

	case NMINIMAP_DIALOG_IMAGESTATIC:
		{
			switch ( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					//드래그 시작점을 기억
					m_pImageStatic->GetImageStaticPos( m_ClickStartPoint );		
				}
				break;
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{					
					// 드래그 하기전의 원래의 위치로 미니맵을 돌려놓음
					m_MovedPoint.x = 0;
					m_MovedPoint.y = 0;
				}
				break;
			case EVENT_IMAGESTATIC_LBUTTONDRAG:
			//case EVENT_IMAGESTATIC_MOUSEMOVE:
				{	
                    m_pImageStatic->GetImageStaticPos( m_DragEndPoint );
					
					if( IsInArea(m_DragEndPoint.x + GetX(),m_DragEndPoint.y + GetY() ) == false) break;

					m_MovedPoint.x = m_ClickStartPoint.x - m_DragEndPoint.x;
					m_MovedPoint.y = m_ClickStartPoint.y - m_DragEndPoint.y;
				}
				break;

			case EVENT_IMAGESTATIC_MOUSEWHEEL:
				{
					cltMapClient *pMap = (cltMapClient * )pclClient->pclMapManager->pclCurrentMap;
					if ( pMap == NULL )
					{
						return;
					}

					if( m_MovedPoint.x || m_MovedPoint.y )
					{
						return;
					}

					SI32 ZDelta = m_pImageStatic->GetZDelta();

					if( ZDelta > 0 )
						m_fScale = m_fScale * 0.9f;
					else
						m_fScale = m_fScale * 1.1f;

					if( m_fScale > 1 ) m_fScale = 1;
					if( m_fScale < 0.1 ) m_fScale = 0.1f;
	
					m_siCurrentMapWidth = m_siSurfaceWidth * m_fScale;
					m_siCurrentMapHeight = m_siSurfaceHeight * m_fScale;
					
					ReadyRateMap( pMap );
				}
				break;
			}
		}
		break;
	case NMINIMAP_DIALOG_BUTTON_RACCOON_GO_WORLD:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(pclClient->IsCountrySwitch( Switch_Go_RaccoonWorld ))
					{
						if( pclClient->IsWhereServiceArea( ConstServiceArea_Korea ) )
						{
							cltCharClient *pclCharMe = pclClient->pclCM->GetCharClient(1)	;
							if(pclCharMe != NULL )
							{
								if( pclCharMe->pclCI->clBI.m_uiRealAge	>=	18 )	// 한국은 18세 이상만 버튼이 보인다 
								{
									// 18세 이상 성인 유저만 라쿤월드로 이동
									cltClient *pclclient = ( cltClient * )pclClient;

									TCHAR* pTitle = GetTxtFromMgr(40851) ;
									TCHAR* pText = GetTxtFromMgr(40852) ;

									stMsgBox MsgBox;
									MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
									pclclient->SetMsgBox( &MsgBox, NULL, 0 );
								}
							}
						}
						else
						{
							// 18세 이상 성인 유저만 라쿤월드로 이동
							cltClient *pclclient = ( cltClient * )pclClient;

							TCHAR* pTitle = GetTxtFromMgr(40851) ;
							TCHAR* pText = GetTxtFromMgr(40852) ;

							stMsgBox MsgBox;
							MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
							pclclient->SetMsgBox( &MsgBox, NULL, 0 );

						}
					}
				}
				break;

			}
		}

	}
	return;
}

void CNMiniMapDlg::Set( TCHAR *SmallMapFileName, cltMapClient *pclmap, SI32 MapIndex )
{	

	m_fScale = 1;

	m_bSet = false;

	if ( pclmap == NULL )
	{
		return;
	}

	if ( pclClient->GameMode != GAMEMODE_CLIENT )
		return ; 
	if ( SmallMapFileName == NULL || _tcslen( SmallMapFileName ) < 1 )
	{
		StringCchCopy( m_strSmallMapFileName, FILE_NAME_SIZE, TEXT("") );
	}
	else
	{
		_tcsncpy( m_strSmallMapFileName, SmallMapFileName, FILE_NAME_SIZE - 1 );
		m_strSmallMapFileName[ FILE_NAME_SIZE - 1 ] = NULL;		
	}

	SAFE_RELEASE( m_lpSmallMapSurface );
	SAFE_RELEASE( m_lpSmallMapWorkSurface );


	TSpr Spr;
	if( GP.LoadSprFromFile( SmallMapFileName, &Spr ) == FALSE )
	{
		return;
	}

	m_siSurfaceWidth = Spr.GetXSize();
	m_siSurfaceHeight = Spr.GetYSize();

	if( g_graphics.CreateSurface16( &m_lpSmallMapSurface, Spr.GetXSize(), Spr.GetYSize() ) == FALSE )
	{
		MessageBox(NULL, TEXT("4357"), TEXT("Error"), MB_OK|MB_TOPMOST);
	}

	if( g_graphics.CreateSurface16( &m_lpSmallMapWorkSurface, Spr.GetXSize(), Spr.GetYSize() ) == FALSE )
	{
		MessageBox(NULL, TEXT("43572"), TEXT("Error"), MB_OK|MB_TOPMOST);
	}

	// 클리핑 영역을 설정한다. 
	SI32 x1, y1, x2, y2;
	GP.GetClipArea( x1, y1, x2, y2 );

	GP.SetClipArea( 0, 0, Spr.GetXSize()-1, Spr.GetYSize()-1 );

	if(GP.LockSurface( m_lpSmallMapSurface ) == TRUE )
	{
		GP.PutSpr( &Spr, 0, 0, 0 );
		GP.UnlockSurface( m_lpSmallMapSurface );
	}
	// 클리핑 영역을 복구한다.
	GP.SetClipArea( x1, y1, x2, y2 );

	if( Spr.pImage )
	{
		GP.FreeSpr( Spr );
		Spr.pImage = NULL;
	}

	m_siRealMapXSize = pclmap->MH.MapXsize * MAP_TILE_XSIZE_HALF + pclmap->MH.MapYsize * MAP_TILE_XSIZE_HALF;
	m_siRealMapYSize = pclmap->MH.MapXsize * MAP_TILE_YSIZE_HALF + pclmap->MH.MapYsize * MAP_TILE_YSIZE_HALF;

	m_siCurrentMapWidth = m_siSurfaceWidth * m_fScale;
	m_siCurrentMapHeight = m_siSurfaceHeight * m_fScale;

	ReadyRateMap( pclmap );

	m_bSet = true;

	return;
}

// 원본 맵을 비율에 맞게 축소해서 작업용 서피스에 옮겨 그린다. 
void CNMiniMapDlg::ReadyRateMap( cltMapClient *pMap )
{			
	if ( pMap == NULL )
	{
		return;
	}

	if ( m_siCurrentMapWidth < 1 || m_siCurrentMapHeight < 1 )
	{
		return;
	}

	//-----------------------------------------------------------
	// 원본 지도를 확대/축소해서 pSmallMapWorkSurface에 그린다. 
	//-----------------------------------------------------------
	// 목적지 영역. 
	RECT rect;
	rect.left	= 0;
	rect.right	= m_siCurrentMapWidth;
	rect.top	= 0;
	rect.bottom	= m_siCurrentMapHeight;

	g_graphics.ClearSurface( m_lpSmallMapWorkSurface, COLOR_BLACK );

	if(m_lpSmallMapSurface != NULL)		m_lpSmallMapWorkSurface->Blt( &rect, m_lpSmallMapSurface, NULL, DDBLT_WAIT, NULL );

	SI32 clipx1, clipx2, clipy1, clipy2;
	GP.GetClipArea( clipx1, clipy1, clipx2, clipy2 );
	GP.SetClipArea( 0, 0, m_siCurrentMapWidth - 1, m_siCurrentMapHeight - 1 );

	SI32 index;
	//-----------------------------------------
	// 마을의 위치를 찍어준다. 
	//-----------------------------------------
  	if(pclClient->pclVillageManager && pclClient->pclMapManager->CurrentMapIndex == 0 && m_fScale > 0.3f  )		// 메인 지도여야 한다. 
	{
		for(index = 0;index < MAX_VILLAGE_NUMBER;index++)
		{
			if(pclClient->pclVillageManager->pclVillageInfo[index] == NULL)continue;

			SI32 x = pclClient->pclVillageManager->pclVillageInfo[index]->PosX;
			SI32 y = pclClient->pclVillageManager->pclVillageInfo[index]->PosY;

			TCHAR* pname = pclClient->pclVillageManager->pclVillageInfo[index]->szName;

			// 적에게 점령당한 상태라면, 붉게 표시. 
			if(pclClient->pclVillageManager->pclVillageInfo[index]->bEnemyCaptureSwitch == true)
			{
				DrawSMapInfo( m_lpSmallMapWorkSurface, 0, 0, x, y, pname, RGB(240, 0, 0));
			}
			else
			{
				DrawSMapInfo( m_lpSmallMapWorkSurface, 0, 0, x, y, pname);
			}
		}
	}

	//-----------------------------------------
	// 마을내 건물의 위치를 찍어준다. 
	//-----------------------------------------
		
	if(pclClient->pclVillageManager && pclClient->pclMapManager->CurrentMapIndex != 0 
		&& pclClient->pclCM->CR[1]->GetCurrentVillageUnique() > 0
		&& m_fScale > 0.5f  )		// 마을 이어야 한다. 
	{
 		cltVillageInfo* CurrentVillage = pclClient->pclVillageManager->pclVillageInfo[ pclClient->pclCM->CR[1]->GetCurrentVillageUnique() ];
		if(CurrentVillage != NULL)
		{
			SI32 strucureinfo = 0;
			//TCHAR szBuffer[256] = {'\0',};			          

			for( SI16 j = 0; j < MAX_CONSTRUCTION_NUM; j++)
			{
				strucureinfo =	CurrentVillage->clVillageVary.clStructureInfo.Get(j);
				
				m_OldStructureInfo[j] =  strucureinfo;

 				if( strucureinfo == 0 ) continue;

			//	StringCchPrintf( szBuffer,256,TEXT("- %d -"),strucureinfo);				

                DrawSMapStructureInfo( m_lpSmallMapWorkSurface, 0, 0, point[j].x, point[j].y, strucureinfo);
			}
		}			
	}
	

	//-----------------------------------------
	// 퀘스트 관련 NPC 의 위치를 찍어준다. 
	//-----------------------------------------
	
	if( m_fScale > 0.3f ){
		for( SI32 i = 0; i < MAX_MINIMAP_NPC; i++ )
		{
			if( m_MinimapNPC[i].siKind > 0 )
			{
				if(m_MinimapNPC[i].clPosMinimap.GetMapIndex() == pclClient->pclMapManager->CurrentMapIndex)
				{
					DrawSMapNPCInfo( m_lpSmallMapWorkSurface, 0, 0,m_MinimapNPC[i].clPosMinimap.GetX() , m_MinimapNPC[i].clPosMinimap.GetY(), m_MinimapNPC[i].siKind  );
				}
			}
		}  
	}


	//-----------------------------------------
	// 몬스터 서식지 표시
	//-----------------------------------------
	for( SI32 i = 0; i < MAX_MONSTERHOME_NUMBER; i++ )
	{
		if( pclClient->pclMapManager->CurrentMapIndex == MAPINDEX_MAINFIELD )
		{
			if( m_MonsterHome[i].IsValid() )
				DrawSMapInfo( m_lpSmallMapWorkSurface, 0, 0, m_MonsterHome[i].siXpos, m_MonsterHome[i].siYpos, m_MonsterHome[i].szHomeName );
		}
	}  

	//----------------------------------
	// 게이트를 표시한다. 
	//----------------------------------
	cltGate clGate;
	index = 0;
	SI32 mapindex = pclClient->pclMapManager->CurrentMapIndex;
	while(pclClient->pclMapManager->pclGateManager->FindGate(index, &clGate) == true && m_fScale > 0.15f )
	{
		index++;		
		if(clGate.SrcMapIndex == mapindex /*&& !(clGate.siUnique >= 400 && clGate.siUnique <= 414 )*/)
		{
			SI32 x = clGate.SrcX;
			SI32 y = clGate.SrcY;
			TCHAR* pname = clGate.szName;

			DrawSMapInfo( m_lpSmallMapWorkSurface, 0, 0, x, y, pname );
		}
	}
	
	
	GP.SetClipArea( clipx1, clipy1, clipx2, clipy2 );
	m_bDraw = true;
	return;
}

void CNMiniMapDlg::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
{
	if ( ! IsShow() )
	{
		return;
	}
	if ( m_bMini )
	{	
		return;
	}
	SI32 XPos = GetX() + m_pImageStatic->GetX() + ScreenRX;
	SI32 YPos = GetY() + m_pImageStatic->GetY() + ScreenRY;

	GP.FillBox( XPos, YPos, XPos + m_siMiniMapWidth - 1, YPos + m_siMiniMapHeight - 1, 0, false );

	cltMapClient *pMap = (cltMapClient * )pclClient->pclMapManager->pclCurrentMap;
	if ( pMap == NULL )
	{
		return;
	}

	if ( m_lpSmallMapWorkSurface == NULL )
	{/*
		pmaptype = pclMapTypeManager->GetMapTypeNameFromUnique(pclClient->pclMapManager->pclCurrentMap->siMapUnique);
		if(pmaptype == NULL)return false;
	//	Set(pclClient->pclMapManager->GetMapName()
		TCHAR mapfilename[FILE_NAME_SIZE];
		StringCchPrintf(mapfilename, FILE_NAME_SIZE, TEXT("map\\%s\\%s.Spr"), pmaptype, pmaptype);
		
		Set( mapfilename, this, siMapIndex )*/
		return;
	}

	if ( ! m_bSet )
	{
		return;
	}
	{

		if( m_bDraw )
		{
			ReadyRateMap( pMap ); 
		}

		SI32 dotx, doty;
 		if( GetSmallMapCordFromTileXY( pMap->VirtualMapX, pMap->VirtualMapY, &dotx, &doty ) == TRUE )
		{


			SI32 width, height;
			width = m_siMiniMapWidth;
			height = m_siMiniMapHeight;

			RECT SrcRect, DestRect;
  			SI32 MovePointX = m_MovedPoint.x;
			SI32 MovePointY = m_MovedPoint.y;

  			dotx -= ( m_siMiniMapWidth / 2 );
			doty -= ( m_siMiniMapHeight / 2 );
   			if( dotx < 0 ) { width += (dotx + MovePointX) ; XPos -= (dotx + MovePointX); dotx = 0; MovePointX = 0; };
			if( doty < 0 ) { height += (doty + MovePointY) ; YPos -= (doty + MovePointY) ; doty = 0; MovePointY = 0; };
			if( dotx + width + MovePointX > m_siCurrentMapWidth ) { width = m_siCurrentMapWidth - (dotx + MovePointX); };
			if( doty + height + MovePointY > m_siCurrentMapHeight ) { height = m_siCurrentMapHeight - (doty+ MovePointY); };
	
   			SetRect( &SrcRect, dotx + MovePointX, doty + MovePointY, dotx + width - 1 + MovePointX, doty + height - 1  + MovePointY);
  			SetRect( &DestRect, XPos , YPos, XPos + width - 1 , YPos + height - 1);
			// 클리핑
			


			if( SrcRect.left < 0 )
			{
				DestRect.left -= SrcRect.left;
				DestRect.right -= SrcRect.left;
				SrcRect.right -= SrcRect.left;
				SrcRect.left = 0;
			}

			if( DestRect.left < GetX() + m_pImageStatic->GetX() + ScreenRX )
			{
				SrcRect.left -=  (DestRect.left - (GetX() + m_pImageStatic->GetX() + ScreenRX));
				DestRect.left = (GetX() + m_pImageStatic->GetX() + ScreenRX);
			}
 			if( DestRect.left < 0  )
			{
				SrcRect.left -= DestRect.left;
				DestRect.left = 0;
			}
			if( DestRect.top < GetY() + m_pImageStatic->GetY() + ScreenRX )		
			{
				SrcRect.top -= (DestRect.top - (GetY() + m_pImageStatic->GetY() + ScreenRY));
				DestRect.top = (GetY() + m_pImageStatic->GetY() + ScreenRY);
			}
			if( DestRect.top < 0)
			{
				SrcRect.top -= DestRect.top;
				DestRect.top = 0;

				if(SrcRect.top < 0)
				{
					DestRect.top -= SrcRect.top;
					SrcRect.top = 0;
				}

			}
			if( DestRect.right > GetX() + m_pImageStatic->GetX() + ScreenRX + m_pImageStatic->GetWidth())
			{
				SrcRect.right -=  (DestRect.right - (GetX() + m_pImageStatic->GetX() + ScreenRX+ m_pImageStatic->GetWidth()));
				DestRect.right = GetX() + m_pImageStatic->GetX() + ScreenRX + m_pImageStatic->GetWidth();

			}
			if( DestRect.right > GP.GetScreenXsize())
			{
				SrcRect.right -= (DestRect.right - GP.GetScreenXsize());
				DestRect.right = GP.GetScreenXsize();
			}

			if(DestRect.bottom > GetY() + m_pImageStatic->GetY() + ScreenRY + m_pImageStatic->GetHeight())
			{
				SrcRect.bottom -=  (DestRect.bottom - (GetY() + m_pImageStatic->GetY() + ScreenRY + m_pImageStatic->GetHeight()));
				DestRect.bottom  = GetY() + m_pImageStatic->GetY() + ScreenRY + m_pImageStatic->GetHeight();
			}
			if( DestRect.bottom > GP.GetScreenYsize())
			{
				SrcRect.bottom -= (DestRect.bottom - GP.GetScreenYsize());
				DestRect.bottom = GP.GetScreenYsize();
			}

			if( SrcRect.top < 0 )
			{
			//	DestRect.bottom -= (SrcRect.top - 1);
				DestRect.top -= (SrcRect.top );
			//	SrcRect.bottom -= (SrcRect.top );
				SrcRect.top = 0;
			}

			////////클리핑 끝////

  			HRESULT hr = lpBackScreen->Blt( &DestRect, m_lpSmallMapWorkSurface, &SrcRect, DDBLT_WAIT, NULL );
			//lpBackScreen->Blt( &DestRect, m_lpSmallMapWorkSurface, NULL, DDBLT_WAIT, NULL );
			if( hr != DD_OK)
			{
				hr = hr;
			}

			//----------------------------------
			// 파티원의 위치를 표시한다.
			//----------------------------------
			
			// 맵을 새로 그려주었다면 자신, 파티원의 위치는 그리지 않고 끝낸다. 
			// 맵에 자신의 초기 위치가 계속 남아있던 문제 때문에 [영진]
			if(m_bDraw == TRUE)
			{
				m_bDraw = FALSE;
				return;
			}

 			cltClient* pclclient = (cltClient*)pclClient;

				//위치알림막음
			if(pclclient->m_pDialog[NPARTY_DLG] != NULL &&   ((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->IsPartyStatus()  )
			{				
				

 				for(SI16 i = 0; i < MAX_PARTY_USER_NUM; i++)
				{
					if(((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->m_stClientPartyUser[i].bPartyStatus == false) 
						continue;

					// 나 자신이면 파티원의 위치를 그려주지 않는다.
					if(((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->m_stClientPartyUser[i].siCharUnique 
						== pclclient->pclCM->CR[1]->GetCharUnique())
					{
						continue;
					}	

					// 위치를 얻어온다.
					cltCharPos * pclCharPos = ((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->GetPartyUserPos( i );
					SI32 siCharUnique = 	((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->m_stClientPartyUser[i].siCharUnique;
					if(pclCharPos == NULL) 
						continue;
										
					// 위치값이 초기상태이면 무효
					if(pclCharPos->GetMapIndex() == 0 && pclCharPos->GetX() == 0 && pclCharPos->GetY() == 0)
						continue;
					// 내 맵인덱스가 야외지역 이거나 파티원이 나와 다른 인덱스를 가지고있다면 무효
				//	if(pclClient->pclMapManager && pclClient->pclMapManager->CurrentMapIndex == 0)
					//	continue;
					if(pclClient->pclMapManager->CurrentMapIndex != pclCharPos->GetMapIndex())
						continue;
					SI32 PartyUserdotX=0,PartyUserdotY=0, PartyUserXpos = 0, PartyUserYpos = 0, MydotX=0, MydotY=0;

					// 지도위에 점찍기
					
					SI32 id = pclclient->pclCM->GetIDFromCharUnique( siCharUnique );
					if( id )
					{
						GetSmallMapCordFromTileXY( pclclient->pclCM->CR[id]->GetX(), pclclient->pclCM->CR[id]->GetY(), &PartyUserdotX , &PartyUserdotY);
					}
					else
					{
						// 서버로부터 받은 이동좌표를 그대로 출력한다. 
						GetSmallMapCordFromTileXY( pclCharPos->GetX(), pclCharPos->GetY(), &PartyUserdotX , &PartyUserdotY);

					}
					// 자신의 미니맵 위치를 구함
	 				GetSmallMapCordFromTileXY( pMap->VirtualMapX , pMap->VirtualMapY , &MydotX , &MydotY);

					// 그려줄 위치가 미니맵 다이얼로그 안에 들어 간다면 출력
					if( abs(PartyUserdotX - MydotX) + 5  < ( m_pImageStatic->GetWidth() / 2 ) 
    			 			&& abs(PartyUserdotY - MydotY) +5 < ( m_pImageStatic->GetHeight() / 2 ) )    
					{
						PartyUserXpos = GetX() + m_pImageStatic->GetX() + ScreenRX + ( m_siMiniMapWidth / 2 ) + PartyUserdotX - MydotX - m_MovedPoint.x;
						PartyUserYpos = GetY() + m_pImageStatic->GetY() + ScreenRY  + ( m_siMiniMapHeight / 2 ) + PartyUserdotY - MydotY - m_MovedPoint.y;

						GP.Box( PartyUserXpos - 2,   
							PartyUserYpos - 2,
							PartyUserXpos + 2,
							PartyUserYpos + 2, colorArray[i] );
 						GP.Box( PartyUserXpos - 3,   
							PartyUserYpos - 3,
							PartyUserXpos + 3,
 							PartyUserYpos + 3, colorArray[i] ); // 인덱스값에 따라 다른색으로 파티원을 찍어준다.
						GP.Box( PartyUserXpos - 4,
							PartyUserYpos - 4,
							PartyUserXpos + 4,
							PartyUserYpos + 4, colorArray[i]);
						GP.Box( PartyUserXpos - 5,   
							PartyUserYpos - 5,
							PartyUserXpos + 5,
							PartyUserYpos + 5, colorArray[i] ); 
				
 					}
				}
			}
			
			// 자신의 위치를 얼굴로 그려준다.

			// 그려줄 위치가 미니맵 다이얼로그 안에 들어 간다면 출력

			if(     ( abs( m_siMiniMapWidth  / 2 ) > abs(m_MovedPoint.x) +  m_MiniPlayerSpr.GetXSize()/2 )
				&&  ( abs( m_siMiniMapHeight / 2 ) > abs(m_MovedPoint.y) +  m_MiniPlayerSpr.GetYSize()/2 )
			  )
			{

				XPos = GetX() + m_pImageStatic->GetX() + ScreenRX + ( m_siMiniMapWidth / 2 ) - m_MovedPoint.x;
				YPos = GetY() + m_pImageStatic->GetY() + ScreenRY + ( m_siMiniMapHeight / 2 ) - m_MovedPoint.y;


				SI32 MiniPlayerfont = 0;

				switch (pclclient->pclCM->CR[1]->GetKind())
				{
				case KIND_HERO3:
					{
						MiniPlayerfont = 0;
						break;
					}
				case KIND_HEROINE3:
					{
						MiniPlayerfont = 1;
						break;
					}
				case KIND_HERO4:
					{
						MiniPlayerfont = 2;
						break;
					}
				case KIND_HEROINE4:
					{
						MiniPlayerfont = 3;
						break;
					}
				}

				GP.PutSpr( &m_MiniPlayerSpr ,XPos- m_MiniPlayerSpr.GetXSize()/2,YPos - m_MiniPlayerSpr.GetYSize()/2, MiniPlayerfont);
			} 	
		}

		//-----------------------------------------
		// 버튼 이미지를 찍어준다. 
		//-----------------------------------------
		// 확대, 축소 버튼을 그린다. SPR 이미지로 직접 그려야만 미니맵 이미지 위에 덧그릴수있다.

		SI32 ButtonDetailXpos = GetX() + m_pButtonDetail->GetX() + ScreenRX;
		SI32 ButtonDetailYpos = GetY() + m_pButtonDetail->GetY() + ScreenRY;

		SI32 ButtonDetailImageFont = 0;

		if( m_bEnlarge == TRUE  )
		{
			ButtonDetailImageFont = 4;
		}

		GP.PutSpr( & m_ButtonSpr , ButtonDetailXpos , ButtonDetailYpos , ButtonDetailImageFont);

		//------------------------------------------
		// 메인맵 버튼 이미지를 찍어준다.
		//------------------------------------------
		SI32 ButtonMainmapXpos = GetX() + m_pButtonMainmap->GetX() + ScreenRX;
		SI32 ButtonMainmapYpos = GetY() + m_pButtonMainmap->GetY() + ScreenRY;

		SI32 ButtonMainMapImageFont = 8;

		GP.PutSpr( &m_ButtonSpr, ButtonMainmapXpos, ButtonMainmapYpos, ButtonMainMapImageFont );

		if(pclClient->IsCountrySwitch( Switch_Go_RaccoonWorld ))
		{
			if( pclClient->IsWhereServiceArea( ConstServiceArea_Korea ) )
			{
				cltCharClient *pclCharMe = pclClient->pclCM->GetCharClient(1)	;
				if(pclCharMe	!= NULL )
				{
					if( pclCharMe->pclCI->clBI.m_uiRealAge	>=	18 )	// 한국은 18세 이상만 버튼이 보인다 
					{
						//------------------------------------------
						// 메인맵 버튼 이미지를 찍어준다.
						//------------------------------------------
						SI32 ButtonRaccoonXpos = GetX() + m_pButtonGoRaccoonWorld->GetX() + ScreenRX;
						SI32 ButtonRaccoonYpos = GetY() + m_pButtonGoRaccoonWorld->GetY() + ScreenRY;

						SI32 ButtonRaccoonImageFont = 11;

						GP.PutSpr( &m_ButtonSpr, ButtonRaccoonXpos, ButtonRaccoonYpos, ButtonRaccoonImageFont );
					}
				}
			}
			else
			{
				//------------------------------------------
				// 메인맵 버튼 이미지를 찍어준다.
				//------------------------------------------
				SI32 ButtonRaccoonXpos = GetX() + m_pButtonGoRaccoonWorld->GetX() + ScreenRX;
				SI32 ButtonRaccoonYpos = GetY() + m_pButtonGoRaccoonWorld->GetY() + ScreenRY;

				SI32 ButtonRaccoonImageFont = 11;

				GP.PutSpr( &m_ButtonSpr, ButtonRaccoonXpos, ButtonRaccoonYpos, ButtonRaccoonImageFont );
			}
		}
	}

	m_bDraw = false;

	return;
}

// 텍스트를 출력한다. 
void CNMiniMapDlg::DrawExplain( HDC hdc, SI32 DrawXPos, SI32 DrawYPos, SI32 dotx, SI32 doty, TCHAR* text, COLORREF rgb )
{
	SIZE size;
	GetTextExtentPoint32(hdc, text, _tcslen(text), &size);

	SI32 x = dotx - m_siDrawStartXPos - size.cx/2;
	SI32 y = doty - m_siDrawStartYPos - size.cy;

	if ( x < 0 || y < 0 || x + size.cx > m_siCurrentMapWidth || 
		 y + size.cy > m_siCurrentMapHeight )
	{
		return;
	}

	
	//RECT Rect;
	SetTextColor( hdc, RGB(0, 0, 0));
	TextOut(hdc, DrawXPos + x+1, DrawYPos + y+1, text, lstrlen(text));

	SetTextColor( hdc, rgb);
	TextOut(hdc, DrawXPos + x, DrawYPos + y, text, lstrlen(text));
}

// 마크를 출력한다. 
void CNMiniMapDlg::DrawMark( SI32 DrawXPos, SI32 DrawYPos, SI32 dotx, SI32 doty )
{
	GP.FillBox(	
		DrawXPos + dotx - 1 - m_siDrawStartXPos, 
		DrawYPos + doty - 1 - m_siDrawStartYPos, 
		DrawXPos + dotx + 1 - m_siDrawStartXPos, 
		DrawYPos + doty + 1 - m_siDrawStartYPos, 
		40,//SMAP_INFO_COLOR,
		FALSE);
}

// 작은 지도상의 정보를 표시한다.
void CNMiniMapDlg::DrawSMapInfo( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 DrawXPos, SI32 DrawYPos, SI32 x, SI32 y, TCHAR* text, COLORREF rgb )
{
	SI32 dotx, doty;
	if( GetSmallMapCordFromTileXY(x, y, &dotx, &doty) == TRUE ) 
	{
		// 보이는 영역 안에 있을 때만 계산한다. 
		if(GP.IsInClipArea(	DrawXPos + dotx-1 - m_siDrawStartXPos, 
			                DrawYPos + doty-1 - m_siDrawStartYPos, 
			                DrawXPos + dotx+1 - m_siDrawStartXPos, 
							DrawYPos + doty+1 - m_siDrawStartYPos ) == TRUE)
		{
			if( GP.LockSurface(lpBackScreen) == TRUE )
			{
				DrawMark( DrawXPos, DrawYPos, dotx, doty);

				GP.UnlockSurface(lpBackScreen);
			}

			HDC hdc;
			if(lpBackScreen->GetDC(&hdc) == DD_OK)
			{
				// 폰트를 설정한다. 
				HFONT hFont;
				if ( pclClient->siServiceArea == ConstServiceArea_English
					|| pclClient->siServiceArea == ConstServiceArea_NHNChina
					|| pclClient->siServiceArea == ConstServiceArea_EUROPE
				)
				{
					hFont = g_FontManager.SetFont( 13, TEXT("Tahoma") );
				}
				else
				{
					hFont = g_FontManager.SetFont();
				}

				HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

				SetBkMode(hdc, TRANSPARENT);

				DrawExplain(hdc, DrawXPos, DrawYPos, dotx, doty, text, rgb);

				SelectObject( hdc, hOldFont );

				lpBackScreen->ReleaseDC(hdc);
			}
		}
	}
}

// 작은 지도상의 건물 정보를 표시한다.
void CNMiniMapDlg::DrawSMapStructureInfo( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 DrawXPos, SI32 DrawYPos, SI32 x, SI32 y, SI32 siStructureUnique )
{
	SI32 dotx, doty;
	if( GetSmallMapCordFromTileXY(x, y, &dotx, &doty) == TRUE )
	{
		// 보이는 영역 안에 있을 때만 계산한다. 
		if(GP.IsInClipArea(	DrawXPos + dotx-1 - m_siDrawStartXPos, 
			DrawYPos + doty-1 - m_siDrawStartYPos, 
			DrawXPos + dotx+1 - m_siDrawStartXPos, 
			DrawYPos + doty+1 - m_siDrawStartYPos ) == TRUE)
		{
			SI32 imgfontindex = 0;

			TCHAR* text = NULL;
			switch (siStructureUnique)
			{
  				case  CITYHALL_UNIQUE:			imgfontindex =  0; //관청
												text = GetTxtFromMgr(2830);
												break;	
				case  BANK_UNIQUE:				imgfontindex =  4; //은행
												text = GetTxtFromMgr(1437);
												break;	
				case  STOCK_UNIQUE:				imgfontindex =  3;	//증권소
												text = GetTxtFromMgr(1469);
												break;	
				case  MARKET_UNIQUE:			imgfontindex =  1; 	//마켓
												text = GetTxtFromMgr(1664);
												break;	
				case  HOUSE_UNIQUE:				imgfontindex =  2; 	//저장고
												text = GetTxtFromMgr(370);
												break;	
				case  HUNT_UNIQUE:				imgfontindex = 10; 	//수렵장
												text = GetTxtFromMgr(1704);
												break;	
				case  POSTOFFICE_UNIQUE:		imgfontindex =  7; 	//우체국
												text = GetTxtFromMgr(5010);
												break;	
				case  HORSEMARKET_UNIQUE:		imgfontindex = 12; 	//사복시(탈것 거래소)
												text = GetTxtFromMgr(1757);
												break;	
				case REALESTATEMARKET_UNIQUE:	imgfontindex =  6; 	//부동산  
												text = GetTxtFromMgr(1775);
												break;	
				case SUMMONMARKET_UNIQUE:		imgfontindex = 13; 	//[진성] 소환수 거래소 이미지 13번으로... => 2008-7-16
												text = GetTxtFromMgr(1847);
												break;	
				case LAND_UNIQUE:				imgfontindex =  8; 	//농장
												text = GetTxtFromMgr(1826);
												break;	
				case FEAST_UNIQUE:				imgfontindex = 11; 	//내자시
												text = GetTxtFromMgr(1836);
												break;	
  			  //case SUMMONHEROMARKET_UNIQUE:	imgfontindex = 52; break;	//비변사
												//text = GetTxtFromMgr(2830);
												//break;	
				case MINE_UNIQUE:				imgfontindex =  9; 	//광산
												text = GetTxtFromMgr(2832);
												break;	
				case GUILD_UNIQUE:				imgfontindex =  5; 	//길드상단
												text = GetTxtFromMgr(5360);
												break;	
				case NEWMARKET_UNIQUE:		imgfontindex =  1; 	// [진성] 통합마켓 2008-1-5
												text = GetTxtFromMgr(9832);
												break;	
							
			default: 
				{
					return;
				}
				
			}
		

			if(GP.LockSurface(lpBackScreen) == TRUE )
			{
				GP.PutSpr( &m_MiniStructureSpr ,dotx- m_MiniStructureSpr.GetXSize()/2,doty, imgfontindex);

				GP.UnlockSurface(lpBackScreen);
			}

			HDC hdc;
			if(lpBackScreen->GetDC(&hdc) == DD_OK)
			{
				// 폰트를 설정한다. 
				HFONT hFont;
				if ( pclClient->siServiceArea == ConstServiceArea_English
					|| pclClient->siServiceArea == ConstServiceArea_NHNChina
					|| pclClient->siServiceArea == ConstServiceArea_EUROPE
				)
				{
					hFont = g_FontManager.SetFont( 13, TEXT("Tahoma") );
				}
				else
				{
					hFont = g_FontManager.SetFont();
				}

				HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

				SetBkMode(hdc, TRANSPARENT);
 
				DrawExplain(hdc, DrawXPos, DrawYPos+45, dotx, doty, text, RGB(255, 255, 255));

				SelectObject( hdc, hOldFont );

				lpBackScreen->ReleaseDC(hdc);
			}
		}
	}
}



// 작은 지도상의 건물 정보를 표시한다.
void CNMiniMapDlg::DrawSMapNPCInfo( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 DrawXPos, SI32 DrawYPos, SI32 x, SI32 y, SI32 Kind )
{
	SI32 dotx, doty;
	if( GetSmallMapCordFromTileXY(x, y, &dotx, &doty) == TRUE )
	{
		// 보이는 영역 안에 있을 때만 계산한다. 
		if(GP.IsInClipArea(	DrawXPos + dotx-1 - m_siDrawStartXPos, 
			DrawYPos + doty-1 - m_siDrawStartYPos, 
			DrawXPos + dotx+1 - m_siDrawStartXPos, 
			DrawYPos + doty+1 - m_siDrawStartYPos ) == TRUE)
		{
            SI32 imgfontindex = 0;
			
			if ( Kind == pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGMAN")) )
				imgfontindex = 0;
			else if ( Kind == pclClient->GetUniqueFromHash( TEXT("KIND_ROYALGUARD")) )
				imgfontindex = 1;
			else if ( Kind == pclClient->GetUniqueFromHash( TEXT("KIND_JANGGUMI")) )
				imgfontindex = 2;
			else if ( Kind == pclClient->GetUniqueFromHash( TEXT("KIND_JANGYOUNGSIL")) )
				imgfontindex = 3;
			else if ( Kind == pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGWOMAN")) )
				imgfontindex = 4;
			else
				return;



			if(GP.LockSurface(lpBackScreen) == TRUE )
			{
				GP.PutSpr( & m_MiniNPCSpr ,dotx- m_MiniNPCSpr.GetXSize()/2,doty - m_MiniNPCSpr.GetYSize()/2, imgfontindex);

				GP.UnlockSurface(lpBackScreen);
			}
		}
	}
}

// 축소지도 상에서 타일 x, y 좌표에 해당하는 좌표를 구해준다. 
bool CNMiniMapDlg::GetSmallMapCordFromTileXY( SI32 tilex, SI32 tiley, SI32* pdotx, SI32* pdoty )
{
	cltMapClient *pMap = (cltMapClient * )pclClient->pclMapManager->pclCurrentMap;
	if ( pMap )
	{	
		*pdotx = (( pMap->MH.MapYsize * MAP_TILE_XSIZE_HALF + tilex * MAP_TILE_XSIZE_HALF - tiley * MAP_TILE_XSIZE_HALF ) 
			/ (double)m_siRealMapXSize ) * (double)m_siCurrentMapWidth;
		*pdoty = (( tilex * MAP_TILE_YSIZE_HALF + tiley * MAP_TILE_YSIZE_HALF ) / (double)m_siRealMapYSize ) * (double)m_siCurrentMapHeight;

		return true;
	}

	return false;
}

// 지도와의 연동을 계산한다. 
void CNMiniMapDlg::RefreshPos()
{
	if ( ! IsShow() )
	{
		return;
	}

	// 지도에 연동되는 모드면 지도의 움직임에 따라 축소지도도 움직여준다. 
	if( m_bFollowMovePos )
	{
		SI32 RealDotX, RealDotY;

		if( GetSmallMapCordFromTileXY( m_siStandardPosX, m_siStandardPosY, &RealDotX, &RealDotY ) )
		{
			m_siDrawStartXPos = max( 0, RealDotX - ( m_siMiniMapWidth / 2 ) );
			m_siDrawStartXPos = min( m_siDrawStartXPos, m_siCurrentMapWidth - m_siMiniMapWidth );
			m_siDrawStartXPos = max( 0, m_siDrawStartXPos );

			m_siDrawStartYPos = max( 0, RealDotY - ( m_siMiniMapHeight / 2 ) );
			m_siDrawStartYPos = min( m_siDrawStartYPos, m_siCurrentMapHeight - m_siMiniMapHeight );
			m_siDrawStartYPos = max( 0, m_siDrawStartYPos );
		}
	}

	return;
}

void CNMiniMapDlg::SetPosition(TCHAR* szPosition)
{
	if(m_bMini) return;

	SetTitle(szPosition,CONTROLLER_TITLE_COLOR,BASE_FONT_HEIGHT,10,0);
}

void CNMiniMapDlg::ClearMinimapNPC()
{
	memset(m_MinimapNPC,0,sizeof(stMinimapNPC) * MAX_MINIMAP_NPC );
	m_MinimapNPCTable = 0;
}

void CNMiniMapDlg::SetMinimapNPC( SI32 Kind, cltCharPos* Pos  )
{
	if( Kind == pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGMAN") ) )
		m_MinimapNPCTable |= 0x000001;
	else if( Kind == pclClient->GetUniqueFromHash( TEXT("KIND_ROYALGUARD") ) )
		m_MinimapNPCTable |= 0x000002;
	else if( Kind == pclClient->GetUniqueFromHash( TEXT("KIND_JANGGUMI") ) )
		m_MinimapNPCTable |= 0x000004;
	else if( Kind == pclClient->GetUniqueFromHash( TEXT("KIND_JANGYOUNGSIL") ) )
		m_MinimapNPCTable |= 0x000008;
	else if( Kind == pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGWOMAN") ) )
		m_MinimapNPCTable |= 0x000010;
	else
		return;

	for( SI32 i = 0; i < MAX_MINIMAP_NPC; i++)
	{
		// 미니맵 엔피씨 기억 배열의 빈곳을 찾아 데이터를 넣는다.
		if( m_MinimapNPC[i].siKind == 0 )
		{
			m_MinimapNPC[i].siKind = Kind;
			m_MinimapNPC[i].clPosMinimap.Set( Pos );    
			break;
		}
	}       
}

void CNMiniMapDlg::Action()
{
	// 미니맵 상의 NPC, 건물의 정보가 갱신되었따면 미니맵을 새로 그려준다.
	if( pclClient->GameMode != GAMEMODE_CLIENT )
		return;


	if( m_fScale > 0.3f )	
	{

 		for( SI32 i = 0; i < MAX_MINIMAP_NPC ; i++)
		{
			if( m_OldMinimapNPCTable != m_MinimapNPCTable  )
			{
				m_OldMinimapNPCTable = m_MinimapNPCTable;
                					
				cltMapClient *pMap = (cltMapClient * )pclClient->pclMapManager->pclCurrentMap;
				if ( pMap == NULL )
				{
					return;
				}

				ReadyRateMap( pMap );

				break;
			}
		}
		
		for( i = 0; i < MAX_CONSTRUCTION_NUM; i++ )
		{
			cltVillageInfo* CurrentVillage = pclClient->pclVillageManager->pclVillageInfo[ pclClient->pclCM->CR[1]->GetCurrentVillageUnique() ];
			if(CurrentVillage != NULL)
			{
				SI32 strucureinfo = 0;
				
				for( SI16 j = 0; j < MAX_CONSTRUCTION_NUM; j++)
				{
					strucureinfo =	CurrentVillage->clVillageVary.clStructureInfo.Get(j);

					if(m_OldStructureInfo[j] != strucureinfo)
					{
						cltMapClient *pMap = (cltMapClient * )pclClient->pclMapManager->pclCurrentMap;
						if ( pMap == NULL )
						{
							return;
						}

						ReadyRateMap( pMap );
					}
				}
			}	
		}
	}

}

void CNMiniMapDlg::SetMonsterHome()
{
	if( pclClient->GameMode != GAMEMODE_CLIENT )
		return;

	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile(  TEXT("Data/MonsterHome.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(  TEXT("Data/MonsterHome.txt") );
	}


	if (!bLoadFile)
	{
		MsgBox(TEXT("Not LoadFromFile MonsterHome.txt"),TEXT("Error"));
	}

	SI32 xpos = 0;
	SI32 ypos = 0;
	TCHAR homename[8] = {0,};

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&xpos,		4,
		NDATA_INT32,	&ypos,		4,
		NDATA_MBSTRING, homename,	8,
		0, 0, 0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 index = 0;
	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData() )
		{
			if ( index >= MAX_MONSTERHOME_NUMBER )
				break;

			TCHAR* pText = GetTxtFromMgr( _ttoi( homename ) );
			m_MonsterHome[index].Set( xpos, ypos, pText );

			index++;
		}
	}
}
void CNMiniMapDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( MsgIndex )
	{
	case 0:	// 검색된 개인 상점으로 이동
		{
			if( false == RetValue )			return;
			GoRaccoonWorld()	;
		}
		break;
	}

}
void CNMiniMapDlg::GoRaccoonWorld()
{
	cltClient *pclclient = ( cltClient * )pclClient;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient(1)	;
	if(pclChar == NULL )	return	;

	cltGameMsgRequest_GoRaccoonWorld clshopdata( pclClient->pclCM->CR[1]->pclCI->GetPersonID());
	cltMsg clMsg( GAMEMSG_REQUEST_GO_RACCOONWORLD, sizeof( clshopdata ), (BYTE*)&clshopdata );
	
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
}