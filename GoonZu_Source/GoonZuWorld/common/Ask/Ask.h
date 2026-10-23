//---------------------------------
// 2004/1/17 김태곤
//---------------------------------
#ifndef _ASK_H
#define _ASK_H

#include <Directives.h>
#include <CommonHeader.h>

#define MAX_ASK_TEXT_SIZE	80

#define MAX_ASK_NUMBER		50

//-----------------------------
// 질문 한개 정보. 
//-----------------------------

class cltAskUnit{
public:
	SI16 siCharUnique;		// 질문자의 CharUnique
	SI32 siPersonID;		// 질문자의 PersonID
	SI32 siSerialNum;		// 일련번호.	
	TCHAR szPersonName[MAX_PLAYER_NAME];
	TCHAR szAskText[MAX_ASK_TEXT_SIZE];
	


	cltAskUnit()
	{
		Init();
	}

	cltAskUnit(SI16 charunique, SI32 personid, SI32 serial, TCHAR* ppersonname, TCHAR* ptext)
	{
		siCharUnique	= charunique;
		siPersonID		= personid;
		siSerialNum		= serial;
		
		MStrCpy( szPersonName, ppersonname, MAX_PLAYER_NAME );
		MStrCpy( szAskText, ptext, MAX_ASK_TEXT_SIZE );
	}
	
	void Init()
	{
		siCharUnique	= 0;
		siPersonID		= 0;
		siSerialNum		= 0;
	}


};


//----------------------------
// 질문 관리자. 
//----------------------------
class cltAskManager{
public:
	HWND m_hDlg;					// 아이템 정보. 

	RECT DlgRect;
	bool bDlgRectInitSwitch;


	SI32		siIndex;			// 순서. 
#ifdef _SAFE_MEMORY
	NSafeTArray<cltAskUnit, MAX_ASK_NUMBER>		clUnit;
#else
	cltAskUnit clUnit[MAX_ASK_NUMBER];
#endif

	bool		bUpdateSwitch;

	cltAskManager();
	~cltAskManager();


	// 질문을 추가한다. 
	bool Add(SI16 charunique, SI32 personid, TCHAR* ppersonname, TCHAR* ptext);

	// personid를 가지고 질문을 삭제한다. 
	void DelAskPersonID(SI32 personid);

	// 정보를 갱신한다. 
	void Update();

	BOOL CreateDlg(HINSTANCE hinst, HWND hwnd);
	static BOOL CALLBACK StaticAskDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK AskDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

};

#endif
