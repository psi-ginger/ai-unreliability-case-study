#ifndef _GLOBALIMGFILE_H
#define _GLOBALIMGFILE_H

#include <Directives.h>
#include <Graphic_Capsule.h>

/*
#define MAX_GLOBAL_IMG_FILE_NUMBER		300

#define GIMG_CHARSHADOW					0
#define GIMG_POISON						1
#define	GIMG_CUT						2
#define GIMG_EXPCOUNTER					3
#define GIMG_WATERDROP					4
#define GIMG_HELPPANNEL					5
#define GIMG_DMGCOUNTER					6
#define GIMG_FIELDITEM					7
#define	GIMG_LEVELUP					8
#define GIMG_REGENERATION				9
#define GIMG_FIREFLY					10			// 반딧불
#define GIMG_BIGBOSS					11			// 대행수
#define GIMG_SMALLBOSS					12			// 행수
#define GIMG_CRITICALEFFECT				13			// 필살기
#define GIMG_CLICKPOINT					14			// 마우스 클릭
#define GIMG_MONSTERINFOBACK			15			// 몬스터 정보창에 있는 몬스터 이미지 배경	
#define GIMG_WARKILL					16			// 전쟁에서 처치한 적군의 모습. 
#define GIMG_WARKILLNUM					17			// 전쟁에서 처치한 적군의 수. 
#define GIMG_JIM						18			// 스킬 방지를 위한 찜
#define GIMG_SKILLLEVELUP				19			// 스킬 레벨 업
#define GIMG_ICEFLASH					20			// 얼음성 반짝이
//#define GIMG_LOADING								// 로딩 중
#define GIMG_SERVERNAME					21			// 서버 이름
#define GIMG_SKILLNAME					22			// 스킬명
#define GIMG_SKILLEXPCOUNTER			23			// 스킬 Counter

// 캐릭터 특수 이펙트 관련 - 부적 붙이고 공격
#define GIMG_SPECIAL_SWORD					24		// 검 공격
#define GIMG_SPECIAL_SPEAR_BRANDISH			25		// 양손 휘두르기 창 공격
#define GIMG_SPECIAL_SPEAR_JAB				26		// 양손 찌르기 창 공격
#define GIMG_SPECIAL_AXE					27		// 도끼 공격

#define GIMG_SPECIAL_SWORD_HORSE			28		// 말 검 공격
#define GIMG_SPECIAL_SPEAR_BRANDISH_HORSE	29		// 말 양손 휘두르기 창 공격
#define GIMG_SPECIAL_SPEAR_JAB_HORSE		30		// 말 양손 찌르기 창 공격
#define GIMG_SPECIAL_AXE_HORSE				31		// 말 도끼 공격

#define GIMG_SPECIAL_SWORDDMG				32		// 검 공격에 맞음
#define GIMG_SPECIAL_SPEARDMG_BRANDISH		33		// 양손 휘두르기 창 공격에 맞음
#define GIMG_SPECIAL_SPEARDMG_JAB			34		// 양손 찌르기 공격에 맞음
#define GIMG_SPECIAL_AXEDMG					35		// 도끼 공격에 맞음

// 학교 관련
#define GIMG_SCHOOL1						40		// 학교 로고 1
#define GIMG_SCHOOL2						41		// 학교 로고 2
#define GIMG_SCHOOL3						42		// 학교 로고 3

#define GIMG_MAGIC_THUNDER					50

#define GIMG_MAGIC_ROCKSLIDE				51			// 낙석 마법
#define GIMG_MAGIC_FLAME					52
#define GIMG_MAGIC_WHIRLPOOL				53

#define GIMG_MAGIC_WAR						54

#define GIMG_MAGIC_PROMPTBAR				55

#define GIMG_MAGIC_DEFUP					56			// 방어력 향상. 

#define GIMG_VILLAGENAMEBACK				57		// 마을이름 배경.  

#define GIMG_FIRE1							58		// 불1
#define GIMG_FIRE2							59		// 불2
#define GIMG_FIRE3							60		// 불3
#define GIMG_FIRE4							61		// 불4

#define GIMG_NOFIRSTATTACK					62		// 선공 방어 효과. 

#define GIMG_RANKMARK						63		// 기자등의 신분 표시.

#define GIMG_HEAL							64		// 체력 회복 마법 효과. 

#define GIMG_MAKEITEMDRAWPART1				65		// 물품 생산 출력 정보(1칸짜리)
#define GIMG_MAKEITEMDRAWPART2				66		// 물품 생산 출력 정보(틀)
#define GIMG_MAKEITEMDRAWPART3				67		// 물품 생산 출력 정보(숫자)

#define GIMG_PARTYEMOTICON					68		// 파티 동료들 위에 보여주는 것

#define GIMG_GAMESTATUS						69		// 게임의 상태에 따라 화면에 보여준다. (공성전, 보너스시간)
#define GIMG_KINGEFFECT						70		// 왕위에 찍어줄 Effect
#define GIMG_GODEFFECT						71		// 성황신 위에 찍어줄 Effect

#define GIMG_FRIENDLOGIN					72		// 친구 로그인 할때 보여 줄 부분

#define GIMG_NAESI							73		// 내시 이미지
#define GIMG_ICEFROZEN						74		// 얼음에 얼렸을때 
#define GIMG_MAGIC_DRAIN					75		//  
#define GIMG_HEALCOUNTER					76		// HEAL COUNT

#define GIMG_VILLAGENOTICEBACKGROUND		77		// 마을 공지문

#define GIMG_MAGIC_POISONFLAME				87		// MAGIC_STAFF_POISONFLAME 중독방사술 

#define GIMG_SYIDEOCK						88		// 김시덕
#define GIMG_NOICEATTACK					89		// ICE 방어 효과. 
#define GIMG_SUMMONMARK						90		// 소환수 마크 

#define GIMG_VILLAGEMARK01					91		// 길드(마을) 마크
#define GIMG_MAGIC_DRAGONCANNON				92		// 
#define GIMG_MAGIC_DRAGONSHADOW				93		// 

#define GIMG_GWANGGAETO_EFFECT				94		//
#define GIMG_MAGIC_DROPMONEY				95		// 마법 낙전술 
#define GIMG_MAGIC_BLACKINK					96		// 마법 먹물 
#define GIMG_MAGIC_DROPSNOWMAN				97		// 마법 눈사람 떨구기 

#define GIMG_MAGIC_DROPFORK_FRONT			98		// 마법 마법 떨구기 
#define GIMG_MAGIC_DROPFORK_BACK			99		// 마법 마법 떨구기 


#define GIMG_RESIDENCELOGIN					100		// 마을 주민 로그인 할때 보여 줄 부분

#define GIMG_MAGIC_CURSEATTACK				101		// 공격력 저하 
#define GIMG_MAGIC_CURSECRITICAL			102		// Critical 저하 
#define GIMG_MAGIC_CURSEDEFENSE				103		// 방어력 저하 
#define GIMG_MAGIC_CURSEMOVESPEED			104		// 이동속도 저하 

//#define GIMG_ITEMIMG0						105		// 아이템 이미지. 
//#define GIMG_ITEMIMG1						106		// 아이템 이미지. 
//#define GIMG_ITEMIMG2						107		// 아이템 이미지. 
//#define GIMG_ITEMIMG3						116		// 아이템 이미지. 

#define GMIG_MAGIC_SWORDBOMB1				108		// 권율마법 
#define GMIG_MAGIC_SWORDBOMB2				109

#define GIMG_NOTICEEVENT_STAMP1				110		// 도장 찍기 이벤트1
#define GIMG_NOTICEEVENT_STAMP2				111		// 도장 찍기 이벤트2
#define GIMG_NOTICEEVENT_STAMP3				112		// 도장 찍기 이벤트3

#define GIMG_HUMAN_EFFECT					113		// 특수한 캐릭터에게 찍는 이미지

#define GIMG_MAKEMASTER						114		// 최고 제조 장인 표시 


#define GIMG_INTERFACE_CLOSEBUTTON			115		// Interface CloseButton

#define GIMG_MAGIC_UPPERDRAGON1				118		// 장보고 마법1
#define GIMG_MAGIC_UPPERDRAGON2				119		// 장보고 마법2
#define GIMG_MAGIC_UPPERDRAGON3				120		// 장보고 마법3

#define GIMG_FOG							121		// 포그
#define GIMG_PCROOM_MARK					122		// 캐릭터 옆에 pc방 표시
#define GIMG_PCROOM_STATUS					123		// 화면에 pc방 표시

#define GIMG_MAGICFIRE						124		// 몸에 붙는 마법의 불
#define GIMG_MAGICFIRE_BURN					125		// 불해티 마법
#define GIMG_MAGICFIRE_BURNRING				126		// 불해티 마법링
#define GIMG_VILLAGEUNITNPC					127		// 건물 초상

#define GIMG_JINSOO							128		// 진수
#define GIMG_SYIDEOCKCRY					129		// 시덕 우는 모양

#define GIMG_SWORDMETEOR					130		// 낙하수리검
#define GIMG_MAGICFROZEN					131		// 얼음성 보스에게 얼렸을 때

#define GIMG_MAGIC_FIREWHIRLWIND			132		// 돌개바람 마법

#define GIMG_NEW_ATTACK_STAFF				133		// 주먹, 지팡이
#define GIMG_NEW_ATTACK_SWORD				134		// 검
#define GIMG_NEW_ATTACK_SPEAR				135		// 창
#define GIMG_NEW_ATTACK_AXE					136		// 도끼
#define GIMG_NEW_ATTACK_BOWGUN				137		// 활, 총

#define GIMG_NEW_ATTACK_STAFF2				138		// 주먹, 지팡이
#define GIMG_NEW_ATTACK_SWORD2				139		// 검
#define GIMG_NEW_ATTACK_SPEAR2				140		// 창
#define GIMG_NEW_ATTACK_AXE2				141		// 도끼
#define GIMG_NEW_ATTACK_BOWGUN2				142		// 활, 총

#define GIMG_MAGIC_SUMMONDRAGON				143		// 김시민 마법

#define GIMG_SETITEMEFFECT1					144		// 세트아이템1
#define GIMG_SETITEMEFFECT2					145		// 세트아이템

#define GIMG_MAGIC_EARTHQUAKE				146		// 지폭술

//#define GIMG_ARROW							147		// NPC 위에 나올 화살표
//#define GIMG_ARROWEFFECT					147		// NPC 위에 나올 화살표 이펙트
#define GIMG_QUESTNPCEFFECT					147		// NPC 위에 나올 퀘스트 이펙트
#define GIMG_CLICKMOUSE						148		// NPC 위에 나올 화살표 이펙트

#define GIMG_PREMIUM_STATUS					149	// 프리미엄 서비스

#define GIMG_MAGIC_THUNDER1					150

#define GIMG_RANKING						151		// 전쟁 랭킹 이미지
#define GIMG_MVPMARK1						152		// 전쟁 MVP 이미지
#define GIMG_GMMARK							153		// GM 이미지
#define GIMG_MVP_EFFECT						154		// MVP 선정시 이펙트
#define GIMG_MVP_AROUND_EFFECT				155		// MVP 선정시 주위에 뿌려지는 이펙트

#define GIMG_SEEDDROP1						156		// 홀씨1
#define GIMG_SEEDDROP2						157		// 홀씨2
#define GIMG_SEEDDROP3						158		// 홀씨3
#define GIMG_SEEDDROP4						159		// 홀씨4

#define GIMG_ITEMSEALED						160		// 아이템봉인이미지

#define GIMG_MAGIC_FROZENGOLEM				161		// 프로즌골렘 마법
#define GIMG_MAGIC_FROZENQUEEN				162		// 프로즌퀸 마법
#define GIMG_MAGIC_FROZENSORCERER			163		// 프로즌소서러 마법
#define GIMG_MAGIC_GOTPA					164		// 온천갓파 마법

#define GIMG_ENCHANT_EFFECT_HERO_WE			165		// 인첸트 이펙트 남자 무기
#define GIMG_ENCHANT_EFFECT_HERO_AM			166		// 인첸트 이펙트 남자 방어구
#define GIMG_ENCHANT_EFFECT_HEROINE_WE		167		// 인첸트 이펙트 여자 무기
#define GIMG_ENCHANT_EFFECT_HEROINE_AM		168		// 인첸트 이펙트 여자 방어구

#define GIMG_MAGIC_DRACULA					169		// 드라큘라 마법
#define GIMG_MAGIC_DRACULAFX				170		// 드라큘라 마법 FX

#define	GIMG_MAGIC_PRIEST					171		// 타락성직자 마법

#define	GIMG_BUFIMAGE						172		// 버프이미지들
#define GIMG_BEGINWARP						173		// 워프이미지

#define GIMG_QUEST_EFFECT_RECEIVE				174		// 퀘스트 받기 이펙트
#define GIMG_QUEST_EFFECT_CLEAR					175		// 퀘스트 클리어 이펙트

#define GIMG_ITEMIMG01						181		// 아이템 이미지. 
#define GIMG_ITEMIMG02						182		// 아이템 이미지. 
#define GIMG_ITEMIMG03						183		// 아이템 이미지. 
#define GIMG_ITEMIMG04						184		// 아이템 이미지. 
#define GIMG_ITEMIMG05						185		// 아이템 이미지. 
#define GIMG_ITEMIMG06						186		// 아이템 이미지. 
#define GIMG_ITEMIMG07						187		// 아이템 이미지. 
#define GIMG_ITEMIMG08						188		// 아이템 이미지. 
#define GIMG_ITEMIMG09						189		// 아이템 이미지. 
#define GIMG_ITEMIMG10						190		// 아이템 이미지. 
#define GIMG_ITEMIMG11						191		// 아이템 이미지. 
#define GIMG_ITEMIMG12						192		// 아이템 이미지. 
#define GIMG_ITEMIMG13						193		// 아이템 이미지. 
#define GIMG_ITEMIMG14						194		// 아이템 이미지. 
#define GIMG_ITEMIMG15						195		// 아이템 이미지. 
#define GIMG_ITEMIMG16						196		// 아이템 이미지. 
#define GIMG_ITEMIMG17						197		// 아이템 이미지. 
#define GIMG_ITEMIMG18						198		// 아이템 이미지. 
#define GIMG_ITEMIMG19						199		// 아이템 이미지. 
#define GIMG_ITEMIMG20						200		// 아이템 이미지. 
#define GIMG_VILLAGEWAR_NOTICE				201		// 아이템 이미지. 

#define GIMG_MAGIC_MANDRAGORA				202		// 만드라고라 마법 이미지.
#define GIMG_BUFPERCENT						203		// 버프 이미지 밑에 효과 %수치

#define GIMG_SKILLLEVELUP_EFF				204		// 스킬레벨업 이펙트
#define GIMG_LEVELUP_EFF					205		// 스킬레벨업 이펙트

#define GIMG_CHRISTMAS_BUF					206		// 스킬레벨업 이펙트

#define GIMG_GUILDWAR_NOTICE				207		// 길드전 공지 이미지
#define GIMG_GUILDWAR_COUNT					208		// 길드전 점수 이미지

#define GIMG_NEWYEAR_EVENT_NOTICE			209

#define GIMG_SUMMON_EVENT_BUF				210
#define GIMG_EMOTICON						211		// 이모티콘

#define GIMG_MAGIC_PAPAGOBIN				212		//파파고블린 마법 이미지. 
#define GIMG_HEART_EFFECT					213		// 바닥 하트 이펙트
#define GIMG_EMOTICON_SMALL					214		// 채팅 이모티콘

#define GIMG_MISSHIT						215		// 채팅 이모티콘
	
#define GIMG_HORSEWING1						216		// 말 날개 이미지1 ( 1~8까지 연속된 값을 가져야 합니다)
#define GIMG_HORSEWING2						217		// 말 날개 이미지2
#define GIMG_HORSEWING3						218		// 말 날개 이미지3
#define GIMG_HORSEWING4						219		// 말 날개 이미지4
#define GIMG_HORSEWING5						220		// 말 날개 이미지5
#define GIMG_HORSEWING6						221		// 말 날개 이미지6
#define GIMG_HORSEWING7						222		// 말 날개 이미지7
#define GIMG_HORSEWING8						223		// 말 날개 이미지8

#define GIMG_QUESTARROW						224		// 퀘스트 방향 표시

#define GIMG_FOURLEAF						225		// 네잎 클로버

#define GIMG_GOVERNMENT_RANKMARK			226		// 영등위 등급표시

#define GIMG_CUSHION_1						227		// 쿠션 이미지 1
#define GIMG_CUSHION_2						228		// 쿠션 이미지 2
#define GIMG_CUSHION_3						229		// 쿠션 이미지 3
#define GIMG_CUSHION_4						230		// 쿠션 이미지 4

#define GIMG_CUSHION_CAT_1					231		// 쿠션 고양이 이미지 1
#define GIMG_CUSHION_CAT_2					232		// 쿠션 고양이 이미지 2
#define GIMG_CUSHION_CAT_3					233		// 쿠션 고양이 이미지 3
#define GIMG_CUSHION_CAT_4					234		// 쿠션 고양이 이미지 4

#define GIMG_SPECIAL_PCROOM_MARK_1			235		// 군주_스페셜 PC방 마크(순간이동무제한)
#define GIMG_SPECIAL_PCROOM_MARK_2			236		// 군주_스페셜 PC방 마크(사냥경험치)

#define GIMG_SPECIAL_PCROOM_LARGE_MARK		237		// 
#define GIMG_SPECIAL_NO_PCROOM_LARGE_MARK	238		// 

#define GIMG_DAMAGE_PC_0					239
#define GIMG_DAMAGE_PC_1					240
#define GIMG_DAMAGE_PC_2					241
#define GIMG_DAMAGE_PC_3					242
#define GIMG_DAMAGE_PC_4					243
#define GIMG_DAMAGE_PC_5					244
#define GIMG_DAMAGE_PC_6					245
#define GIMG_DAMAGE_PC_7					246
#define GIMG_DAMAGE_PC_8					247
#define GIMG_DAMAGE_PC_9					248
#define GIMG_DAMAGE_NPC_0					249
#define GIMG_DAMAGE_NPC_1					250
#define GIMG_DAMAGE_NPC_2					251
#define GIMG_DAMAGE_NPC_3					252
#define GIMG_DAMAGE_NPC_4					253
#define GIMG_DAMAGE_NPC_5					254
#define GIMG_DAMAGE_NPC_6					255
#define GIMG_DAMAGE_NPC_7					256
#define GIMG_DAMAGE_NPC_8					257
#define GIMG_DAMAGE_NPC_9					258
#define GIMG_DAMAGE_PC_MISS					259
#define GIMG_DAMAGE_NPC_MISS				260
#define GIMG_DAMAGE_CRITICAL				261

#define GIMG_ITEMEXPIRED					262
#define GIMG_ITEMCANNOTEQIP					263

//KHY - 1109 - 가챠레어- 강아지방석추가
#define GIMG_CUSHIOND_1						264		// 쿠션 이미지 5
#define GIMG_CUSHIOND_2						265		// 쿠션 이미지 6
#define GIMG_CUSHIOND_3						266		// 쿠션 이미지 7
#define GIMG_CUSHIOND_4						267		// 쿠션 이미지 8

#define GIMG_CUSHION_DOG_1					268		// 쿠션 강아지 이미지 1
#define GIMG_CUSHION_DOG_2					269		// 쿠션 강아지 이미지 2
#define GIMG_CUSHION_DOG_3					270		// 쿠션 강아지 이미지 3
#define GIMG_CUSHION_DOG_4					271		// 쿠션 강아지 이미지 4

#define GIMG_VILLAGEMARK02					272		// 길드(마을) 마크2
#define GIMG_VILLAGEMARK03					273		// 길드(마을) 마크3

// [영훈] PC방 프리미엄 강화(제조,네잎관련 PC방 마크 추가) - 2008.03.02
#define GIMG_SPECIAL_PCROOM_MARK_3			274		// 군주_스페셜 PC방 마크(제조경험치)
#define GIMG_SPECIAL_PCROOM_MARK_4			275		// 군주_스페셜 PC방 마크(네잎클로버지급)

//KHY - 0313 - 변신 100%효과 발생.
#define GIMG_TRANSFORM_SYNCHRO100			276		// 변신 100% 효과 

//PCK : 용갱장 마법 추가 (08.03.23)
#define GIMG_MAGIC_EMPERORMASTER			277		// 용갱장 마법

// [진성] 아티펙트 관련 이미지 추가.
#define GIMG_ARTIFACT_SLOT_STATUS			278
#define GIMG_ARTIFACT_GAUGE_BACK			279	
#define GIMG_ARTIFACT_GAUGE					280
#define GIMG_ARTIFACT_SLOT_EFFECT			281
#define GIMG_ARTIFACT_SLOT_REMOVE			282

// PCK : 괴수손 마법 추가 (08.04.11)
#define GIMG_MAGIC_GREATWALLSOLDIER		283
#define GIMG_MAGIC_GREATWALLSOLDIERFX	284

// PCK : 머리만 마법 추가 (08.04.11)
#define GIMG_MAGIC_GREATWALLMAGICIAN	285

// [진성] 화살표 들어왔다 나가는 이미지.
#define GIMG_ARROW_IN_OUT				286
*/
enum GLOBAL_IMAGE
{
	GIMG_CHARSHADOW						= 0,
	GIMG_POISON							= 1,		
	GIMG_SETITEM_POISON,
	GIMG_CUT,
	GIMG_EXPCOUNTER,
	GIMG_WATERDROP,
	GIMG_HELPPANNEL,
	GIMG_DMGCOUNTER,
	GIMG_FIELDITEM,
	GIMG_LEVELUP,
	GIMG_REGENERATION,
	GIMG_FIREFLY,								// 반딧불
	GIMG_BIGBOSS,								// 대행수
	GIMG_SMALLBOSS,								// 행수
	GIMG_CRITICALEFFECT,						// 필살기
	GIMG_CLICKPOINT,							// 마우스 클릭
	GIMG_MONSTERINFOBACK,						// 몬스터 정보창에 있는 몬스터 이미지 배경	
	GIMG_WARKILL,								// 전쟁에서 처치한 적군의 모습. 
	GIMG_WARKILLNUM,							// 전쟁에서 처치한 적군의 수. 
	GIMG_JIM,									// 스킬 방지를 위한 찜
	GIMG_SKILLLEVELUP,							// 스킬 레벨 업
	GIMG_ICEFLASH,								// 얼음성 반짝이

