//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Client\Client.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <Mouse.h>
#include "Map\FieldObject\FieldObject.h"

#include "mapedit.h"
#include "..\Client\SmallMap\SmallMap.h"

#include "..\resource.h"

extern cltCommonLogic* pclClient;
extern _InputDevice				IpD;

static DWORD CaptureClock ;      // 게임을 캡춰한 시간. 

cltMapEdit::cltMapEdit( TCHAR* appname, HWND hwnd, HINSTANCE hinst, SI32 gamemode, SI32 defaultservicearea, SI32 runmode ) : cltClient( appname, hwnd, hinst, gamemode, defaultservicearea, NULL, runmode )
{
	SI32 i, j;
	
	
	hMapEditDlg					= NULL;
	hGeoInfoDlg					= NULL;
	
	CurrentMode					= MAPEDIT_MODE_TERRAIN;
	CurrentTerrainUnique		= 0;
	CurrentFieldObjectUnique	= 0;
	CurrentFieldObjectBrush		= -1;
	SelectedGeoInfoSlot			= 0;
	CurrentCustomCollision		= BLOCK_CUSTOM;
	SelectedFieldObjectUnique	= 0;

	uiCollisionBrush			= 1;
	
	StringCchCopy(MapFileName, 256,		TEXT(""));
	StringCchCopy(MapFileShortName, 128,TEXT(""));
	
	StringCchCopy(CurrentDirectory, MAX_PATH, TEXT(""));
	
	// 멥에디터에서 사용하는 File Open Box변수들의 초기화 
	PopMapFileInitializeMap(GetHwnd());
	
	
	//-------------------------------------
	// FieldObject 표시용 Surface
	//--------------------------------------
	pFOSurface		= NULL;
	pFOSurface2		= NULL;
	
	for(i = 0;i < FIELDOBJECT_COLL_MAP_YSIZE;i++)
		for(j = 0;j < FIELDOBJECT_COLL_MAP_XSIZE;j++)
		{
			FieldObjectCollMap[j][i] = 0;
		}
		
		FoTileSize	= 2;
}

cltMapEdit::~cltMapEdit()
{
	if(pFOSurface)
	{
		pFOSurface->Release();
		pFOSurface = NULL;
	}
	
	if(pFOSurface2)
	{
		pFOSurface2->Release();
		pFOSurface2 = NULL;
	}
	
	if (m_pNewMapPCXSpr.pImage)
	{
		GP.FreeSpr(m_pNewMapPCXSpr);
	}
	if(FloorSpr.pImage)
	{
		GP.FreeSpr(FloorSpr);
	}
	
}

void cltMapEdit::Create()
{
	cltClient::Create();
	
	/// 바로 게임으로 들어간다. 
	SetGameStatus(GS_GAME);
	
	GP.CreateSurface(pFOSurface,  FOSURFACE_XSIZE, FOSURFACE_YSIZE);
	GP.CreateSurface(pFOSurface2, FOSURFACE_XSIZE, FOSURFACE_YSIZE);
	
	
	// FieldObject 를 보여주기 위한 바닥 타일을 로딩한다. 
	GP.LoadSprFromFile(TEXT("MapEdit\\Floor.Spr"), &FloorSpr);
}

void cltMapEdit::CreateNewMapTool()
{
	DialogBox(GetInstance(), MAKEINTRESOURCE(IDD_NEWMAP), GetHwnd(), (DLGPROC)StaticNewMapDlgProc);
}


