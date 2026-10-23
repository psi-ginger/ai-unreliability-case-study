//---------------------------------
// 2004/2/5 김태곤
//---------------------------------
#include "..\Client.h"
#include "VillageDetailInfo.h"
#include "..\..\Resource.h"

#include "Msg\MsgType-Village.h"
#include "MsgType-Structure.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

cltVillageDetailData clvillagedata[]={
	VILLAGEDETAILINFOTYPE_VILLAGELIST,		"", "", 							
	VILLAGEDETAILINFOTYPE_VILLAGELEVEL,		"",	"", 								
	VILLAGEDETAILINFOTYPE_VISITORNUM,		"",	"", 								
	VILLAGEDETAILINFOTYPE_VILLAGEMONEY,		"",	"", 								
	VILLAGEDETAILINFOTYPE_VILLAGEPROFIT,	"",	"", 								
	VILLAGEDETAILINFOTYPE_VILLAGESTOCK,		"",	"", 								
	VILLAGEDETAILINFOTYPE_GENERALMEETING,	"",	"", 								
	VILLAGEDETAILINFOTYPE_PEOPLE,			"",	"", 								
	VILLAGEDETAILINFOTYPE_WAR,				"",	"", 								
	VILLAGEDETAILINFOTYPE_WARPERRESIDENT,	"", "", 								
	VILLAGEDETAILINFOTYPE_USER,				"",	"", 								
	VILLAGEDETAILINFOTYPE_PREVYEARDBID,		"",	"", 								
	VILLAGEDETAILINFOTYPE_DECLAREWAR,		"",	"", 								
	VILLAGEDETAILINFOTYPE_HUNT,				"",	"", 								
	VILLAGEDETAILINFOTYPE_HORSEMARKET,		"",	"", 								
	VILLAGEDETAILINFOTYPE_STOCK,			"",	"", 								
	VILLAGEDETAILINFOTYPE_REALESTATEMARKET,	"",	"",								
	VILLAGEDETAILINFOTYPE_SUMMONMARKET,		"",	"", 								
	VILLAGEDETAILINFOTYPE_SUMMONHEROMARKET,	"",	"", 								
	VILLAGEDETAILINFOTYPE_PORT,				"",	"", 								
	VILLAGEDETAILINFOTYPE_SHIPMARKET,		"",	"", 								
	VILLAGEDETAILINFOTYPE_FARM,				"",	"", 	
	VILLAGEDETAILINFOTYPE_FEAST,			"",	"", 	
	VILLAGEDETAILINFOTYPE_MINE,				"",	"",
	VILLAGEDETAILINFOTYPE_GUILD,			"", "",
	VILLAGEDETAILINFOTYPE_CITYHALLSTATE,	"", "",
	0,										"", ""												
};

cltVillageDetailInfoManager::cltVillageDetailInfoManager()
{
	for(SI32 index=VILLAGEDETAILINFOTYPE_VILLAGELIST-1;index<VILLAGEDETAILINFOTYPE_MINE;index++)
	{
		char* pText = GetTxtFromMgr(3643+index);
		strcpy(clvillagedata[index].szTypeText,pText);
	}

	// 상단
	{
		char* pText = GetTxtFromMgr(5360);
		strcpy(clvillagedata[ VILLAGEDETAILINFOTYPE_GUILD - 1 ].szTypeText,pText);
	}

	// 관청 상태 - 파산 등
	{
		char* pText = GetTxtFromMgr(5403);
		strcpy(clvillagedata[ VILLAGEDETAILINFOTYPE_CITYHALLSTATE - 1 ].szTypeText,pText);
	}


	for(SI32 index=VILLAGEDETAILINFOTYPE_HORSEMARKET-1;index<VILLAGEDETAILINFOTYPE_MINE;index++)
	{
		char* pExplainText=GetTxtFromMgr(3667+index-14);
		strcpy(clvillagedata[index].szExplainText,pExplainText);
	}

	// 상단
	{
		char* pText = GetTxtFromMgr(5405);
		strcpy(clvillagedata[ VILLAGEDETAILINFOTYPE_GUILD - 1 ].szExplainText,pText);
	}

	// 관청 상태 - 파산 등
	{
		char* pText = GetTxtFromMgr(5404);
		strcpy(clvillagedata[ VILLAGEDETAILINFOTYPE_CITYHALLSTATE - 1 ].szExplainText,pText);
	}


	m_hDlg = NULL;
	bDlgRectInitSwitch = false;
	bCheckSortVillage	= false;
	siCurrentType		= 0;
	siOldCurrentType	= 0;
	siOldViliage		= 0;
	m_hEditBrush		= NULL;
	m_hWarpBitmap		= NULL;

}


