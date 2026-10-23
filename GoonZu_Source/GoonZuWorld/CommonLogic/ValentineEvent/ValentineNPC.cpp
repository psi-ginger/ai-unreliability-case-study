#include "ValentineEvent.h"
#include "../client/client.h"
#include "Char/CharManager/CharManager.h"
#include "../Client/ninterface/NNewNotify/NNewNotifyDlg.h"

bool CValentineNPC::ReceiveItem(SI32 siVillageUnique, SI32 siGivedItemCount)
{
	if( siVillageUnique < 0 || siVillageUnique >= MAX_VILLAGE_NUMBER)
		return false;
	if(siGivedItemCount <= 0)
		return false;

	m_siReceivedItemCount = m_siReceivedItemCount + siGivedItemCount;

	SetPresentItemByVillageUser(siVillageUnique,siGivedItemCount);

	return true;
}

bool CValentineNPC::SetPresentItemByVillageUser(SI32 siVillageUnique, SI32 siPresentItemCount)
{
	if( siVillageUnique <= 0 || siVillageUnique >= MAX_VILLAGE_NUMBER)
		return false;
	if(siPresentItemCount <= 0)
		return false;

	m_TheNumberOfItemEachVillageList[siVillageUnique] = m_TheNumberOfItemEachVillageList[siVillageUnique] + siPresentItemCount;

	return true;
}

void CValentineNPC::Init()
{
	m_siReceivedItemCount = 0;
	for(SI32 siVillageIndex = 0; siVillageIndex < MAX_VILLAGE_NUMBER; siVillageIndex++)
	{
		m_TheNumberOfItemEachVillageList[siVillageIndex] = 0;
	}
}

SI32 CValentineNPC::GetUniqueOfBestVillage()
{
	SI32 siVillageUnique = 0;
	SI32 siTmpMaxCount = 0;

	for(SI32 siVillageIndex = 0; siVillageIndex < MAX_VILLAGE_NUMBER; siVillageIndex++)
	{
		if( siTmpMaxCount < m_TheNumberOfItemEachVillageList[siVillageIndex])
		{
			siVillageUnique = siVillageIndex;
		}
	}
	return siVillageUnique;
}

void CValentineNPC::ShowImpression(SI32 siVillageUnique)
{
	cltClient* pclClientLogic = (cltClient*)pclClient;

	NTCHARString256	strText(GetTxtFromMgr(m_siThxTextIndex));	

	TCHAR *pszVillageName = NULL;

	if(siVillageUnique <= 0 || MAX_VILLAGE_NUMBER <= siVillageUnique)
	{
		pszVillageName = GetTxtFromMgr(258);
	}
	else
	{
		pszVillageName = pclClientLogic->pclVillageManager->GetName(siVillageUnique);
	}


	strText.Replace("#village#", pszVillageName);

	if( NULL == pclClientLogic->m_pDialog[NNEWNOTIFY_DLG])
		pclClientLogic->CreateInterface(NNEWNOTIFY_DLG);
	if( NULL != pclClientLogic->m_pDialog[NNEWNOTIFY_DLG])
	{
		((CNNewNotifyDlg*)pclClientLogic->m_pDialog[NNEWNOTIFY_DLG])->MovePos(330, 150);
		((CNNewNotifyDlg*)pclClientLogic->m_pDialog[NNEWNOTIFY_DLG])->SetImage(m_siCharKind);
		((CNNewNotifyDlg*)pclClientLogic->m_pDialog[NNEWNOTIFY_DLG])->SetText(GetTxtFromMgr(15630),strText);
	}
}
