#ifndef _DIALOGRESOURCE_H_
#define _DIALOGRESOURCE_H_


////////////////////////////////////// DANGER //////////////////////////////
// 다이얼로그 중간에 추가하면 안됩니다 >_< - 영진
// 순서대로 다이얼로그 위치가 저장되고 있습니다
////////////////////////////////////////////////////////////////////////////


enum
{
	NMSGBOX_DLG,		// 0
	NMSGBOX0_DLG,
	NMSGBOX1_DLG,
	NMSGBOX2_DLG,
	NMSGBOX3_DLG,
	NMSGBOX4_DLG,
	NMSGBOX5_DLG,
	NMSGBOX6_DLG,
	NMSGBOX7_DLG,
	NMSGBOX8_DLG,
	NMSGBOX9_DLG,		// 10
	NCANDIDATE_DLG,
	TOOLBAR_DLG,

	STATUSBAR_DLG,
	NCHAT_DLG,
	TEST_DLG,
	NOTIFYMSG_DLG,
	NOTIFYGETITEM_DLG,
	NOTICEEVENTMSG_DLG,
	MONSTERCORPORATION_DLG,		//제조시 미니게임
	NINVENTORY_DLG,		// 20
	NEXITCONFIRM_DLG,
	NEXIT_DLG,
	NITEMINFO_DLG,
	NHUNTMAPINFO_DLG,
	NHOMEPAGE_DLG,
	NRESIDENTSINFO_DLG,
	NMAGICINFO_DLG,
	NNEWMAGICINFO_DLG,
	NFATHER_DLG,
	NCHILD_DLG,
	NPARTY_DLG,			// 30
	NPARTYINVITE_DLG,
	NSOUNDONOFF_DLG,
	NMONSTERINFO_DLG,
	NVILLAGEDETAILINFO_DLG,
	NFRIEND_DLG,
	NFRIENDACCEPT_DLG,
	NQUEST_DLG,
	NDEATH_DLG,
	NPORTPAYTAX,
	NEVENTNOTIC_DLG,	// 40
	NVILLAGEWARINFO_DLG,
	NSELECTWORLDLIST_DLG,
	NMASTERINFO_DLG,
	NBOBUSANG_DLG,
	NFARMING_DLG,
	NRAREITEM_DLG,
	NLOGIN_DLG,
	NMAKEITEM_DLG,
	NSELECTCHAR_DLG,
	NSELECTNOLIMITWARPTICKET_DLG,	// 50
	NMAKECHAR_DLG,
	NNOTICEADMIN_DLG,
	NRECOMMENDPERSON_DLG,
	NVOTEKING_DLG,
	NDELETECHAR_DLG,
	NSEIGEWIN_DLG,
	NFISHING_DLG,
	NFOREIGNMERCHANT_DLG,	
	NTRADEMERCHANTHORSE_DLG,
	NFOREIGNMERCHANTBUYITEM_DLG,	// 60
	NFOREIGNMERCHANTBUYCASHITEM_DLG,
	NTRADEMERCHANT_DLG, // 교역 상인
	NTRADEMERCHANTHIGHCLASS_DLG,	// 명품 상인
	NHOZOBASE_DLG,	
	NHOZORESIDENT_DLG,
	NHOZOPRIZE_DLG,
	NHOZOCHANGEDNAME_DLG,
	NHOZOFIXATION_DLG,
	NHOZONEWCHARRATE_DLG,
	NHOZOANGEL_DLG,				// 70
	NHOZOANGELAPPYER_DLG,
	NHOZOAPPOINTMENT_DLG,
	NBYUNGZO_DLG,
	NIZOBASE_DLG,
	NIZOPRICEVALUE_DLG,
	NGONGZO_DLG,
	NGOONZU_DLG,
	NYEZOBASE_DLG,
	NYEZOANGEL_DLG,
	NYEZOANGELAPPYER_DLG,		// 80
	NYEZOAPPOINTMENT_DLG,
	NYEZOJAPANWAR_DLG,
	NHYUNGZOBASE_DLG,
	NHYUNGZOANNOUNCEMENT_DLG,
	NHYUNGZOMAIN_DLG,
	NHYUNGZOUSER_DLG,
	NHYUNGZOPOLICE_DLG,
	NHYUNGZOCRIMINAL_DLG,
	NNEWQUEST_DLG,
	NSUMMONUNITBUYNPC_DLG,		// 90
	QUESTVIEWER_DLG,


