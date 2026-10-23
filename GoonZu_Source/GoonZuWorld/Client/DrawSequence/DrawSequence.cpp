#include "DrawSequence.h"
#include "DrawSequenceMgr.h"

CDrawSequence::CDrawSequence( SI16 DrawSequenceObjNum )
{
	//m_pDrawSequenceBuffer = new CDrawSequenceBuffer();
	m_pDrawSequenceMgr = new CDrawSequenceMgr();
	m_pDrawSequenceMgr->CreateMaxEmptyObj( DrawSequenceObjNum );

	Initialize();
}	

CDrawSequence::~CDrawSequence()
{
	Destroy();
}


void CDrawSequence::Initialize()
{
	return;
}

void CDrawSequence::Destroy()
{
	/*
	if ( m_pDrawSequenceBuffer )
	{
		delete m_pDrawSequenceBuffer;
		m_pDrawSequenceBuffer = NULL;
	}
	*/

	if ( m_pDrawSequenceMgr )
	{
		delete m_pDrawSequenceMgr;
		m_pDrawSequenceMgr = NULL;
	}

	return;
}

void CDrawSequence::Push( SI32 XPos, SI32 YPos, SI32 XSize, SI32 YSize, cltDrawObjData *pclData )
{
	m_pDrawSequenceMgr->Push( XPos, YPos, XSize, YSize, pclData );
	//m_pDrawSequenceBuffer->Push( XPos, YPos, XSize, YSize, pData );
	return;
}

cltDrawObjData *CDrawSequence::Pop()
{
	return m_pDrawSequenceMgr->Pop();
	//return m_pDrawSequenceBuffer->Pop( pData, si32Size );
}