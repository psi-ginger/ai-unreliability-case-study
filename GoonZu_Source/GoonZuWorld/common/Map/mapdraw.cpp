#include "..\Client\Client.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

//------------------------------------
// CommonLogic
//------------------------------------
#include "..\..\CommonLogic\MagicMgr\MagicMgr.h"

//------------------------------------
// Common
//------------------------------------
#include "Bullet\Bullet.h"
#include "Smoke\Smoke.h"
#include "map.h"
#include "..\Resource.h"
#include "..\Order\Order.h"
#include "Char\CharManager\CharManager.h"
#include "Map\FieldObject\FieldObject.h"

//-----------------------------------
// Client
//-----------------------------------
//#include "..\..\Client\ColorTable\ColorTable.h"
#include "..\..\Client\DrawSequence\DrawSequence.h"
#include "Help\Help.h"
#include "..\..\Client\Ninterface\NMinimapdlg\Nminimapdlg.h"

#include "../../mapEdit/mapedit.h"
extern cltCommonLogic* pclClient;

extern	RECT				ScreenRect;							// 지도가 표시되는 영역 

// 출력해야 할 중심 위치를 구한다. 
void cltMapClient::GetDrawCenterXY(SI32 x, SI32 y, SI32* pcenterx, SI32* pcentery)
{

	*pcenterx =		   - VirtualMapX * MAP_TILE_XSIZE_HALF
					   + VirtualMapY * MAP_TILE_XSIZE_HALF
					   +x*MAP_TILE_XSIZE_HALF
					   -y*MAP_TILE_XSIZE_HALF
					   + ScreenRect.left+(ScreenRect.right-ScreenRect.left+1)/2;
	
	*pcentery =		   -VirtualMapY*MAP_TILE_YSIZE_HALF
					   -VirtualMapX*MAP_TILE_YSIZE_HALF
					   +ScreenRect.top+(ScreenRect.bottom-ScreenRect.top+1)/2
					   +y*MAP_TILE_YSIZE_HALF 
					   +x*MAP_TILE_YSIZE_HALF ;


}

// 지도의 충돌 영역을 표시한다. 
void cltMapClient::DrawMapCollInfo()
{
	if(bCollDrawSwitch == false)return;

	SI32 i, j;
	SI32 centerx, centery;
	int clipx1, clipy1, clipx2, clipy2;

	cltClient* pclclient = (cltClient*)pclClient;

	GP.GetClipArea(clipx1, clipy1, clipx2, clipy2 );
	GP.SetClipArea(0, 0, pclclient->siClientScreenXsize - 1, pclclient->siClientScreenYsize - 1 );

	   for(i = VirtualMapY - pclclient->siClientScreenHalfSize;i < VirtualMapY + pclclient->siClientScreenHalfSize;i++)
		   for(j = VirtualMapX - pclclient->siClientScreenHalfSize;j < VirtualMapX + pclclient->siClientScreenHalfSize;j++)
		   {
			   
			   // 시작점으로 간다.
			   if(IsInMapArea(j, i) == true)
			   {
				   // 출력해야 할 중심 위치를 구한다. 
				   GetDrawCenterXY(j, i, &centerx, &centery);

				   SI32 collinfo = CalcCollInfo(j, i);
				   // 육지속성은 표시하지 않는다. 
				   if(collinfo & BLOCK_LAND)
				   {
					   collinfo -= BLOCK_LAND;
				   }

				   if(collinfo)
				   {
					   SHORT _screenx, _screeny;
					   GetCenterXY(j, i, _screenx, _screeny);
					   
					   if(collinfo& BLOCK_CUSTOM)
					   {
						   GP.FillBox(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, 100,FALSE);
					   }
					   else if(collinfo& BLOCK_EVENT1)
					   {
						   GP.FillBox(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, 40,FALSE);
					   }
					   else if(collinfo& BLOCK_EVENT2)
					   {
						   GP.FillBox(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, 228,FALSE);
					   }
	   				   else if(collinfo& BLOCK_EVENT3)
					   {
						   GP.FillBox(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, 98,FALSE);
						   //g_graphics.FillBox16C(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, MakeRGB16(0,0,0));
					   }
					   else if(collinfo& BLOCK_EVENT4)
					   {
						   GP.FillBox(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, 130,FALSE);
						   //g_graphics.FillBox16C(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, MakeRGB16(0,0,0));
					   }
					   else if(collinfo& BLOCK_EVENT5)
					   {
						   GP.FillBox(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, 180,FALSE);
						   //g_graphics.FillBox16C(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, MakeRGB16(0,0,0));
					   }
					   else if(collinfo& BLOCK_EVENT6)
					   {
						   GP.FillBox(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, 200,FALSE);
						   //g_graphics.FillBox16C(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, MakeRGB16(0,0,0));
					   }
					   else if(collinfo& BLOCK_EVENT7)
					   {
						   GP.FillBox(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, 230,FALSE);
						   //g_graphics.FillBox16C(_screenx - 15, _screeny - 15, _screenx + 15, _screeny + 15, MakeRGB16(0,0,0));
					   }
				

					   GP.FillBox(_screenx - 10, _screeny - 10, _screenx + 10, _screeny + 10, 0,FALSE);
				   }
			   }
			   
		   }

	GP.SetClipArea(clipx1, clipy1, clipx2, clipy2 );
}


// 현재 마우스가 위치한 타일의 위치를 표시한다. 
void cltMapClient::DrawCurrentTilePos(SI32 mapx, SI32 mapy)
{
	if(IsInMapArea(mapx, mapy) == false )return ;

	// 출력해야 할 중심 위치를 구한다. 
	SI32 centerx, centery;
	GetDrawCenterXY(mapx, mapy, &centerx, &centery);
	
	UI08 color = 200;

	GP.Line(centerx-MAP_TILE_XSIZE_HALF, centery, centerx, centery -MAP_TILE_YSIZE_HALF, color ,FALSE);
	GP.Line(centerx, centery-MAP_TILE_YSIZE_HALF, centerx+MAP_TILE_XSIZE_HALF, centery, color,FALSE );

	GP.Line(centerx-MAP_TILE_XSIZE_HALF, centery, centerx, centery +MAP_TILE_YSIZE_HALF, color,FALSE );
	GP.Line(centerx, centery+MAP_TILE_YSIZE_HALF, centerx+MAP_TILE_XSIZE_HALF, centery, color ,FALSE);

	GP.FillBox(centerx-5,  centery-5, centerx+5,  centery+5, color,FALSE);






}


