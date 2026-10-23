
#pragma once

//#include "Resource.h"

#include "../../InterfaceMgr/DialogController.h"
#include "../../../common/CommonHeader.h"
#include "../../../common/Item/ItemCommon/cltItem.h"

#include <list>
 
//class CButton;


struct stPresentData
{
	SI32 siFromServerUnique;
	TCHAR szFromPlayerName[MAX_PLAYER_NAME];

	TCHAR szComment[200];
	TCHAR szItemName[50];
	
	SI32 siItemNum;	

	SI32 siItemImageFont;	

	TCHAR szFromPlayerAccount[MAX_PLAYER_NAME];
	
	stPresentData( SI32 FromServerUnique, TCHAR * FromPlayerName, TCHAR * Comment, TCHAR * ItemName
		, SI32 ItemNum, SI32 ItemImageFont, TCHAR * FromPlayerAccount )
	{
		siFromServerUnique = FromServerUnique;

		StringCchCopy(szFromPlayerName,MAX_PLAYER_NAME,FromPlayerName);
		StringCchCopy(szComment,200,Comment);
		StringCchCopy(szItemName,50,ItemName);
        
		siItemNum =ItemNum;
		siItemImageFont = ItemImageFont;

		StringCchCopy(szFromPlayerAccount, MAX_PLAYER_NAME, FromPlayerAccount);
	}

	stPresentData()
	{
		Init();
	}

	void Init()
	{
		memset( this, 0, sizeof( stPresentData ) );
	}
};



class CNGetPresentDlg  : public CDialogController
{
public:
	CNGetPresentDlg();
	~CNGetPresentDlg();

	static void CALLBACK	StatiCNGetPresentDlg(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NGetPresentDlgdProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	void Create();	

	void PreButtonClicked();
	void NextButtonClicked();
	void SetCurPresentData();

	void SetPresentData( SI32 FromServerUnique, TCHAR * FromPlayerName, TCHAR * Comment, TCHAR * ItemName
		, SI32 	ItemNum, SI32 ItemImageFont, TCHAR * FromPlayerAccount);

	private:

	InterfaceFactory m_InterfaceFactory;
	
	SI32	m_siItemId;
	SI32	m_siItemNum;
	cltItem	m_clItemData;
	
	SI32	m_siLetterNum;
	SI32	m_siCurLetter;
	
	SI16	m_siCurLetterServerUnique;
	TCHAR	m_szCurLetterPlayerAccount[MAX_PLAYER_NAME];
	TCHAR	m_szCurLetterPlayerName[MAX_PLAYER_NAME];


	
	list< stPresentData > m_PresentList;
	list< stPresentData >::iterator m_PresentListItr;

};