#ifndef _NBITARRAY_H_
#define _NBITARRAY_H_

#include "NDataTypes.h"

//=============================================================================
//  CONSTANTS
//=============================================================================
const UI32 BIT_OFFSET_MASK		= 0x0000001F;
const UI32 BITS_PER_UI32_SHIFT	= 5;				// 2^5 = 32 bit


//=============================================================================
//	CLASS   NBitArray
//! @brief 
//=============================================================================
template<SI32 T_MAXBIT>
class NBitArray
{
private:
	UI32	m_uiCountUI32;
	UI32	m_uiBits[(T_MAXBIT >> BITS_PER_UI32_SHIFT) + 1];

public:
	NBitArray()	:	m_uiCountUI32((T_MAXBIT >> BITS_PER_UI32_SHIFT) + 1)	{		Init();		}

	void Init()					{	ClearAll();													}
	void SetAll      ()			{	FillMemory(m_uiBits, sizeof(UI32) * m_uiCountUI32, 0xFF);	}
	void ClearAll    ()			{	ZeroMemory(m_uiBits, sizeof(UI32) * m_uiCountUI32);			}

	void SetBit      (UI32 uiIndex)
	{
		if(uiIndex < 0 || uiIndex >= T_MAXBIT)		return;
		m_uiBits[uiIndex >> BITS_PER_UI32_SHIFT] |= (1 << (uiIndex & BIT_OFFSET_MASK));
	}

	void ClearBit    (UI32 uiIndex)
	{
		if(uiIndex < 0 || uiIndex >= T_MAXBIT)		return;
		m_uiBits[uiIndex >> BITS_PER_UI32_SHIFT] &= ~(1 << (uiIndex & BIT_OFFSET_MASK));
	}

	bool GetBit      (UI32 uiIndex) const
	{
		if(uiIndex < 0 || uiIndex >= T_MAXBIT)		return false;
		return ((m_uiBits[uiIndex >> BITS_PER_UI32_SHIFT] & (1 << (uiIndex & BIT_OFFSET_MASK))) ? true : false) ;
	}

	void Set(NBitArray& arr)
	{
        memcpy(m_uiBits, arr.m_uiBits, sizeof(UI32) * m_uiCountUI32);
	}
};

#endif