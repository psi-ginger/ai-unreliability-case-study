
//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "Directives.h"
#include "../Common/CommonHeader.h"
#include "..\Common\NPCManager\NPC.h"
#include "Client.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

// APEX-
#include "../Apex/ApexGoonZu.h"
#include "../Apex/gamecode.h"
// -APEX

//-----------------------------------
// Common
//-----------------------------------
#include "../Common/Map/Map.h"
#include "Char\CharManager\CharManager.h"
#include "Map\FieldObject\FieldObject.h"

//----------------------------------
// Client
//----------------------------------
#include "Help\Help.h"


#include "../resource.h"
#include "../Server/Candidate/Candidate.h"
#include "Music/Music.h"
#include "../Server/Server.h"
//#include "ColorTable/ColorTable.h"
#include "Registry/RegistryMgr.h"
#include "SmallMap/SmallMap.h"
#include "Inventory/ItemInventory.h"
#include "Interface/NPC1/NPCTradeDlg.h"
#include "StatusBar/StatusBar.h"
#include "Skill/Skill-Manager.h"
#include "MiniHomeUpdates/MiniHomeUpdates.h"
#include "MiniHomeUpdates/PICAManagerCheck.h"
#include "MiniHomeUpdates/PopupCheck.h"

#include "../CommonLogic/MsgType-Person.h"
#include "../CommonLogic/BlackArmy/BlackArmy.h"
#include "../common/NPCManager/NPC.h"

#include "Interface/HouseUnit/HouseUnitBaseInfo.h"
#include "Interface/TradeMerchantDlg/TradeMerchantDlg.h"
#include "Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "NInterface/NMonsterInfoDlg/NMonsterInfoDlg.h"

//#include "Interface/ForeignMerchant/ForeignMerchantDlg.h"
//#include "Interface/ForeignMerchant/ForeignMerchantOpenDlg.h"

//#include "Interface/PersonalShop/PersonalShopDlg.h"
#include "NInterface/NPersonalShopDlg/NPersonalShopDlg.h"
#include "Interface/PersonalShopSearchDlg/PersonalShopSearchDlg.h"
#include "NInterface/NPersonalShopSearchDlg/NPersonalShopSearchDlg.h"

#include "Ninterface/NManufactureDlg/NManufactureDlg.h"

#include "Interface/ServerOnStepNotifyDlg/ServerOnStepNotifyDlg.h"
#include "ninterface/NDestroyItemDlg/NDestroyItemDlg.h"

#include "ninterface/nmarketconditionsdlg/NMarketConditionsDlg.h"
#include "NStructureDlg/PostOffice/NPostOfficeGroupListDlg.h"


//#include "Interface/PartyDlg/PartyDlg.h"
//#include "Interface/PartyInviteDlg/PartyInviteDlg.h"


//#include "Interface/FriendDlg/FriendDlg.h"
//#include "Interface/FriendDlg/FriendAddAcceptDlg.h"
#include "NInterface/NFriendDlg/NFriendDlg.h"
#include "NInterface/NFriendDlg/NNewFriendDlg.h"
#include "NInterface/NFriendDlg/NFriendAddAcceptDlg.h"
#include "ninterface/nprivatechatdlg/NAutoAnswerDlg.h"
#include "NInterface/NVoteDlg/NVoteDlg.h"
#include "NInterface/NVoteDlg/NVoteStatusDlg.h"
#include "NInterface/NFriendDlg/NFriendChangeGroupNameDlg.h"

#include "NInterface/NFriendDlg/NNewFriendDlg.h"

#include "NInterface/NAutoHunting/NAutoHunting.h"
#include "ImageInterface/FriendLoginDraw/FriendLoginDraw.h"
#include "ImageInterface/ResidentLoginDraw/ResidentLoginDraw.h"
#include "ImageInterface/VillageNotice/VillageNotice.h"
#include "ImageInterface/VillageWarNotice/VillageWarNotice.h"
#include "ImageInterface/NewYearEvent/NewYearEvent.h"

#include "Interface/FocusManager/FocusManager.h"

#include "Interface/LetterDlg/LetterDlg.h"

#include "Interface/RichEditChatMgrDlg/RichEditChatMgrDlg.h"

#include "Interface/RecommendPersonDlg/RecommendPersonDlg.h"
#include "Interface/StockDistributionDlg/StockDistributionDlg.h"
#include "NInterface/NStockDistributionDlg/NStockDistributionDlg.h"
#include "Interface/VoteDlg/VoteDlg.h"
#include "Interface/ResidentsViewDlg/ResidentsViewDlg.h"
#include "Interface/OnGameNetEnviroment/OnGameNetEnviromentDlg.h"
#include "Interface/DailyQuestDlg/DailyQuestSelectDlg.h"
#include "NInterface/NDailyQuestDlg/NDailyQuestSelectDlg.h"

#include "Interface/DailyQuestDlg/DailyQuestSituationDlg.h"
#include "Interface/GomZiDlg/GomZiDlg.h"
#include "Interface/NotifyGetExpAndItemDlg/NotifyGetExpAndItemDlg.h"
#include "NInterface/NNotifyGetExpAndItemDlg/NNotifyGetExpAndItemDlg.h"
#include "Interface/Durability/Durability.h"
#include "Interface/SearchDetailDlg/SearchDetailDlg.h"

#include "NInterface/NMakeItemDemoDlg/NMakeItemDemo.h"
#include "NInterface/NChange/NChangeDlg.h"

// 윈도우 좌표 기억 관련
#include "Interface/RememberWindowPos/RememberWindowPosMgr.h"

// 마우스 오른쪽 버튼 눌렀을때 메뉴
#include "Interface/RightMouseMenu/RightMouseMenu.h"
#include "NInterface/nRightMouseUserListDlg/NRightMouseUserListDlg.h"

// 우측 상단에 있는 정보들 보여주는 다이얼로그
#include "Interface/ToolBarBaseInfoDlg/ToolBarBaseInfoDlg.h"

//#include "Interface/Notify_GetItem_From_Monster/NotifyGetItemFromMonsterDlg.h"

#include "Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "NInterface/NErrorMsgDlg/NErrorMsgDlg.h"
#include "Interface/RareItemDlg/RareItemDlg.h"
#include "Interface/RareItemDlg/RareItemDlg.h"

#include "Interface/PersonalInfo/PersonalInfo.h"
#include "NInterface/NPersonalInfoDlg/NPersonalInfo.h"

//#include "Interface/DeathDlg/DeathDlg.h"
#include "NInterface/NDeathDlg/NDeathDlg.h"

#include "Interface/LoginNoticeDlg/LoginNoticeDlg.h"
#include "Interface/ResidenceCheck/ResidenceCheck.h"
#include "Interface/SiegeWinDlg/SiegeWinDlg.h"
#include "Interface/FanPriest/FanPriestDlg.h"
#include "Interface/WennyOldMan/WennyOldMan.h"

#include "Interface/InvestmentDlg/InvestmentDlg.h"

#include "AttackEffect/AttackEffectMgr.h"

#include "Prompt/PromptBaseDefinition.h"
#include "Prompt/PromptMgr.h"

#include "DrawSequence/DrawSequence.h"

#include "../Common/Letter/Letter.h"

#include "../Common/Bullet/Bullet.h"
#include "../Common/Smoke/Smoke.h"
#include "../CommonLogic/MagicMgr/MagicMgr.h"
#include "../Client/ItemInfo/ItemInfo.h"
#include "../Client/QuitConfirm/QuitConfirm.h"
#include "../Common/Father/Father.h"

#include "../Common/HintDlg/HintDlg.h"

#include "../Client/Effect/Emoticon/EmoticonMgr.h"

// 경험치 관련 
#include "Effect/ExpCounter/ExpCounter.h"

#include "../Common/Order/Order.h"
#include "ITZEncrypt.h"

#include "Msg/MsgType-Quest.h"
#include "Msg/MsgType-PersonalShop.h"
#include "Msg/MsgType-Horse.h"
#include "MsgType-Person.h"


#include "Effect/FireFly/FireFlyMgr.h"

#include "Effect/IceFlash/IceFlashMgr.h"

#include "Cursor/Cursor.h"

#include "StatusBar/ChatStatusBar/ChatStatusBarBaseInfo.h"
#include "StatusBar/ChatStatusBar/ChatStatusBar.h"

// 채팅 관련. 
#include "Interface/DrawChat/DrawChatSurface.h"
#include "Interface/DrawChat/DrawChat.h"

// 캐릭터 이름 출력 관련
#include "Interface/DrawCharName/DrawCharNameSurface.h"


// 귓속말 채팅 관련. 
//#include "Interface/PrivateChatDlg/PrivateChatBaseInfo.h"
//#include "Interface/PrivateChatDlg/PrivateChatDlgMgr.h"

// 사운드 on off 창 // 환경 설정창
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

// 필살기 효과. 
#include "Char/CharClient/CharCriticalEffect/CharCriticalEffect.h"

#include "../CommonLogic/CityHall/TaxPaymentListDlg.h"

// 조합형 캐릭터 서피스 관리자. 
#include "Char/CCD/CCDSurfaceMgr.h"
//이동 표식. 
#include "Effect/ClickEffect/ClickEffect.h"
// 게임 약관 
#include "GameContract/GameContract.h"
// 마을 상세 정보 
#include "NInterface/NVillageDetailInfo/NVillageDetailInfo.h"

#include "../Common/School/School.h"

// 사냥터 상세 정보창
#include "../../common/HuntMapInfo/HuntMapInfo.h"

// 아이템 몰 
#include "../../Client/ninterface/nnewitemmalldlg/NNewItemMall.h"
#include "../../Client/ninterface/nnewitemmalldlg/NBuyWindowDlg.h"
#include "../../Client/ninterface/nnewitemmalldlg/NNewItemInfoDlg.h"
#include "../../Client/ninterface/nnewitemmalldlg/NPresentInfoWindowDlg.h"
#include "../../Client/ninterface/nnewitemmalldlg/NPresentWindowDlg.h"

#include "NInterface/NTradeServerDlg/NTradeServerDlg.h"

#include "../../Common/util/util.h"

// 건물
#include "../../CommonLogic/Cityhall/Cityhall.h"
#include "../../CommonLogic/Bank/Bank-Manager.h"
#include "../../CommonLogic/Stock/Stock.h"
#include "../../CommonLogic/Market/Market.h"
#include "../../CommonLogic/House/House.h"
#include "../../CommonLogic/Hunt/Hunt.h"
#include "../../CommonLogic/PostOffice/PostOffice.h"
#include "../../CommonLogic/HorseMarket/HorseMarket.h"
#include "../../CommonLogic/RealEstate/RealEstate.h"
#include "../../CommonLogic/SummonMarket/SummonMarket.h"
#include "../../CommonLogic/Land/Land.h"
#include "../../CommonLogic/Feast/Feast.h"
#include "../../CommonLogic/SummonHeroMarket/SummonHeroMarket.h"
#include "../../CommonLogic/Guild/Guild.h"
#include "../../CommonLogic/Guild/GuildWar.h"

// SJY , 아큐관련
#include "../../CommonLogic/WorldTraderMgr/WorldTraderManager.h"
#include "../../CommonLogic/WorldMoneyMgr/WorldMoneyMgr.h"

#include "MsgType-House.h"

// NPC수리 
#include "../Client/NPCRepair/NPCRepair.h"
#include <Mouse.h>

// 화면 캡쳐 
#include "ScreenShot/CaptureJPG.h"

// 육조판서 & 군주(왕)
#include "Interface/Byungzo/ByungzoDlg.h"
#include "Interface/Yezo/YezoDlg.h"
#include "Interface/Hozo/HozoDlg.h"
#include "Interface/GoonzuDlg/GoonzuDlg.h"
#include "Interface/Izo/IzoDlg.h"
#include "Interface/Gongzo/GongzoDlg.h"
#include "Interface/Hyungzo/HyungzoDlg.h"


// 아이템 제조 알림창
//#include "Interface/MakeItemDlg/MakeItemDlg.h"
#include "NInterface/NMakeItemDlg/NMakeItemDlg.h"


// 공성전 결과 알림창
#include "Interface/VillageWarInfoDlg/VillageWarInfoDlg.h"
#include "NInterface/NVillageWarInfoDlg/NVillageWarInfoDlg.h"

// 장길산 창 - SBS
#include "Interface/JangGilSan/JangGilSanDlg.h"

// 무당 
#include "Interface/MuDang/MuDangDlg.h"
#include "NInterface/NMudangDlg/NMudangDlg.h"

#include "Interface/FishingDlg/FishingDlg.h"
//#include "Interface/FarmingDlg/FarmingDlg.h"
#include "NInterface/NFarmingDlg/NFarmingDlg.h"


#include "Interface/TradeMerchantDlg/TradeMerchantDlg.h"
#include "Interface/TradeMerchantHorse/TradeMerchantHorse.h"
#include "Interface/TradeMerchantDish/TradeMerchantDish.h"
#include "Interface/ForeignMerchant/ForeignMerchantBuyCashItemDlg.h"

#include "Interface/NPCMenuDlg/NPCMenuDlg.h"

// 웹 이벤트
#include "Interface/EventNotice/EventNotice.h"
#include "Interface/WebEventDlg/WebEventDlg.h"
#include "Interface/HtmlDlg/HtmlDlg.h"

// 비밀번호 변경 권고창
#include "Interface/DecreePasswdDlg/DecreePasswdDlg.h"

// DB 메지시 
#include "../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../DBManager/GameDBManager_World/DBGameLogMsg.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"

#include "InterfaceMgr/InterfaceMgr.h"
#include "InterfaceMgr/NMsgBoxDlg.h"
#include "InterfaceMgr/NCandidateDlg.h"
#include "Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "Interface/NotifyGetItemDlg/NotifyGetItemDlg.h"
#include "Interface/NoticeEventMsgDlg/NoticeEventMsgDlg.h"
#include "Interface/TestDlg/TestDlg.h"
#include "Interface/MiniGameDlg/MiniGameDlg.h"
#include "Interface/MinigameDlg/RouletteGameDlg/RouletteGameDlg.h"
#include "Interface/NInventory/NInventory.h"

#include "NInterface/NNewInventory/NNewInventory.h"
#include "ninterface/nnewmakeitem/NNewMakeItemDlg.h"
#include "ninterface/NTreasureBoxDlg/NTreasureBoxDlg.h"
#include "NInterface/ncharstatus/NEquipWindow.h"
#include "NInterface/ncharstatus/NCharQuest.h"
#include "NInterface/NItemInfoDlg/ItemInfoDlg.h"
#include "Interface/ToolBarDlg/ToolBarDlg.h"
#include "NInterface/NCharMenuBarDlg/NCharMenuBarDlg.h"
#include "NInterface/NSysMenuBarDlg/NSysMenuBarDlg.h"
#include "NInterface/NStatusBarDlg/NStatusBarDlg.h"
#include "NInterface/NChatBarDlg/NChatBarDlg.h"
#include "ninterface/ninfodlg/NInfoDlg.h"

#include "NInterface/NChatDlg/NChatDlg.h"
#include "Interface/TotalChatDlg/TotalChatDlg.h"
#include "Interface/NExitDlg/NExitDlg.h"
#include "NInterface/NHomePageDlg/HomePageDlg.h"
#include "NInterface/NHuntMapInfoDlg/HuntMapInfoDlg.h"
#include "NInterface/NResidentsInfoDlg/ResidentsInfoDlg.h"
#include "NInterface/NMagicInfoDlg/MagicInfoDlg.h"
#include "NInterface/NMagicInfoDlg/NewMagicInfoDlg.h"
#include "NInterface/NTeacherDlg/FatherDlg.h"
#include "NInterface/NTeacherDlg/ChildDlg.h"
#include "NInterface/NPartyDlg/NPartyDlg.h"
#include "NInterface/NPartyDlg/NPartyInviteDlg.h"
#include "NInterface/NQuestDlg/NQuestDlg.h"
#include "NInterface/NEventNotice/NEventNoticeDlg.h"
#include "NInterface/NRareItemDlg/NRareItemDlg.h"
#include "NInterface/NSelectWorldListDlg/SelectWorldListDlg.h"
#include "NInterface/NLogin/NLogin.h"
#include "NInterface/NSelectChar/NSelectChar.h"
#include "NInterface/NMakeChar/NMakeChar.h"
#include "NInterface/NLetterDlg/NLetterDlg.h"
#include "DrawLogo/DrawLogo.h"
#include "NInterface/NNoticeAdminDlg/NNoticeAdminDlg.h"
#include "NInterface/NRecommendPersonDlg/NRecommendPersonDlg.h"
#include "NInterface/NDeleteCharDlg/NDeleteCharDlg.h"
#include "NInterface/NForeignMerchantDlg/NForeignMerchantDlg.h"
#include "NInterface/NForeignMerchantDlg//NForeignMerchantBuyItemDlg.h"
#include "NInterface/NForeignMerchantDlg/NForeignMerchantBuyCashItemDlg.h"
#include "NInterface/NNPCMenuDlg/NNPCMenuDlg.h"
#include "NInterface/NTradeMerchantHighClassDlg/NTradeMerchantHighClassDlg.h"
#include "NInterface/NItemMallManager/NItemMallManager.h"
#include "NInterface/NItemMallManager/NItemMallManager1.h"
#include "NInterface/NItemMallManager/NItemMallGiveStatus.h"
#include "NInterface/NNewQuestDlg/NNewQuestDlg.h"
#include "NInterface/NSummonUnitBuyNpcDlg/NSummonUnitBuyNpcDlg.h"
#include "ninterface/NPremiumStatus/NPremiumStatus.h"
#include "ninterface/NBuffStatus/NBuffstatus.h"

// 욕설 필터
#include "AbuseFilter/AbuseFilter.h"

#include "NInterface/NMaster/NMaster.h"
#include "NInterface/NBobusangDlg/NBobusangDlg.h"
#include "NInterface/NSelecWarpTicket/NSelectNoLimitWarpTicket.h"
#include "NInterface/NCharStatus/NCharStatus.h"
#include "NInterface/NCharStatus/NCharNameChange.h"
#include "NInterface/NCharStatus/NCharGreetingChange.h"
#include "NInterface/NCharStatus/NCharKindChange.h"
#include "NInterface/NCharStatus/NCharHorseTypeChange.h"
#include "NInterface/NCharStatus/NCharSummonReset.h"
#include "NInterface/NDecreePasswdDlg/NDecreePasswdDlg.h"
#include "NInterface/NSiegeWinDlg/NSiegeWinDlg.h"
#include "NInterface/NFishingDlg/NFishingDlg.h"
#include "NInterface/NMiningDlg/NMiningDlg.h"
#include "NInterface/NInvestmentDlg/NInvestmentDlg.h"
#include "NInterface/NNPCRepairDlg/NNPCRepair.h"
#include "NInterface/NNPC1/NNPCTradeDlg.h"
#include "NInterface/NTradeMerchantHorseDlg/NTradeMerchantHorseDlg.h"
#include "NInterface/NForeignMerchantDlg/NForeignMerchantBuyItemDlg.h"
#include "NInterface/NForeignMerchantDlg/NForeignMerchantBuyCashItemDlg.h"
#include "NInterface/NTradeMerchant/NTradeMerchant.h"
#include "NInterface/NWennyOldMan/NWennyOldMan.h"
//#include "NInterface/NWennyOldMan/NWennyChild1.h"
#include "NInterface/NBossMarket/NBossMarketDlg.h"
#include "NInterface/NHintDlg/NHintDlg.h"
#include "NInterface/NQuestViewer/NQuestViewerDlg.h"
#include "NInterface/NGeneralMeetingLetterDlg.h"


#include "NInterface/NHozoDlg/NHozoBaseDlg.h"
#include "NInterface/NByungzoDlg/NByungzoDlg.h"
#include "NInterface/NIzoDlg/NIzoBaseDlg.h"
#include "NInterface/NGoonzuDlg/NGoonzuDlg.h"
#include "NInterface/NGoonzuDlg/NGoonzuWorkDlg.h"
#include "ninterface/ngoonzudlg/NGoonzuSiege.h"
#include "NInterface/NGongzoDlg/NGongzoDlg.h"
#include "NInterface/NHyungzoDlg/NHyungzoBaseDlg.h"
#include "NInterface/NYezoDlg/NYezoBaseDlg.h"
#include "NInterface/NPrivateTradeDlg/NPrivateTradeDlg.h"
#include "NInterface/NPrivateTradeDlg/NPrivateTradeAcceptDlg.h"
#include "NInterface/NPrivateRepair/NPrivateRepairDlg.h"

#include "NInterface/NSkillDlg/NSkillDlg.h"
#include "NInterface/NWeaponSkillDlg/NWeaponSkillDlg.h"
#include "NInterface/NResidentsDlg/NResidentsDlg.h"
#include "NInterface/NGuildChatDlg/NGuildChatDlg.h"
#include "NInterface/NHongGilDongDlg/NHongGilDongDlg.h"
#include "NInterface/NWarJoinDlg/NWarJoinDlg.h"

#include "NInterface/NPlayMatchDlg/NPlayMatchDlg.h"
#include "ninterface/NPlayMatchDlg/NBlackArmyMVPDlg.h"
#include "NInterface/NPlayMatchDlg/NGuildDungeonDlg.h"
#include "NInterface/NPlayMatchDlg/NGuildDungeonApplyDlg.h"

#include "ninterface/ngetpresentdlg/NGetPresentDlg.h"
#include "ninterface/ngetpresentdlg/NThanksPresentDlg.h"
#include "ninterface/NMusicDlg/NMusicDlg.h"


//건물
#include "NStructureDlg/Bank/NBankDlg.h"
#include "NStructureDlg/Stock/NStockDlg.h"
#include "NStructureDlg/Market/NMarketDlg.h"
#include "NStructureDlg/House/NHouseDlg.h"
#include "NStructureDlg/PostOffice/NPostOfficeDlg.h"
#include "NStructureDlg/Hunt/NHuntDlg.h"
#include "NStructureDlg/Land/NLandDlg.h"
#include "NStructureDlg/Mine/NMineDlg.h"
#include "NStructureDlg/Cityhall/NCityhallDlg.h"
#include "NStructureDlg/SummonMarket/NSummonMarketDlg.h"
#include "NStructureDlg/SummonHeroMarket/NSummonHeroMarketDlg.h"
#include "NStructureDlg/Feast/NFeastDlg.h"
#include "NStructureDlg/RealEstateMarket/NRealEstateMarketDlg.h"
#include "NStructureDlg/HorseMarket/NHorseMarketDlg.h"
#include "NStructureDlg/Guild/GuildDlg.h"
#include "NStructureDlg/CityHall/NCityHallSubmitDlg.h"
#include "NStructureDlg/Cityhall/NCityhallGeneralMeeting2Dlg.h"
#include "NStructureDlg/Cityhall/NAccountBook.h"
#include "NStructureDlg/NewMarket/NNewMarketDlg.h"

#include "NInterface/NPrivateChatDlg/NPrivateChatDlgMgr.h"
#include "NInterface/NPrivateChatDlg/NPrivateChatDlg.h"
#include "NInterface/NPrivateChatDlg/NPrivateChatRejectDlg.h"

#include "NStructureDlg/CityHall/NHireSystemNPCDlg.h"
#include "NStructureDlg/Cityhall/NCityhallConstructDlg.h"
#include "NStructureDlg/Cityhall/NCityhallResidentListDlg.h"
#include "NStructureDlg/Cityhall/NAppointmentChiefDlg.h"
#include "NStructureDlg/Cityhall/NStructureMessageDlg.h"

#include "NInterface/NNewEnchantOther/NNewEnchantOther.h"
#include "NInterface/NNewEnchantOther/NNewEnchantOtherAccept.h"

#include "NInterface/NMenuDlg/NMenuDlg.h"
#include "NInterface/NRightMouseMenuDlg/NRightMouseMenuDlg.h"
#include "NInterface/NMiniMapDlg/NMiniMapDlg.h"
#include "NInterface/NSaveItemListDlg/NSaveItemListDlg.h"
#include "NStructureDlg/cityhall/NBeResidentDlg.h"
//건물 상세
#include "NStructureDlg/House/UserInfo/NHouseUserInfoBaseDlg.h"
#include "NStructureDlg/House/NHouseExtensionDlg.h"
#include "NStructureDlg/House/NHouseNameChangeDlg.h"

#include "NStructureDlg/Cityhall/NVillageTreeDlg.h"

#include "NInterface/NNewMakeItem/NNewMakeItemDlg.h"

#include "PCRoom/PCRoomIntro.h"

// 내자시 이펙트
#include "../Client/FeastEffect/FeastEffectManager.h"
#include "../Client/DLGPositionSave/DLGPositionSave.h"

// 운영자메뉴
#include "ninterface/ngamemasterdlg/NGameMaster.h"
#include "ninterface/NCommonMsgDlg/NCommonMsgDlg.h"

//시스템 메뉴바 차일드메뉴
#include "ninterface/nsysmenubardlg/nsyschildmenudlg.h"

//아이템 획득
#include "ninterface/NShowGetItemDlg/NShowGetItemDlg.h"

// 무역관련
#include "ninterface/NAqTradeWindowDlg/NAqTradeWindowDlg.h"
#include "ninterface/NSetWorldMoneyBuyDlg/NSetWorldMoneyBuyDlg.h"
//#include "ninterface/NTradeServerDlg/NTradeServerDlg.h"

//추천 포인트 관련
#include "ninterface/nrpmerchantdlg/NRPList.h"
#include "ninterface/nrpmerchantdlg/NRPMerchant.h"
#include "ninterface/nrpmerchantdlg/NRPBase.h"

//국가 정보 관련
#include "ninterface/nSetCountryDlg/NSetCountryDlg.h"
#include "ninterface/NUserListByCountryDlg/NUserListByCountryDlg.h"
#include "ninterface/NDownloadAddonDlg/NDownloadAddon.h"

// 아이템몰 아이템 정보 관련 다이얼로그
#include "ninterface/nItemMallProductDlg/NItemMallProductDlg.h"

// 090326 [종호]	기간제 아이템 정보 관련 다이알로그 
#include "ninterface/nPeriodItemListDlg/NPeriodItemListDlg.h"

// 아이템 분리창
#include "ninterface/NSelectItemCountDlg/NSelectItemCountDlg.h"

// 오프닝 배너
#include "ninterface/NOpeningBannerDlg/NOpeningBannerDlg.h"

// 20렙 이하 유저
#include "NInterface/NLowLevelDlg/NLowLevelDlg.h"

// 자기 길드 정보창
#include "NStructureDlg/Guild/MyGuild/MyGuildBase.h"
#include "NStructureDlg/Guild/MyGuild/NBeGuildMemberDlg.h"

// 길드 정보창
#include "ninterface/NGuildInfoDlg/NGuildInfoDlg.h"
#include "ninterface/NGuildInfoDlg/NGuildMemberDlg.h"
#include "ninterface/NGuildInfoDlg/NGuildSendMsg.h"

// 이모티콘
#include "ninterface/NEmoticonDlg/NEmoticonDlg.h"

#include "ninterface/nfrienddlg/NFriendMemoDlg.h"
#include "Ninterface/NManufactureNPCDlg/NManufactureNPC.h"
#include "Ninterface/NBeginnerWeponNPCDlg/NBeginnerWeaponNPC.h"
#include "Ninterface/NGuildWarApplyDlg/NGuildWarApply.h"
#include "NStructureDlg/Guild/GuildWarRecommand/GuildWarRecommand.h"

#include "Ninterface/ngachadlg/NGachaDlg.h"
#include "Ninterface/ngachadlg/NGachaDlgNew.h"
#include "Ninterface/ngachadlg/NChoiseItemGameStartDlg.h"
#include "Ninterface/ngachadlg/NOpenBoxDlg.h"
#include "Ninterface/ngachadlg/NRecievedItemDlg.h"

#include "Ninterface/ngacha2dlg/NGacha2Dlg.h"

#include "Ninterface/NMofuMofuList/NMofuMofuList.h"

#include "Ninterface/NMykey/NMykey.h"
#include "Ninterface/NMykey/NMykeyMini.h"
#include "Ninterface/NAmberBoxDlg/NAmberBoxDlg.h"
#include "Ninterface/NYesOrNoDlg/NYesOrNoDlg.h"
#include "Ninterface/NHelperDlg/NHelperMiniDlg.h"
#include "Ninterface/NHelperDlg/NHelperQuestionOrAnswerDlg.h"
#include "Ninterface/NHelperDlg/NHelperQuestionListDlg.h"
#include "Ninterface/NHelperDlg/NHelperAnswerCompleteDlg.h"

#include "Ninterface/NElixirDlg/NElixirStatusDlg.h"
#include "Ninterface/NElixirDlg/NElixirUseDlg.h"
#include "Ninterface/NElixirDlg/NElixirNPCDlg.h"

#include "BeginnerText/BeginnerText.h"
#include "LevelUpText/LevelUpText.h"
#include "Gametip/GameTip.h"
#include "MarkManager/MarkManager.h"
#include "ninterface/NNamingQuest/NNamingQuestListDlg.h"
#include "ninterface/NHelpButton/NHelpButtonDlg.h"
#include "ninterface/NNamingQuest/NSelectNamingDlg.h"
#include "ninterface/NSystemRewardList/NSystemRewardListDlg.h"
#include "ninterface/NAttendCheck/NAttendCheckDlg.h"
#include "ninterface/NHappyBean/NHappyBeanDlg.h"//해피빈이벤트 [2007.08.22 손성웅]
#include "ninterface/NQuestSelectItem/NQuestSelectItemDlg.h"
#include "ninterface/NNewNotify/NNewNotifyDlg.h"
#include "ninterface/NAuctionEvent/NAuctionEventDlg.h"
#include "ninterface/NSystemRewardList/NExchangeStock.h"
#include "ninterface/NChristmasEventDlg/NChristmasEventDlg.h"

#include "ninterface/ChristmasQuestDlg/ChristmasQuestDlg.h"
#include "ninterface/NNewYearDlg/NNewYearDlg.h"
#include "ninterface/NCharTransformDlg/CharTransformDlg.h"
#include "ninterface/NCharTransformDlg/CharTransformStoryDlg.h"
#include "ninterface/NCharTransformDlg/CharTransformStoryReadDlg.h"
#include "ninterface/NInstantDungeonDlg/NInstantDungeonBoardDlg.h"
#include "ninterface/NInstantDungeonDlg/InstantDungeonRunInfoDlg.h"
#include "ninterface/NInstantDungeonDlg/InstanstDungeonStatusDlg.h"
#include "ninterface/NInstantDungeonDlg/NInstantDungeonNotipyDlg.h"
#include "ninterface/NBlackWarReformDlg/NBlackWarReformRunDlg.h"
#include "ninterface/NBlackWarReformDlg/NBlackWarReformRewardDlg.h"
#include "ninterface/NSpaceBoxDlg/NSpaceBoxDlg.h"
#include "ninterface/NArtifactDlg/NArtifactBaseDlg.h"
#include "ninterface/NArtifactDlg/NArtifactMixtureDlg.h"
#include "ninterface/NArtifactDlg/NArtifactSlotCreateDlg.h"
#include "ninterface/NArtifactDlg/NArtifactRestoreDlg.h"
#include "ninterface/NArtifactDlg/NArtifactRemoveDlg.h"

#include "ninterface/NPartyhallDlg/DLG_PartyHall_In_Base.h"
#include "ninterface/NPartyhallDlg/DLG_PartyHall_In_Costume_User.h"
#include "ninterface/NPartyhallDlg/DLG_PartyHall_In_firecracker_Chief.h"
#include "ninterface/NPartyhallDlg/DLG_PartyHall_In_Invite_Chief.h"

#include "ninterface/NAttendCheck/NAttendNoticeDlg.h"

#include "NStructureDlg/Cityhall/NInviteEmpire.h"
#include "../lib/MD5/md5.h"

#include "NInterface/NDailyQuest2/NDailyQuest2SelectDlg.h"
#include "ninterface/NElectionStumpDlg/NElectionStumpDlg.h"
#include "ninterface/NHiddenQuestDlg/NHiddenQuestDlg.h"

#include "ninterface/ncharmenubardlg/NCharMenuBarDlg.h"
// [영훈] 내부 UI : Pannel
#include "Interface/pannel/NPannel_DateMoneyDlg.h"		//	날짜와 돈이 표시됨
#include "Interface/pannel/NPannel_CharStatusDlg.h"		//	케릭터 기본정보
#include "Interface/pannel/NPannel_SummonDlg.h"			//	소환수
#include "Interface/pannel/NPannel_OrganDlg.h"			//	내장기관

#include "ninterface/ncharstatus/NCharAbilityDatailDlg.h"
#include "ninterface/NOpenBigItemBoxDlg/NOpenBigItemBoxDlg.h"
#include "NInterface/NDailyQuest3/NDailyQuest3SelectDlg.h"

#include "ninterface/NMonsterAttackEventDlg/NMonsterAttackEventDlg.h"

#include "ninterface/NGMInfo/NGMInfoDlg.h"

#include "NInterface/NSetChatDlg/NSetChatDlg.h"	// 대화창 메세지 선별 출력을 위한
#include "NInterface/NBuffRoulette/NBuffRoulette.h"	// 대화창 메세지 선별 출력을 위한

#include "ninterface/NMapNameDlg/NMapNameDlg.h"

#include "ninterface/NServerEventDlg/NServerEventRankDlg.h"
#include "ninterface/NServerEventDlg/NServerEventQuestDlg.h"
#include "ninterface/NServerEventDlg/NServerInstanceMapBounusDlg.h"
#include "ninterface/NServerEventDlg/NServerEventIndunInfo.h"
#include "ninterface/NTreasureExtraBoxDlg/NTreasureBoxExtraDlg.h" // [성웅] 2차 가차 박스

#include "ninterface/NQuestInfoDlg/NQuestInfoDlg_Parent.h"
#include "ninterface/NQuestInfoDlg/NQuestInfoDlg_GoingQuest.h"
#include "ninterface/NQuestInfoDlg/NQuestInfoDlg_AllQuest.h"

#include "ninterface/NQuestNoticeDlg/NQuestNoticeDlg.h"

#include "ninterface/NMySummonDlg/NMySummonDlg.h"				// [성웅]소환수 보관 시스템

#include "CheckProcess/CheckProcess.h"

#include "ninterface/NAcuTradeDlg/NAcuTradeDlg_Parent.h"
#include "ninterface/NCharRoulette/NCharRoulette.h"

#include "ninterface/NMagicCoolTimeDlg/NMagicCoolTimeDlg.h"
#include "ninterface/NPVPLeagueDlg/NPVPLeageuDlg.h"
#include "ninterface/NNewYearEventRouletteDlg/NNewYearEventRoulette.h"
#include "ninterface/NValantineDlg/NValantineDlg.h"

#include "ninterface/NNewSelectBoxDlg/NNewSelectBoxDlg.h"
#include "ninterface/NSummonInventory//NSummonInventory.h"
#include "ninterface/NPlayMatchDlg_New/NGuildDungeonDlg_New.h"	//길드 사냥터 소유 방식 변경
#include "ninterface/NPlayMatchDlg_New/NRequestHuntingMapWarDlg.h"	//길드 사냥터 소유 방식 변경

#include "ninterface/NCarryOverGachaDlg/NCarryOverGachaDlg.h"
#include "ninterface/NCarryOverGachaDlg/NCarryOver_ProductDlg.h"
#include "ninterface/NCarryOverGachaDlg/NCarryOver_ResultDlg.h"

#include "ninterface/NGuildWarResult/NGuildWarResult.h"
#include "ninterface/NKillSignDlg/NKillSignDlg.h"
#include "ninterface/NPromoter/NPromoter_InfoDlg.h"
#include "ninterface/NPromoter/NPromoter_Give_Item_Notice.h"
#include "ninterface/NPromoter/NPromoter_Mini.h"
#include "ninterface/NPromoter/NPromoter_Introduce.h"

#include "ninterface/NWeddingCeremony/NWeddingCeremony.h"
#include "ninterface/NWeddingCeremony/NProposeQuestionDlg.h"
#include "ninterface/NWeddingCeremony/NProposeAnswerDlg.h"
#include "ninterface/NMarriageLoveLevelDlg/NMarriageLoveLevelDlg.h"
#include "ninterface/NWeddingAnniversaryDlg/NWeddingAnniversaryDlg.h"
#include "ninterface/NWeddingCeremony/NWeddingCeremonyListDlg.h"

#include "ninterface/NFamilyInfoDlg/NFamilyInfoDlg.h"
#include "ninterface/NMyFamilyInfoDlg//NMyFamilyInfoDlg.h"
#include "ninterface/NBalanceCurrencyDlg/NBalanceCurrencyDlg.h"

#include "ninterface/NFamilyInfoDlg//NButtonMenuDlg.h"
#include "ninterface/NFamilyInfoDlg//NFamily_Add_ChildrenDlg.h"
#include "ninterface/NFamilyCookieDlg/NFamilyCookieDlg.h"

#include "interface/pannel/NPannel_SoulGuardDlg.h"
#include "ninterface/ncharstatus/NCharSoulGuardHouse.h"
#include "ninterface/ncharstatus/NCharSoulGuardInfo.h"

#include "ninterface/NPCRoom/NPCRoom_Display.h"
#include "ninterface/NPCRoom/NPCRoom_Reward.h"
#include "ItemPeriodMgr/ItemPeriodMgr.h"

#include "ninterface/NRacconDodgeEventDlg/NRacconDodgeEventDlg.h"
#include "ninterface/NRacconDodgeEventDlg/NRacoonDodgeEventReserveListDlg.h"

#include "ninterface/NBokBulBokEventDlg/NBokBulBokEventDlg.h"				// [지연] 복불복 이벤트
#include "ninterface/NBokBulBokEventDlg/NBokBulBokEventReserveListDlg.h"	
#include "ninterface/NGawiBawiBoDlg/NGawiBawiBoDlg.h"

#include "GawiBawiBoMgr/GawiBawiBoMgr_Client.h"
#include "Client_BokBulBokMgr/Client_BBBMgr.h"


//	[기형] 휴면 계정 시스템
#include "ninterface/NDormancyAccount/NDormancyArmourBoxDlg.h"
#include "ninterface/NDormancyAccount/NDormancyWeaponBoxDlg.h"
#include "ninterface/NDormancyAccount/NDormancyPointDlg.h"

#include "ninterface/nnpcrepairdlg/NNewNPCRepair.h"

#include "ninterface/NAvatartQuestDlg/NAvatartQuestDlg.h"
#include "Interface/NHalloweenSpiritBox/NHalloweenSpiritBox.h"

#include "ninterface/NWarnDurabilityDlg/NWarnDurabilityDlg.h"

// 야외지역 전체맵
#include "ninterface/NMainMapDlg/NMainMapDlg.h"

// 몬스터 생성 창
#include "ninterface/NBalanceTestGMDlg/NMonsterCreateDlg.h"

// 장비 생성 창
#include "ninterface/NBalanceTestGMDlg/NEquipmentCreateDlg.h"

#include "ninterface/NCharHouseDlg/NCharHouseDlg.h"

#include "ninterface/NChangeCharName/NChangeCharName.h"
#include "ninterface/NCopfield_NPC/NCopfield_NPC.h"
#include "ninterface/ngoonzudlg/NGoonzuRaccoonEventDlg.h"

#include "ninterface/NBattleRoyal/NBattleRoyalResultDlg.h"
#include "ninterface/NBattleRoyal/NBattleRoyalTimeDlg.h"
#include "ninterface/NBattleRoyal/NBattleRoyalMoveDlg.h"

// 2010신년이벤트 포춘쿠키 운세 창.
#include "ninterface/N2010NewYearEventDlg/N2010NewYearDlg.h"

#include "GZMacroAnalysis/GMA.h"

// 초보자 파티 퀘스트(파티매칭)
#include "ninterface/NPartyQuest_PartyMatchingDlg/NPartyMatching_SelectQuestDlg.h"
#include "ninterface/NPartyQuest_PartyMatchingDlg/NPartyMatching_ApplytoPartyDlg.h"
#include "ninterface/NPartyQuest_PartyMatchingDlg/NPartyMatching_CreatePartyDlg.h"
#include "ninterface/NPartyQuest_PartyMatchingDlg/NPartyMatching_PartyJoinDlg.h"
#include "../../CommonLogic/PartyMatchingMgr/Client_PartyMatchingMgr.h"
#include "PartyQuestMgr/PartyQuestMgr_Client.h"

#include "ninterface/NGiantRaccoonResurrectEventDlg/NGiantRaccoonResurrectEventDlg.h"
#include "ninterface/NGiantRaccoonResurrectEventDlg/NPotRaccoonDlg.h"
#include "ninterface/NPVPMapDlg/NPVPMapDlg.h"
#include "ninterface/NManufactureDlg_New/NManufactureDlg_New.h"
#include "ninterface/NManufactureSerchDlg/NManufactureSerchDlg.h"
#include "ninterface/NVanlentineDayNPC/NValentineDayNPCDlg.h"
#include "ninterface/NVanlentineDayNPC/NValentineDayPackingDlg.h"
#include "ninterface/NVanlentineDayNPC/NValentineDayTradeDlg.h"
#include "ninterface/NVanlentineDayNPC/NValentineDayConfessionQuestionDlg.h"
#include "ninterface/NVanlentineDayNPC/NValentineDayConfessionAnswerDlg.h"

class CItemPeriodMgr;

#ifdef USE_GAME_GUARD_CLIENT
#ifdef _TAIWAN
#include "../GameGuard/NPGameLib_MD_DE.h"
#endif
#endif



#include "../../NLib/NUtil.h"

extern cltCommonLogic* pclClient;
extern	RECT				ScreenRect;							// 지도가 표시되는 영역 
extern SI32		gsiResolution;		// 게임의 해상도. 
extern _InputDevice					IpD;
extern DWORD dwQuitTimer;
extern bool bActiveSwitch;
extern bool g_bHanAuth;
extern bool g_bUSA_Auth;

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
extern bool g_bPorteAuth;
extern char g_szServerIP[20];
extern char g_szServerPORTE[10];
#endif

#ifdef USE_GAME_GUARD_CLIENT
extern CNPGameLib * pNpgl;
extern TCHAR g_szHackMsg[256];
#endif


static DWORD PrevFrameClock;
//extern	CDeathDlg		g_DeathDlg;
bool bDrawSkipSwitch;
CDlgPositionSave g_DlgPositionSave;

#define DEFAULT_FONTFILE_NAME		TEXT("Font/MSGOTHIC.TTC")

bool bSetScreenCenterSwitch = true;
SI32 siOldScreenCenterX, siOldScreenCenterY;

extern SI16 g_siChannelingCode;
extern char g_nm_id[ 1024 ];
extern char g_nm_pw[ 1024 ];

// NUtil.cpp에 선언된 변수
extern UI64	uiSec; // 시간 계산을 위한 기준 변수

//------------------------------------
// 장비 내구도 검사용 클래스. 
//------------------------------------
class cltcheckdurinfo{
public:
	SI32 siPos;
	SI32 siShow;
	TCHAR szName[24];
};

cltClient::cltClient(TCHAR* appname, HWND hwnd, HINSTANCE hinst, SI32 gamemode, SI32 defaultservicearea, TCHAR* pszHanAuth_GameString, SI32 runmode ) : cltCommonLogic(appname, hwnd, hinst, gamemode,defaultservicearea, runmode)
{
	m_szHanAuth_UserID[0]		= '\0';
	m_siHanAuth_TryCount		= 0;

	m_siHanAuth_ServiceTypeCode = 0;

#ifdef _USA_NEW_AUTH_
	m_szUSAAuth_String[0] = '\0';
#endif

#ifdef USE_HANAUTH
	m_szHanAuth_GameString[0] = '\0';
	m_szHanAuth_AuthString[0] = '\0';
#endif // USE_HANAUTH

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
	m_szPorteAuth_UserID[0] = '\0';
#ifndef _SERVER //서버가 정의되어 있지 않다면.
	m_szPorteAuth_GameInfoString[0] = '\0';
	m_szPorteAuth_szAuthTICKET[0] = '\0'; 
#endif
#endif // USE_PORTEAUTH

//	kHanAuth_UpdateTimer_GameRunAuth.Init(4 * 60 * 1000);
//	kHanAuth_UpdateTimer_GameRunAuth.SetActive(true, ::GetTickCount());

	// APEX-
#ifdef _CHINA
	InitializeCriticalSection( &m_CSSendToGameServer );
#endif
	// -APEX

#ifdef _USA_NEW_AUTH_
	if(g_bUSA_Auth && pszHanAuth_GameString != NULL)
	{
		StringCchCopy( m_szUSAAuth_String, sizeof(m_szUSAAuth_String), pszHanAuth_GameString );
	}
#endif

	// -HanAuth
#ifdef USE_HANAUTH
	if(g_bHanAuth && pszHanAuth_GameString != NULL)
	{
		StringCchCopy(m_szHanAuth_GameString, sizeof(m_szHanAuth_GameString), pszHanAuth_GameString);
		SI32	siHanAuthRet = 0;
		//TCHAR	szRet[32];

		// HanAuth InitGameString
		siHanAuthRet = HanAuthInitGameString(m_szHanAuth_GameString);
		if(siHanAuthRet != 0)		{		m_szHanAuth_GameString[0] = '\0';		}

		// HanAuth For Client
		siHanAuthRet = HanAuthForClientGameString(m_szHanAuth_GameString);
		if(siHanAuthRet != 0)		{		m_szHanAuth_GameString[0] = '\0';		}

		siHanAuthRet = HanAuthGetServiceTypeCode(m_szHanAuth_GameString, &m_siHanAuth_ServiceTypeCode);
		if(siHanAuthRet != 0)		{		m_szHanAuth_GameString[0] = '\0';		}

		// HanAuth Get ID
		siHanAuthRet = HanAuthGetId(m_szHanAuth_GameString, m_szHanAuth_UserID, MAX_PLAYER_NAME);
		if(siHanAuthRet != 0)		{		m_szHanAuth_GameString[0] = '\0';		}

		// HanAuth Refresh
		siHanAuthRet = HanAuthRefresh(m_szHanAuth_UserID);
		if(siHanAuthRet != 0)		{		m_szHanAuth_GameString[0] = '\0';		}
	}
#endif // USE_HANAUTH
	// -HanAuth

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
#ifndef _SERVER //서버가 정의되어 있지 않다면.
	if(g_bPorteAuth&& pszHanAuth_GameString != NULL)  //pszHanAuth_GameString 은 실제로 szPorteAuth_GameInfoString 이다.
	{
		StringCchCopy(m_szPorteAuth_GameInfoString, sizeof(m_szPorteAuth_GameInfoString), pszHanAuth_GameString);

		SI32	siPorteAuthRet = 0;

		// 초기화
		siPorteAuthRet = PubAuthInit(m_szPorteAuth_GameInfoString , "Game Client");
		if(siPorteAuthRet != 0)		{		m_szPorteAuth_GameInfoString[0] = '\0';		}

		// HANAUTH를 안쓰는 중국에 게임 아이디 및 정보를 얻는다.
		siPorteAuthRet = PubGetUserID( m_szPorteAuth_UserID,sizeof(m_szPorteAuth_UserID));
		if(siPorteAuthRet != 0)		{		m_szPorteAuth_UserID[0] = '\0';		}

		TCHAR szServerIP[30] = { '\0', };
		TCHAR szServerPORTE[10] = { '\0', };
		
		int GetValueretIP  	 = PubGetValue("H1",szServerIP, sizeof(szServerIP));
		int GetValueretPORTE  = PubGetValue("H2",szServerPORTE, sizeof(szServerPORTE));


		if((GetValueretIP ==0) && (GetValueretPORTE == 0))
		{
			StringCchCopy( g_szServerIP, sizeof(g_szServerIP) , szServerIP);
			StringCchCopy( g_szServerPORTE, sizeof(g_szServerPORTE) , szServerPORTE);
		}

		// 알파/리얼 구분 포트 어스에서는 이걸로 받아 온대.
		int GetValueretGamecode  = PubGetValue("P2",m_szPorteAuth_szServiceCode, sizeof(m_szPorteAuth_szServiceCode));
		if(m_szPorteAuth_szServiceCode != NULL)
		{
			m_siHanAuth_ServiceTypeCode = atoi( m_szPorteAuth_szServiceCode);
		}
		//pclClient->pclLog->FilePrint( "config\\GetArtument.txt" , "1.GameString = %s \n2.g_szServerIP = %s",m_szPorteAuth_GameInfoString, g_szServerIP );

		/*
		// ID 받아오기.
		siPorteAuthRet = PubGetUserID(m_szPorteAuth_UserID, sizeof(m_szPorteAuth_UserID));
		if(siPorteAuthRet != 0)		{		m_szPorteAuth_UserID[0] = '\0';		}

		// Auth 티켓 받아 오기.
		siPorteAuthRet = PubGetAuthTicket(m_szPorteAuth_szAuthTICKET, sizeof(m_szPorteAuth_szAuthTICKET));
		if(siPorteAuthRet != 0)		{		m_szPorteAuth_szAuthTICKET[0] = '\0';		}
		*/
	}
#endif
#endif // USE_PORTEAUTH

#ifdef USE_HANREPORT

	#if defined (_KOREA)
		if (m_siHanAuth_ServiceTypeCode == SERVICE_ALPHA)
			HanReportInit("K_GOONZU", SERVICE_KOR | SERVICE_ALPHA, true);
		else
			HanReportInit("K_GOONZU", SERVICE_KOR | SERVICE_REAL, true);
	#elif defined (_USA)
	
		if(m_siHanAuth_ServiceTypeCode == SERVICE_ALPHA)
			HanReportInit("U_GOONZU", SERVICE_USA | SERVICE_ALPHA, true);
		else
			HanReportInit("U_GOONZU", SERVICE_USA | SERVICE_REAL, true);
		//중국 안정화 지표모듈
	#elif defined (_NHN_CHINA)
		//if(m_siHanAuth_ServiceTypeCode == SERVICE_ALPHA)
			HanReportInit("C_GZ", SERVICE_CHN | m_siHanAuth_ServiceTypeCode, false);
		//else
			//HanReportInit("C_GZ", SERVICE_CHN | SERVICE_REAL, false);
		#endif 
#endif // USE_HANREPORT

#if defined (USE_HANPOLL)
	// 한게임 설문 조사 모듈 초기화. - 종료팝업
	HanPollInit( m_szHanAuth_GameString );
#endif

	SI16 i = 0;

	RegisterClientMsgMap();

	siScreenMode = MODE_GRAPHICS800_600;
	siScreenModeYOffSet = 0;
	siScreenMode800X600YSize = 640;

	siScreenModeXOffSet = 0;
	siScreenMode800X600XSize = 880;

	memset( strTextFontFileName, 0, MAX_PATH );

	// 클라이언트는 40프레임
	siFrameDelay	= 1000 / GetFpsFromFile();
	m_dwNextFrameTick = 0;
	m_dwDelayTick = 0;

	bDropTrySwitch				= false;
	bReceiveServerInfoSwitch	= false;

	siRX	=	siRY	= 0;				// 스크린의 RX, RY
	//-------------------------------------------
	// 디폴트 서비스 지역 
	//-------------------------------------------
	siServiceArea	= defaultservicearea;

	//사용언어
	
	//siLanguage		= GetLanguageFromFile();


	// 아이디와 암호를 초기화한다. 
	StringCchCopy(szID, FILE_NAME_SIZE, TEXT(""));
	StringCchCopy(szPassword, FILE_NAME_SIZE, TEXT(""));

	bWindowNotice		= false;

	siSelectedOrder			= 0;				// 선택된 명령 
	siSelectedOrderPara1	= 0;			// 선택된 명령의 부속 파라메타. 
	siSelectedOrderPara2	= 0;
	ObjectTempSelectedSwitch = FALSE;

	ScrollProcSwitch	= FALSE;

	MyCharUnique	= 0;
	Char_flag		= 0;
	CurrentCharInfoList	= 0;

	IDPara1		= 0; // 각 아이디 값의 파라메타 
	IDPara2		= 0; // 각 아이디 값의 파라메타 

	SelectCharDlg		= NULL;
	MakeCharDlg			= NULL;
	SelectWorldListDlg	= NULL;

	//------------------------------------------
	// SetChar
	//-------------------------------------------
	siSetCharBufferIndex	= 0;
	pclLogo			= NULL;
	pclMusic		= NULL;
	pclGImg			= NULL;

	pclPersonNameManager	= NULL;
	pclNewPersonNameManager = NULL;

	m_pLoadingImg	= NULL ;

	//---------------------------------------------
	// 네트워크 관련 
	//---------------------------------------------
	pGameSession	= NULL;
	pFrontSession	= NULL;
	siAuthKey		= 0;
	bFrontServerActionSwitch	= true;
	bGameServerActionSwitch		= false;

	bFrontServerSecretKeySwitch	= false;
	bGameServerSecretKeySwitch	= false;

	//----------------------------------------
	// 서피스 
	//----------------------------------------
	lpBackScreen	= NULL;

	m_pNPrivateChatDlgMgr = NULL;

	//--------------------------------------------
	// 포커스 상태 관리
	//--------------------------------------------	
	pclFocusManager = NULL;


	//--------------------------------------------
	// 레지스트리 관리 
	//--------------------------------------------
	pclRegistry		= NULL;

	//--------------------------------------------
	// 툴바. 
	//--------------------------------------------
	pclToolBar		= NULL;


	//-------------------------------------------
	// 판넬 
	//------------------------------------------
	pclPannel		= NULL;
	

	//-------------------------------------------
	// 마을 안내문
	//------------------------------------------
	pclVillageNotice  = NULL;

	//-----------------------------------------
	// 수리 모드 
	//-----------------------------------------
	bRepairSwitch	= false;


	//-----------------------------------------
	// 자막 모드 
	//-----------------------------------------
	bPromptSwitch	= true;

	//-----------------------------------------
	// SMALL MAP 모드
	//-----------------------------------------
	bSmallMapSwitch = true;

	//----------------------------------------
	// 출력 순서 정렬 
	//----------------------------------------
	pclDrawSequence	= NULL;

	//----------------------------------------
	// 마을 건물 정보 수신 확인 스위치. 
	//----------------------------------------
	bWaitVillageStructureInfoSwitch = false;		// 마을 건물 정보가 오기를 기다리지 않고 캐릭터를 생성한다. 

	//----------------------------------------
	// 주인공 캐릭터가 최초로 설정되었는지 여부 
	//----------------------------------------
	//bFirstHeroCharInitSwitch	= false;

	//-------------------------------------
	// 채팅 말풍선 관련. 
	//-------------------------------------
	pclDrawChatSurface = NULL;

	//-------------------------------------
	// 캐릭터 이름 출력 관련. 
	//-------------------------------------
	pclDrawCharNameSurface = NULL;

	//---------------------------------------------
	// 윈도우 위치 기억
	//---------------------------------------------
	//pclRememberWindowPosMgr	 = NULL;


	//------------------------------------
	// DB이상 여부.
	//------------------------------------
	bDBFailSwitch	= false;

	//------------------------------------
	// 이동 표식. 
	//------------------------------------
	pclClickManager	= NULL;

	//--------------------------
	// 시간 초기화 
	//--------------------------
	clClientDate.Set(0, 0, 0, 0, 0);

	//--------------------------------
	// 주인공 캐릭터 정보 백업 장치. 
	//--------------------------------
	bReceiveMyCharAllInfoSwitch	= false;		// 주인공 캐릭터의 정보를 받았는지 여부. 
	bBackUpSwitch				= false;
	pclHeroDataBackup			= NULL;

	//---------------------------------------
	// 서버와의 마지막 교신 시간. 
	//---------------------------------------
	dwLastReceiveFromServerClock	= 0;

	//--------------------------------------
	// 서버 점검 중에도 접속할 권한이 있는가?
	//---------------------------------------
	bLoginWhileServerDownSwitch		= false;

	//--------------------------------------
	// 게임 서버 로그인 대기 시간
	//---------------------------------------
	m_kTimer_WaitGameLogin.Init(1000);	// 최대 20초를 기다린다.
 
	//--------------------------------------
	// 마지막으로 접속했던 서버가 어디인가?
	//--------------------------------------
	memset(szLastConnectServerName,0,20);

	//--------------------------------
	// 게임 약관. 
	//--------------------------------
//	pclGameContract	= NULL;

	//--------------------------------------
	// 선택된 월드 초기화.
	//--------------------------------------
	StringCchCopy(clSelectedWorldInfo.baseinfo.worldname, 20, TEXT(""));

	//--------------------------------
	// 화면캡쳐 
	//--------------------------------
	pclCapture	 = NULL;

	//--------------------------------
	// 친구 로그인
	//--------------------------------
	m_pFriendLoginDraw = NULL;

	//--------------------------------
	// 마을 전쟁 상황 알림 메니져
	//--------------------------------
	m_pVillageWarNotice = NULL;

	//--------------------------------
	// 주민 로그인
	//--------------------------------
	m_pResidentLoginDraw = NULL;

	//pclMiniHomeUpdates = NULL;

	//-------------------------------------
	// 한게임 PICA매니져 체크
	//-------------------------------------
	m_pclPICAManagerCheck = NULL;

	//-------------------------------------
	// 웹페이지 호출 체크
	//-------------------------------------
	m_pPopupCheck		 = NULL;

	//---------------------------
	// 주민등록번호 뒷자리 입력창
	//----------------------------
	m_pResidenceCheck = NULL ;

//	m_pHtmlDlgManager = NULL ;

	//-----------------------------
	// 지식검색 
	//-----------------------------
	//m_pSearchDetailDlg = NULL;


	//----------------------------------------
	// 클라이언트 영역 크기. 
	//----------------------------------------
	siClientScreenXsize	= 0;
	siClientScreenYsize	= 0;
	siClientScreenHalfSize	= 0;

	// 화면에 찍을 지도를 임시로 저장하는 곳 
	lpScreenMapBuffer = NULL;
	GameMouseX= 10;
	GameMouseY= 10;

	//----------------------------------------
	// 관전 모드 여부 
	//----------------------------------------
	bWatchModeSwitch	= false;
	siWatchModeDetailX	= 0;
	siWatchModeDetailY	= 0;

	//----------------------------------------
	// 작업 중지 여부 스위치(광산, 농장)
	//----------------------------------------
	bStopDoingSwitch	= false;

	bPassFirstOrder		= false;	// 처음 들어왔을시에는 아무런 오더 받지 않도록 수정

	//-------------------------------
	// 불가사리 사냥 이벤트. 
	//-------------------------------
	bHuntingEventSwitch	= false;
	//-------------------------------
	// 모후모후 던젼 이벤트 진행 국가. 
	//-------------------------------
	siMofuMofuEvent_VilliageUnique = 0;
	//------------------------------
	// 고구려 이벤트 
	//------------------------------
	bGoguEventSwitch	= false;
	//------------------------------
	// 오사카성 이벤트 
	//------------------------------
	bOsakaEventSwitch	= false;
	//------------------------------
	// 강화도 이벤트 
	//------------------------------
	bKanghwadoEventSwitch = false;
	//-------------------------------
	// 서버와의 연결이 단절되었을때의 스위치 by cyj
	//-------------------------------
	bServerNotConnectSwitch = false;
	//-------------------------------
	// 부활 메세지 수신 여부 스위치 by cyj
	//-------------------------------
	bReviveRequestSwitch = false;

	//-------------------------------
	// 길드전
	//-------------------------------
	pclGuildWarClient			=	NULL;

	//-------------------------------
	// 배틀로얄
	//-------------------------------
	m_pclBattleRoyal			=	NULL;

	//-----------------------------
	// 도움말 
	//-----------------------------
	pclHelpManager				= NULL;

	// 로그인공지창
	bLoginNotice		= false ;

	//-----------------------------
	// 알림 메시지창
	//----------------------------

	//	m_DrawLogo = NULL
	//	m_DrawLogo = new CDrawLogo();

	//m_pCAbuseFilter= NULL;

	// 내자시 이펙트

	m_pFeastEffectManager = NULL;

	m_pBeginnerText = NULL;
	m_pLevelUpText  = NULL;
	m_pclGameTip	= NULL;

	m_pMarkManager = NULL;


	m_CCDLastCheckClock = 0;
	m_DeleteFOLastCheckClock = 0;
	m_SentHeartBeatClock = 0;

	// dhm-
	m_pClientDiffHellMan = new CGameClient;


    m_pItemPeriodMgr = NULL;
	m_pGawiBawiBoMgr = NULL;
	m_pClient_BBBMgr = NULL;

	m_pClient_PartyMatchingMgr = NULL;

	m_pPartyQuestMgr = NULL;
	
	//-----------------------------
	// InterfaceMgr
	//----------------------------
	m_pInterfaceMgr				= NULL;
	for ( i = 0; i < DIALOG_END; ++i )
	{
		m_pDialog[ i ] = NULL;
	}

	//--------------------------------
	// 게임 등급 표시
	//--------------------------------
	m_siDrawMode	= 0;
	m_kTimer_ShowRank_Interval.Init(60 * 60 * 1000);	// 1시간
	m_kTimer_ShowRank_Time.Init(3 * 1000);				// 3초

	m_kTimer_ShowRank_Interval.SetActive(true, GetTickCount());

	m_kTimer_Check_GameGuard_Interval.Init(10 * 1000);	// 10초

//	m_pPCRoomIntro	= NULL;

	LastSendEmoticonClock = 0;

	m_siSucceedEventType = 0;

	m_siGoStopPockerEventType = 0;

	uiTiredState = 0;

	// apex-
	m_bStartApex = false;
	// -apex

	// 매크로
	m_pclGZFtp					=	NULL;

	m_pclGMA						=	NULL;
	m_siGMA_ByAdminPersonID			=	0;
	m_bGMA_ReportedByAdminCommand	=	FALSE;

}


void cltClient::Create() 
{
	switch(gsiResolution)
	{
	case 2:	
		{
			siScreenMode = MODE_GRAPHICS1024_768;

			if ( g_bFullScreen )
			{
				siScreenModeXOffSet = 224;		//<-- 내부 화면창 및 풀 스크린
				siScreenModeYOffSet = 168;		//<-- 내부 화면창 및 풀 스크린
			}
			else
			{
				siScreenModeXOffSet = 160;		//<-- 내부 화면창
				siScreenModeYOffSet = 120;		//<-- 내부 화면창
			}
		}
		break;		
	case 3:
		{
			siScreenMode = MODE_GRAPHICS1280_1024;
			siScreenModeYOffSet = 0;
			siScreenModeXOffSet = 0;
		}
		break;
	// KHY - 1218 - 1024x600 해상도 추가. imsi
	case 4:	
		{
			siScreenMode = MODE_GRAPHICS1024_600;

			if ( g_bFullScreen )
			{
				siScreenModeXOffSet = 224;		//<-- 내부 화면창 및 풀 스크린
				siScreenModeYOffSet = 0;		//<-- 내부 화면창 및 풀 스크린
			}
			else
			{
				siScreenModeXOffSet = 160;		//<-- 내부 화면창
				siScreenModeYOffSet = -10;		//<-- 내부 화면창
			}
		}
		break;		
	default:
		{
			siScreenMode = MODE_GRAPHICS800_600;
			siScreenModeYOffSet = 0;
			siScreenModeXOffSet = 0;
		}
		break;
	}

	memcpy( strTextFontFileName, DEFAULT_FONTFILE_NAME, MAX_PATH - 1 );
	strTextFontFileName[ MAX_PATH - 1 ] = NULL;

	if ( AddFontResource( strTextFontFileName ) == 0 )
	{
	}
	else
	{
		SendMessage( GetHwnd(), WM_FONTCHANGE, 0, 0 );
	}


	if ( ! g_SoundOnOffDlg.m_bFullScreenMode || GameMode == GAMEMODE_MAPEDIT )
	{
		/*
		g_ChatStatusBar = new CChatStatusBar();
		g_ChatStatusBar->Create( GetHwnd(), GetInstance() );
		*/
	}

	//---------------------------
	// 툴바 및 상태창 생성. 
	//----------------------------
	if ( ! g_SoundOnOffDlg.m_bFullScreenMode || GameMode == GAMEMODE_MAPEDIT )
	{
		CreateToolBar();
	}

	CreateDirectDraw();

	// 윈도우 크기를 재조정한다. 
	// 툴바와 같은 것들이 추가되기 때문에 클라이언트 영역을 다시 설정해야 하기 때문이다.
	if( g_graphics.IsWindowed() )
	{
		RECT rect;
		if( GetClientSize(&rect) == TRUE )
		{
			AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, FALSE );

			//cyj 클라이언트 x, y좌표 변경
			SI32 xPos = 0, yPos = 0;
			xPos = GetSystemMetrics(SM_CXSCREEN) / 2;
			yPos = GetSystemMetrics(SM_CYSCREEN) / 2;
			xPos = xPos - (rect.right-rect.left) / 2;
			yPos = yPos - (rect.bottom-rect.top) / 2;
			// 비스타일경우 3픽셀 작게 반환해준다

			if( dwPlatformId == VER_PLATFORM_WIN32_NT )
			{
				// 비스타인가?
				if( dwMajorVersion == 6 && dwMinorVersion == 0 && wProductType == VER_NT_WORKSTATION )
				{
					rect.bottom -= 8;
					rect.right -= 8;
				}
				// 윈도우 7
				else if( dwMajorVersion == 6 && dwMinorVersion == 1 && wProductType == VER_NT_WORKSTATION )
				{
					rect.bottom -= 8;
					rect.right -= 8;
				}
			}

			MoveWindow( GetHwnd(),	xPos,  yPos, 
				rect.right-rect.left - 2, rect.bottom-rect.top - 2, TRUE ); 
		}
	}

	SetGameStatus(GS_INITCONNECTFRONTSERVER);

	//	g_graphics.ClearSurface(g_graphics.GetBackBuffer()); 

	g_ServerOnStepNotifyDlg.Show();

	cltCommonLogic::Create();
	//g_ServerOnStepNotifyDlg.LoadSpr();

#ifdef USE_HANAUTH

	// 한게임 인증 체크
	if(strlen(m_szHanAuth_GameString) <= 0)
	{
		TCHAR* pTitle = GetTxtFromMgr(3168);
		TCHAR* pText  = GetTxtFromMgr(7504);

		MsgBox( pTitle, pText);

		exit(0);
	}
#endif // USE_HANAUTH

#ifdef _JAPAN
#ifndef _DEBUG
	if( g_siChannelingCode != CHANNELINGCODE_NDEJP )
	{
		if( _tcslen(g_nm_id) <= 0 || _tcslen(g_nm_pw) <= 0 )
		{
			TCHAR* pTitle = GetTxtFromMgr(3168);
			TCHAR* pText  = GetTxtFromMgr(7504);
			TCHAR	szMsg[2048] = { '\0', };
			_tcscpy(szMsg, pText);

			FILE *fp = fopen("PatchInfo\\ExecWebMsg.txt", "r");
            if( fp != NULL )
			{
				fseek(fp, 0, SEEK_END);
				int size = ftell(fp);
				fseek(fp, 0, SEEK_SET);

				fread(szMsg, sizeof(TCHAR), size, fp);
				fclose(fp);
			}
			MessageBox(NULL, szMsg, pTitle, MB_OK);

			exit(0);
		}
	}

#endif
#endif

	// KHY - 0810 - 이모티콘 초기화 위치 수정
	clEmoticonInfo.Initialize();

	g_ServerOnStepNotifyDlg.Set( 46 );

	g_ServerOnStepNotifyDlg.Set( 47 );

	bWindowNotice	= false;

	g_ServerOnStepNotifyDlg.Set( 48 );	


	g_ServerOnStepNotifyDlg.Set( 50 );


	DrawInitBack();

	pclHeroDataBackup			= new cltHeroDataBackup;

	g_ServerOnStepNotifyDlg.Set( 51 );

	//--------------------------------------------
	// 레지스트리 관리 
	//--------------------------------------------
	//pclRegistry = new CRegistryMgr(siServiceArea);
	//if(pclRegistry)
	//{
	//	StringCchCopy(SavePath, 256, pclRegistry->GetPath());
	//}

	//--------------------------------------------
	// 실행위치 저장
	//--------------------------------------------
	char FilePathBuffer[ MAX_PATH ] = "";
	::GetModuleFileName( NULL, FilePathBuffer, sizeof( FilePathBuffer ) );

	char Drive[_MAX_DRIVE];
	char Path[_MAX_PATH];
	char Filename[_MAX_FNAME];
	char Ext[_MAX_EXT];

	::_splitpath( FilePathBuffer, Drive, Path, Filename, Ext );

	char PathBuffer[ MAX_PATH ] = "";
	strcpy( PathBuffer, Drive );
	strcat( PathBuffer, Path );
    
	StringCchCopy(SavePath, 256, PathBuffer);


	g_ServerOnStepNotifyDlg.Set( 53 );

	pclUnderMouseObject = new cltUnderMouseObject(this, pclCM);

	m_pNPrivateChatDlgMgr = new CNPrivateChatDlgMgr();


	// 윈도우 모드로 작동한다.
	//cyj SetWindowModeSwitch 사용안함
	//SetWindowModeSwitch(TRUE);

	//--------------------------------------------
	// 포커스 상태 관리
	//--------------------------------------------
	pclFocusManager = new CFocusManager();

	g_ServerOnStepNotifyDlg.Set( 54 );

	//-----------------------------------
	// 사운드 설정 
	//-----------------------------------
	pclMusic = new cltMusic(this);

	g_ServerOnStepNotifyDlg.Set( 55 );

	//-------------------------------------
	// 네트워크를 연결한다. 
	//-------------------------------------
	WinSockConnect();

	//-----------------------------------
	// 로고를 만든다. 
	//----------------------------------
	pclLogo = new cltLogo();

	//---------------------
	// 포그 관련
	//---------------------
	InitFogNode();

	//-----------------------------------
	// GlobalImgFile
	//-----------------------------------
	pclGImg	= new cltGlobalImgFile();

	//------------------------------------
	// PersonName 관리자. 
	//------------------------------------
	pclPersonNameManager    = new cltPersonNameManager( 500, 1000 );
	pclNewPersonNameManager = new cltNewPersonNameManager( 500, 1000 );
	
	//----------------------------------
	// 메시지 박스 
	//----------------------------------
	g_ErrorMsg.Create(GetInstance(), GetHwnd());
	g_LoginNoticeDlg.Create(GetInstance(),GetHwnd());

	// 명품 아이템 추가
	//m_pRareItemMsgDlg = new CRareItemMsgDlg();
	//m_pRareItemMsgDlg->Create(GetInstance(),GetHwnd());

	g_ServerOnStepNotifyDlg.Set( 56 );

	if ( !g_SoundOnOffDlg.m_bFullScreenMode )
	{
		// 주민등록번호 뒷자리 입력창
		m_pResidenceCheck = new CResidenceCheck();
		m_pResidenceCheck->Create(GetInstance(),GetHwnd());

		// 로딩비율 
		g_ServerOnStepNotifyDlg.Set( 57 );

		if(pclCM->CR[1])
		{
			g_ServerOnStepNotifyDlg.Set( 70 );
			
			g_ServerOnStepNotifyDlg.Set( 73 );

			// 쪽지 인터페이스 생성.
			//g_LetterDlg.Create( GetHwnd(), GetInstance() );
			
			// Sound On Off 설정하는 창
			g_SoundOnOffDlg.Create( GetInstance(), GetHwnd() );

			g_ServerOnStepNotifyDlg.Set( 74 );
		}
		else
		{
			MsgBox(TEXT("fds98jf"), TEXT("fds8j2"));
		}

		g_ServerOnStepNotifyDlg.Set( 75 );


	}

	//m_pSearchDetailDlg =  new CSearchDetailDlg();
	//m_pSearchDetailDlg->Create(GetInstance(),GetHwnd());

	m_pLoadingImg = new CDrawLoadingImg(GetHwnd(),siClientScreenXsize,siClientScreenYsize);

	g_ServerOnStepNotifyDlg.Set( 77 );


	g_ServerOnStepNotifyDlg.Set( 78 );


	g_ServerOnStepNotifyDlg.Set( 79 );

	if(pclCM->CR[1])
	{
		// 개인 거래 다이얼로그를 생성한다.

		g_ServerOnStepNotifyDlg.Set( 80 );

		g_ServerOnStepNotifyDlg.Set( 81 );

	}
	else
	{
		MsgBox(TEXT("fds89jf"), TEXT("fds8j3"));
	}

	g_ServerOnStepNotifyDlg.Set( 82 );

	//------------------------------------
	// 프롬프트. 
	//------------------------------------
	g_PromptMgr.Create();

	//-------------------------------------------
	// 판넬 
	//------------------------------------------
#ifndef _NEW_INTERFACE_
	pclPannel		= new cltPannel( 80, 60 );
#else
	pclPannel		= new cltPannel( 80, 40 );
#endif


	//-------------------------------------------
	// 내구도 이미지 표시
	//------------------------------------------
	pclDurability	= new CDurability( 80, 100 );

	
	//-------------------------------------------
	// 마을 안내문
	//------------------------------------------
	pclVillageNotice = new CVillageNotice();

	g_ServerOnStepNotifyDlg.Set( 85 );
	
	//----------------------------------------
	// 출력 순서 정렬 
	//----------------------------------------
	pclDrawSequence = new CDrawSequence();

	//-------------------------------------
	// 채팅 말풍선 관련. 
	//-------------------------------------
	pclDrawChatSurface = new CDrawChatSurface();

	//-------------------------------------
	// 캐릭터 이름 출력 관련. 
	//-------------------------------------
	pclDrawCharNameSurface = new CDrawCharNameSurface();


	g_ServerOnStepNotifyDlg.Set( 90 );

	//-------------------------------------
	//	매크로
	//-------------------------------------

	m_pclGZFtp			=	new cltGZFtp;

	//---------------------------
	// 도움말 
	//---------------------------
	pclHelpManager		= new cltHelpManager();

	//------------------------------------
	// 이동 표식. 
	//------------------------------------
	pclClickManager = new CClickEffect( pclClient->GetGlobalSpr(GIMG_CLICKPOINT) );

	//--------------------------------
	// 게임 약관. 
	//--------------------------------
//	pclGameContract = new cltGameContract();
	
	g_ServerOnStepNotifyDlg.Set( 100 );

	//--------------------------------
	// 화면캡쳐 
	//--------------------------------
	pclCapture	 = new CaptureJPG(GetHwnd());

	//--------------------------------
	// 친구 로그인
	//--------------------------------
	m_pFriendLoginDraw = new CFriendLoginDraw();
	
	//--------------------------------
	// 마을 전쟁 상황 알림 메니져
	//--------------------------------
	m_pVillageWarNotice = new CVillageWarNotice();


	//--------------------------------
	// 신년 이벤트 상황 알림 메니져
	//--------------------------------

	m_pNewYearEventNotice = new	CNewYearEventNotice();


	//--------------------------------
	// 주민 로그인
	//--------------------------------
	m_pResidentLoginDraw = new CResidentLoginDraw();

	//--------------------------------
	// 길드전
	//--------------------------------
	pclGuildWarClient = new cltGuildWarClient();


	//--------------------------------
	// 꼼지 새로 올라온 글 
	//--------------------------------
	//pclMiniHomeUpdates = new cltMiniHomeUpdates();
	//-------------------------------------
	// 한게임 PICA매니져 체크
	//-------------------------------------
	m_pclPICAManagerCheck = new cltPICAManagerCheck();

	//-------------------------------------
	// [진성] 팝업창 띄우기 체크. 2008-5-27
	//-------------------------------------
	//if( pclClient->IsCountrySwitch(Switch_PopupCheck) )
	//{
	//	m_pPopupCheck = new cltPopupCheck;
	//	m_kUpdateTimer_PopupCheck.Init(30 * 60 * 1000);	// 30분 마다 체크
	//}

	g_ServerOnStepNotifyDlg.Set( 105 );

	//m_pCAbuseFilter = new CAbuseFilter();
	//m_pCAbuseFilter->Create( TEXT("GImg\\mirror1.txt"), TEXT("GImg\\mirror2.txt") );

	// 내자시 이펙트
	m_pFeastEffectManager = new CFeastEffectManager;

	// 초보자 도움말
	if( ConstServiceArea_China == siServiceArea )
	{
		m_pBeginnerText = new CBeginnerText;
		m_pBeginnerText->LoadBeginnerText( TEXT("TextMgr\\BeginnerGuide.txt") );
	}

	// 국가 스위치 추가 - By LEEKH 2007.09.19
	if(pclClient->IsCountrySwitch(Switch_LevelupUpText))
	{
		m_pLevelUpText = new CLevelUpText();
	}

	if(pclClient->IsCountrySwitch(Switch_GameTip))
	{
		m_pclGameTip = new CGameTip;
	}

	m_pMarkManager = new cltMarkManager();
	m_pMarkManager->LoadMarkFromFile();

	NPCMgr();

	m_pclCheckProcess = new cltCheckProcess;


	m_pItemPeriodMgr = new CItemPeriodMgr;
	bool bResult = m_pItemPeriodMgr->Initialize();

	if(bResult == false)
		MsgBox("Error", "Cannot Loaded to [ItemPeriod_Info.txt]");

	//  [8/11/2009 종호_GAWIBAWIBO]
	if(pclClient->IsCountrySwitch(Switch_GawiBawiBo) == true)
		m_pGawiBawiBoMgr = new CGawiBawiBoMgr_Client;

	// [지연] - 복불복 클라이언트 매니저
	if ( pclClient->IsCountrySwitch(Switch_BokBulBok) == true )
	{
		m_pClient_BBBMgr = new CClient_BBBMgr();
	}

	// [기형] - 배틀로얄
	if ( pclClient->IsCountrySwitch(Switch_BattleRoyal) == true )
	{
		m_pclBattleRoyal = new CBattleRoyalClient();
	}

	// [기형] - 발렌타인데이이벤트
	if ( pclClient->IsCountrySwitch(Switch_ValentineDayEvent) == true )
	{
		m_pclValentineEventMgr = new CValentineEventClientMgr();
		if( NULL != m_pclValentineEventMgr)
			m_pclValentineEventMgr->Create();
	}
	// [지연] - 초보자 파티 퀘스트 시스템 (파티매칭)
	if ( pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true )
	{
		m_pClient_PartyMatchingMgr = new CClient_PartyMatchingMgr();
	}


	// [종호] 파티퀘스트
	if ( pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true )
	{
		m_pPartyQuestMgr = new CPartyQuestMgr_Client();
		m_pPartyQuestMgr->Initialize();
	}

	

	//---------------------------------------
	// Interface Mgr
	//-------------------------------------
	m_pInterfaceMgr = new CInterfaceMgr();
	switch(gsiResolution)
	{
	case 2:		// 1024 X 768     
		{				
			m_pInterfaceMgr->SetAvailableScreenRect( 80, 40, 
				1040 +(g_SoundOnOffDlg.m_bMaxWindowMode ? 64 : 0), 
				770 +(g_SoundOnOffDlg.m_bMaxWindowMode ? 40 : 0) ); //  전체화면일경우 좌표 더해줌
			
		}
		break;		
	case 3:		// 1280 X 1024
		{

		}
		break;
	default:	// 800 X 600
		{
			m_pInterfaceMgr->SetAvailableScreenRect( 80, 67, 880, 590 );
			
		}
		break;
	}

	g_FontManager.InstallCustomFont(GetHwnd());

	// 타일 이미지 출력 속도 향상을 위한 마스트를 만든다. 
	GetTileImgMask();

	CreateBaseInterface();

	g_ServerOnStepNotifyDlg.Set( 105 );

	//	if( !g_bFullScreen ) {
	ShowWindow( GetHwnd(), SW_SHOW );
	UpdateWindow( GetHwnd() );
	SetFocus( GetHwnd() );

	((cltClient *)pclClient)->GameDrawUpdateScreen();

#ifndef _DEBUG
	
	if(pclClient->GameMode == GAMEMODE_CLIENT)
	{
		// 일본판일 경우만 OS체크
		if( pclClient->siMaster == 0 && pclClient->siServiceArea == ConstServiceArea_Japan )
		{
			if( IsCorrectOS() == FALSE )
			{
				TCHAR buf[256];
	
				switch(pclClient->siServiceArea)
				{
				case ConstServiceArea_Korea :
					StringCchPrintf(buf, 256, TEXT("Korea use only"));
					break;
				case ConstServiceArea_Japan :
					StringCchPrintf(buf, 256, TEXT("Japan use only"));
					break;
				case ConstServiceArea_China :
					StringCchPrintf(buf, 256, TEXT("China use only"));
					break;
				case ConstServiceArea_Taiwan :
					StringCchPrintf(buf, 256, TEXT("Taiwan use only"));
					break;
				case ConstServiceArea_USA  :
					StringCchPrintf(buf, 256, TEXT("USA use only"));
					break;

				//case ConstServiceArea_English :
				//	StringCchPrintf(buf, 256, TEXT("This version is not accessible if you are connecting from Korea, Japan or using Korean, Japanese version Windows"));
				//	break;
				}

				MsgBox( TEXT("Error"), buf, MB_OK);
				dwQuitTimer = 1;
			}
		}
	}	
#endif 

#ifdef USE_GAME_GUARD_CLIENT

//	pNpgl->SetHwnd( GetHwnd() );

//	pclLog->FilePrint( "config\\nProtectLog.log" , "SetHwnd" );

#endif
	//	}
}

// 인터페이스 Create 부분
void cltClient::CreateBaseInterface()
{	
	for ( SI16 i = 0; i < DIALOG_END; ++i )
	{
		switch( i )
		{
		case NMSGBOX_DLG:
		case NMSGBOX0_DLG:
		case NMSGBOX1_DLG:
		case NMSGBOX2_DLG:
		case NMSGBOX3_DLG:
		case NMSGBOX4_DLG:
		case NMSGBOX5_DLG:
		case NMSGBOX6_DLG:
		case NMSGBOX7_DLG:
		case NMSGBOX8_DLG:
		case NMSGBOX9_DLG:
		case NPRIVATETRADEACCEPT_DLG:
		case NPRIVATETRADEACCEPT_MSGBOX_DLG:
		case TEST_DLG:
		case NMONSTERINFO_DLG:
		case NEXIT_DLG:
		case NEXITCONFIRM_DLG:
		case NMASTERINFO_DLG:
		case NVILLAGEDETAILINFO_DLG:		
		case NBOBUSANG_DLG:
		case NSELECTNOLIMITWARPTICKET_DLG:
		case CLTNCHARSTATUS_DLG:
		case CLTNCHARNAMECHANGE_DLG:
		case CLTNCHARGREETINGCHANGE_DLG:
		case CLTNCHARKINDCHANGE_DLG:
		case CLTNHORSETYPECHANGE_DLG:
		case NNOTICEADMIN_DLG:
			//by tae-yang
		case NDEATH_DLG:
		case QUESTVIEWER_DLG:
		case NEVENTNOTIC_DLG:
		case NLETTER_DLG:
		case NPERSONALSHOP_POP_DLG:
		case NFARMING_DLG:
		case NRAREITEM_DLG:
		case NAUTOHUNT_DLG:
		case NBOSSMARKET_DLG:
		case NFOREIGNMERCHANT_DLG:
		case NPERSONMALINFO_DLG:
		case NNPCREPAIR_DLG:
		case NTRADEMERCHANTHORSE_DLG:
		case NNPCMENU_DLG:
		case NFOREIGNMERCHANTBUYITEM_DLG:
		case NFOREIGNMERCHANTBUYCASHITEM_DLG:
		case NTRADEMERCHANT_DLG:
		case NTRADEMERCHANTHIGHCLASS_DLG:
		case NNPCTRADE1_DLG:
		case NWENNYMAN_DLG:	
		case NPERSONALSHOP_DLG:
		case NPERSONALSHOPSEARCH_DLG:
		case NHOZOBASE_DLG:
		case NBYUNGZO_DLG:
		case NVOTEKING_DLG:
		case NDECREEPASSWD_DLG:		
		case NSEIGEWIN_DLG:		
		case NMAKEITEM_DLG:
		case NVILLAGEWARINFO_DLG:
		case NIZOBASE_DLG:
		case NPRIVATETRADE_DLG:
		case NPTRADEREPRAIR_DLG:
		case NCANDIDATE_DLG:
		case NINVESTMENT_DLG:
		case NGONGZO_DLG:
		case NGOONZU_DLG:
		case NYEZOBASE_DLG:
		case NHYUNGZOBASE_DLG:
		case NMUDANG_DLG:
		case NCHILD_DLG:
		case NFATHER_DLG:
		case NSKILL_DLG:
		case NPRIVATECHAT0_DLG:
		case NPRIVATECHAT1_DLG:
		case NPRIVATECHAT2_DLG:
		case NPRIVATECHAT3_DLG:
		case NPRIVATECHAT4_DLG:
		case NPRIVATECHAT5_DLG:
		case NPRIVATECHAT6_DLG:
		case NPRIVATECHAT7_DLG:
		case NPRIVATECHAT8_DLG:
		case NPRIVATECHAT9_DLG:
		case NPRIVATECHATREJECT_DLG:
		case NITEMMALLMANAGER_DLG: 
		case NHINT_DLG:
		case NITEMMALL_DLG:
		case NITEMMALL_GIVESTATUS_DLG:
		case NMENU_DLG:
		case NRIGHTMOUSEMENU_DLG:
		//case NMINIMAP_DLG:
		case NWEAPONSKILL_DLG:
		case NHONGGILDONG_DLG:
		case NWARJOIN_DLG:
		case NEVENT_DLG:
		case NPLAYMATCH_DLG:
		case NBLACKARMYMVP_DLG:
		case NSAVEITEMLIST_DLG:
		case NCITYHALL_DLG:
		case NBANK_DLG:
		case NSTOCK_DLG:
		case NMARKET_DLG:
		case NMARKET_SUB_DLG:
		case NHOUSE_DLG:
		case NPOSTOFFICE_DLG:
		case NHUNT_DLG:
		case NLAND_DLG:
		case NMINE_DLG:
		case NSUMMONMARKET_DLG:
		case NFEAST_DLG:
		case NHORSEMARKET_DLG:
		case NREALESTATEMARKET_DLG:
		case NSUMMONHEROMARKET_DLG:
		case NHIRENPC_DLG:
		case NCITYHALLCONSTRUCT_DLG:
		case NRESIDENTLIST_DLG:
		case NBERESIDENT_DLG:
		case NAPPOINTMENTCHIEF_DLG:
		case NSTRUCTUREMESSAGE_DLG:
		case NERRORMSGPRINT_DLG:
		case NERRORMSGPRINT1_DLG:
		case NNEWQUEST_DLG:
		case NSUMMONUNITBUYNPC_DLG:
		case NDAILYQUEST_DLG:
		case NNOTIFYGETEXPANDITEM_DLG:
		case NCITYHALLSUBMIT_DLG:
		case NSTOCK_DISTRIBUTION_DLG:
		case NVILLAGETREE_DLG:
			// 시전 정보
		case NHOUSEUSERINFOBASE_DLG:
		case NGUILDSTR_DLG:			// 길드상단
		case NGUILD_DLG:			// 길드
		case MONSTERCORPORATION_DLG:
		case ROULETTEGAME_DLG:
		case NGAMEMASTERORDER_DLG:
		case NCOMMONMSG_DLG:
		case NMAKECHAR_DLG:
		case NSYSCHILDMENU_DLG:
		case TOOLBAR_DLG:
		case NGENERALMEETINGLETTER_DLG:
		case NGENERALMEETING2_DLG:
		
		case NNEWITEMMAKE_DLG:
		case NNEWENCHANTOTHER_DLG:
		case NNEWITEMMALL_DLG:
		case NBUYWINDOW_DLG:
		case NPRESENTWINDOW_DLG:
		case NBUYITEMINFOWINDOW_DLG:	// 물건 사고 난 후의 정보창
		case NPRESENTINFOWINDOW_DLG:	// 선물 하고 난 후의 정보창

		case NACCOUNTBOOKBASE_DLG:
		case NNEWENCHANTOTHERACCEPT_DLG:
		case NNEWENCHANTOTHERACCEPT_MSGBOX_DLG:

		case NGOONZUPOLICY_DLG:
		case NGOONZUSIEGE_DLG:
		case NGETPRESENT_DLG:	// 선물 받음 알림창
		case NTHANKSPRESENT_DLG:
		case NSHOWGETITEM_DLG:
		case NTHANKSPRESENT_ANSWER_DLG:
		case NAQTRADEWINDOW_DLG:
		case NSETWORLDMONEYBUT_DLG:

		case NTRADESERVERBASE_DLG:
		case NTRADESERVERSELL_DLG:
		case NTRADESERVERBUY_DLG:
		case NRPMERCHANT_DLG:
		case NRPLIST_DLG:
		case NSETCOUNTRY_DLG:
		case NUSERLISTBYCOUNTRY_DLG:
		case NMANUFACTURE_DLG:
		case NRPBASE_DLG:
		case NDOWNLOADADDON_DLG:
		case NITEMMALLPRODUCT_DLG:
		case NPERIODITEMLIST_DLG:
		case NSELECTITEMCOUNT_DLG:
		case NOPENINGBANNER_DLG:
		case NLOWLEVEL_DLG:
		case NMYGUILDBASE_DLG:
		case NBEGUILDMEMBER_DLG:
		case NSELECTCHAR_DLG:
		case NGUILDINFO_DLG:
		case NTREASUREBOX_DLG:
		case NGUILDDUNGEON_DLG:
		case NGUILDDUNGEONAPPLY_DLG:
		case NGUILDMEMBER_DLG:
		case NGUILDSENDMSG_DLG:
		case NFRIENDMEMO_DLG:
		case NMANUFACTURENPC_DLG:
		case NFRIENDCHANGEGROUPNAME_DLG:
		case NBEGINNERWEAPONNPC_DLG:
		case NGUILDWARAPPLY_DLG:
		case NGUILDWARRECOMMAND_DLG:
		case NMAKEITEMDEMO_DLG:
		case NGACHA_DLG:
		case NGACHA2_DLG:							// [희영] //KHY - 1104 -  갓차2 -라쿤 보물 상자. 
		case NMOFUMOFULIST_DLG:
		case NAUTOANSWER_DLG:
		case NBUYSECONDWINDOW_DLG:
		case NNAMINGQUESTLIST_DLG:
		case NSELECTNAMING_DLG:
		case NHELPBUTTON_DLG:
		case NSYSTEMREWARDLIST_DLG:
		case NATTENDCHECK_DLG:
		case NHAPPYBEAN_DLG:
		case NQUESTSELECTITEM_DLG:
		case NINVITEEMPIRE_DLG:
		case NAUCTIONEVENT_DLG:
		case NEXCHANGESTOCK_DLG:
		case NCHRISTMASEVENT_DLG://크리스마스이벤트
		case NCHRISMASQUEST_DLG:					//[추가 : 황진성 2007. 11. 20 크리스마스 퀘스트.]
		case NNEWYEAR_DLG:							//[추가 : 황진성 2007. 12. 11 신년 이벤트 버튼창.]
		case CHARTRANSFORMINFO_DLG:					//[추가 : 황진성 2007. 12. 20 변신 정보창.]
		case CHARTRANSFORMINFO_STORY_DLG:			//[추가 : 황진성 2007. 12. 20 변신 관련 스토리창.]
		case CHARTRANSFORMINFO_STORY_READ_DLG:		//[추가 : 황진성 2007. 12. 20 변신 관련 스토리 읽기창.]
		case NINSTANST_DUNGEON_STATUS_DLG:			//[추가 : 황진성 2007.  1.  8 인스턴스던젼 상태창.]
		case NINSTANST_DUNGEON_RUN_INFO_DLG:		//[추가 : 황진성 2007.  1.  9 인스턴스던젼 진행정보 창.]
		case NINSTANTDUNGEONBOARD_DLG:				//인스턴트 던전 - 순위,명예의전당 2007.01.07 손성웅
		case NINSTANST_DUNGEON_NOTIPY:				// [추가 : 황진성 2008. 1. 18 => 인스턴트 던젼 관련 알림 창.]
		case NBLACK_WAR_RUN_DLG:					// [추가 : 황진성 2008. 1. 23 => 흑의 군단 개편 진행 상태 창.]
		case NBLACK_WAR_REWARD_DLG:					// [추가 : 황진성 2008. 1. 25 => 흑의 군단 개편 결과(보상) 창.]
		case NSPACE_BOX:							// [추가 : 황진성 2008. 1. 29 => 공간 상자 창]
		case ARTIFACT_BASE_DLG:						// [추가 : 황진성 2008. 2. 26 => 아티펙트 창.]
		case ARTIFACT_MIXTURE_DLG:					// [추가 : 황진성 2008. 2. 26 => 아티펙트 창.]
		case ARTIFACT_SLOT_CREATE_DLG:				// [추가 : 황진성 2008. 2. 26 => 아티펙트 창.]
		case ARTIFACT_REMOVE_DLG:					// [추가 : 황진성 2008. 2. 26 => 아티펙트 창.]
		
		case PARTYHALL_IN_BASE_DLG:					// [추가 : 황진성 2008. 3.  8 => 파티홀 창.]
		case PARTYHALL_IN_COSTUME_USER_DLG:				
		case PARTYHALL_IN_FIRECRAKER_CHIEF_DLG:				
		case PARTYHALL_IN_INVITE_CHIEF_DLG:	
		case NDAILYQUEST2_DLG:						// 데일리 퀘스트 2
		case ATTENDNOTICE_DLG:						// [영훈] 출석체크 : 출석체크 간단 알림창
		case ELECTIONSTUMP_DLG:						// [영훈] 선거시스템 : 선거유세창
		case HIDDENQUEST2_DLG:						// 히든 퀘스트일때 넘어오는 창.
		case PANNEL_DATEMONEY_DLG:					// [영훈] 내부 UI : Pannel(날짜와 돈)
		case PANNEL_CHARSTATUS_DLG:					// [영훈] 내부 UI : Pannel(케릭터 기본정보)
		case PANNEL_SUMMON_DLG:						// [영훈] 내부 UI : Pannel(소환수)
		case PANNEL_ORGAN_DLG:						// [영훈] 내부 UI : Pannel(내장기관)
		case CHARSTATUS_ABILITY_DETAIL_DLG:			// [진성] 캐릭터 정보창에서 상세 능력치 정보를 보여줄 창. 2008-3-24
		case NVOTESTATUS_DLG:						// [영훈] 선거 개표현황
		case NOPENBIGITEMBOX_DLG:					// [강형] 대박 아이템 개봉 창
		case NDAILYQUEST3_DLG:						// 데일리 퀘스트 2
		case NMONSTER_ATTACK_EVENT:					// [진성] 마을이벤트 - 몬스터 습격. => 2008-3-27
		case NGMINFO_DLG:							// [영훈] GM에게 필요한 정보 표시창
		case NCHARSUMMONRESET_DLG:					// [영훈] 수환수 강화 정보 및 초기화
		case NCHANGE_DLG:							// [ 성웅 ] 버그 및 사고시 사용될 버그수정 NPC 다이얼로그 
		case NSETCHAT_DLG:							// [ 성웅 ] 채팅창에서 보여질 메세지 셋팅 다이얼로그
		case NBUFF_ROULETTE_DLG:					// [진성] 영웅의 동상. => 2008-6-9
		case NMAPNAME_DLG:							// [영훈] 멥 이름 보여주기 (2008.06.17)
		case SERVER_EVENT_RANK_DLG:					// [진성] 서버 이벤트 점수 창. => 2008-6-24	
		case SERVER_EVENT_QUEST_DLG:				// [영훈] 서버 이벤트 퀘스트 상태창
		case SERVER_EVENT_INSTANCE_MAP_BONUS:		// [진성] 서버 이벤트 인던 종료 후 조건 만족시 나오는 보너스 창. => 2008-6-25	
		case SERVER_EVENT_INDUNINFO:				// [영훈] 서버 이벤트 보너스 인던 상황판
		case NTREASUREBOXEXTRA_DLG:					// [성웅] 2차 가차 주머니
		case NDESTROYITEM_DLG:						// [성웅] 아이템 드롭시 삭제 
		case NMARKETCONDITIONS_DLG: 				// [희영] 시세확인 2008-07-18
		case NPOSTOFFICE_GROUP_LIST_DLG:			// [진성] 단체 메시지 보내기. 유저 리스트 정보창. => 2008-8-4
		case NQUESTINFODLG_PARENT:					// [영훈] 퀘스트 알림창 변경 (부모창)
		case NQUESTNOTICE_DLG:						// [영훈] 퀘스트 공지창
		case NMYSUMMON_DLG:							// [성웅] 소환수 보관 시스템
		case NGACHANEW_DLG:							// [진성] 가차 개편.						    => 2008-8-29
		case NCHOISE_ITEM_GAME_START_DLG:			// [진성] 가차 개편. 아이템 선택 후 게임 시작.  => 2008-8-29
		case NOPENBOX:								// [진성] 가차 개편. 상자 열기 애니메이션 창.   => 2008-8-29
		case RECIEVED_ITEM_DLG:						// [진성] 가차 상자 아이템 받는 창 .			=> 2008-8-29
		//case NMYKEY:								// [진성] 나만의 메뉴 큰창.						=> 2008-9-3
		//case NMYKEYMINI:							// [진성] 나만의 메뉴 작은창.					=> 2008-9-3
		case NACUTRADEDLG_PARENT:					// [영훈] 아큐 거래창 리뉴얼
		case NAMBER_BOX_DLG:						// [진성] 호박상자								=> 2008-9-12
		case NCHARBUFF_ROULETTE_DLG:				// [강형] 캐릭터 버프 획득창					=> 2008-09-11
		case NYESORNODLG:							// [진성] 예, 아니오 창.						=> 2008-10-8
		case NHELPER_CHAT_DLG:						// [진성] 도우미 대화창.						=> 2008-10-13
		case NMAGICCOOLTIME_DLG:					// [영훈] 마법 쿨타임 표시창					=> 2008-10-15
		case NHELPER_QUESTION_OR_ANSWER_DLG:		// [진성] 도우미. 질문도 하고 답변도 하는 창.	=> 2008-11-04
		case NHELPER_QUESTION_LIST_DLG:				// [진성] 도우미. 질문 리스트 보는 창 .			=> 2008-11-04
		case NHELPER_ANSWER_COMPLETE_DLG:			// [진성] 도우미. 답변을 받는 창.				=> 2008-11-04
		case PVPLEAGUE_DLG:							// [영훈] PVP 무한대전 창						=> 2008-11-04
		case NEWYEAR_ROULETTE_DLG:					// [춘기] 신년운세룰렛 창						=> 2008-11-12
		case NELIXIR_STATUS_DLG:					// [진성] 엘렉서 스테이터스 설정 창.			=> 2008-12-04
		case NELIXIR_USE_DLG:						// [진성] 엘렉서 사용 창.						=> 2008-12-04
		case NELIXIR_NPC_DLG:						// [진성] 엘렉서 NPC 창.						=> 2008-12-04
		case NVALANTINE_DLG:						// [성웅] 발렌타인데이 잔혹한 질문창.			=> 2008-12-11
		case NEWSELECTBOX_DLG:						// [영훈] 새로운 선택상자(신년상자)				=> 2008-12-17
		case NQUESTINFO_DLG:						// [진성] 이전 퀘스트 정보창은 생성하지 않는다. 
		case NNEWMARKET_DLG:						// [진성] 통합마켓.								=> 2009-01-05
		case NNEWMARKET_SUB_DLG:					// [진성] 통합마켓.								=> 2009-01-05
		case NSUMMON_INVENTORY_DLG:					// [성웅] 가챠 인벤토리 소환수					=> 2009-02-19
		case NGUILDDUNGEON_NEW_DLG:					// [성웅] 길드 사냥터 소유방식 변경				=> 2009-03-24
		case NREQUESTHUNTMAPWARREQUEST:				// [성웅] 길드 사냥터 소유방식 변경-길드전신청다이얼로그=> 2009-03-24
		case NCARRYOVERGACHA_DLG:					// [영훈] 이월 가챠 다이얼로그					=> 2009-04-07
		case NGACHAPRODUCT_DLG:						// [영훈] 이월 가챠(아이템 선택)				=> 2009-04-08
		case NGACHARESULT_DLG:						// [영훈] 이월 가챠(결과창)						=> 2009-04-09
		case NGUILDWARRESULT_DLG:					// [종호] 사냥터획득 길드전 결과창
		case NGUILDWARKILLSIGN_DLG:					// [종호] 사냥터획득 길드전 전투중 킬뎃 현황
		case NWEDDINGCEREMONY_MENU_DLG:				// [영훈] 예식 메뉴 다이얼로그					=> 2009-04-21
		case NPROMOTER_INFO:		
		case NPROPOSE_QUESTION_DLG:					// [영훈] 결혼시스템 : 프로포즈 질문			=> 2009-04-23
		case NPROPOSE_ANSWER_DLG:					// [영훈] 결혼시스템 : 프로포즈 답변			=> 2009-04-28
		case NMARRIAGELOVELEVEL_DLG:				// [영훈] 결혼시스템 : 애정도 표시 다이얼로그	=> 2009-05-11
		case NWEDDINGANNIVERSARY_DLG:				// [영훈] 결혼시스템 : 결혼 기념일 다이얼로그	=> 2009-05-12
		case NWEDDINGCEREMONY_LIST_DLG:				// [영훈] 결혼시스템 : 예식 리스트 다이얼로그	=> 2009-05-18
		case NFAMILYINFO_DLG:						// [영훈] 가족 정보창							=> 2009-05-20
		case NMYFAMILYINFO_DLG_WITH_CHILDREN:		// [성웅] 가족 정보창							=> 2009-05-25
		case NBALANCECURRENCY_DLG:					// [종호] 통화량 조절 룰렛 버프창
		case NBUTTONMENU_DLG	:					// [성웅] 가족시스템 버튼클릭
		case NFAMILY_DLG_ADD_CHILDREN	:			// [성웅] 가족시스템 자식추가다이얼로그
		case NFAMILYCOOKIELEVEL_DLG	:				// [성웅] 가족시스템 자식추가다이얼로그
		case PANNEL_SOULGUARD_DLG:					// [영훈] 수호정령 : 패널						=> 2009-06-10
		case CLTNSOULGUARDHOUSESTATUS_DLG:			// [영훈] 수호정령 보관소 상태정보창			=> 2009-06-16
		case NCHARSOULGUARDINFO_DLG:				// [영훈] 수호정령 강화 정보창					=> 2009-06-17
		case NPCROOM_DISPLAY_DLG:					// [영훈] PC방 연동 표시창						=> 2009-07-01
		case NPCROOM_DLG:							// [영훈] PC방 연동 보상목록창					=> 2009-07-01
		case NPROMOTER_GIVE_ITEM_NOTICE:			// [진성] 프로모터 보상 관련 창. 
		case NPROMOTER_MINI:						// [진성] 프로모터 정보창을 띄워주는 작은 버튼. 
		case NPROMOTER_INTRODUCE:					// [진성] 프로모터 새내기 인사창.			
		case NRACOONDODGE_EVENT_DLG:				// [성웅] 라쿤 피하기 이벤트  
		case NRACOONDODGE_EVENT_RESERVELIST_DLG:	// [성웅] 라쿤 피하기 이벤트  예약 리스트 
		case NBOKBULBOK_EVENT_DLG:					// [지연] 복불복 이벤트 시작 창.
		case NBOKBULBOK_RESERVELIST_DLG:			// [지연] 복불복 대기리스트 등록 창.
		//case NBOKBULBOK_RESERVE_CANCEL_DLG:			// [지연] 복불복 대기자 리스트에서 삭제 요청 하는 창.
		case NDORMANCYARMOURBOX_DLG:				// [기형] 휴면 계정 시스템	
		case NDORMANCYWEAPONBOX_DLG:				// [기형] 휴면 계정 시스템
		case NDORMANCYPOINT_DLG:					// [기형] 휴면 계정 시스템
		case NGAWIBAWIBO_DLG:						// [종호] 가위바위보
		case NITEMREPAIR_DLG:						// [강형]
		case NNEWNPCREPAIR_DLG:						// [강형]
		case NAVATARQUEST_DLG:						// [종호] 아바타퀘스트
		case NWARNDURABILITY_DLG:					// [영훈] 내구도 경고 표시창
		case NHALLOWEENSPIRITBOX_DLG:				// [종호] 할로윈 스피릿 dlg
		case NMAINMAP_DLG:							// [지연] 야외지역 전체맵 메인창
		case NMONSTERCREATE_DLG:					// [춘기] 몬스터 생성 창
		case NEQUIPMENTCREATE_DLG:					// [춘기] 장비 생성 창
		case NCHARHOUSE_DLG:						// [영훈] 캐릭터 보관소
		case NCHANGECHARNAME_DLG:					// [종호] 캐릭터 이름 변경 dlg
		case NCOPFIELD_DLG:							// [종호] 캐릭터 이름 변경 dlg
		case NRACCOONEVENT_CHANGE_GOONZUEVENT_DLG:	// [성웅] 라쿤이벤트 군주 이벤트로 변경
		case NBATTLEROYALRESULT1_DLG:				// [기형] 배틀로얄 경기 결과 
		case NBATTLEROYALRESULT2_DLG:				// [기형] 배틀로얄 경기 결과(예비) 
		case NBATTLEROYALTIME_DLG:					// [기형] 배틀로얄 - 로이( 시작까지 남은 시간)
		case NBATTLEROYALMOVE_DLG:					// [기형] 배틀로얄 - 경기장 이동
		case N2010NEWYEAR_FORTUNE_DLG:				// [지연] 2010신년이벤트 - 포춘쿠키 운세보여지는 창.
		case NSELECT_QUEST_BASE_DLG:				// [지연] 초보자 파티 퀘스트 시스템 - 레벨별 퀘스트파티 선택 창. (파티매칭)
		case NPARTY_MATCHING_BASE_DLG:				// [지연] 초보자 파티 퀘스트 시스템 - 조회된 파티리스트에서 들어갈 파티 선택하는 창.
		case NCREATE_PARTY_BASE_DLG:				// [지연] 초보자 파티 퀘스트 시스템 - 파티 생성 창.
		case NPARTY_JOIN_DLG:						// [지연] 초보자 파티 퀘스트 시스템 - 파티가입 수락/거절 질문 대화상자
		case NHOUSEEXTENSION_DLG:					// [기형] 창고 기간 연장
		case NHOUSENAMECHANGE_DLG:					// [기형] 창고 이름 변경
		case NGIANTRACCOONRESURRECTEVENTDLG:		// [진성] 기부 관련 공통창. (사용하는 클레스: 자이언트 라쿤 부활 이벤트,)
		case NPVPMAPGODLG:							// [성웅] PVP 맵 이동 npc 다이얼로그
		case NPOTRACCOONDLG:						// [진성] 자이언트 라쿤 부활 이벤트 부활빵 기부창.
		case NMANUFACTURE_NEW_DLG:					// [성웅] 제조의뢰 인터페이스 개선
		case MANUFACTURESEARCH_DLG:					// [성웅] 제조의뢰 인터페이스 개선
		case VALENTINEDAY_NPC_DLG:					// [진성] 발렌타인 데이 NPC
		case VALENTINEDAY_PACKING_DLG:				// [진성] 발렌타인 데이 포장하기
		case VALENTINEDAY_TRADE_DLG:				// [진성] 발렌타인 데이 구매하기
		case VALENTINEDAY_CONFESSION_QUESTION_DLG:	// [진성] 발렌타인데이 고백	하기.
		case VALENTINEDAY_CONFESSION_ANSWER_DLG	:	// [진성] 발렌타인데이 고백	하기.
			
			
			
			
			continue;
		}
		CreateInterface( i, true );
	}

//	m_pPCRoomIntro = new cltPCRoomIntro;

	return;
}

void cltClient::CreateInterface( SI16 DialogID, bool ShowHideMethod )
{
#define CREATE_INTERFACE(DialogPtr, ClassType, ShowHideMethodSelect)	\
	{\
	if( DialogPtr == NULL )\
	{\
	DialogPtr = new ClassType();\
	}\
	DialogPtr->Create();\
	DialogPtr->SetShowHideMethod(ShowHideMethodSelect);\
	}

	switch( DialogID )
	{
	case NMSGBOX_DLG:
	case NMSGBOX0_DLG:
	case NMSGBOX1_DLG:
	case NMSGBOX2_DLG:
	case NMSGBOX3_DLG:
	case NMSGBOX4_DLG:
	case NMSGBOX5_DLG:
	case NMSGBOX6_DLG:
	case NMSGBOX7_DLG:
	case NMSGBOX8_DLG:
	case NMSGBOX9_DLG:
	case NPRIVATETRADEACCEPT_MSGBOX_DLG:
		{
			CREATE_INTERFACE( m_pDialog[ DialogID ], CNMsgBoxDlg, ShowHideMethod );
		}
		break;
	case NPRIVATETRADEACCEPT_DLG:
		{
			CREATE_INTERFACE( m_pDialog[DialogID], CNPrivateTradeAcceptDlg, ShowHideMethod );
		}
		break;

	case NCANDIDATE_DLG:
		{
			CREATE_INTERFACE( m_pDialog[ DialogID ], CNCandidateDlg, ShowHideMethod );
		}
		break;
	case TOOLBAR_DLG:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{				
				CREATE_INTERFACE( m_pDialog[ DialogID ], CToolBarDlg, ShowHideMethod );
				m_pDialog[ DialogID ]->Show( false );
			}
		}
		break;
	case NCHARMENUBAR_DLG:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{				
				CREATE_INTERFACE( m_pDialog[ DialogID ], CNCharMenuBarDlg, ShowHideMethod );
				m_pDialog[ DialogID ]->Show( false );
			}
		}
		break;
	//case NSYSMENUBAR_DLG:
	//	{
	//		if ( g_SoundOnOffDlg.m_bFullScreenMode )
	//		{				
	//			CREATE_INTERFACE( m_pDialog[ DialogID ], CNSysMenuBarDlg, ShowHideMethod );
	//			m_pDialog[ DialogID ]->Show( false );
	//		}
	//	}
	//	break;
	case NCHATBAR_DLG:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{				
				CREATE_INTERFACE( m_pDialog[ DialogID ], CNChatBarDlg, ShowHideMethod );
				m_pDialog[ DialogID ]->Show( false );
			}
		}
		break;	
	case STATUSBAR_DLG:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{
				CREATE_INTERFACE( m_pDialog[ DialogID ], CNStatusBarDlg, ShowHideMethod );
				m_pDialog[ DialogID ]->Show( false );
			}
		}
		break;
	case STATUSBAR_POPUPPARENT_DLG:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{
				CREATE_INTERFACE( m_pDialog[ DialogID ], CNStatusBarPopupParentDlg, ShowHideMethod );
				m_pDialog[ DialogID ]->Show( false );
			}
		}
		break;
	case STATUSBAR_POPUPCHILDSYSTEM_DLG:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{
				CREATE_INTERFACE( m_pDialog[ DialogID ], CNStatusBarPopupChildSystemDlg, ShowHideMethod );
				m_pDialog[ DialogID ]->Show( false );
			}
		}
		break;
	case STATUSBAR_POPUPCHILDCOMMUNITY_DLG:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{
				CREATE_INTERFACE( m_pDialog[ DialogID ], CNStatusBarPopupChildCommunityDlg, ShowHideMethod );
				m_pDialog[ DialogID ]->Show( false );
			}
		}
		break;
	case NCHAT_DLG:
		{
			//CREATE_INTERFACE( m_pDialog[ DialogID ], CNChatDlg, ShowHideMethod );
			//m_pDialog[ DialogID ]->Show( false );
		}
		break;
	case NRESIDENTS_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNResidentsDlg, ShowHideMethod );		break;	
	case TEST_DLG:					CREATE_INTERFACE( m_pDialog[ DialogID ], CTestDlg, ShowHideMethod );			break;
	case NOTIFYMSG_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], CNotifyMsgDlg, ShowHideMethod );		break;
	case NOTIFYGETITEM_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNotifyGetItemDlg, ShowHideMethod );	break;			
	case MONSTERCORPORATION_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CMiniGameDlg, ShowHideMethod )			break;

	case NNEWINVENTORY_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNNewInventory, ShowHideMethod );		break;
	case NNEWINVENTORY_HORSE_DLG:
		{			
			CREATE_INTERFACE(m_pDialog[DialogID], CNNewInventory, ShowHideMethod ); 
			((CNNewInventory*)m_pDialog[DialogID])->SetInventoryMode(NNEWINVENTORY_HORSE_DLG);
		}
		break;
	case NNEWINVENTORY_BAG0_DLG:
		{			
			CREATE_INTERFACE(m_pDialog[DialogID], CNNewInventory, ShowHideMethod ); 
			((CNNewInventory*)m_pDialog[DialogID])->SetInventoryMode(NNEWINVENTORY_BAG0_DLG);
		}
		break;
	case NNEWINVENTORY_BAG1_DLG:
		{			
			CREATE_INTERFACE(m_pDialog[DialogID], CNNewInventory, ShowHideMethod ); 
			((CNNewInventory*)m_pDialog[DialogID])->SetInventoryMode(NNEWINVENTORY_BAG1_DLG);
		}
		break;
	case NNEWEQUIPWINDOW_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], 		CNEquipWindow, ShowHideMethod );			break;
	case NNEWITEMMAKE_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], 		CNewMakeItemDlg, ShowHideMethod  );			break;
	case NNEWENCHANTOTHERACCEPT_DLG: CREATE_INTERFACE( m_pDialog[ DialogID], 		CNNewEnchantOthereAccept, ShowHideMethod ); break;
	case NNEWENCHANTOTHERACCEPT_MSGBOX_DLG: CREATE_INTERFACE( m_pDialog[ DialogID], CNMsgBoxDlg, ShowHideMethod );				break;
	case NNEWENCHANTOTHER_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], 		CNNewEnchantOther, ShowHideMethod  );		break;
	case NQUESTINFO_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], 		cltNQuestStatus, ShowHideMethod );			break;
	
	//case NINVENTORY_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNInventory, ShowHideMethod );			break;

	case NEXIT_DLG:					CREATE_INTERFACE( m_pDialog[ DialogID ], CNExitDlg,					ShowHideMethod );	break;
	case NEXITCONFIRM_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNExitConfirmDlg,			ShowHideMethod );	break;
	case NITEMINFO_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], CNItemInfoDlg,				ShowHideMethod );	break;
	//case NHOMEPAGE_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], CNHomePageDlg, 		ShowHideMethod );	break;
	case NHUNTMAPINFO_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNHuntMapInfoDlg,			ShowHideMethod );	break;
	case NRESIDENTSINFO_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], CNResidentsInfoDlg,		ShowHideMethod );	break;
	case NMAGICINFO_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNMagicInfoDlg,			ShowHideMethod );	break;
	case NNEWMAGICINFO_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNNewMagicInfoDlg, 		ShowHideMethod );	break;
	case NFATHER_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], CNFatherDlg,				ShowHideMethod );	break;
	case NCHILD_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], CNChildDlg,				ShowHideMethod );	break;
	case NPARTY_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], CNPartyDlg,				ShowHideMethod );	break;
	case NPARTYINVITE_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNPartyInviteDlg,			ShowHideMethod );	break;	
	case NMONSTERINFO_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNMonsterInfoDlg,			ShowHideMethod );	break;
	case NITEMMALLMANAGER_DLG:      CREATE_INTERFACE( m_pDialog[ DialogID ], CNItemMallButtonManager,	ShowHideMethod );	break;
	case NVILLAGEDETAILINFO_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNVillageDetailInfo,		ShowHideMethod );	break;
//	case NFRIEND_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], CNFriendDlg,				ShowHideMethod );	break;
	case NNEWFRIEND_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNNewFriendDlg,			ShowHideMethod );	break;
	case NFRIENDACCEPT_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNFriendAddAcceptDlg,		ShowHideMethod );	break;
	case NOTICEEVENTMSG_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], CNoticeEvnetMsgDlg,		ShowHideMethod );	break;
	case NQUEST_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], cltNQuestDlg,				ShowHideMethod );	break;	    
	case NMASTERINFO_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNMasterInfo,				ShowHideMethod );	break;
	case CLTNCHARSTATUS_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], cltNCharStatus,			ShowHideMethod );	break;
	case CLTNCHARNAMECHANGE_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], cltNCharNameChange,		ShowHideMethod );	break;
	case CLTNCHARGREETINGCHANGE_DLG: CREATE_INTERFACE( m_pDialog[ DialogID ], cltNCharGreetingChange,	ShowHideMethod );	break;
	case CLTNCHARKINDCHANGE_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], cltNCharKindChange,		ShowHideMethod );	break;
	case CLTNHORSETYPECHANGE_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], cltNHorseTypeChange,		ShowHideMethod );	break;
	
	
	case NDEATH_DLG:		        CREATE_INTERFACE( m_pDialog[ DialogID ], CNDeathDlg, ShowHideMethod );			break;
	case QUESTVIEWER_DLG:		    CREATE_INTERFACE( m_pDialog[ DialogID ], CNQuestViewerDlg, ShowHideMethod );	break;
	
	case NMAKEITEM_DLG:	        	CREATE_INTERFACE( m_pDialog[ DialogID ], CNMakeItemDlg, ShowHideMethod );	    break;
	case NLETTER_DLG:            	CREATE_INTERFACE( m_pDialog[ DialogID ], CNLetterDlg, ShowHideMethod );	
		((CNLetterDlg*) m_pDialog[ DialogID ])->ShowMinimizeButton( true );
		break;
	case NAUTOHUNT_DLG:  	        CREATE_INTERFACE( m_pDialog[ DialogID ], CNcltAutoHuntingManager, ShowHideMethod );		break;
	case NVOTEKING_DLG:         	CREATE_INTERFACE( m_pDialog[ DialogID ], CNVoteDlg, ShowHideMethod);			break;
	case NNPCMENU_DLG:  	        CREATE_INTERFACE( m_pDialog[ DialogID ], CNNPCMenuDlg, ShowHideMethod);	    	break;
	case NNPCTRADE1_DLG:            CREATE_INTERFACE( m_pDialog[ DialogID ], CNNPCTradeDlg, ShowHideMethod);	    break;		
	case NEVENTNOTIC_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNEventNoticeDlg, ShowHideMethod );	break;
	case NWENNYMAN_DLG:	            CREATE_INTERFACE( m_pDialog[ DialogID ], CNWennyOldMan, ShowHideMethod );		break;	
	case NFARMING_DLG:    			CREATE_INTERFACE( m_pDialog[ DialogID ], CNFarmingDlg, ShowHideMethod );	    break;
	case NRAREITEM_DLG:			    CREATE_INTERFACE( m_pDialog[ DialogID ], CNRareItemDlg, ShowHideMethod );		break;
	//case NDECREEPASSWD_DLG:		    CREATE_INTERFACE( m_pDialog[ DialogID ], CNDecreePasswdDlg, ShowHideMethod  );  break;
	case NVILLAGEWARINFO_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], CNVillageWarInfoDlg, ShowHideMethod  );break;
	case NSEIGEWIN_DLG:		        CREATE_INTERFACE( m_pDialog[ DialogID ], CNSiegeWinDlg, ShowHideMethod  );      break;
	case NFISHING_DLG:		        CREATE_INTERFACE( m_pDialog[ DialogID ], CNFishingDlg, ShowHideMethod  );       break;
	case NINVESTMENT_DLG:		    CREATE_INTERFACE( m_pDialog[ DialogID ], CNInvestmentDlg, ShowHideMethod  );	break;
	case NPRIVATETRADE_DLG:		    CREATE_INTERFACE( m_pDialog[ DialogID ], CNPrivateTradeDlg, ShowHideMethod  );  break;
	case NPTRADEREPRAIR_DLG:	    CREATE_INTERFACE( m_pDialog[ DialogID ], CNPrivateRepairDlg, ShowHideMethod  ); break;
	case NITEMMALL_DLG:             CREATE_INTERFACE( m_pDialog[ DialogID ], CNItemMallManager1, ShowHideMethod  ); break;
	case NITEMMALL_GIVESTATUS_DLG:             CREATE_INTERFACE( m_pDialog[ DialogID ], CNItemMallGiveStatus, ShowHideMethod  ); break;
	case NHIRENPC_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], CNHireSystemNPCDlg, ShowHideMethod  ); break;
	case NHINT_DLG:                 CREATE_INTERFACE( m_pDialog[ DialogID ], NHintDlgExplain, ShowHideMethod  );    break;
	case NBOSSMARKET_DLG:		    CREATE_INTERFACE( m_pDialog[ DialogID ], CNBossMarketDlg, ShowHideMethod  );	break;
	case NPERSONMALINFO_DLG:	    CREATE_INTERFACE( m_pDialog[ DialogID ], CNPersonalInfoDlg, ShowHideMethod  );	break;
	case NPERSONALSHOP_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNPersonalShopDlg, ShowHideMethod  );	break;
	case NPERSONALSHOPSEARCH_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNPersonalShopSearchDlg, ShowHideMethod  );     break;
	case MANUFACTURESEARCH_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNManufactureSearchDlg, ShowHideMethod  );     break;		
	case NSOUNDONOFF_DLG:
		{
			if(pclCM->CR[1])
			{
				CREATE_INTERFACE( m_pDialog[ DialogID ], CNSoundOnOffDlg, ShowHideMethod );
			}
		}
		break;
	case NSELECTNOLIMITWARPTICKET_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], cltNSelectNoLimitWarpTicketDlg, ShowHideMethod );	break;
	case NBOBUSANG_DLG:					CREATE_INTERFACE( m_pDialog[ DialogID ], cltNBobusangDlg, ShowHideMethod );	break;
	case NSELECTWORLDLIST_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNSelectWorldListDlg, ShowHideMethod );			break;	
	case NLOGIN_DLG:					CREATE_INTERFACE( m_pDialog[ DialogID ], CNLoginDlg, ShowHideMethod );		break;
	case NSELECTCHAR_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], CNSelectCharDlg, ShowHideMethod );	break;
	case NMAKECHAR_DLG:					CREATE_INTERFACE( m_pDialog[ DialogID ], CNMakeCharDlg, ShowHideMethod );	break;


	case NNOTICEADMIN_DLG:	CREATE_INTERFACE(m_pDialog[ DialogID ] , cltNNoticeAdminDlg, ShowHideMethod );			break;
	case NRECOMMENDPERSON_DLG:	CREATE_INTERFACE(m_pDialog[ DialogID ] , CNRecommendPersonDlg, ShowHideMethod );	break;
	case NDELETECHAR_DLG:	CREATE_INTERFACE(m_pDialog[ DialogID ] , CNDeleteCharDlg, ShowHideMethod );				break;
	case NMINING_DLG:		CREATE_INTERFACE(m_pDialog[ DialogID ] , CNMiningDlg, ShowHideMethod );					break;
	case NNPCREPAIR_DLG:		CREATE_INTERFACE(m_pDialog[ DialogID ] , NcltNPCRepairManager, ShowHideMethod );		break;
	case NFOREIGNMERCHANT_DLG: CREATE_INTERFACE(m_pDialog[ DialogID ], CNForeignMerchantDlg, ShowHideMethod);		break;
	case NTRADEMERCHANTHORSE_DLG: CREATE_INTERFACE(m_pDialog[ DialogID ], CNTradeMerchantHorseDlg, ShowHideMethod );break;
	case NFOREIGNMERCHANTBUYITEM_DLG: CREATE_INTERFACE(m_pDialog[ DialogID ], CNForeignMerchantBuyItemDlg, ShowHideMethod);	break;
	case NFOREIGNMERCHANTBUYCASHITEM_DLG: CREATE_INTERFACE(m_pDialog[ DialogID ], CNForeignMerchantBuyCashItemDlg, ShowHideMethod);	break;
	case NTRADEMERCHANT_DLG: CREATE_INTERFACE(m_pDialog[ DialogID ], CNTradeMerchantDlg, ShowHideMethod );			break;
	case NTRADEMERCHANTHIGHCLASS_DLG: CREATE_INTERFACE(m_pDialog[ DialogID ], CNTradeMerchantHighClassDlg, ShowHideMethod );		break;
	case NHOZOBASE_DLG: CREATE_INTERFACE(m_pDialog[ DialogID ], CNHozoBaseDlg, ShowHideMethod );					break;
	case NBYUNGZO_DLG: CREATE_INTERFACE(m_pDialog[ DialogID ], CNByungzoDlg, ShowHideMethod );						break;
	case NIZOBASE_DLG: CREATE_INTERFACE(m_pDialog[ DialogID ], CNIzoBaseDlg, ShowHideMethod );						break;
	case NGONGZO_DLG:	CREATE_INTERFACE(m_pDialog[ DialogID ], CNGongzoDlg, ShowHideMethod );						break;
	case NGOONZU_DLG:	CREATE_INTERFACE(m_pDialog[ DialogID ], CNGoonzuDlg, ShowHideMethod );						break;
	case NYEZOBASE_DLG: CREATE_INTERFACE(m_pDialog[ DialogID ], CNYezoBaseDlg, ShowHideMethod );					break;
	case NHYUNGZOBASE_DLG: CREATE_INTERFACE(m_pDialog[ DialogID ], CNHyungzoBaseDlg, ShowHideMethod );				break;

	case NMUDANG_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNMudangDlg, ShowHideMethod );						break;
	
	case NNEWQUEST_DLG: CREATE_INTERFACE( m_pDialog[ DialogID ], CNNewQuestDlg, ShowHideMethod );				break;

	case NSKILL_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNSkillDlg, ShowHideMethod );						break;
	case NWEAPONSKILL_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNWeaponSkillDlg, ShowHideMethod );			break;
	case NHONGGILDONG_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNHongGilDongDlg, ShowHideMethod );			break;
	case NMENU_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], CNMenuDlg, ShowHideMethod );						break;
	
	case NSYSCHILDMENU_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNSysChildMenuDlg, ShowHideMethod );			break;
	case NPRIVATECHATREJECT_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNPrivateChatRejectDlg, ShowHideMethod );	break;	
	case NPRIVATECHAT0_DLG:
	case NPRIVATECHAT1_DLG:
	case NPRIVATECHAT2_DLG:
	case NPRIVATECHAT3_DLG:
	case NPRIVATECHAT4_DLG:
	case NPRIVATECHAT5_DLG:
	case NPRIVATECHAT6_DLG:
	case NPRIVATECHAT7_DLG:
	case NPRIVATECHAT8_DLG:
	case NPRIVATECHAT9_DLG:
		{	
			m_pDialog[ DialogID ] = new CNPrivateChatDlg();
			((CNPrivateChatDlg*)m_pDialog[ DialogID ])->Create( DialogID );
			m_pDialog[ DialogID ]->SetShowHideMethod( ShowHideMethod );
		}
		break;
	case NRIGHTMOUSEMENU_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNRightMouseMenuDlg, ShowHideMethod );		break;
	case NBUTTONMENU_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], CNButtonMenuDlg, ShowHideMethod );		break;
	case NMINIMAP_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNMiniMapDlg, ShowHideMethod );
	
		break;
	case NERRORMSGPRINT_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNErrorMsgPrint, ShowHideMethod );			break;
	case NERRORMSGPRINT1_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], CNErrorMsgPrint1, ShowHideMethod );			break;
	case NSUMMONUNITBUYNPC_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNSummonUnitBuyNpcDlg, ShowHideMethod );			break;
	case NDAILYQUEST_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNDailyQuestSelectDlg, ShowHideMethod );			break;
	case NNOTIFYGETEXPANDITEM_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNNotifyGetExpAndItemDlg, ShowHideMethod );			break;
	case NCITYHALLSUBMIT_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], NCityHallSubmitDlg, ShowHideMethod );			break;
	case NSTOCK_DISTRIBUTION_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNStockDistributionDlg, ShowHideMethod );			break;
	case NWARJOIN_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNWarJoinDlg, ShowHideMethod); break;
	case ROULETTEGAME_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CRouletteGameDlg, ShowHideMethod); break;
	case NSAVEITEMLIST_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNSaveItemListDlg, ShowHideMethod); break;

	case NPERSONALSHOP_POP_DLG: CREATE_INTERFACE( m_pDialog[DialogID],  CNLetterDlg, ShowHideMethod); break;		


		// 건물
	case NCITYHALL_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], NCityhallDlg, ShowHideMethod );			break;
	case NBANK_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], NBankDlg, ShowHideMethod );				break;
	case NSTOCK_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], NStockDlg, ShowHideMethod );				break;
	case NMARKET_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], NMarketDlg, ShowHideMethod );				break;
	case NMARKET_SUB_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], NMarketUserBuyDlgSub, ShowHideMethod );				break;
	case NHOUSE_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], NHouseDlg, ShowHideMethod );				break;
	case NPOSTOFFICE_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], NPostOfficeDlg, ShowHideMethod );			break;
	case NHUNT_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], NHuntDlg, ShowHideMethod );				break;
	case NLAND_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], NLandDlg, ShowHideMethod );				break;
	case NMINE_DLG:				CREATE_INTERFACE( m_pDialog[ DialogID ], NMineDlg, ShowHideMethod );				break;
	case NSUMMONMARKET_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], NSummonMarketDlg, ShowHideMethod );		break;
	case NSUMMONHEROMARKET_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], NSummonHeroMarketDlg, ShowHideMethod );	break;
	case NFEAST_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], NFeastDlg, ShowHideMethod );				break; 
	case NREALESTATEMARKET_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], NRealEstateMarketDlg, ShowHideMethod );	break;
	case NHORSEMARKET_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], NHorseMarketDlg, ShowHideMethod );break;
	case NGUILDSTR_DLG:			CREATE_INTERFACE( m_pDialog[ DialogID ], NGuildDlg, ShowHideMethod );	break;
	case NCITYHALLCONSTRUCT_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], NConstructDlg, ShowHideMethod );	break;
	case NRESIDENTLIST_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], NResidentListDlg, ShowHideMethod );		break;
	case NBERESIDENT_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], NBeResidentDlg, ShowHideMethod );		break;
	case NAPPOINTMENTCHIEF_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ],  NAppointmentChiefDlg, ShowHideMethod);		break;
	case NSTRUCTUREMESSAGE_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ],  NStructureMessageDlg, ShowHideMethod);		break;

	case NVILLAGETREE_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], NVillageTreeDlg, ShowHideMethod);			break;
	case NACCOUNTBOOKBASE_DLG: CREATE_INTERFACE( m_pDialog[ DialogID ], CNAccountBookBaseDlg, ShowHideMethod );	break;
		// 시전 상세 정보
	case NHOUSEUSERINFOBASE_DLG: CREATE_INTERFACE( m_pDialog[ DialogID ], CNHouseUserInfoBaseDlg, ShowHideMethod );		break;
	case NHOUSENAMECHANGE_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNHouseNameChangeDlg, ShowHideMethod );		break;
	case NHOUSEEXTENSION_DLG: CREATE_INTERFACE( m_pDialog[ DialogID ], CNHouseExtensionDlg, ShowHideMethod );		break;
	case NGAMEMASTERORDER_DLG : CREATE_INTERFACE( m_pDialog[ DialogID ], CNGameMaster, ShowHideMethod);	break;	// 운영자메뉴
	case NCOMMONMSG_DLG : CREATE_INTERFACE( m_pDialog[ DialogID ], CNCommonMsgDlg, ShowHideMethod);	break;	// 공통 메세지다이얼로그
	case NGENERALMEETINGLETTER_DLG : CREATE_INTERFACE( m_pDialog[ DialogID ], NGeneralMeetingLetterDlg, ShowHideMethod);	break;	
	case NGENERALMEETING2_DLG : CREATE_INTERFACE( m_pDialog[ DialogID ], NGeneralMeeting2Dlg, ShowHideMethod);	break;	
	case NPLAYMATCH_DLG : CREATE_INTERFACE( m_pDialog[ DialogID ], NPlayMatchDlg, ShowHideMethod);	break;	
	case NBLACKARMYMVP_DLG : CREATE_INTERFACE( m_pDialog[ DialogID ], NBlackArmyMVPDlg, ShowHideMethod);	break;	
	case NNEWITEMMALL_DLG : CREATE_INTERFACE( m_pDialog[DialogID], CNNewItemMallDlg, ShowHideMethod);	break;
	case NBUYWINDOW_DLG	: CREATE_INTERFACE( m_pDialog[DialogID], CNBuyWindowDlg, ShowHideMethod);	break;
	case NPRESENTWINDOW_DLG:		CREATE_INTERFACE( m_pDialog[ DialogID ], CNPresentWindowDlg , ShowHideMethod);	break;
	case NBUYITEMINFOWINDOW_DLG: CREATE_INTERFACE( m_pDialog[ DialogID ], CNNewItemMallBuyInfoDlg, ShowHideMethod );	break;
	case NPRESENTINFOWINDOW_DLG: CREATE_INTERFACE( m_pDialog[ DialogID ], CNPresentInfoWindowDlg , ShowHideMethod);	break;
	case NGOONZUPOLICY_DLG: CREATE_INTERFACE( m_pDialog[ DialogID ], CNGoonZuPolicyDlg , ShowHideMethod );	break;
	case NGOONZUSIEGE_DLG: CREATE_INTERFACE( m_pDialog[ DialogID ], CNGoonzuSiegeDlg , ShowHideMethod );	break;
	case NGETPRESENT_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNGetPresentDlg, ShowHideMethod ); break;
	case NTHANKSPRESENT_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNThanksPresentDlg, ShowHideMethod ); break;
	case NRIGHTMOUSEUSERLIST_DLG: 
		{
			CREATE_INTERFACE( m_pDialog[DialogID], NRightMouseUserListDlg, ShowHideMethod );
			m_pDialog[ DialogID ]->Show( false );
			break;
		}
	case NSHOWGETITEM_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNShowGetItemDlg, ShowHideMethod ); break;
	case NTHANKSPRESENT_ANSWER_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNThanksPresentDlg, ShowHideMethod ); break;
	case NAQTRADEWINDOW_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNAqTradeWindowDlg, ShowHideMethod ); break;
	case NSETWORLDMONEYBUT_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNSetWorldMoneyBuyDlg, ShowHideMethod ); break;
	case NINFO_DLG: CREATE_INTERFACE(m_pDialog[DialogID], cltNInfoDlg, ShowHideMethod ); break;
	case NINFO_SUB_DLG:
		{
			CREATE_INTERFACE(m_pDialog[DialogID], cltNInfoDlg, ShowHideMethod ); 
			m_pDialog[ DialogID]->SetDialogControllerID(NINFO_SUB_DLG);
		}
		break;
	case NTRADESERVERBASE_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNTradeServerBaseDlg, ShowHideMethod); break;
	case NTRADESERVERSELL_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNTradeServerSellDlg, ShowHideMethod); break;
	case NTRADESERVERBUY_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNTradeServerBuyDlg, ShowHideMethod); break;
	case NRPMERCHANT_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNRPMerchant,ShowHideMethod); break;
	case NRPLIST_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNRPList, ShowHideMethod); break;
	case NSETCOUNTRY_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNSetCountryDlg, ShowHideMethod); break;
	case NUSERLISTBYCOUNTRY_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNUserListByCountryDlg, ShowHideMethod);	break;
	case NMANUFACTURE_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNManufactureDlg,ShowHideMethod); break;
	case NRPBASE_DLG:	CREATE_INTERFACE(m_pDialog[DialogID], CNRPBase,ShowHideMethod); break;
	case NDOWNLOADADDON_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNDownloadAddonDlg,ShowHideMethod); break;
	case NITEMMALLPRODUCT_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNItemMallProductDlg,ShowHideMethod); break;
	case NPERIODITEMLIST_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNPeriodItemListDlg,ShowHideMethod); break;
	case NSELECTITEMCOUNT_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNSelectItemCountDlg, ShowHideMethod); break;
	case NOPENINGBANNER_DLG: CREATE_INTERFACE(m_pDialog[DialogID], CNOpeningBannerDlg, ShowHideMethod);	break;
	case NLOWLEVEL_DLG: CREATE_INTERFACE( m_pDialog[ DialogID ], CNLowLevelDlg, ShowHideMethod ); break;
	case NMYGUILDBASE_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNMyGuildBase,ShowHideMethod ); break;
	case NBEGUILDMEMBER_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], NBeGuildMemberDlg,ShowHideMethod ); break;
	case NGUILDINFO_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID ], CNGuildInfoDlg,ShowHideMethod ); break;
	case NTREASUREBOX_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID], CNTreasureBoxDlg, ShowHideMethod ); break;
	case NGUILDDUNGEON_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID], CNGuildDungeonDlg, ShowHideMethod );
	case NGUILDDUNGEONAPPLY_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID], CNGuildDungeonApplyDlg, ShowHideMethod );
	case NGUILDMEMBER_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID], CNGuildMemberDlg, ShowHideMethod); break;
	case NGUILDSENDMSG_DLG: CREATE_INTERFACE( m_pDialog[ DialogID], CNGuildSendMsgDlg, ShowHideMethod); break;
	case NEMOTICON_DLG: CREATE_INTERFACE( m_pDialog[ DialogID], CNEmoticonDlg, ShowHideMethod); break;
	case NFRIENDMEMO_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNFriendMemoDlg, ShowHideMethod); break;
	case NMANUFACTURENPC_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNManufactureNPCDlg, ShowHideMethod); break;
	case NBEGINNERWEAPONNPC_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNBeginnerWeaponNPCDlg, ShowHideMethod); break;
	case NGUILDWARAPPLY_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNGuildWarApplyDlg, ShowHideMethod); break;
	case NGUILDWARRECOMMAND_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNGuildWarRecommand, ShowHideMethod); break;
	case NFRIENDCHANGEGROUPNAME_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNFriendChangeGroupNameDlg, ShowHideMethod); break;
	case NMAKEITEMDEMO_DLG:	CREATE_INTERFACE( m_pDialog[DialogID], CNMakeItemDemoDlg, ShowHideMethod); break;
	case NGACHA_DLG : CREATE_INTERFACE( m_pDialog[DialogID], CNGachaDlg, ShowHideMethod); break;
	case NGACHA2_DLG : CREATE_INTERFACE( m_pDialog[DialogID], CNGacha2Dlg, ShowHideMethod); break;
	case NMOFUMOFULIST_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNMofuMofuListDlg, ShowHideMethod); break;
	case NAUTOANSWER_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNAutoAnswerDlg, ShowHideMethod ); break;
	case NBUYSECONDWINDOW_DLG	: CREATE_INTERFACE( m_pDialog[DialogID], CNBuySecondWindowDlg, ShowHideMethod);	break;
	case NNAMINGQUESTLIST_DLG:	CREATE_INTERFACE( m_pDialog[DialogID], CNNamingQuestListDlg, ShowHideMethod ); break;
	case NHELPBUTTON_DLG:	CREATE_INTERFACE( m_pDialog[DialogID], CNHelpButtonDlg, ShowHideMethod ); break;
	case NHAPPYBEAN_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNHappyBeanDlg, ShowHideMethod ); break;//해피빈이벤트[2007.08.22 손성웅]	
	case NAUCTIONEVENT_DLG:CREATE_INTERFACE( m_pDialog[DialogID], CNAuctionEventDlg, ShowHideMethod ); break;//옥션이벤트[ 2007.09.07 손성웅]
	case NSELECTNAMING_DLG:	CREATE_INTERFACE( m_pDialog[DialogID], CNSelectNamingDlg, ShowHideMethod ); break;

	case NSYSTEMREWARDLIST_DLG:	CREATE_INTERFACE( m_pDialog[DialogID], CNSystemRewardListDlg, ShowHideMethod); break;
	case NATTENDCHECK_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNAttendCheckDlg, ShowHideMethod ); break;
	case NQUESTSELECTITEM_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNQuestSelectItemDlg, ShowHideMethod ); break;
	case NNEWNOTIFY_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNNewNotifyDlg, ShowHideMethod ); break;
//	case NMUSIC_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNMusicDlg, ShowHideMethod ); break;
	case NCHRISTMASEVENT_DLG: CREATE_INTERFACE( m_pDialog[DialogID], NChristmasEventDlg, ShowHideMethod ); break;//크리스마스이벤트[2007.11.20 손성웅]	
	case NTREASUREBOXEXTRA_DLG:	CREATE_INTERFACE( m_pDialog[ DialogID], CNTreasureBoxExtraDlg, ShowHideMethod ); break;// [성웅] 2차 가차 주머니
	case NPREMIUMSTATUS_DLG: 
		{
			 CREATE_INTERFACE( m_pDialog[DialogID], CNPremiumStatus, ShowHideMethod );
			 m_pDialog[ DialogID ]->Show(false);						
		}
		break;

	case NBUFFSTATUS_DLG: //툴팁추가.[2007.07.12 성웅]
		{
			CREATE_INTERFACE( m_pDialog[DialogID],CNBuffstatus, ShowHideMethod );
			m_pDialog[ DialogID ]->Show(false);						
		}
		break;
	case NINVITEEMPIRE_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNInviteEmpireDlg, ShowHideMethod ); break;
	case NEXCHANGESTOCK_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNExchangStockDlg, ShowHideMethod ); break;
	
	//[추가 : 황진성 2007. 11. 20 크리스마스 퀘스트창.]
	case NCHRISMASQUEST_DLG: CREATE_INTERFACE( m_pDialog[DialogID], CNChristmasQuestDlg, ShowHideMethod ); break;
	
	//[추가 : 황진성 2007. 12. 11 신년 이벤트 버튼창.]
	case NNEWYEAR_DLG:		 CREATE_INTERFACE( m_pDialog[DialogID], CNNewYearDlg, ShowHideMethod ); break;

	//[추가 : 황진성 2007. 12. 20 변신 정보 창.]
	case CHARTRANSFORMINFO_DLG:		 CREATE_INTERFACE( m_pDialog[DialogID], CNChartransformDlg, ShowHideMethod ); break;
	
	//[추가 : 황진성 2007. 12. 20 변신 관련 스토리 창.]
	case CHARTRANSFORMINFO_STORY_DLG:		 CREATE_INTERFACE( m_pDialog[DialogID], CNChartransformStroyDlg, ShowHideMethod ); break;
		
	//[추가 : 황진성 2007. 12. 20 변신 관련 스토리 창.]
	case CHARTRANSFORMINFO_STORY_READ_DLG:		 CREATE_INTERFACE( m_pDialog[DialogID], CNChartransformStroyReadDlg , ShowHideMethod ); break;
	//인스턴트 던전 - 순위,명예의전당 2007.01.07 손성웅
	case NINSTANTDUNGEONBOARD_DLG:		 CREATE_INTERFACE( m_pDialog[DialogID], NInstantDungeonBoradDlg, ShowHideMethod ); break;
	
	//[추가 : 황진성 2008. 1. 8 인스턴트던젼  창.]
	case NINSTANST_DUNGEON_STATUS_DLG:			CREATE_INTERFACE( m_pDialog[DialogID], InstanstDungeonStatusDlg,			ShowHideMethod );	break;

	//[추가 : 황진성 2008. 1. 9 인스턴트던젼 진행 창.]
	case NINSTANST_DUNGEON_RUN_INFO_DLG:		CREATE_INTERFACE( m_pDialog[DialogID], InstanstDungeonRunInfoDlg,			ShowHideMethod );	break;

	//[추가 : 황진성 2008. 1. 9 인스턴트던젼 진행 창.]			
	case NINSTANST_DUNGEON_NOTIPY:				CREATE_INTERFACE( m_pDialog[DialogID], InstanstDungeonNotipyDlg,			ShowHideMethod );	break;

	// [추가 : 황진성 2008. 1. 23 => 흑의 군단 개편 진행 상태 창.]
	case NBLACK_WAR_RUN_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNBlackWarReformRun,					ShowHideMethod );	break;

	// [추가 : 황진성 2008. 1. 24 => 흑의 군단 개편 결과(보상) 창.]
	case NBLACK_WAR_REWARD_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNBlackWarReformReward,				ShowHideMethod );	break;
	
	// [추가 : 황진성 2008. 1. 29 => 공간 상자 창.]
	case NSPACE_BOX:							CREATE_INTERFACE( m_pDialog[DialogID], CNSpaceBoxDlg,						ShowHideMethod );	break;
		
	// [추가 : 황진성 2008. 2. 26 => 아티펙트 창.]
	case ARTIFACT_BASE_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], NArtifactBaseDlg,					ShowHideMethod );	break;
	case ARTIFACT_MIXTURE_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], NArtifactMixtureDlg,					ShowHideMethod );	break;
	case ARTIFACT_SLOT_CREATE_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], NArtifactSlotCreateDlg,				ShowHideMethod );	break;
	case ARTIFACT_RESTORE_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], NArtifactRestoreDlg,					ShowHideMethod );	break;
	case ARTIFACT_REMOVE_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], NArtifactRemoveDlg,					ShowHideMethod );	break;	

	// [추가 : 황진성 2008. 3.  8 => 파티홀 창.]
	case PARTYHALL_IN_BASE_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], NPartyHall_In_BaseDlg,				ShowHideMethod );	break;
	case PARTYHALL_IN_COSTUME_USER_DLG:			CREATE_INTERFACE( m_pDialog[DialogID], NPartyHall_In_Costume_User_Dlg,		ShowHideMethod );	break;		
	case PARTYHALL_IN_FIRECRAKER_CHIEF_DLG:		CREATE_INTERFACE( m_pDialog[DialogID], NPartyHall_In_Firecracker_Chief_Dlg,	ShowHideMethod );	break;		
	case PARTYHALL_IN_INVITE_CHIEF_DLG:			CREATE_INTERFACE( m_pDialog[DialogID], NPartyHall_In_Invite_Chief_Dlg,		ShowHideMethod );	break;
	case NDAILYQUEST2_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CDailyQuest2SelectDlg,				ShowHideMethod );	break;
	// [영훈] 출석체크 : 출석체크 알림창
	case ATTENDNOTICE_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNAttendNoticeDlg,					ShowHideMethod );	break;
	// [영훈] 선거시스템 : 선거유세창
	case ELECTIONSTUMP_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNElectionStumpDlg,					ShowHideMethod );	break;
	case HIDDENQUEST2_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], NHiddenQuestDlg,						ShowHideMethod );	break;
	// [영훈] 내부 UI : Pannel
	case PANNEL_DATEMONEY_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNPannel_DateMoneyDlg,				ShowHideMethod );	break;
	case PANNEL_CHARSTATUS_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNPannel_CharStatusDlg,				ShowHideMethod );	break;
	case PANNEL_SUMMON_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNPannel_SummonDlg,					ShowHideMethod );	break;
	case PANNEL_ORGAN_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNPannel_OrganDlg,					ShowHideMethod );	break;

	case CHARSTATUS_ABILITY_DETAIL_DLG:			CREATE_INTERFACE( m_pDialog[DialogID], NCharAbilityDatailDlg,				ShowHideMethod );	break;
	// [영훈] 선거 개표현황
	case NVOTESTATUS_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNVoteStatusDlg,						ShowHideMethod );	break;
	case NOPENBIGITEMBOX_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNOpenBigItemBoxDlg,					ShowHideMethod );	break;
	case NDAILYQUEST3_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CDailyQuest3SelectDlg,				ShowHideMethod );	break;
	//[진성] 마을 이벤트 - 몬스터 습격. => 2008-3-26
	case NMONSTER_ATTACK_EVENT:					CREATE_INTERFACE( m_pDialog[DialogID], NMonsterAttackEventDlg,				ShowHideMethod );	break;
	// [영훈] GM Info
	case NGMINFO_DLG:							CREATE_INTERFACE( m_pDialog[DialogID], CNGMInfoDlg,							ShowHideMethod );	break;
	// [영훈] 수환수 강화 정보 및 초기화
	case NCHARSUMMONRESET_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNCharSummonReset,					ShowHideMethod );	break;
	// [ 성웅 ] 버그 수정 NPC 다이얼로그 
	case NCHANGE_DLG:							CREATE_INTERFACE( m_pDialog[DialogID], NChangeDlg,							ShowHideMethod );	break;
	case NSETCHAT_DLG:							CREATE_INTERFACE( m_pDialog[DialogID], CNSetChatDlg,						ShowHideMethod );	break;
	
	//[진성] 영웅의 동상. => 2008-6-9
	case NBUFF_ROULETTE_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNBuffRoulette,						ShowHideMethod );	break;
	// [영훈] 멥 이름 보여주기 (2008.06.17)
	case NMAPNAME_DLG:							CREATE_INTERFACE( m_pDialog[DialogID], CNMapNameDlg,						ShowHideMethod );	break;

	//[진성] 서버 이벤트 점수 출력창. => 2008-6-23
	case SERVER_EVENT_RANK_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], NServerEventRankDlg,					ShowHideMethod );	break;
	// [영훈] 서버 이벤트 퀘스트 상태 보여주기
	case SERVER_EVENT_QUEST_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNServerEventQuestDlg,				ShowHideMethod );	break;
	//[진성]서버 이벤트 인던 종료 후 조건 만족시 나오는 보너스 창. => 2008-6-25	
	case SERVER_EVENT_INSTANCE_MAP_BONUS:		CREATE_INTERFACE( m_pDialog[DialogID], NServerInstanceMapBounusDlg,			ShowHideMethod );	break;
	// [영훈] 서버 이벤트 인던내 정보
	case SERVER_EVENT_INDUNINFO:				CREATE_INTERFACE( m_pDialog[DialogID], CNServerEventIndunInfoDlg,			ShowHideMethod );	break;
	// [ 성웅 ] 아이템 땅에 드롭시 삭제 여부 다이얼로그 
	case NDESTROYITEM_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], NDestroyItemDlg,						ShowHideMethod );	break;
	// [희영] 시세확인 2008-07-18
	case NMARKETCONDITIONS_DLG: 				CREATE_INTERFACE( m_pDialog[DialogID], CNMarketConditionsDlg,				ShowHideMethod); break;
	

	// [진성] 단체 메시지 보내기. 리스트 창. => 2008-8-4
	case NPOSTOFFICE_GROUP_LIST_DLG: 			CREATE_INTERFACE( m_pDialog[DialogID], NPostOfficeGroupListDlg,				ShowHideMethod); break;

	// [영훈] 퀘스트 알림창 변경 (부모창)
	case NQUESTINFODLG_PARENT:					CREATE_INTERFACE( m_pDialog[DialogID], CNQuestInfoDlg_Parent,				ShowHideMethod);	break;
	case NQUESTINFODLG_GOINGQUEST:				CREATE_INTERFACE( m_pDialog[DialogID], CNQuestInfoDlg_GoingQuest,			ShowHideMethod);	break;
	case NQUESTINFODLG_ALLQUEST:				CREATE_INTERFACE( m_pDialog[DialogID], CNQuestInfoDlg_AllQuest,				ShowHideMethod);	break;
	// [영훈] 다음 진행 퀘스트 표시창
	case NQUESTNOTICE_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNQuestNoticeDlg,					ShowHideMethod);	break;
	//[성웅] 소환수 보관 시스템
	case NMYSUMMON_DLG:							CREATE_INTERFACE( m_pDialog[DialogID], CNMySummonDlg,						ShowHideMethod );	break;

	// [진성] 가차 개편. 
	case NGACHANEW_DLG:							CREATE_INTERFACE( m_pDialog[DialogID], CNGachaDlgNew,						ShowHideMethod );	break;
	case NCHOISE_ITEM_GAME_START_DLG:			CREATE_INTERFACE( m_pDialog[DialogID], CNChoiseItemGameStartDlg,			ShowHideMethod );	break;
	case NOPENBOX:								CREATE_INTERFACE( m_pDialog[DialogID], CNOpenBoxDlg,						ShowHideMethod );	break;
	case RECIEVED_ITEM_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNRecievedItemDlg,					ShowHideMethod );	break;

	case NMYKEY:								CREATE_INTERFACE( m_pDialog[DialogID], CNMykeyDlg,							ShowHideMethod );	break;
	case NMYKEYMINI:							CREATE_INTERFACE( m_pDialog[DialogID], CNMykeyMiniDlg,						ShowHideMethod );	break;
	// [영훈] 아큐 거래창 리뉴얼
	case NACUTRADEDLG_PARENT:					CREATE_INTERFACE( m_pDialog[DialogID], CNAcuTradeDlg_Parent,				ShowHideMethod );	break;
		
	case NAMBER_BOX_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNAmberBoxDlg,						ShowHideMethod );	break;
	case NCHARBUFF_ROULETTE_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNCharRouletteDlg,					ShowHideMethod );	break;
	case NYESORNODLG:							CREATE_INTERFACE( m_pDialog[DialogID], CNYesOrNoDlg,						ShowHideMethod );	break;			
	case NHELPER_DLG:							CREATE_INTERFACE( m_pDialog[DialogID], NHelperMiniDlg,						ShowHideMethod );	break;			
	case NMAGICCOOLTIME_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNMagicCoolTimeDlg,					ShowHideMethod );	break;
	case NHELPER_QUESTION_OR_ANSWER_DLG:		CREATE_INTERFACE( m_pDialog[DialogID], NHelperQuestionOrAnswerDlg,			ShowHideMethod );	break;
	case NHELPER_QUESTION_LIST_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], NHelperQuestionListDlg,				ShowHideMethod );	break;
	case NHELPER_ANSWER_COMPLETE_DLG:			CREATE_INTERFACE( m_pDialog[DialogID], NHelperAnswerCompleteDlg,			ShowHideMethod );	break;
	case PVPLEAGUE_DLG:							CREATE_INTERFACE( m_pDialog[DialogID], CNPVPLeageuDlg,						ShowHideMethod );	break;
	case NEWYEAR_ROULETTE_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNNewYearEventRoulette,				ShowHideMethod );	break;
	case NELIXIR_STATUS_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], NElixirStatusDlg,					ShowHideMethod );	break;
	case NELIXIR_USE_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], NElixirUseDlg,						ShowHideMethod );	break;
	case NELIXIR_NPC_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], NElixirNPCDlg,						ShowHideMethod );	break;
	case NVALANTINE_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNValantineDlg,						ShowHideMethod );	break;
	case NEWSELECTBOX_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNNewSelectBoxDlg,					ShowHideMethod );	break;
	case NNEWMARKET_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], NNewMarketDlg,						ShowHideMethod );	break;
	case NNEWMARKET_SUB_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], NNewMarketUserBuyDlgSub,				ShowHideMethod );	break;
	//case NSUMMON_INVENTORY_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNSummonInventory,					ShowHideMethod );	break;
	case NGUILDDUNGEON_NEW_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNGuildDungeonDlg_New,				ShowHideMethod );	break;
	case NREQUESTHUNTMAPWARREQUEST:				CREATE_INTERFACE( m_pDialog[DialogID], CNRequestHuntingMapWarDlgDlg,		ShowHideMethod );	break;
	case NCARRYOVERGACHA_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNCarryOverGachaDlg,					ShowHideMethod );	break;
	case NGACHAPRODUCT_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNCarryOver_ProductDlg,				ShowHideMethod );	break;
	case NGACHARESULT_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNCarryOver_ResultDlg,				ShowHideMethod );	break;
	case NGUILDWARRESULT_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNGuildWarResult,					ShowHideMethod );	break;
	case NGUILDWARKILLSIGN_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNKillSignDlg,						ShowHideMethod );	break;
	case NWEDDINGCEREMONY_MENU_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNWeddingCeremony,					ShowHideMethod );	break;
	case NPROMOTER_INFO:						CREATE_INTERFACE( m_pDialog[DialogID], NPromoter_InfoDlg,					ShowHideMethod );	break;
	case NPROPOSE_QUESTION_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNProposeQuestionDlg,				ShowHideMethod );	break;
	case NPROPOSE_ANSWER_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNProposeAnswerDlg,					ShowHideMethod );	break;
	case NPROMOTER_GIVE_ITEM_NOTICE:			CREATE_INTERFACE( m_pDialog[DialogID], NPromoter_Give_Item_Notice,			ShowHideMethod );	break;
	case NPROMOTER_MINI:						CREATE_INTERFACE( m_pDialog[DialogID], NPromoter_Mini,						ShowHideMethod );	break;
	case NPROMOTER_INTRODUCE:					CREATE_INTERFACE( m_pDialog[DialogID], NPromoter_Introduce,					ShowHideMethod );	break;
	case NMARRIAGELOVELEVEL_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNMarriageLoveLevelDlg,				ShowHideMethod );	break;
	case NWEDDINGANNIVERSARY_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNWeddingAnniversaryDlg,				ShowHideMethod );	break;
	case NWEDDINGCEREMONY_LIST_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNWeddingCeremonyListDlg,			ShowHideMethod );	break;
	case NFAMILYINFO_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNFamilyInfoDlg,						ShowHideMethod );	break;
	//case NMYFAMILYINFO_DLG_WITH_CHILDREN:						CREATE_INTERFACE( m_pDialog[DialogID], CNMyFamilyInfoDlg,						ShowHideMethod );	break;
	case NBALANCECURRENCY_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNBalanceCurrencyDlg,				ShowHideMethod );	break;	// [종호] 통화량 조절 룰렛 버프창
	case NFAMILY_DLG_ADD_CHILDREN:				CREATE_INTERFACE( m_pDialog[DialogID], CNFamily_Add_ChildrenDlg,			ShowHideMethod );	break;	// [종호] 통화량 조절 룰렛 버프창
	case NFAMILYCOOKIELEVEL_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNFamilyCookieDlg,					ShowHideMethod );	break;
	case PANNEL_SOULGUARD_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNPannel_SoulGuardDlg,				ShowHideMethod );	break;
	case CLTNSOULGUARDHOUSESTATUS_DLG:			CREATE_INTERFACE( m_pDialog[DialogID], cltNSoulGuardHouseStatus,			ShowHideMethod );	break;
	case NCHARSOULGUARDINFO_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNCharSoulGuardInfo,					ShowHideMethod );	break;
	case NPCROOM_DISPLAY_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNPCRoom_Display,					ShowHideMethod );	break;
	case NPCROOM_DLG:							CREATE_INTERFACE( m_pDialog[DialogID], CNPCRoom_Reward,						ShowHideMethod );	break;
	case NRACOONDODGE_EVENT_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], NRacconDldgeEventDlg,				ShowHideMethod );	break;
	case NRACOONDODGE_EVENT_RESERVELIST_DLG:	CREATE_INTERFACE( m_pDialog[DialogID], NRacoonDodgeEventReserveListDlg,		ShowHideMethod );	break;
	case NBOKBULBOK_EVENT_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], NBokBulBokEventDlg,					ShowHideMethod );	break;	// 복불복 시작 창.
	case NBOKBULBOK_RESERVELIST_DLG:			CREATE_INTERFACE( m_pDialog[DialogID], CBokBulBokEventReserveListDlg,		ShowHideMethod );	break;	// 복불복 대기자예약창.
	//case NBOKBULBOK_RESERVE_CANCEL_DLG:			CREATE_INTERFACE( m_pDialog[DialogID], CBokBulBokEventReserveListDlg,		ShowHideMethod );	break;	// 복불복 대기자예약창.
	case NGAWIBAWIBO_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], NGawiBawiBoDlg,						ShowHideMethod );	break;	// 가위바위보
	case NDORMANCYARMOURBOX_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNDormancyArmourBoxDlg,				ShowHideMethod );	break;
	case NDORMANCYWEAPONBOX_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNDormancyWeaponBoxDlg,				ShowHideMethod );	break;
	case NDORMANCYPOINT_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNDormancyPointDlg,					ShowHideMethod );	break;
	case NNEWNPCREPAIR_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], NNewNPCRepairDlg,					ShowHideMethod );	break;
	case NAVATARQUEST_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNAvatarQuestDlg,					ShowHideMethod );	break;
	case NWARNDURABILITY_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNWarnDurabilityDlg,					ShowHideMethod );	break;
		
	case NHALLOWEENSPIRITBOX_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], NHalloweenSpiritBoxDlg,				ShowHideMethod );	break;
	case NMAINMAP_DLG:							CREATE_INTERFACE( m_pDialog[DialogID], CMainMapDlg,							ShowHideMethod );	break;

	case NMONSTERCREATE_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNMonsterCreateDlg,					ShowHideMethod );	break;
	case NEQUIPMENTCREATE_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNEquipmentCreateDlg,				ShowHideMethod );	break;

	case NCHARHOUSE_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNCharHouseDlg,						ShowHideMethod );	break;
	
	case NCHANGECHARNAME_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNChangeCharName,					ShowHideMethod );	break;
	case NCOPFIELD_DLG:							CREATE_INTERFACE( m_pDialog[DialogID], NCopfield_NPC,						ShowHideMethod );	break;
	case NRACCOONEVENT_CHANGE_GOONZUEVENT_DLG:	CREATE_INTERFACE( m_pDialog[DialogID], CNGoonzuRaccoonEventDlg,				ShowHideMethod );	break;//[성웅] 라쿤이벤트 군주 이벤트로 변경 2009-10
	case NBATTLEROYALRESULT1_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNBattleRoyalResultDlg,				ShowHideMethod );	break;
	case NBATTLEROYALRESULT2_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNBattleRoyalResultDlg,				ShowHideMethod );	break;	
	case NBATTLEROYALTIME_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNBattleRoyalTimeDlg,				ShowHideMethod );	break;	
	case NBATTLEROYALMOVE_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNBattleRoyalMoveDlg,				ShowHideMethod );	break;	
	case N2010NEWYEAR_FORTUNE_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CFortuneCookieDlg,					ShowHideMethod );	break;
	case NSELECT_QUEST_BASE_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNSelectQuest_PartyMatching,			ShowHideMethod );	break;	// [지연] 파티매칭
	case NPARTY_MATCHING_BASE_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNApplytoParty_PartyMatching,		ShowHideMethod );	break;
	case NCREATE_PARTY_BASE_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], CNCreatePartyDlg,					ShowHideMethod );	break;
	case NGIANTRACCOONRESURRECTEVENTDLG:		CREATE_INTERFACE( m_pDialog[DialogID], NGiantRaccoonResurrectEventDlg,		ShowHideMethod );	break;
	case NPARTY_JOIN_DLG:						CREATE_INTERFACE( m_pDialog[DialogID], CNPartyMatching_PartyJoinDlg,		ShowHideMethod );	break;	// [지연] 파티매칭을 위한 새로운 파티 가입 수락 창
	case NPVPMAPGODLG:							CREATE_INTERFACE( m_pDialog[DialogID], CNPVPMapDlg,							ShowHideMethod );	break;	// [성웅] PVP 맵 이동 npc
	case NPOTRACCOONDLG:						CREATE_INTERFACE( m_pDialog[DialogID], NPotRaccoonDlg,						ShowHideMethod );	break;	// [성웅] PVP 맵 이동 npc
	case NMANUFACTURE_NEW_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], CNManufactureDlg_New,				ShowHideMethod );	break;	// [성웅] 제조의뢰 인터페이스 개선	
	case VALENTINEDAY_NPC_DLG:					CREATE_INTERFACE( m_pDialog[DialogID], NValentineDayNPCDlg,					ShowHideMethod );	break;	// [진성] 발렌타인데이 NPC
	case VALENTINEDAY_PACKING_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], NValentineDayPackingDlg,				ShowHideMethod );	break;	// [진성] 발렌타인데이 NPC
	case VALENTINEDAY_TRADE_DLG:				CREATE_INTERFACE( m_pDialog[DialogID], NValentineDayTradeDlg,				ShowHideMethod );	break;	// [진성] 발렌타인데이 NPC
	case VALENTINEDAY_CONFESSION_QUESTION_DLG:	CREATE_INTERFACE( m_pDialog[DialogID], NValentineDayConfessionQuestionDlg,	ShowHideMethod );	break;	// [진성] 발렌타인데이 NPC
	case VALENTINEDAY_CONFESSION_ANSWER_DLG:	CREATE_INTERFACE( m_pDialog[DialogID], NValentineDayConfessionAnswerDlg,	ShowHideMethod );	break;	// [진성] 발렌타인데이 NPC
		
		
	default:
		{			
		}
		break;
	}

	return;
}

void cltClient::DestroyInterface( CControllerMgr *pDialogController )
{
	if ( pDialogController == NULL )
	{
		return;
	}


	for ( SI16 i = 0; i < DIALOG_END; ++i )
	{
		if ( m_pDialog[ i ] == pDialogController )
		{
			if(m_pDialog[ i ] == NULL) continue;
			m_pDialog[ i ]->SavePosToFile();

			// 삭제되는 다이얼로그가 포커스를 가지고 있다면 포커스 삭제
            if( m_pInterfaceMgr->GetFouceMgr() == pDialogController )
			{
				m_pInterfaceMgr->ClearFocusMgr( );
			}
			SAFE_DELETE( m_pDialog[ i ] );

			break;
		}
	}

	return;
}

cltClient::~cltClient() 
{
	// APEX-
#ifdef _CHINA
	StopApexClient();

	Sleep( 1000 );

	DeleteCriticalSection( &m_CSSendToGameServer );
#endif
	// -APEX

	SI16 i  = 0;
	pGameSession	= NULL;
	pFrontSession	= NULL;

	// dhm-
	NDelete( m_pClientDiffHellMan );

	NDelete( pclGuildWarClient );
	NDelete( m_pMarkManager );
	NDelete( m_pBeginnerText );
	NDelete( m_pLevelUpText );
	NDelete( m_pclGameTip );
	
	NDelete( m_pFeastEffectManager );
	NDelete( m_pclBattleRoyal );
	
//	NDelete( m_pPCRoomIntro );

	//NDelete( m_pCAbuseFilter )

	//NDelete( pclMiniHomeUpdates ) ;
	NDelete( m_pclPICAManagerCheck );
	NDelete( m_pPopupCheck );
	NDelete( m_pFriendLoginDraw );
	NDelete( m_pVillageWarNotice );
	NDelete( m_pNewYearEventNotice );

	NDelete( m_pResidentLoginDraw );
	NDelete(pclCapture);

	NDelete( m_pNPrivateChatDlgMgr );

//	NDelete(pclGameContract);
	NDelete(pclClickManager);
	NDelete(pclHelpManager);
	NDelete(pclFocusManager);
	NDelete(pclDrawChatSurface);
	NDelete(pclDrawCharNameSurface);

	NDelete(pclDrawSequence);

	NDelete(pclGImg);
	NDelete(pclPersonNameManager);
	NDelete(pclNewPersonNameManager);
	NDelete(pclLogo);
	NDelete(m_pLoadingImg);

	NDelete(pclPannel);

	NDelete( pclDurability );
	
	NDelete( pclVillageNotice );

	NDelete(pclMusic);

	NDelete(pclUnderMouseObject);

	NDelete(pclRegistry);

	NDelete(pclToolBar);
	NDelete(m_pResidenceCheck);

//	NDelete( m_pHtmlDlgManager );

//	NDelete(m_pSearchDetailDlg);

	NRelease( lpScreenMapBuffer );

	NDelete(pclHeroDataBackup);

	

	for ( i = 0; i < DIALOG_END; ++i )
	{
		
		NDelete( m_pDialog[ i ] );
	}

	NDelete( m_pInterfaceMgr );

	//cyj m_pClientMsgPtr 메모리 해제 추가
	NDelete_Array( m_pClientMsgPtr );

	if ( strTextFontFileName )
	{
		RemoveFontResource( strTextFontFileName );
	}
	
	g_DlgPositionSave.SavePosition();
    
	GP.ReleaseGP();

	NDelete(m_pclCheckProcess);
	NDelete(m_pItemPeriodMgr);
	NDelete(m_pGawiBawiBoMgr);
	NDelete(m_pClient_BBBMgr);
	NDelete(m_pClient_PartyMatchingMgr);
	NDelete(m_pPartyQuestMgr);


	if( m_pclGZFtp )
	{
		delete m_pclGZFtp;

		m_pclGZFtp	=	NULL;
	}
}

BOOL cltClient::CreateDirectDraw()
{

	BOOL ret = FALSE;

	switch( siScreenMode )
	{

	case MODE_GRAPHICS800_600:
		siClientScreenXsize		= 80 * (10 + 2) ;
		siClientScreenYsize		= 40 * (15 + 2) ;
		siClientScreenHalfSize	= 14;

		if( g_bFullScreen ) {
			ret = GP.InitGP(GetHwnd(), siScreenMode,lpBackScreen,siClientScreenXsize,siClientScreenYsize, TRUE, 800, 600 );
		} else {
			ret = GP.InitGP(GetHwnd(), siScreenMode,lpBackScreen,siClientScreenXsize,siClientScreenYsize );
		}

		break;
	case MODE_GRAPHICS1024_768:

		if( g_bFullScreen ) {
			siClientScreenXsize = 80 * (13 + 2);
			siClientScreenYsize = 40 * (20 + 2);
			siClientScreenHalfSize	= 18;
			ret = GP.InitGP(GetHwnd(), siScreenMode,lpBackScreen,siClientScreenXsize,siClientScreenYsize, TRUE, 1024, 768, pclClient->IsCountrySwitch(Switch_ChangeFullScreen) );
		} else {
			siClientScreenXsize = 80 * (12 + 2);
			siClientScreenYsize = 40 * (18 + 2);
			siClientScreenHalfSize	= 18;
			ret = GP.InitGP(GetHwnd(), siScreenMode,lpBackScreen,siClientScreenXsize,siClientScreenYsize );
		}

		break;
	// KHY - 1218 - 1024x600 해상도 추가. imsi	
	case MODE_GRAPHICS1024_600:

		if( g_bFullScreen ) {
			siClientScreenXsize = 80 * (13 + 2);
			siClientScreenYsize		= 40 * (15 + 2) ;
			siClientScreenHalfSize	= 16;
			ret = GP.InitGP(GetHwnd(), siScreenMode,lpBackScreen,siClientScreenXsize,siClientScreenYsize, TRUE, 1024, 600, pclClient->IsCountrySwitch(Switch_ChangeFullScreen) );
		} else {
			siClientScreenXsize = 80 * (12 + 2);
			siClientScreenYsize = 40 * (15+2);
			siClientScreenHalfSize	= 16;
			ret = GP.InitGP(GetHwnd(), siScreenMode,lpBackScreen,siClientScreenXsize,siClientScreenYsize );
		}

		break;
	case MODE_GRAPHICS1280_1024:
		siClientScreenXsize = 80 * (16 + 2);
		siClientScreenYsize = 40 * (23 + 2);
		siClientScreenHalfSize	= 20;
		break;

	default:
		MsgBox(TEXT("fd92kf"), TEXT("Fd3:%d"), siScreenMode);
		break;
	}



	if( ret ) {

		//		pScreenMapBuffer	= new UI08[siClientScreenXsize * siClientScreenYsize];

		GP.CreateSurface( lpScreenMapBuffer, siClientScreenXsize, siClientScreenYsize );
	}

	if( !g_bFullScreen ) {
		ShowWindow( GetHwnd(), SW_HIDE );
		UpdateWindow( GetHwnd() );
	}
	return ret;
}


void cltClient::SetFlashWindowNotice( bool bNotice )
{
	if( g_bFullScreen ) return;

	bWindowNotice = bNotice;
}

// 클라이언트인 경우 서버로 명령을 보내는 처리를 한다. 
BOOL cltClient::OrderOperation(cltOrderData order)
{
	bool bdirectorderswitch = false;

	// 이동이나 공격명령인 경우에는 클라이언트에서 바로 조치한다. 
	if( order.ODI.siOrder == ORDER_MOVE || 
		order.ODI.siOrder == ORDER_ATTACK || 
		order.ODI.siOrder == ORDER_ATTACK_CLIENT )
	{
		bdirectorderswitch = true;
	}
	
	// 클라이언트에 직접 명령을 입력할 수 있는 모드라면.. 
	if(bdirectorderswitch)	LetOrder(&order, 1);

	cltMsg clMsg(GAMEMSG_REQUEST_USERORDER, sizeof(cltOrderData), (BYTE*)&order);	

	SendMsgToServer((sPacketHeader*)&clMsg);

	return TRUE;
}


// 서버로 채트 메시지를 보낸다. 
BOOL cltClient::SendChatMessage(SI16 chatmode, TCHAR* pmsg,SI32 systemtxt)
{
	TCHAR chatbuffer[ 256 ] = TEXT("");
	TCHAR* pChatMsg = NULL ;

	memcpy( chatbuffer, pmsg, 256 );
	chatbuffer[ 255 ] = NULL;
	if( chatbuffer[ 0 ] == NULL ) return FALSE;
	pChatMsg = chatbuffer ;
	pChatMsg =	chatbuffer;

	bool bGameMster = true ;
	bool bPlayerGM = true ;

	if ( pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
		bGameMster = false ;

	if( pclCM->CR[1]->GetGameMasterLevel() < BASIC_PLAYERMASTER_LEVEL )
		bPlayerGM = false ;

	if(bGameMster == true || bPlayerGM == true)
	{
#ifdef _IAF_EXPORT
		if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
		{
			if(!(pChatMsg[0] == '/'))
			{
				TCHAR buf[256] = "";
				IAF::Filtering( pmsg, buf, sizeof(buf) );
				memcpy( pmsg, buf, strlen(buf) * sizeof(buf[0]) );
			}
		}
#else
		// 운영자(PlayerGM포함)이고 "/"로 시작하는 메시지는 욕설필터를 하지 않는다.
		if(!(pChatMsg[0] == '/'))
			m_pCAbuseFilter->CheckAbuse(pmsg);
#endif
	}
	else
	{
#ifdef _IAF_EXPORT
		if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
		{
			if(!(pChatMsg[0] == '/'))
			{
				TCHAR buf[256] = "";
				IAF::Filtering( pmsg, buf, sizeof(buf) );
				memcpy( pmsg, buf, strlen(buf) * sizeof(buf[0]) );
			}
		}
#else
		//모든메세지 욕설필터 적용[영진]
		m_pCAbuseFilter->CheckAbuse(pmsg);
#endif
	}

	bool bSecret = false ;
	if(pChatMsg[0] == TEXT('/'))
	{
		// 귓말이면, 
		if(pChatMsg[1] == TEXT('t') || pChatMsg[1] == TEXT('T'))
		{
			//cyj 귓말메세지 욕설필터 적용
			//m_pCAbuseFilter->CheckAbuse(pmsg);

			/*
			TCHAR szname[MAX_PLAYER_NAME] = TEXT("") ;
			TCHAR code[256] = TEXT("");

			_stscanf(&pChatMsg[1], TEXT("%s %s"), code, szname);
			szname[ MAX_PLAYER_NAME - 1 ] = NULL;
			TCHAR* ptext = &pChatMsg[1 + 1+ 1+ (_tcslen(szname) + 1)];

			if ( m_pCAbuseFilter->CheckAbuse(ptext) == false )
			{
				TCHAR* pTitle = GetTxtFromMgr(1853);
				TCHAR* pText = GetTxtFromMgr(1854);
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return FALSE ;
			}
			*/
		}

		bSecret = true ;
	}
	else if ( bPlayerGM == false && bGameMster == false && m_pCAbuseFilter->CheckAbuse(pmsg) == false )
	{
		TCHAR* pTitle = GetTxtFromMgr(1853);
		TCHAR* pText = GetTxtFromMgr(1854);
		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		return FALSE ;
	}


	if ( !bSecret && chatmode == CHATMODE_NEWSITEM )
	{
		TCHAR buffer[256] = TEXT("") ;

		SI32 siTicketUnique = ITEMUNIQUE(24045) ; // 자막광고이용권
		SI32 siColorTicketUnique = ITEMUNIQUE(24047); // 고백자막이용권
		if ( pclCM->CR[1]->pclCI->clCharItem.GetItemNum(siTicketUnique) <= 0 && pclCM->CR[1]->pclCI->clCharItem.GetItemNum(siColorTicketUnique) <= 0 )
		{
			TCHAR* pTitle = GetTxtFromMgr(1856);
			TCHAR* pText = GetTxtFromMgr(1855);

			StringCchPrintf(buffer, 256, pText);
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
			return FALSE;
		}

#ifdef _NEW_INTERFACE_

		if( m_pDialog[ NCHATBAR_DLG ] )
			((CNChatBarDlg *)m_pDialog[ NCHATBAR_DLG  ])->ChatMessage(chatmode,systemtxt,pmsg);

#else //_NEW_INTERFACE_

		if( m_pDialog[ STATUSBAR_DLG ] )
			((CNStatusBarDlg *)m_pDialog[ STATUSBAR_DLG ])->ChatMessage(chatmode,systemtxt,pmsg);

#endif
		return TRUE;

	
	}

	//================================================================
	// LEEKH - 클라이언트 게임 테스트용 명령어
	//================================================================
	
	if(	bGameMster &&
		pChatMsg[0] == TEXT('/') &&
		chatmode == CHATMODE_NORMAL )
	{
		// 몬스터 위치 데이터를 파일로 일목요연하게 기록하게끔 해주는 명령어
 		if( _tcsncmp( &pChatMsg[1], "makemonposdata" , _tcslen("makemonposdata")) == 0 )
		{
			FILE * fp = fopen( TEXT("makemonposdata.txt") , "wt" );
			if( fp == NULL ) return false;

			for( SI16 i = 0 ; i < MAX_KIND_NUMBER ; i ++ )
			{
				cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[ i ];

				if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
					continue;

				switch (pclClient->siServiceArea)
				{
				case ConstServiceArea_Korea :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
						continue;
					break;
				case ConstServiceArea_China :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
						continue;
					break;
				case ConstServiceArea_Japan :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
						continue;
					break;
				case ConstServiceArea_English :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
						continue;
					break;
				case ConstServiceArea_Taiwan :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
						continue;
					break;
				case ConstServiceArea_USA  :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
						continue;
					break;
				case ConstServiceArea_NHNChina :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_NHNCHINA ) == false)
						continue;
					break;
				case ConstServiceArea_EUROPE :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_EUROPE ) == false)
						continue;
					break;

				}

				//============================================
				// 출몰위치정보를 출력한다.
				//============================================
				cltNPCManagerClient* pclnpcmanager = (cltNPCManagerClient*)pclClient->pclNPCManager;
				cltRegenPos	clRegenPos[100];
				SI32	siRegenCount = 0;
 				if(pclnpcmanager->GetKindPos( i , clRegenPos, &siRegenCount) == true)
				{						
					// 리스트를 모두 기록한다. 					
					for(SI32 j = 0 ; j < siRegenCount; ++j)
					{
						fprintf(fp, "%d\t%s\t%s\t%s\t%s\n" , i, pclki->GetName() , clRegenPos[j].szMapName
							,clRegenPos[j].szMapPos , clRegenPos[j].szNearMapInfo );
					}				
				
				}
			}

			fclose(fp);
			return TRUE;
		}
		else if( _tcsncmp( &pChatMsg[1], "makedungeondata" , _tcslen("makedungeondata")) == 0 )
		{
			MakeDungeonDataTxt();
			return TRUE;
		}
		else if( _tcsncmp( &pChatMsg[1], "makenpcdata" , _tcslen("makenpcdata")) == 0 )
		{
			MakeNPCDataTxt();
			return TRUE;
		}
		else if( _tcsncmp( &pChatMsg[1], "makemonsterdata" , _tcslen("makemonsterdata")) == 0 )
		{
			MakeMonsterDataTxt();
			return TRUE;
		}
		else if( _tcsncmp( &pChatMsg[1], "makemonsterbasicstatusdata" , _tcslen("makemonsterbasicstatusdata")) == 0 )
		{
			MakeMonsterBasicStatusDataTxt();
			return TRUE;
		}
		else if( _tcsncmp( &pChatMsg[1], "makemonsteritemdata" , _tcslen("makemonsteritemdata")) == 0 )
		{
			MakeMonsterItemDataTxt();
			return TRUE;
		}
		else if( _tcsncmp( &pChatMsg[1], "itemmaterial" , _tcslen("itemmaterial")) == 0 )
		{
			MakeItemMaterialTxt();
			return TRUE;
		}
		else if( _tcsncmp( &pChatMsg[1], "execfile" , _tcslen("execfile")) == 0 )
		{
			ExecFile( &pChatMsg[1] );
			return TRUE;
		}
		else if( _tcsncmp( &pChatMsg[1], "makeitemmakedata" , _tcslen("makeitemmakedata")) == 0 )
		{
			MakeItemMakeDataTxt();
			return TRUE;
		}
		else if(strncmp(&pChatMsg[1], "showweb", strlen("showweb")) == 0)
		{
			cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

			UI08 md5buf[256] = "" ;
			char buffermd5[256] = "" ;

			TCHAR	szURL[1024];
			TCHAR	szBeforeMD5[256];

			sprintf(szBeforeMD5,  "%d%d%s%s%s", 100, pclchar->pclCI->clBI.siIDNum, pclchar->pclCI->clBI.szAccountID, pclchar->GetName(), "rnswndhsfkdlseoqkr");

			memset(md5buf, 0, sizeof(md5buf));
			// MD5
			SI32 md5size = _tcslen(szBeforeMD5);
			memcpy(md5buf, szBeforeMD5, md5size);
			md5_string(md5buf, buffermd5, md5size);

			sprintf(szURL, "http://bill4.omg.com.tw/charge/goonzu/goonzuitemlist.asp?server_index=%d&m_idPlayer=%d&m_szName=%s&user_id=%s&md5=%s",
				100, pclchar->pclCI->clBI.siIDNum, pclchar->GetName(), pclchar->pclCI->clBI.szAccountID , buffermd5);

			g_LoginNoticeDlg.Set(szURL, 970, 756);
			return TRUE;
		}
		else if(strncmp(&pChatMsg[1], "makeitemtradeinfo", strlen("makeitemtradeinfo")) == 0)
		{
			FILE* fp = fopen( "itemtradeinfo.txt", "w" );
			if( fp )
			{
				fprintf(fp, "ITEMUNIQUE\tNAME\tITEMMALL\tNOTTRADE\n" );
				for( SI32 i=0; i<MAX_ITEMINFO_NUMBER; i++)
				{
					cltItemInfo* pclItemInfo = pclItemManager->pclItemInfo[i];
					if( pclItemInfo == NULL )		continue;

					if( pclItemManager->IsItemInfoAtb( pclItemInfo->clItem.siUnique, ITEMINFOATB_NOTTRADE | ITEMINFOATB_ITEMMALL ) )
					{
						fprintf(fp, "%d\t%s", pclItemInfo->clItem.siUnique, pclItemInfo->GetName() );
						if( pclItemInfo->siItemInfoAtb & ITEMINFOATB_ITEMMALL )
						{				fprintf(fp, "\t%s", "ITEMINFOATB_ITEMMALL" );		}
						else	{		fprintf(fp, "\t%s", "" );							}

						if( pclItemInfo->siItemInfoAtb & ITEMINFOATB_NOTTRADE )
						{				fprintf(fp, "\t%s", "ITEMINFOATB_NOTTRADE" );		}
						else	{		fprintf(fp, "\t%s", "" );							}

						fprintf(fp, "\n" );
					}
				}
				fclose( fp );
			}
			return TRUE;
		}
	}
		
	//================================================================

	// 채팅 블록 - 시스템 메시지는 블록에 걸리지 않는다.
	if( chatmode != CHATMODE_SYSTEM && pclClient->pclCM->GetCharClient(1)->m_stMuteTime.wYear > 0 )
	{
		NTCHARString256		kText(GetTxtFromMgr(9626));
		NTCHARString256		kTitle(GetTxtFromMgr(5614));
		
		SI64 siTimeDiff = TimeOperation(&pclClient->pclCM->GetCharClient(1)->m_stMuteTime, &sTime);
		if( siTimeDiff > 0 )
		{
			kText.Replace("#minute#", SI32ToString(siTimeDiff / (uiSec * 60)));
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( kTitle, kText);
		}
	}
	else
	{
		cltGameMsgRequest_Chat clinfo(chatmode,systemtxt,pmsg);
		cltMsg clMsg(GAMEMSG_REQUEST_CHAT, clinfo.siDataSize, (BYTE*)&clinfo);

		SendMsgToServer((sPacketHeader*)&clMsg);
	}

	return TRUE;
}

//---------------------------------------
// GameData 정리 출력
//---------------------------------------
// 던젼별 몬스터 정보를 텍스트로 정리해서 출력한다.
void cltClient::MakeDungeonDataTxt()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/HuntmapInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/HuntmapInfo.txt") );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile HuntmapInfo"),TEXT("Error"));
		return;
	}

	TCHAR mapname[256] = TEXT(""), mapunique[256] = TEXT("") , submapname[256] = TEXT("");

	SI32 ptextnum, mapinx, cnt = 0;

	SI32 siKind[MAX_MAP_NPCNUMBER] ;
	SI32 siKindLevel[MAX_MAP_NPCNUMBER] ;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&ptextnum,   4,		
		NDATA_MBSTRING,	mapunique,	256,
		NDATA_INT32,	&mapinx,	4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	FILE * fp = fopen( TEXT("makedungeondata.txt") , "wt" );
	if( fp == NULL ) return ;


  	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData() ) 
		{
			if ( mapunique[ 0 ] != NULL)
			{
				++cnt;
				
				StringCchPrintf( mapname ,256, pclClient->pclMapManager->GetMapName( mapinx )  );
				
				
				for( SI16 i = 0; i < MAX_MAP_NUMBER ; i++ )
				{
					if( pclClient->pclMapManager->GetParentMap(i) == mapinx )
					{
						StringCchPrintf( submapname , 256 ,  pclClient->pclMapManager->GetMapName(i) );
						
						for( SI16 j = 0; j < MAX_MAP_NPCNUMBER ; j++ )
						{
							siKind[j] = 0 ;
							siKindLevel[j] = 0 ;
						}
						((cltNPCManagerClient*)pclNPCManager)->GetKindMapInx( i, siKind, siKindLevel, MAX_MAP_NPCNUMBER );

						for( SI16 k = 0; k < MAX_MAP_NPCNUMBER ; k++ )
						{
							if( siKind[k] <= 0 ) break;
							if( siKindLevel[k] <= 0 ) break;

							fprintf(fp,"%d\t%s\t%d\t%s\t%d\t%s\n" , cnt, mapname , mapinx , submapname , siKind[k] , pclClient->pclKindInfoSet->GetName(siKind[k]) )	;		
						}

					}
				}                  					
				
			}
		}
	}	

	fclose(fp);
}




//---------------------------------------
// GameData 정리 출력
//---------------------------------------
//  SYSTEMNPC 정보 출력
void cltClient::MakeNPCDataTxt()
{
	FILE * fp = fopen( TEXT("makenpcdata.txt") , "wt" );
	if( fp == NULL ) return;

	for( SI16 i = 0 ; i < MAX_KIND_NUMBER ; i ++ )
	{
		cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[ i ];

		if( pclki == NULL || pclki->IsAtb( ATB_SYSTEMNPC ) == false) 
			continue;

		switch (pclClient->siServiceArea)
		{
		case ConstServiceArea_Korea :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
				continue;
			break;
		case ConstServiceArea_China :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
				continue;
			break;
		case ConstServiceArea_Japan :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
				continue;
			break;
		case ConstServiceArea_English :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
				continue;
			break;
		case ConstServiceArea_Taiwan :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
				continue;
			break;
		case ConstServiceArea_USA  :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
				continue;
			break;
		case ConstServiceArea_NHNChina :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_NHNCHINA ) == false)
				continue;
			break;
		case ConstServiceArea_EUROPE :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_EUROPE ) == false)
				continue;
			break;

		}

		//============================================
		// 정보를 출력한다.
		//============================================
		cltNPCManagerClient* pclnpcmanager = (cltNPCManagerClient*)pclClient->pclNPCManager;
		
		TCHAR* pPositionText = NULL;
		TCHAR  szNpcName[256];
		szNpcName[0] = 0;
		TCHAR* pExplainText = NULL;
		//SI32 Hire;
		SI32 PositionX = 0, PositionY = 0;

		if ( i == pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGMAN2") ) )
		{
			pPositionText = GetTxtFromMgr(2383);
		}
		else if ( i == pclClient->GetUniqueFromHash( TEXT("KIND_SUMMONMERCHANT") ) )
		{
			pPositionText = GetTxtFromMgr(300);
		}
		else if ( i == pclClient->GetUniqueFromHash( TEXT("KIND_JANGILSAN") ) )
		{
			pPositionText = GetTxtFromMgr(2385);
		}
		else if ( i == pclClient->GetUniqueFromHash( TEXT("KIND_BATTLEZONE") ) )
		{
			pPositionText = GetTxtFromMgr(5497);
		}
		else if ( i == pclClient->GetUniqueFromHash( TEXT("KIND_MERCHANTPOTTER") ) )
		{
			pPositionText = GetTxtFromMgr(518);
		}
		else if ( i == pclClient->GetUniqueFromHash( TEXT("KIND_JAPANTRADER") ) )
		{
			pPositionText = GetTxtFromMgr(518);
		}
		else if ( i == pclClient->GetUniqueFromHash( TEXT("KIND_PRISONER") ) )
		{
			pPositionText = GetTxtFromMgr(2386);
		}
		else if ( i == pclClient->GetUniqueFromHash( TEXT("KIND_MERCHANTHORSE") ) )
		{
			pPositionText = GetTxtFromMgr(299);
		}	
		else if ( i == pclClient->GetUniqueFromHash( TEXT("KIND_HONGILDONG") ) )
		{
			pPositionText = GetTxtFromMgr(5235);
		}	

		fprintf(fp,"%d\t%s\t%s\n", i , pclki->GetName() ,  pPositionText );

		cltRegenPos	clRegenPos[100];
		SI32	siRegenCount = 0;
		if(pclnpcmanager->GetKindPos( i , clRegenPos, &siRegenCount) == true)
		{	


			// 리스트를 모두 기록한다. 					
			for(SI32 j = 0 ; j < siRegenCount; ++j)
			{
				fprintf(fp, "%s\t%s\t%s\t%s\n" , pclki->GetName() , clRegenPos[j].szMapName
					,clRegenPos[j].szMapPos , clRegenPos[j].szNearMapInfo );
			}			

		}


	}

	fclose(fp);
}

void cltClient::MakeMonsterDataTxt()
{

	FILE * fp = fopen( TEXT("makemondata.txt") , "wt" );
	if( fp == NULL ) return;

	for( SI16 i = 0 ; i < MAX_KIND_NUMBER ; i ++ )
	{
		cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[ i ];

		if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
			continue;

		switch (pclClient->siServiceArea)
		{
		case ConstServiceArea_Korea :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
				continue;
			break;
		case ConstServiceArea_China :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
				continue;
			break;
		case ConstServiceArea_Japan :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
				continue;
			break;
		case ConstServiceArea_English :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
				continue;
			break;
		case ConstServiceArea_Taiwan :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
				continue;
			break;
		case ConstServiceArea_USA :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
				continue;
			break;
		case ConstServiceArea_NHNChina :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_NHNCHINA ) == false)
				continue;
			break;
		case ConstServiceArea_EUROPE :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_EUROPE ) == false)
				continue;
			break;

		}

		//============================================
		// 공격력 정보를 출력한다.
		//============================================
		cltNPCManagerClient* pclnpcmanager = (cltNPCManagerClient*)pclClient->pclNPCManager;

		TCHAR szBuffer[512];
		szBuffer[0] = 0;

		pclClient->pclKindInfoSet->GetCharAttackInfo( i, szBuffer, 512 );

		fprintf( fp , "%d\t%s\t%s\n", i , pclki->GetName() , szBuffer );

		
	}

	fclose(fp);

}

void cltClient::MakeMonsterBasicStatusDataTxt()
{
	FILE * fp = fopen( TEXT("makemonsterbasicstatusdata.txt") , "wt" );
	if( fp == NULL ) return;

	for( SI16 i = 0 ; i < MAX_KIND_NUMBER ; i ++ )
	{
		cltKindInfo* pclki = NULL;

		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			pclki = pclClient->pclKindInfoSet->pclSummonKI[i];
		}
		else
		{
			pclki = pclClient->pclKindInfoSet->pclKI[i];
		}	

		if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
			continue;

		BOOL bSummonRank = DATA_TO_TYPE_BOOL(pclClient->pclKindInfoSet->pclSummonKI[i]->siSummonRank);

		// 소환수만 출력함
		if(bSummonRank == FALSE)
		{
			continue;
		}

		switch (pclClient->siServiceArea)
		{
		case ConstServiceArea_Korea :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
				continue;
			break;
		case ConstServiceArea_China :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
				continue;
			break;
		case ConstServiceArea_Japan :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
				continue;
			break;
		case ConstServiceArea_English :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
				continue;
			break;
		case ConstServiceArea_Taiwan :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
				continue;
			break;
		case ConstServiceArea_USA :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
				continue;
			break;
		case ConstServiceArea_NHNChina :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_NHNCHINA ) == false)
				continue;
			break;
		case ConstServiceArea_EUROPE :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_EUROPE ) == false)
				continue;
			break;

		}

		//============================================
		// 기본 스테이터스 정보를 출력한다.
		//============================================
		cltNPCManagerClient* pclnpcmanager = (cltNPCManagerClient*)pclClient->pclNPCManager;

		TCHAR szBuffer[512];
		szBuffer[0] = 0;

		cltPIPInfo			clipinfo2;
		clipinfo2.SetStr(pclki->clBA.GetStr());
		clipinfo2.SetDex(pclki->clBA.GetDex());
		clipinfo2.SetVit(pclki->clBA.GetVit());
		clipinfo2.SetMag(pclki->clBA.GetMag());
		clipinfo2.SetLuk(pclki->clBA.GetLuk());
		clipinfo2.SetWis(pclki->clBA.GetWis());

		sprintf(szBuffer, "%s%d\t%s%d\t%s%d\t%s%d\t%s%d\t%s%d\n", TEXT("Str:"), clipinfo2.GetStr(), TEXT("Dex:"), clipinfo2.GetDex(), TEXT("Vit:"), clipinfo2.GetVit(), TEXT("Mag:"), clipinfo2.GetMag(),
			    TEXT("Luk:"), clipinfo2.GetLuk(), TEXT("Wis:"), clipinfo2.GetWis());

		fprintf( fp , "%d\t%s\t%s\n", i , pclki->GetName() , szBuffer);
	}

	fclose(fp);
}

void cltClient::MakeMonsterItemDataTxt()
{

	FILE * fp = fopen( TEXT("makemonitemdata.txt") , "wt" );
	if( fp == NULL ) return;

	for( SI16 i = 0 ; i < MAX_KIND_NUMBER ; i ++ )
	{
		cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[ i ];

		if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
			continue;

		switch (pclClient->siServiceArea)
		{
		case ConstServiceArea_Korea :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
				continue;
			break;
		case ConstServiceArea_China :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
				continue;
			break;
		case ConstServiceArea_Japan :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
				continue;
			break;
		case ConstServiceArea_English :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
				continue;
			break;
		case ConstServiceArea_Taiwan :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
				continue;
			break;
		case ConstServiceArea_USA  :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
				continue;
			break;
		case ConstServiceArea_NHNChina :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_NHNCHINA ) == false)
				continue;
			break;
		case ConstServiceArea_EUROPE :
			if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_EUROPE ) == false)
				continue;
			break;

		}

		//============================================
		// 아이템 정보를 출력한다.
		//============================================
		cltNPCManagerClient* pclnpcmanager = (cltNPCManagerClient*)pclClient->pclNPCManager;

		TCHAR szBuffer[512];
		szBuffer[0] = 0;

		cltDropItem clDropItem[50];
		SI32		siDropCount = 0;
		if(pclClient->pclKindInfoSet->GetItemFromKindExplain(i, clDropItem, &siDropCount) == true)
		{	
			for( SI32 j = 0 ; j < siDropCount ; ++j )
			{
				// 아이템 이름 , 개수, 확률
				fprintf( fp , "%d\t%s\t%s\t%s\t%s\n", i , pclki->GetName() , clDropItem[j].szName , clDropItem[j].szItemNum , clDropItem[j].szPercent );
			}

		}
		


	}

	fclose(fp);

}

void cltClient::MakeItemMakeDataTxt()
{
	FILE * fp = fopen( TEXT("MakeItemMakeData.txt") , "wt" );
	if( fp == NULL ) return;

	typedef struct _DrpItem
	{
		TCHAR	szName[32];
		SI32	siUnique;
		SI32	siPercent;
		SI32	siMin;
		SI32	siMax;
	} DrpItem;
	DrpItem	kDropItem[MAX_ITEMINFO_NUMBER];

	typedef struct _Mtrl
	{
		TCHAR	szName[32];
		SI32	siCount;
	} Mtrl;
	Mtrl	kMtrl[MAX_KINDINFO_NUMBER];

	SI32	siCountDropItem;
	SI32	siCountMtrl;
	SI32	siItem;
	TCHAR	szName[32];

	for (int i=0; i<MAX_ITEMINFO_NUMBER; i++)
	{
		siCountDropItem = 0;
		siCountMtrl = 0;

		if(pclItemManager->pclItemInfo[i] == NULL)			continue;

		siItem = pclItemManager->pclItemInfo[i]->clItem.siUnique;
		strcpy(szName, pclItemManager->pclItemInfo[i]->GetName());

		int j;
		for( j=0; j<MAX_KINDINFO_NUMBER; j++)
		{
			for (int p=0; p<MAX_ITEM_PROBABILITY_PER_KIND; p++)
			{
				if(pclKindInfoSet->pclKI[j] == NULL)														continue;
				if(pclKindInfoSet->pclKI[j]->clItemPbtSet.clKindItemProbability[p].siItemUnique != siItem)	continue;

				SI32 siItemUnique = pclKindInfoSet->pclKI[j]->clItemPbtSet.clKindItemProbability[p].siItemUnique;
				strcpy(kDropItem[siCountDropItem].szName, pclKindInfoSet->GetName(j));
				kDropItem[siCountDropItem].siUnique		= siItemUnique;
				kDropItem[siCountDropItem].siPercent	= pclKindInfoSet->pclKI[j]->clItemPbtSet.clKindItemProbability[p].siProbability * 100 / 1000;
				kDropItem[siCountDropItem].siMin	= pclKindInfoSet->pclKI[j]->clItemPbtSet.clKindItemProbability[p].siItemMinNum;
				kDropItem[siCountDropItem].siMax	= pclKindInfoSet->pclKI[j]->clItemPbtSet.clKindItemProbability[p].siItemNum;
				siCountDropItem++;
			}
		}

		for( j=0; j<MAX_ITEMINFO_NUMBER; j++)
		{
			if(pclItemManager->pclItemInfo[j] == NULL)		continue;

			for(int p=0; p<MAX_MATERIAL_NUMBER; p++)
			{
				SI32 siMtrlUnique = pclItemManager->pclItemInfo[j]->clProductionInfo.clMaterial[p].siUnique;
			    if(siMtrlUnique == siItem)
				{
					strcpy(kMtrl[siCountMtrl].szName, pclItemManager->pclItemInfo[j]->GetName());
					kMtrl[siCountMtrl].siCount = pclItemManager->pclItemInfo[j]->clProductionInfo.clMaterial[p].siNeedNum;
					siCountMtrl++;
				}
			}
		}

		if(siCountMtrl == 0 && siCountDropItem == 0)		continue;

		SI32 siLoop;
		siLoop = max(siCountMtrl, siCountDropItem);
		siLoop = max(1, siLoop);

		TCHAR	szItem1[128];
		TCHAR	szItem2[128];
		TCHAR	szItem3[128];

		for(int loop = 0; loop < siLoop; loop++)
		{
			if (loop == 0)		{		sprintf(szItem1, "%6d %s", siItem, szName);		}
			else				{		strcpy(szItem1, "");			}

			if(loop < siCountDropItem)
				sprintf(szItem2, "%6d %s\t%3d%%\t%2d~%2d", kDropItem[loop].siUnique, kDropItem[loop].szName, kDropItem[loop].siPercent, kDropItem[loop].siMin, kDropItem[loop].siMax);
			else
				strcpy(szItem2, "\t\t");

			if(loop < siCountMtrl)
				sprintf(szItem3, "%s\t%3d", kMtrl[loop].szName, kMtrl[loop].siCount);
			else
				strcpy(szItem3, "");			

			fprintf(fp, "%s\t%s\t%s\n", szItem1, szItem2, szItem3);
		}
		// 한줄 띄우기
		fprintf(fp, "%s\t%s\t%s\n", "", "", "");
	}

	fclose(fp);
}

void cltClient::MakeItemMaterialTxt()
{
	SI32 i, j;

	FILE* fp = fopen( TEXT("MakeItemMaterial.txt") , "wt" );
	if(fp == NULL)			return;

	cltItemInfo*	pclItemInfo = NULL;

	for( i=0; i<MAX_ITEMINFO_NUMBER; i++)
	{
		pclItemInfo = pclClient->pclItemManager->pclItemInfo[i];
		if(pclItemInfo == NULL)		continue;

		bool bHasMaterial = false;
		cltMaterialInfo* pcltMaterialInfo = NULL;

		for( j=0; j<MAX_MATERIAL_NUMBER; j++)
		{
			pcltMaterialInfo = &pclItemInfo->clProductionInfo.clMaterial[j];
			if(pcltMaterialInfo->siUnique > 0)
			{
				bHasMaterial = true;
				break;
			}
		}

		if(bHasMaterial)
		{
			for( j=0; j<MAX_MATERIAL_NUMBER; j++)
			{
				pcltMaterialInfo = &pclItemInfo->clProductionInfo.clMaterial[j];
				if(pcltMaterialInfo->siUnique > 0)
				{
					if( j == 0 )
					{
						fprintf(fp, "%d\t%s\t%d\t\t%d\t%s\t%d\n", pclItemInfo->clItem.siUnique, pclItemInfo->GetName(), pclItemInfo->clProductionInfo.siProductUnit,
									pcltMaterialInfo->siUnique, pclClient->pclItemManager->GetName(pcltMaterialInfo->siUnique), pcltMaterialInfo->siNeedNum);
					}
					else
					{
						fprintf(fp, "\t\t\t\t%d\t%s\t%d\n",
							pcltMaterialInfo->siUnique, pclClient->pclItemManager->GetName(pcltMaterialInfo->siUnique), pcltMaterialInfo->siNeedNum);
					}
				}
			}
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
}

void cltClient::ExecFile( TCHAR* pszCommand )
{	
	NTCHARString1024	kCommand, kFileName;

	kCommand = pszCommand;
	if( kCommand.Length() == 0 )		return;

	SI32 siFileNamePos = kCommand.Find(" ");
	if( siFileNamePos >= 0 )
	{
		kFileName = kCommand.SubStr(siFileNamePos+1);
	}

	if( kFileName.Length() > 0 )
	{
		FILE* fp = fopen( kFileName, "r" );
		int err = GetLastError();
		if( fp )
		{
			TCHAR				szLine[1024];
			NTCHARString1024	kLine;
			while ( fgets(szLine, sizeof(szLine), fp) )
			{
				kLine = szLine;		kLine.Replace("\n", "");

				cltGameMsgRequest_Chat clinfo(CHATMODE_NORMAL, 0, kLine);
				cltMsg clMsg(GAMEMSG_REQUEST_CHAT, clinfo.siDataSize, (BYTE*)&clinfo);

				SendMsgToServer((sPacketHeader*)&clMsg);
			}
			fclose( fp );
		}
	}
}

// 작은 지도에서 사용자의 위치를 설정한다. 
void cltClient::SetSmallMapUserPos(cltSmallMap* pclSmallMap)
{
	if(pclSmallMap == FALSE)return ;

	// 사용자 캐릭터가 있는 경우에만 사용자 캐릭터의 위치와 작은 지도의 위치를 연동한다. 
	if(MyCharUnique)
	{
		SI32 id = cltCharCommon::CharUniqueInfo[MyCharUnique];
		if(id)
		{
			pclSmallMap->SetUserPos(pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY());
		}
	}

	//Small Map에서 부모 지도의 위치를 변경할 것을 요구한다. 
	if(pclSmallMap->ParentMapX >=0)
	{
		// 맵 에디터인 경우에만 부모 지도의 위치가 변경된다. 
		if(GameMode== GAMEMODE_MAPEDIT)
		{
			cltMapClient* pclmap = (cltMapClient*)pclMapManager->pclCurrentMap;

			pclmap->VirtualMapX = pclSmallMap->ParentMapX;
			pclmap->VirtualMapY = pclSmallMap->ParentMapY;
		}



		//부모 지도의 '요구'를 초기화한다. 
		pclSmallMap->ParentMapX = -1;
		pclSmallMap->ParentMapY = -1;
	}


}

//---------------------------------------
// Hero View Operate
//---------------------------------------
void cltClient::HeroViewOperate()
{
	// 일본버전만 옛날 스크롤방식
	if (pclClient->siServiceArea == ConstServiceArea_Japan)
	{
		SI32 id = 1;

		cltMapClient* pclmap = (cltMapClient*)pclMapManager->pclCurrentMap;

		if ( pclmap )
		{

			// 주인공 좌표로 지도 좌표를 설정한다.
			SI32 oldvirtualx	= pclmap->VirtualMapX;
			SI32 oldvirtualy	= pclmap->VirtualMapY;

			pclmap->VirtualMapX = pclCM->CR[id]->GetX();
			pclmap->VirtualMapY = pclCM->CR[id]->GetY();

			if(bWatchModeSwitch)
			{
				pclmap->VirtualMapX += siWatchModeDetailX;
				pclmap->VirtualMapY += siWatchModeDetailY;
			}

			cltCharClient* pclchar = (cltCharClient*)pclCM->CR[1];
			SI32 centerx = 0;
			SI32 centery = 0;
			pclmap->GetXYDotFromXYCord(pclchar->GetX(), pclchar->GetY(), centerx, centery);

			//임시테스트 by cyj
			//static UI32 moveRecordFrame2 = pclClient->GetFrame();
			//static SI32 lastcenterx = centerx;
			//static SI32 lastcentery = centery;

			//static int lastmapx = pclchar->GetX();
			//static int lastmapy = pclchar->GetY();

			//if (TABS(pclClient->GetFrame() - moveRecordFrame2) >= 10)
			//{
			//	moveRecordFrame2 = pclClient->GetFrame();


			//	if (TABS(centerx - lastcenterx) > 80)
			//	{
			//		MsgBox("Error", "Error");
			//	}
			//	if (TABS(centery - lastcentery) > 80)
			//	{
			//		MsgBox("Error", "Error");
			//	}


			//	lastcenterx = centerx;
			//	lastcentery = centery;

			//	lastmapx = pclmap->VirtualMapX;
			//	lastmapy = pclmap->VirtualMapY;


			//	TCHAR sztext[128];
			//	StringCchPrintf(sztext, 128, "FRAME : %d, VirtualMapX : %d, VirtualMapY : %d, centerx : %d, centery : %d", 
			//		pclClient->GetFrame(), pclmap->VirtualMapX, pclmap->VirtualMapY, centerx, centery);
			//	pclLog->FilePrint(TEXT("Config\\MoveInfo.txt"), sztext);

			//}

			if(oldvirtualx != pclmap->VirtualMapX || oldvirtualy != pclmap->VirtualMapY)
			{
				pclmap->SetMapDrawSwitch(true);
			}



			if ( m_pDialog[ NMINIMAP_DLG ] )
			{
				((CNMiniMapDlg *)m_pDialog[ NMINIMAP_DLG ] )->ChangeStandardPos(pclmap->VirtualMapX, pclmap->VirtualMapY);
			}
		}
	}
	else
	{
		SI32 id = 1;
		static SI32 siLastCenterX, siLastCenterY;
		static SI32 siLastMoveStep;


		cltMapClient* pclmap = (cltMapClient*)pclMapManager->pclCurrentMap;

		if ( pclmap )
		{

			// 주인공 좌표로 지도 좌표를 설정한다.
			SI32 oldvirtualx = pclmap->VirtualMapX;
			SI32 oldvirtualy = pclmap->VirtualMapY;

			//-----------------------------------------------
			// 화면 변위 설정. 
			//-----------------------------------------------

			SI32 rx = 0, ry = 0;



			//-----------------------------------------
			// 화면의 진짜 중심을 구한다. 
			//-----------------------------------------
			cltCharClient* pclchar = (cltCharClient*)pclCM->CR[1];
			SI32 centerx = 0;
			SI32 centery = 0;
			pclmap->GetXYDotFromXYCord(pclchar->GetX(), pclchar->GetY(), centerx, centery);
			rx = pclchar->GetRX();
			ry = pclchar->GetRY();

			// 이전의 화면 좌표를 기억하고 있지 않다면 캐릭터가 있는 곳을 화면 좌표의 중심으로 간주한다. 
			if(bSetScreenCenterSwitch == true)
			{
				bSetScreenCenterSwitch = false;

				//---------------------------------
				// 캐릭터 중심을 구한다. 
				//---------------------------------
				siOldScreenCenterX = centerx;
				siOldScreenCenterY = centery;

				siLastCenterX  = centerx;
				siLastCenterY  = centery;
				siLastMoveStep  = 0;
			}


			SI32 diffx = 0 , diffy = 0;

			SI32 sibuffer = 0;
			if(pclchar->MoveDone() == false)
			{
				siLastCenterX = centerx + rx;
				siLastCenterY = centery + ry;

				//siLastMoveStep= 5;//++;    
				//sibuffer = 5;			

				siLastMoveStep=10;//5;//++;    // 태곤테스트
				sibuffer = 10;//5;				// 태곤테스트

			}
			else
			{
				sibuffer = 1;
			}

			if(siLastMoveStep >= sibuffer)
			{
				if(siLastCenterX > siOldScreenCenterX)
				{
					diffx = max(1, (siLastCenterX - siOldScreenCenterX) / siLastMoveStep);
				}
				if(siLastCenterX < siOldScreenCenterX)
				{
					diffx = min(-1, (siLastCenterX - siOldScreenCenterX) / siLastMoveStep);

				}
				if(siLastCenterY > siOldScreenCenterY)
				{
					diffy = max(1, (siLastCenterY - siOldScreenCenterY) / siLastMoveStep);

				}
				if(siLastCenterY < siOldScreenCenterY)
				{
					diffy = min(-1, (siLastCenterY - siOldScreenCenterY) / siLastMoveStep);

				}

				siLastMoveStep--;

			}


			// 화면의 궁극적인 위치를 구한다. 
			SI32 dstscreenx = siOldScreenCenterX + diffx;
			SI32 dstscreeny = siOldScreenCenterY + diffy;

			//char sztext[128];
			//sprintf(sztext, "%d %d", diffx, diffy);
			//pclLog->FilePrint(TEXT("Config\\MoveInfo.txt"), sztext);


			// 가장 가까운 타일 좌표를 구한다. 
			SI32 tilex, tiley;
			pclmap->GetRealTileByRealDot(dstscreenx, dstscreeny, &tilex, &tiley);
			pclmap->VirtualMapX = tilex;
			pclmap->VirtualMapY = tiley;

			pclmap->GetXYDotFromXYCord(pclmap->VirtualMapX, pclmap->VirtualMapY, centerx, centery);
			rx = dstscreenx - centerx;
			ry = dstscreeny - centery;


			// 다시 화면 좌표를 구한다. 
			pclmap->GetXYDotFromXYCord(pclmap->VirtualMapX, pclmap->VirtualMapY, centerx, centery);
			centerx += rx;
			centery += ry;

			siOldScreenCenterX = centerx;
			siOldScreenCenterY = centery;

			// 화면의 RX, RY를 설정한다. 
			SetScreenRXY(rx, ry);

			pclUnderMouseObject->SetRX(rx);
			pclUnderMouseObject->SetRY(ry);

			//임시테스트 by cyj
			//static UI32 moveRecordFrame = pclClient->GetFrame();
			//static SI32 lastcenterx = centerx;
			//static SI32 lastcentery = centery;

			//static int lastmapx = pclmap->VirtualMapX;
			//static int lastmapy = pclmap->VirtualMapY;

			//if (TABS(pclClient->GetFrame() - moveRecordFrame) >= 10)
			//{
			//	moveRecordFrame = pclClient->GetFrame();

			//	if (TABS(centerx - lastcenterx) > 30)
			//	{
			//		MsgBox("Error", "Error");
			//	}
			//	if (TABS(centery - lastcentery) > 30)
			//	{
			//		MsgBox("Error", "Error");
			//	}

			//	lastcenterx = centerx;
			//	lastcentery = centery;

			//	lastmapx = pclmap->VirtualMapX;
			//	lastmapy = pclmap->VirtualMapY;


			//	TCHAR sztext[128];
			//	StringCchPrintf(sztext, 128, "FRAME : %d, VirtualMapX : %d, VirtualMapY : %d, centerx : %d, centery : %d", 
			//		pclClient->GetFrame(), pclmap->VirtualMapX, pclmap->VirtualMapY, centerx, centery);
			//	pclLog->FilePrint(TEXT("Config\\MoveInfo.txt"), sztext);

			//}

			if(oldvirtualx != pclmap->VirtualMapX || oldvirtualy != pclmap->VirtualMapY)
			{
				pclmap->SetMapDrawSwitch(true);
			}

			//// 축소지도의 기준점을 변경한다. 
			//pclmap->SetSmallMapStandardPos(pclmap->VirtualMapX, pclmap->VirtualMapY);
			//// 축소지도와 실제 지도간의 연계를 설정하여 서로 연결되게 한다. 
			//pclmap->SetSmallMapConnectToMapSwitch(TRUE);

			if ( m_pDialog[ NMINIMAP_DLG ] )
			{
				((CNMiniMapDlg *)m_pDialog[ NMINIMAP_DLG ] )->ChangeStandardPos(pclmap->VirtualMapX, pclmap->VirtualMapY);
			}
		}
	}

}

//---------------------------------------
// User Char List
//---------------------------------------
// 사용자의 캐릭터 정보를 리스트에 추가한다. 
// 이렇게 확보된 캐릭터 정보는 사용자로부터 선택될 수 있다.
BOOL cltClient::PutCharList(cltCharListInfo* pcharinfo)
{
	if(pcharinfo == NULL)return FALSE;

	memcpy(&clCharListInfo[ pcharinfo->clBI.GetIndex() ], pcharinfo, sizeof(cltCharListInfo) );


	return TRUE;
}

// 캐릭터 정보를 삭제한다.
BOOL cltClient::DeleteCharList(cltGameMsgResponse_DelUserChar* pcharinfo)
{
	SI32 slot = pcharinfo->siCharSlot;

	clCharListInfo[slot].Init();

	return TRUE;
}

BOOL cltClient::PreDeleteCharList(cltGameMsgResponse_PreDelUserChar* pcharinfo)//케릭터삭제	유예기간
{
	SI32 slot = pcharinfo->siCharSlot;

	clCharListInfo[slot].Init();

	return TRUE;
}


//-------------------------------------------
// Skill
//-------------------------------------------
// 스킬 다이얼로그 박스를 만든다. 
BOOL cltClient::CreateSkillDlg(SI32 charunique)
{
	if(charunique == 0)
	{
		charunique = MyCharUnique;
	}

	//-----------------------------------------------------------
	// 캐릭터 기술 정보창을 여는 퀘스트가 진행중이라면 
	// 게임 서버로 퀘스트가 완료되었음을 알려준다.
	//-----------------------------------------------------------
	SI16 questtype, questunique, para1, para2;
	SI16 questclearcond =  QUEST_CLEAR_COND_OPENSKILL;
	if(pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
	{
		cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
		cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		SendMsgToServer((sPacketHeader*)&clMsg);
	}

	return 0;
}


BOOL cltClient::CreateStrDlg(SI32 charunique, cltStructureMngBasic* pclmgr, SI32 dlgitem, DLGPROC dlgproc)
{
	if(pclmgr == NULL)return 0;

	if(charunique == 0)
	{
		charunique = MyCharUnique;
	}

	pclmgr->CharUnique = charunique;

	
	return 0;
}


// 주가 정보를 일정 시간마다 화면에 보여준다. 
void cltClient::ShowStockPrice()
{
	static SI32 CureentStockVillage = 0;

	if(GetFrame()  % 50 )return ;
	if(pclVillageManager == NULL)return ;
	if(pclVillageManager->pclVillageInfo[CureentStockVillage] == NULL)return ;


	if(pclVillageManager->pclVillageInfo[CureentStockVillage])
	{
		GMONEY stockprice = pclVillageManager->pclVillageInfo[CureentStockVillage]->clVillageVary.clVillageStockPrice.siCurrentStockPrice;
		if(stockprice)
		{
			TCHAR buffer[256];
			TCHAR* pText = GetTxtFromMgr(940);


			StringCchPrintf(buffer, 256, pText, 
				pclVillageManager->pclVillageInfo[CureentStockVillage]->szName, 
				stockprice);

			//			pclStatusBar->SetText(buffer, 3);
		}
	}

	CureentStockVillage ++;
	CureentStockVillage %= MAX_VILLAGE_NUMBER;

}
//------------------------------------------
// CityHall
//------------------------------------------
BOOL cltClient::CreateCityHallDlg(SI32 charunique)
{
	if(charunique == 0)
	{
		charunique = MyCharUnique;
	}

	return 0;
}



//-----------------------------------------------
// 사운드 관련 
//-----------------------------------------------
BOOL cltClient::PushEffect(SI32 effect, SI32 x, SI32 y)
{
	return pclMusic->PushEffect(effect, x, y);
}

void cltClient::PopEffect()
{
	pclMusic->PopEffect((cltMapClient*)pclMapManager->pclCurrentMap);
}


//----------------------------------------------
// 스크롤 관련 
//----------------------------------------------
void cltClient::SetScrollProcSwitch(BOOL mode)
{
	ScrollProcSwitch = mode;
}

void cltClient::ScrollAction()
{
	if(ScrollProcSwitch == FALSE)
	{	

		if(GameMode == GAMEMODE_MAPEDIT )
		{
			if(bActiveSwitch == true)
			{
				cltMapClient* pclmap = (cltMapClient*)pclMapManager->pclCurrentMap;
				pclmap->ScrollOperation();
			}
		}
		else
		{	
			cltMapClient* pclmap = (cltMapClient*)pclMapManager->pclCurrentMap;
			pclmap->ScrollOperation();
		}
		SetScrollProcSwitch(TRUE);
	}

}

//--------------------------------------
// Action
//---------------------------------------
SHORT cltClient::GameAction()
{
	static bool bgameactionswitch = true;

	if(bgameactionswitch == true)
	{
		// 적절한 지도가 불려져 있지 않으면 실패. (클라이언트에서. ) 
		if(GameMode == GAMEMODE_CLIENT)
		{
			cltMapClient* pclmap = (cltMapClient* )pclMapManager->pclCurrentMap;
			if(pclmap == NULL)return 0;
		}


		bgameactionswitch = false;

		ClientAction2();
		IncreaseFrame();

		//-----------------------------------
		// [지연] 복불복 게임 메니져 Action
		//-----------------------------------
		if(IsCountrySwitch(Switch_BokBulBok)  == true )
		{
			if(m_pClient_BBBMgr != NULL )
				m_pClient_BBBMgr->Action();
		}

		if ( true == IsCountrySwitch(Switch_PartyQuest_PartyMatching) )
		{
			if ( NULL != m_pClient_PartyMatchingMgr )
			{
				m_pClient_PartyMatchingMgr->Action();
			}
		}

		// 캐릭터의 행동 
		pclCM->Action();


		pclBulletManager->Action();						// 총알들의 행동 

		pclSmokeManager->Action();						// 연기들의 행동 

		pclMagicManager->Action();						// 마법들의  행동 
		
		// 마을 행동 
		if(pclVillageManager)
		{
			pclVillageManager->Action();
		}

	}

	// 클라이언트가 해야 할 부분을 행한다. 
	if(ClientAction1() == FALSE)
	{
		return 0;
	}

	bgameactionswitch = true;

	return 0;
}


BOOL cltClient::ClientAction1()
{
 	cltMapClient* pclmap = (cltMapClient* )pclMapManager->pclCurrentMap;
	if(pclmap == NULL)return false;



	// 5분에 한번씩만 체크하도록 변경
	// 중국은 체크 하지 않음

	// cltCharClient::DrawNormal() 함수에서 스프라이트 파일 체크로 변경 - by LEEKH

/*	if ( siServiceArea != ConstServiceArea_China )
	{
		static clock_t lastcheckclock = 0;
		if(TABS( CurrentClock - lastcheckclock ) > 1000 * 300 )
		{
			HANDLE	hFile = NULL ;
			DWORD	dwSizeHigh = 0 ;

			hFile = CreateFile("char\\DomaFig\\DomaFig.spr", GENERIC_READ , FILE_SHARE_READ , NULL ,
				OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL);

			if ( hFile != INVALID_HANDLE_VALUE )
			{
				DWORD dwSizeLow = GetFileSize(hFile,&dwSizeHigh);

				if ( dwSizeLow != 4699828 )
				{
					MsgBox("Error","Find the Valid File!!");
					CloseHandle(hFile);
					dwQuitTimer = 1;	
					return FALSE ;
				}
			}
			else
			{
				if( GAMEMODE_CLIENT ==  GameMode )
				{
					MsgBox("Error","Can't Read Important file!!");
					CloseHandle(hFile);
					dwQuitTimer = 1;		
					return FALSE ;
				}
			}

			CloseHandle(hFile);
		}
	}*/


	
	// 명령 관련 처리를 수행한다. 
	OrderAction();

	// 게임을 지연시킨다.  
	// 클라이언트일 경우에만, 서버일 경우는 라이브러리에서 지연처리
	if(GameDelay() == FALSE)
	{
		return FALSE;
	}

	// 포그 처리. 
	if((pclClient->CurrentClock / 50) % 40 == 0)
	{
		if ( pclMapManager->IsFogMap(pclmap->siMapIndex) )
		{
			pclmap->FogRecoverAction();
			pclCM->CR[1]->FogAction();

			SI32 id;
			SI32 index = 0 ;
			while(id = pclCM->GetCharOrder(index))
			{
				index++;
				if ( id != 1 )
				{
					pclCM->CR[id]->FogAction();
				}
			}
		}
	}
	if( m_pclGZFtp )
	{
		m_pclGZFtp->Poll();
	}

	return TRUE;

}

BOOL cltClient::ClientAction2()
{
	SI32 id		= 1;
	cltMapClient* pclmap = (cltMapClient* )pclMapManager->pclCurrentMap;
	if(pclmap == NULL)return false;

	// 초보 도움말 
	if( pclCM->CR[id]->pclCI->clIP.GetLevel() <= 10 )
	{
		if( ConstServiceArea_China == siServiceArea )
		{
			m_pBeginnerText->Action();
		}
		else
		{
			if((pclClient->CurrentClock / 50) % 800 == 0)
			{
				// 체력이 떨어져 있으면, 
				if(pclCM->IsAlive(id) && (pclCM->CR[id]->clPB.GetMaxLife() / 2 ) > pclCM->CR[id]->pclCI->clIP.GetLife())
				{
					TCHAR* pText = GetTxtFromMgr(941);

					//[진성]공지 분리 텍스트 세팅. => 2008-6-2
					if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
					{
						pclMessage->SetPersonalMsg(pText);
					}
					else
					{
						pclMessage->SetMsg(pText);
					}
				}
			}

			if((pclClient->CurrentClock / 50) % 2000 == 0)
			{
				// 초보자 사냥터에서 전투하는 것이 안전하다. 
				if(pclCM->IsAlive(id) && pclCM->CR[id]->pclCI->clIP.GetLevel() < 10 
					&& (pclCM->CR[id]->GetMapIndex() != MAPINDEX_BEGINNER1 /*||
																		   pclCM->CR[id]->GetMapIndex() != MAPINDEX_BEGINNER2 */))
				{

					TCHAR* pText;
					pText = GetTxtFromMgr(942);
					//[진성]공지 분리 텍스트 세팅. => 2008-6-2
					if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
					{
						pclMessage->SetPersonalMsg(pText);
					}
					else
					{
						pclMessage->SetMsg(pText, 0);
					}
				}
			}
		}
	}

	// 장비의 내구도 검사. 
	//CheckuserEquipDur();



	// 불가사리 사냥 이벤트 중이라면, 
/*	if(bHuntingEventSwitch)
	{
		if((pclClient->CurrentClock / 50) % 1000 == 0)
		{
			TCHAR* pText = GetTxtFromMgr(943);


			pclMessage->SetMsg(pText);
		}

	}
*/
	// 고구려 유적 방어 이벤트 중이라면, 
	if(bGoguEventSwitch)
	{
		if((pclClient->CurrentClock / 50) % 1000 == 0)
		{
			TCHAR* pText = GetTxtFromMgr(944);


			pclMessage->SetMsg(pText);
		}

	}

	// 오사카성 이벤트 중이라면, 
	if(bOsakaEventSwitch)
	{
		/*	if((pclClient->CurrentClock / 50) % 1000 == 0)
		{
		TCHAR* pText = GetTxtFromMgr(5300);

		pclMessage->SetMsg(pText);
		}*/
	}

	// 강화도 이벤트 중이라면, 
	if(bKanghwadoEventSwitch)
	{
		if((pclClient->CurrentClock / 50) % 1000 == 0)
		{
			TCHAR* pText = GetTxtFromMgr(944);


			pclMessage->SetMsg(pText);
		}

	}

	pclDurability->Action();

	// 배경 효과음 
	pclmap->PlayBackEffect();

	// 하트비트. 
	if(TABS( pclClient->CurrentClock - m_SentHeartBeatClock ) > 30 * 1000 )
	{
		m_SentHeartBeatClock = pclClient->CurrentClock;
		SI32 id = 1;
		if(pclCM->IsValidID(id))
		{
			cltMsg clMsg(GAMEMSG_REQUEST_HEARTBEAT, 0, NULL);
			SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}

	// 물방울의 행동
	if( pclmap->pclWaterDrop )
	{
		pclmap->pclWaterDrop->Action();
	}

	// 반딧불의 행동
	if ( pclmap->pclFireFly )
	{
		pclmap->pclFireFly->Action();
	}

	// 얼음성 반짝이의 행동
	if ( pclmap->pclIceFlash )
	{
		pclmap->pclIceFlash->Action();
	}

	// 작은풀숲 홀씨의 행동
	if ( pclmap->pclSeedDrop )
	{
		if((pclClient->CurrentClock / 50) % 2 == 0)
		{
			pclmap->pclSeedDrop->Action();
		}
	}

	// 주가 표시 
	ShowStockPrice();

	// 프롬프트 
	//if(bPromptSwitch)
	{
		g_PromptMgr.Action();
	}

	//  [8/11/2009 종호_GAWIBAWIBO]
	if(pclClient->IsCountrySwitch(Switch_GawiBawiBo) == true)
		m_pGawiBawiBoMgr->Action();

	//-----------------------------------------------------
	// 예약되어 있는 캐릭터가 있다면 생산한다. (Client)
	//-----------------------------------------------------
	SetCharReservedChar();

	// Hit Effect
	g_AttackEffectMgr.Action();

	//------------------------------------
	// DB/게임서버 이상 여부.
	//------------------------------------
	if(pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL)
	{
		if((pclClient->CurrentClock / 50) % 100 == 0)
		{
			// 일정 시간 이상 게임서버로부터 연락이 없으며. 
			if( dwLastReceiveFromServerClock && TABS(CurrentClock - dwLastReceiveFromServerClock) > 20000)
			{
				if(bDBFailSwitch == true)
				{
					TCHAR* pText = GetTxtFromMgr(945);


					PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_SERVERFAIL")), 0, 0);
					pclMessage->SetMsg(pText);
				}

				TCHAR* pText = GetTxtFromMgr(946);
				PushEffect(GetUniqueFromHash(TEXT("SOUND_EFFECT_SERVERFAIL")), 0, 0);
				pclMessage->SetMsg(pText);

			}
		}
	}

	//------------------------------------
	// 불필요한 Field Object 삭제 
	//------------------------------------
	//if ( IsWhereServiceArea( ConstSwitch_FieldObjectPreLoad ) == false )
	if (pclClient->IsCountrySwitch(Switch_FieldObjectPreLoad) )
	{
		if(TABS( pclClient->CurrentClock - m_DeleteFOLastCheckClock ) > 10 * 1000 )
		{
			m_DeleteFOLastCheckClock = pclClient->CurrentClock;

			if(pclFieldObjectManager)
			{
				pclFieldObjectManager->DeleteIfNotUse( GetFrame() );
			}
			if(pclKindInfoSet)
			{
				//pclKindInfoSet->DeleteIfNotUse( CurrentClock );
			}
		}
	}

	//---------------------------------
	// 건강 상태를 파악한다.
	//---------------------------------
	if((pclClient->CurrentClock / 50)  % 800 == 0)
	{
		SI32 id = 1;

		if(pclCM->IsAlive(id))
		{
			SI32 status = pclCM->CR[id]->clPB.clHealth.AnalyzeHealth( pclCM->CR[id]->pclCI->clIP.GetLevel());

			if(status & (HEALTH_STATUS_HUNGRY_1 | HEALTH_STATUS_HUNGRY_2))
			{
				TCHAR* pText = GetTxtFromMgr(947);

				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclMessage->SetPersonalMsg(pText);
				}
				else
				{
					pclMessage->SetMsg(pText);
				}
			}
			if(status & HEALTH_STATUS_HUNGRY_3)
			{
				TCHAR* pText = GetTxtFromMgr(533);


				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclMessage->SetPersonalMsg(pText);
				}
				else
				{
					pclMessage->SetMsg(pText);
				}
			}
			if(status & HEALTH_STATUS_HUNGRY_7)
			{
				TCHAR* pText = GetTxtFromMgr(948);


				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclMessage->SetPersonalMsg(pText);
				}
				else
				{
					pclMessage->SetMsg(pText);
				}

				pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_HUNGRY_WARNING")), 0, 0);
			}
			if(status & (HEALTH_STATUS_HUNGRY_8 | HEALTH_STATUS_HUNGRY_9))
			{
				TCHAR* pText = GetTxtFromMgr(949);


				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclMessage->SetPersonalMsg(pText);
				}
				else
				{
					pclMessage->SetMsg(pText);
				}

				pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_HUNGRY_WARNING")), 0, 0);
			}

			// 너무 무겁게 하고 다니면, 
			if( siServiceArea != ConstServiceArea_China )
			{
				if(pclCM->CR[id]->GetTotalWeight() >= pclCM->CR[id]->GetMaxWeight())
				{
					TCHAR* pText = GetTxtFromMgr(950);
					//[진성]공지 분리 텍스트 세팅. => 2008-6-2
					if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
					{
						pclMessage->SetPersonalMsg(pText);
					}
					else
					{
						pclMessage->SetMsg(pText);
					}
				}
			}

			// 건강이 부족한 상황이면, 
			if(pclCM->CR[id]->clPB.clHealth.pclInfo->uiHealthPoint < 100)
			{
				TCHAR* pText = GetTxtFromMgr(951);

				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclMessage->SetPersonalMsg(pText);
				}
				else
				{
					pclMessage->SetMsg(pText);
				}
			}
		}

	}

	//--------------------------------
	// 현재 진행중인 퀘스트 정보 표시. 
	//--------------------------------
	static DWORD lastshowframe = 0;
	if(TABS(GetFrame() - lastshowframe) >= 1200)
	{
		lastshowframe = GetFrame();

		SI32 id = 1;

		// 처음 게임하는 사람에게 퀘스트를 알려준다. 
		static bool bnaisiswitch = false;
		if(pclCM->CR[id]->pclCI->clIP.GetLevel() < 5)
		{
			// 퀘스트 정보를 플레이어로부터 받은 상태임에도 아무 퀘스트 정보가 없다면, 
			if(bReceiveMyCharAllInfoSwitch == true)
			{
				TCHAR* pText;
				// 신이일때
				if( pclCM->CR[1]->pclKI->siDefaultCountry == ConstServiceArea_China )
				{
					pText = TEXT("");
				}
				else
				{
					pText = GetTxtFromMgr(952);
				}

				if ( siServiceArea != ConstServiceArea_China )
				{
					pclMessage->SetMsg(pText);
				}

				if(bnaisiswitch == false)
				{
					bnaisiswitch = true;

					if(pclCM->CR[1]->ClearQuest(30003) == false)
					{
						//	pclNaesiPortraitDraw->Set( true );
					}
				}
			}
		}
	}
	if(pclCM->CR[1]->ClearQuest(30003) == true)
	{
		//	pclNaesiPortraitDraw->Set( false );
	}



	//---------------------------------
	// 시전 계약 기간 만료 확인 
	//---------------------------------
	static DWORD dwlasthousecheckclock = 0;
	if(TABS(CurrentClock - dwlasthousecheckclock) > 300000)
	{
		dwlasthousecheckclock = CurrentClock;

		//시전의 계약 만료 기간을 체크한다. 
		SI32 curvary = clClientDate.GetDateVary();
		for(SI32 i = 0;i < MAX_REAL_ESTATE_NUMBER_PER_PERSON;i++)
		{
			if(pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siType == 0)continue;

			// 계약 종료일을 구한다. 
			cltDate cldate;

			cldate.Set(&pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].clContract.clDate);
			cldate.AddMonth(pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].clContract.siRentPeriod * 12);

			SI32 endvary = cldate.GetDateVary();

			if(endvary <= curvary + 90)
			{
				SI32 villageunique = pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siVillageUnique;

				TCHAR buffer[256];

				if( pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siType == REALESTATE_TYPE_HOUSE ) {
					TCHAR num[128]= TEXT(" ") ;
					TCHAR month[128]= TEXT(" ") ;

					TCHAR* pText = GetTxtFromMgr(953);
					StringCchPrintf(num, 128, TEXT("%d"),pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siIndex + 1);
					StringCchPrintf(month, 128, TEXT("%d"),(endvary-curvary)/30);
					Nsprintf(buffer,pText,TEXT("village"),   pclVillageManager->GetName(villageunique)  ,
						TEXT("num"), num   ,TEXT("month"),  month  ,NULL);



					//sprintf(buffer, pText, pclVillageManager->GetName(villageunique), pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siIndex + 1, (endvary-curvary)/30);

				} else if( pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siType == REALESTATE_TYPE_LAND ) { 

					TCHAR num[128]= TEXT(" ") ;
					TCHAR month[128]= TEXT(" ") ;

					TCHAR* pText = GetTxtFromMgr(954);
					StringCchPrintf(num, 128, TEXT("%d"),pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siIndex + 1);
					StringCchPrintf(month, 128, TEXT("%d"),(endvary-curvary)/30);
					Nsprintf(buffer,pText,TEXT("village"),   pclVillageManager->GetName(villageunique)  ,
						TEXT("num"), num   ,TEXT("month"),  month  ,NULL);



					//sprintf(buffer, pText, pclVillageManager->GetName(villageunique), pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siIndex + 1, (endvary-curvary)/30);

				} else if( pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siType == REALESTATE_TYPE_MINE ) { 

					TCHAR* pText = GetTxtFromMgr(3240);					

					TCHAR index[128]= TEXT(" ") ;
					TCHAR month[128]= TEXT(" ") ;

					StringCchPrintf(index, 128, TEXT("%d"),pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siIndex + 1);
					StringCchPrintf(month, 128, TEXT("%d"), (endvary-curvary)/30);
					Nsprintf(buffer,pText,TEXT("village"),  pclVillageManager->GetName(villageunique)  ,TEXT("index"), index  ,TEXT("month"),month    ,NULL);
					//sprintf(buffer, pText, pclVillageManager->GetName(villageunique), pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siIndex + 1, (endvary-curvary)/30);
				}


				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclMessage->SetPersonalMsg(buffer);
				}
				else
				{
					pclMessage->SetMsg(buffer);
				}

			}
		}

	}

	//-----------------------------
	// 말의 상태 표시. 
	//-----------------------------
	if((pclClient->CurrentClock / 50) % 800 == 0)
	{
		HorseStatusShow(NULL);
	}

	//-----------------------------
	// 좌표 출력
	//-----------------------------
	SI32 corddelay = 1;
	if(GameMode == GAMEMODE_CLIENT)
	{
		corddelay = 10;
	}
	else
	{
		corddelay = 10;
	}

	if((pclClient->CurrentClock / 50) % corddelay == 0)
	{
		TCHAR Buffer[256 ] = TEXT("");

		TCHAR *strMapName = pclMapManager->GetMapName( pclCM->CR[1]->GetMapIndex() );


		if ( strMapName != NULL )
		{
			SI32 x, y;
			if(GameMode == GAMEMODE_CLIENT)
			{
				x = pclCM->CR[1]->GetX();
				y = pclCM->CR[1]->GetY();

			}
			else
			{
				x = pclUnderMouseObject->MapX;
				y = pclUnderMouseObject->MapY;
			}

			TCHAR* pText = GetTxtFromMgr(955);


			StringCchPrintf( Buffer, 256, pText, strMapName, x, y );
/*
			if ( g_ChatStatusBar )
			{
				g_ChatStatusBar->SetText( Buffer, 1 );
			}
*/
		}
	
#ifndef _NEW_INTERFACE_
		if ( g_SoundOnOffDlg.m_bFullScreenMode )
		{

			if ( m_pDialog[ TOOLBAR_DLG ] )
			{

				( ( CToolBarDlg * )m_pDialog[ TOOLBAR_DLG ])->SetPosition( Buffer );
			
			}
			else
			{
				/*
				if ( g_ChatStatusBar )
				{
					g_ChatStatusBar->SetText( Buffer, 1 );
				}
				*/
			}

		}
		else
		{
			/*
			if ( g_ChatStatusBar )
			{
				g_ChatStatusBar->SetText( Buffer, 1 );
			}
			*/
		}
#else
		if( m_pDialog[NMINIMAP_DLG] )
		{
			( (CNMiniMapDlg* )m_pDialog[ NMINIMAP_DLG] )->SetPosition( Buffer );
		}

#endif


	}

	// 캐릭터 조합 사용 안하는 이미지들을 주기적으로 관리하기 위해서...
	if(TABS(  pclClient->CurrentClock - m_CCDLastCheckClock ) > 60 * 1000 )
	{
		m_CCDLastCheckClock = pclClient->CurrentClock;
		g_XSPRMgr.Action();
	}

	//-----------------------------------------------------------
	// 특정 지도로 이동하는 퀘스트라면, 
	// 게임 서버로 퀘스트가 완료되었음을 알려준다.
	//-----------------------------------------------------------
	if((pclClient->CurrentClock / 50) % 10 == 0)
	{
		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond = QUEST_CLEAR_COND_GOMAP;
		if(pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			// QUEST_CLEAR_COND_GOMAP시 맵INDEX 체크를 안함 - by LEEKH 2007.11.21
			BOOL bIsInMap = FALSE;
			if (pclClient->IsCountrySwitch(Switch_NoCheckMapIndex_Quest	) )
			{
				// 모든 마을로 이동해도 완료 된다.
				if( 1 <= pclCM->CR[1]->GetMapIndex() &&	42 >= pclCM->CR[1]->GetMapIndex() )	bIsInMap = TRUE;
			}
			else
			{
				if( pclCM->CR[1]->GetMapIndex() == para1 )									bIsInMap = TRUE;
			}

			if( bIsInMap )
			{
				cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
				cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}

		// [지연] 요술램프 마을 순회
		questclearcond = QUEST_CLEAR_COND_LAMPGOMAP;
		if (pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			// QUEST_CLEAR_COND_GOMAP시 맵INDEX 체크를 안함 - by LEEKH 2007.11.21
			BOOL bIsInMap = FALSE;
			if (pclClient->IsCountrySwitch(Switch_NoCheckMapIndex_Quest	) )
			{
				// 모든 마을로 이동해도 완료 된다.
				if( 1 <= pclCM->CR[1]->GetMapIndex() &&	42 >= pclCM->CR[1]->GetMapIndex() )	bIsInMap = TRUE;
			}
			else
			{
				if( pclCM->CR[1]->GetMapIndex() == para1 )									bIsInMap = TRUE;
			}

			if( bIsInMap )
			{
				cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
				cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
	}

	// PCK : 잃어버린 퀘스트 수행중이라면, 게임 서버로 퀘스트가 완료되었음을 알려준다.
	if((pclClient->CurrentClock / 50) % 20 == 0)
	{
		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond =  QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS;
		SI32 siGetX = pclCM->CR[1]->GetX();
		SI32 siGetY = pclCM->CR[1]->GetY();
		SI32 x1 = 38;
		SI32 y1 = 21;
		SI32 x2 = 45;
		SI32 y2 = 28;

		if(pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			if((pclCM->CR[1]->GetMapIndex() == para2) && (pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_TRANSFORM)))
			{
				// BUF_TRANSFORM 타입으로 변신된 상황이면 무조건 완료 시키는 것으로 변경되어 주석처리.
				//if(pclCM->CR[1]->siBufTransformKind == pclClient->GetUniqueFromHash(TEXT("KIND_TRICERAMONS")))
				{
					bool bStartArea = (x1 <= siGetX && y1 <= siGetY);
					bool bEndArea = (x2 >= siGetX && y2 >= siGetY);	

					if(bStartArea && bEndArea)
					{
						cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
						cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
						SendMsgToServer((sPacketHeader*)&clMsg);
					}
				}
			}
		}
	}

	//------------------------------------
	// 스킬, 스테이터스 초기화를 공지한다.
	//------------------------------------
	if((pclClient->CurrentClock / 50) % 6000 == 0)
	{
		SI32 level = pclCM->CR[1]->pclCI->clIP.GetLevel();

		if(level > 5 && level < INIT_LEVEL)
		{
			if ( ConstServiceArea_China != siServiceArea )
			{
				TCHAR buffer[256];
				TCHAR* pText = GetTxtFromMgr(956);


				StringCchPrintf(buffer, 256, pText, 
					INIT_LEVEL, INIT_LEVEL_SECOND , INIT_LEVEL , INIT_LEVEL_SECOND );
				
				//[진성]공지 분리 텍스트 세팅. => 2008-6-2
				if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
				{
					pclMessage->SetPersonalMsg(buffer);
				}
				else
				{
					pclMessage->SetMsg(buffer);
				}
			}
		}
	}

	//------------------------------------
	// 친구 로그인
	//------------------------------------
	m_pFriendLoginDraw->Action();
	
	//--------------------------------
	// 마을 전쟁 상황 알림 메니져
	//--------------------------------
	m_pVillageWarNotice->Action();

	//--------------------------------
	// 신년 이벤트 상황 알림 메니져
	//--------------------------------

	m_pNewYearEventNotice->Action();

	//------------------------------------
	// 주민 로그인
	//------------------------------------
	m_pResidentLoginDraw->Action();

	//------------------------------------
	// 꼼지 새글 검색 
	//------------------------------------
	//if( (pclClient->CurrentClock / 50) % 20 == 0 ) {

	//	pclMiniHomeUpdates->CheckUpdate();

	//	if( (pclClient->CurrentClock / 50) % (1200 * 10) == 0 )
	//	{
	//		pclMiniHomeUpdates->Request( szID );
	//	}

	//}

	//------------------------------------
	// PC방 PICAMANAGER 이벤트 종료 후 코드 삭제 요망!!
	if( (pclClient->CurrentClock / 50) % 100 )
	{
		if(m_pclPICAManagerCheck->CheckUpdate_PICAManager())
		{
			SI32 siResult = 0;
			siResult = m_pclPICAManagerCheck->m_bPCBang == TRUE ? 1 : 0;
			cltGameMsgRequest_CharServerRTN clCharServerRTN( siResult );
			cltMsg clMsg( GAMEMSG_REQUEST_CHARSERVERRTN, sizeof( clCharServerRTN ), (BYTE*)&clCharServerRTN );
			cltClient *pclclient = ( cltClient * )pclClient;
			pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}

	if( m_pFeastEffectManager )
	{
		if((pclClient->CurrentClock / 50) % 2 == 0)
			m_pFeastEffectManager->Action();
	}

	if ( m_pInterfaceMgr )
	{
		m_pInterfaceMgr->Action( CurrentClock );
	}

	return TRUE;
}


void cltClient::SetTitleText()
{
	//if(pclRegistry)
	//{
		
		TCHAR buffer[256];
		//float fversion = GetVersion() / 100.0f;
		//sprintf(buffer, TEXT("%s-V%.2f[%s]"), NAME, fversion, clSelectedWorldInfo.baseinfo.worldname);
	
		// PCK : 클라이언트에 서버명과 버전을 같이 표시 하도록 변경 (08.11.04)
		StringCchPrintf(buffer, 256, TEXT("[%s_v.%d]"), clSelectedWorldInfo.baseinfo.worldname, pclClient->GetVersion());
		//SetWindowText(GetHwnd(), buffer);
	
		if ( m_pDialog[ STATUSBAR_DLG] )
		{
			/*
			stNStatusBarDlgData NStatusBarData;
			NStatusBarData.Set( buffer, 1 );
			m_pDialog[ STATUSBAR_DLG ]->SetData( &NStatusBarData );*/
            
			((CNStatusBarDlg*)m_pDialog[STATUSBAR_DLG])->SetVersionAndServerName(buffer);
		}		
	
	//}
}
void cltClient::Init()
{

	// 버전을 표시한다. 
	SetTitleText();

	SetMouse(MOUSE_NORMAL);

	// 화면의 영역을 설정한다. 
	ScreenRect.left=0;
	ScreenRect.right=siClientScreenXsize-1;
	ScreenRect.top=0;
	ScreenRect.bottom=siClientScreenYsize-1;

	GP.SetClipArea(0,0,siClientScreenXsize-1,siClientScreenYsize-1 );


}

//---------------------------------------
// 툴바 관련 
//----------------------------------------
BOOL cltClient::CreateToolBar()
{
	SI32 mode = 0;
	if(pclToolBar)return FALSE;

	if(GameMode == GAMEMODE_MAPEDIT)
	{
		mode = 1;
	}
	else
	{
		mode = 0;
	}

	pclToolBar = new cltToolBar(GetHwnd(), GetInstance(), mode);

	return TRUE;
}

TCHAR* cltClient::GetToolTip(SI32 idcommand)
{
	if(pclToolBar== NULL)return NULL;

	return pclToolBar->GetToolTip(idcommand);
}


BOOL cltClient::GetClientSize(RECT* prect)
{
	/*
	if ( siScreenMode == MODE_GRAPHICS1024_768 )
	{
	prect->top		= 0;
	prect->bottom	= 768;	// siClientScreenYsize - (MAP_TILE_YSIZE * 2);
	prect->left		= 0;
	prect->right	= 1024;	// siClientScreenXsize - (MAP_TILE_XSIZE * 2);
	}
	else
	{
	prect->top		= 0;
	prect->bottom	= siClientScreenYsize - (MAP_TILE_YSIZE * 2);
	prect->left		= 0;
	prect->right	= siClientScreenXsize - (MAP_TILE_XSIZE * 2);
	}
	*/

	// KHY - 1218 - 1024x600 해상도 추가. imsi
	if ( siScreenMode == MODE_GRAPHICS1024_768 )
	{
		prect->top		= 0;
		prect->bottom	= siClientScreenYsize - (MAP_TILE_YSIZE * 2);
		prect->left		= 0;
		prect->right	= siClientScreenXsize - (MAP_TILE_XSIZE * 2);

		if(pclToolBar)
		{
			prect->bottom	+= pclToolBar->GetToolBarYsize() + 2;
		}
	}
	else if( siScreenMode == MODE_GRAPHICS1024_600 )
	{
		prect->top		= 0;
		prect->bottom	= siClientScreenYsize - (MAP_TILE_YSIZE * 2);
		prect->left		= 0;
		prect->right		= siClientScreenXsize - (MAP_TILE_XSIZE * 2);

		if(pclToolBar)
		{
			prect->bottom	+= pclToolBar->GetToolBarYsize()+2;
		}
	}
	else
	{
		prect->top		= 0;
		prect->bottom	= siClientScreenYsize - (MAP_TILE_YSIZE * 2);
		prect->left		= 0;
		prect->right	= siClientScreenXsize - (MAP_TILE_XSIZE * 2);

		if(pclToolBar)
		{
			prect->bottom	+= pclToolBar->GetToolBarYsize() + 2;
		}
	}
	
/*
	if ( g_ChatStatusBar && g_ChatStatusBar->IsCreate() )
	{
		prect->bottom += g_ChatStatusBar->GetYsize();
	}
*/
	return TRUE;
}


//-----------------------------------
// 모다리스 대화 상자 처리 관련 
//-----------------------------------
BOOL cltClient::ModalessDlgProc(MSG* pmsg)
{

	cltMapClient* pclMap = (cltMapClient*)pclMapManager->pclCurrentMap;

	if(cltCommonLogic::ModalessDlgProc(pmsg) == TRUE)
	{
		return TRUE;
	}

	return  FALSE;

}


//-----------------------------------
// WM_COMMAND메시지 처리 함수. 
//-----------------------------------
bool cltClient::DoMsg_WM_COMMAND(WPARAM wParam)
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch(LOWORD(wParam))
	{

	case ID_TOOLBAR_HOMEPAGE:
		{
			//TCHAR url[ MAX_PATH ];
			//
			//if( siServiceArea == ConstServiceArea_Korea )
			//	GetGoonZuUrl( url, pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );

			//ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), url, NULL, SW_SHOWNORMAL );

		}
		break;
	case ID_MENUITEM_MYGOMZI:
		{
			SI32 siCharUnique = pclClient->pclCM->CR[1]->GetCharUnique();
			cltGameMsgRequest_Userid clUserid( siCharUnique );
			cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
			cltClient *pclclient = ( cltClient * )pclClient;
			pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;
	case ID_MENUITEM_NEWS:
		{	
			//TCHAR url[ MAX_PATH ];
			//GetSubSiteUrl( url, TEXT("center/board/media_center.asp?menu=new"), pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );

			//ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), url, NULL, SW_SHOWNORMAL );

			////-----------------------------------------------------------
			//// 게임 홈페이지로 가는 퀘스트가 진행중이면, 
			//// 게임 서버로 퀘스트가 완료되었음을 알려준다.
			////-----------------------------------------------------------
			//SI16 questtype, questunique, para1, para2;
			//SI16 questclearcond =  QUEST_CLEAR_COND_NEWS;
			//if(pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
			//{
			//	cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
			//	cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
			//	SendMsgToServer((sPacketHeader*)&clMsg);
			//}

		}
		break;
	//case ID_MENUITEM_KNOWLEDGE:
	//	{
	//		if(m_pSearchDetailDlg)	m_pSearchDetailDlg->Show(BULLETINBOARD_KNOWLEDGE, TEXT(""));
	//	}
	//	break;
	//case ID_MENUITEM_SARANGBANG:
	//	{	
	//		if(m_pSearchDetailDlg)	m_pSearchDetailDlg->Show(BULLETINBOARD_SARANGBANG, TEXT(""));
	//	}
	//	break;
	//case ID_MENUITEM_HYUNGZO:
	//	{
	//		if(m_pSearchDetailDlg)	m_pSearchDetailDlg->Show(BULLETINBOARD_HYUNGZO, TEXT(""));
	//	}
	//	break;

	//case ID_MENUITEM_VILLAGERESIDENT:
	//	{
	//		if ( m_pSearchDetailDlg && pclCM->CR[1]->pclCI->clBI.siHomeVillage > 0 &&
	//			pclCM->CR[1]->pclCI->clBI.siHomeVillage <= CUR_VILLAGE_LIMIT &&
	//			pclVillageManager->GetName(pclCM->CR[1]->pclCI->clBI.siHomeVillage) != NULL )
	//			m_pSearchDetailDlg->Show(BULLETINBOARD_VILLAGERESIDENT, TEXT(""));
	//		else
	//		{
	//			cltMsg clMsg(GAMEMSG_REQUEST_VILLAGEBOARD,0,NULL);
	//			SendMsgToServer((sPacketHeader*)&clMsg);
	//		}
	//	}
	//	break;
	case ID_MENUITEM_MARKET:
		{	
			//if(m_pSearchDetailDlg)	m_pSearchDetailDlg->Show(BULLETINBOARD_MARKET, TEXT(""));
		}
		break;
	case ID_TOOLBAR_CONFIG:
		{
#if 1 //KHY - 20070604 - NHN32- Alt+O 
			if ( m_pDialog[ NSOUNDONOFF_DLG ] )
			{

				if( m_pDialog[ NSOUNDONOFF_DLG ]->IsShow() )
				{
					((CNSoundOnOffDlg*)m_pDialog[ NSOUNDONOFF_DLG ])->Hide();
				}
				else
				{
					((CNSoundOnOffDlg*)m_pDialog[ NSOUNDONOFF_DLG ])->Show();
				}

			}			
#else
			if ( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ NSOUNDONOFF_DLG ] )
			{
				g_SoundOnOffDlg.Hide();
				((CNSoundOnOffDlg*)m_pDialog[ NSOUNDONOFF_DLG ])->Show();
			}
			else
			{
				if(m_pDialog[ NSOUNDONOFF_DLG ])
					((CNSoundOnOffDlg*)m_pDialog[ NSOUNDONOFF_DLG ])->Hide();
				g_SoundOnOffDlg.Show();
			}

#endif
		}
		break;
	case ID_TOOLBAR_ASK:		// 나중에 지식검색으로 바꾼다.
		{
			/*	if(pclAskManager)
			{
			pclAskManager->CreateDlg(GetInstance(), GetHwnd());
			}
			*/
		}
		break;
	

	case ID_TOOLBAR_SCREENSHOT:
		{
			pclCapture->CaptureToJPG(); 

			TCHAR TempWorkDir[MAX_PATH];
			_tgetcwd( TempWorkDir, MAX_PATH );

			TCHAR buffer[256];
			TCHAR* pText = GetTxtFromMgr(960);

			StringCchPrintf(buffer, 256, pText, 
				TempWorkDir,pclCapture->GetFileName());

			//[진성]공지 분리 텍스트 세팅. => 2008-6-2
			if (true == pclClient->IsCountrySwitch( Switch_Notice_Separate))
			{
				pclMessage->SetPersonalMsg(buffer);
			}
			else
			{
				pclMessage->SetMsg(buffer);
			}
		}
		break;
	case ID_TOOLBAR_ITEMMALL:
		{	
			if( pclClient->bItemMallUseSwitch == FALSE)
			{	
				TCHAR* pText = GetTxtFromMgr(962);


				m_pDialog[ NOTIFYMSG_DLG ]->SetText(TEXT("ItemMall"),pText, 
					clSelectedWorldInfo.baseinfo.worldname);
			}
			else
			{
				if( g_bFullScreen )
				{
					ShowWindow( pclclient->GetHwnd(), SW_MINIMIZE );
				}
				//pclClient->pclItemMallManager->CreateItemMallDlg(GetHwnd(),GetInstance(),0);
			}
		}
		break;
	case ID_TOOLBAR_MONSTERINFO:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{	
				CreateInterface( NMONSTERINFO_DLG );
			}
			else
			{
				if( m_pDialog[ NMONSTERINFO_DLG ])
				{
					CreateInterface( NMONSTERINFO_DLG );		// 원래는 hide 코드 였는데, 이제 삭제 코드가 됨.
				}				
			}
		}
		break;

	case ID_TOOLBAR_HUNTINGINFO:
		{	
		}
		break;
	case ID_TOOLBAR_NPCINFO:
		{
			
		}
		break;
	case ID_TOOLBAR_VILLAGEINFO:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode /*&& pclNVillageDetailInfoManager*/ )
			{	
				CreateInterface( NVILLAGEDETAILINFO_DLG );
			}
		}

		break;
	case ID_TOOLBAR_VILLAGERESIDENTINFO:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{
				if ( m_pDialog[ NRESIDENTS_DLG ] )
				{
					( ( CNResidentsDlg * )m_pDialog[ NRESIDENTS_DLG ] )->Show();
				}
			}
		}
		break;
	case ID_TOOLBAR_MASTERINFO:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode ) 
			{
				//if(pclMasterManager && pclMasterManager->m_hDlg)
				//	ShowWindow(pclMasterManager->m_hDlg, SW_HIDE);

				CreateInterface( NMASTERINFO_DLG );				
			}
		}
		break;

	case ID_TOOLBAR_MAGICINFO:
		if ( g_SoundOnOffDlg.m_bFullScreenMode )
		{	
			if( pclClient->IsCountrySwitch(Switch_SkillBook) )
			{
				if ( ((CNNewMagicInfoDlg*)m_pDialog[ NNEWMAGICINFO_DLG ])->IsShow() == false )
				{
					((CNNewMagicInfoDlg*)m_pDialog[ NNEWMAGICINFO_DLG ])->Show();
				}
				else
				{
					((CNNewMagicInfoDlg*)m_pDialog[ NNEWMAGICINFO_DLG ])->Hide();
				}
			}
			else
			{
				if ( ((CNMagicInfoDlg*)m_pDialog[ NMAGICINFO_DLG ])->IsShow() == false )
				{
					((CNMagicInfoDlg*)m_pDialog[ NMAGICINFO_DLG ])->Show();
				}
				else
				{
					((CNMagicInfoDlg*)m_pDialog[ NMAGICINFO_DLG ])->Hide();
				}
			}

			
		}
		break;

	case ID_TOOLBAR_SCHOOLINFO:
		/*
		if(pclSchoolManager)
		{
			pclSchoolManager->CreateDlg(GetInstance(), GetHwnd());
		}
		*/
		break;

	case ID_TOOLBAR_FRIEND:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ NNEWFRIEND_DLG ])
			{
				((CNNewFriendDlg*)m_pDialog[ NNEWFRIEND_DLG ])->Show();
			}
			else
			{
				if(m_pDialog[ NNEWFRIEND_DLG ])
					((CNNewFriendDlg*)m_pDialog[ NNEWFRIEND_DLG ])->Hide();
			}
		}
		break;

	case ID_TOOLBAR_ITEMINFO:
		{


		}
		break;
	case ID_TOOLBAR_SKILL:
		if ( GetGameStatus() == GS_GAME )
		{
			if(pclSkillManager)
			{
					CreateInterface( NSKILL_DLG );
			}
		}
		break;
#ifdef _NEW_INTERFACE_
	case ID_TOOLBAR_QUEST:
		if ( GetGameStatus() == GS_GAME)
		{
			if(pclQuestManager)
			{
				if ( true == pclClient->IsCountrySwitch(Switch_QuestInfoDlg_Renewal) )
				{
					CreateInterface( NQUESTINFODLG_PARENT );
				}
				else
				{
					if ( ((cltNQuestStatus*)m_pDialog[ NQUESTINFO_DLG ])->IsShow() == false )
					{
						((cltNQuestStatus*)m_pDialog[ NQUESTINFO_DLG ])->Show();
					}
					else
					{
						((cltNQuestStatus*)m_pDialog[ NQUESTINFO_DLG ])->Hide();
					}	
				}
			}
		}
		break;
#endif
		// 개인상점 
	case ID_TOOLBAR_PERSONALSHOP:
		{
			cltGameMsgRequest_PersonalShopData clinfo(pclClient->pclCM->CR[1]->GetCharUnique(),FALSE,FALSE); // 만드는창 아니면 FALSE 
			cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);

			SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;
	case ID_MENUITEM_SHOPSEARCH:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode )
			{
				//if( ((CNPersonalShopSearchDlg*)(m_pDialog[ NPERSONALSHOPSEARCH_DLG ]))->IsCreate() == false )
				{
					CreateInterface(NPERSONALSHOPSEARCH_DLG);
				}
			}
		}
		break;
	case ID_TOOLBAR_CHARSTATUS:
		{
			CreateCharStatusDlg();
		}
		break;

	case ID_TOOLBAR_INVENTORY:
		if ( g_SoundOnOffDlg.m_bFullScreenMode )
		{	
#ifndef _NEW_INTERFACE_
			//if ( m_pDialog[ NINVENTORY_DLG ]->IsShow() == false )
			//{
			//	((CNInventory*)m_pDialog[ NINVENTORY_DLG ])->Show();
			//}
			//else
			//{
			//	((CNInventory*)m_pDialog[ NINVENTORY_DLG ])->Hide();
			//}
#else
			if ( m_pDialog[ NNEWINVENTORY_DLG ]->IsShow() == false )
			{
				((CNNewInventory*)m_pDialog[ NNEWINVENTORY_DLG ])->Show();
			}
			else
			{
				((CNNewInventory*)m_pDialog[ NNEWINVENTORY_DLG ])->Hide();
			}
#endif
		}		
		break;
	
	case ID_TOOLBAR_EQUIP:
#ifdef _NEW_INTERFACE_
		
		if ( m_pDialog[ NNEWEQUIPWINDOW_DLG]->IsShow() == false )
		{
			((CNEquipWindow*)m_pDialog[ NNEWEQUIPWINDOW_DLG ])->Show();
		}
		else
		{
			((CNEquipWindow*)m_pDialog[ NNEWEQUIPWINDOW_DLG ])->Hide();
		}
#endif
		break;

	case ID_TOOLBAR_SMALLMAP:
		{	
			if(pclclient->m_pDialog[ NMINIMAP_DLG])
			{
				if(pclclient->m_pDialog[ NMINIMAP_DLG]->IsShow() == false)
					pclclient->m_pDialog[ NMINIMAP_DLG]->Show(TRUE);
			
				((CNMiniMapDlg*)pclclient->m_pDialog[ NMINIMAP_DLG])->Minimi( !((CNMiniMapDlg*)pclclient->m_pDialog[ NMINIMAP_DLG])->GetMinimi()   );
			}
			else
				pclclient->CreateInterface( NMINIMAP_DLG );

		}
		break;
		//case IDC_BUTTON_GUARDIAN:
	case ID_TOOLBAR_FATHER:
		{	
		}
		break;

	case ID_TOOLBAR_HELP:
		{
			if( IsWhereServiceArea(	ConstServiceArea_Japan		|
									ConstServiceArea_English	|
									ConstServiceArea_NHNChina	|
									ConstServiceArea_China		|
									ConstServiceArea_EUROPE
									) 
			)
			{
				pclclient->CreateInterface( NHINT_DLG );
				((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show(TEXT("NULL"));
			}
		}
		break;
	}
	return true;
}


// 게임의 속도를 일정하게 만든다. 
BOOL cltClient::GameDelay()
{

	//---------------------------------
	// 태곤테스트
	//---------------------------------
	//g_graphics.SetSkipDraw( FALSE );
	//GameDraw();	
	//return true;
	//---------------------------------


	// 12월 12일 적용..
	// 문제가 생긴다면 client.h에 #define JEWAN_FRAME_SKIP을 제거해주세요...
#ifdef JEWAN_FRAME_SKIP

	if( m_dwNextFrameTick == 0 ) 
	{
		m_dwNextFrameTick = GetTickCount();
		m_dwDelayTick = 0;

	} else {
		double diff = m_dwNextFrameTick - (double)GetTickCount();
		if( diff > 0 ) {
			if( diff >= 15 ) Sleep(10);
			return FALSE;
		}

		m_dwDelayTick += (double)GetTickCount() - m_dwNextFrameTick;
	}


	if( m_dwDelayTick > siFrameDelay * 10 ) {

		double tick = GetTickCount();

		while( m_dwNextFrameTick > tick ) {
			m_dwNextFrameTick -= siFrameDelay;
		}

		m_dwNextFrameTick += siFrameDelay;
		m_dwDelayTick = 0;

		return FALSE;
	} 
	/*
	else
	if( m_dwDelayTick > (double)siFrameDelay * 2 ) {
	// Frame Skip
	m_dwNextFrameTick += siFrameDelay;			

	if( g_graphics.m_bSkipDraw == FALSE ) {
	g_graphics.SetSkipDraw( TRUE );
	m_dwDelayTick -= siFrameDelay;
	//GameDraw( 0 );
	} else {
	g_graphics.SetSkipDraw( FALSE );
	GameDraw( 0 );
	}
	} 
	*/
	else 
	{

		m_dwNextFrameTick += siFrameDelay;

		g_graphics.SetSkipDraw( FALSE );

		GameDraw();	
	}

	return TRUE;

#else
	// 기존..

	static DWORD dwlastclock = 0;
	static bool bDrawswitch = false;

	if(TABS(CurrentClock - dwlastclock) >= siFrameDelay)
	{
		// 이미 출력을 시작했다면 스킵은 불가.
		if(bDrawswitch == true)
		{
			bDrawswitch = false;
			bDrawSkipSwitch = false;
		}
		// 너무 늦었으면 출력을 스킵한다. 
		else if(bDrawSkipSwitch == true)
		{
			bDrawSkipSwitch = false;
		}
		else
		{
			if(TABS(CurrentClock - dwlastclock) >= siFrameDelay + 20)
			{
				bDrawSkipSwitch = true;
			}
			else
			{
				bDrawSkipSwitch = false;
			}
		}

		dwlastclock = CurrentClock;
		return TRUE;
	}

	//Sleep(1);


	// 10ms이상 여유가 있다면, 
	if(TABS(CurrentClock - dwlastclock) + 10 < siFrameDelay)
	{
		GameDraw();
		bDrawswitch = true;
	} 


#endif
}



// 명령을 처리하는 함수. 
BOOL cltClient::OrderAction()
{
	// 건물 정보를 받을 때까지 대기해야 한다. 
	if(bWaitVillageStructureInfoSwitch == true)
	{
		return FALSE;
	}

	cltOrderData order;

	// 사용자 입력 명령이 있다면, 
	if(pclOrderManager->clOrder.Action(&order) == TRUE)
	{
		// 클라이언트인 경우 서버로 명령을 보내는 처리를 한다. 
		OrderOperation(order);
	}

	return TRUE;
}

// 말의 불만 사항을 표시한다. 
void cltClient::HorseStatusShow(TCHAR* pnormaltext)
{
	SI32 id = 1;
	TCHAR buffer[1024]	= TEXT("");
	TCHAR text[1024]		= TEXT("");

	cltHorse* pclhorse = pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse();
	if(pclhorse == NULL)					return;

	cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
	SI32 horseid = pclChar->siSlaveID;

	// 말이 있다면, 
	if(pclhorse->siHorseUnique)
	{
		//요구 조건이 있는 상태라면, 
		if(pclhorse->siNeed)
		{
			//TCHAR* pText = GetTxtFromMgr(963);

			//_tcscpy(buffer, pText);

			// 요구 조건을 얻어온다. 
			if(pclHorseManager->GetNeedText(pclhorse, text, 1024) == true)
			{
				StringCchCat(buffer, 1024, text);
			}
		}

		if(pnormaltext)
		{
			StringCchCat(buffer, 1024, pnormaltext);
		}


		if(_tcscmp(buffer, TEXT("")) != 0)
		{
			if(pclCM->IsAlive(horseid))
			{
				if(pclChar->pclDrawChat)
				{
					pclChar = (cltCharClient*)pclCM->CR[horseid];
					pclChar->pclDrawChat->Set(pclDrawChatSurface, buffer, false, NPC_CHATTING );
				}
			}
			else
			{
				if(pclChar->pclDrawChat)
				{
					pclChar = (cltCharClient*)pclCM->CR[id];
					pclChar->pclDrawChat->Set(pclDrawChatSurface, buffer, false, NPC_CHATTING );
				}
			}
		}

	}

}

// 말의 불만 사항을 표시를 삭제한다.
void cltClient::HorseStatusHide()
{
	SI32 id = 1;
	cltHorse* pclhorse = pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse();
	if(pclhorse == NULL)					return;

	cltCharClient* pclChar = (cltCharClient*)pclCM->CR[id];
	SI32 horseid = pclChar->siSlaveID;

	// 말이 있다면, 
	if(pclhorse->siHorseUnique)
	{
		if (horseid > 0)
		{
			pclChar = (cltCharClient*)pclCM->CR[horseid];
			pclChar->pclDrawChat->StopDraw();
		}
		pclChar = (cltCharClient*)pclCM->CR[id];
		pclChar->pclDrawChat->StopDraw();
	}
}



//----------------------------------------------------------------
// CharStatus 
//----------------------------------------------------------------
BOOL cltClient::CreateCharStatusDlg()
{	
	if( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		if( m_pDialog[ CLTNCHARSTATUS_DLG ] && m_pDialog[ CLTNCHARSTATUS_DLG ]->IsCreate() )
		{
			CreateInterface( CLTNCHARSTATUS_DLG );
		}
		else
		{
			CreateInterface( CLTNCHARSTATUS_DLG );

			if( m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)m_pDialog[ CLTNCHARSTATUS_DLG ])->IsShow() == false )
				((cltNCharStatus*)m_pDialog[ CLTNCHARSTATUS_DLG ])->Show();
			else if( m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)m_pDialog[ CLTNCHARSTATUS_DLG ])->IsShow() == true ) 
				((cltNCharStatus*)m_pDialog[ CLTNCHARSTATUS_DLG ])->Hide();
		}

		//-----------------------------------------------------------
		// 캐릭터 창을 여는 퀘스트가 진행중이라면 
		// 게임 서버로 퀘스트가 완료되었음을 알려준다.
		//-----------------------------------------------------------
	
	pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENCHARSTATUS);

		return TRUE;
	}

	
	return FALSE;
}

// personid에게 귓말을 요청한다. 
bool cltClient::RequestSecretChatByPersonID(SI32 personid,TCHAR * buf)
{
	if(personid <= 0)return false;

	if ( buf )
	{
		if ( pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
		{
#ifdef _IAF_EXPORT
			if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
			{
				TCHAR ResultBuf[256] = "";
				if( 1 == IAF::Filtering( buf, ResultBuf, sizeof(ResultBuf) ) )
				{
					TCHAR* pTitle = GetTxtFromMgr(1853);
					TCHAR* pText = GetTxtFromMgr(1854);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					return FALSE ;
				}
			}
#else
			if ( m_pCAbuseFilter->CheckAbuse(buf) == false )
			{
				TCHAR* pTitle = GetTxtFromMgr(1853);
				TCHAR* pText = GetTxtFromMgr(1854);
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return FALSE ;
			}
#endif
		}

		cltGameMsgRequest_MakeSecretChatChannel clinfo(personid, 0,TEXT(""),buf);
		cltMsg clMsg(GAMEMSG_REQUEST_MAKESECRETCHATCHANNEL, sizeof(clinfo), (BYTE*)&clinfo);
		SendMsgToServer((sPacketHeader*)&clMsg);
	}
	else
	{
		cltGameMsgRequest_MakeSecretChatChannel clinfo(personid, 0,TEXT(""));
		cltMsg clMsg(GAMEMSG_REQUEST_MAKESECRETCHATCHANNEL, sizeof(clinfo), (BYTE*)&clinfo);
		SendMsgToServer((sPacketHeader*)&clMsg);
	}

	return true;
}

// charunique에게 귓말을 요청한다. 
bool cltClient::RequestSecretChatByCharUnique(SI32 charunique,TCHAR * buf)
{
	if(charunique <= 0)return false;

	if ( buf )
	{
		if ( pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
		{
#ifdef _IAF_EXPORT
			if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
			{
				TCHAR ResultBuf[256] = "";
				if( 1 == IAF::Filtering( buf, ResultBuf, sizeof(ResultBuf) ) )
				{
					TCHAR* pTitle = GetTxtFromMgr(1853);
					TCHAR* pText = GetTxtFromMgr(1854);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					return FALSE ;
				}
			}
#else
			if ( m_pCAbuseFilter->CheckAbuse(buf) == false )
			{
				TCHAR* pTitle = GetTxtFromMgr(1853);
				TCHAR* pText = GetTxtFromMgr(1854);
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return FALSE ;
			}
#endif
		}


		if ( pclCM->CR[1]->GetCharUnique() == charunique )//[영진]
		{
			//TCHAR* pTitle = GetTxtFromMgr(312);
			//TCHAR* pText = GetTxtFromMgr(312);
			//m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
			//pclMessage->SetMsg(pText);
			return FALSE ;
		}

		cltGameMsgRequest_MakeSecretChatChannel clinfo(0, charunique,TEXT(""),buf);
		cltMsg clMsg(GAMEMSG_REQUEST_MAKESECRETCHATCHANNEL, sizeof(clinfo), (BYTE*)&clinfo);
		SendMsgToServer((sPacketHeader*)&clMsg);
	}
	else
	{
		cltGameMsgRequest_MakeSecretChatChannel clinfo(0, charunique,TEXT(""));
		cltMsg clMsg(GAMEMSG_REQUEST_MAKESECRETCHATCHANNEL, sizeof(clinfo), (BYTE*)&clinfo);
		SendMsgToServer((sPacketHeader*)&clMsg);
	}

	return true;
}

bool cltClient::RequestSecretChatByCharName(TCHAR * szName,TCHAR * buf )
{
	if ( !_tcscmp(szName,TEXT("")) || szName == NULL ) return false ;

	if ( buf )
	{
		if ( pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
		{
#ifdef _IAF_EXPORT
			if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
			{
				CHAR ResultBuf[256] = "";
				if( 1 == IAF::Filtering( buf, ResultBuf, sizeof(ResultBuf) ) )
				{
					TCHAR* pTitle = GetTxtFromMgr(1853);
					TCHAR* pText = GetTxtFromMgr(1854);
					m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					return FALSE ;
				}
			}
#else
			if ( m_pCAbuseFilter->CheckAbuse(buf) == false )
			{
				TCHAR* pTitle = GetTxtFromMgr(1853);
				TCHAR* pText = GetTxtFromMgr(1854);
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return FALSE ;
			}
#endif
		}


		cltGameMsgRequest_MakeSecretChatChannel clinfo(0, 0,szName,buf);
		cltMsg clMsg(GAMEMSG_REQUEST_MAKESECRETCHATCHANNEL, sizeof(clinfo), (BYTE*)&clinfo);
		SendMsgToServer((sPacketHeader*)&clMsg);
	}
	else
	{
		cltGameMsgRequest_MakeSecretChatChannel clinfo(0, 0,szName);
		cltMsg clMsg(GAMEMSG_REQUEST_MAKESECRETCHATCHANNEL, sizeof(clinfo), (BYTE*)&clinfo);
		SendMsgToServer((sPacketHeader*)&clMsg);
	}

	return true ;
}

// 배경 음악을 연주한다. 
void cltClient::PlayBackgroundMusic(SI32 mapindex)
{
	pclMusic->pclSoundManager->BackGroundSoundStop();

	TCHAR filename[256];
	StringCchCopy( filename, 256 , pclMapManager->clUnitMapInfo[mapindex].szBGMFileName );
 
/*
#ifdef EVENT_CHRISTMAS

// 마을이라면
SI32 randdata = rand()%3;

if(pclMapManager->pclCurrentMap->siVillageUnique)
{
	if ( randdata ==  0 )
	{
		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_JingGleBell01.ogg"), true, false );
	}
	else if( randdata ==  1 )
	{
		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Carol02.ogg"), true, false );
	}
	else
	{
			pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Carol01.ogg"), true, false );
	}
		
}
// 배경음악이 등록되어있지 않은 경우
else if( filename == NULL || _tcscmp( filename, TEXT("") ) == 0 )
{
	if ( randdata ==  0 )
	{
		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_JingGleBell01.ogg"), true, false );
	}
	else if( randdata ==  1 )
	{
		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Carol02.ogg"), true, false );
	}
	else
	{
		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Carol01.ogg"), true, false );
	}
}
// 배경음악이 등록되어있는 경우
else
{
	pclMusic->pclSoundManager->PlayBackGroundFile( pclMapManager->clUnitMapInfo[mapindex].szBGMFileName, true, false );
}

#else
*/
	
	//GetSystemTime(&sTime);
	bool Christmas = false;

	//KHY - 1113 - 크리스마스 이벤트
	if(pclClient->pclEventTimeManager->InEventTime("ChristmasEvent", &sTime) )
	{
		Christmas = true;
	}

	// 마을이라면
	SI32 randdata = rand();
 
	if( pclMapManager != NULL &&
		pclMapManager->pclCurrentMap != NULL &&
		pclMapManager->pclCurrentMap->siVillageUnique)
	{
		// [추가 : 황진성 2007. 12. 3 => 크리스마스에 마을 음악을 캐롤로 바꿔.]
		if(true == Christmas)
		{
			if ( randdata % 2 == 1 )
			{
				pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Carol01.ogg"), true, false );
			}
			else
			{
				pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_JingGleBell01.ogg"), true, false );
			}
		}
		else
		{
			if ( randdata % 2 == 1 )
			{
				pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Village01.ogg"), true, false );
			}
			else
			{
				pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Village02.ogg"), true, false );
			}
		}
	}
	// 배경음악이 등록되어있지 않은 경우
	else if( filename == NULL || _tcscmp( filename, TEXT("") ) == 0 )
	{
		// [추가 : 황진성 2007. 12. 3 => 크리스마스에 배경 음악을 캐롤로 바꿔.]
		if(true == Christmas)
		{
			switch ( randdata % 2 )
			{
			case 0: 
				pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_carol11.ogg"), true, false );
				break;
			case 1: 
				pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_carol12.ogg"), true, false );
				break;
			}
		}
		else
		{
			switch ( randdata % 3 )
			{
			case 0: 
				pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Main1.ogg"), true, false );
				break;
			case 1: 
				pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Main2.ogg"), true, false );
				break;
			default:
				pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Main3.ogg"), true, false );
				break;
			}
			pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Ambience/Main1.ogg"), true, false );
		}
	}
	// 배경음악이 등록되어있는 경우
	else
	{
		pclMusic->pclSoundManager->PlayBackGroundFile( pclMapManager->clUnitMapInfo[mapindex].szBGMFileName, true, false );
	}

//#endif

	//SI32 randdata = rand() ;
	//
	//// 마을이라면,
	//if(pclMapManager->pclCurrentMap->siVillageUnique)
	//{
	//	if ( randdata % 2 == 1 )
	//	{
	//		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Village01.ogg"), true, false );
	//	}
	//	/*else if ( randdata % 3 == 1 )
	//	{
	//	pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Carol02.ogg"), true, false );
	//	}*/
	//	else
	//	{
	//		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Village02.ogg"), true, false );
	//	}
	//}
	//// 남한산성 수어장대.
	//else if(mapindex == 93 || mapindex == 97 || mapindex == 112 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Castle01.ogg"), true, false );
	//}
	//// 웅녀동굴1층, 2층. 
	//else if(mapindex == 90 || mapindex == 91 || mapindex == 107)
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Bear01.ogg"), true, false );
	//}
	//// 작은풀숲1, 2층 
	//else if(mapindex == 99 || mapindex == 100 || mapindex == 101)
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_SmallForest01.ogg"), true, false );
	//}
	//// 나무소굴 
	//else if(mapindex == 95 || mapindex == 96)
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Wood01.ogg"), true, false );
	//}
	//// 산타작업장.
	//else if(mapindex == 102)
	//{
	//	if(randdata % 3 == 2 )
	//	{
	//		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Carol01.ogg"), true, false );
	//	}
	//	else if(randdata % 3 == 1 )
	//	{
	//		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_JingGleBell01.ogg"), true, false );
	//	}
	//	else
	//	{
	//		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Carol02.ogg"), true, false );
	//	}
	//}
	//// 용궁 1층, 2층
	//else if(mapindex == 94 || mapindex == 109)
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_SeaPalace01.ogg"), true, false );
	//}
	//// 깨비집
	//else if ( mapindex == 98 || mapindex == 110 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Goblin.ogg"),true,false);
	//}
	//// 지하감옥
	//else if ( mapindex == 116 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_UnderPrison.ogg"),true,false);
	//}
	//// 경상우수영
	//else if ( mapindex == 114 || mapindex == 163 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Porthole.ogg"),true,false);
	//}
	//// 경복궁
	//else if ( mapindex == MAPINDEX_PALACE )
	//{
	//	//if ( randdata % 2 )
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Palace.ogg"),true,false);
	//	//else
	//	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Carol02.ogg"),true,false);
	//}
	//else if ( mapindex == MAPINDEX_CHINABORDER )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_China.ogg"),true,false);
	//}
	//// 하회마을
	//else if ( mapindex == 152 || mapindex == 167 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Hahoi.ogg"),true,false);
	//}
	//// 한과마을
	//else if ( mapindex == 153 || mapindex == 166 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_KoreanSweets.ogg"),true,false);
	//}
	//// 귀곡산장
	//else if ( mapindex == 113 || mapindex == 164 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_GhostVilla.ogg"),true,false);
	//}
	//// 금하굴
	//else if ( mapindex == 103 || mapindex == 151 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Gumhatunnel.ogg"),true,false);
	//}
	//// 백두산
	//else if ( mapindex == 170 || mapindex == 171 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_MTBaeckDoo.ogg"),true,false);
	//}
	////  초보자사냥터
	//else if ( mapindex == 108 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Beginner01.ogg"),true,false);
	//}
	//// 흑의군대 전쟁맵
	//else if ( mapindex >= BLACKARMY_MAP_STARTINDEX && mapindex < BLACKARMY_MAP_STARTINDEX + MAX_BLACKARMY_MAP_CNT )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Battlezone.ogg"),true,false);
	//}
	//// 고엔라쿠
	//else if ( mapindex == 120 || mapindex == 121 || mapindex == 122 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Goenraku.ogg"),true,false);
	//}
	//// 아오지
	//else if ( mapindex == 157 || mapindex == 168 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Bear01.ogg"),true,false);
	//}
	//// 얼음성
	//else if ( mapindex == 154 || mapindex == 169 || mapindex == 155 )
	//{
	//	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_IceCastle.ogg"),true,false);
	//}


	//// 천상계
	////else if ( mapindex == 156 )
	////{
	////	pclMusic->pclSoundManager->PlayBackGroundFile(TEXT("./Music/Music_Heaven01.ogg"),true,false);
	////}
	//// 기타. 
	//else
	//{
	//	if(randdata % 3 == 2)
	//	{
	//		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Main1.ogg"), true, false );
	//	}
	//	else if (randdata % 3 == 1)
	//	{
	//		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Main2.ogg"), true, false );
	//	}
	//	else
	//	{
	//		pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_Main3.ogg"), true, false );
	//	}
	//}

}

void cltClient::GetNearMapInfo(SI32 mapindex,SI32 x,SI32 y,TCHAR * buf , SI16 txtSize)
{
	if ( mapindex < 0 || mapindex >= MAX_MAP_NUMBER ) return ;
	TCHAR * pmapname = pclMapManager->GetMapName(mapindex) ;

	SI32 siVillageUnique = 0 ;
	TCHAR strdir[256] = TEXT("") ;
	TCHAR tempbuf[256] = TEXT("") ;

	if(mapindex == 0)
	{
		siVillageUnique = pclVillageManager->FindNearVillage(x, y);
		SI32 vilx = pclVillageManager->pclVillageInfo[siVillageUnique]->PosX;
		SI32 vily = pclVillageManager->pclVillageInfo[siVillageUnique]->PosY;

		GetDirectionStr(vilx, vily, x, y, strdir, 256);
		pmapname = pclMapManager->GetMapName(siVillageUnique);
		StringCchPrintf(tempbuf, 256, TEXT("%s %s"),		pmapname, strdir);
		StringCchCopy(buf, txtSize, tempbuf);
	}
	else
	{
		TCHAR* pText[25];// = NULL;
		TCHAR* pWhereText[25];// = NULL;

		SI32 i;
		for(i=0;i<25;++i)
		{
			pText[i] = GetTxtFromMgr(964+i);
		}

		for(i=0;i<25;++i)
		{
			pWhereText[i] = GetTxtFromMgr(989+i);
		}

		StringCchCopy(strdir, 256, TEXT(""));
		if(_tcsstr(pmapname,      pText[0]))		{	StringCchCopy(strdir, 256,  pWhereText[0]);	}
		else if(_tcsstr(pmapname, pText[1]))		{	StringCchCopy(strdir, 256,  pWhereText[1]);	}
		else if(_tcsstr(pmapname, pText[2]))		{	StringCchCopy(strdir, 256,  pWhereText[2]);	}
		else if(_tcsstr(pmapname, pText[3]))			{	StringCchCopy(strdir, 256,  pWhereText[3]);	}
		else if(_tcsstr(pmapname, pText[4]))			{	StringCchCopy(strdir, 256, pWhereText[4]);	}
		else if(_tcsstr(pmapname, pText[5]))	{	StringCchCopy(strdir, 256,  pWhereText[5]);		}

		// 지도 바꿀때 꼭 확인할껏 
		else if(_tcsstr(pmapname, pText[6]))		{	StringCchCopy(strdir, 256,  pWhereText[6]);		}
		else if(_tcsstr(pmapname, pText[7]))		{	StringCchCopy(strdir, 256,  pWhereText[7]);		}
		else if(_tcsstr(pmapname, pText[8]))		{	StringCchCopy(strdir, 256,  pWhereText[8]);		}
		else if(_tcsstr(pmapname, pText[9]))		{	StringCchCopy(strdir, 256,  pWhereText[9]);		}
		else if(_tcsstr(pmapname, pText[10]))		{	StringCchCopy(strdir, 256, pWhereText[10]);		}
		else if(_tcsstr(pmapname, pText[11]))			{	StringCchCopy(strdir, 256,  pWhereText[11]);		}

		else if(_tcsstr(pmapname, pText[12]))		{	StringCchCopy(strdir, 256,  pWhereText[12]);		}
		else if(_tcsstr(pmapname, pText[13]))		{	StringCchCopy(strdir, 256,  pWhereText[13]);		}
		else if(_tcsstr(pmapname, pText[14]))			{	StringCchCopy(strdir, 256,  pWhereText[14]);		}
		else if(_tcsstr(pmapname, pText[15]))			{	StringCchCopy(strdir, 256,  pWhereText[15]);		}
		else if(_tcsstr(pmapname, pText[16]))		{	StringCchCopy(strdir, 256,  pWhereText[16]);		}

		else if(_tcsstr(pmapname, pText[17]))		{	StringCchCopy(strdir, 256,  pWhereText[17]);		}
		else if(_tcsstr(pmapname, pText[18]))		{	StringCchCopy(strdir, 256,  pWhereText[18]);		}
		else if(_tcsstr(pmapname, pText[19]))			{	StringCchCopy(strdir, 256,  pWhereText[19]);		}
		else if(_tcsstr(pmapname, pText[20]))			{	StringCchCopy(strdir, 256,  pWhereText[20]);		}
		else if(_tcsstr(pmapname, pText[21]))			{	StringCchCopy(strdir, 256,  pWhereText[21]);		}
		else if(_tcsstr(pmapname, pText[22]))			{	StringCchCopy(strdir, 256,  pWhereText[22]);		}
		else if(_tcsstr(pmapname, pText[23]))			{	StringCchCopy(strdir, 256,  pWhereText[23]);		}
		else if(_tcsstr(pmapname, pText[24]))			{	StringCchCopy(strdir, 256, pWhereText[24]);		}

		if(_tcslen(strdir))	StringCchPrintf(tempbuf, 256, TEXT("%s"),		strdir);
		else				StringCchPrintf(tempbuf, 256, TEXT("%s"),		pmapname);
		StringCchCopy(buf, txtSize, tempbuf);
	}
}

bool cltClient::GetDirectionStr(SI32 sx, SI32 sy, SI32 dx, SI32 dy, TCHAR *szText, SI16 txtSize)
{
	SI32  rsx, rsy, rdx, rdy;
	rsx = sx*2-sy*2;	rsy = sy*1+sx*1;
	rdx = dx*2-dy*2;	rdy = dy*1+dx*1;

	switch(DecideDirection8(rsx, rsy, rdx, rdy))
	{
	case SOUTH|EAST:	
		{
			TCHAR* pText = GetTxtFromMgr(1014);


			StringCchCopy(szText, txtSize, pText);	
		}
		break;
	case SOUTH:			{
		TCHAR* pText = GetTxtFromMgr(1015);


		StringCchCopy(szText, txtSize, pText);	
						}
						break;
	case SOUTH|WEST:	{
		TCHAR* pText = GetTxtFromMgr(1016);


		StringCchCopy(szText, txtSize, pText);	
						}
						break;
	case WEST:			{
		TCHAR* pText = GetTxtFromMgr(1017);


		StringCchCopy(szText, txtSize, pText);	
						}
						break;
	case WEST|NORTH:	{
		TCHAR* pText = GetTxtFromMgr(1018);


		StringCchCopy(szText, txtSize, pText);	
						}
						break;
	case NORTH:			{
		TCHAR* pText = GetTxtFromMgr(1019);


		StringCchCopy(szText, txtSize, pText);	
						}
						break;
	case NORTH|EAST:	{
		TCHAR* pText = GetTxtFromMgr(1020);


		StringCchCopy(szText, txtSize, pText);	
						}
						break;
	case EAST:			{
		TCHAR* pText = GetTxtFromMgr(1021);


		StringCchCopy(szText, txtSize, pText);	
						}
						break;
	default:			{
		TCHAR* pText = GetTxtFromMgr(1022);


		StringCchCopy(szText, txtSize, pText);	
						}
						break;
	}

	return true;
}

// 지도 이동시 불필요한 인터페이스를 지운다. 
void cltClient::DestroyNeedlessInterface()
{

	if( m_pDialog[ NGUILD_DLG ] ) 						m_pDialog[ NGUILD_DLG ]->DeleteDialog();	// 길드 
	if( m_pDialog[ NGUILDSTR_DLG ] )					m_pDialog[ NGUILDSTR_DLG ]->DeleteDialog();	// 길드 상단
	if( m_pDialog[ NCITYHALL_DLG ] ) 					m_pDialog[ NCITYHALL_DLG ]->DeleteDialog();
	if( m_pDialog[ NPOSTOFFICE_DLG ] ) 					m_pDialog[ NPOSTOFFICE_DLG ]->DeleteDialog();
	if( m_pDialog[ NHORSEMARKET_DLG ] ) 				m_pDialog[ NHORSEMARKET_DLG ]->DeleteDialog();
	if( m_pDialog[ NREALESTATEMARKET_DLG ] ) 			m_pDialog[ NREALESTATEMARKET_DLG ]->DeleteDialog();
	if( m_pDialog[ NFEAST_DLG ] ) 						m_pDialog[ NFEAST_DLG ]->DeleteDialog();
	if( m_pDialog[ NSUMMONMARKET_DLG ] ) 				m_pDialog[ NSUMMONMARKET_DLG ]->DeleteDialog();
	if( m_pDialog[ NSUMMONHEROMARKET_DLG ] ) 			m_pDialog[ NSUMMONHEROMARKET_DLG ]->DeleteDialog();
	if( m_pDialog[ NHUNT_DLG ] ) 						m_pDialog[ NHUNT_DLG ]->DeleteDialog();
	if( m_pDialog[ NHOUSE_DLG ] ) 						m_pDialog[ NHOUSE_DLG ]->DeleteDialog();
	if( m_pDialog[ NHOUSEUSERINFOBASE_DLG ])			m_pDialog[ NHOUSEUSERINFOBASE_DLG ]->DeleteDialog();
	if( m_pDialog[ NHOUSENAMECHANGE_DLG ])				m_pDialog[ NHOUSENAMECHANGE_DLG ]->DeleteDialog();
	if( m_pDialog[ NHOUSEEXTENSION_DLG ])				m_pDialog[ NHOUSEEXTENSION_DLG ]->DeleteDialog();
	if( m_pDialog[ NBANK_DLG ] ) 						m_pDialog[ NBANK_DLG ]->DeleteDialog();
	if( m_pDialog[ NMINE_DLG] )							m_pDialog[ NMINE_DLG ]->DeleteDialog();
	if( m_pDialog[ NMARKET_DLG ] ) 						m_pDialog[ NMARKET_DLG ]->DeleteDialog();
	if( m_pDialog[ NMARKET_SUB_DLG ] ) 					m_pDialog[ NMARKET_SUB_DLG ]->DeleteDialog();
	if( m_pDialog[ NSTOCK_DLG ] ) 						m_pDialog[ NSTOCK_DLG ]->DeleteDialog();
	if( m_pDialog[ NNPCTRADE1_DLG ])					m_pDialog[ NNPCTRADE1_DLG ]->DeleteDialog();
	if( m_pDialog[ NINVESTMENT_DLG ])					m_pDialog[ NINVESTMENT_DLG ]->DeleteDialog();
	if( m_pDialog[ NNPCMENU_DLG ])						m_pDialog[ NNPCMENU_DLG ]->DeleteDialog();
	if( m_pDialog[ NTRADEMERCHANT_DLG ] )				m_pDialog[ NTRADEMERCHANT_DLG ]->DeleteDialog(); 	// 무역상인 생성되어 있으면 지운다
	if( m_pDialog[ NTRADEMERCHANTHORSE_DLG ] )			m_pDialog[ NTRADEMERCHANTHORSE_DLG ]->DeleteDialog();// 말상인

	// 지도 이용시 데미안 터틀백 다이얼로그를 지운다.
	if( m_pDialog[ NDAILYQUEST2_DLG  ])					m_pDialog[ NDAILYQUEST2_DLG ]->DeleteDialog();
	if( m_pDialog[ NDAILYQUEST3_DLG  ])					m_pDialog[ NDAILYQUEST3_DLG ]->DeleteDialog();
	if( m_pDialog[ NNPCREPAIR_DLG  ])					m_pDialog[ NNPCREPAIR_DLG ]->DeleteDialog();

	if( m_pDialog[ NFOREIGNMERCHANTBUYCASHITEM_DLG ] )	m_pDialog[ NFOREIGNMERCHANTBUYCASHITEM_DLG ]->DeleteDialog(); 	// 캐쉬 아이템 명나라
	if( m_pDialog[ NFOREIGNMERCHANTBUYITEM_DLG ] )		m_pDialog[ NFOREIGNMERCHANTBUYITEM_DLG ]->DeleteDialog(); 	// 일반 아이템 명나라
	if( m_pDialog[ NFOREIGNMERCHANT_DLG ] )				m_pDialog[ NFOREIGNMERCHANT_DLG ]->DeleteDialog(); 	// 명나라
	if( m_pDialog[ NMUDANG_DLG ] )						m_pDialog[ NMUDANG_DLG ]->DeleteDialog(); 			// 선무당(미스터M)
	if( m_pDialog[ NTRADESERVERBASE_DLG ] )				m_pDialog[ NTRADESERVERBASE_DLG ]->DeleteDialog(); 	// 야린스키(무역NPC)
	if( m_pDialog[ NAQTRADEWINDOW_DLG ] )				m_pDialog[ NAQTRADEWINDOW_DLG ]->DeleteDialog(); 	// 호설암이면(아큐거래상)
	if( m_pDialog[ NWENNYMAN_DLG ] )					m_pDialog[ NWENNYMAN_DLG ]->DeleteDialog();			// 혹부리영감이면,(이호식)
	if( m_pDialog[ NPLAYMATCH_DLG ] )					m_pDialog[ NPLAYMATCH_DLG ]->DeleteDialog();		// 결투장NPC 이면
	if( m_pDialog[ NRPLIST_DLG ] )						m_pDialog[ NRPLIST_DLG ]->DeleteDialog();			// 추천인NPC 이면
	if( m_pDialog[ NMANUFACTURENPC_DLG ] )				m_pDialog[ NMANUFACTURENPC_DLG ]->DeleteDialog(); 	// 제조NPC이면
	if( m_pDialog[ NMARKETCONDITIONS_DLG ] )			m_pDialog[ NMARKETCONDITIONS_DLG ]->DeleteDialog();	// 시세확인  창
	if( m_pDialog[ NHONGGILDONG_DLG ] )					m_pDialog[ NHONGGILDONG_DLG ]->DeleteDialog();		// 전투향상술
	if( m_pDialog[ NRACOONDODGE_EVENT_RESERVELIST_DLG ] )		m_pDialog[ NRACOONDODGE_EVENT_RESERVELIST_DLG ]->DeleteDialog();	//라쿤피하기 이벤트
	if( m_pDialog[ NRACOONDODGE_EVENT_DLG ] )					m_pDialog[ NRACOONDODGE_EVENT_DLG ]->DeleteDialog();				//라쿤피하기 이벤트
	if( m_pDialog[ NSELECT_QUEST_BASE_DLG ] )			m_pDialog[ NSELECT_QUEST_BASE_DLG ]->DeleteDialog();	// 파티매칭 1번 창 (카테고리 선택창)
	if( m_pDialog[ NPARTY_MATCHING_BASE_DLG ] )			m_pDialog[ NPARTY_MATCHING_BASE_DLG ]->DeleteDialog();	// 파티매칭 2번 창 (파티매칭 리스트창)
	if( m_pDialog[ NCREATE_PARTY_BASE_DLG ] )			m_pDialog[ NCREATE_PARTY_BASE_DLG ]->DeleteDialog();	// 파티매칭 3번 창 (파티매칭 생성창)

	if( m_pDialog[ NDAILYQUEST_DLG ] != NULL && m_pDialog[ NDAILYQUEST_DLG ]->IsCreate() == true )
		CreateInterface( NDAILYQUEST_DLG );

	// 인던 정보창 이면, 잠시 창을 보이지 않게 한다.
	// DeleteDialog()를 하면, 싱크가 맞지 않는다.
	if( m_pDialog[NINSTANST_DUNGEON_RUN_INFO_DLG] != NULL )
		m_pDialog[NINSTANST_DUNGEON_RUN_INFO_DLG]->Show( false );

	if( m_pDialog[PARTYHALL_IN_BASE_DLG] != NULL )
		m_pDialog[PARTYHALL_IN_BASE_DLG]->Show( false );

	if( m_pDialog[NBUFF_ROULETTE_DLG] != NULL )
		m_pDialog[ NBUFF_ROULETTE_DLG ]->DeleteDialog();

	if( m_pDialog[NCHARBUFF_ROULETTE_DLG] != NULL )
		m_pDialog[ NCHARBUFF_ROULETTE_DLG ]->DeleteDialog();

	if( m_pDialog[NEWYEAR_ROULETTE_DLG] != NULL )
		m_pDialog[ NEWYEAR_ROULETTE_DLG ]->DeleteDialog();

	if( m_pDialog[NNEWMARKET_DLG] != NULL )
		m_pDialog[NNEWMARKET_DLG]->DeleteDialog();

	if( m_pDialog[NBALANCECURRENCY_DLG] != NULL )
		m_pDialog[NBALANCECURRENCY_DLG]->DeleteDialog();
	
	// [기형] 휴면 계정 관련
	if( m_pDialog[NDORMANCYARMOURBOX_DLG] != NULL )
		m_pDialog[NDORMANCYARMOURBOX_DLG]->DeleteDialog();
	if( m_pDialog[NDORMANCYWEAPONBOX_DLG] != NULL )
		m_pDialog[NDORMANCYWEAPONBOX_DLG]->DeleteDialog();

	if( m_pDialog[NPVPMAPGODLG] != NULL )
		m_pDialog[NPVPMAPGODLG]->DeleteDialog();

	if( m_pDialog[NGIANTRACCOONRESURRECTEVENTDLG] != NULL )
		m_pDialog[NGIANTRACCOONRESURRECTEVENTDLG]->DeleteDialog();

	if( m_pDialog[NPOTRACCOONDLG] != NULL )
		m_pDialog[NPOTRACCOONDLG]->DeleteDialog();

	if( m_pDialog[NRIGHTMOUSEMENU_DLG] != NULL )
		m_pDialog[NRIGHTMOUSEMENU_DLG]->DeleteDialog();

	if( m_pDialog[NRIGHTMOUSEUSERLIST_DLG] != NULL )
		m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->DeleteDialog();
}

// 현재 설치된 디렉토리 경로 받아오기 ( 뒤에 \가 반드시 붙어 있음. )
bool cltClient::GetDirectoryPath( TCHAR *DirectoryPath )
{
	if ( DirectoryPath == NULL )	
	{
		return false;
	}

	GetCurrentDirectory( _MAX_PATH, DirectoryPath );

	SI32 DirectoryPathSize = _tcslen ( DirectoryPath );

	if ( DirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
	{
		StringCchCat( DirectoryPath, MAX_PATH, TEXT("\\") );
	}

	return true;
}

#include <Iptypes.h>
#include <IPhlpapi.h>

#pragma comment(lib, TEXT("iphlpapi.lib"))

void cltClient::GetMacAddress( TCHAR *szAddress , SI16 txtSize)
{
	char ip[ 32 ];

	SOCKADDR_IN addr;
	int len = sizeof( addr );

	getsockname( sm.GetSession(1)->GetSocket(), (SOCKADDR *)&addr, &len );

	memcpy( ip, inet_ntoa( addr.sin_addr ), 20 );
	ip[ 19 ] = NULL;

	char buffer[ 20 ];
	memset(buffer, 0 ,sizeof(buffer));

	IP_ADAPTER_INFO AdapterInfo[16];			// Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);		// Save the memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
		AdapterInfo,							// [out] buffer to receive data
		&dwBufLen);								// [in] size of receive data buffer

	if(dwStatus == ERROR_SUCCESS)
	{
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info

		do
		{
			unsigned char *MACData = pAdapterInfo->Address;

			// 서버에 접속된 소켓의 IP 와 비교하여 IP가 같으면 MacAddress 를 저장할것...
			if ( 0 == strncmp(pAdapterInfo->IpAddressList.IpAddress.String, ip/* Connection된 소켓 IP로 교체 */, 16) )
			{
				sprintf(buffer, "%02X%02X%02X%02X%02X%02X",
					MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
				break;
			}

			pAdapterInfo = pAdapterInfo->Next;		// Progress through linked list
		}
		while(pAdapterInfo);
	}
	TCHAR wbuffer[20];
#ifndef _UNICODE
	StringCchCopy(wbuffer, 20, buffer);
#else
	MultiByteToWideChar( CP_ACP, MB_COMPOSITE, buffer, -1, wbuffer, 20 );
#endif

	StringCchCopy( szAddress, txtSize, wbuffer );
}

void cltClient::GetMacAddress( SOCKET hSocket, TCHAR *szAddress , SI16 txtSize)
{
	char ip[ 32 ];

	SOCKADDR_IN addr;
	int len = sizeof( addr );

	getsockname( hSocket, (SOCKADDR *)&addr, &len );

	memcpy( ip, inet_ntoa( addr.sin_addr ), 20 );
	ip[ 19 ] = NULL;

	char buffer[ 20 ];
	memset(buffer, 0 ,sizeof(buffer));

	IP_ADAPTER_INFO AdapterInfo[16];			// Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);		// Save the memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
		AdapterInfo,							// [out] buffer to receive data
		&dwBufLen);								// [in] size of receive data buffer

	if(dwStatus == ERROR_SUCCESS)
	{
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info

		do
		{
			unsigned char *MACData = pAdapterInfo->Address;

			// 서버에 접속된 소켓의 IP 와 비교하여 IP가 같으면 MacAddress 를 저장할것...
			if ( 0 == strncmp(pAdapterInfo->IpAddressList.IpAddress.String, ip/* Connection된 소켓 IP로 교체 */, 16) )
			{
				sprintf(buffer, "%02X%02X%02X%02X%02X%02X",
					MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
				break;
			}

			pAdapterInfo = pAdapterInfo->Next;		// Progress through linked list
		}
		while(pAdapterInfo);
	}
	TCHAR wbuffer[20];
#ifndef _UNICODE
	StringCchCopy(wbuffer, 20, buffer);
#else
	MultiByteToWideChar( CP_ACP, MB_COMPOSITE, buffer, -1, wbuffer, 20 );
#endif

	StringCchCopy( szAddress, txtSize, wbuffer );
}


TSpr* cltClient::GetGlobalSpr(SI32 index)
{
	return pclGImg->GetGlobalSpr(index);
}


// 장비의 내구도 검사. 
void cltClient::CheckuserEquipDur()
{
	/*
	static DWORD dwLastCheckEquipDurClock = 0;
	TCHAR Nbuffer[512]=TEXT(" ");
	if((pclClient->CurrentClock / 50) % 200)return ;

	SI32 id	= 1;
	SI32 stddur	= 40;
	SI32 mindur	= stddur;
	// 캐릭터가 유효하고 만들어진지 일정시간이 경과해야 한다. 
	cltCharClient* pclchar = pclCM->GetCharClient( id );
	if ( NULL == pclchar )
	{
		return;
	}

	if(TABS(GetFrame() - pclCM->CR[id]->dwMakeFrame) < 100 )return ;


	TCHAR buffer[256] = TEXT("");
	DWORD	 dwDurabilityItem = 0;

	TCHAR* pWeaponText[11];// = NULL;
	SI32 i;
	for(i=0;i<11;++i)
	{
		pWeaponText[i] = GetTxtFromMgr(1023+i);
	}

	cltcheckdurinfo cldurinfo[] ={
		{PERSONITEM_WEAPON1,WEAPON_DURABILITY,	TEXT("")},
		{PERSONITEM_WEAPON2,WEAPON_DURABILITY,	TEXT("")},
		{PERSONITEM_HELMET, HELMET_DURABILITY,	TEXT("")},
		{PERSONITEM_HAT,	HELMET_DURABILITY,	TEXT("")},
		{PERSONITEM_ARMOUR, ARMOUR_DURABILITY,	TEXT("")},
		{PERSONITEM_DRESS,	ARMOUR_DURABILITY,	TEXT("")},
		{PERSONITEM_RING1,	RING_DURABILITY,	TEXT("")},
		{PERSONITEM_RING2,	RING_DURABILITY,	TEXT("")},
		{PERSONITEM_BELT,	BELT_DURABILITY,	TEXT("")},
		{PERSONITEM_SHOES,	SHOES_DURABILITY,	TEXT("")},
		{PERSONITEM_NECK,	NECK_DURABILITY,	TEXT("")},
		{PERSONITEM_MANTLE,	MANTLE_DURABILITY,	TEXT("")},
		{PERSONITEM_BAG,	BAG_DURABILITY,		TEXT("")}
	};

	TCHAR* pText = GetTxtFromMgr(3812);
	for(int sun_count=0;sun_count<13; sun_count++)
	{
		pText = GetTxtFromMgr(3812+sun_count);
		StringCchCopy(cldurinfo[sun_count].szName, 24, pText);

	}



	SI32 checknum	= sizeof(cldurinfo) / sizeof(cltcheckdurinfo);

	cltItem* pclitem;
	const TCHAR* pname = NULL;
	SI32 index = 0;

	for(SI32 person = 0;person < 3;person++)
	{
		if(person == 0)
		{
			pname = pclchar->GetName();
			index = 0;
		}
		else if(person == 1)
		{
			pname = pclCM->CR[id]->pclCI->clSummonInfo.clSummon[0].szName;
			index = PERSONITEM_SUMMON1EQUIP0;
		}
		else if(person == 2)
		{
			pname = pclCM->CR[id]->pclCI->clSummonInfo.clSummon[1].szName;
			index = PERSONITEM_SUMMON2EQUIP0;
		}

		for(SI32 i = 0;i < checknum;i++)
		{
			if ( person == 0 )
			{
				pclitem = & pclCM->CR[id]
			}
			pclitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[ index + cldurinfo[i].siPos ];			
			if(pclitem->siUnique == 0)continue;

			if(pclitem->GetDurability() <= stddur)
			{
				dwDurabilityItem |= cldurinfo[i].siShow;

				// 최저 내구도 물품을 경고한다. 
				if(pclitem->GetDurability() < mindur)
				{
					mindur = pclitem->GetDurability();

					TCHAR* pWaringText = GetTxtFromMgr(1034);
					TCHAR dura[128]= TEXT(" ") ;

					StringCchPrintf(dura, 128, TEXT("%d"),mindur);
					Nsprintf(buffer,pWaringText,TEXT("user"), pname   ,TEXT("item"),   cldurinfo[i].szName ,TEXT("dura"), dura   ,NULL);				
					//sprintf(buffer, pWaringText,
					//	pname, cldurinfo[i].szName, mindur);
				}
			}

			// 위험한 내구도 물품이 없어야 경고가 뜬다. 
			if(mindur == stddur)
			{
				SI32 returnval = 0;
				if(pclCM->CR[id]->CheckEquipCondition(pclItemManager, index + cldurinfo[i].siPos, pclitem, &returnval) == false)
				{
					TCHAR* pWaringText = GetTxtFromMgr(1035);

					Nsprintf(Nbuffer,pText,TEXT("user"), pWaringText   ,TEXT("item"), cldurinfo[i].szName   ,NULL);
					//sprintf(buffer, pWaringText,
					//	pname, cldurinfo[i].szName);
				}
			}

		}

		if(_tcscmp(buffer, TEXT(""))  && TABS(CurrentClock - dwLastCheckEquipDurClock) > 60000)
		{
			dwLastCheckEquipDurClock = CurrentClock;

			PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DURABILITY_WARNING")), 0, 0);

			pclMessage->SetMsg(buffer);
		}
	}


	// 내구도가 닳은게 새로 있거나, 모두 수정된 경우이지...
	// 내구도가 닳은게 새로 있다면, 표시를 하겠고...
	// 내구도 닳은게 없다면, 표시를 없애야지...
	if ( pclDurability->IsTypeExist( dwDurabilityItem ) == FALSE )
	{
		if ( dwDurabilityItem == 0 )				// 내구도 닳은게 없음.
		{
			pclDurability->Set( dwDurabilityItem, FALSE );
		}
		else
		{
			pclDurability->Set( dwDurabilityItem, TRUE );
		}
	}

	*/
}

BOOL cltClient::GetItemUseAbleDay(cltItem* clItem, SI32 * outUseAbleDay)
{
	*outUseAbleDay = 0; // 0값으로 초기화

	if(clItem == NULL) return FALSE; // 아이템 포인터가 잘못되었다면 에러
	SI32 itemunique = clItem->siUnique ;
	if(itemunique <= 0)return FALSE; // 아이템 유니크가 잘못되었다면 에러


	if(clItem->IsSealingUp(pclItemManager) == TRUE)
	{
		*outUseAbleDay = clItem->uiDateUseDay;	// 밀봉된 아이템이라면 사용기간 반환
		return TRUE;
	}


	if(clItem->IsHaveUseDay() == TRUE)  // 밀봉 풀린 기간제 아이템이라면 사용기간 계산해서 반환
	{
		// 서버의 실제 시간 이용
		NDate ndate;
		ndate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);
		SI32 todaydatevary = ndate.GetDateVary() ;

		ndate.SetDate(clItem->uiDateYear+2000,clItem->uiDateMonth,clItem->uiDateDay);
		SI32 datevary = ndate.GetDateVary() + clItem->uiDateUseDay ;

		// 아이템 사용 가능한 기한 - 오늘. 
		// 양수면 사용 가능일,  0 이면 마지막 사용일, 음수면 사라져야 할 아이템
		*outUseAbleDay =  datevary - todaydatevary;

		//KHY - 0723 - 기간아이템 파괴 방지. 혹시 몰라서.
		if (pclClient->IsCountrySwitch(Switch_DateOverItemNoDestroy	) )
		{
			if(*outUseAbleDay < 0)
			{
/*
				SI32 GlobalImageIndex = -1;
				SI32 GlobalImageFont = -1;

				memcpy(&m_clUpViewItem,clItem, sizeof(cltItem));

				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clUpViewItem.siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{
					char *ItemName = (char*)pclClient->pclItemManager->GetName( m_clUpViewItem.siUnique );

					if ( ItemName )
					{
						char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";

						pclClient->pclItemManager->GetExplain( &m_clUpViewItem, ToolTipMsg );

						SI32 size = sizeof(ToolTipMsg);
						SI32 ddd = strlen(ToolTipMsg);
						SI16 numUpView = 1;

						stListViewItemData ListViewItemData;
						ListViewItemData.UpViewSet(&numUpView); 
						ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 , m_clUpViewItem.siItemNum, ItemName, ToolTipMsg );

						m_pClientListView->InsertItem( &ListViewItemData );
					}
				}
*/
				pclClient->sUpViewEffect = 1;
				
			}
		
			if(*outUseAbleDay < (-10000))
				*outUseAbleDay = -10000;
		}

		return TRUE;
	}
	else if(clItem->uiDateUseDay == 0 ) // 날짜가 없는 일반 아이템이다.
		return TRUE;		

	return FALSE ; //  Error 루틴. 
}

void cltClient::DrawInitBack()
{
	// bitmap으로 화면을 덮어 출력한다. 
	HBRUSH hBrush,hOldBrush;
	HDC hDC = GetDC( GetHwnd() );	
	RECT rt;

	GetClientRect(GetHwnd(),&rt);

	hBrush = (HBRUSH) GetStockBrush(BLACK_BRUSH);
	hOldBrush = (HBRUSH) SelectObject(hDC,hBrush);
	Rectangle(hDC,rt.left,rt.top,rt.right,rt.bottom);
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);

	//		memDC = CreateCompatibleDC( hDC );

	//		hOldBitmap = (HBITMAP)SelectObject( memDC, hButtonBitmap );
	//		BitBlt( hDC, (rt.right-800)/2, (rt.bottom-600)/2, 800, 600, memDC, 0, 0, SRCCOPY );

	//		SelectObject ( memDC, hOldBitmap );
	//		DeleteObject(hOldBitmap);
	//		DeleteDC( memDC );
	ReleaseDC( GetHwnd(), hDC );
}

// 주인공이 말을 타거나 내리게 한다. 
void cltClient::RideOrUnrideHorse()
{
	cltCharClient* pclChar = (cltCharClient*)pclClient->pclCM->CR[1];

	cltHorse* pclhorse = pclChar->pclCI->clHorseInfo.GetCurrentShowHorse();
	if(pclhorse == NULL)						return;

	//KHY - 1001 - 기승동물 5마리 보유 수정.
	SI32 siHorseIndex = pclChar->pclCI->clHorseInfo.GetCurrentShowHorseIndex();

	// 말이 있으면 말을 태운다. 
	if(pclChar->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == false)
	{
		if(pclhorse->siHorseUnique)
		{
			if(pclChar->pclCI->clHorseInfo.CanRide(siHorseIndex, &clClientDate) == true)
			{
				cltGameMsgRequest_RideOrUnrideHorse clinfo(siHorseIndex);
				cltMsg clMsg(GAMEMSG_REQUEST_RIDEHORSE, sizeof(clinfo), (BYTE*)&clinfo);
				SendMsgToServer((sPacketHeader*)&clMsg);
			}
			else
			{
				return ;
			}
		}
	}
	else
	{
		cltGameMsgResponse_RideOrUnrideHorse clinfo(siHorseIndex);
		cltMsg clMsg(GAMEMSG_REQUEST_UNRIDEHORSE, sizeof(clinfo), (BYTE*)&clinfo);
		SendMsgToServer((sPacketHeader*)&clMsg);
	}

}

// 클라이언트에서 서비스 지역을 설정하고 적절한 조치를 취한다. 
void cltClient::SetServiceArea(SI32 area)
{

	if(area == siServiceArea)return ;

	siServiceArea										= area;	


	pclVillageManager->DelAllVillage();
	pclVillageManager->LoadVillage(GameMode, siServiceArea);

	//MapInfo.txt를 재설정한다. 
	pclMapManager->LoadMapName(siServiceArea);
}

SI32 cltClient::GetScreenMouseX()
{
	return GameMouseX;
}

SI32 cltClient::GetScreenMouseY()
{
	return GameMouseY;
}

// 마우스를 움직일때의 처리. 
void cltClient::MouseMoveAction(SI32 x, SI32 y)
{
	IpD.Mouse_X	=	x;
	IpD.Mouse_Y	=	y;

	// 클라이언트의 상황에 따라 마우스 좌표를 조정한다.
	// (툴바틍의 좌표에 의해 조정될 수 있다. ) 
	SetMousePos(&IpD.Mouse_X, &IpD.Mouse_Y);

	//if(GetFrame() > 40)
	if ( GetGameStatus() == GS_GAME )
	{
		if(GameMode == GAMEMODE_CLIENT)
		{
			pclUnderMouseObject->siUnderMouseLockChar = 0;
		}

		cltKey clkey(KEY_MODE_NONE, KEY_MODE_NONE, KEY_MODE_NONE, true, IpD.Mouse_X, IpD.Mouse_Y, 0);
		PushKey(&clkey);
	}
	else
	{
		GameMouseX = IpD.Mouse_X + MAP_TILE_XSIZE + GetScreenRX();
		GameMouseY = IpD.Mouse_Y + MAP_TILE_YSIZE + GetScreenRY();
	}
}

void cltClient::ChangeTextFont( TCHAR *FileName, TCHAR *FontKind )
{
	if ( strTextFontFileName == NULL || FontKind == NULL )
	{	
		return;
	}
	RemoveFontResource( strTextFontFileName );

	memcpy( strTextFontFileName, FileName, MAX_PATH - 1 );
	strTextFontFileName[ MAX_PATH - 1 ] = NULL;

	if ( AddFontResource( strTextFontFileName ) == 0 )
	{
		/*
#ifdef _DEBUG
		MessageBox( GetHwnd(), TEXT("Font Add Disable"), TEXT("Font Add Disable"), MB_OK );
#endif
		*/
	}

	m_pInterfaceMgr->SetFontKind( FontKind );

	return;
}

SI32 cltClient::GetScreenXSize()
{
	return siScreenMode800X600XSize + siScreenModeXOffSet;
}

SI32 cltClient::GetScreenYSize()
{
	return siScreenMode800X600YSize + siScreenModeYOffSet;
}

SI32 cltClient::GetScreenStatusBarYSize()
{
	if( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		if ( m_pDialog[ STATUSBAR_DLG ] )
		{
			return m_pDialog[ STATUSBAR_DLG ]->GetHeight();
		}
	}

	return 0;
}

void cltClient::SetMsgBox( stMsgBox *pMsgBox, BYTE *pData, SI16 DataLen )
{
	if ( pMsgBox == NULL || DataLen >= 256 )
	{
		return;
	}

	m_pInterfaceMgr->SetMsgBox( pMsgBox, pData, DataLen );

	return;
}

// 타일 출력 속도 향상을 위한 마스크 정보 획득
void cltClient::GetTileImgMask()
{
	
	SI32 yindex = 0;
	ZeroMemory(TileMaskTransArray,	sizeof(SI32) *  MAP_TILE_YSIZE);
	ZeroMemory(TileMaskImgArray,	sizeof(SI32) *  MAP_TILE_YSIZE);

	TSpr *pspr = pclClient->GetGlobalSpr( GIMG_FOG);

	SI32 transcolornum	= 0, imgcolornum = 0;
	SI32 startpos = pspr->header.ImageInfo[15].iStartPos;
	SI32 imgsize = pspr->header.ImageInfo[16].iStartPos - startpos;

	SI32 xsize = 0;

	for(SI32 i = 0;i < imgsize;i++)
	{

		SI32 data = pspr->pImage[startpos + i];
		if(data == 254)
		{
			i++;

			//  아직 이미지를 만나지 못해야 이미지 좌측의 공백을 점검하는 것이 된다. 
			if(imgcolornum == 0)
			{
				transcolornum = pspr->pImage[startpos + i];
			}

			xsize += pspr->pImage[startpos + i];
		}
		else
		{
			imgcolornum ++;
			xsize ++;
		}

		if(xsize >= pspr->GetXSize())
		{
			// 한줄이 넘어갔다. 
			TileMaskTransArray[yindex] = transcolornum;
			transcolornum = 0;

			TileMaskImgArray[yindex] = imgcolornum;
			imgcolornum = 0;

			xsize = 0;
			yindex++;
		}
	}

}

WORD cltClient::GetPrimaryLanguage( HKL hKeyboardLayout )
{
	return PRIMARYLANGID( LOWORD( hKeyboardLayout ) );
}

WORD cltClient::GetSubLanguage( HKL hKeyboardLayout )
{
	return SUBLANGID( LOWORD( hKeyboardLayout ) );
}

DWORD cltClient::GetCharSet()
{	
	HKL hKeyboardLayout = GetKeyboardLayout( 0 );

	switch ( GetPrimaryLanguage( hKeyboardLayout ) )
	{
	case 0x04:
		{
			switch ( GetSubLanguage( hKeyboardLayout ) )
			{
			case 0x02:				return 134;
			case 0x01:				return 136;
			}
		}
		break;
	case 0x12:				return 129;
	case 0x11:				return 128;
	}
	return 1;
}

BOOL cltClient::IsCorrectOS()
{
	

	int Result;
	TCHAR buf[16];

	Result = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_ICOUNTRY, buf, 16);

	if (Result)
	{
		SI32 CountryCode = _tstoi(buf);

		if (siServiceArea == ConstServiceArea_Korea)
		{
			return TRUE;
		}
		if (siServiceArea == ConstServiceArea_Japan)
		{
			if (CountryCode == CTRY_JAPAN)
				return TRUE;
		}
		if (siServiceArea == ConstServiceArea_China)
		{
			if (CountryCode == CTRY_PRCHINA)
				return TRUE;
		}
		if (siServiceArea == ConstServiceArea_Taiwan)
		{
			if (CountryCode == CTRY_TAIWAN)
				return TRUE;
		}
		if (siServiceArea == ConstServiceArea_USA)
		{
			if (CountryCode == CTRY_UNITED_STATES)
				return TRUE;
		}

		//cyj 글로벌버전은 일본, 한글윈도우에서 실행안되도록 수정
		if (siServiceArea == ConstServiceArea_English)
		{
			if (CountryCode != CTRY_JAPAN && CountryCode != CTRY_SOUTH_KOREA)
				return TRUE;
		}
		//cyj NHNCHINA버전은 일본, 한글윈도우에서 실행안되도록 수정
		if (siServiceArea == ConstServiceArea_NHNChina)
		{
			if (CountryCode != CTRY_JAPAN && CountryCode != CTRY_SOUTH_KOREA)
				return TRUE;
		}
		//cyj 유럽버전은 일본, 한글윈도우에서 실행안되도록 수정
		if (siServiceArea == ConstServiceArea_EUROPE)
		{
			if (CountryCode != CTRY_JAPAN && CountryCode != CTRY_SOUTH_KOREA)
				return TRUE;
		}

	}

	return FALSE;
	
	/* 키보드 레이아웃 방식

	HKL hKeyboardLayout = GetKeyboardLayout( 0 );

	WORD LangType = GetPrimaryLanguage( hKeyboardLayout );

	switch (siServiceArea)
	{
	case ConstServiceArea_Korea :
		{
			if (LangType == LANG_KOREAN)
				return TRUE;
		}
		break;
	case ConstServiceArea_China :
		{
			if (LangType == LANG_CHINESE)
				return TRUE;
		}
		break;
	case ConstServiceArea_Japan :
		{
			if (LangType == LANG_JAPANESE)
				return TRUE;
		}
		break;
	case ConstServiceArea_English :
		{
			if (LangType == LANG_ENGLISH)
				return TRUE;
		}
		break;
	}

	return FALSE;
	*/
}

bool cltClient::GetFameLevelName( SI16 FameLevel, TCHAR *FameLevelName )
{
	if( FameLevelName == NULL )
	{
		return false;
	}

	FILE *fp = _tfopen( TEXT("Fame/FameLevelName.txt"), TEXT("rt") );

	if ( fp )
	{
		TCHAR buffer[ 256 ] = TEXT("");

		for( SI16 i = 0; i < FameLevel + 6; ++i )
		{
			if ( _fgetts( buffer, 256, fp) == NULL )
			{
				fclose( fp );
				return false;
			}
		}

		SI32 Temp;
		_stscanf( buffer, TEXT("%d %s"), &Temp, FameLevelName );

		fclose( fp );

		return true;
	}

	return false;
}

void cltClient::InitMembers()
{
	RECT rtwin;
	GetWindowRect(GetHwnd(),&rtwin);

	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	{
	}
	else
	{
	}

	for ( SI32 i = 0; i < DIALOG_END; ++i )
	{
		if ( i == NLOGIN_DLG ) continue ;
		if ( i == NSELECTWORLDLIST_DLG ) continue ;
		if ( i == NSELECTCHAR_DLG ) continue ;
		if ( i == NMAKECHAR_DLG ) continue ;
		if ( i == NRECOMMENDPERSON_DLG ) continue ;

		DestroyInterface(m_pDialog[i]);
	}
} 

// 화폐단위에 따라 색을 구함
COLORREF cltClient::GetColorByPrice( SI64 price )
{
	if( price >=  1000000000)
		return RGB(250, 20, 20);
	else if( price >= 1000000)
		return RGB(255, 115, 0);
//	else if( price >= 1000000)
//		return RGB(0, 150, 0);
	else
		return RGB(0, 0, 0);
}

TCHAR * cltClient::GetServerNameFromUnique(SI16 serverunique)
{
#ifdef _DEBUG
	static TCHAR buffer[16] = "";
	_itot( serverunique, buffer, 10 );
	return buffer;
#else
   	TCHAR * pText = GetTxtFromMgr(1287);

	if(pclClient->siServiceArea == ConstServiceArea_English)
	{
		switch(serverunique)
		{
			case 1:	
				pText = GetTxtFromMgr(8557); break;	// 제우스
			case 2:
				pText = GetTxtFromMgr(8558); break;	// 헤라
			case 3:
				pText = GetTxtFromMgr(8559); break; // 아테나
		}
	}
	else if(pclClient->siServiceArea == ConstServiceArea_USA)
	{
		switch(serverunique)
		{
			case 1:
				pText = GetTxtFromMgr(8558); break;	// 헤라
			case 2:	
				pText = GetTxtFromMgr(8559); break;	// 아테나
		}
	}
	else if(pclClient->siServiceArea == ConstServiceArea_Korea)
	{
		switch(serverunique)
		{
			case 1:
				pText = GetTxtFromMgr(8560); break;	// 로타네브
			case 2:		
				pText =	GetTxtFromMgr(8561); break;	// 파이시즈
			case 3:	
				pText = GetTxtFromMgr(8562); break;	// 타우르스
			case 4:	
				pText = GetTxtFromMgr(8563); break;	// 제미니
		}
	}
	else if(pclClient->siServiceArea == ConstServiceArea_Japan)
	{
		switch(serverunique)
		{
			case 1:
				pText = GetTxtFromMgr(6090); break;
			case 2:	
				pText = GetTxtFromMgr(6091); break;
			case 3:
				pText = GetTxtFromMgr(6928); break;
			case 4:
				pText = GetTxtFromMgr(7110); break;
			case 5: //일본 5번째 서버용.
				pText = GetTxtFromMgr(7110); break;
		}
	}
	else if(pclClient->siServiceArea == ConstServiceArea_Taiwan)
	{
		switch(serverunique)
		{
			case 1:	
				pText = GetTxtFromMgr(8557); break;
			case 2:
				pText = GetTxtFromMgr(8558); break;	
		}
	}
	else if(pclClient->siServiceArea == ConstServiceArea_NHNChina)// 이름은 그냥 가져다 씀.
	{
		switch(serverunique)
		{
		case 1:
			pText = GetTxtFromMgr(8558); break;	// 헤라
		case 2:	
			pText = GetTxtFromMgr(8559); break;	// 아테나
		}
	}
	else if(pclClient->siServiceArea == ConstServiceArea_EUROPE)// [영훈] 현재는 나온것이 없음 바꿔야함
	{
		switch(serverunique)
		{
		case 1:
			pText = GetTxtFromMgr(8558); break;	// 헤라
		case 2:	
			pText = GetTxtFromMgr(8559); break;	// 아테나
		}
	}
     	
	return pText;
#endif
}

bool cltClient::IsEnterGate( SI32 gateunique )
{
	if( gateunique >= INSTANCEMAPGATE_START )		return true;

	SI32 siMapIndex =  pclMapManager->pclGateManager->clGate[gateunique].DstMapIndex;
	SI32 siMapUnique = pclMapManager->GetMapUnique( siMapIndex );

    TCHAR *pText = pclMapManager->pclMapTypeManager->pclMapTypeInfo[siMapUnique]->szMapTypeName;
	TCHAR buffer[256] = TEXT("");

	StringCchPrintf( buffer , 256, TEXT("map/%s/%s.map"), pText, pText );

	if( (_taccess( buffer, 0 )) == -1)
		return false;
	else
        return true;
}
SI16 cltClient::GetLanguageFromFile()
{
	FILE *pFile;
	TCHAR strLine[256]=TEXT("");
	if ( pFile = _tfopen(TEXT("Config/language.dat"), TEXT("r")) )
	{
		_fgetts(strLine, 256, pFile );
		fclose( pFile );
	}
	else
	{
		return ConstLanguage_English;
	}

	SI16 language =	_tstoi( strLine );
	switch( language )
	{
	case ConstLanguage_China:
		return ConstLanguage_China;
	default:
		return ConstLanguage_English;
	}
}

SI16 cltClient::GetFpsFromFile()
{
	FILE *pFile;
	TCHAR strLine[256]=TEXT("");
	if ( pFile = _tfopen(TEXT("Config/framerate.dat"), TEXT("r")) )
	{
		_fgetts(strLine, 256, pFile );
		fclose( pFile );
	}
	else
	{
#ifdef _KOREA
		// 군주스페셜(한국) 기본 40FPS로 변경 - By LEEKH - 2007-06-12
		return ConstFrameRate_High;
#else
		return ConstFrameRate_Low;
#endif
	}

	SI16 framerate = _tstoi( strLine );
	if ( framerate != ConstFrameRate_Low && framerate != ConstFrameRate_High )
	{
#ifdef _KOREA
		// 군주스페셜(한국)기본 40FPS로 변경 - By LEEKH - 2007-06-12
		return ConstFrameRate_High;
#else
		return ConstFrameRate_Low;
#endif
	}
	else
	{
		return framerate;
	}
}
void cltClient::SendEmoticon(SI16 index )
{
	if( TABS(CurrentClock - LastSendEmoticonClock ) > 1000 )
	{
		cltGameMsgRequest_SetEmoticon clinfo(index);
		cltMsg clMsg( GAMEMSG_REQUEST_SETEMOTICON, sizeof(clinfo), (BYTE*)&clinfo );
		SendMsgToServer( (sPacketHeader*)&clMsg );

		LastSendEmoticonClock = CurrentClock;
	}
}

void cltClient::ShowHelpWindow( TCHAR* szFileName )
{
	if ( ConstServiceArea_China == siServiceArea )
	{
		g_LoginNoticeDlg.Set( TEXT("http://help.hljz.net/"), 600, 500 );
	}
	else
	{
		// 한국 서비스에선 도움말 창 표시 안하기	 - By LEEKH
		if( ConstServiceArea_Korea == siServiceArea )
			return;
		// 중국 서비스에선 도움말 창 표시 안하기 - 손성웅
		if( ConstServiceArea_NHNChina == siServiceArea )
			return;

		if( m_pDialog[ NHINT_DLG ] == NULL)
		{
			CreateInterface( NHINT_DLG );
		}
		((NHintDlgExplain*)m_pDialog[ NHINT_DLG])->Show(szFileName);
	}
}
bool cltClient::CanActionBySecond( SI32 second )
{
	if ( GetFrame() % ( 1000 / GetFrameDelay() * second ) == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool cltClient::IsOpenInterface( SI32 index )
{
	if( m_pDialog[index] && m_pDialog[index]->IsShow() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. => 2008-6-17
bool cltClient::IsBuffRouletteAction(SI16 _siBuffKind)
{
	if(	   NULL != m_pDialog[NBUFF_ROULETTE_DLG]  
		&& NULL != pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(_siBuffKind))
	{
		// 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
		if(    BUF_GET_TYPE_BUFROULETTE == pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(_siBuffKind)->siGetType
			&& true == ((CNBuffRoulette*)m_pDialog[NBUFF_ROULETTE_DLG])->GetRouletteAction() )
			return true;
	}


	if( pclClient->IsCountrySwitch(Switch_BalanceCurrency) )
	{
		if((m_pDialog[NBALANCECURRENCY_DLG] != NULL) && (pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(_siBuffKind) != NULL))
		{
			// 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. 
			if(    BUF_GET_TYPE_BUFROULETTE == pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(_siBuffKind)->siGetType
				&& true == ((CNBuffRoulette*)m_pDialog[NBALANCECURRENCY_DLG])->GetRouletteAction() )
				return true;
		}
	}
	return false;
}

// APEX-
void cltClient::DoMsg_GAMEMSG_RESPONSE_APEXDATA(cltMsg* pclMsg, SI32 id)
{
	OutputDebugString( "Receive Apex Data\r\n" );

	sPacketHeader *pPacket = (sPacketHeader *)pclMsg;
	char *pPacketChar = (char*)pPacket;

	NoticeApec_UserData( pPacketChar + sizeof( sPacketHeader ), pPacket->usSize - sizeof( sPacketHeader ) );
}
// -APEX

// -HanAuth_Lib
void cltClient::HanAuth_Lib_HanReportSendPosLog_Client(TCHAR* pszID, TCHAR* pszValue)
{
	if(pszID == NULL)		return;
	if(pszValue == NULL)	return;

	TCHAR	szReportString[1024];
	StringCchPrintf(szReportString, sizeof(szReportString), "id=%s&value=%s", pszID, pszValue);

#ifdef USE_HANREPORT
	HanReportSendPosLog(szReportString);
#endif // USE_HANREPORT

}
void cltClient::HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA( SI32 siserverid, TCHAR* pszUserID, TCHAR* pszValue)//서버아이디/유저 아이디/ 밸류
{
	if(siserverid <  0)		return;
	if(pszUserID == NULL)		return;
	if(pszValue == NULL)		return;

	TCHAR	szReportString[1024];

	StringCchPrintf(szReportString, sizeof(szReportString), "svrno=%0.5d%&id=%s&value=%s", siserverid,pszUserID, pszValue);

	#ifdef USE_HANREPORT
		HanReportSendPosLog(szReportString);
	#endif // USE_HANREPORT
}
#ifdef USE_GAME_GUARD_CLIENT


// 메시지 처리 Callback 함수
// 게임 종료시에는 false를 return 해주고, 종료하지 않는 경우는 true를 return 합니다.
BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
	if(pclClient)
	{
		//pclClient->pclLog->FilePrint("config\\nProtectLog.log" ,"CALLBACK NPGameMonCallback" );
	}		

	switch (dwMsg)
	{
	case NPGAMEMON_CHECK_CSAUTH2:
		if(pclClient)
		{
			GG_AUTH_DATA clGGAuthData;
			memcpy( &clGGAuthData , (PVOID)dwArg , sizeof(GG_AUTH_DATA) );

			cltGameMsgRequest_GameGuardAuth clinfo(&clGGAuthData) ;
			cltMsg clMsg(GAMEMSG_REQUEST_GAME_GUARD_AUTH,sizeof(clinfo),(BYTE*)&clinfo);
			((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg);

			//if(pclClient)	pclClient->pclLog->FilePrint("config\\nProtectLog.log" ,"NPGAMEMON_CHECK_CSAUTH2" );
			return true;
		}

	case NPGAMEMON_COMM_ERROR:
	case NPGAMEMON_COMM_CLOSE:
		dwQuitTimer = 1; // 종료 코드
		return false;
	case NPGAMEMON_INIT_ERROR:
#ifdef  _KOREA
		wsprintf(g_szHackMsg, "게임가드 초기화 에러 : %lu", dwArg);
#else
		wsprintf(g_szHackMsg, "GameGuard initialization error : %lu", dwArg);
#endif
		dwQuitTimer = 1; // 종료 코드
		return false;
	case NPGAMEMON_SPEEDHACK:
#ifdef _KOREA
		wsprintf(g_szHackMsg, "스피드핵이 감지되었습니다.");
#else
		wsprintf(g_szHackMsg, "Speedhack detected.");
#endif
		dwQuitTimer = 1; // 종료 코드
		return false;
	case NPGAMEMON_GAMEHACK_KILLED:
#ifdef _KOREA
		wsprintf(g_szHackMsg, "게임핵이 발견되었습니다.\r\n%s", pNpgl->GetInfo());
#else
		wsprintf(g_szHackMsg, "Gamehack detected.\r\n%s", pNpgl->GetInfo());
#endif
		dwQuitTimer = 1; // 종료 코드
		return false;
	case NPGAMEMON_GAMEHACK_DETECT:
#ifdef _KOREA
		wsprintf(g_szHackMsg, "게임핵이 발견되었습니다.\r\n%s", pNpgl->GetInfo());
#else
		wsprintf(g_szHackMsg, "Gamehack detected.\r\n%s", pNpgl->GetInfo());
#endif
		dwQuitTimer = 1; // 종료 코드
		return false;
	case NPGAMEMON_GAMEHACK_DOUBT:
#ifdef _KOREA
		wsprintf(g_szHackMsg, "게임이나 게임가드가 변조되었습니다.");
#else
		wsprintf(g_szHackMsg, "Game or GameGuard file corrupted.");
#endif
		dwQuitTimer = 1; // 종료 코드
		return false;
#ifdef _TAIWAN
	case NPGAMEMON_GAMEHACK_REPORT:
		{
		DWORD dwHackInfoSize = 0;
		LPBYTE pHackInfo = NULL;
		pHackInfo = GetHackInfoFromGameMon(&dwHackInfoSize);
		if(pHackInfo && dwHackInfoSize > 0)
			{
				//서버로 데이터 전송
				//SendMsgToServer(pHackInfo);
			}
		}
		return true;
#endif
	}

	// 게임 가드 관련 코드 수행 종료 전에 저장된 오류 메세지 있으면 메세지 박스 띄움
	if (g_szHackMsg[0])
	{
		MessageBox(NULL, g_szHackMsg, TEXT("nProtect GameGuard"), MB_OK);
	}

	return true; // 계속 진행
}

#endif

