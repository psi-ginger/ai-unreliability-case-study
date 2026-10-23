//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
#include <CommonLogic.h>
#include "Map.h"
#include "Map\FieldObject\FieldObject.h"

// Field Object 브러시를 사용해서 찍어준다. 
BOOL cltMapCommon::PutFieldObjectBrush(SI32 six, SI32 siy, SI32 brush)
{
	if(IsInMapArea(six, siy) == false)	return FALSE;
	if(pFieldObjectMap == NULL)			return FALSE;

	return PutFieldObject( six, siy, pclFieldObjectManager->pclFieldObjectBrush[brush]->GetRandUnique() );
}




// six, siy좌표에 있는 Field Object를 삭제한다. 
BOOL cltMapCommon::DeleteFieldObject(SI32 six, SI32 siy)
{
	SI32 i, j;
	if(IsInMapArea(six, siy) == false)	return FALSE;
	if(pFieldObjectMap == NULL)			return FALSE;

	SI32 siunique = GetFieldObject_Unique(six , siy);

	if(siunique == 0)return FALSE;

	SI32 ref		= pclFieldObjectManager->FindRefFromUniqueMinus(siunique);
	if(ref < 0)return FALSE;

	SI32 tilexsize	= pclFieldObjectManager->pclInfo[ref]->TileXsize;
	SI32 tileysize	= pclFieldObjectManager->pclInfo[ref]->TileYsize;
	SI32 tilex		= six + GetFieldObjectLengthToRight(six, siy);
	SI32 tiley		= siy + GetFieldObjectLengthToBottom(six, siy);

	for(i = 0; i< tileysize;i++)
		for(j = 0;j < tilexsize ;j++)
		{
			SI32 x = tilex - tilexsize + 1 + j;
			SI32 y = tiley - tileysize + 1 + i;

			if(IsInMapArea(x, y) == true)
			{
				pFieldObjectMap[x + YOff[y]] = 0;
			}
		}

	return TRUE;
}

BOOL cltMapCommon::PutFieldObject(SI32 six, SI32 siy, TCHAR* name, SI32 para)
{
	SI32 siunique = pclFieldObjectManager->FindUniqueFromName(name);
	if(siunique == 0)
	{
		MsgBox(TEXT("f8rj33r"), TEXT("2984:%s"), name);
		return FALSE;
	}

	return PutFieldObject(six, siy, siunique, para);
}

BOOL cltMapCommon::PutFieldObject(SI32 six, SI32 siy, SI32 siunique, SI32 para)
{
	if(IsInMapArea(six, siy) == false)	return 0;
	if(pFieldObjectMap == NULL)			return 0;

	SI32 i, j;

	SI32 ref		= pclFieldObjectManager->FindRefFromUniqueMinus(siunique);
	if(ref < 0)return FALSE;

	// 사이즈등의 상세 정보를 하드에서 읽어온다.
	pclFieldObjectManager->pclInfo[ref]->LoadDetailInfo();

	SI32 tilexsize	= pclFieldObjectManager->pclInfo[ref]->TileXsize;
	SI32 tileysize	= pclFieldObjectManager->pclInfo[ref]->TileYsize;
	SI32 tilecenterx = pclFieldObjectManager->pclInfo[ref]->TileCenterX;
	SI32 tilecentery = pclFieldObjectManager->pclInfo[ref]->TileCenterY;

	//------------------------------------------------------------
	// Field Object 영역 안에 다른 Field Object가 있는지 확인한다. 
	// 다른 것이 있다면 새로운 Field Object를 찍을 수 없다. 
	//------------------------------------------------------------
	for(i = 0; i< tileysize;i++)
		for(j = 0;j < tilexsize ;j++)
		{
			SI32 x = six + j - tilexsize + 1 + tilecenterx ;
			SI32 y = siy + i - tileysize + 1 + tilecentery ;

			SI32 unique = GetFieldObject_Unique(x , y);
				
			if( unique )	return FALSE;
		}

	//-----------------------------------------------------------------
	// 다른 Field Object가 없는 상태라면 안전하니까 새로운 것을 찍는다.
	//-----------------------------------------------------------------
	for(i = 0; i< tileysize;i++)
		for(j = 0;j < tilexsize ;j++)
		{
			SI32 x = six + tilecenterx - tilexsize + 1 + j;
			SI32 y = siy + tilecentery - tileysize + 1 + i;
			SetFieldObject_Unique(	x , y, siunique);

			// 출력 무효한 필드 오브젝트의 파라메타 영역에는 
			// 출력의 중심이 되는 맨 하단 우측 좌표와의 변위를 기록한다.
			// 이 변위와 위치를 더한 값은 중심값의 좌표가 된다. 
			// 어디를 출력 중심으로 하는 필드 오브젝트인지를 쉽게 판단할 수 있게 만드는 것이 
			// 이 작업의 목표다. 
			SetFieldObjectLengthToBottom(x, y, tileysize - i - 1);
			SetFieldObjectLengthToRight(x, y,  tilexsize - j - 1); 

			SetFieldObjectPara(x, y, para);


			// 중심점에는 필드 오브젝트를 유효하게 만드는 스위치를 켜준다. 
			if(x == six && y == siy)
			{
				SetFieldObjectValidSwitch(x, y, TRUE);

				// 랜덤값도 넣어준다. 
				SetFieldObjectRandomValue(x, y, rand() % 16);
			}

		}

	

	return TRUE;
}

