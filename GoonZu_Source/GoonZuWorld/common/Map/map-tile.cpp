//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
#include <CommonLogic.h>
#include "Map\Map.h"
#include "..\Message\Message.h"

// -2 주변에 아무것도 없다. 
static SHORT Dir8Q[]={
-2, 0, 1, 4, 3, 7,13,11, 
 2,12, 5, 8, 6,10, 9,14
};

void cltMapCommon::PutTileXpr( SI32 layer, SI32 six, SI32 siy, SI32 unique )
{
	pclLayer[layer]->SetDrawInfo_Unique(six, siy, unique);
	
	SI32 ref = pclTileManager->FindRefFromUnique_TileSet(unique);
	if(ref < 0)return ;

	pclLayer[layer]->SetDrawInfo_Ref(six, siy, ref);
	

	SI32 index = six + siy * MH.MapXsize;

	pclLayer[layer]->SetDrawInfo_File(six, siy, 0 );
	pclLayer[layer]->SetDrawInfo_Font(six, siy, index );
}



void cltMapCommon::PutTile0(SI32 layer, SI32 six, SI32 siy, SI32 unique)
{
	pclLayer[layer]->SetDrawInfo_Unique(six, siy, unique);
	
	SI32 ref = pclTileManager->FindRefFromUnique_TileSet(unique);
	if(ref < 0)return ;

	pclLayer[layer]->SetDrawInfo_Ref(six, siy, ref);
	

	SI32 index = six + siy * MH.MapXsize;

	SI32 file = index / 250;
	SI32 font = index % 250;

	pclLayer[layer]->SetDrawInfo_File(six, siy, file );
	pclLayer[layer]->SetDrawInfo_Font(six, siy, font );

}


// layer, six, siy 지점의 의 출력 타일 세트가 unique타일 세트의 자식 세트인지 확인한다. 
BOOL cltMapCommon::IsChildTile(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 unique, SI32* pchildunique)
{
	// 유효한 지도 영역인지 확인한다. 
	if(IsInMapArea(six, siy) == false)		return FALSE;
	if(pclLayer[layer] == NULL)				return FALSE;
	if(pclLayer[layer]->IsValid() == false)	return FALSE;

	six = (six / (rate/2)) * (rate/2);
	siy = (siy / (rate/2)) * (rate/2);

	//원본의 레퍼런스를 찾는다. 
	SI32 siorgref = pclTileManager->FindRefFromUnique_TileSet(unique);
	if(siorgref < 0)return FALSE;

	// 비교해야할 타일의 레퍼런스를 찾는다.
	SI32 sicompref = pclLayer[layer]->GetDrawInfo_Ref(six, siy);
	if(sicompref >= MAX_TILESET_NUMBER)return FALSE;

	if(pclTileManager->pclTileInfo[sicompref]->pParent == pclTileManager->pclTileInfo[siorgref])
	{
		*pchildunique = pclTileManager->pclTileInfo[sicompref]->Unique;
		return TRUE;
	}

	return FALSE;
}

// sisrcunique와 sidstunique가 서로 이종타일(같지도 않고 부모도 아닌)인지의 여부를 알려준다. 
BOOL cltMapCommon::IsDiffTileSet(SI32 sisrcunique, SI32 sidstunique)
{
	SI32 orgref	= pclTileManager->FindRefFromUnique_TileSet(sisrcunique);
	if(orgref < 0)return FALSE;

	SI32 dstref = pclTileManager->FindRefFromUnique_TileSet(sidstunique);
	if(dstref < 0)return FALSE;

	if(sisrcunique != sidstunique
		&& pclTileManager->pclTileInfo[orgref]->pParent != pclTileManager->pclTileInfo[dstref]
		&& pclTileManager->pclTileInfo[dstref]->pParent != pclTileManager->pclTileInfo[orgref])
	{
		return TRUE;
	}

	return FALSE;

}