BOOL CALLBACK cltMapEdit::StaticNewMapDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return ((cltMapEdit*)pclClient)->NewMapDlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltMapEdit::NewMapDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	
	HWND hwnd;
	TCHAR buffer[256];
	
	switch(iMsg)
	{
	case WM_INITDIALOG:
		// 지도 정보 
        hwnd=GetDlgItem(hDlg, IDC_MAP_XSIZE);
		StringCchPrintf(buffer, 256, TEXT("%d"), pclMapManager->pclCurrentMap->MH.MapXsize);
		SetWindowText(hwnd, buffer);
		
        hwnd=GetDlgItem(hDlg, IDC_MAP_YSIZE);
		StringCchPrintf(buffer, 256, TEXT("%d"), pclMapManager->pclCurrentMap->MH.MapYsize);
		SetWindowText(hwnd, buffer);
		
		// 타일 세트의 초기값을 설정한다.
		StringCchPrintf(buffer, 256, TEXT("%d"), pclMapManager->pclCurrentMap->MH.TileSet1);
		SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_TILESET, buffer);
		
		return TRUE;
		
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_LOADSPR:
			{
				PopMapFileInitializeSpr(GetHwnd());
				if ( PopMapFileOpenDlg(GetHwnd(), MapFileName, MapFileShortName) )
				{
					if ( GP.LoadSprFromFile(MapFileName,&m_pNewMapPCXSpr) )
					{
						LPDIRECTDRAWSURFACE7 pSurface = NULL;
						SHORT xsize = m_pNewMapPCXSpr.GetXSize() ;
						SHORT ysize = m_pNewMapPCXSpr.GetYSize() ;

						GP.CreateSurface( pSurface, xsize,ysize);
						if( GP.LockSurface( pSurface ) ) {
							GP.PutSpr(&m_pNewMapPCXSpr, 0, 0, 0 );

							GP.UnlockSurface( pSurface );
						}

						//if ( pclClient->siServiceArea == ConstServiceArea_China )
						//{
						//	pclTileManager->LoadTileSet(230, 121);

						//	delete pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ];
						//	pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ] = NULL;
						//	pclMapManager->pclCurrentMap = NULL;

						//	pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ] = new cltMapClient(pclMapManager->CurrentMapIndex, 0, xsize, ysize, 0, 90, pclClient->pclCM, pclClient->pclTileManager, pclClient->pclFieldObjectManager);
						//	pclMapManager->MakeLastMap(pclMapManager->CurrentMapIndex);
						//	pclMapManager->pclCurrentMap  = pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ];

						//	pclMapManager->pclCurrentMap->MH.TileSet1	= 230;
						//	pclMapManager->pclCurrentMap->MH.TileSet2	= 121;
						//}
						//else
						//{
							pclTileManager->LoadTileSet(0, 90);

							delete pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ];
							pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ] = NULL;
							pclMapManager->pclCurrentMap = NULL;

							pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ] = new cltMapClient(pclMapManager->CurrentMapIndex, 0, xsize, ysize, 0, 90, pclClient->pclCM, pclClient->pclTileManager, pclClient->pclFieldObjectManager);
							pclMapManager->MakeLastMap(pclMapManager->CurrentMapIndex);
							pclMapManager->pclCurrentMap  = pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ];

							pclMapManager->pclCurrentMap->MH.TileSet1	= 0;
							pclMapManager->pclCurrentMap->MH.TileSet2	= 90;
						//}

						SI32 index[20] = {0,};
						SI32 k = 0 ;

						for ( SI32 i = 0 ; i < xsize ; i ++ )
						{
							for ( SI32 j = 0 ; j < ysize ; j ++)
							{
								//if ( pclClient->siServiceArea == ConstServiceArea_China )
								//{
								//	if ( g_graphics.GetSurfacePixel(i,j) == 0 ) // 검정
								//	{
								//		pclMapManager->pclCurrentMap->PutTile2(0, i, j, 238); // 얼음
								//	}
								//	else if ( g_graphics.GetSurfacePixel(i,j) == 61728 ) // 빨강
								//	{
								//		pclMapManager->pclCurrentMap->PutTile2(0, i, j, 234); // 봄잔디
								//	}
								//	else if ( g_graphics.GetSurfacePixel(i,j) == 183 ) // 파랑
								//	{
								//		pclMapManager->pclCurrentMap->PutTile2(0, i, j, 231); // 근해
								//	}
								//	else if ( g_graphics.GetSurfacePixel(i,j) == 63234 ) // 노랑
								//	{
								//		pclMapManager->pclCurrentMap->PutTile2(0, i, j, 236); // 서브2
								//	}
								//	else if ( g_graphics.GetSurfacePixel(i,j) == 26593 ) // 초록
								//	{
								//		pclMapManager->pclCurrentMap->PutTile2(0, i, j, 237); // 가을
								//	}
								//	else if ( g_graphics.GetSurfacePixel(i,j) == 19997 ) // 하늘
								//	{
								//		pclMapManager->pclCurrentMap->PutTile2(0, i, j, 232); // 심해
								//	}
								//	else
								//	{
								//		pclMapManager->pclCurrentMap->PutTile2(0, i, j, 230); // 기본
								//	}
								//}
								//else
								//{
									if ( g_graphics.GetSurfacePixel(i,j) == 0 ) // 검정
									{
										pclMapManager->pclCurrentMap->PutTile2(0, i, j, 70); // 얼음
									}
									else if ( g_graphics.GetSurfacePixel(i,j) == 61728 ) // 빨강
									{
										pclMapManager->pclCurrentMap->PutTile2(0, i, j, 30); // 봄잔디
									}
									else if ( g_graphics.GetSurfacePixel(i,j) == 183 ) // 파랑
									{
										pclMapManager->pclCurrentMap->PutTile2(0, i, j, 4); // 근해
									}
									else if ( g_graphics.GetSurfacePixel(i,j) == 63234 ) // 노랑
									{
										pclMapManager->pclCurrentMap->PutTile2(0, i, j, 12); // 중국꽃잔디
									}
									else if ( g_graphics.GetSurfacePixel(i,j) == 26593 ) // 초록
									{
										pclMapManager->pclCurrentMap->PutTile2(0, i, j, 60); // 가을
									}
									else if ( g_graphics.GetSurfacePixel(i,j) == 19997 ) // 하늘
									{
										pclMapManager->pclCurrentMap->PutTile2(0, i, j, 9); // 심해
									}
									else
									{
										pclMapManager->pclCurrentMap->PutTile2(0, i, j, 1); // 기본
									}
								//}
							}
						}

						// 작은 지도도 만든다.
						if(GameMode != GAMEMODE_SERVER)
						{
							pclMapManager->pclCurrentMap->MakeSmallMap(SMALLMAPMODE_MAPEDIT, NULL);
						}

						pSurface->Release();
						pSurface = NULL;


					}
				}
				EndDialog(hDlg, 0 );
			}
			return TRUE;
		case IDOK:
			{
				hwnd=GetDlgItem(hDlg, IDC_MAP_XSIZE);
				GetWindowText(hwnd, buffer, 10);
				SHORT xsize=_tstoi(buffer);
				
                hwnd=GetDlgItem(hDlg, IDC_MAP_YSIZE);
				GetWindowText(hwnd, buffer, 10);
				SHORT ysize=_tstoi(buffer);
				
				// 타일 세트의 초기값을 설정한다.
				GetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_TILESET, buffer, 60);
				SI32 tileset1 = _tstoi(buffer);
				SI32 tileset2 = 0;
				
				//-----------------------------
				// 타일 세트를 불러온다. 
				//-----------------------------
			pclTileManager->LoadTileSet(tileset1, tileset2);
				
				//-----------------------------
				// 지도를 초기화한다. 
				//-----------------------------
				delete pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ];
				pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ] = NULL;
				pclMapManager->pclCurrentMap = NULL;
				
				pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ] = new cltMapClient(pclMapManager->CurrentMapIndex, 0, xsize, ysize, tileset1, tileset2, pclClient->pclCM, pclClient->pclTileManager, pclClient->pclFieldObjectManager);
				pclMapManager->MakeLastMap(pclMapManager->CurrentMapIndex);
				pclMapManager->pclCurrentMap  = pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ];
				
				// 지도의 타일 세트를 기록한다. 
				pclMapManager->pclCurrentMap ->MH.TileSet1	= tileset1;
				
				// 작은 지도도 만든다.
				if(GameMode != GAMEMODE_SERVER)
				{
					pclMapManager->pclCurrentMap->MakeSmallMap(SMALLMAPMODE_MAPEDIT, NULL);
				}
				EndDialog(hDlg, 0 );
			}
			return TRUE;
		case IDCANCEL:
			// 게임을 종료한다. 
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	
	return FALSE;
}


