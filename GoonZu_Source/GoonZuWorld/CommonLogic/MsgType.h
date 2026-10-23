//---------------------------------
// 2003/6/9 김태곤
//---------------------------------
#ifndef _MSGTYPE_H
#define _MSGTYPE_H

#include "../NetworkLib/ITZNetLib/ITZBase.h"
#include "../../CommonLogic/Market/Market.h"

// 패킷 데이터를 보낸때 사용하려고 ㅋ
// 예> cltMsg clMsg( GAMEMSG_REQUEST_SKILLBOOK_BUYITEM, sizeof(clSkillBookBuyItem), (BYTE*)&clSkillBookBuyItem );
//     -> cltMsg clMsg( GAMEMSG_REQUEST_SKILLBOOK_BUYITEM, PACKET(clSkillBookBuyItem) );
#define PACKET(DATA) sizeof(DATA), (BYTE*)&DATA
#define PACKET_EMPTY NULL, NULL

// 서버와 클라이언트간의 메시지. 
enum eGameNetworkMsg{

	//-------------------------------
	// 로그인 
	//-------------------------------
	GAMEMSG_RESPONSE_CLIENTCHARUNIQUE	= 1, //[태곤]
	GAMEMSG_REQUEST_LOGIN		,			//[태곤]
	GAMEMSG_RESPONSE_CHARLIST	,			//[태곤] 유저의 캐릭터 리스트를 클라이언트에게 보낸다. (캐릭터 마다 분리해서 보낸다. )

	GAMEMSG_REQUEST_SELECTCHAR	,			//[태곤] 캐릭터를 선택한다는 메시지. 
	GAMEMSG_RESPONSE_LOGIN		,			//[태곤]
	GAMEMSG_RESPONSE_LOGINMANY	,			//[태곤] 여러 캐릭터의 로그인 정보를 보낸다. 

	GAMEMSG_REQUEST_MAKENEWCHAR	,			//[태곤] 새로운 캐릭터를 만든다는 메시지. 
	GAMEMSG_RESPONSE_MAKENEWCHAR,			//[태곤] 새로운 캐릭터를 만들어서 클라이언트에 통보. 	

	GAMEMSG_REQUEST_DELUSERCHAR	,			//[제형] 케릭터 삭제.
	GAMEMSG_RESPONSE_DELUSERCHAR,			//[제형]

	GAMEMSG_RESPONSE_LOGOUT		,			//[태곤]

	GAMEMSG_REQUEST_APEXDATA = 77,			//[제완] APEX-
	GAMEMSG_RESPONSE_APEXDATA,				//[제완] APEX-

	GAMEMSG_REQUEST_HANAUTH_LOGIN = 80,		//[강형] NHN HANAUTH 로그인
	GAMEMSG_REQUEST_TAIWAN_LOGIN,			//[강형] TAIWAN 로그인

	GAMEMSG_REQUEST_HANAUTH_GAMERUNAUTH,	//[강형] NHN HANAUTH 로그인

	GAMEMSG_REQUEST_PREDELUSERCHAR	,		//[성웅] 케릭터 삭제유예메세지.
	GAMEMSG_RESPONSE_PREDELUSERCHAR,		//[성웅]

	GAMEMSG_REQUEST_PERSONSLOTNUMCHANGE,	//[영훈] 캐릭터 보관소에서 가져온다


	//--------------------------------
	// 시스템 
	//--------------------------------
	// 시스템 
	GAMEMSG_REQUEST_HEARTBEAT		= 100,	//100[태곤] 클라이언트가 살아 있다는 메시지. 

	GAMEMSG_RESPONSE_USERNUM,				//101[태곤] 현재/최대 사용자 숫자를 통보. 		

	GAMEMSG_REQUEST_USERCHARINFO,			//102[태곤] GM이 사용자 캐릭터에 대한 정보를 요청한다. 
	GAMEMSG_RESPONSE_USERCHARINFO,			//103[태곤]

	GAMEMSG_REQUEST_ACCOUNTINFO,			//104[제완] GM이 사용자 계정에 대한 정보를 요청한다.
	GAMEMSG_RESPONSE_ACCOUNTINFO,			//105[제완]

	GAMEMSG_RESPONSE_SERVERINFO,			//106[태곤] 서버의 일반적인 정보들을 통보  

	GAMEMSG_REQUEST_HOPETOQUIT,				//107[태곤] 종료를 희망함. 
	GAMEMSG_RESPONSE_HOPETOQUIT,			//108[준엽] 종료 희망에 대한 응답

	GAMEMSG_REQUEST_HOPETOFRONT,				//109[준엽] 프론트로 되돌아가기를 원한다.
	GAMEMSG_RESPONSE_HOPETOFRONT,				//110

	GAMEMSG_RESPONSE_SERVEREVENTINFO,			//111[태곤] 서버 이벤트 정보를 보낸다. 

	// 공지 
	GAMEMSG_RESPONSE_NOTICE		,				//112[태곤] 공지알림. 
	GAMEMSG_RESPONSE_NOTICELOGIN,				//113[태곤] 로그인시 공지. 
	GAMEMSG_RESPONSE_GIFTNT,					//114[상민]XBox, 자전거, 플스 등등 선물 주는거 알려줌.
	GAMEMSG_RESPONSE_ANOTHERAUTH,				//115[준엽] 다른 곳에서의 접속 알림

	// 쪽지. 
	GAMEMSG_RESPONSE_LETTER,					//116[태곤] 쪽지. 

	// 채팅 
	GAMEMSG_REQUEST_CHAT		,				//117[태곤]
	GAMEMSG_RESPONSE_CHAT		,				//118[태곤]

	GAMEMSG_REQUEST_SECRETCHAT,					//119[태곤] 귓말 요청. 

	GAMEMSG_RESPONSE_SECRETCHATRESULT,			//120[태곤] 귓말 성공 여부. 

	GAMEMSG_REQUEST_HEADCHAT,					//121[태곤] 머릿말 요청. 
	GAMEMSG_RESPONSE_HEADCHAT,					//122[태곤] 머릿말 통보.

	GAMEMSG_REQUEST_MAKESECRETCHATCHANNEL,		//123[준엽] 상대 PersonID로 귓말 채널을 만든다. 
	GAMEMSG_RESPONSE_MAKESECRETCHATCHANNEL,		//124[준엽]
	GAMEMSG_REQUEST_ACCEPTSECRETCHATCHANNEL,	//125[영진] 귓속말 허락
	GAMEMSG_RESPONSE_ACCEPTSECRETCHATCHANNEL,	//126

	GAMEMSG_RESPONSE_ENEMYCAPTURE,				//127[태곤] 적의 점령 상태를 통보한다.(마을) 
	GAMEMSG_RESPONSE_ENEMYCAPTUREALL,			//128[태곤] 적의 모든 점령 상태를 통보한다. 

	GAMEMSG_RESPONSE_WINWARPRIZE,				//129[준엽] 전쟁 승리시 얻은 돈과 경험치를 통보한다.

	GAMEMSG_REQUEST_WARJOIN,					//130[광명] 30이하 유저들은 짐꾼왜구 마을로 보내준다

	// 질문 
	GAMEMSG_REQUEST_ASKLIST,					//131[태곤] 질문 목록을 요청한다.  
	GAMEMSG_RESPONSE_ASKLIST,					//132[태곤]
	GAMEMSG_REQUEST_DELASK,						//133[태곤] 특정 사용자의 질문을 삭제하도록 요청한다. 


	// 장인 정보. 
	GAMEMSG_REQUEST_SKILLMASTERLIST,			//134[태곤] 장인리스트 요청. 	
	GAMEMSG_RESPONSE_SKILLMASTERLIST,			//135[태곤] 장인리스트 통보. 

	GAMEMSG_REQUEST_REGISTMASTER,				//136[태곤] 장인등록/해지 요청.

	// 명품. 
	GAMEMSG_RESPONSE_RAREITEMMADE,				//137[태곤] 명품이 만들어졌음을 통보. 
	// 대박 전리품	
	GAMEMSG_RESPONSE_BIGWARITEM,				//138[태곤] 대박 전리품 확득 통보. 
	// 흑마, 백마, 적토마 생김.
	GAMEMSG_RESPONSE_GOODHORSE,					//139[태곤] 명마 생김 통보. 

	GAMEMSG_RESPONSE_NOADDHORSE,				//140[희영] 말 추가 실패. - 0 = 다 찾음. 

	// 높은 단계 소환수 출연 
	GAMEMSG_RESPONSE_HIGHSUMMON,				//141[제형] 높은 Rank의 소환수 생김 통보. 
	// 추천인 포상 공지 
	GAMEMSG_RESPONSE_RECOMMENDREWARD,			//142[태곤] 추천인 포상을 받았음을 전체 공지. 
	// 명성레벨 포상 공지 
	GAMEMSG_RESPONSE_FAMELEVELREWARD,			//143[태곤] 명성레벨 향상에 따른 포상을 받았음을 전체 공지. 


	// 공성전 
	GAMEMSG_RESPONSE_WARVILLAGE,				//144[제형] 공성전의 시작과 종료를 알림

	// 보너스 타임. 
	GAMEMSG_RESPONSE_BONUSTIME,					//145[태곤] 보너스 타임 시작과 종료를 알림

	// 공간이동
	GAMEMSG_REQUEST_WARPVILLAGE,				//146[태곤] 해당 마을로 Warp를 희망함 
	GAMEMSG_REQUEST_RESIDENTWARPVILLAGE,		//147[상민] 주민이 자기 마을로 Warp 를 희망함.


	// 환경 설정 ( 귓말 거부, 거래 거부 )
	GAMEMSG_REQUEST_WHISPERCHAT_REJECT,			//148[상민] 모든 귓말 거부
	GAMEMSG_RESPONSE_WHISPERCHAT_REJECT,		//149[상민]

	GAMEMSG_REQUEST_PRIVATETRADE_REJECT,		//150[상민] 모든 거래 거부
	GAMEMSG_RESPONSE_PRIVATETRADE_REJECT,		//151[상민]

	GAMEMSG_REQUEST_PARTYINVITE_REJECT,			//152[상민] 파티 초대 거부
	GAMEMSG_RESPONSE_PARTYINVITE_REJECT,		//153[상민]

	GAMEMSG_REQUEST_FRIENDADD_REJECT,			//154[상민] 친구 추가 거부
	GAMEMSG_RESPONSE_FRIENDADD_REJECT,			//155[상민]

	// 추천인. 
	GAMEMSG_RESPONSE_RECOMMENDPERSONAVAILABLE,	//156[태곤] 유저 추천 가능

	GAMEMSG_REQUEST_RECOMMENDPERSON,			//157[상민] 유저 추천
	GAMEMSG_RESPONSE_RECOMMENDPERSON,			//158[상민]

	// 보스 
	GAMEMSG_RESPONSE_BOSSAPPEAR,				//159[태곤] 보스 등장 정보 알림. 

	GAMEMSG_REQUEST_CAPTIONKINDCHANGE,			//160[상민] 밑에 흘러가는 자막 종류 변경

	// 불가사리 사냥 이벤트. 
	GAMEMSG_RESPONSE_HUNTINGINFO,				//161[태곤] 불가사리 사냥 정보. 
	GAMEMSG_RESPONSE_HUNTINGREWARD,				//162[태곤] 불가사리 사냥 성공시 보상. 

	// 고구려 유적 방어 이벤트. 
	GAMEMSG_RESPONSE_GOGUINFO,					//163[태곤] 고구려 유적 방어 정보. 
	GAMEMSG_RESPONSE_GOGUREWARD,				//164[태곤] 고구려 유적 방어  성공시 보상. 

	// 오사카성 이벤트. 
	GAMEMSG_RESPONSE_OSAKAINFO,					//165[광명] 오사카성 공격 정보. 
	GAMEMSG_RESPONSE_OSAKAREWARD,				//166[광명] 오사카성 성공시 보상. 

	// 강화도 이벤트. 
	GAMEMSG_RESPONSE_KANGHWADOINFO,				//167[광명] 강화도 방어 정보. 
	GAMEMSG_RESPONSE_KANGHWADOREWARD,			//168[광명] 강화도 방어 성공시 보상. 

	// 감옥 
	//GAMEMSG_RESPONSE_PRISONINFO, 

	// 자동 사냥
	GAMEMSG_REQUEST_AUTOHUNTING,				//169[준엽] 자동 사냥 신고
	GAMEMSG_RESPONSE_AUTOHUNTING,				//170

	GAMEMSG_REQUEST_AUTOHUNTING_ASK,			//171[준엽] 자동 사냥 답변하기
	GAMEMSG_RESPONSE_AUTOHUNTING_ASK,			//172

	GAMEMSG_RESPONSE_AUTOHUNTING_TIMEUPDATE,	//173[준엽] 답변 제한 시간을 흘러가는 데로 알려준다.

	GAMEMSG_RESPONSE_PERSONALSHOPMAKEINFO,		//174[태곤]개인 상점 제조의뢰 내역 클라이언트로 방송. 

	GAMEMSG_RESPONSE_INSTANCEMAPINFO,			//175[준엽] 인스턴스 던전의 정보(남은 몹수,최대 몹수, 남은 시간)

	GAMEMSG_RESPONSE_ACCOUNTLETTER,				//176

	GAMEMSG_RESPONSE_SERVERTIME,				//177[연준] 서버의 시간 정보(분단위)

	GAMEMSG_RESPONSE_USERCHARITEMINFO,			//178[영훈] 사용자 아이템 정보 요청 답변
	GAMEMSG_RESPONSE_USERCHARITEMINFO2,			//179[영훈] 용량때문에 두개로 나눔

	GAMEMSG_REQUEST_GET_HOMEPAGE_URL,			//180[희영] 홈페이지 주소를 가져온다.
	GAMEMSG_RESPONSE_GET_HOMEPAGE_URL,			//181

	GAMEMSG_REQUEST_SET_HOMEPAGE_URL,			//182[희영] 홈페이지 주소를 셋팅한다.
	GAMEMSG_RESPONSE_SET_HOMEPAGE_URL,			//183

	//------------------------------------
	// Person
	//------------------------------------
	GAMEMSG_RESPONSE_CHARUPDATE	= 200,		//[태곤] 정기적인 기본 정보 업데이트. 

	//--------------------------------
	//  - 절대 메시지 추가하지 마시오...
	// 스승
	GAMEMSG_REQUEST_SETFATHER,				//[태곤] 스승 지정. 
	GAMEMSG_RESPONSE_SETFATHER,				//[태곤]

	GAMEMSG_RESPONSE_CHILDINFO,				//[태곤] 스승에 대한 정보를 보낸다.

	GAMEMSG_REQUEST_APPLYFATHER,			//[태곤] 스승 등록 신청한다.

	GAMEMSG_REQUEST_FATHERAPPLYERLIST,		//[태곤]
	GAMEMSG_RESPONSE_FATHERAPPLYERLIST,		//[태곤] 스승 등록 신정차 명단을 통보한다. 

	GAMEMSG_REQUEST_UPDATECHILDLIST,		//[태곤] 제자 명단 업데이트 요청. 
	GAMEMSG_RESPONSE_UPDATECHILDLIST,		//[태곤]

	GAMEMSG_REQUEST_RENOUNCEPUPIL,			//[광명] 스승이 제자를 짜른다. 
	GAMEMSG_RESPONSE_RENOUNCEPUPIL,			//[광명]

	GAMEMSG_RESPONSE_OVERTAKETEACHERLEVEL,	//[광명] 제자가 스승레벨을 앞지른다(청출어람)

	GAMEMSG_RESPONSE_REQUEST_SETFATHER,		//[광명] 스승 요청을 스승에게 물어보게 한다.

	GAMEMSG_RESPONSE_PVPRESULT,				//[광명] PVP결과를 알려준다.
	//---------------------------------

	//  - 절대 메시지 추가하지 마시오...
	//----------------------------------
	// 캐릭터의 특정 정보 
	GAMEMSG_REQUEST_PERSON_IP,				//[태곤] ImportantParameta 정보를 보내줄 것을 요청한다. 
	GAMEMSG_RESPONSE_PERSON_IP,				//[태곤] ImportantParameta 정보를 클라이언트로 보낸다. 
	GAMEMSG_RESPONSE_PERSON_HEALTH,			//[태곤] 건강 정보를 클라이언트로 보낸다. 		
	GAMEMSG_RESPONSE_PERSON_SKILL,			//[태곤] SkillData를 클라이안트로 보낸다. 
	GAMEMSG_RESPONSE_PERSON_ONESKILL,		//[태곤] 사용자의 한가지 기술 정보를 클라이안트로 보낸다. 
	GAMEMSG_RESPONSE_PERSON_ALLITEM,		//[태곤] 소지한 모든 아이템 정보를 보낸다. 
	GAMEMSG_RESPONSE_PERSON_BANK,			//[태곤] 경제 정보를 보낸다. (전장 )
	GAMEMSG_RESPONSE_PERSON_STOCK,			//[태곤] 경제 정보를 보낸다. (증권-기본)
	GAMEMSG_RESPONSE_PERSON_STOCKORDER,		//[태곤] 경제 정보를 보낸다. (증권-주문내역)
	GAMEMSG_RESPONSE_PERSON_MARKET,			//[태곤] 객주 정보.  
	GAMEMSG_RESPONSE_PERSON_MARKETORDER,	//[태곤] 객주 주문 정보를 통보한다. 
	GAMEMSG_RESPONSE_PERSON_QUEST,			//[태곤] Person의 퀘스트 정보를 통보한다. 
	GAMEMSG_RESPONSE_PERSON_HORSE,			//[태곤] Person의 말정보를 통보한다. 
	GAMEMSG_RESPONSE_PERSON_GUILTY,			//[태곤] Person의 죄 정보를 통보한다. 
	GAMEMSG_RESPONSE_PERSON_INNERSTATUS,	//[태곤] Person의 InnerStatus전체를 통보한다. 
	GAMEMSG_RESPONSE_PERSON_WORLDMONEY,			//[준엽] 아큐 정보를 보낸다.

	GAMEMSG_REQUEST_CHANGESTATUSINFO,		//[태곤] 캐릭터의 상태를 변경할 것을 요청한다. 
	GAMEMSG_RESPONSE_CHANGESTATUSINFO,		//[제형] 캐릭터의 상태가 변경되었음을 알린다.

	GAMEMSG_REQUEST_SETGREETING,		//[광명] 캐릭터의 인사말 변경
	GAMEMSG_RESPONSE_SETGREETING,

	GAMEMSG_REQUEST_GETGREETING,		//[광명] 캐릭터의 인사말 얻어오기
	GAMEMSG_RESPONSE_GETGREETING,

	//-----------------------------------
	//  - 절대 메시지 추가하지 마시오...
	//-----------------------------------
	// 학교 
	GAMEMSG_REQUEST_SETSCHOOL,				//[태곤] 학교등록. 

	GAMEMSG_RESPONSE_SCHOOL,				//[태곤] 학교 설정 통보. 

	GAMEMSG_REQUEST_SCHOOLUSERNUM,			//[태곤] 접속중인 학교 등록자 수. 
	GAMEMSG_RESPONSE_SCHOOLUSERNUM,	
	//-------------------------------------

	GAMEMSG_REQUEST_USERORDER	,			//[태곤] 캐릭터 명령 요청. 
	GAMEMSG_RESPONSE_USERORDER	,			//[태곤]

	GAMEMSG_RESPONSE_DEALTHPENALTYINFO,		//[태곤] 사망 페널티 정보. 

	GAMEMSG_REQUEST_NAME		,			//[태곤] 이름 정보 요청. 
	GAMEMSG_RESPONSE_NAME		,			//[태곤]

	GAMEMSG_RESPONSE_USERLEVELUP,			//[태곤] 사용자 레벨업 통보. 

	GAMEMSG_REQUEST_ALLCHARINFO,			//[태곤] 사용자 캐릭터가 캐릭터의 모든 정보를 요청한다. 
	GAMEMSG_RESPONSE_ALLCHARINFO,			//[태곤]
	GAMEMSG_RESPONSE_ALLCHARINFOONE,		//[광명] 캐릭터 정보1
	GAMEMSG_RESPONSE_ALLCHARINFOTWO,		//[광명] 캐릭터 정보2


	GAMEMSG_REQUEST_USEBONUS,				//[태곤] 보너스를 사용하겠다는 메시지를 보낸다.

	GAMEMSG_REQUEST_DECREASESKILLBONUS,		//[태곤] 스킬보너스를 복구한다. 

	GAMEMSG_RESPONSE_INCLIFE,				//[태곤] 생명력 증가. 
	GAMEMSG_RESPONSE_INCMANA,				//[태곤] 

	GAMEMSG_REPONSE_INCHUNGRY,				//[태곤]  음식을 먹어서 배가 불러짐을 클라이언트에게 알린다. 

	GAMEMSG_REQUEST_HORSEFREE,				//[태곤] 말을 풀어준다. 
	GAMEMSG_RESPONSE_HORSEFREE,				//[태곤]

	//KHY - 1001 - 기승동물 5마리 보유 수정.
	GAMEMSG_REQUEST_CHANGEHORSE,				//[희영]  말을 바꾼다.
	GAMEMSG_RESPONSE_CHANGEHORSE,				//[희영]

	GAMEMSG_RESPONSE_CHANGEMONEY,			//[태곤] 돈이 변경되었따.

	GAMEMSG_REQUEST_USERID,					//[태곤] 캐릭터 명이 아닌, 유저 아이디를 요청함 ( 미니 홈페이지 사용하려고.. )
	GAMEMSG_RESPONSE_USERID,				//[태곤] 응답

	GAMEMSG_RESPONSE_WARKILL,				//[태곤] 전과 통보. 

	GAMEMSG_RESPONSE_JIM,					//[태곤] 찜정보. 

	GAMEMSG_RESPONSE_HOMEVILLAGE,			//[태곤] 주소지 설정 통보. 


	GAMEMSG_REQUEST_PLAYTIME,				//[태곤] 플레이 시간을 요청한다.
	GAMEMSG_RESPONSE_PLAYTIME,				//[태곤]

	GAMEMSG_REQUEST_HIT,					//[광명] 타격 판정을 요청한다.
	GAMEMSG_RESPONSE_HIT,					//[태곤] 맞았음을 보낸다. 

	GAMEMSG_REQUEST_SETBULLET,				//[광명] 원거리 공격을 설정을 서버로 요청한다.

	GAMEMSG_RESPONSE_GETEXP,				//[태곤] 경험치를 획득했음을 통보. 

	GAMEMSG_RESPONSE_GENERALEFT,			//[태곤] 지휘효과 통보. 

	GAMEMSG_REQUEST_PERSONPRIVATEINFO,		//[태곤] 개인 정보 요청.
	GAMEMSG_REQUEST_PERSONPRIVATEINFOBYPERSONID, //[상민] 개인 정보 요청( PersonID 로 )
	GAMEMSG_RESPONSE_PERSONPRIVATEINFO,		//[태곤]

	GAMEMSG_REQUEST_ADDINFO,				//[제형] 특수캐릭터인 경우 추가정보를 요청한다.
	GAMEMSG_RESPONSE_ADDINFO,				//[제형]

	GAMEMSG_RESPONSE_CHANGESUMMONSTATUS,	//[태곤]내가 가진 소환수의 정보가 변경되었다. 

	GAMEMSG_REQUEST_CHANGEMAGIC,			//[제형] 캐릭터의 현재 마법을 바꾸었다.
	GAMEMSG_RESPONSE_CHANGEMAGIC,			//[제형]

	GAMEMSG_RESPONSE_EVENTINFO,				//[태곤] 이벤트 정보를 보낸다. 

	GAMEMSG_RESPONSE_WARPEFFECT,			//[태곤] 유료 아이템으로 워프한 경우 이를 클라이언트에게 알린다. 

	// 퀘스트 
	GAMEMSG_RESPONSE_QUESTUSERINFO,			//[태곤]특정 사용자의 특정 퀘스트 정보를 통보한다. 

	GAMEMSG_RESPONSE_SPECIALQUESTMADE,		//[태곤]특수 임무가 설정되었으을 알린다. 

	GAMEMSG_RESPONSE_SPECIALQUESTREWARD,	//[태곤]특수 임무 완료후에 포상 내용을 알려준다. 

	GAMEMSG_RESPONSE_QUESTEFFECT,		//[영진]퀘스트 수락, 완료 이펙트를 알려줌.

	GAMEMSG_RESPONSE_CLEAR_SPECIALWORD,		//[준엽]단어 조합 완성 수를 통보함

	GAMEMSG_REQUEST_PRIZE_GAMEEVENT,		//[준엽] 이벤트 당첨 여부를 확인한다.
	GAMEMSG_RESPONSE_PRIZE_GAMEEVENT,

	GAMEMSG_REQUEST_SETSCHOOLEVENT,			//[광명] 학교 이벤트 신청
	GAMEMSG_RESPONSE_SETSCHOOLEVENT,

	//GAMEMSG_REQUEST_COUNTATTACK_INFO,		//[준엽] 조선의 반격 이벤트
	//GAMEMSG_RESPONSE_COUNTATTACK_INFO,

	GAMEMSG_REQUEST_EVENTNPC,				//[광명] 이벤트 참여(지금은 천군)
	GAMEMSG_RESPONSE_EVENTNPC,

	// WeaponSkill
	GAMEMSG_REQUEST_GET_WEAPONSKILLCOST,	//[준엽] 전투향상술 익히는데 드는 비용을 가져온다.
	GAMEMSG_RESPONSE_GET_WEAPONSKILLCOST,

	GAMEMSG_REQUEST_SET_WEAPONSKILL,		//[준엽] 전투향상술스킬을 올린다.
	GAMEMSG_RESPONSE_SET_WEAPONSKILL,

	GAMEMSG_REQUEST_RIGHTMOUSE_USERLIST_NAME,	// [영진] 오른쪽 마우스로 겹쳐진 여러명의 유저들의 목록을 띄울때 필요한 이름을 얻어주는 메시지
	GAMEMSG_RESPONSE_RIGHTMOUSE_USERLIST_NAME,

	// !!!!!!!!!! WARNRING !!!!!!!!!!!  299까지 모두다 찾습니다.   !!!!!!!!!! WARNRING !!!!!!!!!!!
	// !!!!!!!!!! WARNRING !!!!!!!!!!!  더 이상 추가하지 마십시요. !!!!!!!!!! WARNRING !!!!!!!!!!!
	GAMEMSG_RESPONSE_TIRED_STATUS,
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//---------------------------
	// 아이템 관련 메시지 
	//---------------------------
	GAMEMSG_REQUEST_MAKEITEMINV	= 300,				//300[태곤] 인번토리에서 아이템을 만든다 

	GAMEMSG_REQUEST_MAKEITEMSWITCH,					//301[광명] 제조시 제조이미지 셋팅
	GAMEMSG_RESPONSE_MAKEITEMSWITCH,				//302[광명] 제조시 제조이미지 셋팅

	GAMEMSG_REQUEST_ITEMMOVE,						//303[태곤] 아이템을 캐릭터 내에서 이동시킨다.

	GAMEMSG_REQUEST_ENCHANTITEMINV,					//304[태양] 인챈트창에서 아아템을 인챈트한다.

	GAMEMSG_REQUEST_ITEMCHANGE,						//305[태양] 아이템을 캐릭터 내에서 바꿔준다. 

	GAMEMSG_REQUEST_USEITEM,						//306[태곤] 아이템을 사용할 것을 요청한다. 

	GAMEMSG_REQUEST_INVDROP,						//307[태곤] 사용자가 갖고 있는 아이템을 드랍하도록 요청한다. 

	GAMEMSG_REQUEST_INVPICKUP,						//308[태곤] 땅에 떨어진 아이템을 줍도록 요청한다. 

	GAMEMSG_RESPONSE_ITEMDROP,						//309[태곤] 땅에 떨어진 아이템의 정보. 

	GAMEMSG_REQUEST_ENCHANT,						//310[태곤] 부적을 장비에 부착한다. 

	GAMEMSG_RESPONSE_MAKEITEMDELAY,					//311[태곤] 물품 생산 지연 시간을 통보한다. 

	GAMEMSG_REQUEST_NPCREPAIRPRICE,					//312[태곤] NPC수리가격 요청. 
	GAMEMSG_RESPONSE_NPCREPAIRPRICE,				//313[태곤]

	GAMEMSG_REQUSET_NPCREPAIR,						//314[태곤] NPC수리 요청. 

	GAMEMSG_REQUEST_EXCHANGEWEAPON,					//315[태곤] 무기 1, 2번의 active 상황을 바꿔준다.
	GAMEMSG_RESPONSE_EXCHANGEWEAPON,				//316[태곤]	

	GAMEMSG_REQUEST_EXCHANGECLOTHES,				//317[태양] 의복 1, 2번의 active 상황을 바꿔준다.
	GAMEMSG_RESPONSE_EXCHANGECLOTHES,				//318[태양]	

	GAMEMSG_REQUEST_UPGRADEITEM,					//319[상민] 아이템 업그레이드 시도 ( 일반->호품->명품->신기 ) - 혹부리 영감 이용권을 사용해서.
	GAMEMSG_RESPONSE_UPGRADEITEM,					//320[상민]

	GAMEMSG_REQUEST_PERFORMANCEUPGRADEITEM,			//321[상민] 아이템 업그레이드 시도 ( 일반->호품->명품->신기 ) - 혹부리 영감 이용권을 사용해서.
	GAMEMSG_RESPONSE_PERFORMANCEUPGRADEITEM,		//322[상민]