BOOL cltMapCommon::PutGateFieldObject(SI32 six, SI32 siy, SI32 siunique, SI32 para)
{
	if(IsInMapArea(six, siy) == false)	return 0;
	if(pFieldObjectMap == NULL)			return 0;

	SI32 i, j;

	SI32 ref		= pclFieldObjectManager->FindRefFromUniqueMinus(siunique);
	if(ref < 0)return FALSE;

	// 사이즈등의 상세 정보를 하드에서 읽어온다.
	pclFieldObjectManager->pclInfo[ref]->LoadDetailInfo();

	SI32 tilexsize	= pclFieldObjectManager->pclInfo[ref]->TileXsize;
	SI32 tileysize	= pclFieldObjectManager->pclInfo[ref]->TileYsize;
	SI32 tilecenterx = pclFieldObjectManager->pclInfo[ref]->TileCenterX;
	SI32 tilecentery = pclFieldObjectManager->pclInfo[ref]->TileCenterY;

	//------------------------------------------------------------
	// Field Object 영역 안에 다른 Field Object가 있는지 확인한다. 
	// 다른 것이 있다면 새로운 Field Object를 찍을 수 없다. 
	//------------------------------------------------------------
	for(i = 0; i< tileysize;i++)
	{
		for(j = 0;j < tilexsize ;j++)
		{
			SI32 x = six + j - tilexsize + 1 + tilecenterx ;
			SI32 y = siy + i - tileysize + 1 + tilecentery ;

			SI32 unique = GetFieldObject_Unique(x , y);

			// 요청한것과 다른 것이 찍혀있으면 안된다.
			if( unique > 0 && unique != siunique )	return FALSE;
		}
	}

	//-----------------------------------------------------------------
	// 다른 Field Object가 없는 상태라면 안전하니까 새로운 것을 찍는다.
	//-----------------------------------------------------------------
	for(i = 0; i< tileysize;i++)
	{
		for(j = 0;j < tilexsize ;j++)
		{
			SI32 x = six + tilecenterx - tilexsize + 1 + j;
			SI32 y = siy + tilecentery - tileysize + 1 + i;
			SetFieldObject_Unique(	x , y, siunique);

			// 출력 무효한 필드 오브젝트의 파라메타 영역에는 
			// 출력의 중심이 되는 맨 하단 우측 좌표와의 변위를 기록한다.
			// 이 변위와 위치를 더한 값은 중심값의 좌표가 된다. 
			// 어디를 출력 중심으로 하는 필드 오브젝트인지를 쉽게 판단할 수 있게 만드는 것이 
			// 이 작업의 목표다. 
			SetFieldObjectLengthToBottom(x, y, tileysize - i - 1);
			SetFieldObjectLengthToRight(x, y,  tilexsize - j - 1); 

			SetFieldObjectPara(x, y, para);

			// 중심점에는 필드 오브젝트를 유효하게 만드는 스위치를 켜준다. 
			if(x == six && y == siy)
			{
				SetFieldObjectValidSwitch(x, y, TRUE);

				// 랜덤값도 넣어준다. 
				SetFieldObjectRandomValue(x, y, rand() % 16);
			}

		}
	}

	return TRUE;
}

BOOL cltMapCommon::SetFieldObject_Unique(SI32 six, SI32 siy, SI32 siunique)
{
	if(IsInMapArea(six, siy) == false) return FALSE;
	if(pFieldObjectMap == NULL)return FALSE;

	uFieldObjectUnion fUnion;

	fUnion.Data32 = pFieldObjectMap[six + YOff[siy]];

	fUnion.FieldObject_Normal.SetFieldUnique(siunique);
	pFieldObjectMap[six + YOff[siy]]	= fUnion.Data32;

	return TRUE;
}

