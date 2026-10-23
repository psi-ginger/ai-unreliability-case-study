#pragma once

#include "../CommonLogic.h"
#include "../../NLib/NTech/sdk/nkcore/Include/NkVector.h"
#include "../../NLib/NTech/sdk/nkcore/Include/NkRbTreeMap.h"

const SI32 Const_Max_BoxItemMgr_Probability	= 10000;	// 박스아이템 사용시 1/10000 확률까지 설정할수있게한다

/************************************************************************************************************************
* 클래스 이름 : CBoxItemUnit
* 목적 : 상자 아이템에 들어갈 개별 아이템 데이터
************************************************************************************************************************/
class CBoxItemUnit
{
private:
	UI16	m_uiItemUnique;		// 아이템 유니크
	UI16	m_uiMinNum;			// 최소갯수
	UI16	m_uiMaxNum;			// 최대갯수
	UI16	m_uiItemUseDate;	// 사용기간
	UI16	m_uiProbability;	// 확률
	UI16	m_uiCanNotice;		// 대박알림여부

public:
	CBoxItemUnit( void );
	CBoxItemUnit( UI16 uiItemUnique, UI16 uiMinNum, UI16 uiMaxNum, UI16 uiItemUseDate, UI16 uiProbability, UI16 uiCanNotice );
	CBoxItemUnit( const CBoxItemUnit* pclBoxItemUnit );

	~CBoxItemUnit( void );

//interface
public:
	void	Init( void );

	void	SetBoxItemUnit( UI16 uiItemUnique, UI16 uiMinNum, UI16 uiMaxNum, UI16 uiItemUseDate, UI16 uiProbability, UI16 uiCanNotice );
	void	SetBoxItemUnit( const CBoxItemUnit* pclBoxItemUnit );


	void	SetItemUnique( const UI16 uiItemUnique )	{	m_uiItemUnique = uiItemUnique;		}
	UI16	GetItemUnique( void ) const					{	return m_uiItemUnique;				}

	void	SetMinNum( const UI16 uiMinNum )			{	m_uiMinNum = uiMinNum;				}
	UI16	GetMinNum( void ) const						{	return m_uiMinNum;					}

	void	SetMaxNum( const UI16 uiMaxNum )			{	m_uiMaxNum = uiMaxNum;				}
	UI16	GetMaxNum( void ) const						{	return m_uiMaxNum;					}

	void	SetItemUseDate( const UI16 uiItemUseDate )	{	m_uiItemUseDate = uiItemUseDate;	}
	UI16	GetItemUseDate( void ) const				{	return m_uiItemUseDate;				}

	void	SetProbability( const UI16 uiProbability )	{	m_uiProbability = uiProbability;	}
	UI16	GetProbability( void ) const				{	return m_uiProbability;				}

	void	SetCanNotice( const UI16 uiCanNotice )		{	m_uiCanNotice = uiCanNotice;		}
	UI16	GetCanNotice( void ) const					{	return m_uiCanNotice;				}

// Utility
public:
	// MinNum, MaxNum을 이용하여 사이값을 구한다
	UI16	GetItemNum( void ) const;

};
typedef NkVector<CBoxItemUnit*>				NkVec_BoxItemUnit;


/************************************************************************************************************************
* 클래스 이름 : CBoxItemSet
* 목적 : 하나의 상자 아이템을 위한 클래스
************************************************************************************************************************/

enum BOXITEM_TYPE
{
	BOXITEM_TYPE_NONE	= 0,

	BOXITEM_TYPE_NORMAL,	// 일반적인 박스아이템(인벤토리에서 실행가능)
	BOXITEM_TYPE_AMBER,		// 호박 상자류
	BOXITEM_TYPE_SPECIAL,	// 특수한 박스아이템(소스작업을 통해 사용가능하도록 함)

	// 새로운 타입은 이 위로
	BOXITEM_TYPE_MAX
};

class CBoxItemSet
{
private:
	UI16				m_uiBoxItemUnique;
	UI16				m_uiBoxItemType;

	// 이녀석은 서버에서만 유효하다
	NkVec_BoxItemUnit*	m_pclVecBoxItemUnit;

public:
	CBoxItemSet( void );
	CBoxItemSet( UI16 uiBoxItemUnique, UI16 uiBoxItemType, NkVec_BoxItemUnit* pclVecBoxItemUnit=NULL );
	CBoxItemSet( const CBoxItemSet& rclBoxItemSet );

	~CBoxItemSet( void );

public:
	void			SetBoxItemUnique( const UI16 uiBoxItemUnique )	{	m_uiBoxItemUnique = uiBoxItemUnique;	}
	UI16			GetBoxItemUnique( void )						{	return m_uiBoxItemUnique;				}

	void			SetBoxItemType( const UI16 uiBoxItemType )		{	m_uiBoxItemType = uiBoxItemType;		}
	UI16			GetBoxITemType( void )							{	return m_uiBoxItemType;					}

	void			Init( void );
	void			ClearBoxItemUnit( void );

	CBoxItemUnit*	GetRandItem( void );
	
	BOOL			LoadBoxItemSet( TCHAR* const szFileName );

	UI32			GetInItemList( OUT SI32* psiInBoxItemList, IN const UI32 uiListCount );
	BOOL			IsItemCanNotice( const SI32 siInItemUnique );


};
typedef NkRbTreeMap<SI32, CBoxItemSet> NkMap_BoxItemSet;

/************************************************************************************************************************
* 클래스 이름 : CBoxItemMgr
* 목적 : 모든 상자 아이템을 관리함
************************************************************************************************************************/
class CBoxItemMgr
{
private:
	NkMap_BoxItemSet	m_clMapBoxItemSet;

public:
	CBoxItemMgr( void );
	~CBoxItemMgr( void );

public:
	BOOL			LoadBoxItemInfo( void );
	void			LoadAllBoxItemSet( void );

	CBoxItemUnit*	GetRandItemFromUnique( const SI32 siBoxItemUnique );

	BOOL			IsSameBoxItemType( const SI32 siBoxItemUnique, const SI32 siBoxItemType  );

	BOOL			IsNormalBoxItem( const SI32 siBoxItemUnique )	{	return IsSameBoxItemType( siBoxItemUnique, BOXITEM_TYPE_NORMAL );	};
	BOOL			IsAmberBoxItem( const SI32 siBoxItemUnique )	{	return IsSameBoxItemType( siBoxItemUnique, BOXITEM_TYPE_AMBER );	};

	UI32			GetInBoxItemList( IN const SI32 siBoxItemUnique, OUT SI32* psiInBoxItemList, IN const UI32 uiListCount );

	BOOL			IsInBoxItemCanNotice( const SI32 siBoxItemUnique, const SI32 siInItemUnique );

};
