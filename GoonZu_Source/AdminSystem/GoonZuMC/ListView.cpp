//===============================================================
// 제작자 : 박기형
// 날짜 :  2009.1 .25
// 설명 :  리스트 뷰 클래스
//===============================================================
#include <windows.h>
#include <tchar.h>
#include "ListView.h"


ListView::ListView(void)
{
	m_Handle = NULL;	
	//	설정 된 컬럼 개수
	m_ColCount = 0;
	//	선택된 데이터 포인터
	m_Select = NULL;
	//	유형 ( 0 : 외부핸들 사용, 1 : 내부 자체 생성)
	m_Type = false;

	//	이미지 확장 수
	int imgCount = 100;

	//	이미지 리스트 생성
	m_HImgList = ImageList_Create(GetSystemMetrics(SM_CXICON), 
									 GetSystemMetrics(SM_CYICON), 
									 ILC_COLORDDB, 1, imgCount);

	/*m_HImgList = ImageList_Create(32, 
									 32, 
									 ILC_COLORDDB, 1, imgCount);*/

	// 이미지 리스트 생성
	// m_HImgList = ImageList_LoadImage(m_hInstance, lpbmp, cx, cGrow,RGB(255, 255, 255),IMAGE_BITMAP, 
	//	LR_DEFAULTCOLOR  | LR_LOADMAP3DCOLORS | LR_CREATEDIBSECTION
	//	);
	//	선택된 레코드 인덱스
	m_SelctedRecordIndex = -1;

}
ListView::~ListView(void)
{
	Release();
}

