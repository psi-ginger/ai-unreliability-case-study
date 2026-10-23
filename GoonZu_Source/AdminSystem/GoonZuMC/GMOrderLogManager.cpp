#include ".\gmorderlogmanager.h"

CGMOrderLogManager::CGMOrderLogManager(void)
{
	m_pGMOrderLogBuffer = NULL;
}

CGMOrderLogManager::~CGMOrderLogManager(void)
{
	if(m_pGMOrderLogBuffer != NULL)
	{
		delete m_pGMOrderLogBuffer;
		m_pGMOrderLogBuffer = NULL;
	}
}
BOOL	CGMOrderLogManager::Create()
{
	if(m_pGMOrderLogBuffer != NULL)
		return FALSE;

	m_pGMOrderLogBuffer = new StaticBuffer();
	m_pGMOrderLogBuffer->Create(sizeof(CGMOrderLog), ConstGMOrderBufferSize);

	return TRUE;
}
