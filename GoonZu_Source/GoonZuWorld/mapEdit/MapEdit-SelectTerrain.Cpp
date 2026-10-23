//---------------------------------
// 2003/7/23 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Client\Client.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

//-----------------------------
// Common
//-----------------------------
#include "Map\FieldObject\FieldObject.h"
#include <Mouse.h>


#include "mapedit.h"

#include "..\resource.h"

extern cltCommonLogic* pclClient;

void cltMapEdit::OpenMapEditTerrainTool()
{

	if(!IsWindow(hMapEditDlg))
	{
		ShowCursor(TRUE);
		hMapEditDlg = CreateDialog(GetInstance(), MAKEINTRESOURCE(IDD_SELECTTERRAIN), GetHwnd(), (DLGPROC)StaticSelectTerrainDlgProc);
		
		ShowWindow(hMapEditDlg, SW_SHOW);
		ShowCursor(FALSE);
		
	}

}

BOOL CALLBACK cltMapEdit::StaticSelectTerrainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return ((cltMapEdit*)pclClient)->SelectTerrainDlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltMapEdit::SelectTerrainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR buffer[FILE_NAME_SIZE] = TEXT("");
	HWND hwnd;
	SHORT hindex=0;


	switch(iMsg)
	{
	case WM_INITDIALOG:

		//-----------------------------------------
		// Terrain 
		// IDC_TREE_TERRAIN
		//-----------------------------------------
		InsertTree_Terrain(hDlg, (HTREEITEM)0, NULL);

		//------------------------------------------
		//  Field Object 
		// IDC_TREE_FIELDOBJECT
		//------------------------------------------
		InsertTree_FieldObject(hDlg, (HTREEITEM)0, NULL);

		//-----------------------------------------
		// Field Object Brush
		//-----------------------------------------
		InsertFieldObjectBrush(hDlg);

		// FOShape를 조절하기 위한 컨트롤을 설정한다. 
		InitFOShape(hDlg);

    	return TRUE;

	case WM_DESTROY:
		hMapEditDlg = NULL;
		return TRUE;


	case WM_NOTIFY:
		{

			LPNMHDR pnmhdr=(LPNMHDR)lParam;

			// Terrain으로부터 온 메시지 
			if(pnmhdr->hwndFrom == 	GetDlgItem(hDlg, IDC_TREE_TERRAIN) )
			{
				if(pnmhdr->code==TVN_SELCHANGED)
				{
					TV_ITEM tvi;
					
					tvi.mask=TVIF_PARAM;
					tvi.hItem=((LPNM_TREEVIEW)pnmhdr)->itemNew.hItem;

					if(TreeView_GetItem(GetDlgItem(hDlg, IDC_TREE_TERRAIN), &tvi) == TRUE)
					{
						CurrentTerrainUnique	= tvi.lParam;
						CurrentMode				= MAPEDIT_MODE_TERRAIN;
					}
					
				}
			}
			// Field Object으로부터 온 메시지 
			else if(pnmhdr->hwndFrom == 	GetDlgItem(hDlg, IDC_TREE_FIELDOBJECT) )
			{
				if(pnmhdr->code==TVN_SELCHANGED)
				{
					TV_ITEM tvi;
					
					tvi.mask=TVIF_PARAM;
					tvi.hItem=((LPNM_TREEVIEW)pnmhdr)->itemNew.hItem;

					if(TreeView_GetItem(GetDlgItem(hDlg, IDC_TREE_TERRAIN), &tvi) == TRUE)
					{
						CurrentFieldObjectUnique = tvi.lParam;
						CurrentMode				 = MAPEDIT_MODE_FIELDOBJET;

						SelectedFieldObjectUnique= tvi.lParam;

						// 선택된 FieldObject의 이미지를 FOSurface로 옮겨 그린다. 
						SetFOShape(hDlg, CurrentFieldObjectUnique);
						DrawFOShape(hDlg, CurrentFieldObjectUnique);

					}
					
				}
			}
			// 스핀 콘트롤로부터 신호가 들어오면, 
			else if(pnmhdr->hwndFrom == 	GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_FOTILESIZE) )
			{
				hwnd				= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_FOTILESIZE);
				FoTileSize			= LOWORD( SendMessage(hwnd, UDM_GETPOS, 0, 0 ) );

				DrawFOShape(hDlg, CurrentFieldObjectUnique);


			}
			// 스핀 콘트롤로부터 신호가 들어오면, 
			else if(pnmhdr->hwndFrom == 	GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_FOXSIZE) )
			{
				hwnd				= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_FOXSIZE);

				SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(CurrentFieldObjectUnique);
				if(ref >= 0 )
				{
					pclFieldObjectManager->pclInfo[ref]->TileXsize	= LOWORD( SendMessage(hwnd, UDM_GETPOS, 0, 0 ) );
					
					DrawFOShape(hDlg, CurrentFieldObjectUnique);

					SaveFieldObjectInfo(CurrentFieldObjectUnique);
				}
				
			}
			else if(pnmhdr->hwndFrom == 	GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_FOYSIZE) )
			{
				hwnd				= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_FOYSIZE);

				SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(CurrentFieldObjectUnique);
				if(ref >= 0)
				{
					pclFieldObjectManager->pclInfo[ref]->TileYsize	= LOWORD( SendMessage(hwnd, UDM_GETPOS, 0, 0 ) );
					
					DrawFOShape(hDlg, CurrentFieldObjectUnique);

					SaveFieldObjectInfo(CurrentFieldObjectUnique);
				}
			}
			else if(pnmhdr->hwndFrom == 	GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_CENTERX) )
			{
				hwnd				= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_CENTERX);

				SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(CurrentFieldObjectUnique);
				if(ref >= 0)
				{
					pclFieldObjectManager->pclInfo[ref]->TileCenterX	= LOWORD( SendMessage(hwnd, UDM_GETPOS, 0, 0 ) );
					
					DrawFOShape(hDlg, CurrentFieldObjectUnique);

					SaveFieldObjectInfo(CurrentFieldObjectUnique);
				}
			}
			else if(pnmhdr->hwndFrom == 	GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_CENTERY) )
			{
				hwnd				= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_CENTERY);

				SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(CurrentFieldObjectUnique);
				if(ref >= 0)
				{
					pclFieldObjectManager->pclInfo[ref]->TileCenterY	= LOWORD( SendMessage(hwnd, UDM_GETPOS, 0, 0 ) );
					
					DrawFOShape(hDlg, CurrentFieldObjectUnique);

					SaveFieldObjectInfo(CurrentFieldObjectUnique);
				}
			}
			else if(pnmhdr->hwndFrom == 	GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_INDENTX) )
			{
				hwnd				= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_INDENTX);

				SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(CurrentFieldObjectUnique);
				if(ref >= 0)
				{
					// Over flow가 일어나지 않으려면 (SI16)으로 캐스팅 해야 한다.
					pclFieldObjectManager->pclInfo[ref]->IndentX	= (SI16)LOWORD( SendMessage(hwnd, UDM_GETPOS, 0, 0 ) );
					
					DrawFOShape(hDlg, CurrentFieldObjectUnique);

					SaveFieldObjectInfo(CurrentFieldObjectUnique);
				}
			}
			else if(pnmhdr->hwndFrom == 	GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_INDENTY) )
			{
				hwnd				= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_INDENTY);

				SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(CurrentFieldObjectUnique);
				if(ref >= 0)
				{
					// Over flow가 일어나지 않으려면 (SI16)으로 캐스팅 해야 한다.
					SI16 s = (SI16)LOWORD( SendMessage(hwnd, UDM_GETPOS, 0, 0 ) );
					pclFieldObjectManager->pclInfo[ref]->IndentY	= s;
					
					DrawFOShape(hDlg, CurrentFieldObjectUnique);

					SaveFieldObjectInfo(CurrentFieldObjectUnique);
				}
			}
		}	
		return 0;


	case WM_COMMAND:

		switch(LOWORD(wParam))
		{

		case IDC_BUTTON_MAPEDIT_ANISET:
			{
				SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(CurrentFieldObjectUnique);
				if(ref >= 0)
				{
					TCHAR temp[256];
					// FrameNum
					GetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_FOFRAMENUM, temp, 100);
					pclFieldObjectManager->pclInfo[ref]->AniFrame = _tstoi(temp);
					
					// Delay
					GetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_FODELAY, temp, 100);
					pclFieldObjectManager->pclInfo[ref]->AniDelay = _tstoi(temp);

					SaveFieldObjectInfo(CurrentFieldObjectUnique);
				}
				
			}
			return TRUE;

		case IDC_FIELDOBJECTBRUSH:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				CurrentFieldObjectBrush = SendMessage(GetDlgItem(hDlg, IDC_FIELDOBJECTBRUSH), LB_GETCURSEL, 0, 0);
				CurrentMode				= MAPEDIT_MODE_FIELDOBJETBRUSH;
				
				SelectedFieldObjectUnique = pclFieldObjectManager->pclFieldObjectBrush[CurrentFieldObjectBrush]->GetRandUnique();

				// 선택된 브러시의 내용물을 리스트 박스에 추가한다. 
				{
					// 새로운 내용물을 추가하기 전에 기존 내용물을 제거한다.
					while(SendMessage(GetDlgItem(hDlg, IDC_LIST_BRUSHCONTENT), LB_DELETESTRING, 0, 0) > 0)
					{
					}

					// 새로운 내용을 추가한다. 
					SI32 i;
					for(i = 0; i < pclFieldObjectManager->pclFieldObjectBrush[CurrentFieldObjectBrush]->FielObjectNumber;i++)
					{
						TCHAR* name = pclFieldObjectManager->pclFieldObjectBrush[CurrentFieldObjectBrush]->pFieldObjectInfo[i]->pszName;
						
						SendMessage(GetDlgItem(hDlg, IDC_LIST_BRUSHCONTENT), LB_ADDSTRING, 0, (LPARAM)name);
					}
				}
				
				break;
			}
			return TRUE;

			// BUSH에 따른 강제 선택 
		case IDC_LIST_BRUSHCONTENT:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				CurrentFieldObjectBrush = SendMessage(GetDlgItem(hDlg, IDC_FIELDOBJECTBRUSH), LB_GETCURSEL, 0, 0);
				CurrentMode				= MAPEDIT_MODE_FIELDOBJETBRUSH;

				SI32 siBrushContent = SendMessage(GetDlgItem(hDlg, IDC_LIST_BRUSHCONTENT), LB_GETCURSEL, 0, 0);
				if(pclFieldObjectManager->pclFieldObjectBrush[CurrentFieldObjectBrush]->FielObjectNumber>siBrushContent)
				{
					SelectedFieldObjectUnique = pclFieldObjectManager->pclFieldObjectBrush[CurrentFieldObjectBrush]->pFieldObjectInfo[siBrushContent]->Unique;
				}
				break;
			}

			return TRUE;

		case IDOK:
		    DestroyWindow(hMapEditDlg);
		   return TRUE;

		case IDCANCEL:
		    DestroyWindow(hMapEditDlg);
		   return TRUE;

		}
		break;
	}
	return FALSE;
}