	GAMEMSG_RESPONSE_MAKEITEMDLG,					//323[준엽] 아이템 제조후 결과창 뛰우기

	GAMEMSG_RESPONSE_ENCHANTITEMDLG,				//324[태양] 아이템 인챈트후 결과창 뛰우기

	GAMEMSG_RESPONSE_ISSKILLITEM,					//325[준엽] 미니게임을 돌릴 수있나 없나... 즉 스킬로 만드는 아이템이냐 만들어도 되냐

	//	GAMEMSG_REQUEST_MONSTERCORP_START,				//[광명] 미니게임을 시작한다.
	GAMEMSG_REQUEST_MONSTERCORP_SCORE,				//326[광명] 몬스터주식회사 점수를 보낸다.
	GAMEMSG_REQUEST_MONSTERCORP_COMBOINFO,			//327[광명] 몬스터주식회사 점수를 서버에서도 계산할 수 있게 콤보 정보를 보낸다.

	GAMEMSG_REQUEST_ROULETTEGAME_INFO,				//328[광명] 박대박 베팅정보를 보낸다
	GAMEMSG_RESPONSE_ROULETTEGAME_RESULT,			//329[광명] 박대박 결과
	GAMEMSG_REQUEST_ROULETTEGAME_STOP,				//330[광명] 박대박 중지

	GAMEMSG_RESPONSE_PREMIUM_INFO,					//331[준엽] 프리미엄 기간 정보를 통보한다.
	GAMEMSG_REQUEST_MAKEITEMINVINDEX,				//332[태양] ->[영진] 아이템 제조

	GAMEMSG_REQUEST_ENCHANTITEMINVINDEX,			//333[태양] 제조창 인벤토리의 인덱스를 저장
	GAMEMSG_RESPONSE_ENCHANTITEMSTARTOREND,			//334[태양] 제조창 인벤토리의 인덱스를 저장
	GAMEMSG_REQUEST_ENCHANTITEMSTARTOREND,			//335[태양] 제조창 인벤토리의 인덱스를 저장

	GAMEMSG_REQUEST_ENCHANTOTHERSHOW,				//336[태양]
	GAMEMSG_RESPONSE_ENCHANTOTHERSHOW,				//337[태양]

	GAMEMSG_REQUEST_ENCHANTOTHERPRICE,				//338[태양]
	GAMEMSG_RESPONSE_ENCHANTOTHERPRICE,				//339[태양]

	GAMEMSG_REQUEST_ENCHANTITEMSWITCH,				//340
	GAMEMSG_REQUEST_ITEMSPLIT,						//341

	GAMEMSG_REQUEST_ENCHANT_USEDAYPLUS,				//342[영진] 아이템 기간 연장
	GAMEMSG_RESPONSE_ENCHANT_USEDAYPLUS,			//343[영진]

	GAMEMSG_REQUEST_CHANGEENCHANT,					//344[영훈] 아이템 인첸트 속성 변경 요청 - 2008.02.20

	GAMEMSG_REQUEST_SUMMONPUSHITEM,					//345[영훈] 사람에서 소환수로 아이템 이동
	GAMEMSG_REQUEST_SUMMONPOPITEM,					//346[영훈] 소환수에서 사람에게로 아이템 이동

	GAMEMSG_RESPONSE_SUMMONCHANGEITEM,				//347[영훈] 소환수 아이템이 변경되었다 (Push/Pop) 동시처리


	//******************************************************************************************
	//[추가 : 황진성 2008. 1. 29 =>  공간 상자 명령 메시지.]
	GAMEMSG_REQUEST_SPACE_BOX_LIST,					//348 공간 상자 리스트 요청.
	GAMEMSG_RESPONSE_SPACE_BOX_LIST,				//349

	GAMEMSG_REQUEST_TRESURE_BOX_LIST,				//350 보물 상자 리스트 요청.
	GAMEMSG_RESPONSE_TRESURE_BOX_LIST,				//351

	GAMEMSG_REQUEST_KEEP_SPACE_BOX_MOVE_ITEM,		//352 인벤과 공간 상자의 이동.
	GAMEMSG_RESPONSE_KEEP_SPACE_BOX_MOVE_ITEM,		//353

	GAMEMSG_REQUEST_RECOVERY_SPACE_BOX_MOVE_ITEM,	//354 인벤과 공간 상자의 이동.
	GAMEMSG_RESPONSE_RECOVERY_SPACE_BOX_MOVE_ITEM,	//355

	GAMEMSG_RESPONSE_USE_SPACEBOX_ITEM,				//356[영훈] 공간상자 아이템을 사용하였다
	GAMEMSG_RESPONSE_USE_TREASURE_ITEM,				//357[영훈] 가챠상자 아이템을 사용하였다

	//******************************************************************************************

	GAMEMSG_REQUEST_OPENBIGITEMBOX,					// [강형] : 대박 상자 아이템 개봉

	// 6조 판서 
	GAMEMSG_REQUEST_GETBYUNGZOINFO = 400,			//[준엽] 병조 판서의 정보를 요청한다.
	GAMEMSG_RESPONSE_GETBYUNGZOINFO,				//[준엽]

	GAMEMSG_REQUEST_GETGONGZOINFO,					//[준엽]
	GAMEMSG_RESPONSE_GETGONGZOINFO,					//[준엽]

	GAMEMSG_REQUEST_GETHYUNGZOINFO,					//[준엽]
	GAMEMSG_RESPONSE_GETHYUNGZOINFO,				//[준엽]

	GAMEMSG_REQUEST_GETHOZOINFO,					//[준엽]
	GAMEMSG_RESPONSE_GETHOZOINFO,					//[준엽]

	GAMEMSG_REQUEST_GETIZOINFO,						//[준엽]
	GAMEMSG_RESPONSE_GETIZOINFO,					//[준엽]

	GAMEMSG_REQUEST_GETYEZOINFO,					//[준엽] 예조 판서의 정보를 요청한다.
	GAMEMSG_RESPONSE_GETYEZOINFO,					//[준엽]

	GAMEMSG_REQUEST_GETGOONZUINFO,					//[준엽] 군주의 정보를 요청한다.
	GAMEMSG_RESPONSE_GETGOONZUINFO,					//[준엽]

	GAMEMSG_REQUEST_SETBYUNGZOINFO,					//[준엽] 병조 판서의 정보를 설정한다.

	GAMEMSG_REQUEST_SETBYUNGZOINFO_VILLAGEWARINFO,	//[영진] 병조 판서의 공성전 시간 정보를 설정한다.

	GAMEMSG_REQUEST_SETGONGZOINFO,					//[준엽]

	GAMEMSG_REQUEST_SETHYUNGZOINFO,					//[준엽]

	GAMEMSG_REQUEST_SETHYUNGZOPRISON,				//[준엽] 형조판서가 죄수를 설정한다 

	GAMEMSG_REQUEST_SETHOZOINFO,					//[준엽]

	GAMEMSG_REQUEST_SETIZOINFO,						//[준엽]

	GAMEMSG_REQUEST_SETYEZOINFO,					//[준엽] 예조 판서의 정보를 설정한다.

	GAMEMSG_REQUEST_SETGOONZUINFO,					//[준엽] 군주의 정보를 설정한다.

	GAMEMSG_REQUEST_GOONZU_SETMINISTER,				//[준엽] 군주가 판서를 임명한다.
	GAMEMSG_RESPONSE_GOONZU_SETMINISTER,

	GAMEMSG_REQUEST_GETANGELAPPLYERLIST,			//[준엽] 수호천사 신청자 목록을 요청한다.
	GAMEMSG_RESPONSE_GETANGELAPPLYERLIST,

	GAMEMSG_REQUEST_GETANGELLIST,					//[준엽] 수호천사 목록을 요청한다.
	GAMEMSG_RESPONSE_GETANGELLIST,

	GAMEMSG_REQUEST_SETANGELAPPLYER,				//[준엽] 수호천사 등록 신청을 요청한다.
	GAMEMSG_RESPONSE_SETANGELAPPLYER,

	GAMEMSG_REQUEST_SETANGEL,						//[준엽] 수호천사 임명을 요청한다.
	GAMEMSG_RESPONSE_SETANGEL,

	GAMEMSG_REQUEST_GETCHANGEDNAME,					//[준엽] 이름 변경한 기록을 요청한다.
	GAMEMSG_RESPONSE_GETCHANGEDNAME,

	GAMEMSG_REQUEST_GETPOLICELIST,					//[광명] 포교 리스트를 요청한다
	GAMEMSG_RESPONSE_GETPOLICELIST,

	GAMEMSG_REQUEST_SETPOLICE,						//[광명] 포교를 임명을 요청한다.
	GAMEMSG_RESPONSE_SETPOLICE,

	GAMEMSG_REQUEST_GETCRIMINALLIST,				//[광명] 현행범 목록을 요청한다.
	GAMEMSG_RESPONSE_GETCRIMINALLIST,

	GAMEMSG_REQUEST_SETCRIMINAL,					//[광명] 현행범 등록을 요청
	GAMEMSG_RESPONSE_SETCRIMINAL,

	GAMEMSG_REQUEST_GOONZU_SETLIQUIDATION,			//[광명] 군주가 파산 마을을 설정한다.
	GAMEMSG_RESPONSE_GOONZU_SETLIQUIDATION,

	GAMEMSG_REQUEST_GOONZU_GETBONUSTIME,			//[영진] 군주가 설정한 보너스 타임을 가져온다.
	GAMEMSG_RESPONSE_GOONZU_GETBONUSTIME,	

	GAMEMSG_REQUEST_GOONZU_SETBONUSTIME,			//[영진] 군주가 보너스 타임을 설정한다.
	GAMEMSG_RESPONSE_GOONZU_SETBONUSTIME,	


	//--------------------------------
	// 상단
	//--------------------------------
	GAMEMSG_REQUEST_BECHIEF = 1100,					//[태곤] Chief이 되겠다는 요청. 

	GAMEMSG_REQUEST_VILLAGESTRUCT,					//[태곤] 마을의 건물 정보를 요청한다. 
	GAMEMSG_RESPONSE_VILLAGESTRUCT,					//[태곤]

	GAMEMSG_RESPONSE_RANK,							//[태곤] 신분을 통보 

	GAMEMSG_REQUEST_CANDIDATE,						//[태곤] 후보자등록 요청. 

	GAMEMSG_REQUEST_CANCELCANDIDATE,				//[태곤] 후보자등록 취소 요청. 

	GAMEMSG_REQUEST_CANDIDATELIST,					//[태곤] 후보자 명단 요청. 
	GAMEMSG_RESPONSE_CANDIDATELIST,					//[태곤] 대행수 후보자 명단 통보 . 

	GAMEMSG_RESPONSE_SERVERRTN,						//[태곤] 서버의 리턴값  통보

	GAMEMSG_RESPONSE_CHARSERVERRTN,					//[희영][KHY] - 0910 - 클라이언트로 캐릭터형태의 리턴값을 보낸다.
	GAMEMSG_REQUEST_CHARSERVERRTN,					//클라이언트가 서버로 웹에서 읽은 정보를 보낸다. 

	GAMEMSG_REQUEST_DELCANDIDATE,					//[태곤] 후보자 목록 삭제 요청. 

	GAMEMSG_REQUEST_RESIGN,							//[태곤] Chief에서 사임 요청 

	GAMEMSG_REQUEST_INVESTLIST,						//[태곤] 마을의 투자자 리스트를 요청한다. 
	GAMEMSG_RESPONSE_INVESTLIST,					//[태곤] 마을의 투자자 리스트를 통보한다. 

	GAMEMSG_REQUEST_SETSYMBOLSTATUS,				//[제완] 대행수가 비용을 지불하고 심볼(성황신)의 수치를 조정한다 
	GAMEMSG_RESPONSE_SETSYMBOLSTATUS,				//[제완]

	GAMEMSG_REQUEST_SETCASTLESTATUS,		//[제완] 대행수가 비용을 지불하고 성문의 수치를 조정한다 
	GAMEMSG_RESPONSE_SETCASTLESTATUS,		//[제완]

	GAMEMSG_REQUEST_PAYDIVIDEND,			//[제완] 대행수가 주주들에게 주식배당을 실시한다 
	GAMEMSG_RESPONSE_PAYDIVIDEND,			//[제완]

	GAMEMSG_REQUEST_HIRENPC,				//[제완] 대행수가 마을NPC를 고용한다 
	GAMEMSG_RESPONSE_HIRENPC,				//[제완]

	GAMEMSG_REQUEST_GETNPCLIST,				//[제완] 마을NPC 리스트를 요청한다 
	GAMEMSG_RESPONSE_GETNPCLIST,			//[제완]

	GAMEMSG_REQUEST_APPLYRESIDENT,			//[제완] 마을 주민등록신청을 한다 
	GAMEMSG_RESPONSE_APPLAYRESIDENT,		//[제완]	

	GAMEMSG_REQUEST_APPLYRESIDENT2,			// [진성] 스승, 제자. 누군가의 초대로 인한 주민등록 신청.

	GAMEMSG_REQUEST_CANCELRESIDENT,			//[제완] 마을 주민등록신청을 취소한다 
	GAMEMSG_RESPONSE_CANCELRESIDENT,		//[제완]

	GAMEMSG_REQUEST_BERESIDENT,				//[제완] 대행수가 신청자를 마을 주민으로 등록한다 
	GAMEMSG_RESPONSE_BERESIDENT,			//[제완]

	GAMEMSG_REQUEST_RESIDENTAPPLYERLIST,	//[제완] 마을 주민등록신청자 리스트를 요청한다
	GAMEMSG_RESPONSE_RESIDENTAPPLYERLIST,	//[제완]

	GAMEMSG_REQUEST_RESIDENTLIST,			//[제완] 마을 주민 리스트를 요청한다 
	GAMEMSG_RESPONSE_RESIDENTLIST,			//[제완]

	GAMEMSG_REQUEST_RESIDENTINFO,			//[제완] 마을 주민의 상세 정보를 요청한다 
	GAMEMSG_RESPONSE_RESIDENTINFO,			//[제완]

	GAMEMSG_REQUEST_SETRESIDENTSCORE,		//[제완] 마을 주민의 점수를 설정한다 
	GAMEMSG_RESPONSE_SETRESIDENTSCORE,		//[제완]

	GAMEMSG_REQUEST_GETSTRUCTURELIST,		//[제완] 마을 건물 목록을 요청한다 
	GAMEMSG_RESPONSE_GETSTRUCTURELIST,		//[제완]

	GAMEMSG_REQUEST_UPDATESTRUCTURELIST,	//[제완] 마을 건물 목록을 갱신한다 
	GAMEMSG_RESPONSE_UPDATESTRUCTURELIST,	//[제완]	

	GAMEMSG_REQUEST_BUILDSTRUCTURE,			//[제완] 마을 건물을 건설한다 
	GAMEMSG_RESPONSE_BUILDSTRUCTURE,		//[제완]

	GAMEMSG_REQUEST_DESTROYSTRUCTURE,		//[제완] 마을 건물을 해체한다 
	GAMEMSG_RESPONSE_DESTROYSTRUCTURE,		//[제완]

	GAMEMSG_REQUEST_VILLAGENOTICE,			//[상민]
	GAMEMSG_RESPONSE_VILLAGENOTICE,			//[상민]

	GAMEMSG_REQUEST_ALLSTRCHIEFINFO,		//[제완] 마을의 모든 행수 정보를 얻어온다
	GAMEMSG_RESPONSE_ALLSTRCHIEFINFO,

	GAMEMSG_REQUEST_PROFITTOCAPITALFUND,	//[제완] 이익금->자본금으로 전환
	GAMEMSG_RESPONSE_PROFITTOCAPITALFUND,	//[제완]

	GAMEMSG_REQUEST_STARTEVENT,				//[제완] 마을 이벤트 시작
	GAMEMSG_RESPONSE_STARTEVENT,

	GAMEMSG_REQUEST_SETVILLAGEMARK,			//[제완] 마을 마크를 설정한다 
	GAMEMSG_RESPONSE_SETVILLAGEMARK,		//[제완]

	GAMEMSG_REQUEST_SETVILLAGEMAP,			//[제완] 마을 맵을 설정한다.
	GAMEMSG_RESPONSE_SETVILLAGEMAP,			//[제완]

	GAMEMSG_REQUEST_VILLAGELEVELDETAILINFO,	//[제완] 마을 레벨 상세 정보 
	GAMEMSG_RESPONSE_VILLAGELEVELDETAILINFO,//[제완]

	GAMEMSG_REQUEST_GENERALMEETING_INFO,			//[제완] 주주총회 정보를 요청한다
	GAMEMSG_RESPONSE_GENERALMEETING_INFO,			//[제완]

	GAMEMSG_REQUEST_GENERALMEETING_SUGGEST,			//[제완] 안건을 발의 한다 
	GAMEMSG_RESPONSE_GENERALMEETING_SUGGEST,		//[제완]

	GAMEMSG_REQUEST_GENERALMEETING_VOTE,			//[제완] 투표 한다
	GAMEMSG_RESPONSE_GENERALMEETING_VOTE,			//[제완]

	GAMEMSG_REQUEST_GENERALMEETING_ACCEPTCANDIDATE,	//[제완] 대행수 후보자의 수락 
	GAMEMSG_RESPONSE_GENERALMEETING_ACCEPTCANDIDATE,//[제완]

	GAMEMSG_RESPONSE_SETVILLAGEUPGRADE,				//[준엽] 레벨에 따른 건물 업그레이드

	//#if defined(_CITYHALL_BANKRUPTCY)
	GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE, //[용래] 서버의 마을 상태 관련 알림 통보

	//#endif


	GAMEMSG_RESPONSE_GENERALMEETINGLETTER,			//[제완] 주주총회 참여 여부를 묻는다

	GAMEMSG_REQUEST_GENERALMEETING_PARTICIPATE,		//[제완] 주주총회 참여 여부를 설정한다
	GAMEMSG_RESPONSE_GENERALMEETING_PARTICIPATE,	//[제완]

	GAMEMSG_REQUEST_GENERALMEETING_CHAT,			//[제완] 
	GAMEMSG_RESPONSE_GENERALMEETING_CHAT,			//[제완] 

	GAMEMSG_REQUEST_GENERALMEETING_CHATJOIN,		//[제완] 
	GAMEMSG_RESPONSE_GENERALMEETING_CHATJOIN,		//[제완] 

	GAMEMSG_REQUEST_GENERALMEETING_CHATOUT,			//[제완] 
	GAMEMSG_RESPONSE_GENERALMEETING_CHATOUT,		//[제완] 

	GAMEMSG_NOTICE_GENERALMEETING,					//[제완] 

	GAMEMSG_REQUEST_CHANGESTRBASEMONEY,				//[광명] 건물 자본금 설정(수리비)
	GAMEMSG_RESPONSE_CHANGESTRBASEMONEY,

	GAMEMSG_REQUEST_GENERALMEETING_VOTE_REFRESH,	//[연준]	투표 상황 갱신요청

	GAMEMSG_RESPONSE_VILLAGEWARRESULT_FOR_VILLAGEMEMBER,	//[영진] 해당 마을 주민들에게만 보여주는 공성전 결과

	GAMEMSG_REQUEST_DONATEVILLAGEMONEY,				//[광명] 마을 기부금 내기
	GAMEMSG_RESPONSE_DONATEVILLAGEMONEY,

	GAMEMSG_REQUEST_DONATEMEMBERLIST,				//[광명] 기부한 사람보기
	GAMEMSG_RESPONSE_DONATEMEMBERLIST,

	//--------------------------------
	// 전장 
	//--------------------------------
	GAMEMSG_REQUEST_CREATEBANKACCOUNT	= 1200,		//[태곤] 전장에 계좌를 만들것을 요청한다
	GAMEMSG_REQUEST_CLOSEBANKACCOUNT,				//[태곤] 전장에 계좌 해지를 요청한다

	GAMEMSG_REQUEST_INPUTTOBANK,					//[태곤] 전장에 돈을 맡기는 것을 요청한다. 

	GAMEMSG_REQUEST_INPUTTOBANKALLMONEY,			//[태곤] 소지금을 모두 입금 

	GAMEMSG_REQUEST_OUTPUTFROMBANK,					//[태곤] 전장에서 돈을 찾을 것을 요청한다. 

	GAMEMSG_REQUEST_OUTPUTFROMBANKALLMONEY,			//[광명] 전장에서 수수료를 빼고 모든 돈을 찾을 것을 요청한다. 

	GAMEMSG_RESPONSE_BANKMONEY,						//[태곤] 전장 잔액을 클라이언트에게 통보한다. 

	GAMEMSG_REQUEST_SETBANKFEE,						//[태곤] 전장 수수료 설정 요청. 

	GAMEMSG_RESPONSE_DATE,							//[태곤] 현재시간 통보. 

	GAMEMSG_RESPONSE_CANCEL_STOCKORDER,				//[연준] 전장의 돈이 모자라 매수주문취소

	GAMEMSG_RESPONSE_CREATEBANKACCOUNT_CANCEL,		//[추가 : 황진성 2008. 1. 9 => 은행 내구도가 0이어서 계좌만들기 실패.]	
	//-------------------------
	// 여각 
	//-------------------------
	GAMEMSG_REQUEST_STOCKORDERINFO = 1300,			//[태곤] 여각에 올라온 주문중에 적절한 것을 요청한다. 
	GAMEMSG_RESPONSE_STOCKORDERINFO,				//[태곤]

	GAMEMSG_REQUEST_CANCELSTOCKORDER,				//[태곤] 주식 주문 취소 요청.  	

	GAMEMSG_RESPONSE_STOCKPRICE,					//[태곤] 각 마을의 주가를 클라이언트에게 통보 

	GAMEMSG_REQUEST_SELLSTOCK,						//[태곤] 주식을 팔 것을 요청한다. 
	GAMEMSG_REQUEST_BUYSTOCK,						//[태곤] 주식을 살 것을 요청한다.

	GAMEMSG_REQUEST_STOCKBIDVILLAGELIST,			//[제완] 증자중인 마을 리스트를 요청한다 
	GAMEMSG_RESPONSE_STOCKBIDVILLAGELIST,			//[제완]

	GAMEMSG_REQUEST_STOCKBIDDERLIST,				//[제완] 입찰자 리스트를 요청한다 
	GAMEMSG_RESPONSE_STOCKBIDDERLIST,				//[제완]

	GAMEMSG_REQUEST_BIDSTOCK,						//[제완] 입찰 한다 
	GAMEMSG_RESPONSE_BIDSTOCK,						//[제완]

	GAMEMSG_REQUEST_CANCELBIDSTOCK,					//[제완] 입찰 취소
	GAMEMSG_RESPONSE_CANCELBIDSTOCK,				//[제완]

	GAMEMSG_REQUEST_STOCKBIDDATE,					//[제완] 증자 개시일을 요청한다 (주기적으로)
	GAMEMSG_RESPONSE_STOCKBIDDATE,					//[제완]

	GAMEMSG_REQUEST_SETSTOCKFEE,					//[태곤] 여각 수수료 설정 요청. 

	GAMEMSG_RESPONSE_STOCKTRADEINFO,				//[태곤] 여각에 올려진 사자, 팔자 주문의 정보. 

	GAMEMSG_REQUEST_BUYBIDSTOCK,					//[광명] 증자시작된 주식을 산다.
	GAMEMSG_RESPONSE_BUYBIDSTOCK,

	GAMEMSG_REQUEST_MYSTOCKINFO,					//[광명] 증자시작된 주식때문에 정보를 요청

	GAMEMSG_REQUEST_STOCKOFFERINGORDER,				//[유상] 

	GAMEMSG_REQUEST_STOCKOFFERINGINFO,				//[제완]
	GAMEMSG_RESPONSE_STOCKOFFERINGINFO,				//[제완]

	GAMEMSG_REQUEST_EXCHANGESTOCK,					//[광명] 주식을 현금으로 바꾼다.
	GAMEMSG_RESPONSE_EXCHANGESTOCK,					//[광명]

	//------------------------------
	// 객주 관련 메시지. 
	//------------------------------
	GAMEMSG_REQUEST_GETFROMMARKET	= 1400,			//[태곤] 객주 창고에서 물건을 회수하라는 요청. 

	GAMEMSG_REQUEST_MARKETORDER,					//[태곤] 특정 물품의 주문 내역을 보내줄 것을 요청. 
	GAMEMSG_RESPONSE_MARKETORDER,					//[태곤] 물품 주문 내역요청에 대한 응답. 

	GAMEMSG_REQUEST_SELLITEM,						//[태곤] 물품을 팔 것을 요청한다. 
	GAMEMSG_REQUEST_BUYITEM,						//[태곤] 물품을 살 것을 요청한다. 

	GAMEMSG_RESPONSE_PRODUCTPRICE,					//[태곤] 물품 가격에 대한 통보.	

	GAMEMSG_RESPONSE_MARKETSELLINFO,				//[태곤] 객주에서 내 물건이 팔린 내역을 통보. 

	GAMEMSG_REQUEST_SETMARKETFEE,					//[태곤] 객주 수수료 설정 요청. 

	GAMEMSG_REQUEST_PRODUCTPRICE,					//[유상] 물품 가격 요청.



	//---------------------------
	// 시전 관련 메시지 
	//---------------------------
	GAMEMSG_REQUEST_CHANGEHOUSECONTRACT  = 1500,	//[태곤] 시전의 계약 조건을 변경한다. 

	GAMEMSG_RESPONSE_MYHOUSEUNITLIST,				//[기형] 나의 시전유니트의 리스트를 통보한다.

	GAMEMSG_REQUEST_HOUSEUNITLIST,					//[태곤] 시전유니트의 리스트를 요청한다. (일반인 누구나 사용 ) 
	GAMEMSG_RESPONSE_HOUSEUNITLIST,					//[태곤] 시전유니트의 리스트를 통보한다. 

	GAMEMSG_REQUEST_HOUSEUNITLIST_DETAIL,			//[태곤] 시전유니트의 상세 정보 리스트를 요청한다. (행수만 사용 ) 
	GAMEMSG_RESPONSE_HOUSEUNITLIST_DETAIL,			//[태곤] 시전유니트의 상세 정보 리스트를 통보한다. 

	GAMEMSG_REQUEST_HOUSEAPPLYERLIST,				//[태곤] 시전 임차 희망자 리스트를 요청한다.	
	GAMEMSG_RESPONSE_HOUSEAPPLYERLIST,				//[태곤] 시전 임차 희망자 리스트를 통보한다.

	GAMEMSG_REQUEST_HOUSERENTAPPLY,					//[태곤] 시전 임차 신청을 요청한다. 	

	GAMEMSG_REQUEST_HOUSERENTAPPLYCANCEL,			//[태곤] 시전 임대 신청 취소 요청. 

	GAMEMSG_REQUEST_HOUSERENT,						//[태곤] 시전을 임대할 것을 요청한다.
	//GAMEMSG_REQUEST_HOUSERENT_USER,					//[성웅] 시전을 임대신청을 요청한다.

	GAMEMSG_REQUEST_DELHOUSEAPPLYER,				//[태곤] 시전 임차 희망자 명단을 삭제하도록 요청한다. 
	GAMEMSG_RESPONSE_DELHOUSEAPPLYER,				//[태곤]

	GAMEMSG_REQUEST_STGINFO,						//[태곤] 창고 정보를 요청한다. 
	GAMEMSG_RESPONSE_STGINFO,						//[태곤] 창고 정보를 보낸다. 

	GAMEMSG_REQUEST_MOVEITEMINVTOSTG,				//[태곤] 인벤에서 창고로 아이템을 넣을 것을 요청한다. 
	GAMEMSG_REQUEST_MOVEITEMSTGTOINV,				//[태곤] 창고에서 인벤으로 아이템을 넣을 것을 요청한다. 	
	GAMEMSG_REQUEST_MAKEITEM,						//[태곤] 창고에서 아이템을 만들것을 요청한다. 

	GAMEMSG_RESPONSE_STGITEMINFO,					//[태곤] 창고 아이템 정보를 보낸다.(1개 아이템) 

	GAMEMSG_REQUEST_HOUSEUNITSTABLECREATE,			//[상민] 창고에 마구간을 생성한다.
	GAMEMSG_RESPONSE_HOUSEUNITSTABLECREATE,			//[상민]	

	GAMEMSG_REQUEST_HOUSEUNITHORSEIN,				//[상민] 마구간에 말을 넣는다.
	GAMEMSG_RESPONSE_HOUSEUNITHORSEIN,				//[상민]

	GAMEMSG_REQUEST_HOUSEUNITHORSEOUT,				//[상민] 마구간에서 말을 꺼낸다.
	GAMEMSG_RESPONSE_HOUSEUNITHORSEOUT,				//[상민]	

	GAMEMSG_RESPONSE_HOUSEUNITHORSEINFOSET,			//[상민] 마구간에 말의 정보가 변경되었음으로, 셑팅한다.
	GAMEMSG_RESPONSE_HOUSEUNITHORSEEATFOOD,			//[상민] 마구간에 말이 먹이가 먹었음을 통보한다.


	GAMEMSG_REQUEST_HOUSEUNITUSEDGOODSBUILDINGCREATE,		//[상민] 조선소에서 고물상을 생성한다.
	GAMEMSG_RESPONSE_HOUSEUNITUSEDGOODSBUILDINGCREATE,		//[상민]

	GAMEMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODSINFO,		// [상민] 고물에서 아이템 뽑을 때 정보를 나타낸다.	
	GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODSINFO,	// [상민]

	GAMEMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODS,			//[상민] 고물에서 아이템을 뽑아낸다.
	GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODS,		//[상민]

	GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODSNOTIFY,	//[상민] 고물에서 아이템을 뽑아낸것을 주위 사람들에게 알림

	GAMEMSG_REQUEST_HOUSEUNITNAMECHANGE,				//[기형] 창고 이름 변경
	GAMEMSG_RESPONSE_HOUSEUNITNAMECHANGE,				//[기형] 

	GAMEMSG_REQUEST_HOUSEITEMARRANGEMENT,				//[기형] 창고 아이템 정리

	//---------------------------
	// 농업 어업 축산업 등등 산업 ( Agriculture - 통합 지칭 )
	// 공통용
	//---------------------------
	GAMEMSG_REQUEST_AGRICULTUREAUTOGAIN	= 1800,		//[상민] 자동 획득
	GAMEMSG_RESPONSE_AGRICULTUREAUTOGAIN,			//[상민]

	GAMEMSG_RESPONSE_AGRICULTUREGETPRODUCTS,		//[상민] 물품 획득

	GAMEMSG_RESPONSE_AGRICULTUREGETPRODUCTSNOTIFY,	//[상민] 물품을 획득 했음을 나를 포함한 주위사람들에게 알림

	GAMEMSG_RESPONSE_AGRICULTURESTART,				//[상민] 생산 시작
	GAMEMSG_RESPONSE_AGRICULTUREPAYRENTFEE,			//[상민] 생산시설 임대하고 지불 했음을 알려줌

	GAMEMSG_RESPONSE_AGRICULTUREFORNOTRENTMAN,		//[상민] 건물에 임대하지 않은 사람도 사용하도록 해 주기 위해서.
	GAMEMSG_RESPONSE_AGRICULTURELEASELEFTTIME,		//[유상] 광산, 농산을 임시로 빌렸을때 남은 시간

	//---------------------------
	// 낚시
	//---------------------------
	GAMEMSG_RESPONSE_FISHINGSTART,					//[상민] 낚시 시작 알림

	GAMEMSG_RESPONSE_FISHINGDECREASETOOLDURABILITY, //[상민] 낚시대를 던질때 내구도를 닳게 함.

	GAMEMSG_RESPONSE_FISHINGGETMARINEPRODUCTS,		//[상민] 물고기를 낚았음을 주인에게 알려줌

	GAMEMSG_RESPONSE_FISHINGGETMARINEPRODUCTSNOTIFY,//[상민] 누가 물고기를 낚았음을 나를 포함한 주위사람들에게 알려줌. ( 채팅으로 표시하기 위해서 )

	//---------------------------
	// 농업
	//---------------------------
	GAMEMSG_REQUEST_FARMINGSTART,					//[상민] 농사 시작 요청 또는 획득 농산물 변경

	GAMEMSG_RESPONSE_FARMINGGETPRODUCTS,			//[상민] 농산물 획득 또는 실패

	GAMEMSG_RESPONSE_FARMINGGETPRODUCTSNOTIFY,		//[상민] 농산물을 획득 했음을 나를 포함한 주위사람들에게 알림

	GAMEMSG_REQUEST_FARMINGPAYRENTFEE,				//[상민] 농경지를 렌트 하지 않은 사람이 경작하기위해서 돈을 내고 경작 시작



	//---------------------------
	// 채굴
	//---------------------------
	GAMEMSG_REQUEST_MININGSTART,					//[상민] 채굴 시작

	GAMEMSG_RESPONSE_MININGGETPRODUCTS,				//[상민] 농산물 획득 또는 실패

	GAMEMSG_RESPONSE_MININGGETPRODUCTSNOTIFY,		//[상민] 주위에 알림

	GAMEMSG_REQUEST_MININGPAYRENTFEE,				//[상민] 광산 렌트	



	//---------------------------
	// 수렵장 관련 메시지 
	//---------------------------
	GAMEMSG_REQUEST_ENTERHUNT	= 1900,				//[태곤] 수렵장으로 들어갈 것을 요청. 

	GAMEMSG_REQUEST_HUNTMONSTERFIELDLISTADD,		//[상민] 몬스터 배치 추가
	GAMEMSG_RESPONSE_HUNTMONSTERFIELDLISTADD,		//[상민]

	GAMEMSG_REQUEST_HUNTMONSTERFIELDLISTDEL,		//[상민] 몬스터 배치 삭제
	GAMEMSG_RESPONSE_HUNTMONSTERFIELDLISTDEL,		//[상민]

	GAMEMSG_REQUEST_HUNTMONSTERIN,					//[상민] 몬스터 인형 창고에 넣기
	GAMEMSG_RESPONSE_HUNTMONSTERIN,					//[상민]

	GAMEMSG_REQUEST_HUNTMONSTEROUT,					//[상민] 몬스터 인형 창고에서 빼기
	GAMEMSG_RESPONSE_HUNTMONSTEROUT,				//[상민]

	GAMEMSG_RESPONSE_HUNTADDRATE,					//[태곤] 수렵장의 어드벤티지를 구한다. 

	//------------------------------
	// 역참 관련 메시지. 
	//------------------------------
	GAMEMSG_REQUEST_POSTOFFICE_SENDITEM = 2000,		//[제완] 물품 전달을 요청한다
	GAMEMSG_RESPONSE_POSTOFFICE_SENDITEM,			//[제완]

	GAMEMSG_REQUEST_POSTOFFICE_SENDMONEY,			//[제완] 송금을 요청한다 
	GAMEMSG_RESPONSE_POSTOFFICE_SENDMONEY,			//[제완]

	GAMEMSG_REQUEST_POSTOFFICE_SENDMSG,				//[제완] 메시지 전달을 요청한다
	GAMEMSG_RESPONSE_POSTOFFICE_SENDMSG,			//[제완]

	GAMEMSG_REQUEST_POSTOFFICE_RECVITEMLIST,		//[제완] 수신 아이템 목록을 요청한다
	GAMEMSG_RESPONSE_POSTOFFICE_RECVITEMLIST,		//[제완]

	GAMEMSG_REQUEST_POSTOFFICE_RECVMONEYLIST,		//[제완] 수신 현금 목록을 요청한다
	GAMEMSG_RESPONSE_POSTOFFICE_RECVMONEYLIST,		//[제완]

	GAMEMSG_REQUEST_POSTOFFICE_RECVMSGLIST,			//[제완] 수신 메시지 목록을 요청한다
	GAMEMSG_RESPONSE_POSTOFFICE_RECVMSGLIST,		//[제완]

	GAMEMSG_REQUEST_POSTOFFICE_GETITEM,				//[제완] 받은 아이템을 인벤토리로 넣는다
	GAMEMSG_RESPONSE_POSTOFFICE_GETITEM,			//[제완]

	GAMEMSG_REQUEST_POSTOFFICE_GETMONEY,			//[제완] 받은 현금을 내 소지금에 추가한다 
	GAMEMSG_RESPONSE_POSTOFFICE_GETMONEY,			//[제완]

	GAMEMSG_REQUEST_POSTOFFICE_DELMSG,				//[제완] 읽은 메시지를 삭제한다
	GAMEMSG_RESPONSE_POSTOFFICE_DELMSG,				//[제완]

	GAMEMSG_REQUEST_POSTOFFICE_SETFEE,				//[제완] 수수료를 설정한다

	GAMEMSG_REQUEST_POSTOFFICE_GETRECORD,			//[제완] 송수신내역을 요청한다
	GAMEMSG_RESPONSE_POSTOFFICE_GETRECORD,			//[제완]

	//[진성] 우체국에서 단체 메시지 보내기. => 2008-8-4
	GAMEMSG_REQUEST_POSTOFFICE_SENDITEM_GROUP,		//[진성] 단체로 아이템 보내기. 
	GAMEMSG_REQUEST_POSTOFFICE_SENDMONEY_GROUP,		//[진성] 단체로 돈 보내기. 
	GAMEMSG_REQUEST_POSTOFFICE_SENDMSG_GROUP,		//[진성] 단체로 메시지 보내기. 

	GAMEMSG_REQUEST_POSTOFFICE_USER_LIST_GROUP,		//[진성] 단체로 보낼 리스트 요청.( 길드, 마을주민, 친구 )
	GAMEMSG_RESPONSE_POSTOFFICE_USER_LIST_GROUP,	//[진성] 단체 메시지 보내기. 단체로 보낼 유저리스트를 얻음. => 2008-8-4 	

	GAMEMSG_REQUEST_CHAR_NAME_CHECK,				//[진성] 직접 입력하는 유저의 이름을 받아 DB에 이름이 있는지 요청하게 될 메시지.
	GAMEMSG_RESPONSE_CHAR_NAME_CHECK,				//[진성] 단체 메시지 보내기. 단체로 보낼 리스트에 추가할 1명의 유저를 얻음. => 2008-8-4 




	GAMEMSG_REQUEST_GET_SAVEUSERITEM,				//[광명] 아이템을 얻어온다
	GAMEMSG_RESPONSE_GET_SAVEUSERITEM,

	GAMEMSG_REQUEST_GET_SAVEUSERHORSE,				//[희영] 기한이 지난 기승동물을 얻어온다
	GAMEMSG_RESPONSE_GET_SAVEUSERHORSE,

	GAMEMSG_REQUEST_GET_SAVEUSERITEMLIST,			//[광명] 아이템 리스트를 얻어온다
	GAMEMSG_RESPONSE_GET_SAVEUSERITEMLIST,

	GAMEMSG_REQUEST_MARKETCONDITIONS,				//[희영] 클라라 - 시세확인에 관한 정보를 주고 받는다.
	GAMEMSG_RESPONSE_MARKETCONDITIONS,

	//------------------------------
	// 사복시 관련 메시지. 
	//------------------------------
	GAMEMSG_REQUEST_HORSEMARKET_OUTHORSE	= 2100,	//[제완] 사복시 창고에서 말을 회수하라는 요청. 
	GAMEMSG_RESPONSE_HORSEMARKET_OUTHORSE,

	GAMEMSG_REQUEST_HORSEMARKET_MYACCOUNT,			//[제완] 내 주문 정보
	GAMEMSG_RESPONSE_HORSEMARKET_MYACCOUNT,			//[제완]

	GAMEMSG_REQUEST_HORSEMARKET_ORDERLIST,			//[제완] 말 주문 리스트
	GAMEMSG_RESPONSE_HORSEMARKET_ORDERLIST,			//[제완]

	GAMEMSG_REQUEST_HORSEMARKET_SELLHORSE,			//[제완] 말 팔자 주문
	GAMEMSG_RESPONSE_HORSEMARKET_SELLHORSE,			//[제완]

	GAMEMSG_REQUEST_HORSEMARKET_BUYHORSE,			//[제완] 말 구입
	GAMEMSG_RESPONSE_HORSEMARKET_BUYHORSE,			//[제완]

	GAMEMSG_REQUEST_HORSEMARKET_SETFEE,				//[제완] 사복시 수수료 설정 요청. 

	//------------------------------
	// 거간 관련 메시지. 
	//------------------------------
	GAMEMSG_REQUEST_REALESTATEMARKET_CANCELORDER = 2200,	//[제완] 주문을 취소한다 
	GAMEMSG_RESPONSE_REALESTATEMARKET_CANCELORDER,			//[제완]

	GAMEMSG_REQUEST_REALESTATEMARKET_MYORDER,				//[제완] 내 주문 정보를 얻어온다
	GAMEMSG_RESPONSE_REALESTATEMARKET_MYORDER,				//[제완]

	GAMEMSG_REQUEST_REALESTATEMARKET_ORDERLIST,				//[제완] 주문 리스트를 얻어온다
	GAMEMSG_RESPONSE_REALESTATEMARKET_ORDERLIST,			//[제완]

	GAMEMSG_REQUEST_REALESTATEMARKET_SELL,					//[제완] 팔자 주문을 한다 
	GAMEMSG_RESPONSE_REALESTATEMARKET_SELL,					//[제완]

	GAMEMSG_REQUEST_REALESTATEMARKET_BUY,					//[제완] 사자 주문을 한다 
	GAMEMSG_RESPONSE_REALESTATEMARKET_BUY,					//[제완]

	GAMEMSG_REQUEST_REALESTATEMARKET_SETFEE,				//[제완] 세금을 설정한다 

	GAMEMSG_REQUEST_REALESTATEMARKET_DETAILINFO,			//[제완] 상세 정보를 얻어온다 
	GAMEMSG_RESPONSE_REALESTATEMARKET_DETAILINFO,			//[제완]

	//------------------------------
	// 장예원 관련 메시지. 
	//------------------------------
	GAMEMSG_REQUEST_SUMMONMARKET_CANCELORDER = 2300,		//[제완] 주문을 취소한다 
	GAMEMSG_RESPONSE_SUMMONMARKET_CANCELORDER,				//[제완]

	GAMEMSG_REQUEST_SUMMONMARKET_MYORDER,					//[제완] 내 주문 정보를 얻어온다 
	GAMEMSG_RESPONSE_SUMMONMARKET_MYORDER,					//[제완]

	GAMEMSG_REQUEST_SUMMONMARKET_ORDERLIST,					//[제완] 주문 리스트를 얻어온다 
	GAMEMSG_RESPONSE_SUMMONMARKET_ORDERLIST,				//[제완]

	GAMEMSG_REQUEST_SUMMONMARKET_SELL,						//[제완] 팔자 주문을 한다
	GAMEMSG_RESPONSE_SUMMONMARKET_SELL,						//[제완]

	GAMEMSG_REQUEST_SUMMONMARKET_BUY,						//[제완] 사자 주문을 한다 
	GAMEMSG_RESPONSE_SUMMONMARKET_BUY,						//[제완]

	GAMEMSG_REQUEST_SUMMONMARKET_SETFEE,					//[제완] 세금을 설정한다 


	//------------------------------
	// 농장 관련 메시지. 
	//------------------------------
	GAMEMSG_REQUEST_LAND_CHANGECONTRACT  = 2500,	//[제완] 농장 계약 조건을 변경한다. 

	GAMEMSG_REQUEST_LAND_UNITLIST,					//[제완] 농장 유니트의 리스트를 요청한다. (일반인 누구나 사용 ) 
	GAMEMSG_RESPONSE_LAND_UNITLIST,					//[제완] 

	GAMEMSG_REQUEST_LAND_UNITDETAIL,				//[제완] 농장 유니트의 상세 정보 리스트를 요청한다. (행수만 사용 ) 
	GAMEMSG_RESPONSE_LAND_UNITDETAIL,				//[제완] 

	GAMEMSG_REQUEST_LAND_APPLYERLIST,				//[제완] 농장 임차 희망자 리스트를 요청한다.	
	GAMEMSG_RESPONSE_LAND_APPLYERLIST,				//[제완]

	GAMEMSG_REQUEST_LAND_RENTAPPLY,					//[제완] 농장 임차 신청을 요청한다. 	
	GAMEMSG_RESPONSE_LAND_RENTAPPLY,				//[제완]

	GAMEMSG_REQUEST_LAND_CANCELAPPLY,				//[제완] 농장 임대 신청 취소 요청. 
	GAMEMSG_RESPONSE_LAND_CANCELAPPLY,				//[제완]

	GAMEMSG_REQUEST_LAND_RENT,						//[제완] 농장를 임대할 것을 요청한다. (행수)
	GAMEMSG_RESPONSE_LAND_RENT,

	GAMEMSG_REQUEST_LAND_DELAPPLYER,				//[제완] 농장 임차 희망자 명단을 삭제하도록 요청한다. 
	GAMEMSG_RESPONSE_LAND_DELAPPLYER,				//[제완]

	//------------------------------
	// 내자시 관련 메시지. 
	//------------------------------
	GAMEMSG_REQUEST_FEAST_RESERVE = 2600,			//[제완] 연회 예약 신청을 한다 
	GAMEMSG_RESPONSE_FEAST_RESERVE,					//[제완]

	GAMEMSG_REQUEST_FEAST_SETCOST,					//[제완] 행사비를 설정한다 
	GAMEMSG_RESPONSE_FEAST_SETCOST,					//[제완]

	GAMEMSG_REQUEST_FEAST_ADDPRESENTLIST,			//[제완] 선물 목록을 추가한다 
	GAMEMSG_RESPONSE_FEAST_ADDPRESENTLIST,			//[제완]

	GAMEMSG_REQUEST_FEAST_DELPRESENTLIST,			//[제완] 선물 목록을 삭제한다 
	GAMEMSG_RESPONSE_FEAST_DELPRESENTLIST,			//[제완]

	GAMEMSG_REQUEST_FEAST_STGITEMIN,				//[제완] 창고에 아이템을 넣는다 
	GAMEMSG_RESPONSE_FEAST_STGITEMIN,				//[제완]

	GAMEMSG_REQUEST_FEAST_STGITEMOUT,				//[제완] 창고에서 아이템을 뺀다
	GAMEMSG_RESPONSE_FEAST_STGITEMOUT,				//[제완]

	GAMEMSG_REQUEST_FEAST_GIVEITEM,					//[제완] 연회 신청자가 아이템을 지급한다
	GAMEMSG_RESPONSE_FEAST_GIVEITEM,				//[제완]

	GAMEMSG_REQUEST_FEAST_SENDCHAT,					//[제완] 마을에 모인 사람들에게 공지를 한다.

	//#if defined(_LEON_FEAST_EFFECT)
	GAMEMSG_REQUEST_FEAST_USEEFFECT,					//[용래] 연회 중 마을에 모인 사람들 이펙트를 뿌린다.
	GAMEMSG_RESPONSE_FEAST_USEEFFECT,					//[용래] 
	//#endif

	GAMEMSG_REQUEST_PARTYHALL_TOTAL_INFO,						//[진성] 파티홀. 전체 정보 얻기. C->S => 2008. 3. 17
	GAMEMSG_RESPONSE_PARTYHALL_TOTAL_INFO,						//[진성] 파티홀. 전체 정보 얻기. S->C => 2008. 3. 17

	GAMEMSG_REQUEST_PARTYHALL_RESERVATION,						//[진성] 파티홀. 파티예약 등록. C->S => 2008. 3. 11		
	GAMEMSG_RESPONSE_PARTYHALL_RESERVATION,						//[진성] 파티홀. 파티예약 등록. S->C => 2008. 3. 11	

	GAMEMSG_REQUEST_PARTYHALL_ADD_PRESENT,						//[진성] 파티홀. 선물 등록. C->S => 2008. 3. 11
	GAMEMSG_RESPONSE_PARTYHALL_ADD_PRESENT,						//[진성] 파티홀. 선물 등록. S->C => 2008. 3. 11	

	GAMEMSG_REQUEST_PARTYHALL_ENTRANCE_APPLICATION,				//[진성] 파티홀. 출입 신청. C->S => 2008. 3. 11
	GAMEMSG_RESPONSE_PARTYHALL_ENTRANCE_APPLICATION,			//[진성] 파티홀. 출입 신청. S->C => 2008. 3. 11	

	GAMEMSG_REQUEST_PARTYHALL_ENTRANCE_APPLICATION_LIST,		//[진성] 파티홀. 출입 신청자. C->S => 2008. 3. 11
	GAMEMSG_RESPONSE_PARTYHALL_ENTRANCE_APPLICATION_LIST,		//[진성] 파티홀. 출입 신청자. S->C => 2008. 3. 11	

	GAMEMSG_REQUEST_PARTYHALL_GET_COSTUME,						//[진성] 파티홀. 옷 얻기. C->S => 2008. 3. 11
	GAMEMSG_RESPONSE_PARTYHALL_GET_COSTUME,						//[진성] 파티홀. 옷 얻기. S->C => 2008. 3. 11	

	GAMEMSG_REQUEST_PARTYHALL_ADD_DECORATION,					//[진성] 파티홀. 장식물 추가. C->S => 2008. 3. 11
	GAMEMSG_RESPONSE_PARTYHALL_ADD_DECORATION,					//[진성] 파티홀. 장식물 추가. S->C => 2008. 3. 11

	GAMEMSG_REQUEST_PARTYHALL_GIVEITEM,							//[진성] 파티홀. 장식물 추가. C->S => 2008. 3. 19
	GAMEMSG_RESPONSE_PARTYHALL_GIVEITEM,						//[진성] 파티홀. 장식물 추가. S->C => 2008. 3. 19

	GAMEMSG_REQUEST_PARTYHALL_EXIT,								//[진성] 파티홀. 파티장에서 나가기. C->S => 2008. 3. 19

	GAMEMSG_REQUEST_PARTYHALL_INVITATION,						//[진성] 파티홀. 파티장에 초대하기. C->S => 2008. 3. 23
	GAMEMSG_RESPONSE_PARTYHALL_INVITATION,						//[진성] 파티홀. 파티장에 초대하기. S->C => 2008. 3. 23

	GAMEMSG_REQUEST_PARTYHALL_USEEFFECT,						//[진성] 파티홀. 파티장 폭죽. C->S => 2008. 3. 28
	GAMEMSG_RESPONSE_PARTYHALL_USEEFFECT,						//[진성] 파티홀. 파티장 폭죽. S->C => 2008. 3. 28

	GAMEMSG_RESPONSE_PARTYHALL_DECORATION_LIST,
	//------------------------------
	// 비변사 관련 메시지. 
	//------------------------------
	GAMEMSG_REQUEST_SUMMONHEROMARKET_CANCELORDER = 2700,		//[제완] 주문을 취소한다 
	GAMEMSG_RESPONSE_SUMMONHEROMARKET_CANCELORDER,				//[제완]

	GAMEMSG_REQUEST_SUMMONHEROMARKET_MYORDER,					//[제완] 내 주문 정보를 얻어온다 
	GAMEMSG_RESPONSE_SUMMONHEROMARKET_MYORDER,					//[제완]

	GAMEMSG_REQUEST_SUMMONHEROMARKET_ORDERLIST,					//[제완] 주문 리스트를 얻어온다 
	GAMEMSG_RESPONSE_SUMMONHEROMARKET_ORDERLIST,				//[제완]

	GAMEMSG_REQUEST_SUMMONHEROMARKET_SELL,						//[제완] 팔자 주문을 한다
	GAMEMSG_RESPONSE_SUMMONHEROMARKET_SELL,						//[제완]

	GAMEMSG_REQUEST_SUMMONHEROMARKET_BUY,						//[제완] 사자 주문을 한다 
	GAMEMSG_RESPONSE_SUMMONHEROMARKET_BUY,						//[제완]

	GAMEMSG_REQUEST_SUMMONHEROMARKET_SETFEE,					//[제완] 세금을 설정한다 

	//------------------------------
	// 광산 관련 메시지. 
	//------------------------------
	GAMEMSG_REQUEST_MINE_CHANGECONTRACT  = 2800,	//[제완] 광산 계약 조건을 변경한다. 

	GAMEMSG_REQUEST_MINE_UNITLIST,					//[제완] 광산 유니트의 리스트를 요청한다. (일반인 누구나 사용 ) 
	GAMEMSG_RESPONSE_MINE_UNITLIST,					//[제완] 

	GAMEMSG_REQUEST_MINE_UNITDETAIL,				//[제완] 광산 유니트의 상세 정보 리스트를 요청한다. (행수만 사용 ) 
	GAMEMSG_RESPONSE_MINE_UNITDETAIL,				//[제완] 

	GAMEMSG_REQUEST_MINE_APPLYERLIST,				//[제완] 광산 임차 희망자 리스트를 요청한다.	
	GAMEMSG_RESPONSE_MINE_APPLYERLIST,				//[제완]

	GAMEMSG_REQUEST_MINE_RENTAPPLY,					//[제완] 광산 임차 신청을 요청한다. 	
	GAMEMSG_RESPONSE_MINE_RENTAPPLY,				//[제완]

	GAMEMSG_REQUEST_MINE_CANCELAPPLY,				//[제완] 광산 임대 신청 취소 요청. 
	GAMEMSG_RESPONSE_MINE_CANCELAPPLY,				//[제완]

	GAMEMSG_REQUEST_MINE_RENT,						//[제완] 광산를 임대할 것을 요청한다. (행수)
	GAMEMSG_RESPONSE_MINE_RENT,

	GAMEMSG_REQUEST_MINE_DELAPPLYER,				//[제완] 광산 임차 희망자 명단을 삭제하도록 요청한다. 
	GAMEMSG_RESPONSE_MINE_DELAPPLYER,				//[제완]

	//-------------------------------------
	// Guild
	//-------------------------------------
	GAMEMSG_REQUEST_GUILD_CREATEGUILD	= 2900,		//[준엽] 길드 생성을 요청한다.
	GAMEMSG_RESPONSE_GUILD_CREATEGUILD,

	GAMEMSG_REQUEST_GUILD_DESTROYGUILD,				//[준엽] 길드 해체를 요청한다.
	GAMEMSG_RESPONSE_GUILD_DESTROYGUILD,

	GAMEMSG_REQUEST_GUILD_JOINGUILD,				//[준엽] 길드 가입을 요청한다.
	GAMEMSG_RESPONSE_GUILD_JOINGUILD,

	GAMEMSG_REQUEST_GUILD_CONFIRMJOINGUILD,			//[준엽] 길드 가입을 허가한다.
	GAMEMSG_RESPONSE_GUILD_CONFIRMJOINGUILD,

	GAMEMSG_REQUEST_GUILD_LEAVEGUILD,				//[준엽] 길드 탈퇴를 요청한다.
	GAMEMSG_RESPONSE_GUILD_LEAVEGUILD,

	GAMEMSG_REQUEST_GUILD_CHANGEVILLAGE,			//[준엽] 길드 본거지 이동을 요청한다.
	GAMEMSG_RESPONSE_GUILD_CHANGEVILLAGE,

	GAMEMSG_REQUEST_GUILD_GETGUILDLIST,				//[준엽] 길드 리스트를 불러온다.
	GAMEMSG_RESPONSE_GUILD_GETGUILDLIST,

	GAMEMSG_REQUEST_GUILD_GETGUILDINFO,				//[준엽] 특정 길드의 정보를 불러온다.
	GAMEMSG_RESPONSE_GUILD_GETGUILDINFO,

	GAMEMSG_REQUEST_GUILD_CONFIRMCREATEGUILD,		//[준엽] 길드 생성을 허가한다.
	GAMEMSG_RESPONSE_GUILD_CONFIRMCREATEGUILD,

	GAMEMSG_REQUEST_GUILD_SETSECONDMASTER,			//[준엽] 부길드장을 선출한다.
	GAMEMSG_RESPONSE_GUILD_SETSECONDMASTER,

	GAMEMSG_REQUEST_GUILD_GIVEGUILDMONEY,			//[준엽] 길드 자금을 납부한다.
	GAMEMSG_RESPONSE_GUILD_GIVEGUILDMONEY,

	GAMEMSG_REQUEST_GUILD_SETSTRFEERATE,			//[준엽] 길드 건물 수수료를 설정한다. - 행수
	GAMEMSG_RESPONSE_GUILD_SETSTRFEERATE,

	GAMEMSG_REQUEST_GUILD_CHANGESTATUS,				//[준엽] 길드 능력치를 변경한다.
	GAMEMSG_RESPONSE_GUILD_CHANGESTATUS,

	GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS,			//[준엽] 길드 능력치를 길드원에게 알려준다.

	GAMEMSG_REQUEST_GUILD_SETMARK,					//[준엽] 마크를 결정한다.
	GAMEMSG_RESPONSE_GUILD_SETMARK,

	GAMEMSG_REQUEST_GUILD_CHANGENAME,				//[준엽] 이름을 바꾼다.
	GAMEMSG_RESPONSE_GUILD_CHANGENAME,

	GAMEMSG_REQUEST_GUILD_CHANGEMASTER,				//[유상] 길드마스터 변경
	GAMEMSG_RESPONSE_GUILD_CHANGEMASTER,			

	GAMEMSG_REQUEST_GUILDWAR_RESERVELIST,			//[유상] 길드전 방 예약 리스트
	GAMEMSG_RESPONSE_GUILDWAR_RESERVELIST,			

	GAMEMSG_REQUEST_GUILDWAR_APPLYRESERVE,			//[유상] 길드전 예약 신청
	GAMEMSG_RESPONSE_GUILDWAR_APPLYRESERVE,			//[유상] 길드전 예약 신청

	GAMEMSG_RESPONSE_GUILDWAR_START,				//[유상] 길드전 시작 알림
	GAMEMSG_RESPONSE_GUILDWAR_END,					//[유상] 길드전 끝 알림
	GAMEMSG_RESPONSE_GUILDWAR_KILLPOINT,			//[유상] 길드전 KillPoint 알림

	GAMEMSG_REQUEST_GUILDDUNGEON_APPLYLIST,			//[유상] 사냥터 소유권 신청자 리스트
	GAMEMSG_RESPONSE_GUILDDUNGEON_APPLYLIST,

	GAMEMSG_REQUEST_GUILDDUNGEON_APPLY,				//[유상] 사냥터 소유권 신청하기
	GAMEMSG_RESPONSE_GUILDDUNGEON_APPLY,

	GAMEMSG_REQUEST_GUILDDUNGEON_STATUS,			//[유상] 사냥터 소유권 현황
	GAMEMSG_RESPONSE_GUILDDUNGEON_STATUS,

	GAMEMSG_REQUEST_GUILD_MEMBERLIST,				//[유상] 길드 멤버 리스트
	GAMEMSG_RESPONSE_GUILD_MEMBERLIST,