SI32 cltMapCommon::GetFieldObject_Unique(SI32 six, SI32 siy)
{
	if(IsInMapArea(six, siy) == false) return 0;
	if(pFieldObjectMap == NULL)return 0;

	uFieldObjectUnion fUnion;

	fUnion.Data32 = pFieldObjectMap[six + YOff[siy]];


	return fUnion.FieldObject_Normal.GetFieldUnique();
}


BOOL cltMapCommon::SetFieldObjectPara(SI32 six, SI32 siy, SI32 para)
{
	if(IsInMapArea(six, siy) == false) return FALSE;
	if(pFieldObjectMapPara == NULL)return FALSE;


	pFieldObjectMapPara[six + YOff[siy]] = para;

	return TRUE;
}

SI32 cltMapCommon::GetFieldObjectPara(SI32 six, SI32 siy)
{
	if(IsInMapArea(six, siy) == false) return 0;
	if(pFieldObjectMapPara == NULL)return 0;

	return pFieldObjectMapPara[six + YOff[siy]];
}

// 출력을 해도 되는 유효한 필드 오브젝트인가 공갈 오브젝트인가 검색한다. 
BOOL cltMapCommon::GetFieldObjectValidSwitch(SI32 six, SI32 siy)
{
	if(IsInMapArea(six, siy) == false) return 0;
	if(pFieldObjectMap == NULL)return 0;

	uFieldObjectUnion fUnion;

	fUnion.Data32 = pFieldObjectMap[six + YOff[siy]];

	if(fUnion.FieldObject_Normal.GetValidSwitch())return TRUE;

	return FALSE;
}

// 필드 오브젝트를 출력 유효하게 하거나 무효하게 한다. 
void cltMapCommon::SetFieldObjectValidSwitch(SI32 six, SI32 siy, BOOL bmode)
{
	if(IsInMapArea(six, siy) == false) return ;
	if(pFieldObjectMap == NULL)return ;

	uFieldObjectUnion fUnion;

	fUnion.Data32 = pFieldObjectMap[six + YOff[siy]];

	if(bmode == TRUE)
	{
		fUnion.FieldObject_Normal.SetValidSwitch(1);
		pFieldObjectMap[six + YOff[siy]] = fUnion.Data32;
	}
	else
	{
		fUnion.FieldObject_Normal.SetValidSwitch(0);
		pFieldObjectMap[six + YOff[siy]] = fUnion.Data32;
	}
}


void cltMapCommon::SetFieldObjectRandomValue(SI32 six, SI32 siy, BOOL value)
{
	if(IsInMapArea(six, siy) == false) return ;
	if(pFieldObjectMap == NULL)return ;

	uFieldObjectUnion fUnion;

	fUnion.Data32 = pFieldObjectMap[six + YOff[siy]];

	fUnion.FieldObject_Normal.SetRandomValue(value);
	pFieldObjectMap[six + YOff[siy]] = fUnion.Data32;
}

UI32 cltMapCommon::GetFieldObjectRandomValue(SI32 six, SI32 siy)
{
	if(IsInMapArea(six, siy) == false)	return 0;
	if(pFieldObjectMap == NULL)			return 0;

	uFieldObjectUnion fUnion;

	fUnion.Data32 = pFieldObjectMap[six + YOff[siy]];

	return fUnion.FieldObject_Normal.GetRandomValue(); 
}

void cltMapCommon::SetFieldObjectLengthToBottom(SI32 six, SI32 siy, SI32 length)
{
	if(IsInMapArea(six, siy) == false) return ;
	if(pFieldObjectMap == NULL)return ;

	uFieldObjectUnion fUnion;

	fUnion.Data32 = pFieldObjectMap[six + YOff[siy]];

	fUnion.FieldObject_Normal.SetLengthToBottom(length);
	pFieldObjectMap[six + YOff[siy]] = fUnion.Data32;

}

UI32 cltMapCommon::GetFieldObjectLengthToBottom(SI32 six, SI32 siy)
{
	if(IsInMapArea(six, siy) == false)	return 0;
	if(pFieldObjectMap == NULL)			return 0;

	uFieldObjectUnion fUnion;

	fUnion.Data32 = pFieldObjectMap[six + YOff[siy]];

	return fUnion.FieldObject_Normal.GetLengthToBottom();
}