// 지도 타일을 그린다. 
BOOL cltMapClient::DrawMap()
{
 
	int i, j;
	SI32 centerx, centery;
	int clipx1, clipy1, clipx2, clipy2;
	SI32 pgroup = 0;

	cltClient* pclclient = (cltClient*)pclClient;

	
	if(GP.LockSurface(pclclient->lpScreenMapBuffer )==FALSE ) return FALSE;
	
	GP.GetClipArea(clipx1, clipy1, clipx2, clipy2 );
	GP.SetClipArea(0, 0, pclclient->siClientScreenXsize-1, pclclient->siClientScreenYsize-1 );

	// 맵 에디터는 강제로 찍는다. 
	if(pclClient->GameMode == GAMEMODE_MAPEDIT)
	{
		bMapDrawSwitch = true;
	}

	
	// 북쪽으로 스크롤
	if((VirtualMapX  + 1 == siOldVirtualMapX) && (VirtualMapY  + 1 == siOldVirtualMapY))
	{
		Scroll_North(pclclient);
	}
	// 남쪽으로 스크롤 
	else if((VirtualMapX  - 1 == siOldVirtualMapX) && (VirtualMapY  - 1 == siOldVirtualMapY))
	{
		Scroll_South(pclclient);
	}
	// 동쪽으로 스크롤 
	else if((VirtualMapX  - 1 == siOldVirtualMapX) && (VirtualMapY  + 1 == siOldVirtualMapY))
	{
		Scroll_East(pclclient);
	}
	// 서쪽으로 스크롤 
	else if((VirtualMapX  + 1 == siOldVirtualMapX) && (VirtualMapY  - 1 == siOldVirtualMapY))
	{
		Scroll_West(pclclient);
	}
	// 북동쪽으로 스크롤
	else if((VirtualMapX   == siOldVirtualMapX) && (VirtualMapY  + 1 == siOldVirtualMapY))
	{
		Scroll_NorthEast(pclclient);
	}
	// 북서쪽으로 스크롤
	else if((VirtualMapX + 1 == siOldVirtualMapX) && (VirtualMapY  == siOldVirtualMapY))
	{
		Scroll_NorthWest(pclclient);
	}
	// 남서쪽으로 스크롤
	else if((VirtualMapX  == siOldVirtualMapX) && (VirtualMapY - 1 == siOldVirtualMapY))
	{
		Scroll_SouthWest(pclclient);
	}
	// 남동쪽으로 스크롤
	else if((VirtualMapX - 1 == siOldVirtualMapX) && (VirtualMapY  == siOldVirtualMapY))
	{
		Scroll_SouthEast(pclclient);
	}
	// 완전히 다시 그리기. 
	else if(bMapDrawSwitch == true)
	{
		if(VirtualMapX < pclclient->siClientScreenHalfSize || VirtualMapX > MH.MapXsize - pclclient->siClientScreenHalfSize
		|| VirtualMapY < pclclient->siClientScreenHalfSize || VirtualMapY > MH.MapYsize - pclclient->siClientScreenHalfSize)
		{
			GP.FillBox(0, 0, pclclient->siClientScreenXsize-1, pclclient->siClientScreenYsize-1, (UI08)0,TRUE);
		}
		   

	   for(i = VirtualMapY - pclclient->siClientScreenHalfSize;i < VirtualMapY + pclclient->siClientScreenHalfSize;i++)
		   for(j = VirtualMapX - pclclient->siClientScreenHalfSize;j < VirtualMapX + pclclient->siClientScreenHalfSize;j++)
		   {
			   // 시작점으로 간다.
			   if(IsInMapArea(j, i) == false)continue;

			   // 출력해야 할 중심 위치를 구한다. 
			   GetDrawCenterXY(j, i, &centerx, &centery);

			   if(pclLayer[1]->GetDrawInfo_Unique(j, i) != 90)
			   {
				   DrawLayer(centerx, centery, 1, j, i);
			   }
			   else
			   {
				   DrawLayer(centerx, centery, 0, j, i);
			   }
		}
	}

	bMapDrawSwitch = false;

	GP.UnlockSurface(pclclient->lpScreenMapBuffer );

	GP.SetClipArea(clipx1, clipy1, clipx2, clipy2 );

	// 가장 최근의 VirtualX, Y를 기억한다. 
	siOldVirtualMapX = VirtualMapX;
	siOldVirtualMapY = VirtualMapY;
		
	return TRUE;
}


// 땅에 떨어진 아이템을 그린다. 
void cltMapClient::DrawLandItem()
{
	SI32 i, j;
	SI32 centerx, centery;

	cltClient* pclclient = (cltClient*)pclClient;

	for(i=(VirtualMapY-pclclient->siClientScreenHalfSize);i<(VirtualMapY+pclclient->siClientScreenHalfSize);i++)
		for(j=(VirtualMapX-pclclient->siClientScreenHalfSize);j<(VirtualMapX+pclclient->siClientScreenHalfSize);j++)
		{
			if(IsInMapArea(j, i) == false)continue;

			cltItemMapInfo* pclinfo = &pclItemMap[j+ YOff[i]];

			if(pclinfo->uiItemUnique)
			{
				SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(pclinfo->uiItemUnique);
				if(ref > 0)
				{
				   // 출력해야 할 중심 위치를 구한다. 
				   GetDrawCenterXY(j, i, &centerx, &centery);

					TSpr* pSpr = pclClient->GetGlobalSpr(GIMG_FIELDITEM);

					if(pSpr)
					{
						//cyj 바닥에 떨어지면 모두 아이템주머니로 보이도록 수정
						//SI32 fnt = pclClient->pclItemManager->pclItemInfo[ref]->siSmallImageIndex;
						SI32 fnt = 0;
						GP.PutSpr(pSpr, centerx - (pSpr->GetXSize() /2), centery - (pSpr->GetYSize() /2), fnt);
					}
				}
			}

		}
}

// 캐릭터를 지도에 그린다. 
void cltMapClient::DrawCharInMap(SI32 x, SI32 y)
{
	SI32 id;
	SI32 length;
	cltClient* pclclient = (cltClient*)pclClient;

	SI32 index = 0;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		if(pclClient->pclCM->IsValidID(id))
		{
			length = max( TABS(pclCM->CR[id]->GetX() - x), TABS(pclCM->CR[id]->GetY() - y));

			if( true == g_SoundOnOffDlg.m_bAloneDraw )
			{
				if( id == 1 || 
					(pclCM->CR[id]->pclKI != NULL && pclCM->CR[id]->pclKI->IsAtb(ATB_MONSTER)) ||
					(pclCM->CR[id]->pclKI != NULL && pclCM->CR[id]->pclKI->IsAtb(ATB_SYSTEMNPC)) ||
					(pclCM->CR[id]->pclKI != NULL && pclCM->CR[id]->pclKI->IsAtb(ATB_DEFAULTCASTLE)) )
				{
					// 너무 먼 거리에 있는 캐릭터는 출력하지 않는다. 
					if(length > 25)
						continue;
					if(GetFogMap(pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY()) >= FOG_HALF)
						continue;

					// 다른 사람 소환수도 안 그려준다
					if( id != 1 && pclCM->CR[id]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
						continue;

					if(pclCM->CR[id]->GetCurrentOrder() == ORDER_DEAD)
					{
						((cltCharClient*)(pclCM->CR[id]))->Draw();
					}
					else
					{
						cltDrawObjData cldata(DRAWOBJ_TYPE_CHAR, id);
						pclclient->pclDrawSequence->Push(pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY(), 1, 1, &cldata);
					}
				}
			}
			else
			{
				// 너무 먼 거리에 있는 캐릭터는 출력하지 않는다. 
				if(length > 25)continue;
				if(GetFogMap(pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY()) >= FOG_HALF)continue;

				if(pclCM->CR[id]->GetCurrentOrder() == ORDER_DEAD)
				{
					((cltCharClient*)(pclCM->CR[id]))->Draw();
				}
				else
				{
					cltDrawObjData cldata(DRAWOBJ_TYPE_CHAR, id);
					pclclient->pclDrawSequence->Push(pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY(), 1, 1, &cldata);
				}
			}
		}
	}
}