// FOShape를 조절하기 위한 컨트롤을 설정한다. 
void cltMapEdit::InitFOShape(HWND hDlg)
{
	HWND hspin;
	HWND hedit;

	// 타일 사이즈 관리. 
	hspin	= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_FOTILESIZE);
	hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_FOTILESIZE);

	SendMessage(hspin, UDM_SETBUDDY, (WPARAM)hedit, 0);
	SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM((FIELDOBJECT_COLL_MAP_XSIZE-1)/2, 2));

	// FOXsize
	hspin	= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_FOXSIZE);
	hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_FOXSIZE);

	SendMessage(hspin, UDM_SETBUDDY, (WPARAM)hedit, 0);
	SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(10, 1));


	// FOYsize
	hspin	= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_FOYSIZE);
	hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_FOYSIZE);

	SendMessage(hspin, UDM_SETBUDDY, (WPARAM)hedit, 0);
	SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(10, 1));

	// CenterX
	hspin	= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_CENTERX);
	hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_CENTERX);

	SendMessage(hspin, UDM_SETBUDDY, (WPARAM)hedit, 0);
	SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(10, 0));

	// CenterY
	hspin	= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_CENTERY);
	hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_CENTERY);

	SendMessage(hspin, UDM_SETBUDDY, (WPARAM)hedit, 0);
	SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(10, 0));


	// IndentX
	hspin	= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_INDENTX);
	hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_INDENTX);

	SendMessage(hspin, UDM_SETBUDDY, (WPARAM)hedit, 0);
	SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(MAP_TILE_XSIZE*4, -MAP_TILE_XSIZE*4));

	// IndentY
	hspin	= GetDlgItem(hDlg, IDC_SPIN_MAPEDIT_INDENTY);
	hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_INDENTY);

	SendMessage(hspin, UDM_SETBUDDY, (WPARAM)hedit, 0);
	SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(MAP_TILE_YSIZE*8, -MAP_TILE_YSIZE*8));

}

