#include "DrawSequenceBuffer.h"
#include "DrawSequenceDataObj.h"

CDrawSequenceBuffer::CDrawSequenceBuffer()
{	
	Initialize();
}

CDrawSequenceBuffer::~CDrawSequenceBuffer()
{
	Destroy();	
}

void CDrawSequenceBuffer::Initialize()
{
	m_siDrawSequenceDataObjCount = 0;
	m_pFirst = NULL;
	m_pLast = NULL;

	return;
}

void CDrawSequenceBuffer::Destroy()
{
	CDrawSequenceDataObj *pTemp = m_pFirst;
	CDrawSequenceDataObj *pTemp1 = NULL;
	
	while ( pTemp )
	{
		pTemp1 = pTemp;
		pTemp = pTemp->GetNext();
		delete pTemp1;
		pTemp1 = NULL;
		--m_siDrawSequenceDataObjCount;
	}

	m_pFirst = NULL;
	m_pLast = NULL;

	m_siDrawSequenceDataObjCount = 0;

	return;
}

void CDrawSequenceBuffer::Push( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize, char *pData )
{
	CDrawSequenceDataObj *pTemp = new CDrawSequenceDataObj( XPos, YPos, XSize, YSize, pData );

	if ( m_pFirst )
	{
		CDrawSequenceDataObj *pTemp1 = m_pFirst;
		
		while ( pTemp1 )
		{
			// -------------- 다음에 출력되어야 하는 경우
			if ( pTemp1->IsDrawingLater( XPos, YPos, XSize, YSize ) )
			{
				pTemp1 = pTemp1->GetNext();
				continue;
			}
			// -------------- 이전에 출력되어야 하는 경우
			else
			{
				if ( pTemp1->GetPre() )
				{
					pTemp->SetNext( pTemp1 );
					pTemp->SetPre( pTemp1->GetPre() );

					pTemp1->GetPre()->SetNext( pTemp );
					pTemp1->SetPre( pTemp );
				}
				else
				{
					pTemp->SetNext( pTemp1 );
					pTemp1->SetPre( pTemp );
					m_pFirst = pTemp;
				}


				++m_siDrawSequenceDataObjCount;

				return;
			}
		}

		if ( pTemp1 )
		{
			if ( pTemp1->GetNext() )
			{
				pTemp->SetNext( pTemp1->GetNext() );
				pTemp->SetPre( pTemp1 );

				pTemp1->GetNext()->SetPre( pTemp );
				pTemp1->SetNext( pTemp );
			}
			else
			{
				pTemp1->SetNext( pTemp );
				pTemp->SetPre( pTemp1 );

				m_pLast = pTemp;
			}
		}
		else
		{
			m_pLast->SetNext( pTemp );
			pTemp->SetPre( m_pLast );
			m_pLast = pTemp;
		}
	}
	else
	{
		m_pFirst = pTemp;
		m_pLast = pTemp;
	}


	++m_siDrawSequenceDataObjCount;

	return;
}

bool CDrawSequenceBuffer::Pop( char *pData, SI32 si32Size )
{
	if ( m_siDrawSequenceDataObjCount == 0 )
	{
		return false;
	}

	if ( si32Size > MAX_DRAW_DATA_SIZE )
	{
		MessageBox( NULL, TEXT("Size Over"), TEXT("error"), MB_OK |MB_TOPMOST);
		return false;
	}


	CDrawSequenceDataObj *pTemp = m_pFirst;
	memcpy( pData, pTemp->GetDrawSequenceData(), si32Size );
	
	if ( m_siDrawSequenceDataObjCount > 1 )
	{
		m_pFirst->GetNext()->SetPre( NULL );
		m_pFirst = m_pFirst->GetNext();
	}
	else
	{
		m_pFirst = NULL;
		m_pLast = NULL;
	}

	--m_siDrawSequenceDataObjCount;

	delete pTemp;
	pTemp = NULL;

	return true;

}