	GIMG_SERVERNAME,							// 서버 이름
	GIMG_SKILLNAME,								// 스킬명
	GIMG_SKILLEXPCOUNTER,						// 스킬 Counter

	// 캐릭터 특수 이펙트 관련 - 부적 붙이고 공격
	GIMG_SPECIAL_SWORD,							// 검 공격
	GIMG_SPECIAL_SPEAR_BRANDISH,				// 양손 휘두르기 창 공격
	GIMG_SPECIAL_SPEAR_JAB,						// 양손 찌르기 창 공격
	GIMG_SPECIAL_AXE,							// 도끼 공격

	GIMG_SPECIAL_SWORD_HORSE,					// 말 검 공격
	GIMG_SPECIAL_SPEAR_BRANDISH_HORSE,			// 말 양손 휘두르기 창 공격
	GIMG_SPECIAL_SPEAR_JAB_HORSE,				// 말 양손 찌르기 창 공격
	GIMG_SPECIAL_AXE_HORSE,						// 말 도끼 공격

	GIMG_SPECIAL_SWORDDMG,						// 검 공격에 맞음
	GIMG_SPECIAL_SPEARDMG_BRANDISH,				// 양손 휘두르기 창 공격에 맞음
	GIMG_SPECIAL_SPEARDMG_JAB,					// 양손 찌르기 공격에 맞음
	GIMG_SPECIAL_AXEDMG,						// 도끼 공격에 맞음

