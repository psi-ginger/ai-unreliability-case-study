#ifndef _LIBOBJBUFFER_H_
#define _LIBOBJBUFFER_H_

#include "LibListBaseInfo.h"

class CLibObjBuffer
{
public:
	CLibObjBuffer( UI32 uiMaxBufferSize );							// 생성자
	~CLibObjBuffer();												// 소멸자

	void Initialize();												// 초기화
	void Destroy();													// 종료처리

	void SetData( BYTE *pData, UI32 ui32DataSize );					// 데이터를 버퍼에 삽입
	void GetData( BYTE *pData, UI32 *pDataSize );					// 버퍼로부터 데이터를 읽어서 복사해서 넣어줌
	BYTE *GetDataPtr();												// 버퍼의 데이터 포인터를 반환, 포인터로 데이터 접근 정도로 사용할 것이면 이게 훨씬 빠르므로..., 단 주의할 점은 읽는 동안에는 데이터가 절대 변해서는 안 된다는 것이다.

	bool IsDataExist();												// 데이터 사이즈를 받아오는 것보다 데이터 있느냐 없느냐만 체크하는게 주된 것이므로 데이터가 있느냐만 체크 ( 이게 성능이 더 좋음 )	

private:
	BYTE	*m_pData;
	UI32	m_uiDataSize;
};

#endif