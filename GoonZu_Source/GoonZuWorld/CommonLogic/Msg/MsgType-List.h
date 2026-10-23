#ifndef _MSGTYPELIST_H
#define _MSGTYPELIST_H

#define LISTID_POSTOFFICE_RECVITEM			10

class cltGameMsgResponse_ListDeleteAll
{
public:
	SI32	siListID;

	cltGameMsgResponse_ListDeleteAll( SI32 listid )
	{
		siListID = listid;
	}
};

class cltGameMsgResponse_ListDelItem
{
public:
	SI32	siListID;
	LPARAM	lParam;

	cltGameMsgResponse_ListDelItem( SI32 listid, LPARAM lparam )
	{
		siListID = listid;
		lParam = lparam;
	}

};

#endif