cltVillageDetailInfoManager::~cltVillageDetailInfoManager()
{

}


BOOL cltVillageDetailInfoManager::CreateDlg(HINSTANCE hinst, HWND hwnd)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{

		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_VILLAGEINFO), hwnd, StaticDlgProc);

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

BOOL CALLBACK cltVillageDetailInfoManager::StaticDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;
	return pclclient->pclVillageDetailInfoManager->DlgProc(hDlg, iMsg, wParam, lParam);
}


BOOL CALLBACK cltVillageDetailInfoManager::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;
	SI32 id = 1;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			SetTimer(hDlg, 1, 1000, NULL);

			siCurrentType		= VILLAGEDETAILINFOTYPE_VILLAGELIST;
			siOldCurrentType	= 0;
			siOldViliage		= 0;
			SetDataTypeInCombo(hDlg, IDC_COMBO_VILLAGEDETAIL);

			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE);

			m_hWarpBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface/HitButton/VillageWarp.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

			if ( m_hWarpBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_BUTTON_WARPTOVILLAGE, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hWarpBitmap);
			}

		}
		return TRUE;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_VILLAGEINFO ) == (HWND)lParam)
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
			}
		}
		break;
	case WM_NOTIFY:
		return TRUE;

	case WM_TIMER:
		{
			// 마을 이름순 정렬 보이기, 안보이기
			if(siCurrentType == VILLAGEDETAILINFOTYPE_VILLAGELIST)
				ShowWindow(GetDlgItem(hDlg,IDC_CHECK_SORTVILLAGE), SW_SHOW);
			else
				ShowWindow(GetDlgItem(hDlg,IDC_CHECK_SORTVILLAGE), SW_HIDE);
			// 선택된 항목이 있다면,
			if(siCurrentType && (siCurrentType != siOldCurrentType))
			{
				siOldCurrentType = siCurrentType;

				// 적대적인 관계는 클라이언트에서 이미 파악하고있다. 
				if(siCurrentType == VILLAGEDETAILINFOTYPE_DECLAREWAR)
				{
					// 기존의 리스트를 삭제한다. 
					ListDelete(m_hDlg, IDC_LIST_VILLAGEDETAIL);
					
					SI32 i, j;
					for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
					{
						if(pclClient->pclVillageManager->pclVillageInfo[i] == NULL)continue;

						for(j = 0;j < MAX_VILLAGE_NUMBER;j++)
						{
							if(pclClient->pclVillageManager->pclVillageInfo[j] == NULL)continue;

							// 선전 포고가 되어 있으면, 리스트에 추가한다. 
							if(pclClient->pclVillageManager->siDeclareWarVillage[i] == j)
							{
								char buffer[256];
								char* pText = GetTxtFromMgr(1412);

								Nsprintf(buffer,pText,"who",pclClient->pclVillageManager->GetName(i)    ,"whom",  pclClient->pclVillageManager->GetName(j)  ,NULL);
								//sprintf(buffer, pText, 
								//	pclClient->pclVillageManager->GetName(i),
								//	pclClient->pclVillageManager->GetName(j));
								SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);

							}
						}
					}

				}
				else if(siCurrentType == VILLAGEDETAILINFOTYPE_VILLAGELIST)
				{
					// 기존의 리스트를 삭제한다. 
					ListDelete(m_hDlg, IDC_LIST_VILLAGEDETAIL);

					char* sidata[MAX_VILLAGE_NUMBER];
					SI32  siVillageInfo[MAX_VILLAGE_NUMBER];

					SI32 i;
					for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
					{
						//	sidata[i] = pclinfo->siVillageLevel[i];
						sidata[i] = pclClient->pclVillageManager->GetName(i);
					}
					SI32 index = Sort(sidata, siVillageInfo);

					SI32 villindex = 1;
					if(TRUE == bCheckSortVillage)
					{
						SendMessage(GetDlgItem(hDlg,IDC_CHECK_SORTVILLAGE), BM_SETCHECK, BST_CHECKED, 0 );
						for(i = 0;i < index; ++i)
						{
							SI32 villageunique = siVillageInfo[i];

							if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

							char buffer[256];
							sprintf(buffer, "%d. %s", villindex, pclClient->pclVillageManager->GetName(villageunique)); 
							SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);
							villindex++;

							// 리스트 항목에 데이터를 연결한다. 
							ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);

						}
					}
					else
					{
						SendMessage(GetDlgItem(hDlg,IDC_CHECK_SORTVILLAGE), BM_SETCHECK, BST_UNCHECKED, 0 );
						for(SI32 i = 0;i < MAX_VILLAGE_NUMBER;i++)
						{

							if(pclClient->pclVillageManager->pclVillageInfo[i] == NULL)continue;

							char buffer[256];
							sprintf(buffer, "%d. %s", villindex, pclClient->pclVillageManager->GetName(i)); 
							villindex++;

							SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);


							// 리스트 항목에 데이터를 연결한다. 
							ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)i);

						}
					}
				}
				else
				{
					cltGameMsgRequest_VillageDetailInfo clinfo(siCurrentType);
					cltMsg clMsg(GAMEMSG_REQUEST_VILLAGEDETAILINFO, sizeof(clinfo), (char*)&clinfo);
					pclClient->pclCM->CR[1]->SendNetMsg((char*)&clMsg);


					// 기존의 리스트를 삭제한다. 
					ListDelete(m_hDlg, IDC_EDIT_VILLAGEINFO);

					// 도움말을 보여준다.
					cltVillageDetailData* pinfo = GetVillageDetailData(siCurrentType);
					if(pinfo)
					{
						ShowStrInfo(pinfo->szExplainText);
					}
					

				}

			}
			
			if(siCurrentType)
			{
				// 현재 선택된 학교를  확보한다. 
				SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_VILLAGEDETAIL);
				if(index < 0)break;
				
				SI32 siCurrentVillage = SendDlgItemMessage(hDlg, IDC_LIST_VILLAGEDETAIL, LB_GETITEMDATA,	index, 0);
				if(siCurrentVillage > 0 && siCurrentVillage != siOldViliage)
				{
					siOldViliage = siCurrentVillage;
					// 새로운정보를 요청한다.
					if(siCurrentType == VILLAGEDETAILINFOTYPE_HUNT)
					{
						cltGameMsgRequest_VillageInfoStrInfo clinfo(siCurrentVillage, RANKTYPE_HUNT);
						cltMsg clMsg(GAMEMSG_REQUEST_VILLAGEINFOSTRINFO, sizeof(clinfo), (char*)&clinfo);
						pclClient->pclCM->CR[1]->SendNetMsg((char*)&clMsg);

					}
					else if(siCurrentType == VILLAGEDETAILINFOTYPE_CITYHALLSTATE )
					{
						cltGameMsgRequest_VillageInfoStrInfo clinfo(siCurrentVillage, RANKTYPE_CITYHALL);
						cltMsg clMsg(GAMEMSG_REQUEST_VILLAGEINFOSTRINFO, sizeof(clinfo), (char*)&clinfo);
						pclClient->pclCM->CR[1]->SendNetMsg((char*)&clMsg);
					}
				}
			}

		}
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{


		case IDOK:
			{

			}
			return TRUE;
			
		case IDCANCEL:
			DestroyWindow(m_hDlg);
			return TRUE;


		case IDC_BUTTON_WARPTOVILLAGE:
			{
				cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

				// 현재 선택된 마을을  확보한다. 
				SI32 siCurrentVillage = 0;
				SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_VILLAGEDETAIL);
				if(index < 0)
				{
					char* pTitle = GetTxtFromMgr(1413);
					char* pText = GetTxtFromMgr(1415);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}
				else
				{
					siCurrentVillage = SendDlgItemMessage(hDlg, IDC_LIST_VILLAGEDETAIL, LB_GETITEMDATA,	index, 0);
					
					// 이동할 마을을 선택한다. 
					if(siCurrentVillage <= 0)
					{
						char* pTitle = GetTxtFromMgr(1413);
						char* pText = GetTxtFromMgr(1415);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return TRUE;
					}
				}
				
				// 탑승중이면 워프 불가. 
				if(pclchar->clInnerStatus.siRideShipID)
				{
					char* pTitle = GetTxtFromMgr(1413);
					char* pText = GetTxtFromMgr(1416);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					return TRUE;
				}

				char buffer[256] = "" ;
				char* pTitle = NULL ;
				char* pText = NULL ;

				char* pname = pclClient->pclVillageManager->GetName(siCurrentVillage);
				if(pname == NULL)return TRUE;

				if ( !pclchar->pclCI->clBI.bUseNoLimitTicketWarp && 
					 pclchar->pclCI->clBI.IsPCRoom() == false && pclclient->pclCM->CR[1]->pclCI->clBI.uiPoliceSwitch == false )
				{
					// 순간이동이용권을 보유하고 있는지 확인한다. 
					SI32 itemunique = 14005 ;  // 순간이동이용권
					SI32 pos = pclchar->pclCI->clCharItem.FindItemInv(itemunique);
					if(pos < 0)
					{
						char* pTitle = GetTxtFromMgr(1413);
						char* pText = GetTxtFromMgr(1414);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

						return TRUE;
					}
					pTitle = GetTxtFromMgr(1418);
					pText = GetTxtFromMgr(1417);

					sprintf(buffer,pText,pname);
				}
				else
				{
					pTitle = GetTxtFromMgr(3189);
					pText = GetTxtFromMgr(1893);

					sprintf(buffer,pText,pname);
				}

				if (  MessageBox(NULL, buffer,pTitle, MB_ICONQUESTION | MB_YESNO) == IDNO )
				{
					return TRUE;
				}

				//KHY - 0703 - 게임 머니로 귀환하기.클레스 수정.
				cltGameMsgRequest_HuntMapWarp pclInfo(siCurrentVillage,0,false);
				cltMsg clMsg( GAMEMSG_REQUEST_HUNTMAPWARP, sizeof( pclInfo ), (char*)&pclInfo );
				pclClient->SendMsgToServer((char*)&clMsg);

			}
			
			return TRUE;

		case IDC_CHECK_SORTVILLAGE:
			{
				if(siCurrentType == VILLAGEDETAILINFOTYPE_VILLAGELIST)
				{
					char* sidata[MAX_VILLAGE_NUMBER];
					SI32  siVillageInfo[MAX_VILLAGE_NUMBER];

					// 기존의 리스트를 삭제한다. 
					ListDelete(m_hDlg, IDC_LIST_VILLAGEDETAIL);

					SI32 i;

					for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
					{
					//	sidata[i] = pclinfo->siVillageLevel[i];
						sidata[i] = pclClient->pclVillageManager->GetName(i);
					}
					SI32 index = Sort(sidata, siVillageInfo);

					SI32 villindex = 1;
					bCheckSortVillage = !bCheckSortVillage;
					if(TRUE == bCheckSortVillage)
					{
						for(i = 0;i < index; ++i)
						{
							SI32 villageunique = siVillageInfo[i];

							if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

							char buffer[256];
							sprintf(buffer, "%d. %s", villindex, pclClient->pclVillageManager->GetName(villageunique)); 
							SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);
							villindex++;

							// 리스트 항목에 데이터를 연결한다. 
							ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);

						}
					}
					else
					{
						for(SI32 i = 0;i < MAX_VILLAGE_NUMBER;i++)
						{

							if(pclClient->pclVillageManager->pclVillageInfo[i] == NULL)continue;

							char buffer[256];
							sprintf(buffer, "%d. %s", villindex, pclClient->pclVillageManager->GetName(i)); 
							villindex++;

							SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);


							// 리스트 항목에 데이터를 연결한다. 
							ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)i);

						}

					}
				}
			}
			return TRUE;

			
		case IDC_COMBO_VILLAGEDETAIL:
			{
				switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					{
						SI32 id = 1;
						if(pclClient->pclCM->IsAlive(id) == false)break;

						char szSelText[128]="";
						ComboGetString(hDlg, IDC_COMBO_VILLAGEDETAIL, (LPARAM)szSelText);

						// 적절한 코드를 찾는다.
						SI32 type = GetVillageDetailType(szSelText);

						if(type)
						{
							siCurrentType = type;
						}
					}
					break;
				}
			}
			break;



		}
		return TRUE;
		
	case WM_DESTROY:
		{
			// 타이머를 해제한다. 
			KillTimer(hDlg, 1);
			
			// 윈도우의 위치를 기억한다. 
			GetWindowRect(hDlg, &DlgRect);
			bDlgRectInitSwitch = true;

			if ( m_hWarpBitmap )
			{
				DeleteObject(m_hWarpBitmap);
				m_hWarpBitmap = NULL ;
			}

			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush= NULL;
			}
			m_hDlg = NULL;
		}
		return TRUE;

	}

	return FALSE;
}