void cltMapEdit::OpenMapEditSaveTool()
{
	PopMapFileInitializeMap(GetHwnd());

	// 마을 정보가 있는 상태에서는 저장이 안된다. 
	if(pclVillageManager)
	{
		SI32 result = MessageBox(NULL, TEXT("마을 정보를 지도에서 삭제해야 지도를 저장할 수 있습니다.\n")
			TEXT(" 지도에서 마을을 삭제하고 지도를 저장하시겠습니까?"), 
			TEXT("Warning"), MB_OKCANCEL|MB_ICONEXCLAMATION  );

		if(result == IDOK)
		{
			// 마을 정보를 지도에서 삭제한다. 
			Village();
			pclMapManager->pclCurrentMap->DeattachGate();
		}
		else
		{
			return ;
		}
	}
	
	
	GetCurrentDirectory(512, CurrentDirectory);
	   
	if(PopMapFileSaveDlg(GetHwnd(), MapFileName, MapFileShortName))
	{
		pclMapManager->pclCurrentMap->Save(MapFileName);
	}
	
	SetCurrentDirectory(CurrentDirectory);
}

void cltMapEdit::OpenMapEditLoadTool()
{
	TCHAR buffer[256];
	PopMapFileInitializeMap(GetHwnd());	

	// 마을 정보가 있는 상태에서는 불러오기가  안된다. 
	if(pclVillageManager)
	{
		SI32 result = MessageBox(NULL, TEXT("마을 정보를 지도에서 삭제해야 지도를 불러올 수 있습니다. \n") TEXT(" 지도에서 마을을 삭제하고 지도를 불러오시겠습니까?"), 
			TEXT("Warning"), MB_OKCANCEL|MB_ICONEXCLAMATION  );

		if(result == IDOK)
		{
			// 마을 정보를 지도에서 삭제한다. 
			Village();
		}
		else
		{
			return ;
		}
	}
	
	GetCurrentDirectory(512, CurrentDirectory);
	   
	if(PopMapFileOpenDlg(GetHwnd(), MapFileName, MapFileShortName))
	{
		// 작업 폴더를 원래 폴더로 변경한다. 
		SetCurrentDirectory(CurrentDirectory);
		
		// 지도를 불러온다 
		delete pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ];
		pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ] = NULL;
		pclMapManager->pclCurrentMap = NULL;
		
		
		//----------------------------
		// 지도의 헤더를 분석한다. 
		//----------------------------
		cltMapHeader clHeader;
		if(GetMapInfo(MapFileName, &clHeader) == FALSE)
		{
			MsgBox(TEXT("12f9nnf"), TEXT("f9j2f:%s"), buffer);
			return ;
		}
		
		
		//-----------------------------
		// 타일 세트를 불러온다. 
		//-----------------------------
		pclTileManager->LoadTileSet(clHeader.TileSet1, clHeader.TileSet2); 
		
		pclMapManager->Load(pclMapManager->CurrentMapIndex, 0, MapFileName, pclCM, pclTileManager,pclFieldObjectManager);
		pclMapManager->MakeLastMap(pclMapManager->CurrentMapIndex);
		
		pclMapManager->pclCurrentMap = pclMapManager->pclMapArray[pclMapManager->CurrentMapIndex ];
		
		// 작은 지도도 만든다.
		pclMapManager->pclCurrentMap->MakeSmallMap(SMALLMAPMODE_MAPEDIT, NULL);
		
		
	}
	   
	SetCurrentDirectory(CurrentDirectory);
}