	// 학교 관련
	GIMG_SCHOOL1,								// 학교 로고 1
	GIMG_SCHOOL2,								// 학교 로고 2
	GIMG_SCHOOL3,								// 학교 로고 3

	GIMG_MAGIC_THUNDER,

	GIMG_MAGIC_ROCKSLIDE,						// 낙석 마법
	GIMG_MAGIC_FLAME,
	GIMG_MAGIC_WHIRLPOOL,

	GIMG_MAGIC_WAR,

	GIMG_MAGIC_PROMPTBAR,

	GIMG_MAGIC_DEFUP,							// 방어력 향상. 

	GIMG_VILLAGENAMEBACK,						// 마을이름 배경.  

	GIMG_FIRE1,									// 불1
	GIMG_FIRE2,									// 불2
	GIMG_FIRE3,									// 불3
	GIMG_FIRE4,									// 불4

	GIMG_NOFIRSTATTACK,							// 선공 방어 효과. 

	GIMG_RANKMARK,								// 기자등의 신분 표시.

	GIMG_HEAL,									// 체력 회복 마법 효과. 

	GIMG_MAKEITEMDRAWPART1,						// 물품 생산 출력 정보(1칸짜리)
	GIMG_MAKEITEMDRAWPART2,						// 물품 생산 출력 정보(틀)
	GIMG_MAKEITEMDRAWPART3,						// 물품 생산 출력 정보(숫자)

