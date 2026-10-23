//---------------------------------
// 2004/10/14 김태곤
//---------------------------------

#include "../Common/CommonHeader.h"
#include "Client.h"


//---------------------------------------------
// Common
//---------------------------------------------
#include "Skill\Skill-Manager.h"
#include "Quest\Quest.h"
#include "NInterface/NAutoHunting/NAutoHunting.h"

#include "NInterface/NItemMallManager/NItemMallManager1.h"
#include "NInterface/nmudangdlg/NMuDangDlg.h"
#include "NInterface\NCommonMsgDlg\NCommonMsgDlg.h"
#include "Char\CharManager\CharManager.h"

#include "MiniHomeUpdates/PICAManagerCheck.h"
#include "MiniHomeUpdates/PopupCheck.h"
#include "../common/Event/MonsterAttackEvent/MonsterAttack.h"

//---------------------------------------------
// CommonLogic
//---------------------------------------------
#include "MsgRval-Define.h"
#include "MsgType-System.h"
#include "MsgType-Person.h"
#include "Cityhall\Cityhall.h"
#include "Bank/Bank-Manager.h"
#include "Hunt/Hunt.h"
#include "HorseMarket/HorseMarket.h"
#include "RealEstate/RealEstate.h"
#include "SummonMarket/SummonMarket.h"
#include "Land/Land.h"
#include "Feast/Feast.h"
#include "SummonHeroMarket/SummonHeroMarket.h"
#include "Mine/Mine.h"
#include "Guild/Guild.h"

//---------------------------------------------
// Client
//---------------------------------------------
#include "Music/Music.h"
#include "InterfaceMgr/InterfaceMgr.h"
#include "Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "Interface/Hyungzo/HyungzoDlg.h"
#include "Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "Interface/NoticeEventMsgDlg/NoticeEventMsgDlg.h"
#include "NInterface/NHongGilDongDlg/NHongGilDongDlg.h"
#include "NInterface/nwennyoldman/NWennyOldMan.h"
#include "Ninterface/nnewquestdlg/NNewQuestDlg.h"
#include "ninterface/NTreasureBoxDlg/NTreasureBoxDlg.h"
#include "ninterface/NManufactureNPCDlg/NManufactureNPC.h"
#include "ninterface/ngachadlg/NGachaDlg.h"
#include "ninterface/NSpaceBoxDlg/NSpaceBoxDlg.h"
#include "ninterface/ninfodlg/NInfoDlg.h"
#include "ninterface/NHiddenQuestDlg/NHiddenQuestDlg.h"
#include "ninterface/NArtifactDlg/NArtifactBaseDlg.h"
#include "ninterface/NMonsterAttackEventDlg/NMonsterAttackEventDlg.h"
#include "ninterface/NOpenBigItemBoxDlg/NOpenBigItemBoxDlg.h"
#include "ninterface/nnotifygetexpanditemdlg/NNotifyGetExpAndItemDlg.h"
#include "NInterface/NFishingDlg/NFishingDlg.h"
#include "NInterface/nfarmingdlg/NFarmingDlg.h"
#include "NInterface/nminingdlg/NMiningDlg.h"
#include "ninterface/NServerEventDlg/NServerInstanceMapBounusDlg.h"
#include "ninterface/NNamingQuest/NNamingQuestListDlg.h"
#include "ninterface/NYesOrNoDlg/NYesOrNoDlg.h"
#include "ninterface/NHelperDlg/NHelperQuestionOrAnswerDlg.h"
#include "ninterface/NPromoter/NPromoter_Give_Item_Notice.h"
#include "ninterface/NGiantRaccoonResurrectEventDlg/NGiantRaccoonResurrectEventDlg.h"
#include "ninterface/NNewNotify/NNewNotifyDlg.h"
#include "Interface/DrawChat/DrawChat.h"

//---------------------------------------------
// DBManager
//---------------------------------------------
#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../common/VoteMgr/VoteBaseInfo.h"
#include "../../Common/Order/Order.h"

extern cltCommonLogic* pclClient;

