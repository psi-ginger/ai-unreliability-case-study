#ifndef _DBGAMEPROTOCOL_H
#define _DBGAMEPROTOCOL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDB.h"
#include "DBMsg-System.h"
#include "ITZNetworkThread.h"
#include "..\..\GoonzuWorld\CommonLogic\House\House.h"
#include "HanGameBillingProcess/NHanGameBillingProcess.h"
#include "HanGameBillingProcess/NIJJIBillingProcess.h"
#include "HanGameBillingProcess/NHannEverBillingProcess.h"
#include "BillManager_Germany.h"

struct givecash
{
	SI32	personid;
	char	id[ 32];
	SI32	amount;
};


class cltGameDate
{
public:
	SI32	siYear;
	SI32	siMonth;
	SI32	siDay;
	SI32	siHour;
	SI32	siMinute;

	cltGameDate()
	{
		ZeroMemory(this, sizeof(cltGameDate));
	}

	cltGameDate(cltDate *pcltDate)
	{
		siYear		= (SI32)(pcltDate->uiYear);
		siMonth		= (SI32)(pcltDate->uiMonth);
		siDay		= (SI32)(pcltDate->uiDay);
		siHour		= (SI32)(pcltDate->uiHour);
		siMinute	= (SI32)(pcltDate->uiMinute);
	}
};

class cltDBFee
{
public:
	SI32	siFeeRate;
	SI64	siMaxFee;
	SI64	siMinFee;

	cltDBFee()
	{
		ZeroMemory(this, sizeof(cltDBFee));
	}

	cltDBFee(cltFee *pclFee)
	{
		siFeeRate	= pclFee->GetFeeRate();
		siMaxFee	= pclFee->GetMaxFee();
		siMinFee	= pclFee->GetMinFee();
	}
};

class DBGameProtocol : public ITZNetworkThread
{
public:
	DBGameProtocol();
	~DBGameProtocol();

public:
	void			Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW );
	void			InitBillingProcess( );

	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );


	void			MessageProc();


	void			SendPacket( BYTE *pPacket );
private:
	ITZSessionFactory< cGame >	m_SF;

private:
	ITZSession		*m_pSession;
	sPacketHeader	*m_pPacket;


//#ifdef USE_DBGW
//	DBGW			m_odbc;
//#else
	RichODBC		m_odbc;
//#endif

	char			m_szSPName[ 512 ];

