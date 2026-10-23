//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _IDMANAGER_H
#define _IDMANAGER_H

#include <directives.h>
#include "..\..\CommonHeader.h"

class cltIDManagerNode{
public:
	SI32 ID;
	cltIDManagerNode* pNextNode;

	cltIDManagerNode() {ID = 0; pNextNode = NULL;}
};


class cltIDInfo{
public:
	SI32 siKind;		// 캐릭터의 종류. 
	bool bPC;			// PC여부. 
	bool bInstance;		// 인스턴스 NPC;

	cltIDInfo();
	void Init();
	void Set(SI32 kind, bool bnpc, bool binstance);
};

class cltIDManager{
private:
	SI32 MaxIDNumber;
	SI32 MaxSessionNumber;
	SI32 MaxInstanceNPC;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltIDInfo, MAX_CHAR_NUMBER>		clIDInfo;
#else
	cltIDInfo clIDInfo[MAX_CHAR_NUMBER];				// Kind정보가 저장될 공간. 
#endif

	// 유닛 생성 순서 
	SI32* pCharOrder;
	SI32  CharOrderIndex;

	SI32 UserCharNumber;
	SI32 NPCCharNumber;
	SI32 InstanceNPCNumber;
	SI32 ActiveNPCNumber;

	public:
		

		cltIDManager(SI32 maxidnumber, SI32 sessionnumber, SI32 instancenpc);
		~cltIDManager();

		SI32 GetMaxIDNumber() const;

		SI32 GetID(SI32 mode);
		void ReleaseID(SI32 id);

		// id가 할당되어 있는지 확인한다. 
		bool IsValidID(SI32 id);

		SI32 GetCharOrderIndex() const;

		SI32 GetCharOrder(SI32 index) const;

		BOOL DeleteChar(SI32 index);

		// 캐릭터를 추가한다. 
		SI32 AddChar(SI32 id, SI32 kind, bool bpcswitch, bool binstanceswitch);

		SI32 GetUserCharNumber()const;
		void AddUserCharNumber();
		void SubUserCharNumber();

		SI32 GetNPCCharNumber()const;
		void AddNPCCharNumber();
		void SubNPCCharNumber();

		SI32 GetActiveNPCNumber()const;
		void AddActiveNPCNumber();
		void SubActiveNPCNumber();

		SI32 GetInstanceNPCNumber()const;
		void AddInstanceNPCNumber();
		void SubInstanceNPCNumber();

		SI32 GetTotalCharNumber()const;

};


#endif
