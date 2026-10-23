//---------------------------------
// 2003/8/7 김태곤
//---------------------------------

#include "Item-ManagerClient.h"
#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "..\..\Skill\Skill-Manager.h"
#include "Char\CharManager\CharManager.h"

#include "MsgType-Person.h"

#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;
extern SI32 hDrag;
extern SI32 hotx, hoty;
extern BOOL g_bDrag;
extern SI32 siDragFrom;

#include "..\..\Resource.h"
//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"

//-------------------------------------
// 아이템 관리자. 
//-------------------------------------
cltItemManagerClient::cltItemManagerClient(HWND hwnd, SI32 gamemode) : cltItemManagerCommon()
{
	SI32 i;

	CharUnique	= 0;
	
	for(i = 0;i < MAX_ITEM_IMAGE_NUM ;i++)
	{
		StringCchCopy(BitmapFileName[i], 64, TEXT("NULL"));
	}

#ifdef _SAFE_MEMORY
	m_ItemImageBigUnique.ZeroMem();
#else
	ZeroMemory(m_ItemImageBigUnique, sizeof(m_ItemImageBigUnique));
#endif
}

cltItemManagerClient::~cltItemManagerClient()
{

}

// 아이템의 비트맵 이미지 정보를 설정한다. (클라이언트 전용) 
void cltItemManagerClient::SetBitmapImage()
{
	SI32 i;
	TCHAR buffer[256];
	FILE* fp;

	fp = _tfopen(TEXT("Item\\ItemImageList.txt"), TEXT("rt"));
	if(fp == NULL)
	{
		#ifdef _DEBUG
			MsgBox(TEXT("파일 읽기 오류"), TEXT("ItemImageList.txt 파일을 읽지 못했습니다"));
		#else
			MsgBox(TEXT("fdsfijf"), TEXT("vssfdsa"));
		#endif
		return ;
	}

	for(i = 0;i < 6;i++)
	{
		_fgetts(buffer, 100, fp);
	}
	
	SI32 index = 0;
	while(_fgetts(buffer, 100, fp) != NULL)
	{
		TCHAR filename[128];

		_stscanf(buffer, TEXT("%s"), filename);
		if(_tcsncmp(buffer, TEXT("NULL"), 4) == 0)break;

		_tcsncpy(BitmapFileName[index], filename, 64-1);
		index++;

		if(index >= MAX_ITEM_IMAGE_NUM){ break; }
	}

	fclose(fp);

}


// 아이템의 포지션 정보를 가지고 그 아이템의 포인터를 구한다.
cltItem* cltItemManagerClient::GetItemPointerFromPosInfo(SI32 id, SI32 siDragFrom)
{
	return  &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[siDragFrom];			
}


SI32 cltItemManagerClient::GetBitmapIndexFromBitmapName(TCHAR* bitmapname)
{
	return GetBitmapIndexFromFileName(bitmapname);
}



SI32 cltItemManagerClient::GetBitmapIndexFromFileName(const TCHAR* filename)
{
	SI32 index = 0;
	while(_tcscmp(BitmapFileName[index], TEXT("NULL")) != 0)
	{
		if(_tcscmp(_tcslwr(BitmapFileName[index]), _tcslwr((TCHAR*)filename)) == 0)
		{
			return index;
		}
		index++;
	}
	return -1;
}

HBITMAP cltItemManagerClient::MakeDDBFromDIB(HDC hdc, SI32 imageindex)
{
	return MakeDDBFromDIB(hdc, BitmapFileName[imageindex]);
}

