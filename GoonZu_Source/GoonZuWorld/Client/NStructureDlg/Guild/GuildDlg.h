#ifndef _NGUILDDLG_H
#define _NGUILDDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../../CommonLogic/Guild/Guild.h"
#include "../../../CommonLogic/Guild/GuildWar.h"


class NStrInventory;

class NGuildDlg : public NStrBaseDlg
{
public:
	NGuildDlg();
	virtual ~NGuildDlg();

	void Create();

	void GreetingMsg();

	void RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void RefreshAllList();

	SI32 TrimChar( TCHAR *InTextData,TCHAR ch, SI32 siStrLen );
};

// 광산 목록
class NGuildListDlg : public NStrTabDlg
{
public:
	enum {
		LIST_GUILDLIST,
		BUTTON_GUILDENTER,
		BUTTON_GUILDMAKE,
		BUTTON_GUILDJOIN,
	};

	NGuildListDlg();
	virtual ~NGuildListDlg();

	void InitDlg();

	void Action();

	void ShowGuildUnitList();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	SI32 siSelectedListIndex;
	SI32 siSelectedGuildUnique;

	SI16 siMemberNum[MAX_GUILD_PER_VILLAGE];
};



// 사용자 메뉴
class NGuildUserDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_GUILDNAME,

		BUTTON_APPLY,
		BUTTON_CANCEL_APPLY,
		BUTTON_CHANGE_VILLAGE,
		EDITBOX_EXPLAIN,
	};
	NGuildUserDlg();
	virtual ~NGuildUserDlg();

	void InitDlg();

	void Action();

	void ShowRentContractInUser();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	virtual void Show();
};


 /*길드 행수 메뉴 */
class NGuildChiefDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_RENT_FEE,
		EDIT_FEE_RATE,
		BUTTON_SETUP,
		LIST_APPLYLIST,
		BUTTON_ACCEPT,
		STATIC_RATE
	};

	NGuildChiefDlg();
	virtual ~NGuildChiefDlg();

	void InitDlg();

	void Action();

	void ShowRentContract();
	void ShowGuildUnitListInChief();
	void ShowGuildApplyerListInChief();
	void ShowSelectedGuildUnitExplain( int index );

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};

class NGuildManageDlg : public NStrTabDlg
{
public:
	enum{
		LIST_APPLICATION,
		BUTTON_ACCEPT,
		BUTTON_REJECT
	};

	NGuildManageDlg();
	virtual ~NGuildManageDlg();

	void InitDlg();
	void Action();
	void NDlgTabProc(UINT nEvent, int nControlID, CControllerObj* pControl );

	void RefreshList();

	SI32 m_siSelectListIndex;
	SI32 m_siSelectGuildUnique;
};

class NGuildWarListDlg : public NStrTabDlg
{
public:
	enum{
		COMBO_ROOM,
		STATIC_NOWBATTLE,
		EDIT_NOWBATTLE,
		LIST_WARLIST,
		BUTTON_RECOMMAND,
		EDIT_ENEMYGUILD,
		BUTTON_APPLY
	};

	NGuildWarListDlg();
	virtual ~NGuildWarListDlg();

	void InitDlg();
	void Action();
	void NDlgTabProc(UINT nEvent, int nControlID, CControllerObj* pControl );

	SI32	m_siFieldIndex;
	cltGuildWarReserveUnit m_clUnit[GUILDWAR_ROOMNUM];

	void RefreshList();

	void SetGuildName( TCHAR* guildName );
};



#endif