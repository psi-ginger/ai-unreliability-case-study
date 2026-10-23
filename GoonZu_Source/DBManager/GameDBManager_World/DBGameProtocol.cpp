#include "DBGameProtocol.h"
#include "DBMsg.h"

#include "ndate.h"

#define DBMessageMap(AAA,BBB) m_pDBMsgPtr[AAA].Func = &DBGameProtocol::BBB

// 한게임 설정 정보 - 빌링 설정
extern char				g_BillingServerIP[ 256 ];
extern UI16				g_BillingServerPort;
extern UI32				g_BillingTimeOut;

extern SI32				g_BillingMode;

extern void RichODBC_ErroyQuery(const char* pszErrorQuery);

DBGameProtocol::DBGameProtocol()
{
	ZeroMemory( givecashbuffer, sizeof( givecashbuffer ) );
	givecashcount = 0;

	m_pDBMsgPtr = new stDBMsgPtr[DBMSG_END];
	if(m_pDBMsgPtr == NULL)
		MessageBox(NULL, "함수포인터 할당 실패", NULL, MB_OK);
	else
	{
		// 알 수 없는 메시지가 왔을때 처리할 함수를 할당해 준다.
		for(int i = 0; i < DBMSG_END; i++)
			m_pDBMsgPtr[i].Func = &DBGameProtocol::NullFuncPtr;

//		m_pDBMsgPtr[DBMSG_REQUEST_STARTGAME].Func = &DBGameProtocol::StartGame;

		/*/////////////////////////////////////////////////
			System
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_PING, Ping);
		DBMessageMap(DBMSG_REQUEST_STARTGAME, StartGame);
		DBMessageMap(DBMSG_REQUEST_MASTERGIVEITEM, MasterGiveItem);
		DBMessageMap(DBMSG_REQUEST_GIVEITEM, GiveItem);
		DBMessageMap(DBMSG_REQUEST_DBSYSTEMINFO, DBSystemInfo);
		DBMessageMap(DBMSG_REQUEST_VOTEINIT, VoteInit);
		DBMessageMap(DBMSG_REQUEST_DBINITCOMPLETE, DBInitComplete);
		DBMessageMap(DBMSG_REQUEST_USERCHARINFO, UserCharInfo);
		DBMessageMap(DBMSG_REQUEST_USERCHARITEMINFO, UserCharItemInfo);
		DBMessageMap(DBMSG_REQUEST_CHARINFOFORHANXNOTI, CharInfoForHanXNoti);
		DBMessageMap(DBMSG_REQUEST_DBSTARTINFO, DBStartInfo);
		DBMessageMap(DBMSG_REQUEST_GETDAILYNEWCHAR, GetDailyNewChar);
		DBMessageMap(DBMSG_REQUEST_SETINSTANCEPROFIT_VILLAGE, SetInstanceProfitVillage);
		DBMessageMap(DBMSG_REQUEST_CHANGE_INSTANCEPROFIT, ChangeInstanceProfit);
		DBMessageMap(DBMSG_REQUEST_VOTEKINGAGGREGATE, VoteKingAggregate);
		DBMessageMap(DBMSG_REQUEST_WRITESTATISTICS, ServerStatisticsSet);
		DBMessageMap(DBMSG_REQUEST_INITSTATISTICS, ServerStatisticsGet);
		DBMessageMap(DBMSG_REQUEST_GETWARRANKINFO, GetWarRank);
		DBMessageMap(DBMSG_REQUEST_SETWARRANKINFO, SetWarRank);
		DBMessageMap(DBMSG_REQUEST_SETLIQUIDATION, SetLiquidation);

		DBMessageMap(DBMSG_REQUEST_MOFUMOFUEVENT_RESERVE, MofuMofuEvent_Reserve);
		DBMessageMap(DBMSG_REQUEST_MOFUMOFUEVENT_RESERVELIST, MofuMofuEvent_ReserveList);

		DBMessageMap(DBMSG_REQUEST_VILLAGEBANKRUPTCY, VillageBankruptcy);

		DBMessageMap(DBMSG_REQUEST_AUTOMACRO, AutoMacro);

		DBMessageMap(DBMSG_REQUEST_SERVER_EVENT_RANK_INFO,				 	ServerEvnetRank);
		DBMessageMap(DBMSG_REQUEST_SERVER_EVENT_SERVER_SCORE_ADD,		 	ServerEvnetServerScoreAdd);
		DBMessageMap(DBMSG_REQUEST_SERVER_EVENT_SERVER_DAILY_SCORE_ADD,  	ServerEvnetServerDailyScoreAdd);
		DBMessageMap(DBMSG_REQUEST_SERVER_EVENT_CLEAR,						ServerEvnetClear);
		DBMessageMap(DBMSG_REQUEST_SERVER_EVENT_SERVER_BUF_KIND,			ServerEventServerBufKind);
		
		DBMessageMap(DBMSG_REQUEST_SERVER_EVENT_QUEST_INFO_SET,		 ServerEventQuestInfoSet);
		DBMessageMap(DBMSG_REQUEST_SERVER_EVENT_QUEST_INFO_GET,		 ServerEventQuestInfoGet);

		DBMessageMap(DBMSG_REQUEST_SERVER_EVENT_PERSON_SCORE_SET	,	 ServerEventPersonScoreSet);
		DBMessageMap(DBMSG_REQUEST_SERVER_EVENT_PERSON_SCORE_GET,	 ServerEventPersonScoreGet);
		
		DBMessageMap(DBMSG_REQUEST_SETFOURLEAF,		SetFourLeaf);


		//-------------------------------------------------------------------------------------------------------------------
		//[성웅] 라쿤이벤트 군주 이벤트로 변경
		//-------------------------------------------------------------------------------------------------------------------
		DBMessageMap(DBMSG_REQUEST_GOONZURACCOONEVENT_SETINFO,		SetGoonzuRaccoonEvent);	// 데이터를 저장 한다 
		DBMessageMap(DBMSG_REQUEST_GOONZURACCOONEVENT_GETINFO,		GetGoonzuRaccoonEvent);	// 서버가 열릴때 데이터를 요구 한다 .

		/*/////////////////////////////////////////////////
			Person
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_PERSONTOTALINFOCREATE, PersonTotalInfoCreate);
		DBMessageMap(DBMSG_REQUEST_PERSONTOTALINFOGET, PersonTotalInfoGet);
		DBMessageMap(DBMSG_REQUEST_PERSONTOTALINFOGETBYIDNUM, PersonTotalInfoGetByIDNum);
		DBMessageMap(DBMSG_REQUEST_PERSONLISTINFOGET, PersonListInfoGet);
		DBMessageMap(DBMSG_REQUEST_PERSONTOTALINFODELETE, PersonTotalInfoDelete);
		DBMessageMap(DBMSG_REQUEST_PERSONTOTALINFOPREDELETE, PersonTotalInfoPreDelete);
		DBMessageMap(DBMSG_REQUEST_PERSONTOTALINFOSET, PersonTotalInfoSet);
		DBMessageMap(DBMSG_REQUEST_CHANGEMONEY, ChangeMoney);
		DBMessageMap(DBMSG_REQUEST_USERID, UserID);
		DBMessageMap(DBMSG_REQUEST_BLOCKUSER, BlockUser);
		DBMessageMap(DBMSG_REQUEST_MOVEMONEY, MoveMoney);
		DBMessageMap(DBMSG_REQUEST_SETSCHOOL, SetSchool);
		DBMessageMap(DBMSG_REQUEST_SETFATHER, SetFather);
		DBMessageMap(DBMSG_REQUEST_SETTEACHERPOINT, SetTeacherPoint);
		DBMessageMap(DBMSG_REQUEST_GETITEM_FROM_TEACHERPOINT, GetItemFromTeacherPoint);
		DBMessageMap(DBMSG_REQUEST_RENOUNCEPUPIL, RenouncePupil);
		DBMessageMap(DBMSG_REQUEST_PERSONSIMPLEINFO, PersonSimpleInfo);
		DBMessageMap(DBMSG_REQUEST_INCREASECHILDNUMBER, IncreaseChildNumber);
		DBMessageMap(DBMSG_REQUEST_GETCHILDLIST, GetChildList);
		DBMessageMap(DBMSG_REQUEST_OVERTAKETEACHERLEVEL, OverTakeTeacherLevel);
		DBMessageMap(DBMSG_REQUEST_CHANGEEXP, ChangeExp);
		DBMessageMap(DBMSG_REQUEST_RECOMMENDPERSON, RecommendPerson);
		DBMessageMap(DBMSG_REQUEST_VOTEKINGSELECTION, VoteKingSelection);
		DBMessageMap(DBMSG_REQUEST_LOGOUTRECORD, LogoutRecord);
		DBMessageMap(DBMSG_REQUEST_ACCOUNTINFO, AccountInfo);
		DBMessageMap(DBMSG_REQUEST_ACCOUNTINFOFORHANXNOTI, AccountInfoForHanXNoti);		// For HanXNoti
		DBMessageMap(DBMSG_REQUEST_CHANGENAME, ChangeName);
		DBMessageMap(DBMSG_REQUEST_GETCHANGEDNAME, GetChangedName);
		DBMessageMap(DBMSG_REQUEST_CHANGE_CHARKIND, ChangeCharKind);
		DBMessageMap(DBMSG_REQUEST_PRIZE_GAMEEVENT, GameEvent);
		DBMessageMap(DBMSG_REQUEST_COUNTATTACK_INFO, CountAttackInfo);
		DBMessageMap(DBMSG_REQUEST_SETGREETING, GreetingSet);
		DBMessageMap(DBMSG_REQUEST_GETGREETING, GreetingGet);
		DBMessageMap(DBMSG_REQUEST_ITEMMAKERNAME, GetPersonName);
		DBMessageMap(DBMSG_REQUEST_ITEMMAKERNAMENEW, GetPersonNameNew);		
		DBMessageMap(DBMSG_REQUEST_CHANGERECOMMENDPOINT, ChangeRecommendPoint);
		DBMessageMap(DBMSG_REQUEST_GETRECOMMENDPOINT, GetRecommendPoint);
		DBMessageMap(DBMSG_REQUEST_BUYRECOMMENDPRODUCT, BuyRecommendProduct);
		DBMessageMap(DBMSG_REQUEST_RECOMMENDPOINT_LIST, RecommendPointList);
		DBMessageMap(DBMSG_REQUEST_GIVEFP, GIveFP);
		DBMessageMap(DBMSG_REQUEST_SETCOUNTRY, SetCountry);
		DBMessageMap(DBMSG_REQUEST_SETSHOWCOUNTRY, SetShowCountry);
		DBMessageMap(DBMSG_REQUEST_ADDBUF, AddBuf);
		DBMessageMap(DBMSG_REQUEST_DELETEBUF, DeleteBuf);
		DBMessageMap(DBMSG_REQUEST_SETSHOWPUCHIKOMI, SetShowPuchikomi);
		DBMessageMap(DBMSG_REQUEST_PVPRESULT, PVPResult);
		DBMessageMap(DBMSG_REQUEST_ATTEND, Attend);
		DBMessageMap(DBMSG_REQUEST_SETATTEND, SetAttend);
		DBMessageMap(DBMSG_REQUEST_TRANSFORM_SET, TransForm_Set);
		DBMessageMap(DBMSG_REQUEST_TRANSFORM_ADDCHAR, TransForm_AddChar);
		DBMessageMap(DBMSG_REQUEST_TRANSFORM_ADDTIME, TransForm_AddTime);
		DBMessageMap(DBMSG_REQUEST_TIMEMETERRATE_ADDTIME, TimeMeterRate_AddTime);
		DBMessageMap(DBMSG_REQUEST_TIMEMETERRATE_USETIME, TimeMeterRate_useTime);
		DBMessageMap(DBMSG_REQUEST_CHAR_OPTION, CharOption);
		DBMessageMap(DBMSG_REQUEST_CHAR_GET_OPTION, CharGetOption);
		DBMessageMap(DBMSG_REQUEST_SET_HOMEPAGE_URL, SetHomePage);
		DBMessageMap(DBMSG_REQUEST_GET_HOMEPAGE_URL, GetHomePage);

		//[진성] 도우미.
		DBMessageMap(DBMSG_REQUEST_QUESTION_DAILY_COUNT_SET,	HelperDailyCountSet);
		DBMessageMap(DBMSG_REQUEST_QUESTION_DAILY_COUNT_GET,	HelperDailyCountGet);
		DBMessageMap(DBMSG_REQUEST_QUESTION_DAILY_COUNT_DEL,	HelperDailyCountDel);

		DBMessageMap(DBMSG_REQUEST_SETMUTETIME,	SetMuteTime);
		DBMessageMap(DBMSG_REQUEST_GETMUTETIME,	GetMuteTime);

		// 발렌타인 데이		
		DBMessageMap(DBMSG_REQUEST_VALANTINE_REQUEST,	SetValentineWarInfo);
		DBMessageMap(DBMSG_REQUEST_VALANTINE_INFO_GET,	GetValentineWarInfo);
		DBMessageMap(DBMSG_REQUEST_VALANTINE_GET_PRIZE,	ValentineWarGetPrize);

		
		/*/////////////////////////////////////////////////
			PersonItem
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_PERSONITEMINFOGET, GetPersonItem );
		DBMessageMap(DBMSG_REQUEST_INPUTITEMTOPERSON, InputItemToPerson);
		DBMessageMap(DBMSG_REQUEST_MOVEITEM, MoveItem);
		DBMessageMap(DBMSG_REQUEST_OUTPUTITEMFROMPERSON, OutputItemFromPerson);
		DBMessageMap(DBMSG_REQUEST_MOVEITEMPERSON, MoveItemPerson);
		DBMessageMap(DBMSG_REQUEST_USEITEM, UseItem);
		DBMessageMap(DBMSG_REQUEST_SELLNPC, SellNPC);
		DBMessageMap(DBMSG_REQUEST_BUYNPC, BuyNPC);
		DBMessageMap(DBMSG_REQUEST_MOVEITEMINVTOSTG, MoveItemInvToStg);
		DBMessageMap(DBMSG_REQUEST_MOVEITEMSTGTOINV, MoveItemStgToInv);
		DBMessageMap(DBMSG_REQUEST_WASTEDURABILITY, WasteDurability);
		DBMessageMap(DBMSG_REQUEST_INVREPAIR, InvRepair);
		DBMessageMap(DBMSG_REQUEST_DROPITEM, DropItem);
		DBMessageMap(DBMSG_REQUEST_MAKEITEMINV, MakeItemInv);
		DBMessageMap(DBMSG_REQUEST_ENCHANT, EnchantItem);
		DBMessageMap(DBMSG_REQUEST_ENCHANTITEMINV, EnchantItemInv);		
		DBMessageMap(DBMSG_REQUEST_PRIVATETRADE, PrivateTrade);
		DBMessageMap(DBMSG_REQUEST_WARPVILLAGE, WarpVillage);
		DBMessageMap(DBMSG_REQUEST_UPGRADEITEM, UpgradeItem);
		DBMessageMap(DBMSG_REQUEST_PERSONALSHOPBUYITEM, PersonalShopBuyItem);
		DBMessageMap(DBMSG_REQUEST_PERSONALSHOPSELLITEM, PersonalShopSellItem);
		DBMessageMap(DBMSG_REQUEST_STARTPERSONALSHOP, StartPersonalShop);
		DBMessageMap(DBMSG_REQUEST_FISHINGGETMARINEPRODUCTS, FishingGetMarineProducts);
		DBMessageMap(DBMSG_REQUEST_FISHINGDECREASETOOLDURABILITY, FishingDecreaseToolDurability);
		DBMessageMap(DBMSG_REQUEST_PERFORMANCEUPGRADEITEM, PerformanceUpgradeItem);
		DBMessageMap(DBMSG_REQUEST_AGRICULTUREGETPRODUCTS, AgricultureGetProducts);
		DBMessageMap(DBMSG_REQUEST_SETPREMIUM, PremiumSet);
		DBMessageMap(DBMSG_REQUEST_WENNYOLDMAN_INVENTORYLIST,		WennyOldManInvList);
		DBMessageMap(DBMSG_REQUEST_WENNYOLDMAN_SAVEINVENTORY,		WennyOldManSaveInv);
		DBMessageMap(DBMSG_REQUEST_WENNYOLDMAN_MOVEITEMSTGTOINV,	WennyOldManMoveStgToInv);
		DBMessageMap(DBMSG_REQUEST_WENNYOLDMAN_MOVEITEMSTGTOINV,	WennyOldManMoveStgToInv);
		DBMessageMap(DBMSG_REQUEST_EXCHANGEITEM,					ExchangeItem);
		DBMessageMap(DBMSG_REQUEST_OPENTREASUREBOX,					OpenTreasureBox);
		DBMessageMap(DBMSG_REQUEST_ENCHANT_USEDAYPLUS,				EnchantUseDayPlus);
		DBMessageMap(DBMSG_REQUEST_SYSTEMREWARDLIST,				SystemRewardList);
		DBMessageMap(DBMSG_REQUEST_RECIEVESYSTEMREWARD,				RecieveSystemReward);
		DBMessageMap(DBMSG_REQUEST_GIVESYSTEMREWARD,				GiveSystemReward);
		DBMessageMap(DBMSG_REQUEST_SPACE_BOX_ITEM_LIST,				SpaceBoxList);
		DBMessageMap(DBMSG_REQUEST_KEEP_SPACE_BOX_MOVE_ITEM ,		SpaceBoxMoveItem);
		DBMessageMap(DBMSG_REQUEST_RECOVERY_SPACE_BOX_MOVE_ITEM ,	SpaceBoxMoveItem);
		DBMessageMap(DBMSG_REQUEST_USE_SPACEBOX_ITEM,				SpaceBoxUseItem);
		DBMessageMap(DBMSG_REQUEST_USE_TREASURE_ITEM,				SpaceBoxUseItem);
		DBMessageMap(DBMSG_REQUEST_CHANGEENCHANT,					ChangeEnchant);
		DBMessageMap(DBMSG_REQUEST_ARTIFACT_INVENTORYLIST,			ArtifactInventoryList);
		DBMessageMap(DBMSG_REQUEST_ARTIFACT_SAVEINVENTORY,			ArtifactSaveInventory);
		DBMessageMap(DBMSG_REQUEST_ARTIFACT_MOVEITEMSTGTOINV,		ArtifactMoveItemStgToInv);
		DBMessageMap(DBMSG_REQUEST_ARTIFACT_MIXTURE,				ArtifactMixture);
		DBMessageMap(DBMSG_REQUEST_ARTIFACT_REMOVE,					ArtifactRemove);
		DBMessageMap(DBMSG_REQUEST_ARTIFACT_SLOT_CREATE,			ArtifactSlot_Create);
		DBMessageMap(DBMSG_REQUEST_CRYSTALDURABILITY,				CrystalDurability);
		DBMessageMap(DBMSG_REQUEST_SUMMONPUSHITEM,					SummonPushItem);
		DBMessageMap(DBMSG_REQUEST_SUMMONPOPITEM, 					SummonPopItem);
		DBMessageMap(DBMSG_REQUEST_SUMMONUSEITEM, 					SummonUseItem);
		DBMessageMap(DBMSG_REQUEST_SUMMONITEM_STATUSCHANGE,			SummonItem_StatusChange);
		DBMessageMap(DBMSG_REQUEST_REPAIRITEM,						RepairItem);
		
		DBMessageMap(DBMSG_REQUEST_AMBER_BOX_SELECT_ITEM_LIST, 		AmberboxSelectItemList);		//[진성] 호박상자 => 2008-9-16
		DBMessageMap(DBMSG_REQUEST_AMBER_BOX_SELECT_ITEM_ADD,  		AmberboxSelectItemAdd);			//[진성] 호박상자 => 2008-9-16
		DBMessageMap(DBMSG_REQUEST_AMBER_BOX_SELECT_ITEM_GET,  		AmberboxSelectItemGet);			//[진성] 호박상자 => 2008-9-16

		DBMessageMap(DBMSG_REQUEST_NEWSELECTBOX_ITEM_GET,			NewSelectBoxItemGet);			// [영훈] 새로운 선택 상자 2008.12.19
		
		DBMessageMap(DBMSG_REQUEST_CONTRIBUTION_ITEM_LIST_INPUT,	ContributionItemListInput);		// [진성] 아이템 기부하기.
		DBMessageMap(DBMSG_REQUEST_CONTRIBUTION_ITEM_LIST_DELETE,	ContributionItemListDelete);	// [진성] 아이템 기부하기.
		DBMessageMap(DBMSG_REQUEST_CONTRIBUTION_MAKE_NPC,			ContributionMakeNPC);			// [진성] 
		
		
		
		
		/*/////////////////////////////////////////////////
			Bank
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_CREATEBANKACCOUNT, CreateBankAccount);
		DBMessageMap(DBMSG_REQUEST_CLOSEBANKACCOUNT, CloseBankAccount);
		DBMessageMap(DBMSG_REQUEST_INPUTMONEYTOBANK, InputMoneyToBank);
		DBMessageMap(DBMSG_REQUEST_OUTPUTMONEYFROMBANK, OutputMoneyFromBank);
		DBMessageMap(DBMSG_REQUEST_SETBANKFEE, SetBankFee);

		/*/////////////////////////////////////////////////
			House
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_CHANGEHOUSECONTRACT, ChangeHouseContract);
		DBMessageMap(DBMSG_REQUEST_MANYHOUSEUNIT, ManyHouseUnit);
		DBMessageMap(DBMSG_REQUEST_GETHOUSEAPPLYERLIST, GetHouseApplyerList);
		DBMessageMap(DBMSG_REQUEST_HOUSERENTAPPLY, HouseRentApply);
		DBMessageMap(DBMSG_REQUEST_HOUSERENT, HouseRent);
		DBMessageMap(DBMSG_REQUEST_GETHOUSESTGPOS, GetHouseStgPos);
		DBMessageMap(DBMSG_REQUEST_MAKEITEM, MakeItem);
		DBMessageMap(DBMSG_REQUEST_HOUSERENTAPPLYCANCEL, HouseRentApplyCancel);
		DBMessageMap(DBMSG_REQUEST_DELHOUSEAPPLYER, DelHouseApplyer);
		DBMessageMap(DBMSG_REQUEST_PAYRENTFEE, PayRentFee);
		DBMessageMap(DBMSG_REQUEST_RECALLHOUSE, RecallHouse);
		DBMessageMap(DBMSG_REQUEST_EXTENDCONTRACT, ExtendContract);
		DBMessageMap(DBMSG_REQUEST_HOUSEUNITHORSEIN, HouseUnitHorseIn);
		DBMessageMap(DBMSG_REQUEST_HOUSEUNITHORSEOUT, HouseUnitHorseOut);
		DBMessageMap(DBMSG_REQUEST_HOUSEUNITSTABLECREATE, HouseUnitStableCreate);
		DBMessageMap(DBMSG_REQUEST_HOUSEUNITHORSEINFOSET, HouseUnitHorseInfoSet);
		DBMessageMap(DBMSG_REQUEST_HOUSEUNITHORSEEATFOOD, HouseUnitHorseEatFood);
		DBMessageMap(DBMSG_REQUEST_HOUSEUNITUSEDGOODSBUILDINGCREATE, HouseUnitUsedGoodsBuildingCreate);
		DBMessageMap(DBMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODS, HouseUnitMakeItemFromUsedGoods);
		DBMessageMap(DBMSG_REQUEST_HOUSEUNITNAMECHANGE, ChangHouseUnitName);

		/*/////////////////////////////////////////////////
			Structure
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_GETSTRUCTUREINFO, GetStructureInfo);
		DBMessageMap(DBMSG_REQUEST_SETSTRINFO, SetStrInfo);
		DBMessageMap(DBMSG_REQUEST_GETSTRINFO, GetStrInfo);
		DBMessageMap(DBMSG_REQUEST_PAYFEE, PayFee);
		DBMessageMap(DBMSG_REQUEST_CHANGESTRMONEY, ChangeStrMoney);
		DBMessageMap(DBMSG_REQUEST_SETSTRUCTUREMESSAGE, SetStructureMessage);
		DBMessageMap(DBMSG_REQUEST_BUILDSTRUCTURE, BuildStructure);
		DBMessageMap(DBMSG_REQUEST_DESTROYSTRUCTURE, DestroyStructure);
		DBMessageMap(DBMSG_REQUEST_CHIEFINFO, ChiefInfo);
		DBMessageMap(DBMSG_REQUEST_SETSTRREPAIRITEM, SetStrRepairItem);
		DBMessageMap(DBMSG_REQUEST_SETSTRREPAIRAMOUNT, SetStrRepairAmount);
		DBMessageMap(DBMSG_REQUEST_SETSTRDURA, SetStrDur);
		DBMessageMap(DBMSG_REQUEST_CHANGESTRDURA, ChangeStrDur);
		DBMessageMap(DBMSG_REQUEST_REPAIRSTRUCTURE, RepairStructure);
		DBMessageMap(DBMSG_REQUEST_CHANGESTRBASEMONEY, ChangeStrBaseMoney);

		/*/////////////////////////////////////////////////
			Tax
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SETTAX, SetTax);
		DBMessageMap(DBMSG_REQUEST_PAYTAX, PayTax);

		/*/////////////////////////////////////////////////
			CityHall
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_PAYDIVIDEND, PayDividend);
		DBMessageMap(DBMSG_REQUEST_APPLYRESIDENT, AppplyResident);
		DBMessageMap(DBMSG_REQUEST_GETRESIDENTAPPLYERLIST, GetResidentApplyerList);
		DBMessageMap(DBMSG_REQUEST_GETRESIDENTLIST, GetResidentList);
		DBMessageMap(DBMSG_REQUEST_BERESIDENT, BeResident);
		DBMessageMap(DBMSG_REQUEST_CANCELRESIDENT, CancelResident);
		DBMessageMap(DBMSG_REQUEST_GETRESIDENTINFO, GetResidentInfo);
		DBMessageMap(DBMSG_REQUEST_SETRESIDENTSCORE, SetResidentScore);
		DBMessageMap(DBMSG_REQUEST_RESIDENTSHARE, ResidentShare);
		DBMessageMap(DBMSG_REQUEST_MANYRESIDENTOUT, ResidentManyOut);
		DBMessageMap(DBMSG_REQUEST_CITYHALL_SETINOUTMONEY, SetInOutMoney);
		DBMessageMap(DBMSG_REQUEST_CHANGECITYHALLPROFITMONEY, ChangeCityHallProfitMoney);
		DBMessageMap(DBMSG_REQUEST_PROFITTOCAPITALFUND, ProfitToCapitalFund);
		DBMessageMap(DBMSG_REQUEST_CITYHALL_STATE_CHANGED, CityHallStateChange);
		DBMessageMap(DBMSG_REQUEST_NOTIFY_CITYHALL_BANKRUPTCY, NotifyCityhallBankruptcy);
		DBMessageMap(DBMSG_REQUEST_AUTOPAYDIVIDEND, AutoPayDividend);
		DBMessageMap(DBMSG_REQUEST_GIVEVILLAGEITEM, GiveVillageItem);
		DBMessageMap(DBMSG_REQUEST_DONATEVILLAGEMONEY, DonateVillageMoney);
		DBMessageMap(DBMSG_REQUEST_DONATEMEMBERLIST, DonateMemberList);

		/*/////////////////////////////////////////////////
			Letter
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_PUSHLETTER, PushLetter);
		DBMessageMap(DBMSG_REQUEST_POPLETTER, PopLetter);
		DBMessageMap(DBMSG_REQUEST_PUSHACCOUNTLETTER, PushAccountLetter);
		DBMessageMap(DBMSG_REQUEST_POPACCOUNTLETTER, PopAccountLetter);

		/*/////////////////////////////////////////////////
			Market
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_ORDERMARKET, OrderMarket);
		DBMessageMap(DBMSG_REQUEST_GETMANYMARKETORDER, GetManyMarketOrder);
		DBMessageMap(DBMSG_REQUEST_TRADEMARKET, TradeMarket);
		DBMessageMap(DBMSG_REQUEST_MARKETACCOUNT, MarketAccount);
		DBMessageMap(DBMSG_REQUEST_GETITEMFROMMARKET, GetItemFromMarket);
		DBMessageMap(DBMSG_REQUEST_MANYITEMPRICE, ManyItemPrice);
		DBMessageMap(DBMSG_REQUEST_SETITEMPRICE, SetItemPrice);
		DBMessageMap(DBMSG_REQUEST_REDUCEORDERPRICE, ReduceOrderPrice);
		DBMessageMap(DBMSG_REQUEST_SETMARKETFEE, SetMarketFee);
		DBMessageMap(DBMSG_REQUEST_SET_SYSTEMBUYITEMNOWMONEY, SetSystemBuyItemNowMoney);
		DBMessageMap(DBMSG_REQUEST_GET_SYSTEMBUYITEMNOWMONEY, GetSystemBuyItemNowMoney);
		
		/*/////////////////////////////////////////////////
			Rank
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_CANDIDATE, Candidate);
		DBMessageMap(DBMSG_REQUEST_BERANK, BeRank);
		DBMessageMap(DBMSG_REQUEST_CANCELCANDIDATE, CancelCandidate);
		DBMessageMap(DBMSG_REQUEST_MANYCANDIDATEINFO, ManyCandidateInfo);
		DBMessageMap(DBMSG_REQUEST_DELCANDIDATE, DelCandidate);
		DBMessageMap(DBMSG_REQUEST_RESIGN, Resign);
		DBMessageMap(DBMSG_REQUEST_MANYRANK, ManyRank);

		/*/////////////////////////////////////////////////
			Skill
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_INCREASESKILL, IncreaseSkill);
		DBMessageMap(DBMSG_REQUEST_DECREASESKILL, DecreaseSkill);
		DBMessageMap(DBMSG_REQUEST_SETSKILLEXP, SetSkillExp);
		DBMessageMap(DBMSG_REQUEST_INCREASEMASTERSKILL, IncreaseMasterSkill);
		DBMessageMap(DBMSG_REQUEST_SET_WEAPONSKILL, SetWeaponSkill);

		/*/////////////////////////////////////////////////
			Stock
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_GETMANYSTOCKORDER,									GetManyStockOrder);
		DBMessageMap(DBMSG_REQUEST_TRADESTOCK,											TradeStock);
		DBMessageMap(DBMSG_REQUEST_ORDERSTOCK,											OrderStock);
		DBMessageMap(DBMSG_REQUEST_PERSONSTOCKACCOUNTGET,								PersonStockAccountGet);
		DBMessageMap(DBMSG_REQUEST_CANCELSTOCKORDER,									CancelStockOrder);
		DBMessageMap(DBMSG_REQUEST_INVESTLIST,											InvestList);
		DBMessageMap(DBMSG_REQUEST_MANYSTOCKPRICE,										ManyStockPrice);
		DBMessageMap(DBMSG_REQUEST_SETSTOCKFEE,											SetStockFee);
		DBMessageMap(DBMSG_REQUEST_SETSTOCKBIDVILLAGE,									SetStockBidVillage);
		DBMessageMap(DBMSG_REQUEST_GETSTOCKBIDVILLAGELIST,								GetStockBidVillageList);
		DBMessageMap(DBMSG_REQUEST_BIDSTOCK,											BidStock);
		DBMessageMap(DBMSG_REQUEST_GETSTOCKBIDDERLIST,									GetStockBidderList);
		DBMessageMap(DBMSG_REQUEST_COMPLETESTOCKBIDVILLAGE,								CompleteStockBidVillage);
		DBMessageMap(DBMSG_REQUEST_SETBEGINNINGPRICE,									SetBeginningPrice);
		DBMessageMap(DBMSG_REQUEST_STOCKDISTRIBUTIONORDER,								StockDistributionOrder);
		DBMessageMap(DBMSG_REQUEST_MAXSTOCKDISTRIBUTIONNUM,								MaxStockDistributionNum);
		DBMessageMap(DBMSG_REQUEST_SETVILLAGEWARLOSS,									SetVillageWarLoss);
		DBMessageMap(DBMSG_REQUEST_RETURNSTOCKBIDVILLAGE,								ReturnStockBidVillage);

		DBMessageMap(DBMSG_REQUEST_BIDSTOCK,											BidStock);
		DBMessageMap(DBMSG_REQUEST_BUYBIDSTOCK,											BuyBidStock);
		DBMessageMap(DBMSG_REQUEST_MYSTOCKINFO,											MyStockInfo);

		DBMessageMap(DBMSG_REQUEST_GETSTOCKOFFERINGINFO,								GetStockOfferingInfo);
		DBMessageMap(DBMSG_REQUEST_SETSTOCKOFFERING,									SetStockOffering);
		DBMessageMap(DBMSG_REQUEST_ENDSTOCKOFFERING,									EndStockOffering);
		DBMessageMap(DBMSG_REQUEST_STOCKOFFERINGORDER,									StockOfferingOrder);
		DBMessageMap(DBMSG_REQUEST_GETSTOCKOFFERINGORDERLIST,							GetStockOfferingOrderList);
		DBMessageMap(DBMSG_REQUEST_EXECUTESTOCKOFFERINGORDER,							ExecuteStockOfferingOrder);
		DBMessageMap(DBMSG_REQUEST_RESETSTOCKOFFERINGORDERLIST,							ResetStockOfferingOrderList);
		DBMessageMap(DBMSG_REQUEST_EXCHANGEFROMSTOCK,									ExchangeFromStock);

		DBMessageMap(DBMSG_REQUEST_GIVESTOCK,											GiveStock);

		/*/////////////////////////////////////////////////
			Time
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SETTIME, SetTime);
		DBMessageMap(DBMSG_REQUEST_GETTIME, GetTime);
		DBMessageMap(DBMSG_REQUEST_SETUSERNUM, SetUserNum);
		DBMessageMap(DBMSG_REQUEST_BLOCK, Block);

		/*/////////////////////////////////////////////////
			ImportantParameta
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SETLEVELINFO, SetLevelInfo);
		DBMessageMap(DBMSG_REQUEST_SETBONUSINFO, SetBonusInfo);
		DBMessageMap(DBMSG_REQUEST_SETPOSLIFEMANA, SetPosLifeMana);
		DBMessageMap(DBMSG_REQUEST_INITSTATUS, InitStatus);
		DBMessageMap(DBMSG_REQUEST_CHANGEFAMEEXP, ChangeFameExp);

		/*/////////////////////////////////////////////////
			Health
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SETHEALTH, SetHealth);

		/*/////////////////////////////////////////////////
			Quest
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SETQUESTINFO, SetQuestInfo);
		DBMessageMap(DBMSG_REQUEST_DAILYQUESTADD, DailyQuestAdd);
		DBMessageMap(DBMSG_REQUEST_DAILYQUESTDEL, DailyQuestDel);
		DBMessageMap(DBMSG_REQUEST_DAILYQUESTCHANGE, DailyQuestChange);

		DBMessageMap(DBMSG_REQUEST_DAILYQUEST2ADD, DailyQuest2Add);
		DBMessageMap(DBMSG_REQUEST_DAILYQUEST2DEL, DailyQuest2Del);
		DBMessageMap(DBMSG_REQUEST_DAILYQUEST2CHANGE, DailyQuest2Change);

		DBMessageMap(DBMSG_REQUEST_DAILYQUEST3ADD, DailyQuest3Add);
		DBMessageMap(DBMSG_REQUEST_DAILYQUEST3DEL, DailyQuest3Del);
		DBMessageMap(DBMSG_REQUEST_DAILYQUEST3CHANGE, DailyQuest3Change);

		DBMessageMap(DBMSG_REQUEST_SETSPECIALQUESTINFO, SetSpecialQuestInfo);

		DBMessageMap(DBMSG_REQUEST_SETNAMINGQUESTINFO, SetNamingQuestInfo);
		DBMessageMap(DBMSG_REQUEST_SETPERSONNAMINGINFO, SetPersonNamingInfo);
		DBMessageMap(DBMSG_REQUEST_SETPERSONCURRENTNAMING, SetPersonCurrentNaming);

		DBMessageMap(DBMSG_REQUEST_SETHAPPYBEANQUESTINFO, SetHappyBeanQuestInfo);
		DBMessageMap(DBMSG_REQUEST_GETHAPPYBEANQUESTINFO, GetHappyBeanClearInfo);

		DBMessageMap(DBMSG_REQUEST_SETMISSIONQUESTINFO, SetMissionQuestInfo);

		/*/////////////////////////////////////////////////
			NPC
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SETFOREIGNMERCHANTNPCINFO, SetForeignMerchantNPCInfo);
		DBMessageMap(DBMSG_REQUEST_GETFOREIGNMERCHANTNPCINFO, GetForeignMerchantNPCInfo);
		DBMessageMap(DBMSG_REQUEST_HIRENPC, HireNPC);
		DBMessageMap(DBMSG_REQUEST_TRADEMERCHANTSET, TradeMerchantSet);
		DBMessageMap(DBMSG_REQUEST_TRADEMERCHANTBUYITEMFROMPERSON, TradeMerchantBuyItemFromPerson);
		DBMessageMap(DBMSG_REQUEST_TRADEMERCHANTBUYHORSEFROMPERSON, TradeMerchantBuyHorseFromPerson);
		DBMessageMap(DBMSG_REQUEST_TRADEMERCHANTBUYITEM, TradeMerchantBuyItem);
		DBMessageMap(DBMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEM, TradeMerchantBuyCashItem);

		/*/////////////////////////////////////////////////
			Horse
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SETHORSE, SetHorse);
		DBMessageMap(DBMSG_REQUEST_SETRIDEHORSE, SetRideHorse);
		DBMessageMap(DBMSG_REQUEST_HORSENAMECHANGE, HorseNameChange);
		DBMessageMap(DBMSG_REQUEST_SETAUTOFOOD, SetAutoFood);

		/*/////////////////////////////////////////////////
			Village
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_GETVILLAGEINFO, GetVillageInfo);
		DBMessageMap(DBMSG_REQUEST_DECLAREWAR, DeclareWar);
		DBMessageMap(DBMSG_REQUEST_CANCELDECLAREWAR, CancelDeclareWar);
		DBMessageMap(DBMSG_REQUEST_SETPARENTVILLAGE, SetParentVillage);
		DBMessageMap(DBMSG_REQUEST_SETVILLAGESYMBOL, SetVillageSymbol);
		DBMessageMap(DBMSG_REQUEST_SETSYMBOLSTATUS, SetSymbolStatus);
		DBMessageMap(DBMSG_REQUEST_SETVILLAGECASTLE, SetVillageCastle);
		DBMessageMap(DBMSG_REQUEST_SETCASTLESTATUS, SetCastleStatus);
		DBMessageMap(DBMSG_REQUEST_SETVILLAGELEVEL, SetVillageLevel);
		DBMessageMap(DBMSG_REQUEST_SETVILLAGEMARK, SetVillageMark);
		DBMessageMap(DBMSG_REQUEST_MAPTYPEINFO, MapTypeInfo);
		DBMessageMap(DBMSG_REQUEST_SETVILLAGEMAP, SetVillageMap);
