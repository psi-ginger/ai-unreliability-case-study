#include "DrawSequenceObj.h"

CDrawSequenceObj::CDrawSequenceObj() : CObj()
{
	Initialize();
}

CDrawSequenceObj::~CDrawSequenceObj()
{
	Destroy();
}

void CDrawSequenceObj::Initialize()
{
	m_si32XPos = 0;
	m_si32YPos = 0;
	m_si32XSize = 0;
	m_si32YSize = 0;

	return;
}

void CDrawSequenceObj::Destroy()
{
	return;
}

void CDrawSequenceObj::SetData( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize, cltDrawObjData *pclData )
{
	m_si32XPos = XPos;
	m_si32YPos = YPos;
	m_si32XSize = XSize;
	m_si32YSize = YSize;

	m_clDrawData = *pclData;
	//memcpy( &m_DrawData, pData, MAX_DRAW_DATA_SIZE );

	return;
}

int CDrawSequenceObj::IsDrawingLater( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize )
{
	if ( m_si32XPos >= XPos && m_si32XPos + m_si32XSize <= XPos + XSize && m_si32YPos >= YPos && m_si32YPos + m_si32YSize <= YPos + YSize )
	{
		return LATER_DRAW_SEQUENCE;
	}
	
	if ( m_si32XPos + m_si32XSize > XPos && m_si32YPos + m_si32YSize > YPos )
	{

		if ( m_si32XPos >= XPos && m_si32XPos + m_si32XSize <= XPos + XSize && m_si32YPos + m_si32YSize > YPos + YSize )
		{
			return BEFORE_DRAW_SEQUENCE;
		}
		else if ( m_si32YPos >= YPos && m_si32YPos + m_si32YSize <= YPos + YSize && m_si32XPos + m_si32XSize > XPos + XSize )
		{
			return BEFORE_DRAW_SEQUENCE;
		}
		
		
		if ( m_si32XPos + m_si32XSize - XPos > m_si32YPos - ( YPos + YSize ) ||  m_si32XPos - ( XPos + XSize ) < m_si32YPos + m_si32YSize - YPos )
		{
			return BEFORE_DRAW_SEQUENCE;
		}
		
		/*
		if ( m_si32YPos + m_si32YSize > YPos + YSize )
		{
			if ( m_si32XPos + m_si32XSize > XPos && m_si32XPos + m_si32XSize <= XPos + XSize )
			{
				return BEFORE_DRAW_SEQUENCE;
			}

			if ( 

		}
		else if ( m_si32XPos + m_si32XSize > XPos + XSize )
		{
		}
		*/
		
		return NOT_RELATIVE_SEQUENCE;
	}
	else
	{
		if ( XPos + XSize > m_si32XPos && YPos + YSize > m_si32YPos )
		{
			/*
			if ( XPos + XSize - m_si32XPos > YPos - ( m_si32YPos + m_si32YSize ) && XPos - ( m_si32XPos + m_si32XSize ) < YPos + YSize - m_si32YPos )
			{
				return LATER_DRAW_SEQUENCE;
			}
			else if ( XPos + XSize - m_si32XPos < YPos - ( m_si32YPos + m_si32YSize ) && XPos - ( m_si32XPos + m_si32XSize ) > YPos + YSize - m_si32YPos )
			{
				return LATER_DRAW_SEQUENCE;
			}
			*/

			if ( XPos >= m_si32XPos && XPos + XSize <= m_si32XPos + m_si32XSize && YPos + YSize > m_si32YPos + m_si32YSize )
			{
				return LATER_DRAW_SEQUENCE;
			}
			else if ( YPos >= m_si32YPos && YPos + YSize <= m_si32YPos + m_si32YSize && XPos + XSize > m_si32XPos + m_si32XSize )
			{
				return LATER_DRAW_SEQUENCE;
			}

			if ( XPos + XSize - m_si32XPos > YPos - ( m_si32YPos + m_si32YSize ) ||  XPos - ( m_si32XPos + m_si32XSize ) < YPos + YSize - m_si32YPos )
			{
				return LATER_DRAW_SEQUENCE;
			}
			
		}
		
		return NOT_RELATIVE_SEQUENCE;
	}
	
	/*
	if ( m_si32XPos + m_si32XSize < XPos + XSize && m_si32YPos < YPos + YSize )
	{
		return true;
	}
	

	if ( m_si32YPos + m_si32YSize < YPos + YSize && m_si32XPos < XPos + XSize )
	{
		return true;
	}
	
	// 새로운 것이 기존것 보다 X 축으로 적거나 같은 곳에 있고, Y축으로 크거나 같은 곳에 있는 경우
	// 새로운 것이 기존것 보다 X 축으로 크거나 같은 곳에 있고, Y축으로 작거나 같은 곳에 있는 경우
	// 새로운 것이 기존것 보다 X축, Y축 모두 크거나 같은 곳에 있는 경우	
	if ( ( m_si32XPos >= XPos + XSize && m_si32YPos + m_si32YSize <= YPos ) || ( m_si32XPos + m_si32XSize <= XPos && m_si32YPos >= YPos + YSize ) || ( m_si32XPos + m_si32XSize <= XPos && m_si32YPos + m_si32YSize <= YPos ) ) 
	{
		return true;
	}

	if ( ( m_si32XPos >= XPos + XSize && m_si32XPos - ( XPos + XSize ) >= m_si32YSize ) || ( m_si32YPos >= YPos + YSize && m_si32YPos - ( YPos + YSize ) >= m_si32XSize ) )
	{
		return true;
	}

	return false;
	*/
}

cltDrawObjData *CDrawSequenceObj::GetDrawSequenceData()
{
	return &m_clDrawData;
}

/*
CDrawSequenceObj *CDrawSequenceObj::GetVerticalPre()
{
	return m_pVerticalPre;
}

void CDrawSequenceObj::SetVerticalPre( CDrawSequenceObj	*pObj )
{
	m_pVerticalPre = pObj;
	return;
}

CDrawSequenceObj *CDrawSequenceObj::GetVerticalNext()
{
	return m_pVerticalNext;
}

void CDrawSequenceObj::SetVerticalNext( CDrawSequenceObj	*pObj )
{
	m_pVerticalNext;
	return;
}
*/