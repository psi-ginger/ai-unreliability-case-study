#ifndef _PERSONALSHOPBASEINFO_H_
#define _PERSONALSHOPBASEINFO_H_

#include <Directives.h>
#include <windows.h>

#include "..\..\..\Resource.h"

#include "..\..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "..\..\..\Common\SystemNPC\ForeignMerchantNPC\ForeignMerchantNPC.h"
#include "..\..\..\CommonLogic\ImageList\ImageList.h"

#include "../../../Common/Char/CharCommon/Char-EconomyInfo.h"
#include "..\..\FontManager\FontManager.h"
#include "..\..\Inventory\ItemInventoryBaseDefinition.h"
#include "..\..\..\CommonLogic\Msg\MsgType-PrivateTrade.h"
#include "..\..\..\CommonLogic\Stock\Stock.h"


#define PERSONALSHOPDLG_SETTIMER_TIME		300

class CPersonalShopDlg;
extern CPersonalShopDlg g_PersonalShopDlg;

#endif