//		DBMessageMap(DBMSG_REQUEST_NEWYEAREVENTSUCCESS, NewYearEventSuccess);
		DBMessageMap(DBMSG_REQUEST_VILLAGEEVENTSUCCESS, VillageEventSuccess);
		DBMessageMap(DBMSG_REQUEST_VILLAGEWARKILLNUMBER, VillageWarKillNumber);
		DBMessageMap(DBMSG_REQUEST_VILLAGEJOINPLAYTIME, VillageJoinPlayTime);
		DBMessageMap(DBMSG_REQUEST_VILLAGEJOINLEVELUP, VillageJoinLevelUp);
		DBMessageMap(DBMSG_REQUEST_VILLAGEJOINWARKILLNUMBER, VillageJoinWarKillNumber);
		DBMessageMap(DBMSG_REQUEST_VILLAGEJOINMAKERAREITEM, VillageJoinMakeRareItem);
		DBMessageMap(DBMSG_REQUEST_VILLAGERESIDENTSVISITORSINFO, VillageResidentsVisitorsInfo);
		DBMessageMap(DBMSG_REQUEST_VILLAGEJOINMAKEITEM, VillageJoinMakeItem);
		DBMessageMap(DBMSG_REQUEST_SETVILLAGEWAR_RESULT, VillageWarResult);
		DBMessageMap(DBMSG_REQUEST_EVENT_INSERTMATERIAL, Event_InsertMaterial);
		DBMessageMap(DBMSG_REQUEST_EVENT_MATERIALLIST, Event_MaterialList);
		DBMessageMap(DBMSG_REQUEST_EVENT_MATERIALCOMPLETE, Event_MaterialComplete);
		DBMessageMap(DBMSG_REQUEST_ADD_VILLAGE_SCORE,		Add_Village_Score);
		

		/*/////////////////////////////////////////////////
			Hunt
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_HUNTSTORAGEMONSTERIN, HuntStorageMonsterIn);
		DBMessageMap(DBMSG_REQUEST_HUNTSTORAGEMONSTEROUT, HuntStorageMonsterOut);
		DBMessageMap(DBMSG_REQUEST_HUNTMONSTERFIELDLISTADD, HuntMonsterFieldListAdd);
		DBMessageMap(DBMSG_REQUEST_HUNTMONSTERFIELDLISTDEL, HuntMonsterFieldListDel);
		DBMessageMap(DBMSG_REQUEST_HUNTSTORAGEDECREASEDOLLITEMNUM, HuntStorageDecreaseDollItemNum);
		DBMessageMap(DBMSG_REQUEST_HUNTLEFTMONSTERSTGIN, HuntLeftMonsterStgIn);
		DBMessageMap(DBMSG_REQUEST_INSTANCEMAPPOINT_INSERT, InstanceMapPoint_Insert);
		DBMessageMap(DBMSG_REQUEST_YEARRANKING, InstantMapYearRanking);
		DBMessageMap(DBMSG_REQUEST_HOFLIST, InstantMapHOFList);

		/*/////////////////////////////////////////////////
			PostOffice
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_SENDITEM, PostOffice_SendItem);
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_SENDMONEY, PostOffice_SendMoney);
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_SENDMSG, PostOffice_SendMsg);
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_RECVITEMLIST, PostOffice_RecvItemList);
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_RECVMONEYLIST, PostOffice_RecvMoneyList);
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_RECVMSGLIST, PostOffice_RecvMsgList);
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_GETITEM, PostOffice_GetItem);
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_DELMSG, PostOffice_DelMsg);
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_SETFEE, PostOffice_SetFee);
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_GETMONEY, PostOffice_GetMoney);
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_MASTERSENDITEM, PostOffice_MasterSendItem);
		DBMessageMap(DBMSG_REQUEST_POSTOFFICE_GETRECORD, PostOffice_GetRecord);
		DBMessageMap(DBMSG_REQUEST_GET_SAVEUSERITEM, HouseSaveUserItemGet);
		DBMessageMap(DBMSG_REQUEST_GET_SAVEUSERHORSE, HouseSaveUserHorseGet);
		DBMessageMap(DBMSG_REQUEST_GET_SAVEUSERITEMLIST, HouseSaveUserItemListGet);
		DBMessageMap(DBMSG_REQUEST_CHAR_NAME_CHECK, CharNameCheck);

		DBMessageMap(DBMSG_REQUEST_FRIEND_INFO,				CharFriendInfo);
		DBMessageMap(DBMSG_REQUEST_GUILD_INFO,				CharGuildInfo);
		DBMessageMap(DBMSG_REQUEST_VILLAGE_RESIDENT_INFO,	CharVillageResidentInfo);
		

		/*/////////////////////////////////////////////////
			Friend
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_FRIENDADDLIST, FriendAddList);
		DBMessageMap(DBMSG_REQUEST_FRIENDDELLIST, FriendDelList);
		DBMessageMap(DBMSG_REQUEST_FRIENDMEMO, FriendMemo);
		DBMessageMap(DBMSG_REQUEST_FRIENDMEMOSET, FriendMemoSet);
		DBMessageMap(DBMSG_REQUEST_FRIENDCHANGEGROUP, FriendChangeGroup);
		DBMessageMap(DBMSG_REQUEST_FRIENDCHANGEGROUPNAME, FriendChangeGroupName);
		DBMessageMap(DBMSG_REQUEST_FRIENDALLLIST, FriendAllList);

		/*/////////////////////////////////////////////////
			Minister
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SETYEZOINFO, SetYezoInfo);
		DBMessageMap(DBMSG_REQUEST_GETYEZOINFO, GetYezoInfo);
		DBMessageMap(DBMSG_REQUEST_SETBYUNGZOINFO, SetByungzoInfo);
		DBMessageMap(DBMSG_REQUEST_SETBYUNGZOINFO_VILLAGEWARTIME, SetByungzoInfoVillageWarTime);
		DBMessageMap(DBMSG_REQUEST_GETBYUNGZOINFO, GetByungzoInfo);
		DBMessageMap(DBMSG_REQUEST_SETHOZOINFO, SetHozoInfo);
		DBMessageMap(DBMSG_REQUEST_GETHOZOINFO, GetHozoInfo);
		DBMessageMap(DBMSG_REQUEST_SETGOONZUINFO, SetGoonzuInfo);
		DBMessageMap(DBMSG_REQUEST_GETGOONZUINFO, GetGoonzuInfo);
		DBMessageMap(DBMSG_REQUEST_SETIZOINFO, SetIzoInfo);
		DBMessageMap(DBMSG_REQUEST_GETIZOINFO, GetIzoInfo);
		DBMessageMap(DBMSG_REQUEST_SETGONGZOINFO, SetGongzoInfo);
		DBMessageMap(DBMSG_REQUEST_GETGONGZOINFO, GetGongzoInfo);
		DBMessageMap(DBMSG_REQUEST_SETHYUNGZOINFO, SetHyungzoInfo);
		DBMessageMap(DBMSG_REQUEST_GETHYUNGZOINFO, GetHyungzoInfo);
		DBMessageMap(DBMSG_REQUEST_SETGOONZUFINANCES, SetGoonzuFinances);
		DBMessageMap(DBMSG_REQUEST_SETANGEL, SetAngel);
		DBMessageMap(DBMSG_REQUEST_GETANGELLIST, GetAngelList);
		DBMessageMap(DBMSG_REQUEST_SETANGELAPPLYER, SetAngelApplyer);
		DBMessageMap(DBMSG_REQUEST_GETANGELAPPLYERLIST, GetAngelApplyerList);
		DBMessageMap(DBMSG_REQUEST_SETMINISTER, SetMinister);
		DBMessageMap(DBMSG_REQUEST_SETPOLICE, SetPolice);
		DBMessageMap(DBMSG_REQUEST_GETPOLICELIST, GetPoliceList);
		DBMessageMap(DBMSG_REQUEST_SETCRIMINAL, SetCriminal);
		DBMessageMap(DBMSG_REQUEST_GETCRIMINALLIST, GetCriminalList);
		DBMessageMap(DBMSG_REQUEST_GOONZU_SETBONUSTIME, GoonzuSetBonusTime);
		
		/*/////////////////////////////////////////////////
			HorseMarket
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_HORSEMARKET_GETMANYORDER, HorseMarketGetManyOrder);
		DBMessageMap(DBMSG_REQUEST_HORSEMARKET_SETFEE, HorseMarketSetFee);
		DBMessageMap(DBMSG_REQUEST_HORSEMARKET_ORDERSELL, HorseMarketOrderSell);
		DBMessageMap(DBMSG_REQUEST_HORSEMARKET_MYACCOUNT, HorseMarketMyAccount);
		DBMessageMap(DBMSG_REQUEST_HORSEMARKET_OUTHORSE, HorseMarketOutHorse);
		DBMessageMap(DBMSG_REQUEST_HORSEMARKET_BUYHORSE, HorseMarketBuyHorse);
		DBMessageMap(DBMSG_REQUEST_REDUCEHORSEORDERPRICE, HorseMarketReduceOrderPrice);
		DBMessageMap(DBMSG_REQUEST_SETCURRENTHORSEPRICE, SetHorsePrice);
		DBMessageMap(DBMSG_REQUEST_GETCURRENTHORSEPRICE, GetHorsePrice);


		/*/////////////////////////////////////////////////
			Summon
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SETSUMMON, SetSummon);
		DBMessageMap(DBMSG_REQUEST_SUMMONREVIVE, SummonRevive);
		DBMessageMap(DBMSG_REQUEST_SUMMONEVOLVE, SummonEvolve);
		DBMessageMap(DBMSG_REQUEST_SUMMONSCROLL, SummonScroll);
		DBMessageMap(DBMSG_REQUEST_SUMMON_STATUSINIT, SummonStatusInit);
		DBMessageMap(DBMSG_REQUEST_SUMMON_SETAUTOHEAL, SummonSetAutoHeal);
		DBMessageMap(DBMSG_REQUEST_SUMMONTRADE, SummonTrade);
		DBMessageMap(DBMSG_REQUEST_SUMMONSLOTOUT, SummonSlotOut);
		DBMessageMap(DBMSG_REQUEST_SUMMONSLOTIN, SummonSlotIn);

		
		/*/////////////////////////////////////////////////
			ItemMall
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_CHECK_ACCOUNTID, CheckAccountID);
		DBMessageMap(DBMSG_REQUEST_ITEMMALLEXISTENCE, ItemMallExistence);
		DBMessageMap(DBMSG_REQUEST_ITEMMALLITEMLIST, ItemMallItemList);
		DBMessageMap(DBMSG_REQUEST_GET_ITEMMALL_ITEMINFO, ItemMallItemInfo);
		DBMessageMap(DBMSG_REQUEST_GET_SEL_PGLIST, ItemMallSelPGList);

		DBMessageMap(DBMSG_REQUEST_NEWSCHATADDMISSION, NewsChatAdmission);
		DBMessageMap(DBMSG_REQUEST_NOLIMITTICKET_WARP, NoLimitTicketWarp);

		// Japan
		DBMessageMap(DBMSG_REQUEST_GET_MYCASH_INFO, ItemMallMyCashInfo);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_GIFTITEM, ItemMallGiftItem);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_BUYITEM, ItemMallBuyItem);
		DBMessageMap(DBMSG_REQUEST_ITEMMALLGETITEM, ItemMallGetItem);

		// Global
		DBMessageMap(DBMSG_REQUEST_GET_MYCASH_INFO_GLOBAL, ItemMallMyCashInfo_Global);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_GIFTITEM_GLOBAL, ItemMallGiftItem_Global);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_BUYITEM_GLOBAL, ItemMallBuyItem_Global);
		DBMessageMap(DBMSG_REQUEST_ITEMMALLGETITEM_GLOBAL, ItemMallGetItem_Global);

		// China
		DBMessageMap(DBMSG_REQUEST_GET_MYCASH_INFO_CHINA, ItemMallMyCashInfo_China);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_GIFTITEM_CHINA, ItemMallGiftItem_China);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_BUYITEM_CHINA, ItemMallBuyItem_China);
		DBMessageMap(DBMSG_REQUEST_ITEMMALLGETITEM_CHINA, ItemMallGetItem_China);

		// Special
		DBMessageMap(DBMSG_REQUEST_GET_MYCASH_INFO_KOREA, ItemMallMyCashInfo_Korea);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_GIFTITEM_KOREA, ItemMallGiftItem_Korea);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_BUYITEM_KOREA, ItemMallBuyItem_Korea);
		DBMessageMap(DBMSG_REQUEST_ITEMMALLGETITEM_KOREA, ItemMallGetItem_Korea);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_BUYITEM_FORXNOTI, ItemMallBuyItem_XNoti);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_GIFTITEM_FORXNOTI, ItemMallGiftItem_XNoti);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_CANCELORDER_XNOTI, ItemMall_CancelOrder_XNoti);

		// [종호_NHN->NDOORS] 금액조희
		DBMessageMap(DBMSG_REQUEST_GET_MYCASH_INFO_NDOORS_KOREA, ItemMallMyCashInfo_nDoors_Korea);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_BUYITEM_NDOORS_KOREA, ItemMallBuyItem_nDoors_Korea);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_GIFTITEM_NDOORS_KOREA, ItemMallGiftItem_nDoors_Korea);
		
		
		// Taiwan
		DBMessageMap(DBMSG_REQUEST_ITEMMALLGETITEM_TAIWAN,		ItemMallGetItem_Taiwan);

		// USA
		DBMessageMap(DBMSG_REQUEST_GET_MYCASH_INFO_USA, ItemMallMyCashInfo_USA);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_GIFTITEM_USA, ItemMallGiftItem_USA);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_BUYITEM_USA, ItemMallBuyItem_USA);
		DBMessageMap(DBMSG_REQUEST_ITEMMALLGETITEM_USA, ItemMallGetItem_USA);

		//KHY - 0304 - 중국 NHNChinaBilling - 
		DBMessageMap(DBMSG_REQUEST_GET_MYCASH_INFO_NHNCHINA, ItemMallMyCashInfo_NHNChina);  // 잔액확인.
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_GIFTITEM_NHNCHINA, ItemMallGiftItem_NHNChina);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_BUYITEM_NHNCHINA, ItemMallBuyItem_NHNChina);
		DBMessageMap(DBMSG_REQUEST_ITEMMALLGETITEM_NHNCHINA, ItemMallGetItem_NHNChina);

		// [종호] Germany
		DBMessageMap(DBMSG_REQUEST_GET_MYCASH_INFO_GERMANY, ItemMallMyCashInfo_Germany);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_BUYITEM_GERMANY, ItemMallBuyItem_Germany);
		DBMessageMap(DBMSG_REQUEST_ITEMMALL_GIFTITEM_GERMANY, ItemMallGiftItem_Germany);
		DBMessageMap(DBMSG_REQUEST_ITEMMALLGETITEM_GERMANY, ItemMallGetItem_Germany);

		/*/////////////////////////////////////////////////
			RealEstateMarket
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_REALESTATEMARKET_ORDERSELL, RealEstateMarketOrderSell);
		DBMessageMap(DBMSG_REQUEST_REALESTATEMARKET_CANCELORDER, RealEstateMarketCancelOrder);
		DBMessageMap(DBMSG_REQUEST_REALESTATEMARKET_MYORDER, RealEstateMarketMyOrder);
		DBMessageMap(DBMSG_REQUEST_REALESTATEMARKET_GETMANYORDER, RealEstateMarketGetManyOrder);
		DBMessageMap(DBMSG_REQUEST_REALESTATEMARKET_SETFEE, RealEstateMarketSetFee);
		DBMessageMap(DBMSG_REQUEST_REDUCEREALESTATEORDERPRICE, RealEstateMarketReduceOrderPrice);
		DBMessageMap(DBMSG_REQUEST_REALESTATEMARKET_BUY, RealEstateMarketBuy);
		DBMessageMap(DBMSG_REQUEST_REALESTATEMARKET_DELORDER, RealEstateMarketDelOrder);

		/*/////////////////////////////////////////////////
		// Guilty
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_PRISONUSER, PrisonUser);
		DBMessageMap(DBMSG_REQUEST_GUILTYINFORECORD, GuiltyInfoRecord);

		
		/*/////////////////////////////////////////////////
			SummonMarket
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SUMMONMARKET_SETFEE, SummonMarketSetFee);
		DBMessageMap(DBMSG_REQUEST_SUMMONMARKET_ORDERSELL, SummonMarketOrderSell);
		DBMessageMap(DBMSG_REQUEST_SUMMONMARKET_GETMANYORDER, SummonMarketGetManyOrder);
		DBMessageMap(DBMSG_REQUEST_SUMMONMARKET_MYORDER, SummonMarketMyOrder);
		DBMessageMap(DBMSG_REQUEST_SUMMONMARKET_CANCELORDER, SummonMarketCancelOrder);
		DBMessageMap(DBMSG_REQUEST_SUMMONMARKET_BUY, SummonMarketBuy);
		DBMessageMap(DBMSG_REQUEST_REDUCESUMMONORDERPRICE, SummonMarketReduceOrderPrice);


		/*/////////////////////////////////////////////////
			Land
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_MANYLANDUNIT, LandUnitManyGet);
		DBMessageMap(DBMSG_REQUEST_LAND_CHANGECONTRACT, LandContractChange);
		DBMessageMap(DBMSG_REQUEST_LAND_RENTAPPLY, LandRentApply);
		DBMessageMap(DBMSG_REQUEST_LAND_CANCELRENTAPPLY, LandRentApplyCancel);
		DBMessageMap(DBMSG_REQUEST_LAND_APPLYERLIST, LandApplyerListGet);
		DBMessageMap(DBMSG_REQUEST_LAND_DELAPPLYER, LandApplyerDel);
		DBMessageMap(DBMSG_REQUEST_LAND_RENT, LandRent);
		DBMessageMap(DBMSG_REQUEST_LAND_PAYRENTFEE, LandPayRentFee);
		DBMessageMap(DBMSG_REQUEST_LAND_RECALL, LandRecall);
		DBMessageMap(DBMSG_REQUEST_AGRICULTUREPAYMONEYFORRENTLAND, AgriculturePayMoneyForRent);
		DBMessageMap(DBMSG_REQUEST_GETAGRICULTURELEASEINFO, GetAgricultureLeaseInfo);
		DBMessageMap(DBMSG_REQUEST_EXPIREDAGRICULTERLEASE, ExpiredAgricultureLease);

		/*/////////////////////////////////////////////////
			Mine
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_MANYMINEUNIT, MineUnitManyGet);
		DBMessageMap(DBMSG_REQUEST_MINE_CHANGECONTRACT, MineContractChange);
		DBMessageMap(DBMSG_REQUEST_MINE_RENTAPPLY, MineRentApply);
		DBMessageMap(DBMSG_REQUEST_MINE_CANCELRENTAPPLY, MineRentApplyCancel);
		DBMessageMap(DBMSG_REQUEST_MINE_APPLYERLIST, MineApplyerListGet);
		DBMessageMap(DBMSG_REQUEST_MINE_DELAPPLYER, MineApplyerDel);
		DBMessageMap(DBMSG_REQUEST_MINE_RENT, MineRent);
		DBMessageMap(DBMSG_REQUEST_MINE_PAYRENTFEE, MinePayRentFee);
		DBMessageMap(DBMSG_REQUEST_MINE_RECALL, MineRecall);

		/*/////////////////////////////////////////////////
		Guild
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_GUILD_CREATEGUILD, CreateGuild);
		DBMessageMap(DBMSG_REQUEST_GUILD_DESTROYGUILD, DestroyGuild);
		DBMessageMap(DBMSG_REQUEST_GUILD_CHANGEVILLAGE, GuildChangeVillage);
		DBMessageMap(DBMSG_REQUEST_GUILD_CONFIRMJOINGUILD, ConfirmJoinGuild);
		DBMessageMap(DBMSG_REQUEST_GUILD_LEAVEGUILD, LeaveGuild);
		DBMessageMap(DBMSG_REQUEST_GUILD_JOINGUILD, JoinGuild);
		DBMessageMap(DBMSG_REQUEST_GUILD_GETGUILDINFO, GetGuildInfo);
		DBMessageMap(DBMSG_REQUEST_GUILD_CONFIRMCREATEGUILD, ConfirmCreateGuild);
		DBMessageMap(DBMSG_REQUEST_GUILD_GIVEGUILDMONEY, GiveGuildMoney);
		DBMessageMap(DBMSG_REQUEST_GUILD_SETSECONDMASTER, GuildSetSecondMaster);
		DBMessageMap(DBMSG_REQUEST_GUILD_SETSTRFEERATE, GuildSetStrFeeRate);
		DBMessageMap(DBMSG_REQUEST_GUILD_CHANGEGUILDPOINT, ChangeGuildPoint);
		DBMessageMap(DBMSG_REQUEST_GUILD_GIVE_FEE, GuildGiveFee);
		DBMessageMap(DBMSG_REQUEST_GUILD_CHANGESTATUS, GuildChangeStatus);
		DBMessageMap(DBMSG_REQUEST_GUILD_LEVELUP, GuildLevelUp);
		DBMessageMap(DBMSG_REQUEST_GUILD_CHANGENAME, GuildChangeName);
		DBMessageMap(DBMSG_REQUEST_GUILD_SETMARK, GuildSetMark);
		DBMessageMap(DBMSG_REQUEST_GUILD_CHANGEMASTER, GuildChangeMaster);
		DBMessageMap(DBMSG_REQUEST_GUILDWAR_ADDRESERVE, GuildWarAddReserve);
		DBMessageMap(DBMSG_REQUEST_GUILDWAR_RESERVELIST, GuildWarReserveList);
		DBMessageMap(DBMSG_REQUEST_GUILDDUNGEON_APPLY, GuildDungeonApply);
		DBMessageMap(DBMSG_REQUEST_GUILDDUNGEON_SAVESTATUS, GuildDungeonSaveStatus);
		DBMessageMap(DBMSG_REQUEST_GUILDDUNGEON_LOADSTATUS, GuildDungeonLoadStatus);
		DBMessageMap(DBMSG_REQUEST_GUILDDUNGEON_TRANSFEROWNERSHIP, GuildDungeonTransferOwnership);
		DBMessageMap(DBMSG_REQUEST_GUILD_CHECKDESTROY_MONTH, GuildCheckDestroyMonth);
		DBMessageMap(DBMSG_REQUEST_GUILD_NOTCONNECT_CHANGEMASTER, GuildNotCOnnectChangeMaster);

		DBMessageMap(DBMSG_REQUEST_GUILDHUNTMAPWAR_REGLIST, SaveGuildHuntMapWarList);

		DBMessageMap(DBMSG_REQUEST_GUILDHUNTMAPWAR_GET_REGLIST, GetGuildHuntMapWarList);
		DBMessageMap(DBMSG_REQUEST_GUILDHUNTMAPWAR_RANK_GET, GetGuildHuntMapRank);


		/*/////////////////////////////////////////////////
			GeneralMeeting
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_GENERALMEETING_SUGGEST_CHIEF, GeneralMeetingSuggestChief);
		DBMessageMap(DBMSG_REQUEST_GENERALMEETING_INFO, GeneralMeetingInfo);
		DBMessageMap(DBMSG_REQUEST_GENERALMEETING_VOTE, GeneralMeetingVote);
		DBMessageMap(DBMSG_REQUEST_GENERALMEETING_CLOSE_CHIEF, GeneralMeetingCloseChief);
		DBMessageMap(DBMSG_REQUEST_GENERALMEETING_ACCEPTCANDIDATE, GeneralMeetingAcceptCandidate);
		DBMessageMap(DBMSG_REQUEST_GENERALMEETING_SETVOTERSTATE, GeneralMeetingSetVoterState);
		DBMessageMap(DBMSG_REQUEST_GENERALMEETING_SUGGEST_ON_OFF, GeneralMeetingSuggestOnOff);


		/*/////////////////////////////////////////////////
			Cash
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_GIVECASH, GiveCash);


		/*/////////////////////////////////////////////////
			Feast
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_FEAST_RESERVE, FeastReserve);
		DBMessageMap(DBMSG_REQUEST_FEAST_ADDPRESENTLIST, FeastAddPresentList);
		DBMessageMap(DBMSG_REQUEST_FEAST_DELPRESENTLIST, FeastDelPresentList);
		DBMessageMap(DBMSG_REQUEST_FEAST_STGITEMIN, FeastStgItemIn);
		DBMessageMap(DBMSG_REQUEST_FEAST_STGITEMOUT, FeastStgItemOut);
		DBMessageMap(DBMSG_REQUEST_FEAST_SETCOST, FeastSetCost);
		DBMessageMap(DBMSG_REQUEST_FEAST_OPEN, FeastOpen );
		DBMessageMap(DBMSG_REQUEST_FEAST_CLOSE, FeastClose );
		DBMessageMap(DBMSG_REQUEST_FEAST_GIVEITEM, FeastGiveItem );
		DBMessageMap(DBMSG_REQUEST_FEAST_USEEFFECT, FeastUseEffect);
		DBMessageMap(DBMSG_REQUEST_PARTYHALL_RESERVATION, PartyHallReservation);
		DBMessageMap(DBMSG_REQUEST_PARTYHALL_ADD_DECORATION, PartyHallAddDecoration);
		DBMessageMap(DBMSG_REQUEST_PARTYHALL_ADD_PRESENT, PartyHallAddPresent);
		DBMessageMap(DBMSG_REQUEST_PARTYHALL_GIVEITEM, PartyHallGiveItem);
		DBMessageMap(DBMSG_REQUEST_PARTYHALL_RESERVELIST, PartyHallReserveList);
		DBMessageMap(DBMSG_REQUEST_PARTYHALL_DECORATION_LIST, PartyHallDecorationList);

		/*/////////////////////////////////////////////////
			SummonHeroMarket
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SUMMONHEROMARKET_SETFEE, SummonHeroMarketSetFee);
		DBMessageMap(DBMSG_REQUEST_SUMMONHEROMARKET_ORDERSELL, SummonHeroMarketOrderSell);
		DBMessageMap(DBMSG_REQUEST_SUMMONHEROMARKET_GETMANYORDER, SummonHeroMarketGetManyOrder);
		DBMessageMap(DBMSG_REQUEST_SUMMONHEROMARKET_MYORDER, SummonHeroMarketMyOrder);
		DBMessageMap(DBMSG_REQUEST_SUMMONHEROMARKET_CANCELORDER, SummonHeroMarketCancelOrder);
		DBMessageMap(DBMSG_REQUEST_SUMMONHEROMARKET_BUY, SummonHeroMarketBuy);
		DBMessageMap(DBMSG_REQUEST_REDUCESUMMONHEROORDERPRICE, SummonHeroMarketReduceOrderPrice);

		/*/////////////////////////////////////////////////
		// WorldMoney
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_DISTRIBUTION_WORLDMONEY, WorldMoneyDistribution);
		DBMessageMap(DBMSG_REQUEST_WORLDMONEY_TRADELIST, WorldMoneyTradeList);
		DBMessageMap(DBMSG_REQUEST_WORLDMONEY_SELLORDER, WorldMoneySellOrder);
		DBMessageMap(DBMSG_REQUEST_WORLDMONEY_CANCELORDER, WorldMoneyCancelOrder);
		DBMessageMap(DBMSG_REQUEST_WORLDMONEY_BUYORDER, WorldMoneyBuyOrder);
		DBMessageMap(DBMSG_REQUEST_WORLDMONEY_SETAVERAGEPRICE, WorldMoneySetAveragePrice);
		DBMessageMap(DBMSG_REQUEST_WORLDMONEY_REDUCEPRICE, WorldMoneyReducePrice);
		DBMessageMap(DBMSG_REQUEST_WORLDMONEY_SET_CHAR, WorldMoneySetChar);

		/*/////////////////////////////////////////////////
		// WorldPassport
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_WORLDPASSPORT_SET,		WorldPassportSet);
		DBMessageMap(DBMSG_REQUEST_WORLDPASSPORT_EXTEND,	WorldPassportExtend);


		/*/////////////////////////////////////////////////
		// TradeServer
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_TRADESERVER_SET_SELLINFO, TradeServerSellInfo);
		DBMessageMap(DBMSG_REQUEST_TRADESERVER_BUYITEM, TradeServerBuyItem);
		DBMessageMap(DBMSG_REQUEST_TRADESERVER_CANCELORDER, TradeServerCancelOrder);
		DBMessageMap(DBMSG_REQUEST_TRADESERVER_SETORDER, TradeServerSetOrder);
		DBMessageMap(DBMSG_REQUEST_TRADESERVER_GET_WITHDRAW_ACU, TradeServerGetWithdrawAcu);
		

		/*/////////////////////////////////////////////////
		// Gacha
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_GACHABUY, GachaBuy);

		/*/////////////////////////////////////////////////
		// Empire
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_CREATEEMPIRE, CreateEmpire);
		DBMessageMap(DBMSG_REQUEST_DESTROYEMPIRE, DestroyEmpire);
		DBMessageMap(DBMSG_REQUEST_JOINEMPIRE, JoinEmpire);
		DBMessageMap(DBMSG_REQUEST_SECEDEEMPIRE, SecedeEmpire);
		DBMessageMap(DBMSG_REQUEST_ALLEMPIREINFO, AllEmpireInfo);
		DBMessageMap(DBMSG_REQUEST_SETEMPIREMONEY, SetEmpireMoney);
		DBMessageMap(DBMSG_REQUEST_CHANGEEMPEROR, ChangeEmperor);

		/*/////////////////////////////////////////////////
		// Log
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_WRITELOG, WriteLog );
		DBMessageMap(DBMSG_REQUEST_WRITELOG_PCROOM, WriteLogPCRoom ); 
		DBMessageMap(DBMSG_REQUEST_WRITELOGTODB, WriteLogToDB);
		DBMessageMap(DBMSG_REQUEST_LOG_GETHAPPYBEANQUESTINFO, WriteLogSetHappyBeanClearInfo);
		//KHY - 0906 - 게임방 이벤트 쿠폰.
		DBMessageMap(DBMSG_REQUEST_LOG_GETEVENTCOUPONINFO, WriteLogSetEventCouponInfo);

		/*/////////////////////////////////////////////////
		// GMTool
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_GMTOOL_CHARBASICINFO, BasicInfoByName);
		DBMessageMap(DBMSG_REQUEST_GMTOOL_SKILLINFO, SkillByName);
		DBMessageMap(DBMSG_REQUEST_GMTOOL_ANIMALLINFO, AnimalByName);
		DBMessageMap(DBMSG_REQUEST_GMTOOL_INVENTORYINFO, InventoryByName);
		DBMessageMap(DBMSG_REQUEST_GMTOOL_REALESTATEINFO, RealestateByName);
		DBMessageMap(DBMSG_REQUEST_GMTOOL_SUMMONINFO, SummonByName);

		/*/////////////////////////////////////////////////
		// 명나라 상인 구매제한
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_SET_FOREIGNMERCHANTNOWMONEY, SetForeignMerchantNowMoney);
		DBMessageMap(DBMSG_REQUEST_GET_FOREIGNMERCHANTNOWMONEY, GetForeignMerchantNowMoney);
		DBMessageMap(DBMSG_REQUEST_SET_FOREIGNMERCHANTBUYLIMIT, SetForeignMerchantBuyLimit);
		DBMessageMap(DBMSG_REQUEST_GET_FOREIGNMERCHANTBUYLIMIT, GetForeignMerchantBuyLimit);

		/*/////////////////////////////////////////////////
		// PVP League
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_PVP_LEAGUE_INFOGET,				PVP_LeagueInfoGet);
		DBMessageMap(DBMSG_REQUEST_PVP_LEAGUE_UI_INFOGET,			PVP_LeagueUIInfoGet);
		DBMessageMap(DBMSG_REQUEST_PVP_LEAGUE_BATTLE_END,			PVP_LeagueBattleEnd);
		DBMessageMap(DBMSG_REQUEST_PVP_LEAGUE_BATTLE_CHANGEGRADE,	PVP_LeagueBattleChangeGrade);

		/*/////////////////////////////////////////////////
		// 엘릭서
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_ELIXIR_SET, SetElixir);
		DBMessageMap(DBMSG_REQUEST_ELIXIR_GET, GetElixir);

		DBMessageMap(DBMSG_REQUEST_PROMOTER_INFO_GET, PromoterInfoGet);

		/*/////////////////////////////////////////////////
		// 발렌타인데이 
		/////////////////////////////////////////////////*/
		DBMessageMap(DBMSG_REQUEST_VALANTINE_REQUEST, SetValentineWarInfo);
		DBMessageMap(DBMSG_REQUEST_VALANTINE_INFO_GET, GetValentineWarInfo);
		DBMessageMap(DBMSG_REQUEST_VALANTINE_GET_PRIZE, ValentineWarGetPrize);


		// [진성] 통합마켓.
		DBMessageMap(DBMSG_REQUEST_NEWORDERMARKET,				NewOrderMarket);
		DBMessageMap(DBMSG_REQUEST_NEWMARKETACCOUNT,			UnionMarketAccount);
		DBMessageMap(DBMSG_REQUEST_NEWTRADEMARKET,				TradeUnionMarket);
		DBMessageMap(DBMSG_REQUEST_GETITEMFROMNEWARKET, 		GetItemFromUnionMarket);
		DBMessageMap(DBMSG_REQUEST_NEWMARKETITEMLIST,			UnionMarketItemList);
		DBMessageMap(DBMSG_REQUEST_NEWMARKETRESULT,				UnionMarketResult);
		DBMessageMap(DBMSG_REQUEST_NEWMARKET_INCREASEBANKMONEY, UnionMarketInputBankMoney);
		DBMessageMap(DBMSG_REQUEST_SETNEWMARKETFEE,				SetUinonMarketFee);
		DBMessageMap(DBMSG_REQUEST_MANYITEMNEWPRICE,			ManyNewItemPrice);
		DBMessageMap(DBMSG_REQUEST_SETITEMNEWPRICE,				SetNewItemPrice);
		DBMessageMap(DBMSG_REQUEST_NEWMARKETREDUCEPRICENOTICE,	NewMarketReducePriceNotice);

		// [진성] 핸드폰 인증 여부.
		DBMessageMap(DBMSG_REQUEST_CELLULAR_PHONE_AUTH,			GetCellularPhoneAuth);

		DBMessageMap(DBMSG_REQUEST_GUILDHUNTMAPWAR_RESULT,		EndGuildHuntMapWar);

		// [영훈] 결혼 시스템
		DBMessageMap(DBMSG_REQUEST_SET_MARRY,					SetMarry);
		DBMessageMap(DBMSG_REQUEST_GET_MARRY_INFO,				GetMarryInfo);

		DBMessageMap(DBMSG_REQUEST_MARRY_ITEMGET_SET,			MarryItemGetSet);
		DBMessageMap(DBMSG_REQUEST_MARRY_LOVE_SET,				MarryLoveSet);
		DBMessageMap(DBMSG_REQUEST_WEDDING_RESERVE_LIST_GET,	WeddingReserveListGet);
		DBMessageMap(DBMSG_REQUEST_WEDDING_RESERVE,				WeddingReserve );
		DBMessageMap(DBMSG_REQUEST_WEDDING_RESERVE_CANCEL,		WeddingReserveCancel);

		// 가족 시스템
		DBMessageMap(DBMSG_REQUEST_FAMILY_ADD_CHILDREN,			Add_Family_Children );		// 자식 추가
		DBMessageMap(DBMSG_REQUEST_FAMILY_DEL_CHILDREN,			Del_Family_Children );		// 자식 삭제

		DBMessageMap(DBMSG_REQUEST_GET_FAMILYINFO,				Get_Family_info );			// 자신의 가족정보를 가지고 온다.
		
		DBMessageMap(DBMSG_REQUEST_FAMILY_PLUS_ADD_DATE,		Plus_FamilyEffect_date );	// 대상의 가족 효과 연장
		DBMessageMap(DBMSG_REQUEST_FAMILY_SET_COOKIE_EFFECT,	Set_Cookie_Effect );		// 간식용 쿠키 효과
		


		//	[종호] 통화량 조절 - 사용 가능 금액
		DBMessageMap(DBMSG_REQUEST_GETBALANCECANUSECURRENCY,	GetBalanceCanUseCurrency);
		DBMessageMap(DBMSG_REQUEST_SETBALANCECANUSECURRENCY,	SetBalanceCanUseCurrency);

		// [영훈] 수호정령
		DBMessageMap(DBMSG_REQUEST_SOULGUARD_CREATE,			SoulGuardCreate);
		DBMessageMap(DBMSG_REQUEST_SOULGUARD_GET,				SoulGuardGet);
		DBMessageMap(DBMSG_REQUEST_SOULGUARD_FELLOWSHIP_SET,	SoulGuardFellowShipSet);
		DBMessageMap(DBMSG_REQUEST_SOULGUARD_CHANGE_NAME,		SoulGuardChangeName);
		DBMessageMap(DBMSG_REQUEST_SOULGUARD_DEAD,				SoulGuardDead);

		DBMessageMap(DBMSG_REQUEST_SOULGUARD_HOUSE_GET,			SoulGuardHouse_Get);
		DBMessageMap(DBMSG_REQUEST_SOULGUARD_HOUSE_CHANGE,		SoulGuardHouse_Change);
		DBMessageMap(DBMSG_REQUEST_SOULGUARD_HOUSE_CREATE,		SoulGuardHouse_Create);
		
		// [기형] 휴면 계정 시스템
		DBMessageMap(DBMSG_REQUEST_DORMANCY_POINTITEM_BUY,		DormancyBuyPointItem);
		DBMessageMap(DBMSG_REQUEST_DORMANCY_POINTINFO_SET,		DormancySetPointInfo);
		DBMessageMap(DBMSG_REQUEST_DORMANCY_STATE_CHANGE,		DormancyChangeState);

		// [영훈] 캐릭터 보관소에서 이동하기
		DBMessageMap(DBMSG_REQUEST_PERSONSLOTNUMCHANGE,			PersonSlotNumChange);

		// [진성] 스킬북 마법.
		
		DBMessageMap(DBMSG_REQUEST_SKILLBOOK_MAGICINFO_GET,		SkillBookMagicInfoGet);
		DBMessageMap(DBMSG_REQUEST_SKILLBOOK_MAGIC_ADD,			SkillBookMagicAdd);
		DBMessageMap(DBMSG_REQUEST_SKILLBOOK_MAGIC_DEL,			SkillBookMagicDel);
		DBMessageMap(DBMSG_REQUEST_SKILLBOOK_SUBMAGIC_ADD,		SkillBookSubMagicAdd);
		DBMessageMap(DBMSG_REQUEST_SKILLBOOK_SUBMAGIC_DEL,		SkillBookSubMagicDel);
		
		// [지연] 복불복 - 이벤트 보상 금액 저장
		DBMessageMap(DBMSG_REQUEST_BBB_SET_REWARD_MONEY,		Set_bbbRewardAccumPrice);
		DBMessageMap(DBMSG_REQUEST_BBB_GET_REWARD_MONEY,		Get_bbbRewardAccumPrice);

		// [지연] 군주데이미션 : 미션 부여 정보를 디비에 저장, 요청
		DBMessageMap(DBMSG_REQUEST_GOONZUDAYMISSIONINFO_SET,	GoonzuDayMissionSet);
		DBMessageMap(DBMSG_REQUEST_GOONZUDAYMISSIONINFO_GET,	GoonzuDayMissionGet);
		
		//DBMessageMap(DBMSG_REQUEST_NEWS, News);

		// [성웅] 티켓을 사용 하여 목표 지역으로 바로 날라간다 
		DBMessageMap(DBMSG_REQUEST_WARP_TARGETWORLD, Warp_TargetWorld);

	}
}