void cltMapEdit::MapEditAction()
{
	// 화면이동관련
	ScrollAction();
	
	SI32 mapx	= pclUnderMouseObject->MapX;
	SI32 mapy	= pclUnderMouseObject->MapY;
	
	// 상태란에 정보를 업데이트 한다. 
	TCHAR str[128];
	StringCchPrintf(str, 128, TEXT("MapX:%d MapY:%d"), mapx, mapy);
	SetStatusBarText(str, 0);
	
	StringCchPrintf(str, 128, TEXT("CurrentMode:%d"), CurrentMode);
	SetStatusBarText(str, 1);
	
	
	if(IpD.LeftPressSwitch==TRUE)
	{
		
		if(CurrentMode == MAPEDIT_MODE_TERRAIN)
		{
			// Terrain의 종류에 따라서 다른 타일찍기 방법을 구사한다. 
			if(CurrentTerrainUnique)
			{
				SI32 siref		= pclMapManager->pclCurrentMap->pclTileManager->FindRefFromUnique_TileSet(CurrentTerrainUnique);
				SI32 atb			= pclMapManager->pclCurrentMap->pclTileManager->pclTileInfo[siref]->dwAtb;
				
				if(atb & TILEATB_NODIR)
				{
				}
				else if(atb & TILEATB_8DIR)
				{
					if(atb & TILEATB_LAYER1)
					{
						pclMapManager->pclCurrentMap->PutTile2(0, mapx, mapy, CurrentTerrainUnique);
					}
					else if(atb & TILEATB_LAYER2)
					{
						pclMapManager->pclCurrentMap->PutTile4(1, mapx, mapy, CurrentTerrainUnique);
					}
				}
				
			}
		}
		else if(CurrentMode == MAPEDIT_MODE_FIELDOBJET)
		{
			if(CurrentFieldObjectUnique)
			{
				SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(CurrentFieldObjectUnique);
				if ( ref < 0 && pclFieldObjectManager->pclInfo[ref] == NULL ) return ;

				if ( pclFieldObjectManager->pclInfo[ref]->siTreeShadow )
				{
					if ( pclMapManager->pclCurrentMap->PutFieldObject(mapx+1, mapy+1, pclFieldObjectManager->pclInfo[ref]->siTreeShadow ) )
					{
                        if ( pclMapManager->pclCurrentMap->PutFieldObject(mapx, mapy, CurrentFieldObjectUnique) == FALSE )
						{
							pclMapManager->pclCurrentMap->DeleteFieldObject(mapx+1, mapy+1);
						}
					}
				}
				else
				{
					pclMapManager->pclCurrentMap->PutFieldObject(mapx, mapy, CurrentFieldObjectUnique);
				}
				
			}
		}
		else if(CurrentMode == MAPEDIT_MODE_FIELDOBJETBRUSH)
		{
			if(CurrentFieldObjectBrush >=0 && SelectedFieldObjectUnique)
			{
				SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(SelectedFieldObjectUnique);
				if ( ref < 0 && pclFieldObjectManager->pclInfo[ref] == NULL ) return ;

				if ( pclFieldObjectManager->pclInfo[ref]->siTreeShadow )
				{
					if ( pclMapManager->pclCurrentMap->PutFieldObject(mapx+1, mapy+1, pclFieldObjectManager->pclInfo[ref]->siTreeShadow ) )
					{
						if ( pclMapManager->pclCurrentMap->PutFieldObject(mapx, mapy, SelectedFieldObjectUnique) == FALSE )
						{
							pclMapManager->pclCurrentMap->DeleteFieldObject(mapx+1, mapy+1);
						}
						else
						{
							SelectedFieldObjectUnique = pclFieldObjectManager->pclFieldObjectBrush[CurrentFieldObjectBrush]->GetRandUnique();
						}
					}
				}
				else
				{
					if(pclMapManager->pclCurrentMap->PutFieldObject(mapx, mapy, SelectedFieldObjectUnique)==TRUE)
					{
						// 다음의 랜덤한 Object를 선택한다.
						SelectedFieldObjectUnique = pclFieldObjectManager->pclFieldObjectBrush[CurrentFieldObjectBrush]->GetRandUnique();
					}
				}
			}
		}
		else if(CurrentMode == MAPEDIT_MODE_GEOINFO)
		{
			if(SelectedGeoInfoSlot > 0)
			{
				pclMapManager->pclCurrentMap->pclMapGeoInfo->SetGeoMap(mapx, mapy, SelectedGeoInfoSlot);
			}
		}
		else if(CurrentMode == MAPEDIT_MODE_CUSTOMCOLL)
		{

			div_t dv;

			if( uiCollisionBrush <= 0 || uiCollisionBrush >= MAX_COLLISION_BRUSH_NUM ) uiCollisionBrush = 1;

			dv = div(uiCollisionBrush, 2);

			if(dv.rem == 0)
				uiCollisionBrush -= dv.rem;

			SI32 i=0, j=0;

			SI32 startx = mapx - dv.quot;
			SI32 starty = mapy - dv.quot;

			for(i=0; i<uiCollisionBrush; i++)
			{
				for(j=0; j<uiCollisionBrush; j++)
				{
					// Map에서의 모든 Custom Collision을 제거한후에 설정한다.
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_CUSTOM);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT2);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT3);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT4);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT5);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT6);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT7);

					pclMapManager->pclCurrentMap->SetCollMap(startx + i, starty + j, CurrentCustomCollision);

				}
			}

			// Map에서의 모든 Custom Collision을 제거한후에 설정한다.
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_CUSTOM);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT1);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT2);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT3);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT4);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT5);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT6);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT7);

			//pclMapManager->pclCurrentMap->SetCollMap(mapx, mapy, CurrentCustomCollision);
		}
	}
	else if(IpD.RightPressSwitch==TRUE)
	{
		if(CurrentMode == MAPEDIT_MODE_TERRAIN)
		{
			// 부모 타일을 찍어준다.(지워주는 효과) 
			if(CurrentTerrainUnique)
			{
				SI32 ref		= pclMapManager->pclCurrentMap->pclTileManager->FindRefFromUnique_TileSet(CurrentTerrainUnique);
				SI32 atb		= pclMapManager->pclCurrentMap->pclTileManager->pclTileInfo[ref]->dwAtb;
				
				SI32 unique;
				if(pclMapManager->pclCurrentMap->pclTileManager->pclTileInfo[ref]->pParent)
				{
					unique	= pclMapManager->pclCurrentMap->pclTileManager->pclTileInfo[ref]->pParent->Unique;
					
					if(atb & TILEATB_NODIR)
					{
						// 아무 조치도 하지 않는다. 
					}
					else if(atb & TILEATB_8DIR)
					{
						if(atb & TILEATB_LAYER1)
						{
							pclMapManager->pclCurrentMap->PutTile2(0, mapx, mapy, unique);
						}
						else if(atb & TILEATB_LAYER2)
						{
							pclMapManager->pclCurrentMap->PutTile4(1, mapx, mapy, unique);
							
						}
					}
				}
				
			}
		}
		
		if(CurrentMode == MAPEDIT_MODE_FIELDOBJET
			|| CurrentMode == MAPEDIT_MODE_FIELDOBJETBRUSH)
		{
			//if(CurrentFieldObjectUnique)
			{
				pclMapManager->pclCurrentMap->DeleteFieldObject(mapx, mapy);
			}
		}
		else if(CurrentMode == MAPEDIT_MODE_GEOINFO)
		{
			//if(SelectedGeoInfoSlot > 0)
			{
				pclMapManager->pclCurrentMap->pclMapGeoInfo->SetGeoMap(mapx, mapy, 0);
			}
		}
		else if(CurrentMode == MAPEDIT_MODE_CUSTOMCOLL)
		{
			div_t dv;

			if( uiCollisionBrush <= 0 || uiCollisionBrush >= MAX_COLLISION_BRUSH_NUM ) uiCollisionBrush = 1;

			dv = div(uiCollisionBrush, 2);

			if(dv.rem == 0)
				uiCollisionBrush -= dv.rem;

			SI32 i=0, j=0;

			SI32 startx = mapx - dv.quot;
			SI32 starty = mapy - dv.quot;

			for(i=0; i<uiCollisionBrush; i++)
			{
				for(j=0; j<uiCollisionBrush; j++)
				{
					// Map에서의 모든 Custom Collision을 제거한후에 설정한다.
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_CUSTOM);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT2);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT3);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT4);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT5);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT6);
					pclMapManager->pclCurrentMap->DeleteCollMap(startx + i, starty + j, BLOCK_EVENT7);
				}
			}


			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_CUSTOM);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT1);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT2);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT3);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT4);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT5);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT6);
			//pclMapManager->pclCurrentMap->DeleteCollMap(mapx, mapy, BLOCK_EVENT7);
		}   
		
		
	}
	
}