// 캐릭터 이름을 표시한다. 
void cltMapClient::DrawCharName()
{
	SI32 i, j;
	SI32 id;

	cltClient* pclclient = (cltClient*)pclClient;

	for(i=(VirtualMapY-pclclient->siClientScreenHalfSize);i<=(VirtualMapY+pclclient->siClientScreenHalfSize);i++)
		for(j=(VirtualMapX-pclclient->siClientScreenHalfSize);j<=(VirtualMapX+pclclient->siClientScreenHalfSize);j++)
		{
			if(IsInMapArea(j, i) == false)continue;

			id = GetIDMap(j, i);
			if(id)
			{
				pclCM->CR[id]->DrawName();
			}
		}
		// 여기까지 for
}

// 지도 타일을 제외한 각 요소를 그린다. 
BOOL cltMapClient::DrawObj()
{
	SHORT i, j;
	cltClient* pclclient = (cltClient*)pclClient;


	// 캐릭터를 지도에 그린다. 
	DrawCharInMap(VirtualMapX, VirtualMapY);

		
		for(i=(VirtualMapY-pclclient->siClientScreenHalfSize-6);i<=(VirtualMapY+pclclient->siClientScreenHalfSize+6);i++)
			for(j=(VirtualMapX-pclclient->siClientScreenHalfSize-6);j<=(VirtualMapX+pclclient->siClientScreenHalfSize+6);j++)
			{
				if(IsInMapArea(j, i) == false)continue;
				
				SI32 fieldobject = GetFieldObject_Unique(j, i);
				if(fieldobject && GetFieldObjectLengthToBottom(j, i) == 0 && GetFieldObjectLengthToRight(j, i) == 0)
				{
					SHORT cx, cy;
					GetCenterXYByMapCenterXY(j, i, cx, cy);

					SI32 ref	= pclClient->pclFieldObjectManager->FindRefFromUniqueMinus(fieldobject);
					if(ref >= 0)
					{
						SI32 type	= pclClient->pclFieldObjectManager->pclInfo[ref]->dwType;

						//사이즈등의 상세 정보를 하드에서 읽어온다.
						pclClient->pclFieldObjectManager->pclInfo[ref]->LoadDetailInfo();

						// 바닥에 찍는 오브젝이면 
						if(pclClient->pclFieldObjectManager->pclTypeInfo[type]->dwAtb & FOATB_FLOOR)
						{
							DrawFieldObject(j, i, cx, cy, fieldobject);
						}
						else
						{

							cltFieldObjDrawData clfodata;
							
							clfodata.x			= j;
							clfodata.y			= i;
							clfodata.cx			= cx;
							clfodata.cy			= cy;
							clfodata.founique	= fieldobject;
							clfodata.type		= type;
							
							cltDrawObjData cldata(DRAWOBJ_TYPE_FIELDOBJ, &clfodata);
							pclclient->pclDrawSequence->Push(
								j-pclClient->pclFieldObjectManager->pclInfo[ref]->TileXsize + 1, 
								i-pclClient->pclFieldObjectManager->pclInfo[ref]->TileYsize + 1, 
								pclClient->pclFieldObjectManager->pclInfo[ref]->TileXsize, 
								pclClient->pclFieldObjectManager->pclInfo[ref]->TileYsize, 
								&cldata);

						}
					}
				}

				if(pclClient->GameMode == GAMEMODE_MAPEDIT)
				{
					//	if(GetIDMap(j, i))
					{
						SHORT _screenx, _screeny;
						
						GetCenterXY(j, i, _screenx, _screeny);
						GP.FillBox(_screenx - 1, _screeny - 1, _screenx + 1, _screeny + 1, 16,FALSE);
					}
				}

				
			}
			// 여기까지 for

				
	// 땅에 떨어진 아이템을 그린다. 
	DrawLandItem();
	
	// 미니맵에 표시하여 줄 정보를 초기화 한다. 캐릭터를 그려주면서 다시 채워진다.
	((CNMiniMapDlg*)pclclient->m_pDialog[NMINIMAP_DLG])->ClearMinimapNPC();

	cltDrawObjData *pcldata = pclclient->pclDrawSequence->Pop();	
	while( pcldata )
	{
		switch(pcldata->siType)
		{
		case DRAWOBJ_TYPE_CHAR:
			((cltCharClient*)(pclCM->CR[pcldata->siID]))->Draw();
			break;

		case DRAWOBJ_TYPE_FIELDOBJ:
			DrawFieldObject(pcldata->clFODrawData.x, pcldata->clFODrawData.y, pcldata->clFODrawData.cx, pcldata->clFODrawData.cy, pcldata->clFODrawData.founique);

			// 마을이면 이름도 찍어준다. 
			if(pcldata->clFODrawData.type == FOTYPE_VILLAGE)
			{
				SI32 villageunique = GetFieldObjectPara(pcldata->clFODrawData.x, pcldata->clFODrawData.y);
				
				SHORT centerx, centery;
				GetRefXY(pcldata->clFODrawData.x, pcldata->clFODrawData.y, centerx, centery);
				
				if(villageunique >=0 && pclClient->pclVillageManager->pclVillageInfo[villageunique])
				{
					pclClient->pclVillageManager->pclVillageInfo[villageunique]->Draw(centerx, centery);


					//------------------------------------------
					// 점령된 상태라면 불도 그려준다. 
					//------------------------------------------
					if(pclClient->pclVillageManager->pclVillageInfo[villageunique]->bEnemyCaptureSwitch)
					{
						DrawFire(pcldata->clFODrawData.x,	pcldata->clFODrawData.y, pcldata->clFODrawData.cx, pcldata->clFODrawData.cy - 10, 0);
						DrawFire(pcldata->clFODrawData.x,	pcldata->clFODrawData.y, pcldata->clFODrawData.cx - 40, pcldata->clFODrawData.cy- 20, 1);
						DrawFire(pcldata->clFODrawData.x,	pcldata->clFODrawData.y, pcldata->clFODrawData.cx + 40, pcldata->clFODrawData.cy-20, 2);
						DrawFire(pcldata->clFODrawData.x,	pcldata->clFODrawData.y, pcldata->clFODrawData.cx - 60, pcldata->clFODrawData.cy-60, 3);


					}
				}


			}
			break;
		default:
			break;
		}

		pcldata = pclclient->pclDrawSequence->Pop();
	}

	// 2003.10.29
	// 연기를 그린다음에 총알을 그리는 것으로 순서 바꿈

	
	// 연기를 그린다. 
	pclClient->pclSmokeManager->Draw();

	// 총알을 그린다. 
	pclClient->pclBulletManager->Draw();
	
	// 마법을 그린다. 
	pclClient->pclMagicManager->Draw();
	

	// 지도의 충돌 영역을 표시한다. 
	DrawMapCollInfo();
	
	// 현재 마우스가 위치한 타일의 위치와 선택한 OBJECT를 그려준다.
	if(pclClient->GameMode == GAMEMODE_MAPEDIT)
	{
		SI32 siX = pclclient->pclUnderMouseObject->MapX;
		SI32 siY = pclclient->pclUnderMouseObject->MapY;
		
		switch(((cltMapEdit *)pclClient)->CurrentMode)
		{
		case MAPEDIT_MODE_FIELDOBJETBRUSH:
		case MAPEDIT_MODE_FIELDOBJET:
			if(IsInMapArea(siX, siY))
			{
				SI32 siUnique	= ((cltMapEdit *)pclClient)->SelectedFieldObjectUnique;
				SI32 ref		= pclFieldObjectManager->FindRefFromUniqueMinus(siUnique);
				if(ref>0)
				{
					//SI32 tilexsize	= pclFieldObjectManager->pclInfo[ref]->TileXsize;
					//SI32 tileysize	= pclFieldObjectManager->pclInfo[ref]->TileYsize;
					SI32 tilecenterx = pclFieldObjectManager->pclInfo[ref]->TileCenterX;
					SI32 tilecentery = pclFieldObjectManager->pclInfo[ref]->TileCenterY;
					
					SI32 siDrawX = siX + tilecenterx;
					SI32 siDrawY = siY + tilecentery;

					SHORT cx, cy;
					GetCenterXYByMapCenterXY(siDrawX, siDrawY, cx, cy);

					DrawFieldObject(siDrawX, siDrawY, cx, cy, siUnique, TRANSPARENT_MODE_SCREEN);
				}
				
				DrawCurrentTilePos(siX, siY);
			}
			break;
		case MAPEDIT_MODE_CUSTOMCOLL:
			{
				POINT mousePoint;
				GetCursorPos( &mousePoint );
				ScreenToClient( ((cltMapEdit *)pclClient)->GetHwnd(), &mousePoint );

				SI32 _screenx = mousePoint.x + ((cltMapEdit *)pclClient)->GetDrawStartX();
				SI32 _screeny = mousePoint.y + 16;

				SI32 CurrentCustomCollision = ((cltMapEdit *)pclClient)->CurrentCustomCollision;
				UI08 uiCollisionBrush = ((cltMapEdit *)pclClient)->uiCollisionBrush;
				uiCollisionBrush--;
				if( uiCollisionBrush <= 0 || uiCollisionBrush >= MAX_COLLISION_BRUSH_NUM ) uiCollisionBrush = 1;

				UI08 uiColor = 0;

				if( CurrentCustomCollision  & BLOCK_CUSTOM)
				{
					uiColor = 100;
				}
				else if( CurrentCustomCollision & BLOCK_EVENT2)
				{
					uiColor = 228;
				}
				else if( CurrentCustomCollision & BLOCK_EVENT3)
				{
					uiColor = 98;
				}
				else if( CurrentCustomCollision & BLOCK_EVENT4)
				{
					uiColor = 130;
				}
				else if( CurrentCustomCollision & BLOCK_EVENT5)
				{
					uiColor = 180;
				}
				else if( CurrentCustomCollision & BLOCK_EVENT6)
				{
					uiColor = 200;
				}
				else if( CurrentCustomCollision & BLOCK_EVENT7)
				{
					uiColor = 230;
				}

				SI32 startx = _screenx - MAP_TILE_XSIZE_HALF * uiCollisionBrush;
				SI32 starty = _screeny - MAP_TILE_YSIZE_HALF * uiCollisionBrush;
				SI32 width = MAP_TILE_XSIZE * uiCollisionBrush;
				SI32 height = MAP_TILE_YSIZE * uiCollisionBrush;
				GP.Box( startx, starty, startx + width, starty + height, uiColor);
				GP.Box( startx + 1, starty + 1, startx + width - 1, starty + height -1 , uiColor);

				GP.Line( startx, _screeny, _screenx, starty,  40, false );
				GP.Line( _screenx, starty, startx + width, _screeny, 40, false );
				GP.Line( startx + width, _screeny, _screenx, starty + height, 40, false );
				GP.Line( _screenx, starty + height, startx, _screeny, 40, false );

				GP.Box(_screenx - 10 * uiCollisionBrush, _screeny - 10 * uiCollisionBrush, _screenx + 10 * uiCollisionBrush, _screeny + 10 * uiCollisionBrush, 0);       
			}
			break;

		}
	}

	PopEffect();

			
	return TRUE;
}