	CLTNCHARSTATUS_DLG,
	CLTNCHARKINDCHANGE_DLG,
	CLTNHORSETYPECHANGE_DLG,
	CLTNCHARNAMECHANGE_DLG,
	CLTNCHARGREETINGCHANGE_DLG,

	NLETTER_DLG,
	NDECREEPASSWD_DLG,
	NGAMECONTRACT_DLG,
	NAUTOHUNT_DLG,				// 100        
	NNPCMENU_DLG,               //봉이김선달,야린스키 첫 메뉴
	NNPCREPAIR_DLG,              //장영실

	NINVESTMENT_DLG,            //봉이김선달
	NNPCTRADE1_DLG,              //무수리
	NBOSSMARKET_DLG,			// 왕초 첫화면
	NBOSSMARKETDETAIL_DLG,		// 왕초 상세화면
	NHIRENPC_DLG,               //NPC 고용


	NPERSONMALINFO_DLG	,
	NPERSONALSHOP_DLG,
	NPERSONALSHOPSEARCH_DLG,	// 110

	NWENNYMAN_DLG,             //혹부리 영감
	NWENNYMANCHILD1_DLG,
	NWENNYMANCHILD2_DLG,

	NPRIVATETRADEACCEPT_DLG,
	NPRIVATETRADEACCEPT_MSGBOX_DLG,
	NPRIVATETRADE_DLG,    //개인거래 
	NPTRADECHILD1_DLG,
	NPTRADECHILD2_DLG,
	NPTRADECHILD3_DLG,
	NPTRADECHILD4_DLG,			// 120

	NPTRADEREPRAIR_DLG,   //개인 수리 	
	NITEMMALLMANAGER_DLG, //아이템몰 버튼 
	NITEMMALL_DLG, // 아이템몰 
	NHINT_DLG,     //도움말 메뉴

	NMUDANG_DLG	,
	NMUDANG_EVOLVE_DLG	,
	NMUDANG_REVIVE_DLG	,
	NMUDANG_SCROLL_DLG	,

	NPLAYMATCH_DLG,	// 결투장, 줄다리기 선택..

	NMINING_DLG,			// 130

	NSKILL_DLG,
	NWEAPONSKILL_DLG,
	NHONGGILDONG_DLG,
	NWARJOIN_DLG,

	NRESIDENTS_DLG,

	NMENU_DLG,
	NRIGHTMOUSEMENU_DLG,
	NMINIMAP_DLG,

	NPRIVATECHAT0_DLG,
	NPRIVATECHAT1_DLG,		// 140
	NPRIVATECHAT2_DLG,
	NPRIVATECHAT3_DLG,
	NPRIVATECHAT4_DLG,
	NPRIVATECHAT5_DLG,
	NPRIVATECHAT6_DLG,
	NPRIVATECHAT7_DLG,
	NPRIVATECHAT8_DLG,
	NPRIVATECHAT9_DLG,

	NPRIVATECHATREJECT_DLG,

	NERRORMSGPRINT_DLG,		// 150
	NERRORMSGPRINT1_DLG,
	NDAILYQUEST_DLG	,

	NNOTIFYGETEXPANDITEM_DLG,
	NCITYHALLSUBMIT_DLG	,
	NSTOCK_DISTRIBUTION_DLG	, 
	NEVENT_DLG,	// 외부 이벤트
	ROULETTEGAME_DLG,	// 박대박 미니게임
	NSAVEITEMLIST_DLG,
	NGUILDCHAT_DLG,			// 길드 채팅


	NPERSONALSHOP_POP_DLG,	// 160

	// 건물 다이얼로그 
	NSTR_BASE_DLG,
	NSTR_INFO_DLG,

	NCITYHALL_DLG,			// 상단
	NCITYHALLCONSTRUCT_DLG,
	NRESIDENTLIST_DLG	, 
	NBERESIDENT_DLG	, 
	NAPPOINTMENTCHIEF_DLG,
	NSTRUCTUREMESSAGE_DLG,

	NBANK_DLG,				// 전장
	NSTOCK_DLG,				// 여각		170
	NMARKET_DLG,			// 객주 
	NHOUSE_DLG,				// 시전 	
	NPOSTOFFICE_DLG,		// 역참	
	NHUNT_DLG,				// 수렵장
	NPORT_DLG,				// 포구
	NHORSEMARKET_DLG,		// 사복시
	NREALESTATEMARKET_DLG,	// 거간
	NSUMMONMARKET_DLG,		// 장예원
	NSUMMONHEROMARKET_DLG,	// 비변사
	NFEAST_DLG,				// 내자시	180
	NLAND_DLG,				// 농장	
	NMINE_DLG,				// 광산
	NGUILDSTR_DLG,			// 길드상단
	NGUILD_DLG,				// 길드