DBGameProtocol::~DBGameProtocol()
{
	m_odbc.Disconnect();
	if(m_pDBMsgPtr != NULL)
		delete [] m_pDBMsgPtr;
}

void DBGameProtocol::Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW )
{

	m_odbc.Connect( "SQL Server", szDBIP, szDatabase, szUserID, szUserPW );

	m_odbc.SetErroyQueryFunc( RichODBC_ErroyQuery );

	m_SF.CreateFactory( GetSessionManager(), 1, 10000 * 1000, 10000 * 1000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );
	
	GetListen()->CreateListen( GetSessionManager(), usPort );

	SetFPS( 15 );

	SetDefinedNumber();

	InitBillingProcess();
}

void DBGameProtocol::InitBillingProcess( )
{
	if(g_BillingMode == HANGAME_BILLINGMODE_SOCKET)
		clHangameBillingProcess.Init( g_BillingServerIP, g_BillingServerPort);
	else if(g_BillingMode == HANGAME_BILLINGMODE_NEVERBILL)
		clHannEverBillingProcess.Init( "K_GOONZU" );

	clIJJIBillingProcess.Init(g_BillingServerIP, g_BillingServerPort, g_BillingTimeOut );
}

void DBGameProtocol::Update()
{
/*
	SYSTEMTIME systime;
	GetLocalTime( &systime );

//	if( systime.wMinute >= 59 || systime.wMinute <= 5 ) return;
	
	if( givecashcount == 0 ) return;
	
	NDate date;
	
	char szStartDt[ 32 ];
	char szEndDt[ 32 ];

	date.SetDate( systime.wYear, systime.wMonth, systime.wDay );

	sprintf( szStartDt, "%4d%02d%02d", date.m_uiYear, date.m_uiMonth, date.m_uiDay );

	date.AddDays( 3 );

	sprintf( szEndDt, "%4d%02d%02d", date.m_uiYear, date.m_uiMonth, date.m_uiDay );


	IPGTXCli *pIPGTXCli = NULL;

	HRESULT hr = CoCreateInstance( 
		CLSID_PGTXCli, 
		NULL, 
		CLSCTX_LOCAL_SERVER,
		IID_IPGTXCli,
		(void**)&pIPGTXCli );

	if( FAILED( hr ) ) return;

	_variant_t ret;
	_variant_t userid, userno, companycd, eventcd, amount;
	_variant_t startdt, enddt, bxaid, retval, retmsg;
	

	for( int i = 0; i < givecashcount; ++i ) {

		userid.SetString( givecashbuffer[ i ].id );
		userno.SetString( "" );
		companycd.SetString( "BX00" );
		eventcd.SetString( "IP00000010" );	
		amount = (long)givecashbuffer[ i ].amount;
		startdt.SetString( szStartDt );
		enddt.SetString( szEndDt );
		bxaid.SetString( "" );
		retval.SetString( "" );
		retmsg.SetString( "" );

		ret = pIPGTXCli->PkgMeterRate(
			&userid,
			&userno,
			&companycd,
			&eventcd,
			&amount,
			&startdt,
			&enddt,
			&bxaid,
			&retval,
			&retmsg
			);

		// 로그 작성
		{
			char log[512] = "";
			sprintf(log, "ret = %d, Amount = %d"
					, ret.intVal, givecashbuffer[ i ].amount
					);
			sDBLogMsg_QueryString LogMsg("Give Cash", givecashbuffer[ i ].personid, NULL, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		//}	
	}

	pIPGTXCli->Release();

	ZeroMemory( givecashbuffer, sizeof( givecashbuffer ) );
	givecashcount = 0;
*/
}