	GIMG_PARTYEMOTICON,							// 파티 동료들 위에 보여주는 것

	//GIMG_GAMESTATUS,							// 게임의 상태에 따라 화면에 보여준다. (공성전, 보너스시간)
	GIMG_KINGEFFECT,							// 왕위에 찍어줄 Effect
	GIMG_GODEFFECT,								// 성황신 위에 찍어줄 Effect

	GIMG_FRIENDLOGIN,							// 친구 로그인 할때 보여 줄 부분

	GIMG_NAESI,									// 내시 이미지
	GIMG_ICEFROZEN,								// 얼음에 얼렸을때 
	GIMG_MAGIC_DRAIN,
	GIMG_HEALCOUNTER,							// HEAL COUNT

	GIMG_VILLAGENOTICEBACKGROUND,				// 마을 공지문

	GIMG_MAGIC_POISONFLAME,						// MAGIC_STAFF_POISONFLAME 중독방사술 

	GIMG_SYIDEOCK,								// 김시덕
	GIMG_NOICEATTACK,							// ICE 방어 효과. 
	GIMG_SUMMONMARK,							// 소환수 마크 

	GIMG_VILLAGEMARK01,							// 길드(마을) 마크
	GIMG_MAGIC_DRAGONCANNON,
	GIMG_MAGIC_DRAGONSHADOW,

