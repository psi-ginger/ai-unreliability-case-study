#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"

#include "../../Server/GachaManager/GachaManager.h"

//--------------------------------------------------
// 0	= 이달의 상품
// 1~8	= 이월 상품

//const SI32 Max_CarryOver_Gacha_Product		= 9;

enum CARRYOVER_SELECT_MODE
{
	CARRYOVER_SELECT_MODE_GACHA = 0,
	CARRYOVER_SELECT_MODE_PRODUCT,

	CARRYOVER_SELECT_MODE_MAX
};

class CNCarryOverGachaDlg : public CDialogController
{

// -- Member Variable Area
private:
	InterfaceFactory	m_InterfaceFactory;

	SI32				m_siSelectGachaProductIndex;	// 초기값이 -1이여야함
	SI32				m_siSelectGachaProductUnique;	// 가챠 아이템 유니크

	SI32				m_siKeyControllerID;			// 키 컨트롤러 ID
	SI32				m_siKeyKind;					// 가챠 키 카인드

	SI32				m_siGachaGage;					// 대박확률

	TCHAR*				m_pText;						// 설명 텍스트

	NTCHARString512		m_strGachaExplain;				// 가챠아이템 설명 텍스트

	// 이달,이월 가챠상품 Unique
	SI32				m_siCarryOverItems[TOTAL_GACHA_PRODUCT];

	// 선택한 가챠의 품목
	SI32				m_siGachaProducts[MAX_GACHA_PRODUCT];

	TSpr				m_sprNumber;					// 가챠키 번호 이미지
	TSpr				m_sprGage;						// 대박확률 게이지

	SI32				m_siSelectMode;
protected:

public:


// -- Member Function Area
private:

protected:

public:
	CNCarryOverGachaDlg( void );
	~CNCarryOverGachaDlg( void );

	void Init();
	void Create();

	static void CALLBACK StaticCallBackDialogNCarryOverGachaDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNCarryOverGachaDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );
	void Text( SI32 ScreenRX, SI32 ScreenRY );

public:
	void OnClickGachaProduct( const SI32 siControllerID );
	void OnClickKey( const SI32 siControllerID );
	void OnClickKeyBuy( void );
	void OnClickGameStart( void );

public:
	void OnEventBack( void );

public:

	void ToggleKeyButtonEnable( const bool bGachaKeyEnable, const bool bAllRoundKeyEnable );
	void SetExplain2( TCHAR* pText );

	void HideItemControllers( void );

	void SetEnableGameStartButton( bool bEnable );

public:
	void SetTotalGachaListAndGachaCount( SI32* psiTotalGachaList, SI32 siGachaCount );
	void SetGachaProducts( SI32* psiGachaProducts );
	void ShowItemList( const SI32* const psiItemList, const SI32 siItemCount );

	bool IsHaveKey( void );

public:
	void SetGachaGage( SI32 siGachaGage )		{	m_siGachaGage = siGachaGage;			}

	SI32 GetKeyKind( void )						{	return m_siKeyKind;						}
	SI32 GetSelectGachaProductIndex( void )		{	return m_siSelectGachaProductIndex;		}
	SI32 GetSelectGachaProductUnique( void )	{	return m_siSelectGachaProductUnique;	}

};