void cltMapEdit::PopMapFileInitializeMap(HWND hwnd)
{
	static TCHAR szFilter[]=TEXT("map file(*.,map)\0*.map\0") \
		TEXT("total file(*.*)\0*.*\0\0");
	
	ZeroMemory(&MapFileOpen, sizeof(OPENFILENAME));
	
	MapFileOpen.lStructSize=sizeof(OPENFILENAME);
	MapFileOpen.hwndOwner=hwnd;
	MapFileOpen.lpstrFilter=szFilter;
	MapFileOpen.lpstrFile=NULL;
	MapFileOpen.nMaxFile=_MAX_PATH;
	MapFileOpen.lpstrFileTitle=NULL;
	MapFileOpen.nMaxFileTitle=_MAX_FNAME+_MAX_EXT;
	MapFileOpen.lpstrInitialDir=NULL;
	MapFileOpen.lpstrTitle=NULL;
	MapFileOpen.Flags=0;
	MapFileOpen.nFileOffset=0;
	MapFileOpen.nFileExtension=0;
	MapFileOpen.lpstrDefExt=TEXT("map");
	MapFileOpen.lCustData=0L;
	MapFileOpen.lpfnHook=NULL;
	MapFileOpen.lpTemplateName=NULL;
	
}

void cltMapEdit::PopMapFileInitializeSpr(HWND hwnd)
{
	static TCHAR szFilter[]=TEXT("SPR file(*.,spr)\0*.spr\0") \
		TEXT("total file(*.*)\0*.*\0\0");
	
	ZeroMemory(&MapFileOpen, sizeof(OPENFILENAME));
	
	MapFileOpen.lStructSize=sizeof(OPENFILENAME);
	MapFileOpen.hwndOwner=hwnd;
	MapFileOpen.lpstrFilter=szFilter;
	MapFileOpen.lpstrFile=NULL;
	MapFileOpen.nMaxFile=_MAX_PATH;
	MapFileOpen.lpstrFileTitle=NULL;
	MapFileOpen.nMaxFileTitle=_MAX_FNAME+_MAX_EXT;
	MapFileOpen.lpstrInitialDir=NULL;
	MapFileOpen.lpstrTitle=NULL;
	MapFileOpen.Flags=0;
	MapFileOpen.nFileOffset=0;
	MapFileOpen.nFileExtension=0;
	MapFileOpen.lpstrDefExt=TEXT("spr");
	MapFileOpen.lCustData=0L;
	MapFileOpen.lpfnHook=NULL;
	MapFileOpen.lpTemplateName=NULL;
	
}