	GIMG_GWANGGAETO_EFFECT,
	GIMG_MAGIC_DROPMONEY,						// 마법 낙전술 
	GIMG_MAGIC_BLACKINK,						// 마법 먹물 
	GIMG_MAGIC_DROPSNOWMAN,						// 마법 눈사람 떨구기 

	GIMG_MAGIC_DROPFORK_FRONT,					// 마법 마법 떨구기 
	GIMG_MAGIC_DROPFORK_BACK,					// 마법 마법 떨구기 

	GIMG_RESIDENCELOGIN,						// 마을 주민 로그인 할때 보여 줄 부분

	GIMG_MAGIC_CURSEATTACK,						// 공격력 저하 
	GIMG_MAGIC_CURSECRITICAL,					// Critical 저하 
	GIMG_MAGIC_CURSEDEFENSE,					// 방어력 저하 
	GIMG_MAGIC_CURSEMOVESPEED,					// 이동속도 저하 

	GMIG_MAGIC_SWORDBOMB1,						// 권율마법 
	GMIG_MAGIC_SWORDBOMB2,
	
	GIMG_MAGIC_SKILL_BOOK_ICERAIN,				// 스킬북 마법.
	GIMG_MAGIC_SKILL_BOOK_CRASH,	
	GIMG_MAGIC_SKILL_BOOK_TORNADO,


	GIMG_NOTICEEVENT_STAMP1,					// 도장 찍기 이벤트1
	GIMG_NOTICEEVENT_STAMP2,					// 도장 찍기 이벤트2
	GIMG_NOTICEEVENT_STAMP3,					// 도장 찍기 이벤트3

	GIMG_HUMAN_EFFECT,							// 특수한 캐릭터에게 찍는 이미지
	GIMG_HUMAN_EFFECT1,							// 특수한 캐릭터에게 찍는 이미지
	GIMG_HUMAN_EFFECT2,							// 특수한 캐릭터에게 찍는 이미지
	GIMG_HUMAN_EFFECT3,							// 특수한 캐릭터에게 찍는 이미지
	GIMG_HUMAN_EFFECT4,							// 특수한 캐릭터에게 찍는 이미지
	GIMG_HUMAN_EFFECT5,							// 특수한 캐릭터에게 찍는 이미지
	GIMG_HUMAN_EFFECT6,							// 특수한 캐릭터에게 찍는 이미지
	GIMG_HUMAN_EFFECT7,							// 특수한 캐릭터에게 찍는 이미지
	
	GIMG_MAKEMASTER,							// 최고 제조 장인 표시 

	GIMG_INTERFACE_CLOSEBUTTON,					// Interface CloseButton

