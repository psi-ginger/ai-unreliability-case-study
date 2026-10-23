#include "Char\CharManager\CharManager.h"
#include "..\Client\Client.h"
#include "..\Common\Order\Order.h"

//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

// 캐릭터에게 직접적으로 명령을 전달한다.
BOOL cltCommonLogic::LetOrder(cltOrderData *porder, SI32 id)
{
	if(porder == NULL)return FALSE;

	if(id == 0)
	{
		id = pclCM->GetIDFromCharUnique(porder->CharUnique);
	}

	// 부활 명령이 아니면 죽은 캐릭터에게 줄 수 없다.
	if(porder->ODI.siOrder != ORDER_REVIVE)
	{
		if(pclCM->IsAlive(id)==FALSE)return FALSE;
	}

	cltOrderDataInfo odi(&porder->ODI);

	//--------------------------------------------
	// CharUnique 정보를 Id 정보로 교환한다. 
	//--------------------------------------------
    odi.siPara2		=	pclCM->GetIDFromCharUnique(porder->ODI.siPara2) ;

	pclCM->CR[id]->PushOrder(odi);
	
	return TRUE;
}



