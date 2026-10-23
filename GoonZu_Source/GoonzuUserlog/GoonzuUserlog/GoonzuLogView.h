#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"

#ifdef _DEBUG
#define ASSERT_VALID_STRING( str ) ASSERT( !IsBadStringPtr( str, 0xfffff ) )
#else	//	_DEBUG
#define ASSERT_VALID_STRING( str ) ( (void)0 )
#endif	//	_DEBUG


// CGoonzuLogView 대화 상자입니다.
class cltItemManagerCommon;
class CItemExplain;

class CGoonzuLogView : public CDialog
{
	DECLARE_DYNAMIC(CGoonzuLogView)

public:
	CGoonzuLogView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGoonzuLogView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	

	DECLARE_MESSAGE_MAP()
public:

	void OnCancel();

	char m_username[128];
	char m_password[64];
	
	int m_siMaxRowcount;
	int m_siCurRow;
	int m_subitem;
	int m_siServerSelect;

	DWORD m_dwProcessId;
	HWND m_ChildHwnd;

	HANDLE m_ChildProcess;
	char m_strAppFullPath[512];

	bool m_bExit;

	void SetServerSelect(int select) {m_siServerSelect = select;};
	void SetCombo();
	void SetListView();
	void SetFilterBox();
	void ShowUserLog();
	void SetAccount(char* UserID,char * PassWord );
	void ShowRowcount();
	void ShowItemInfo();
	bool GetStringFromLogfile(char *source, char *out, int *index, int size);
	void OpenChildProcess();
	void InsertLogUnit(CLogUnit *pLogUnit);
	void SendSelectedLogUnit();
	void InsertFilter(int nIndex, char *strFilter);

	bool IsNumber(LPCTSTR pszText);
	int NumberCompare( LPCTSTR pszNumber1, LPCTSTR pszNumber2 );
	bool IsDate( LPCTSTR pszText );
	int DateCompare( const CString& strDate1, const CString& strDate2 );

	void EnableButton();
	void DisableButton();

	// 쿼리 실행
	HANDLE m_hQueryThread;
	static unsigned __stdcall QueryLogData(LPVOID lpParam);

	// 파일관련
	CString m_szFileName;
	HANDLE m_hLoadThread;
	void SaveLogFile(CString szFileName, bool bInfo = true);
	bool LoadLogFile(CString szFileName);
	static unsigned __stdcall LoadLogFile(LPVOID lpParam);

	// 정렬
	int m_iSortColumn;
	BOOL m_bSortAscending;
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

	// 검색
	int ListFindItem(CString szSearch, int siStart);

	// 윈도우찾기
	static int CALLBACK FindChildWindow(HWND hwnd, LPARAM lParam);

	// 툴바
	HACCEL m_hAccel;

	CComboBox m_SearchServer;
	CEdit m_UserEdit;
	CEdit m_ItemEdit;
	CButton m_SearchButton3;
	CButton m_ButtonSaveFile;
	CListCtrl m_ListView;
	CComboBox m_SearchItem;
	CListBox m_ListBox;
	afx_msg void OnBnClickedButton3();
	CEdit m_ViewEdit;

	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton4();
	CEdit m_FileNameEdit;
	CEdit m_SearchItemEdit;
	afx_msg void OnBnClickedButton1();
	CEdit m_BinaryEdit;
	CEdit m_UniqueEdit;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_RowcountEdit;
	CEdit m_SearchEdit;
	afx_msg void OnBnClickedButtonSearch();

	cltItemManagerCommon *m_pItemManager;
	CItemExplain *m_pItemExplain;
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnBnClickedButtonUnselectedDelete();
	CEdit m_UserSearchEdit;
	CDateTimeCtrl m_DTDate;
	CDateTimeCtrl m_DTStartTime;
	CDateTimeCtrl m_DTEndTime;
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSendchildwindow();
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedButtonSelectedDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnKeydownList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnSetfocusEdit1();
	CButton m_ButtonGetUnique;
protected:
	virtual void OnOK();
public:
	afx_msg void OnEnSetfocusEdit4();
	afx_msg void OnEnSetfocusEdit5();
	afx_msg void OnEnSetfocusEditSearch();
	afx_msg void OnEnSetfocusEdit2();
	afx_msg void OnEnSetfocusEdit3();
	afx_msg void OnEnSetfocusEditUserSearch();
	afx_msg void OnLbnSetfocusList1();
	afx_msg void OnExitApp();
	afx_msg void OnOpenFile();
	afx_msg void OnSaveFile();
	afx_msg void OnSendData();
	afx_msg void OnDeleteSelected();
	afx_msg void OnDeleteUnselected();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonDatevary();
	afx_msg void OnEnSetfocusEditDatevary();
};