BOOL cltMapCommon::GetProperFileNFont(SI32 unique, SI32 result, SI32& file, SI32& font)
{
	SI32 varycount = 0 ;
	SI32 type;


	// 레퍼런스를 구한다. 
	SI32 siref = pclTileManager->FindRefFromUnique_TileSet(unique);
	if(siref < 0)return FALSE;

	type = Dir8Q[result];

	// 변종의 갯수를 구한다. 
	if(pclTileManager->pclTileInfo[siref]->pTileSetArrayInfo)
	{
		varycount = pclTileManager->pclTileInfo[siref]->pTileSetArrayInfo[0];
	}
	else
	{
		return FALSE;
	}

	// 적절한 파일과 폰트를 구한다. 

	// 변종중 하나를 고른다. (vary) 
	if(varycount == 0)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("fd8932hdf"), TEXT("fdf83yd"), MB_OK|MB_TOPMOST);
		return FALSE;
	}

	SI32 vary = rand() % varycount;
	

	file = pclTileManager->pclTileInfo[siref]->pTileSetArrayInfo[1 +  (type*2) * varycount + (vary*2)		]; 
	font = pclTileManager->pclTileInfo[siref]->pTileSetArrayInfo[1 +  (type*2) * varycount + (vary*2)+ 1	];

	return TRUE;

}

// sx, sy: 기본 좌표 
BOOL cltMapCommon::PutTile2(SI32 layer, SI32 six, SI32 siy, SI32 siunique)
{
	SI32 result = 0;
	SI32 i, j;
	SI32 rate	= 2;
	
	// 유효한 지도 영역인지 확인한다. 
	if(IsInMapArea(six, siy) == false)	return FALSE;
	if(pclLayer[layer] == NULL)			return FALSE;
	if(pclLayer[layer]->IsValid() == false)return FALSE;
	
	// 타일의 타입을 설정한다. 
	SetUnique(layer, six, siy, rate, siunique);

	// 타일의 남쪽에 타입을 설정한다.
	SetUniqueSouth(layer, six, siy,rate, siunique);
	
	// 타일의 서쪽에 타입을 설정한다.
	SetUniqueWest(layer, six, siy,rate, siunique);
	
	// 타일의 북쪽에 타입을 설정한다.
	SetUniqueNorth(layer, six, siy,rate, siunique);
	
	// 타일의 동쪽에 타입을 설정한다.
	SetUniqueEast(layer, six, siy,rate, siunique);

	SI32 childunique = 0;
	SI32 workunique = siunique;

	if(IsChildTile(layer, six, siy, rate, siunique, &childunique) == TRUE)
	{
		workunique = childunique;
	}
	
	// 자기 블럭의 상태를 파악한다. 
	result = GetSelfUniqueInfo(layer, six, siy, rate, workunique);

	// 전혀 Data가 없다. 
	// 이런 경우 부모 타일 세트를 찍어준다. 
	if(result == 0)
	{
		result = 15;
		
		SI32 ref = pclTileManager->FindRefFromUnique_TileSet(workunique);
		if(ref < 0)return FALSE;
		
		if(pclTileManager->pclTileInfo[ref]->pParent)
		{
			SI32 parentunique = pclTileManager->pclTileInfo[ref]->pParent->Unique;
			
			SI32 sifile, sifont;
			if(GetProperFileNFont(parentunique, result, sifile, sifont) == TRUE)
			{
				// 출력용 정보를 타일에 기록한다. 
				SetDrawInfo(layer, six, siy, rate, parentunique, sifile, sifont);
			}
			else
			{
				MsgBox(TEXT("f9d73"), TEXT("dh8hd"));
			}
		}
		
	}
	else
	{
		SI32 sifile, sifont;
		if(GetProperFileNFont(workunique, result, sifile, sifont) == TRUE)
		{
			// 출력용 정보를 타일에 기록한다. 
			SetDrawInfo(layer, six, siy, rate, workunique, sifile, sifont);
		}
		
	}

	// 주변 -2 ~ 2까지의 타일 정보를 검색해서 이종 타일이면 부모 타일을 찍어준다. 
	// 이렇게 하는 이유는 이종 타일간에 너무 인접해 있으면 타일이 깨져 보이기 때문이다.
	for(i = -2;i <= 2;i++)
		for(j = -2; j<= 2;j++)
		{
			if(IsInMapArea(six + j, siy + i) == false)continue;

			SI32 unique = pclLayer[layer]->GetDrawInfo_Unique(six + j, siy + i);

			if(IsDiffTileSet(unique, siunique) == TRUE)
			{
				SI32 dstref = pclTileManager->FindRefFromUnique_TileSet(siunique);
				if(dstref < 0)return FALSE;

				if(pclTileManager->pclTileInfo[dstref]->pParent)
				{
					PutTile2(layer, six + j, siy + i, pclTileManager->pclTileInfo[dstref]->pParent->Unique);
				}
			}
		}
	
		
	return TRUE;

}