public:
	// Ping
	void	Ping();

	// System
	void	StartGame();
	void	MasterGiveItem();
	void	GiveItem();
	void	DBSystemInfo();
	void	VoteInit();
	void	DBInitComplete();
	void	DBStartInfo();
	void	GetDailyNewChar();
	void	SetInstanceProfitVillage();
	void	ChangeInstanceProfit();
	void	VoteKingAggregate();
	void	ServerStatisticsSet();
	void	ServerStatisticsGet();
	void	GetWarRank();
	void	SetWarRank();
	void	SetLiquidation();

	void	MofuMofuEvent_Reserve();
	void	MofuMofuEvent_ReserveList();

	void	VillageBankruptcy();

	void	AutoMacro();
	
	void	ServerEvnetRank();					
	void	ServerEvnetServerDailyScoreAdd();
	void	ServerEvnetServerScoreAdd();
	void	ServerEvnetClear();
	void	ServerEventServerBufKind();
	void	ServerEventQuestInfoSet();
	void	ServerEventQuestInfoGet();
	void	ServerEventPersonScoreSet();
	void	ServerEventPersonScoreGet();

	void	SetFourLeaf();

	void	SetGoonzuRaccoonEvent();
	void	GetGoonzuRaccoonEvent();

	// Person
	void	PersonTotalInfoCreate();

	void	PersonTotalInfoGet();
	void	PersonTotalInfoGetByIDNum();
	void	PersonTotalInfoGetCommon(SI32 siPersonID, UI16 usCharID,bool bDormancyState = FALSE, char szIPAddress[20] = NULL, bool bTiredSystem = false, DWORD dwPlayingTime = 0 );

	void	PersonListInfoGet();

	void	PersonTotalInfoDelete();
	void	PersonTotalInfoPreDelete();

	void	PersonTotalInfoSet();
	void	ChangeMoney();
	void	UserID();
	void	UserCharInfo();
	void	UserCharItemInfo();
	void	CharInfoForHanXNoti();
	void	BlockUser();
	void	MoveMoney();
	void	SetSchool();
	void	SetFather();
	void	SetTeacherPoint();
	void	GetItemFromTeacherPoint();
	void	RenouncePupil();
	void	IncreaseChildNumber();
	void	PersonSimpleInfo();
	void	GetChildList();
	void	OverTakeTeacherLevel();
	void	RecommendPerson();
	void	VoteKingSelection();
	void	LogoutRecord();
	void	AccountInfo();
	void	AccountInfoForHanXNoti();		// For HanXNoti
	void	ChangeName();
	void	ChangeCharKind();
	void	GameEvent();
	void	CountAttackInfo();
	void	GreetingSet();
	void	GreetingGet();
	void	GetPersonNameNew();
	void	GetPersonName();
	void	ChangeRecommendPoint();
	void	GetRecommendPoint();
	void	BuyRecommendProduct();
	void	RecommendPointList();
	void	GIveFP();
	void	SetCountry();
	void	SetShowCountry();
	void	AddBuf();
	void	DeleteBuf();
	void	SetShowPuchikomi();
	void	PVPResult();
	void	Attend();
	void	SetAttend();
	void	TransForm_Set();
	void	TransForm_AddChar();
	void	TransForm_AddTime();
	void	TimeMeterRate_AddTime();
	void	TimeMeterRate_useTime();
	void	CharOption();
	void	CharGetOption();
	void	SetHomePage();
	void	GetHomePage();
	void	HelperDailyCountSet();
	void	HelperDailyCountGet();
	void	HelperDailyCountDel();
	void	SetMuteTime();
	void	GetMuteTime();
	void	SetValentineWarInfo();
	void	GetValentineWarInfo();
	void	ValentineWarGetPrize();
	void	GetCellularPhoneAuth();	//[진성] 핸드폰 인증 여부 요청.
	void	SetMarry();
	void	GetMarryInfo();
	void	MarryItemGetSet();
	void	MarryLoveSet();
	void	PersonSlotNumChange();
	void	SkillBookMagicInfoGet();
	void	SkillBookMagicAdd();
	void	SkillBookMagicDel();	
	void	SkillBookSubMagicAdd();
	void	SkillBookSubMagicDel();
	
	// PersonItem
	void	InputItemToPerson();
	void	MoveItem();
	void	OutputItemFromPerson();
	void	MoveItemPerson();
	void	UseItem();
	void	SellNPC();
	void	BuyNPC();
	void	MoveItemInvToStg();
	void	MoveItemStgToInv();
	void	WasteDurability();
	void	InvRepair();
	void	DropItem();
	void	MakeItemInv();
	void	EnchantItem();
	void	EnchantItemInv();
	void	PrivateTrade();
	void	WarpVillage();
	void	UpgradeItem();
	void	PersonalShopBuyItem();
	void	PersonalShopSellItem();
	void	StartPersonalShop();
	void	FishingGetMarineProducts();
	void	FishingDecreaseToolDurability();
	void	PerformanceUpgradeItem();
	void	AgricultureGetProducts();
	void	PremiumSet();
	void	GetPersonItem();
	void	WennyOldManInvList();
	void	WennyOldManSaveInv();
	void	WennyOldManMoveStgToInv();
	void	ExchangeItem();
	void	OpenTreasureBox();
	void	EnchantUseDayPlus();
	void	SystemRewardList();
	void	RecieveSystemReward();
	void	GiveSystemReward();
	void	SpaceBoxList();
	void	SpaceBoxMoveItem();
	void	SpaceBoxUseItem();
	void	ChangeEnchant();
	void	ArtifactInventoryList();
	void	ArtifactSaveInventory();
	void	ArtifactMoveItemStgToInv();
	void	ArtifactMixture();
	void	ArtifactRemove();
	void	ArtifactSlot_Create();
	void	CrystalDurability();
	void	SummonPushItem();
	void	SummonPopItem();
	void	SummonUseItem();
	void	SummonItem_StatusChange();
	void	RepairItem();
	
	void	AmberboxSelectItemList();		//[진성] 호박상자. 선택 아이템 리스트. => 2008-9-16
	void	AmberboxSelectItemAdd();		//[진성] 호박상자. 선택 아이템 추가.   => 2008-9-16
	void	AmberboxSelectItemGet();		//[진성] 호박상자. 선택한 아이템 획득. => 2008-9-16

	// Cityhall
	void	GetCityhallStrInfo();
	void	SendCityhallStrInfo( SI32 siVillageUnique, UI16 usCharID );
	void	PayDividend();

	void	AppplyResident();
	void	GetResidentApplyerList();
	void	GetResidentList();
	void	BeResident();
	void	CancelResident();
	void	GetResidentInfo();
	void	SetResidentScore();
	void	ResidentShare();
	void	ResidentManyOut();
	void	SetInOutMoney();
	void	ChangeCityHallProfitMoney();
	void	ProfitToCapitalFund();
	void	CityHallStateChange();
	void	NotifyCityhallBankruptcy();
	void	AutoPayDividend();
	void	GiveVillageItem();
	void	DonateVillageMoney();
	void	DonateMemberList();

	// GeneralMeeting
	void	GeneralMeetingSuggestChief();
	void	GeneralMeetingInfo();
	void	GeneralMeetingVote();
	void	GeneralMeetingAcceptCandidate();
	void	GeneralMeetingCloseChief();
	void	GeneralMeetingSetVoterState();
	void	GeneralMeetingSuggestOnOff();
	
	// Bank
	void	GetBankStrInfo();				//	rsp_BankInfoGet
	void	SendBankStrInfo(SI32 siVillageUnique, UI16 usCharID);	//	rsp_BankInfoGet
	void	CreateBankAccount();			//	rsp_BankAccountCreate
	void	CloseBankAccount();				//	rsp_BankAccountClose
	void	InputMoneyToBank();				//	rsp_BankMoneyInput
	//	rsp_BankInfoGet

	void	OutputMoneyFromBank();			//	rsp_BankMoneyOutput
	//	rsp_BankInfoGet

	void	SetBankInfo();					//	rsp_BankInfoSet
	void	SetBankFee();					//	rsp_BankFeeSet

	// Stock
	void	GetStockStrInfo();					//
	void	SendStockStrInfo(SI32 siVillageUnique, UI16 usCharID);
	void	GetManyStockOrder();			//	rsp_StockOrderManyGet
	void	TradeStock();					//	rsp_StockTrade
	void	OrderStock();					//	rsp_StockOrderSet
	void	PersonStockAccountGet();		//	rsp_StockPersonAccountInfoGet
	void	CancelStockOrder();				//	rsp_StockOrderCancel
	void	ManyStockPrice();				//	rsp_StockPriceManyGet
	void	InvestList();					//	rsp_StockInvestListGet
	void	SetStockFee();					//	rsp_StockFeeSet
	void	SetStockBidVillage();			//	rsp_StockBidVillage
	void	GetStockBidVillageList();		//	rsp_StockBidVillageListGet
	void	GetStockBidderList();			//	
	void	SendStockBidderList( SI32 siVillageUnique );			//	rsp_StockBidderListGet
	void	CompleteStockBidVillage();		//	rsp_StockBidVillageComplete
	void	SetBeginningPrice();			//	rsp_StockBidBiginningPrice
	void	StockDistributionOrder();		//	rsp_StockDistributionOrder
	void	MaxStockDistributionNum();		//	rsp_StockMaxDistributionSet

	void	SetVillageWarLoss();			//	rsp_StockVillageWarLoss
	void	ReturnStockBidVillage();

	void	BidStock();
	void	BuyBidStock();
	void	MyStockInfo();

	void	GetStockOfferingInfo();
	void	SetStockOffering();
	void	EndStockOffering();
	void	StockOfferingOrder();
	void	GetStockOfferingOrderList();
	void	ExecuteStockOfferingOrder();
	void	ResetStockOfferingOrderList();
	void	ExchangeFromStock();
	void	GiveStock();

	// Market
	void	GetMarketStrInfo();				//
	void	SendMarketStrInfo(SI32 siVillageUnique, UI16 usCharID);

	void	OrderMarket();					//	rsp_MarketOrderSet
											//	rsp_MarketPersonAccountInfoGet

	void	GetManyMarketOrder();			//	rsp_MarketOrderManyGet
	void	TradeMarket();					//	rsp_MarketTrade
	void	MarketAccount();				//	rsp_MarketPersonAccountInfoGet
	void	GetItemFromMarket();			//	rsp_PersonItemGetFromMarket
	void	ManyItemPrice();				//	rsp_ItemPriceManyGet
	void	SetItemPrice();					//	rsp_ItemPriceSet
	void	ReduceOrderPrice();				//	rsp_MarketOrderPriceReduce
	void	SetMarketFee();					//	rsp_MarketFeeSet
	void	SetForeignMerchantNowMoney();
	void	SetForeignMerchantBuyLimit();
	void	GetForeignMerchantBuyLimit();
	void	GetForeignMerchantNowMoney();
	void	SetSystemBuyItemNowMoney();
	void	GetSystemBuyItemNowMoney();

	void	UnionMarketAccount();			//	rsp_UnionMarketPersonAccountInfoGet
	void	NewOrderMarket();				//	rsp_UnionMarketOrderSet
	void	TradeUnionMarket();				//	rsp_UnionMarketTrade
	void	UnionMarketItemList();			//	rsp_UnionMarketItemListGet
	void	GetItemFromUnionMarket();		//	rsp_UnionMarketItemGet
	void	UnionMarketResult();			//  rsp_UnionMarketLogGet
	void	UnionMarketInputBankMoney();	//  rsp_UnionMarketInputBankMoney
	void	SetUinonMarketFee();			//	rsp_UnionMarketFeeSet
	void    ManyNewItemPrice();				//  rsp_UnionMarketItemPriceGet
	void	SetNewItemPrice();				//	rsp_UnionMarketItemPriceSet
	void	NewMarketReducePriceNotice();	//	rsp_UnionMarketReducePriceNotice


	// Rank
	void	Candidate();					//	rsp_CandidateSet
	void	BeRank();						//	rsp_RankSet
	void	CancelCandidate();				//	rsp_CandidateCancel
	void	ManyCandidateInfo();			//	rsp_CandidateManyInfoGet
	void	DelCandidate();					//	rsp_CandidateDelete
	void	Resign();						//	rsp_RankResign
	void	ManyRank();						//	rsp_RankManyGet
	void	SendRankInfo( cltSimplePerson *pclPerson );

	// GameTime
	void	SetTime();						//	rsp_GameTimeSet
	void	GetTime();						//	rsp_GameTimeGet
	void	SetUserNum();					//	rsp_UserNumSet
	void	Block();						//	rsp_Block

	// House
	void	GetHouseStrInfo();					//	rsp_HouseInfo
	void	ChangeHouseContract();			//	rsp_HouseContractChange
	void	ManyHouseUnit();				//	rsp_HouseUnitManyGet
	void	GetHouseApplyerList();			//	rsp_HouseApplyerListGet
	void	HouseRentApply();				//	rsp_HouseRentApply
	void	HouseRent();					//	rsp_HouseRent
	void	HouseRentApplyCancel();			//	rsp_HouseRentApplyCalcel
	void	DelHouseApplyer();				//	rsp_HouseApplyerDelByOwner
	void	PayRentFee();					//	rsp_HouseRentFeePay
	void	RecallHouse();					//	rsp_HouseRecall
	void	ExtendContract();				//	rsp_HouseRentExtend

	void	GetHouseStgPos();				//	rsp_HouseStgPosGet
	void	MakeItem();						//	rsp_HouseMakeItem

	void	HouseUnitHorseIn();				//	rsp_HouseUnitHorseIn
	void	HouseUnitHorseOut();			//	rsp_HouseUnitHorseOut
	void	HouseUnitStableCreate();		//	rsp_HouseUnitStableCreate
	void	HouseUnitHorseInfoSet();		//	rsp_HosueUnitHorseInfoSet
	void	HouseUnitHorseEatFood();

	void	HouseUnitUsedGoodsBuildingCreate();
	void	HouseUnitMakeItemFromUsedGoods();
	void	ChangHouseUnitName();

	void	HouseSaveUserItemListGet();
	void	HouseSaveUserItemGet();
	void	HouseSaveUserHorseGet();

	void	CharNameCheck();					
	void	CharFriendInfo();
	void	CharGuildInfo();
	void	CharVillageResidentInfo();

	// Structure
	void	GetStructureInfo();				//	rsp_StructureInfoGet
	void	SetStrInfo();
	void	GetStrInfo();

	void	PayFee();						//	rsp_StructureFeePay

	void	ChangeStrMoney();				//	rsp_StructureMoneyChange

	void	SetStructureMessage();			//	rsp_StructureMessageSet

	void	BuildStructure();				//	rsp_StructureBuild
	void	DestroyStructure();				//	rsp_StructureDestroy
	void	ChiefInfo();					//	rsp_StructureChiefInfo
	void	SetStrRepairItem();				//	rsp_StructureSetStrRepairItem
	void	SetStrRepairAmount();
	void	SetStrDur();					//	rsp_StructureSetStrDur
	void	ChangeStrDur();					//	rsp_StructureChangeStrDur
	void	RepairStructure();				//	rsp_StructureRepair

	void	ChangeStrBaseMoney();

	// Tax
	void	SetTax();						//	rsp_TaxSet
	void	PayTax();						//	rsp_TaxPay

	// Skill
	void	IncreaseSkill();				//	rsp_SkillIncrease
	void	DecreaseSkill();				//	rsp_SkillDecrease
	void	SetSkillExp();
	void	IncreaseMasterSkill();			//	rsp_SkillIncrease
	void	SetWeaponSkill();

	// Letter
	void	PushLetter();					//	rsp_LetterPush
	void	PopLetter();					//	rsp_LetterPop
	void	PushAccountLetter();
	void	PopAccountLetter();

	// ImportantParameta
	void	SetLevelInfo();					//	rsp_IPLevelInfoSet
	void	SetBonusInfo();					//	rsp_IPBonusInfoSet
	void	SetPosLifeMana();				//	rsp_IPPosLifeManaSet
	void	ChangeExp();					//	rsp_IPExpChange
	void	InitStatus();					//	rsp_IPStatusInit
	void	ChangeFameExp();

	// Health
	void	SetHealth();					//	rsp_HealthSet

	// Quest
	void	SetQuestInfo();

	void	DailyQuestAdd();				//	rsp_DailyQuestAdd
	void	DailyQuestDel();				//	rsp_DailyQuestDel
	void	DailyQuestChange();				//	rsp_DailyQuestChange

	void	DailyQuest2Add();
	void	DailyQuest2Del();
	void	DailyQuest2Change();

	void	DailyQuest3Add();
	void	DailyQuest3Del();
	void	DailyQuest3Change();

	void    Set_bbbRewardAccumPrice();	// 복불복 누적금액
	void	Get_bbbRewardAccumPrice();	// 
	void	SetSpecialQuestInfo();

	void	SetNamingQuestInfo();
	void	SetPersonNamingInfo();
	void	SetPersonCurrentNaming();

	void	SetHappyBeanQuestInfo();
	void	GetHappyBeanClearInfo();

	void	SetMissionQuestInfo();

	void	GoonzuDayMissionSet();		// [지연] 군주데이 미션
	void	GoonzuDayMissionGet();

	// NPC
	void	SetForeignMerchantNPCInfo();	//	rsp_ForeignMerchatNPCInfoSet
	void	GetForeignMerchantNPCInfo();	//	rsp_ForeignMerchatNPCInfoGet
	void	HireNPC();						//	rsp_NpcHire
	void	TradeMerchantSet();
	void	TradeMerchantBuyItemFromPerson();
	void	TradeMerchantBuyHorseFromPerson();
	void	TradeMerchantBuyItem();
	void	TradeMerchantBuyCashItem();

	// Horse
	void	SetRideHorse();					//	rsp_HorseRideSet
	void	SetHorse();						//	rsp_HorseSet
	void	HorseNameChange();				//	rsp_HorseNameChange
	void	SetAutoFood();

	// Village
	void	GetVillageInfo();				//	rsp_VillageInfoGet
	void	DeclareWar();					//	rsp_VillageDeclareWar
	void	SetParentVillage();				//	rsp_VillageParentSet
	void	CancelDeclareWar();				//	rsp_VillageDeclareWarCancel
	void	SetVillageSymbol();				//	rsp_VilllageSymbolSet
	void	SetSymbolStatus();				//	rsp_VillageSymbolStatusSet
	void	SetVillageCastle();				//	rsp_VilllageCastleSet
	void	SetCastleStatus();				//	rsp_VillageCastleStatusSet
	void	SetVillageLevel();				//	rsp_VillageLevelSet
	void	SetVillageMark();
	void	MapTypeInfo();
	void	SetVillageMap();
	void	Event_InsertMaterial();			// 
	void	Event_MaterialList();			// 
	void	Event_MaterialComplete();		//
	void	Add_Village_Score();			// [진성] 마을점수 추가. 


	//	void	NewYearEventSuccess();
	void	VillageEventSuccess();
	void	VillageWarKillNumber();
	void	VillageJoinPlayTime();
	void	VillageJoinLevelUp();
	void	VillageJoinWarKillNumber();
	void	VillageJoinMakeRareItem();
	void	VillageJoinMakeItem();

	void	VillageResidentsVisitorsInfo();

	void	VillageWarResult();

	// Hunt
	void	GetHuntStrInfo();				//	rsp_HuntInfoGet
	void	HuntStorageMonsterIn();			//	rsp_HuntStorageMonsterIn
	void	HuntStorageMonsterOut();		//	rsp_HuntStorageMonsterOut
	void	HuntMonsterFieldListAdd();		//	rsp_HuntMonsterFieldListAdd
	void	HuntMonsterFieldListDel();		//	rsp_HuntMonsterFieldListDel
	void	HuntMoneyChange();				//	rsp_HuntMoneyChange
	void	HuntBaseInfoApply();			//	rsp_HuntBaseInfoApply
	void	HuntStorageDecreaseDollItemNum();	//	rsp_HuntStgDecreaseItemNum
	void	HuntGetEnterFee();				//	rsp_HuntEnterFeeGet
	void	HuntPaySuccessMoney();			//	rsp_HuntSuccessMoneyPay
	void	HuntLeftMonsterStgIn();			//	rsp_HuntLeftMonsterStgIn
	void	InstanceMapPoint_Insert();
	void	InstantMapYearRanking();
	void	InstantMapHOFList();

	// PostOffice
	void	GetPostOfficeStrInfo();			//	rsp_PostInfoGet
	void	PostOffice_SendItem();			//	rsp_PostSendItem
	void	PostOffice_SendMsg();			//	rsp_PostSendMsg
	void	PostOffice_SendMoney();			//	rsp_PostSendMoney
	void	PostOffice_RecvItemList();		//	rsp_PostItemListGet
	void	PostOffice_RecvMsgList();		//	rsp_PostMsgListGet
	void	PostOffice_RecvMoneyList();		//	rsp_PostMoneyListGet
	void	PostOffice_GetItem();			//	rsp_PostGetItem
	void	PostOffice_DelMsg();			//	rsp_PostDelMsg
	void	PostOffice_SetFee();			//	rsp_PostFeeSet
	void	PostOffice_GetMoney();			//	rsp_PostGetMoney
	void	PostOffice_MasterSendItem();	//	rsp_PostSendItemByMaster
	void	PostOffice_GetRecord();

	// HorseMarket
	void	GetHorseMarketStrInfo();
	void	HorseMarketGetManyOrder();		//	rsp_HorseMarketManyOrderGet
	void	HorseMarketSetFee();			//	rsp_HorseMarketFeeSet
	void	HorseMarketOrderSell();			//	rsp_HorseMarketOrderSell
	void	HorseMarketMyAccount();			//	rsp_HorseMarketMyAccount
	void	HorseMarketOutHorse();			//	rsp_HorseMarketHorseOut
	void	HorseMarketBuyHorse();			//	rsp_HorseMarketHorseBuy
	void	HorseMarketReduceOrderPrice();

	void	SetHorsePrice();
	void	GetHorsePrice();

	// Friend
	void	FriendAddList();				//	rsp_FriendAddList
	void	FriendDelList();				//	rsp_FriendDelList
	void	FriendMemo();
	void	FriendMemoSet();
	void	FriendChangeGroup();
	void	FriendChangeGroupName();

	void	FriendAllList();				//

	// Minister
	void	SetYezoInfo();					//	rsp_MinisterYezoinfoSet
	void	GetYezoInfo();					//	rsp_MinisterYezoinfoGet
	void	SetByungzoInfo();				//	rsp_MinisterByungzoInfoSet
	void	SetByungzoInfoVillageWarTime();
	void	GetByungzoInfo();				//	rsp_MinisterByungzoInfoGet
	void	SetHozoInfo();					//	rsp_MinisterHozoInfoSet
	void	GetHozoInfo();					//	rsp_MinisterHozoInfoGet
	void	SetGoonzuInfo();				//	rsp_MinisterGoonzuInfoSet
	void	GetGoonzuInfo();				//	rsp_MinisterGoonzuInfoGet
	void	SetIzoInfo();					//	rsp_MinisterIzoInfoSet
	void	GetIzoInfo();					//	rsp_MinisterIzoInfoGet
	void	SetGongzoInfo();				//	rsp_MinisterGongzoInfoSet
	void	GetGongzoInfo();				//	rsp_MinisterGongzoInfoGet
	void	SetHyungzoInfo();
	void	GetHyungzoInfo();
	void	SetGoonzuFinances();
	void	SetAngelApplyer();	// 수호천사 신청
	void	GetAngelApplyerList();
	void	SetAngel();	// 수호천사 임명
	void	GetAngelList();
	void	GetChangedName();
	void	SetMinister();
	void	SetPolice();
	void	GetPoliceList();
	void	SetCriminal();
	void	GetCriminalList();
	void	GoonzuSetBonusTime();

	// Summon
	void	SetSummon();
	void	SummonRevive();
	void	SummonEvolve();
	void	SummonScroll();
	void	SummonStatusInit();
	void	SummonSetAutoHeal();
	void	SummonTrade();
	void	SummonSlotOut();
	void	SummonSlotIn();
	
	// ItemMall
	void	CheckAccountID();
	void	ItemMallItemList();
	void	ItemMallExistence();
	void	NewsChatAdmission();
	void	NoLimitTicketWarp();

	void	Warp_TargetWorld();

	void	ItemMallMyCashInfo();
	void	ItemMallGiftItem();
	void	ItemMallBuyItem();
	void	ItemMallGetItem();

	void	ItemMallMyCashInfo_Global();
	void	ItemMallGiftItem_Global();
	void	ItemMallBuyItem_Global();
	void	ItemMallGetItem_Global();

	void	ItemMallMyCashInfo_China();
	void	ItemMallGiftItem_China();
	void	ItemMallBuyItem_China();
	void	ItemMallGetItem_China();

	void	ItemMallMyCashInfo_Korea();
	void	ItemMallBuyItem_Korea();
	void	ItemMallGiftItem_Korea();
	void	ItemMallGetItem_Korea();

	bool	BuyItemProcess_Korea(char *accountid, signed int *price, signed int *realmoney, signed int *bonusmoney, signed int *eventmoney, char *txcode, bool bGift );

	void	ItemMallMyCashInfo_nDoors_Korea();
	void	ItemMallBuyItem_nDoors_Korea();
	void	ItemMallGiftItem_nDoors_Korea();

	void	ItemMallMyCashInfo_USA();
	void	ItemMallBuyItem_USA();
	void	ItemMallGiftItem_USA();
	void	ItemMallGetItem_USA();

	void	ItemMallBuyItem_XNoti();
	void	ItemMallGiftItem_XNoti();
	void	ItemMall_CancelOrder_XNoti();

	void	ItemMallGetItem_Taiwan();



	void	ItemMallMyCashInfo_Germany();							//	[종호] 독일 Billing - 잔액 조회
	void	ItemMallBuyItem_Germany();								//	[종호] 독일 Billing - 구매
	void	ItemMallGiftItem_Germany();								//	[종호] 독일 Billing - 선물 
	void	ItemMallGetItem_Germany();
	BILL_PACK_BALANCE*	GetBalance_Germany(SI32 siAccountNumber, char* accountid);
	BILL_PACK_BUY*		GetBuy_Germany(SI32 siAccountNumber, TCHAR* accountid, SI32 siProductKey, SI32 siProductPrice, TCHAR* szProductName);			
	BILL_PACK_GIFT*		GetGift_Germany(SI32 siAccountNumber,	TCHAR* accountid, TCHAR* Recvaccountid,	SI32 siProductKey, SI32 siProductPrice, TCHAR* szProductName);			

	



	//KHY - 0304 - 중국 NHNChinaBilling -
	void ItemMallMyCashInfo_NHNChina();
	void ItemMallBuyItem_NHNChina();
	SI32 ItemMallProductBuyProcess_NHNChina(IN SI32 siServerID ,				// 서버ID
											IN char * accountid ,		// 계정 명.
											IN char * szName ,			// 캐릭터 명
											IN SI32 siProductKey,		// 아이템 코드
											IN SI32 siBuySeqNum ,		// 주문번호  - 게임사 거래번호
											IN SI32 siProductNum,		// 아이템 개수
											IN SI32 siPrice ,           // 거래 금액
											IN SI32 siEventPrice ,      // 거래 금액
											IN char * szIP,             // Client IP
											OUT char *logseq );
	void	ItemMallGiftItem_NHNChina();
	void	ItemMallGetItem_NHNChina();
	

	SI32	ItemMallProductBuyProcess_Korea(IN char * accountid , IN SI32 siPrice , 
		IN SI32 siProductKey ,IN SI32 siProductNum );

	SI32	ItemMallProductBuyProcess_USA(IN char * accountid , IN SI32 siIDNo, TCHAR* pszIP, IN SI32 siPrice ,
		IN SI32 siProductKey ,IN TCHAR* pszProductName, IN SI32 siProductNum );
	
	SI32	ItemMallProductGiftProcess_USA(IN TCHAR* pszGiftSendID , IN TCHAR* pszGiftRecvID , IN SI32 siIDNo, TCHAR* pszIP, IN SI32 siPrice ,
		IN SI32 siProductKey ,IN TCHAR* pszProductName, IN SI32 siProductNum );
	

	void	ItemMallItemInfo();
	void	ItemMallSelPGList();
	void	ItemMallSellLimitCheck(SI32 siPersonID, SI32 siOrderPrice, bool *pbResult, char *accountid, SI32 siProductKey, SI32 siProductNum);
	bool	ItemMallIPGProcess(char *accountid, long *price, long *realmoney, long *bonusmoney, 	long *eventmoney, char *txcode, SI32 ProductKey, char *ProductName, char *UserIP, SI32 ProductNum);
	bool	ItemMallIPGCashInfo(char *accountid, long *realmoney, long *bonusmoney, long *eventmoney);

	// RealEstate
	void	GetRealEstateMarketStrInfo();
	void	RealEstateMarketOrderSell();
	void	RealEstateMarketCancelOrder();
	void	RealEstateMarketMyOrder();
	void	RealEstateMarketGetManyOrder();
	void	RealEstateMarketSetFee();
	void	RealEstateMarketReduceOrderPrice();
	void	RealEstateMarketBuy();
	void	RealEstateMarketDelOrder();

	// Guilty
	void	PrisonUser();
	void	GuiltyInfoRecord();

	// SummonMarket
	void	GetSummonMarketStrInfo();
	void	SummonMarketSetFee();
	void	SummonMarketOrderSell();
	void	SummonMarketGetManyOrder();
	void	SummonMarketMyOrder();
	void	SummonMarketCancelOrder();
	void	SummonMarketBuy();
	void	SummonMarketReduceOrderPrice();

	// SummonHeroMarket
	void	GetSummonHeroMarketStrInfo();
	void	SummonHeroMarketSetFee();
	void	SummonHeroMarketOrderSell();
	void	SummonHeroMarketGetManyOrder();
	void	SummonHeroMarketMyOrder();
	void	SummonHeroMarketCancelOrder();
	void	SummonHeroMarketBuy();
	void	SummonHeroMarketReduceOrderPrice();


	// Land
	void	GetLandStrInfo();
	void	LandUnitManyGet();
	void	LandContractChange();
	void	LandRentApply();
	void	LandRentApplyCancel();
	void	LandApplyerListGet();
	void	LandApplyerDel();
	void	LandRent();
	void	LandPayRentFee();
	void	LandRecall();
	void	FarmingPayMoneyForRentLand();

	// Mine
	void	GetMineStrInfo();
	void	MineUnitManyGet();
	void	MineContractChange();
	void	MineRentApply();
	void	MineRentApplyCancel();
	void	MineApplyerListGet();
	void	MineApplyerDel();
	void	MineRent();
	void	MinePayRentFee();
	void	MineRecall();
	void	AgriculturePayMoneyForRent();
	void	GetAgricultureLeaseInfo();
	void	ExpiredAgricultureLease();

	// Guild
	void	GetGuildStrInfo();
	void	CreateGuild();
	void	DestroyGuild();
	void	GuildChangeVillage();
	void	ConfirmJoinGuild();
	void	LeaveGuild();
	void	JoinGuild();
	void	GetGuildInfo();
	void	ConfirmCreateGuild();
	void	GiveGuildMoney	();
	void	GuildSetSecondMaster();
	void	GuildSetStrFeeRate();
	void	ChangeGuildPoint();
	void	GuildGiveFee();
	void	GuildChangeStatus();
	void	GuildLevelUp();
	void	GuildChangeName();
	void	GuildSetMark();
	void	GuildChangeMaster();
	void	GuildWarAddReserve();
	void	GuildWarReserveList();
	void	GuildDungeonApply();
	void	GuildDungeonSaveStatus();
	void	GuildDungeonLoadStatus();
	void	GuildDungeonTransferOwnership();
	void	GuildCheckDestroyMonth();
	void	GuildNotCOnnectChangeMaster();
	void	SaveGuildHuntMapWarList();	// 길드전을 신청 했다
	void	GetGuildHuntMapWarList();	// 길드전 신청리스트를 가지고 온다
	void	GetGuildHuntMapRank();		//BGP순위
	void	EndGuildHuntMapWar();

	//Union Market (통합마켓)
	void	GetNewMarketInfo();	
	void	SendNewMarketInfo(SI32 siVillageUnique, UI16 usCharID);
    
	// Cash
	void	GiveCash();

	// Feast
	void	GetFeastStrInfo();
	void	FeastReserve();
	void	FeastOpen();
	void	FeastClose();
	void	FeastAddPresentList();
	void	FeastDelPresentList();
	void	FeastStgItemIn();
	void	FeastStgItemOut();
	void	FeastGiveItem();
	void	FeastSetCost();
	void	FeastUseEffect();
	void	PartyHallReservation();
	void	PartyHallAddDecoration();
	void	PartyHallAddPresent();
	void	PartyHallGiveItem();
	void	PartyHallReserveList();
	void	PartyHallDecorationList();

	// WorldMoney
	void	WorldMoneyDistribution();
	void	WorldMoneyTradeList();
	void	WorldMoneySellOrder();
	void	WorldMoneyCancelOrder();
	void	WorldMoneyBuyOrder();
	void	WorldMoneySetAveragePrice();
	void	WorldMoneyReducePrice();
	void	WorldMoneySetChar();

	// WorldPassport
	void	WorldPassportSet();
	void	WorldPassportExtend();


	// TradeServer
	void	TradeServerSellInfo();
	void	TradeServerBuyItem();
	void	TradeServerCancelOrder();
	void	TradeServerSetOrder();
	void	TradeServerGetWithdrawAcu();

	// Gacha
	void	GachaBuy();

	// Empire
	void	CreateEmpire();
	void	DestroyEmpire();
	void	JoinEmpire();
	void	SecedeEmpire();
	void	AllEmpireInfo();
	void	SetEmpireMoney();
	void	ChangeEmperor();

	// Log
	void	WriteLog();
	void	WriteLogPCRoom();
	void	WriteLogToDB();
	void	WriteLogSetHappyBeanClearInfo();
	
	//KHY - 0906 - 게임방 이벤트 쿠폰.
	void	WriteLogSetEventCouponInfo();


	// GMTool
	void	BasicInfoByName();
	void	SkillByName();
	void	AnimalByName();
	void	InventoryByName();
	void	RealestateByName();
	void	SummonByName();

	// PVP League
	void	PVP_LeagueInfoGet();
	void	PVP_LeagueUIInfoGet();
	void	PVP_LeagueBattleEnd();
	void	PVP_LeagueBattleChangeGrade();

	//[진성] 엘릭서.
	void	GetElixir();
	void	SetElixir();
	
	//[진성] 프로모터
	void	PromoterInfoGet();

	void	NewSelectBoxItemGet();

	//[진성] 아이템 기부 하기. (기부 후 리스트 정보를 준다.)
	void	ContributionItemListInput();
	void	ContributionItemListDelete();
	void	ContributionMakeNPC();


	// 결혼
	void	WeddingReserveListGet();
	void	WeddingReserve();
	void	WeddingReserveCancel();

	// 가족
	void	Add_Family_Children();	// 자식 추가
	void	Del_Family_Children();	// 자식 삭제
	void	Get_Family_info();	// 가족정보를 가지고 온다
	void	Plus_FamilyEffect_date();	// 가족정보를 가지고 온다
	void	Set_Cookie_Effect();		// 쿠키 사용 이팩트 
	
	// 수호정령
	void	SoulGuardCreate();
	void	SoulGuardGet();
	void	SoulGuardFellowShipSet();
	void	SoulGuardChangeName();
	void	SoulGuardDead();
	// 수호정령 보관소
	void	SoulGuardHouse_Get();
	void	SoulGuardHouse_Change();
	void	SoulGuardHouse_Create();
	