	// 시전
	NHOUSENAMECHANGE_DLG,		// 창고 이름 변경
	NHOUSEEXTENSION_DLG,		// 기간 연장
	NHOUSEUSERINFOBASE_DLG,		// 시전 정보틀
	NHOUSEUSERITEM_DLG,			// 창고
	NHOUSEUSERHORSE_DLG,		// 마구간
	NHOUSEUSERSECONDHAND_DLG,	// 고물
	NHOUSEUSERMANAGEMENT_DLG,	// 관리
	NHOUSEUSERNEWMANAGEMENT_DLG, // 새로운 관리
	NVILLAGETREE_DLG,				// 190
	NGAMEMASTERORDER_DLG,		// GM명령
	NCOMMONMSG_DLG,				// 공통메세지 다이얼로그
	NCHARMENUBAR_DLG,
	NSYSMENUBAR_DLG,
	NCHATBAR_DLG,
	NSYSCHILDMENU_DLG,
	NNEWINVENTORY_DLG,
	NNEWEQUIPWINDOW_DLG,
	NQUESTINFO_DLG,

	// 주주총회 관련
	NGENERALMEETINGLETTER_DLG,		// 200
	NGENERALMEETING2_DLG,

	// 외적(흑의군단)
	NBLACKARMYMVP_DLG,

	// 아이템 제조 인터페이스
	NNEWITEMMAKE_DLG, 
	NNEWMAKECHILD_DLG,
	NNEWENCHANTCHILD_DLG,
	NNEWENCHANTOTHER_DLG,

	// 새롭게 들어갈 아이템 몰
	NNEWITEMMALL_DLG,	
	NBUYWINDOW_DLG,
	NPRESENTWINDOW_DLG,
	NBUYITEMINFOWINDOW_DLG,	// 물건 사고 난 후의 정보창		210
	NPRESENTINFOWINDOW_DLG,	// 선물 하고 난 후의 정보창

	NACCOUNTBOOKBASE_DLG,	// 마을 회계장부
	NACCOUNTBOOKPART1_DLG,	// 자본금부분
	NACCOUNTBOOKPART2_DLG,	// 이익금부분
	NACCOUNTBOOKPART3_DLG,	// 전체
	NNEWENCHANTOTHERACCEPT_DLG,
	NNEWENCHANTOTHERACCEPT_MSGBOX_DLG,

	NGOONZUPOLICY_DLG,		// 군주 정책창

	NRIGHTMOUSEUSERLIST_DLG, // 마우스 오른쪽 키를 눌렀을때 플레이어가 겹쳐있으면 먼저 목록을 보여준다.

	NGETPRESENT_DLG,	// 아이템몰 선물이 왔음을 보여주는 창	220
	NTHANKSPRESENT_DLG, // 선물해줘서 고맙다는 메시지가 왔음을 보여주는 창

	NITEMMALL_GIVESTATUS_DLG,

	NSHOWGETITEM_DLG,		// 아이템 획득창
	NTHANKSPRESENT_ANSWER_DLG,  // 아이템 줘서 고맙다는 감사의 메시지 보내주는 창

	NAQTRADEWINDOW_DLG,		// Aq 트레이드 다이얼로그
	NSETWORLDMONEYBUT_DLG,

	NINFO_DLG,	//

	// 무역 다이얼로그
	NTRADESERVERSELL_DLG,
	NTRADESERVERBUY_DLG,
	NTRADESERVERBASE_DLG,		// 230

	// 통합정보창 서브 
	NINFO_SUB_DLG,

	// 추천포인트 관리, 소모 npc 다이얼로그
	NRPMERCHANT_DLG,
	NRPLIST_DLG,

	NSETCOUNTRY_DLG,
	NUSERLISTBYCOUNTRY_DLG,

	NMANUFACTURE_DLG,

	NRPBASE_DLG,

	// 말 인벤토리
	NNEWINVENTORY_HORSE_DLG,

	// 가방 인벤토리
	NNEWINVENTORY_BAG0_DLG,
	NNEWINVENTORY_BAG1_DLG,		// 240

