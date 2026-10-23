class GMTMessageReq_K_GOONZU			// 게임방송공지
{
    int32 nIdx;				// 메시지 index
    string strMessage;		// 방송 메시지
    int32 nContinueTime;		// 공지 지속시간(초)
};

class GMTMessageAns_K_GOONZU
{
    int32 nIdx;
    int32 nErrCode;
};

class GMTKickReq_K_GOONZU			// 강퇴
{
    int32 nIdx;				// 메시지 index
    string strPlayerName;		// 캐릭터명
    string strTime;			// 지속시간(분) -1 : 지속시간 없음(단순 강퇴), 0 : 게임접속 블록 해제, >0 지속시간(분)
};

class GMTKickAns_K_GOONZU
{
    int32 nIdx;
    int32 nErrCode;
};

class GMTChatBlockReq_K_GOONZU		// 채팅 블록
{
    int32 nIdx;				// 메시지 index
    string strPlayerName;		// 캐릭터명
    string strTime;			// 지속시간(분) 0 : 채팅 블록 해제, >0 지속시간(분)
};

class GMTChatBlockAns_K_GOONZU
{
    int32 nIdx;
    int32 nErrCode;
};

class GMTCmdReq_K_GOONZU			// 게임머니, 아이템 추가
{
    int32 nIdx;				// 메시지 index
    int32 nServerId;			// 서버 id
    string strCmd;			// 게임머니, 아이템 추가
};

class GMTCmdAns_K_GOONZU
{
    int32 nIdx;
    int32 nErrCode;
};
