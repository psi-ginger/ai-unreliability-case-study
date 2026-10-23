#ifndef _MINISTERMGR_H_
#define _MINISTERMGR_H_

enum  
{
	MINISTER_TYPE_START = 1,
	MINISTER_IZO = MINISTER_TYPE_START,	// 이조판서 
	MINISTER_HOZO,						// 호조 
	MINISTER_YEZO,						// 예조
	MINISTER_BYUNGZO,					// 병조 
	MINISTER_HYUNGZO,					// 형조 
	MINISTER_GONGZO,					// 공조 
	MINISTER_GOONZU,					// 군주
	MINISTER_LAST = MINISTER_GOONZU,
	MINISTER_TOTAL,
};

class CMinisterBase;

class CMinisterMgr
{

public:
#ifdef _SAFE_MEMORY
	NSafeTArray<CMinisterBase*, MINISTER_TOTAL>		m_MinisterBase;
#else
	CMinisterBase	*m_MinisterBase[MINISTER_TOTAL];		// 6조의 Pointer 
#endif


	CMinisterMgr();
	~CMinisterMgr();


	void	Initialize();
	void	Destroy();
	CMinisterBase *GetMinister(SI32 siKind);

	//===============================================================================
	// SERVER - DB : Server가 초기 부팅시 DB로부터 정보를 얻어온다.
	//===============================================================================
	// DB에 Get을 요청한다.
	void	InitializeDB();			
	// DB에서 판서 정보를 받으면 값을 Class에 설정한다.
	void	DoMsg_DBMsgResponse_GetMinisterInfo(SI32 siKind, sPacketHeader* pPacket);
	//===============================================================================

	//===============================================================================
	// CLIENT - SERVER : Client의 요청에 따라 Server의 정보를 전달 
	//===============================================================================
	// Client에서 판서의 정보를 요청하면 지금 Class에 있는 정보를 전송한다.
	void	DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID, SI32 siKind);
	//===============================================================================

	//===============================================================================
	// CLIENT - SERVER - DB : Client의 Set요청을 DB를 통해 설정하고 Client에 전달한다.
	//===============================================================================
	// Client에서 판서정보를 새로 설정하기를 원하면 서버에서는 DB에 정보 설정을 요청한다.
	void	DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID, SI32 siKind);
	// DB에서 정보의 쓰기가 완료되면 Client에게 다시 전송하여준다.
	void    DoMsg_DBMsgResponse_SetMinisterInfo(SI32 siKind, sPacketHeader* pPacket);
	//===============================================================================
};



#endif