	GAMEMSG_REQUEST_GUILD_GETVILLAGELIST,			//[유상] 길드 건물 가지고있는 마을리스트
	GAMEMSG_RESPONSE_GUILD_GETVILLAGELIST,

	GAMEMSG_REQUEST_GUILDWAR_APPLYTOENEMY,			//[유상] 상대 길드에게 길드전 신청(신청길드->서버)
	GAMEMSG_RESPONSE_GUILDWAR_APPLYTOENEMY,			//서버->상대길드

	GAMEMSG_REQUEST_GUILDWAR_CONFIRMAPPLY,			//[유상] 길드전신청 응답 ( 상대길드->서버 )
	GAMEMSG_RESPONSE_GUILDWAR_CONFIRMAPPLY,			//서버->신청길드

	GAMEMSG_REQUEST_GUILDWAR_RECOMMANDLIST,			//[유상] 추천길드 목록 요청
	GAMEMSG_RESPONSE_GUILDWAR_RECOMMANDLIST,

	//GAMEMSG_RESPONSE_GUILD_CLOSEGUILD,			//[유상] 갈드 폐쇄(운영자 명령어)

	/*	GAMEMSG_REQUEST_GUILD_APPLYWAR,					//[준엽] 길드대전을 신청한다.
	GAMEMSG_RESPONSE_GUILD_APPLYWAR,

	GAMEMSG_REQUEST_GUILD_CONFIRMWAR,				//[준엽] 길드대전 수락 여부를 결정한다.
	GAMEMSG_RESPONSE_GUILD_CONFIRMWAR,

	GAMEMSG_RESPONSE_GUILD_STARTWAR,				//[준엽] 길드대전이 시작되었다

	GAMEMSG_REQUEST_GUILD_GETHUNTINGMAP,			//[준엽] 길드에 사냥터를 귀속시킨다.
	GAMEMSG_RESPONSE_GUILD_GETHUNTINGMAP,

	GAMEMSG_REQUEST_GUILD_APPLYWARMEMBER,			//[준엽] 길드전에 참가신청한다.
	GAMEMSG_RESPONSE_GUILD_APPLYWARMEMBER,

	GAMEMSG_REQUEST_GUILD_CONFIRMWARMEMBER,			//[준엽] 길드전 참가 신청을 허가 및 불허한다.
	GAMEMSG_RESPONSE_GUILD_CONFIRMWARMEMBER,

	GAMEMSG_REQUEST_GUILD_CONFIRMSTARTWAR,			//[준엽] 길드전 시작시 시작하겠다고 통보
	GAMEMSG_RESPONSE_GUILD_CONFIRMSTARTWAR,*/

	//----------------------------
	// 부동산 관련 메시지 
	//----------------------------
	GAMEMSG_RESPONSE_REALESTATE	= 5000,			//[태곤] Person의 부동산 정보를 통보한다. 

	GAMEMSG_REQUEST_EXTENDCONTRACT,				//[태곤] 부동산 계약 연장 요청. 


	//--------------------------
	// 건물 관련 메시지. 
	//--------------------------
	GAMEMSG_REQUEST_STRINFO	= 5100,			//[태곤] 건물에 대한 정보를 요청한다. 
	GAMEMSG_RESPONSE_STRINFO,				//[태곤] 건물에 대한 정보를 통보한다. 

	GAMEMSG_REQUEST_GETTAXLIST,				//[제완]
	GAMEMSG_RESPONSE_GETTAXLIST,			//[제완]

	GAMEMSG_REQUEST_SETTAX,					//[제완]
	GAMEMSG_RESPONSE_SETTAX,				//[제완]

	GAMEMSG_REQUEST_PAYTAX,					//[제완]
	GAMEMSG_RESPONSE_PAYTAX,				//[제완]

	GAMEMSG_REQUEST_GETDIVIDENDLIST,		//[제완]	
	GAMEMSG_RESPONSE_GETDIVIDENDLIST,		//[제완]

	GAMEMSG_REQUEST_DECLAREWAR,				//[제완]
	GAMEMSG_RESPONSE_DECLAREWAR,			//[제완]

	GAMEMSG_REQUEST_CANCELDECLAREWAR,		//[제완]
	GAMEMSG_RESPONSE_CANCELDECLAREWAR,		//[제완]

	GAMEMSG_RESPONSE_VILLAGEWARRESULT,		//[제완]
	GAMEMSG_RESPONSE_ENDVILLAGEWAR,			//[제완]

	GAMEMSG_REQUEST_RESIDENTSHARE,			//[제완]
	GAMEMSG_RESPONSE_RESIDENTSHARE,			//[제완]

	// 주민 신청자 리스트를 업데이트 한다(등록되거나 삭제된 행을 지운다)
	GAMEMSG_RESPONSE_UPDATERESIDENTAPPLYERLIST,	//[제완]

	GAMEMSG_REQUEST_SETSTRUCTUREMESSAGE,		//[제완]
	GAMEMSG_RESPONSE_SETSTRUCTUREMESSAGE,		//[제완]

	GAMEMSG_REQUEST_GETSTRNOTICE,				//[제완]
	GAMEMSG_RESPONSE_GETSTRNOTICE,				//[제완]

	GAMEMSG_RESPONSE_LIST_DELETEALL,			//[제완]
	GAMEMSG_RESPONSE_LIST_DELITEM,				//[제완]

	GAMEMSG_REQUEST_NPCPAYMENT,					//[제완]
	GAMEMSG_RESPONSE_NPCPAYMENT,				//[제완]

	GAMEMSG_REQUEST_VILLAGEINFOSTRINFO,			//[태곤] 마을 상세 정보중에서 특정 건물의 정보를 요청한다. 
	GAMEMSG_RESPONSE_VILLAGEINFOSTRINFO,		//[태곤]

	GAMEMSG_REQUEST_CHIEFINFO,					//[태곤] 현재의 Chief에 대한 정보를 요청. 
	GAMEMSG_RESPONSE_CHIEFINFO,					//[태곤] 현재의 Chief정보  대한 응답. 

	GAMEMSG_REQUEST_REPAIRSTRUCTURE,			//[제완] 마을 건물을 수리한다 
	GAMEMSG_RESPONSE_REPAIRSTRUCTURE,			//[제완]

	GAMEMSG_REQUEST_VILLAGEBOARD,				//[준엽] 마을 게시판을 요청한다.
	GAMEMSG_RESPONSE_VILLAGEBOARD,				//[준엽]

	GAMEMSG_REQUEST_HIREEVENTNPC,				//[강형] 이벤트 NPC고용을 요청한다.
	GAMEMSG_RESPONSE_HIREEVENTNPC,

	GAMEMSG_REQUEST_EVENT_INSERTMATERIAL,		//[강형] 만들기 이벤트 재료 넣기.
	GAMEMSG_RESPONSE_EVENT_INSERTMATERIAL,

	GAMEMSG_REQUEST_EVENT_MATERIALLIST,			//[강형] 만들기 이벤트 재료 현재 정보
	GAMEMSG_RESPONSE_EVENT_MATERIALLIST,

	GAMEMSG_REQUEST_EVENT_MATERIALCOMPLETE,		//[강형] 만들기 이벤트 재료 완성
	GAMEMSG_RESPONSE_EVENT_MATERIALCOMPLETE,

	GAMEMSG_RESPONSE_EVENT_COMPLETEBUILD,		//[강형] 만들기 이벤트 완성 메시지

	//--------------------------
	// 시스템 NPC
	//--------------------------
	GAMEMSG_REQUEST_SELLNPC = 5200,				//[상민] NPC에게 판매함을 요청한다. 
	GAMEMSG_RESPONSE_SELLNPC,					//[상민]

	GAMEMSG_REQUEST_BUYNPC,						//[상민] NPC에게서 사들임을 요청한다. 
	GAMEMSG_RESPONSE_BUYNPC,					//[상민]

	GAMEMSG_REQUEST_NPCSELLITEMINFO,			//[상민] NPC(무수리) 가 판매하는 아이템 정보를 요청
	GAMEMSG_RESPONSE_NPCSELLITEMINFO,			//[상민]

	GAMEMSG_REQUEST_NPCPOS,						//[태곤] NPC위치 정보 요청. 
	GAMEMSG_RESPONSE_NPCPOS,					//[태곤]

	GAMEMSG_REQUEST_INVESTMENTGETINFO,			//[준엽] 투자상담가(봉이 김선달) 정보 요청
	GAMEMSG_RESPONSE_INVESTMENTGETINFO,

	GAMEMSG_REQUEST_GETRECOMMANDPOINT,			//[영진] 나에게 있는 추천 포인트를 얻어 온다
	GAMEMSG_RESPONSE_GETRECOMMANDPOINT,

	GAMEMSG_REQUEST_BUYRECOMMANDPRODUCT,			//[영진] 추천포인트[RP]로 아이템을 구입
	GAMEMSG_RESPONSE_BUYRECOMMANDPRODUCT,

	GAMEMSG_REQUEST_RECOMMANDPOINTLIST,			//[영진] 자신을 추천한 유저의 목록을 가져옴
	GAMEMSG_RESPONSE_RECOMMANDPOINTLIST,

	GAMEMSG_REQUEST_VILLAGEHAVEEVENTNPC,			//[강형] 자신의 마을에 이벤트NPC가 있는지 알려줌
	GAMEMSG_RESPONSE_VILLAGEHAVEEVENTNPC,

	GAMEMSG_REQUEST_MANUFACTURENPC,
	GAMEMSG_REQUEST_RECEIVEBEGINNERITEM,


	//-------------------------
	// 교역 상인(청, 일본) NPC 와의 거래
	//-------------------------
	GAMEMSG_REQUEST_SELLTOFOREIGNMERCHANT = 5300,	//[상민] 교역 상인 NPC에게 판매를 요청

	GAMEMSG_REQUEST_FOREIGNMERCHANTNPCBUYINFO,		//[상민] 교역 상인 NPC 가 구매중인 아이템에 대한 정보를 요청	
	GAMEMSG_RESPONSE_FOREIGNMERCHANTNPCBUYINFO,		//[상민]

	GAMEMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEMINFO,	//[상민] 교역 상인이 구입하는 캐쉬 아이템 정보 요청
	GAMEMSG_RESPONSE_FOREIGNMERCHANTBUYCASHITEMINFO,	//[상민]

	GAMEMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEM,		//[상민] 교역 상인이 캐쉬 아이템 구입
	GAMEMSG_RESPONSE_FOREIGNMERCHANTBUYCASHITEM,	//[상민]


	GAMEMSG_REQUEST_TRADEMERCHANTBUYINFO,			//[상민] 무역 상인이 구입하는 정보를 요청
	GAMEMSG_RESPONSE_TRADEMERCHANTBUYINFO,			//[상민]

	GAMEMSG_REQUEST_TRADEMERCHANTBUYITEMFROMPERSON,	//[상민] 무역 상인이 물품을 구입
	GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEMFROMPERSON,//[상민]


	GAMEMSG_REQUEST_TRADEMERCHANTHORSEBUYINFO,		//[상민] 상인이 말을 구입하는 정보를 요청
	GAMEMSG_RESPONSE_TRADEMERCHANTHORSEBUYINFO,		//[상민]

	GAMEMSG_REQUEST_TRADEMERCHANTBUYHORSEFROMPERSON,//[상민] 상인이 말을 구입
	GAMEMSG_RESPONSE_TRADEMERCHANTBUYHORSEFROMPERSON,//[상민]

	GAMEMSG_REQUEST_TRADEMERCHANTBUYITEMINFO,		//[상민] 상인이 물건을 구입하는 정보를 요청
	GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEMINFO,		//[상민]

	GAMEMSG_REQUEST_TRADEMERCHANTBUYITEM,			//[상민] 상인이 물건을 구입
	GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEM,			//[상민]

	//-------------------------
	// 주식 공모
	//-------------------------
	GAMEMSG_RESPONSE_STOCKDISTRIBUTIONSET,			//[상민] 주식 공모를 알림

	GAMEMSG_REQUEST_STOCKDISTRIBUTIONORDER,			//[상민]
	GAMEMSG_RESPONSE_STOCKDISTRIBUTIONORDER,		//[상민]

	GAMEMSG_REQUEST_GETBEGINNERVILLAGESTOCK,		// [영훈] 초보자 마을의 주식을 달라고 요청

	//-------------------------
	// 개인간 거래
	//-------------------------
	GAMEMSG_REQUEST_PRIVATETRADE = 5400,		//[상민] 개인 거래를 요청한다.
	GAMEMSG_RESPONSE_PRIVATETRADE,				//[상민] 개인 거래 요청에 대한 응답.

	GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT,		//[영진] 개인 거래 수락 요청
	GAMEMSG_RESPONSE_PRIVATETRADE_ACCEPT,		//[영진] 개인 거래 수락 요청에 대한 응답

	GAMEMSG_REQUEST_PRIVATETRADE_CANCEL,		//[상민] 개인 거래를 취소한다.
	GAMEMSG_RESPONSE_PRIVATETRADE_CANCEL,		//[상민] 개인 거래 취소에 대한 응답.

	GAMEMSG_REQUEST_PRIVATETRADE_ADDITEM,		//[상민] 아이템을 거래한다.
	GAMEMSG_RESPONSE_PRIVATETRADE_ADDITEM,

	GAMEMSG_REQUEST_PRIVATETRADE_DELLIST,		//[상민] 거래 목록에서 삭제한다.
	GAMEMSG_RESPONSE_PRIVATETRADE_DELLIST,

	GAMEMSG_REQUEST_PRIVATETRADE_DELITEM,		//[상민] 거래 목록에서 아이템만 삭제한다.
	GAMEMSG_RESPONSE_PRIVATETRADE_DELITEM,

	GAMEMSG_REQUEST_PRIVATETRADE_ADDPRICE,		//[상민] 돈을 거래한다.
	GAMEMSG_RESPONSE_PRIVATETRADE_ADDPRICE,

	GAMEMSG_REQUEST_PRIVATETRADE_ADDSTOCK,		//[상민] 주식을 거래한다.
	GAMEMSG_RESPONSE_PRIVATETRADE_ADDSTOCK,

	GAMEMSG_REQUEST_PRIVATETRADE_ADDHOUSEUNIT,	//[상민] 시전을 거래한다.
	GAMEMSG_RESPONSE_PRIVATETRADE_ADDHOUSEUNIT,

	GAMEMSG_REQUEST_PRIVATETRADE_ADDHORSE,		//[상민] 말을 거래한다.
	GAMEMSG_RESPONSE_PRIVATETRADE_ADDHORSE,

	GAMEMSG_REQUEST_PRIVATETRADE_MYACCEPT,		//[상민] 개인 거래에서 내 목록을 수락 
	GAMEMSG_RESPONSE_PRIVATETRADE_MYACCEPT,		//[상민] 개인 거래에서 내 목록을 수락 응답

	GAMEMSG_REQUEST_PRIVATETRADE_MYCANCEL,		//[상민] 개인 거래에서 내 목록을 취소
	GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL,		//[상민] 개인 거래에서 내 목록을 취소 응답

	GAMEMSG_RESPONSE_PRIVATETRADE_OTHERACCEPT,	//[상민] 개인 거래에서 상대방이 목록을 수락 했음을 알림
	GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL,	//[상민] 개인 거래에서 상대방이 목록을 취소 했음을 알림

	GAMEMSG_RESPONSE_PRIVATETRADE_END,			//[상민] 개인 거래 종료

	//--------------------------
	// 말 
	//--------------------------
	GAMEMSG_REQUEST_RIDEHORSE		= 5500,	//[태곤] 말을 타라는 요청
	GAMEMSG_RESPONSE_RIDEHORSE,				//[태곤]

	GAMEMSG_REQUEST_UNRIDEHORSE,			//[태곤] 말에서 내리라는 요청. 
	GAMEMSG_RESPONSE_UNRIDEHORSE,			//[태곤]

	GAMEMSG_REQUEST_HORSERELEASE,			//[태곤] 말을 풀어준다. 

	GAMEMSG_REQUEST_HORSENAMECHANGE,		//[상민] 말 이름 변경
	GAMEMSG_RESPONSE_HORSENAMECHANGE,		//[상민]

	//-------------------------
	// 퀘스트 
	//-------------------------
	GAMEMSG_REQUEST_SETQUESTGOING	= 5700,			//[태곤] 퀘스트 진행 요청 메시지. 

	GAMEMSG_REQUEST_CANCELQUEST,					//[태곤] 퀘스트 취소. 

	GAMEMSG_REQUEST_QUESTSTATUS,					//[태곤] 퀘스트 진행 상황을 서버로 보낸다. 

	//-------------------------
	// 귓속말
	//-------------------------
	GAMEMSG_REQUEST_WHISPERCHAT_CREATE,				//[상민] 귓말하기 신청( 오른쪽 마우스로 귓속말 하기 또는 '/t' 를 한 경우 )
	GAMEMSG_RESPONSE_WHISPERCHAT_CREATE,			//[상민] 귓말하기 신청되었음을 알려줌.

	GAMEMSG_REQUEST_WHISPERCHAT_INVITE,				//[상민] 초대 요청
	GAMEMSG_RESPONSE_WHISPERCHAT_INVITE,			//[상민] 초대 되었음을 알려줌

	GAMEMSG_REQUEST_WHISPERCHAT_USERLIST,			//[상민] 귓속말중인 사람들의 리스트를 요청
	GAMEMSG_RESPONSE_WHISPERCHAT_USERLIST,			//[상민]

	GAMEMSG_REQUEST_WHISPERCHAT_OUT,				//[상민] 귓속말에서 나감
	GAMEMSG_RESPONSE_WHISPERCHAT_OUT,				//[상민] 귓속말에서 탈퇴했음을 알려줌


	//-------------------------
	// 파티
	//-------------------------
	GAMEMSG_REQUEST_PARTY_CREATEINVITE = 5800,		//[상민] 파티 생성 및 초대( 파티 생성이 안 된 단계지.. )
	GAMEMSG_RESPONSE_PARTY_CREATEINVITE,			//[상민] 파티 생성 및 초대 응답( 파티 생성이 안 된 단계지.. )

	GAMEMSG_REQUEST_PARTY_CREATEJOIN,				//[상민] 파티 생성 및 초대에 응하겠다.
	GAMEMSG_RESPONSE_PARTY_CREATEJOIN,				//[상민] 파티장이 될 사람에게 파티 시작을 알림

	GAMEMSG_REQUEST_PARTY_INVITE,					//[상민] 파티 초대 요청
	GAMEMSG_RESPONSE_PARTY_INVITE,					//[상민] 파티 초대 응답

	GAMEMSG_REQUEST_PARTY_INVITEREJECT,				//[상민] 파티 초대 거절 ( 파티 초대자 -> 서버 )
	GAMEMSG_RESPONSE_PARTY_INVITEREJECT,			//[상민] 파티 초대 거절 ( 서버 -> 파티장 )

	GAMEMSG_REQUEST_PARTY_JOIN,						//[상민] 파티 참여 요청
	GAMEMSG_RESPONSE_PARTY_JOIN,					//[상민] 파티 참여 응답

	GAMEMSG_REQUEST_PARTY_LEAVE,					//[상민] 파티 탈퇴 요청
	GAMEMSG_REQUEST_PARTY_FORCELEAVE,				//[상민] 파티 강제 퇴장 요청	

	GAMEMSG_REQUEST_PARTY_ALLUSERLIST,				//[상민] 파티 유저 리스트 전부 요청
	//[상민] 응답은 GAMEMSG_RESPONSE_PARTY_ADDUSER 이것이 루프 돌면서 다 보냄

	GAMEMSG_RESPONSE_PARTY_DELETE,					//[상민] 파티 삭제 알림

	GAMEMSG_RESPONSE_PARTY_ADDUSER,					//[상민] 파티 유저 추가 알림
	GAMEMSG_RESPONSE_PARTY_DELUSER,					//[상민] 파티 유저 탈퇴 알림
	GAMEMSG_RESPONSE_PARTY_FORCEDELUSER,			//[상민] 파티 유저 강제 탈퇴 알림 (파티장에 의한 것)

	GAMEMSG_RESPONSE_PARTY_CHANGELEADER,			//[상민] 파티장 변경 알리기 위해서.

	GAMEMSG_REQUEST_PARTY_CHANGETITLE,				//[상민] 파티 이름 변경
	GAMEMSG_RESPONSE_PARTY_CHANGETITLE,		

	GAMEMSG_RESPONSE_PARTY_NOTIFYEXP,				//[상민] 경험치 얻은 것을 알림

	GAMEMSG_REQUEST_PARTY_CHANGEITEMGETPATTERN,		//[영진] 파티 아이템 획득 방식 변경
	GAMEMSG_RESPONSE_PARTY_CHANGEITEMGETPATTERN,	//[영진] 파티 아이템 획득 방식 변경

	GAMEMSG_RESPONSE_PARTY_NOTIFYITEMGET,			//[영진] 파티원으 아이템 획득 알림

	GAMEMSG_RESPONSE_PARTY_PARTYUSERPOS,			//[영진] 파티원의 위치가 변경될때마다 파티원들에게 x, y, mapindex 를 보내준다.

	GAMEMSG_RESPONSE_PARTY_NOTIFYLEVELUP,			//[영진] 파티원의 레벨업 을 파티원들에게 알려준다.

	// [지연] 파티매칭으로 생긴 메세지 추가
	GAMEMSG_REQUEST_PARTY_WANTJOIN,					//[지연] 유저가 파장에게 파티 가입을 요청 (클라->서버)
	GAMEMSG_RESPONSE_PARTY_WANTJOIN,				//[지연] 서버가 받은 유저의 파티 가입 요청을 파장 클라에게 전달 (서버->클라)

	GAMEMSG_REQUEST_PARTY_WANTJOINRESULT,			//[지연] 유저의 파티 가입 요청에 대한 파장의 응답을 서버로 전달 (클라->서버)
	GAMEMSG_RESPONSE_PARTY_WANTJOINRESULT,			//[지연] 서버가 받은 파장의 파티 가입 요청에 대한 응답을 요청 유저 클라에 전달 (서버->클라)


	//-------------------------
	// 운영자 권한 
	//-------------------------
	GAMEMSG_RESPONSE_GAMEMASTERLEVEL	= 5900,	//[태곤] 운영자 권한에 관한 응답을 보낸다. 

	GAMEMSG_RESPONSE_GMGIVEMONEY,				//[태곤] 운영자가 돈을 준 것에 대한 결과. 

	GAMEMSG_REQUEST_PERSONAL_LOG,				//[준엽] 운영자가 사용자의 로그를 요청한다.
	GAMEMSG_RESPONSE_PERSONAL_LOG,

	//	GAMEMSG_RESPONSE_ITEMINFOGET,				//[준엽] 아이템의 정보를 얻어온다. 유니크 및 바이너리로

	GAMEMSG_REQUEST_GETGAMEMASTER_SERVERINFO,	//[준엽] 서버의 상태를 얻어온다.
	GAMEMSG_RESPONSE_GETGAMEMASTER_SERVERINFO,

	GAMEMSG_RESPONSE_NOTICEADMIN,				//[준엽] 중요한 공지를 한다.
	GAMEMSG_RESPONSE_GMGIVERECOMMANDPOINT,		//[유상] 운영자가 FP를 준 것에 다한 결과.

	//-------------------------
	// 웹 
	//-------------------------
	GAMEMSG_RESPONSE_NEWS	= 6000,				//[태곤]신문 정보 통보. 

	GAMEMSG_REQUEST_MYPUCHIKOMIURL,				//[영진] 자신의 로그인된 푸치코미 주소를 알아온다.
	GAMEMSG_RESPONSE_MYPUCHIKOMIURL,

	GAMEMSG_REQUEST_USERPUCHIKOMIURL,			//[영진] 다른유저의 푸치코미 주소를 알아온다.
	GAMEMSG_RESPONSE_USERPUCHIKOMIURL,


	//-------------------------
	// 친구
	//-------------------------
	GAMEMSG_REQUEST_FRIENDADD = 6200,			//6200 [상민] 친구 추가 요청
	GAMEMSG_RESPONSE_FRIENDADD,					//6201 [상민]

	GAMEMSG_REQUEST_FRIENDADDACCEPT,			//6202 [상민] 친구 추가 수락 요청
	GAMEMSG_RESPONSE_FRIENDADDACCEPT,			//6203 [상민]

	GAMEMSG_REQUEST_REJECTFRIENDADD,			//6204 [광명] 친구 추가 거부

	GAMEMSG_REQUEST_FRIENDDEL,					//6205 [상민] 친구 삭제 요청
	GAMEMSG_RESPONSE_FRIENDDEL,					//6206 [상민]

	GAMEMSG_RESPONSE_FRIENDLOGIN,				//6207 [상민] 친구 로그인 알림
	GAMEMSG_RESPONSE_FRIENDLOGOUT,				//6208 [상민] 친구 로그아웃 알림

	GAMEMSG_REQUEST_FRIENDLIST,					//6209 [유상] 친구 리스트 요청
	GAMEMSG_RESPONSE_FRIENDLIST,				//6210 [유상]

	GAMEMSG_REQUEST_FRIENDMEMO,					//6211 [유상] 친구 메모 요청
	GAMEMSG_RESPONSE_FRIENDMEMO,				//6212 [유상]

	GAMEMSG_REQUEST_FRIENDMEMOSET,				//6213 [유상] 친구 메모 저장
	GAMEMSG_RESPONSE_FRIENDMEMOSET,				//6214 [유상]

	GAMEMSG_REQUEST_FRIENDCHANGEGROUP,			//6215 [강형] 친구 그룹 변경
	GAMEMSG_RESPONSE_FRIENDCHANGEGROUP,			//6216 [강형]

	GAMEMSG_REQUEST_FRIENDCHANGEGROUPNAME,		//6217 [강형] 친구 그룹 이름 변경
	GAMEMSG_RESPONSE_FRIENDCHANGEGROUPNAME,		//6218 [강형]

	GAMEMSG_RESPONSE_FRIENDALLLIST,				//6219 [강형] 친구 전체 리스트

	//-------------------------
	// 투표
	//-------------------------
	GAMEMSG_REQUEST_VOTEKINGAVAILABLE,			//6220 [영훈] 군주 투표 목록을 보여달라는 요청
	GAMEMSG_RESPONSE_VOTEKINGAVAILABLE,			//6221 [상민] 왕 투표 가능 하므로 투표에 참여하라는 메시지. 

	GAMEMSG_REQUEST_VOTEKINGSELECTION,			//6222 [상민] 투표
	GAMEMSG_RESPONSE_VOTEKINGSELECTION,			//6223 [상민] 투표 결과 응답

	GAMEMSG_RESPONSE_VOTEKINGAGGREGATE,			//6224 [연준] 군주 투표 집계 결과 응답

	GAMEMSG_REQUEST_STARTELECTIONSTUMP,			//6225 [영훈] 선거시스템 : 유세시작
	GAMEMSG_RESPONSE_STARTELECTIONSTUMP,		//6226 

	GAMEMSG_REQUEST_ENDELECTIONSTUMP,			//6227 [영훈] 선거시스템 : 유세종료
	GAMEMSG_RESPONSE_ENDELECTIONSTUMP,			//6228 

	GAMEMSG_REQUEST_ELECTIONSTUMP,				//6229 [영훈] 선거시스템 : 케릭터 정보가 초기화 되었을때 유세 정보를 다시 보냄

	GAMEMSG_REQUEST_STARTVOTESTATUS,			//6230 [영훈] 선거 개표 현황 시작
	GAMEMSG_RESPONSE_STARTVOTESTATUS,			//6231 [영훈] 선거 개표 현황 시작알림

	GAMEMSG_RESPONSE_VOTESTATUS,				//6232 [영훈] 선개 개표 현황

	GAMEMSG_REQUEST_WARPTOPOLLMAP,				//6233 [영훈] 군주 투표소 지역으로 이동하라

	//-------------------------
	// Daily 퀘스트
	//-------------------------	
	GAMEMSG_REQUEST_DAILYQUESTLIST,				// [상민] 퀘스트 리스트 요청
	GAMEMSG_RESPONSE_DAILYQUESTLIST,			// [상민]

	GAMEMSG_REQUEST_DAILYQUESTSELECT,			// [상민] 퀘스트 선택
	GAMEMSG_RESPONSE_DAILYQUESTSELECT,			// [상민]

	GAMEMSG_REQUEST_DAILYQUESTSITUATION,		// [상민] 퀘스트 상황
	GAMEMSG_RESPONSE_DAILYQUESTSITUATION,		// [상민]

	GAMEMSG_RESPONSE_DAILYQUESTCHANGE,			// [상민] 퀘스트 남은 시간과 몬스터 잡은수를 업데이트 해줌.

	GAMEMSG_REQUEST_DAILYQUESTEND,				// [상민] 퀘스트 종료
	GAMEMSG_RESPONSE_DAILYQUESTEND,				// [상민]


	//---------------------------------
	// 가족관련
	//---------------------------------
	GAMEMSG_REQUEST_PROPOSE_QUESTION	= 6500,	// [영훈] 프로포즈 질문
	GAMEMSG_RESPONSE_PROPOSE_QUESTION,			// [영훈] 

	GAMEMSG_REQUEST_PROPOSE_ANSWER,				// [영훈] 프로포즈 답변

	GAMEMSG_RESPONSE_SET_MARRY,					// [영훈] 결혼 했다

	GAMEMSG_RESPONSE_GET_MARRY_INFO,			// [영훈] 결혼관련 정보 얻기

