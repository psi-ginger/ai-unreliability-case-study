#ifndef _NSELECT_CHAR_H_
#define _NSELECT_CHAR_H_

#include "../../InterfaceMgr/DialogController.h"
#include "../common/Char/CharCommon/Char-Common.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;
class CCharacterDrawing;


class CN3DCharDraw
{
private:
	TSpr*	m_pkImage;
	TSpr*	m_pkBack;
	TSpr*	m_pkAura;

	SI32	m_siFontIndex;
	SI32	m_siMaxFontIndex;
	SI32	m_siAruaFontIndex;
	SI32	m_siAlphaValue;
	SI32	m_siX, m_siY;

	bool	m_bDrawMask;
	bool	m_bDrawAura;

public:
	CN3DCharDraw()
	{
		m_pkImage		= NULL;
		m_pkBack		= NULL;
		m_pkAura		= NULL;

		m_siAlphaValue	= MAX_ALPHA_LEVEL - 1;
		
		m_siAruaFontIndex= 0;
		m_siFontIndex	= 0;
		m_siMaxFontIndex= 0;
		m_siX			= 0;
		m_siY			= 0;

		m_bDrawMask		= false;
		m_bDrawAura		= false;
	}

	void SetXY(SI32 siX, SI32 siY)
	{
		m_siX	= siX;
		m_siY	= siY;
	}

	void SetImage(TSpr* pkImage, SI32 siMaxFrame)
	{
		m_pkImage			= pkImage;
		m_siMaxFontIndex	= siMaxFrame;
	}

	void SetAlpha(SI32 siAlpha)
	{
		m_siAlphaValue	= siAlpha;
	}

	void SetBack(TSpr* pkBack)
	{
		m_pkBack	= pkBack;
	}

	void SetAura(TSpr* pkAura)
	{
		m_pkAura	= pkAura;
	}

	void DrawMask(bool bDraw)
	{
		m_bDrawMask = bDraw;
	}

	void DrawAura(bool bDraw)
	{
		m_bDrawAura = bDraw;
	}

	void MoveNextFrame()
	{
		if(m_pkImage && m_siMaxFontIndex)
		{
			m_siFontIndex++;
			m_siFontIndex = m_siFontIndex % m_siMaxFontIndex;
		}
		else
			m_siFontIndex = 0;

		if(m_pkAura && m_pkAura->GetImageNum() )
		{
			m_siAruaFontIndex++;
			m_siAruaFontIndex = m_siAruaFontIndex % m_pkAura->GetImageNum();
		}
	}

	void ResetFrame()
	{
		m_siFontIndex		= 0;
		m_siAruaFontIndex	= 0;
	}

	void Draw();
};

class CNSelectCharDlg : public CDialogController
{
public:

	CNSelectCharDlg();
	~CNSelectCharDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNSelectCharDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNSelectCharDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	void Action();

	//	BOOL SetSelectCharInfoText( SI32 slot );
	void SetSelectChar( SI32 slot );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );

	void SelectChar();

	BOOL IsShowCheck()					{	return m_bShowCheck;	}
	void SetShowCheck(BOOL bcheck)		{	m_bShowCheck = bcheck;	}

	bool IsCharSlotEmpty();

private:
	CButton*		m_pButtonCharSlot[MAX_CHAR_PER_USER];	// 캐릭터 슬롯 버튼 (일본)
	CN3DCharDraw	m_3DCharDraw[MAX_CHAR_PER_USER];		// 3D 캐릭터 그림
	CImageStatic*	m_pImageStaticSlot[MAX_CHAR_PER_USER];	// 캐릭터 슬롯 이미지 (일본 외)

	CButton*		m_pButtonSelect; // 캐릭 선택
	CButton*		m_pButtonMake;	 // 캐릭 만들기
	CButton*		m_pButtonDelete; // 캐릭 삭제
	CButton*		m_pButtonCancelDelete; // 캐릭 삭제 취소
	CButton*		m_pButtonReturn; // 돌아가기
	CImageStatic*	m_pImageStaticChar[MAX_CHAR_PER_USER];	// 캐릭터 삭제대기이미시스태틱
	CButton*		m_pButtonCancelDel[MAX_CHAR_PER_USER]; // 삭제취소
	CImageStatic*	m_pImageStaticdeldateBack[MAX_CHAR_PER_USER];	// 캐릭터 삭제대기시 날짜가 보일때 배경
	CEdit*			m_pEditboxDeldate[MAX_CHAR_PER_USER];// 기간을 써줄 에디트박스
	CEdit*			m_pEditboxwaitdel[MAX_CHAR_PER_USER]; //제목 삭재대기중
	
	cltAniInfo		clAniInfo;
	CCharacterDrawing* m_pCharDraw;

	SI32 siCharEffectFrame;
	SI32 siCharEffectMaxFrame;

	TSpr sprCharBack;
	TSpr sprCharAura;
	TSpr sprLoginChar[4];
	
	TSpr sprCharImg;
	TSpr sprEmptySlot;

	SI32 DelayClock;
	SI32 m_siHeartBeatCount;

	BOOL m_bShowCheck;
};

#endif