	GIMG_MAGIC_UPPERDRAGON1,					// 장보고 마법1
	GIMG_MAGIC_UPPERDRAGON2,					// 장보고 마법2
	GIMG_MAGIC_UPPERDRAGON3,					// 장보고 마법3

	GIMG_FOG,									// 포그
	GIMG_PCROOM_MARK,							// 캐릭터 옆에 pc방 표시
	GIMG_PCROOM_STATUS,							// 화면에 pc방 표시

	GIMG_MAGICFIRE,								// 몸에 붙는 마법의 불
	GIMG_MAGICFIRE_BURN,						// 불해티 마법
	GIMG_MAGICFIRE_BURNRING,					// 불해티 마법링
	GIMG_VILLAGEUNITNPC,						// 건물 초상

	GIMG_JINSOO,								// 진수
	GIMG_SYIDEOCKCRY,							// 시덕 우는 모양

	GIMG_SWORDMETEOR,							// 낙하수리검
	GIMG_MAGICFROZEN,							// 얼음성 보스에게 얼렸을 때

	GIMG_MAGIC_FIREWHIRLWIND,					// 돌개바람 마법

	GIMG_NEW_ATTACK_STAFF,						// 주먹, 지팡이
	GIMG_NEW_ATTACK_SWORD,						// 검
	GIMG_NEW_ATTACK_SPEAR,						// 창
	GIMG_NEW_ATTACK_AXE,						// 도끼
	GIMG_NEW_ATTACK_BOWGUN,						// 활, 총

	GIMG_NEW_ATTACK_STAFF2,						// 주먹, 지팡이
	GIMG_NEW_ATTACK_SWORD2,						// 검
	GIMG_NEW_ATTACK_SPEAR2,						// 창
	GIMG_NEW_ATTACK_AXE2,						// 도끼
	GIMG_NEW_ATTACK_BOWGUN2,					// 활, 총

	GIMG_MAGIC_SUMMONDRAGON,					// 김시민 마법

	GIMG_MAGIC_EARTHQUAKE,						// 지폭술

	GIMG_QUESTNPCEFFECT,						// NPC 위에 나올 퀘스트 이펙트
	GIMG_CLICKMOUSE,							// NPC 위에 나올 화살표 이펙트

	GIMG_PREMIUM_STATUS,						// 프리미엄 서비스

	GIMG_MAGIC_THUNDER1,

	GIMG_RANKING,								// 전쟁 랭킹 이미지
	GIMG_MVPMARK1,								// 전쟁 MVP 이미지
	GIMG_GMMARK,								// GM 이미지
	GIMG_MVP_EFFECT,							// MVP 선정시 이펙트
	GIMG_MVP_AROUND_EFFECT,						// MVP 선정시 주위에 뿌려지는 이펙트

	GIMG_SEEDDROP1,								// 홀씨1
	GIMG_SEEDDROP2,								// 홀씨2
	GIMG_SEEDDROP3,								// 홀씨3
	GIMG_SEEDDROP4,								// 홀씨4

	GIMG_ITEMSEALED,							// 아이템봉인이미지

	GIMG_MAGIC_FROZENGOLEM,						// 프로즌골렘 마법
	GIMG_MAGIC_FROZENQUEEN,						// 프로즌퀸 마법
	GIMG_MAGIC_FROZENSORCERER,					// 프로즌소서러 마법
	GIMG_MAGIC_GOTPA,							// 온천갓파 마법

	GIMG_ENCHANT_EFFECT_HERO_WE,				// 인첸트 이펙트 남자 무기
	GIMG_ENCHANT_EFFECT_HERO_AM,				// 인첸트 이펙트 남자 방어구
	GIMG_ENCHANT_EFFECT_HEROINE_WE,				// 인첸트 이펙트 여자 무기
	GIMG_ENCHANT_EFFECT_HEROINE_AM,				// 인첸트 이펙트 여자 방어구

	GIMG_MAGIC_DRACULA,							// 드라큘라 마법
	GIMG_MAGIC_DRACULAFX,						// 드라큘라 마법 FX

	GIMG_MAGIC_PRIEST,							// 타락성직자 마법

	GIMG_BUFIMAGE,								// 버프이미지들
	GIMG_BEGINWARP,								// 워프이미지

	GIMG_QUEST_EFFECT_RECEIVE,					// 퀘스트 받기 이펙트
	GIMG_QUEST_EFFECT_CLEAR,					// 퀘스트 클리어 이펙트

	GIMG_ITEMIMG01,								// 아이템 이미지. 
	GIMG_ITEMIMG02,								// 아이템 이미지. 
	GIMG_ITEMIMG03,								// 아이템 이미지. 
	GIMG_ITEMIMG04,								// 아이템 이미지. 
	GIMG_ITEMIMG05,								// 아이템 이미지. 
	GIMG_ITEMIMG06,								// 아이템 이미지. 
	GIMG_ITEMIMG07,								// 아이템 이미지. 
	GIMG_ITEMIMG08,								// 아이템 이미지. 
	GIMG_ITEMIMG09,								// 아이템 이미지. 
	GIMG_ITEMIMG10,								// 아이템 이미지. 
	GIMG_ITEMIMG11,								// 아이템 이미지. 
	GIMG_ITEMIMG12,								// 아이템 이미지. 
	GIMG_ITEMIMG13,								// 아이템 이미지. 
	GIMG_ITEMIMG14,								// 아이템 이미지. 
	GIMG_ITEMIMG15,								// 아이템 이미지. 
	GIMG_ITEMIMG16,								// 아이템 이미지. 
	GIMG_ITEMIMG17,								// 아이템 이미지. 
	GIMG_ITEMIMG18,								// 아이템 이미지. 
	GIMG_ITEMIMG19,								// 아이템 이미지. 
	GIMG_ITEMIMG20,								// 아이템 이미지. 
	GIMG_VILLAGEWAR_NOTICE,						// 아이템 이미지. 

