#include "Ask.h"
#include "CommonLogic.h"
#include "..\..\Client\Client.h"
#include "..\Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"
#include "..\..\Resource.h"

// 귓속말 채팅 관련. 
//#include "..\..\Client\Interface\PrivateChatDlg\PrivateChatBaseInfo.h"
//#include "..\..\Client\Interface\PrivateChatDlg\PrivateChatDlgMgr.h"

#include "MsgType-System.h"

extern cltCommonLogic* pclClient;

/*
cltAskManager::cltAskManager()
{
	m_hDlg = NULL;
	bDlgRectInitSwitch	= false;
	
	siIndex				= 0;

	bUpdateSwitch		= false;
}


cltAskManager::~cltAskManager()
{
}

// 정보를 갱신한다. 
void cltAskManager::Update()
{
	SI32 i;
	TCHAR buffer[256];

	// 기존의 리스트를 삭제한다. 
	ListDelete(m_hDlg, IDC_LIST_ASK);

	for(i = 0;i < MAX_ASK_NUMBER;i++)
	{
		if(clUnit[i].siCharUnique <= 0)continue;
		
		sprintf(buffer, TEXT("%s-%s"), clUnit[i].szPersonName, clUnit[i].szAskText);
		SI32 index = ListAddString(m_hDlg, IDC_LIST_ASK, (LPARAM)buffer);
		
		// 리스트 항목에 데이터를 연결한다. 
		ListSetItemData(m_hDlg, IDC_LIST_ASK, index, (LPARAM)i);

	}
}

// 질문을 추가한다. 
bool cltAskManager::Add(SI16 charunique, SI32 personid, TCHAR* ppersonname, TCHAR* ptext)
{
	SI32 i;
	SI32 index = -1;

	//--------------------------------------
	// 같은 사용자의 질문이 있는지 확인한다. 
	//--------------------------------------
	for(i = 0;i < MAX_ASK_NUMBER;i++)
	{
		if(clUnit[i].siPersonID == personid)
		{
			index = i;
			break;
		}
	}

	// 이미 질문을 올려 놓은 것이 있는 경우 그 질문을 덮어쓴다. 
	if(index >= 0)
	{
		clUnit[index].siCharUnique	= charunique;
		clUnit[index].siPersonID	= personid;
		clUnit[index].siSerialNum	= siIndex;

		MStrCpy( clUnit[index].szPersonName, ppersonname, MAX_PLAYER_NAME );
		MStrCpy( clUnit[index].szAskText, ptext, MAX_ASK_TEXT_SIZE );


		siIndex++;

		return true;

	}
	// 가장 오래된 질문을 찾는다. 
	else
	{
		SI32 minindex	= 2000000000;
		SI32 minpos		= -1;
		for(i = 0;i < MAX_ASK_NUMBER;i++)
		{
			if(clUnit[i].siSerialNum < minindex)
			{
				minindex	= clUnit[i].siSerialNum;
				minpos		= i;
			}
		}

		if(minpos >= 0)
		{
			clUnit[minpos].siCharUnique	= charunique;
			clUnit[minpos].siPersonID	= personid;
			clUnit[minpos].siSerialNum	= siIndex;

			MStrCpy( clUnit[minpos].szPersonName, ppersonname, MAX_PLAYER_NAME );
			MStrCpy( clUnit[minpos].szAskText, ptext, MAX_ASK_TEXT_SIZE );
	
			siIndex++;

			return true;
		}

	}

	return false;
}

// personid를 가지고 질문을 삭제한다. 
void cltAskManager::DelAskPersonID(SI32 personid)
{
	for(SI32 i = 0;i < MAX_ASK_NUMBER;i++)
	{
		if(clUnit[i].siCharUnique == 0)continue;

		if(clUnit[i].siPersonID == personid)
		{
			clUnit[i].Init();
		}
	}
	
}

BOOL cltAskManager::CreateDlg(HINSTANCE hinst, HWND hwnd)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{

		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_ASK), hwnd, StaticAskDlgProc);

		if(bDlgRectInitSwitch == true)
		{
			MoveWindow(m_hDlg, 
				DlgRect.left, DlgRect.top, 
				DlgRect.right - DlgRect.left, 
				DlgRect.bottom - DlgRect.top, FALSE);
		}

		ShowWindow(m_hDlg, SW_SHOW);

		return TRUE;
	}
	else
	{
		DestroyWindow(m_hDlg);
	}


	return 0;
}

BOOL CALLBACK cltAskManager::StaticAskDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	return pclClient->pclAskManager->AskDlgProc(hDlg, iMsg, wParam, lParam);
}


BOOL CALLBACK cltAskManager::AskDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			SetTimer(hDlg, 1, 1000, NULL);

			// 기존의 리스트를 삭제한다. 
			ListDelete(hDlg, IDC_LIST_ASK);

			//-----------------------------------------
			// 질문 정보를 초기화한다. 
			//-----------------------------------------
			for(SI32 i = 0;i < MAX_ASK_NUMBER;i++)
			{
				clUnit[i].Init();
			}


			//-------------------------------------------------
			// 게임서버로 질문 리스트를 요청한다. 
			//-------------------------------------------------
			cltMsg clMsg(GAMEMSG_REQUEST_ASKLIST, 0, NULL);
			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		return TRUE;

	case WM_NOTIFY:
		return TRUE;

	case WM_TIMER:
		if(bUpdateSwitch == true)
		{
			bUpdateSwitch = false;

			Update();
		}
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{

				//----------------------------------------------------------
				// 선택한 질문을 찾는다. 
				//----------------------------------------------------------
				SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_ASK);
				if(index < 0)break;

				SI32 pos = SendDlgItemMessage(hDlg, IDC_LIST_ASK, LB_GETITEMDATA,	index, 0);
				if(pos < 0)break;
				
				//----------------------------------------------------------
				// 서버에 메시지를 삭제하도록 요청한다. 
				//----------------------------------------------------------
				cltGameMsgResponse_DelAsk clinfo(clUnit[pos].siPersonID);
				cltMsg clMsg(GAMEMSG_REQUEST_DELASK, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);


				//-------------------------------------------
				//귓말 
				//-------------------------------------------
				SI32 charunique		= clUnit[pos].siCharUnique;
				TCHAR* pplayername	= clUnit[pos].szPersonName;

				cltClient* pclclient = (cltClient*)pclClient;

				//pclclient->pclPrivateChatDlgMgr->Push(charunique, pplayername, TEXT(""));
				pclclient->RequestSecretChatByCharUnique(charunique);

				//---------------------------------------
				// 해당 메시지를 클라이언트에서 삭제한다.
				//---------------------------------------
				DelAskPersonID(clUnit[pos].siPersonID);
				bUpdateSwitch = true;

			}
			return TRUE;
			
		case IDCANCEL:
			DestroyWindow(m_hDlg);
			return TRUE;
		}
		return TRUE;
		
	case WM_DESTROY:


		//-------------------------
		// 질문 리스트를 삭제한다. 
		//-------------------------
		for(SI32 i = 0;i < MAX_ASK_NUMBER;i++)
		{
			clUnit[i].Init();
		}

		// 타이머를 해제한다. 
		KillTimer(hDlg, 1);

		// 윈도우의 위치를 기억한다. 
		GetWindowRect(hDlg, &DlgRect);
		bDlgRectInitSwitch = true;

		m_hDlg = NULL;
		return TRUE;

	}

	return FALSE;
}
*/