#pragma once

#include "../../common/CommonHeader.h"
#include "../../common/Char/ParametaBox/parametabox.h"

//----------------------------------------------------------
// 캐릭터 생성시 보상하는 아이템 관련
//----------------------------------------------------------
const SI32 Const_Max_CreateCharReward_Item	= 20;
const SI32 Const_Max_RecommandReward_Item	= (MAX_CHARACTER_LEVEL) + 1;


class cltItem;

class cltRewardItem
{
private:
	SI32	m_siItemUnique;
	SI32	m_siItemNum;
	SI32	m_siPara1;			// 각종예외상황용1
	SI32	m_siPara2;			// 각종예외상황용2

public:
	cltRewardItem()							{	Init();								}

	cltRewardItem( cltRewardItem* pclRewardItem )
	{
		if ( pclRewardItem )
		{
			memcpy( this, pclRewardItem, sizeof(cltRewardItem) );
		}
	}

	cltRewardItem( SI32 siItemUnique, SI32 siItemNum, SI32 siPara1=0, SI32 siPara2=0 )
	{
		m_siItemUnique	= siItemUnique;
		m_siItemNum		= siItemNum;
		m_siPara1		= siPara1;
		m_siPara2		= siPara2;
	}

	~cltRewardItem()	{};

	void Init( void )
	{
		ZeroMemory( this, sizeof(cltRewardItem) );
	}

	void Set( cltRewardItem* pclRewardItem )
	{
		if ( pclRewardItem )
		{
			memcpy( this, pclRewardItem, sizeof(cltRewardItem) );
		}
	}

	void Set( SI32 siItemUnique, SI32 siItemNum, SI32 siPara1=0, SI32 siPara2=0 )
	{
		m_siItemUnique	= siItemUnique;
		m_siItemNum		= siItemNum;
		m_siPara1		= siPara1;
		m_siPara2		= siPara2;
	}

	void SetItemUnique( SI32 siItemUnique )		{	m_siItemUnique = siItemUnique;	}
	SI32 GetItemUnique( void )					{	return m_siItemUnique;			}

	void SetItemNum( SI32 siItemNum )			{	m_siItemNum = siItemNum;		}
	SI32 GetItemNum( void )						{	return m_siItemNum;				}

	void SetPara1( SI32 siPara1 )				{	m_siPara1 = siPara1;			}
	SI32 GetPara1( void )						{	return m_siPara1;				}

	void SetPara2( SI32 siPara2 )				{	m_siPara2 = siPara2;			}
	SI32 GetPara2( void )						{	return m_siPara2;				}

};

//----------------------------------------------------------------------------------
// 캐릭터 생성시 보상 클레스
//----------------------------------------------------------------------------------
class cltCreateCharReward
{
//-----------------------------------------
// Variable Area
//-----------------------------------------
private:
	cltRewardItem*	m_pclCreateCharReward[Const_Max_CreateCharReward_Item];
	SI32			m_siCreateCharRewardCount;

//-----------------------------------------
// Function Area
//-----------------------------------------
public:
	cltCreateCharReward();
	~cltCreateCharReward();

	void Init( void );
	// 보상 리스트를 읽어온다
	bool LoadRewardItemList( void );
	// 실제 보상 작업을 한다
	void GiveRewardItem( SI32 siPersonID );

private:
	bool GetRewardItem( IN SI32 siItemUnique, IN SI32 siItemNum, OUT cltItem* pclGiveItem );
	bool AddRewardItem( cltRewardItem* pclRewardItem );
};

//----------------------------------------------------------------------------------
// 캐릭터 추천인 보상 클레스
//----------------------------------------------------------------------------------
class cltRecommendReward
{
//-----------------------------------------
// Variable Area
//-----------------------------------------
private:
	cltRewardItem*	m_pclRecommendReward[Const_Max_RecommandReward_Item];			// 추천인 보상 아이템.
	cltRewardItem*	m_pclRecommendeeReward[Const_Max_RecommandReward_Item];			// 피 추천인 보상 아이템.