//// 지도화일 불러오기
BOOL cltMapEdit::PopMapFileOpenDlg(HWND hwnd, TCHAR* pstrFileName,  TCHAR* pstrTitleName)
{
	
	MapFileOpen.hwndOwner=hwnd;
	MapFileOpen.lpstrFile=pstrFileName;
	MapFileOpen.lpstrFileTitle=pstrTitleName;
	MapFileOpen.Flags=OFN_HIDEREADONLY|OFN_CREATEPROMPT;
	
	return GetOpenFileName(&MapFileOpen);
	
}

//// 지도화일 저장하기 
BOOL cltMapEdit::PopMapFileSaveDlg(HWND hwnd, TCHAR* pstrFileName, TCHAR* pstrTitleName)
{
	MapFileOpen.hwndOwner	= hwnd;
	MapFileOpen.lpstrFile   = pstrFileName;
	MapFileOpen.lpstrFileTitle=pstrTitleName;
	
	MapFileOpen.Flags=OFN_OVERWRITEPROMPT;
	
	return GetSaveFileName(&MapFileOpen);
}



// Terrain Tree를 추가하는 법. (Windows API 정복 690페이지 참조) 
void cltMapEdit::InsertTree_Terrain(HWND hDlg, HTREEITEM pNode, cltTileInfo* pclParentTileInfo)
{
	SI32 index = 0;
	
	while(pclClient->pclTileManager->pclTileInfo[index])
	{
		
		cltTileInfo* pclTileInfo = pclClient->pclTileManager->pclTileInfo[index];
		
		if(pclParentTileInfo == pclTileInfo->pParent)
		{
			HTREEITEM Node  = TreeViewInsert(GetDlgItem(hDlg, IDC_TREE_TERRAIN), pclTileInfo->szName, pclTileInfo->Unique, pNode);
			
			InsertTree_Terrain(hDlg, Node, pclTileInfo);
		}
		
		index++;
	}
}