	NDOWNLOADADDON_DLG,
	NITEMMALLPRODUCT_DLG,

	NSELECTITEMCOUNT_DLG,

	NOPENINGBANNER_DLG,

	// 이조판서 
	NIZOWORLDTRADETAX_DLG,
	NIZOMARKETPRICE_DLG,

	// 초보자 리스트
	NLOWLEVEL_DLG,

	NMARKET_SUB_DLG,

	// 공성전신청
	NGOONZUSIEGE_DLG,

	NMYGUILDBASE_DLG,		// 250
	NMYGUILD_DLG,
	NMYGUILDMASTER_DLG,

	NMYGUILDMASTERMANAGE_DLG,
	NMYGUILDMASTERSTATUS_DLG,
	NMYGUILDMASTERHUNTMAP_DLG,
	NMYGUILDMASTERGVG_DLG,
	NMYGUILDMASTERMARK_DLG,

	NBEGUILDMEMBER_DLG,

	NGUILDINFO_DLG,
	NTREASUREBOX_DLG,		// 260

	NGUILDDUNGEON_DLG,
	NGUILDDUNGEONAPPLY_DLG,

	NGUILDMEMBER_DLG,
	NGUILDSENDMSG_DLG,

	NEMOTICON_DLG,
	NFRIENDMEMO_DLG,

	NNEWFRIEND_DLG,
	NFRIENDCHANGEGROUPNAME_DLG,

	NMANUFACTURENPC_DLG,
	NBEGINNERWEAPONNPC_DLG,	// 270

	NGUILDWARAPPLY_DLG,
	NGUILDWARRECOMMAND_DLG,

	NMAKEITEMDEMO_DLG,
	NAUTOANSWER_DLG,
	NGACHA_DLG,

	NMOFUMOFULIST_DLG,

	NPREMIUMSTATUS_DLG,

	NBUYSECONDWINDOW_DLG,

	NBUFFSTATUS_DLG,//툴팁추가.[2007.07.12 성웅]

	NNAMINGQUESTLIST_DLG,	//tys	280
	NHELPBUTTON_DLG,		//tys
	NSELECTNAMING_DLG,		//tys
	NHAPPYBEAN_DLG,//해피빈 이벤트 [2007.08.22 손성웅]

	NSYSTEMREWARDLIST_DLG,	//tys - systemreward
	NATTENDCHECK_DLG,		//tys - attend
	NQUESTSELECTITEM_DLG,	//tys - questitemselect
	NNEWNOTIFY_DLG,			//tys

	NMUSIC_DLG,

	NINVITEEMPIRE_DLG,	// kkm 

	NAUCTIONEVENT_DLG,		// 290

	NEXCHANGESTOCK_DLG,

	NCHRISTMASEVENT_DLG,//크리스마스이벤트

	NCHRISMASQUEST_DLG,			//[추가 : 황진성 2007. 11. 20 크리스마스 퀘스트창 추가.]

	NNEWYEAR_DLG,				//[추가 : 황진성 2007. 12. 11 신년 이벤트 버튼은 대화상자로...]

	CHARTRANSFORMINFO_DLG,				//[추가 : 황진성 2007. 12. 20 변신 정보창 추가.]
	CHARTRANSFORMINFO_STORY_DLG,		//[추가 : 황진성 2007. 12. 20 변신 관련 스토리창 추가.]
	CHARTRANSFORMINFO_STORY_READ_DLG,	//[추가 : 황진성 2007. 12. 20 변신 관련 스토리 읽기창 추가.]
	NINSTANTDUNGEONBOARD_DLG,			//인스턴트 던전 - 순위,명예의전당 2008.01.07 손성웅

	NINSTANST_DUNGEON_STATUS_DLG,		// [추가 : 황진성 2008. 1. 8  => 인스턴트 던젼 상태보기.]
	NINSTANST_DUNGEON_RUN_INFO_DLG,		// [추가 : 황진성 2008. 1. 9  => 인스턴트 던젼 진행 상태보기.]	300
	NINSTANST_DUNGEON_NOTIPY,			// [추가 : 황진성 2008. 1. 18 => 인스턴트 던젼 관련 알림 창.]

	NBLACK_WAR_RUN_DLG,					// [추가 : 황진성 2008. 1. 24 => 흑의 군단 개편 진행 상태 창.]
	NBLACK_WAR_REWARD_DLG,				// [추가 : 황진성 2008. 1. 25 => 흑의 군단 개편 진행 상태 창.]

