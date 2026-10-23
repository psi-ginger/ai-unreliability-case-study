#include "Tokenizer.h"
#include <string.h>

////////////////////////////////////////////////////////////////////
// Public functions
//
CTokenizer::CTokenizer( char* pstrInput, const char strHeaderDelimiter, const char strDelimiter, const char strEndline )
{
	for(int i=0; i<MAX_CONTROL_PROPERTY_NUM; i++)
		m_pstra[i] = 0;

	m_propertycount = 0;

	Init(pstrInput, strHeaderDelimiter, strDelimiter, strEndline);
}

CTokenizer::~CTokenizer()
{
	for(int i=0; i<MAX_CONTROL_PROPERTY_NUM; i++)
	{
		if(m_pstra[i] != 0)
		{
			delete m_pstra[i];
			m_pstra[i] = 0;
		}
	}
}

void CTokenizer::Init( char* pstrInput, const char strHeaderDelimiter, const char strDelimiter, const char strEndline )
{

	char* pcopy=0;

	int i=0, length = 0;
	int nFound = 0;

	int start_pos = 0;	// 입력 받은 문자열에서의 처음 시작 위치
	int header_length = 0; // 헤더 부분 문자열 길이

	if(pstrInput == 0)
		return;

	// 0 string terminate 문자 제외한 전체 길이 리턴
	length = strlen(pstrInput);

	if(length < 1 || pstrInput[length] != '\0')
		return;

	pcopy = new char[length+1];

	strcpy(pcopy, pstrInput);

	// 라인 마지막 문자 체크
	if(pcopy[length-1] != strEndline)
	{
		delete pcopy;
		pcopy = 0;
		return;
	}
	
	// 마지막 문자( ';' ) 제외
	length--;

	// 초기화
	for(i=0; i<MAX_CONTROL_PROPERTY_NUM; i++)
	{
		if(m_pstra[i] != 0)
		{
			delete m_pstra[i];
			m_pstra[i] = 0;
		}
	}

	// 초기화 전 아이템 개수 초기화
	m_propertycount = 0;

	// 헤더 타입 가져 오기
	for(i=0; i<length; i++)
	{
	    if(pcopy[i] == strHeaderDelimiter)
		{
			break;
		}
	}

	if(i == length || i > MAX_HEADER_TYPE_LENGTH - 2)
	{
		delete pcopy;
		return;
	}

	// 시작 위치가 전체 길이를 넘는지 비교하기 위한 변수
	// 0 <= start_pos < length  : 유효 범위
	// 시작 위치 재설정
	// 구분자 뒤로 이동 : + 1
	start_pos = i + 1;

	header_length = i;

	for(i=0; i< header_length; i++)
		m_headertype[i] = pcopy[i];

	m_headertype[i] = '\0';	

	// 구분자로 끝났을 때
	if(start_pos >= length)
	{
		delete pcopy;
		return;
	}

	for(i=start_pos; i<length; i++)
	{
	    if(pcopy[i] == strDelimiter)
		{
			break;
		}
	}

	if(i == length)
	{
		delete pcopy;
		return;
	}
    
	// 문자열에서의 위치
	// nFound 값 : 찾는 문자의 배열에서의 위치
	nFound = i - start_pos;

	bool loop = true;
	while(loop)
	{
		m_pstra[m_propertycount]=0;
		m_pstra[m_propertycount] = new char[nFound+1];
		/*m_pstra[m_propertycount-1] = '\0';*/
		
		// 찾는 문자(구분자)는 제외한 값을 저장함
		for(i=0; i<nFound; i++)
			(m_pstra[m_propertycount])[i] = pcopy[start_pos + i];

		// 0 string terminator 저장
		// 연속된 구분자(,,,,) 이럴 경우 0 문자열 만듦
		(m_pstra[m_propertycount])[i] = '\0';

		m_propertycount++;

		// 시작 위치 재설정
		// 구분자 뒤로 이동 : + 1
		start_pos = start_pos + nFound + 1;

		// 구분자로 끝났을 때
		if(start_pos >= length)
		{
			nFound = 0;
			break;
		}
		
		// start_pos로 시작 위치 재설정
		for(i=start_pos; i<length; i++)
		{
			if(pcopy[i] == strDelimiter)
			{
				break;
			}
		}

		// 더 이상 구분자가 없을 때
		if(i == length)
		{
			nFound = length - start_pos;
			break;
		}

		// 구분자의 상대 위치값
		nFound = i - start_pos;
	}
	
	// 마지막 구분자 다음 내용 저장
	m_pstra[m_propertycount]=0;
	m_pstra[m_propertycount] = new char[nFound+1];
	
	// 찾는 문자(구분자)는 제외한 값을 저장함
	for(i=0; i<nFound; i++)
		(m_pstra[m_propertycount])[i] = pcopy[start_pos + i];

	// 0 string terminator 저장
	// 연속된 구분자(,,,,) 이럴 경우 0 문자열 만듦
	(m_pstra[m_propertycount])[i] = '\0';

	m_propertycount++;

}

int CTokenizer::GetSize(  ) const
{
	return m_propertycount;
}

// max_length : 입력 받아올 pchar 배열의 수, '\0' 개수도 포함한 값
bool CTokenizer::GetAt( int nIndex, char* pchar, int max_length) const
{
	int length = 0;
	// 각 control enum type이 0부터 시작하므로 nIndex
	if( nIndex < GetSize() )
	{
        length = strlen(m_pstra[nIndex]);

		if(length < max_length)
		{
			strcpy(pchar, m_pstra[nIndex]);
			return true;
		}
		else
		{
			pchar[0] = '\0';
			return false;
		}
	}
	else
	{
		pchar[0] = '\0';
		return false;
	}
}