	GAMEMSG_REQUEST_WEDDINGANNIVERSARY_ITEMGET,	// [영훈] 결혼 기념일 아이템 받기 요청
	GAMEMSG_RESPONSE_MARRY_ITEMGET_SET,			// [영훈] 결혼 기념일 아이템 받은 날짜를 설정한다
	GAMEMSG_RESPONSE_MARRY_LOVE_SET,			// [영훈] 결혼 애정도 관련 설정


	//---------------------------------
	// ItemMall
	//---------------------------------
	GAMEMSG_REQUEST_ITEMMALLEXISTENCE = 6800,	//[준엽] 아이템 몰에서 산 아이템의 정보를 요청한다.
	GAMEMSG_RESPONSE_ITEMMALLEXISTENCE,

	GAMEMSG_REQUEST_ITEMMALLITEMLIST,			//[준엽] 아이템 몰에서 산 아이템의 정보를 요청한다.
	GAMEMSG_RESPONSE_ITEMMALLITEMLIST,

	GAMEMSG_REQUEST_ITEMMALLGETITEM,			//[준엽] 아이템 몰에서 산 아이템을 인벤토리로 옮긴다.
	GAMEMSG_RESPONSE_ITEMMALLGETITEM,

	GAMEMSG_REQUEST_INITSTATUS,					//[준엽] 상태초기화이용권을 사용하여 스테이터스 초기화 요청

	GAMEMSG_REQUEST_HUNTMAPWARP,				//[준엽] 순간이동이용권을 이용하여 사냥터로 이동한다.

	GAMEMSG_REQUEST_PERSONALSHOPWARP,			//[제형] 순간이동이용권을 이용하여 개인상점으로 이동한다.

	GAMEMSG_REQUEST_FRIENDWARP,					//[제형] 순간이동이용권을 이용하여 친구에게로 이동한다.

	GAMEMSG_REQUEST_PARTYWARP,					//[준엽] 파티원에게 이동한다.

	GAMEMSG_REQUEST_HORSEINIT,					//[준엽] 말상태초기화이용권을 사용하여 말의 스테이터스 초기화 요청

	GAMEMSG_REQUEST_USEHORSE_BONUS,				//[준엽] 말의 보너스를 이용하여 능력치를 올린다.

	GAMEMSG_REQUEST_CHANGE_HORSETYPE,			//[준엽] 말의 품종을 바꾼다.

	GAMEMSG_REQUEST_CHANGE_CHARNAME,			//[준엽] 캐릭터 이름을 변경한다.
	GAMEMSG_RESPONSE_CHANGE_CHARNAME,

	GAMEMSG_REQUEST_CHANGE_CHARKIND,			//[준엽] 캐릭터 종류를 변경한다.
	GAMEMSG_RESPONSE_CHANGE_CHARKIND,

	GAMEMSG_REQUEST_HORSE_ADDSTATUS,			//[준엽] 말능력향상이용권을 이용해서 말능력을 향상시킨다.

	GAMEMSG_REQUEST_NOLIMITTICKET_WARP,			//[준엽] 순간이동정액권을 이용해서 무한 워프를 설정한다.(기간제)
	GAMEMSG_RESPONSE_NOLIMITTICKET_WARP,

	GAMEMSG_REQUEST_GET_SEL_PGLIST,				//[준엽] 상품 목록 불러오기
	GAMEMSG_RESPONSE_GET_SEL_PGLIST,

	GAMEMSG_REQUEST_GET_ITEMMALL_ITEMINFO,		//[준엽] 상품의 정보를 요청한다.
	GAMEMSG_RESPONSE_GET_ITEMMALL_ITEMINFO,

	GAMEMSG_REQUEST_GET_MYCASH_INFO,			//[준엽] 내 캐쉬 정보를 요청한다.
	GAMEMSG_RESPONSE_GET_MYCASH_INFO,

	GAMEMSG_REQUEST_ITEMMALL_BUYITEM,			//[준엽] 아이템 몰에서 아이템 사기
	GAMEMSG_RESPONSE_ITEMMALL_BUYITEM,

	GAMEMSG_REQUEST_ITEMMALL_GIFTITEM,			//[준엽] 아이템 몰에서 아이템 선물하기
	GAMEMSG_RESPONSE_ITEMMALL_GIFTITEM,

	GAMEMSG_REQUEST_CHECK_ACCOUNTID,			//[준엽] 아이디 조회
	GAMEMSG_RESPONSE_CHECK_ACCOUNTID,

	GAMEMSG_REQUEST_BILLPAGE_URL,				//[영진] 서버로부터 암호화된 빌 페이지 URL 얻어오기
	GAMEMSG_RESPONSE_BILLPAGE_URL,

	GAMEMSG_RESPONSE_SET_CHANGEHERO,			//[준엽] 변신할 수 있는 영웅 설정

	GAMEMSG_REQUEST_CHANGE_HERO,				//[준엽] 영웅으로 변신!

	GAMEMSG_REQUEST_PRESENT_ANSWER,				//[영진] 선물 줘서 고맙다는 답장을 보낸다

	GAMEMSG_REQUEST_GACHAPAGE_URL,				//[영진] 가챠 페이지 주소 얻어오기
	GAMEMSG_RESPONSE_GACHAPAGE_URL,				//	

	GAMEMSG_REQUEST_LOGINPAGE_URL,				//
	GAMEMSG_RESPONSE_LOGINPAGE_URL,

	GAMEMSG_REQUEST_OILLING_USEITEM ,		//[희영 ] 휘발유를 사용했다. - 주유 , 시간제 아이템 사용.
	GAMEMSG_RESPONSE_OILLING_USEITEM ,			

	//--------------------------------
	// 기타
	//--------------------------------
	GAMEMSG_REQUEST_GACHAPRODUCT_LIST = 6900,	//[영진] 가차상품의 목록을 요청한다.
	GAMEMSG_RESPONSE_GACHAPRODUCT_LIST,
	GAMEMSG_REQUEST_GACHABUY,
	GAMEMSG_RESPONSE_GACHABUY,

	//KHY - 1104 -  갓차2 -라쿤 보물 상자. 
	GAMEMSG_REQUEST_GACHA2PRODUCT_LIST ,	//[희영] 가차2 상품의 목록을 요청한다.
	GAMEMSG_RESPONSE_GACHA2PRODUCT_LIST,
	GAMEMSG_REQUEST_GACHA2BUY,
	GAMEMSG_RESPONSE_GACHA2BUY,

	// 이월 가챠
	GAMEMSG_REQUEST_CARRYOVER_GACHA_LIST,			//[영훈] 현재/이월 가차 목록을 요청한다
	GAMEMSG_RESPONSE_CARRYOVER_GACHA_LIST,

	GAMEMSG_REQUEST_CARRYOVER_SELECTPRODUCT_LIST,	//[영훈] 현재/이월중 선택한 가차 상품의 목록을 요청한다
	GAMEMSG_RESPONSE_CARRYOVER_SELECTPRODUCT_LIST,

	GAMEMSG_REQUEST_CARRYOVER_GACHABUY,				//[영훈] 가챠를 실행 한다
	GAMEMSG_RESPONSE_CARRYOVER_GACHABUY,




	//-------------------------
	// 개인상점 
	//-------------------------
	GAMEMSG_REQUEST_PERSONALSHOPBANNER = 7000,	//[제형] 개인상점 배너
	GAMEMSG_RESPONSE_PERSONALSHOPBANNER,

	GAMEMSG_REQUEST_PERSONALSHOPDATA,			//[제형] 개인상점 데이터 
	GAMEMSG_RESPONSE_PERSONALSHOPDATA,

	GAMEMSG_RESPONSE_UPDATEPERSONALSHOPBANNER,	//[제형] 개인상점 베너 설정  

	GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA,	//[제형] 개인상점 데이터 설정  

	GAMEMSG_REQUEST_STARTPERSONALSHOP,			//[제형] 개인상점 시작 
	GAMEMSG_RESPONSE_STARTPERSONALSHOP,		

	GAMEMSG_REQUEST_STOPPERSONALSHOP,			//[제형] 개인상점 종료 
	GAMEMSG_RESPONSE_STOPPERSONALSHOP,

	GAMEMSG_REQUEST_BUYPERSONALSHOP,			//[제형] 개인상점 에서 본인이 물건을 사기 
	GAMEMSG_RESPONSE_BUYPERSONALSHOP,

	GAMEMSG_REQUEST_SELLPERSONALSHOP,			//[제형] 개인상점 에서 본인이 물건을 팔기 
	GAMEMSG_RESPONSE_SELLPERSONALSHOP,

	GAMEMSG_REQUEST_SEARCHPERSONALSHOPITEM,		//[제형] 개인상점물품 검색
	GAMEMSG_RESPONSE_SEARCHPERSONALSHOPITEM,

	GAMEMSG_REQUEST_MAKEPERSONALSHOP,			//[제형] 개인상점물품제작
	GAMEMSG_RESPONSE_MAKEPERSONALSHOP,

	//-------------------------
	// 소환수 SUMMON
	//-------------------------
	GAMEMSG_REQUEST_SUMMONFREE= 7100,			//[제형] 소환수를 풀어준다. 
	GAMEMSG_RESPONSE_SUMMONFREE,				//[제형]

	GAMEMSG_REQUEST_SUMMONNAMECHANGE,			//[제형] 소환수 이름 변경
	GAMEMSG_RESPONSE_SUMMONNAMECHANGE,			//[제형]

	GAMEMSG_REQUEST_SUMMONINFO,					//[제형] 소환수 정보를 요청한다.
	GAMEMSG_RESPONSE_SUMMONINFO,

	GAMEMSG_REQUEST_SUMMONREVIVE,				//[제형] 소환수를 부활시킨다.
	GAMEMSG_RESPONSE_SUMMONREVIVE,

	GAMEMSG_REQUEST_USESUMMONBONUS,				//[제형] 소환수의 보너스를 사용하게 한다.

	GAMEMSG_REQUEST_TAKEINOUT,					//[제형] 소환수 넣었다 뺐다 한다.
	
	GAMEMSG_REQUEST_SUMMONATTACKCHANGE,			//[제형] 소환수 공격속성 변경 
	GAMEMSG_RESPONSE_SUMMONATTACKCHANGE,		//[제형]

	GAMEMSG_REQUEST_SUMMONEVOLVE,				//[제형] 소환수 진화 
	GAMEMSG_RESPONSE_SUMMONEVOLVE,				//[제형] 

	GAMEMSG_REQUEST_SUMMONSCROLL,				//[제형] 소환수 강화 
	GAMEMSG_RESPONSE_SUMMONSCROLL,				//[제형] 

	GAMEMSG_REQUEST_SUMMON_STATUSINIT,			//[준엽] 소환수 상태 초기화

	GAMEMSG_REQUEST_SUMMON_SETAUTOHEAL,			//[준엽] 소환수 자동 회복 설정
	GAMEMSG_RESPONSE_SUMMON_SETAUTOHEAL,

	GAMEMSG_REQUEST_TRADEMERCHANTBUYSUMMONINFO,	//[광명] 소환수 팔기 정보 얻기
	GAMEMSG_RESPONSE_TRADEMERCHANTBUYSUMMONINFO,	

	GAMEMSG_REQUEST_SUMMONTRADE,				//[광명] 소환수 팔기
	GAMEMSG_RESPONSE_SUMMONTRADE,

	GAMEMSG_REQUEST_SELECT_SUMMONCHAR,			//[준엽] 소환수를 캐릭터로 선택
	GAMEMSG_RESPONSE_SELECT_SUMMONCHAR,

	GAMEMSG_REQUEST_SUMMON_TRANSFORM,			// 소환수 변신.
	GAMEMSG_RESPONSE_SUMMON_TRANSFORM,
	//-------------------------	
	//-------------------------
	// 마을 
	//-------------------------
	GAMEMSG_REQUEST_VILLAGEDETAILINFO = 7200,	//[태곤] 마을 상세 정보의 요청. 
	GAMEMSG_RESPONSE_VILLAGEDETAILINFO,			//[태곤]

	GAMEMSG_REQUEST_GETVILLAGEINFO,				//[제완] 마을 정보 ( 선전포고, 부모마을 )
	GAMEMSG_RESPONSE_GETVILLAGEINFO,			//[제완]	

	GAMEMSG_REQUEST_GETNOWVILLAGEINFO,			//[영진] 특정 시간에 일괄 업데이트되는 당일 공성전 정보를 클라이언트로 보내줌. 
	GAMEMSG_RESPONSE_GETNOWVILLAGEINFO,

	GAMEMSG_REQUEST_RESIDENTSLIST,				//[상민] 마을 주민중 접속해 있는 주민 정보 요청
	GAMEMSG_RESPONSE_RESIDENTSLIST,				//[상민]

	GAMEMSG_RESPONSE_RESIDENTLOGIN,				//[상민] 마을 주민이 접속했음을 주민들에게 알려줌.

	GAMEMSG_RESPONSE_SETGAMEDELAY,				//[태곤] 게임의 속도를 클라이언트로 보낸다. 

	GAMEMSG_RESPONSE_PERSONITEMINFO,			//[태곤] Person의 한개 아이템 정보를 클라이언트로 보낸다. 

	GAMEMSG_RESPONSE_PERSONENCHANTITEMINFO,		//[] Person의 한개 아이템 정보를 클라이언트로 보낸다. 

	GAMEMSG_RESPONSE_PERSONALLSHAPEINFO,		//[태곤] 다른 사용자의 전체 복장 정보를 통보한다. 

	GAMEMSG_RESPONSE_PERSONMAKEITEMSWITCH,		//[광명] 다른 사용자의 제조 상태를 통보

	GAMEMSG_REQUEST_INVREPAIR,					//[태곤] 사용자가 갖고 있는 아이템을 수리하도록 요청한다. 
	GAMEMSG_RESPONSE_INVREPAIR,					//[태곤] 수리된 결과를 통보한다. 


	GAMEMSG_RESPONSE_WEATHER,					//[태곤] 날씨를 통보한다. 

	GAMEMSG_RESPONSE_PERSONHORSE,				//[태곤] Person의 Horse 정보를 보낸다. (한필의 정보.) 

	GAMEMSG_RESPONSE_NEIGHBOURHORSE,			//[태곤] Person의 Horse 정보를 이웃에게 보낸다. 

	GAMEMSG_RESPONSE_LOWPRICESTOCKORDER,		//[태곤] 객주에 올라온 주문중 가장 낮은 가격의 물품 정보를 보낸다. 

	GAMEMSG_REQUEST_READY,						//[태곤] 클라이언트가 준비되었음을 통보. 

	GAMEMSG_RESPONSE_LEVELUP,					//[태곤] 레벨업 되었음을 통보한다. 

	GAMEMSG_RESPONSE_LEVELUPNOTICE,				//[희영] 레벨업 되었음을 통보한다. 채팅창에 같은 길드 및 마을일경우 표시한다. 

	GAMEMSG_REQUEST_INV_CANCELLATIONSEALINGUP,	//[준엽] 가지고 있는 아이템의 밀봉을 해제한다.
	GAMEMSG_RESPONSE_INV_CANCELLATIONSEALINGUP, //[준엽]

	GAMEMSG_REQUEST_REVIVE,						//[연준] 클라이언트에서 서버로 캐릭터 부활시켜 달라고요청
	GAMEMSG_RESPONSE_REVIVE,					//[연준] 서버로 캐릭터 부활시킬 것인지 문의

	GAMEMSG_REQUEST_RESURRECTION,				//[영진] 클라이언트에서 서버로 즉시 부활을 요청

	GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY,		//[영진] 아이템 사용기간 3일 미만 남음 알림.

	GAMEMSG_REQUEST_PRESELLTOFOREIGNMERCHANT,	//[연준] 상인에게 물건팔기전에 기간에 따른 가격변동 요청
	GAMEMSG_RESPONSE_PRESELLTOFOREIGNMERCHANT,	//[연준] 상인에게 물건팔기전에 기간에 따른 가격변동 응답

	GAMEMSG_REQUEST_SITDOWN,					//[상민] 앉기

	GAMEMSG_REQUEST_HOUSECANCELLATIONPENALTYMONEY,//[영진] 시전 해약시 위약금액 요청
	GAMEMSG_RESPONSE_HOUSECANCELLATIONPENALTYMONEY,
	GAMEMSG_REQUEST_HOUSECANCELLATION,			//[영진] 시전 해약
	GAMEMSG_RESPONSE_HOUSECANCELLATION,			

	GAMEMSG_RESPONSE_VILLAGEGENERALMEETING,		//[추가 : 황진성 2008. 2. 21 => 마을 주주총회 발동 관련 정보 통보.]

	GAMEMSG_RESPONSE_CRYSTALITEMINFO, //KHY - 0421 - 결정체 정보를 클라이언트로 보낸다.

	GAMEMSG_REQUEST_BLACKARMYAPPLY = 7300,				//[연준] 흑의군대 이벤트 신청
	GAMEMSG_RESPONSE_BLACKARMYAPPLY,			//[연준] 흑의군대 이벤트 응답

	GAMEMSG_RESPONSE_BLACKARMYWAR,				//[연준] 전쟁메시지

	GAMEMSG_REQUEST_BLACKARMY_RANKINFO,			//[연준] 전쟁 순위 요청
	GAMEMSG_RESPONSE_BLACKARMY_RANKINFO,		//[연준] 전쟁 순위 응답

	GAMEMSG_REQUEST_BLACKARMY_WARPTOGATE,		//[연준] 전쟁맵게이트로 워프 요청

	GAMEMSG_RESPONSE_BLACKARMY_MVPEFFECT,		//[연준] 전쟁 MVP 이펙트 응답

	GAMEMSG_RESPONSE_BLACKWAR,					//[광명] 흑의군단 전쟁메세지

	GAMEMSG_REQUEST_PERSONNAME_SERIALID,		//[진성] 아이템 시리얼번호로 제작자 이름 요청
	GAMEMSG_RESPONSE_PERSONNAME_SERIALID,		//[진성] 아이템 시리얼번호로 제작자 이름 응답

	GAMEMSG_REQUEST_PERSONNAME_FROMPERSONID,	//[연준] 제작자 이름 요청
	GAMEMSG_RESPONSE_PERSONNAME_FROMPERSONID,	//[연준] 제작자 이름 응답

	GAMEMSG_REQUEST_BELONGITEM_DESTROY,			//[연준] 귀속아이템 파괴요청

	GAMEMSG_REQUEST_DISASSEMBLEITEM,			//[연준] 아이템분해 요청
	GAMEMSG_RESPONSE_DISASSEMBLEITEM,			//[연준] 아이템분해 응답
	GAMEMSG_REQUEST_DISASSEMBLEITEM_LIST,		//[연준] 아이템분해 리스트 요청
	GAMEMSG_RESPONSE_DISASSEMBLEITEM_LIST,		//[연준] 아이템분해 리스트 응답
	GAMEMSG_REQUEST_DISASSEMBLEITEM_RECIEVE,	//[연준] 분해된 아이템 수령 요청
	GAMEMSG_RESPONSE_DISASSEMBLEITEM_RECIEVE,	//[연준] 분해된 아이템 수령 응답

	GAMEMSG_RESPONSE_OPENTREASUREBOX,			//[유상] 보물상자 오픈

	GAMEMSG_RESPONSE_KINGGURIINFO,				///[광명] 대왕구리댁 이벤트

	GAMEMSG_RESPONSE_KITERACCOONINFO,			// [영훈] 카이트라쿤 이벤트

	GAMEMSG_RESPONSE_RAREITEMFROMTREASUREBOX,	// [영훈] 랜덤상자에서 레어아이템이 나왔을때(레어 아이템은 소스에 박았다)

	//--------------------------------
	// WorldMoney ( 아큐 )
	//--------------------------------
	GAMEMSG_RESPONSE_SETWORLDMONEY_BUYDLG = 7400 ,		//[준엽] 마스터 명령어로 아큐를 취득하는 박스를 연다

	GAMEMSG_REQUEST_DISTRIBUTION_WORLDMONEY,			//[준엽] 아큐를 시스템에게서 취득한다.
	GAMEMSG_RESPONSE_DISTRIBUTION_WORLDMONEY,

	GAMEMSG_REQUEST_WORLDMONEY_TRADELIST,				//[준엽] 아큐 거래소 리스트를 요청한다.
	GAMEMSG_RESPONSE_WORLDMONEY_TRADELIST,

	GAMEMSG_REQUEST_WORLDMONEY_SELLORDER,				//[준엽] 아큐 거래소에 아큐 팔기를 요청한다.
	GAMEMSG_RESPONSE_WORLDMONEY_SELLORDER,

	GAMEMSG_REQUEST_WORLDMONEY_BUYORDER,				//[준엽] 아큐 거래소에서 아큐 사기를 요청한다.
	GAMEMSG_RESPONSE_WORLDMONEY_BUYORDER,

	GAMEMSG_REQUEST_WORLDMONEY_CANCELORDER,				//[준엽] 아큐 거래소의 주문 등록 취소를 요청한다.
	GAMEMSG_RESPONSE_WORLDMONEY_CANCELORDER,

	GAMEMSG_RESPONSE_WORLDMONEY_SET_CHAR,				//[준엽] 아큐를 업데이트 해줬다.

	//--------------------------------
	// TradeServer ( 무역 )
	//--------------------------------

	GAMEMSG_REQUEST_TRADESERVER_SET_SELLORDER = 7500,	//[준엽] 트레이드 서버에 주문을 올린다.
	GAMEMSG_RESPONSE_TRADESERVER_SET_SELLORDER,

	GAMEMSG_REQUEST_TRADESERVER_CANCEL_SELLORDER,		//[준엽] 트레이드 서버에 주문을 내린다.
	GAMEMSG_RESPONSE_TRADESERVER_CANCEL_SELLORDER,

	GAMEMSG_REQUEST_TRADESERVER_SET_BUYORDER,			//[준엽] 트레이드 서버에 주문을 산다.
	GAMEMSG_RESPONSE_TRADESERVER_SET_BUYORDER,

	GAMEMSG_REQUEST_TRADESERVER_GET_LISTINFO,			//[준엽] 트레이드 서버에 리스트를 요청한다.
	GAMEMSG_RESPONSE_TRADESERVER_GET_LISTINFO,

	GAMEMSG_RESPONSE_TRADESERVER_GET_SELLINFO,			//[준엽] 트레이드 서버에서 팔린 물건에 대한 정보를 받았다.

	GAMEMSG_REQUEST_TRADESERVER_GET_MYINFO,				//[준엽] 트레이드 서버에 올라간 내 물건의 정보를 요청한다.
	GAMEMSG_RESPONSE_TRADESERVER_GET_MYINFO,

	GAMEMSG_REQUEST_TRADESERVER_GET_SAVEDACU_LIST,		//[영훈] 무역서버에 저장된 아큐/최신거래내역 요청
	GAMEMSG_RESPONSE_TRADESERVER_GET_SAVEDACU_LIST,

	GAMEMSG_REQUEST_TRADESERVER_GET_WITHDRAW_ACU,		//[영훈] 무역서버에 저장된 아큐 받기 요청
	GAMEMSG_RESPONSE_TRADESERVER_GET_WITHDRAW_ACU,

	//--------------------------------
	// WorldPassport ( 무역 여권 )
	//--------------------------------
	GAMEMSG_REQUEST_WORLDPASSPORT_SET = 7550,			//[영훈] 무역관련 여권 발급
	GAMEMSG_RESPONSE_WORLDPASSPORT_SET,

	GAMEMSG_REQUEST_WORLDPASSPORT_EXTEND,				//[영훈] 무역관련 여권 연장
	GAMEMSG_RESPONSE_WORLDPASSPORT_EXTEND,

	//--------------------------------
	// 기타
	//--------------------------------

	GAMEMSG_RESPONSE_COUNTRYINFO = 7600,				//[유상] 국가 정보 받기.
	GAMEMSG_REQUEST_SETCOUNTRYINFO,					    //[유상] 국가 정보 설정 요청
	GAMEMSG_RESPONSE_SETCOUNTRYINFO,					//[유상] 국가 정보 설정 응답 - 국가 유니크 설
	GAMEMSG_REQUEST_SETSHOWCOUNTRY,
	GAMEMSG_RESPONSE_SETSHOWCOUNTRY,					//[유상] 국가 정보 설정 응답 - 보여주기 유무 

	GAMEMSG_REQUEST_SETSHOWPUCHIKOMI,					//[영진] 자신의 푸치코미를 다른유저에게 보여주나
	GAMEMSG_RESPONSE_SETSHOWPUCHIKOMI,					//[영진] 자신의 푸치코미를 다른유저에게 보여주나

	GAMEMSG_REQUEST_USERLISTBYCOUNTRY,					//[유상] 국가별 유저 리스트 요청
	GAMEMSG_RESPONSE_USERLISTBYCOUNTRY,			

	GAMEMSG_RESPONSE_NOTGAMEPOTAUTH,					//[연준] 게임팟 인증이 안된 유저이면

	GAMEMSG_REQUEST_ADDCHINESEUSER,						//[유상] 중국언어 사용자 통계
	GAMEMSG_RESPONSE_NOTICECOUNTRYUSERMAKE,				//[유상] 국가 입력시 소속국가가 같은 유저에게 메세지 보내기

	GAMEMSG_REQUEST_LOWLEVELLIST,						//[유상] 20미만 레벨 유저 요청
	GAMEMSG_RESPONSE_LOWLEVELLIST,						//[유상] 20미만 레벨 유저 통보

	GAMEMSG_RESPONSE_ADDBUF,							//[유상] 버프를 추가한다.
	GAMEMSG_RESPONSE_DELETEBUF,							//[유상] 버프를 삭제한다.

	GAMEMSG_RESPONSE_SETTEACHERPOINT,					//[광명] 스승점수를 업데이트 한다.

	GAMEMSG_RESPONSE_SETITEMTYPEINFO,					//[광명] 현재 내 세트 아이템 정보를 클라이언트로 알려준다
	GAMEMSG_REQUEST_SETEMOTICON,						//[유상] 이모티콘
	GAMEMSG_RESPONSE_SETEMOTICON,						//[유상] 

	GAMEMSG_RESPONSE_CANNOTLOGIN_AGE,					//[강형] 나이제한 때문에 로그인 할 수 없다.
	GAMEMSG_RESPONSE_GAMEDISCONNECT_AGE,				//[춘기] 게임실행중에 나이제한 체크를 계속 한다.
	GAMEMSG_RESPONSE_CANNOTLOGIN_ERROR,					//[강형] 한게임 인증 모듈 오류

	//--------------------------------
	// 모후모후 이벤트
	//--------------------------------

	GAMEMSG_REQUEST_MOFUMOFUEVENT_RESERVE = 7700,		//[강형] 모후모후 이벤트 예약
	GAMEMSG_RESPONSE_MOFUMOFUEVENT_RESERVE,				//[강형]

	GAMEMSG_REQUEST_MOFUMOFUEVENT_RESERVELIST,			//[강형] 모후모후 이벤트 예약 리스트
	GAMEMSG_RESPONSE_MOFUMOFUEVENT_RESERVELIST,			//[강형]

	GAMEMSG_RESPONSE_MOFUMOFUEVENT_START,				//[강형] 모후모후 이벤트 시작
	GAMEMSG_RESPONSE_MOFUMOFUEVENT_END,					//[강형] 모후모후 이벤트 종료

	GAMEMSG_REQUEST_MOFUMOFUEVENT_ENTERFIELD,			//[강형] 모후모후 이벤트 필드 입장 요청
	GAMEMSG_REQUEST_MOFUMOFUEVENT_WARP,					//[강형] 모후모후 이벤트 던젼 입구 워프 요청

	GAMEMSG_RESPONSE_MOFUMOFUEVENT_SETINFO,				//[강형] 모후모후 이벤트 필드 설정
	GAMEMSG_RESPONSE_MOFUMOFUEVENT_INFO,				//[강형] 모후모후 이벤트 정보
	GAMEMSG_RESPONSE_MOFUMOFUEVENT_MAMAMOFUMOFU,		//[강형] 마마 모후모후 설정

	//-------------------------------
	//마을 이벤트 - 몬스터 습격. 
	//-------------------------------
	GAMEMSG_RESPONSE_MONSTER_ATTACK_START,				//[진성] 몬스터 습격이 진행중임.
	GAMEMSG_RESPONSE_MONSTER_ATTACK_REWARD,				//[지연] 몬스터 습격 이벤트의 보상 내역 정보 통보용

	//-------------------------------
	// 명칭 퀘스트
	//-------------------------------
	GAMEMSG_REQUEST_NAMINGQUEST_START = 7800,			//[유상] 명칭 퀘스트 시작

	GAMEMSG_RESPONSE_SETPERSONNAMINGINFO,				//[유상] 유저가 가지고 있는 명칭정보

	GAMEMSG_REQUEST_SETPERSONCURRENTNAMING,				//[유상] 유저가 현재 선택한 명칭
	GAMEMSG_RESPONSE_SETPPERSONCURRENTNAMING,			//'유상]

	//-------------------------------
	// 보상 NPC
	//-------------------------------
	GAMEMSG_REQUEST_SYSTEMREWARDLIST	= 7810,			//[유상] 이벤트 보상 목록 요청
	GAMEMSG_RESPONSE_SYSTEMREWARDLIST,

	GAMEMSG_REQUEST_RECIEVESYSTEMREWARD,				//[유상] 보상 받기
	GAMEMSG_RESPONSE_RECIEVESYSTEMREWARD,

	//출석체크
	GAMEMSG_RESPONSE_DEMANDATTEND	= 7820,				//[유상] 출석 체크를 요청한다 ( 서버->클라이언트 )