void DBGameProtocol::MessageProc()
{
	if( m_pPacket == NULL ) return;

	if(m_pPacket->usCmd >= 0 && m_pPacket->usCmd < DBMSG_END)
	{
		(this->*(this->m_pDBMsgPtr[m_pPacket->usCmd].Func))();
	}
	else
	{
		NullFuncPtr();
	}
}

void DBGameProtocol::SessionProc( ITZSession *pSession )
{
	m_pSession = pSession;

	SI32 count = pSession->GetRecvBuffer()->GetPacketCount();


	// 현재 버퍼에 있는 패킷 개수 만큼 메시지 처리 루틴 반복 
	for( int i = 0; i < count; ++i ) 
	{
		// 버퍼에서 패킷을 하나 꺼내온다 
		m_pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );
																								
		if( m_pPacket == NULL ) return;	// 패킷이 없으면 나간다

		MessageProc();

		InterlockedIncrement( (LPLONG)&g_requestCount );

		if(!m_odbc.GetReconnectStat())
			pSession->GetRecvBuffer()->RemoveFirstPacket();

	}

}

void DBGameProtocol::SendPacket( BYTE *pPacket )
{
	if( m_pSession->GetSendBuffer()->Write( pPacket ) == FALSE )
	{
		OutputDebugString("!!!!!! ---- Write Buffer is Full ---- !!!!!!");
	}

/*	if( ((sPacketHeader*)pPacket)->usCmd != 5018 )
	{
		char	szBuf[1024];
		SYSTEMTIME	st;
		GetLocalTime(&st);
		wsprintf( szBuf, "%02d:%02d:%02d.%03d\tSendPacket\t%d\t%d\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
			((sPacketHeader*)pPacket)->usCmd, ((sPacketHeader*)pPacket)->usSize );
		OutputDebugString(szBuf);
	}*/

	InterlockedIncrement( (LPLONG)&g_responseCount );
}