// 지형 정보 데이터를 보여준다. 
void cltMapClient::SetGeoInfo(HDC hdc)
{
	SI32 i, j;
	SI32 centerx, centery;
	TCHAR buffer[256];

	cltClient* pclclient = (cltClient*)pclClient;

	// 폰트를 설정한다. 
	HFONT hFont = g_FontManager.SetFont();
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

	SetBkMode(hdc, TRANSPARENT);

	// 지형정보의 위치 변수를 초기화한다. 
	pclMapGeoInfo->InitGeoPosInfo();

   for(i = VirtualMapY - pclclient->siClientScreenHalfSize;i < VirtualMapY + pclclient->siClientScreenHalfSize;i++)
   {
	   for(j = VirtualMapX - pclclient->siClientScreenHalfSize;j < VirtualMapX + pclclient->siClientScreenHalfSize;j++)
	   {
		   // 시작점으로 간다.
		   if(IsInMapArea(j, i) == true)
		   {
			   // 출력해야 할 중심 위치를 구한다. 
			   GetDrawCenterXY(j, i, &centerx, &centery);
			   
			   SI32 data = pclMapGeoInfo->GetGeoMap(j, i);
			   if(data )
			   {
				   // 지형정보 위치를 업데이트 한다. 
				   pclMapGeoInfo->UpdateGeoPos(j, i, data);

				   // 맵에디터인경우에만 표시한다. 
				   if(pclClient->GameMode == GAMEMODE_MAPEDIT)
				   {
					   StringCchPrintf(buffer, 256, TEXT("%d"), data);
					   
					   // 문자열의 사이즈를 구한다. 
					   SIZE size;
					   GetTextExtentPoint32(hdc, buffer, lstrlen(buffer), &size);
					   
					   TextOut(hdc, centerx-size.cx/2, centery-size.cy/2, buffer, lstrlen(buffer));
				   }
				   
			   }
		   }
	   }
   }


/////////////////////////

	if(pclClient->GameMode == GAMEMODE_MAPEDIT)
	{
		StringCchPrintf(buffer, 256, TEXT("%d , %d"), pclclient->pclUnderMouseObject->MapX ,pclclient->pclUnderMouseObject->MapY );
		TextOut(hdc, 500, 700, buffer, lstrlen(buffer));
		SelectObject(hdc, hOldFont);
	}

}