	GAMEMSG_REQUEST_ATTEND,								//[유상] 출석체크
	GAMEMSG_RESPONSE_ATTEND,							//[유상]

	GAMEMSG_REQUEST_ATTENDREWARDITEM,					//[영훈] 출석체크 보상 아이템



	//-------------------------------
	// 해피빈 퀘스트
	//-------------------------------
	//KHY - 0822 - 해피빈 퀘스트 추가.

	GAMEMSG_REQUEST_HAPPYBEANQUEST_START = 7900,		//퀘스트 시작
	GAMEMSG_RESPONSE_HAPPYBEANQUEST_START_NO,			// 퀘스트 불가.
	GAMEMSG_RESPONSE_HAPPYBEANQUEST_START_OK,			// 퀘스트 허가.
	GAMEMSG_RESPONSE_HAPPYBEANQUEST_REWARD,				//퀘스트 보상을 알린다.

	// [지연] 군주데이 미션
	GAMEMSG_RESPONSE_GOONZUDAYMISSION_CLEARREWARD,		// 미션 완료에 대한 보상을 알림

	//-------------------------------
	// 게임방 이벤트 쿠폰.
	//-------------------------------
	//KHY - 0906 - 게임방 이벤트 쿠폰.
	GAMEMSG_RESPONSE_EVENTCOUPON, 						//이벤트 쿠폰에 대한 결과를 알린다.
	////-------------------------------
	//// 옥셕이벤트 2007.09.10 손성웅
	////-------------------------------
	////KHY - 0906 - 옥션 이벤트 쿠폰.
	//GAMEMSG_RESPONSE_AUCTIONEVENTCOUPON, 				//이벤트 쿠폰에 대한 결과를 알린다.

	GAMEMSG_RESPONSE_GAME_GUARD_AUTH,					//[영진] 서버에서 클라이언트로 게임 가드 인증을 요청한다
	GAMEMSG_REQUEST_GAME_GUARD_AUTH,	

	GAMEMSG_RESPONSE_SANTARACCOONINFO,					//[추가 : 황진성 2007. 11. 29 산타라쿤 이벤트 메시지.]

	GAMEMSG_REQUEST_NEWYEAR_CLICK,						//[추가 : 황진성 2007. 12. 11 신년 버튼을 클릭 햇다는 메시지.	C->S]
	GAMEMSG_RESPONSE_NEWYEAR_BTN,						//[추가 : 황진성 2007. 12. 11 신년 버튼 띄우라는 메시지.		S->C]

	GAMEMSG_REQUEST_MISSIONQUEST_START,					// 미션 퀘스트 설정
	GAMEMSG_RESPONSE_MISSIONQUEST_START,				//

	GAMEMSG_REQUEST_MISSIONQUEST_ACCEPT,				// 미션 퀘스트 진행
	GAMEMSG_REQUEST_MISSIONQUEST_CANCEL,				// 미션 퀘스트 취소

	/// 제국
	GAMEMSG_REQUEST_CREATEEMPIRE = 7950,				//[광명] 제국을 만든다
	GAMEMSG_RESPONSE_CREATEEMPIRE,

	GAMEMSG_REQUEST_DESTROYEMPIRE,						//[광명] 제국해체
	GAMEMSG_RESPONSE_DESTROYEMPIRE,

	GAMEMSG_REQUEST_INVITEEMPIRE,						//[광명] 제국으로 초대한다.
	GAMEMSG_RESPONSE_INVITEEMPIRE,

	GAMEMSG_REQUEST_SECEDEEMPIRE,						//[광명] 제국에서 탈퇴한다.
	GAMEMSG_RESPONSE_SECEDEEMPIRE,

	GAMEMSG_RESPONSE_UPDATEEMPIREINFO,					//[광명] 자기 자신의 제국 정보 업데이트

	GAMEMSG_REQUEST_EMPIREINFO,							//[광명] 제국의 정보를 요청한다.
	GAMEMSG_RESPONSE_EMPIREINFO,

	GAMEMSG_REQUEST_JOINEMPIRE,							//[광명] 제국에 가입한다.
	GAMEMSG_RESPONSE_JOINEMPIRE,

	GAMEMSG_REQUEST_SETEMPIREMONEY,						//[광명] 제국 자본금을 넣는다.
	GAMEMSG_RESPONSE_SETEMPIREMONEY,

	GAMEMSG_RESPONSE_AUTOMACRO_CHECK,					//[추가 : 황진성 2007. 12. 3  => 서버에서에서 클라이언트로 오토프로그램 검사하라고.]
	GAMEMSG_REQUEST_AUTOMACRO_ON,						//[추가 : 황진성 2007. 12. 5  => 서버에게 클라이언트가 오토프로그램 사용 중이라고 전달.]
	GAMEMSG_RESPONSE_AUTOMACRO_ON_OTHER,				//[추가 : 황진성 2007. 12. 10 => 서버에서에서 클라이언트로 오토프로그램을 사용하는 유저를 공지.]

	//보상아이템을 줄때 구분 

	//-------------------------------
	//KHY - 1220 - 변신시스템.
	//-------------------------------
	GAMEMSG_REQUEST_TRANSFORM = 8000,			//[희영 ] 캐릭터를 변신 시킨다.
	GAMEMSG_RESPONSE_TRANSFORM,

	GAMEMSG_REQUEST_TRANSFORM_END,			//[희영 ] 캐릭터 변신을 해제 시킨다.
	//GAMEMSG_RESPONSE_TRANSFORM_END,

	GAMEMSG_REQUEST_TRANSFORM_USEITEM ,			//[희영 ] 변신에 관련된 아이템을 사용 한다. - 캐릭터 등록, 시간제 아이템 사용.
	GAMEMSG_RESPONSE_TRANSFORM_USEITEM ,			

	GAMEMSG_RESPONSE_MAXSYNCRO_ATTAINMENT,		// [강형] 싱크로율 100% 달성

	GAMEMSG_RESPONSE_TRANSFORM_LIMIT_TIME,		//[추가 : 황진성 2008. 1. 9 => 변신한 시간이 30초가 안되서 해제 실패 메시지 띄우게.]

	//-------------------------------
	// 종량제 서비스 정보 - by LEEKH 2007.12.28
	//-------------------------------
	GAMEMSG_RESPONSE_TIMEMETERRATE_UPDATE = 8050,		//[강형] 종량제 서비스 업데이트

	//---------------------------------------------------------------------------------
	//명예의 전당 리스트
	//--------------------------------------------------------------------------------
	GAMEMSG_REQUEST_HOFLIST,					//[성웅]명예의 전당 리스트 패이지.
	GAMEMSG_RESPONSE_HOFLIST, 

	GAMEMSG_REQUEST_RAKINGLIST,								//[성웅]랭킹리스트
	GAMEMSG_RESPONSE_RAKINGLIST, 

	//-------------------------------
	// 추가 : 황진성 2008. 1. 9 => 인스턴스던젼 메시지.  
	//-------------------------------
	GAMEMSG_REQUEST_INSTANTSDUNGEON_STATUS,			
	GAMEMSG_RESPONSE_INSTANTSDUNGEON_STATUS,			
	GAMEMSG_RESPONSE_INSTANTSDUNGEON_RUN_STATUS,
	GAMEMSG_RESPONSE_INSTANTMAP_GATEINFO,					//[강형] 인스턴스 맵 게이트 생성정보
	GAMEMSG_RESPONSE_INSTANTSDUNGEON_REWARD,
	GAMEMSG_RESPONSE_INSTANTSDUNGEON_TOP_CHANGE,

	//--------------------------------------------------------------
	// 추가 : 황진성 2008. 1. 23 => 흑의 군단 개편 메시지.  
	//--------------------------------------------------------------
	GAMEMSG_RESPONSE_BLACK_WAR_REFORM_CHAR_RESULT,			//흑의 군단 개편 이벤트 종료 후 결과를 알릴 메시지.  
	GAMEMSG_RESPONSE_BLACK_WAR_REFORM_VILLAGE_RESULT,

	GAMEMSG_REQUEST_FILECRCCHECK,							// [강형] 파일 보안 체크
	GAMEMSG_RESPONSE_FILECRCCHECK,							// 서버에서 파일을 체크하라는 명령을 보낸다

	//--------------------------------------------------------------
	// 추가 : 황진성 2008. 2. 28 => 아티펙트 관련 메시지. C < - > S
	//--------------------------------------------------------------
	GAMEMSG_REQUEST_ARTIFACT_MIXTURE,						// 결정체 조합.
	GAMEMSG_RESPONSE_ARTIFACT_MIXTURE,	

	GAMEMSG_REQUEST_ARTIFACT_REMOVE,						// 결정체 제거.
	GAMEMSG_RESPONSE_ARTIFACT_REMOVE,

	GAMEMSG_REQUEST_ARTIFACT_SLOT_CREATE,					// 슬롯 생성.
	GAMEMSG_RESPONSE_ARTIFACT_SLOT_CREATE,


	GAMEMSG_REQUEST_PIRATESHIP_ENTERFIELD,					// [영훈] 해적선 : 해적선으로 입장이 가능한지 물어봄

	GAMEMSG_RESPONSE_INSTANTDUNGEON_PARTY_CLEAR,			// [영훈] 해적선 : 클리어 후 보상을 받음

	GAMEMSG_RESPONSE_PIRATESHIP_APPEAR,						// [영훈] 해적선 : 해적선이 나타났다는 메시지

	//-------------------------
	// Daily 퀘스트2
	//-------------------------	
	GAMEMSG_REQUEST_DAILYQUEST2LIST,			//퀘스트 리스트 요청
	GAMEMSG_RESPONSE_DAILYQUEST2LIST,		//

	GAMEMSG_REQUEST_DAILYQUEST2SELECT,		// 퀘스트 선택
	GAMEMSG_RESPONSE_DAILYQUEST2SELECT,		//

	GAMEMSG_REQUEST_DAILYQUEST2SITUATION,	// 퀘스트 상황
	GAMEMSG_RESPONSE_DAILYQUEST2SITUATION,	//

	GAMEMSG_RESPONSE_DAILYQUEST2CHANGE,		// 퀘스트 남은 시간과 몬스터 잡은수를 업데이트 해줌.

	GAMEMSG_REQUEST_DAILYQUEST2END,			// 퀘스트 종료
	GAMEMSG_RESPONSE_DAILYQUEST2END,			//

	//-------------------------
	// Daily 퀘스트3
	//-------------------------	
	GAMEMSG_REQUEST_DAILYQUEST3LIST,			//퀘스트 리스트 요청
	GAMEMSG_RESPONSE_DAILYQUEST3LIST,		//

	GAMEMSG_REQUEST_DAILYQUEST3SELECT,		// 퀘스트 선택
	GAMEMSG_RESPONSE_DAILYQUEST3SELECT,		//

	GAMEMSG_REQUEST_DAILYQUEST3SITUATION,	// 퀘스트 상황
	GAMEMSG_RESPONSE_DAILYQUEST3SITUATION,	//

	GAMEMSG_RESPONSE_DAILYQUEST3CHANGE,		// 퀘스트 남은 시간과 몬스터 잡은수를 업데이트 해줌.

	GAMEMSG_REQUEST_DAILYQUEST3END,			// 퀘스트 종료
	GAMEMSG_RESPONSE_DAILYQUEST3END,			//

	GAMEMSG_RESPONSE_HEALTH_CHANGE,			//[진성] 건강정보를 변경 시킨다. => 2008-4-6

	GAMEMSG_REQUEST_INV_CHANGITEMFORMNPC,	// 연금술사로부터 아이템 변경을 요청한다.
	GAMEMSG_RESPONSE_INV_CHANGITEMFORMNPC,	

	// 케릭터 옵션 셋팅
	GAMEMSG_REQUEST_CHAR_OPTION,		//[성웅] 사용자의 옵션을 설정한다.
	GAMEMSG_RESPONSE_CHAR_OPTION,		//[성웅]

	// 케릭터 옵션 요청
	GAMEMSG_RESPONSE_CHAR_GET_OPTION,	//[성웅] 케릭터 옵션 요청

	// [진성] 영웅의 동상 이벤트 버프룰렛 돌릴때 사용하는 패킷. => 2008-6-10
	GAMEMSG_REQUEST_BUFF_ROULETTE,		
	GAMEMSG_RESPONSE_BUFF_ROULETTE,		

	//	[종호_BALANCECURRENCY]  protocol
	GAMEMSG_REQUEST_BALANCECURRENCY_BUFF,		
	GAMEMSG_RESPONSE_BALANCECURRENCY_BUFF,		


	// [진성] 서버 이벤트 => 2008-6-19 
	GAMEMSG_REQUEST_SERVER_EVENT_RANK_INFO,					// [진성] 서버, 개인 랭킹정보 요청.			=> 2008-6-20
	GAMEMSG_RESPONSE_SERVER_EVENT_RANK_INFO,				// [진성] 서버, 개인 랭킹정보 받기.			=> 2008-6-20
	GAMEMSG_RESPONSE_SERVER_EVENT_SERVER_INFO,				// [진성] 서버 랭킹정보 받기.				=> 2008-6-20
	GAMEMSG_RESPONSE_SERVER_DAILY_EVENT_SERVER_INFO,		// [진성] 하루 서버 랭킹정보 받기.			=> 2008-6-20
	GAMEMSG_RESPONSE_SERVER_EVENT_PERSONAL_INFO,			// [진성] 개인 랭킹정보 받기.				=> 2008-6-20
	GAMEMSG_REQUEST_SERVER_EVENT_BONUS_STAGE,				// [진성] 서버 이벤트 종료 후 1등 서버에 생기는 차원의 문 입장.=> 2008-6-24
	GAMEMSG_RESPONSE_SERVER_EVENT_INSTANCEMAP_BONUS_FIGURE,	// [진성] 서버 이벤트 인던 종료 후 각 인던의 단계에 따른 조합이 맞는경우 보너스 알림 => 2008-6-27

	GAMEMSG_REQUEST_SERVER_EVENT_QUEST_START,				// [영훈] 서버 이벤트 퀘스트 시작 정보를 요청함
	GAMEMSG_RESPONSE_SERVER_EVENT_QUEST_START,				// [영훈] 서버 이벤트 퀘스트 시작 정보를 받음
	GAMEMSG_RESPONSE_SERVER_EVENT_QUEST_RUNINFO,			// [영훈] 서버 이벤트 퀘스트 진행 정보를 받음
	GAMEMSG_RESPONSE_SERVER_EVENT_QUEST_CLEARSTATUS,		// [영훈] 서버 이벤트 퀘스트 클리어 상태 정보를 받음

	GAMEMSG_REQUEST_SERVER_EVENT_LOBBY_ENTERFIELD,			// [영훈] 서버 이벤트 인던 로비 입장 요청

	GAMEMSG_RESPONSE_SERVER_EVENT_INSTANCEDUNGEON_RUNINFO,	// [영훈] 서버 이벤트 퀘스트 클리어 상태 정보를 받음

	//GAMEMSG_RESPONSE_TANGWANGGIRL,							// [성웅] 2차 가차
	GAMEMSG_REQUEST_SUMMON_SPTCHARGE,						// [춘기] 소환수 스태미너 충전
	GAMEMSG_RESPONSE_SUMMON_STPCHARGE,

	GAMEMSG_REQUEST_SUMMONUSTON,							//[성웅] 소환수에 무기 장착석을 박는다.
	GAMEMSG_RESPONSE_SUMMONUSTON,

	GAMEMSG_REQUEST_GUILD_NOTCONNECT_CHANGEMASTER,			//[성웅] 한달이상 길드장이 접속 안했을때 길드장 위임
	GAMEMSG_RESPONSE_GUILD_NOTCONNECT_CHANGEMASTER,			

	GAMEMSG_REQUEST_BASESUMMONINFO,							//[제형] 소환수 정보를 요청한다.
	GAMEMSG_RESPONSE_BASESUMMONINFO,

	GAMEMSG_REQUEST_SUMMONSLOTIN,							//[성웅] Base의 소환수를 SLot으로 넣는다.
	GAMEMSG_RESPONSE_SUMMONSLOTIN,

	GAMEMSG_REQUEST_SUMMONSLOTOUT,							//[성웅] SLot에서 해당 소환수를빼낸다 
	GAMEMSG_RESPONSE_SUMMONSLOTOUT,


	//-------------------------------
	// 추가 : 시스템 메시지
	//-------------------------------
	GAMEMSG_REQUEST_EVENTTIME_UPDATE,						//[강형] EventTime 정보 업데이트

	GAMEMSG_REQUEST_CHECKPROCESS,							// [영훈] 프로세스 모듈이 몇개 실행되는지 보냄
	GAMEMSG_RESPONSE_CHECKPROCESS,							// [영훈] 프로세스 모듈이 몇개 실행되는지 요청함

	GAMEMSG_RESPONSE_CHECKPROCESS_RESULT,					// [영훈] 프로세스 모듈 실행 결과를 보냄(운영자에게)

	//-----------------------------------
	//[진성] 호박상자 => 2008-9-16
	//-----------------------------------
	GAMEMSG_RESPONSE_AMBER_BOX_LIST,						// [진성] 호박상자 리스트 주기 => 2008-9-16

	GAMEMSG_REQUEST_AMBER_BOX_SELECT_ITEM_LIST,				// [진성] 호박상자 선택 아이템 리스트 요청 => 2008-9-16
	GAMEMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_LIST,			// [진성] 호박상자 선택 아이템 리스트 주기 => 2008-9-16

	GAMEMSG_REQUEST_AMBER_BOX_SELECT_ITEM_ADD,				// [진성] 호박상자 선택 아이템 추가. => 2008-9-16
	//GAMEMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_LIST,			// [진성] 호박상자 선택 아이템 리스트 주기 => 2008-9-16

	GAMEMSG_REQUEST_AMBER_BOX_SELECT_ITEM_GET,				// [진성] 호박상자 리스트 주기 => 2008-9-16
	GAMEMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_GET,				// [진성] 
	//-----------------------------------
	GAMEMSG_RESPONSE_NOTICE_QUEST,							//	[ 성웅 ] 호박퀘스트 할때 퀘스트를 전체 공지 하기 위해서

	GAMEMSG_REQUEST_CHAR_ROULETTE,							// [강형] 캐릭터 룰렛
	GAMEMSG_RESPONSE_CHAR_ROULETTE,		

	//---------------------------------------
	//	일반맵에서 개인간의 PVP
	//---------------------------------------
	GAMEMSG_REQUEST_PVP_FILED,							// [성웅] 일반 필드에서의 개인간의 pvp 신청,허락,거절// NEW PVP - 손성웅 2008.10.08
	GAMEMSG_RESPONSE_PVP_FILED,

	GAMEMSG_RESPONSE_PVP_LEAGUE_INFOGET,				// [영훈] PVP 무한대전 개인 정보

	GAMEMSG_REQUEST_PVP_LEAGUE_UI_INFOGET,				// [영훈] PVP 무한대전 다이얼로그 정보요청
	GAMEMSG_RESPONSE_PVP_LEAGUE_UI_INFOGET,

	GAMEMSG_REQUEST_PVP_LEAGUE_APPLY,					// [영훈] PVP 무한대전 참가신청

	GAMEMSG_REQUEST_PVP_LEAGUE_CANCEL,					// [영훈] PVP 무한대전 참가취소

	GAMEMSG_REQUEST_PVP_LEAGUE_AGAIN_RESERVE,			// [영훈] PVP 무한대전 경기에 매칭되지 않아서 다음 경기에 예약을 할것인가?
	GAMEMSG_RESPONSE_PVP_LEAGUE_BATTLE_CHANGEGRADE,		// [영훈] PVP 무한대전 등급이 변경 되었다




	//[진성] 스승, 제자
	GAMEMSG_REQUEST_VILLAGE_RESIDENT_INVITE,			//[진성] 마을 주민 초대하기. => 2008-10-8
	GAMEMSG_RESPONSE_VILLAGE_RESIDENT_INVITE,


	//[진성] 도우미 2008-11-05
	GAMEMSG_REQUEST_QUESTION_LIST_GET,
	GAMEMSG_RESPONSE_QUESTION_LIST_GET,  
	GAMEMSG_REQUEST_QUESTION_CONTENTS_GET,
	GAMEMSG_RESPONSE_QUESTION_CONTENTS_GET,  
	GAMEMSG_REQUEST_CHECK_QUESTION,
	GAMEMSG_REQUEST_ADD_QUESTION,
	GAMEMSG_RESPONSE_ADD_QUESTION,  
	GAMEMSG_REQUEST_ADD_ANSWER,
	GAMEMSG_RESPONSE_ADD_ANSWER,  
	GAMEMSG_REQUEST_ADD_ANSWER_COMPLETE,
	GAMEMSG_RESPONSE_ADD_ANSWER_COMPLETE,  
	GAMEMSG_REQUEST_ADD_ANSWER_COMPLETE_RESULT,
	GAMEMSG_RESPONSE_ADD_ANSWER_COMPLETE_RESULT,  
	GAMEMSG_REQUEST_DEL_QUESTION,  
	GAMEMSG_REQUEST_DEL_ANSWER,

	GAMEMSG_RESPONSE_SETMUTE,							//[강형]채팅블록리스트
	GAMEMSG_RESPONSE_MUTELIST,							//[강형]채팅블록리스트

	GAMEMSG_REQUEST_DIVINATION_ROULETTE,				//[춘기] 신년운세룰렛
	GAMEMSG_RESPONSE_DIVINATION_ROULETTE,				//[춘기] 신년운세룰렛

	//KHY - 1113 - 크리스마스 이벤트 -  사냥터별 분리 입장.
	GAMEMSG_REQUEST_CHRISTMASEVET_ENTERFIELD, 

	//=======================================================================
	//[진성] 엘릭서.
	//-----------------------------------------------------------------------
	GAMEMSG_REQUEST_ELIXIR_NPC_ITEM_TYPE_LIST_GET,		// 해당 아이템 타입에 관련된 정보 요청. (아이템의 유니크, 마력의 정수 개수)
	GAMEMSG_RESPONSE_ELIXIR_NPC_ITEM_TYPE_LIST_GET,

	GAMEMSG_REQUEST_ELIXIR_NPC_CHANGE_ITEM,				// 엘릭서 NPC에게 장비를 마력의 비약으로 아이템 교환.
	GAMEMSG_RESPONSE_ELIXIR_NPC_CHANGE_ITEM,

	GAMEMSG_REQUEST_ELIXIR_USE_START,
	GAMEMSG_RESPONSE_ELIXIR_USE_START,

	GAMEMSG_REQUEST_ELIXIR_SET,							// 엘릭서 세팅.
	GAMEMSG_RESPONSE_ELIXIR_SET,
	//=======================================================================

	// 발렌타인데이 전쟁 정보 
	GAMEMSG_REQUEST_VALENTINE_INFO_GET,
	GAMEMSG_RESPONSE_VALENTINE_INFO_GET,

	GAMEMSG_REQUEST_VALENTINE_REQUEST,
	GAMEMSG_RESPONSE_VALENTINE_REQUEST,

	GAMEMSG_RESPONSE_VALENTINE_RUSOLO_RUCOUPLE_ONTIME,	// 12시다 발렌타인데이에 솔로인지나 물어보자~

	GAMEMSG_RESPONSE_VALENTINE_OPENDLG,					//발렌타인데이 정보 창을 열어라

	GAMEMSG_REQUEST_NEWSELECTBOX_SELECT,				// [영훈] 새로운 선택 상자에서 12가지중 하나를 선택한다
	GAMEMSG_RESPONSE_NEWSELECTBOX_SELECT,

	GAMEMSG_REQUEST_NEWSELECTBOX_GETITEM,				// [영훈] 새로운 선택 상자에서 12가지중 하나를 선택한다
	GAMEMSG_RESPONSE_NEWSELECTBOX_GETITEM,
	GAMEMSG_RESPONSE_VALENTINE_PRIZEGET,				// 발렌타인데이 상품을 받았다.


	//=======================================================================
	// [진성] 통합마켓.
	//-----------------------------------------------------------------------
	GAMEMSG_REQUEST_ITEM_LIST,							// 통합마켓의 해당 아이템 주문 정보 얻기.
	GAMEMSG_RESPONSE_ITEM_LIST,							//

	GAMEMSG_REQUEST_SELL_NEWMARKET,						// 통합마켓에 판매 주문 넣기.
	GAMEMSG_RESPONSE_SELL_NEWMARKET,					//

	GAMEMSG_REQUEST_BUY_NEWMARKET,						// 통합마켓에서 구입 하기.
	GAMEMSG_RESPONSE_BUY_NEWMARKET,						//

	GAMEMSG_REQUEST_RECALL_NEWMARKET,					// 통합마켓에서 판매 주문한 아이템 회수하기.
	GAMEMSG_RESPONSE_RECALL_NEWMARKET,					//

	GAMEMSG_REQUEST_NEWMARKETACCOUNT,					//[광명] 통합마켓에 올려진 내 물품 목록 요청
	GAMEMSG_RESPONSE_NEWMARKETACCOUNT,

	GAMEMSG_REQUEST_NEWMARKETITEMLIST,					//[광명] 통합마켓에 살려고 하는 물품 목록 요청
	GAMEMSG_RESPONSE_NEWMARKETITEMLIST,

	GAMEMSG_REQUEST_GETFROMNEWMARKET,					//[광명] 통합마켓에 물건을 회수하라는 요청. 
	GAMEMSG_RESPONSE_GETFROMNEWMARKET,

	GAMEMSG_REQUEST_NEWSELLITEM,						//[광명] 통합마켓에 물품을 팔 것을 요청한다. 
	GAMEMSG_RESPONSE_NEWSELLITEM,

	GAMEMSG_REQUEST_NEWBUYITEM,							//[광명] 통합마켓에 물품을 살 것을 요청한다. 
	GAMEMSG_RESPONSE_NEWBUYITEM,

	GAMEMSG_REQUEST_SETNEWMARKETFEE,					//[광명] 통합마켓에 수수료 설정 요청. 
	GAMEMSG_RESPONSE_SETNEWMARKETFEE,
	//=======================================================================

	//=======================================================================
	GAMEMSG_REQUEST_GUILDHUNTMAPWAR_MAPREGISTER,				//[성웅]  길드 사냥터 도전 맵등록
	GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_MAPREGISTER,

	GAMEMSG_REQUEST_GUILDHUNTMAPWAR_WARREGISTER,				//[성웅]  길드 사냥터 길드전 신청
	GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_WARREGISTER,
	
	GAMEMSG_REQUEST_GUILDHUNTMAPWAR_REQUEST_WARP,				//[성웅]  길드 사냥터 길드전워프 할것인가 질문 
	GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_REQUEST_WARP,
	
	GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_START,						//[성웅] 길드전 시작 알림
	
	GAMEMSG_REQUEST_GUILDHUNTMAPWAR_GETTOP10,					//[성웅]  길드 사냥터 길드전 맵별 top10
	GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_GETTOP10,

	GAMEMSG_REQUEST_GUILDHUNTMAPWAR_WAREND,					//[성웅]  길드 사냥터 길드전 종료
	GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_WAREND,
	
	GAMEMSG_REQUEST_GUILDHUNTMAPWAR_ADD_KILLPOINT,					//[성웅]  길드 사냥터 킬포인트 추가
	GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_ADD_KILLPOINT,

	GAMEMSG_REQUEST_GUILDHUNTMAPWAR_MAPUSERLIST,
	GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_MAPUSERLIST,
	
	GAMEMSG_REQUEST_GUILDHUNTMAPWAR_SERVERTIME,					//[성웅]  길드 사냥터 도전 서버시간을 알아온다
	GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_SERVERTIME,

	//////////////////////////////////////////////////////////////////////////
	
	GAMEMSG_REQUEST_PROMOTERNPC_GIVE_ITEM_GET,					//[진성] 프로모터 시스템에서 주는 아이템.
	GAMEMSG_RESPONSE_PROMOTERNPC_GIVE_ITEM_GET,

	GAMEMSG_REQUEST_PROMOTER_INFO_TOTAL_GET,					//[진성] 프로모터 관련 전체 정보. (캐릭터와 URL)
	GAMEMSG_RESPONSE_PROMOTER_INFO_TOTAL_GET,

	GAMEMSG_REQUEST_PROMOTER_INFO_GET,							//[진성] 프로모터 캐릭터 정보.
	GAMEMSG_RESPONSE_PROMOTER_INFO_GET,

	GAMEMSG_REQUEST_PROMOTER_URL_GET,							//[진성] 프로모터 URL 정보.
	GAMEMSG_RESPONSE_PROMOTER_URL_GET,							
 
	GAMEMSG_REQUEST_PROMOTER_SET,								//[진성] 프로모터 정보를 세팅.
	GAMEMSG_RESPONSE_PROMOTER_SET,							
	
	GAMEMSG_REQUEST_PROMOTER_WARP,								//[진성] 프로모터가 자신을 추천한 캐릭에게 이동한다

	GAMEMSG_RESPONSE_RECOMMEND_INTRODUCE,						//[진성] 프로모터에게 새내기 캐릭이 자신을 소개한다.
	GAMEMSG_RESPONSE_RECOMMENDEE_INTRODUCE,						//[진성] 새내기에게 프로모터 캐릭이 자신을 소개한다.
	
	//////////////////////////////////////////////////////////////////////////
	

	GAMEMSG_REQUEST_GUILDHUNTMAPWAR_HAVEMAP,					//[성웅] 길드 사냥터신청을 했는가.
	GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_HAVEMAP,
	
