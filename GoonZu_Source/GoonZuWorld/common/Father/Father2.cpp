//---------------------------------
// 2004/3/4 김태곤
//---------------------------------
#include "CommonLogic.h"
#include "..\..\Client\Client.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "Father.h"
#include "Char\CharManager\CharManager.h"


#include "..\..\Resource.h"
#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "JwLib\IsCorrectID\IsCorrectID.h"
//#include "../../common/HintDlg/HintDlg.h"
#include "NInterface/NHintDlg/NHintDlg.h"

extern cltCommonLogic* pclClient;

// 피후견자 리스트를 구한다 
bool cltFatherManager::AddChildList(cltChildInfo* pclinfo)
{
	SI32 i;
	
	if(pclinfo->clPerson.siPersonID <= 0)return false;
	
	// 이미 리스트에 있는지 확인한다. 
	for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
	{
		if(clChildList[i].clPerson.siPersonID == pclinfo->clPerson.siPersonID)return false;
	}
	
	for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
	{
		if(clChildList[i].clPerson.siPersonID == 0)
		{
			clChildList[i].Set(pclinfo);
			return true;
		}
	}
	
	return false;
	
}

// 피후견자 리스트를 모두 삭제한다. 
void cltFatherManager::DelChildList()
{
	SI32 i;
	
	for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
	{
		clChildList[i].Init();
	}
}


// 피후견자 이름을 구한다. 
TCHAR* cltFatherManager::GetChildName(SI32 personid)
{
	SI32 i;
	
	if(personid <= 0)return NULL;
	
	
	for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
	{
		if(clChildList[i].clPerson.GetPersonID() == personid)
		{
			return clChildList[i].clPerson.szName;
		}
	}
	
	return NULL;
}

void cltFatherManager::ChildSort(SI32 sisortdata[])
{
	SI32 i;
	SI32 index = 0;
	
	bool bChildInfo[MAX_CHILD_LIST_IN_CLIENT_NUMBER];
	
	for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
	{
		m_siChildListInx[i] = -1 ;
		bChildInfo[i] = false;
	}
	
	for(SI32 vill = 0;vill < MAX_CHILD_LIST_IN_CLIENT_NUMBER;vill++)
	{
		SI32 maxnum = -1;
		SI32 maxindex = -1;
		
		for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
		{
			if(bChildInfo[i] == false && sisortdata[i] > maxnum)
			{
				maxnum		= sisortdata[i];
				maxindex	= i;
			}
		}
		
		if(maxindex >= 0)
		{
			bChildInfo[maxindex] = true;
			m_siChildListInx[index] = maxindex;
			index++;
		}
	}
}