// sx, sy: 기본 좌표 
BOOL cltMapCommon::PutTile4(SI32 layer, SI32 six, SI32 siy, SI32 siunique)
{
	SI32 result = 0;
	SI32 i, j;
	SI32 rate	= 4;
	
	// 유효한 지도 영역인지 확인한다. 
	if(IsInMapArea(six, siy) == false)	return FALSE;
	if(pclLayer[layer] == NULL)			return FALSE;
	if(pclLayer[layer]->IsValid() == false)return FALSE;
	
	// 타일의 타입을 설정한다. 
	SetUnique(layer, six, siy, rate, siunique);

	// 타일의 남쪽에 타입을 설정한다.
	SetUniqueSouth(layer, six, siy,rate, siunique);

	// 타일의 서쪽에 타입을 설정한다.
	SetUniqueWest(layer, six, siy,rate, siunique);

	// 타일의 북쪽에 타입을 설정한다.
	SetUniqueNorth(layer, six, siy,rate, siunique);

	// 타일의 동쪽에 타입을 설정한다.
	SetUniqueEast(layer, six, siy,rate, siunique);

	
	SI32 childunique = 0;
	SI32 workunique = siunique;

	if(IsChildTile(layer, six, siy, rate, siunique, &childunique) == TRUE)
	{
		workunique = childunique;
	}

	// 자기 블럭의 상태를 파악한다. 
	result = GetSelfUniqueInfo(layer, six, siy, rate, workunique);	

	// 전혀 Data가 없다. 
	// 이런 경우 부모 타일 세트를 찍어준다. 
	if(result == 0)
	{
		result = 15;
		
		SI32 ref = pclTileManager->FindRefFromUnique_TileSet(workunique);
		if(ref < 0)return FALSE;
		
		if(pclTileManager->pclTileInfo[ref]->pParent)
		{
			SI32 parentunique = pclTileManager->pclTileInfo[ref]->pParent->Unique;
			
			SI32 sifile, sifont;
		
			if(GetProperFileNFont(parentunique, result, sifile, sifont) == TRUE)
			{
				// 출력용 정보를 타일에 기록한다. 
				SetDrawInfo(layer, six, siy, rate, parentunique, sifile, sifont);
			}
			else
			{
				MsgBox(TEXT("f9d73"), TEXT("dh8hd"));
			}
		}
		
	}
	else
	{
		SI32 sifile, sifont;
		if(GetProperFileNFont(workunique, result, sifile, sifont) == TRUE)
		{
			// 출력용 정보를 타일에 기록한다. 
			SetDrawInfo(layer, six, siy, rate, workunique, sifile, sifont);
		}
		
	}

	// 주변 -2 ~ 2까지의 타일 정보를 검색해서 이종 타일이면 부모 타일을 찍어준다. 
	// 이렇게 하는 이유는 이종 타일간에 너무 인접해 있으면 타일이 깨져 보이기 때문이다.
	for(i = -2;i <= 2;i++)
		for(j = -2; j<= 2;j++)
		{
			if(IsInMapArea(six + j, siy + i) == false)continue;

			SI32 unique = pclLayer[layer]->GetDrawInfo_Unique(six + j, siy + i);

			if(IsDiffTileSet(unique, siunique) == TRUE)
			{
				SI32 dstref = pclTileManager->FindRefFromUnique_TileSet(siunique);
				if(dstref < 0 )return FALSE;

				if(pclTileManager->pclTileInfo[dstref]->pParent)
				{
					PutTile4(layer, six + j, siy + i, pclTileManager->pclTileInfo[dstref]->pParent->Unique);
				}
			}
		}
	
		
	return TRUE;

}


// 타일의 배수등을 고려하여 Unique를 설정한다.
void cltMapCommon::SetUnique(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 siunique)
{
	six = (six / (rate/2)) * (rate/2);
	siy = (siy / (rate/2)) * (rate/2);

	pclLayer[layer]->SetUnique(six, siy, siunique);
}

