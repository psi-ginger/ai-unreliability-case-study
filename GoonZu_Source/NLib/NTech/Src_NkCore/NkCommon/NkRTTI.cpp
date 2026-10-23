#include "NkRTTI.h"
#include <cstring>

//------------------------------------------------------------------------------
// NkRTTI::NkRTTI
//------------------------------------------------------------------------------
NkRTTI::NkRTTI(const char* pcName, const NkRTTI* pkBaseRTTI) :
	m_pcName(pcName), m_pkBaseRTTI(pkBaseRTTI)
{
}

//------------------------------------------------------------------------------
// NkRTTI::CopyName
//------------------------------------------------------------------------------
bool NkRTTI::CopyName(char* acNameBuffer, unsigned int uiMaxSize) const
{
	const char* pcName = GetName();
	if (!pcName || !acNameBuffer)
	{
		strcpy(acNameBuffer, "\0");
		return false;
	}

	strcpy(acNameBuffer, pcName);
	return true;
}

//------------------------------------------------------------------------------
