/* ==========================================================================
	클래스 :		CNMudangDlg

	작성일 :		05/04/08
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#define _LEON_NNMUDANGDLG_

//#if defined(_DEBUG) && defined( _LEON_NNMUDANGDLG_)

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CStatic;
class CButton;
class CEdit;
class CList;
class CComboBox;
class CRadioButton;
class COutLine;
class CNMudangDlg;

#define NMUDANGDLG_SETTIMER_TIME 300
#define MAX_NMUDANG_PAGE_NUMBER	3

class CNMudangReviveDlg : public CDialogController
{
public:
	CNMudangReviveDlg();
	~CNMudangReviveDlg();

	static	void CALLBACK NMuDangReviveStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NMuDangReviveProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create( CControllerMgr *pParent );
	void Show();
	void Hide();
	bool IsShow();

	bool m_bShow;
	CNMudangDlg*		m_pParent;


	CStatic		*m_pStatic_now_summon	;
	CButton		*m_pButton_summon_revive	;
	CEdit		*m_pEdit_summon_revive_explain	;
	CComboBox	*m_pComboBox_now_summon	;
};

class CNMudangScrollDlg : public CDialogController
{
public:
	CNMudangScrollDlg();
	~CNMudangScrollDlg();

	static	void CALLBACK NMudangScrollStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NMudangScrollProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create( CControllerMgr *pParent );
	void Show();
	void Hide();
	bool IsShow();

	bool m_bShow;
	CNMudangDlg*		m_pParent;

	CStatic			*m_pStatic_now_summon	;
	CStatic			*m_pStatic_arrow	;
	CStatic			*m_pStatic_scroll	;
	CStatic			*m_pStatic_scroll_percent_1	;
	CStatic			*m_pStatic_scroll_percent_2	;
	CRadioButton	*m_pRadioButton_have_scroll_1	;
	CRadioButton	*m_pRadioButton_have_scroll_2	;
	CButton			*m_pButton_summon_scroll	;
	CEdit			*m_pEdit_summon_scroll_explain	;
	CComboBox		*m_pComboBox_now_summon	;
	COutLine		*m_pOutLine_scroll	;

};

class CNMudangEvolveDlg : public CDialogController
{
public:
	CNMudangEvolveDlg();
	~CNMudangEvolveDlg();

	static	void CALLBACK NMudangEvolveStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NMudangEvolveProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create( CControllerMgr *pParent );
	void Show();
	void Hide();
	bool IsShow();

	CNMudangDlg*		m_pParent;
	bool m_bShow;

	CStatic		*m_pStatic_now_summon	;
	CStatic		*m_pStatic_arrow	;
	CStatic		*m_pStatic_new_summon	;
	CButton		*m_pButton_summon_evolve	;
	CEdit		*m_pEdit_summon_evolve_explain	;
	CList		*m_pList_new_summon	;
	CComboBox	*m_pComboBox_now_summon	;

	CRadioButton	*m_pRadioButton_evolution	;
	CRadioButton	*m_pRadioButton_devolution	;
	CRadioButton	*m_pRadioButton_transform	;

};

class CNMudangEquipItemDlg : public CDialogController
{
protected:

	//cltSummon		*pclSummon;
	CNMudangDlg*	m_pParent;
	bool			m_bShow;
	cltSummonInfo *pclSummonInfo;

	SI16 invennum ;
	SI32 matnum ;
	SI16 siArmorStonUnique ;
	SI16 siArmorStonNUM ;
	SI16 siWeaponestonUnique ;
	SI16 siWeaponestonNUM ;

	//컨트롤은 총 17개가 필요함.
	CStatic			*m_pStatic_now_summon		;	// TEXT : 보유 소환수 
	CImageStatic	*m_pStaticimage_summonface	;	// 서먼몬스터이미지
	COutLine		*m_pOutline_summonface		;	// 서먼몬스터이미지아웃라인
	CStatic			*m_pStatic_summonlevel		;	// TEXT : 소환수레벨
	CStatic			*m_pStatic_weaponston		;	// TEXT : 무기착용석
	CStatic			*m_pStatic_armorston		;	// TEXT : 방어구착용석 
	CStatic			*m_pStatic_summonlevelNow		;	// TEXT : 현제 소환수레벨
	CStatic			*m_pStatic_weaponstonNow		;	// TEXT : 현제 무기착용석
	CStatic			*m_pStatic_armorstonNow		;	// TEXT : 현제방어구착용석 
	COutLine		*m_pOutline_summonstat		;	// 서먼몬스터이미지아웃라인

	CStatic			*m_pStatic_weapontitle		;	// TITLE : 무기 착용석
	CStatic			*m_pStatic_armortitle		;	// TITLE : 방어구 착용석
	CListView		*m_pListview_weaponston	;	// 무기 착용석 이미지
	CListView		*m_pListview_armorston	;	// 방어구 착용석 이미지
	COutLine		*m_pOutline_weaponston		;	// 무기 착용석 이미지아웃라인	
	COutLine		*m_pOutline_armorston		;	// 방어구 착용석 이미지아웃라인
	//CStatic			*m_pStatic_maxweaponston	;	// TEXT : 방어구착용석 
	//CStatic			*m_pStatic_maxarmorston		;	// TEXT : 방어구착용석 
	COutLine		*m_pOutline_ston			;	// 착용석 이미지아웃라인

	CStatic			*m_pStatic_useston			;	// TEXT : 사용할 착용석 
	CEdit			*m_pEdit_useston			;	// 사용할 아이템 착용석 갯수
	CButton			*m_pButton_weaponuseston			;	// 무기석사용 버튼
	CButton			*m_pButton_armoruseston			;	// 방어구석 사용 버튼

public:
	CNMudangEquipItemDlg();
	~CNMudangEquipItemDlg();

	static	void CALLBACK NMudangEquipItemStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NMudangEquipItemProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create( CControllerMgr *pParent );
	void Show();
	void Hide();
	bool IsShow();
	void Showsummonstatus( );	// 현제 가능한 소환수 정보를 업데이트 한다.
	bool GetSummonmonsterNow( cltSummon *pclSummon );	// 현제 가능한 소환수 정보를 업데이트 한다.
	bool CheckUseston( SI32 ItemType,  SI32 summonlevel,SI32 siStonNum, SI16 siSummonIndex ) ;
	bool IsShowSummon()	;
	void InitTextnImage()	;
	void showitemImage();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};


class CNMudangDlg : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 100,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 100,
	};

	struct stTabTextInfo
	{
		CControllerObj	*st_pControllerObj;
		COLORREF	st_Color;
		SI16		st_siWeight;

		stTabTextInfo()
		{
			Init();
		}

		void Init()
		{
			memset( this, 0, sizeof( stTabTextInfo ));
		}

		void Set( CControllerObj* in_pControllerObj, COLORREF in_Color, SI16 in_siWeight )
		{
			st_pControllerObj = in_pControllerObj;
			st_Color = in_Color;
			st_siWeight = in_siWeight;
		}

	};

	CNMudangDlg();
	~CNMudangDlg();

	void Create();
	void Show();
	void Hide();
	bool IsShow();
	void Action();

	void ShowTabBy( int i_Index);
	void SetTabTextInfo( stTabTextInfo* in_SelectedText );

	void Set();
	
	static	void CALLBACK NMudangStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );

	void CALLBACK NMudangProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	BOOL GetComboSummonNameIndex( TCHAR *CurrSummonName, SI16 txtSize, SI32 *siSummonIndex);
	
	void UpdateCurrentSummonInfo( BOOL bALive);
	void UpdateScrollSummonInfo();

	// Evolve, Devolution, Transform
	void UpdateEvolveSummonInfo();
	void UpdateDevolutionSummonInfo();
	void UpdateTransformSummonInfo();
	// 소환수 장착석 해당 몬스터 업데이트
	void UpdateEquipItwmInfo();

	// Evolve, Devolution, Transform
	BOOL SendSummonEvolveMessageToServer(SI32 id);
	BOOL SendSummonDevolutionMessageToServer(SI32 id);
	BOOL SendSummonTransformMessageToServer(SI32 id);

	BOOL SendSummonScrollMessageToServer( SI32 id);
	BOOL SendSummonReviveMessageToServer( SI32 id);
	
	BOOL DisplayMessage(BOOL bSuccess, TCHAR *pStr);

	void UpdateReviveMent();
	void UpdateScrollMent();
	void UpdateEvolveMent();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void SetTextBySummonEquip();

	void SetMudangPresentTxt(TCHAR * Txt);

public:

	RECT				m_DlgRect;
	bool				m_bRect;

	clock_t				m_LastSendSummonTime;
	BOOL				m_bChangedScrollPercent;
	SI32				m_siActiveScrollIndex;
	SI32				m_CurrentStatusPage;

	// Evolve, Devolution, Transform
	SI32			m_siSelectEvolveDlg;

	bool m_bShow;

	CNMudangReviveDlg	*m_pTabReviveDlg;
	CNMudangScrollDlg	*m_pTabScrollDlg;
	CNMudangEvolveDlg	*m_pTabEvolveDlg;
	CNMudangEquipItemDlg	*m_pTabEquipitemDlg;

	CImageStatic		*m_pImageStatic_portrait_mudang	;
	CButton				*m_pButton_revive	;
	CButton				*m_pButton_scroll	;
	CButton				*m_pButton_evolve	;
	CButton				*m_pButton_equipitem	;// 아이템 장착	
	CButton				*m_pButton_close	;
	CEdit				*m_pEdit_mudang	;
	CEdit				*m_pEdit_message	;
	COutLine			*m_pOutLine_portrait_mudang	;

};

//#endif	