void DBGameProtocol::NullFuncPtr()
{
	char buffer[128];
	sprintf(buffer, "Not Defined Msg:%d", m_pPacket->usCmd);
	MessageBox(NULL, "DBManager Error", buffer, MB_OK);
}


void DBGameProtocol::WriteLog()
{
	sDBRequest_WriteLog *pRecvMsg = (sDBRequest_WriteLog *)m_pPacket;

	sDBLogMsg_QueryString LogMsg( pRecvMsg->szCmd, pRecvMsg->siPersonID, pRecvMsg->siDestPersonID, pRecvMsg->siItemUnique, pRecvMsg->szStr );
	SendLogMsg((sPacketHeader *)&LogMsg);
}

void DBGameProtocol::WriteLogToDB()
{
	sDBRequest_WriteLogToDB *pRecvMsg = (sDBRequest_WriteLogToDB *)m_pPacket;

	sDBLogMsg_NewLog LogMsg;

	//MStrCpy(LogMsg.m_szDate,	pRecvMsg->m_szDate, sizeof(LogMsg.m_szDate));
	LogMsg.m_siUseLog			= pRecvMsg->m_siUseLog;
	memcpy(&LogMsg.m_stTime,		&pRecvMsg->m_stTime, sizeof(LogMsg.m_stTime));
	memcpy(&LogMsg.m_szServerName,	&pRecvMsg->m_szServerName, sizeof(LogMsg.m_szServerName));
	MStrCpy(LogMsg.m_szIP,		pRecvMsg->m_szIP,	sizeof(LogMsg.m_szIP));
	LogMsg.m_siCommandType		= pRecvMsg->m_siCommandType;
	LogMsg.m_siCommandIndex		= pRecvMsg->m_siCommandIndex;
	LogMsg.m_uiReason			= pRecvMsg->m_uiReason;
	LogMsg.m_siIDNum			= pRecvMsg->m_siIDNum;
	MStrCpy(LogMsg.m_szPersonName,	pRecvMsg->m_szPersonName,	sizeof(LogMsg.m_szPersonName));
	LogMsg.m_siPersonID			= pRecvMsg->m_siPersonID;
	LogMsg.m_siLevel			= pRecvMsg->m_siLevel;
	LogMsg.m_siExp				= pRecvMsg->m_siExp;
	LogMsg.m_siMoney			= pRecvMsg->m_siMoney;
	LogMsg.m_siMoneySum			= pRecvMsg->m_siMoneySum;
	LogMsg.m_clItem.Set(&pRecvMsg->m_clItem);
	LogMsg.m_siItemSum			= pRecvMsg->m_siItemSum;
	LogMsg.m_clSummon.Set(&pRecvMsg->m_clSummon);
	LogMsg.m_siMonKind			= pRecvMsg->m_siMonKind;
	LogMsg.m_siTargetPersonID	= pRecvMsg->m_siTargetPersonID;
	LogMsg.m_siVillageUnique	= pRecvMsg->m_siVillageUnique;
	LogMsg.m_siNpcKind			= pRecvMsg->m_siNpcKind;
	LogMsg.m_clHealthInfo.Set(&pRecvMsg->m_clHealthInfo);
	LogMsg.m_siRPPoint			= pRecvMsg->m_siRPPoint;
	LogMsg.m_siFameExp			= pRecvMsg->m_siFameExp;
	LogMsg.m_siParam1			= pRecvMsg->m_siParam1;
	LogMsg.m_siParam2			= pRecvMsg->m_siParam2;
	LogMsg.m_siParam3			= pRecvMsg->m_siParam3;
	LogMsg.m_siParam4			= pRecvMsg->m_siParam4;
	LogMsg.m_siParam5			= pRecvMsg->m_siParam5;
	MStrCpy(LogMsg.m_szCharParam1, pRecvMsg->m_szCharParam1, sizeof(LogMsg.m_szCharParam1));
	MStrCpy(LogMsg.m_szCharParam2, pRecvMsg->m_szCharParam2, sizeof(LogMsg.m_szCharParam2));

	SendLogMsg((sPacketHeader *)&LogMsg);
}

