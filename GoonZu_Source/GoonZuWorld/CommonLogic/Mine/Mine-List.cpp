#include <CommonLogic.h>
#include "..\..\Server\Server.h"

#include "Char\CharCommon\Char-Common.h"
#include "..\..\Common\Util\Control\Control.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\Resource.h"

#include "Mine.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Mine.h"

extern cltCommonLogic* pclClient;




// 시전리스트를 만든다.
BOOL cltMineManager::MakeMineUnitList(HWND hDlg)
{

	// 이미지 리스트를 리스트 뷰 컨트롤에 등록한다. 
	HWND hwnd = GetDlgItem(hDlg, IDC_LIST_MINEUNITLIST);
	
	if(hImageList)
	{
		SendMessage(hwnd, LVM_SETIMAGELIST, (WPARAM)LVSIL_SMALL, (LPARAM)hImageList);
	}

	ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);
	

	// 컬럼을 추가한다. 
	char* pText = GetTxtFromMgr(1695);
	ListViewInsertColumn(hwnd, 60,		pText,		0, 0);
	pText = GetTxtFromMgr(1696);
	ListViewInsertColumn(hwnd, 90,		pText,	1, 0);
	pText = GetTxtFromMgr(1697);
	ListViewInsertColumn(hwnd, 180,		pText,		2, 0);

	return TRUE;
}


// 시전리스트를 보여준다. 
BOOL cltMineManager::ShowMineUnitList(HWND hDlg)
{
	SI32 i;
	char buffer[256];

	HWND hwnd = GetDlgItem(hDlg, IDC_LIST_MINEUNITLIST);

	// 리스트뷰를 모두 지운다. 
	ListviewDelete(hDlg, IDC_LIST_MINEUNITLIST);

	for( i = (MAX_MINEUNIT_NUMBER-1); i >= 0; i-- )
	{
		sprintf(buffer, "%d", i+1);
		ListViewInsertItem(hwnd, 0,		0, 0, buffer, i);

		if(clClientUnit[i].clPerson.GetPersonID())
		{
			ListViewInsertItem(hwnd, -1,	0, 1, (char*)clClientUnit[i].clPerson.GetName(), -1);
			ListViewInsertItem(hwnd, -1,	0, 2, "NOTEXT", -1);
		}
		else
		{
			char* pText = GetTxtFromMgr(3245);
			ListViewInsertItem(hwnd, -1,	0, 1, pText, -1);
		}
	}
	
	return TRUE;
}