	NSPACE_BOX,							// [추가 : 황진성 2008. 1. 29 => 공간 상자 창.]

	ARTIFACT_BASE_DLG,					// [추가 : 황진성 2008. 2. 26 => 아티펙트 기본 창. 텝 버튼창.]
	ARTIFACT_MIXTURE_DLG,				// [추가 : 황진성 2008. 2. 26 => 아티펙트 조합 창.]
	ARTIFACT_SLOT_CREATE_DLG,			// [추가 : 황진성 2008. 2. 26 => 아티펙트 슬롯생성 창.]
	ARTIFACT_REMOVE_DLG,				// [추가 : 황진성 2008. 2. 26 => 아티펙트 결정체 제거 창.]
	ARTIFACT_RESTORE_DLG,				// [추가 : 황진성 2008. 2. 26 => 아티펙트 아이템 환원 창.]

	PARTYHALL_IN_BASE_DLG,				// 310
	PARTYHALL_IN_COSTUME_USER_DLG,
	PARTYHALL_IN_FIRECRAKER_CHIEF_DLG,
	PARTYHALL_IN_INVITE_CHIEF_DLG,

	NDAILYQUEST2_DLG,					// 데일리 퀘스트 2 손성웅
	ATTENDNOTICE_DLG,					// [영훈] 출석체크 : 간단 출석체크창
	ELECTIONSTUMP_DLG,					// [영훈] 선거시스템 : 유세 시스템
	HIDDENQUEST2_DLG,					// 히든 퀘스트2 대박 다이얼로그 
	PREMIUMSTATUS_DLG,

	// 내부 UI : Pannel
	PANNEL_DATEMONEY_DLG,				// [영훈] 시간과 돈 표시 다이얼로그
	PANNEL_CHARSTATUS_DLG,				// [영훈] 캐릭터 정보표시		320
	PANNEL_SUMMON_DLG,					// [영훈] 소환수 정보
	PANNEL_ORGAN_DLG,					// [영훈] 내장기관 정보
	PANNEL_SOULGUARD_DLG,				// [영훈] 수호정령 정보

	STATUSBAR_POPUPPARENT_DLG,
	STATUSBAR_POPUPCHILDCOMMUNITY_DLG,
	STATUSBAR_POPUPCHILDSYSTEM_DLG,

	NDAILYQUEST3_DLG,					// 데일리 퀘스트 3 손성웅
	CHARSTATUS_ABILITY_DETAIL_DLG,		// [진성] 캐릭터 정보창에서 상세 능력치 정보를 보여줄 창. 2008-3-24

	NVOTESTATUS_DLG,					// [영훈] 선거 개표현황

	NOPENBIGITEMBOX_DLG,

	NMONSTER_ATTACK_EVENT,				// [진성] 마을 이벤트 - 몬스터 습격. => 2008-3-26	330

	NGMINFO_DLG,						// [영훈] GM에게 필요한 정보 표시창

	NCHARSUMMONRESET_DLG,				// [영훈] 소환수 강화 정보 및 초기화 창

	NCHANGE_DLG,						// [ 성웅 ] 버그 및 사고시 사용될 버그 수정 NPC 다이얼로그 
	NSETCHAT_DLG,						// [ 성웅 ] 채팅창에 올라올 메세지들을 분류할때 슬 창

	NBUFF_ROULETTE_DLG,					// [진성] 영웅의 동상. => 2008-6-9

	NMAPNAME_DLG,						// [영훈] 멥 이름 보여주기 (2008.06.17)

	// 서버 이벤트.
	SERVER_EVENT_RANK_DLG,				// [진성] 서버 이벤트 전체(서버, 개인)랭킹 보여줄 게시판.
	SERVER_EVENT_QUEST_DLG,				// [영훈] 서버 이벤트 퀘스트 정보
	SERVER_EVENT_INSTANCE_MAP_BONUS,	// [진성] 서버 이벤트 인던 종료 후 조건 만족시 보너스 창. => 2008-6-25
	SERVER_EVENT_INDUNINFO,				// [영훈] 서버 이벤트 보너스 인던내에 상황		340

	NTREASUREBOXEXTRA_DLG,				// [성웅] 2차 가차 주머니

	NDESTROYITEM_DLG,					// [성웅] 아이템 드롭시 아이템 이름을 적는 다이얼로그

	NMUDANG_EQUIPITEM_DLG	,			// [성웅] 소환수 장비 장착석 

