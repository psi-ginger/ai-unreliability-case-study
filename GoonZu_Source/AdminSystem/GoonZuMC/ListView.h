#pragma once

//===============================================================
// 제작자 : 박기형
// 날짜 :  2009.1 .25
// 설명 :  리스트 뷰 클래스
//===============================================================

#include <commctrl.h>

////	리스트 뷰 컨트롤 클래스
class ListView
{

	////	생성자 소멸자
public:
	ListView(void);
	~ListView(void);

	////	클래스 내부 변수
public:
	//	이미지 리스트
	HIMAGELIST m_HImgList;

	//	리스트 뷰 윈도우 핸들
	HWND	m_Handle;	
	//	설정 된 컬럼 개수
	int		m_ColCount;
	//	선택된 데이터 포인터
	LPVOID	m_Select;
	//	유형 ( 0 : 외부핸들 사용, 1 : 내부 자체 생성)
	BOOL	m_Type;
	//	선택된 레코드 인덱스
	int		m_SelctedRecordIndex;

	////	클래스 내부 함수
public:
	//	생성함수
	BOOL	Create(BOOL p_Type, HWND p_Hwnd, char *p_Tit = NULL, int Xx = 0, int Yy = 0, int Ww = 200, int Hh = 200, BOOL p_Check = FALSE);
	//	삭제함수
	BOOL	Release();
	
	//	초기 사용할 컬럼 설정
	BOOL	Set_Col(char *p_Tit, int p_size = 50);

	//	리스트 데이터 추가
	BOOL	Set_Data(int p_Row_N, int p_Col_N, TCHAR *p_Text, VOID *p_Data = NULL);
	BOOL	Set_Data(int p_Row_N, int p_Col_N, int p_Value, VOID *p_Data = NULL);
	//	이미지 추가
	BOOL	Set_ImgIcon(HICON p_HIcon);
	BOOL	Set_ImgBmp(HBITMAP p_HBitmap);
	//	이미지 리스트 등록
	BOOL	RegistImgList();

	//	리스트의 특정 텍스트 값 가져오기
	BOOL	Get_Data(TCHAR* data,int size, int p_Row_N, int p_Col_N);
	
	//	특정 셀의 연결 데이터 보여주기
	LPVOID	GetData(int p_Rows,int p_Cols );

	//	리스트 뷰 통보 처리
	int		Notify(WPARAM wParam, LPARAM lParam);

	//	리스트 뷰 아이템 수
	int		GetRecordCount();

	//	모든 내용 삭제
	BOOL	Clear();

	//	선택된 레코드 삭제
	BOOL	Delete_Data(int index);

	//	선택된 레코드 인덱스 얻기
	int		GetSelectedRCIdx();

	//	레코드 체크 여부
	BOOL	IsCheckedRecord(int index);

	//	레코드 체크 하기
	BOOL	SetCheckRecord(int index);

	//	레코드 체크 풀기
	BOOL	SetUncheckRecord(int index);

	//	레코드 찾기 (셀의 문자열 값으로)
	int		FindRecord(TCHAR* p_Data,int p_Data_Size,int p_Col);
};
