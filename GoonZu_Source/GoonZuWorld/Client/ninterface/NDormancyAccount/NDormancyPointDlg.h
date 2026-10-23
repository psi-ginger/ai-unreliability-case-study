#pragma once
// NDormancyPointDlg.h: interface for the NDormancyPointDlg class.
//
//////////////////////////////////////////////////////////////////////
/* ==========================================================================
클래스 :		CNDormancyPointDlg

작성일 :		09/08/13
작성자 :		박기형

목  적 :		휴면계정포인트 구입 및 포인트 정보
용  법 :		
변경사항 :		없음

차후작업 :		

========================================================================*/
#include "Resource.h"

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CImageStatic;
class CListBox;
class CStatic;
class CEdit;

class CNDormancyPointDlg  : public CDialogController
{
public:
	CNDormancyPointDlg();
	~CNDormancyPointDlg();

	static VOID CALLBACK	StatiCNDormancyPointProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	VOID CALLBACK			NDormancyPointDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	VOID Create();

	VOID Destroy();
	
	// 화면 버튼 아이콘 
	VOID Minimi(BOOL bMini);
	SI32 GetMinimizedSlot(){return m_siMinimizedSlot;};
	VOID SetMinimizedSlot(SI16 MinimizedSlot){  m_siMinimizedSlot = MinimizedSlot; };

	// 포인트 아이템 목록 가져오기
	VOID ShowPointItemList();

	// 선택 아이템 구입
	VOID BuySelectItem();

	//	아이템 선택
	VOID SelectItem();

	//  휴면 정보 갱신
	VOID RefreshDormancyInfo();

	VOID VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );

	//	메세지 박스 처리
	VOID RetMsgBox( bool bRetValue, SI16 siMsgIndex, BYTE *pData, SI16 siDataLen );
private:
	InterfaceFactory m_InterfaceFactory;
	
	SI32		m_siSelctedUnique;		// 선택된 아이템 유니크

	SI32		m_siImageType;			// 아이템 이미지 관련
	SI32		m_siImageIndex;			// 아이템 이미지 관련

	//	아이템 이미지
	CImageStatic	*m_pItemImage;
	//	아이템 리스트
	CList		*m_pItemListBox;
	//	구입 버튼
	CButton		*m_pBuyButton;
	//	종료 버튼
	CButton		*m_pCloseButton;
	//	설명
	CStatic		*m_pTextStatic;
	//  휴면 포인트 정보
	CEdit		*m_pPointInfoEdit;
	//	미니미 버튼
	CButton		*m_pMinimiButton;

	BOOL		m_bMini	;
	SI32		m_siOldPosX;
	SI32		m_siOldPosY;
	SI32		m_siOldXSize;
	SI32		m_siOldYSize;

	SI32		m_siMinimizedSlot;
	TCHAR		m_szTempBgImgFilePath[MAX_PATH];	// 임시 배경 이미지 파일 경로
	TCHAR		m_szTempTitle[MAX_PATH];	// title 임시 저장
};