// 지형 정보 데이터를 보여준다. 
void cltMapClient::DrawGeoInfo(LPDIRECTDRAWSURFACE7 lpsurface, cltHelpManager* pclhelpmanager)
{
	SI32 i;
	SI32 centerx, centery;

   // 지형정보를 화면에 출력한다. 
   for(i = 0;i < MAX_GEO_NUMBER;i++)
   {
	   if(pclMapGeoInfo->siGeoPosAccount[i] > 0)
	   {
		   SI32 x = pclMapGeoInfo->siGeoPosX[i] /  pclMapGeoInfo->siGeoPosAccount[i];
		   SI32 y = pclMapGeoInfo->siGeoPosY[i] /  pclMapGeoInfo->siGeoPosAccount[i];
		   TCHAR* text = pclMapGeoInfo->szGetText[i];
		   
		   
		   if(IsInScreenTile(x, y) == TRUE)
		   {
			   // 출력해야 할 중심 위치를 구한다. 
			   GetDrawCenterXY(x, y, &centerx, &centery);

			   SI32 helpref = pclhelpmanager->GetHelp(text);
			   if(helpref >= 0)
			   {
				   pclhelpmanager->SetPos(helpref, centerx, centery);

			   }
			   else
			   {
					pclhelpmanager->Add(text, centerx, centery);
			   }

			   pclhelpmanager->Draw(lpsurface);

		   }
	   }
	   
   }
}

// 불을 그려준다. 
void cltMapClient::DrawFire(SHORT mapx, SHORT mapy, SHORT cx, SHORT cy, SI32 file)
{
	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_FIRE1 + file);

	if(pspr == NULL)return ;

	//-------------------------
	// 출력 시작 위치를 찾는다.
	//-------------------------
	SI32 startx = cx - (pspr->GetXSize()/2)					;//+ pclFieldObjectManager->pclInfo[ref]->IndentX;
	SI32 starty = cy + MAP_TILE_YSIZE_HALF - pspr->GetYSize()	;//+ pclFieldObjectManager->pclInfo[ref]->IndentY;

	SI32 font = (pclClient->CurrentClock / 100) % pspr->GetImageNum();

	GP.PutSprLight1(pspr, startx, starty, font);

}

void cltMapClient::DrawFieldObject(SHORT mapx, SHORT mapy, SHORT cx, SHORT cy, SI32 fieldobjectunique, SI32 siForceTransMode)
{

	SI32 ref = pclFieldObjectManager->FindRefFromUniqueMinus(fieldobjectunique);
	if(ref < 0)return ;

	SI32 file = 0;
    SI32 font = 0;

	TSpr* pspr = pclFieldObjectManager->pclInfo[ref]->GetSpr(pclFieldObjectManager, pclClient->GetFrame());
	if(pspr == NULL)
	{
		return;
	}

	//-----------------------------------------------
	// 출력의 근거지 타일의 centerx, centery를 구한다. 
	//------------------------------------------------
	SI32 tilex = mapx;
	SI32 tiley = mapy;

	//-------------------------
	// 출력 시작 위치를 찾는다.
	//-------------------------
	SI32 startx = cx - (pspr->GetXSize()/2)					+ pclFieldObjectManager->pclInfo[ref]->IndentX;
	SI32 starty = cy + MAP_TILE_YSIZE_HALF - pspr->GetYSize()	+ pclFieldObjectManager->pclInfo[ref]->IndentY;

	//------------------------------
	// 출력해야 할 이미지를 찾는다. 
	//------------------------------
/*	if ( ref == 511 )
	{
		int sss = 0 ;
	}*/
	SI32 aniframe = pclFieldObjectManager->pclInfo[ref]->AniFrame;
	if(aniframe == 0)aniframe = 1;

	SI32 anidelay = pclFieldObjectManager->pclInfo[ref]->AniDelay;
	if(anidelay == 0)anidelay = 1;

	//font =   ( (pclClient->GetFrame() + GetFieldObjectRandomValue(tilex, tiley)) / anidelay)  % aniframe;
	font =   ( ( max(0, pclClient->CurrentClock / 50) + GetFieldObjectRandomValue(tilex, tiley)) / anidelay)  % aniframe;

	SI32 maxfont = pspr->GetImageNum() ;
	if ( font >= maxfont )
		return;

	SI32 simode = pclFieldObjectManager->pclInfo[ref]->siTransMode;
	if(siForceTransMode)	simode = siForceTransMode;
	SI32 alphavalue = pclFieldObjectManager->pclInfo[ref]->siAlphaValue ;
	if ( alphavalue < 0 || alphavalue > 8 ) alphavalue = 4 ;
	
	//--------------------
	// 이미지를 출력한다. 
	//--------------------

	if(pspr->pImage)
	{
		SI32 xsize = pspr->GetXSize();
		SI32 ysize = pspr->GetYSize();

		if(GP.IsInClipArea(startx, starty, startx + xsize-1, starty + ysize-1) == TRUE)
		{
			SI32 mprmode = 0 ;
			switch(simode)
			{
			case TRANSPARENT_MODE_SCREEN: mprmode = PUT_SPR_SCREEN ; break;
			case TRANSPARENT_MODE_NORMAL: mprmode = PUT_SPR_SPR ; break;
			case TRANSPARENT_MODE_OLDTRANS:
			case TRANSPARENT_MODE_HALFTRANS:
			case TRANSPARENT_MODE_MULTIPLY: mprmode = PUT_SPR_MULTIPLY ; break;
			case TRANSPARENT_MODE_ADD: break;
			case TRANSPARENT_MODE_ALPHA: mprmode = PUT_SPR_ALPHA ; break ;
			default: mprmode = PUT_SPR_SPR ; break;
			}

			GP.PutMpr(pspr, startx, starty, mprmode, FALSE, font,alphavalue );

		}
		else
		{
			SI32 mprmode = 0 ;
			switch(simode)
			{
			case TRANSPARENT_MODE_SCREEN: mprmode = PUT_SPR_SCREEN ; break;
			case TRANSPARENT_MODE_NORMAL: mprmode = PUT_SPR_SPR ; break;
			case TRANSPARENT_MODE_OLDTRANS:
			case TRANSPARENT_MODE_HALFTRANS:
			case TRANSPARENT_MODE_MULTIPLY: mprmode = PUT_SPR_MULTIPLY ; break;
			case TRANSPARENT_MODE_ADD:
			case TRANSPARENT_MODE_ALPHA: mprmode = PUT_SPR_ALPHA ; break ;
			default: mprmode = PUT_SPR_SPR ; break;
			}
			
			GP.PutMpr(pspr, startx, starty, mprmode, TRUE , font,alphavalue);
		}
	}
	// 적절한 이미지를 로딩하지 못했다면 그 내용을 표시해준다. 
	else
	{
		TCHAR buffer[256];
		StringCchPrintf(buffer, 256, TEXT("Field Object is Empty:%d"), fieldobjectunique);
		pclClient->pclMessage->SetMsg(buffer);
	}


}


