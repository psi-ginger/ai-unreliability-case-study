// Durability.h: interface for the CDurability class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DURABILITY_H__E5CF08FE_FB12_46B0_8E73_2E384063FB96__INCLUDED_)
#define AFX_DURABILITY_H__E5CF08FE_FB12_46B0_8E73_2E384063FB96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DurabilityInfo.h"
#include "TSpr.h"

class CDurability  
{
public:
	CDurability(SI32 x , SI32 y);
	virtual ~CDurability();

	void Set(DWORD dwType,BOOL bRepair); // bRepair = FALSE 수리 , bRepair = TRUE 내구도 낮음
	void Action();
	void Draw(LPDIRECTDRAWSURFACE7 lpsurface,SI32 x , SI32 y );
	BOOL IsTypeExist(DWORD dwType); // TRUE 면 존재함 FALSE 면 존재하지 않음
private:
	TSpr	m_DurabilityChar;

	TSpr m_DurabilityWeapon;
	TSpr m_DurabilityArmour;
	TSpr m_DurabilityHelmet;
	TSpr m_DurabilityBelt;
	TSpr m_DurabilityShoes;
	TSpr m_DurabilityRing;
	TSpr m_DurabilityNeck;
	TSpr m_DurabilityMantle;

	BOOL m_bDraw;

	SI32 m_siDelay;
	SI32 m_siDrawX;
	SI32 m_siDrawY;

	DWORD m_dwType;
};

#endif // !defined(AFX_DURABILITY_H__E5CF08FE_FB12_46B0_8E73_2E384063FB96__INCLUDED_)

