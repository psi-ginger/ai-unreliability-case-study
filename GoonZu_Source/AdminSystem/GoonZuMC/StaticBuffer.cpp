#include "StaticBuffer.h"

StaticBuffer::StaticBuffer(void)
{
	//	전체 버퍼
	m_Buffer = NULL;

	//	버퍼외의 변수 초기화
	init();
}

StaticBuffer::~StaticBuffer(void)
{
	if(m_Buffer != NULL)
	{
		free(m_Buffer);
		m_Buffer = NULL;
	}
}
//	멤버 변수 초기화
void	StaticBuffer::init()
{
	//	1개 데이터 크기
	m_Size = 0;
	//	전체 데이터 개수
	m_Max_Count = 0;
	//	입력 지시자
	m_In_Index = 0;
	//	출력 지시자
	m_Out_Index = 0;
	//	순환 값
	m_Cyc_TF = false;
	//	버퍼 아이디
	m_Buffer_ID = 0;
}

//	생성
BOOL	StaticBuffer::Create(DWORD p_Size, DWORD p_Count)
{
	//	기존 버퍼 삭제
	Delete();
	m_Size = p_Size;
	m_Max_Count = p_Count;

	//	버퍼 할당
	m_Buffer = (BYTE*) malloc(m_Max_Count * m_Size);
	if( m_Buffer == NULL)
		return false;
	//	버퍼 초기화	
	//memset(m_Buffer,0, m_Max_Count * m_Size);
	return true;
}
//	삭제
BOOL	StaticBuffer::Delete()
{
	if(m_Buffer != NULL)
	{
		//	버퍼 초기화	
		memset(m_Buffer,0, m_Max_Count * m_Size);
	}
	// 초기화
	init();

	return true;
}

//	버퍼에 데이터 입력
BOOL	StaticBuffer::In_Data_Buffer(BYTE* p_In)
{
	if(p_In == NULL)
		return false;

	return In_Data_Buffer(p_In, m_Size);
}
BOOL	StaticBuffer::In_Data_Buffer(BYTE* p_In,DWORD p_Size, DWORD p_StartPoint)
{
	//	인자 확인
	if(p_In == NULL)
		return false;
	if(p_Size > m_Size) 
		return false;
	if(m_Buffer == NULL)
		return false;

	//	1. cyc, t 입력 >= 출력 return false
	if(m_In_Index< m_Max_Count)
	{
		//	2. 데이터 복사
		memcpy((LPVOID)&(m_Buffer[m_In_Index * m_Size + p_StartPoint]), p_In, p_Size);

		//	3. 입력 인덱스 증가
		++m_In_Index;
		//	4. 순환 확인
		if(	m_In_Index >= m_Max_Count)
		{
			m_Cyc_TF = true;
			m_In_Index = 0;
		}
	}
	else
	{
		//	버퍼 오류
		m_Cyc_TF = false;
		m_In_Index = 0;
		
	}
	return true;
}

//	버퍼에 데이터 출력
BOOL	StaticBuffer::Out_Data_Buffer(BYTE * p_Out)
{
	//	인자 확인
	if(p_Out == NULL)
		return false;

	return Out_Data_Buffer(p_Out, m_Size);
}
BOOL	StaticBuffer::Out_Data_Buffer(BYTE * p_Out,DWORD p_Size)
{
	//	인자 확인
	if(p_Out == NULL)
		return false;
	if(p_Size > m_Size) 
		return false;
	if(m_Buffer == NULL)
		return false;

	//	버퍼의 내용 뺴오기
	if(m_Cyc_TF)
	{
		// 루프가 설정된 경우
		if(m_Max_Count > m_Out_Index)
		{
			// 버퍼에 저장
			memcpy(p_Out,&m_Buffer[m_Size*m_Out_Index], p_Size);
			m_Out_Index++;
			if(m_Out_Index >= m_Max_Count)
			{
				// 루프 초기화
				m_Cyc_TF = false;
				m_Out_Index = 0;
			}
			return true;
		}
		else
		{
			// 루프 초기화
			m_Cyc_TF = false;
			m_Out_Index = 0;
		}
	}
	else if(m_In_Index > m_Out_Index)
	{
		// 버퍼에 저장
		memcpy(p_Out,&m_Buffer[m_Size*m_Out_Index], p_Size);
		m_Out_Index++;
		if(m_Out_Index >= m_Max_Count)
		{
			m_Out_Index = 0;
		}
		return true;
	}
	return false;
}

//	버퍼 데이터 참조값 얻기
BYTE*	StaticBuffer::ReadData()
{
	if(m_Buffer == NULL)
		return NULL;

	return &m_Buffer[m_Size*m_Out_Index];;
}

// 1개 버퍼 사이즈
DWORD	StaticBuffer::GetSize()
{
	return m_Size;
}

//	가지고 있는 데이터 수
DWORD	StaticBuffer::GetExistDataCount()
{
	int rValue = 0;
	if(!m_Cyc_TF)
		rValue = m_In_Index - m_Out_Index;
	else
		rValue = m_Out_Index - m_In_Index;

	return rValue;
}

//	버퍼에 분할해서 데이터에 입력(DWORD p_StartPoint <= 시작주소)
BOOL	StaticBuffer::In_Data_Buffer_N(BYTE* p_In, DWORD p_Size, DWORD p_StartPoint)
{
	//	인자 확인
	if(p_In == NULL)
		return false;
	if(p_Size > m_Size) 
		return false;
	if(p_StartPoint > m_Size) 
		return false;
	if(p_Size + p_StartPoint > m_Size)
		return false;
	if(m_Buffer == NULL)
		return false;

	//	2. 데이터 복사
	memcpy((LPVOID)&(m_Buffer[m_In_Index * m_Size + p_StartPoint]), p_In, p_Size);

	return true;
}
