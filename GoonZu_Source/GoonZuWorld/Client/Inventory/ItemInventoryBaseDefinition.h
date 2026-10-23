#ifndef _ITEMINVENTORYBASEDEFINITION_H_H
#define _ITEMINVENTORYBASEDEFINITION_H_H

#include <Directives.h>
#include <commctrl.h> 
#include <stdio.h>

#include "..\..\Resource.h"
#include "..\..\Common\Item\ItemCommon\ItemUnit.h"

//------------------------------------------- 인벤토리 백그라운드 이미지 
#define MAX_ITEMDLG_BACKGROUND_BITMAP_NUM	2

#define MAX_BUFFER_INVENTORY_SIZE			384

#define	INVENTORY_SETTIMER_VALUE			1000

//------------------------------------------- 툴팁 사이즈
#define MAX_TOOLTIP_WIDTH					150

//------------------------------------------- 아이템 이미지 사이즈
#define	ITEM_IMAGE_WIDTH					48
#define ITEM_IMAGE_HEIGHT					48

//------------------------------------------- 
#define	MAX_IMAGELIST_NUM					100			// 이미지 리스트에 등록하는 최대 개수


//------------------------------------------- 툴팁의 가로 최대 길이
#define	MAX_TOOLTIP_LENGTH					200


//------------------------------------------- 인벤토리에 사용되는 메시지들
#define	INV_MOVE_ITEM						20000		// 아이템 이동시 si32lParam 에서 LOWORD 는 움직일 인벤토리 이고, HIWORD는 목표하는 인벤토리 이다.
#define	INV_USE_ITEM						20001


// si32lParam = 아이템 위치, clMoveItem = 아이템
#define	INV_REPAIR_ITEM						20002		// 아이템 수리

#define	INV_DROP_ITEM						20003

#define	INV_MOUNT_ITEM						20004		// 더블 클릭시에 아이템 장착 및 해제


// si32lParam 은 생산 가능 아이템의 index, lParam1 은 생산 개수
#define	INV_MAKE_ITEM						20005		// 인벤토리 내에서 아이템 생산시에 

#define INV_ADD_ITEM						20006		// 인벤토리 내에서 아이템을 합침

#define INV_USE_NOTIFY_MSG_ITEM				20007		// 공지 메시지 아이템을 사용한다.

#define	INV_CHANGE_ITEM						20008       //인벤에 아이템 바꾸기 

#define INV_CANCELLATIONSEALINGUP_ITEM		20009		// 밀봉 아이템 해제하기

#define	INV_ENCHANT_ITEM					20010		// 인챈트창에서 인챈트하기(by taeyang)

#define INV_SPLIT_ITEM						20011		// 아이템 분리

struct stMoveData
{
	SI16	si16Message;
	SI32	si32lParam;
	SI32	si32lParam1;								// 개수
	SI16	si16lParam2;								// 유니크
	cltItem clMoveItem;	
};

struct stUseNotifyMsg
{
	SI16	si16Message;
	TCHAR	strNotifyMsg[ 256 ];						// 공지 메시지.
};

struct	stImageList					// 이미지 리스트를 제대로 관리하기 위해서...
{
	bool	bImageStatus;			// 이미지가 차있는 상태
	SI32	si32Index;
	SI32	si32ItemUnique;

	stImageList()
	{
		memset( this, 0, sizeof( stImageList ) );
	}
};

#endif