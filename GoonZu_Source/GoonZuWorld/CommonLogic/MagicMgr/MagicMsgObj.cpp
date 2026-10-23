#include "MagicMsgObj.h"

CMagicMsgObj::CMagicMsgObj()
{
	Initialize();
}

CMagicMsgObj::~CMagicMsgObj()
{
	Destroy();
}

void CMagicMsgObj::Initialize()
{
	memset ( m_MagicMsgData, 0, MAX_MAGIC_MSG_LENGTH );
	m_si32MsgSize = 0;
	return;
}

void CMagicMsgObj::Destroy()
{
	return;
}

void CMagicMsgObj::GetMagicMsg( BYTE *pData, SI32 *pMsgSize )
{
	memcpy( pData, m_MagicMsgData, m_si32MsgSize );
	*pMsgSize = m_si32MsgSize;
	return;
}

void CMagicMsgObj::SetMagicMsg( BYTE *pData, SI32 si32MsgSize )
{
	memcpy( m_MagicMsgData, pData, si32MsgSize );	
	m_si32MsgSize = si32MsgSize;

	return;
}