// Field Object를 트리에 추가. 
void cltMapEdit::InsertTree_FieldObject(HWND hDlg, HTREEITEM pNode, cltFieldObjectTypeInfo* pclParentInfo)
{
	SI32 i;
	
	for(i = 0;i < MAX_FIELDOBJECTTYPE_NUMBER;i++)
	{
		if(pclClient->pclFieldObjectManager->pclTypeInfo[i] == NULL)continue;
		
		cltFieldObjectTypeInfo* pclTypeInfo = pclClient->pclFieldObjectManager->pclTypeInfo[i];
		
		if(pclParentInfo == pclTypeInfo->pParent)
		{
			HTREEITEM Node  = TreeViewInsert(GetDlgItem(hDlg, IDC_TREE_FIELDOBJECT), pclTypeInfo->pszName, pclTypeInfo->Unique, pNode);
			
			// 실질적인 하위 Field Object 추가. 
			SI32 temp_index = 0;
			while(pclClient->pclFieldObjectManager->pclInfo[temp_index] )
			{
				cltFieldObjectInfo* pclinfo = pclClient->pclFieldObjectManager->pclInfo[temp_index];
				
				if(pclinfo->dwType == pclTypeInfo->dwType)
				{
					TreeViewInsert(GetDlgItem(hDlg, IDC_TREE_FIELDOBJECT), pclinfo->pszName, pclinfo->Unique, Node);
				}
				
				temp_index++;
			}
			
			
			InsertTree_FieldObject(hDlg, Node, pclTypeInfo);
		}
	}
}


