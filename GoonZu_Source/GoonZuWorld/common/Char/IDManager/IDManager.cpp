//---------------------------------
// 2003/5/17 ���°�
//---------------------------------

#include "IDManager.h"
#include <CommonLogic.h>
#include "..\..\..\Server\TraceBoot.h"

extern cltCommonLogic* pclClient;

cltIDManager::cltIDManager(SI32 maxidnumber, SI32 ssessionnumber, SI32 instancenpc)
{
	SI32 i;

	MaxIDNumber			= maxidnumber;
	MaxSessionNumber	= ssessionnumber;
	MaxInstanceNPC		= instancenpc;

	UserCharNumber	= 0;
	NPCCharNumber	= 0;
	
	ActiveNPCNumber = 0;
	InstanceNPCNumber = 0;

	for(i = 0;i < MAX_CHAR_NUMBER;i++)
	{
		clIDInfo[i].Init();
	}

	pCharOrder	= new SI32[MaxIDNumber];
	for(i = 0;i < MaxIDNumber;i++)
	{
		pCharOrder[i] = 0;
	}

	CharOrderIndex	= 0;
}


cltIDManager::~cltIDManager()
{
	if(pCharOrder)
	{
		delete[] pCharOrder;
		pCharOrder = NULL;
	}
}


SI32 cltIDManager::GetID(SI32 mode)
{
	SI32 i;

	// ����� 
	if(mode == 1)
	{
		for(i = 1;i < MaxSessionNumber;i++)
		{
			if(clIDInfo[i].siKind == 0)return i;
		}
	}
	// InstanceNPC
	else if( mode == 2 )
	{
		for(i = (MaxSessionNumber );i < (MaxSessionNumber +MaxInstanceNPC);i++)
		{
			if(clIDInfo[i].siKind == 0)return i;
		}
	}
	// NPC
	else
	{
		for(i = (MaxSessionNumber +MaxInstanceNPC);i < MaxIDNumber;i++)
		{
			if(clIDInfo[i].siKind ==0)return i;
		}
	}
	return 0;

}




// id�� �Ҵ�Ǿ� �ִ��� Ȯ���Ѵ�. 
bool cltIDManager::IsValidID(SI32 id)
{
	if(id <= 0 || id >= MAX_CHAR_NUMBER)return false;

	if(clIDInfo[id].siKind)
	{
		return true;
	}

	return false;
}


BOOL cltIDManager::DeleteChar(SI32 id)
{
	SI32 i;

	if(CharOrderIndex == 0)return FALSE;
	if(id < 0 || id >= MaxIDNumber)return FALSE;

	for(i = 0;i < CharOrderIndex;i++)
	{
		if(pCharOrder[i] == id)
		{
			// ����� ������ �ڸ��� �� ������ ������ �ִ´�.
			pCharOrder[i]					= pCharOrder[CharOrderIndex-1];
			pCharOrder[CharOrderIndex-1]	= 0;
			CharOrderIndex--;

			if(clIDInfo[id].bPC)
			{
				SubUserCharNumber();
			}
			else if(clIDInfo[id].bInstance)
			{
				SubInstanceNPCNumber();
			}
			else
			{
				SubNPCCharNumber();
			}
			
			// id�� idǮ�� �����ش�. 
			clIDInfo[id].Init();


			return TRUE;
		}
	}

	return FALSE;
}


// ĳ���͸� �߰��Ѵ�. 
SI32 cltIDManager::AddChar(SI32 id, SI32 kind, bool bpcswitch, bool binstanceswitch)
{
	TraceBoot("ADDCHAR enter id=%d kind=%d pc=%d inst=%d existingKind=%d CharOrderIndex=%d MaxIDNumber=%d",
		id, kind, (int)bpcswitch, (int)binstanceswitch, clIDInfo[id].siKind, CharOrderIndex, MaxIDNumber);

	if(CharOrderIndex >= MaxIDNumber)return -1;
	if(id < 0 || id >= MaxIDNumber)return -1;

	// ������ ���� ���̵� �����ؼ��� �ȵȴ�. 
	if(clIDInfo[id].siKind)
	{
		if (pclClient->GameMode == GAMEMODE_SERVER)
			pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("cltIDManager::AddChar  id:%d kind1:%d kind2:%d"), id, clIDInfo[id].siKind, kind);
		return -1;
	}

	clIDInfo[id].Set(kind, bpcswitch, binstanceswitch);

	pCharOrder[CharOrderIndex ] = id;
	CharOrderIndex++;

	TraceBoot("ADDCHAR OK id=%d -> pCharOrder[%d], CharOrderIndex now %d", id, CharOrderIndex-1, CharOrderIndex);

	if(bpcswitch == true)
	{
		AddUserCharNumber();
	}
	else if(binstanceswitch == true )
	{
		AddInstanceNPCNumber();
	}
	else
	{
		AddNPCCharNumber();
	}

	return CharOrderIndex - 1;
}

//=========================================
// cltIDManager �߰�
//=========================================

SI32 cltIDManager::GetMaxIDNumber() const 
{
	return MaxIDNumber;
}

SI32 cltIDManager::GetCharOrderIndex() const
{
	return CharOrderIndex;
}

SI32 cltIDManager::GetCharOrder(SI32 index) const
{
	if( CharOrderIndex <=0 )					return 0;
	if( index < 0 || index >= CharOrderIndex )	return 0;	// �ݵ�� �˻簡 ���� �Ѵ�.
	return pCharOrder[index];
}

SI32 cltIDManager::GetUserCharNumber() const
{
	return UserCharNumber;
}

void cltIDManager::AddUserCharNumber()
{
	UserCharNumber++;
}

void cltIDManager::SubUserCharNumber()
{
	UserCharNumber--;
}

SI32 cltIDManager::GetNPCCharNumber()const 
{
	return NPCCharNumber;
}

void cltIDManager::AddNPCCharNumber()
{
	NPCCharNumber++;
}

void cltIDManager::SubNPCCharNumber()
{
	NPCCharNumber--;
}

SI32 cltIDManager::GetActiveNPCNumber()const
{
	return ActiveNPCNumber;
}

void cltIDManager::AddActiveNPCNumber()
{
	ActiveNPCNumber++;
}

void cltIDManager::SubActiveNPCNumber()
{
	ActiveNPCNumber--;
}

SI32 cltIDManager::GetInstanceNPCNumber() const
{
	return InstanceNPCNumber;
}

void cltIDManager::AddInstanceNPCNumber()
{
	InstanceNPCNumber++;
}

void cltIDManager::SubInstanceNPCNumber()
{
	InstanceNPCNumber--;
}

SI32 cltIDManager::GetTotalCharNumber() const
{
	return GetUserCharNumber() + GetNPCCharNumber()  + GetInstanceNPCNumber() ;
}

//=========================================
// cltIDInfo
//=========================================
cltIDInfo::cltIDInfo()
{
	Init();
}

void cltIDInfo::Init()
{
	siKind	= 0;
	bPC		= 0;
	bInstance = 0;
}

void cltIDInfo::Set(SI32 kind, bool bnpc, bool binstance)
{
	siKind	= kind;
	bPC		= bnpc;
	bInstance = binstance;
}