// 지도 요소를 그리는 함수들 
// 언덕을 그린다. 
BOOL cltMapClient::DrawLayer(SHORT centerx, SHORT centery, SI32 layer, SHORT x, SHORT y)
{

	SHORT startx, starty;
	SHORT ref, file, font;
	int sx1, sy1, sx2, sy2;

	if(pclLayer[layer] == NULL)return FALSE;
	
	startx=centerx-MAP_TILE_XSIZE_HALF;
	starty=centery-MAP_TILE_YSIZE_HALF;
	
	ref = pclLayer[layer]->GetDrawInfo_Ref(x, y) ;
	file = pclLayer[layer]->GetDrawInfo_File(x, y);
	font = pclLayer[layer]->GetDrawInfo_Font(x, y);

	if(ref < 0 || ref >= MAX_TILESET_NUMBER)return FALSE;
	if(pclTileManager->pclTileInfo[ref] == NULL)return FALSE;

	TSpr* pspr = pclTileManager->pclTileInfo[ref]->pSpr[file];
	if(pspr)
	{
		GP.FitToClipArea(startx, starty, startx+(pspr->GetXSize())-1, starty+(pspr->GetYSize())-1, 
			sx1, sy1, sx2, sy2);
	}

	DrawTile(ref, file, font, x, y, startx, starty);

	return TRUE;

}


void cltMapClient::DrawTile(SI32 ref, SI32 file, SI32 font, SI32 x, SI32 y, SI32 startx, SI32 starty)
{
	
	TSpr* pspr = pclTileManager->pclTileInfo[ref]->pSpr[file];

	if(pspr)
	{
		if(GP.IsInClipArea(startx, starty, startx + pspr->GetXSize() -1, starty + pspr->GetYSize() -1) == TRUE)
		{
			
			if( pclClient->GameMode == GAMEMODE_MAPEDIT )
				GP.PutMpr(pspr,  startx, starty, PUT_SPR_SPR, TRUE, font );
			else
				GP.PutMpr(pspr,  startx, starty, PUT_SPR_SPR, FALSE, font );
		}
		else
		{
			// LeeKH - 맵 에디트 모드만 일단 사용해 본다. - 클립핑 드로우모드
			if( pclClient->GameMode == GAMEMODE_MAPEDIT )
				GP.PutMpr(pspr,  startx, starty, PUT_SPR_SPR, TRUE, font );
		}
	}
}


// 북쪽으로 스크롤. 
void cltMapClient::Scroll_North(cltClient* pclclient)
{
	SI32 i, j;
	SI32 centerx, centery;


	SI32 xsize = pclclient->siClientScreenXsize;
	SI32 ysize = pclclient->siClientScreenYsize;


	SI32 linesize = xsize * 2;

	// 중복되는 지역을 복사한다. 
	for(i = (ysize - MAP_TILE_YSIZE) - 1;i >= 0;i--)
	{
		memcpy(GP.GetSurfacePtr(0, i+MAP_TILE_YSIZE), GP.GetSurfacePtr(0, i), linesize);
	}

	// 새로 드러난 공간을 지운다. 
	for(i = 0;i < MAP_TILE_YSIZE;i++)
	{
		memset(GP.GetSurfacePtr(0, i), 0, linesize);
	}


   for(i = VirtualMapY - pclclient->siClientScreenHalfSize;i < VirtualMapY + pclclient->siClientScreenHalfSize;i++)
	   for(j = VirtualMapX - pclclient->siClientScreenHalfSize;j < VirtualMapX + pclclient->siClientScreenHalfSize;j++)
	   {
		   // 시작점으로 간다.
		   if(IsInMapArea(j, i) == false)continue;
		   
		   // 출력해야 할 중심 위치를 구한다. 
		   GetDrawCenterXY(j, i, &centerx, &centery);

		   if((centery) > MAP_TILE_YSIZE )
		   {
			   continue;	// 새로 찍어야 할 부분만 찍는다. 
		   }
		   
		   if(pclLayer[1]->GetDrawInfo_Unique(j, i) != 90)
		   {
			   DrawLayer(centerx, centery, 1, j, i);
		   }
		   else
		   {
			   DrawLayer(centerx, centery, 0, j, i);
		   }
	   }
}


// 남쪽으로 스크롤. 
void cltMapClient::Scroll_South(cltClient* pclclient)
{
	SI32 i, j;
	SI32 centerx, centery;


	SI32 xsize = pclclient->siClientScreenXsize;
	SI32 ysize = pclclient->siClientScreenYsize;


	SI32 linesize = xsize * 2;

	// 중복되는 지역을 복사한다. 
	for(i = MAP_TILE_YSIZE;i < ysize;i++)
	{
		memcpy(GP.GetSurfacePtr(0, i - MAP_TILE_YSIZE), GP.GetSurfacePtr(0, i), linesize);
	}

	// 새로 드러난 공간을 지운다. 
	for(i = ysize - MAP_TILE_YSIZE;i < ysize;i++)
	{
		memset(GP.GetSurfacePtr(0, i), 0, linesize);
	}



   for(i = VirtualMapY - pclclient->siClientScreenHalfSize;i < VirtualMapY + pclclient->siClientScreenHalfSize;i++)
	   for(j = VirtualMapX - pclclient->siClientScreenHalfSize;j < VirtualMapX + pclclient->siClientScreenHalfSize;j++)
	   {
		   // 시작점으로 간다.
		   if(IsInMapArea(j, i) == false)continue;
		   
		   // 출력해야 할 중심 위치를 구한다. 
		   GetDrawCenterXY(j, i, &centerx, &centery);

		   if(centery < ysize - MAP_TILE_YSIZE )
		   {
			   continue;	// 새로 찍어야 할 부분만 찍는다. 
		   }
		   
		   if(pclLayer[1]->GetDrawInfo_Unique(j, i) != 90)
		   {
			   DrawLayer(centerx, centery, 1, j, i);
		   }
		   else
		   {
			   DrawLayer(centerx, centery, 0, j, i);
		   }
	   }

}


