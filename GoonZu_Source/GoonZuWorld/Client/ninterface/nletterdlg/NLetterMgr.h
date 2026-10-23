
#ifdef __SUN_NLETTER__


#include "NLetterDlgBaseInfo.h"
#include "..\..\..\Common\SMLib\LibList\LibListMgr.h"

class CLetterMgr : public CLibListMgr
{
public:
	CLetterMgr();
	~CLetterMgr();

	void Initialize();
	void Destroy();

	void CreateEmptyObj();

	void Clear();

	bool IsDuplicate( SI32 siLetterIndex );
	//bool IsExist( SI32 siLetterIndex );

	//void PushDeleteIndex( SI32 siLetterIndex );
	
	bool Push( char *strLetterMsg );	// 쪽지를 더 넣을 수 없는 경우, false 리턴함.
		

private:
	SI32 m_siDeleteSequenceIndex[ MAX_LETTERMSG_NUM ];
	SI32 m_siDeleteCount;
};


#endif
