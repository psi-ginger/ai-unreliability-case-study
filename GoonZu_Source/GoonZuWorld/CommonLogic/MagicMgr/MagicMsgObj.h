#ifndef _MAGICMSGOBJ_H_
#define _MAGICMSGOBJ_H_

#include "MagicBaseInfo.h"
#include "../../Common/SMLib/LibList/Obj.h"

class	CMagicMsgObj : public CObj
{
public:
	CMagicMsgObj();
	~CMagicMsgObj();

	void				Initialize();
	void				Destroy();

	void				GetMagicMsg( BYTE *pData, SI32 *pMsgSize );
	void				SetMagicMsg( BYTE *pData, SI32 si32MsgSize );
	
private:
	TCHAR				m_MagicMsgData[ MAX_MAGIC_MSG_LENGTH ];
	SI32				m_si32MsgSize;



};

#endif