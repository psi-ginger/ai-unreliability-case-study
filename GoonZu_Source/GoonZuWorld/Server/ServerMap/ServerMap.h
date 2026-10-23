#ifndef _SERVERMAP_H
#define _SERVERMAP_H

#include "../NetworkLib/ITZNetLib/ITZMap.h"

class cltServerMap : public ITZMap
{
public:
	cltServerMap();
	~cltServerMap();

public:
	virtual void		SendPersonEnterMsgToOthers( ITZPerson *pPerson, bool bticketusewarpswitch );
	virtual void		SendPersonLeaveMsgToOthers( ITZPerson *pPerson );
	virtual void		SendPersonMoveMsgToOthers( ITZPerson *pPerson );
	
	virtual void		SendAddItemMsgToOthers( sItem *pItem );
	virtual void		SendDelItemMsgToOthers( sItem *pItem );

	virtual void		SendNewOthersInfoToPerson( ITZPerson *pPerson );
	virtual void		SendNewItemsInfoToPerson( ITZPerson *pPerson );
	
};

#endif