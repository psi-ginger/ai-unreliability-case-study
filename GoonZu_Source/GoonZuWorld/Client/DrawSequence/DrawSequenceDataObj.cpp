#include "DrawSequenceDataObj.h"

CDrawSequenceDataObj::CDrawSequenceDataObj( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize, char *pDrawSequenceData )
{
	m_si32XPos = XPos;
	m_si32YPos = YPos;
	m_si32XSize = XSize;
	m_si32YSize = YSize;

	memcpy( &m_DrawData, pDrawSequenceData, MAX_DRAW_DATA_SIZE );
	m_pPre = NULL;
	m_pNext = NULL;
}

CDrawSequenceDataObj::~CDrawSequenceDataObj()
{
}

CDrawSequenceDataObj* CDrawSequenceDataObj::GetPre()
{
	return m_pPre;
}

void CDrawSequenceDataObj::SetPre( CDrawSequenceDataObj * pTradeData )
{
	m_pPre = pTradeData;
	return;
}
	
CDrawSequenceDataObj* CDrawSequenceDataObj::GetNext()
{
	return m_pNext;
}

void CDrawSequenceDataObj::SetNext( CDrawSequenceDataObj * pNextData )
{
	m_pNext = pNextData;
	return;
}

bool CDrawSequenceDataObj::IsDrawingLater( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize )
{
	/*
	if ( m_si32XPos + m_si32XSize > XPos && m_si32YPos + m_si32YSize > YPos )
	{
		if ( m_si32XPos + m_si32XSize - XPos > m_si32YPos - ( YPos + YSize ) )
		{
			return false;
		}
		else if ( m_si32XPos - ( XPos + XSize ) < m_si32YPos + m_si32YSize - YPos )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}	
	*/

	// 위 주석 부분과 거의 같음. 그냥 연산속도를 올리려고 한 정도임
	SI32 m_XPosPlusXSize = m_si32XPos + m_si32XSize;
	SI32 m_YPosPlusYSize = m_si32YPos + m_si32YSize;


	if ( m_XPosPlusXSize < XPos || m_YPosPlusYSize < YPos )
	{
		return true;
	}
	else
	{
		if ( m_XPosPlusXSize - XPos > m_si32YPos - ( YPos + YSize ) )
		{
			return false;
		}
		else if ( m_si32XPos - ( XPos + XSize ) < m_YPosPlusYSize - YPos )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

char* CDrawSequenceDataObj::GetDrawSequenceData()
{
	return m_DrawData;
}