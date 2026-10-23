#ifndef _NNEWMAGIC_INFO_H_
#define _NNEWMAGIC_INFO_H_

#include "../../InterfaceMgr/DialogController.h"

#include "..\..\..\Common\Char\KindInfo\KindInfo.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

#define MAGIC_NUM			10
#define SKILLBOOK_MAGIC		0

class CNNewMagicInfoDlg : public CDialogController
{
public:

	CNNewMagicInfoDlg();
	~CNNewMagicInfoDlg();
	
	void Clear( void );
	void Create();

	static void CALLBACK StaticCallBackDialogNNewMagicInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNNewMagicInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	void Action();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );
	bool IsChanged();
	BOOL SetMagic();		// 무기별 마법 이름
	void SetExplain();		// 마법 설명
	SI16 GetMagicKind();
	SI16 GetMagicKind(SI32 index);
	BOOL SetNewMagic(SI32 index);
	
	void ShowMagicList(SI32 SelectButtonIdex);
	void AddMagicList( TCHAR *MagicName, SI32 index );

	void DrawMagicIcon( LPDIRECTDRAWSURFACE7 lpsurface,SI32 x , SI32 y );
	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
	void SetButtonTextColor(SI32 Index);

	void StartMagicCoolTime(SI32 CoolTime);

	void DrawMagic( SI32 ScreenRX, SI32 ScreenRY );
	void SetMagicInfo( SI32 index );
	void Refresh( void );
	void SetActiveMagicKind( SI32 _siActiveMagicKind );
	
private:

	SI32			m_siCurrentButtonIndex;
	SI32			m_siSelectListIndex;
	SI32			m_siActiveMagicKind;			// 사용하는 마법의 종류.
	SI32			m_siMagicKind;
	SI32			m_siParentMagicKind;
	SI32			m_siNewMagicKind[MAGIC_NUM];

	DWORD			m_dwLastChangedMagic; 

	CList*			m_pList[2];				// 0 : 마법종류 1 : 마법설명
	CButton*		m_pMagicNameButton[6];	// 마법 이름 버튼
	CButton*		m_pButton[2];			// 0 : 적용 , 1 : 닫기
	CEdit*			m_pEdit;
	CEdit*			m_pEdit2;
	CImageStatic*	m_pMagicImageStatic[MAGIC_NUM];		
	CImageStatic*	m_pSelcetMagicImageStatic;
	CImageStatic*	m_pUpgradeMagicImageStatic;
	CStatic*		m_pKey;
	CStatic*		m_pKeyExplain;
	CStatic*		m_pMagicName;
	CStatic*		m_pSubMagicName;
	
	CButton*		m_pMagicCancelButton;			


	TSpr			m_SprMagicIcon;
	TSpr			m_SprMagicIconOut;
	TSpr			m_SprMagicCoolTime;
	TSpr			m_SprSkillBook;

	// 마법 쿨타임 표시용 변수
	clock_t			m_CoolTimeStart;
	SI32			m_CoolTime;
};

#endif _NMAGIC_INFO_H_