void cltMapCommon::SetFieldObjectLengthToRight(SI32 six, SI32 siy, SI32 length)
{
	if(IsInMapArea(six, siy) == false) return ;
	if(pFieldObjectMap == NULL)return ;

	uFieldObjectUnion fUnion;

	fUnion.Data32 = pFieldObjectMap[six + YOff[siy]];

	fUnion.FieldObject_Normal.SetLengthToRight(length);
	pFieldObjectMap[six + YOff[siy]] = fUnion.Data32;

}

UI32 cltMapCommon::GetFieldObjectLengthToRight(SI32 six, SI32 siy)
{
	if(IsInMapArea(six, siy) == false)	return 0;
	if(pFieldObjectMap == NULL)			return 0;

	uFieldObjectUnion fUnion;

	fUnion.Data32 = pFieldObjectMap[six + YOff[siy]];

	return fUnion.FieldObject_Normal.GetLengthToRight();
}

// six, siy지점에 있는 Field Object의 중심 위치를 구해서 리턴한다. 
bool cltMapCommon::GetFieldObjectCenterPos(SI32 six, SI32 siy, SI32* pcenterx, SI32* pcentery)
{
	// 우선 해당 위치에 FieldObject가 있어야 한다. 
	SI32 siunique = GetFieldObject_Unique(six, siy);
	if(siunique == 0)return false;

	SI32 ref		= pclFieldObjectManager->FindRefFromUniqueMinus(siunique);
	if(ref < 0)return false;

	SI32 tilecenterx = pclFieldObjectManager->pclInfo[ref]->TileCenterX;
	SI32 tilecentery = pclFieldObjectManager->pclInfo[ref]->TileCenterY;

	*pcenterx = six - tilecenterx + GetFieldObjectLengthToRight(six, siy);
	*pcentery = siy - tilecentery + GetFieldObjectLengthToBottom(six, siy);

	return true;
}

// 지도에 새롭게 Field Object를 배치한다. 
void cltMapCommon::NewFieldObjectSetInMap()
{
	SI32 i, j;

	// 기존의 정보를 저장할 새로운 공간을 만든다. 
	SI32* pFOMap = new SI32[MH.MapXsize * MH.MapYsize];

	ZeroMemory(pFOMap, sizeof(SI32) * MH.MapXsize * MH.MapYsize);

	// 새로 할당한 공간에 기존 정보를 백업한다. 
	for(i = 0;i < MH.MapYsize;i++)
		for(j = 0;j < MH.MapXsize;j++)
		{
			if(GetFieldObject_Unique(j, i))
			{
				if(GetFieldObjectValidSwitch(j, i))
				{
					pFOMap[j + i*MH.MapXsize] = GetFieldObject_Unique(j, i);
				}
			}
		}


	// 지도의 FieldObject정보를 초기화한다.
	for(i = 0;i < MH.MapYsize;i++)
		for(j = 0;j < MH.MapXsize;j++)
		{
			pFieldObjectMap[j + i*MH.MapXsize]		= 0;
			pFieldObjectMapPara[j + i*MH.MapXsize]	= 0;
		}
	
	// 백업된 정보로 지도의  FieldObject를 재 설정한다. 
	for(i = 0;i < MH.MapYsize;i++)
		for(j = 0;j < MH.MapXsize;j++)
		{
			if(pFOMap[j + i*MH.MapXsize])
			{
				PutFieldObject(j, i, pFOMap[j + i*MH.MapXsize]);
			}
		}

	// 새롭게 할당한 공간을 지운다.
	if(pFOMap)
	{
		delete[] pFOMap;
		pFOMap = NULL;
	}

}
void cltMapCommon::DeleteFieldObjectUnique(SI32 siUnique)
{
	SI32 i, j;

	SI32 siDeletedUnique = 0;

	for(i = 0;i < MH.MapYsize;i++)
	{
		for(j = 0;j < MH.MapXsize;j++)
		{
			SI32 unique = GetFieldObject_Unique(j, i);
			if(unique == 0)continue;

			SI32 ref	= pclFieldObjectManager->FindRefFromUniqueMinus(unique);
			if(ref < 0)continue ;

			if ( pclFieldObjectManager->pclInfo[ref]->Unique == siUnique )
            {
				DeleteFieldObject(j, i);
				siDeletedUnique++;
			}
            
		}
	}
	TCHAR buffer[256];
	StringCchPrintf( buffer, 256, "%d개의 오브젝트가 삭제되었습니다", siDeletedUnique);
	MessageBox( NULL, buffer, TEXT("삭제완료"), MB_OK|MB_TOPMOST );

}