// Field Object 표시기의 정보를 설정한다. 
void cltMapEdit::SetFOShape(HWND hDlg, SI32 founique)
{
	TCHAR buffer[256];
	HWND hedit;

	// 타일 사이즈를 설정한다. 
	hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_FOTILESIZE);

	StringCchPrintf(buffer, 256, TEXT("%d"), FoTileSize);
	SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_FOTILESIZE, buffer);

	SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(founique);
	if(ref >= 0 )
	{
		//FOXsize
		hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_FOXSIZE);
		
		StringCchPrintf(buffer, 256, TEXT("%d"), pclFieldObjectManager->pclInfo[ref]->TileXsize);
		SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_FOXSIZE, buffer);
		
		//FOYsize
		hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_FOYSIZE);
		
		StringCchPrintf(buffer, 256, TEXT("%d"), pclFieldObjectManager->pclInfo[ref]->TileYsize);
		SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_FOYSIZE, buffer);

		//CenterX
		hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_CENTERX);
		
		StringCchPrintf(buffer, 256, TEXT("%d"), pclFieldObjectManager->pclInfo[ref]->TileCenterX);
		SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_CENTERX, buffer);

		//CenterY
		hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_CENTERY);
		
		StringCchPrintf(buffer, 256, TEXT("%d"), pclFieldObjectManager->pclInfo[ref]->TileCenterY);
		SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_CENTERY, buffer);

		//IndentX
		hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_INDENTX);
		
		StringCchPrintf(buffer, 256, TEXT("%d"), pclFieldObjectManager->pclInfo[ref]->IndentX);
		SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_INDENTX, buffer);

		//IndentY
		hedit	= GetDlgItem(hDlg, IDC_EDIT_MAPEDIT_INDENTY);
		
		StringCchPrintf(buffer, 256, TEXT("%d"), pclFieldObjectManager->pclInfo[ref]->IndentY);
		SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_INDENTY, buffer);

		// FrameNum
		StringCchPrintf(buffer, 256, TEXT("%d"), pclFieldObjectManager->pclInfo[ref]->AniFrame);
		SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_FOFRAMENUM, buffer);
		
		// Delay
		StringCchPrintf(buffer, 256, TEXT("%d"), pclFieldObjectManager->pclInfo[ref]->AniDelay);
		SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_FODELAY, buffer);
		
	}
}