//	생성함수
BOOL	ListView::Create(BOOL p_Type, HWND p_Hwnd, char *p_Tit, int Xx, int Yy, int Ww, int Hh, BOOL p_Check)
{
	//	유형
	m_Type = p_Type;
	//	유형에 따라서 별도 처리
	if(m_Type == TRUE)
	{
		//	윈도우 생성
		if( p_Hwnd != NULL)
		{
			//	자식 리스트 뷰 만들기
			m_Handle = CreateWindowA( WC_LISTVIEWA, p_Tit,
			LVS_SHOWSELALWAYS | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE | WS_BORDER | WS_THICKFRAME | LVS_REPORT | WS_CHILD,
			Xx, Yy, Ww, Hh, p_Hwnd, NULL, NULL, NULL);
		}
		else
		{
			//	독립 리스트 뷰 만들기
			m_Handle = CreateWindowA( WC_LISTVIEWA, p_Tit,
			LVS_SHOWSELALWAYS | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE | WS_BORDER | WS_THICKFRAME | LVS_REPORT,
			Xx, Yy, Ww, Hh, p_Hwnd, NULL, NULL, NULL);
		}
		//	윈도우 핸들 확인
		if(m_Handle == NULL)
		{
			return false;
		}
		
	}
	else
	{
		//	대화상자 리소스의 리스트 뷰 핸들 사용
		m_Handle = p_Hwnd;
		if(m_Handle == NULL)
			return false;
	}
	//	체크박스 확장 스타일
	if(p_Check)
	{
		//	체크박스 추가
		ListView_SetExtendedListViewStyle(m_Handle,
			LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP | LVS_EX_CHECKBOXES);
	}
	else
	{
		//	체크박스 없음
		ListView_SetExtendedListViewStyle(m_Handle,
			LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	}
	return true;
}
//	삭제함수
BOOL	ListView::Release()
{
	//	유형에 따라서 검사
	if(m_Type == TRUE)
	{
		DestroyWindow(m_Handle);
		m_Handle = NULL;
	}
	else
	{
		//	대화상자 리소스의 리스트 뷰 핸들
		m_Handle = NULL;
	}
	m_ColCount = 0;
	m_Select = NULL;

	//이미지 리스트 제거
	ImageList_Destroy(m_HImgList);

	return true;
}
	
//	초기 사용할 컬럼 설정
BOOL	ListView::Set_Col(char *p_Tit, int p_size)
{
	if(m_Handle == NULL)
	{
		return false;
	}

	//	컬럼 정보 구조체 설정
	LVCOLUMNA	t_Col;
	t_Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	t_Col.fmt = LVCFMT_LEFT;	//	왼쪽 정렬
	t_Col.iSubItem = m_ColCount;		//	서브 번호
	t_Col.cx = p_size;			//	컬럼 크기
	t_Col.pszText = p_Tit;		//	문자열

	//	메시지로 내용 추가
	SendMessageA(m_Handle, LVM_INSERTCOLUMNA, m_ColCount, (LPARAM)&t_Col);
	//	컬럼 개수 증가
	m_ColCount++;
	return true;
}

//	리스트 데이터 추가
BOOL	ListView::Set_Data(int p_Row_N, int p_Col_N, TCHAR *p_Text, VOID *p_Data)
{
	//	리스트 뷰 생성 확인
	if(m_Handle == NULL)
	{
		return false;
	}

	//	리스트 개수 보다 적은 경우 해당 리스트 수정
	if(p_Row_N < (int)SendMessage(m_Handle, LVM_GETITEMCOUNT, 0, 0))
	{
		
			LVITEM	t_Item;
			memset(&t_Item,0,sizeof(LVITEM));
			//	컬럼 번호
			t_Item.iSubItem = p_Col_N;
			//	문자열
			t_Item.pszText = p_Text;
			//	이름변경
			SendMessage(m_Handle, LVM_SETITEMTEXT, p_Row_N, (LPARAM)&t_Item);

	}
	//	신규 추가
	else
	{
		//	추가
		LVITEM	t_Item;
		memset(&t_Item,0,sizeof(LVITEM));
		//	구조체 내부에 사용할 정보 설정
		t_Item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		//	컬럼 번호
		t_Item.iSubItem = p_Col_N;
		//	라인 번호
		t_Item.iItem = p_Row_N;
		//	문자열
		t_Item.pszText = p_Text;
		//	임의의 데이터 버퍼
		t_Item.lParam = (LPARAM)p_Data;
		//	이미지 index
		t_Item.iImage = p_Row_N;
		//	추가
		SendMessage(m_Handle, LVM_INSERTITEM, 0, (LPARAM)&t_Item);
	}

	return true;
}

//	리스트 데이터 추가
BOOL	ListView::Set_Data(int p_Row_N, int p_Col_N, int p_Value, VOID *p_Data)
{
	//	리스트 뷰 생성 확인
	if(m_Handle == NULL)
	{
		return false;
	}

	TCHAR t_Text[11];
	_itoa(p_Value , (char*)t_Text,11);

	//	리스트 개수 보다 적은 경우 해당 리스트 수정
	if(p_Row_N < (int)SendMessageA(m_Handle, LVM_GETITEMCOUNT, 0, 0))
	{
		
			LVITEMA	t_Item;
			memset(&t_Item,0,sizeof(LVITEMA));
			//	컬럼 번호
			t_Item.iSubItem = p_Col_N;
			//	문자열
			t_Item.pszText = (LPSTR)t_Text;
			//	이름변경
			SendMessageA(m_Handle, LVM_SETITEMTEXTA, p_Row_N, (LPARAM)&t_Item);

	}
	//	신규 추가
	else
	{
		//	추가
		LVITEMA	t_Item;
		memset(&t_Item,0,sizeof(LVITEMA));
		//	구조체 내부에 사용할 정보 설정
		t_Item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		//	컬럼 번호
		t_Item.iSubItem = p_Col_N;
		//	라인 번호
		t_Item.iItem = p_Row_N;
		//	문자열
		t_Item.pszText = (LPSTR)t_Text;
		//	임의의 데이터 버퍼
		t_Item.lParam = (LPARAM)p_Data;
		//	이미지 index
		t_Item.iImage = p_Row_N;
		//	추가
		SendMessageA(m_Handle, LVM_INSERTITEMA, 0, (LPARAM)&t_Item);
	}

	return true;
}


//	이미지 추가
BOOL	ListView::Set_ImgIcon(HICON p_HIcon)
{
	if( p_HIcon == NULL)
		return false;

	// 이미지 리스트에 아이콘 추가
	ImageList_AddIcon(m_HImgList, p_HIcon);  
	return true;
}

BOOL	ListView::Set_ImgBmp(HBITMAP p_HBitmap)
{
	if( p_HBitmap == NULL)
		return false;

	// 이미지 리스트에 아이콘 추가
	ImageList_Add(m_HImgList, p_HBitmap,NULL);  
	return true;
}
//	이미지 리스트 등록
BOOL	ListView::RegistImgList()
{
	//	이미지 리스트 등록
	ListView_SetImageList(m_Handle, m_HImgList, LVSIL_SMALL); 
	return true;
}




//	리스트의 특정 텍스트 값 가져오기
BOOL ListView::Get_Data(TCHAR* data,int size, int p_Row_N, int p_Col_N)
{
	if(	data == NULL || size < 1 || p_Row_N < 0 || p_Col_N < 0)
		return false;
	
	ListView_GetItemText(m_Handle,p_Row_N,p_Col_N,data,size);
	return true;
}


//	리스트 뷰 통보 처리
int		ListView::Notify(WPARAM wParam, LPARAM lParam)
{
	//	통보 메시지 구조체 포인터 형변환
	LPNMHDR	t_hdr;
	t_hdr = (LPNMHDR)lParam;
	if( t_hdr->hwndFrom != m_Handle )
	{
		return -1;
	}

	switch (t_hdr->code){
	case LVN_ITEMCHANGED:
		{
			//	해당 리스트 뷰 선택 메시지 도착
			LPNMLISTVIEW	p_LSV = (LPNMLISTVIEW)lParam;
			//	새로운 리스트 선택 확인
			if(p_LSV->uChanged == LVIF_STATE && p_LSV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
			{
				//	리스트 추가 지정한 데이터 포인터
				m_Select = (LPVOID)p_LSV->lParam;
				return (p_LSV->iItem);
			}
			break;
		}
	case NM_DBLCLK:
	case NM_CLICK:
		{
			//	클릭시 선택 레코드 인덱스값 얻기
			m_SelctedRecordIndex = (int)SendMessage(m_Handle,LVM_GETSELECTIONMARK,0,0);

			break;
		}
	default:
		{
			break;
		}
	}
	
	return t_hdr->code;
}

//	모든 내용 삭제
BOOL	ListView::Clear()
{
	if(m_Handle != NULL)
	{
		// 리스트 뷰의 모든 내용 삭제
		ListView_DeleteAllItems(m_Handle);
		return true;
	}

	return false;
	
}


//	리스트 뷰 아이템 수
int	ListView::GetRecordCount()
{
	return (int)SendMessage(m_Handle, LVM_GETITEMCOUNT, 0,0);
}

//	선택된 레코드 인덱스 얻기
int	 ListView::GetSelectedRCIdx()
{
	return m_SelctedRecordIndex;
}

//	선택된 레코드 삭제
BOOL ListView::Delete_Data(int index)
{
	if(index < 0)
	{
		return false;
	}
	
	return  ListView_DeleteItem(m_Handle, index);
}
//	레코드 체크 여부
BOOL	ListView::IsCheckedRecord(int index)
{
	if(index < 0)
	{
		return false;
	}
	
	return ListView_GetCheckState(m_Handle, index);
}

//	레코드 찾기 (데이터로 찾기)
int	ListView::FindRecord(TCHAR* p_Data,int p_Data_Size, int p_Col)
{
	TCHAR t_Data[1024];
	ZeroMemory( t_Data, 1024 *sizeof(TCHAR));

	int rValue = 0;
	for(int i = 0 ; i < GetRecordCount(); i++)
	{
		Get_Data(t_Data,1024,i, p_Col);
		
		if(_tcscmp(p_Data, t_Data) == 0)
		{
			rValue = i;
			break;
		}

	}
	return rValue;
 
}

//	레코드의 연결 데이터 보여주기
LPVOID	ListView::GetData(int p_Rows,int p_Cols)
{
	//	구조체 선언
	LVITEM	t_Item;
	memset(&t_Item,0,sizeof(LVITEMA));
	//	구조체 내부에 사용할 정보 설정
	t_Item.mask =  LVIF_PARAM;
	//	컬럼 번호
	t_Item.iSubItem = p_Cols;
	//	라인 번호
	t_Item.iItem = p_Rows;

	//	레코드 데이터 얻기
	ListView_GetItem(m_Handle, &t_Item);

	return (LPVOID)t_Item.lParam;
}
//	레코드 체크 하기
BOOL	ListView::SetCheckRecord(int index)
{
	ListView_SetCheckState(m_Handle,index,TRUE);
	return TRUE;
}
//	레코드 체크 풀기
BOOL	ListView::SetUncheckRecord(int index)
{
	ListView_SetCheckState(m_Handle,index,FALSE);
	return TRUE;
}