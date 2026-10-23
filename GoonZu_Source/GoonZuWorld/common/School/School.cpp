/*
//---------------------------------
// 2004/2/10 김태곤
//---------------------------------
#include "CommonLogic.h"
#include "..\..\Client\Client.h"

#include "School.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\Resource.h"
#include "MsgType-Person.h"

extern cltCommonLogic* pclClient;

// 전체 이름을 구한다. 
bool cltSchool::GetFullName(cltSchoolManager* pchoolmanager, char* pname)
{
	sprintf(pname, "%s %s%s", pchoolmanager->clSchoolArea[siAreaCode].szName, szName, pchoolmanager->clSchoolGrade[ siGradeCode ].szName) ;

	return true;
}

cltSchoolManager::cltSchoolManager()
{
	m_hDlg	= NULL;					// 아이템 정보. 

	bDlgRectInitSwitch	= false;

	siCurrentSchoolArea		= 0;
	siCurrentSchoolGrade	= 0;

	siSchoolIndex		= 0;

	siCurrentSchoolUnique	= -1;
	bShowUserNumSwitch		= false;
	siUserNum				= 0;

	//-----------------------------
	// 학교 등급을 등록한다. 
	//-----------------------------
	cltSchoolGrade* pclgrade;

	char* pText = GetTxtFromMgr(3102);
	pclgrade = new cltSchoolGrade(pText);
	AddGrade(SCHOOLGRADE_PRI,	pclgrade);
	delete pclgrade;

	pText = GetTxtFromMgr(3103);
	pclgrade = new cltSchoolGrade(pText);
	AddGrade(SCHOOLGRADE_MID,	pclgrade);
	delete pclgrade;

	pText = GetTxtFromMgr(3104);
	pclgrade = new cltSchoolGrade(pText);
	AddGrade(SCHOOLGRADE_HIG,	pclgrade);
	delete pclgrade;

	pText = GetTxtFromMgr(3105);
	pclgrade = new cltSchoolGrade(pText);
	AddGrade(SCHOOLGRADE_UNV,	pclgrade);
	delete pclgrade;

	pclgrade = new cltSchoolGrade("Secondary");
	AddGrade(SCHOOLGRADE_SECON,	pclgrade);
	delete pclgrade;

	pclgrade = new cltSchoolGrade("InternationalSchool");
	AddGrade(SCHOOLGRADE_INTER,	pclgrade);
	delete pclgrade;

	pclgrade = new cltSchoolGrade("HighSchool");
	AddGrade(SCHOOLGRADE_HIGH,	pclgrade);
	delete pclgrade;

	//-----------------------------
	//학교 지역을 등록한다. 
	//-----------------------------
	cltSchoolArea* pclarea;

	pText = GetTxtFromMgr(3106);
	pclarea = new cltSchoolArea(pText);  //서울
	AddArea(SCHOOLAREA_SEOUL,	pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3107);
	pclarea = new cltSchoolArea(pText); //강원도
	AddArea(SCHOOLAREA_KANGWON, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3108);
	pclarea = new cltSchoolArea(pText);//경상북도
	AddArea(SCHOOLAREA_KYOUNGBOOK, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3109);
	pclarea = new cltSchoolArea(pText); // 광주
	AddArea(SCHOOLAREA_KWANGJU, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3110);
	pclarea = new cltSchoolArea(pText); //대구
	AddArea(SCHOOLAREA_DAIGU, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3111);
	pclarea = new cltSchoolArea(pText); //부산
	AddArea(SCHOOLAREA_BOOSAN, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3112);
	pclarea = new cltSchoolArea(pText); //전라남도
	AddArea(SCHOOLAREA_JOUNNAM, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3113);
	pclarea = new cltSchoolArea(pText);  //충청북도
	AddArea(SCHOOLAREA_CHOONGBOOK, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3114);
	pclarea = new cltSchoolArea(pText); //경기도
	AddArea(SCHOOLAREA_KYOUNGKI, pclarea);
	delete pclarea;
	
	pText = GetTxtFromMgr(3115);
	pclarea = new cltSchoolArea(pText); //경상남도
	AddArea(SCHOOLAREA_KYOUNGNAM, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3116);
	pclarea = new cltSchoolArea(pText);  //중국
	AddArea(SCHOOLAREA_CHINA, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3117);
	pclarea = new cltSchoolArea(pText);  //울산
	AddArea(SCHOOLAREA_WOOLSAN, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3118);
	pclarea = new cltSchoolArea(pText); //충청남도
	AddArea(SCHOOLAREA_CHOONGNAM, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3119);
	pclarea = new cltSchoolArea(pText); //전라북도
	AddArea(SCHOOLAREA_JOUNBOOK, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3120);
	pclarea = new cltSchoolArea(pText);   //인천
	AddArea(SCHOOLAREA_INCHEON, pclarea);
	delete pclarea;
	
	pText = GetTxtFromMgr(3121);
	pclarea = new cltSchoolArea(pText);  //제주도
	AddArea(SCHOOLAREA_ZEJU, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3122);
	pclarea = new cltSchoolArea(pText);   //대전
	AddArea(SCHOOLAREA_DAIJEON, pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3123);
	pclarea = new cltSchoolArea(pText);  //원주
	AddArea(SCHOOLAREA_WONJU,pclarea);
	delete pclarea;
	
	pText = GetTxtFromMgr(3124);
	pclarea = new cltSchoolArea(pText);  //캐나다
	AddArea(SCHOOLAREA_CANADA,pclarea);
	delete pclarea;
	
	pText = GetTxtFromMgr(3125);
	pclarea = new cltSchoolArea(pText);  //천안
	AddArea(SCHOOLAREA_CHEONAN,pclarea); 
	delete pclarea;

	pText = GetTxtFromMgr(3126);
	pclarea = new cltSchoolArea(pText); //싱가포르
	AddArea(SCHOOLAREA_SINGAPORE,pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3127);
	pclarea = new cltSchoolArea(pText); //뉴질랜드
	AddArea(SCHOOLAREA_NEWZEALAND,pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3128);
	pclarea = new cltSchoolArea(pText);  //미국
	AddArea(SCHOOLAREA_USA,pclarea);
	delete pclarea;

	pText = GetTxtFromMgr(3129);
	pclarea = new cltSchoolArea(pText); //춘천
	AddArea(SCHOOLAREA_CHUNCHEON,pclarea);
	delete pclarea;

	//----------------------------
	// 학교를 등록한다. 
	//----------------------------
//	SetSchoolList();



}

cltSchoolManager::~cltSchoolManager()
{

}

// 유니크를 근거로 ref를 구한다. 
SI32 cltSchoolManager::GetRefFromUnique(SI32 unique)
{
	SI32 i;

	for(i = 0;i < siSchoolIndex;i++)
	{
		if(clSchool[i].siUnique == unique)return i;
	}
	return -1;
}

// 학교등급을 등록한다. 
bool cltSchoolManager::AddGrade(SI32 areacode, cltSchoolGrade* pclgrade)
{
	clSchoolGrade[areacode].Set(pclgrade);
	return true;
}

// 등급명을 근거로 등급 코드를 얻는다. 
SI32 cltSchoolManager::GetGradeCode(char* pname)
{
	SI32 i;

	for(i = 0;i < MAX_SCHOOL_GRADE;i++)
	{
		if(strcmp(clSchoolGrade[i].szName, pname) == 0)
		{
			return i;
		}
	}

	return 0;
}

// 학교지역을 등록한다. 
bool cltSchoolManager::AddArea(SI32 areacode, cltSchoolArea* pclarea)
{
	clSchoolArea[areacode].Set(pclarea);
	return true;
}

// 지역명을 근거로 지역 코드를 얻는다. 
SI32 cltSchoolManager::GetAreaCode(char* pname)
{
	SI32 i;

	for(i = 0;i < MAX_SCHOOL_AREA;i++)
	{
		if(strcmp(clSchoolArea[i].szName, pname) == 0)
		{
			return i;
		}
	}

	return 0;
}

// 학교를 등록한다. 
bool cltSchoolManager::Add(cltSchool* pclschool)
{
	if(siSchoolIndex >= MAX_SCHOOL_NUMBER)return false;

	clSchool[siSchoolIndex].Set(pclschool);
	siSchoolIndex++;

	return true;

}


BOOL cltSchoolManager::CreateDlg(HINSTANCE hinst, HWND hwnd)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{

		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_SCHOOL), hwnd, StaticDlgProc);

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

BOOL CALLBACK cltSchoolManager::StaticDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	return pclClient->pclSchoolManager->DlgProc(hDlg, iMsg, wParam, lParam);
}

//학교명단을 보여준다.  
void cltSchoolManager::ShowSchoolList()
{
	SI32 i;
	char buffer[256] ="";

	// 기존의 리스트를 삭제한다. 
	ListDelete(m_hDlg, IDC_LIST_SCHOOL);

	for(i = 0;i < siSchoolIndex;i++)
	{
		if(siCurrentSchoolArea	 && clSchool[i].siAreaCode != siCurrentSchoolArea)continue;
		if(siCurrentSchoolGrade  && clSchool[i].siGradeCode != siCurrentSchoolGrade)continue;
		
		sprintf(buffer, "[%s %s]", clSchool[i].szName, clSchoolGrade[ clSchool[i].siGradeCode ].szName) ;
		
		//clSchool[i]->GetFullName();

		SI32 index = ListAddString(m_hDlg, IDC_LIST_SCHOOL, (LPARAM)buffer);
		
		// 리스트 항목에 데이터를 연결한다. 
		ListSetItemData(m_hDlg, IDC_LIST_SCHOOL, index, (LPARAM)clSchool[i].siUnique);
	}

}


BOOL CALLBACK cltSchoolManager::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 i;
	SI32 id = 1;
	static SI32 siLastUpdateSchoolUnique = -1;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			siCurrentSchoolArea		= 0;
			siCurrentSchoolGrade	= 0;

			// 지역명을 등록한다. 
			for(i = 0;i < MAX_SCHOOL_AREA;i++)
			{
				if(strcmp(clSchoolArea[i].szName, "") == 0)continue;

				ComboAddString(hDlg, IDC_COMBO_SCHOOLAREA, (LPARAM)clSchoolArea[i].szName);
			}

			// 등급을 등록한다. 
			for(i = 0;i < MAX_SCHOOL_GRADE;i++)
			{
				if(strcmp(clSchoolGrade[i].szName, "") == 0)continue;

				ComboAddString(hDlg, IDC_COMBO_SCHOOL_GRADE, (LPARAM)clSchoolGrade[i].szName);
			}

			SetTimer(hDlg, 1, 1000, NULL);
		}
		return TRUE;

	case WM_NOTIFY:
		return TRUE;

	case WM_TIMER:
		{
			// 서버로 학교 사용자 수를 요청한다. 
			if(siCurrentSchoolUnique > 0)
			{
				if(siCurrentSchoolUnique != siLastUpdateSchoolUnique)
				{
					siLastUpdateSchoolUnique = siCurrentSchoolUnique;
					
					cltGameMsgRequest_SchoolUserNum clinfo(siCurrentSchoolUnique);
					cltMsg clMsg(GAMEMSG_REQUEST_SCHOOLUSERNUM, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
			}
		
			if(bShowUserNumSwitch)
			{
				bShowUserNumSwitch = false;
				
				char buffer[256];
				sprintf(buffer, "%d", siUserNum);
				SetDlgItemText(hDlg, IDC_EDIT_SCHOOLUSERNUM, buffer);
				
			}
		}
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_LIST_SCHOOL:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				// 현재 선택된 학교를  확보한다. 
				SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_SCHOOL);
				if(index < 0)break;
				
				siCurrentSchoolUnique = SendDlgItemMessage(hDlg, IDC_LIST_SCHOOL, LB_GETITEMDATA,	index, 0);
				
				if(siCurrentSchoolUnique > 0)
				{
					// 학교 로고를 그린다. 
					DrawLogo(siCurrentSchoolUnique);
				}
				break;
			}
			return TRUE;
		case IDC_COMBO_SCHOOLAREA:
			{
				switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					{
						
						char szarea[128] = "NULL";
						ComboGetString(hDlg, IDC_COMBO_SCHOOLAREA, (LPARAM)szarea);
						
						siCurrentSchoolArea	= GetAreaCode(szarea);

						//학교명단을 보여준다.  
						ShowSchoolList();
						
					}
					break;
				}
			}
			return TRUE;

		case IDC_COMBO_SCHOOL_GRADE:
			{
				switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					{
						
						char szgrade[128] = "NULL";
						ComboGetString(hDlg, IDC_COMBO_SCHOOL_GRADE, (LPARAM)szgrade);
						
						siCurrentSchoolGrade	= GetGradeCode(szgrade);

						//학교명단을 보여준다.  
						ShowSchoolList();
						
					}
					break;
				}
			}
			return TRUE;

		case IDC_BUTTON_CANCELSCHOOL:
			{
				if(pclClient->pclCM->CR[1]->pclCI->clBI.siSchoolUnique <= 0)
				{
					char* pTitle = GetTxtFromMgr(3131);
					char* pText = GetTxtFromMgr(3132);

					MsgBox(pTitle, pText);
					break;
				}

				char* pTitle = GetTxtFromMgr(3134);
				char* pText = GetTxtFromMgr(3133);


				if(MessageBox(hDlg, pText, pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES)
				{
					// 서버로 학교 등록취소할 것을 요청한다. 
					cltGameMsgRequest_SetSchool clinfo(0);
					cltMsg clMsg(GAMEMSG_REQUEST_SETSCHOOL, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}

			}
			return TRUE;

		
		case IDOK:
			{
				// 현재 선택된 학교를  확보한다. 
				SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_SCHOOL);
				if(index < 0)break;

				SI32 schoolunique = SendDlgItemMessage(hDlg, IDC_LIST_SCHOOL, LB_GETITEMDATA,	index, 0);
				if(schoolunique <= 0)break;

				char* pTitle = GetTxtFromMgr(3136);
				char* pText = GetTxtFromMgr(3135);

				if(MessageBox(hDlg, pText, pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES)
				{
					
					// 서버로 학교를  등록할 것을 요청한다. 
					cltGameMsgRequest_SetSchool clinfo(schoolunique);
					cltMsg clMsg(GAMEMSG_REQUEST_SETSCHOOL, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}

				
			}
			return TRUE;
			
		case IDCANCEL:
			DestroyWindow(m_hDlg);
			return TRUE;
			
			
	}
	return TRUE;
	
	case WM_DESTROY:
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


// 학교 리스트를 등록한다. 
void cltSchoolManager::SetSchoolList()
{
	SI32 i;
	char buffer[1024];

	FILE* fp;

	fp = fopen("School\\School.txt", "rt");

	if(fp == NULL)
	{
		MsgBox("fds982jf", "fdsf932:%s", "School.txt");
		return ;
	}

	// 6줄 건너띈다. 
	for(i = 0;i < 6;i++)
	{
		fgets(buffer, 1024, fp);
	}


	SI32 unique;
	SI32 file;
	SI32 icon;
	char area[128];
	char grade[128];
	char name[128];

	cltSchool* pclschool;

	while( fgets(buffer, 1024, fp) != NULL)
	{
		sscanf(buffer, "%d %s %s %s %d %d", &unique, area, grade, name, &file, &icon);

		// 지역 텍스트를 기반으로 짗역 코드를 얻어온다. 
		SI32 areacode = GetAreaCode(area);
		if(areacode <= 0)
		{
			MsgBox("AreaCode", "%s", area);
			return ;
		}

		SI32 gradecode = GetGradeCode(grade);
		if(gradecode <= 0)
		{
			MsgBox("GradeCode", "%s", grade);
			return ;
		}
		pclschool = new cltSchool(unique, areacode, gradecode, name, file, icon);
		Add(pclschool);
		delete pclschool;
	
	}	

	fclose(fp);

}

// 학교 로고를 출력한다. 
void cltSchoolManager::DrawLogo(SI32 schoolunique)
{

	cltClient* pclclient = (cltClient*)pclClient;
	LPDIRECTDRAWSURFACE7 psurface = pclclient->lpBackScreen;

	// 로고를 서피스에 출력한다. 
	SI32 ref	= GetRefFromUnique(schoolunique);
	if(ref < 0)return ;
	

	SI32 file		= clSchool[ref].siFile;
	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_SCHOOL1 + file);

	if(pspr == NULL)return ;


	SI32 icon	= clSchool[ref].siIconIndex;

	if(GP.LockSurface(psurface) == TRUE)
	{
		GP.PutSpr(pspr, 0, 0, clSchool[icon].siIconIndex);

		GP.UnlockSurface(psurface);
	}

	//----------------------------------------------
	// 그려진 이미지를 다이얼로그 박스로 옮긴다. 
	//----------------------------------------------
	HWND hwnd = GetDlgItem(m_hDlg, IDC_STATIC_SCHOOLLOGO);

	RECT rect;
	RECT rect2;
	GetClientRect( hwnd, &rect );
	rect2 = rect;
	
	// 얻어온 클라이언트 좌표 2개를 스크린 좌표로 변경한다.
	ClientToScreen( hwnd, (POINT*)&rect );
	ClientToScreen( hwnd, (POINT*)&rect + 1);

	HDC srchdc;
	HDC desthdc = GetDC(hwnd);
	if(desthdc )
	{
		if(psurface->GetDC(&srchdc) == DD_OK)
		{
			
			BitBlt(desthdc, 0, 0, 
				pspr->GetXSize(), pspr->GetYSize(),
				srchdc, 0, 0, SRCCOPY);

			psurface->ReleaseDC(srchdc);
			
		}
		
		ReleaseDC(hwnd, desthdc);
	}

}
*/