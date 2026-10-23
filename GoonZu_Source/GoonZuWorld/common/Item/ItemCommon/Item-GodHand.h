#pragma once 

#include "../../../Lib/Directives.h"
#include <vector>

class CGodHandData	
{
private:

	SI32 m_siUnique;
	SI32 m_siNumber;
	SI32 m_siRate;
	bool m_bShowAllUser;

public:

	CGodHandData() : m_siUnique( 0 ), m_siNumber( 0 ), m_siRate( 0 ), m_bShowAllUser( false )
	{
	}

	CGodHandData( SI32 Unique, SI32 Number, SI32 Rate, bool ShowAllUser) : 
	              m_siUnique( Unique ), m_siNumber( Number ), m_siRate( Rate ), m_bShowAllUser( ShowAllUser )
	{
	}

	~CGodHandData()
	{
	}

	void SetData( SI32 Unique, SI32 Number, SI32 Rate, bool ShowAllUser )
	{
		m_siUnique = Unique;
		m_siNumber = Number;
		m_siRate = Rate;
		m_bShowAllUser = ShowAllUser;
	}

	SI32 GetUnique()
	{
		return m_siUnique;
	}

	SI32 GetNumber()
	{
		return m_siNumber;
	}

	SI32 GetRate()
	{
		return m_siRate;
	}

	bool IsShowAllUser()
	{
		return m_bShowAllUser;
	}
};

typedef std::vector<CGodHandData*>  vGodHandDataInfo;
typedef vGodHandDataInfo::iterator	vIterGodHandDataInfo;

class CItemGodHandManager
{
public:

	vGodHandDataInfo m_vGodHandManager;

	CItemGodHandManager();
	~CItemGodHandManager();
	
	void LoadFile();
	CGodHandData*	FindItem( SI32 RateValue );
};