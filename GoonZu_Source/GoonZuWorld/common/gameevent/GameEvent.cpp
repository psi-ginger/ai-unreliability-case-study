//---------------------------------
// 2005/3/17 성준엽
//---------------------------------

#include "CommonLogic.h"
#include "../../Server/Server.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "GameEvent.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "MsgType-Item.h"

#include "Char/CharManager/CharManager.h"
#include "Char/CharServer/Char-Server.h"

#include "../../DBManager/GameDBManager_World/DBMsg-item.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"

extern cltCommonLogic* pclClient;


cltGameEvent::cltGameEvent()
{
	Init();
}

cltGameEvent::~cltGameEvent()
{
}


void cltGameEvent::Init()
{
	//m_siWinPrizeMemebers = 0 ;
	//m_siTotalCheckMemebers = 0 ;

	m_siPara1 = 0 ;
	m_siPara2 = 0 ;
}

bool cltGameEvent::CheckPrizeEvent(SI32 id,SI32 ticket)
{
/*	if ( m_siWinPrizeMemebers >= MAX_WINPRIZE_MEMBER ) return false ;
	if ( IsPC(id) == false ) return false ;

	cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
	if ( pclchar == NULL ) return false ;
	if ( pclchar->pclCI->clBI.bNewAccount == false ) return false ; // 신규 가입자인가?
	if ( pclchar->pclCI->clBI.bGameEvent == true ) return false ; // 이미 다른 캐릭으로라도 했었는가 안 했었는가?
	if ( pclchar->pclCI->clBI.bWinPrizeEvent == true ) return false ; // 이미 당첨되었는가?

	m_siTotalCheckMemebers ++ ;

	if ( m_siTotalCheckMemebers % 4 == 0 )
	{
		return true ;
	}*/

/*	if ( rand() % 100 == 0 )
	{
		if ( !pclClient->pclCM->CR[id]->pclCI->clBI.bGameEvent )
		{
			sDBRequest_Prize_GameEvent clevent(pclClient->pclCM->CR[id]->pclCI->GetPersonID(), id, 1, false);
			((cltServer*)pclClient)->SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clevent);
			((cltServer*)pclClient)->SendPostOfficeItemToPerson(pclClient->pclCM->CR[id]->pclCI->GetPersonID(), NULL,
																"경복궁의 보부상을 확인해주세요.");
			return true ;
		}
	}*/

	/*cyj  상 이벤트 임시 삭제	
	SI32 itemunique = 0 ;
	SI32 itemnum	= 0 ;
	cltItem clitem;
	clitem.Init();

	SI32 index = rand() % 8 ;
	BOOL bRareitem = FALSE ;

	if ( ticket == ITEMUNIQUE(14500) ) // 노력상
	{
		if ( (pclClient->siServiceArea == ConstServiceArea_Korea && rand() % 1000 == 0 ) )
		{
			bRareitem = TRUE ;
			index = rand() % 100 ;

			if ( index > 85 )
			{
				itemunique = ITEMUNIQUE(14165);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("순간이동[1일]정액권"));
				itemnum = 1 ;
			}
			else if ( index > 70 )
			{
				itemunique = ITEMUNIQUE(18110);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("파란망토"));
				itemnum = 1 ;
			}
			else if ( index > 55 )
			{
				itemunique = ITEMUNIQUE(24015);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("소환수[진화]이용권"));
				itemnum = 1 ;
			}
			else if ( index > 40 )
			{
				itemunique = ITEMUNIQUE(15100);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("가방"));
				itemnum = 1 ;
			}
			else if ( index > 25 )
			{
				itemunique = ITEMUNIQUE(13390);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("강시모"));
				itemnum = 1 ;
			}
			else if ( index > 20 )
			{
				itemunique = ITEMUNIQUE(5506);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("백마망아지"));
				itemnum = 1 ;
			}
			else if ( index > 5 )
			{
				itemunique = ITEMUNIQUE(3500);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("짐승사료"));
				itemnum = 100 ;
			}
			else if ( index >= 0 )
			{
				itemunique = ITEMUNIQUE(11760);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("김시민"));
				itemnum = 1 ;
			}
		}
		else
		{
			switch(index) {
			case 0:
				{
					itemunique = ITEMUNIQUE(2565);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("쌍화탕"));
					itemnum = 20 ;
				}
				break;
			case 1:
				{
					itemunique = ITEMUNIQUE(2590);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("사군자탕"));
					itemnum = 20 ;
				}
				break;
			case 2:
				{
					itemunique = ITEMUNIQUE(3265);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("꼬치구이"));
					itemnum = 10 ;
				}
				break;
			case 3:
				{
					itemunique = ITEMUNIQUE(5000);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("석탄"));
					itemnum = 10 ;
				}
				break;
			case 4:
				{
					itemunique = ITEMUNIQUE(6105);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("대나무"));
					itemnum = 10 ;
				}
				break;
			case 5:
				{
					itemunique= ITEMUNIQUE(7000);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("쇠가죽"));
					itemnum = 10 ;
				}
				break;
			case 6:
				{
					itemunique = ITEMUNIQUE(13305);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("철가면"));
					itemnum = 10 ;
				}
				break;
			case 7:
				{
					itemunique = ITEMUNIQUE(8660);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("최대내구상승부적"));
					itemnum = 1 ;
				}
				break;
			default:
				return false ;
			}
		}
	}
	else if ( ticket == ITEMUNIQUE(14400) ) // 우등상
	{
		if ( (pclClient->siServiceArea == ConstServiceArea_Korea && rand() % 500 == 0 ) )
		{
			bRareitem = TRUE ;
			index = rand() % 100 ;

			if ( index > 85 )
			{
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("순간이동[1주]정액권"));
				itemunique = ITEMUNIQUE(14170);
				itemnum = 1 ;
			}
			else if ( index > 70 )
			{
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("명궁의활집"));
				itemunique = ITEMUNIQUE(18400);
				itemnum = 1 ;
			}
			else if ( index > 55 )
			{
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("소환수[진화]이용권"));
				itemunique= ITEMUNIQUE(24015);
				itemnum = 2 ;
			}
			else if ( index > 40 )
			{
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("영원의가방"));
				itemunique = ITEMUNIQUE(15300);
				itemnum = 1 ;
			}
			else if ( index > 25 )
			{
				itemunique = ITEMUNIQUE(13380);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("일지매모"));
				itemnum = 1 ;
			}
			else if ( index > 20 )
			{
				itemunique = ITEMUNIQUE(5540); 
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("천리마망아지"));
				itemnum = 1 ;
			}
			else if ( index > 5 )
			{
				itemunique = ITEMUNIQUE(3500);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("짐승사료"));
				itemnum = 300 ;
			}
			else if ( index >= 0 )
			{
				itemunique = ITEMUNIQUE(11730);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("장보고"));
				itemnum = 1 ;
			}
		}
		else
		{
			switch(index) {
			case 0:
				{
					itemunique = ITEMUNIQUE(2575);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("십전대보탕"));
					itemnum = 50;
				}
				break;
			case 1:
				{
					itemunique = ITEMUNIQUE(2595);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("독삼탕"));
					itemnum = 50 ;
				}
				break;
			case 2:
				{
					itemunique = ITEMUNIQUE(3295); 
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("전복죽"));
					itemnum = 20 ;
				}
				break;
			case 3:
				{
					itemunique = ITEMUNIQUE(5015);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("철괴"));
					itemnum = 20 ;
				}
				break;
			case 4:
				{
					itemunique = ITEMUNIQUE(6100);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("닥나무"));
					itemnum = 20 ;
				}
				break;
			case 5:
				{
					itemunique = ITEMUNIQUE(7050); 
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("실"));
					itemnum = 20 ;
				}
				break;
			case 6:
				{
					itemunique = ITEMUNIQUE(8055);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("훈장의회초리"));
					itemnum = 20 ;
				}
				break;
			case 7:
				{
					itemunique = ITEMUNIQUE(8660);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("최대내구상승부적"));
					itemnum = 3 ;
				}
				break;
			default:
				return false ;
			}
		}
	}
	else if ( ticket == ITEMUNIQUE(14300) ) // 모범상
	{
		if ( (pclClient->siServiceArea == ConstServiceArea_Korea && rand() % 333 == 0 ) )
		{
			bRareitem = TRUE ;
			index = rand() % 100 ;

			if ( index > 85 )
			{
				itemunique = ITEMUNIQUE(14190);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("순간이동[1달]정액권"));
				itemnum = 1 ;
			}
			else if ( index > 70 )
			{
				itemunique = ITEMUNIQUE(18800);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("타락천사날개"));
				itemnum = 1 ;
			}
			else if ( index > 55 )
			{
				itemunique = ITEMUNIQUE(24015);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("소환수[진화]이용권"));
				itemnum = 4 ;
			}
			else if ( index > 40 )
			{
				//cyj 상태초기화이용권 유니크변경 14090->24030
				//itemunique = 14090 ;  // 상태초기화이용권
				itemunique = ITEMUNIQUE(24030) ;  // 상태초기화이용권
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("상태초기화이용권"));
				itemnum = 1 ;
			}
			else if ( index > 25 )
			{
				itemunique = ITEMUNIQUE(13400);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("견우직녀모"));
				itemnum = 1 ;
			}
			else if ( index > 20 )
			{
				itemunique = ITEMUNIQUE(5560);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("아기호랑이"));
				itemnum = 1 ;
			}
			else if ( index > 5 )
			{
				itemunique = ITEMUNIQUE(3500);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("짐승사료"));
				itemnum = 500 ;
			}
			else if ( index >= 0 )
			{
				itemunique = ITEMUNIQUE(11750);
				//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("다모"));
				itemnum = 1 ;
			}
		}
		else
		{
			switch(index) {
			case 0:
				{
					itemunique = ITEMUNIQUE(2595);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("우황청심원"));
					itemnum = 100 ;
				}
				break;
			case 1:
				{
					itemunique = ITEMUNIQUE(2090);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("녹용대보탕"));
					itemnum = 100 ;
				}
				break;
			case 2:
				{
					itemunique = ITEMUNIQUE(3315); 
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("갈미찜"));
					itemnum = 50 ;
				}
				break;
			case 3:
				{
					itemunique =  ITEMUNIQUE(5045);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("금괴"));
					itemnum = 30 ;
				}
				break;
			case 4:
				{
					itemunique = ITEMUNIQUE(6125);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("소나무"));
					itemnum = 50 ;
				}
				break;
			case 5:
				{
					itemunique = ITEMUNIQUE(7070);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("금사"));
					itemnum = 30 ;
				}
				break;
			case 6:
				{
					itemunique= ITEMUNIQUE(13306);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("마도귀의눈"));
					itemnum = 50 ;
				}
				break;
			case 7:
				{
					itemunique = ITEMUNIQUE(8100);
					//itemunique = pclClient->pclItemManager->FindUniqueFromName(TEXT("선박수리자재"));
					itemnum = 5 ;
				}
				break;
			default:
				return false ;
			}
		}
	}
	else return false ;

	bool brareswitch = false;
	if(pclClient->pclItemManager->MakeRandItemUnique( itemunique, &clitem, 0, &brareswitch) == true)
	{
		clitem.SetItemNum(itemnum);

		SI32 rtnvalue = 0;
		SI32 inputreason = INPUTITEMPERSON_OPTION_WAR ;
		if ( bRareitem )
			inputreason = INPUTITEMPERSON_OPTION_CRAZYEVENT ;
		if(((cltCharServer*)pclClient->pclCM->CR[id])->InputItem(pclClient->pclItemManager, &clitem,
			inputreason, &rtnvalue) == false)
		{
			((cltServer*)pclClient)->SendLog_InventoryFull( pclClient->pclCM->CR[id]->pclCI->GetPersonID(),itemunique,itemnum,
				inputreason);
		}

		bool setitem = false ;
		// 강시모이면
		if ( itemunique == ITEMUNIQUE(13390) )
		{
			setitem = true ;
			itemunique = ITEMUNIQUE(9440) ;
			itemnum = 1 ;
		}
		// 일지매모라면
		else if ( itemunique == ITEMUNIQUE(13380))
		{
			setitem = true ;
			itemunique = ITEMUNIQUE(9430) ;
			itemnum = 1 ;
		}
		// 견우직녀모라면
		else if (itemunique == ITEMUNIQUE(13400) )
		{
			setitem = true ;
			itemunique = ITEMUNIQUE(9450) ;
			itemnum = 1 ;
		}

		if ( setitem )
		{
			clitem.Init() ;
			if(pclClient->pclItemManager->MakeRandItemUnique( itemunique, &clitem, 0, &brareswitch) == true)
			{
				clitem.SetItemNum(itemnum);

				if(((cltCharServer*)pclClient->pclCM->CR[id])->InputItem(pclClient->pclItemManager, &clitem,
					inputreason, &rtnvalue) == false)
				{
					((cltServer*)pclClient)->SendLog_InventoryFull( pclClient->pclCM->CR[id]->pclCI->GetPersonID(),itemunique,itemnum,
						inputreason);
				}
			}
		}
	}
	*/

	return true ;
}

void cltGameEvent::GetItemUniqueAndNumRamdomSelect(SI32 * unique , SI32 * num)
{
/*	if ( m_siWinPrizeMemebers >= MAX_WINPRIZE_MEMBER ) return ;

	if ( m_siPara1 > MAX_PARA_1 && m_siPara2 > MAX_PARA_2 ) return ;

	m_siWinPrizeMemebers ++ ;

	if ( m_siPara1 > MAX_PARA_1 )
	{
		m_siPara2 ++ ;

		*unique = ITEMUNIQUE(2040) ;
		*num = 400 ;

		return ;
	}
	else if ( m_siPara2 > MAX_PARA_2 )
	{
		m_siPara1 ++ ;

		*unique = 14005 ;
		*num = 15 ;

		return ;
	}
	else
	{
		if ( rand() % 2 )
		{
			m_siPara2 ++ ;

			*unique = ITEMUNIQUE(2040);
			*num = 400 ;

			return ;
		}
		else
		{
			m_siPara1 ++ ;

			*unique = 14005 ;
			*num = 15 ;

			return ;
		}
	}*/
}