// 콤보에 데이터 타입을 넣는다. 
void cltVillageDetailInfoManager::SetDataTypeInCombo(HWND hDlg, SI32 dlgitem)
{

	// 콤보 박스를 지운다.
	ComboDelete(hDlg, dlgitem);

	SI32 index = 0;
	while(clvillagedata[index].siType > 0)
	{
		ComboAddString(hDlg, dlgitem, (LPARAM)clvillagedata[index].szTypeText);

		index++;
	}

}

// 텍스트를 근거로 마을 상세 정보 코드를 구한다. 
SI32 cltVillageDetailInfoManager::GetVillageDetailType(char* ptext)
{
	SI32 index = 0;

	while(clvillagedata[index].siType > 0)
	{
		if(strcmp(clvillagedata[index].szTypeText, ptext) == 0)
		{
			return clvillagedata[index].siType;
		}

		index++;
	}

	return 0;
}

// 타입을 근거로 정보 포인터를 구한다. 
cltVillageDetailData* cltVillageDetailInfoManager::GetVillageDetailData(SI32 type)
{
	SI32 index = 0;

	while(clvillagedata[index].siType > 0)
	{
		if(clvillagedata[index].siType == type)
		{
			return &clvillagedata[index];
		}

		index++;
	}

	return NULL;
}

