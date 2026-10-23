#include "..\Client\Client.h"
#include "Key.h"


cltKeyManager::cltKeyManager()
{
	Init();
}


cltKeyManager::~cltKeyManager()
{

}



void cltKeyManager::Push(cltKey* pclkey)
{

	if(siKeyInputIndex >= MAX_KEY_INPUT)return ;

	if(siKeyInputIndex > 0 
		&& pclkey->siLeftPressMode		== KEY_MODE_NONE
		&& pclkey->siRightPressMode	== KEY_MODE_NONE
		&& pclkey->wParam				== 0)	
	{
		if(clKey[siKeyInputIndex - 1].siLeftPressMode		== KEY_MODE_NONE 
		&& clKey[siKeyInputIndex - 1].siRightPressMode		== KEY_MODE_NONE
		&& clKey[siKeyInputIndex - 1].siDoubleLeftPressMode == KEY_MODE_NONE
		&& clKey[siKeyInputIndex - 1].wParam				== 0)
		{
			clKey[siKeyInputIndex - 1].Set(pclkey);
			return ;
		}
	}

	clKey[siKeyInputIndex].Set(pclkey);

	siKeyInputIndex++;

}

bool cltKeyManager::Pop(cltKey* pclkey)
{

	int i;

	if(siKeyInputIndex <= 0)return false;
	
	pclkey->Set(&clKey[0]);

	for(i = 0;i	< (siKeyInputIndex - 1);i++)
	{
		clKey[i].Set(&clKey[i + 1]);
	}

	siKeyInputIndex --;
	
	return true;
	   
}