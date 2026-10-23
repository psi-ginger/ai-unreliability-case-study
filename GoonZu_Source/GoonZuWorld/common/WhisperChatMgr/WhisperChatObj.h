#ifndef _WHISPERCHATOBJ_H_
#define _WHISPERCHATOBJ_H_

#include "WhisperChatBaseDefine.h"
#include "../SMLib/LibList/Obj.h"

struct sPacketHeader;

struct stWhisperChatMember
{
	SI32 siCharID;
	SI32 siPersonID;
	TCHAR strCharName[ MAX_PLAYER_NAME ];

	void Init()
	{
		memset( this, 0, sizeof( stWhisperChatMember ) );
	}

	void Set( SI32 CharID, SI32 PersonID, TCHAR *CharName )
	{
		siCharID = CharID;
		siPersonID = PersonID;

		StringCchCopy( strCharName, MAX_PLAYER_NAME, CharName );
	}

	void Set( stWhisperChatMember *pWhisperChatMember )
	{
		memcpy( this, pWhisperChatMember, sizeof( stWhisperChatMember ) );
	}
};

class CWhisperChatObj : public CObj
{
public:
	CWhisperChatObj( SI32 Index );
	~CWhisperChatObj();

	void Init();

	SI32 GetWhisperChatIndex();

	SI32 Create( SI32 CharID1, SI32 PersonID1, TCHAR *CharName1, SI32 CharID2, SI32 PersonID2, TCHAR *CharName2 );
	bool IsCreated( SI32 PersonID1, SI32 PersonID2 );
	
	bool AddMember( SI32 CharID, SI32 PersonID, TCHAR *CharName );
	void DelMember( SI32 PersonID );

	SI16 GetEmptyGroupListIndex();
	void GetWhisperChatMemberList( stWhisperChatMember *pWhisperChatMemberList );


	void SendWhisperChat( SI32 FromCharID, SI32 FromPersonID );	

	void SendMsgToWhisperChatMembers( sPacketHeader *pclMsg );

	
	


private:
	SI32 m_siIndex;
	SI16 m_siCounter;

	SI16 m_siMemberNum;

#ifdef _SAFE_MEMORY
	NSafeTArray<stWhisperChatMember, MAX_WHISPERCHAT_OBJ_USERNUM>		m_stWhisperChatMemberList;
#else
	stWhisperChatMember m_stWhisperChatMemberList[ MAX_WHISPERCHAT_OBJ_USERNUM ];
#endif
	
};

#endif