	GIMG_MAGIC_MANDRAGORA,						// 만드라고라 마법 이미지.
	GIMG_BUFPERCENT,							// 버프 이미지 밑에 효과 %수치

	GIMG_SKILLLEVELUP_EFF,						// 스킬레벨업 이펙트
	GIMG_LEVELUP_EFF,							// 스킬레벨업 이펙트

	GIMG_CHRISTMAS_BUF,							// 스킬레벨업 이펙트

	GIMG_GUILDWAR_NOTICE,						// 길드전 공지 이미지
	GIMG_GUILDWAR_COUNT,						// 길드전 점수 이미지

	GIMG_NEWYEAR_EVENT_NOTICE,

	GIMG_SUMMON_EVENT_BUF,
	GIMG_EMOTICON,								// 이모티콘
	GIMG_VALENTINEEMOTICON,						// 발렌타인 이모티콘

	GIMG_MAGIC_PAPAGOBIN,						// 파파고블린 마법 이미지. 
	GIMG_HEART_EFFECT,							// 바닥 하트 이펙트
	GIMG_EMOTICON_SMALL,						// 채팅 이모티콘

	GIMG_MISSHIT,								// 채팅 이모티콘

	GIMG_HORSEWING1,							// 말 날개 이미지1 ( 1~8까지 연속된 값을 가져야 합니다)
	GIMG_HORSEWING2,							// 말 날개 이미지2
	GIMG_HORSEWING3,							// 말 날개 이미지3
	GIMG_HORSEWING4,							// 말 날개 이미지4
	GIMG_HORSEWING5,							// 말 날개 이미지5
	GIMG_HORSEWING6,							// 말 날개 이미지6
	GIMG_HORSEWING7,							// 말 날개 이미지7
	GIMG_HORSEWING8,							// 말 날개 이미지8

	GIMG_QUESTARROW,							// 퀘스트 방향 표시

	GIMG_FOURLEAF,								// 네잎 클로버

	GIMG_GOVERNMENT_RANKMARK,					// 영등위 등급표시

	GIMG_CUSHION_1,								// 쿠션 이미지 1
	GIMG_CUSHION_2,								// 쿠션 이미지 2
	GIMG_CUSHION_3,								// 쿠션 이미지 3
	GIMG_CUSHION_4,								// 쿠션 이미지 4

	GIMG_CUSHION_CAT_1,							// 쿠션 고양이 이미지 1
	GIMG_CUSHION_CAT_2,							// 쿠션 고양이 이미지 2
	GIMG_CUSHION_CAT_3,							// 쿠션 고양이 이미지 3
	GIMG_CUSHION_CAT_4,							// 쿠션 고양이 이미지 4

	GIMG_SPECIAL_PCROOM_MARK_1,					// 군주_스페셜 PC방 마크(순간이동무제한)
	GIMG_SPECIAL_PCROOM_MARK_2,					// 군주_스페셜 PC방 마크(사냥경험치)

	GIMG_SPECIAL_PCROOM_LARGE_MARK,				// 
	GIMG_SPECIAL_NO_PCROOM_LARGE_MARK,			// 

	GIMG_DAMAGE_PC_0,
	GIMG_DAMAGE_PC_1,
	GIMG_DAMAGE_PC_2,
	GIMG_DAMAGE_PC_3,
	GIMG_DAMAGE_PC_4,
	GIMG_DAMAGE_PC_5,
	GIMG_DAMAGE_PC_6,
	GIMG_DAMAGE_PC_7,
	GIMG_DAMAGE_PC_8,
	GIMG_DAMAGE_PC_9,
	GIMG_DAMAGE_NPC_0,
	GIMG_DAMAGE_NPC_1,
	GIMG_DAMAGE_NPC_2,
	GIMG_DAMAGE_NPC_3,
	GIMG_DAMAGE_NPC_4,
	GIMG_DAMAGE_NPC_5,
	GIMG_DAMAGE_NPC_6,
	GIMG_DAMAGE_NPC_7,
	GIMG_DAMAGE_NPC_8,
	GIMG_DAMAGE_NPC_9,
	GIMG_DAMAGE_PC_MISS,
	GIMG_DAMAGE_NPC_MISS,
	GIMG_DAMAGE_CRITICAL,

	GIMG_ITEMEXPIRED,
	GIMG_ITEMCANNOTEQIP,

	//KHY - 1109 - 가챠레어- 강아지방석추가
	GIMG_CUSHIOND_1,							// 쿠션 이미지 5
	GIMG_CUSHIOND_2,							// 쿠션 이미지 6
	GIMG_CUSHIOND_3,							// 쿠션 이미지 7
	GIMG_CUSHIOND_4,							// 쿠션 이미지 8

	GIMG_CUSHION_DOG_1,							// 쿠션 강아지 이미지 1
	GIMG_CUSHION_DOG_2,							// 쿠션 강아지 이미지 2
	GIMG_CUSHION_DOG_3,							// 쿠션 강아지 이미지 3
	GIMG_CUSHION_DOG_4,							// 쿠션 강아지 이미지 4

	GIMG_VILLAGEMARK02,							// 길드(마을) 마크2
	GIMG_VILLAGEMARK03,							// 길드(마을) 마크3

	// [영훈] PC방 프리미엄 강화(제조,네잎관련 PC방 마크 추가) - 2008.03.02
	GIMG_SPECIAL_PCROOM_MARK_3,					// 군주_스페셜 PC방 마크(제조경험치)
	GIMG_SPECIAL_PCROOM_MARK_4,					// 군주_스페셜 PC방 마크(네잎클로버지급)

	//KHY - 0313 - 변신 100%효과 발생.
	GIMG_TRANSFORM_SYNCHRO100,					// 변신 100% 효과 

	//PCK : 용갱장 마법 추가 (08.03.23)
	GIMG_MAGIC_EMPERORMASTER,					// 용갱장 마법