// 주어진 타일의 남쪽에 같은 unique를 설정한다.
void cltMapCommon::SetUniqueSouth(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 siunique)
{
	six = (six / (rate/2)) * (rate/2);
	siy = (siy / (rate/2)) * (rate/2);

	if((siy%rate) == (rate/2) && pclLayer[layer]->GetUnique(six, siy+(rate/2) ) != siunique)
	{
		if(rate == 2)
		{
			PutTile2(layer, six, siy + (rate/2), siunique);
		}
		else if(rate == 4)
		{
			PutTile4(layer, six, siy + (rate/2), siunique);
		}
		else
		{
			MsgBox(TEXT("fds8"), TEXT("fdv9j:%d"), rate);
		}
	}
}

// 주어진 타일의 서쪽에 같은 unique를 설정한다.
void cltMapCommon::SetUniqueWest(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 siunique)
{
	six = (six / (rate/2)) * (rate/2);
	siy = (siy / (rate/2)) * (rate/2);

	if((six % rate) == 0 && pclLayer[layer]->GetUnique(six-(rate/2), siy) != siunique)
	{
		if(rate == 2)
		{
			PutTile2(layer, six-(rate/2), siy, siunique);
		}
		else if(rate == 4)
		{
			PutTile4(layer, six-(rate/2), siy, siunique);
		}
	}
}

// 주어진 타일의 북쪽에 같은 unique를 설정한다.
void cltMapCommon::SetUniqueNorth(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 siunique)
{
	six = (six / (rate/2)) * (rate/2);
	siy = (siy / (rate/2)) * (rate/2);

	if((siy % rate) == 0 && pclLayer[layer]->GetUnique(six, siy- (rate/2)) !=siunique)
	{
		if(rate == 2)
		{
			PutTile2(layer, six, siy-(rate/2), siunique);
		}
		else if(rate == 4)
		{
			PutTile4(layer, six, siy-(rate/2), siunique);
		}
	}
}

// 주어진 타일의 동쪽에 같은 unique를 설정한다.
void cltMapCommon::SetUniqueEast(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 siunique)
{
	six = (six / (rate/2)) * (rate/2);
	siy = (siy / (rate/2)) * (rate/2);

	if((six%rate) == (rate/2) && pclLayer[layer]->GetUnique(six+(rate/2), siy) != siunique)
	{
		if(rate == 2)
		{
			PutTile2(layer, six+(rate/2), siy, siunique);
		}
		else if(rate== 4)
		{
			PutTile4(layer, six+(rate/2), siy, siunique);
		}
	}
}

// 자기 블럭의 상태를 파악한다. 
SI32 cltMapCommon::GetSelfUniqueInfo(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 workunique)
{
	SI32 result = 0;

	six = (six / rate) * (rate);
	siy = (siy / rate) * (rate);
	
	if(pclLayer[layer]->GetUnique(six,	siy) == workunique)
	{
		result|=1;
	}
	if(pclLayer[layer]->GetUnique(six + (rate/2),	siy) == workunique)
	{
		result|=2;
	}
	if(pclLayer[layer]->GetUnique(six,	siy + (rate/2)) ==workunique)
	{
		result|=4;
	}
	if(pclLayer[layer]->GetUnique(six + (rate/2),	siy + (rate/2)) == workunique)
	{
		result|=8;
	}

	return result;
}

// 출력용 정보를 타일에 기록한다. 
void cltMapCommon::SetDrawInfo(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 unique, SI32 sifile, SI32 sifont)
{
	SI32 i, j;

	six = (six / rate) * (rate);
	siy = (siy / rate) * (rate);

	SI32 siref = pclTileManager->FindRefFromUnique_TileSet(unique);
	if(siref <0)return ;

	for(i = 0;i < rate;i++)
		for(j = 0;j < rate;j++)
		{
			pclLayer[layer]->SetDrawInfo_Unique	(six + j, siy + i, unique);
			pclLayer[layer]->SetDrawInfo_Ref	(six + j, siy + i, siref);
						
			pclLayer[layer]->SetDrawInfo_File	(six + j, siy + i, sifile );
			pclLayer[layer]->SetDrawInfo_Font	(six + j, siy + i, sifont+j+(i*rate));
		}
					
}
