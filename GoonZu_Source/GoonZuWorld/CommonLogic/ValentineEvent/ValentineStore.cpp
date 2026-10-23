#include "ValentineStore.h"
#include "../Client/Client.h"
#include "../Client/ninterface/NNewNotify/NNewNotifyDlg.h"

CValentineStore::CValentineStore()
{
	m_siCharUnique = 0;
	m_siNameTextUnique = 0;
	Init();
}

CValentineStore::~CValentineStore()
{
	Destroy();
}

void CValentineStore::Init()
{
	m_siRemainedChocolateCount = 0;
	m_siRemainedCandyCount = 0;
	m_bOpened = false;
}
void CValentineStore::Create(SI32 siCharUnique,SI32 siNameTextUnique)
{
	m_siCharUnique = siCharUnique;
	m_siNameTextUnique = siNameTextUnique;
	Init();
}
void CValentineStore::Destroy()
{
	m_siCharUnique = 0;
	m_siNameTextUnique = 0;
	Init();
}
BOOL CValentineStore::Open()
{
	m_bOpened = true;

	return TRUE;
}

BOOL CValentineStore::MakeItem(SI32 siChocolateCountThatNPCBuy, SI32 siCandyCountThatNPCBuy)
{
	if ( siChocolateCountThatNPCBuy > MAXCHOCOLATECOUNT || siCandyCountThatNPCBuy > MAXCANDYCOUNT)
	{
		return FALSE;
	}

	m_siRemainedChocolateCount = MAXCHOCOLATECOUNT - siChocolateCountThatNPCBuy;
	m_siRemainedCandyCount = MAXCANDYCOUNT - siCandyCountThatNPCBuy;

	return TRUE;
}

void CValentineStore::Close()
{
	Init();
}

BOOL CValentineStore::SellChocolate(SI32 siCount)
{
	if( HaveChocolate() )
	{
		m_siRemainedChocolateCount = m_siRemainedChocolateCount - siCount;
		if( m_siRemainedChocolateCount < 0)
			m_siRemainedChocolateCount = 0;
		return TRUE;
	}
	
	return FALSE;
}

BOOL CValentineStore::SellCandy(SI32 siCount)
{
	if( HaveCandy() )
	{
		m_siRemainedCandyCount = m_siRemainedCandyCount - siCount;
		if( m_siRemainedCandyCount < 0)
			m_siRemainedCandyCount = 0;
		return TRUE;
	}

	return FALSE;
}
BOOL CValentineStore::IsOpened()
{
	return m_bOpened;
}
SI32 CValentineStore::GetCharUnique()
{
	return m_siCharUnique;
}

BOOL CValentineStore::HaveChocolate()
{
	if(m_siRemainedChocolateCount <= 0)
		return FALSE;

	return TRUE;
}
BOOL CValentineStore::HaveCandy()
{
	if(m_siRemainedCandyCount <= 0)
		return FALSE;

	return TRUE;
}

SI32 CValentineStore::GetNameTextUnique()
{
	return m_siNameTextUnique;
}

BOOL CValentineStore::IsCreate()
{
	if( m_siCharUnique == 0)
		return false;
	
	return true; 
}

SI32 CValentineStore::GetRemainedChocolateCount()
{
	return m_siRemainedChocolateCount;
}
SI32 CValentineStore::GetRemainedCandyCount()
{
	return m_siRemainedCandyCount;
}
void CValentineStore::AdvertiseOpening()
{
	cltClient* pclClientLogic = (cltClient*)pclClient;

	if( NULL == pclClientLogic->m_pDialog[NNEWNOTIFY_DLG])
		pclClientLogic->CreateInterface(NNEWNOTIFY_DLG);
	
	if( NULL != pclClientLogic->m_pDialog[NNEWNOTIFY_DLG])
	{
		((CNNewNotifyDlg*)pclClientLogic->m_pDialog[NNEWNOTIFY_DLG])->MovePos(330, 150);
		((CNNewNotifyDlg*)pclClientLogic->m_pDialog[NNEWNOTIFY_DLG])->SetImage(pclClientLogic->GetUniqueFromHash("KIND_CHOCOLATE"));
		((CNNewNotifyDlg*)pclClientLogic->m_pDialog[NNEWNOTIFY_DLG])->SetText(GetTxtFromMgr(15613),GetTxtFromMgr(15614));
	}
}