// 트리구조에  추가한다.
HTREEITEM cltMapEdit::TreeViewInsert(HWND hwnd, TCHAR* name, SI32 para, HTREEITEM node)
{
	TV_ITEM tvi;
	TV_INSERTSTRUCT tvis;
	TCHAR cName[256];
	
	if(para == 0)		StringCchPrintf(cName, 256, TEXT("%s"), name);
	else				StringCchPrintf(cName, 256, TEXT("%s (%d)"), name, para);
	
	tvis.hInsertAfter	=	TVI_LAST;
	tvi.mask			=	TVIF_TEXT|TVIF_PARAM; 
	tvi.pszText			=	cName;	// name;
	tvi.lParam			=	para;
	
	if(node == 0)		tvis.hParent		=	TVI_ROOT;
	else				tvis.hParent		=	node;
	
	tvis.item			=	tvi;
	
	return TreeView_InsertItem(hwnd, &tvis);
	
}


void cltMapEdit::InsertFieldObjectBrush(HWND hDlg)
{
	
	SI32 index = 0;
	while(pclFieldObjectManager->pclFieldObjectBrush[index])
	{
		SendMessage(GetDlgItem(hDlg, IDC_FIELDOBJECTBRUSH), LB_ADDSTRING, index, (LPARAM)pclFieldObjectManager->pclFieldObjectBrush[index]->szName);
		index++;
	}
}



//-----------------------------------
// 모다리스 대화 상자 처리 관련 
//-----------------------------------
BOOL cltMapEdit::ModalessDlgProc(MSG* pmsg)
{
	if(cltClient::ModalessDlgProc(pmsg) == TRUE)
	{
		return TRUE;
	}
	
	if(hMapEditDlg && IsWindow(hMapEditDlg) && IsDialogMessage(hMapEditDlg, pmsg))
	{
		return TRUE;
	}
	
	if(hGeoInfoDlg && IsWindow(hGeoInfoDlg) && IsDialogMessage(hGeoInfoDlg, pmsg))
	{
		return TRUE;
	}
	
	return FALSE;
}

//-----------------------------------
// WM_COMMAND메시지 처리 함수. 
//-----------------------------------
bool cltMapEdit::DoMsg_WM_COMMAND(WPARAM wParam)
{
	cltClient::DoMsg_WM_COMMAND(wParam);
	
	switch(LOWORD(wParam))
	{
		
	case ID_TOOLBAR_NEWMAP:
		CreateNewMapTool();
		break;
	case ID_TOOLBAR_LOAD:
		OpenMapEditLoadTool();
		break;
	case ID_TOOLBAR_SAVE:
		OpenMapEditSaveTool();
		break;
		// SEPARATOR
	case ID_TOOLBAR_TERRAINTOOL:
		OpenMapEditTerrainTool();
		break;
	case ID_TOOLBAR_HEADERMANAGER:
		OpenMapEditHeaderManager();
		break;
	case ID_TOOLBAR_GEOINFO:
		CreateGeoInfoTool();
		break;
		// SEPARATOR
		
	case ID_TOOLBAR_CAPTUREMAP:
		if(TABS(CurrentClock  - CaptureClock ) > 1500)
		{
			CaptureClock = CurrentClock;
			
			cltMapClient* pclmap =(cltMapClient*)pclMapManager->pclCurrentMap;
			
			pclmap->MapCapture();
		}
		break;
		
	case ID_TOOLBAR_VILLAGE:
		Village();
		break;
		// SEPARATOR
		
	case ID_TOOLBAR_COLLDRAW:
		{
			cltMapClient* pclmap = (cltMapClient*)pclMapManager->pclCurrentMap;
			if(pclmap->bCollDrawSwitch == false)
			{
				pclmap->bCollDrawSwitch = true;
			}
			else
			{
				pclmap->bCollDrawSwitch = false;
			}
		}
		break;
		
	case ID_TOOLBAR_NEWCOLL:
		pclMapManager->pclCurrentMap->NewFieldObjectSetInMap();
		break;
		// 임의 충돌 지역설정. 
	case ID_TOOLBAR_CUSTOMCOLL:
		OpenCustomCollision();
		break;

	case ID_TOOLBAR_DELETEUNIQUE:
		DeleteUniqueTool();
		break;
		
		
		
	}
	
	return true;
	
}