void DBGameProtocol::WriteLogPCRoom()
{
	sDBRequest_PCRoom_Log *pRecvMsg = (sDBRequest_PCRoom_Log *)m_pPacket;

	pRecvMsg->packetHeader.usCmd = DBLOGMSG_REQUEST_PCROOM_LOG;

	SendLogMsg( (sPacketHeader *)pRecvMsg );

}

void DBGameProtocol::WriteLogSetHappyBeanClearInfo()
{
	// 받은 패킷
	sDBRequest_Log_SetHappyBeanClearInfo *pRecvMsg = (sDBRequest_Log_SetHappyBeanClearInfo *)m_pPacket;

	pRecvMsg->packetHeader.usCmd = DBLOGMSG_REQUEST_SETHAPPYBEANCLEARINFO_LOG;

	SendLogMsg( (sPacketHeader *)pRecvMsg );
}

//KHY - 0906 - 게임방 이벤트 쿠폰.
void DBGameProtocol::WriteLogSetEventCouponInfo()
{
	// 받은 패킷
	sDBRequest_Log_SetEventCouponInfo *pRecvMsg = (sDBRequest_Log_SetEventCouponInfo *)m_pPacket;

	pRecvMsg->packetHeader.usCmd = DBLOGMSG_REQUEST_SETEVENTCOUPONINFO_LOG;

	SendLogMsg( (sPacketHeader *)pRecvMsg );
}