// 건물 상세 정보를 표시한다. 
void cltVillageDetailInfoManager::ShowStrInfo(char*  ptext)
{
	SetDlgItemText(m_hDlg, IDC_EDIT_VILLAGEINFO, ptext);

}

// 서버로 부터 받은 정보를 표시한다. 
void cltVillageDetailInfoManager::ShowInfo(cltGameMsgResponse_VillageDetailInfo* pclinfo)
{
	SI32 i;
	char buffer[256] = "";


	// 기존의 리스트를 삭제한다. 
	ListDelete(m_hDlg, IDC_LIST_VILLAGEDETAIL);


	SI32 sidata[MAX_VILLAGE_NUMBER];
	SI32 siVillageInfo[MAX_VILLAGE_NUMBER];

	// 전쟁 결과 통보. 
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_WAR)
	{
		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = pclinfo->siWarKillNum[i];
		}
		SI32 index = Sort(sidata, siVillageInfo);

		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

			sprintf(buffer, "%d. %s - %d", i+1, pclClient->pclVillageManager->GetName(villageunique),pclinfo->siWarKillNum[villageunique]); 
			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);
			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);

		}

	}
	// 전쟁 결과 통보. (주민 100명당) 
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_WARPERRESIDENT)
	{
		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = pclinfo->siWarKillNum[i];
		}
		SI32 index = Sort(sidata, siVillageInfo);

		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

			sprintf(buffer, "%d. %s - %d", i+1, pclClient->pclVillageManager->GetName(villageunique),pclinfo->siWarKillNum[villageunique]); 
			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);
			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);

		}

	}


	// 주민수 결과 통보. 
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_PEOPLE)
	{
		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = pclinfo->uiPeople[i];
		}
		SI32 index = Sort(sidata, siVillageInfo);

		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

			char* pText = GetTxtFromMgr(1419);

			sprintf(buffer, pText, i+1, pclClient->pclVillageManager->GetName(villageunique),pclinfo->uiPeople[villageunique]); 
			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);
			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);

		}
	}
	// 현재 마을 사용자수. 
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_USER)
	{
		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = pclinfo->siUserNum[i];
		}
		SI32 index = Sort(sidata, siVillageInfo);

		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

			char* pText = GetTxtFromMgr(1419);

            sprintf(buffer, pText, i+1, pclClient->pclVillageManager->GetName(villageunique),pclinfo->siUserNum[villageunique]); 
			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);
			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);


		}
	}

	// 전년도 배당액 
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_PREVYEARDBID)
	{
		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = pclinfo->siBidMoney[i];
		}
		SI32 index = Sort(sidata, siVillageInfo);

		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

			sprintf(buffer, "%d. %s - %d", i+1, pclClient->pclVillageManager->GetName(villageunique),pclinfo->siBidMoney[villageunique]); 
			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);
			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);

		}
	}

	// 수렵장 레벨 제한. 
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_HUNT
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_HORSEMARKET
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_STOCK
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_REALESTATEMARKET
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_SUMMONMARKET
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_SUMMONHEROMARKET
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_PORT
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_SHIPMARKET
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_FARM
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_FEAST
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_MINE
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_GUILD)
	{
		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = pclinfo->siStrData[i];
		}
		SI32 index = Sort(sidata, siVillageInfo);

		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

			// 유효하지 않은 건물은 제외한다. 
			if(pclinfo->siStrData[villageunique] <= 0)continue;

			if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_HUNT)
			{
				char* pText = GetTxtFromMgr(1420);

				sprintf(buffer, pText, i+1, 
					pclClient->pclVillageManager->GetName(villageunique),
					max(0, (pclinfo->siStrData[villageunique] - 1)) );	// 서버에서 1명을 더했기 때문에 클라이언트에서는 1명을 뺀다. 
			}
			else if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_HORSEMARKET
				||  pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_STOCK
				||  pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_REALESTATEMARKET
				||  pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_SUMMONMARKET
				||  pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_SUMMONHEROMARKET
				||  pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_SHIPMARKET)
			{
				char* pText = GetTxtFromMgr(1421);

				sprintf(buffer, pText, i+1, 
					pclClient->pclVillageManager->GetName(villageunique),
					(float)((pclinfo->siStrData[villageunique] - 1)/ 10.0)); 
			}
			else if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_PORT)
			{
				char* pText = GetTxtFromMgr(1422);

				sprintf(buffer, pText, 
					i+1, pclClient->pclVillageManager->GetName(villageunique),pclinfo->siStrData[villageunique] - 1);
			}
			else if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_FARM)
			{
				char* pText = GetTxtFromMgr(1423);
				sprintf(buffer, pText, 
					i+1, pclClient->pclVillageManager->GetName(villageunique),pclinfo->siStrData[villageunique] - 1);
			}
			else if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_FEAST)
			{
				char* pText = GetTxtFromMgr(1424);
				sprintf(buffer, pText, 
					i+1, pclClient->pclVillageManager->GetName(villageunique), pclinfo->siStrData[villageunique] - 1);
			}
			
			else if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_MINE)
			{
				char* pText = GetTxtFromMgr(1423);
				sprintf(buffer, pText, 
					i+1, pclClient->pclVillageManager->GetName(villageunique),pclinfo->siStrData[villageunique] - 1);
			}

			else if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_GUILD)
			{
				char* pText = GetTxtFromMgr(1423);
				sprintf(buffer, pText, 
					i+1, pclClient->pclVillageManager->GetName(villageunique),
					pclinfo->siStrData[villageunique] - 1);
			}


			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);

			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);
	
		}
		
	}
	// 마을레벨 
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_VILLAGELEVEL)
	{
		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = pclinfo->siVillageLevel[i];
		}
		SI32 index = Sort(sidata, siVillageInfo);

		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

			sprintf(buffer, "%d. %s - Lv[%d]", i+1, pclClient->pclVillageManager->GetName(villageunique),pclinfo->siVillageLevel[villageunique]); 
			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);

			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);
	
		}
	}
	// 마을명단  
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_VILLAGELIST)
	{

	}


	// 마을 자금 
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_VILLAGEMONEY
	|| pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_VILLAGEPROFIT)
	{
		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = pclinfo->siStrData[i];
		}
		SI32 index = Sort(sidata, siVillageInfo);

		char buf[256];
		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

			// 숫자에 콤마를 찍는다. 
			g_JYLibFunction.SetNumUnit( pclinfo->siStrData[villageunique], buf, "냥" );

			sprintf(buffer, "%d. %s - %s", i+1, 
				pclClient->pclVillageManager->GetName(villageunique), buf);

			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);

			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);
	
		}
		
	}
	// 마을 방문자수
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_VISITORNUM)
	{
		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = pclinfo->siStrData[i];
		}
		SI32 index = Sort(sidata, siVillageInfo);

		char buf[256];
		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

			// 숫자에 콤마를 찍는다. 
			g_JYLibFunction.SetNumUnit( pclinfo->siStrData[villageunique], buf, "명" );

			sprintf(buffer, "%d. %s - %s", i+1, 
				pclClient->pclVillageManager->GetName(villageunique), buf);

			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);

			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);
	
		}
		
	}

	// 마을 주식수
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_VILLAGESTOCK)
	{
		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = pclinfo->siStrData[i];
		}
		SI32 index = Sort(sidata, siVillageInfo);

		char buf[256];
		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

			// 숫자에 콤마를 찍는다. 
			g_JYLibFunction.SetNumUnit( pclinfo->siStrData[villageunique], buf, "주" );

			sprintf(buffer, "%d. %s - %s", i+1, 
				pclClient->pclVillageManager->GetName(villageunique), buf);

			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);

			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);
	
		}
		
	}
	// 주주총회 진행마을 
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_GENERALMEETING)
	{
		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = pclinfo->siStrData[i];
		}
		SI32 index = Sort(sidata, siVillageInfo);

		char buf[256] = "" ;
		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;
			
			// 유효하지 않은 건물은 제외한다. 
			if(pclinfo->siStrData[villageunique] <= 0)continue;


			sprintf(buffer, "%d. %s", i+1, 
				pclClient->pclVillageManager->GetName(villageunique));

			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);

			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);
	
		}
		
	}

	// 관청상태정보(정상, 파산, 안정 등 )
	if(pclinfo->siVillageDetailInfoType == VILLAGEDETAILINFOTYPE_CITYHALLSTATE )
	{
		//UI08 uidata[MAX_VILLAGE_NUMBER];

		//--------------------------------
		// 내림차순으로 정렬한다. 
		//--------------------------------
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			sidata[i] = (SI32)pclinfo->uiCityHallState[i];
			siVillageInfo[i] = i;
		}
		//SI32 index = Sort(sidata, siVillageInfo);
		SI32 index = MAX_VILLAGE_NUMBER;
		SI32 count = 0;

		char buf[256] = "" ;
		for(i = 0;i < index;i++)
		{
			SI32 villageunique = siVillageInfo[i];

			if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL 
				|| sidata[i] >= CITYHALL_UNDEFINED_STATE 
				|| sidata[i] >= CITYHALL_BEING_PROCESSING )continue;
			
			char* pStateName = NULL;

            switch( sidata[ i ] )
			{
			case 	CITYHALL_BEING_BANKRUPT	:		// 파산기
				pStateName = GetTxtFromMgr(5240);
				break;
			case 	CITYHALL_BEING_INITIALIZED	:	// 초기화기
				pStateName = GetTxtFromMgr(5241);
				break;
			case 	CITYHALL_BEING_STABLE		:	// 안정기
				pStateName = GetTxtFromMgr(5242);
				break;
			case CITYHALL_BEING_NORMAL			:	// 정상기
				pStateName = GetTxtFromMgr(5239);
				break;
			default:
				continue;
				break;
			}

			sprintf(buffer, "%d. %s - %s", ++count, 
				pclClient->pclVillageManager->GetName(villageunique), 
				pStateName );

			SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGEDETAIL, (LPARAM)buffer);

			// 리스트 항목에 데이터를 연결한다. 
			ListSetItemData(m_hDlg, IDC_LIST_VILLAGEDETAIL, index, (LPARAM)villageunique);
	
		}
		
	}


}