// 동쪽으로 스크롤. 
void cltMapClient::Scroll_East(cltClient* pclclient)
{
	SI32 i, j;
	SI32 centerx, centery;


	SI32 xsize = pclclient->siClientScreenXsize;
	SI32 ysize = pclclient->siClientScreenYsize;


	SI32 linesize	= (xsize - MAP_TILE_XSIZE) * 2;
	SI32 delsize	= MAP_TILE_XSIZE *2;


	// 중복되는 지역을 복사한다. 
	for(i = 0;i < ysize;i++)
	{
		memmove(GP.GetSurfacePtr(0, i), GP.GetSurfacePtr(MAP_TILE_XSIZE, i), linesize);
	}

	// 새로 드러난 공간을 지운다. 
	for(i = 0;i < ysize;i++)
	{
		memset(GP.GetSurfacePtr(xsize - MAP_TILE_XSIZE, i), 0, delsize);
	}




   for(i = VirtualMapY - pclclient->siClientScreenHalfSize;i < VirtualMapY + pclclient->siClientScreenHalfSize;i++)
	   for(j = VirtualMapX - pclclient->siClientScreenHalfSize;j < VirtualMapX + pclclient->siClientScreenHalfSize;j++)
	   {
		   // 시작점으로 간다.
		   if(IsInMapArea(j, i) == false)continue;
		   
		   // 출력해야 할 중심 위치를 구한다. 
		   GetDrawCenterXY(j, i, &centerx, &centery);

		   if(centerx < xsize - MAP_TILE_XSIZE )
		   {
			   continue;	// 새로 찍어야 할 부분만 찍는다. 
		   }
		   
		   if(pclLayer[1]->GetDrawInfo_Unique(j, i) != 90)
		   {
			   DrawLayer(centerx, centery, 1, j, i);
		   }
		   else
		   {
			   DrawLayer(centerx, centery, 0, j, i);
		   }
	   }

}

// 서쪽으로 스크롤. 
void cltMapClient::Scroll_West(cltClient* pclclient)
{
	SI32 i, j;
	SI32 centerx, centery;


	SI32 xsize = pclclient->siClientScreenXsize;
	SI32 ysize = pclclient->siClientScreenYsize;


	SI32 linesize	= (xsize - MAP_TILE_XSIZE) * 2;
	SI32 delsize	= MAP_TILE_XSIZE *2;

	// 중복되는 지역을 복사한다. 
	for(i = 0;i < ysize;i++)
	{
		memmove(GP.GetSurfacePtr(MAP_TILE_XSIZE, i), GP.GetSurfacePtr(0, i), linesize);
	}

	// 새로 드러난 공간을 지운다. 
	for(i = 0;i < ysize;i++)
	{
		memset(GP.GetSurfacePtr(0, i), 0, delsize);
	}




   for(i = VirtualMapY - pclclient->siClientScreenHalfSize;i < VirtualMapY + pclclient->siClientScreenHalfSize;i++)
	   for(j = VirtualMapX - pclclient->siClientScreenHalfSize;j < VirtualMapX + pclclient->siClientScreenHalfSize;j++)
	   {
		   // 시작점으로 간다.
		   if(IsInMapArea(j, i) == false)continue;
		   
		   // 출력해야 할 중심 위치를 구한다. 
		   GetDrawCenterXY(j, i, &centerx, &centery);

		   if(centerx > MAP_TILE_XSIZE )
		   {
			   continue;	// 새로 찍어야 할 부분만 찍는다. 
		   }
		   
		   if(pclLayer[1]->GetDrawInfo_Unique(j, i) != 90)
		   {
			   DrawLayer(centerx, centery, 1, j, i);
		   }
		   else
		   {
			   DrawLayer(centerx, centery, 0, j, i);
		   }
	   }

}

// 북동쪽으로 스크롤. 
void cltMapClient::Scroll_NorthEast(cltClient* pclclient)
{
	SI32 i, j;
	SI32 centerx, centery;


	SI32 xsize = pclclient->siClientScreenXsize;
	SI32 ysize = pclclient->siClientScreenYsize;


	SI32 linesize	= (xsize - MAP_TILE_XSIZE_HALF) * 2;
	SI32 delsize	= MAP_TILE_XSIZE_HALF *2;


	// 중복되는 지역을 복사한다. 
	for(i = (ysize - MAP_TILE_YSIZE_HALF) - 1;i >= 0;i--)
	{
		memmove(GP.GetSurfacePtr(0, i+MAP_TILE_YSIZE_HALF), GP.GetSurfacePtr(MAP_TILE_XSIZE_HALF, i), linesize);
	}

	// 새로 드러난 공간을 지운다. 
	for(i = 0;i < MAP_TILE_YSIZE_HALF;i++)
	{
		memset(GP.GetSurfacePtr(0, i), 0, linesize + delsize);
	}
	for(i = 0;i < ysize;i++)
	{
		memset(GP.GetSurfacePtr(xsize - MAP_TILE_XSIZE_HALF, i), 0, delsize);
	}


   for(i = VirtualMapY - pclclient->siClientScreenHalfSize;i < VirtualMapY + pclclient->siClientScreenHalfSize;i++)
	   for(j = VirtualMapX - pclclient->siClientScreenHalfSize;j < VirtualMapX + pclclient->siClientScreenHalfSize;j++)
	   {
		   // 시작점으로 간다.
		   if(IsInMapArea(j, i) == false)continue;
		   
		   // 출력해야 할 중심 위치를 구한다. 
		   GetDrawCenterXY(j, i, &centerx, &centery);

		   if((centery) > MAP_TILE_YSIZE_HALF && centerx < (xsize - MAP_TILE_XSIZE_HALF) )
		   {
			   continue;
		   }

		   
		   if(pclLayer[1]->GetDrawInfo_Unique(j, i) != 90)
		   {
			   DrawLayer(centerx, centery, 1, j, i);
		   }
		   else
		   {
			   DrawLayer(centerx, centery, 0, j, i);
		   }
	   }

}

