#include "ITZPerson.h"
#include "ITZSession.h"

ITZPerson::ITZPerson()
{
	m_pSession = NULL;
	m_uiCharID = 0;

	m_pMap = NULL;
	m_pCell = NULL;
	m_siCellPersonIndex = -1;
}

ITZPerson::~ITZPerson()
{
}

ITZRecvBuffer* ITZPerson::GetRecvBuffer()
{
	if( m_pSession ) return m_pSession->GetRecvBuffer();

	return NULL;
}

ITZSendBuffer* ITZPerson::GetSendBuffer()
{
	if( m_pSession ) return m_pSession->GetSendBuffer();

	return NULL;
}

