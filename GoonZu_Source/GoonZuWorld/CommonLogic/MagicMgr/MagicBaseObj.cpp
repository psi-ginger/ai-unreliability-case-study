#include "MagicBaseObj.h"
#include "MagicMsgObj.h"
#include "MagicMgr.h"
#include "./MagicAttribute/MagicAttribute.h"

#include "../../Client/client.h"
extern cltCommonLogic* pclClient;

CMagicBaseObj::CMagicBaseObj( CMagicMgr *pMagicMgr) : CObj()
{
	m_pMagicMgr = pMagicMgr;
	m_pMagicAttributeData = NULL;
	
	Initialize();
}

CMagicBaseObj::~CMagicBaseObj()
{
	Destroy();
}

void CMagicBaseObj::Initialize()
{
	m_pMagicSpr = NULL;
	m_si32Status = MAGIC_REST_STATUS;
	m_pMagicHeader = NULL;	 
	m_si32CurrentFrame = 0;
	m_si32CurrentMagicDelayFrame = 0;
	m_si32SkillLevel = 0;
	m_siMagicKind = 0;	

	return;
}

void CMagicBaseObj::Destroy()
{
	return;
}

SI32 CMagicBaseObj::GetStatus()
{
	return m_si32Status;
}

void CMagicBaseObj::SetStatus( SI32 si32Status )
{
	m_si32Status = si32Status;
	return;
}

void CMagicBaseObj::GetMagicAttribute( SI32 si32MagicKind )
{
	m_pMagicAttributeData = m_pMagicMgr->GetMagicAttribute( si32MagicKind );
	return;
}

stMagicAttributeData *CMagicBaseObj::GetMagicAttributePtr()
{
	return m_pMagicAttributeData;
}

TSpr *CMagicBaseObj::GetMagicSpr()
{
	return m_pMagicSpr;
}

void CMagicBaseObj::SetMagicSpr( SI32 gImageIndex )
{
	m_pMagicSpr = pclClient->GetGlobalSpr( gImageIndex );

	return;
}

SI32 CMagicBaseObj::GetCurrentFrame()
{
	return m_si32CurrentFrame;
}

void CMagicBaseObj::SetCurrentFrame( SI32 siCurrentFrame )
{
	m_si32CurrentFrame = siCurrentFrame;
	return;
}

SI32 CMagicBaseObj::GetCurrentMagicDelayFrame()
{
	return m_si32CurrentMagicDelayFrame;
}

SI32 CMagicBaseObj::GetMaxFrameNum()
{
	return m_si32MaxFrameNum;
}

stMagicHeader *CMagicBaseObj::GetMagicHeader()
{
	return m_pMagicHeader;
}

SI32 CMagicBaseObj::GetSkillLevel()
{
	return m_si32SkillLevel;
}

void CMagicBaseObj::SetKind(SI32 kind)
{
	GetMagicAttribute( kind );
	m_si32MaxFrameNum = m_pMagicAttributeData->si32MagicMaxDrawFrame;
	m_siMagicKind = kind;
}

bool CMagicBaseObj::SetMagic( stMagicHeader *pstMagicHeader, SI32 si32SkillLevel )
{
	m_pMagicHeader = pstMagicHeader;
	
	SetStatus( MAGIC_PLAY_STATUS );
	m_si32CurrentFrame			 = 0;
	m_si32CurrentMagicDelayFrame = 0;

	if ( si32SkillLevel < 1 )
	{
		m_si32SkillLevel = 1;			// 나중에 SkillLevel - 1 한 값으로 계산하는데... 음수 값이면 이상하므로...
	}
	else
	{
		m_si32SkillLevel = si32SkillLevel;
	}

	return true;
}

bool CMagicBaseObj::Action()
{
	if( ++m_si32CurrentMagicDelayFrame >= m_pMagicAttributeData->si32MagicNextFrameDelay )
	{
		m_si32CurrentMagicDelayFrame = 0;

		if ( ++m_si32CurrentFrame >= m_si32MaxFrameNum )
		{
			SetStatus( MAGIC_REST_STATUS );
			m_si32CurrentFrame = 0;
			
			return true;
		}
	}

	return false;
}

void CMagicBaseObj::Draw()
{
	return;
}

// Push
bool CMagicBaseObj::Push( BYTE *pData, SI32 si32Size )
{
	m_pMagicMgr->Push( pData, si32Size );

	return true;
}