// 북서쪽으로 스크롤. 
void cltMapClient::Scroll_NorthWest(cltClient* pclclient)
{
	SI32 i, j;
	SI32 centerx, centery;


	SI32 xsize = pclclient->siClientScreenXsize;
	SI32 ysize = pclclient->siClientScreenYsize;


	SI32 linesize	= (xsize - MAP_TILE_XSIZE_HALF) * 2;
	SI32 delsize	= MAP_TILE_XSIZE_HALF *2;


	// 중복되는 지역을 복사한다. 
	for(i = (ysize - MAP_TILE_YSIZE_HALF) - 1;i >= 0;i--)
	{
		memmove(GP.GetSurfacePtr(MAP_TILE_XSIZE_HALF, i+MAP_TILE_YSIZE_HALF), GP.GetSurfacePtr(0, i), linesize);
	}

	// 새로 드러난 공간을 지운다. 
	for(i = 0;i < MAP_TILE_YSIZE_HALF;i++)
	{
		memset(GP.GetSurfacePtr(0, i), 0, linesize + delsize);
	}
	for(i = 0;i < ysize;i++)
	{
		memset(GP.GetSurfacePtr(0, i), 0, delsize);
	}


   for(i = VirtualMapY - pclclient->siClientScreenHalfSize;i < VirtualMapY + pclclient->siClientScreenHalfSize;i++)
	   for(j = VirtualMapX - pclclient->siClientScreenHalfSize;j < VirtualMapX + pclclient->siClientScreenHalfSize;j++)
	   {
		   // 시작점으로 간다.
		   if(IsInMapArea(j, i) == false)continue;
		   
		   // 출력해야 할 중심 위치를 구한다. 
		   GetDrawCenterXY(j, i, &centerx, &centery);

		   if((centery) > MAP_TILE_YSIZE_HALF && centerx > MAP_TILE_XSIZE_HALF )
		   {
			   continue;
		   }

		   
		   if(pclLayer[1]->GetDrawInfo_Unique(j, i) != 90)
		   {
			   DrawLayer(centerx, centery, 1, j, i);
		   }
		   else
		   {
			   DrawLayer(centerx, centery, 0, j, i);
		   }
	   }

}


// 남서쪽으로 스크롤. 
void cltMapClient::Scroll_SouthWest(cltClient* pclclient)
{
	SI32 i, j;
	SI32 centerx, centery;


	SI32 xsize = pclclient->siClientScreenXsize;
	SI32 ysize = pclclient->siClientScreenYsize;


	SI32 linesize	= (xsize - MAP_TILE_XSIZE_HALF) * 2;
	SI32 delsize	= MAP_TILE_XSIZE_HALF *2;

	// 중복되는 지역을 복사한다. 
	for(i = MAP_TILE_YSIZE_HALF;i < ysize;i++)
	{
		memmove(GP.GetSurfacePtr(MAP_TILE_XSIZE_HALF, i - MAP_TILE_YSIZE_HALF), GP.GetSurfacePtr(0, i), linesize);
	}


	// 새로 드러난 공간을 지운다. 
	for(i = ysize - MAP_TILE_YSIZE_HALF;i < ysize;i++)
	{
		memset(GP.GetSurfacePtr(0, i), 0, linesize + delsize);
	}

	for(i = 0;i < ysize;i++)
	{
		memset(GP.GetSurfacePtr(0, i), 0, delsize);
	}


   for(i = VirtualMapY - pclclient->siClientScreenHalfSize;i < VirtualMapY + pclclient->siClientScreenHalfSize;i++)
	   for(j = VirtualMapX - pclclient->siClientScreenHalfSize;j < VirtualMapX + pclclient->siClientScreenHalfSize;j++)
	   {
		   // 시작점으로 간다.
		   if(IsInMapArea(j, i) == false)continue;
		   
		   // 출력해야 할 중심 위치를 구한다. 
		   GetDrawCenterXY(j, i, &centerx, &centery);

		   if(centery < ysize - MAP_TILE_YSIZE_HALF && centerx > MAP_TILE_XSIZE_HALF )
		   {
			   continue;
		   }

		   
		   if(pclLayer[1]->GetDrawInfo_Unique(j, i) != 90)
		   {
			   DrawLayer(centerx, centery, 1, j, i);
		   }
		   else
		   {
			   DrawLayer(centerx, centery, 0, j, i);
		   }
	   }

}


// 남동쪽으로 스크롤. 
void cltMapClient::Scroll_SouthEast(cltClient* pclclient)
{
	SI32 i, j;
	SI32 centerx, centery;


	SI32 xsize = pclclient->siClientScreenXsize;
	SI32 ysize = pclclient->siClientScreenYsize;


	SI32 linesize	= (xsize - MAP_TILE_XSIZE_HALF) * 2;
	SI32 delsize	= MAP_TILE_XSIZE_HALF *2;

	// 중복되는 지역을 복사한다. 
	for(i = MAP_TILE_YSIZE_HALF;i < ysize;i++)
	{
		memmove(GP.GetSurfacePtr(0, i - MAP_TILE_YSIZE_HALF), GP.GetSurfacePtr(MAP_TILE_XSIZE_HALF, i), linesize);
	}


	// 새로 드러난 공간을 지운다. 
	for(i = ysize - MAP_TILE_YSIZE_HALF;i < ysize;i++)
	{
		memset(GP.GetSurfacePtr(0, i), 0, linesize + delsize);
	}

	for(i = 0;i < ysize;i++)
	{
		memset(GP.GetSurfacePtr(xsize - MAP_TILE_XSIZE_HALF, i), 0, delsize);
	}


   for(i = VirtualMapY - pclclient->siClientScreenHalfSize;i < VirtualMapY + pclclient->siClientScreenHalfSize;i++)
	   for(j = VirtualMapX - pclclient->siClientScreenHalfSize;j < VirtualMapX + pclclient->siClientScreenHalfSize;j++)
	   {
		   // 시작점으로 간다.
		   if(IsInMapArea(j, i) == false)continue;
		   
		   // 출력해야 할 중심 위치를 구한다. 
		   GetDrawCenterXY(j, i, &centerx, &centery);

		   if(centery < ysize - MAP_TILE_YSIZE_HALF && centerx < xsize - MAP_TILE_XSIZE_HALF )
		   {
			   continue;
		   }

		   
		   if(pclLayer[1]->GetDrawInfo_Unique(j, i) != 90)
		   {
			   DrawLayer(centerx, centery, 1, j, i);
		   }
		   else
		   {
			   DrawLayer(centerx, centery, 0, j, i);
		   }
	   }
}

void cltMapClient::PopEffect()
{
	for ( SI32 i = 0 ; i < CurrentEffect ; i ++ )
	{
		if ( pclEffectMap[i].gimg <= 0 || pclEffectMap[i].gimg > MAX_GLOBAL_IMG_FILE_NUMBER ) continue;

		TSpr* pspr	= pclClient->GetGlobalSpr(pclEffectMap[i].gimg);
		if ( pspr )
		{
			GP.PutSprScreen(pspr, pclEffectMap[i].x , pclEffectMap[i].y, pclEffectMap[i].font,pclEffectMap[i].reverse);
		}
		pclEffectMap[i].Init();
	}

	CurrentEffect = 0 ;
}

void cltMapClient::PushEffect(cltEffectMapInfo * pclinfo)
{
	if ( pclinfo == NULL ) return ;

	pclEffectMap[CurrentEffect].Set(pclinfo);
	CurrentEffect ++ ;
}