	//[진성] 중국에서 보상을 줄때 핸드폰 인증이 안됐으때 주는 아이템 정보.
	cltRewardItem*	m_pclRecommendRewardNoAuth[Const_Max_RecommandReward_Item];		// 추천인 보상 아이템.
	cltRewardItem*	m_pclRecommendeeRewardNoAuth[Const_Max_RecommandReward_Item];	// 피 추천인 보상 아이템.
	
	SI32			m_siRecommendGiveItemMaxLevel;									// 추천인이 보상을 받는 최고 레벨.   (중국은 인증됏을 때와 안됐을때 보상을 받는 레벨이 같아야 한다.)
	SI32			m_siRecommendeeGiveItemMaxLevel;								// 피 추천인이 보상을 받는 최고 레벨.(중국은 인증됏을 때와 안됐을때 보상을 받는 레벨이 같아야 한다.)

//-----------------------------------------
// Function Area
//-----------------------------------------
public:
	cltRecommendReward();
	~cltRecommendReward();

	void Init( void );
	// 보상 리스트를 읽어온다
	bool LoadRewardItemList( void );
	
	bool LoadRecommendRewardItemList( void );		// 추천을 한 사람의 아이템.
	bool LoadRecommendeeRewardItemList( void );		// 추천을 받은 사람의 아이템.

	// 실제 보상 작업을 한다
	bool GiveRewardItem( SI32 siPersonID, SI32 siLevel, bool bGiveMode, TCHAR* pszRecommenderName, bool _bAuth = false ); // true=추천자, false=추천을받은사람
	bool IsGiveRewardItem( SI32 siPersonID, SI32 siLevel, bool bGiveMode, TCHAR* pszRecommenderName );

	cltRewardItem**	GetRecommendRewardList()	{	return &m_pclRecommendReward[0];	}
	cltRewardItem**	GetRecommendeeRewardList()	{	return &m_pclRecommendeeReward[0];	}

	SI32	GetRecommendGiveItemMaxLevel( void )	{	return m_siRecommendGiveItemMaxLevel;	}
	SI32	GetRecommendeeGiveItemMaxLevel( void )	{	return m_siRecommendeeGiveItemMaxLevel;	}

private:
	bool AddRecommendRewardItemNoAuth( cltRewardItem* pclRewardItem );
	bool AddRecommendRewardItem( cltRewardItem* pclRewardItem );

	bool AddRecommendeeRewardItemNoAuth( cltRewardItem* pclRewardItem );
	bool AddRecommendeeRewardItem( cltRewardItem* pclRewardItem );
	
	void SendRecommenderMessage( cltRewardItem* pclRewardItem, SI32 siPersonID, SI32 siLevel, bool _bAuth  ); 
	void SendRecommendMessage( cltRewardItem* pclRewardItem, SI32 siPersonID, SI32 siLevel, TCHAR* pszRecommenderName, bool _bAuth  );
};


//----------------------------------------------------------------------------------
// 각종 보상관련 메니져
//----------------------------------------------------------------------------------
class cltRewardMgr
{
private:
	cltCreateCharReward	m_clCreateCharReward;
	cltRecommendReward	m_clRecommendReward;

public:
	cltRewardMgr();
	~cltRewardMgr();

	void			Init( void );

	void			LoadRewardList( void );

	// 캐릭터 생성 보상
	void			GiveCreateCharRewardItem( SI32 siPersonID );

	// 추천인 레벨업 보상
	bool			GiveRecommendRewardItem( SI32 siPersonID, SI32 siLevel, bool bGiveMode, TCHAR* pszRecommenderName, bool _bAuth = false ); // true=추천자, false=추천을받은사람
	bool			IsGiveRecommendRewardItem( SI32 siPersonID, SI32 siLevel, bool bGiveMode, TCHAR* pszRecommenderName );

	cltRewardItem**	GetRecommendRewardItemList( void );
	cltRewardItem**	GetRecommendeeRewardItemList( void );

	SI32			GetRecommendGiveItemMaxLevel( void )	{	return m_clRecommendReward.GetRecommendGiveItemMaxLevel();	}
	SI32			GetRecommendeeGiveItemMaxLevel( void )	{	return m_clRecommendReward.GetRecommendeeGiveItemMaxLevel();	}
	/*
	// 캐릭터 레벨업 보상
	void	GiveLevelUpRewardItem(  );
	*/

};