	NMARKETCONDITIONS_DLG	,			// [희영] 시세확인 2008-07-18

	NPOSTOFFICE_GROUP_LIST_DLG,			// [진성] 단체 메시지 보내기. 유저 리스트 정보창. => 2008-8-4

	NQUESTINFODLG_PARENT,				// [영훈] 변경된 퀘스트 알림창 부모창
	NQUESTINFODLG_GOINGQUEST,			// [영훈] 변경된 퀘스트 알림창 진행 중인 퀘스트 창
	NQUESTINFODLG_ALLQUEST,				// [영훈] 변경된 퀘스트 알림창 전체 퀘스트 창

	NQUESTNOTICE_DLG,					// [영훈] 다음 진행 퀘스트 표시창

	NMYSUMMON_DLG,						// [성웅] 소환수 보관 시스템		350

	NGACHANEW_DLG,						// [진성] 가차 창 변경.				   			=> 2008-8-29
	NCHOISE_ITEM_GAME_START_DLG,		// [진성] 가차 창 아이템 선택 후 게임 시작.		=> 2008-8-29
	NOPENBOX,							// [진성] 가차 상자 낚는 애니메이션 창 .		=> 2008-8-29
	RECIEVED_ITEM_DLG,					// [진성] 가차 상자 아이템 받는 창 .			=> 2008-8-29

	NMYKEY,								// [진성] 나만의 메뉴 큰창.						=> 2008-9-3
	NMYKEYMINI,							// [진성] 나만의 메뉴 작은창.					=> 2008-9-3

	NACUTRADEDLG_PARENT,				// [영훈] 아큐 거래창 리뉴얼 부모창
	NACUTRADEDLG_SELL,					// [영훈] 아큐 거래창 리뉴얼 팔기창
	NACUTRADEDLG_BUY,					// [영훈] 아큐 거래창 리뉴얼 사기창
	NACUTRADEDLG_WITHDRAW,				// [영훈] 아큐 거래창 리뉴얼 회수창		360
	NACUTRADEDLG_PASSPORT,				// [영훈] 아큐 거래창 리뉴얼 여권창

	NAMBER_BOX_DLG,						// [진성] 호박상자 => 2008-9-12

	NCHARBUFF_ROULETTE_DLG,				// [강형] 캐릭터 버프 획득창 => 2008-09-11

	POSTBOX_BASE_DLG,					// [희영] 우체통 - 0919
	POSTBOX_INTRO_DLG,
	POSTBOX_PANPAL_DLG,
	POSTBOX_SENDMSG_DLG,
	POSTBOX_RECEIVEMSG_DLG,

	NYESORNODLG,						// [진성] 예, 아니오 창.							=> 2008-10-08
	NHELPER_DLG,						// [진성] 도우미 창.								=> 2008-10-10
	NHELPER_CHAT_DLG,					// [진성] 도우미 대화 창.						=> 2008-10-10
	NHELPER_QUESTION_OR_ANSWER_DLG,		// [진성] 질문 창이기도 하고 답변창이기도 허네~	=> 2008-11-04
	NHELPER_QUESTION_LIST_DLG,			// [진성] 질문 리스트를 볼 촹~					=> 2008-11-04
	NHELPER_ANSWER_COMPLETE_DLG,		// [진성] 답변 내용 창~							=> 2008-11-04

	NMAGICCOOLTIME_DLG,					// [영훈] 마법 쿨타임 표시창					=> 2008-10-15
	PVPLEAGUE_DLG,						// [영훈] PVP 무한대전 UI						=> 2008-11-04

	NGACHA2_DLG,						// [희영] //KHY - 1104 -  갓차2 -라쿤 보물 상자. 

	NEWYEAR_ROULETTE_DLG,				// [춘기] 신년운세룰렛창

	NELIXIR_STATUS_DLG,					// [진성] 엘릭서 스테이터스 창.
	NELIXIR_USE_DLG,					// [진성] 엘릭서 사용 창.
	NELIXIR_NPC_DLG,					// [진성] 엘릭서 NPC 창.

	NVALANTINE_DLG,						// [성웅] 발렌타인데이 질문 다이얼로그.

	NEWSELECTBOX_DLG,					// [영훈] 새로운 선택상자(신년상자)				=> 2008-12-17

	NNEWMARKET_DLG,						// [진성] 통합마켓
	NNEWMARKET_SUB_DLG,					// [진성] 통합마켓.