//void DBGameProtocol::News()
//{
//	// 받은 패킷
//	sDBRequest_News *pRecvMsg = (sDBRequest_News *)m_pPacket;
//	
//	// 보낼 패킷
//	sDBResponse_News sendMsg;
//	
//	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWS;
//	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//
//	char szTime[20];
//	ZeroMemory(szTime, sizeof(szTime));
//
//	sprintf( szTime, "%d-%d-%d %d:%d:%d"
//			, pRecvMsg->sTime.wYear, pRecvMsg->sTime.wMonth, pRecvMsg->sTime.wDay
//			, pRecvMsg->sTime.wHour, pRecvMsg->sTime.wMinute, pRecvMsg->sTime.wSecond);
//
//	// DB Query
//	BEGIN_SP( "rsp_News" )
//		m_odbc.SetParam( SQL_CHAR, szTime, sizeof(szTime) );
//	END_SP()
//
//	EXEC_SQL()
//
//	BEGIN_FETCHDATA()
//		ZeroMemory( sendMsg.szNewsTitle, sizeof(sendMsg.szNewsTitle) );
//		ZeroMemory( sendMsg.szNewspaperName, sizeof(sendMsg.szNewspaperName) );
//		ZeroMemory( sendMsg.szNewURL, sizeof(sendMsg.szNewURL) );
//
//		sendMsg.siResult = 1;
//		m_odbc.GetData( sendMsg.szNewspaperName, sizeof(sendMsg.szNewspaperName) );
//		m_odbc.GetData( sendMsg.szNewsTitle, sizeof(sendMsg.szNewsTitle) );
//		m_odbc.GetData( sendMsg.szNewURL, sizeof(sendMsg.szNewURL) );
//		// send
//		SendPacket( (BYTE *)&sendMsg );
//	END_FETCHDATA()
//}