//	[종호] 통화량 조절 - 사용가능 금액	
	void	SetBalanceCanUseCurrency();	
	void	GetBalanceCanUseCurrency();

	// [기형] 휴면 계정 시스템 
	void	DormancyBuyPointItem();	//포인트 구입
	void	DormancySetPointInfo(); //포인트 관련 정보 저장
	void	DormancyChangeState();  //휴면 상태 변경

	// Assist Function
	void	SetItemParam(const class cltItem *pclItem);

	void	GetItemData(class cltItem *pclItem);

	void	SendMarketAccountInfo(SI32 siPersonID, UI16 uiToID);
	void	SendUnionMarketAccountInfo(SI32 siPersonID, UI16 uiToID, SI16 siServerUnique);

	void	SetSimplePerson(const class cltSimplePerson *pclPerson);
	void	GetSimplePerson(class cltSimplePerson *pclPerson);

	void	SetGameDate(const class cltGameDate *pclGameDate);
	void	GetGameDate(class cltDate *pclDate);

	void	SetRentContract(const class cltRentContract *pclContract, cltGameDate *pclGameDate);
	void	GetRentContract(class cltRentContract *pclContract);

	void	GetRentApplyer(class cltRentApplyer *pclApplyer);

	void	GetBankAccount(class cltBankAccount *pclBankAccout);

	void	SetSimpleRank(const class cltSimpleRank *pclRank);
	void	GetSimpleRank(class cltSimpleRank *pclRank);

	void	SetBasicAbility(const class cltBasicAbility *pclBA);
	void	GetBasicAbility(class cltBasicAbility *pclBA);

	void	SetHealthInfo(const class cltHealthInfo *pclHI);
	void	GetHealthInfo(class cltHealthInfo *pclHI);

	void	SetCharPos(const class cltCharPos *pclPos);
	void	GetCharPos(class cltCharPos *pclPos);

	void	SetPos(const class cltPos *pclPos);
	void	GetPos(class cltPos *pclPos);

	void	SetQuestUnitInfo(const class cltPersonQuestUnitInfo *pclQU);
	void	GetQuestUnitInfo(class cltPersonQuestUnitInfo *pclQU);
	void	SetQuestInfoCommon(const class cltQuestInfoCommon *pclQC);
	void	GetQuestInfoCommon(class cltQuestInfoCommon *pclQC);

	void	SetQuestWordAssemblySwitch(const bool *pbWAS);
	void	GetQuestWordAssemblySwitch(bool *pbWAS);

	void	GetTaxList( class cltTaxInfo *pclTaxInfo, SI32 siVillageUnique, SI32 siStrType );

	void	SetHorseInfo( const class cltHorse *pclHorse, const SI32 *psiBirthDay, const SI32 *psiAddStatusDate
		, const SI32 *psiPremiumPartsDate_Slot1, const SI32 *psiPremiumPartsDate_Slot2, const SI32 *psiPremiumPartsDate_Slot3 );

	void	GetHorseInfo( class cltHorse *pclHorse );

	void	SetFeeInfo( const class cltDBFee *pclFee );
	void	GetFeeInfo( class cltFee *pclFee );
	void	GetFeeInfoNew( class cltFee *pclFee );

	void	SetChildSkillList( const class sDBRequest_DecreaseSkill *pclSkill );

	//	void	SetResidentApplyerInfo( const class cltResidentApplyer *clApplyer );
	void	GetResidentApplyerInfo( class cltResidentApplyer *clApplyer );

	void	GetResidentInfo( class cltResident *clApplyer );

	// Set Defined Number
	void	SetDefinedNumber();

	void	GetStockBidder( class cltStockBidder *pclStockBidder );

	void	SetFatherInfo( const class cltPFatherInfo *pclFater );
	void	GetFatherInfo( class cltPFatherInfo *pclFater );

	void	ResidentShareSetVillage( SI32 siVillageUnique, GMONEY Money, SI32 *psiResult, GMONEY *pitMoney );
	void	ResidentShareSetPerson( SI32 siVillageUnique, SI32 PersonID, GMONEY Money, GMONEY *psiMoney );

	void	GetMinisterBase( class CMinisterBase *pclMB);

	void	SetHorseOrder( const class cltHorseOrder *pclHorseOrder, const SI32 *psiOrderDay, const SI32 *psiBirthDay, const SI32 *psiAddStatusDate
		, const SI32 *psiPremiumPartsDate_Slot1, const SI32 *psiPremiumPartsDate_Slot2, const SI32 *psiPremiumPartsDate_Slot3);
	void	GetHorseOrder( class cltHorseOrder *pclHorseOrder );


	//	void	SetSummonInfo( const class cltSummonInfo *pclSummon, const int *psiDateVary );
	//	void	GetSummonInfo( class cltSummonInfo *pclSummon);
	void	SetSummon( const class cltSummon *pclSummon );
	void	GetSummon( class cltSummon *pclSummon );
	void	SetSummonOrder( const class cltSummonOrder *pclOrder, const int *psiOrderDateVary );
	void	GetSummonOrder( class cltSummonOrder *pclOrder );
	void	GetSimpleSummon( class cltSimpleSummon *pclSimpleSummon );

	void	SetPIPInfoCommon( const class cltPIPInfoCommon *pclInfoCommon );
	void	GetPIPInfoCommon( class cltPIPInfoCommon *pclInfoCommon );
	void	SetPIPInfo( const class cltPIPInfo *pclInfo );
	void	GetPIPInfo( class cltPIPInfo *pclInfo );

	void	SetLevelInfo( const class cltLevelInfo *pclInfo );
	void	GetLevelInfo( class cltLevelInfo *pclInfo );

	void	SetRealEstateOrderInfo( const class cltRealEstateOrder *pclInfo, const SI32 *psiDateVary );
	void	GetRealEstateOrderInfo( cltRealEstateOrder *pclInfo );

	void	SetGuiltyInfo( const class cltGuiltyInfo *pclInfo );
	void	GetGuiltyInfo( class cltGuiltyInfo *pclInfo );

	void	GetAngelInfo( struct HozoAngelList *pclInfo );

	void	GetGeneralMeetingVoter( class cltGeneralMeetingVoter *pclVoter );

	void	SetSimpleGuildRank(const class cltSimpleGuildRank *pclSimpleRank);
	void	GetSimpleGuildRank(class cltSimpleGuildRank *pclSimpleRank);

	void	GetPoliceInfo(struct HyungzoPoliceList *pclInfo);
	void	GetCriminalInfo(struct HyungzoCriminalList *pclInfo);

	void	GetItemMallDateItemInfo(class cltItemMallDateItemInfo *pclInfo);
	void	GetItemMallListData(class cltItemMallListData *pclData);

	void	GetStockBidVillageInfo(class cltStockBidVillageInfo *pclInfo);
	void	SetStockBidVillageInfo(const class cltStockBidVillageInfo *pclInfo, SI32 siDate);

	void	GetBufInfo(class cltBufInfo *pclInfo, SI32 siIndex);
	void	SetBufInfo(const class cltBufInfo *pclInfo, SI32 siIndex);

	void	GetGuildInfo(class cltGuildInfo *pclInfo);
	void	GetGuildPoint(class cltGuildPoint *pclPoint);
	void	GetGuildMember(class cltGuildMember *pclMember);
	void	GetGuildStandingMember(class cltGuildStandingMemeber *pclMember);
	void	GetGuildStatus(class cltGuildStatus *pclInfo);
	void	SetGuildStatus(const class cltGuildStatus *pclInfo);
	void	GetGuildWarReserveUnit(class cltGuildWarReserveUnit *pclUnit);
	void	GetGuildDungeonChangeGPMoney(class cltGuildDungeon_ChangedGPMoney *pclInfo);
	void	GetGuildDungeonNewOwnership(class cltGuildDungeon_NewOwnership *pclInfo);

	void	Convert_SYSTEMTIME_from_TIMESTAMP(_SYSTEMTIME *stST, TIMESTAMP_STRUCT *stTS);
	void	Convert_TIMESTAMP_from_SYSTEMTIME(TIMESTAMP_STRUCT *stTS, _SYSTEMTIME *stST);
	void	GetSystemRewardInfo(class cltSystemRewardInfo *pclInfo);

	void	SetCharOptionInfo(const cltPCharOptionInfo *pclInfo);
	void	GetCharOptionInfo(cltPCharOptionInfo *pclInfo);

	void	GetServerEventInfo(sDBResponse_Server_Event_Rank_Info *pclSendMsg);

	void	GetAmberboxSelectItemInfo(AmberboxSelectItemInfo *pclInfo);
	void	SetAmberboxSelectItemInfo(const AmberboxSelectItemInfo *pclInfo);

	void	GetPVP_League_OtherUserInfo( cltPVP_League_OtherUserInfo* pclInfo );

	void	GetGuildMapWar_RegList( cltGuildHuntMapWarListData* pclInfo );
	void	GetGuildMapWar_GetRank( cltGuildHuntMapWarListRankData* pclInfo );

	void	SetGoonzuRaccoonEventSetinfo( cGoonzuRaccoonEventData	*m_GoonzuRaccoonEventData)	;
	void	SetGoonzuRaccoonEventSetinfo_Map(RocData *m_RocData)	;

	// 군주신문
	//void	News();

	class stDBMsgPtr
	{
	public:
		void (DBGameProtocol::*Func)(void);
	};

	stDBMsgPtr *m_pDBMsgPtr;
	void	NullFuncPtr();

	givecash	givecashbuffer[ 1024 ];
	SI32		givecashcount;

	SI32		m_siBillingMode;

	NHanGameBillingProcess	clHangameBillingProcess;
	NIJJIBillingProcess		clIJJIBillingProcess;
	NHannEverBillingProcess	clHannEverBillingProcess;
	CBillManager_Germany	cBillBalanceManager;			//	[종호] 독일 빌링Manager

};

#endif