	NSUMMON_INVENTORY_DLG,				// [성웅] 가챠 : 인벤토리 소환수

	NGUILDDUNGEON_NEW_DLG,				// [성웅] 길드 사냥터 소유방식변경-사냥터 현황 다이얼로그
	NREQUESTHUNTMAPWARREQUEST,			// [성웅] 길드 사냥터 소유방식변경-길드전 신청 다이얼로그

	NPERIODITEMLIST_DLG,				//	기간제아이템 다이얼로그^

	NCARRYOVERGACHA_DLG,				// [영훈] 이월 가챠 다이얼로그					=> 2009-04-07
	NGACHAPRODUCT_DLG,					// [영훈] 이월 가챠(아이템 다이얼로그)			=> 2009-04-08
	NGACHARESULT_DLG,					// [영훈] 이월 가챠(결과창)						=> 2009-04-09

	NGUILDWARRESULT_DLG,				// [종호] 사냥터획득 길드전 결과창 다이얼로그
	NGUILDWARKILLSIGN_DLG,				// [종호] 사냥터 획득 길드전 전투중 킬뎃 현황 다이얼로그

	NPROMOTER_INFO,						// [진성] 프로모터 캐릭터 정보및 획득 아이템 정보.
	NPROMOTER_URL,						// [진성] 프로모터 URL.
	NPROMOTER_GIVE_ITEM_NOTICE,			// [진성] 프로모터 보상 관련 알림창.
	NPROMOTER_MINI,						// [진성] 프로모터 추천을 한 캐릭터가 레벨업을 할때 1분동안 출력되는 버튼 창.
	NPROMOTER_INTRODUCE,				// [진성] 프로모터 새내기 인사창.

	NWEDDINGCEREMONY_MENU_DLG,			// [영훈] 결혼시스템 : 예식 메뉴 다이얼로그		=> 2009-04-21
	NPROPOSE_QUESTION_DLG,				// [영훈] 결혼시스템 : 프로포즈 질문			=> 2009-04-23
	NPROPOSE_ANSWER_DLG,				// [영훈] 결혼시스템 : 프로포즈 답변			=> 2009-04-28

	NMARRIAGELOVELEVEL_DLG,				// [영훈] 결혼시스템 : 애정도 표시 다이얼로그	=> 2009-05-11
	NWEDDINGANNIVERSARY_DLG,			// [영훈] 결혼시스템 : 결혼 기념일 다이얼로그	=> 2009-05-12

	NWEDDINGCEREMONY_LIST_DLG,			// [영훈] 결혼시스템 : 예식 리스트 다이얼로그	=> 2009-05-18
	NFAMILYINFO_DLG,					// [영훈] 가족 정보창							=> 2009-05-20

	NMYFAMILYINFO_DLG_WITH_CHILDREN,	// [성웅] 가족 정보창							=> 2009-05-25
	NBUTTONMENU_DLG,					// [성웅] 가족 버튼 클릭 팝업 다이얼로그		=> 2009-05-27
	NBALANCECURRENCY_DLG,				// [종호] 통화량 조절 룰렛 버프 창
	NFAMILY_DLG_ADD_CHILDREN,			// [성웅] 자녀 추가 창							=> 2009-05-29
	NFAMILYCOOKIELEVEL_DLG,				// [성웅] 가족시스템 : 쿠키사용표시 다이얼로그  => 2009-06-05

	CLTNSOULGUARDHOUSESTATUS_DLG,		// [영훈] 수호정령 보관소 상태 정보창			=> 2009-06-16
	NCHARSOULGUARDINFO_DLG,				// [영훈] 수호정령 강화 정보창					=> 2009-06-17

	NPCROOM_DISPLAY_DLG,				// [영훈] PC방 연동 표시창						=> 2009-07-01
	NPCROOM_DLG,						// [영훈] PC방 연동 보상목록창					=> 2009-07-01
	NRACOONDODGE_EVENT_DLG,				// [성웅] 라쿤 피하기 이벤트 예약 다이얼로그	==> 2009-07-23
	NRACOONDODGE_EVENT_RESERVELIST_DLG,	// [성웅] 라쿤 피하기 이벤트 예약리스트			==> 2009-08-6
	NBOKBULBOK_EVENT_DLG,				// [지연] 복불복 이벤트							=> 2009-08-06
	NBOKBULBOK_RESERVELIST_DLG,			// [지연] 복불복 이벤트대기자리스트 예약창
	NNEWNPCREPAIR_DLG,					// [강형] NPC 내구도 수리창						=> 2009-08-07
	NGAWIBAWIBO_DLG,					// [종호] 가위바위보