	GAMEMSG_REQUEST_GUILDHUNTMAPWAR_SENDMATCHINGRESULT,			//[성웅] 매칭리스트 결과를 각 길드의 길드원들에게 알려준다
	GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_SENDMATCHINGRESULT,

	GAMEMSG_REQUEST_TREASUREEVENT_KILLNPC,						// [성웅] 보물찾기 이벤트 낡은 쇠뭉치를 클릭하고 개를 죽여 버린다.
	GAMEMSG_RESPONSE_TREASUREEVENT_KILLNPC,						//

	GAMEMSG_REQUEST_TREASUREEVENT_USEOKARA,						// [성웅] 보물찾기 이벤트 오카라 10개로 아무짝에 쓸모없는 버프 받기..
	GAMEMSG_RESPONSE_TREASUREEVENT_USEOKARA,	

	// 결혼시스템
	GAMEMSG_REQUEST_WEDDING_RESERVE,							// [영훈] 결혼 예약하기
	GAMEMSG_RESPONSE_WEDDING_RESERVE,

	GAMEMSG_REQUEST_WEDDING_SIMPLELIST_GET,						// [영훈] 결혼 간단 리스트 가져오기
	GAMEMSG_RESPONSE_WEDDING_SIMPLELIST_GET,

	GAMEMSG_REQUEST_WEDDING_QUESTION_ANSWER,					// [영훈] 결혼 관련 질문에 답변한다

	GAMEMSG_REQUEST_WEDDING_LIST_GET,							// [영훈] 결혼 리스트 가져오기
	GAMEMSG_RESPONSE_WEDDING_LIST_GET,

	GAMEMSG_REQUEST_WEDDING_HALL_ENTER,							// [영훈] 예식장 들어가기

	GAMEMSG_REQUEST_WEDDING_RESERVE_CANCEL,						// [영훈] 결혼식 취소하기
	GAMEMSG_RESPONSE_WEDDING_RESERVE_CANCEL,					

	GAMEMSG_REQUEST_MARRIAGEMATE_WARP,							// [영훈] 배우자에게 워프하기	

	GAMEMSG_REQUEST_BALANCECANUSECURRENCY,						// [종호] 사용가능 통화량 요청
	GAMEMSG_RESPONSE_BALANCECANUSECURRENCY,						// [종호] 사용가능 통화량 응답

	GAMEMSG_RESPONSE_GETBALANCECANUSECURRENCY,					// [종호] GM이 요청한 사용 가능 통화량

	GAMEMSG_REQUEST_FAMILY_REQUEST_ADD_CHILDREN,				// [성웅] 가족에 자녀 추가 요청
	GAMEMSG_RESPONSE_FAMILY_REQUEST_ADD_CHILDREN,				// [성웅] 

	GAMEMSG_REQUEST_FAMILY_ADD_FAMILY,							// [성웅] 가족 추가
	GAMEMSG_RESPONSE_FAMILY_ADD_FAMILY,							// [성웅] 

	GAMEMSG_REQUEST_FAMILY_DEL_CHILDREN,						// [성웅] 가족에 자녀 삭제
	GAMEMSG_RESPONSE_FAMILY_DEL_CHILDREN,						// [성웅]

	GAMEMSG_REQUEST_FAMILY_GET_FAMILYINFO,						// [성웅] 가족 정보 받기
	GAMEMSG_RESPONSE_FAMILY_GET_FAMILYINFO,						// [성웅]

	GAMEMSG_REQUEST_FAMILY_UPDATE,								// [성웅] 가족정보 업데이트
	GAMEMSG_RESPONSE_FAMILY_UPDATE,								// [성웅] 

	GAMEMSG_REQUEST_FAMILY_WARP,								// [성웅] 목표 가족에게 워프 
	GAMEMSG_RESPONSE_FAMILY_WARP,								// [성웅]

	GAMEMSG_REQUEST_FAMILY_CHAT,								// [성웅] 가족챗
	GAMEMSG_RESPONSE_FAMILY_CHAT,								// [성웅]

	GAMEMSG_REQUEST_FAMILY_PLUS_FAMILYEFFECT,					// [성웅] 가족효과 기간 연장
	GAMEMSG_RESPONSE_FAMILY_PLUS_FAMILYEFFECT,					// [성웅]

	GAMEMSG_REQUEST_SOULGUARD_STATUSINOUT,						// [영훈] 수호정령 : 소환/해재
	GAMEMSG_RESPONSE_SOULGUARD_STATUSINOUT,						// [영훈]

	GAMEMSG_RESPONSE_SOULGUARD_FELLOWSHIPSET,					// [영훈] 수호정령 : 친화력 변경됨

	GAMEMSG_REQUEST_SOULGUARD_CHANGENAME,						// [영훈] 수호정령 : 이름 변경
	GAMEMSG_RESPONSE_SOULGUARD_CHANGENAME,						// [영훈]

	GAMEMSG_REQUEST_SOULGUARD_DEAD,								// [영훈] 수호정령 : 죽음
	GAMEMSG_RESPONSE_SOULGUARD_DEAD,							// [영훈]

	GAMEMSG_RESPONSE_SOULGUARD_GET,								// [영훈] 수호정령 : 캐릭터에 있는 정보 가져오기

	GAMEMSG_RESPONSE_SOULGUARD_HOUSE_GET,						// [영훈] 수호정령 보관소 정보 얻어오기

	GAMEMSG_REQUEST_SOULGUARD_HOUSE_CHANGE,						// [영훈] 캐릭터에 있는 수호정령과 보관소에 있는 수호정령을 맞교환 한다

	GAMEMSG_REQUEST_SOULGUARD_HOUSE_CREATE,						// [영훈] 수호정령 보관소 갯수를 늘림
	GAMEMSG_RESPONSE_SOULGUARD_HOUSE_CREATE,					// [영훈]


	//Switch_10MonthEvent_RacoonDodge
	GAMEMSG_REQUEST_RACOONDODGE_EVENT_RESERVATION,				// [성웅] 라쿤 이벤트 예약 
	GAMEMSG_RESPONS_RACOONDODGE_EVENT_RESERVATION,				// [성웅]
	GAMEMSG_REQUEST_RACOONDODGE_EVENT_REQUEST_PLAYEVENT,		// [성웅] 라쿤 이벤트 플레이 선정 되었음 할건지 물어 본다 
	GAMEMSG_RESPONS_RACOONDODGE_EVENT_REQUEST_PLAYEVENT,		// [성웅]
	GAMEMSG_REQUEST_RACOONDODGE_EVENT_WAIT5SECOND,				// [성웅] 라쿤 이벤트5초 대기 
	GAMEMSG_RESPONS_RACOONDODGE_EVENT_WAIT5SECOND,				// [성웅]
	GAMEMSG_REQUEST_RACOONDODGE_EVENT_OVER5SECOND_START,		// [성웅] 5초 시간 다 기다렸음 시작 한다.
	GAMEMSG_RESPONS_RACOONDODGE_EVENT_OVER5SECOND_START,		// [성웅]
	GAMEMSG_REQUEST_RACOONDODGE_EVENT_GET_RESERVE_LIST,			// [성웅] 예약 리스트를 가지고 온다
	GAMEMSG_RESPONS_RACOONDODGE_EVENT_GET_RESERVE_LIST,			// [성웅]
	
	GAMEMSG_REQUEST_RACOONDODGE_EVENT_30SECOND,					// [성웅] 30초 시간 카운트 시작
	GAMEMSG_RESPONS_RACOONDODGE_EVENT_30SECOND,					// [성웅]
	GAMEMSG_REQUEST_RACOONDODGE_EVENT_ROUNDCOUNT,				// [성웅] 라운드 표시
	GAMEMSG_RESPONS_RACOONDODGE_EVENT_ROUNDCOUNT,
	
	GAMEMSG_REQUEST_RACOONDODGE_EVENT_30SECOND_CLEAR,					// [성웅] 30초 시간 카운트 중지
	GAMEMSG_RESPONS_RACOONDODGE_EVENT_30SECOND_CLEAR,					// [성웅]
	GAMEMSG_REQUEST_RACOONDODGE_EVENT_ROUNDCOUNT_CLEAR,					// [성웅] 라운드 표시	중지
	GAMEMSG_RESPONS_RACOONDODGE_EVENT_ROUNDCOUNT_CLEAR,					// [성웅]



	GAMEMSG_REQUEST_DORMANCY_TAKEOUTEQUIPITEMINBOX,				// [기형] 보급 상자에서 장비 아이템을 꺼내기
	GAMEMSG_RESPONSE_DORMANCY_TAKEOUTEQUIPITEMINBOX,			// [기형] 보급 상자에서 장비 아이템을 꺼내기
	GAMEMSG_RESPONSE_DORMANCY_REFRESHPOINTINFO,					// [기형] 서버에서 지속적으로 휴면 포인트 전송
	GAMEMSG_REQUEST_DORMANCY_BUYPOINTITEM,						// [기형] 포인트로 아이템 구입

	GAMEMSG_REQUEST_GAWIBAWIBO_CHARGED_MONEY,					//  [8/12/2009 종호_GAWIBAWIBO] 가위바위보 금액 차감		Client -> Server
	GAMEMSG_RESPONSE_GAWIBAWIBO_CHARGED_MONEY,					//	[8/12/2009 종호_GAWIBAWIBO] 가위바위보 금액 차감결과	Server -> Client

	GAMEMSG_REQUEST_GAWIBAWIBO_CALCULATE_RESULT,				//  [8/12/2009 종호_GAWIBAWIBO] 가위바위보 승무패 결과		Client -> Server
	GAMEMSG_RESPONSE_GAWIBAWIBO_CALCULATE_RESULT,				//	[8/12/2009 종호_GAWIBAWIBO] 가위바위보 승무패 결과		Server -> Client
	GAMEMSG_RESPONSE_GAWIBAWIBO_NOTICE_RESULT,					

	//GAMEMSG_REQUEST_GAWIBAWIBO_COUNT,							//	[8/19/2009 종호_GAWIBAWIBO] 가위바위보 연승정보 		Client -> Server
	//GAMEMSG_RESPONSE_GAWIBAWIBO_COUNT,							//	[8/19/2009 종호_GAWIBAWIBO] 가위바위보 연승정보 		Server -> Client

	GAMEMSG_REQUEST_GAWIBAWIBO_REWARD,							//	[8/20/2009 종호_GAWIBAWIBO] 가위바위보 보상				Client -> Server
	GAMEMSG_RESPONSE_GAWIBAWIBO_REWARD,							//	[8/20/2009 종호_GAWIBAWIBO] 가위바위보 보상				Server -> Client

	// 복불복----------------------------------------------------------------------------------------------
	GAMEMSG_REQUEST_BBB_CHECK_PlAY_CONDITION,				// [지연] 복불복 게임 참여 가능 여부를 체크한다. (바로필드이동 or 예약자등록) 
	GAMEMSG_RESPONSE_BOKBULBOK_CHECK_ENTERMAP_ENABLE,		// [지연] 맵에 입장가능
	GAMEMSG_RESPONSE_BOKBULBOK_CHECK_WAITLIST_ENABLE,		// [지연] 대기자 리스트에 등록 가능
	GAMEMSG_RESPONSE_BOKBULBOK_CHECK_ENTRYREQUEST_FAIL,		// [지연] 게임참여 할 수 없음.
	
	GAMEMSG_REQUEST_BBB_WAITLIST_ANSWER,					// [지연] 복불복 대기자리스트등록 수락시.(클라->서버)
	GAMEMSG_RESPONSE_BBB_WAITLIST_ANSWER,					// [지연] 대기자리스트(서버->클라)
	
	GAMEMSG_REQUEST_BBB_WAITLIST_CANCEL,					// [지연] 대기 취소 요청 (클라->서버)
	GAMEMSG_RESPONSE_BBB_WAITLIST_CANCEL,					// [지연] 대기 취소 요청에 대한 응답 (서버->클라)
	
	GAMEMSG_RESPONSE_BBB_STAGESTART,						// [지연] 매 스테이지 시작할때 클라로 보내는 메세지
	
	GAMEMSG_REQUEST_BBB_STAGEFAIL,							// [지연] 스테이지 실패 (클라->서버)
	GAMEMSG_RESPONSE_BBB_STAGEFAIL,							// [지연] 스테이지 실패 (서버->클라)

	GAMEMSG_RESPONSE_BBB_WARPMAPQUESTION,					// [지연] 대기자 1순위에게 날리는 "지금 게임맵으로 워프할래요?" 메세지 (서버->클라)
	GAMEMSG_REQUEST_BBB_WARPMAPANSWER,						// [지연] 대기자 1순위게 날린 GAMEMSG_REQUEST_BBB_WARPMAPQUESTION 메세지에 대한 응답 (클라->서버);

	GAMEMSG_RESPONSE_BBB_NOTENOUGHMONEY,					// [지연] 게임 참가비가 부족하다고 클라에 알려줌 (서버->클라)
	GAMEMSG_RESPONSE_BBB_STAGECLEAR,						// [지연] 스테이지 클리어시 보여줄 Win 이미지 (매 스테이지 클리어시 출력)
	GAMEMSG_RESPONSE_BBB_TOTALSTAGECLEAR,					// [지연] 최종 스테이지 클리어시 보여줄 Win 이미지 (최종 한번만 출력)

	GAMEMSG_RESPONSE_BBB_GM_SHOWMONSTERTYPE,				// [지연] gm 명령어로 몬스터 머리위에 종류별로 숫자 출력 (테스트 위한)
	GAMEMSG_RESPONSE_BBB_10CLEARNOTICE,						// [지연] 10단계 클리어 보상 내역을 전체 공지 한다.
	GAMEMSG_REQUEST_BBB_ALREADYRESERVED,					// [지연] 이미 예약 되어 있는지 질문 
	GAMEMSG_RESPONSE_BBB_ALREADYRESERVED,					// [지연] 이미 예약 되어 있는지에 대한 답변  
	//-----------------------------------------------------------------------------------------------------

	GAMEMSG_REQUEST_2010NEWYEAREVENT_QUESTCANCEL,			// [지연] 2010신년이벤트 - 퀘스트 거절 메세지
	GAMEMSG_RESPONSE_GOONZUDAYMISSION,						// [지연] 군주데이 - 이벤트 아이템으로 생성된 미션에 대한 통보


	GAMEMSG_REQUEST_NEWNPCREPAIR,								// [강형] 새로운 내구도 수리 메시지
	GAMEMSG_RESPONSE_NEWNPCREPAIR,								// [강형] 새로운 내구도 수리 메시지

	GAMEMSG_REQUEST_NEWITEMREPAIR,								// [강형] 새로운 내구도 수리 메시지
	GAMEMSG_RESPONSE_NEWITEMREPAIR,								// [강형] 새로운 내구도 수리 메시지

	GAMEMSG_REQUEST_NEWITEMREPAIRREQUESTINFO,					// [강형] 내구도 수리 의뢰 정보 요청
	GAMEMSG_RESPONSE_NEWITEMREPAIRREQUESTINFO,					// [강형] 내구도 수리 의뢰 정보 요청

	GAMEMSG_REQUEST_FESTIVAL_EVENT_ENTERFIELD_EVENTAREA,		// [기형] 축제 이벤트 지역으로 이동 요청

	GAMEMSG_REQUEST_HALLOWEENSPIRITBOX_OK,						// [종호] 할로윈 스피릿 박스 확인시

	GAMEMSG_REQUEST_MONSTERCREATE,								// [춘기] 몬스터 생성 요청
	GAMEMSG_RESPONSE_MONSTERCREATE,								// [춘기] 몬스터 생성 요청

	GAMEMSG_REQUEST_EQUIPMENTCREATE,							// [춘기] 장비 생성 요청
	GAMEMSG_RESPONSE_EQUIPMENTCREATE,							// [춘기] 장비 생성 요청

	// 초보자 파티 퀘스트 시스템 (파티매칭)-----------------------------------------------------
	GAMEMSG_REQUEST_PARTYMATCHING_LEVEL,						// [지연] 파티 퀘스트 수행 가능 레벨 체크 요청
	GAMEMSG_RESPONSE_PARTYMATCHING_LEVEL,						// [지연] 파티 퀘스트 수행 가능 레벨 체크 응답

	GAMEMSG_REQUEST_APPYLYTOPARTYJOIN,							// [지연] 파티 가입 신청 요청
	GAMEMSG_RESPONSE_APPLYTOPARTYJOIN,

	GAMEMSG_REQUEST_CREATEMYPARTY,								// [지연] 파티 만들기 신청
	GAMEMSG_RESPONSE_CREATEMYPARTY,

	GAMEMSG_REQUEST_CREATECOMPLETEPARTY,						// [지연] 조건 입력 후 파티 생성 신청.
	GAMEMSG_RESPONSE_CREATECOMPLETEPARTY,						// [지연] 파티 생성 결과

	GAMEMSG_REQUEST_REMOVEPARTYMATCHING,						// [지연] 대화상자의 파티매칭 삭제 버튼 눌렀을때

	GAMEMSG_REQUEST_GETPARTYMEMBERNAMELIST,						// [지연] 선택한 파티매칭의 파티원 이름 정보를 요청
	GAMEMSG_RESPONSE_GETPARTYMEMBERNAMELIST,					// [지연] 파티멤버 이름 요청에 대한 서버 처리 응답

	GAMEMSG_RESPONSE_PARTYMATCHINGEXCEPTION,					// [지연] 파티매칭 기능 수행중 예외 발생한걸 클라에 알려줌


	//-------------------------------------------------------------------------------------------------------------------
	//[성웅] 라쿤이벤트 군주 이벤트로 변경
	//-------------------------------------------------------------------------------------------------------------------
	GAMEMSG_REQUEST_RACCOONEVENT_TOGOONZU,						//[성웅] 라쿤이벤트 군주 이벤트로 변경
	GAMEMSG_RESPONSE_RACCOONEVENT_TOGOONZU,						//[성웅] 라쿤이벤트 군주 이벤트로 변경

	GAMEMSG_REQUEST_GOONZURACCOONEVENT_SETINFO,					//[성웅] 라쿤이벤트 군주 이벤트로 변경-현제정보를 가지고온다
	GAMEMSG_RESPONSE_GOONZURACCOONEVENT_SETINFO,				//[성웅] 라쿤이벤트 군주 이벤트로 변경

	//-------------------------------------------------------------------------------------------------------------------
	//[진성] 스킬
	//-------------------------------------------------------------------------------------------------------------------
	GAMEMSG_REQUEST_SKILLBOOK_BUYITEM,							// 스킬북 아이템 구입.
	GAMEMSG_RESPONSE_SKILLBOOK_BUYITEM,		
	GAMEMSG_RESPONSE_SKILLBOOK_MAGICINFO_GET,					// 스킬북으로 새로운 마법 획득.
	GAMEMSG_RESPONSE_SKILLBOOK_MAGIC_ADD,						// 스킬북으로 새로운 마법 획득.
	GAMEMSG_RESPONSE_SKILLBOOK_MAGIC_DEL,						// 스킬북으로 새로운 마법 획득.
	GAMEMSG_RESPONSE_SKILLBOOK_SUBMAGIC_ADD,					// 스킬북으로 보조 마법 획득.
	GAMEMSG_REQUEST_SKILLBOOK_SUBMAGIC_DEL,						// 스킬북으로 보조 마법 획득.
	GAMEMSG_RESPONSE_SKILLBOOK_SUBMAGIC_DEL,					// 스킬북으로 보조 마법 획득.
	GAMEMSG_REQUEST_SKILLBOOK_SUBMAGIC_LIST_GET,				// 스킬북으로 보조 마법 획득.
	GAMEMSG_RESPONSE_SKILLBOOK_SUBMAGICLIST_DEL,				// 스킬북으로 보조 마법 획득.
	GAMEMSG_RESPONSE_CURSEINFO,

	
	//-------------------------------------------------------------------------------------------------------------------
	//[기형] 배틀로얄
	//-------------------------------------------------------------------------------------------------------------------
	GAMEMSG_RESPONSE_BATTLEROYAL_NOTICERESULT,					// [기형] 배틀로얄 - 게임 결과 메시지
	GAMEMSG_REQUEST_BATTLEROYAL_REMAINEDTIME,					// [기형] 배틀로얄 - 게임 시간 
	GAMEMSG_RESPONSE_BATTLEROYAL_REMAINEDTIME,					// [기형] 배틀로얄 - 게임 시간 
	GAMEMSG_RESPONSE_BATTLEROYAL_STARTGAME,						// [기형] 배틀로얄 - 게임 시작
	GAMEMSG_RESPONSE_BATTLEROYAL_ENDGAME,						// [기형] 배틀로얄 - 게임 종료
	GAMEMSG_RESPONSE_BATTLEROYAL_POWERUPMODE,					// [기형] 배틀로얄 - 캐릭터 파워업 모드
	GAMEMSG_REQUEST_BATTLEROYAL_MOVETOSTADIUM,					// [기형] 배틀로얄 경기장으로 이동
	GAMEMSG_RESPONSE_BATTLEROYAL_OPENSTADIUM,					// [기형] 배틀로얄 - 경기장이 열림
	//=======================================================================

	//[지연] 2010신년이벤트
	GAMEMSG_RESPONSE_FORTUNERESULT,								// [지연] 나온 운세에 대한 결과를 클라에 통보한다

	//-------------------------------------------------------------------------------------------------------------------
	// [성웅] 메크로 유저 정보 분석
	//-------------------------------------------------------------------------------------------------------------------
	GAMEMSG_REQUEST_RECORD_ACTION,				// [2009.06.29 양정모] 클라이언트의 행동을 기록
	GAMEMSG_RESPONSE_RECORD_ACTION,				// [2009.06.29 양정모] 클라이언트의 행동을 기록
	GAMEMSG_REQUEST_RECORD_ACTION_FINISHED,		// [2009.06.29 양정모] 클라이언트의 행동 기록 완료
	GAMEMSG_RESPONSE_RECORD_ACTION_FINISHED,	// [2009.06.29 양정모] 클라이언트의 행동 기록 완료을 운영자에게 전달

	GAMEMSG_GZFTP_PROTOCOL,						// [2009.06.30 양정모] GZFtpProtocol


	GAMEMSG_REQUEST_WARP_TARGETWORLD,									//[성웅] 목표 지역으로 직접 날라가는 티켓 ->현제는 라쿤월드만 있음
	GAMEMSG_RESPONSE_WARP_TARGETWORLD,

	GAMEMSG_REQUEST_PARTY_QUEST_CHECKSTART,								// [종호] 파티퀘스트 시작 조건 체크
	GAMEMSG_RESPONSE_PARTY_QUEST_CHECKSTART,							
	GAMEMSG_REQUEST_PARTY_QUEST_START,									// [종호] 파티퀘스트 시작 요청
	GAMEMSG_RESPONSE_PARTY_QUEST_START,	
	GAMEMSG_RESPONSE_PARTY_QUEST_UPDATE,								// 파티퀘스트 갱신
	GAMEMSG_RESPONSE_PARTY_QUEST_GIVEUP,								// 파티퀘스트 포기

	GAMEMSG_REQUEST_CONTRIBUTION_ITEM_LIST,								// [진성] 기부 할 아이템의 정보와 기부한 아이템의 정보 획득.
	GAMEMSG_RESPONSE_CONTRIBUTION_ITEM_LIST,

	GAMEMSG_REQUEST_CONTRIBUTION_ITEM_LIST_INPUT,
	GAMEMSG_RESPONSE_CONTRIBUTION_ITEM_LIST_INPUT,

	GAMEMSG_REQUEST_CONTRIBUTION_ITEM_INPUT,							// [진성] 아이템 기부.
	GAMEMSG_RESPONSE_CONTRIBUTION_ITEM_INPUT,	

	GAMEMSG_REQUEST_CONTRIBUTION_MAKE_NPC,								// [진성] 아이템 기부가 완료되고 NPC를 만들때.

	//-------------------------------------------------------------------------------------------------------------------
	// [기형] 발렌타인데이 이벤트 
	//-------------------------------------------------------------------------------------------------------------------
	GAMEMSG_RESPONSE_VALENTINEEVENT_CHANGEBUF,	
	GAMEMSG_RESPONSE_VALENTINEEVENT_BUF,	
	GAMEMSG_RESPONSE_VALENTINEEVENT_START,	
	GAMEMSG_RESPONSE_VALENTINEEVENT_END,
	GAMEMSG_REQUEST_VALENTINEEVENT_ISHOLDING,		
	GAMEMSG_RESPONSE_VALENTINEEVENT_ISHOLDING,	
	GAMEMSG_REQUEST_VALENTINEEVENT_GIVE_PRESENT_TONPC,		
	GAMEMSG_RESPONSE_VALENTINEEVENT_GIVE_PRESENT_TONPC,	
	
	GAMEMSG_RESPONSE_GIANTRACCOON_RESURRECT_EVENT_START,			// [진성] 자이언트 라쿤 부활 이벤트 시작 알림.
	GAMEMSG_REQUEST_GIANTRACCOON_RESURRECT_EVENT_ITEM_GET,			// [진성] 자이언트 라쿤 부활 이벤트로 부활탑을 완공하면 아이템을 준다.
	GAMEMSG_REQUEST_NPOTRACCOON_ITEM_GET,							// [진성] 자이언트 라쿤 부활 빵을 기부하면 자이언트 라쿤 부활 알 세트 얻기.
	

	GAMEMSG_REQUEST_MAKEPERSONALSHOP_MANUFACTURE,					//[성웅]	제조 의뢰 인터페이스 개선 
	GAMEMSG_RESPONSE_MAKEPERSONALSHOP_MANUFACTURE,

	GAMEMSG_REQUEST_SEARCHMANUFACTURE,								//[성웅] 제조 의뢰 인터페이스 개선 -> 검색 
	GAMEMSG_RESPONSE_SEARCHMANUFACTURE,
	
	GAMEMSG_REQUEST_GO_RACCOONWORLD,								//[성웅] 라쿤월드로 이동 
	GAMEMSG_RESPONSE_GO_RACCOONWORLD,
	// [성웅] 레벨 보상 상자에서 아이템을 준다 
	GAMEMSG_REQUEST_OPEN_LEVEL_LIMIT_BOX,							//[성웅] 레벨 제한 상자를 열었다- 중국만 사용 한다 
	GAMEMSG_RESPONSE_OPEN_LEVEL_LIMIT_BOX,
	
	GAMEMSG_REQUEST_PVPMAPWARP,										//[성웅] 순간이동이용권을 이용하여 pvp사냥터로 이동한다.

	GAMEMSG_REQUEST_JUSTITEMUSE,									//[성웅] // 죽었던 말던 어떤상황이던 아이템을 줄인다
	
	//[진성] 발렌타인 데이 과금 아이템.
	GAMEMSG_REQUEST_VALENTINEDAY_ITEM_PACKING,						// 포장의뢰.
	GAMEMSG_RESPONSE_VALENTINEDAY_ITEM_PACKING,
	GAMEMSG_REQUEST_VALENTINEDAY_BUY_ITEM,							// 구입.
	GAMEMSG_RESPONSE_VALENTINEDAY_BUY_ITEM,
	GAMEMSG_REQUEST_VALENTINEDAY_CONFESSION_QUESTION,				// 고백.
	GAMEMSG_RESPONSE_VALENTINEDAY_CONFESSION_QUESTION,
	GAMEMSG_REQUEST_VALENTINEDAY_CONFESSION_ANSWER,					// 대답.
	GAMEMSG_RESPONSE_VALENTINEDAY_CONFESSION_ANSWER,

	
	
	
	
	
	
	GAMEMSG_END	//실제 메시지가 아닌 종료자. 

};



#pragma pack(1)

//-------------------------------------------------------------------
// 클라이언트에서 서버로 보내는 일반적인 게임 메시지를 담을 공간. 
//-------------------------------------------------------------------
#define MAX_CLTMSG_DATA_SIZE   11000 // 4의 배수여야 한다. 

class cltMsg{
public:
	sPacketHeader clHead;	

	union{
		BYTE    cData[MAX_CLTMSG_DATA_SIZE];				// 실제 데이터가 들어있는 공간. 
		SI32	siData32[MAX_CLTMSG_DATA_SIZE/4];
	};


	cltMsg(){};

	cltMsg(UI16 type, SI32 datasize, const BYTE* pdata)
	{
		clHead.usCmd	=	type;

		if(datasize >= MAX_CLTMSG_DATA_SIZE)
		{
			MsgBox(TEXT("cltMsg"), TEXT("type:%d datasize:%d"), type, datasize);
		}
		else
		{
			clHead.usSize	=	sizeof(sPacketHeader) + datasize;

			if(pdata && datasize)
			{
				memcpy(cData, pdata, datasize);
			}
		}
	}

	// 32bit 변수 설정시 생성자.
	cltMsg(UI16 type, SI32 sidata)
	{
		clHead.usCmd	=	type;
		clHead.usSize	=	sizeof(sPacketHeader) + sizeof(SI32);

		siData32[0] = sidata;
	}

	SI32 GetTotalSize(){return clHead.usSize;}
	SI32 GetDataSize(){return clHead.usSize - sizeof(sPacketHeader);}
};

#pragma pack()


//-------------------------------------------------------
// 객주에서 내 물건이 팔린 내역을 클라이언트로 통보한다. 
//-------------------------------------------------------
class cltGameMsgResponse_MarketSellInfo{
public:
	SI32 siUnique;		// 물건의 유니크.
	SI32 siItemNum;		// 팔린 개수. 
	GMONEY siPrice;		// 단가. 