// 내림차순으로 정렬한다. 
SI32 cltVillageDetailInfoManager::Sort(SI32 sisortdata[], SI32* pvillageunique)
{
	SI32 i;
	SI32 index = 0;
	
	bool bVillageInfo[MAX_VILLAGE_NUMBER];
	
	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		bVillageInfo[i] = false;
		pvillageunique[i] = 0;
	}
	
	for(SI32 vill = 0;vill < MAX_VILLAGE_NUMBER;vill++)
	{
		SI32 maxnum = -1;
		SI32 maxindex = -1;
		
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclClient->pclVillageManager->pclVillageInfo[i] == NULL)continue;
			
			if(bVillageInfo[i] == false && sisortdata[i] > maxnum)
			{
				maxnum		= sisortdata[i];
				maxindex	= i;
			}
		}

		if(maxindex >= 0)
		{
			bVillageInfo[maxindex] = true;
			pvillageunique[index] = maxindex;
			index++;
		}
	}

	return index;
}

SI32 cltVillageDetailInfoManager::Sort(char* psortname[], SI32* pvillageunique)
{
	SI32 i;
	SI32 index = 0;

	bool bVillageInfo[MAX_VILLAGE_NUMBER];

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		bVillageInfo[i] = false;
		pvillageunique[i] = 0;
	}


	for(SI32 vill = 0;vill < MAX_VILLAGE_NUMBER;vill++)
	{
		// 히히는 그냥 비교용
		char buffer[12] = "히히";
		SI32 maxindex = -1;

		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclClient->pclVillageManager->pclVillageInfo[i] == NULL)continue;

			SI32 result = strcmp(psortname[i], buffer);
			if(bVillageInfo[i] == false && result < 0)
			{
				strcpy(buffer, psortname[i]);
				maxindex	= i;
			}
		}

		if(maxindex >= 0)
		{
			bVillageInfo[maxindex] = true;
			pvillageunique[index] = maxindex;
			index++;
		}
	}

	return index;
}