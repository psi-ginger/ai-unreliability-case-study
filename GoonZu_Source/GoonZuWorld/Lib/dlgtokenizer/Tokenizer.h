#include "./DLGDefine.h"

#define MAX_CONTROL_PROPERTY_NUM  30
#define MAX_HEADER_TYPE_LENGTH 30

class CTokenizer
{
public:
// Construction/destruction
	CTokenizer( char* pstrInput, const char strHeaderDelimiter = DLG_HEADER_SPLITTER_CHAR, const char strDelimiter = DLG_ITEM_SPLITTER_CHAR, const char strEndline = DLG_ENDLINE_CHAR );
	~CTokenizer();

// Operations
	void Init( char* pstrInput, const char strHeaderDelimiter = DLG_HEADER_SPLITTER_CHAR, const char strDelimiter = DLG_ITEM_SPLITTER_CHAR, const char strEndline = DLG_ENDLINE_CHAR );
	int GetSize(  ) const;
	bool GetAt( int nIndex, char* pchar, int max_length) const;
   
// Attributes

public:
	char  m_headertype[MAX_HEADER_TYPE_LENGTH];	// 헤더 타입 저장 변수
	char* m_pstra[MAX_CONTROL_PROPERTY_NUM];	// 문자열에서 추출한 각 속성열 속성을 가리키는 포인터 배열
    int	  m_propertycount;	// 문자열에서 추출한 속성의 총 개수, m_propertycount < MAX_CONTROL_PROPERTY_NUM

};
