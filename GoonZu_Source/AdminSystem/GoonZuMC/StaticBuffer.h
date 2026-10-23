#pragma once

//===============================================================
// 제작자 : 박기형
// 설명 :  고정 버퍼 클래스
//		   다중 쓰레드 사용 가능
//===============================================================

#include <windows.h>

class StaticBuffer
{
//	내부 버퍼
protected:
	//	전체 버퍼
	BYTE*	m_Buffer;
	//	1개 데이터 크기
	DWORD	m_Size;
	//	전체 데이터 개수
	DWORD	m_Max_Count;
	
	//	입력 지시자
	DWORD	m_In_Index;
	//	출력 지시자
	DWORD	m_Out_Index;
	//	순환 값
	bool	m_Cyc_TF;

//	내부 메소드
protected:
	//	멤버 변수 초기화
	void	init();

public:
	//	버퍼 아이디
	DWORD	m_Buffer_ID;

public:
	StaticBuffer(void);
	~StaticBuffer(void);
	
	//	생성
	BOOL	Create(DWORD p_Size, DWORD p_Count);
	//	삭제
	BOOL	Delete();

	//	버퍼에 데이터 입력
	BOOL	In_Data_Buffer(BYTE* p_In);
	BOOL	In_Data_Buffer(BYTE* p_In,DWORD p_Size, DWORD p_StartPoint = 0);
	//	버퍼에 분할해서 데이터에 입력(DWORD p_StartPoint <= 시작주소)
	BOOL	In_Data_Buffer_N(BYTE* p_In, DWORD p_Size, DWORD p_StartPoint);

	//	버퍼에 데이터 출력
	BOOL	Out_Data_Buffer(BYTE * p_Out);
	BOOL	Out_Data_Buffer(BYTE * p_Out,DWORD p_Size);

	//	버퍼 데이터 참조값 얻기
	BYTE*	ReadData();

	// 1개 버퍼 사이즈
	DWORD	GetSize();

	//	가지고 있는 데이터 수
	DWORD	GetExistDataCount();
};