// 에러 박스를 사용하여 표시되는 메시지. 
void cltClient::OperateByServerResponseValueGlobal(const cltServerResponseValue* pclreturn)
{

	TCHAR Nbuffer[512];
	const SI32 constEffectSuccess = GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS"));
	const SI32 constEffectFailure = GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE"));

	static DWORD dwLastQuestCheckClock = 0;


	switch(pclreturn->siReturnValue)
	{
	case SRVAL_TRADEMERCHANT_MONEY_INSUFFICIENTY:
		{
			TCHAR * pTitle = GetTxtFromMgr(191);
			TCHAR * pText = GetTxtFromMgr(192);
			if ( !pTitle || !pText ) break ;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
		}
		break;
	case SRVAL_TRADEMERCHANT_NOT_BUY_TIME:
		{
			TCHAR * pTitle = GetTxtFromMgr(191);
			TCHAR * pText = GetTxtFromMgr(193);
			if ( !pTitle || !pText ) break ;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
		}
		break;
	case SRVAL_TRADEMERCHANT_PREPARE:
		{
			TCHAR * pTitle = GetTxtFromMgr(191);
			TCHAR * pText = GetTxtFromMgr(194);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
		}
		break;
	case SRVAL_FOREIGNMERCHANT_NOTBUYCASHITEM_SBS:
		{
			TCHAR* pTitle = GetTxtFromMgr(513);
			TCHAR* pText = NULL; //GetTxtFromMgr(516);


			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FOREIGNMERCHANT_NOTBUYCASHITEM_PRICE:
		{
			TCHAR* pTitle = GetTxtFromMgr(513);
			TCHAR* pText = GetTxtFromMgr(517);


			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_MARKET_SELL_FAIL:
		{
			PushEffect(constEffectFailure, 0, 0);

			TCHAR* ptitle = GetTxtFromMgr(195);
			TCHAR* ptext = NULL ;


			switch(pclreturn->siPara1)
			{
			case 100:		// 렉으로 실패. 
				ptext = GetTxtFromMgr(196);
				break;
			case 200:
				ptext = GetTxtFromMgr(197);
				break;
			case 300:
				ptext = GetTxtFromMgr(198);
				break;
			case 400:
				ptext = GetTxtFromMgr(199);
				break;

			case 500:
				ptext = GetTxtFromMgr(200);
				break;

			case 550:
				ptext = GetTxtFromMgr(201);
				break;

			case 600:
				ptext = GetTxtFromMgr(202);
				break;
			case 700:
				ptext = GetTxtFromMgr(203);
				break;

			case 800:
				ptext = GetTxtFromMgr(204);
				break;

			case 900:
				ptext = GetTxtFromMgr(205);
				break;

			case 1000:
				ptext = GetTxtFromMgr(206);
				break;

			case 1200:
				ptext = GetTxtFromMgr(207);
				break;

			case 1300:
				ptext = GetTxtFromMgr(208);
				break;

			case 1400:
				ptext = GetTxtFromMgr(6354);
				break;

			default:
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(ptitle,TEXT("ErrorCode:%d"), pclreturn->siPara1);
				break;
			}

			if ( !ptext ) break ;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(ptitle, ptext);
		}
		break;

		//------------------------------
		// 시전
		//------------------------------
	case SRVAL_HOUSE_VILLAGELEVELLIMIT:
		{
			TCHAR * pTitle = GetTxtFromMgr(209) ;
			TCHAR * pText = GetTxtFromMgr(210) ;
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1 );
		}
		break;

		//------------------------------
		// 주식 공모  
		//------------------------------
	case SRVAL_STOCKGIVEFAIL_FULL:
		{
			TCHAR * pTitle = GetTxtFromMgr(214) ;
			TCHAR * pText = GetTxtFromMgr(215) ;
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1);
		}
		break;
	case SRVAL_STOCK_FAIL_DISTRIBUTIONSTOP:
		{
			TCHAR * pTitle = GetTxtFromMgr(216) ;
			TCHAR * pText = GetTxtFromMgr(217) ;
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
		}
		break;
	case SRVAL_AUTOSTOCKDISTRIBUTION:
		{
			TCHAR * pTitle = GetTxtFromMgr(218) ;
			TCHAR * pText = NULL ;
			if ( !pTitle ) break ;

			if( pclreturn->siPara1 )
			{
				pText = GetTxtFromMgr(219);
				if ( !pText ) break ;
			}
			else
			{
				pText = GetTxtFromMgr(220);
				if ( !pText ) break ;
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

		//------------------------------
		// 퀘스트. 
		//------------------------------
	case SRVAL_QUEST_FAIL_FULLINV:
		{
/*
			static SI32 siCnt = 5;
			siCnt++;
			if( siCnt < 5 )
			{
				break;
			}
			else
			{
				siCnt = 0;
			}
*/
			bool bForceNotify = (bool)(pclreturn->siPara1 == 1);
			//KHY - 0710 - 30초 에 한번씩 경고창 출력.
			if(TABS(pclClient->CurrentClock - dwLastQuestCheckClock ) >= 30000)
			{
				dwLastQuestCheckClock = pclClient->CurrentClock;
			}
			else
			{
				// 강제 출력이 아닌경우는 스킵
				if( bForceNotify == false )
					break;
			}

			// 100 프레임에 한번만 메시지 박스가 업데이트 되도록 카운트
 			TCHAR * pTitle = GetTxtFromMgr(221) ;
			TCHAR * pText = GetTxtFromMgr(222) ;
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
		//------------------------------
		// Daily 퀘스트. 
		//------------------------------
	case SRVAL_DAILYQUEST_FAIL_NOTEXISTLIST:
		{
			TCHAR * pTitle = GetTxtFromMgr(223) ;
			TCHAR * pText = GetTxtFromMgr(224) ;
			if ( !pTitle || !pText ) break ;
			PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_FAIL")), 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

		//------------------------------
		// 로그인 
		//------------------------------
	case SRVAL_MAKEPERSON_FAIL_DUPNAME:
		{
			TCHAR * pTitle = GetTxtFromMgr(225) ;
			TCHAR * pText = GetTxtFromMgr(226) ;
			if ( !pTitle || !pText ) break ;
			//PushEffect(constEffectFailure, 0, 0);
			//m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			g_ErrorMsg.Set(false,pTitle,pText);
		}
		break;
		//------------------------------
		// DELETE CAHR
		//------------------------------
	case SRVAL_DELPERSON_FAIL_UNKNOWN:
		{
			TCHAR * pTitle = GetTxtFromMgr(227) ;
			TCHAR * pText = GetTxtFromMgr(228) ;
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_DELPERSON_FAIL_IDMATCHERROR:
		{
			TCHAR * pTitle = GetTxtFromMgr(227) ;
			TCHAR * pText = GetTxtFromMgr(229) ;
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_LOGINFAIL_LIMITMAXUSER:
		{
			TCHAR *pTitle = GetTxtFromMgr(5727);
			TCHAR *pText = GetTxtFromMgr(5728);

			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;


		//------------------------------
		// IP차단 
		//------------------------------
	case SRVAL_NOTCORRECTIP:
		{
			TCHAR * pTitle = GetTxtFromMgr(836);
			TCHAR * pText = GetTxtFromMgr(6096);
			if ( !pTitle || !pText ) break ;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

			cltClient *pclclient = (cltClient*)pclClient;

			if(pclclient->m_pDialog[ NCOMMONMSG_DLG ]==NULL)
			{
				pclclient->CreateInterface( NCOMMONMSG_DLG );
			}
			((CNCommonMsgDlg*)pclclient->m_pDialog[ NCOMMONMSG_DLG ])->Set(COMMONDLG_EXITCOUNT);

		}
		break;

		//------------------------------
		// 기술 
		//------------------------------
	case SRVAL_NOFATHERNAME:
		{
			TCHAR * pText = GetTxtFromMgr(230) ;
			if ( !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			pclMessage->SetMsg(pText);
		}
		break;
	case SRVAL_SAMEACCOUNT:
		{
			TCHAR * pText = GetTxtFromMgr(231) ;
			if ( !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			pclMessage->SetMsg(pText);
		}
		break;

	case SRVAL_SETSKILL:
		{
			TCHAR skillname[128];
			if(pclSkillManager->GetSkillName(pclreturn->siPara1, skillname) == true)
			{
				PushEffect(constEffectSuccess, 0, 0);
				TCHAR buffer[256];
				TCHAR age[64];
				StringCchPrintf(age, 64, TEXT("%d"),pclreturn->siPara2);
				TCHAR * pText = GetTxtFromMgr(232) ;
				if ( !pText ) break ;
				Nsprintf(buffer, pText, TEXT("skill"), skillname, TEXT("level"), age, NULL);
				//sprintf(buffer, pText, skillname, pclreturn->siPara2);
				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclClient->pclMessage->SetPersonalMsg(buffer);
				}
				else
				{
					pclClient->pclMessage->SetMsg(buffer);
				}

				cltCharClient* pclchar = (cltCharClient* )pclCM->CR[1];
				pclchar->SetSkillLevelUp();
				pclMusic->PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_SKILL_LEVELUP")), 0, 0);

			}
		}
		break;

	case SRVAL_DECSKILL_FAIL_HAVECHILD:
		TCHAR skillname[128];
		if(pclSkillManager->GetSkillName(pclreturn->siPara1, skillname) == true)
		{
			TCHAR * pTitle = GetTxtFromMgr(233) ;
			TCHAR * pText = GetTxtFromMgr(234) ;
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, skillname);
		}
		break;
	case SRVAL_WEAPONSKILL_FAIL_MONEY:
		{
			PushEffect(constEffectFailure, 0, 0);

			if ( m_pDialog[NHONGGILDONG_DLG] )
			{
				stHongGilDongData Data;
				Data.Type			= HONGGILDONG_SETDATATYPE_NOTENOUGHTMONEY;
				Data.CostMoney		= pclreturn->siPara1 ;
				Data.SkillLevel		= 0 ;
				Data.SkillUnique	= 0 ;

				m_pDialog[NHONGGILDONG_DLG]->SetData(&Data);
			}
		}
		break;

	case SRVAL_REJECT_SETFATHER:
		{
			PushEffect(constEffectFailure, 0, 0);

			TCHAR * pTitle = GetTxtFromMgr(10032) ;
			TCHAR * pText = GetTxtFromMgr(10034) ;
			if ( !pTitle || !pText ) break ;
			
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_ACCEPT_SETFATHER:
		{
			PushEffect(constEffectFailure, 0, 0);

			TCHAR * pTitle = GetTxtFromMgr(10032) ;
			TCHAR * pText = GetTxtFromMgr(10035) ;
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_PENALTY_SETFATHER_TEACHER:
		{
			PushEffect(constEffectFailure, 0, 0);

			TCHAR * pTitle = GetTxtFromMgr(10032) ;
			TCHAR * pText = GetTxtFromMgr(10036) ;
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_PENALTY_SETFATHER_PUPIL:
		{
			PushEffect(constEffectFailure, 0, 0);

			TCHAR * pTitle = GetTxtFromMgr(10032) ;
			TCHAR * pText = GetTxtFromMgr(10037) ;
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_SKILLNOTREADYFORMAKE:
		{
			TCHAR * pTitle = GetTxtFromMgr(235) ;
			TCHAR * pText = GetTxtFromMgr(236) ;
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_INSUFFICIENTMATERIAL:
		{
			PushEffect(constEffectFailure, 0, 0);
			TCHAR buffer[256];
			TCHAR * pTitle = GetTxtFromMgr(235) ;
			TCHAR * pText = GetTxtFromMgr(237) ;
			if ( !pTitle || !pText ) break ;
			StringCchCopy(buffer, 256, pText);
			const TCHAR* pname = pclItemManager->GetName(pclreturn->siPara1);
			if(pname)
			{
				TCHAR temp[256];
				StringCchPrintf(temp, 256, TEXT("-[%s]"), pname);
				StringCchCat(buffer, 256, temp);
			}
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
		}

		break;
	case SRVAL_INSUFFICIENTTOOL:
		{

			PushEffect(constEffectFailure, 0, 0);

			TCHAR buffer[256];
			TCHAR * pTitle = GetTxtFromMgr(235) ;
			TCHAR * pText = GetTxtFromMgr(238) ;
			if ( !pTitle || !pText ) break ;
			StringCchCopy(buffer, 256, pText);
			const TCHAR* pname = pclItemManager->GetName(pclreturn->siPara1);
			if(pname)
			{
				TCHAR temp[256];
				StringCchPrintf(temp, 256, TEXT("-[%s]"), pname);
				StringCchCat(buffer, 256, temp);
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
		}
		break;

	case SRVAL_MATMATCHNUM:
		{
			TCHAR * pTitle = GetTxtFromMgr(235) ;
			TCHAR * pText = GetTxtFromMgr(239) ;
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_NOTENOUGHTOOLDURABILITY:
		{

			PushEffect(constEffectFailure, 0, 0);

			TCHAR durability[64];

			StringCchPrintf(durability, 64, TEXT("%d"), pclreturn->siPara2);
			TCHAR * pTitle = GetTxtFromMgr(235) ;
			TCHAR * pText = GetTxtFromMgr(240) ;
			if ( !pTitle || !pText ) break ;

			const TCHAR* pname = pclItemManager->GetName(pclreturn->siPara1);
			if(pname)
			{
				Nsprintf(Nbuffer, pText, TEXT("item"), pname, TEXT("dura"), durability, NULL);
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, Nbuffer);

				//m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pname, pclreturn->siPara2);

			}
		}

		break;

	case SRVAL_TOOMANYITEMMAKE:
		{
			PushEffect(constEffectFailure, 0, 0);
			
			TCHAR limit[64];
			TCHAR trycount[64];
			TCHAR * pTitle = GetTxtFromMgr(235) ;
			TCHAR * pText = GetTxtFromMgr(241) ;
			if ( !pTitle || !pText ) break ;

			StringCchPrintf(limit, 64, TEXT("%d"),MAX_ITEM_PILE_NUMBER );
			StringCchPrintf(trycount, 64, TEXT("%d"),pclreturn->siPara1 );

			Nsprintf(Nbuffer, pText, TEXT("limit"), limit, TEXT("trycount"), trycount, NULL);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, Nbuffer);
			//m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, MAX_ITEM_PILE_NUMBER, pclreturn->siPara1);

		}

		break;

	case SRVAL_FAILPROBILITY:
		{
			TCHAR * pTitle = GetTxtFromMgr(235) ;
			TCHAR * pText = GetTxtFromMgr(242) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_MAKEFAIL")), 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_NOEMPTYAREA:
		{
			TCHAR * pTitle = GetTxtFromMgr(235) ;
			TCHAR * pText = GetTxtFromMgr(243) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle,pText);
		}
		break;
		// 사용자 러그인 허용/제한 
	case SRVAL_BLOCK_BLOCKED:
		{
			TCHAR * pTitle = GetTxtFromMgr(244) ;
			TCHAR * pText = GetTxtFromMgr(245) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectSuccess, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_BLOCK_UNBLOCKED:
		{
			TCHAR * pTitle = GetTxtFromMgr(244) ;
			TCHAR * pText = GetTxtFromMgr(246) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectSuccess, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_BLOCKUSER_BLOCKED:
		{
			TCHAR * pTitle = GetTxtFromMgr(244) ;
			TCHAR * pText = GetTxtFromMgr(247) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectSuccess, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_BLOCKUSER_UNBLOCKED:
		{
			TCHAR * pTitle = GetTxtFromMgr(244) ;
			TCHAR * pText = GetTxtFromMgr(248) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectSuccess, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_NOTENOUGHLEVELTOMAP:
		{
			TCHAR * pTitle = GetTxtFromMgr(249) ;
			TCHAR * pText = GetTxtFromMgr(250) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1);
		}
		break;
	case SRVAL_FAIL_RACCOONWORLD:
		{
			TCHAR * pTitle = GetTxtFromMgr(249) ;
			TCHAR * pText = GetTxtFromMgr(40424) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1);
		}break;
	case SRVAL_TOOHIGHLEVEL:
		{
			TCHAR * pTitle = GetTxtFromMgr(249) ;
			TCHAR * pText = GetTxtFromMgr(6113) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1);
		}
		break;

	case SRVAL_TOOHIGHLEVEL_SANTA:
		{
			TCHAR * pTitle = GetTxtFromMgr(249) ;
			TCHAR * pText = GetTxtFromMgr(6647) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1);
		}
		break;

	case SRVAL_FAIL_WARP_LEVEL_RANGE:
		{
			TCHAR * pTitle = GetTxtFromMgr(6816) ;
			TCHAR * pText = GetTxtFromMgr(6814) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1, pclreturn->siPara2);
		}
		break;

	case SRVAL_FAIL_FORBID_ENTERMAP:
		{
			TCHAR * pTitle = GetTxtFromMgr(6816) ;
			TCHAR * pText = GetTxtFromMgr(3231) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1, pclreturn->siPara2);
		}
		break;

	case SRVAL_FAIL_WARP_NOITEM:
		{
			TCHAR * pTitle = GetTxtFromMgr(6816) ;
			TCHAR * pText = GetTxtFromMgr(6815) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			const TCHAR* pszItemName = pclItemManager->GetName(pclreturn->siPara1);
			if(pszItemName != NULL)
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pszItemName, pclreturn->siPara2);
		}
		break;

	case SRVAL_FAIL_WARP_BLOCKWARP:
		{
			TCHAR * pTitle = GetTxtFromMgr(6816) ;
			TCHAR * pText = GetTxtFromMgr(6857) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_CANNOTMOVEITEM_PERSONALSHOP:
		{
			TCHAR * pTitle = GetTxtFromMgr(512) ;
			TCHAR * pText = GetTxtFromMgr(6649) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	//KHY - 0530 명나라 상인 구매제한.
	case SRVAL_CANNOTBUYITEM_FOREIGNMERCHANT:
		{
			TCHAR * pTitle = GetTxtFromMgr(512) ;
			TCHAR * pText = GetTxtFromMgr(8450) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	//----------------------------------------------------------

	case SRVAL_FAIL_WARP_LIMITTIME: // 순간이동은 3초마다 한번만
		{
			TCHAR * pTitle = GetTxtFromMgr(1413) ;
			TCHAR * pText = GetTxtFromMgr(3770) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1);
		}
		break;
	
	case SRVAL_FAIL_FORBID_BATTLE:
		{
			TCHAR * pTitle = GetTxtFromMgr(512) ;
			TCHAR * pText = GetTxtFromMgr(8296) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1);
		}
		break;
	case SRVAL_GUILDDUNGEON_FAIL_REG:		// 길드 사냥터 등록 실패
		{
			TCHAR * pTitle = GetTxtFromMgr(512) ;
			TCHAR * pText = GetTxtFromMgr(8345) ;
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1);
		}
		break;
		
	case SRVAL_NOPARTY:
		{
			TCHAR * pTitle = GetTxtFromMgr(3231) ;
			TCHAR * pText = GetTxtFromMgr(3232) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_NOINSTANCEMAP:
		{
			TCHAR * pTitle = GetTxtFromMgr(3231) ;
			TCHAR * pText = GetTxtFromMgr(3275) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_MAKE_BOSSMONSTER:
		{
			TCHAR * pTitle = GetTxtFromMgr(3271) ;
			TCHAR * pText = GetTxtFromMgr(3276) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_CLEAR_INSTANCEMAP:
		{
			TCHAR * pTitle = GetTxtFromMgr(3277) ;
			TCHAR * pText = GetTxtFromMgr(3278) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_TIMEOUT:
		{
			TCHAR * pTitle = GetTxtFromMgr(3279) ;
			TCHAR * pText = GetTxtFromMgr(3280) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

		//-------------------------------------
		// 왜구 전쟁 관련 
		//-------------------------------------
	case SRVAL_NOWARSEASON:
		{
			TCHAR* pTitle = GetTxtFromMgr(302);
			TCHAR* pText = GetTxtFromMgr(303);


			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_OUTOFWARKILL:
		{
			TCHAR* pTitle = GetTxtFromMgr(304);
			TCHAR* pText = GetTxtFromMgr(305);



			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1);
		}
		break;
	case SRVAL_AUTOWAR:
		{
			TCHAR temp[256];

			if(pclreturn->siPara1)
			{
				TCHAR* pText = GetTxtFromMgr(306);


				StringCchPrintf(temp, 256, pText);
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(307);


				StringCchPrintf(temp, 256, pText);
			}
			TCHAR* pTitle = GetTxtFromMgr(308);


			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, temp);
		}
		break;

		//-------------------------------------
		// 흑의군단 관련
		//-------------------------------------
	case SRVAL_BLACKARMY_MAPPOSITION:
		{
			TCHAR *pTitle = GetTxtFromMgr(4903);
			TCHAR *pText = GetTxtFromMgr(5862);
			TCHAR buf[256];

			SI32 gateunique = pclreturn->siPara1;

			if (gateunique > 0)
			{
				cltGate gate = pclMapManager->pclGateManager->clGate[gateunique];

				StringCchPrintf(buf, 256, pText, gate.SrcX, gate.SrcY);

				if (m_pDialog[NOTIFYMSG_DLG])
				{
					stMsgBox MsgBox;
					MsgBox.Set( m_pDialog[NOTIFYMSG_DLG], pTitle, buf, MSGBOX_TYPE_YESNO, SRVAL_BLACKARMY_MAPPOSITION );
					SetMsgBox( &MsgBox, (BYTE*)&gateunique, sizeof(SI32) );

				}

			}

		}
		break;

	case SRVAL_BLACKARMY_NOTALLOC:
		{
			TCHAR *pTitle = GetTxtFromMgr(836);
			TCHAR *pText = GetTxtFromMgr(5864);

			if (m_pDialog[NOTIFYMSG_DLG])
			{
				m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pText);
			}


		}
		break;

	case SRVAL_BLACKARMY_NOTWAR:
		{
			TCHAR *pTitle = GetTxtFromMgr(836);
			TCHAR *pText = GetTxtFromMgr(5863);

			if (m_pDialog[NOTIFYMSG_DLG])
			{
				m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pText);
			}


		}
		break;

	case SRVAL_BLACKARMY_WARAPPLY:
		{
			TCHAR *pTitle = GetTxtFromMgr(2089);
			TCHAR *pText;

			if (pclreturn->siPara1 == 1)
				pText = GetTxtFromMgr(5867);
			else
				pText = GetTxtFromMgr(5868);

			if (m_pDialog[NOTIFYMSG_DLG])
			{
				m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pText);
			}


		}
		break;

	case SRVAL_BLACKARMY_FAILALLOC:
		{
			TCHAR *pTitle = GetTxtFromMgr(5614);
			TCHAR *pText = GetTxtFromMgr(5896);

			if (m_pDialog[NOTIFYMSG_DLG])
			{
				m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pText);
			}


		}
		break;

		//-------------------------------------
		// 혹부리(2호식) 관련
		//-------------------------------------
	case SRVAL_WENNYOLDMAN_FULLINVEN:
		{
			TCHAR *pText = GetTxtFromMgr(1368);

			if (m_pDialog[NWENNYMAN_DLG])
			{
				pclClient->PushEffect( constEffectFailure, 0, 0 );
				((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG])->SetWennyText(pText);
				((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG])->ClearAutoState();
			}

		}
		break;
	case SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_UNLUCK:
		{
			TCHAR *pText = GetTxtFromMgr(5937);

			if (m_pDialog[NWENNYMAN_DLG])
			{
				pclClient->PushEffect( constEffectFailure, 0, 0 );
				((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG])->SetWennyText(pText);
			}

		}
		break;
	case SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_LOWPRICE:
		{
			TCHAR *pText = GetTxtFromMgr(5939);

			if (m_pDialog[NWENNYMAN_DLG])
			{
				pclClient->PushEffect( constEffectFailure, 0, 0 );
				((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG])->SetWennyText(pText);
			}

		}
		break;
	case SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_CANNOTITEM:
		{
			TCHAR *pText = GetTxtFromMgr(5941);

			if (m_pDialog[NWENNYMAN_DLG])
			{
				pclClient->PushEffect( constEffectFailure, 0, 0 );
				((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG])->SetWennyText(pText);
			}

		}
		break;
		// [성웅] 레벨 보상 상자에서 아이템을 준다 
	case SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_FULLINVEN:
		{
			TCHAR *pText = GetTxtFromMgr(5942);

			if (m_pDialog[NWENNYMAN_DLG])
			{
				pclClient->PushEffect( constEffectFailure, 0, 0 );
				((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG])->SetWennyText(pText);
			}

		}
		break;
		// [성웅] 레벨 보상 상자에서 아이템을 준다 
	case SRVAL_FAIL_OPEN_LEVELLIMTBOX_NOT_OVER_LEVEL:// 상자를 열기에 레벨이 충분하지 않음 
		{
			CHAR *pTitle = GetTxtFromMgr(512);
			NTCHARString256		kText(GetTxtFromMgr(40868));
			kText.Replace("#level#", SI32ToString(pclreturn->siPara1));
			if (m_pDialog[NOTIFYMSG_DLG])
			{
				m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, kText);
			}
		}
		break;
	case SRVAL_FAIL_OPEN_LEVELLIMTBOX_NOT_NOT_ENOUGH_INV:	// 상자를 열기에 빈공간이 충분 하지 않음 
		{
			CHAR *pTitle = GetTxtFromMgr(512);
			TCHAR *pText = GetTxtFromMgr(205);

			if (m_pDialog[NOTIFYMSG_DLG])
			{
				m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pText);
			}
		}
		break;

	case SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN:
		{
			TCHAR *pText = GetTxtFromMgr(6634);

			if( pclreturn->siPara1 == 0 )
			{
				if ( m_pDialog[NTREASUREBOX_DLG])
				{
					pclClient->PushEffect( constEffectFailure, 0, 0 );
					((CNTreasureBoxDlg*)m_pDialog[NTREASUREBOX_DLG])->SetErrorText(pText);
				}
			}
			else if ( pclreturn->siPara1 == 1 )
			{
				if ( m_pDialog[NOPENBIGITEMBOX_DLG])
				{
					((CNOpenBigItemBoxDlg*)m_pDialog[NOPENBIGITEMBOX_DLG])->SetResult(0, 0);
				}
			}
			else if ( pclreturn->siPara1 == 100 )
			{
				// [진성] 인벤에 아이템을 추가하지 못함을 알림창으로 띄우자.
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(GetTxtFromMgr(5614), pText);				
			}
			else if ( pclreturn->siPara1 == 200 )
			{
				// [진성] 인벤에 아이템을 추가하지 못함을 알림창으로 띄우자.
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(GetTxtFromMgr(5614), GetTxtFromMgr(1368));				
			}
		}
		break;

	case SRVAL_FEAST_FAIL_USE_ONLY_VILLAGE:
		{
			TCHAR *pTitle = GetTxtFromMgr(1078);			
			TCHAR *pText = GetTxtFromMgr(6713);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);			
		}
		break;

		//-------------------------------------
		// 왜구 전쟁 관련 
		//-------------------------------------
	case SRVAL_NOOSAKAWARSEASON:
		{
			TCHAR* pTitle = GetTxtFromMgr(302);
			TCHAR* pText = GetTxtFromMgr(5303);


			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;


		//------------------------------------
		// 고구려 유적 방어 이베트 
		//------------------------------------
	case SRVAL_GOGUWARPFAIL_NOTVILLAGE:
		{
			TCHAR* pTitle = GetTxtFromMgr(309);
			TCHAR* pText = GetTxtFromMgr(310);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_GOGUWARPFAIL_NOEVENTTIME:
		{
			TCHAR* pTitle = GetTxtFromMgr(521);
			TCHAR* pText = GetTxtFromMgr(311);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

		//------------------------------------
		// 강화도 방어 이베트 
		//------------------------------------
	case SRVAL_KANGHWADOWARPFAIL_NOTVILLAGE:
		{
			TCHAR* pTitle = GetTxtFromMgr(309);
			TCHAR* pText = GetTxtFromMgr(310);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_KANGHWADOWARPFAIL_NOEVENTTIME:
		{
			TCHAR* pTitle = GetTxtFromMgr(521);
			TCHAR* pText = GetTxtFromMgr(311);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MOFUMOFUEVENT_START_LEFTTIME:
		{
			cltClient *pclclient = (cltClient*)pclClient;

			if(pclclient->m_pDialog[ NCOMMONMSG_DLG ]==NULL)
			{
				pclclient->CreateInterface( NCOMMONMSG_DLG );
			}
			((CNCommonMsgDlg*)pclclient->m_pDialog[ NCOMMONMSG_DLG ])->Set(COMMONDLG_MOFUMOFU_AUTO_WARP);
			TCHAR* pText = GetTxtFromMgr(7066);
			TCHAR  Msg[256];
			StringCchPrintf(Msg, 256, pText, pclreturn->siPara2);;
			((CNCommonMsgDlg*)pclclient->m_pDialog[ NCOMMONMSG_DLG ])->SetText(Msg);
		}
		break;

	case SRVAL_MOFUMOFUEVENT_FAIL_RESERVE:
		{
			TCHAR* pTitle = GetTxtFromMgr(6694);
			TCHAR* pText = GetTxtFromMgr(7073);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MOFUMOFUEVENT_SUCCESS_SETENEMYCOUNT:
		{
			TCHAR* pTitle = GetTxtFromMgr(10076);
			TCHAR* pText = GetTxtFromMgr(10077);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1 );
		}
		break;

	case SRVAL_MOFUMOFUEVENT_FAIL_SETENEMYCOUNT:
		{
			TCHAR* pTitle = GetTxtFromMgr(10076);
			TCHAR* pText = NULL;

			if ( pclreturn->siPara1 == 0 )
			{
				pText = GetTxtFromMgr( 10078 );
			}
			else
			{
				pText = GetTxtFromMgr( 10079 );
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;

	case SRVAL_PIRATESHIP_APPEAR:
		{
			NTCHARString64	kMessage = GetTxtFromMgr( 7983 );

			if( pclClient->pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL )
			{
				kMessage += "(X:";
				kMessage += SI32ToString(pclreturn->siPara1);
				kMessage += "Y:";
				kMessage += SI32ToString(pclreturn->siPara2);
				kMessage += ")";
			}

			pclMessage->SetMsg( kMessage, 0, RGB(255, 255, 0) );
		}
		break;

	case SRVAL_WHISPERCHAT_REJECT:
		{
			TCHAR* pTitle = GetTxtFromMgr(312);
			TCHAR* pText = GetTxtFromMgr(313);

			PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_NO_WHISPERCHAT")), 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRIVATETRADE_REJECT:
		{
			TCHAR* pTitle = GetTxtFromMgr(314);
			TCHAR* pText = GetTxtFromMgr(315);

			PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_NO_PRIVATETRADE")), 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}

		break;
	case SRVAL_PARTYINVITE_REJECT:
		{
			TCHAR* pTitle = GetTxtFromMgr(316);
			TCHAR* pText = GetTxtFromMgr(317);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FRIENDADD_REJECT:
		{
			TCHAR* pTitle = GetTxtFromMgr(318);
			TCHAR* pText = GetTxtFromMgr(319);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;
	
	case SRVAL_EVENTCHAR:
		{
			TCHAR temp[256];
			TCHAR* pTitle = GetTxtFromMgr(320);

			StringCchPrintf(temp, 256, pTitle, pclreturn->siPara1);


			if(pclreturn->siPara2)
			{
				TCHAR* pText = GetTxtFromMgr(321);



				m_pDialog[ NOTIFYMSG_DLG ]->SetText(temp, pText);

			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(322);



				m_pDialog[ NOTIFYMSG_DLG ]->SetText(temp, pText);
			}
		}
		break;

	case SRVAL_EVENT:
		{
			TCHAR temp[256];
			TCHAR* pTitle = GetTxtFromMgr(320);

			StringCchPrintf(temp, 256, pTitle, pclreturn->siPara1);


			if(pclreturn->siPara2)
			{
				TCHAR* pText = GetTxtFromMgr(323);


				m_pDialog[ NOTIFYMSG_DLG ]->SetText(temp, pText);

			}
			else
			{TCHAR* pText = GetTxtFromMgr(324);


			m_pDialog[ NOTIFYMSG_DLG ]->SetText(temp, pText);
			}
		}
		break;

	case SRVAL_LOTTOFAIL:
		{
			TCHAR* pTitle = GetTxtFromMgr(325);
			TCHAR* pText = GetTxtFromMgr(326);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_LEVELUPFAIL_NOCLEARQUEST:
		{
			SI32 npckind = pclQuestManager->GetNPCKindFromQuestType(pclreturn->siPara1);
			if(npckind)
			{
				const TCHAR* pname = pclKindInfoSet->GetName(npckind);
				if(pname)
				{
					TCHAR buffer[256];
					TCHAR* pText = GetTxtFromMgr(327);

					StringCchPrintf(buffer, 256, pText, pname);
					pclMessage->SetMsg(buffer);
				}
			}
		}
		break;


	case SRVAL_PUCHIKOMI_FAIL_OP_REJECTION:
		{
			TCHAR* pTitle = GetTxtFromMgr( 3921 );
			TCHAR* pText = GetTxtFromMgr( 6682 );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;


		//---------------------------------------------
		// 운영자 명령 
		//----------------------------------------------
	case SRVAL_INVALIDGMORDER:
		{
			TCHAR* pTitle = GetTxtFromMgr(328);
			TCHAR* pText = GetTxtFromMgr(329);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			PushEffect(constEffectFailure, 0, 0);
		}
		break;

		// GAMEMASTER
	case SRVAL_GAMEMASTER_VILLAGEWAR:
		{
			TCHAR buffer[256 ] = TEXT("");
			if(pclreturn->siPara1 == 1)
			{
				StringCchPrintf(buffer, 256, "[MASTER] Village War Enabled");
			}
			else
			{
				StringCchPrintf(buffer, 256, "[MASTER] Village War Disabled");
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText("", buffer);
		}
		break;

		// GAMEMASTER
	case SRVAL_GAMEMASTER_GOONZUDAY:
		{
			TCHAR buffer[256 ] = TEXT("");
			if(pclreturn->siPara1 == 1)
			{
				StringCchPrintf(buffer, 256, "[MASTER] GoonzuDay Enabled");
			}
			else
			{
				StringCchPrintf(buffer, 256, "[MASTER] GoonzuDay Disabled");
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText("", buffer);
		}
		break;

	case SRVAL_GAMEMASTER_MAKETIMEREDUCE:
		{
			TCHAR buffer[256 ] = TEXT("");
			if(pclreturn->siPara1 == 1)
			{
				StringCchPrintf(buffer, 256, "[MASTER] MakeTimeReduce Enabled");
			}
			else
			{
				StringCchPrintf(buffer, 256, "[MASTER] MakeTimeReduce Disabled");
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText("", buffer);
		}
		break;

	case SRVAL_GAMEMASTER_QUESTCOUNT:
		{
			NTCHARString128 kMessage( TEXT("[#count#] Set QuestCount") );


			kMessage.Replace( TEXT("#count#"), SI32ToString(pclreturn->siPara1) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( TEXT(""), kMessage);
		}
		break;

	case SRVAL_GAMEMASTER_FOURLEAF:
		{
			NTCHARString128 kMessage( TEXT("FourLeaf Unique is [#unique#]") );

			kMessage.Replace( TEXT("#unique#"), SI32ToString(pclreturn->siPara1) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( TEXT(""), kMessage);
		}
		break;

	case SRVAL_GAMEMASTER_VOTECAMPAGIN:
		{
			TCHAR buffer[256 ] = TEXT("");
			if(pclreturn->siPara1 == 1)
			{
				StringCchPrintf(buffer, 256, "[MASTER] VoteCampagin Enabled");
			}
			else
			{
				StringCchPrintf(buffer, 256, "[MASTER] VoteCampagin Disabled");
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText("", buffer);
		}
		break;

	case SRVAL_GAMEMASTER_POSTOFFICE_NOPERSON:
		{
			NTCHARString256		kText("Unknown CharName");

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( TEXT("PostOffice"), kText);
		}
		break;

	case SRVAL_GAMEMASTER_POSTOFFICE_NOTENOUGHBOX:
		{
			NTCHARString256		kText("Not Enough MailBox");

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( TEXT("PostOffice"), kText);
		}
		break;

	case SRVAL_GAMEMASTER_POSTOFFICE_SYSTEMERROR:
		{
			NTCHARString256		kText("SYSTEM ERROR");

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( TEXT("PostOffice"), kText);
		}
		break;

	case SRVAL_GAMEMASTER_SYSTEM_PACKET_ENABLE:
		{
			NTCHARString256		kText("Packet Cmd:#number# is #enable#.");

			kText.Replace("#number#", SI32ToString(pclreturn->siPara1));
			if( pclreturn->siPara2 == 1)
				kText.Replace("#enable#", "Enabled");
			else
				kText.Replace("#enable#", "Disabled");

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( TEXT("SYSTEM"), kText);
		}
		break;

	case SRVAL_GAMEMASTER_BLOCK_PACKET:
		{			
			NTCHARString256		kText(GetTxtFromMgr(9595));
			NTCHARString256		kTitle(GetTxtFromMgr(5614));

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( kTitle, kText);
		}
		break;

/*	case SRVAL_GAMEMASTER_SETMUTETIME:
		{
			NTCHARString256		kText;
			NTCHARString256		kTitle(GetTxtFromMgr(5614));

			// 시간 설정
			pclClient->pclCM->GetCharClient(1)->m_siMuteTime = pclreturn->siPara1;
			SI32 siMinute = pclClient->pclCM->GetCharClient(1)->m_siMuteTime - clClientDate.GetMinuteVary();

			if(pclreturn->siPara1 > 0)
			{
				kText = GetTxtFromMgr(9626);
				kText.Replace("#minute#", SI32ToString(siMinute));
			}
			else
			{
				kText = GetTxtFromMgr(9627);
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( kTitle, kText);
		}
		break;*/
	case SRVAL_SUCCESSASK:
		{
			TCHAR* pTitle = GetTxtFromMgr(330);
			TCHAR* pText = GetTxtFromMgr(331);

			PushEffect(constEffectSuccess, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_INPUTBIDMONEY:
		{
			TCHAR* pTitle = GetTxtFromMgr(332);
			TCHAR* pText = GetTxtFromMgr(333);

			PushEffect(constEffectSuccess, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_ENCHANTFAILTOOMUCH:
		{
			PushEffect(constEffectFailure, 0, 0);

			TCHAR* pTitle = NULL;
			TCHAR* pText = NULL;

			pTitle = GetTxtFromMgr(334);
			pText = GetTxtFromMgr(335);

			if(pclreturn->siPara2 == 0)	 m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1);

			pTitle = GetTxtFromMgr(334);
			pText = GetTxtFromMgr(336);

			if(pclreturn->siPara2 == 1)	 m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText , pclreturn->siPara1);

			pTitle = GetTxtFromMgr(334);
			pText = GetTxtFromMgr(337);

			if(pclreturn->siPara2 == 2)	 m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText , pclreturn->siPara1*10);
		}
		break;
	case SRVAL_ENCHANTFAILNOTATBMATCH:
		{
			TCHAR* pTitle = GetTxtFromMgr(334);
			TCHAR* pText = GetTxtFromMgr(338);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);		

		}
		break;
	case SRVAL_ENCHANTFAILTOOMUCHKIND:
		{
			TCHAR* pTitle = GetTxtFromMgr(334);
			TCHAR* pText = GetTxtFromMgr(339);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_NOLISTINFATHERAPPLYER:
		{
			TCHAR* pTitle = GetTxtFromMgr(340);
			TCHAR* pText = GetTxtFromMgr(341);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_GUILD_FAIL_NOTCONNECTMASTER: // 길드장 부재 신고
		{
			TCHAR* pTitle = GetTxtFromMgr(404);
			TCHAR* pText = GetTxtFromMgr(15605);
			PushEffect(constEffectSuccess, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUCCESSAPPLYFATHER:
		{
			TCHAR* pTitle = GetTxtFromMgr(342);
			TCHAR* pText = GetTxtFromMgr(343);

			PushEffect(constEffectSuccess, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

			pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_REQUESTMENTORSUCCESS );
		}
		break;
	case SRVAL_FAILAPPLYFATHER_ALREADY:
		{
			TCHAR* pTitle = GetTxtFromMgr(344);
			TCHAR* pText = GetTxtFromMgr(345);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

			pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_REQUESTMENTORSUCCESS );
		}
		break;
	case SRVAL_FAILAPPLYFATHER_FULL:
		{
			TCHAR* pTitle = GetTxtFromMgr(344);
			TCHAR* pText = GetTxtFromMgr(346);


			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUCCESSDELFATHERAPPLY:
		{
			TCHAR* pTitle = GetTxtFromMgr(347);
			TCHAR* pText = GetTxtFromMgr(348);



			PushEffect(constEffectSuccess, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HUNT_FAILENTER_LEVEL:
		{
			TCHAR* pTitle = GetTxtFromMgr(349);
			TCHAR* pText = GetTxtFromMgr(350);



			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara1);
		}
		break;
	case SRVAL_HUNT_FAILENTER_NOFEE:
		{
			TCHAR* pTitle = GetTxtFromMgr(349);
			TCHAR* pText = GetTxtFromMgr(351);



			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_INITSTATUS:
		{
			PushEffect(constEffectSuccess, 0, 0);
			
			if(pclreturn->siPara1 == INITSTATUSMODE_LEVEL_10)
			{
				TCHAR* pTitle = GetTxtFromMgr(352);
				TCHAR* pText = GetTxtFromMgr(353);

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);	
				
				/* 
				if(m_pDialog[ NNEWQUEST_DLG ] == NULL)
				{
				CreateInterface( NNEWQUEST_DLG );

				if(m_pDialog[ NNEWQUEST_DLG ])
				{
				pclQuestManager->bNewQuseOpened = TRUE;
				((CNNewQuestDlg*)m_pDialog[ NNEWQUEST_DLG ])->m_siButtonType = 0;
				((CNNewQuestDlg*)m_pDialog[ NNEWQUEST_DLG ])->SetNewQuest(pclClient->GetUniqueFromHash( TEXT("KIND_JANGYOUNGSIL"))); 
				((CNNewQuestDlg*)m_pDialog[ NNEWQUEST_DLG ])->SetOpenedQuestDlg( true );
				}
				}
				else
				{
				if(m_pDialog[ NNEWQUEST_DLG ])
				{
				pclQuestManager->bNewQuseOpened = TRUE;
				((CNNewQuestDlg*)m_pDialog[ NNEWQUEST_DLG ])->m_siButtonType = 0;
				((CNNewQuestDlg*)m_pDialog[ NNEWQUEST_DLG ])->SetNewQuest(pclClient->GetUniqueFromHash( TEXT("KIND_JANGYOUNGSIL")));
				((CNNewQuestDlg*)m_pDialog[ NNEWQUEST_DLG ])->SetOpenedQuestDlg( true );
				}
				}*/

			}
			else if(pclreturn->siPara1 == INITSTATUSMODE_LEVEL_30)
			{
				TCHAR* pTitle = GetTxtFromMgr(352);
				TCHAR* pText = GetTxtFromMgr(6916);

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);	
			}
			else if ( pclreturn->siPara1 == INITSTATUSMODE_TICKET )
			{
				TCHAR* pTitle = GetTxtFromMgr(354);
				TCHAR* pText = GetTxtFromMgr(355);

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			}

		}
		break;

	case SRVAL_HUNT_NOTENOUGHREWARD:
		{
			TCHAR* pTitle = GetTxtFromMgr(349);
			TCHAR* pText = GetTxtFromMgr(356);



			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HUNT_NOTENOUGHMON:
		{
			TCHAR* pTitle = GetTxtFromMgr(349);
			TCHAR* pText = GetTxtFromMgr(357);



			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HUNT_TIMEOUT:
		{
			TCHAR* pTitle = GetTxtFromMgr(358);
			TCHAR* pText = GetTxtFromMgr(359);



			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_WAR_CANTENTERENEMYCASTLE:
		{
			TCHAR* pTitle = GetTxtFromMgr(360);
			TCHAR* pText = GetTxtFromMgr(361);



			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRIVATETRADE_MONEY_INSUFFICIENCY:
		{
			TCHAR* pTitle = GetTxtFromMgr(362);
			TCHAR* pText = GetTxtFromMgr(363);



			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRIVATETRADE_ITEM_INFO_DIFFERENT:
		{
			TCHAR* pTitle = GetTxtFromMgr(364);
			TCHAR* pText = GetTxtFromMgr(365);



			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRIVATETRADE_PERSON_INVENTORY_FULL:
		{
			TCHAR* pTitle = GetTxtFromMgr(4661);
			TCHAR* pText = GetTxtFromMgr(367);




			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRIVATETRADE_STOCK_INFO_DIFFERENT:	
		{
			TCHAR* pTitle = GetTxtFromMgr(364);
			TCHAR* pText = GetTxtFromMgr(365);




			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRIVATETRADE_HOUSEUNIT_INFO_DIFFERENT:
		{
			TCHAR* pTitle = GetTxtFromMgr(370);
			TCHAR* pText = GetTxtFromMgr(371);




			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRIVATETRADE_HORSE_INFO_DIFFERENT:
		{
			TCHAR* pTitle = GetTxtFromMgr(372);
			TCHAR* pText = GetTxtFromMgr(373);



			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRIVATETRADE_HORSE_INVENTORY_NOTEMPTY:
		{
			TCHAR* pTitle = GetTxtFromMgr(372);
			TCHAR* pText = GetTxtFromMgr(374);



			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRIVATETRADE_HORSE_NAME_SAME:
		{
			TCHAR* pTitle = GetTxtFromMgr(372);
			TCHAR* pText = GetTxtFromMgr(375);



			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
		}
		break;
	case SRVAL_PRIVATETRADE_HOUSEUNIT_LEVEL_LIMIT:
		{
			TCHAR* pTitle = GetTxtFromMgr(376);
			TCHAR* pText = GetTxtFromMgr(377);



			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, LEVELLIMIT_USE_HOUSE );
		}
		break;
	case SRVAL_PRIVATETRADE_HOUSEUNIT_EXCEED_NUMBER_PER_PERSON:
		{
			TCHAR* pTitle = GetTxtFromMgr(376);
			TCHAR* pText = GetTxtFromMgr(378);



			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRIVATETRADE_STOCK_TOOMANY:
		{
			TCHAR* pTitle = GetTxtFromMgr(368);
			TCHAR* pText = GetTxtFromMgr(379);



			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRIVATETRADE_HORSE_NOTEMPTY:
		{
			TCHAR* pTitle = GetTxtFromMgr(314);
			TCHAR* pText = GetTxtFromMgr(380);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
		}
		break;
	case SRVAL_PRIVATETRADe_HORSE_CANNOTTRADE:
		{
			TCHAR* pTitle = GetTxtFromMgr(314);
			TCHAR* pText = GetTxtFromMgr(7043);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
		}
		break;
	case SRVAL_VOTE_INIT:
		{
			TCHAR * pTitle = GetTxtFromMgr(390) ;
			TCHAR * pText = GetTxtFromMgr(391) ;
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FRIEND_NOTCONNECTED:
		{
			TCHAR * pTitle = GetTxtFromMgr(318) ;
			TCHAR * pText = GetTxtFromMgr(392) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_FAIL_SETIZOINFO:
		{
			TCHAR * pTitle = GetTxtFromMgr(393) ;
			TCHAR * pText = GetTxtFromMgr(394);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_FAIL_SETHOZOINFO:		
		{
			TCHAR * pTitle = GetTxtFromMgr(393) ;
			TCHAR * pText = GetTxtFromMgr(395);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_FAIL_SETYEZOINFO:		
		{
			TCHAR * pTitle = GetTxtFromMgr(393) ;
			TCHAR * pText = GetTxtFromMgr(396);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_FAIL_SETBYUNGZOINFO:	
		{
			TCHAR * pTitle = GetTxtFromMgr(393) ;
			TCHAR * pText = GetTxtFromMgr(397);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_FAIL_SETHYUNGZOINFO:	
		{
			TCHAR * pTitle = GetTxtFromMgr(393) ;
			TCHAR * pText = GetTxtFromMgr(398);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_FAIL_SETGONGZOINFO	:
		{
			TCHAR * pTitle = GetTxtFromMgr(393) ;
			TCHAR * pText = GetTxtFromMgr(399);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_FAIL_SETGOONZUINFO:
		{
			TCHAR * pTitle = GetTxtFromMgr(393) ;
			TCHAR * pText = GetTxtFromMgr(400);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_SUCCESS_GETMINISTER:
		{
			TCHAR * pTitle = GetTxtFromMgr(401) ;
			TCHAR * pText = GetTxtFromMgr(402);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectSuccess,0,0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_FAIL_SETHYUNGZOPRISON:
		{
			TCHAR * pTitle = GetTxtFromMgr(393) ;
			TCHAR * pText = GetTxtFromMgr(403);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_FAIL_SETMINISTER:
		{
			TCHAR * pTitle = GetTxtFromMgr(404) ;
			TCHAR * pText = GetTxtFromMgr(406);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_MINISTER_SUCCESS_SETMINISTER:
		{
			TCHAR * pTitle = GetTxtFromMgr(405) ;
			TCHAR * pText = GetTxtFromMgr(407);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	//KHY - 0629 - 해임 성공 통지 추가.	
	case SRVAL_MINISTER_FIRE_SUCCESS_SETMINISTER:
		{
			TCHAR * pTitle = GetTxtFromMgr(7535) ;
			TCHAR * pText = GetTxtFromMgr(7536);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_MINISTER_SUCCESS_SETLIQUIDATION:
		{
			TCHAR * pTitle = GetTxtFromMgr(6058) ;
			TCHAR * pText = GetTxtFromMgr(6059) ;
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_FAIL_SETLIQUIDATION:
		{
			TCHAR * pTitle = GetTxtFromMgr(6058) ;
			TCHAR * pText = GetTxtFromMgr(6060) ;
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_SUCCESS_SETBONUS:
		{
			TCHAR * pTitle = GetTxtFromMgr(6922) ;
			TCHAR * pText = GetTxtFromMgr(6923) ;
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_MINISTER_FAIL_SETBONUS:
		{
			TCHAR * pTitle = GetTxtFromMgr(6922) ;
			TCHAR * pText = GetTxtFromMgr(6924) ;
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;


	case SRVAL_MINISTER_FIRE_MINISTER:
		pclCM->CR[1]->pclCI->clBI.uiGRank = 0 ;
		break;
	case SRVAL_MINISTER_APPOINTMENT_SETMINISTER:
		pclCM->CR[1]->pclCI->clBI.uiGRank = pclreturn->siPara1 ;
		break;
	case SRVAL_MINISTER_FAIL_SETMINISTER_DATE:
		{
			TCHAR * pTitle = GetTxtFromMgr(4958) ;
			TCHAR * pText = GetTxtFromMgr(5513);
			if ( !pTitle || !pText ) break ;
			TCHAR buffer[256] = TEXT("") ;
			StringCchPrintf(buffer, 256, pText,pclreturn->siPara1);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
		}
		break;
		// 형조판서 및 운영자 - 감옥기능
	case SRVAL_PRISON_FAIL_NAMEERROR:
		{
			TCHAR * pTitle = GetTxtFromMgr(408) ;
			TCHAR * pText = GetTxtFromMgr(409);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_PRISON_FAIL_TIMEERROR:
		{
			TCHAR Buf[256] = TEXT("") ;
			TCHAR * pTitle = GetTxtFromMgr(411) ;
			TCHAR * pText = GetTxtFromMgr(410);
			if ( !pTitle || !pText ) break ;

			StringCchPrintf(Buf, 256, pText,MAX_PRISON_SECOND);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle,Buf);
		}
		break;
	case SRVAL_PRISON_FAIL_REASONERROR:
		{
			TCHAR Buf[256] = TEXT("") ;
			TCHAR * pTitle = GetTxtFromMgr(413) ;
			TCHAR * pText = GetTxtFromMgr(412);
			if ( !pTitle || !pText ) break ;
			StringCchPrintf(Buf, 256, pText,MAX_PRISON_REASON_SIZE);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle,Buf);
		}
		break;
	case SRVAL_PRISON_SUCCESS:
		{
			TCHAR * pTitle = GetTxtFromMgr(414) ;
			TCHAR * pText = GetTxtFromMgr(415);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

#ifdef _DEBUG
			/*			if( m_pDialog[ NHYUNGZOBASE_DLG ] )
			{
			m_pDialog[ NHYUNGZOBASE_DLG ]->DeleteDialog();
			}
			CreateInterface( NHYUNGZOBASE_DLG );*/
#else
			//	if( m_pDialog[ NHYUNGZOBASE_DLG ] )
			//	{
			//		CreateInterface( NHYUNGZOBASE_DLG );
			//	}
			//	CreateInterface( NHYUNGZOBASE_DLG );
			/*	if(g_SoundOnOffDlg.m_bFullScreenMode)
			{
			if( m_pDialog[ NHYUNGZOBASE_DLG ] )
			{
			CreateInterface( NHYUNGZOBASE_DLG );
			CreateInterface( NHYUNGZOBASE_DLG );
			}
			}
			else
			{
			if ( IsWindow(m_pHyungzoDlg->GetDlgHwnd()) )
			{
			// 귀찮으니 그냥 한번 닫았다가 열어서 업데이트시킴--;;;;
			m_pHyungzoDlg->Set();
			m_pHyungzoDlg->Set();
			}
			}*/
#endif
			/*			if ( IsWindow(m_pHyungzoDlg->GetDlgHwnd()) )
			{
			// 귀찮으니 그냥 한번 닫았다가 열어서 업데이트시킴--;;;;
			m_pHyungzoDlg->Set();
			m_pHyungzoDlg->Set();
			}*/
		}
		break;
	case SRVAL_AUTOHUNTING_FAIL_NAMEERROR:
		{
			TCHAR * pTitle = GetTxtFromMgr(416) ;
			TCHAR * pText = GetTxtFromMgr(417);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_AUTOHUNTING_FAIL_NOTHUNTING:
		{
			TCHAR buffer[256] = TEXT("");
			TCHAR * pTitle = GetTxtFromMgr(419) ;
			TCHAR * pText = GetTxtFromMgr(418);
			if ( !pTitle || !pText ) break ;

			StringCchPrintf(buffer, 256, pText);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle,buffer);
		}
		break;
	case SRVAL_AUTOHUNTING_FAIL_NOSUSPICION:
		{
			TCHAR buffer[256] = TEXT("");
			TCHAR * pTitle = GetTxtFromMgr(421) ;
			TCHAR * pText = GetTxtFromMgr(420);
			if ( !pTitle || !pText ) break ;

			StringCchPrintf(buffer, 256, pText, NOSUSPICTION_PRISON_TIME);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle,buffer);
#ifdef _DEBUG
			if( m_pDialog[ NHYUNGZOBASE_DLG ] )
				CreateInterface( NHYUNGZOBASE_DLG );
#else
			//	if( m_pDialog[ NHYUNGZOBASE_DLG ] )
			//		CreateInterface( NHYUNGZOBASE_DLG );
			/*	if(g_SoundOnOffDlg.m_bFullScreenMode)
			{
			if( m_pDialog[ NHYUNGZOBASE_DLG ] )
			CreateInterface( NHYUNGZOBASE_DLG );
			}
			else
			{
			if ( IsWindow(m_pHyungzoDlg->GetDlgHwnd()) )
			m_pHyungzoDlg->Set();
			}*/
#endif
			//			if ( IsWindow(m_pHyungzoDlg->GetDlgHwnd()) )
			//				m_pHyungzoDlg->Set();
		}
		break;
	case SRVAL_AUTOHUNTING_FAIL_SERVERERROR:
		{
			TCHAR * pTitle = GetTxtFromMgr(422) ;
			TCHAR * pText = GetTxtFromMgr(423);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_AUTOHUNTING_FAIL_ISREQUEST:
		{
			TCHAR * pTitle = GetTxtFromMgr(416) ;
			TCHAR * pText = GetTxtFromMgr(424);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_AUTOHUNTING_SUCCESS_REQUEST:
		{
			TCHAR buffer[256] = TEXT("");
			TCHAR * pTitle = GetTxtFromMgr(426) ;
			TCHAR * pText = GetTxtFromMgr(425);
			if ( !pTitle || !pText ) break ;

			StringCchPrintf(buffer, 256, pText, NOSUSPICTION_PRISON_TIME);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle,buffer);
		}
		break;
	case SRVAL_AUTOHUNTING_SUCCESS_PRISON:
		{
			TCHAR buffer[256] = TEXT("") ;
			TCHAR min[64]=TEXT("");
			TCHAR exp[64]=TEXT("");
			TCHAR * pTitle = GetTxtFromMgr(426) ;
			TCHAR * pText = GetTxtFromMgr(427);
			StringCchPrintf(exp, 64, TEXT("%d"), pclreturn->siPara1);
			StringCchPrintf(min, 64, TEXT("%d"), pclreturn->siPara2);
			//Nsprintf(pText,TEXT("exp"), exp   ,TEXT("min"), min  ,NULL);



			if ( !pTitle || !pText ) break ;

			//sprintf(buffer,pText,	pclreturn->siPara1 , pclreturn->siPara2 ) ;
			Nsprintf(Nbuffer,pText,TEXT("min"),min,TEXT("exp"),exp,NULL);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle,Nbuffer);
		}
		break;
	case SRVAL_HOZO_FAIL_SETANGELAPPLYER:
		{
			TCHAR * pTitle = GetTxtFromMgr(428) ;
			TCHAR * pText = GetTxtFromMgr(429);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HOZO_FAIL_SETANGEL:
		{
			TCHAR * pTitle = GetTxtFromMgr(404) ;
			TCHAR * pText = GetTxtFromMgr(430);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HOZO_SUCCESS_SETANGELAPPLYER:
		{
			TCHAR * pTitle = GetTxtFromMgr(431) ;
			TCHAR * pText = GetTxtFromMgr(432);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HOZO_SUCCESS_SETANGEL:
		{
			TCHAR * pTitle = GetTxtFromMgr(405) ;
			TCHAR * pText = GetTxtFromMgr(407);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HOZO_SUCCESS_SETANGELAPPLYER_FALSE:
		{
			TCHAR * pTitle = GetTxtFromMgr(433) ;
			TCHAR * pText = GetTxtFromMgr(434);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HOZO_SUCCESS_SETANGELADMIN_FALSE:
		{
			TCHAR * pTitle = GetTxtFromMgr(435) ;
			TCHAR * pText = GetTxtFromMgr(436);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HOZO_SUCCESS_SETANGEL_FALSE_SELF:
		{
			TCHAR * pTitle = GetTxtFromMgr(437) ;
			TCHAR * pText = GetTxtFromMgr(438);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HOZO_FAIL_SETANGELAPPLYER_FALSE:
		{
			TCHAR * pTitle = GetTxtFromMgr(439) ;
			TCHAR * pText = GetTxtFromMgr(440);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HYUNGZO_SUCCESS_SETPOLICE:
		{
			TCHAR * pTitle = GetTxtFromMgr(405) ;
			TCHAR * pText = GetTxtFromMgr(407);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_HYUNGZO_SUCCESS_SETPOLICE_FALSE:
		{
			TCHAR * pTitle = GetTxtFromMgr(435) ;
			TCHAR * pText = GetTxtFromMgr(436);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
		// -------------------------------------------
		// ItemMall
		//--------------------------------------------
	case SRVAL_ITEMMALL_FAIL_DBFAIL:
		{
			TCHAR * pTitle = GetTxtFromMgr(4314);
			TCHAR * pText = GetTxtFromMgr(441);
			if ( !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(NULL, pText);
			//pclItemMallManager->SetGetItemInfoTxt(pText);
		}
		break;
	case SRVAL_ITEMMALL_FAIL_GETITEM_NOTINV:
		{
			TCHAR * pTitle = GetTxtFromMgr(4314);
			TCHAR * pText = GetTxtFromMgr(442);
			if ( !pText ) break ;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(NULL, pText);
			m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
		}
		break;
	case SRVAL_ITEMMALL_FAIL_GETITEM_INFOERROR:
		{
			TCHAR * pTitle = GetTxtFromMgr(4314);
			TCHAR * pText = GetTxtFromMgr(443);
			if ( !pText ) break ;

			TCHAR buffer[512] = TEXT("") ;
			StringCchPrintf(buffer, 512, pText,pclreturn->siPara1);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
			//pclItemMallManager->SetGetItemInfoTxt(buffer);
		}
		break;
	case SRVAL_ITEMMALL_FAIL_CONTINUERENEWAL:
		{
			TCHAR * pTitle = GetTxtFromMgr(4314);
			TCHAR * pText = GetTxtFromMgr(444);
			if ( !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			//pclItemMallManager->SetGetItemInfoTxt(pText);
		}
		break;
	case SRVAL_ITEMMALL_FAIL_CHADEAD:
		{
			TCHAR * pTitle = GetTxtFromMgr(4314);
			TCHAR * pText = GetTxtFromMgr(445);
			if ( !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			//pclItemMallManager->SetGetItemInfoTxt(pText);
		}
		break;
	case SRVAL_ITEMMALL_FAIL_GETITEM_NOITEM:
		{
			TCHAR * pTitle = GetTxtFromMgr(4314);
			TCHAR * pText = GetTxtFromMgr(446);
			if ( !pText ) break ;
			TCHAR buffer[512] = TEXT("") ;
			StringCchPrintf(buffer, 512, pText,pclreturn->siPara1);
			//pclItemMallManager->SetGetItemInfoTxt(buffer);
			if(m_pDialog[ NITEMMALL_DLG])
			{
				((CNItemMallManager1*)m_pDialog[ NITEMMALL_DLG])->SetGetItemInfoTxt(buffer);
			}
		}
		break;
	case SRVAL_ITEMMALL_FAIL_ONLYGROWNUP:
		{
			TCHAR * pTitle = GetTxtFromMgr(3728);
			//TCHAR * pText = GetTxtFromMgr(7712);
			//if ( !pText ) break ;
			NTCHARString128 kText(GetTxtFromMgr(7712));

			if( pclClient->IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_EUROPE) )
			{
				if( pclClient->IsCountrySwitch(Switch_GlobalItemMall_Limit) )
				{
					kText = GetTxtFromMgr(8890);
					kText.Replace("#level", SI16ToString(pclreturn->siPara1));
				}
			}

			stMsgBox MsgBox;
			MsgBox.Set( m_pDialog[ NITEMMALL_DLG],  pTitle, kText, MSGBOX_TYPE_OK, 10000 );					
			SetMsgBox( &MsgBox, NULL, 0 );	
		}
		break;
	case SRVAL_ITEMMALL_FAIL_EQUIPUNABLE:
		{
			if(pclreturn->siPara1 == 1)			
			{
				TCHAR * pText = GetTxtFromMgr(6355);			
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(NULL, pText);
			}
			else if( pclreturn->siPara1 == 2 )
			{
				TCHAR * pText = GetTxtFromMgr(6356);			
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(NULL, pText);
			}
		}
		break;
	case SRVAL_ITEMMALL_FAIL_CANNOT_SERVER:
		{
			TCHAR * pTitle = GetTxtFromMgr(4314);
			TCHAR * pText = GetTxtFromMgr(447);
			if ( !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			//pclItemMallManager->SetGetItemInfoTxt(pText);
		}
		break;
	case SRVAL_ITEMMALL_FAIL_NOADDITEM:
		{
			TCHAR * pTitle = GetTxtFromMgr(4314);
			TCHAR * pText = GetTxtFromMgr(3270);
			if ( !pText ) break ;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			//pclItemMallManager->SetGetItemInfoTxt(pText);
		}
		break;
	case SRVAL_ITEMMALL_FAIL_BADREQUEST:
		{
			TCHAR * pTitle = GetTxtFromMgr(4314);
			TCHAR * pText = GetTxtFromMgr(6076);
			if ( !pText ) break ;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
			//pclItemMallManager->SetGetItemInfoTxt(pText);
		}
		break;
	case SRVAL_ITEMMALL_FAIL_LIMIT_LEVEL:
		{
			TCHAR* pTitle = GetTxtFromMgr(4314);
			TCHAR* pText  = GetTxtFromMgr(8766);
			TCHAR  szTextBuffer[256] = "";
			StringCchPrintf( szTextBuffer, sizeof(szTextBuffer), pText, pclreturn->siPara1 );
						
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, szTextBuffer);
			m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
		}
		break;
		




	case SRVAL_HORSEDIE	:
		{
			TCHAR * pTitle = GetTxtFromMgr(372) ;
			TCHAR * pText = GetTxtFromMgr(448);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

			pclCM->CR[1]->UnrideHorse() ;
		}
		break;
	case SRVAL_SUMMONDIE:
		{
			TCHAR * pTitle = GetTxtFromMgr(449) ;
			TCHAR * pText = GetTxtFromMgr(450);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMONSUCCESS:
		{
			TCHAR * pTitle = GetTxtFromMgr(449) ;
			TCHAR * pText = GetTxtFromMgr(6441);
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectSuccess, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMONHATCHFAIL:
		{
			TCHAR * pTitle = GetTxtFromMgr(449) ;
			TCHAR * pText = GetTxtFromMgr(451);
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_FAIL_OVERSLOT:
		{
			TCHAR * pTitle = GetTxtFromMgr(449) ;
			TCHAR * pText = GetTxtFromMgr(8772);
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_FAIL_LOTSOFDATE:
		{
			TCHAR * pTitle = GetTxtFromMgr(449) ;
			TCHAR * pText = GetTxtFromMgr(8773);
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_FAIL_OVERDATE:
		{
			TCHAR * pTitle = GetTxtFromMgr(449) ;
			TCHAR * pText = GetTxtFromMgr(8769);
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_SUCCES_USESTRAGERICKET:
		{
			TCHAR * pTitle = GetTxtFromMgr(389) ;
			TCHAR * pText = GetTxtFromMgr(8770);
			if ( !pTitle || !pText ) break ;
			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
		
	case SRVAL_SUMMON_ADD_MUDANGPRESENT:
		{
			TCHAR * pTitle = GetTxtFromMgr(6069) ;
			TCHAR * pText = GetTxtFromMgr(8816);
			TCHAR * pText2 = GetTxtFromMgr(2271);

			if ( !pText || !pText2 ) break ;

			TCHAR buffer[512] = TEXT("") ;

			if(pclreturn->siPara2 == 0)
			{
				StringCchCopy(buffer, 512, pText2);
			}
			else
			{
				TCHAR*ItemName = (TCHAR*)pclItemManager->GetName( pclreturn->siPara1 );

				StringCchPrintf(buffer, 512, pText,ItemName,pclreturn->siPara2);
			}

			//m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			
			if(m_pDialog[ NMUDANG_DLG])
			{
				((CNMudangDlg*)m_pDialog[ NMUDANG_DLG])->SetMudangPresentTxt(buffer);
			}
			
		}
		break;
	case SRVAL_SUMMON_FREE_FAIL_HAVEITEM:
		{
			TCHAR * pTitle = GetTxtFromMgr(449) ;
			TCHAR * pText = GetTxtFromMgr(453);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_FAIL_CANNOTSLOTOUTDEAD:
		{
			TCHAR * pTitle = GetTxtFromMgr(449) ;
			TCHAR * pText = GetTxtFromMgr(8776);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_FREE_FAIL_STONNUMINCORRECT:// 장비석 을 정확하게 기입해주세요
		{
			TCHAR * pTitle = GetTxtFromMgr(578) ;
			TCHAR * pText = GetTxtFromMgr(8612);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_FREE_FAIL_STONNUMNOTENOUGHSTON:// 장비석 을 정확하게 기입해주세요
		{
			TCHAR * pTitle = GetTxtFromMgr(578) ;
			TCHAR * pText = GetTxtFromMgr(8615);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMONSUCCESS_USESTON:// 장비석사용이 성공되었습니다.
		{
			TCHAR * pTitle = GetTxtFromMgr(449) ;
			TCHAR * pText = GetTxtFromMgr(8616);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_FREE_FAIL_CANNOTOVERUSESTON:// 장비석 을 정확하게 기입해주세요
		{
			TCHAR * pTitle = GetTxtFromMgr(578) ;
			TCHAR * pText = GetTxtFromMgr(8618);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMONSUCCESS_CHARGESTAMINA:	// 소환수 스태미너 충전하였습니다.
		{
			TCHAR * pTitle = GetTxtFromMgr(8583) ;
			TCHAR * pText = GetTxtFromMgr(8588);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_FAIL_TAKEOUTDELAY:
		{
			TCHAR * pTitle = GetTxtFromMgr(452);
			TCHAR * pText = GetTxtFromMgr(6143);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_WASTEALLSTAMINA:
		{
			TCHAR * pTitle = GetTxtFromMgr(449) ;
			TCHAR * pText = GetTxtFromMgr(6144);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_NEEDCHARGESTAMINA:
		{
			TCHAR * pTitle = GetTxtFromMgr(449) ;
			TCHAR * pText = GetTxtFromMgr(8645);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUMMON_STATUSINIT_FAIL_HAVEITEM:
		{
			NTCHARString64	strTitle( GetTxtFromMgr(449) );
			NTCHARString256	strText( GetTxtFromMgr(8685) );

			if ( strTitle.IsEmpty() || strText.IsEmpty() )
			{
				break;
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		}
		break;
	case SRVAL_SUMMON_DONOTHAVE_PROJECTILE:
		{
			NTCHARString64	strTitle( GetTxtFromMgr(836) );
			NTCHARString256	strText;
			
			if ( 1 == pclreturn->siPara1 )
			{
				strText = GetTxtFromMgr(5835);	// 화살이 없음
			}
			else if ( 2 == pclreturn->siPara1 ) 
			{
				strText = GetTxtFromMgr(5836);	// 총알이 없음				
			}
			else
			{
				break;
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		}
		break;
	case SRVAL_NOPROPERFISHAREA:
		{
			TCHAR * pTitle = GetTxtFromMgr(454) ;
			TCHAR * pText = GetTxtFromMgr(455);
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FISHRUNNINGAWAY:
		{
			PushEffect(constEffectFailure, 0, 0);

			TCHAR *ItemName = (TCHAR*)pclItemManager->GetName( pclreturn->siPara1 );

			TCHAR buffer[256 ] = TEXT("");

			if( ItemName )
			{
				TCHAR * pText = GetTxtFromMgr(456);
				if ( !pText ) break ;
				StringCchPrintf(buffer, 256, pText, ItemName);
				SendChatMessage(CHATMODE_SYSTEM, buffer);
			}
			else
			{
				TCHAR * pText = GetTxtFromMgr(457);
				if ( !pText ) break ;

				StringCchPrintf(buffer, 256, pText );
				SendChatMessage(CHATMODE_SYSTEM, buffer);
			}
		}
		break;
	case SRVAL_FISHSKILLNOTENOUGH:
		{
			PushEffect(constEffectFailure, 0, 0);
			TCHAR *ItemName = (TCHAR*)pclItemManager->GetName( pclreturn->siPara1 );

			TCHAR buffer[256 ] = TEXT("");

			if( ItemName )
			{
				TCHAR * pText = GetTxtFromMgr(458);
				if ( !pText ) break ;

				StringCchPrintf(buffer, 256, pText, ItemName);
				SendChatMessage(CHATMODE_SYSTEM, buffer);
			}
			else
			{
				TCHAR * pText = GetTxtFromMgr(459);
				if ( !pText ) break ;

				StringCchPrintf(buffer, 256, pText );
				SendChatMessage(CHATMODE_SYSTEM, buffer);
			}
		}
		break;
	case SRVAL_FISHTOOLNOTEXIST:
		{
			PushEffect(constEffectFailure, 0, 0);

			TCHAR *ItemName = (TCHAR*)pclItemManager->GetName( pclreturn->siPara1 );

			if( ItemName )
			{
				TCHAR * pTitle = GetTxtFromMgr(460) ;
				TCHAR * pText = GetTxtFromMgr(461);


				if ( !pTitle || !pText ) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, ItemName );
			}
			else
			{
				TCHAR * pTitle = GetTxtFromMgr(460) ;
				TCHAR * pText = GetTxtFromMgr(462);
				if ( !pTitle || !pText ) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
			}
		}
		break;
	case SRVAL_FISHTOOLNOTENOUGHDURABILITY:
		{
			PushEffect(constEffectFailure, 0, 0);

			TCHAR*ItemName = (TCHAR*)pclItemManager->GetName( pclreturn->siPara1 );

			if( ItemName )
			{
				TCHAR* pTitle = GetTxtFromMgr(460) ;
				TCHAR* pText = GetTxtFromMgr(463);
				if ( !pTitle || !pText ) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, ItemName);
			}
			else
			{
				TCHAR* pTitle = GetTxtFromMgr(460) ;
				TCHAR* pText = GetTxtFromMgr(464);
				if ( !pTitle || !pText ) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			}
		}
		break;
	case SRVAL_FISHNOTBAITSHRIMP:
		{
			NTCHARString256	kText( GetTxtFromMgr(8412) );
			NTCHARString64	kItemName( TEXT("") );

			kItemName = pclItemManager->GetName( pclreturn->siPara1 );

			if( kItemName.IsEmpty() == FALSE )
			{
				TCHAR* pTitle = GetTxtFromMgr(460) ;
				kText.Replace( TEXT("#itemname#"), kItemName );

				if ( !pTitle || kText.IsEmpty() == TRUE ) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, kText);
			}
		}
		break;
	case SRVAL_FISHNOTBAITLUGWORM:
		{
			NTCHARString256	kText( GetTxtFromMgr(8413) );
			NTCHARString64	kItemName( TEXT("") );

			kItemName = pclItemManager->GetName( pclreturn->siPara1 );

			if( kItemName.IsEmpty() == FALSE )
			{
				TCHAR* pTitle = GetTxtFromMgr(460) ;
				kText.Replace( TEXT("#itemname#"), kItemName );

				if ( !pTitle || kText.IsEmpty() == TRUE ) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, kText);
			}
		}
		break;
	case SRVAL_FARM_FAIL:
		switch(pclreturn->siPara1)
		{
		case CANFARM_ERORR_NORENT:
			{

				TCHAR* pText = GetTxtFromMgr(466);
				if (!pText ) break ;
				pclMessage->SetMsg(pText);
			}
			break;
		case CANFARM_ERROR_NOSKILL:
			{
				TCHAR* pText = GetTxtFromMgr(467);
				if (!pText ) break ;
				pclMessage->SetMsg(pText);
			}
			break;
		case CANFARM_ERROR_NOTOOL:
			{
				TCHAR* pText = GetTxtFromMgr(468);
				if (!pText ) break ;
				pclMessage->SetMsg(pText);
			}
			break;
		case CANFARM_ERORR_HAVEDISEASE:
			{
				TCHAR* pTitle = GetTxtFromMgr(836);
				TCHAR* pText = GetTxtFromMgr(5781);
				if (!pText || !pTitle) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			}
			break;
		case CANFARM_ERORR_CANNOTLEVEL:
			{
				TCHAR* pTitle = GetTxtFromMgr(836);
				TCHAR* pText = GetTxtFromMgr(5826);
				if (!pText || !pTitle) break ;

				if (!pText || !pTitle) break ;

				TCHAR buffer[256] = {'\0'};
				StringCchPrintf( buffer, 256, pText, CANFARM_LEVEL );
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer );
			}
			break;
		case CANFARM_ERORR_RENTING:
			{
				TCHAR* pTitle = GetTxtFromMgr(836);
				TCHAR* pText = GetTxtFromMgr(8337);
				if (!pText || !pTitle) break ;

				((CNFarmingDlg*)m_pDialog[NFARMING_DLG ])->m_bStopFarming = TRUE;
				TCHAR* VillageName = (TCHAR*)pclVillageManager->GetName( pclreturn->siPara2 );
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, VillageName );
			}
			break;
		default:
			{
				TCHAR* pText = GetTxtFromMgr(469);
				if (!pText ) break ;
				pclMessage->SetMsg(pText);
			}
			break;
		}
		break;
	case SRVAL_FARM_LANDSTRUCTURE_NOT_EXIST:
		{
			TCHAR* pText = GetTxtFromMgr(470);
			if (!pText ) break ;
			pclMessage->SetMsg(pText);
		}
		break;
	case SRVAL_MINE_FAIL:
		switch(pclreturn->siPara1)
		{
		case CANMINE_ERORR_NORENT:
			{
				TCHAR* pText = GetTxtFromMgr(3202);
				if (!pText ) break ;
				pclMessage->SetMsg(pText);
			}
			break;
		case CANMINE_ERROR_NOSKILL:
			{
				TCHAR* pText = GetTxtFromMgr(3203);
				if (!pText ) break ;
				pclMessage->SetMsg(pText);
			}
			break;
		case CANMINE_ERROR_NOTOOL:
			{
				TCHAR* pText = GetTxtFromMgr(3204);
				if (!pText ) break ;
				pclMessage->SetMsg(pText);
			}
			break;
		case CANMINE_ERORR_HAVEDISEASE:
			{
				TCHAR* pTitle = GetTxtFromMgr(836);
				TCHAR* pText = GetTxtFromMgr(5782);
				if (!pText || !pTitle) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			}
			break;
		case CANMINE_ERORR_CANNOTLEVEL:
			{
				TCHAR* pTitle = GetTxtFromMgr(836);
				TCHAR* pText = GetTxtFromMgr(5825);
				if (!pText || !pTitle) break ;
				
				TCHAR buffer[256] = {'\0'};
				StringCchPrintf( buffer, 256, pText, CANMINE_LEVEL );
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer );			
			}
			break;
		case CANMINE_ERORR_RENTING:
			{
				TCHAR* pTitle = GetTxtFromMgr(836);
				TCHAR* pText = GetTxtFromMgr(8336);
				if (!pText || !pTitle) break ;
				
				((CNMiningDlg*)m_pDialog[NMINING_DLG ])->m_bStopMining = TRUE;
				TCHAR* VillageName = (TCHAR*)pclVillageManager->GetName( pclreturn->siPara2 );
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, VillageName );
			}
			break;
		default:
			{
				TCHAR* pText = GetTxtFromMgr(3205);
				if (!pText ) break ;
				pclMessage->SetMsg(pText);
			}
			break;
		}
		break;
	case SRVAL_MINE_MINESTRUCTURE_NOT_EXIST:
		{
			TCHAR* pText = GetTxtFromMgr(3206);
			if (!pText ) break ;
			pclMessage->SetMsg(pText);
		}
		break;

	case SRVAL_DBSYSTEMINFO:
		PushEffect(constEffectSuccess, 0, 0);
		switch(pclreturn->siPara1)
		{
		case DBSYSTEMINFO_TODAY_NEWUSERNUM:
			{
				TCHAR* pTitle = GetTxtFromMgr(489) ;
				TCHAR* pText = GetTxtFromMgr(490);
				if ( !pTitle || !pText ) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText, pclreturn->siPara2);
			}
			break;
		case DBSYSTEMINFO_CALCULATE_WEALTHRANK:
			if(pclreturn->siPara2 == 1)
			{
				TCHAR* pTitle = GetTxtFromMgr(489) ;
				TCHAR* pText = GetTxtFromMgr(491);
				if ( !pTitle || !pText ) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			}
			else if(pclreturn->siPara2 == 2)
			{
				TCHAR* pTitle = GetTxtFromMgr(489) ;
				TCHAR* pText = GetTxtFromMgr(492);
				if ( !pTitle || !pText ) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			}
			break;
		case DBSYSTEMINFO_CALCULATE_TOTALMONEY:
			if(pclreturn->siPara2 == 1 )
			{
				TCHAR* pTitle = GetTxtFromMgr(489) ;
				TCHAR* pText = GetTxtFromMgr(493);
				if ( !pTitle || !pText ) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			}
			else if ( pclreturn->siPara2 == 2 )
			{
				TCHAR* pTitle = GetTxtFromMgr(489) ;
				TCHAR* pText = GetTxtFromMgr(494);
				if ( !pTitle || !pText ) break ;

				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			}
		}
		break;
	case SRVAL_CANUSEMAGIC_FAIL:
		switch(pclreturn->siPara1)
		{
		case CANUSEMAGIC_ERROR_NEEDMANA:
			{
				TCHAR* pText = GetTxtFromMgr(495);
				if ( !pText ) break ;
				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclClient->pclMessage->SetPersonalMsg(pText);
				}
				else
				{
					pclClient->pclMessage->SetMsg(pText);
				}
				PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_NO_MANA")), 0, 0);
			}
			break;
		case CANUSEMAGIC_ERROR_FARDISTANCE:		
			{
				TCHAR* pText = GetTxtFromMgr(496);
				if ( !pText ) break ;
				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclClient->pclMessage->SetPersonalMsg(pText);
				}
				else
				{
					pclClient->pclMessage->SetMsg(pText);
				}
				PushEffect(constEffectFailure, 0, 0);
			}
			break;
		case CANUSEMAGIC_ERROR_NEEDTIMEDELAY:	/*pclMessage->SetMsg(TEXT("[정보] 마법 구현까지 시간이 필요합니다."));*/	break;	
		case CANUSEMAGIC_ERROR_NOTENOUGHSKILL:	
			{
				TCHAR* pText = GetTxtFromMgr(498);
				if ( !pText ) break ;
				
				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclClient->pclMessage->SetPersonalMsg(pText);
				}
				else
				{
					pclClient->pclMessage->SetMsg(pText);
				}
				PushEffect(constEffectFailure, 0, 0);
			}
			break;	
		}
		break;
	case SRVAL_PERSONALSHOP:
		PushEffect(constEffectSuccess, 0, 0);
		switch(pclreturn->siPara1)
		{
		case 1:
			{
				TCHAR* pText = GetTxtFromMgr(504);
				if ( !pText ) break ;

				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclClient->pclMessage->SetPersonalMsg(pText);
				}
				else
				{
					pclClient->pclMessage->SetMsg(pText);
				}
			}
			break;
		case 0:
			{
				TCHAR* pText = GetTxtFromMgr(505);
				if ( !pText ) break ;

				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclClient->pclMessage->SetPersonalMsg(pText);
				}
				else
				{
					pclClient->pclMessage->SetMsg(pText);
				}
			}
			break;
		}
		break;

	case SRVAL_FAIL_PERSONALSHOP_USEITEM:	
		{
			TCHAR* pTitle = GetTxtFromMgr(10039);
			TCHAR* pText = GetTxtFromMgr(10040);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
			PushEffect(constEffectFailure, 0, 0);
		}
		break;
	case SRVAL_FAIL_PERSONALSHOP_MOVEITEM:	
		{
			TCHAR* pTitle = GetTxtFromMgr(1891);
			TCHAR* pText = GetTxtFromMgr(10038);

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

			PushEffect(constEffectFailure, 0, 0);
		}
		break;

	case SRVAL_BADSTEALACTION:
		{
			TCHAR buffer[256];
			TCHAR* pText = GetTxtFromMgr(506);
			if ( !pText ) break ;
			StringCchPrintf(buffer, 256, pText, 
				pclreturn->siPara1, pclreturn->siPara2);
			pclMessage->SetMsg(buffer);

			PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_STEAL_WARNING")), 0, 0);
		}
		break;
	case SRVAL_BADSTEALACTIONPUNISH:
		{
			TCHAR buffer[256];
			TCHAR* pText = GetTxtFromMgr(507);
			if ( !pText ) break ;
			StringCchPrintf(buffer, 256, pText, pclreturn->siPara1);
			pclMessage->SetMsg(buffer);
		}
		break;

	case SRVAL_ADDINV_FAIL_FULL:
		{
			const TCHAR* pitemname = pclItemManager->GetName(pclreturn->siPara1);
			if(pitemname)
			{
				TCHAR buffer[256];
				TCHAR* pText = GetTxtFromMgr(508);
				if ( !pText ) break ;

				StringCchPrintf(buffer, 256, pText, pitemname); 
				pclMessage->SetMsg(buffer);
			}
		}
		break;

	case SRVAL_GIVECASH:
		{
			switch(pclreturn->siPara2)
			{
			case PUSHCASH_REASON_QUEST:
				{
					TCHAR* pTitle = GetTxtFromMgr(509) ;
					TCHAR* pText = GetTxtFromMgr(846);
					if ( !pTitle || !pText ) break ;

					m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText,pclreturn->siPara1 );
				}
				break;
			case PUSHCASH_REASON_ATTENDANCE:
				{
					TCHAR* pTitle = GetTxtFromMgr(509) ;
					TCHAR* pText = GetTxtFromMgr(510);
					if ( !pTitle || !pText ) break ;

					m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText,pclreturn->siPara1 ,3);
				}
				break;
			default: break;
			}
		}
		break;

	case SRVAL_ATTENDANCE:
		{

			//if( pclClient->siServiceArea == ConstServiceArea_Korea )
			//{
			//	SI32 gimg = 0;
			//	switch(pclreturn->siPara2)
			//	{
			//	case 1: gimg = GIMG_NOTICEEVENT_STAMP1;	break;
			//	case 2:	gimg = GIMG_NOTICEEVENT_STAMP2;	break;
			//	case 3:	gimg = GIMG_NOTICEEVENT_STAMP3;	break;
			//	default:
			//		return;
			//	}

			//	TCHAR* pText = GetTxtFromMgr(511);
			//	if ( !pText ) break ;
			//	if( pclClient->siServiceArea ==  ConstServiceArea_Korea )
			//	{
			//		( (CNoticeEvnetMsgDlg *)m_pDialog[ NOTICEEVENTMSG_DLG ])->SetGlobalImage(pText, gimg);
			//	}
			//}
			//else if( pclClient->siServiceArea == ConstServiceArea_China ) //중국어
			//{

			//}
			//else // 영어
			//{

			//}
		}
		break;
	case SRVAL_CANNOTDROPITEM:
		{
			TCHAR* pTitle = GetTxtFromMgr(3266);
			if ( pTitle == NULL ) break ;
			TCHAR* pText = GetTxtFromMgr(3267);
			if ( pText == NULL ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_NOWARP_TRADEPORT_VILLAGE:
		{
			TCHAR buffer[128] = TEXT("") ;
			TCHAR* pTitle = GetTxtFromMgr(1891);
			if ( pTitle == NULL ) break ;
			TCHAR* pText = NULL ;

			if ( pclreturn->siPara1 && pclVillageManager->IsValidVillage(pclreturn->siPara1) )
			{
				pText = GetTxtFromMgr(5348);
				if ( pText == NULL ) break ;

				StringCchPrintf(buffer, 128, pText,pclVillageManager->GetName(pclreturn->siPara1));
			}
			else
			{
				pText = GetTxtFromMgr(5349);
				if ( pText == NULL ) break ;

				MStrCpy(buffer,pText,128);
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer );
		}
		break;
	case SRVAL_INSTANCEPROFIT_VILLAGE:
		{
			TCHAR buffer[128] = TEXT("") ;
			TCHAR* pTitle = GetTxtFromMgr(5350);
			if ( pTitle == NULL ) break ;
			TCHAR* pText = NULL ;

			if ( pclreturn->siPara1 && pclVillageManager->IsValidVillage(pclreturn->siPara1) )
			{
				pText = GetTxtFromMgr(5351);
				if ( pText == NULL ) break ;

				StringCchPrintf(buffer, 128, pText,pclVillageManager->GetName(pclreturn->siPara1));
			}
			else
			{
				pText = GetTxtFromMgr(5352);
				if ( pText == NULL ) break ;

				MStrCpy(buffer,pText,128);
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer );
		}
		break;

	case SRVAL_GET_SAVEUSERITEM_SUCCESS:
		{

			TCHAR* pTitle = GetTxtFromMgr(5640);
			TCHAR* pText = GetTxtFromMgr(5843);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_GET_SAVEUSERITEM_FAIL:
		{
			TCHAR* pTitle = GetTxtFromMgr(5640);
			TCHAR* pText = GetTxtFromMgr(5844);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_NO_BATTLEQUIT:
		{
			TCHAR* pTitle = GetTxtFromMgr(5581);
			TCHAR* pText = GetTxtFromMgr(5582);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_NO_DIEQUIT:
		{
			TCHAR* pTitle = GetTxtFromMgr(5581);
			TCHAR* pText = GetTxtFromMgr(5583);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_DIS_WORLDMONEY:
		{
			TCHAR* pTitle = GetTxtFromMgr(6171);
			TCHAR* pText = GetTxtFromMgr(6172);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_WORLDMONEY_SELLORDER:
		{
			TCHAR* pTitle = GetTxtFromMgr(6173);
			TCHAR* pText = GetTxtFromMgr(6174);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_WORLDMONEY_BUYORDER:
		{
			TCHAR* pTitle = GetTxtFromMgr(6175);
			TCHAR* pText = GetTxtFromMgr(6176);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_WORLDMONEY_BUYORDER_MYSELF:
		{
			TCHAR* pTitle = GetTxtFromMgr(6175);
			TCHAR* pText = GetTxtFromMgr(6444);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_WORLDMONEY_BUYORDER_NOMONEY:
		{
			TCHAR* pTitle = GetTxtFromMgr(1260);
			TCHAR* pText = GetTxtFromMgr(5870);
			if ( !pTitle || !pText ) break;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUCCESS_WORLDMONEY_SETPRICE:
		{
			TCHAR* pTitle = GetTxtFromMgr(6230);
			TCHAR* pText = GetTxtFromMgr(6265);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_WORLDMONEY_SET_CHAR:
		{
			TCHAR* pTitle = GetTxtFromMgr(393);
			TCHAR* pText = GetTxtFromMgr(6329);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUCCESS_WORLDMONEY_SET_CHAR:
		{
			TCHAR* pTitle = GetTxtFromMgr(2045);
			TCHAR* pText = GetTxtFromMgr(6330);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

		// TradeServer
	case SRVAL_FAIL_TRADESERVER_NOMORE_SELL:
		{
			TCHAR* pTitle = GetTxtFromMgr(6203);
			TCHAR* pText = GetTxtFromMgr(6204);
			if ( !pTitle || !pText ) break ;

			char buf[128] = "" ;
			sprintf(buf,pText ,MAX_TRADESERVER_ORDER);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buf);
		}
		break;
	case SRVAL_FAIL_TRADESERVER_SESSION:
		{
			TCHAR* pTitle = GetTxtFromMgr(6205);
			TCHAR* pText = GetTxtFromMgr(6206);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_TRADESERVER_GM:
		{
			TCHAR* pTitle = GetTxtFromMgr(6207);
			TCHAR* pText = GetTxtFromMgr(6208);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_TRADESERVER_LIMITTIME:
		{
			TCHAR* pTitle = GetTxtFromMgr(6209);
			TCHAR* pText = GetTxtFromMgr(6210);
			if ( !pTitle || !pText ) break ;

			char buf[128] = "" ;
			sprintf(buf,pText,
				pclreturn->siPara1,pclreturn->siPara2);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buf);
		}
		break;
	case SRVAL_FAIL_TRADESERVER_NOT_BUY:
		{
			TCHAR* pTitle = GetTxtFromMgr(6211);
			TCHAR* pText = GetTxtFromMgr(6212);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRAVL_SUCCESS_TRADESERVER_SETTIME:
		{
			TCHAR* pTitle = GetTxtFromMgr(6213);
			TCHAR* pText = GetTxtFromMgr(6214);
			if ( !pTitle || !pText ) break ;

			char buf[128] = "" ;
			sprintf(buf,pText,
				pclreturn->siPara1,pclreturn->siPara2);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buf);
		}
		break;
	case SRVAL_SUCCESS_SET_TRADESERVER:
		{
			TCHAR* pTitle = GetTxtFromMgr(6215);
			TCHAR* pText = GetTxtFromMgr(6216);

			if ( pclreturn->siPara1 == 1 )
			{
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			}
			else
			{
				pText = GetTxtFromMgr(6217);
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
			}
		}
		break;
	case SRVAL_FAIL_TRADESERVER_CANCEL_NOT:
		{
			TCHAR* pTitle = GetTxtFromMgr(6218);
			TCHAR* pText = GetTxtFromMgr(6219);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_TRADESERVER_CANCEL_ERROR:
		{
			TCHAR* pTitle = GetTxtFromMgr(6218);
			TCHAR* pText = GetTxtFromMgr(6220);
			if ( !pTitle || !pText ) break ;

			char buf[128] = "" ;
			sprintf(buf,pText,
				pclreturn->siPara1);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle,buf);
		}
		break;

	case SRVAL_FAIL_TRADESERVER_BUY_FULLINV:
		{
			TCHAR* pTitle = GetTxtFromMgr(512);
			TCHAR* pText = GetTxtFromMgr(442);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_NOTICE_TRADESERVER_STATE:
		{
			NTCHARString64	strTitle( GetTxtFromMgr(5614) );
			NTCHARString512	strText( TEXT("Trade Sever State : #state#") );

			switch ( pclreturn->siPara1 )
			{
				case -1:							strText.Replace( TEXT("#state#"), TEXT("Not Allocated") );				break;
				case SESSION_STATE_NONE:			strText.Replace( TEXT("#state#"), TEXT("SESSION_STATE_NONE") );			break;
				case SESSION_STATE_ESTABLISHED:		strText.Replace( TEXT("#state#"), TEXT("SESSION_STATE_ESTABLISHED") );	break;
				default:							strText.Replace( TEXT("#state#"), TEXT("NOT DEFINED") );				break;
			}
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		}
		break;

	case SRVAL_FAIL_WORLDPASSPORT_OVERLAPPING:
		{
			NTCHARString64	strTitle( GetTxtFromMgr(8833) );
			NTCHARString512	strText( GetTxtFromMgr(8834) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		}
		break;

	case SRVAL_NOTICE_EVENT_TOGGLE_SWITCH:
		{
			NTCHARString128 strText( TEXT("[MASTER] #eventname# #toggle#") );
			NTCHARString64	strEventName;
			NTCHARString64	strToggle( TEXT("Enabled") );

			switch ( pclreturn->siPara1 )
			{
				case EVENT_TOGGLE_SWITCH_FISHINGEVENT:	strEventName = TEXT("FishingEvent");	break;
				case EVENT_TOGGLE_SWITCH_SYSTEMBUY:		strEventName = TEXT("SystemBuy");		break;
			}

			if ( 0 >= pclreturn->siPara2 )
			{
				strToggle = TEXT("Disabled");
			}

			strText.Replace( TEXT("#eventname#"),	strEventName );
			strText.Replace( TEXT("#toggle#"),		strToggle );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( TEXT(""), strText );
		}
		break;

	case SRVAL_NOTICE_LEVELUP_REWARD:
		{
			NTCHARString64	strTitle( GetTxtFromMgr(8934) );
			NTCHARString256	strText( GetTxtFromMgr(8932) );

			TCHAR* pszItemName = (TCHAR*)pclItemManager->GetName( pclreturn->siPara2 );

			strText.Replace( TEXT("#level#"),		SI32ToString(pclreturn->siPara1) );
			strText.Replace( TEXT("#itemname#"),	pszItemName );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		}
		break;

	case SRVAL_NPC_MARKET_SELL_FAIL:
		{
			TCHAR* pTitle = GetTxtFromMgr(5730);
			TCHAR* pText = GetTxtFromMgr(205);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
		////////////// 파티 초대 관련 실패 메시지/////
	case SRVAL_PARTYINVITE_FULLPARTY:
		{
			TCHAR* pTitle = GetTxtFromMgr(316);
			TCHAR* pText = GetTxtFromMgr(5886);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_PARTYINVITE_NOTLEADER:
		{
			TCHAR* pTitle = GetTxtFromMgr(316);
			TCHAR* pText = GetTxtFromMgr(5887);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_PARTYINVITE_ALREADYPARTY:
		{
			TCHAR* pTitle = GetTxtFromMgr(316);
			TCHAR* pText = GetTxtFromMgr(5888);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;


	case SRVAL_PARTYINVITE_NOTFINDNAME:
		{
			TCHAR* pTitle = GetTxtFromMgr(316);
			TCHAR* pText = GetTxtFromMgr(5890);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_PARTYINVITE_LEADERALREADYPARTY:
		{
			TCHAR* pTitle = GetTxtFromMgr(316);
			TCHAR* pText = GetTxtFromMgr(5888);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;	

	case SRVAL_NOTRADE_ITEM:
		{
			TCHAR* pTitle = GetTxtFromMgr(836);
			TCHAR* pText = GetTxtFromMgr(5892);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_CAN_NOT_USE_ITEM_IN_PRISON:
		{
			TCHAR* pTitle = GetTxtFromMgr(836);
			TCHAR* pText = GetTxtFromMgr(6033);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_CAN_NOT_CHAT_IN_PRISON:
		{
			TCHAR* pTitle = GetTxtFromMgr(836);
			TCHAR* pText = GetTxtFromMgr(6035);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_CAN_NOT_WARP_IN_PRISON:
		{
			TCHAR* pTitle = GetTxtFromMgr(836);
			TCHAR* pText = GetTxtFromMgr(6034);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_CAN_NOT_MAKEITEM_IN_PRISON:
		{
			TCHAR* pTitle = GetTxtFromMgr(836);
			TCHAR* pText = GetTxtFromMgr(6036);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_CAN_NOT_ENCHANTITEM_IN_PRISON:
		{
			TCHAR* pTitle = GetTxtFromMgr(836);
			TCHAR* pText = GetTxtFromMgr(6037);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_RECOMMANDPERSON: //유저 검색 실패
		{
			TCHAR* pTitle = GetTxtFromMgr(6247);
			TCHAR* pText = GetTxtFromMgr(6248);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;
	case SRVAL_FAIL_RECOMMANDPERSON_CANCEL: // 추천 취소.
		{
			TCHAR* pTitle = GetTxtFromMgr(6247);
			TCHAR* pText = GetTxtFromMgr(8286);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;

	case SRVAL_GIVEITEM_RECOMMANDER: // 추천을 한사람이 아이템을 받는다
		{
			NTCHARString64	strTitle( GetTxtFromMgr(8935) );
			NTCHARString256	strText( GetTxtFromMgr(8936) );

			SI32	siItemUnique	= pclreturn->siPara2;
			TCHAR*	pszItemName		= (TCHAR*)pclClient->pclItemManager->GetName( siItemUnique );
			
			strText.Replace( TEXT("#level#"),		SI32ToString( pclreturn->siPara1 ) );
			strText.Replace( TEXT("#itemname#"),	pszItemName );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );

		}
		break;
	

	case SRVAL_CITYHALL_FAIL_DECLAREWAR_WARTIME:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(15217);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;	

	case SRVAL_CITYHALL_FAIL_DECLAREWAR_NOTMAYOR:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(15219);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;	

	case SRVAL_CITYHALL_FAIL_DECLAREWAR_LOWERRANK:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(15199);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;	

	case SRVAL_CITYHALL_FAIL_DECLAREWAR_AFTERLOSE:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(15198);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;	

	case SRVAL_CITYHALL_FAIL_DECLAREWAR_TARGET_AFTERLOSE:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(15197);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;	
	case SRVAL_CITYHALL_FAIL_DECLAREWAR_NOTNATION:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(6489);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_CITYHALL_FAIL_DECLAREWAR_ALREADY:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(15212);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;	
	
	case SRVAL_CAN_NOT_WARP_IN_VILLAGEWAR:
		{
			TCHAR* pTitle = GetTxtFromMgr(836);
			TCHAR* pText = GetTxtFromMgr(6483);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_CITYHALL_FAIL_DECLAREWAR_ALREADYACT:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(15211);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;	
	
	case SRVAL_CITYHALL_FAIL_DECLAREWAR_NOMASTER:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(15564);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;	

	case SRVAL_CITYHALL_SUCCESS_DECLAREWAR:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(15210);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;	

	case SRVAL_CITYHALL_FAIL_DECLAREWAR_FORCESTOP:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(6489);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;	

	case SRVAL_CITYHALL_FAIL_DECLAREWAR_MADEPROTECT_SELF:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(6486);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;	

	case SRVAL_CITYHALL_FAIL_DECLAREWAR_MADEPROTECT_TARGET:
		{
			TCHAR* pTitle = GetTxtFromMgr(3710);
			TCHAR* pText = GetTxtFromMgr(6485);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

		}
		break;
		// 길드 - SJY
	case SRVAL_FAIL_CONFIRMCREATEGUILD_NOMONEY:
		{
			TCHAR* pTitle = GetTxtFromMgr(6562);
			TCHAR* pText = GetTxtFromMgr(6563);
			if ( !pTitle || !pText ) break ;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_POSTOFFICE_FAIL_SEND_NOTPOSTOFFICE:
		{
			TCHAR* pTitle = GetTxtFromMgr(6759);
			TCHAR* pText = GetTxtFromMgr(6760);

			if ( !pTitle || !pText ) break;

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
		}
		break;
	case SRVAL_FAIL_ALEADYRECEIVE_BEGINNERWEAPON:
		{
			TCHAR* pTitle = GetTxtFromMgr(512);
			TCHAR* pText = GetTxtFromMgr(6860);

			if ( pTitle && pText )
			{
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			}
		}
		break;	
	case SRVAL_FAIL_MANUFATURENPC_NOMONEY:
		{
			TCHAR *pText = GetTxtFromMgr(6875);

			cltClient* pclclient = (cltClient*)pclClient;

			CNManufactureNPCDlg* pDialog = (CNManufactureNPCDlg*)pclclient->m_pDialog[ NMANUFACTURENPC_DLG ];
			pDialog->SetExplain( pText );
		}
		break;
	case SRVAL_GUILDWAR_FAILAPPLY_DENY:
		{
			TCHAR* pTitle = GetTxtFromMgr(6694);
			TCHAR* pText = GetTxtFromMgr(6960);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_GUILDWAR_FAILAPPLY_MYGUILD:
		{
			TCHAR* pTitle = GetTxtFromMgr(6694);
			TCHAR* pText = GetTxtFromMgr(6691);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_GUILDWAR_FAILAPPLY_ALREADY:
		{
			TCHAR* pTitle = GetTxtFromMgr(6694);
			TCHAR* pText = GetTxtFromMgr(6961);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_GUILDWAR_FAILAPPLY_IM_ALREADYRESERVE:
		{
			TCHAR* pTitle = GetTxtFromMgr(6694);
			TCHAR* pText = GetTxtFromMgr(6979);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_GUILDWAR_FAILAPPLY_ENEMY_ALREADYRESERVE:
		{
			TCHAR* pTitle = GetTxtFromMgr(6694);
			TCHAR* pText = GetTxtFromMgr(6980);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_GUILDWAR_FAILAPPLY_NOTALLOWSERVER:
		{
			TCHAR* pTitle = GetTxtFromMgr(6694);
			TCHAR* pText = GetTxtFromMgr(10174);
		}
		break;
	case SRVAL_GUILDWAR_FAILAPPLY_LOWLEVEL:
		{
			TCHAR* pTitle = GetTxtFromMgr(6694);
			TCHAR* pText = GetTxtFromMgr(6692);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_GUILDWAR_FAILAPPLY_NOTCONNECTED:
		{
			TCHAR* pTitle = GetTxtFromMgr(6694);
			TCHAR* pText = GetTxtFromMgr(6959);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_GUILDWAR_FAILAPPLY_FAULTNAME:
		{
			TCHAR* pTitle = GetTxtFromMgr(6694);
			TCHAR* pText = GetTxtFromMgr(6690);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_GUILDWAR_FAILAPPLY_OVERTIME:
		{
			TCHAR* pTitle = GetTxtFromMgr(6694);
			TCHAR* pText = GetTxtFromMgr(8462);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_FAIL_GIVEITEM_NOINVEN:
		{
			TCHAR* pTitle = GetTxtFromMgr(422);
			TCHAR* pText = GetTxtFromMgr(6981);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_SUMMONEGG_USED:
		{
			if(pclreturn->siPara1 > 0)
			{
				TCHAR* pCharName = (TCHAR*)pclCM->CR[1]->GetName();
				TCHAR* pKindName = (TCHAR*)pclClient->pclKindInfoSet->GetName(pclreturn->siPara1);
				TCHAR buffer[256 ] = TEXT("");

				// [%s]님이 [%s]를 소환하였습니다.
				TCHAR* pText = GetTxtFromMgr(7027);
				if ( !pText ) break ;

				if(pCharName && pKindName)
				{
					StringCchPrintf(buffer, 256, pText, pCharName, pKindName);
					SendChatMessage(CHATMODE_SYSTEM, buffer);
				}
			}
		}
		break;
	case SRVAL_GACHA_FAIL_FULLINVEN:
		{
			if ( pclClient->IsCountrySwitch(Switch_CarryOverGacha) )
			{
				if ( m_pDialog[NCARRYOVERGACHA_DLG] )
				{
					m_pDialog[ NCARRYOVERGACHA_DLG ]->DeleteDialog();
				}
			}
			else if ( pclClient->IsCountrySwitch(Switch_Gacha_Reform) )
			{
				if(m_pDialog[NGACHANEW_DLG])
				{
					m_pDialog[NGACHANEW_DLG]->DeleteDialog();
				}
			}
			else
			{
				if(m_pDialog[NGACHA_DLG])
				{
					m_pDialog[NGACHA_DLG]->DeleteDialog();
				}
			}

			if(pclreturn->siPara1 == 100)
			{
				TCHAR* pTitle = GetTxtFromMgr(3921);
				TCHAR* pText  = GetTxtFromMgr(15289);
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			}
			else
			{
				// 인벤이 모자랍니다
				TCHAR* pTitle = GetTxtFromMgr(3921);
				TCHAR* pText = GetTxtFromMgr(7015);
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );	
			}
		}
		break;
	case SRVAL_GACHA_FAIL_KEYNEED:
		{
			if ( pclClient->IsCountrySwitch(Switch_CarryOverGacha) )
			{
				if ( m_pDialog[NCARRYOVERGACHA_DLG] )
				{
					m_pDialog[ NCARRYOVERGACHA_DLG ]->DeleteDialog();
				}

				TCHAR* pTitle = GetTxtFromMgr(3921);
				TCHAR* pText = GetTxtFromMgr(7016);
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			}
			else if ( pclClient->IsCountrySwitch(Switch_Gacha_Reform) )
			{
				TCHAR* pTitle = GetTxtFromMgr(3921);
				TCHAR* pText = GetTxtFromMgr(7016);
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			}
			else
			{
				if(m_pDialog[NGACHA_DLG])
				{
					((CNGachaDlg*)m_pDialog[NGACHA_DLG])->ChangeCurrentStepAndInit(GACHASTEP_KEYNEED);
				}
				else
				{	// 열쇠가 모자랍니다 
					TCHAR* pTitle = GetTxtFromMgr(3921);
					TCHAR* pText = GetTxtFromMgr(7016);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				}
			}
		}
		break;
	case SRVAL_GACHA_FAIL_LOCK:
		{

			if ( pclClient->IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) )
			{
				if ( pclClient->IsCountrySwitch(Switch_CarryOverGacha) )
				{
					if ( m_pDialog[NCARRYOVERGACHA_DLG] )
					{
						m_pDialog[ NCARRYOVERGACHA_DLG ]->DeleteDialog();
					}
					// 라쿤보물 낙시터가 수리중입니다. 새로운 모습으로 다시 찾아 오겠습니다.
					TCHAR* pTitle = GetTxtFromMgr(1078);
					TCHAR* pText = GetTxtFromMgr(30267);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

				}
				else if ( pclClient->IsCountrySwitch(Switch_Gacha_Reform) )
				{
					// 라쿤보물 낙시터가 수리중입니다. 새로운 모습으로 다시 찾아 오겠습니다.
					TCHAR* pTitle = GetTxtFromMgr(1078);
					TCHAR* pText = GetTxtFromMgr(30267);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				}
				else
				{
					if(m_pDialog[NGACHA_DLG])
					{
						((CNGachaDlg*)m_pDialog[NGACHA_DLG])->Destroy();
						// 라쿤보물 낙시터가 수리중입니다. 새로운 모습으로 다시 찾아 오겠습니다.
						TCHAR* pTitle = GetTxtFromMgr(1078);
						TCHAR* pText = GetTxtFromMgr(30267);
						m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
					}
					else
					{
						// 라쿤보물 낙시터가 수리중입니다. 새로운 모습으로 다시 찾아 오겠습니다.
						TCHAR* pTitle = GetTxtFromMgr(1078);
						TCHAR* pText = GetTxtFromMgr(30267);
						m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

					}

				}

			}
		}
		break;
	case SRVAL_GACHA_NOTCORRECT_ITEM:
		{
			// 잘못된 최고 상품입니다 .다시 가챠 해주세요 
			if ( pclClient->IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) )
			{
				if ( pclClient->IsCountrySwitch(Switch_CarryOverGacha) )
				{
					if ( m_pDialog[NCARRYOVERGACHA_DLG] )
					{
						m_pDialog[ NCARRYOVERGACHA_DLG ]->DeleteDialog();
					}
					TCHAR* pTitle = GetTxtFromMgr(1078);
					TCHAR* pText = GetTxtFromMgr(30268);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

				}
				else if ( pclClient->IsCountrySwitch(Switch_Gacha_Reform) )
				{
					TCHAR* pTitle = GetTxtFromMgr(1078);
					TCHAR* pText = GetTxtFromMgr(30268);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				}
				else
				{
					if(m_pDialog[NGACHA_DLG])
					{
						((CNGachaDlg*)m_pDialog[NGACHA_DLG])->Destroy();
						TCHAR* pTitle = GetTxtFromMgr(1078);
						TCHAR* pText = GetTxtFromMgr(30268);
						m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
					}
					else
					{
						// 라쿤보물 낙시터가 수리중입니다. 새로운 모습으로 다시 찾아 오겠습니다.
						TCHAR* pTitle = GetTxtFromMgr(1078);
						TCHAR* pText = GetTxtFromMgr(30268);
						m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

					}

				}

			}
		}
		break;
	case SRVAL_HIDDENQUEST_BOX://히든 퀘스트 
		{
			////CreateInterface( HIDDENQUEST2_DLG );
			//CreateInterface( HIDDENQUEST2_DLG );
		
			//((NHiddenQuestDlg*)m_pDialog[HIDDENQUEST2_DLG])->SetPram(pclreturn->siPara1);

		}break;
	case SRVAL_TIRED_SYSTEM_CHAR:
		{
			TCHAR* pTitle = GetTxtFromMgr( 10080 );
			TCHAR* pText = GetTxtFromMgr( 10081 );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_RECORD_MOUSEACTION_START:
		{
			TCHAR* pTitle = TEXT("")	;
			TCHAR* pText = GetTxtFromMgr( 40355 );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;		
	case SRVAL_FAMILY_ADD_ERROR_NO_CHAR:// 가족 추가 에러 잘못된 대상의 케릭터 
		{
			TCHAR* pTitle = TEXT("")	;
			TCHAR* pText = GetTxtFromMgr( 30125 );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_FAMILY_ADD_WRONG_BEFORE_IN_FAMILY:// 이미 가정이 있는 케릭터 
		{
			TCHAR* pTitle = TEXT("")	;
			TCHAR* pText = GetTxtFromMgr( 30126 );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_FAMILY_ADD_WRONG_NOTCONNECT_MATE:// 부모중에 한명이라도 접속해있지 않으면 안된다.
		{
			TCHAR* pTitle = TEXT("")	;
			TCHAR* pText = GetTxtFromMgr( 30130 );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;
	case SRVAL_FAMILY_ADD_WRONG_No_TICKET: // 티켓이 없다
		{
			TCHAR* pItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclreturn->siPara1 );
			TCHAR* pTitle = TEXT("")	;
			NTCHARString128 kMessage( GetTxtFromMgr(30129) );
			kMessage.Replace( TEXT("#itam#"), pItemName );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kMessage );
		}
		break;
	case SRVAL_FAMILY_ADD_WRONG_DEL_FAMILY: // 가족에서 짤렸다.
		{
			TCHAR* pItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclreturn->siPara1 );
			TCHAR* pTitle = TEXT("")	;
			NTCHARString128 kMessage( GetTxtFromMgr(30132) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kMessage );
		}
		break;
	case SRVAL_FAMILY_GONE_FAMILY:	//기족에서 자식이 탈퇴 하였다.
		{
			TCHAR* pItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclreturn->siPara1 );
			TCHAR* pTitle = TEXT("")	;
			NTCHARString128 kMessage( GetTxtFromMgr(30135) );
			kMessage.Replace(TEXT("[#name#]"),pItemName)	;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kMessage );
		}
	case SRVAL_FAMILY_ADD_WRONG_No_TICKET_PLUSDATE: // 가족연장 이용권이 없다.
		{
			TCHAR* pItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclreturn->siPara1 );
			TCHAR* pTitle = TEXT("")	;
			NTCHARString128 kMessage( GetTxtFromMgr(30138) );
			kMessage.Replace(TEXT("#item#"),pItemName)	;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kMessage );
		}
		break;
	case SRVAL_FAMILY_ADD_WRONG_DEL_HAVECHILD:// 가족에서 탈퇴 실패 - 부모인데 가족이 있다.
		{
			TCHAR* pTitle = TEXT("")	;
			TCHAR* pText = GetTxtFromMgr( 40862 );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		break;		
	case SRVAL_FAMILY_ADD_FAMILY: // 가족으로 등록 되었다
		{
			TCHAR* pItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclreturn->siPara1 );
			TCHAR* pTitle = TEXT("")	;
			NTCHARString1024 kMessage( GetTxtFromMgr(30237) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kMessage );
		}
		break;
		
	case SRVAL_PVP_LEAGUE_APPLY_MESSAGE:
		{
			NTCHARString64	strTitle(GetTxtFromMgr(9581));
			NTCHARString256	strText;

			// 실패
			switch ( pclreturn->siPara1 )
			{
				case PVP_LEAGUE_APPLY_RESULT_SUCCESS:	// 성공
					{
						strText = GetTxtFromMgr( 9601 );
						strText.Replace( TEXT("#num#"), SI32ToString(pclreturn->siPara2) );
					}
					break;
				case PVP_LEAGUE_APPLY_RESULT_ALREADY:	// 이미 신청
					{
						strText = GetTxtFromMgr( 9606 );
					}
					break;
				case PVP_LEAGUE_APPLY_RESULT_FULL:	// 인원 다참
					{
						strText = GetTxtFromMgr( 9602 );
					}
					break;
				default:	
					{
						return;
					}
			}
			
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		}
		break;

	case SRVAL_PVP_LEAGUE_AGAIN_RESERVE:
		{
			NTCHARString64	strTitle(GetTxtFromMgr(9581));
			NTCHARString256	strText(GetTxtFromMgr(9632));

			stMsgBox MsgBox;
			MsgBox.Set( m_pDialog[NOTIFYMSG_DLG], strTitle, strText, MSGBOX_TYPE_YESNO, SRVAL_PVP_LEAGUE_AGAIN_RESERVE );
			SetMsgBox( &MsgBox, NULL, 0 );
		}
		break;

	case SRVAL_RETURN_USERCOUNT:
		{
			TCHAR buf[128] = {0,};
			StringCchPrintf( buf, 128, TEXT("%d"), pclreturn->siPara1 );
			pclMessage->SetMsg(buf);
		}
		break;
	//KHY - 1211 - 과금상품 - 만능 도구			
	case SRVAL_RETURN_USE_ALLPURPOSETOOL:
		break;
	//KHY - 1220 - 변신시스템.
	case SRVAL_FAIL_ALEADYTRANSFROMCHR:
		{
			TCHAR* pTitle = GetTxtFromMgr(10303);
			TCHAR* pText = GetTxtFromMgr(345);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_FAIL_NOTRANSFROMITEM :
		{
			TCHAR* pTitle = GetTxtFromMgr(10039);
			TCHAR* pText = GetTxtFromMgr(10304);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	
	//[추가 : 황진성 2008. 1. 11 => 인던 메시지 추가.]
	case SRVAL_FAIL_INSTANTDUNGEON_NO_TICKET:
		{
			TCHAR* pTitle = GetTxtFromMgr(7825);
			TCHAR* pText  = GetTxtFromMgr(7831);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	//[추가 : 황진성 2008. 1. 15 => 인던에 들어간 나의 길드가 없으면 받는 메시지.]
	case SRVAL_FAIL_INSTANTDUNGEON_NO_GUILD:
		{
			TCHAR* pTitle = GetTxtFromMgr(7825);
			TCHAR* pText  = GetTxtFromMgr(7834);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	//[추가 : 황진성 2008. 1. 15 => 인던에 들어간 길드원의 수가 초과 되면 받는 메시지.]
	case SRVAL_FAIL_INSTANTDUNGEON_OVER_COUNT_GUILD:
		{
			TCHAR* pTitle = GetTxtFromMgr(7825);
			TCHAR* pText  = GetTxtFromMgr(7833);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
		//[추가 : 황진성 2008. 1. 15 => 내가 길드가 없어서 입장 못 받는 메시지.]
	case SRVAL_FAIL_INSTANTDUNGEON_MY_NO_GUILD:
		{
			TCHAR* pTitle = GetTxtFromMgr(7825);
			TCHAR* pText  = GetTxtFromMgr(7867);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_SUCCESS_INSTANTDUNGEON_SET_BESTGUILD:
		{
			SI32 id = pclCM->GetIDFromCharUnique(pclreturn->siPara1);
			if( pclCM->CR[1]->GetCharUnique() == pclreturn->siPara1 )
			{
				// 내 정보 업데이트
				pclCM->CR[1]->uiBestGuild = pclreturn->siPara2;
			}
			else
			{
				// 상대방 정보 업데이트
				if( pclCM->IsValidID(id) )
				{
					pclCM->CR[id]->uiBestGuild = pclreturn->siPara2;
				}
			}
		}
		break;
	case SRVAL_INSTANTDUNGEON_MAKEGATE:
		{
			NTCHARString128	kTitle( GetTxtFromMgr(7825) );
			NTCHARString128	kText( GetTxtFromMgr(7840) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	case SRVAL_GIANTRACCOONRESURRECTEVENT_GIVE_ITEM:
		{
			cltCharClient*  pclChar = pclCM->GetCharClient( pclCM->GetIDFromCharKind( pclClient->GetUniqueFromHash("KIND_POTRACCOON") ) );
			if( pclChar )	pclChar->pclDrawChat->Set( pclDrawChatSurface, GetTxtFromMgr(40853), false, NPC_CHATTING ); 
		}
		break;;
	//[추가 : 황진성 2008. 1. 22 => 흑의 군단 개인 보상 메시지 추가.]
	case SRVAL_BLACKWAR_CHAR_REWARD:
		{
			NTCHARString128	kTitle( GetTxtFromMgr(7880) );
			NTCHARString128	kText( GetTxtFromMgr(7881) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	//[추가 : 황진성 2008. 1. 22 => 흑의 군단 마을 보상 메시지 추가.]
	case SRVAL_BLACKWAR_VILLAGE_REWARD:
		{   
			NTCHARString128	kTitle( GetTxtFromMgr(7880) );
			NTCHARString128	kText( GetTxtFromMgr(7882) );
			kText.Replace("#RewardMoney#", SI32ToString(pclreturn->siPara2));

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}	
		break;
	//[추가 : 황진성 2008. 2. 4 =>  공간 상자에서 아이템 이동중 해당 이동할 인벤에 넣을 곳이 없다면.]
	case SRVAL_SPACE_BOX_INVEN_FULL:
		{
			CNSpaceBoxDlg* pSpaceBox = (CNSpaceBoxDlg*)m_pDialog[ NSPACE_BOX ];

			if( NULL == pSpaceBox )
			{
				return;
			}

			if( true == pSpaceBox->IsCreate() )
			{
				pSpaceBox->TextMsg(CNSpaceBoxDlg::MSG_FULL_BOX);
			}
		}
		break;
	//[추가 : 황진성 2008. 2. 4 =>  공간 상자 시간 종료 메시지.]
	case SRVAL_SPACE_BOX_TIME_OVER:
		{
			CNSpaceBoxDlg* pSpaceBox = (CNSpaceBoxDlg*)m_pDialog[ NSPACE_BOX ];

			if( NULL == pSpaceBox )
			{
				return;
			}

			if( true == pSpaceBox->IsCreate() )
			{
				pSpaceBox->TextMsg(CNSpaceBoxDlg::MSG_TIME_OVER_BOX);
			}
		}
		break;
	//[추가 : 황진성 2008. 2. 20 => 주주총회 발동 가능, 불가능 정보를 보이라는 메시지 받는다. ]
	case SRVAL_GENERAL_MEETING_SUGGEST_ON_OFF_INFO:
		{
			CreateInterface( NINFO_DLG );	
			
			cltNInfoDlg* GeneralMeetingSuggestInfo = (cltNInfoDlg*)m_pDialog[ NINFO_DLG ];

			if(GeneralMeetingSuggestInfo)
				GeneralMeetingSuggestInfo->GeneralMeetingSuggestInfo();
		}
		break;
	//[추가 : 황진성 2008. 2. 20 => 주주총회 발동이 불가능하여 받은 메시지 . ]
	case SRVAL_GENERAL_MEETING_SUGGEST_OFF_FAIL:
		{
			NTCHARString128	kTitle( GetTxtFromMgr(5614) );
			NTCHARString128	kText( GetTxtFromMgr(7957) );
			
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [영훈] CRC체크 실패
	case SRVAL_FAIL_FILECRCCHECK:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 3168 ) );
			NTCHARString128	kText( GetTxtFromMgr( 7980 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [영훈] 해적선 : 파티가 아니면 입장할 수 없음
	case SRVAL_FAIL_INSTANTDUNGEON_NO_PARTY:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 7981 ) );
			NTCHARString128	kText( GetTxtFromMgr( 7982 ) );

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [영훈] 해적선 : 인원이 가득차 입장 할 수 없음
	case SRVAL_FAIL_INSTANTDUNGEON_OVER_PARTY_MAP:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 7981 ) );
			NTCHARString128	kText( GetTxtFromMgr( 7834 ) );

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [영훈] 해적선 : 해적선 맵을 클리어해서 아이템을 받음
	case SRVAL_INSTANTDUNGEON_CLEAR_REWARD_ITEM:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 7981 ) );
			NTCHARString128	kText( GetTxtFromMgr( 8078 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [영훈] PC방 : PC방이라서 아이템을 지급해 준다는 메시지
	case SRVAL_REWARDITEM_FOR_PCROOM:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 5614 ) );
			NTCHARString128	kText( GetTxtFromMgr( 7987 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [영훈] 출석체크 : 출석체크 이벤트 알림
	case SRVAL_NOTICE_SEASON2_ATTEND:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 511 ) );
			NTCHARString256	kText( GetTxtFromMgr( 7989 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [영훈] 출석체크 : 아이템 보상 지급 실패
	case SRVAL_FAIL_SEASON2_ATTEND_REWARDITEM:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 7996 ) );
			NTCHARString128	kText( GetTxtFromMgr( 7998 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [영훈] 출석체크 : 아이템 보상 지급 성공
	case SRVAL_SUCCESS_SEASON2_ATTEND_REWARDITEM:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 7995 ) );
			NTCHARString128	kText( GetTxtFromMgr( 7997 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// 연금술사로 아이템 능력치 변경 성공
	case SRVAL_CHANGE_ITEM_FROM_NPC:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 389 ) );
			NTCHARString128	kText( GetTxtFromMgr( 8426 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}break;
	//[진성] 아티펙트. => 2008. 3. 22
	case SRVAL_FAIL_ARTIFACT:
		{
			// [추가 : 황진성 2008. 3. 5 => 아티펙트 관련창이 떠있는데 인벤이동이 있었다면 아티펙트 관련 정보를 모두 초기화 시키자.]
			if( m_pDialog[ARTIFACT_BASE_DLG] )
			{
				((NArtifactBaseDlg*)m_pDialog[ARTIFACT_BASE_DLG])->Clear();
			}

			NTCHARString64	kTitle( GetTxtFromMgr( 5614 ) );
			NTCHARString128	kText( GetTxtFromMgr( 845 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	case SRVAL_FAIL_ARTIFACT_REMOVE_GETITEM_NOTINV:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 5614 ) );
			NTCHARString128	kText( GetTxtFromMgr( 8380 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	//[진성] 파티홀. => 2008. 3. 22 
	case SRVAL_FAIL_PARTYHALL:
		{
			if(0 > pclreturn->siPara1 || PARTYHALL_MSG_NUM <= pclreturn->siPara1)
				 return;
			
			SI32 TextIndex = 0;

			switch(pclreturn->siPara1)
			{
				case PARTYHALL_ALREADY_USER:				TextIndex = 8146; break;
				case PARTYHALL_ETRANCE_RESERVATION:			TextIndex = 8147; break;
				case PARTYHALL_ETRANCE_RESERVATION_FAIL:	TextIndex = 8148; break;
				case PARTYHALL_ALREADY_RESERVATION:			TextIndex = 8149; break;
				case PARTYHALL_OPEN:						TextIndex = 8150; break;
				case PARTYHALL_CLOSE:						TextIndex = 8151; break;
			}
			if(0 >= TextIndex)
				return;

			NTCHARString64	kTitle( GetTxtFromMgr( 5614 ) );
			NTCHARString128	kText(  GetTxtFromMgr( TextIndex ) );
			
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	//[진성] 마을 이벤트 - 몬스터 습격. 2008-3-27
	case SRVAL_MONSTER_ATTACK_EVENT:
		{
			// 창을 띄워
			if(MONSTER_ATTACK_EVENT_START == pclreturn->siPara1)
			{
				if ( m_pDialog[NMONSTER_ATTACK_EVENT] == NULL )
				{							
					CreateInterface( NMONSTER_ATTACK_EVENT );
				}
			}
			// 창 닫어 
			else if(MONSTER_ATTACK_EVENT_END_SUCCED == pclreturn->siPara1)
			{
				// 별도 패킷으로 분리해 처리 더이상 이곳에선 처리 안한다.
				/*if ( m_pDialog[NMONSTER_ATTACK_EVENT] != NULL )
				{			
					DestroyInterface( m_pDialog[NMONSTER_ATTACK_EVENT] );
				}
				
				if( m_pDialog[ NNOTIFYGETEXPANDITEM_DLG ] == NULL )
					CreateInterface( NNOTIFYGETEXPANDITEM_DLG );

				((CNNotifyGetExpAndItemDlg*)m_pDialog[NNOTIFYGETEXPANDITEM_DLG])->MonsterAttackReward( siRewardItemUnique, siRewardItemNum, pclreturn->siPara2 ); */
			}
			// 창 닫어 
			else if(MONSTER_ATTACK_EVENT_END_FAIL == pclreturn->siPara1)
			{
				if ( m_pDialog[NMONSTER_ATTACK_EVENT] != NULL )
				{			
					DestroyInterface( m_pDialog[NMONSTER_ATTACK_EVENT] );
				}

				NTCHARString64	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	kText(  GetTxtFromMgr( 8161 ) );
			
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
			}
			// 이미 다른마을에서 이벤트 중.
			else if(MONSTER_ATTACK_EVENT_ALREADY_RUN == pclreturn->siPara1)
			{
				NTCHARString64	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	kText(  GetTxtFromMgr( 8160 ) );
			
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
			}
			// 몬스터가 죽었다.
			else if(MONSTER_ATTACK_EVENT_MONSTER_DIE == pclreturn->siPara1)
			{
				if(0 <= pclreturn->siPara2 && CMonsterAttackEvent::MOSTER_NUM > pclreturn->siPara2)
				{
					if ( m_pDialog[NMONSTER_ATTACK_EVENT] != NULL )
					{							
						((NMonsterAttackEventDlg*)m_pDialog[NMONSTER_ATTACK_EVENT])->MonsterDie(pclreturn->siPara2);
					}
				}
			}
			else if(MONSTER_ATTACK_EVENT_REMAIN_TIME == pclreturn->siPara1)
			{
				if ( m_pDialog[NMONSTER_ATTACK_EVENT] != NULL )
				{							
					((NMonsterAttackEventDlg*)m_pDialog[NMONSTER_ATTACK_EVENT])->SetRemainSecondTime(pclreturn->siPara2);
				}
			}
			
		}
		break;

	//[강형] 이관 보상 아이템 사용시 메시지 출력
	case SRVAL_ITEMUSE_REWARDBONUS:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 5614 ) );
			NTCHARString512	kText( GetTxtFromMgr( pclreturn->siPara1 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);

			// 창을 임시로 크게 띄운다.
			/*m_pDialog[ NOTIFYMSG_DLG ]->SetWidth(m_pDialog[ NOTIFYMSG_DLG ]->GetWidth() * 3);
			m_pDialog[ NOTIFYMSG_DLG ]->SetHeight(m_pDialog[ NOTIFYMSG_DLG ]->GetHeight() * 2);

			SI32 xPos = ( GetScreenXSize() - m_pDialog[ NOTIFYMSG_DLG ]->GetWidth() ) / 2 + 40;
			SI32 yPos = ( GetScreenYSize() - m_pDialog[ NOTIFYMSG_DLG ]->GetHeight() ) / 2;

			m_pDialog[ NOTIFYMSG_DLG ]->MovePos( xPos, yPos );*/
		}
		break;

	case SRVAL_NAMINGQUEST_START_FAIL:
		{	
			if(m_pDialog[NNAMINGQUESTLIST_DLG])
			{
				cltClient* pclclient = (cltClient*)pclClient;
				((CNNamingQuestListDlg*)m_pDialog[NNAMINGQUESTLIST_DLG])->SetExplain(8565);
				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			}
		}
		break;
	case SRVAL_NAMINGQUEST_START_SUCCESS :
		{	
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0);
		}
		break;
	// [영훈] 현 지역에서는 마켓을 열 수 없음
	case SRVAL_FAIL_WARPTO_NOTMARKETOPEN_MAP:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 512 ) );
			NTCHARString128	kText( GetTxtFromMgr( 8283 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [영훈] 군주 투표소 지역으로 이동할것인지 물어본다
	case SRVAL_NOTICE_WARPTOPOLL_MAP:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 8284 ) );
			NTCHARString128	kText( GetTxtFromMgr( 8285 ) );


			stMsgBox MsgBox;
			MsgBox.Set( m_pDialog[NOTIFYMSG_DLG], kTitle, kText, MSGBOX_TYPE_YESNO, SRVAL_NOTICE_WARPTOPOLL_MAP );
			SetMsgBox( &MsgBox, NULL, 0 );
		}
		break;
	// [영훈] 이미 투표를 끝마쳤다
	case SRVAL_FAIL_REASON_ALREADYVOTE:
		{
			NTCHARString64	kTitle( GetTxtFromMgr(512) );
			NTCHARString64	kText( GetTxtFromMgr(15232) );


			m_pDialog[NOTIFYMSG_DLG]->SetText( kTitle, kText );
		}
		break;
	// [영훈] 투표할 수 있는 레벨에 도달하지 못했다
	case SRVAL_FAIL_REASON_DONOTARRIVELEVLE:
		{
			NTCHARString64	kTitle( GetTxtFromMgr(512) );
			NTCHARString64	kText( GetTxtFromMgr(8287) );


			kText.Replace( TEXT("#level#"), SI32ToString( Const_CanVote_MinLevel ) );

			m_pDialog[NOTIFYMSG_DLG]->SetText( kTitle, kText );
		}
		break;
	// [영훈] 투표기간이 아니라서 어깨띠를 사지 못한다
	case SRVAL_FAIL_DONOTBUY_ELECTIONSTUMPITEM:
		{
			NTCHARString64	kTitle( GetTxtFromMgr(5614) );
			NTCHARString128	kText( GetTxtFromMgr(8265) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [영훈] 보상아이템을 받지못해 출석 도장을 찍을 수 없음
	case SRVAL_FAIL_SEASON2_ATTEND_NOTGIVEITEM:
		{
			NTCHARString64	kTitle( GetTxtFromMgr(5614) );
			NTCHARString128	kText( GetTxtFromMgr(8430) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;

	// [진성] 매도가능한 주식수를 초과해서 클라에서 받을 메시지 => 2008-5-29
	case SRVAL_OVER_STOCK_ORDER:
		{
			NTCHARString64	kTitle(GetTxtFromMgr(5614));
			NTCHARString128	kText(GetTxtFromMgr(8448));

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;

	// [진성] 개인상점 모드 변경 오류 메시지 => 2008-5-29
	case SRVAL_PERSONAL_SHOP_MODE_CHANGE_FAIL:
		{
			NTCHARString64	kTitle(GetTxtFromMgr(5614));
			NTCHARString128	kText(GetTxtFromMgr(8449));

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [진성] 서버 이베트 랭킹 1위임을 알림. => 2008-6-24
	case SRVAL_SERVER_EVENT_RANK_1:
		{
			if(m_pDialog[SERVER_EVENT_INSTANCE_MAP_BONUS] == NULL)	
				CreateInterface(SERVER_EVENT_INSTANCE_MAP_BONUS);

			((NServerInstanceMapBounusDlg*)m_pDialog[SERVER_EVENT_INSTANCE_MAP_BONUS])->SetDailyBestServerNotice();
		}
		break;
	// [진성] 서버 이벤트 랭킹 결정 기간 종료. => 2008-6-24
	case SRVAL_SERVER_EVENT_END:
		{
			if ( m_pDialog[SERVER_EVENT_RANK_DLG] != NULL )
			{			
				DestroyInterface( m_pDialog[SERVER_EVENT_RANK_DLG] );
			}
			if ( m_pDialog[SERVER_EVENT_QUEST_DLG] != NULL )
			{			
				DestroyInterface( m_pDialog[SERVER_EVENT_QUEST_DLG] );
			}

			pclServerEventManager->SetBonusBufType( pclreturn->siPara1 );
		}
		break;
	// [진성] 서버 이벤트 버프. => 2008-6-24
	case SRVAL_SERVER_EVENT_BUF:
		{
			cltClient *pclclient = (cltClient*)pclClient;

			pclclient->pclServerEventManager->SetBonusBufType(pclreturn->siPara1);
		}
		break;

	case SRVAL_NOTICE_OPENTHE_SERVEREVENTGATE:
		{
			cltClient *pclclient = (cltClient*)pclClient;
			if ( NULL == pclclient )
			{
				return;
			}

			if ( NULL == m_pDialog[ SERVER_EVENT_INSTANCE_MAP_BONUS ] )
			{
				CreateInterface( SERVER_EVENT_INSTANCE_MAP_BONUS );
			}

			NServerInstanceMapBounusDlg* pclBonusDlg = (NServerInstanceMapBounusDlg*)m_pDialog[ SERVER_EVENT_INSTANCE_MAP_BONUS ];
			if ( NULL == pclBonusDlg )
			{
				return;
			}

			pclBonusDlg->SetGateState( true );
		}
		break;

	case SRVAL_NOTICE_CLOSETHE_SERVEREVENTGATE:
		{
			cltClient *pclclient = (cltClient*)pclClient;
			if ( NULL == pclclient )
			{
				return;
			}

			if ( NULL == m_pDialog[ SERVER_EVENT_INSTANCE_MAP_BONUS ] )
			{
				CreateInterface( SERVER_EVENT_INSTANCE_MAP_BONUS );
			}
			else
			{
				return;
			}

			NServerInstanceMapBounusDlg* pclBonusDlg = (NServerInstanceMapBounusDlg*)m_pDialog[ SERVER_EVENT_INSTANCE_MAP_BONUS ];
			if ( NULL == pclBonusDlg )
			{
				return;
			}

			pclBonusDlg->SetGateState( false );

		}
		break;

	//[진성] 네잎크로버가 현재 사용할수 없는 거면 꺼내지 몬한다. => 2008-8-14
	case SRVAL_RETURN_NOT_USE_FOURLEAF:
		{
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(8753) );
		}	
		break;

	//[진성] 이벤트 시간이 지나서 아이템을 만들 수 없다. => 2008-9-8
	case SRVAL_EVENTDAY_OVER_NOT_MAKE_ITEM:
		{
			TCHAR* pItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclreturn->siPara1 );

			NTCHARString128 kMessage( GetTxtFromMgr(8814) );
			kMessage.Replace( TEXT("#itemname#"), pItemName );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), kMessage );
		}	
		break;
	//[희영] 인벤토리가 꽉 찼다.		
	case SRVAL_CANNOTINPUTTOINV:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 512 ) );
			NTCHARString128	kText( GetTxtFromMgr( 1368 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	case SRVAL_GIVE_MENTOR_BUF:
		{
			NTCHARString64	kTitle( GetTxtFromMgr( 5614 ) );
			NTCHARString128	kText( GetTxtFromMgr( 8918 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	case SRVAL_VILLAGE_RESIDENT_INVITE:
		{
			TCHAR* pTitle =  GetTxtFromMgr( 5614 );
			TCHAR* pText  =  NULL;

			switch(pclreturn->siPara1)
			{
				case 0:	
					{
						pText =  GetTxtFromMgr( 8920 );
					}
					break;
				case 1:
					{
						pText =  GetTxtFromMgr( 8921 );	
					}
					break;
			}
			
			if( pText )		m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_VILLAGE_RESIDENT_RESULT:
		{
			TCHAR* pTitle =  GetTxtFromMgr( 5614 );
			TCHAR* pText  =  NULL;

			switch(pclreturn->siPara1)
			{
				case SRVAL_CITYHALL_SUCCESSAPPLYRESIDENT:	pText =  GetTxtFromMgr( 15200 );	break;
				case SRVAL_CITYHALL_FAILALREADYAPPLY:		pText =  GetTxtFromMgr( 15201 );	break;
				case SRVAL_CITYHALL_FAILALREADYRESIDENT:	pText =  GetTxtFromMgr( 15202 );	break;
				case SRVAL_CITYHALL_FAILEXCEEDAPPLYER:		pText =  GetTxtFromMgr( 15203 );	break;
				//case SRVAL_CITYHALL_FAILTIMELIMIT:			pText =  GetTxtFromMgr( 8920 );		break;
			}

			if( pText )		m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_VILLAGE_HELPER:
		{
			TCHAR* pTitle =  GetTxtFromMgr( 5614 );
			TCHAR* pText  =  NULL;
			//int a = 0;
			switch(pclreturn->siPara1)
			{
				case SRVAL_VILLAGE_HELPER_ALREADY_QUESTION:	
					{
						if( 0 < pclreturn->siPara2 )
						{
							if(NULL == m_pDialog[NHELPER_QUESTION_OR_ANSWER_DLG])
							{
								CreateInterface( NYESORNODLG );
							}

							CNYesOrNoDlg* pDialog = (CNYesOrNoDlg*)m_pDialog[NYESORNODLG];
							if( NULL == pDialog ) return;
	
							pDialog->SetHelperAlreadyQuestion();
						}
						else
						{
							if(NULL == m_pDialog[NHELPER_QUESTION_OR_ANSWER_DLG])
							{
								CreateInterface( NHELPER_QUESTION_OR_ANSWER_DLG );
							}
							
							NHelperQuestionOrAnswerDlg* pDialog = (NHelperQuestionOrAnswerDlg*)m_pDialog[NHELPER_QUESTION_OR_ANSWER_DLG];
							if( NULL == pDialog ) return;

							pDialog->SetQuestionDlg( QUESTION_DLG );
						}
					}
					break;
				case SRVAL_VILLAGE_HELPER_ALREADY_ANSWER_ME:	pText = GetTxtFromMgr( 9623 );	break;
				case SRVAL_VILLAGE_HELPER_ALREADY_ANSWER_OTHER:	pText = GetTxtFromMgr( 9619 );	break;	
				case SRVAL_VILLAGE_HELPER_QUESTION_ADD_SUCCESS:	
					{
						if(m_pDialog[NHELPER_QUESTION_OR_ANSWER_DLG])
							m_pDialog[NHELPER_QUESTION_OR_ANSWER_DLG]->DeleteDialog();

						pText = GetTxtFromMgr( 9620 );	break;	
					}
					break;
				//case SRVAL_VILLAGE_HELPER_QUESTION_ADD_FAIL:	a = SRVAL_VILLAGE_HELPER_QUESTION_ADD_FAIL;		break;
				case SRVAL_VILLAGE_HELPER_QUESTION_DEL_SUCCESS:	pText = GetTxtFromMgr( 9616 );	break;	
				case SRVAL_VILLAGE_HELPER_ANSWER_ADD_SUCCESS:	pText = GetTxtFromMgr( 9620 );	break;
				case SRVAL_VILLAGE_HELPER_ANSWER_ADD_FAIL:		pText = GetTxtFromMgr( 9621 );	break;
				case SRVAL_VILLAGE_HELPER_QUESTION_FULL:		pText = GetTxtFromMgr( 9618 );	break;
				case SRVAL_VILLAGE_HELPER_ALREADY_DEL_QUESTION:	
					{
						if(m_pDialog[NHELPER_QUESTION_OR_ANSWER_DLG])
							m_pDialog[NHELPER_QUESTION_OR_ANSWER_DLG]->DeleteDialog();

						pText = GetTxtFromMgr( 9622 );	
					}
					break;
				case SRVAL_VILLAGE_HELPER_LIMIT_COUNT_QUESTION:	pText = GetTxtFromMgr( 9120 );	break;	
				case SRVAL_VILLAGE_HELPER_ANSWER_SEND_SUCCESS:  
					{
						if(m_pDialog[NHELPER_QUESTION_OR_ANSWER_DLG])
							m_pDialog[NHELPER_QUESTION_OR_ANSWER_DLG]->DeleteDialog();

						pText = GetTxtFromMgr( 9625 );		
					}
					break;
			}

			if( pText )		m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_DIVINATION_MSG:
		{
			if(m_pDialog[SERVER_EVENT_INSTANCE_MAP_BONUS] == NULL)	
				CreateInterface(SERVER_EVENT_INSTANCE_MAP_BONUS);

			NServerInstanceMapBounusDlg* pDialog = (NServerInstanceMapBounusDlg*)m_pDialog[ SERVER_EVENT_INSTANCE_MAP_BONUS ];
			SI32	siRand = 9679 + (rand() %40);

			pDialog->SetMessage(siRand);
			
		}
		break;

	case SRVAL_ELIXIR_CHARGE_ITEM_NUM_ZERO_FAIL:
		{
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(GetTxtFromMgr( 5614 ), "0개라 교환이 안된다.");	
		}
		break;
	
	case SRVAL_PROMOTER_NOT_CELLULAR_PHONE_AUTH:
		{
			if( NULL == m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ] ) 
				CreateInterface(NPROMOTER_GIVE_ITEM_NOTICE);

			// 추천인.
			//if( pclreturn->siPara1	)
				//((NPromoter_Give_Item_Notice*)m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ])->SetRecommendText( pclreturn->siPara1, pclreturn->siPara2 );	
		}
		break;		
	case SRVAL_PROMOTER_CHAR_LEVELUP_MINI_BUTTON:
		{
			if( NULL == m_pDialog[ NPROMOTER_MINI ] ) 
				CreateInterface(NPROMOTER_MINI);
		}
		break;

	case SRVAL_NEWMARKET_BUY_FAIL:
		{
			char* ptitle = GetTxtFromMgr(7773);
			char* ptext = NULL ;

			switch(pclreturn->siPara1)
			{
			case 100:		// 렉으로 실패. 
				ptext = GetTxtFromMgr(196);
				break;
			/*case 200:
				ptext = GetTxtFromMgr(7774);
				break;*/
			case 300:
				ptext = GetTxtFromMgr(198);
				break;
			case 400:
				ptext = GetTxtFromMgr(199);
				break;

			case 500:
				ptext = GetTxtFromMgr(200);
				break;

			case 550:
				ptext = GetTxtFromMgr(201);
				break;

			case 600:
				ptext = GetTxtFromMgr(9834);
				break;
			case 700:
				ptext = GetTxtFromMgr(203);
				break;

			case 800:
				ptext = GetTxtFromMgr(204);
				break;

			case 900:
				ptext = GetTxtFromMgr(205);
				break;

			case 1000:
				ptext = GetTxtFromMgr(206);
				break;

			case 1200:
				ptext = GetTxtFromMgr(207);
				break;

			case 1300:
				ptext = GetTxtFromMgr(208);
				break;
			default:
				m_pDialog[ NOTIFYMSG_DLG ]->SetText(ptitle,"ErrorCode:%d", pclreturn->siPara1);
				break;
			}

			if ( !ptext ) break ;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(ptitle, ptext);
		}
		break;
	
	case SRVAL_NEWMARKET_REPAIR_FAIL:
		{
			switch(pclreturn->siPara1)
			{
				case 100:	m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(1429), GetTxtFromMgr(9927) );
					break;
			}
		}
		break;
	case SRVAL_REPAIR_REQUEST_FAIL:
		{
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(40190) );
		}
		break;
	case SRVAL_GIANTRACCOONRESURRECTEVENT_NO_TIME:
		{
			//m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(40397) );
			((NGiantRaccoonResurrectEventDlg*)m_pDialog[ NGIANTRACCOONRESURRECTEVENTDLG ])->SetExplainText( GetTxtFromMgr(40397) );
		}
		break;
	case SRVAL_GIANTRACCOONRESURRECTEVENT_ALREAY_MAKE_NPC:
		{
			//m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(40814) );
			((NGiantRaccoonResurrectEventDlg*)m_pDialog[ NGIANTRACCOONRESURRECTEVENTDLG ])->SetExplainText( GetTxtFromMgr(40814) );
		}
		break;
	case SRVAL_GIANTRACCOONRESURRECTEVENT_EVENT_START:
		{
			m_pDialog[ NNEWNOTIFY_DLG ]->SetText( GetTxtFromMgr(40203), GetTxtFromMgr(40396) );	
			((CNNewNotifyDlg*)m_pDialog[ NNEWNOTIFY_DLG ])->SetImage( pclClient->GetUniqueFromHash( TEXT("KIND_POTRACCOON") ) );
		}
		break;	
	case SRVAL_GIANTRACCOONRESURRECTEVENT_FAIL_USE_ITEM:
		{
			m_pDialog[ NNEWNOTIFY_DLG ]->SetText( GetTxtFromMgr(40203), GetTxtFromMgr(40398) );	
			((CNNewNotifyDlg*)m_pDialog[ NNEWNOTIFY_DLG ])->SetImage( pclClient->GetUniqueFromHash( TEXT("KIND_POTRACCOON") ) );
		}
		break;	
	case SRVAL_TRANSFORM_SYNCRO_UP:
		{			
			switch(pclreturn->siPara1)
			{
				case 100:	
					{
						m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(30001) );	//	[종호][수정] 100%가 되었다면 30001 Text가 보여져야 한다.
					}
					break;
				case 200:
					{
						NTCHARString64	strTitle( GetTxtFromMgr(5614) );
						NTCHARString256	strText( GetTxtFromMgr(30000) );

						cltTransFormInfo* pTransformInfo = &pclClient->pclCM->CR[1]->clTransFormInfo;
						REAL32			  siTransFormSyc =  pTransformInfo->GetSynchroTransForm(pTransformInfo->uiNowTransFormKind);

						/*
						NTCHARStringBuilder kBuilder;
						NTCHARString32	kSynchro;
						kBuilder.SetFormat("{1} %") << (const TCHAR*)REAL32ToString(siTransFormSyc, 6, 2);
						kSynchro = kBuilder;

						strText.Replace( TEXT("#percent#"),	kSynchro );
						*/

						NTCHARString64 kSynchro = TEXT("");
						kSynchro.FormatString(  TEXT("%d"), (SI32)siTransFormSyc );
						strText.Replace( TEXT("#percent#"),	kSynchro );
						
						
																		
						m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
					}
					break;
			}
		}
		break;
		// 성웅 길드전 획득 방식 변경
	case SRVAL_RETURN_BEFORE_REG_MAPWAR :
		{
			TCHAR* pTitle = GetTxtFromMgr(428);
			TCHAR* pText = GetTxtFromMgr(9966);// 이미 길드전신청을 하였습니다.

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_RETURN_REG_CANTSTATUSGUILD :
		{
			TCHAR* pTitle = GetTxtFromMgr(428);
			TCHAR* pText = GetTxtFromMgr(9952);// 사냥터를 신청하기 위해서는 길드레벨4, 길드원 5명 이상이 되어야 합니다.

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_RETURN_NOT_REG_TIME :
		{
			TCHAR* pTitle = GetTxtFromMgr(428);
			TCHAR* pText = GetTxtFromMgr(9970);	//9970	길드전을 신청할수 없는 시간입니다

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_RETURN_CANNOTGUILDWAR_NOGUILD :	// 전투가능한 길드가 없습니다.
		{
			TCHAR* pTitle = GetTxtFromMgr(428);
			TCHAR* pText = GetTxtFromMgr(10328);	//10328	전투가능한 길드가 없습니다.

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_RETURN_CANNOTGUILDWAR_RESEASON :	// 길드전이 초기화 되었다 새롭게 등록해라.
		{
			TCHAR* pTitle = TEXT("");
			TCHAR* pText = GetTxtFromMgr(10329);	// 10329 길드전이 초기화 되었다 새롭게 등록해라.

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_RETURN_NOTMASTER :
		{
			TCHAR* pTitle = GetTxtFromMgr(428);
			TCHAR* pText = GetTxtFromMgr(9967);//길드마스터가 아닙니다.

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;
	case SRVAL_RETURN_NOT_RED_HUTMAP :
		{
			TCHAR* pTitle = GetTxtFromMgr(428);
			TCHAR* pText = GetTxtFromMgr(9968);//길드전을 신청하기 전에 소유할 맵을 신청 해야합니다

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		}
		break;

	case SRVAL_FAIL_PROPOSE_IS_NOT_VALIDCHAR:
		{
			NTCHARString64	strTitle( GetTxtFromMgr(422) );
			NTCHARString256 strText( GetTxtFromMgr(9980) );

			m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		}
		break;

	case SRVAL_FAIL_PROPOSE_ALREADY_MARRIED:
		{
			NTCHARString64	strTitle( GetTxtFromMgr(422) );
			NTCHARString256 strText( GetTxtFromMgr(9982) );

			m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		}
		break;

	case SRVAL_FAIL_PROPOSE_LESSTHAN_MARRY_LEVEL:
		{
			NTCHARString64	strTitle( GetTxtFromMgr(422) );
			NTCHARString256 strText( GetTxtFromMgr(9983) );

			strText.Replace( TEXT("#level#"), SI32ToString(Const_Marriage_Min_Level) );

			m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		}
		break;

	case SRVAL_FAIL_PROPOSE_SAME_SEX:
		{
			NTCHARString64	strTitle( GetTxtFromMgr(422) );
			NTCHARString256 strText( GetTxtFromMgr(9981) );

			m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		}
		break;

	case SRVAL_FAIL_PROPOSE_ANSWER_NO:
		{
			NTCHARString64	strTitle( GetTxtFromMgr(422) );
			NTCHARString256 strText( GetTxtFromMgr(9987) );

			m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		}
		break;

	case SRVAL_SET_MARRY_WITHSAMEMAP:
		{
			cltCharClient* pclchar = pclCM->GetCharClient( 1 );
			if ( NULL == pclchar )
			{
				break;
			}
			
			if ( NULL == pclchar->pclMarriage )
			{
				break;
			}

			pclchar->pclMarriage->SetConnectMode( pclreturn->siPara1 );
			pclchar->pclMarriage->SetLoveLevel( pclreturn->siPara2 );

			// 같은 맵에 접속된 경우 애정도 창을 띄운다
			if ( MARRIAGE_MATE_CONNECT_MODE_SAMEMAP == pclchar->pclMarriage->GetConnectMode() )
			{
				if ( NULL == m_pDialog[NMARRIAGELOVELEVEL_DLG] )
				{
					CreateInterface( NMARRIAGELOVELEVEL_DLG );
				}
			}
			// 그렇지 않은 경우는 창을 닫는다
			else
			{
				if ( m_pDialog[NMARRIAGELOVELEVEL_DLG] )
				{
					m_pDialog[NMARRIAGELOVELEVEL_DLG]->DeleteDialog();
				}
			}

		}
		break;

	case SRVAL_REQUEST_WEDDING_ITEMGET:
		{
			// 결혼 기념일 아이템 받기 창 띄운다
			if ( NULL == m_pDialog[NWEDDINGANNIVERSARY_DLG] ) 
			{
				CreateInterface( NWEDDINGANNIVERSARY_DLG );
			}
		}
		break;

	case SRVAL_WEDDING_HALL_OUT_NOTICE:
		{
			NTCHARString64	strTitle( TEXT("") );
			NTCHARString256 strText( GetTxtFromMgr(30079) );

			m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		}
		break;

	case SRVAL_WEDDING_GIVEITEM_QUESTION:
		{
			NTCHARString128 strText = GetTxtFromMgr(30077);

			stMsgBox MsgBox;
			MsgBox.Set( m_pDialog[NOTIFYMSG_DLG], TEXT(""), strText, MSGBOX_TYPE_OK, SRVAL_WEDDING_GIVEITEM_QUESTION );
			SetMsgBox( &MsgBox, NULL, 0 );
		}
		break;

	case SRVAL_RETURN_LOSTWORLDQUEST_COMPLETE:
		{
			NTCHARString64	strTitle( TEXT("") );
			NTCHARString256 strText( GetTxtFromMgr(30040) );

			m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		}
		break;
	case SRVAL_RETURN_SOULGUARD_SHOT_MAGIC:
		{
			cltClient* pclclient = (cltClient*)pclClient;

			
			pclclient->pclOrderManager->clOrder.MakeOrder(
				ORDER_MAGIC, 
				pclclient->pclUnderMouseObject->MapX, pclclient->pclUnderMouseObject->MapY,
				pclclient->pclCM->GetIDFromCharUnique(pclreturn->siPara1), pclreturn->siPara2, 0);

			pclclient->pclUnderMouseObject;
		}
		break;
		// 라쿤피하기 이벤트//Switch_10MonthEvent_RacoonDodge
	case SRVAL_RACOONDODGEEVENT_NOTEMPTYLISTPLACE: // 빈자리가 없다
		{
			NTCHARString128 kMessage( GetTxtFromMgr(40024) );
			TCHAR* pTitle = TEXT("")	;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kMessage );
		}
		break;
	case SRVAL_RACOONDODGEEVENT_RESERVEPERSON: /// 이미 신청 하였다.//Switch_10MonthEvent_RacoonDodge
		{
			NTCHARString128 kMessage( GetTxtFromMgr(40025) );
			TCHAR* pTitle = TEXT("")	;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kMessage );
		}
		break;	
	case SRVAL_RACOONDODGEEVENT_CLEAR: // 클리어 하고 돈받는 메세지//Switch_10MonthEvent_RacoonDodge
		{
			NTCHARString128 kMessage( GetTxtFromMgr(40026) );
			TCHAR* pTitle = TEXT("")	;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kMessage );
		}
		break;
	case SRVAL_RACOONDODGEEVENT_CANNOTRESERVE_INPLAY: // 플레이 도중에는 예약불가능//Switch_10MonthEvent_RacoonDodge
		{
			NTCHARString128 kMessage( GetTxtFromMgr(40151) );
			TCHAR* pTitle = TEXT("")	;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kMessage );
		}
		break;
	case SRVAL_RACOONDODGEEVENT_NOTENOUGHMONEY:// 충분한 돈이 없다//Switch_10MonthEvent_RacoonDodge
		{
			NTCHARString128 kMessage( GetTxtFromMgr(39999) );
			TCHAR* pTitle = TEXT("")	;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kMessage );
		}
		break;		
	// [기형] 휴면계정시스템 : 장비 상자 2종 보급 알림
	case SRVAL_REWARDITEM_FOR_DORMANCYEQUIPBOX:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 40001 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40002 ) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	// [기형] 휴면계정시스템 : 아이템 구입 실패
	case SRVAL_FAIL_BUY_DORMANCYITEM:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 40091 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40110) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [기형] 인벤토리가 꽉차서 아이템 수령 실패
	case SRVAL_FAIL_IS_FULLIVEN:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 40003 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40004 ) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [기형] 휴면 장비 아이템의 창고 이동 실패
	case SRVAL_FAIL_MOVE_ITEM_INHOUSE:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 40137 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40136) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [기형] 휴면 포인트가 모자름
	case SRVAL_FAIL_NOT_ENOUGH_POINT:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 4313 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40146) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [기형] 경기장이 열리지 않음
	case SRVAL_BATTLEROYAL_NOTOPEND:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 7203 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 30401) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [기형] 전 경기 참가 후 다음 경기에 참가 불가
	case SRVAL_BATTLEROYAL_NOTREJOIN:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 7203 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40277) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [기형] 제조 중에는 참가 불가
	case SRVAL_BATTLEROYAL_NOTREADYFORMAKING:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 7203 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40278) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [기형] 소환수 소환중엔 참가 불가
	case SRVAL_BATTLEROYAL_NOTREADYFORSUMMON:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 7203 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 30408) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [기형] 참가자가 꽉찬 경우 참가 불가
	case SRVAL_BATTLEROYAL_NOTREADYFORFULLEDGAMER:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 7203 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 30402) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [기형] 레벨이 맞지 않다면 참가 불가
	case SRVAL_BATTLEROYAL_NOTPROPERLEVEL:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 7203 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 30403) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
	case SRVAL_BATTLEROYAL_JOINPARTY:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 7203 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 30420) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [성웅] 지정된 시간에 자이언트 라쿤 이벤트가 설정 되었습니다
	case SRVAL_SUCCES_SET_GOONZURACCOONEVENT:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 2045 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40267) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [성웅] 같은 시간대에는 자이언트 라쿤 이벤트 설정이 불가능합니다 
	case SRVAL_FAIL_EVENTSET_SAMETIME:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 393 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40268) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [성웅] 모든 설정이 완료되지 않았습니다.
	case SRVAL_FAIL_EVENTSET_NOT_ENOUGH_TIME:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 393 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40269) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [성웅] 자이언트 라쿤 이벤트 설정은 최초 설정 후 24시간이 지나야 가능 합니다.
	case SRVAL_FAIL_EVENTSET_NOT_OVER_LIMITTIME:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 393 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40270) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [성웅] 군주 월드는 18세 이하는 출입 불가능 
	case SRVAL_FAIL_GORACCOONWORLD_UNDER_18:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 512 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40850) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;
		// [성웅] 해당지역으로는 워프 할수 없습니다 
	case SRVAL_FAIL_GORACCOONWORLD_WARP:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 512 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 40424) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
			
		}
		break;

	case SRVAL_NOTICE_CLOSE_PERSONALSHOP_BY_WARP:
		{
			NTCHARString128		kTitle( GetTxtFromMgr( 5614 ) );
			NTCHARString1024	kText( GetTxtFromMgr( 30535 ) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText);
		}
		break;

	case SRVAL_RETURN_GAWIBAWIBO_SRPWIN:
		{
			TCHAR strText[256] = {0,};
			StringCchPrintf(strText, 256, "Always Win Status: %d", pclreturn->siPara1);

			m_pDialog[NOTIFYMSG_DLG]->SetText(GetTxtFromMgr(5614), TEXT(strText));
		}
		break;

	case SRVAL_RETURN_GAWIBAWIBO_SRPLIMIT:
		{
			TCHAR strText[256] = {0,};
			StringCchPrintf(strText, 256, "Limit Wins Number : %d", pclreturn->siPara1);

			m_pDialog[NOTIFYMSG_DLG]->SetText(GetTxtFromMgr(5614), TEXT(strText));
		}
		break;
		
	case SRVAL_RETURN_SUMMON_TRANSFORM_USE_BONUS:
		{
			NTCHARString128	kText( GetTxtFromMgr( 40180 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), kText );
		}
		break;
	case SRVAL_RETURN_SUMMON_TRANSFORM_USE_STAMINA_ITEM:
		{
			NTCHARString128	kText( GetTxtFromMgr( 40181 ) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), kText );
		}
		break;
		// [춘기] 몬스터 생성 창을 띄운다.
	case SRVAL_RETURN_MONCREATE_DLG:
		{
			if(m_pDialog[ NMONSTERCREATE_DLG ]==NULL)
			{
				CreateInterface( NMONSTERCREATE_DLG );
			}
		}
		break;

		// [춘기] 장비 생성 창을 띄운다.
	case SRVAL_RETURN_EQUIPMENTCREATE_DLG:
		{
			if(m_pDialog[ NEQUIPMENTCREATE_DLG ]==NULL)
			{
				CreateInterface( NEQUIPMENTCREATE_DLG );
			}
		}
		break;

		// [춘기] /CMT 명령어로 몬스터 생성이 성공할 경우
	case SRVAL_RETURN_MONCREATE_DLG_SUCCESS:
		{
			NTCHARString128	strTitle	= GetTxtFromMgr(389);
			NTCHARString128	strText		= GetTxtFromMgr(40198);

			m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		}
		break;

		// [춘기] /SEQ 명령어로 장비 생성이 성공할 경우
	case SRVAL_RETURN_EQUIPMENTCREATE_DLG_SUCCESS:
		{
			NTCHARString128	strTitle	= GetTxtFromMgr(389);
			NTCHARString128	strText		= GetTxtFromMgr(40200);

			m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		}
		break;

		// [춘기] 크리스마스 이벤트 - 엔펠리스에 1단계 대형 크리스마스 트리가 세워졌을때
	case SRVAL_RETURN_WORLDCHRISTMASTREE_SUCCESS:
		{
			cltClient *pclclient = (cltClient*)pclClient;

			pclclient->siWorldTreeID[1] = 0;
			pclclient->siWorldTreeID[0] = pclreturn->siPara1;

			NTCHARString128	strText		= GetTxtFromMgr(20093);

			pclMessage->SetMsg(strText, 0, RGB(200, 200, 0) );
		}
		break;
	
		// [춘기] 크리스마스 이벤트 - 엔펠리스에 2단계 대형 크리스마스 트리가 세워졌을때
	case SRVAL_RETURN_WORLDCHRISTMASTREE2_SUCCESS:
		{
			cltClient *pclclient = (cltClient*)pclClient;

			pclclient->siWorldTreeID[0] = 0;
			pclclient->siWorldTreeID[1] = pclreturn->siPara1;
		}
		break;
	
		// [춘기] 크리스마스 이벤트 - 엔펠리스에 1단계대형 크리스마스 트리가 세워졌을때 (메시지 안날림)
	case SRVAL_RETURN_WORLDCHRISTMASTREE3_SUCCESS:
		{
			cltClient *pclclient = (cltClient*)pclClient;

			pclclient->siWorldTreeID[1] = 0;
			pclclient->siWorldTreeID[0] = pclreturn->siPara1;
		}
		break;

		// [춘기] 크리스마스 이벤트 - 엔펠리스에 대형 크리스마스 트리가 삭제되었을때 (메시지 안날림)
	case SRVAL_RETURN_WORLDCHRISTMASTREE_DEL:
		{
			cltClient *pclclient = (cltClient*)pclClient;

			pclclient->siWorldTreeID[1] = pclreturn->siPara1;
			pclclient->siWorldTreeID[0] = pclreturn->siPara2;
		}
		break;
	case SRVAL_HOUSE_FAILARRANGEITEM:
		{
			TCHAR * pTitle = GetTxtFromMgr(40377) ;
			TCHAR * pText = GetTxtFromMgr(40378) ;
			if ( !pTitle || !pText ) break ;

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);			
		}
		break;
	case SRVAL_VALENITNEDAY_ALREADY_BUF:
		{
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(10457), GetTxtFromMgr(10459) );			
		}
		break;
	case SRVAL_VALENITNEDAY_NOT_MYSELF:
		{
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(10457), GetTxtFromMgr(10465) );			
		}
		break;
	case SRVAL_VALENITNEDAY_NOT_USER:
		{
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(10457), GetTxtFromMgr(10465) );			
		}
		break;
	default:
		MsgBox(TEXT("fdjf83"), TEXT("%d"), pclreturn->siReturnValue);
	}
}

// 서버로부터의 결과값을 분석하여 적절한 곳에 통보한다. 
BOOL cltClient::OperateByServerResponseValue(const cltServerResponseValue* pclreturn)
{
	switch(pclreturn->siReceiver)
	{
	case 0:		
		OperateByServerResponseValueGlobal(pclreturn);		
		break;
	case RANKTYPE_CITYHALL:
		pclCityHallManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_BANK:
		pclBankManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_STOCK:
		pclStockManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_HUNT:
		pclHuntManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_MARKET:
		pclMarketManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_HOUSE:
		pclHouseManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_POSTOFFICE:
		pclPostOfficeManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_HORSEMARKET:
		pclHorseMarketManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_REALESTATEMARKET:
		pclRealEstateMarketManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_SUMMONMARKET:
		pclSummonMarketManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_LAND:
		pclLandManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_FEAST:
		pclFeastManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_SUMMONHEROMARKET:
		pclSummonHeroMarketManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_MINE:
		pclMineManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_GUILD:
		pclGuildManager->siServerResponseValue = pclreturn->siReturnValue;
		break;
	case RANKTYPE_NEWMARKET:
		pclNewMarketManager->siServerResponseValue = pclreturn->siReturnValue;
		if(pclreturn->siReturnValue == SRVAL_NEWMARKET_SUCCESSSETFEE)
		{
			pclNewMarketManager->clClientStrInfo.clNewMarketStrInfo.clFee.SetFeeRate(pclreturn->siPara1);
		}
		break;
	default:
		MsgBox(TEXT("OperateByServerResponseValue()"), TEXT("%d"), pclreturn->siReceiver);
	}
	return TRUE;
}

//KHY - 0910 - 클라이언트로 캐릭터형태의 리턴값을 보낸다.
// 서버로부터의 결과값을 분석하여 적절한 곳에 통보한다. 
BOOL cltClient::OperateByServerResponseCharValue(const cltServerResponseCharValue2* pclreturn)
{
	TCHAR Nbuffer[512];
	const SI32 constEffectSuccess = GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS"));
	const SI32 constEffectFailure = GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE"));

	static DWORD dwLastQuestCheckClock = 0;

	switch(pclreturn->siReturnValue)
	{
	case SRVAL_CHECK_WEBVAL:
		m_pclPICAManagerCheck->Request_PICAManager((TCHAR*)pclreturn->siPara1);
		break;

	//KHY - 0530 명나라 상인 구매제한.
	case SRVAL_BUYLIMITMONEYSET_FOREIGNMERCHANT:
		{
			TCHAR * pTitle = GetTxtFromMgr(235) ;
			TCHAR * pText = GetTxtFromMgr(8451) ;
			if ( !pTitle || !pText ) break ;

			if(((TCHAR*)pclreturn->siPara1 == NULL )||
			   ((TCHAR*)pclreturn->siPara2 == NULL )||
			   ((TCHAR*)pclreturn->siPara3 == NULL ))
				break;

			Nsprintf(Nbuffer, pText, TEXT("limitnum"), (TCHAR*)pclreturn->siPara1,TEXT("limitpercentup"), (TCHAR*)pclreturn->siPara2,TEXT("limitpercentdown"),(TCHAR*)pclreturn->siPara3 , NULL);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, Nbuffer);
		}
		break;
	case SRVAL_BUYLIMITMONEYVIEW_FOREIGNMERCHANT:
		{
			TCHAR * pTitle = GetTxtFromMgr(389) ;
			TCHAR * pText = GetTxtFromMgr(8452) ;
			if ( !pTitle || !pText ) break ;

			if(((TCHAR*)pclreturn->siPara1 == NULL )||
				((TCHAR*)pclreturn->siPara2 == NULL )||
				((TCHAR*)pclreturn->siPara3 == NULL ))
				break;

			Nsprintf(Nbuffer, pText, TEXT("limitnum"), (TCHAR*)pclreturn->siPara1,TEXT("limitpercentup"), (TCHAR*)pclreturn->siPara2,TEXT("limitpercentdown"), (TCHAR*)pclreturn->siPara3, NULL);

			PushEffect(constEffectFailure, 0, 0);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, Nbuffer);
		}
		break;
	case SRVAL_GAMEMASTER_EVENTENABLE:
		{
			NTCHARString256		kText("#eventname# is #enable#");
			
			kText.Replace( "#eventname#", pclreturn->siPara1 );
			kText.Replace( "#enable#", pclreturn->siPara2 );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( TEXT("eventenable"), kText);
		}
		break;
	case SRVAL_GAMEMASTER_GACHALOCK:
		{
			// "1" 이면락이 걸린거고 "2"면 락이 안걸린거다 
			
			NTCHARString256		kText("Gacha [#lock#]!!");
			kText.Replace( "#lock#",pclreturn->siPara1)	;
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( TEXT("Gacha Lock Message"), kText);

		}break	;
	case SRVAL_GAMEMASTER_GACHA_NOT_LOCK_CHANGE_UNIQUE:
		{
			NTCHARString256		kText(GetTxtFromMgr(30269));
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( TEXT("Gacha Lock Message"), kText);

		}break	;

/*	case SRVAL_GAMEMASTER_SETMUTETIME_CHAR:
		{
			NTCHARString256		kText("#charname# is muted during #minute# minutes.");

			if( strcmp(pclreturn->siPara2, "0") == 0 )
			{
				kText = "#charname# is canceled mute ";
				kText.Replace( "#charname#", pclreturn->siPara1 );
			}
			else
			{
				kText = "#charname# is mute during #minute# minutes.";

				kText.Replace( "#charname#", pclreturn->siPara1 );
				kText.Replace( "#minute#", pclreturn->siPara2 );
			}

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( TEXT("MUTE"), kText);
		}
		break;*/
	//---------------------------------------------------------------------------------------
	case SRVAL_GIVEITEM_RECOMMAND:
		{
			NTCHARString64	strTitle( GetTxtFromMgr(8935) );
			NTCHARString256	strText( GetTxtFromMgr(8937) );

			strText.Replace( TEXT("#name1#"),		pclreturn->siPara1 );
			strText.Replace( TEXT("#level#"),		pclreturn->siPara2 );
			strText.Replace( TEXT("#itemname#"),	pclreturn->siPara3 );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		}
		break;
	
	case SRVAL_RECOMMEND_GIVE_ITEM_SUCCEED:
		{
			if( NULL == m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ] ) 
				CreateInterface(NPROMOTER_GIVE_ITEM_NOTICE);

			((NPromoter_Give_Item_Notice*)m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ])->SetRecommendText( 1, pclreturn->siPara1, pclreturn->siPara2, pclreturn->siPara3  );	
		}
		break;

	case SRVAL_RECOMMEND_GIVE_ITEM_SUCCEED_NO_AUTH:
		{
			if( NULL == m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ] ) 
				CreateInterface(NPROMOTER_GIVE_ITEM_NOTICE);

			((NPromoter_Give_Item_Notice*)m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ])->SetRecommendText( 1, pclreturn->siPara1, pclreturn->siPara2, pclreturn->siPara3  );	
		}
		break;

	case SRVAL_RECOMMEND_GIVE_ITEM_FAIL:
		{
			if( NULL == m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ] ) 
				CreateInterface(NPROMOTER_GIVE_ITEM_NOTICE);

			((NPromoter_Give_Item_Notice*)m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ])->SetRecommendText( 0, pclreturn->siPara1, pclreturn->siPara2, pclreturn->siPara3 );	
		}
		break;
	
	case SRVAL_RECOMMENDEE_GIVE_ITEM_SUCCEED:
		{
			if( NULL == m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ] ) 
				CreateInterface(NPROMOTER_GIVE_ITEM_NOTICE);

			((NPromoter_Give_Item_Notice*)m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ])->SetRecommendeeText( 1, pclreturn->siPara1, pclreturn->siPara2, pclreturn->siPara3  );	
		}
		break;
	
	case SRVAL_RECOMMENDEE_GIVE_ITEM_SUCCEED_NO_AUTH:
		{
			if( NULL == m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ] ) 
				CreateInterface(NPROMOTER_GIVE_ITEM_NOTICE);

			((NPromoter_Give_Item_Notice*)m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ])->SetRecommendeeText( 1, pclreturn->siPara1, pclreturn->siPara2, pclreturn->siPara3  );	
		}
		break;

	case SRVAL_RECOMMENDEE_GIVE_ITEM_FAIL:
		{
			if( NULL == m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ] ) 
				CreateInterface(NPROMOTER_GIVE_ITEM_NOTICE);

			((NPromoter_Give_Item_Notice*)m_pDialog[ NPROMOTER_GIVE_ITEM_NOTICE ])->SetRecommendeeText( 0, pclreturn->siPara1, pclreturn->siPara2, pclreturn->siPara3  );	
		}
		break;		

	case SRVAL_WEDDING_MATE1_QUESTION:
		{
			NTCHARString256	strText1( GetTxtFromMgr(30075) );
			NTCHARString64	strText2( GetTxtFromMgr(30092) );
			NTCHARString64	strText3( GetTxtFromMgr(30096) );
			NTCHARString64	strText4( GetTxtFromMgr(30097) );

			strText1.Replace( TEXT("#name1#"), pclreturn->siPara1 );
			strText3.Replace( TEXT("#name2#"), pclreturn->siPara2 );

			strText1 += strText2;
			strText1 += strText3;
			strText1 += strText4;

			stMsgBox MsgBox;
			MsgBox.Set( m_pDialog[NOTIFYMSG_DLG], TEXT(""), strText1, MSGBOX_TYPE_OK, SRVAL_WEDDING_MATE1_QUESTION );
			SetMsgBox( &MsgBox, NULL, 0 );
		}
		break;

	case SRVAL_WEDDING_MATE2_QUESTION:
		{
			NTCHARString256	strText1( GetTxtFromMgr(30075) );
			NTCHARString64	strText2( GetTxtFromMgr(30092) );
			NTCHARString64	strText3( GetTxtFromMgr(30096) );
			NTCHARString64	strText4( GetTxtFromMgr(30097) );

			strText1.Replace( TEXT("#name1#"), pclreturn->siPara1 );
			strText3.Replace( TEXT("#name2#"), pclreturn->siPara2 );

			strText1 += strText2;
			strText1 += strText3;
			strText1 += strText4;

			stMsgBox MsgBox;
			MsgBox.Set( m_pDialog[NOTIFYMSG_DLG], TEXT(""), strText1, MSGBOX_TYPE_OK, SRVAL_WEDDING_MATE2_QUESTION );
			SetMsgBox( &MsgBox, NULL, 0 );
		}
		break;

	case SRVAL_WEDDING_GIVEITEM:
		{
			NTCHARString256 strText		= GetTxtFromMgr(7899);

			NTCHARString64	strSource	= GetTxtFromMgr(30098);	// 예식장
			NTCHARString64	strItemName = (TCHAR*)pclItemManager->GetName( ITEMUNIQUE(23497) );

			strText.Replace( TEXT("#user#"),		pclreturn->siPara1 );
			strText.Replace( TEXT("#treasurebox#"),	strSource );
			strText.Replace( TEXT("#item#"),		strItemName );

			m_pDialog[NOTIFYMSG_DLG]->SetText( TEXT(""), strText );
		
		}
		break;
		//Switch_10MonthEvent_RacoonDodge
	case SRVAL_RETURN_RACOONDODGEEVENT_CLEAR:	// 라쿤 닷지 이벤트 아이템을 준다 종료 
		{
			NTCHARString256 strText		= GetTxtFromMgr(40063);
			strText.Replace( TEXT("#money#"),		pclreturn->siPara1 );
			m_pDialog[NOTIFYMSG_DLG]->SetText( TEXT(""), strText );

		}
		break;

		// [지연] : 서버에서 전달받은 복불복 게임 클리어 보상 안내창
	case SRVAL_RETURN_BOKBULBOK_CLEAR_REWARD:
		{
 			NTCHARString256 strText		= GetTxtFromMgr(40085);
 			strText.Replace( TEXT("#money#"),		pclreturn->siPara1 );
 			m_pDialog[NOTIFYMSG_DLG]->SetText( TEXT(""), strText );
		}
		break;

	default:
		MsgBox(TEXT("OperateByServerResponseCharValue()"), TEXT("%d"), pclreturn->siReturnValue);
	}
	return TRUE;
}