	NDORMANCYARMOURBOX_DLG,				// [기형] 휴면 계정 시스템 : 방어구 보급 상자   => 2009-08-01 
	NDORMANCYWEAPONBOX_DLG,				// [기형] 휴면 계정 시스템 : 무기구 보급 상자   => 2009-08-01 	
	NDORMANCYPOINT_DLG,					// [기형] 휴면 계정 시스템 : 포인트 정보		=> 2009-08-17 	

	NITEMREPAIR_DLG,					// [강형] 아이템 수리 & 수리요청 창
	NAVATARQUEST_DLG,					// [종호] 아바타퀘스트

	NWARNDURABILITY_DLG,				// [영훈] 내구도 경고 표시창					=> 2009-09-03
	NHALLOWEENSPIRITBOX_DLG,			// [종호] 할로윈 스피릿 dlg

	NMAINMAP_DLG,						// [지연] 야외지역 전체맵 창					=> 2009-09-28

	NMONSTERCREATE_DLG,					// [춘기] 몬스터 생성 창						=> 2009-09-30
	NEQUIPMENTCREATE_DLG,				// [춘기] 장비 생성 창							=> 2009-09-30
	NCHANGECHARNAME_DLG,				// [종호] 캐릭터 이름 변경 dlg					=> 2009-10-07	

	NCHARHOUSE_DLG,						// [영훈] 캐릭터 보관소							=> 2009-10-07

	NCOPFIELD_DLG,						// [진성] 스킬북 npc
	NRACCOONEVENT_CHANGE_GOONZUEVENT_DLG,//[성웅] 라쿤이벤트 군주 이벤트로 변경			=> 2009-10-13

	NBATTLEROYALRESULT1_DLG,			// [기형]배틀로얄 경기 결과 
	NBATTLEROYALRESULT2_DLG,			// [기형]배틀로얄 경기 결과 (예비)
	NBATTLEROYALTIME_DLG,				// [기형]배틀로얄 경기가 남은 시간 

	N2010NEWYEAR_FORTUNE_DLG,			// [지연] 2010신년이벤트 - 포춘쿠키 운세창.

	// 파티매칭
	NSELECT_QUEST_BASE_DLG,				// [지연] 초보자 파티 퀘스트 시스템 - 레벨별 퀘스트파티 선택 창.
	NPARTY_MATCHING_BASE_DLG,			// [지연] 초보자 파티 퀘스트 시스템 - 조회된 파티리스트에서 들어갈 파티 선택하는 창.
	NCREATE_PARTY_BASE_DLG,				// [지연] 초보자 파티 퀘스트 시스템 - 파티생성하는 창.
	NPARTY_JOIN_DLG,					// [지연] 파티 가입 요청 대화상자

	NGIANTRACCOONRESURRECTEVENTDLG,		// [진성] 자이언트 라쿤 부활 기부창. 
	NPOTRACCOONDLG,						// [진성] 자이언트 라쿤 부활 부활 빵 기부창. 

	// pvp 맵 npc 이동
	NPVPMAPGODLG,						// [성웅] PVP 맵 이동 npc

	NMANUFACTURE_NEW_DLG,				// [성웅] 제조의뢰 인터페이스 개선
	MANUFACTURESEARCH_DLG,				// [성웅] 제조의뢰 인터페이스 개선->찾기

	NBATTLEROYALMOVE_DLG,				// [기형] 배틀로얄 경기장으로 이동시켜주는 버튼 다이얼로그

	VALENTINEDAY_NPC_DLG,					// [진성] 발렌타인데이 NPC
	VALENTINEDAY_PACKING_DLG,				// [진성] 발렌타인데이 포장하기.
	VALENTINEDAY_TRADE_DLG,					// [진성] 발렌타인데이 포장하기.
	VALENTINEDAY_CONFESSION_QUESTION_DLG,	// [진성] 발렌타인데이 고백	하기.
	VALENTINEDAY_CONFESSION_ANSWER_DLG,		// [진성] 발렌타인데이 고백에 답해주기.

	DIALOG_END
};
//DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER

// 다이얼로그 중간에 추가하시면 안됩니다. DIALOG_END 바로 위에 추가해 주세요. >_< - 영진

// 다이얼로그 위치가 순서대로 파일에 저장되고 있습니다

//DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER DANGER


#endif