//Field Object 정보를 저장한다.
bool cltMapEdit::SaveFieldObjectInfo(SI32 founique)
{
	SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(founique);
	if(ref < 0)return false;

	// 파일에서 배열 정보를 읽어온다. 

	TCHAR szfilename[128];
	if(pclFieldObjectManager->pclInfo[ref]->GetInfoFileName(szfilename) == true)
	{
		return pclFieldObjectManager->pclInfo[ref]->WriteSetArrayInfo(szfilename);
	}

	return false;
}

// Field Object를 FOSurface에 옮겨그린다. 
void cltMapEdit::DrawFOShape(HWND hDlg, SI32 founique)
{
	SI32 i, j;

	//------------------------------------
	//  FieldObject의 영역을 초기화한다.
	//------------------------------------
	for(i = 0;i < FIELDOBJECT_COLL_MAP_YSIZE;i++)
		for(j = 0;j < FIELDOBJECT_COLL_MAP_XSIZE;j++)
		{
			FieldObjectCollMap[j][i] = 0;
		}

	SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(founique);
	if(ref >= 0)
	{
		SI32 tilexsize = pclFieldObjectManager->pclInfo[ref]->TileXsize;
		SI32 tileysize = pclFieldObjectManager->pclInfo[ref]->TileYsize;

		SI32 tilecenterx = pclFieldObjectManager->pclInfo[ref]->TileCenterX;
		SI32 tilecentery = pclFieldObjectManager->pclInfo[ref]->TileCenterY;

		for(i = 0;i < tileysize;i++)
			for(j = 0;j < tilexsize;j++)
			{
				FieldObjectCollMap	[j - tilexsize + 1 + tilecenterx + FIELDOBJECT_COLL_MAP_XSIZE/2]
									[i - tileysize + 1 + tilecentery + FIELDOBJECT_COLL_MAP_YSIZE/2] = 1;
			}
	}



	//------------------------------------
	// Surface의 중심 좌표를 구한다. 
	//------------------------------------
	SI32 x = FOSURFACE_XSIZE / 2 ;
	SI32 y = FOSURFACE_YSIZE / 2 ;

	//-------------------------------------------------
	// 중심 좌표를 기준으로 5*5칸의 타일을 그린다. 
	//-------------------------------------------------
	SI32 centerx, centery;

	//--------------------------------------------
	// Surface를 깨끗하게 지운다. 
	//--------------------------------------------
	GP.ClearSurface(pFOSurface);

	if(GP.LockSurface(pFOSurface) == TRUE)
	{

		//--------------------------------------------
		// 바닥 타일을 그린다. 
		//--------------------------------------------
		for(i = -FoTileSize;i <= FoTileSize;i++)
			for(j = -FoTileSize;j <= FoTileSize;j++)
			{
				centerx = j*MAP_TILE_XSIZE_HALF
					-i*MAP_TILE_XSIZE_HALF
					+ x;
				
				centery = i*MAP_TILE_YSIZE_HALF 
					+j*MAP_TILE_YSIZE_HALF
					+ y;
				
				SI32 startx	=	centerx-MAP_TILE_XSIZE_HALF;
				SI32 starty	=	centery-MAP_TILE_YSIZE_HALF;

				SI32 font;
				
				//-----------------------------------------------------
				// 충돌 지역인지 여부에 따라 출력하는 이미지가 다르다. 
				//-----------------------------------------------------
				if( FieldObjectCollMap[j + FIELDOBJECT_COLL_MAP_XSIZE/2][i + FIELDOBJECT_COLL_MAP_YSIZE/2])
				{
					font = 2;
				}
				else
				{
					font = ((j+FoTileSize) + (i+FoTileSize) * (FoTileSize*2+1) ) % 2;
				}
				

				GP.PutSpr(&FloorSpr, startx, starty, font);
				
				//DrawFloorTile(startx, starty, MAP_TILE_XSIZE, MAP_TILE_YSIZE, 30);
				
			}

			
			//---------------------------------------------
			// 타일 위에 Field Object를 그린다. 
			//---------------------------------------------
			ref = pclFieldObjectManager->FindRefFromUniqueMinus(founique);
			if(ref >= 0)
			{
				
				TSpr* pspr = pclFieldObjectManager->pclInfo[ref]->GetSpr(pclFieldObjectManager, GetFrame());
				
				// Field Object 맨 하단의 위치를 구한다. 
				SI32 tilexsize = pclFieldObjectManager->pclInfo[ref]->TileXsize;
				SI32 tileysize = pclFieldObjectManager->pclInfo[ref]->TileYsize;
				
				SI32 bottomx = pclFieldObjectManager->pclInfo[ref]->TileCenterX;
				SI32 bottomy = pclFieldObjectManager->pclInfo[ref]->TileCenterY;
				
				centerx = bottomx*MAP_TILE_XSIZE_HALF - bottomy*MAP_TILE_XSIZE_HALF + x;
				
				centery = bottomy*MAP_TILE_YSIZE_HALF + bottomx*MAP_TILE_YSIZE_HALF	+ y;
		
				cltMapClient* pclmap = (cltMapClient*)pclMapManager->pclCurrentMap;
				pclmap->DrawFieldObject(bottomx, bottomy, centerx, centery, founique);
			}
			
			GP.UnlockSurface(pFOSurface);
	}
	


	//----------------------------------------------
	// 그려진 이미지를 다이얼로그 박스로 옮긴다. 
	//----------------------------------------------
	HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_FIELDOBJECTSHAPE);

	RECT rect;
	RECT rect2;
	GetClientRect( hwnd, &rect );
	rect2 = rect;
	
	// 얻어온 클라이언트 좌표 2개를 스크린 좌표로 변경한다.
	ClientToScreen( hwnd, (POINT*)&rect );
	ClientToScreen( hwnd, (POINT*)&rect + 1);

	RECT trect;
	trect.left		= max(0, x - (FoTileSize*2+3) * (MAP_TILE_XSIZE/2));
	trect.right		= min(FOSURFACE_XSIZE-1, x + (FoTileSize*2+3) * (MAP_TILE_XSIZE/2));
	trect.top		= max(0, y - (FoTileSize*2+3) * (MAP_TILE_YSIZE/2));
	trect.bottom	= min(FOSURFACE_YSIZE-1, y + (FoTileSize*2+3) * (MAP_TILE_YSIZE/2));

	pFOSurface2->Blt(&rect2, pFOSurface, &trect, DDBLT_WAIT, NULL);
	
	HDC srchdc;
	HDC desthdc = GetDC(hwnd);
	if(desthdc )
	{
		if(pFOSurface2->GetDC(&srchdc) == DD_OK)
		{
			
			BitBlt(desthdc, 0, 0, 
				rect.right - rect.left, rect.bottom - rect.top, 
				srchdc, 0, 0, SRCCOPY);

			pFOSurface2->ReleaseDC(srchdc);
			
		}
		
		ReleaseDC(hwnd, desthdc);
	}
}

void cltMapEdit::DrawFloorTile(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, UI08 color)
{
	GP.Box(startx, starty, startx + xsize-1, starty + ysize - 1, color);
}