	cltGameMsgResponse_MarketSellInfo(SI32 unique, SI32 itemnum, GMONEY price)
	{
		siUnique	= unique;
		siItemNum	= itemnum;
		siPrice		= price;
	}
};

//----------------------------------
// 클라이언트가 게임서버로 접속했을 때 
// CharUnique,암호화키등을 클라이언트로 보낸다. 
//----------------------------------
class cltGameMsgResponse_ClientCharUnique{
public:
	SI32	siCharUnique;
	UI08	uiKey1;
	UI08	uiKey2;
	SI16	siVersion;

	cltGameMsgResponse_ClientCharUnique(SI32 charunique, UI08 key1, UI08 key2, SI16 version)
	{
		siCharUnique	= charunique;
		uiKey1			= key1;
		uiKey2			= key2;
		siVersion		= version;
	}
};

//-----------------------------------
// 레벨업 되었음을 통보 (전투레벨, 명성등급..) 
//-----------------------------------
class cltGameMsgResponse_LevelUp{
public:
	SI16 siCharUnique;		// 레벨이 오른 캐릭터
	SI08 siLevel;			// 레벨이 올라서 결국 얼마가 되었는가?(전투 레벨) 
	SI08 siFameLevel;		// 레벨이 올라서 결국 얼마가 되었는가?(명성 등급)

	cltGameMsgResponse_LevelUp(SI16 charunique, SI08 level, SI08 famelevel)
	{
		siCharUnique	= charunique;
		siLevel			= level;
		siFameLevel		= famelevel;
	}
};

//-----------------------------------
// //KHY - 1205 - 유저레벨업 공지 - 
//-----------------------------------
class cltGameMsgResponse_LevelUpNotice{
public:
	TCHAR szName[MAX_PLAYER_NAME];
	SI32 siLevel;			// 0 이상이면 전투레베업이다.
	SI32 siSkillLevel;		// 0 이상이면 스킬 레벨업이다.
	SI32 siSkillType;		// 0 이상이면 스킬 레벨업이다. 스킬의 종류를 나타낸다.
	SI16 siMessageType;		// 1 = 길드 메시지. 2= 마을 메시지.



	cltGameMsgResponse_LevelUpNotice(TCHAR* name, SI32 level, SI32 skilllevel,SI32 skilltype ,SI16 messagetype)
	{
		MStrCpy( szName, name, MAX_PLAYER_NAME );
		siLevel			= level;
		siSkillLevel		= skilllevel;
		siSkillType		= skilltype;
		siMessageType	= messagetype;
	}
};

//-----------------------------------
// 시전 임대신청 취소 요청. 
//-----------------------------------
class cltGameMsgRequest_HouseRentApplyCancel{
public:
	SI32 siVillageUnique;		// 취소 대상이 되는 마을의 유니크. 
	cltGameMsgRequest_HouseRentApplyCancel(SI32 villageunique)
	{
		siVillageUnique = villageunique;
	}
};


//-----------------------------------
// 마을 건물 정보 
//-----------------------------------
class cltGameMsgResponse_VillageStruct{
public:
	SI16	VillageUnique;			// 마을을 구별하기 위한 유니크. 
	SI16 siVillageStructureInfo[MAX_CONSTRUCTION_NUM];

	cltGameMsgResponse_VillageStruct()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_VillageStruct));
	}

	cltGameMsgResponse_VillageStruct(SI32 villageunique, SI16 pstrinfo[])
	{
		VillageUnique	= villageunique;

		for(SI32 i = 0;i < MAX_CONSTRUCTION_NUM;i++)
		{
			siVillageStructureInfo[i]	= pstrinfo[i];
		}
	}
};

class cltGameMsgResponse_GetVillageInfo
{
public:
	SI32			siMasterVillage[ MAX_VILLAGE_NUMBER ];
	SI32			siParentVillage[ MAX_VILLAGE_NUMBER ];
	SI32			siDeclareWarVillage[ MAX_VILLAGE_NUMBER ];
	NDate			clDeclareWarDate[ MAX_VILLAGE_NUMBER ];
	SI32			siVillageMark[ MAX_VILLAGE_NUMBER ];
	SI32			siTotalVillageScore[ MAX_VILLAGE_NUMBER ];
	SI32			siNewYearEventScore[MAX_VILLAGE_NUMBER];
	SI32			siNowWarVillage[ MAX_VILLAGE_NUMBER ];
	cltGameMsgResponse_GetVillageInfo()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_GetVillageInfo ) );
	}
};


class cltGameMsgResponse_GetNowVillageWarInfo
{
public:
	SI32			siNowWarVillage[ MAX_VILLAGE_NUMBER ];

	cltGameMsgResponse_GetNowVillageWarInfo()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_GetNowVillageWarInfo ) );
	}
};

//--------------------------------------------------------
// 객주에 올라온 주문중 가장 낮은 가격의 정보를 얻어온다. 
//--------------------------------------------------------
#define LOWPRICE_STOCK_ORDER_NUM	2
class cltLowPriceStockOrderUnit{
public:
	SI32 siItemUnique;		// 대상이 되는 물품의 가격. 
	GMONEY siPrice;			// 물품의 가격. 
	SI32 siAmount;			// 물품의 개수. 

	cltLowPriceStockOrderUnit()
	{
		siItemUnique	= 0;
		siPrice			= 0;
		siAmount		= 0;
	}

	cltLowPriceStockOrderUnit(SI32 unique, GMONEY price, SI32 amount)
	{
		siItemUnique	= unique;
		siPrice			= price;
		siAmount		= amount;
	}


	void Set(cltLowPriceStockOrderUnit* pclunit)
	{
		siItemUnique	= pclunit->siItemUnique;
		siPrice			= pclunit->siPrice;
		siAmount		= pclunit->siAmount;
	}
};

class cltGameMsgResponse_LowPriceStockOrder{
public:
	cltLowPriceStockOrderUnit clUnit[LOWPRICE_STOCK_ORDER_NUM];

	void Add(cltLowPriceStockOrderUnit* pclunit)
	{
		for(SI32 i = 0;i < LOWPRICE_STOCK_ORDER_NUM;i++)
		{
			if(clUnit[i].siItemUnique )continue;

			clUnit[i].Set(pclunit);
			return ;
		}
	}
};


//--------------------------------------
// Person의 이름을 요청한다. 
//--------------------------------------
class cltGameMsgRequest_GetName{
public:
	SI32 siCharUnique;	// 이름을 얻어와야 하는 대상의 CharUnique
	cltGameMsgRequest_GetName(SI32 charunique)
	{
		siCharUnique = charunique;
	}
};

class cltGameMsgResponse_GetName{
public:
	SI32 siCharUnique;	// 이름을 얻어와야 하는 대상의 CharUnique
	TCHAR szName[MAX_PLAYER_NAME];

	cltGameMsgResponse_GetName(SI32 charunique, TCHAR* name)
	{
		siCharUnique = charunique;
		MStrCpy( szName, name, MAX_PLAYER_NAME );
	}

};

//----------------------------------------------
// Person의 Horse 정보를 보낸다. 
//----------------------------------------------
class cltGameMsgResponse_NeighbourHorse{
public:
	SI16		siCharUnique;		// 말의 주인. 
	SI16		siHorseUnique;		// 보내지는 말의 유니크. 

	cltGameMsgResponse_NeighbourHorse(SI16 charunique, SI16 horseunique)
	{
		siCharUnique	= charunique;
		siHorseUnique	= horseunique;
	}
};


//---------------------------------------------------------------
// 소지품 내의 아이템을 수리하도록 한다. 
//--------------------------------------------------------------
class cltGameMsgRequest_InvRepair{
public:
	SI32	siItemPos;
	cltItem clItem;

	cltGameMsgRequest_InvRepair(SI32 itempos, cltItem* pclitem)
	{
		siItemPos = itempos;
		clItem.Set(pclitem);
	}
};

class cltGameMsgResponse_InvRepair{
public:
	SI32	siItemPos;
	cltItem clItem;

	cltGameMsgResponse_InvRepair(SI32 itempos, cltItem* pclitem)
	{
		siItemPos = itempos;
		clItem.Set(pclitem);
	}
};

//----------------------------------------------------------------
// 공지 응답. 
//----------------------------------------------------------------
class cltGameMsgResponse_Notice
{
public:
	SI32	siDataSize;
	SI32	PlayerNameLength;
	//KHY - 0129 - 하단 채팅창에 공지 출력여부를 결정한다. - true = 사용.
	bool	UseChatBarNotic;
	// 운영자가 보내는 공지	
	bool	m_bGMNotice;

	TCHAR	szChatText[128];

	cltGameMsgResponse_Notice( TCHAR* playername, TCHAR*text ,bool ChatBarNotic, bool bGMNotice=false )
	{
		siDataSize		= sizeof(SI32) + sizeof(SI32);

		UseChatBarNotic	= ChatBarNotic;
		siDataSize		+= sizeof(bool);

		m_bGMNotice		= bGMNotice;
		siDataSize		+= sizeof(bool);

		StringCchCopy( szChatText, MAX_PLAYER_NAME, playername );

		size_t length;
		StringCchLength( szChatText, 128, &length );
		PlayerNameLength = length;

		StringCchCopyN(&szChatText[ PlayerNameLength ], 128, text, 100 );

		StringCchLength( szChatText, 128, &length );
		siDataSize		+= (SI32)length * (SI32)(sizeof(TCHAR)) + 2;		
	}
};

#define HALLOWEENQUEST		1	// 할로윈 퀘스트 NPC 위치 알림
#define HALLOWEENSUPRISE	2	// 할로윈 깜짝 아이템 알림
//----------------------------------------------------------------
// 퀘스트 발생을 전체 공지한다.( npc 얼굴을 나타내기 위해서)
//----------------------------------------------------------------
class cltGameMsgResponse_Notice_Quset
{
public:
	SI32 siKind		;
	SI32 siMapIndex	;
	SI32 siType		;

	SI16 siX		;		// 할로윈 엔피씨 생성 좌표
	SI16 siY		;		// 할로윈 엔피씨 생성 좌표

	cltGameMsgResponse_Notice_Quset( SI32 Kind , SI32 MapIndex, SI32 Type, SI16 X, SI16 Y )
	{
		siKind = Kind			;	// 해당 퀘스트의 NPC 카인드 나 아이템 
		siMapIndex = MapIndex	;	// 해당 퀘스트의 NPC가 나타난 지역의 맵인덱스 0 이면 맵정보가 필요 없음
		siType = Type			;	// 퀘스트 정보를 알려줄건지 깜짝 아이템을 알려줄건지 타입 
		siX		=	X			;	// 할러윈 엔피씨 생성 좌표
		siY		=	Y			;	// 할로윈 엔피씨 생성 좌표
	}
};


//-------------------------------------------------------------------
// Person의 한개 아이템 정보를 클라이언트로 보내는 메시지의 구조체. 
//-------------------------------------------------------------------
#define PERSONITEMINFO_OPTION_NONE						0	// 아무런 옵션이 없다. 
#define PERSONITEMINFO_OPTION_SHOW						1	// 화면에 정보를 표시해준다.
#define PERSONITEMINFO_OPTION_MAKE						2	// 생산된 아이템의 정보이다. (생산정보 표시해야..) 
#define PERSONITEMINFO_OPTION_WARITEM					3	// 전리품을 획득했다. 
#define PERSONITEMINFO_OPTION_MASTER					4	// 운영자에 의해서 주어진 아이템.
#define PERSONITEMINFO_OPTION_WASTE						5	// 내구도 소모로 삭제됨. 
#define PERSONITEMINFO_OPTION_QUEST						6	// 퀘스트를 통해서 얻었따.
#define PERSONITEMINFO_OPTION_MAKEALMOSTFAIL			7	// 거의 실패한 물건이 만들어졌따. 
#define PERSONITEMINFO_OPTION_MAKEFAIL					8	// 물건 생산 실패. 
#define PERSONITEMINFO_OPTION_MAKELUCK					9	// 물건 생산 성공 (행운이 작용)  
#define PERSONITEMINFO_OPTION_FAMELEVEL					10	// 명성레벨 상승으로 물건 얻음. 
#define PERSONITEMINFO_OPTION_SPECIALQUEST				11	// 흥부박퀘스트를 통해서 얻음. 
#define PERSONITEMINFO_OPTION_TOOLWASTE					12	// 물품 제작으로 도구의 내구도가 감소
#define PERSONITEMINFO_OPTION_ROULETTEGAME				13	// 박대박 게임으로 얻었다
#define PERSONITEMINFO_OPTION_MAKEITEMFAIL_POS			23  //완성품 자리에 다른 물품이 존재
#define PERSONITEMINFO_OPTION_ENCHANTITEMFAIL			24  //인챈트 실패
#define PERSONITEMINFO_OPTION_LASTENCHANTITEMFAIL		25  //인챈트 실패
#define PERSONITEMINFO_OPTION_NOMATENCHANTITEMFAIL		26  //인챈트 재료부족
#define PERSONITEMINFO_OPTION_NOSKILLENCHANTFAIL		28  //인챈트 스킬 실패
#define PERSONITEMINFO_OPTION_NOMONEYENCHANTFAIL		29  //소지금 부족 실패
#define PERSONITEMINFO_OPTION_NOCONDENCHANTFAIL			30  //조건에 맞지 않아 실패
#define PERSONITEMINFO_OPTION_NOSEALINGUP_ENCHANTFAIL	31  //원본의 밀봉이 풀려있어서 실패
#define PERSONITEMINFO_OPTION_BEGINNERWEAPON			32	// 초보자 무기 받음
#define PERSONITEMINFO_OPTION_EVENT						33	// 신년 이벤트		
#define PERSONITEMINFO_OPTION_MOFUMOFUEVENT				34	// 모후모후 이벤트로 받았다.
#define PERSONITEMINFO_OPTION_SERVEREVENTQUEST			35	// 서버 이벤트 퀘스트로 아이템을 받았다
#define PERSONITEMINFO_OPTION_HALLOWEENEVENT			36	// 할로윈 룰렛으로 망토를 받았다.
#define PERSONITEMINFO_OPTION_PVP_LEAGUE				37	// PVP리그에서 목표달성 선물을 받았다.
#define PERSONITEMINFO_OPTION_NEWYEAREVENT				38	// 신년이벤트 운세룰렛을 돌려서 아이템을 얻었다.
#define PERSONITEMINFO_OPTION_BUYFROMNEWMARKET			39	// 통합마켓에서 물품구입
#define PERSONITEMINFO_OPTION__TREASUREEVENTITEMUSE		40	// 보물찾기 아이템 사용
#define PERSONITEMINFO_OPTION_SEQGMCOMMAND				41  // SEQ 명령어를 사용해서 아이템을 생성함.
#define PERSONITEMINFO_OPTION_DORMANCY					42  // 휴면 계정 시스템
#define PERSONITEMINFO_OPTION_BATTLEROYAL				43  // 배틀로얄
#define PERSONITEMINFO_OPTION_ADDITIONALITEM_GET		44	// 부수적인 아이템을 얻었다

class cltGameMsgResponse_PersonItemInfo
{
public:
	SI16	siItemPos;
	SI16	siOption;		// 클라이언트에서 수령한 정보를 어떻게 사용할것인가 ?
	SI16	siChangedNum;	// 아이템 변동량. 
	SI32	siExp;			// 물품 생산으로 획득한 경험치. 
	cltItem clItem;
	GMONEY	siPrice;		// 이 아이템의 시세. 	

	cltGameMsgResponse_PersonItemInfo()
	{
		siItemPos		= 0;
		siOption		= PERSONITEMINFO_OPTION_NONE;
		siChangedNum	= 0;
		siPrice			= 0;
	}
	cltGameMsgResponse_PersonItemInfo(SI16 itempos, SI16 option,  SI16 changednum, SI32 exp, cltItem* pclitem, GMONEY price)
	{
		siItemPos		= itempos;
		siOption		= option;
		siChangedNum	= changednum;
		siExp			= exp;
		clItem.Set(pclitem);
		siPrice			= price;
	}
};

class cltGameMsgResponse_CrystalItemInfo
{
public:
	SI16	siItemPos;

	cltCrystalItemInfo clCrystalItemInfo; // 현재 아이템의 Crystal 관련 정보 (현재는 내구도만)

	cltGameMsgResponse_CrystalItemInfo()
	{
		siItemPos		= 0;
	}
	cltGameMsgResponse_CrystalItemInfo(SI16 itempos, cltItem* pclitem)
	{
		siItemPos		= itempos;

		cltCrystalItemInfo Crystalitem;

		for(SI16 cNum = 0; cNum < MAX_CRYSTAL_NUM ; cNum++)
		{
			Crystalitem.siDurability[cNum] = pclitem->cltItemCrytal[cNum].GetDurability();
		}

		clCrystalItemInfo.Set(&Crystalitem);
	}
};

//-------------------------------------------------------------------
// 다른 Person의 전체 복장 정보를 통보한다.
//-------------------------------------------------------------------
class cltGameMsgResponse_PersonAllShapeInfo
{
public:
	SI32	siCharUnique;

	cltShapeEquipInfo clEquipInfo;

	SI16	siHorse;
	cltDate clAddStatusDate;
	SI32	siStatusTicketNum;

	cltGameMsgResponse_PersonAllShapeInfo()
	{
		siCharUnique	= 0;
		siHorse			= 0;
		clAddStatusDate.MakeFullDateFromDateVary(0);
		siStatusTicketNum = 0;
	}

	cltGameMsgResponse_PersonAllShapeInfo(SI32 charunique, cltShapeEquipInfo* pclEquipInfo,
		SI16 horse,cltDate* cldate,SI32 ticket)
	{
		siCharUnique	= charunique;
		clEquipInfo.Set(pclEquipInfo);

		siHorse			= horse;
		clAddStatusDate.Set(cldate);
		siStatusTicketNum = ticket ;
	}
};


class cltGameMsgResponse_PersonMakeItemSwitch
{
public:
	SI32	siCharUnique;
	SI08    siMakeItemSwitch;

	cltGameMsgResponse_PersonMakeItemSwitch(SI32 charunique, SI08 makeitemswitch)
	{
		siCharUnique	= charunique;
		siMakeItemSwitch = makeitemswitch;
	}
};


//-----------------------------------------------------------------
// 객주에 올라온 특정 물품의 목록을 요청한다. 
//-----------------------------------------------------------------
class cltGameMsgRequest_MarketOrder{
public:
	SI16 siItemUnique;			// 대상 물품의 유니크. 
	GMONEY siPriceCondition;		// 가격 조건. 

	cltGameMsgRequest_MarketOrder(SI32 itemunique, GMONEY pricecondition)
	{
		siItemUnique		= itemunique;
		siPriceCondition	= pricecondition;
	}
};


class cltGameMsgResponse_NewMarketAccount
{
public:

	cltNewMarketAccount	clMarketAccount;

	cltGameMsgResponse_NewMarketAccount(cltNewMarketAccount* pMarketAccount)
	{
		clMarketAccount.Init();
		if( NULL != pMarketAccount )
			clMarketAccount.Set( pMarketAccount );
	}
};

class cltGameMsgRequest_NewMarketItemList{
public:

	enum
	{
		TYPE_NORMAL_LIST = 0,
		TYPE_CHEAP_LIST
	};

	SI16 siItemUnique;			// 대상 물품의 유니크. 
	SI16 siRequestPage;
	UI08 uiType;

	cltGameMsgRequest_NewMarketItemList(SI16 itemunique, SI16 requestpage, UI08 type = TYPE_NORMAL_LIST )
	{
		siItemUnique		= itemunique;
		siRequestPage	= requestpage;
		uiType			 = type;
	}
};

class cltGameMsgResponse_NewMarketItemList{
public:

	UI08				uiType;
	SI16				siMaxPage;		
	SI64				siCurrentPrice;
	cltNewTradeOrder	clNewTradeOrder[MAX_NEWMARKET_ITEMLIST];

	cltGameMsgResponse_NewMarketItemList( UI08 type, SI16 maxpage, SI64 CurrentPrice, cltNewTradeOrder* newtradeorder)
	{
		uiType = type;
		siMaxPage		= maxpage;
		siCurrentPrice = CurrentPrice;
		for( SI16 i = 0; i < MAX_NEWMARKET_ITEMLIST; ++i )
			clNewTradeOrder[i].Set( &newtradeorder[i] );
	}
};

//---------------------------------------------------------------
// 소지품 내의 밀봉 아이템의 해제를 요청한다.
//--------------------------------------------------------------
class cltGameMsgRequest_Inv_CancellationSealingUp{
public:
	SI32	siItemPos;
	cltItem clItem;

	cltGameMsgRequest_Inv_CancellationSealingUp(SI32 itempos, cltItem* pclitem)
	{
		siItemPos = itempos;
		clItem.Set(pclitem);
	}
};

class cltGameMsgResponse_Inv_CancellationSealingUp{
public:
	SI32	siItemPos;
	cltItem clItem;

	cltGameMsgResponse_Inv_CancellationSealingUp(SI32 itempos, cltItem* pclitem)
	{
		siItemPos = itempos;
		clItem.Set(pclitem);
	}
};

//--------------------------------------------------------
// 피로도 시스템의 정보 갱신 및 시간 경고를 한다.
//--------------------------------------------------------
class cltGameMsgResponse_TiredStateNotice{
public:
	UI08 uiTiredState;
	SI32 siTiredTimeNotice;
	NTCHARStringName	m_kCharName;		// 캐릭터 이름

	cltGameMsgResponse_TiredStateNotice(UI08 TiredState, SI32 TiredTimeNotice,  NTCHARStringName kCharName)
	{
		uiTiredState = TiredState;
		siTiredTimeNotice = TiredTimeNotice;
		m_kCharName = kCharName;
	}
};

//--------------------------------------------------------
// 보물상자를 연다.
//--------------------------------------------------------
class cltGameMsgResponse_OpenTreasureBox
{
public:
	SI16	siItemPos;
	cltItem	clItem;

	SI16	siKeyItemPos;
	cltItem	clKeyItem;

	SI16	siToItemPos1;
	cltItem clToItem1;
	SI32	siGetItemNumber1;

	SI16	siToItemPos2;
	cltItem clToItem2;
	SI32	siGetItemNumber2;

	cltGameMsgResponse_OpenTreasureBox(SI16 itempos, cltItem* pclitem, SI16 keyitempos, cltItem* pclkeyitem, SI16 toitempos1, cltItem* pcltoitem1, SI32 getitemnumber1 ,SI16 toitempos2 , cltItem* pcltoitem2 , SI32 getitemnumber2 )
	{
		siItemPos		= itempos;
		clItem.Set(pclitem);

		siKeyItemPos		= keyitempos;
		if(pclkeyitem)	clKeyItem.Set(pclkeyitem);

		siToItemPos1		= toitempos1;
		clToItem1.Set(pcltoitem1);
		siGetItemNumber1 = getitemnumber1;

		siToItemPos2		= toitempos2;
		clToItem2.Set(pcltoitem2);
		siGetItemNumber2 = getitemnumber2;
	}
};
// [영훈] 랜덤박스로부터 레어아이템을 획득했을때 보내는 패킷
class cltGameMsgResponse_RareItemFromTreasureBox
{
public:
	TCHAR	tszUserName[MAX_PLAYER_NAME]; // 아이템을 획득한 User이름
	cltItem	cltTreasureBox;				// 어떤 렌덤박스를 열었는가?
	cltItem	cltGetItem;					// 무슨 아이템을 얻었는가?


	cltGameMsgResponse_RareItemFromTreasureBox( TCHAR* ptszUserName, cltItem* pclTreasureBox, cltItem* pclGetItem )
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_RareItemFromTreasureBox ) );

		StringCchCopy( tszUserName, MAX_PLAYER_NAME, ptszUserName );

		cltTreasureBox.Set( pclTreasureBox );
		cltGetItem.Set( pclGetItem );
	}
};

//---------------------------------------------------------------
// 연금술사로부터 아이템 변경을 요청한다.
//--------------------------------------------------------------
class cltGameMsgRequest_Inv_ChangItemFormNpc{
public:
	SI32	siItemPos;		// 아이템 포지션
	UI16	uiStatusNow;	// 현제 스테이터스
	UI08	uiStatusLater;	// 변경할스테이터스
	//cltItem clItem;

	cltGameMsgRequest_Inv_ChangItemFormNpc(SI32 itempos, UI16	StatusNow, UI08	StatusLater/*, cltItem* pclitem*/)
	{
		siItemPos = itempos;			// 아이템 포지션 
		uiStatusNow = StatusNow;		//  변경할 현제 스테이터스 내용
		uiStatusLater = StatusLater;	// 변경할 스테이터스 
		//clItem.Set(pclitem);
	}
};

class cltGameMsgResponse_Inv_ChangItemFormNpc{
public:
	SI32	siItemPos;
	cltItem clItem;

	cltGameMsgResponse_Inv_ChangItemFormNpc(SI32 itempos, cltItem* pclitem)
	{
		siItemPos = itempos;
		clItem.Set(pclitem);
	}
};
// [2009.06.29 양정모] 클라이언트의 행동을 기록
class cltGameMsgRequest_RecordAction
{
public:

	SI32	siAdminPersonID;	

	SI32	siyourpersonid;
	CHAR	szyourip[ 32 ];
	SI32	time_now;

	BOOL	bReportedByAdminCommand;

public:
	cltGameMsgRequest_RecordAction( SI32 _siadminpersonid, SI32 yourpersonid, CHAR *ip, SI32 _timenow, BOOL breportedbyadmincommand )
	{
		siAdminPersonID			=	_siadminpersonid;
		siyourpersonid			=	yourpersonid;

		strncpy( szyourip, ip, 31 );
		time_now				=	_timenow;

		bReportedByAdminCommand	=	breportedbyadmincommand;
	}

};

// [2009.06.29 양정모] 클라이언트의 행동을 기록(응답)
class cltGameMsgResponse_RecordAction
{
public:

	SI32	siResult;

	SI32	siAdminPersonID;
	BOOL	bReportedByAdminCommand;


public:
	cltGameMsgResponse_RecordAction( SI32 _siresult, SI32 _siadminpersonid, BOOL _breportedbyadmincommand )
	{
		siResult					=	_siresult;
		siAdminPersonID				=	_siadminpersonid;
		bReportedByAdminCommand		=	_breportedbyadmincommand;
	}
};

// [2009.06.29 양정모]클라이언트의 행동 기록 완료
class cltGameMsgRequest_RecordActionFinished
{
public:

	SI32	siAdminPersonID;	
	CHAR	szFilePath[ MAX_PATH ];
	BOOL	bReportedByAdminCommand;

public:
	cltGameMsgRequest_RecordActionFinished( SI32 _siadminpersonid, CHAR *pszFilePath, BOOL _breportedbyadmincommand  )
	{
		siAdminPersonID			=	_siadminpersonid;
		strcpy( szFilePath, pszFilePath );
		bReportedByAdminCommand	=	_breportedbyadmincommand ;
	}
};

// [2009.06.29 양정모]클라이언트의 행동 기록 완료을 운영자에게 전달
class cltGameMsgResponse_RecordActionFinished
{
public:

	CHAR	szCharName[ MAX_PLAYER_NAME ];
	CHAR	szFilePath[ MAX_PATH ];

public:
	cltGameMsgResponse_RecordActionFinished( CHAR *pszPlayerName, CHAR *pszFilePath )
	{
		strncpy( szCharName, pszPlayerName, MAX_PLAYER_NAME - 1 );
		strncpy( szFilePath, pszFilePath, MAX_PATH - 1 );
	}
};

class cltGameMsgGZFTPProtocol
{
public:

	SI32		personid_from;			
	SI32		personid_to;

	enum	GZFtpMsg{	GZFTP_INITIALIZE,						
		GZFTP_REQUEST_GETFILE,
		GZFTP_RESPONSE_GETFILE,
		GZFTP_REQUEST_DATA,
		GZFTP_RESPONSE_DATA

	};


	GZFtpMsg	msg;
	SI32		sibuffersize;

	struct __initialize_data
	{
		SI32	siyourPersonID;
		SI32	siotherPersonID;

		CHAR	szSenderName[ MAX_PLAYER_NAME ];

		BOOL	boption_delete;

		CHAR	szFilePath[ 260 ];			// 260 - MAX_PATH;	
	};

	struct	__req_getfile_data
	{
		DWORD	dwID;

		CHAR	szFilePath[ 260 ];			// 260 - MAX_PATH;	

		BOOL	bDeleteFile;
	};

	struct	__res_getfile_data
	{
		DWORD	dwResult;
		DWORD	dwID;

		LONG	ltotalbytes;
	};

	struct	__req_filedata
	{
		DWORD	dwID;
		BYTE	filedata[ 1024 ];			
		LONG	lsize;
	};

	struct	__res_filedata
	{
		DWORD	dwResult;
		DWORD	dwID;
	};

	union	
	{
		char	buffer[ 1032 ];

		__initialize_data	initialize_data;

		__req_getfile_data	req_getfile_data;

		__res_getfile_data	res_getfile_data;

		__req_filedata		req_filedata;

		__res_filedata		res_filedata;		
	};	

public:

	cltGameMsgGZFTPProtocol( GZFtpMsg _msg, SI32 _personid_from, SI32 _personid_to, BYTE *_buffer, SI32 _sibuffersize )
	{
		msg					=	_msg;

		personid_from		=	_personid_from;
		personid_to			=	_personid_to;

		sibuffersize		=	_sibuffersize;

		memcpy( buffer, _buffer, _sibuffersize );
	}	
};

#endif