	// [진성] 아티펙트 관련 이미지 추가.
	GIMG_ARTIFACT_SLOT_STATUS,
	GIMG_ARTIFACT_GAUGE_BACK,
	GIMG_ARTIFACT_GAUGE,
	GIMG_ARTIFACT_SLOT_EFFECT,
	GIMG_ARTIFACT_SLOT_REMOVE,

	// PCK : 괴수손 마법 추가 (08.04.11)
	GIMG_MAGIC_GREATWALLSOLDIER,
	GIMG_MAGIC_GREATWALLSOLDIERFX,

	// PCK : 머리만 마법 추가 (08.04.11)
	GIMG_MAGIC_GREATWALLMAGICIAN,

	// [진성] 화살표 들어왔다 나가는 이미지.
	GIMG_ARROW_IN_OUT,

	// PCK : 수라 마법 추가 (08.05.28)
	GIMG_MAGIC_GREATWALLSPIDER,

	// [진성] 룰렛 주사위 => 2008-7-8
	GIMG_MAGIC_DICE,

	// [영훈] NPC초상 작은이미지(40x40)
	GIMG_PORTRAIT_SMALL01,
	GIMG_PORTRAIT_SMALL02,

	GIMG_GACHASUMMON_EFFECT,

	GIMG_MARKETCONDITIONS_ICON,

	// PCK : 소환수용 프로즌골렘, 프로즌퀸 추가
	GIMG_MAGIC_SFROZENGOLEM,					
	GIMG_MAGIC_SFROZENQUEEN,	

	//[진성] 아이템 이미지 큰거. => 2008-9-23
	GIMG_ITEMIMAGEBIG01,
	
	GIMG_SETITEME0,								// 세트아이템1
	GIMG_SETITEME2,								// 세트아이템1
	GIMG_SETITEME3,								// 세트아이템1
	GIMG_SETITEME4,								// 세트아이템1
	GIMG_SETITEME5,								// 세트아이템1
	GIMG_SETITEME6,								// 세트아이템1
	GIMG_SETITEME7,								// 세트아이템1
	GIMG_SETITEMEFFECT0,						// 세트아이템1
	GIMG_SETITEMEFFECT2,						// 세트아이템
	GIMG_SETITEMEFFECT3,						// 세트아이템
	GIMG_SETITEMEFFECT4,						// 세트아이템
	GIMG_SETITEMEFFECT5,						// 세트아이템
	GIMG_SETITEMEFFECT6,						// 세트아이템
	GIMG_SETITEMEFFECT7,						// 세트아이템

	// [성웅] PVP전 카운트 다운,
	GIMG_PVP_COUNTDOWN,
	// [영훈] 버프 마법 이펙트
	GIMG_MAGIC_BLESSDAMAGE,
	GIMG_MAGIC_BLESSCIRICAL1,
	GIMG_MAGIC_BLESSCIRICAL2,
	GIMG_MAGIC_BLESSHITRATE,
	GIMG_MAGIC_BLESSRANGE,
	GIMG_MAGIC_BLESSATTACKINTERVAL,
	GIMG_MAGIC_BLESSMPRECOVERYTIME,
	GIMG_MAGIC_BLESSHP,

	GIMG_MAGIC_AURABLADE,
	// PVP 승리 패배 비김 표시
	GIMG_PVP_WLD,

	// 복불복 10 stage 클리어시 뜨는 이미지.
	GIMG_BBB_WL,

	GIMG_DAMAGE_PC_DODGE,
	GIMG_DAMAGE_NPC_DODGE,
	GIMG_PVP_LEAGUE,
	GIMG_VALENTINEWAR,

	GIMG_ELIXIR,
	GIMG_EMBLEM,		// 무기술의 달인 - 엠블렘
	GIMG_MAGIC_GREATWALLASURA, // 버려진요새 아수라 석상.

	GIMG_GUILDWAR_KILLSIGN,		//	사냥터 확정 길드전 킬표시
	GIMG_GUILDWAR_TIME,			//	[종호] 길드전 시간

	GIMG_WEDDING_EFT_S,			// [영훈] 웨딩 이펙트 작음
	GIMG_WEDDING_EFT_M,			// [영훈] 웨딩 이펙트 중간
	GIMG_WEDDING_EFT_L,			// [영훈] 웨딩 이펙트 큼

	GIMG_HEAL_Y,				// [영훈] 힐 이펙트 노란색

	GIMG_PCROOM_CHAR_HEAD,		// [영훈] 캐릭터 위에 나타낼 PC방 마크
	GIMG_HEAL_R,				// [춘기] 힐 이펙트 붉은색

	GIMG_GAWIBAWIBO,			// [종호] 가위바위보 이미지
	GIMG_ROUNDCOUNT,
	GIMG_STUN,					// [성웅]스턴 걸렸을때 
	GIMG_DRAGON_STUN,			// [진성] 소환수 드래곤 스턴.

	GIMG_HEAL_P,				// [춘기] 힐 이펙트 보라색

	GIMG_CURSE_ATTACK,			// [진성] 저주
	GIMG_CURSE_DEFENSE,	
	GIMG_CURSE_DODGE,	
	GIMG_CURSE_HIT,	
	GIMG_HEAL_B,

    GIMG_VALENTINEDAY_HEART,	// [진성] 벨렌타인 데이 하트.
/////////////////////////////////////////////////////////////////////
//	
// 이 위에 추가해 주세요 아래 선언은 건드리면 안됩니다
//
/////////////////////////////////////////////////////////////////////
	MAX_GLOBAL_IMG_FILE_NUMBER
};
	
class cltGlobalImgFile{
protected:
	TSpr*		pTSpr[MAX_GLOBAL_IMG_FILE_NUMBER];
	TCHAR*		pszFileName[MAX_GLOBAL_IMG_FILE_NUMBER];

public:

	cltGlobalImgFile();
	~cltGlobalImgFile();

	void Add(SI32 index, TCHAR* pfilename);
	TSpr* GetGlobalSpr( SI32 index );

	SI16 GetBufImageFontIndex(SI16 siBufType);
};
#endif