HBITMAP cltItemManagerClient::MakeDDBFromDIB(HDC hdc, TCHAR* path)
{
	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER* fh;
	HBITMAP hBit;
	PVOID ih;

	hFile = CreateFile(path, GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	FileSize = GetFileSize(hFile, NULL);

	fh = (BITMAPFILEHEADER*)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);

	ih		= ((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	hBit	= CreateDIBitmap(hdc, (BITMAPINFOHEADER*)ih, CBM_INIT, 
		(PBYTE)fh + fh->bfOffBits, (BITMAPINFO*)ih, DIB_RGB_COLORS);

	free(fh);

	return hBit;
}


//  아이템 관련 효과음을 연주한다. 
void cltItemManagerClient::PushEffect(SI32 effect)
{

	pclClient->PushEffect(effect, 0, 0 );
}

// 효과음을 연주한다. 
void cltItemManagerClient::PlaySound(SI32 itemunique, SI32 type)
{
	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	SI32 soundtype = pclItemInfo[ref]->siSoundType;


	SI32 effect = 0;

	switch(type)
	{
	case ITEMSOUND_MAKE:
		switch(soundtype)
		{
		case ITEMSOUND_BOOK:
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_ETC"));		break;	
		case ITEMSOUND_BOW:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_WEAPON"));	break;
		case ITEMSOUND_CLOTH:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_ARMOUR"));	break;
		case ITEMSOUND_FOOD:
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_FOOD"));		break;
		case ITEMSOUND_HEAL:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_MEDICAL"));	break;
		case ITEMSOUND_HELMET:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_ARMOUR"));	break;
		case ITEMSOUND_SWORD:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_WEAPON"));	break;
		case ITEMSOUND_MEAT:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_FOOD"));		break;
		case ITEMSOUND_NOODLE:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_FOOD"));		break;
		case ITEMSOUND_SOUP:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_FOOD"));		break;
		case ITEMSOUND_VEGETABLE:		
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_FOOD"));		break;
		case ITEMSOUND_AXE:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_WEAPON"));	break;
		case ITEMSOUND_BELT:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_ORNAMENT"));	break;
		case ITEMSOUND_ETC:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_ETC"));		break;
		case ITEMSOUND_GUN:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_WEAPON"));	break;
		case ITEMSOUND_NECK:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_ORNAMENT"));	break;
		case ITEMSOUND_SHOES:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_ARMOUR"));	break;
		case ITEMSOUND_STAFF:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_WEAPON"));	break;
		case ITEMSOUND_TICKET:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_ETC"));		break;
		case ITEMSOUND_CERAMIC:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_CERAMIC"));	break;
		case ITEMSOUND_SMELLHEAL:		
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_MEDICAL"));	break;
		case ITEMSOUND_UNFREEZEHEAL:	
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKE_MEDICAL"));	break;
		}
		break;
		
	case ITEMSOUND_SELECT:
		switch(soundtype)
		{
		case ITEMSOUND_BOOK:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_BOOK"));	break;	
		case ITEMSOUND_BOW:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_BOW"));		break;
		case ITEMSOUND_CLOTH:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_CLOTH"));	break;
		case ITEMSOUND_FOOD:
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_FOOD"));	break;
		case ITEMSOUND_HEAL:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HEAL"));	break;
		case ITEMSOUND_HELMET:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HELMET"));	break;
		case ITEMSOUND_SWORD:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_SWORD"));	break;
		case ITEMSOUND_MEAT:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_FOOD"));	break;
		case ITEMSOUND_NOODLE:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_FOOD"));	break;
		case ITEMSOUND_SOUP:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_FOOD"));	break;
		case ITEMSOUND_VEGETABLE:		
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_FOOD"));	break;
		case ITEMSOUND_AXE:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_AXE"));		break;
		case ITEMSOUND_BELT:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_BELT"));	break;
		case ITEMSOUND_ETC:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_ETC"));		break;
		case ITEMSOUND_GUN:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_GUN"));		break;
		case ITEMSOUND_NECK:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_NECK"));	break;
		case ITEMSOUND_SHOES:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_SHOES"));	break;
		case ITEMSOUND_STAFF:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_STAFF"));	break;
		case ITEMSOUND_TICKET:			
			effect = 0;						break;
		case ITEMSOUND_CERAMIC:			
			effect = 0;						break;
		case ITEMSOUND_SMELLHEAL:		
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HEAL"));	break;
		case ITEMSOUND_UNFREEZEHEAL:	
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HEAL"));	break;
		}
		break;

	case ITEMSOUND_USE:
		switch(soundtype)
		{
		case ITEMSOUND_BOOK:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_USE_BOOK"));		break;	
		case ITEMSOUND_BOW:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_BOW"));		break;
		case ITEMSOUND_CLOTH:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_CLOTH"));	break;
		case ITEMSOUND_FOOD:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_FOOD"));	break;
		case ITEMSOUND_HEAL:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_USE_HEAL"));		break;
		case ITEMSOUND_HELMET:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_HELMET"));	break;
		case ITEMSOUND_SWORD:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_SWORD"));	break;
		case ITEMSOUND_MEAT:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_USE_MEAT"));		break;
		case ITEMSOUND_NOODLE:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_USE_NOODLE"));		break;
		case ITEMSOUND_SOUP:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_USE_SOUP"));		break;
		case ITEMSOUND_VEGETABLE:		
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_USE_VEGETABLE"));	break;
		case ITEMSOUND_AXE:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_AXE"));		break;
		case ITEMSOUND_BELT:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_BELT"));	break;
		case ITEMSOUND_ETC:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_ETC"));		break;
		case ITEMSOUND_GUN:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_GUN"));		break;
		case ITEMSOUND_NECK:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_NECK"));	break;
		case ITEMSOUND_SHOES:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_SHOES"));	break;
		case ITEMSOUND_STAFF:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELECT_STAFF"));	break;
		case ITEMSOUND_TICKET:			
			effect = 0;							break;
		case ITEMSOUND_CERAMIC:			
			effect = 0;							break;
		case ITEMSOUND_SMELLHEAL:		
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_USE_SMELLHEAL"));		break;
		case ITEMSOUND_UNFREEZEHEAL:	
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_USE_UNFREEZEHEAL"));	break;
		}
		break;
	case ITEMSOUND_REPAIR:
		switch(soundtype)
		{
		case ITEMSOUND_BOOK:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;	
		case ITEMSOUND_BOW:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_CLOTH:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_CLOTH"));	break;
		case ITEMSOUND_FOOD:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_HEAL:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_HELMET:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_CLOTH"));	break;
		case ITEMSOUND_SWORD:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_MEAT:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_NOODLE:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_SOUP:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_VEGETABLE:		
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_AXE:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_BELT:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_CLOTH"));	break;
		case ITEMSOUND_ETC:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_GUN:				
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_NECK:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_SHOES:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_CLOTH"));	break;
		case ITEMSOUND_STAFF:			
			effect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIR_SWORD"));	break;
		case ITEMSOUND_TICKET:			
			effect = 0;						break;
		case ITEMSOUND_CERAMIC:			
			effect = 0;						break;
		case ITEMSOUND_SMELLHEAL:		
			effect = 0;						break;
		case ITEMSOUND_UNFREEZEHEAL:	
			effect = 0;						break;
		}
		break;

	default:
		#ifdef _DEBUG
			MsgBox(TEXT("사운 타입 설정 오류"), TEXT("%d 타입의 사운드가 없습니다"), type);
		#else
			MsgBox(TEXT("fdsfs9vj"), TEXT("fds3f:%d"), type);
		#endif
	}


	if(effect)
	{
		PushEffect(effect);
	}
}

bool cltItemManagerClient::GetMadePersonName(SI32 ID, TCHAR* PersonName, SI32 namesize)
{	
	TCHAR buf[MAX_PLAYER_NAME];

	if (pclClient->GameMode == GAMEMODE_CLIENT)
	{
		if (pclClient->pclPersonNameManager->Find(ID, buf))
		{
			StringCchCopy(PersonName, namesize, buf);
			return true;
		}
		else
		{
			cltGameMsgRequest_GetNameFromPersonID clPersonName(ID);
			cltMsg	clMsg(GAMEMSG_REQUEST_PERSONNAME_FROMPERSONID, sizeof(clPersonName), (BYTE*)&clPersonName);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}

	return false;

}

bool cltItemManagerClient::GetMadePersonNameNew(SI32 ID, cltPersonNameInfo* _pclPersonInfo, SI32 namesize)
{	
	if (pclClient->GameMode == GAMEMODE_CLIENT)
	{
		/*static DWORD TickCount = GetTickCount();
		if( 2000 > GetTickCount() - TickCount )	
		{
			TickCount = GetTickCount();
			return  false;
		}*/
		

		cltPersonNameInfo clPersonInfo;
		if( pclClient->pclNewPersonNameManager->Find( ID, &clPersonInfo ) )
		{
			*_pclPersonInfo = clPersonInfo;
			return true;
		}
		else
		{
			cltGameMsgRequest_GetNameFromItemSerialID clPersonName(ID);
			cltMsg	clMsg(GAMEMSG_REQUEST_PERSONNAME_SERIALID, sizeof(clPersonName), (BYTE*)&clPersonName);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}

	return false;

}


//[진성] 아아템 큰 이미지 세팅. => 2008-9-23
void cltItemManagerClient::SetItemImageBigList(void)
{
	FILE* fp = _tfopen(TEXT("Item\\ItemImageBigList.txt"), TEXT("rt"));
	if( fp == NULL )
	{
#ifdef _DEBUG
		MsgBox(TEXT("파일 읽기 오류"), TEXT("ItemImageBigList.txt 파일을 읽지 못했습니다"));
#else
		MsgBox(TEXT("AAAAAAAAA1"), TEXT("BBBBBBBBB1"));
#endif
		return ;
	}

	TCHAR buffer[256];
	for(SI32 i = 0; i < 6; i++)
	{
		_fgetts(buffer, sizeof(buffer), fp);
	}

	SI32 siCount	  = 0;
	SI32 siIndex	  = 0;
	SI32 siItemUnique = 0;
	while(_fgetts(buffer, sizeof(buffer), fp) != NULL)
	{
		_stscanf(buffer, TEXT("%d%d"), &siIndex, &siItemUnique );
		if( -1 == siIndex )	break;

		if(siCount != siIndex)
		{	
#ifdef _DEBUG
			MsgBox(TEXT("이미지 번호 설정 오류"), TEXT("ItemImageBigList.txt파일에 아이템의 인덱스번호의 순서가 틀려~. ") );
#else
			MsgBox(TEXT("AAAAAAAAA4"), TEXT("BBBBBBBBB4"));
#endif
		}

		m_ItemImageBigUnique[siCount++] = ITEMUNIQUE(siItemUnique);// 유니크 번호를 저장.

		if(siCount >= MAX_ITEM_IMAGE_NUM)
		{
#ifdef _DEBUG
			MsgBox(TEXT("이미지 번호 설정 오류"), TEXT("아이템 이미지의 설정 가능한 겟수를 초과했습니다") );
#else
			MsgBox(TEXT("AAAAAAAAA3"), TEXT("BBBBBBBBB3"));
#endif
		}
	}
}

//[진성] 아아템 큰 이미지 파일 이름 얻기. => 2008-9-23
bool cltItemManagerClient::GetItemImageBigFileName( IN SI16 _ItemUnique, OUT TCHAR** _FileName, OUT SI32* _ImageIndex)
{
	static TCHAR FileName[128] = "";
	ZeroMemory(FileName, sizeof(FileName));
	const SI32 FileMaxImgNum = 200;

	SI32 ImageIndex = -1;
	SI32 Count		=  0;
	for(		; Count < MAX_ITEM_IMAGE_NUM; ++Count )
	{	
		if( _ItemUnique == m_ItemImageBigUnique[Count] )
		{
			ImageIndex = Count;
			StringCchPrintf( FileName, sizeof(FileName), "GImg/ITEMIMGBIG%d.SPR", Count/FileMaxImgNum );
			break;
		}
	}

	if( 0 > ImageIndex )	
	{
		*_ImageIndex = -1;
		*_FileName   = NULL;
		return false;
	}
	else
	{
		*_ImageIndex = Count;
		*_FileName   = FileName;
		return true;
	}
}


