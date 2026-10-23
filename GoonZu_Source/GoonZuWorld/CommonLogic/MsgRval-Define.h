#ifndef _MSGRVALDEFINE_H
#define _MSGRVALDEFINE_H


//----------------------------------------------
// 관련하여 서버가 보내는 리턴값. 
//----------------------------------------------
#define SRVAL_ALREADYMAYOR				5		// 이미 대행수로 재직중. 
#define SRVAL_INVALIDCANDIDATE			9		// 후보자 명단에 해당 캐릭터가 없다 
#define SRVAL_NOCANDIDATE				10		// 후보자가 한명도 없다. 
#define SRVAL_NOMAYOR					11		// 대행수가 아니다. 		
#define SRVAL_NOBANKCHIEF				12		// 전장행수가 아니다.
#define SRVAL_SUCCESSRESIGNMAYOR		13		// 대행수에서 사임하는데 성공했따. 
#define SRVAL_SUCCESSSETFEE				14		// 수수료 설정에 성공. 
#define SRVAL_NOSELLPRODUCT				15		// 팔자로 나온 물건이 실제로는 존재하지 않는다. 
#define SRVAL_NOSELLBANKACCOUNT			16		// 팔자의 전장 계좌가 없다. 
#define SRVAL_NOTENOUGHBANKMONEY		17		// 전장에 충분한 돈이 없다. 		
#define SRVAL_NOBUYBANKACCOUNT			18		// 사고자 하는 사람의 전장계좌가 없다.		
#define SRVAL_CANNOTINPUTTOBUYER		19		// 사자에게 물건을 줄 수 없다. 		
#define SRVAL_SUCCESSTOBUYMARKET		20		// 객주에서 성공적으로 물건을 구입했다. 
#define SRVAL_SUCCESSTOSELLMARKET		21		// 객주에서 성공적으로 물건을 내놓았다. 
#define SRVAL_NOMARKETACCOUNT			22		// 객주에 계좌가 없다. 
#define SRVAL_NOMATCHINGITEMININV		23		// 인벤토리에 일치하는 아이템이 없다. 
#define SRVAL_NOTMATCHMARKETACCOUNT		24		// 사용자의 객주 계좌가 잘못 설정되어 있다.
#define SRVAL_NOMATCHINGITEMINSTG		25		// 객주 창고에  일치하는 아이템이 없다. 
#define SRVAL_CANNOTINPUTTOINV			26		// 인벤토리에 넣을 수 없다. 
#define SRVAL_SUCCESSTOGETFROMSTG		27		// 객주창고에서 성공적으로 회수했다.
#define SRVAL_NOEMPTYHOUSE				28		// 빈 시전이 없어서 임대할 수 없다.
#define SRVAL_INVALIDHOUSEAPPLYER		30		// 임차 희망자가 유효하지 않다. 
#define SRVAL_INVALIDBANKACCOUNT		31		// 임차인에게 전장 계좌가 없다. 

//------------------------------------------
// 전역 메시지 
//------------------------------------------
// 기술. 
#define SRVAL_SETSKILL					40		// 기술이 설정되었다. 
#define SRVAL_SKILLNOTREADYFORMAKE		41		// 물품 생산에 필요한 스킬 미비. 	
#define SRVAL_INSUFFICIENTMATERIAL		42		// 원료 부족으로 생산 불가. 
#define SRVAL_INSUFFICIENTTOOL			43		// 도구 부족으로 생산 불가. 
#define SRVAL_MATMATCHNUM				44		// 생산 요구수가 가능수보다 많아 실패. 
#define SRVAL_FAILPROBILITY				45		// 생산 확률에서 실패. 
#define SRVAL_NOEMPTYAREA				46		// 빈자리가 없어서 실패. 
#define SRVAL_BLOCK_BLOCKED				47		// 사용자 로그인이 제한되었다. 
#define SRVAL_BLOCK_UNBLOCKED			48		// 사용자 로그인이 허용되었다. 
#define SRVAL_BLOCKUSER_BLOCKED			49		// 특정 사용자 로그인이 제한되었다. 
#define SRVAL_BLOCKUSER_UNBLOCKED		50		// 특정 사용자 로그인이 허용되었다. 
#define SRVAL_SUCCESSASK				51		// 질문 등록 성공. 
#define SRVAL_INPUTBIDMONEY				52		// 입찰 입금 성공 (입찰 신청 하면서 전장 돈을 지불하는데 성공) 	
#define SRVAL_ENCHANTFAILTOOMUCH		53		// 더이상 부적 부착 실패. 
#define SRVAL_NOFATHERNAME				54		// 스승님이 존재하지 않는다.
#define SRVAL_SAMEACCOUNT				55		// 같은 계정은 스승이 될 수 없다. 
#define SRVAL_ENCHANTFAILNOTATBMATCH	56		// 부작할려는 ITEM의 ATB속성이 일치하지 않는다.
#define SRVAL_ENCHANTFAILTOOMUCHKIND	57		// 부적은 현재 2종류이상 붙일수 없다.	
#define SRVAL_DECSKILL_FAIL_HAVECHILD	58		// 하위 기술이 있어 상위 기술을 초기화 할 수 없다. 
#define SRVAL_WEAPONSKILL_FAIL_MONEY	59		// 돈이 모자라서 못 올린다.
#define SRVAL_REJECT_SETFATHER			60		// 스승이 스승신청 수락을 거부했다
#define SRVAL_ACCEPT_SETFATHER			61		// 스승이 스승신청 수락을 승낙했다.
#define SRVAL_PENALTY_SETFATHER_TEACHER		62		// 스승 패널티때문에 제자를 못 받아서 스승 신청된 사람에게 메세지
#define SRVAL_PENALTY_SETFATHER_PUPIL		63		// 스승 패널티때문에 제자가 되지 못 해서 제자에게 메세지

//전장 관련 
#define SRVAL_BANK_SUCCESSINPUTMONEY						70		// 입금 성공. 
#define SRVAL_BANK_FAILINPUTMONEY_OUTOFMONEY				71		// 입금 실패 - 현금이 부족.  
#define SRVAL_BANK_FAILINPUTMONEY_NOACCOUNT					72		// 입금 실패 - 계좌 없음. 
#define SRVAL_BANK_FAILINPUTMONEY_NOTENOUGHFEE				73		// 입금 실패 - 수수료 부족
#define SRVAL_BANK_SUCCESSOUTPUTMONEY						75		// 출금 성공
#define SRVAL_BANK_SUCCESSCLOSEACCOUNT						76		// 계좌 해지 성공. 
#define SRVAL_BANK_FAILCLOSEACCOUNT							77		// 계좌 해지 실패
#define SRBAL_BANK_FAILCLOSEACCOUNT_OTHERVILLAGE			78		// 계좌 해지 실패 ( 다른 마을 은행임 )
#define SRBAL_BANK_FAILCLOSEACCOUNT_CHECKMARKET				79		// 계좌 해지 실패 ( 마켓에 물품이있음 )

#define SRVAL_BANK_SUCCESSOUTPUTMONEY_DELACCOUNT			80		// 출금 성공 - 계좌삭제. 
#define SRVAL_BANK_FAILOUTPUTMONEY_NOACCOUNT				85		// 출금 실패 - 계좌가 없어 인출 실패. 
#define SRVAL_BANK_FAILOUTPUTMONEY_NOTENOUGHFEE				86		// 출금 실패 - 수수료 부족. 
#define SRVAL_BANK_FAILOUTPUTMONEY_NOTENOUGHMONEYINACCOUNT	90		// 출금 실패 - 잔액이 부족
#define SRVAL_BANK_FAILSETFEE_NOTCHIEF						95		// 수수료 변경 실패 - 전장행수아님 
#define SRVAL_BANK_SUCCESSSETFEE							96		// 수수료 변경 성공 
#define SRVAL_BANK_SUCCESSCREATEACCOUNT						97		// 계좌 생성 성공.
#define SRVAL_BANK_FAILCREATEACCOUNT						98		// 계좌 생성 실패. 
#define SRVAL_BANK_FAILCREATEACCOUNT_ALREADY				99		// 계좌 생성 실패 - 이미 계좌가 있다. 



// 여각 관련 
#define SRVAL_STOCK_BUY_SUCCESSTRADE				110		// 구입 성공. 
#define SRVAL_STOCK_SELL_SUCCESSTRADE				115		// 매각  성공. 
#define SRVAL_STOCK_FAIL_NOTENOUGHFEE				116		// 거래 실패 - 수수료 부족
#define SRVAL_STOCK_BUY_FAILNOACCOUNT				120	// 사자의 전장 계좌가 없다. 
#define SRVAL_STOCK_SELL_FAILNOACCOUNT				125	// 사자의 전장 계좌가 없다. 
#define SRVAL_STOCK_DSTSELLERNOACCOUNT				126	// 나는 사려고 하는데 상대방의 계좌가 없음. 
#define SRVAL_STOCK_DSTBUYERNOACCOUNT				127	// 나는 팔려고 하는데 상대방의 계좌가 없음. 
#define SRVAL_STOCK_BUY_FAILNOTENOUGHMONEY			130 // 사자의 전장 계좌에 잔액이 충분하지 않다. 
#define SRVAL_STOCK_SELL_FAILNOTENOUGHMONEY			135 // 사자의 전장 계좌에 잔액이 충분하지 않다. 
#define SRVAL_STOCK_BUY_FAILLIMITSTOCKKIND			140	// 제한된 종목 이상의 주식은 살 수 없다. 
#define SRVAL_STOCK_SELL_FAILLIMITSTOCKKIND			145	// 제한된 종목 이상의 주식은 살 수 없다. 
#define SRVAL_STOCK_BUY_HALFSUCCESS					150 // 주문중 일부는 처리되고 일부는 처리되지 않았다. 
#define SRVAL_STOCK_SELL_HALFSUCCESS				155 // 주문중 일부는 처리되고 일부는 처리되지 않았다. 
#define SRVAL_STOCK_BUY_FAILLIAR					170 // 상대 주문이 허구여서 한주도 거래가 체결되지 않았다. 
#define SRVAL_STOCK_SELL_FAILLIAR					175 // 상대 주문이 허구여서 한주도 거래가 체결되지 않았다. 
#define SRVAL_STOCK_BUY_SUCCESSORDER				180 // 사자 주문 성공. 
#define SRVAL_STOCK_SELL_SUCCESSORDER				185 // 사자 주문 성공. 
#define SRVAL_STOCK_FAILSETFEE						190	// 수수료 설정 실패. 
#define SRVAL_STOCK_SUCCESSSETFEE					191	// 수수료 설정 성공.
#define SRVAL_STOCK_SUCCESS_BIDDING					192 // 주식증자 성공( 구 입찰 성공)
#define SRVAL_STOCK_FAIL_BIDDING					193	// 주식증자 실패( 구 입찰 실패)
#define SRVAL_STOCK_SUCCESS_BUY_BIDSTOCK			194 // 증자 주식 사기 성공
#define SRVAL_STOCK_FAIL_BUY_BIDSTOCK				195 // 증자 주식 사기 실패
#define SRVAL_STOCK_FAIL_BUY_BIDSTOCK_NOTENOUGHMONEY 196 //	증자 주식 사기 실패 - 소지금 부족
#define SRVAL_STOCK_FAIL_BUY_BIDSTOCK_ALREADY		197 // 증자 주식 사기 실패 - 주주라 이미 주식을 구입했다.
#define SRVAL_STOCK_FAIL_BUY_BIDSTOCK_EXCEEDAMOUNT		198 // 증자 주식 사기 실패 - 수량 초과
#define SRVAL_STOCK_FAIL_BUY_BIDSTOCK_NOTBUYER		199 // 증자 주식 사기 실패 - 증자당시 구입권한이 없다.

//#define SRVAL_STOCK_FAIL_BIDDING_NOBANKACCOUNT		194 // 입찰 실패 - 전장 계좌 없음 
//#define SRVAL_STOCK_FAIL_BIDDING_NOTENOUGHMONEY		195 // 입찰 실패 - 전장 계좌 금액 부족 
//#define SRVAL_STOCK_FAIL_BIDDING_EXCEEDAMOUNT		196 // 입찰 실패 - 수량 초과
//#define SRVAL_STOCK_FAIL_BIDDING_EXCEEDBIDDER		197 // 입찰 실패 - 입찰자 수 초과
//#define SRVAL_STOCK_FAIL_BIDDING_WRONGVALUE			198 // 입찰 실패 - 이미 신청한 금액 보다 적음 
//#define SRVAL_STOCK_FAIL_BIDDING_WRONGVALUE10		199 // 입찰 실패 - 이전 최고 입찰자 금액보다 10%이상 크지 않음

// 상단관련 
#define SRVAL_CITYHALL_SUCCESSCANDIDATE				200	// 입후보 등록 성공. 
#define SRVAL_CITYHALL_FAILCANDIDATE				205	// 입후보 등록 실패. 
#define SRVAL_CITYHALL_FAILCANDIDATE_LEVELERROR		206	// 후보 등록 레벨 조건이 안되서 실패
#define SRVAL_CITYHALL_FAILCANDIDATE_ALREADY		210	// 이미 같은 신분으로 등록되어 있어서 실패. 
#define SRVAL_CITYHALL_FAILCANCELCANDIDATE			215	// 후보등록취소 실패. 
#define SRVAL_CITYHALL_SUCCESSCANCELCANDIDATE		220	// 후보등록취소 성공. 
#define SRVAL_CITYHALL_SUCCESSBERANK				225	// 직위 취임 성공. 
#define SRVAL_CITYHALL_FAIL_BERANK_NOTHOME			226 // 직위 취임 실패 - 다른 마을의 주민임
#define SRVAL_CITYHALL_FAILCANDIDATE_ALREADYRANK	230	// 이미 다른 신분을 보유중. 
#define SRVAL_CITYHALL_FAILCANDIDATE_OVERFLOW		232	// 입후보 등록 실패 - 더이상 입후보할 수 없다. (오버플로) 
#define SRVAL_CITYHALL_DONATE_OVERMONEY				233	// 기부를 할수 없다 - 기부금보다 소지금이 적다
#define SRVAL_CITYHALL_NOTBESTBEHOLDER				235	// 최대주주가 아님. 
#define SRVAL_CITYHALL_NOTMAYOR						237 // 대행수가 아니라 대행수 권한을 행사할 수 없다. 
#define SRVAL_CITYHALL_SUCCESSDELCANDIDATE			240 // 후보자를 성공적으로 삭제함. 
#define SRVAL_CITYHALL_SUCCESSAPPLYRESIDENT			245 // 성공적으로 주민 등록 신청을 함 
#define SRVAL_CITYHALL_FAILALREADYAPPLY				246 // 주민 등록 신청 실패 - 이미 등록 되어있음
#define SRVAL_CITYHALL_FAILALREADYRESIDENT			247 // 주민 등록 신청 실패 - 이미 주민임 
#define SRVAL_CITYHALL_FAILEXCEEDAPPLYER			248 // 주민 등록 신청 실패 - 신청자 숫자를 초과 하였음
#define SRVAL_CITYHALL_FAILTIMELIMIT				249 // 주민 등록 신청 실패 - 마을탈퇴한후 5일간은 재가입 불가
#define SRVAL_CITYHALL_SUCCESSCANCELRESIDENT		250 // 주민 등록 취소 성공 
#define SRVAL_CITYHALL_FAILCANCELRESIDENT			251 // 주민 등록 취소 실패
#define SRVAL_CITYHALL_FAIL_EXCEEDMAXRESIDENT		252 // 주민 등록 신청/등록 실패 - 최대 주민수 초과 
#define SRVAL_CITYHALL_FAIL_VILLAGEATWAR			253 // 주민 등록 신청/등록 실패 - 공성중 주민등록 불가
#define SRVAL_CITYHALL_SUCCESS_GETRESIDENTINFO		255 // 주민 찾기 성공 
#define SRVAL_CITYHALL_FAIL_GETRESIDENTINFO			256 // 주민 찾기 실패 
#define SRVAL_CITYHALL_FAIL_BERESIDENT_WRONGRANK	257 // 주민 등록 허가 실패 - 다른 마을의 대행수나 행수임
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_NOMASTER		258	// 선전 포고 실패 - 주식이 아직 안 풀려서 주인이 없다.
#define SRVAL_CITYHALL_SUCCESS_SETBASEMONEY			259 // 마을 건물에 자본금 설정 성공

#define SRVAL_CITYHALL_SUCCESS_DECLAREWAR			260 // 선전 포고 성공
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_ALREADYACT   261 // 선전 포고 실패 - 이미 선전 포고 한 마을이 있음 
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_ALREADY		262 // 선전 포고 실패 - 이미 선전 포고 되어있음
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_NOTPROPER	263 // 선전 포고 실패 - 하위 마을로 선전 포고 불가
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_GAP			264 // 선전 포고 실패 - 주민수 차이가 심함

#define SRVAL_CITYHALL_SUCCESS_CANCELDECLAREWAR		265 // 선전 포고 취소 성공 
#define SRVAL_CITYHALL_FAIL_CANCELDECLAREWAR		266 // 선전 포고 취소 실패

#define SRVAL_CITYHALL_FAIL_DECLAREWAR_NOTMAYOR		267 // 선전 포고 실패 - 대상 마을에 국수가 없음.

#define SRVAL_CITYHALL_FAIL_DECLAREWAR_WARTIME		268 // 선전 포고 및 선전 포고 취소 실패 - 전쟁 시간
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_DATE			269 // 선전 포고 및 선전 포고 취소 실패 - 1달 이내 불가 
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_LOWERRANK	270 // 선전 포고 실패 - 대상 마을순위가 5초과 낮음.
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_AFTERLOSE	271 // 선전 포고 실패 - 패배 후 보호 기간이다.
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_TARGET_AFTERLOSE	272 // 선전 포고 실패 - 대상 마을이 패배 후 보호 기간이다.
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_NOTNATION	273 // 선전 포고 실패 - 대상 마을이 패배 후 보호 기간이다.
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_MADEPROTECT_TARGET	274 // 선전 포고 실패 - 대상 마을이 국가 건설 후 보호 기간이다.
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_MADEPROTECT_SELF	275 // 선전 포고 실패 - 자신 마을이 국가 건설 후 보호 기간이다.
#define SRVAL_CITYHALL_FAIL_DECLAREWAR_FORCESTOP	276 // 선전 포고 실패 - 공성전 기능이 강제로 막혀져 있다.

#define SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_MAXLEVEL	277 // 성황신 스탯 조정 실패 - 성황신 종류 변경시 하위레벨로 변경 불가
#define SRVAL_CITYHALL_FAIL_CASTLESTATUS_MAXLEVEL	278 // 캐슬 스탯 조정 실패 - 성황신 종류 변경시 하위레벨로 변경 불가
#define SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_LOWERLEVEL	279 // 성황신 스탯 조정 실패 - 성황신 종류 변경시 하위레벨로 변경 불가
#define SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_MONEY		280 // 성황신 스탯 조정 실패 - 상단 자금 부족 
#define SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_DATE		281 // 성황신 스탯 조정 실패 - 1달 이내에는 재조정 불가 
#define SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_RESIDENT	282	// 성황신 스탯 조정 실패 - 마을 주민수 부족 
#define SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_WARTIME	283 // 성황신 스탯 조정 실패 - 공성중에는 조정 불가 
#define SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_NOTEXIST	284 // 성황신 스탯 조정 실패 - 성황신이 필드에 존재하지 않음 

#define SRVAL_CITYHALL_FAIL_CASTLESTATUS_MONEY		285 // 캐슬 스탯 조정 실패 - 상단 자금 부족 
#define SRVAL_CITYHALL_FAIL_CASTLESTATUS_DATE		286 // 캐슬 스탯 조정 실패 - 1달 이내에는 재조정 불가 
#define SRVAL_CITYHALL_FAIL_CASTLESTATUS_RESIDENT	287	// 캐슬 스탯 조정 실패 - 마을 주민수 부족 
#define SRVAL_CITYHALL_FAIL_CASTLESTATUS_WARTIME	288 // 캐슬 스탯 조정 실패 - 공성중에는 조정 불가 
#define SRVAL_CITYHALL_FAIL_CASTLESTATUS_NOTEXIST	289 // 캐슬 스탯 조정 실패 - 캐슬이 필드에 존재하지 않음 

#define SRVAL_CITYHALL_FAILDESTROYSTR_NOLAST		290	// 1달이 지나지 않아서 건물 해체 실패. 

#define SRVAL_CITYHALL_SUCCESS_PROFITTOCAPITALFUND	291 // 이익금->자본금 전환 성공 
#define SRVAL_CITYHALL_FAIL_PROFITTOCAPITALFUND		292 // 이익금->자본금 전환 실패

#define SRVAL_CITYHALL_FAIL_EVENT_NOTENOUGHMONEY	295	// 이벤트 실패 - 자본금 부족
#define SRVAL_CITYHALL_FAIL_EVENT_ALREADYSTARTED	296	// 이벤트 실패 - 다른 마을에서 실시 중
#define SRVAL_CITYHALL_FAIL_EVENT_JAPANWAR			297 // 이벤트 실패 - 왜구 전쟁중에는 실시 불가

#define SRVAL_CITYHALL_FAIL_CONSTRUCT_NOTFARM		298 // 농장을 건설 할 수 없다 
#define SRVAL_CITYHALL_FAIL_DESTROYSTR_MEETING		299 // 주주총회 발의 중에는 건물을 해체할 수 없다 

// 시전. 
#define SRVAL_HOUSE_FAILAPPLY						300	// 임차 지원 실패. - 범용. 
#define SRVAL_HOUSE_FAILAPPLY_NOACCOUNT				305	// 임차 지원 실패 - 임차인의 전장 계좌 없음. 
#define SRVAL_HOUSE_FAILAPPLY_ALREADYAPPLY			310	// 임차 지원 실패 - 이미 신청했음. 
#define SRVAL_HOUSE_SUCCESSAPPLY					315 // 임차 지원 성공. 
#define SRVAL_HOUSE_SUCCESSAPPLYCANCEL				317 // 임차 취소 성공. 
#define SRVAL_HOUSE_FAILSETCONTRACT_NOTCHIEF		320 // 계약 조건 변경 실패. - 시전행수가 아님. 
#define SRVAL_HOUSE_FAILSETCONTRACT_WRONGFEE		321 // 계약 조건 변경 실패. - 임대료 변경폭 제한 
#define SRVAL_HOUSE_FAILSETCONTRACT_DATE			322 // 계약 조건 변경 실패. - 한달에 한번만 변경 가능  
#define SRVAL_HOUSE_SUCCESSSETCONTRACT				325	// 계약 조건 변경 성공. 
#define SRVAL_HOUSE_FAILNOTSAMECONTRACT				327 // 임대 실패 - 임차인의 계약 조건과 다르다.
#define SRVAL_HOUSE_FAILNOMOREREALESTATE			328 // 임대 실패 - 더이상의 부동산은 보유할 수 없다. 
#define SRVAL_HOUSE_SUCCESSHOUSERENT				329	// 임대성공. 
#define SRVAL_HOUSE_SUCCESSDELHOUSEAPPLYER			330 // 임차 희망자 명단 삭제 성공 
#define SRVAL_HOUSE_FAILDELHOUSEAPPLYER_NOTCHIEF	335 // 임차 희망자 명단 삭제 실패  - 시전 행수가 아니다. 
#define SRVAL_HOUSE_FAILEXTENDCONTRAC_NOMONEY		336	// 계약 연장 실패 - 전장 잔고 부족. 
#define SRVAL_HOUSE_VILLAGELEVELLIMIT				337	// 마을에 말을 넣을 시에 마을 레벨이 낮아서 말을 넣을 수가 없을때
#define SRVAL_HOUSE_SUCCESSEXTENDCONTRACT			340	// 계약 연장 성공
#define SRVAL_HOUSE_FAILNOTENOUGHMONEY				341 // 소지금 부족으로 위약금 지불 실패
#define SRVAL_HOUSE_FAILREMAINDITEM					342	// 아이템,말, 소환수가 남아있어서 시전 해약 실패
#define SRVAL_HOUSE_SUCCESSCANCELLATION				343 // 시전 해약 성공
#define SRVAL_HOUSE_FAILINPUT_NOTRADEITEM			344 // 시전거래주문이 올라가있을때는 거래불가아이템 못넣음
#define SRVAL_HOUSE_FAILINPUT_FULLSTG				345	// 시전의 인벤토리가 꽉차서 더이상 넣을수 없음.
#define SRVAL_HOUSE_FAILARRANGEITEM					346 // 창고 정리 실패 

// 객주 
#define SRVAL_MARKET_SUCCESS_ORDERSELL				400 // 팔자 주문 성공. 
#define SRVAL_MARKET_FAIL_NOTRADEITEM				402	// 팔자 주문 실패 - 거래 금지 품목. 
#define SRVAL_MARKET_FAIL_ORDERSELL_OVERFLOW		405	// 팔자 주문 실패 - 주문 수량 초과. 
#define SRVAL_MARKET_SUCCESS_GETFROMMARKET			410 // 물건 회수 성공 
#define SRVAL_MARKET_FAIL_GETFROMMARKET				415 // 물건 회수 실패 - 인벤에 자리가 없음. 
#define SRVAL_MARKET_SUCCESS_TRADE					420 // 물건 구매 성공. 
#define SRVAL_MARKET_FAILSETFEE						470	// 수수료 설정 실패. 
#define SRVAL_MARKET_SUCCESSSETFEE					471	// 수수료 설정 성공. 

// 통합마켓
#define SRVAL_NEWMARKET_SUCCESS_ORDERSELL				472 // 팔자 주문 성공. 
#define SRVAL_NEWMARKET_FAIL_NOTRADEITEM				473	// 팔자 주문 실패 - 거래 금지 품목. 
#define SRVAL_NEWMARKET_FAIL_ORDERSELL_OVERFLOW			474	// 팔자 주문 실패 - 주문 수량 초과. 
#define SRVAL_NEWMARKET_SUCCESS_GETFROMMARKET			475 // 물건 회수 성공 
#define SRVAL_NEWMARKET_FAIL_GETFROMMARKET				476 // 물건 회수 실패 - 인벤에 자리가 없음. 
#define SRVAL_NEWMARKET_SUCCESS_TRADE					477 // 물건 구매 성공. 
#define SRVAL_NEWMARKET_FAILSETFEE						478	// 수수료 설정 실패. 
#define SRVAL_NEWMARKET_SUCCESSSETFEE					479	// 수수료 설정 성공. 
#define SRVAL_NEWMARKET_DONOT_TRADETIME					480	// 수수료 설정 성공
#define	SRVAL_NEWMARKET_FAIL_ORDERSELL_ALREADY_ORDERED	481	// 팔자 주문 실패 - 이미 주문되어 있는 아이템


// 수렵장 
#define SRVAL_HUNT_NOPARTMEMBER						600 // 파티원이 아니어서 출입 실패. 
#define SRVAL_HUNT_ALREADYPARTYUSE					601 // 이미 사용중인 파티가 있다. 
#define SRVAL_HUNT_FAILENTER_LEVEL					602	// 레벨 제한에 걸려 수렵장을 이용할 수 없다. 
#define SRVAL_HUNT_FAILENTER_NOFEE					603	// 입장료가 부족하여 입장 불가. 
#define SRVAL_HUNT_NOTENOUGHREWARD					604 // 포상금 재고가 부족하여 입장 불가. 
#define SRVAL_HUNT_NOTENOUGHMON						605 // 몬스터  재고가 부족하여 입장 불가. 
#define SRVAL_HUNT_TIMEOUT							606 // 시간 제한 걸림. 

#define SRVAL_HUNT_FAIL_HUNTMAP_NOTENOUGHMONEY		607	// 사냥터 지도 변경 실패 - 보유자금 부족
#define SRVAL_HUNT_SUCCESS_HUNTMAP					608	// 사냥터 지도 변경 성공

// 역참 
#define SRVAL_POSTOFFICE_SUCCESS_SENDITEM			700 // 아이템 전송 성공
#define SRVAL_POSTOFFICE_SUCCESS_SENDMSG			701 // 메시지 전송 성공
#define SRVAL_POSTOFFICE_SUCCESS_SENDMONEY			702 // 송금 성공

#define SRVAL_POSTOFFICE_FAIL_SEND_NOTENOUGHMONEY	711 // 전송 실패 - 소지금 부족
#define SRVAL_POSTOFFICE_FAIL_SEND_NOTENOUGHFEE		712 // 전송 실패 - 수수료 부족  
#define SRVAL_POSTOFFICE_FAIL_SEND_WRONGNAME		713 // 전송 실패 - 수신자 아이디가 잘못됨
#define SRVAL_POSTOFFICE_FAIL_SEND_FULLRECVBOX		714 // 전송 실패 - 수신함 꽉참
#define SRVAL_POSTOFFICE_FAIL_SEND_SAMEPERSON		715 // 전송 실패 - 자기 자신에게는 보낼 수 없음
#define SRVAL_POSTOFFICE_FAIL_SEND_USEDATEOVER		716 // 전송 실패 - 사용기간이 끝난 아이템 파괴
#define SRVAL_POSTOFFICE_FAIL_SEND_NOTRADEITEM		717	// 전송 실패 - 거래불가 아이템
#define SRVAL_POSTOFFICE_FAIL_SEND_NOTPOSTOFFICE	718	// 전송 실패 - (길드)현재 위치한 마을에 역참이 없음
//KHY - 0614 - 가차 아이템은 같은 계정간 캐릭터에 한하여 우편으로 주고 받을 수 있다. 
#define SRVAL_POSTOFFICE_FAIL_SEND_ANOTHERPERSON	719	// 전송 실패 - 다른 계정의 캐릭터에게 보내려 햇다.


#define SRVAL_POSTOFFICE_SUCCESS_GETITEM			720 // 아이템 수신 성공 
#define SRVAL_POSTOFFICE_FAIL_GETITEM_FULLINVEN		721 // 아이템 수신 실패 - 인벤토리 다 찼음
#define SRVAL_POSTOFFICE_FAIL_GETITEM_USEDATEOVER	722	// 아이템 수신 실패 - 아이템 사용기간 끝남
#define SRVAL_POSTOFFICE_SUCCESS_GETMONEY			725 // 현금 받기 성공
#define SRVAL_POSTOFFICE_FAIL_GETMONEY				726 // 현금 받기 실패 

#define SRVAL_POSTOFFICE_SUCCESS_SETFEE				730 // 수수료 설정 성공
#define SRVAL_POSTOFFICE_FAIL_SETFEE				731 // 수수료 설정 실패 

#define SRVAL_GET_SAVEUSERITEM_SUCCESS				732	// 봉이 김선달에게서 아이템 얻기 성공
#define SRVAL_GET_SAVEUSERITEM_FAIL					733	// 봉이 김선달에게서 아이템 얻기 실패



// 포구
#define SRVAL_PORT_FAILAPPLY_NOACCOUNT				800 // 전장에 계좌가 없어서 포구 임대 실패
#define SRVAL_PORT_FAILSETCONTRACT_NOTCHIEF			801 // 포구 행수만이 전장 계좌 변경 가능
#define SRVAL_PORT_SUCCESSSETCONTRACT				802 // 계약 조건 변경 성공
#define SRVAL_PORT_FAILNOTSAMECONTRACT				803 // 임차 조건이 틀림
#define SRVAL_PORT_FAILEXTENDCONTRACT_NOMONEY		804 // 전장 잔고 부족으로 연장 계약 안됨
#define SRVAL_PORT_SUCCESSPORTRENT					805 // 포구 임대 성공


// 사복시 
#define SRVAL_HORSEMARKET_SUCCESS_SELLHORSE			900 // 말 팔자 주문 성공 
#define SRVAL_HORSEMARKET_FAIL_SELL					901 // 말 팔자 주문 실패
#define SRVAL_HORSEMARKET_FAIL_SELL_ALREADY			902 // 말 팔자 주문 실패 - 이미 주문 한 말이 있음 
#define SRVAL_HORSEMARKET_FAIL_SELL_NOTEMPTYINVEN	903 // 말 팔자 주문 실패 - 인벤토리에 물품이 있음 
#define SRVAL_HORSEMARKET_FAIL_SELL_NOTHAVE			904 // 말 팔자 주문 실패 - 보유하고 있는 말 없음 
#define SRVAL_HORSEMARKET_FAIL_SELL_NOTENOUGHMONEY	905 // 말 팔자 주문 실패 - 세금으로 지불할 돈 없음
#define SRVAL_HORSEMARKET_FAIL_SELL_CANNOTTRADE		906 // 말 팔자 주문 실패 - 거래가 불가능한 말

#define SRVAL_HORSEMARKET_SUCCESS_GETBACK			910 // 말 회수 성공 
#define SRVAL_HORSEMARKET_FAIL_GETBACK				911 // 말 회수 실패
#define SRVAL_HORSEMARKET_FAIL_GETBACK_HAVEANOTHER	912 // 말 회수 실패 - 현재 보유하고 있는 말 있음 
#define SRVAL_HORSEMARKET_FAIL_GETBACK_NOORDER		913 // 말 회수 실패 - 주문 없음 

#define SRVAL_HORSEMARKET_SUCCESS_BUYHORSE			920 // 말 구입 성공 
#define SRVAL_HORSEMARKET_FAIL_BUY					921 // 말 구입 실패 
#define SRVAL_HORSEMARKET_FAIL_BUY_NOTENOUGHMONEY	922 // 말 구입 실패 - 현금 부족 
#define SRVAL_HORSEMARKET_FAIL_BUY_HAVEANOTHER		923 // 말 구입 실패 - 현재 보유하고 있는 말 있음 
#define SRVAL_HORSEMARKET_FAIL_BUY_NOTSELF			924 // 말 구입 실패 - 자신이 주문한 말은 구입할 수 없다
#define SRVAL_HORSEMARKET_FAIL_BUY_NOTVALID			925 // 말 구입 실패 - 유효한 주문이 아닙니다

#define SRVAL_HORSEMARKET_SUCCESS_SETFEE				930 // 수수료 설정 성공
#define SRVAL_HORSEMARKET_FAIL_SETFEE					931 // 수수료 설정 실패 

// 거간
#define SRVAL_REALESTATEMARKET_SUCCESS_SELL				950 // 부동산 팔자 주문 성공 
#define SRVAL_REALESTATEMARKET_FAIL_SELL				951 // 부동산 팔자 주문 실패 
#define SRVAL_REALESTATEMARKET_FAIL_SELL_NOTENOUGHMONEY	952 // 부동산 팔자 주문 실패 - 세금으로 지불할 돈 없음
#define SRVAL_REALESTATEMARKET_FAIL_SELL_NOTVALID		953 // 부동산 팔자 주문 실패 - 실제 보유 부동산 아님 

#define SRVAL_REALESTATEMARKET_SUCCESS_CANCELORDER		955 // 부동산 주문 취소 성공 
#define SRVAL_REALESTATEMARKET_FAIL_CANCELORDER			956	// 부동산 주문 취소 실패 

#define SRVAL_REALESTATEMARKET_FAIL_SELL_HAVENOTRADE	957 // 부동산 팔자 주문 실패 - 거래불가아이템이 있음
#define SRVAL_REALESTATEMARKET_FAIL_SELL_HAVENOTRADEHORSE	958 // 부동산 팔자 주문 실패 - 거래불가기승동물이 있음

#define SRVAL_REALESTATEMARKET_SUCCESS_BUY				960 // 부동산 구입 성공 
#define SRVAL_REALESTATEMARKET_FAIL_BUY					961 // 부동산 구입 실패 
#define SRVAL_REALESTATEMARKET_FAIL_BUY_NOTENOUGHMONEY	962 // 부동산 구입 실패 - 현금 부족 
#define SRVAL_REALESTATEMARKET_FAIL_BUY_NOMORE			963 // 부동산 구입 실패 - 더 이상 부동산을 보유할 수 없음
#define SRVAL_REALESTATEMARKET_FAIL_BUY_NOTVALID		964 // 부동산 구입 실패 - 유효한 주문 아님 
#define SRVAL_REALESTATEMARKET_FAIL_BUY_NOTSELF			965 // 부동산 구입 실패 - 자신이 주문한 부동산은 구입할 수 없다

#define SRVAL_REALESTATEMARKET_SUCCESS_SETFEE			970 // 수수료 설정 성공
#define SRVAL_REALESTATEMARKET_FAIL_SETFEE				971 // 수수료 설정 실패 

#define SRVAL_STOCKOFFERING_SUCCESS_ORDER				980	
#define SRVAL_STOCKOFFERING_FAIL_ORDERALREADY			981
#define SRVAL_STOCKOFFERING_FAIL_ORDER					982
#define SRVAL_STOCKOFFERING_FAIL_ORDERMONEY				983
#define SRVAL_STOCKOFFERING_FAIL_ORDERLIMIT				984



// 장예원
#define SRVAL_SUMMONMARKET_SUCCESS_SELL					1000 // 소환수 팔자 주문 성공 
#define SRVAL_SUMMONMARKET_FAIL_SELL					1001 // 소환수 팔자 주문 실패 
#define SRVAL_SUMMONMARKET_FAIL_SELL_NOMORE				1002 // 소환수 팔자 주문 실패 - 더이상 주문할 수 없음
#define SRVAL_SUMMONMARKET_FAIL_SELL_NOTHAVE			1003 // 소환수 팔자 주문 실패 - 보유하고 있는 소환수 없음 
#define SRVAL_SUMMONMARKET_FAIL_SELL_NOTENOUGHMONEY		1004 // 소환수 팔자 주문 실패 - 세금으로 지불할 돈 없음
#define SRVAL_SUMMONMARKET_FAIL_SELL_HAVEITEM			1009 // 소환수 팔자 주문 실패 - 소환수가 물품을 장비하고 있음 

#define SRVAL_SUMMONMARKET_SUCCESS_CANCELORDER			1005 // 소환수 주문 취소 성공 
#define SRVAL_SUMMONMARKET_FAIL_CANCELORDER				1006 // 소환수 주문 취소 실패 
#define SRVAL_SUMMONMARKET_FAIL_CANCELORDER_NOMORE		1007 // 소환수 주문 취소 실패 - 더이상 보유할 수 없음 
#define SRVAL_SUMMONMARKET_FAIL_CANCELORDER_NOTVALID	1008 // 소환수 주문 취소 실패 - 유효주문 아님

#define SRVAL_SUMMONMARKET_SUCCESS_BUY					1010 // 소환수 구입 성공 
#define SRVAL_SUMMONMARKET_FAIL_BUY						1011 // 소환수 구입 실패 
#define SRVAL_SUMMONMARKET_FAIL_BUY_NOTENOUGHMONEY		1012 // 소환수 구입 실패 - 현금 부족 
#define SRVAL_SUMMONMARKET_FAIL_BUY_NOMORE				1013 // 소환수 구입 실패 - 더이상 보유할 수 없음
#define SRVAL_SUMMONMARKET_FAIL_BUY_NOTSELF				1014 // 소환수 구입 실패 - 자신이 주문한 소환수는 구입할 수 없다 
#define SRVAL_SUMMONMARKET_FAIL_BUY_NOTVALID			1015 // 소환수 구입 실패 - 유효한 주문 아님 

#define SRVAL_SUMMONMARKET_FAIL_SELL_USER_NO_BANK		1016 // 소환수 팔자 은행이 없어서 주문 실패.

#define SRVAL_SUMMONMARKET_SUCCESS_SETFEE				1020 // 수수료 설정 성공
#define SRVAL_SUMMONMARKET_FAIL_SETFEE					1021 // 수수료 설정 실패 

#define SRVAL_SUMMONMARKET_FAIL_SELECTSELL_WRONGNAME		1022	// 지정거래 팔자 주문시 아이디 잘못됨
#define SRVAL_SUMMONMARKET_FAIL_BUY_NOTSELECTPERSON			1023	// 지정거래 지정 사용자가 아님 구입 불가
#define SRVAL_SUMMONMARKET_FAIL_SELECTSELL_NOTHAVETICKET	1024	// 지정거래 이용권 없음
#define SRVAL_SUMMONMARKET_FAIL_SELECTSELL_WRONGPRICE		1025	// 지정거래 이조판서가 정한 최저 가격 보다 낮음

// 농장
#define SRVAL_LAND_SUCCESS_APPLY					1201	// 임차 신청 성공. 
#define SRVAL_LAND_FAIL_APPLY						1202	// 임차 신청 실패 - 범용. 
#define SRVAL_LAND_FAIL_APPLY_NOACCOUNT				1203	// 임차 신청 실패 - 임차인의 전장 계좌 없음. 
#define SRVAL_LAND_FAIL_APPLY_ALREADYAPPLY			1204	// 임차 신청 실패 - 이미 신청했음. 

#define SRVAL_LAND_SUCCESS_CANCELAPPLY				1210	// 임차 취소 성공. 
#define SRVAL_LAND_FAIL_CANCELAPPLY					1211	// 임차 취소 실패 

#define SRVAL_LAND_SUCCESS_SETCONTRACT				1221	// 계약 조건 변경 성공. 
#define SRVAL_LAND_FAIL_SETCONTRACT					1222	// 
#define SRVAL_LAND_FAIL_SETCONTRACT_WRONGFEE		1223	// 계약 조건 변경 실패. - 임대료 변경폭 제한 
#define SRVAL_LAND_FAIL_SETCONTRACT_DATE			1224	// 계약 조건 변경 실패. - 한달에 한번만 변경 가능  

#define SRVAL_LAND_SUCCESS_RENT						1230	// 임대성공. 
#define SRVAL_LAND_FAIL_RENT						1231	// 임대 실패
#define SRVAL_LAND_FAIL_NOTSAMECONTRACT				1232	// 임대 실패 - 임차인의 계약 조건과 다르다.
#define SRVAL_LAND_FAIL_NOMOREREALESTATE			1233	// 임대 실패 - 더 이상의 부동산은 보유할 수 없다. 
#define SRVAL_LAND_FAIL_RENT_NOTEMPTY				1234	// 임대 실패 -
#define SRVAL_LAND_FAIL_RENT_INVALIDPERSONID		1235	// 임대 실패 -
#define SRVAL_LAND_SUCCESS_DELLANDAPPLYER			1240	// 임차 희망자 명단 삭제 성공 

// 내자시
#define SRVAL_FEAST_SUCCESS_RESERVE					1260	// 연회 예약 성공 
#define SRVAL_FEAST_FAIL_RESERVE					1261	// 연회 예약 실패 
#define SRVAL_FEAST_FAIL_RESERVE_NOTENOUGHMONEY		1262	// 연회 비용 부족 
#define SRVAL_FEAST_FAIL_RESERVE_ALREADY			1263	// 예약자가 있음 
#define SRVAL_FEAST_FAIL_RESERVE_DATE				1264	// 현재 날짜로 부터 1일(현실시간) 이내의 시간으로만 예약가능 
#define SRVAL_FEAST_FAIL_RESERVE_OPEN				1265	// 연회가 진행중임
#define SRVAL_FEAST_FAIL_RESERVE_ITEM				1266	// 물품이 준비되지 않음
#define SRVAL_FEAST_FAIL_NOCHANGE					1267	// 예약자가 있거나 연회중일때는 변경 불가
#define SRVAL_FEAST_SUCCESS_SETCOST					1268	// 연회비용 설정 성공
#define SRVAL_FEAST_FAIL_SETCOST					1269	// 연회비용 설정 실패
#define SRVAL_FEAST_SUCCESS_GIVEITEM				1270	// 연회 물품 지급 성공
#define SRVAL_FEAST_FAIL_GIVEITEM					1271	// 연회 물품 지급 실패
#define SRVAL_FEAST_FAIL_GIVEITEM_WRONGPERSON		1272	// 연회 예약자 아님
#define SRVAL_FEAST_FAIL_GIVEITEM_NOTOPEN			1273	// 연회중 아님
#define SRVAL_FEAST_FAIL_GIVEITEM_ALREADY			1274	// 이미 지급한 물품임
#define SRVAL_FEAST_FAIL_ITEM_WRONG					1275	// 등록할 수 없는 아이템임
#define SRVAL_FEAST_FAIL_ITEM_ALREADY				1276	// 이미 등록된 아이템임

//#if defined(_LEON_FEAST_EFFECT)
	#define SRVAL_FEAST_FAIL_USEEFFECT_NOTHAVETICKET		1277	// 폭죽이용권 없음
	#define SRVAL_FEAST_SUCCESS_USEEFFECT					1278	// 폭죽이용권 사용 성공
//#endif

#define SRVAL_FEAST_FAIL_ITEM_TOOMANY				1279	// 아이템 갯수가 너무 많음

// 비변사
#define SRVAL_SUMMONHEROMARKET_SUCCESS_SELL				1300 // 소환영웅 팔자 주문 성공 
#define SRVAL_SUMMONHEROMARKET_FAIL_SELL				1301 // 소환영웅 팔자 주문 실패 
#define SRVAL_SUMMONHEROMARKET_FAIL_SELL_NOMORE			1302 // 소환영웅 팔자 주문 실패 - 더이상 주문할 수 없음
#define SRVAL_SUMMONHEROMARKET_FAIL_SELL_NOTHAVE		1303 // 소환영웅 팔자 주문 실패 - 보유하고 있는 소환영웅 없음 
#define SRVAL_SUMMONHEROMARKET_FAIL_SELL_NOTENOUGHMONEY	1304 // 소환영웅 팔자 주문 실패 - 세금으로 지불할 돈 없음
#define SRVAL_SUMMONHEROMARKET_FAIL_SELL_HAVEITEM		1309 // 소환영웅 팔자 주문 실패 - 소환영웅가 물품을 장비하고 있음 

#define SRVAL_SUMMONHEROMARKET_SUCCESS_CANCELORDER		1305 // 소환영웅 주문 취소 성공 
#define SRVAL_SUMMONHEROMARKET_FAIL_CANCELORDER			1306 // 소환영웅 주문 취소 실패 
#define SRVAL_SUMMONHEROMARKET_FAIL_CANCELORDER_NOMORE	1307 // 소환영웅 주문 취소 실패 - 더이상 보유할 수 없음 
#define SRVAL_SUMMONHEROMARKET_FAIL_CANCELORDER_NOTVALID	1308 // 소환영웅 주문 취소 실패 - 유효주문 아님

#define SRVAL_SUMMONHEROMARKET_SUCCESS_BUY				1310 // 소환영웅 구입 성공 
#define SRVAL_SUMMONHEROMARKET_FAIL_BUY					1311 // 소환영웅 구입 실패 
#define SRVAL_SUMMONHEROMARKET_FAIL_BUY_NOTENOUGHMONEY	1312 // 소환영웅 구입 실패 - 현금 부족 
#define SRVAL_SUMMONHEROMARKET_FAIL_BUY_NOMORE			1313 // 소환영웅 구입 실패 - 더이상 보유할 수 없음
#define SRVAL_SUMMONHEROMARKET_FAIL_BUY_NOTSELF			1314 // 소환영웅 구입 실패 - 자신이 주문한 소환영웅는 구입할 수 없다 
#define SRVAL_SUMMONHEROMARKET_FAIL_BUY_NOTVALID		1315 // 소환영웅 구입 실패 - 유효한 주문 아님 


#define SRVAL_SUMMONHEROMARKET_SUCCESS_SETFEE				1320 // 수수료 설정 성공
#define SRVAL_SUMMONHEROMARKET_FAIL_SETFEE					1321 // 수수료 설정 실패 

#define SRVAL_SUMMONHEROMARKET_FAIL_SELECTSELL_WRONGNAME		1322	// 지정거래 팔자 주문시 아이디 잘못됨
#define SRVAL_SUMMONHEROMARKET_FAIL_BUY_NOTSELECTPERSON			1323	// 지정거래 지정 사용자가 아님 구입 불가
#define SRVAL_SUMMONHEROMARKET_FAIL_SELECTSELL_NOTHAVETICKET	1324	// 지정거래 이용권 없음
#define SRVAL_SUMMONHEROMARKET_FAIL_SELECTSELL_WRONGPRICE		1325	// 지정거래 이조판서가 정한 최저 가격 보다 낮음





#define SRVAL_STRMSG_SUCCESS						1450	// 건물 안내문 설정 성공

// 세금 
#define SRVAL_TAX_SUCCESS_SETTAX					1480 // 세금 설정 성공 
#define SRVAL_TAX_SUCCESS_PAYTAX					1490 // 세금 납부 성공 
#define SRVAL_TAX_FAIL_PAYTAX_NOTENOUGHMONEY		1491 // 세금 납부 실패 - 시설에 보유자금이 충분하지 않음
#define SRVAL_TAX_FAIL_PAYTAX_NODURA				1492 // 세금 납부 실패 - 건물 내구도 없음
#define SRVAL_TAX_FAIL_WRONGMONEY					1493 // 세금 납부 실패 - 10만냥 미만
#define SRVAL_TAX_FAIL_DATE							1494 // 세금 납부 실패 - 한달에 한번만 납부 가능

// 배당 
#define SRVAL_DIVIDEND_SUCCESS_PAYDIVIDEND			1495 // 배당 성공
#define SRVAL_DIVIDEND_FAIL_WRONGMONEY				1496 // 배당 실패 - 배당금이 적절하지 않음
#define SRVAL_DIVIDEND_FAIL_NOTENOUGHMONEY			1497 // 배당 실패 - 상단에 보유자금이 충분하지 않음 
#define SRVAL_DIVIDEND_FAIL_DATEERROR				1498 // 배당 실패 - 배당일이 적절하지 않음


// 로그인 
#define SRVAL_MAKEPERSON_FAIL_DUPNAME				1500	// 캐릭터 생성 실패. - 중복된 캐릭터. 
#define SRVAL_DELPERSON_FAIL_UNKNOWN				1510	// 캐릭터 지우기 실패 
#define SRVAL_DELPERSON_FAIL_IDMATCHERROR			1520	// 캐릭터 지우기 실패 - 주민등록번호 불일치 
#define SRVAL_LOGINFAIL_LIMITMAXUSER				1221	// 접속제한이 걸려서 로그인 실패

#define SRVAL_NOTCORRECTIP							1530	// 접속불가능한 IP

//KHY - 0910 - 클라이언트로 캐릭터형태의 리턴값을 보낸다.
// 특정 웹주소값을 보내어, 상태를 확인하게 한다.
#define SRVAL_CHECK_WEBVAL							1531	// 접속불가능한 IP

// 퀘스트. 
#define SRVAL_QUEST_FAIL_FULLINV					1600 // 퀘스트 물품 수령 실패 - 인벤이 꽉참. 


// DailyQuest
#define SRVAL_DAILYQUEST_FAIL_NOTEXISTLIST			1650	// DailyQuest 에서 리스트 선택한 것이 리스트에 없을 경우
// DailyQuest2
#define SRVAL_DAILYQUEST2_FAIL_NOTEXISTLIST			1650	// DailyQuest 에서 리스트 선택한 것이 리스트에 없을 경우

// 물품 생산 관련 
#define SRVAL_NOTENOUGHTOOLDURABILITY				1700	// 도구의 내구도가 부족하여 물품 생산 실패. 
#define SRVAL_TOOMANYITEMMAKE						1701	// 한번에 1만개 이상의 물품은 생산 불가. 

// 공간 이동. 
#define SRVAL_NOTENOUGHLEVELTOMAP					1800	// 레벨이 낮아 지도를 옮길 수 없다. 
#define SRVAL_TOOHIGHLEVEL							1801	// 
#define SRVAL_FAIL_WARP_LIMITTIME					1802	// 순간이동은 3초마다 한번만
#define SRVAL_TOOHIGHLEVEL_SANTA					1803	// 산타작업장은 레벨제한
#define SRVAL_FAIL_WARP_LEVEL_RANGE					1804	// 입장 레벨에 벗어나 입장 불가
#define SRVAL_FAIL_WARP_NOITEM						1805	// 입장에 필요한 아이템이 없어서 입장 불가
#define SRVAL_FAIL_WARP_BLOCKWARP					1806	// 워프가 금지되어 있는 맵이다.
#define SRVAL_FAIL_FORBID_ENTERMAP					1807	// 들어갈 수 없는 맵이다.
#define SRVAL_FAIL_FORBID_BATTLE					1808	// 공성전 중에는 친구에게 이동이 불가능 합니다.
#define SRVAL_FAIL_RACCOONWORLD						1809	// 라쿤월드는 워프로 들어갈수 없음
//#define SRVAL_FAIL_WARP_RETURNTOWER					1809	// [지연/090515] - 귀환탑 설치해야만 워프 가능하다는 메세지.

// 파티로만 이동가능
#define SRVAL_NOPARTY								1850	// 파티로만 이동가능
#define SRVAL_NOINSTANCEMAP							1851	// 더이상 인스턴스 던전이 없다.
#define SRVAL_MAKE_BOSSMONSTER						1852	// 보스 몬스터가 만들어졌다
#define SRVAL_CLEAR_INSTANCEMAP						1853	// 인스턴스 던전 클리어
#define SRVAL_FAIL_TIMEOUT							1854	// 던전 시간 초과로 실패

// 전쟁 
#define SRVAL_NOWARSEASON							1900	// 전쟁시에만 이용할 수 있다. 
#define SRVAL_OUTOFWARKILL							1901	// 전공이 부족하여 이용할 수 없다. 
#define SRVAL_AUTOWAR								1902	// 자동전쟁. 

#define SRVAL_BLACKARMY_MAPPOSITION					1910	// 흑의군단 전쟁 포지션 리턴
#define SRVAL_BLACKARMY_NOTALLOC					1911	// 해당하는 맵이 아님
#define SRVAL_BLACKARMY_NOTWAR						1912	// 전쟁시간 아님
#define SRVAL_BLACKARMY_WARAPPLY					1913	// 전쟁신청 (1 : 가능, 2 : 불가능) [GM커맨드 응답]
#define SRVAL_BLACKARMY_FAILALLOC					1914	// 전쟁맵 배정 실패

#define SRVAL_WENNYOLDMAN_FULLINVEN					1950	// 혹부리(2호식) 사용자 인벤이 꽉차서 분해한 아이템 수령 실패
#define SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_FULLINVEN	1951	// 혹부리(2호식) 인벤이 꽉차서 분해 실패
#define SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_UNLUCK	1952	// 혹부리(2호식) 확률로 인하여 분해 실패
#define SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_LOWPRICE	1953	// 혹부리(2호식) 낮은가격은 분해 불가
#define SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_CANNOTITEM	1954	// 혹부리(2호식) 분해 불가 아이템

#define SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN		1960	// 보물상자 인벤이 꽉차서 상자열기 실패
#define SRVAL_FEAST_FAIL_USE_ONLY_VILLAGE			1961	// 폭죽 아이템은 마을안에서만 사용가능하다.

// [성웅] 레벨 보상 상자에서 아이템을 준다 
#define SRVAL_FAIL_OPEN_LEVELLIMTBOX_NOT_OVER_LEVEL		1980	//	상자를 열수 있는 레벨이 안된다 
#define SRVAL_FAIL_OPEN_LEVELLIMTBOX_NOT_NOT_ENOUGH_INV	1981	//	인벤토리 공간이 부족 하다


#define SRVAL_HIDDENQUEST_BOX						10100	// 히든 퀘스트

// 고구려 유적 방어 이베트 
#define SRVAL_GOGUWARPFAIL_NOTVILLAGE				2000	// 고구려 유적지로 이동 불가. - 이벤트 마을 주민이 아님. 
#define SRVAL_GOGUWARPFAIL_NOEVENTTIME				2001	// 고구려 유적지로 이동 불가. - 해당 이벤트 시간이 아님. 

// 오사카 전쟁
#define SRVAL_NOOSAKAWARSEASON						2010	// 전쟁시에만 이용할 수 있다. 

#define SRVAL_NOWARP_TRADEPORT_VILLAGE				2015	// 권한있는 마을 주민이 아니다.
#define SRVAL_INSTANCEPROFIT_VILLAGE				2016	// 인스턴스 던전 이익금 가져갔다.

// 강화도 방어 이베트 
#define SRVAL_KANGHWADOWARPFAIL_NOTVILLAGE			2020	// 강화도 이동 불가. - 이벤트 마을 주민이 아님. 
#define SRVAL_KANGHWADOWARPFAIL_NOEVENTTIME			2021	// 강화도 이동 불가. - 해당 이벤트 시간이 아님. 

// 모후모후 이벤트
#define SRVAL_MOFUMOFUEVENT_START_LEFTTIME			2030	// %d분 남았습니다. 모후모후 놀이공원 앞으로 모이세요. (자동이동료 1000Q)
#define SRVAL_MOFUMOFUEVENT_FAIL_ALREADY_RESERVE	2031	// 이미 신청된 시간입니다. 다른 시간을 선택하여 주십시요.
#define SRVAL_MOFUMOFUEVENT_FAIL_RESERVE			2032	// 이벤트 예약에 실패하였습니다.
#define SRVAL_MOFUMOFUEVENT_SUCCESS_SETENEMYCOUNT	2033	// 출몰 몬스터 숫자 바꾸기 성공
#define	SRVAL_MOFUMOFUEVENT_FAIL_SETENEMYCOUNT		2034	

// 해적선 이벤트
#define	SRVAL_PIRATESHIP_APPEAR						2040	// 해적선 나타남

#define SRVAL_PUCHIKOMI_FAIL_OP_REJECTION			2101	// 푸치코미 - 상대방이 공개 거부중

// 광산
#define SRVAL_MINE_SUCCESS_APPLY					2201	// 임차 신청 성공. 
#define SRVAL_MINE_FAIL_APPLY						2202	// 임차 신청 실패 - 범용. 
#define SRVAL_MINE_FAIL_APPLY_NOACCOUNT				2203	// 임차 신청 실패 - 임차인의 전장 계좌 없음. 
#define SRVAL_MINE_FAIL_APPLY_ALREADYAPPLY			2204	// 임차 신청 실패 - 이미 신청했음. 

#define SRVAL_MINE_SUCCESS_CANCELAPPLY				2210	// 임차 취소 성공. 
#define SRVAL_MINE_FAIL_CANCELAPPLY					2211	// 임차 취소 실패 

#define SRVAL_MINE_SUCCESS_SETCONTRACT				2221	// 계약 조건 변경 성공. 
#define SRVAL_MINE_FAIL_SETCONTRACT					2222	// 
#define SRVAL_MINE_FAIL_SETCONTRACT_WRONGFEE		2223	// 계약 조건 변경 실패. - 임대료 변경폭 제한 
#define SRVAL_MINE_FAIL_SETCONTRACT_DATE			2224	// 계약 조건 변경 실패. - 한달에 한번만 변경 가능  

#define SRVAL_MINE_SUCCESS_RENT						2230	// 임대성공. 
#define SRVAL_MINE_FAIL_RENT						2231	// 임대 실패
#define SRVAL_MINE_FAIL_NOTSAMECONTRACT				2232	// 임대 실패 - 임차인의 계약 조건과 다르다.
#define SRVAL_MINE_FAIL_NOMOREREALESTATE			2233	// 임대 실패 - 더 이상의 부동산은 보유할 수 없다. 
#define SRVAL_MINE_FAIL_RENT_NOTEMPTY				2234	// 임대 실패 -
#define SRVAL_MINE_FAIL_RENT_INVALIDPERSONID		2235	// 임대 실패 -
#define SRVAL_MINE_SUCCESS_DELMINEAPPLYER			2240	// 임차 희망자 명단 삭제 성공 

#define SRVAL_CITYHALL_FAIL_CONSTRUCT_NOTMINE			2250
#define SRVAL_CITYHALL_FAIL_CONSTRUCT_NOTENOUGHMONEY	2251

//----------------------------------
// 길드
//----------------------------------
#define SRVAL_GUILD_SUCCESS_CREATE					2300	// 단위길드 생성 성공
#define SRVAL_GUILD_FAIL_CREATE						2301	// 단위길드 생성 실패
#define SRVAL_GUILD_FAIL_WRONG_NAME					2302	// 단위길드명 오류
#define SRVAL_GUILD_FAIL_FULLSLOT					2303	// 단위길드 생성 오류 - 꽉찼음
#define SRVAL_GUILD_FAIL_ALREADYMEMBER				2304	// 단위길드 생성 오류 - 이미 다른 길드 멤버임
#define SRVAL_GUILD_FAILSETFEE						2305	// 월 유지비 설정 실패. 
#define SRVAL_GUILD_FAILSETFEERATE					2306	// 수수료율 설정 실패. 
#define SRVAL_GUILD_SUCCESSSETCONTRACT				2307	// 월 유지비 및 수수료율 설정 성공. 
#define SRVAL_GUILD_FAILSETCONTRACT_NOTCHIEF		2308	// 계약 조건 변경 실패. - 상단행수가 아님. 
#define SRVAL_GUILD_FAILSETCONTRACT_WRONGFEE		2309	// 월 유지비 인상 폭 오류 
#define SRVAL_GUILD_FAILSETCONTRACT_DATE			2310	// 계약 조건 변경 실패. - 한달에 한번만 변경 가능  
#define SRVAL_GUILD_FAILSETCONTRACT					2311	// 월 유지비 및 수수료율 설정 실패 
#define SRVAL_GUILD_FAIL_GETGUILDRANKLIST			2312	// 길드 랭킹 리스트를 얻는데 실패
#define SRVAL_GUILD_FAIL_NOTENOUGHMONEY				2313	// 소지금이 부족함
#define SRVAL_GUILD_FAIL_NOTCONNECTMASTER			2314	// 길드장의 부재 신고 실패

#define SRVAL_GUILDWAR_FAILAPPLY_DENY				2315	// 상대방 길드에서 거부함
#define SRVAL_GUILDWAR_FAILAPPLY_ALREADY			2316	// 이미 다른 길드에서 예약했음
#define SRVAL_GUILDWAR_FAILAPPLY_LOWLEVEL			2317	// 레벨이 낮음.6692
#define SRVAL_GUILDWAR_FAILAPPLY_NOTCONNECTED		2318	// 상대방 마스터가 접속하지 않음
#define SRVAL_GUILDWAR_FAILAPPLY_FAULTNAME			2319	// 없는 길드!6690
#define SRVAL_GUILDWAR_FAILAPPLY_MYGUILD			2320	// 내길드 6691
#define SRVAL_GUILDWAR_FAILAPPLY_IM_ALREADYRESERVE	2321	// 내 길드가 같은 시각에 이미 예약중이다
#define SRVAL_GUILDWAR_FAILAPPLY_ENEMY_ALREADYRESERVE	2322 // 상대 길드가 같은 시각에 이미 예약중이다
#define SRVAL_GUILDWAR_FAILAPPLY_NOTALLOWSERVER		2323	// 길드전 허용하지 않는 서버
#define SRVAL_GUILDWAR_FAILAPPLY_OVERTIME			2324	// 하루에 길드전은 3회이상 할수 없다.

#define SRVAL_GUILDDUNGEON_FAIL_REG					2400	// 길드사냥터에 이미 등록되어 있어서 실패

#define SRVAL_PAYFEE_SUCCESS						3900 // 수수료 납부 성공
#define SRVAL_PAYFEE_FAIL_NOTENOUGHMONEY			4001 // 수수료 납부 실패 - 계좌에 돈이 충분하지 않음

// 스승인. 
#define SRVAL_NOLISTINFATHERAPPLYER					5000 // 상대가 스승 등록 신청을 취소해서 스승으로 등록할 수 없다. 
#define SRVAL_SUCCESSAPPLYFATHER					5010 // 스승 등록 신청 성공. 
#define SRVAL_FAILAPPLYFATHER_ALREADY				5020 // 이미 스승 등록 신청되어 있어서 등록 실패. 
#define SRVAL_FAILAPPLYFATHER_FULL					5030 // 스승 등록 신청이 꽉차서 더이상 등록 불가. 
#define SRVAL_SUCCESSDELFATHERAPPLY					5040 // 스승 등록 신청 취소 성공. 

#define	SRVAL_INITSTATUS							6000 // 스킬, 스테이터스 초기화 되다. 

// 주식 공모 
#define SRVAL_STOCKGIVEFAIL_FULL					7000	// 주식 공모시 10종목 이상의 주식을 이미 보유하고 있어 실패. 
#define SRVAL_STOCK_FAIL_DISTRIBUTIONSTOP			7010	// 주식 공모 신청한 마을이 현재 진행중이지 않음.
#define SRVAL_AUTOSTOCKDISTRIBUTION					7020	// 자동공모

// 공성전 
#define SRVAL_WAR_CANTENTERENEMYCASTLE				7100	// 공성전 중에는 적대적인 마을에 들어갈 수 없다. 

// 개인 거래
#define SRVAL_PRIVATETRADE_MONEY_INSUFFICIENCY		7300	// 돈 거래에서 돈이 부족
#define SRVAL_PRIVATETRADE_ITEM_INFO_DIFFERENT		7301	// 주식 거래에서 주식의 정보가 틀림
#define SRVAL_PRIVATETRADE_PERSON_INVENTORY_FULL	7302	// 아이템 거래에서 인벤에 빈곳이 없음
#define SRVAL_PRIVATETRADE_STOCK_INFO_DIFFERENT		7303	// 주식 거래에서 주식의 정보가 틀림
#define SRVAL_PRIVATETRADE_HOUSEUNIT_INFO_DIFFERENT	7304	// 시전 거래에서 시전의 정보가 틀림
#define SRVAL_PRIVATETRADE_HORSE_INFO_DIFFERENT		7305	// 말 거래에서 말의 정보가 틀림
#define SRVAL_PRIVATETRADE_HORSE_INVENTORY_NOTEMPTY 7306	// 말 거래에서 말 인벤이 비어 있지 않은 경우
#define SRVAL_PRIVATETRADE_HORSE_NAME_SAME			7307	// 말 거래에서 다 말의 이름이 같은 경우. 말 이름이 다른 경우에만 거래 가능함
#define SRVAL_PRIVATETRADE_HOUSEUNIT_LEVEL_LIMIT	7308	// 시전 거래에서 시전을 거래할 수 있는 최하 레벨이 되지 못한 경우.
#define SRVAL_PRIVATETRADE_HOUSEUNIT_EXCEED_NUMBER_PER_PERSON	7309	// 시전 개수 초과 가질 수 없음.
#define SRVAL_PRIVATETRADE_STOCK_TOOMANY			7310	// 주식 종류 개수 초과
#define SRVAL_PRIVATETRADE_HORSE_NOTEMPTY			7311	// 말을 더 이상 추가 불가
#define SRVAL_PRIVATETRADe_HORSE_CANNOTTRADE		7312	// 거래 불가능한 말임.

// 투표
#define SRVAL_VOTE_INIT								7450	// 투표 초기화 완료

// 친구
#define SRVAL_FRIEND_NOTCONNECTED					7500	// 친구로 추가할려는 사람이 현재 접속중이지 않다.

#define SRVAL_CITYHALL_SUCCESS_HIRENPC				8000	// 고용 성공 
#define SRVAL_CITYHALL_FAIL_HIRENPC_NOTENOUGHMONEY	8001	// 상단 자금 부족 
#define SRVAL_CITYHALL_FAIL_HIRENPC_VILLAGELEVEL	8002	// 필요 마을 레벨 부족
#define SRVAL_CITYHALL_FAIL_HIRENPC_BUILDSTRUCTURE	8003    // NPC슬롯에 건물이 지어져 있음
#define SRVAL_CITYHALL_FAIL_HIRECHRISTMAS_NOTENOUGHMONEY	8004	// 크리스마스 트리 건설 자금 부족

#define SRVAL_CITYHALL_FAIL_CONSTRUCT_NPCZONE		8014	// 건물을 지으려는 슬롯에 NPC가 고용되어있음 
#define SRVAL_CITYHALL_FAIL_CONSTRUCT_VILLAGELEVEL	8015	// 필요 마을 레벨 부족

#define SRVAL_CITYHALL_SUCCESS_VILLAGEMARK				8016	// 마을 마크 설정 성공
#define SRVAL_CITYHALL_FAIL_VILLAGEMARK_NOTENOUGHMONEY	8017	// 마을 마크 설정 실패 - 보유자금 부족
#define SRVAL_CITYHALL_FAIL_VILLAGEMARK_ALREADYUSED		8018	// 마을 마크 설정 실패 - 다른 마을에서 사용중 
#define SRVAL_CITYHALL_FAIL_VILLAGEMARK_DATE			8019	// 마을 마크 설정 실패 - 변경한지 한달이 지나지 않았음 

#define SRVAL_CITYHALL_SUCCESS_VILLAGEMAP				8020	// 마을 마크 설정 성공
#define SRVAL_CITYHALL_FAIL_VILLAGEMAP_NOTENOUGHMONEY	8021	// 마을 마크 설정 실패 - 보유자금 부족
#define SRVAL_CITYHALL_FAIL_VILLAGEMAP_LEVEL			8022	// 마을 마크 설정 실패 - 레벨이 낮음

#define SRVAL_CITYHALL_FAIL_RESIDENTSHARE_WRONGCOUNT		8100		// 주민 숫자 부족
#define SRVAL_CITYHALL_FAIL_RESIDENTSHARE_WRONGMONEY		8101		// 배당액 부적절
#define SRVAL_CITYHALL_FAIL_RESIDENTSHARE_NOTENOUGHMONEY	8102		// 상단 자금 부족
#define SRVAL_CITYHALL_FAIL_RESIDENTSHARE_WRONGTIME			8103		// 배당 가능 시간 아님 
#define SRVAL_CITYHALL_FAIL_RESIDENTSHARE_DATE				8104		// 건물 해체 후 1달이내 배당 불가 

#define SRVAL_GENERALMEETING_SUCCESS_SUGGEST				8200		// 주주총회를 성공적으로 발의 
#define SRVAL_GENERALMEETING_FAIL_SUGGEST					8201		// 주주총회 발의 실패 
#define SRVAL_GENERALMEETING_FAIL_SUGGEST_ALEADY			8202		// 주주총회가 이미 진행 중이다 
#define SRVAL_GENERALMEETING_FAIL_SUGGEST_NOTENOUGHSTOCK	8203		// 발의자의 주식이 부족하다 
#define SRVAL_GENERALMEETING_FAIL_SUGGEST_WRONGCANDIDATE	8204		// 대행수 후보자 명이 잘못 되었다 
#define SRVAL_GENERALMEETING_FAIL_SUGGEST_ALREDYCANDIDATE	8205		// 이미 다른마을의 대행수 후보로 등록되어 있다
#define SRVAL_GENERALMEETING_FAIL_SUGGEST_CHIEFLEVEL		8206		// 대행수 후보 레벨 제한 50

#define SRVAL_GENERALMEETING_SUCCESS_VOTE					8210		// 투표 성공 
#define SRVAL_GENERALMEETING_FAIL_VOTE						8211		// 투표 실패
#define SRVAL_GENERALMEETING_FAIL_VOTE_ALREADY				8212		// 투표 실패 - 이미 투표 하였음
#define SRVAL_GENERALMEETING_FAIL_VOTE_NOTVOTER				8213		// 투표 실패 - 투표권자가 아님

#define SRVAL_GENERALMEETING_SUCCESS_ACCEPTCANDIDATE		8220		// 대행수 후보자 본인동의 성공 
#define SRVAL_GENERALMEETING_FAIL_ACCEPTCANDIDATE			8221		// 대행수 후보자로 지정된 사람이 아니라서 실패

//---------------------------------------------------------------------------------------------
// PVP 관련
//---------------------------------------------------------------------------------------------
#define SRVAL_PVP_LEAGUE_APPLY_MESSAGE						8500		// PVP 무한대전 신청관련 메시지
#define SRVAL_PVP_LEAGUE_AGAIN_RESERVE						8501		// PVP 무한대전 다시 예약을 할것인지 물어본다

//---------------------------------------------------------------------------------------------
//가족관련 
//---------------------------------------------------------------------------------------------
#define SRVAL_FAMILY_ADD_ERROR_NO_CHAR						8600		// 가족 관련 메세지	-	잘못된 케릭터를 자식으로 등록 하려고 할때
#define SRVAL_FAMILY_ADD_WRONG_BEFORE_IN_FAMILY				8601		// 가족 관련 메세지	-	이미 가족이 있습니다
#define SRVAL_FAMILY_ADD_WRONG_NOTCONNECT_MATE				8602		// 가족 관련 메세지	-	부모가 모두 접속해야 합니다.
#define SRVAL_FAMILY_ADD_WRONG_No_TICKET					8603		// 가족 관련 메세지	-	티켓이 없다
#define SRVAL_FAMILY_ADD_WRONG_DEL_FAMILY					8604		// 가족 관련 메세지	-	// 가족에서 짤렸다.
#define SRVAL_FAMILY_ADD_WRONG_No_TICKET_PLUSDATE			8605		// 가족 관련 메세지	-	가족연장 이용권이 없다.
#define SRVAL_FAMILY_ADD_WRONG_DEL_HAVECHILD				8606		// 가족 관련 메세지	-	가족에서 탈퇴 실패 - 부모인데 가족이 있다.
#define SRVAL_FAMILY_ADD_FAMILY								8607		// 가족 관련 메세지	-	가족에 추가 하였다./
#define SRVAL_FAMILY_GONE_FAMILY							8608		// 가족 관련 메세지	-	가족에서탈퇴 하였다.

// 라쿤 피하기 이벤트
//Switch_10MonthEvent_RacoonDodge
#define SRVAL_RACOONDODGEEVENT_NOTEMPTYLISTPLACE			8700		// 빈자리가 없다
#define SRVAL_RACOONDODGEEVENT_RESERVEPERSON				8701		// 이미 신청 하였다.
#define SRVAL_RACOONDODGEEVENT_CLEAR						8702		// 클리어 하고 돈받는 메세지
#define SRVAL_RACOONDODGEEVENT_CANNOTRESERVE_INPLAY			8703		// 경기장안에서는 신청할수 없다
#define SRVAL_RACOONDODGEEVENT_NOTENOUGHMONEY				8704		// 충분한 돈이 없다

// 육조판서
#define SRVAL_MINISTER_FAIL_SETIZOINFO				9000	// 이조판서의 설정 실패 
#define SRVAL_MINISTER_FAIL_SETHOZOINFO				9001	// 호조판서의 설정 실패 
#define SRVAL_MINISTER_FAIL_SETYEZOINFO				9002	// 예조판서의 설정 실패 
#define SRVAL_MINISTER_FAIL_SETBYUNGZOINFO			9003	// 병조판서의 설정 실패 
#define SRVAL_MINISTER_FAIL_SETHYUNGZOINFO			9004	// 형조판서의 설정 실패 
#define SRVAL_MINISTER_FAIL_SETGONGZOINFO			9005	// 공조판서의 설정 실패 
#define SRVAL_MINISTER_FAIL_SETGOONZUINFO			9006	// 군주의	  설정 실패
#define SRVAL_MINISTER_SUCCESS_GETMINISTER			9007	// 운영자 명령어 getministerr로 각 판서들의 설정 가져오기 성공
#define SRVAL_MINISTER_FAIL_SETHYUNGZOPRISON		9008	// 형조판서의 죄수 설정 실패
#define SRVAL_MINISTER_FAIL_SETMINISTER				9009	// 군주 및 판서의 임명 실패
#define SRVAL_MINISTER_SUCCESS_SETMINISTER			9010	// 군주 및 판서의 임명 성공
#define SRVAL_MINISTER_FIRE_MINISTER				9011	// 군주 및 판서의 해임 통지 - 귀찮으니 그냥 에러박스
#define SRVAL_MINISTER_APPOINTMENT_SETMINISTER		9012	// 군주 및 판서의 임명 통지
#define SRVAL_MINISTER_FAIL_SETMINISTER_DATE		9013	// 군주가 판서 임명 날자때문에 실패
#define SRVAL_MINISTER_SUCCESS_SETLIQUIDATION		9014	// 군주가 퇴출 마을 설정 성공
#define SRVAL_MINISTER_FAIL_SETLIQUIDATION			9015	// 군주가 퇴출 마을 설정 실패
#define SRVAL_MINISTER_SUCCESS_SETBONUS				9016	// 군주가 보너스 타임 설정 성공
#define SRVAL_MINISTER_FAIL_SETBONUS				9017	// 군주가 보너스 타임 설정 실패
//KHY - 0629 - 해임 성공 통지 추가.
#define SRVAL_MINISTER_FIRE_SUCCESS_SETMINISTER		9018	// 군주 및 판서의 해임 성공



// 형조판서 및 운영자 - 감옥기능
#define SRVAL_PRISON_FAIL_NAMEERROR					9020	// 이름이 잘못 되었다.
#define SRVAL_PRISON_FAIL_TIMEERROR					9021	// 시간이 잘못 되었다.
#define SRVAL_PRISON_FAIL_REASONERROR				9022	// 죄명이 잘못 되었다.
#define SRVAL_PRISON_SUCCESS						9023	// 감옥 보내기 성공

// 자동 사냥
#define SRVAL_AUTOHUNTING_FAIL_NAMEERROR			9040	// 이름이 잘못 되었다.
#define SRVAL_AUTOHUNTING_FAIL_NOTHUNTING			9041	// 한동안 사냥을 하지 않은 사람이다.
#define SRVAL_AUTOHUNTING_FAIL_NOSUSPICION			9042	// 자동사냥 체크 창에서 대답을 해서 무혐의다.
#define SRVAL_AUTOHUNTING_FAIL_SERVERERROR			9043	// 서버 에러 - 알 수없는 에러이다.
#define SRVAL_AUTOHUNTING_FAIL_ISREQUEST			9044	// 이미 의뢰처리되었다.
#define SRVAL_AUTOHUNTING_SUCCESS_REQUEST			9045	// 의뢰 성공
#define SRVAL_AUTOHUNTING_SUCCESS_PRISON			9046	// 용의자 수감 성공

#define SRVAL_HOZO_FAIL_SETANGELAPPLYER				9050	// 신청 실패
#define SRVAL_HOZO_FAIL_SETANGEL					9051	// 임명 실패
#define SRVAL_HOZO_SUCCESS_SETANGELAPPLYER			9053	// 신청 성공
#define SRVAL_HOZO_SUCCESS_SETANGEL					9054	// 임명 성공
#define SRVAL_HOZO_SUCCESS_SETANGELAPPLYER_FALSE	9055	// 해지 성공
#define SRVAL_HOZO_SUCCESS_SETANGELADMIN_FALSE		9056	// 해임 성공
#define SRVAL_HOZO_SUCCESS_SETANGEL_FALSE_SELF		9057	// 탈퇴 성공
#define SRVAL_HOZO_FAIL_SETANGELAPPLYER_FALSE		9058	// 탈퇴 실패

//예조판서 수호천사
/*cyj 예조판서 삭제
#define SRVAL_YEZO_FAIL_SETANGELAPPLYER				9050	// 신청 실패
#define SRVAL_YEZO_FAIL_SETANGEL					9051	// 임명 실패
#define SRVAL_YEZO_SUCCESS_SETANGELAPPLYER			9053	// 신청 성공
#define SRVAL_YEZO_SUCCESS_SETANGEL					9054	// 임명 성공
#define SRVAL_YEZO_SUCCESS_SETANGELAPPLYER_FALSE	9055	// 해지 성공
#define SRVAL_YEZO_SUCCESS_SETANGELADMIN_FALSE		9056	// 해임 성공
#define SRVAL_YEZO_SUCCESS_SETANGEL_FALSE_SELF		9057	// 탈퇴 성공
#define SRVAL_YEZO_FAIL_SETANGELAPPLYER_FALSE		9058	// 탈퇴 실패
*/

//형조판서 포교
#define SRVAL_HYUNGZO_SUCCESS_SETPOLICE				9060	// 임명 성공
#define SRVAL_HYUNGZO_SUCCESS_SETPOLICE_FALSE		9061	// 임명 실패


// 말 
#define SRVAL_HORSEDIE								9100	// 말이 죽었다.

// ItemMall
#define SRVAL_ITEMMALL_FAIL_DBFAIL					9200	// DB 에러
#define SRVAL_ITEMMALL_FAIL_GETITEM_NOTINV			9201	// 아이템 가져오기 실패 - 중첩 허용이나 인벤토리 칸 부족
#define SRVAL_ITEMMALL_FAIL_GETITEM_INFOERROR		9202	// 아이템 가져오기 실패 - 서버와 데이터가 다름
#define SRVAL_ITEMMALL_FAIL_CONTINUERENEWAL			9203	// 1초 안에 똑같은 패킷은 안 받는다
#define SRVAL_ITEMMALL_FAIL_CHADEAD					9204	// 사망시에는 안된다.
#define SRVAL_ITEMMALL_FAIL_GETITEM_NOITEM			9205	// 아이템 정보가 올바르지 않다.
#define SRVAL_ITEMMALL_FAIL_CANNOT_SERVER			9206	// 받을 수 없는 서버이다
#define SRVAL_ITEMMALL_FAIL_NOADDITEM				9207	// 더이상 받을 수없는 아이템이다
#define SRVAL_ITEMMALL_FAIL_BADREQUEST				9208	// 1초에 5회 이상의 요청은 무리가 간다. 더이상 아이템몰 관련 메시지는 안받는다.
#define SRVAL_ITEMMALL_FAIL_EQUIPUNABLE				9209	// 장착할수 없는 과금 아이템은 받을수도 없다.
#define SRVAL_ITEMMALL_FAIL_ONLYGROWNUP				9210	// 전체 이용가 서버에서는 수령할 수 없는 아이템 입니다. 18세 이상 서버에서 수령해주세요.
#define SRVAL_ITEMMALL_FAIL_LIMIT_LEVEL				9211	// 아이템 몰에서 선물 가능한 레벨이 안되서 실패함.

// 이벤트설정 
#define SRVAL_EVENTCHAR								10000	
#define SRVAL_EVENT									10020	

// 환경 설정
#define SRVAL_WHISPERCHAT_REJECT					11000
#define SRVAL_PRIVATETRADE_REJECT					11001
#define SRVAL_PARTYINVITE_REJECT					11002
#define SRVAL_FRIENDADD_REJECT						11003

#define SRVAL_LOTTOFAIL								11100	// 흥부박 실패 

#define SRVAL_LEVELUPFAIL_NOCLEARQUEST				11200	// 퀘스트 완료하지 않아 레벨업 실패. 

// 운영자 명령 
#define SRVAL_INVALIDGMORDER						12000	// 운영자 명령 실패 - 해당 명령어 없음.

#define SRVAL_GAMEMASTER_SUCCESS					12001	// 운영자 명령어 성공
#define SRVAL_GAMEMASTER_FAILED						12002	// 운영자 명령어 실패
#define SRVAL_GAMEMASTER_VILLAGEWAR					12003	// 공성전 설정 응답
#define SRVAL_GAMEMASTER_GOONZUDAY					12004	// 군주데이 설정 응답
#define SRVAL_GAMEMASTER_MAKETIMEREDUCE				12005	// 제조 시간 단축 명령어 응답
#define SRVAL_GAMEMASTER_QUESTCOUNT					12006	// 퀘스트 카운트 값 설정
#define SRVAL_GAMEMASTER_FOURLEAF					12007	// 네잎클로버 설정 값
#define SRVAL_GAMEMASTER_VOTECAMPAGIN				12008	// 선거유세 가능 설정
#define SRVAL_GAMEMASTER_EVENTENABLE				12009	// 이벤트 가능 설정
#define SRVAL_GAMEMASTER_POSTOFFICE_NOPERSON		12010	// 우체국 보내기 오류 - 캐릭터 없음
#define SRVAL_GAMEMASTER_POSTOFFICE_NOTENOUGHBOX	12011	// 우체국 보내기 오류 - 수신함 꽉참
#define SRVAL_GAMEMASTER_POSTOFFICE_SYSTEMERROR		12012	// 우체국 보내기 오류 - 시스템 오류
#define SRVAL_GAMEMASTER_SYSTEM_PACKET_ENABLE		12013	// 패킷 블럭에 설정에 대한 응답
#define SRVAL_GAMEMASTER_SETMUTETIME				12014	// 채팅 블럭 설정에 대한 응답
#define SRVAL_GAMEMASTER_SETMUTETIME_CHAR			12015	// 채팅 블럭 설정에 대한 응답
#define SRVAL_GAMEMASTER_GACHALOCK					12016	// 가챠 Lock 설정 에 대한 응답 
#define SRVAL_GAMEMASTER_GACHA_NOT_LOCK_CHANGE_UNIQUE	12017	// 가챠 가 lock이 안된상태에서 유니크를 변경 하려 했다.

// 운영자 명령 - 시스템명령
#define SRVAL_GAMEMASTER_BLOCK_PACKET				12900	// 제한된 패킷임

// DB SYSTEM INFO
#define SRVAL_DBSYSTEMINFO							13000	// DB에 정보를 요청 

// 개인 마법 공격 
#define SRVAL_CANUSEMAGIC_FAIL						14000	// 마법 사용실패 


// 소환수  
#define SRVAL_SUMMONDIE									16000	// 소환수가 죽었다.
#define SRVAL_SUMMONHATCHFAIL							16001	// 소환수 인형을 사용하여 소환수를 소환하는데 실패 
#define SRVAL_SUMMON_FREE_FAIL_HAVEITEM					16002	// 소환수가 장착하고 있어서 풀어주기 불가
#define SRVAL_SUMMON_FAIL_TAKEOUTDELAY					16003	// 딜레이때문에 소환수 소환불가
#define SRVAL_SUMMON_WASTEALLSTAMINA					16004	// 스테미너 없어서 소환수 회수
#define SRVAL_SUMMONSUCCESS								16005	// 소환수 소환 성공
#define SRVAL_SUMMON_FREE_FAIL_STONNUMINCORRECT			16006	// 소환수 무기 장착석을 기입 안했을때.
#define SRVAL_SUMMON_FREE_FAIL_STONNUMNOTENOUGHSTON		16007	// 소환수 무기 장착석이 충분하지 않다.
#define SRVAL_SUMMONSUCCESS_USESTON						16008	// 소환수 무기 장착석이 충분하지 않다.
#define SRVAL_SUMMON_FREE_FAIL_CANNOTOVERUSESTON		16009	// 소환수 레벨보다 장비착용석을 사용할수 없다.
#define SRVAL_SUMMONSUCCESS_CHARGESTAMINA				16010	// 소환수 스태미너 충전 아이템을 이용해서 충전 성공
#define SRVAL_SUMMON_NEEDCHARGESTAMINA					16011	// 소환수 스태미너 충전이 필요하다.
#define SRVAL_SUMMON_STATUSINIT_FAIL_HAVEITEM			16012	// 소환수가 장비를 장착해서 상태초기화 불가능
#define SRVAL_SUMMON_FAIL_OVERSLOT						16013	// 소환수 슬롯이 8군대 모두 꽉찾다.
#define SRVAL_SUMMON_FAIL_OVERDATE						16014	// 소환수 사용권 사용기간이 지났습니다
#define SRVAL_SUMMON_FAIL_LOTSOFDATE					16015	// 소환수 사용권이 아직 남아 있습니다.
#define SRVAL_SUMMON_FAIL_CANNOTSLOTOUTDEAD				16016	// 죽은 소환수는 저장할 수 없다.
#define SRVAL_SUMMON_ADD_MUDANGPRESENT					16017	// 선무당의 선물을 받았다.
#define SRVAL_SUMMON_SUCCES_USESTRAGERICKET				16018	// 소환수 티켓을 사용 하였습니다.
#define SRVAL_SUMMON_DONOTHAVE_PROJECTILE				16019	// 소환수 발사체 아이템이 없음

// 개인상점 
#define SRVAL_PERSONALSHOP								17000	// 개인상점
#define SRVAL_FAIL_PERSONALSHOP_USEITEM					17001	// 개인상점이 열려있고 개인 상점에 올려진 개수보다 적을땐 사용 못 한다
#define SRVAL_FAIL_PERSONALSHOP_MOVEITEM				17002	// 개인상점중에는 시전으로 아이템을 옮길 수 없다( 팔고 있는 아이템만)

// 악의적인 스틸 경고 
#define SRVAL_BADSTEALACTION							18000
#define SRVAL_BADSTEALACTIONPUNISH						18010	// 악의적인 스틸에 처벌 

// 낚시. 
#define SRVAL_NOPROPERFISHAREA							18500	// 낚시하기 부적합한 장소. 
#define SRVAL_FISHRUNNINGAWAY							18501	// 물고기 도망갔다.
#define SRVAL_FISHSKILLNOTENOUGH						18502	// 그 물고기 낚는데, 기술이 부족
#define SRVAL_FISHTOOLNOTEXIST							18503	// 물고기 낚는데, 필요한 도구가 없음
#define SRVAL_FISHTOOLNOTENOUGHDURABILITY				18504	// 도구의 내구도 부족
#define SRVAL_FISHNOTBAITITEM							18505	// 미끼로 사용할 수 없는 아이템 ( 수산물 중에도 그런게 미끼로 사용할 수 없는게 있음으로... )
#define	SRVAL_FISHNOTBAITSHRIMP							18506	// 민물낚시지역에서 새우를 쓰면 나오는 메시지
#define SRVAL_FISHNOTBAITLUGWORM						18507	// 해양낚시지역에서 갯지렁이를 쓰면 나오는 메시지

// 농경
#define SRVAL_FARM_FAIL									18510	// 농사 실패. 
#define SRVAL_FARM_LANDSTRUCTURE_NOT_EXIST				18511	// 건물이 없어서 농사 불가

// 채굴
#define SRVAL_MINE_FAIL									18520	// 채굴 실패. 
#define SRVAL_MINE_MINESTRUCTURE_NOT_EXIST				18521	// 건물이 없어서 채굴 불가

// 인벤토리 
#define SRVAL_ADDINV_FAIL_FULL							19000	// 인벤에 빈자리가 없어서 추가 불가. 

// 객주 거래 
#define SRVAL_MARKET_SELL_FAIL							20000	// 객주에서 물건사기 실패. 
#define SRVAL_NEWMARKET_BUY_FAIL						20001	// 통합마켓에서 물건사기 실패

// 교역 상인
#define SRVAL_TRADEMERCHANT_MONEY_INSUFFICIENTY			21000	// 돈 부족으로 교역 상인 물건 구입 안됨
#define SRVAL_TRADEMERCHANT_NOT_BUY_TIME				21001	// 구입 중 아님
#define SRVAL_TRADEMERCHANT_PREPARE						21002	// 준비중

#define SRVAL_FOREIGNMERCHANT_NOTBUYCASHITEM_SBS		21005	// SBS 회원은 캐쉬아이템 판매 못함
#define SRVAL_FOREIGNMERCHANT_NOTBUYCASHITEM_PRICE		21006	// 60000 원 이상 판매 못함


// 캐쉬. 
#define SRVAL_GIVECASH									21100	// 엔코인을 지급. 

// 출석 
#define SRVAL_ATTENDANCE								21200	// 출석 상황 

#define SRVAL_CANNOTDROPITEM							21250	// 드랍할수 없는 아이템 드랍할려는 경우

#define SRVAL_CANNOTMOVEITEM_PERSONALSHOP				21251	// 개인상점 개설중이라 드랍할수 없다.
#define SRVAL_CANNOTSEALINGUP_PERSONALSHOP				21252	// 개인상점 개설중이라 밀봉해제 할 수 없다.

//KHY - 0530 명나라 상인 구매제한.
#define SRVAL_CANNOTBUYITEM_FOREIGNMERCHANT				21253	// 정해진 기간의 구매제한 금액이 넘어 더이상 구매할 수 없다.

#define SRVAL_BUYLIMITMONEYSET_FOREIGNMERCHANT			21254	// 구매제한 금액을 재설정한다.
#define SRVAL_BUYLIMITMONEYVIEW_FOREIGNMERCHANT			21255	//현재 구매제한 금액을 보여준다.


#define SRVAL_NO_BATTLEQUIT								22001	// 전투중에는 못 나간다.
#define SRVAL_NO_DIEQUIT								22002	// 죽었으면 못 나간다.

#define SRVAL_FAIL_DIS_WORLDMONEY						22010	// 공모 실패
#define SRVAL_FAIL_WORLDMONEY_SELLORDER					22013	// 팔기 등록 실패
#define SRVAL_FAIL_WORLDMONEY_BUYORDER					22015	// 사기 실패
#define SRVAL_FAIL_WORLDMONEY_BUYORDER_MYSELF			22016	// 사기 실패 - 자기자신의 주문
#define SRVAL_FAIL_WORLDMONEY_BUYORDER_NOMONEY			22103	// 사기 실패 - 돈 없다
#define SRVAL_SUCCESS_WORLDMONEY_SETPRICE				22104	// 아큐 시세 설정
#define SRVAL_FAIL_WORLDMONEY_SET_CHAR					22105	// 아큐 주기 실패
#define SRVAL_SUCCESS_WORLDMONEY_SET_CHAR				22106	// 아큐 주기 성공

#define SRVAL_NPC_MARKET_SELL_FAIL						22100   // NPC에게 물건 사기 실패

// 파티
#define SRVAL_PARTYINVITE_FULLPARTY						22300	// 파티원이 가득하서 파티 가입 실패
#define SRVAL_PARTYINVITE_NOTLEADER						22301	// 내가 파티장이 아니어서 파티 초대 실패
#define SRVAL_PARTYINVITE_ALREADYPARTY					22302	// 상대방이 이미 파티중이라서 초대실패
#define SRVAL_PARTYINVITE_NOTFINDNAME					22303	// /p 상대방이름  에서 서버에 이름이 없음
#define SRVAL_PARTYINVITE_LEADERALREADYPARTY			22304	// 나를 초대했던 리더가 다른 파티의 파티원이다.

#define SRVAL_NOTRADE_ITEM								22400	// 거래불가아이템

// 감옥
#define SRVAL_CAN_NOT_USE_ITEM_IN_PRISON				22500	// 감옥이라서 아이템사용 할수 없다 는 메시지
#define SRVAL_CAN_NOT_CHAT_IN_PRISON					22501	// 감옥이라서 채팅 할수 없다 는 메시지
#define SRVAL_CAN_NOT_WARP_IN_PRISON					22502	// 감옥이라서 워프 할수 없다 는 메시지
#define SRVAL_CAN_NOT_MAKEITEM_IN_PRISON				22503	// 감옥이라서 아이템 만들수 없다 는 메시지
#define SRVAL_CAN_NOT_ENCHANTITEM_IN_PRISON				22504	// 감옥이라서 아이템 인챈트 할 수 없다 는 메시지

//전쟁중
#define SRVAL_CAN_NOT_WARP_IN_VILLAGEWAR				22600	//  전쟁중 이라서 워프 할 수 없다 는 메시지

// 수수깨끼 소환알
#define SRVAL_SUMMONEGG_USED							22700	// [%s]님이 [%s]를 소환하였습니다.

// 피로방지 적용 대상 알림
#define	SRVAL_TIRED_SYSTEM_CHAR							23000	// 피로방지 적용대상 캐릭터입니다.

// 매크로 예상자의 마우스정보 분석
#define	SRVAL_RECORD_MOUSEACTION_START					24000	// 마우스 움직임 저장 시작 

//가차
#define SRVAL_GACHA_FAIL_FULLINVEN						25000	// 인벤이 가득차서 가차를 못한다.
#define SRVAL_GACHA_FAIL_KEYNEED						25001	// 인벤이 가득차서 가차를 못한다.
#define SRVAL_GACHA_FAIL_LOCK							25002	// 가차를 운영자가 사용하지 못하도록 했다.
#define SRVAL_GACHA_NOTCORRECT_ITEM						25003	// 가차 최고 아이템이 다르다

//KHY - 1220 - 변신시스템.
#define SRVAL_FAIL_ALEADYTRANSFROMCHR 					25101 // 이미 변신 캐릭터가 등록되어 등록 실패. 
#define SRVAL_FAIL_NOTRANSFROMITEM 						25102 // 변신에 필요한 아이템이 없어 실패. 

//[추가 : 황진성 2008. 1. 11 => 인던 메시지 추가.]
#define SRVAL_FAIL_INSTANTDUNGEON_NO_TICKET				25103	// 입장하는데 티켓이 없군.
#define SRVAL_FAIL_INSTANTDUNGEON_NO_GUILD 				25104	// 우리 길드가 없어서 입장 불가.
#define SRVAL_FAIL_INSTANTDUNGEON_OVER_COUNT_GUILD		25105	// 우리 길드가 다 입장해서 출입 불가.
#define SRVAL_FAIL_INSTANTDUNGEON_MY_NO_GUILD			25106	// 내가 길드가 없어서 입장 불가.
#define SRVAL_SUCCESS_INSTANTDUNGEON_SET_BESTGUILD		25107	// 베스트 길드를 설정한다.

//[추가 : 황진성 2008. 1. 22 => 흑의군단 보상 메시지 추가.]
#define SRVAL_BLACKWAR_CHAR_REWARD						25108	// 흑의군단 개인 보상.
#define SRVAL_BLACKWAR_VILLAGE_REWARD					25109	// 흑의군단 마을 보상.

//[추가 : 황진성 2008. 2. 4 => 공간 상자. ]
#define SRVAL_SPACE_BOX_INVEN_FULL						25110	// 인벤이 가득차서 아이템 이동을 할수 없다면.
#define SRVAL_SPACE_BOX_TIME_OVER						25111	// 상자가 사용 기간이 종료되서 사용 불가능.

//[추가 : 황진성 2008. 2. 20 => 주주총회 발동 가능 불가능 정보를 보이라는 메시지. ]
#define SRVAL_GENERAL_MEETING_SUGGEST_ON_OFF_INFO		25112	
#define SRVAL_GENERAL_MEETING_SUGGEST_OFF_FAIL			25113	//[추가 : 황진성 2008. 2. 20 => 주주총회 발동이 불가능하여 보내는 메시지 . ]

// [영훈] CRC 체크 오류
#define SRVAL_FAIL_FILECRCCHECK							26000	// CRC 체크결과 잘못된 파일

#define SRVAL_FAIL_INSTANTDUNGEON_NO_PARTY				26010	// [영훈] 해적선 : 파티를 맺지 못해 인던에 입장 불가
#define SRVAL_FAIL_INSTANTDUNGEON_OVER_PARTY_MAP		26011	// [영훈] 해적선 : 최대 입장 파티 맵을 초과해 인던에 입장 불가
#define SRVAL_INSTANTDUNGEON_CLEAR_REWARD_ITEM			26012	// [영훈] 해적선 : 해적선을 클리어해서 아이템을 받는다

#define SRVAL_CHANGE_ITEM_FROM_NPC						26017	// 연금술사로 아이템 능력치 변경 완료

#define SRVAL_REWARDITEM_FOR_PCROOM						26020	// [영훈] PC방 : PC방이라서 아이템을 지급한다는 메시지


#define SRVAL_NOTICE_SEASON2_ATTEND						26030	// [영훈] 출석체크 : 출석체크 이벤트 알림
#define SRVAL_FAIL_SEASON2_ATTEND_REWARDITEM			26031	// [영훈] 출석체크 : 보상아이템 지급 실패
#define SRVAL_SUCCESS_SEASON2_ATTEND_REWARDITEM			26032	// [영훈] 출석체크 : 보상아이템 지급 성공

#define SRVAL_FAIL_WARPTO_NOTMARKETOPEN_MAP				26033	// [영훈] 해당 지역에서는 마켓을 열 수 없음

#define	SRVAL_NOTICE_WARPTOPOLL_MAP						26034	// [영훈] 군주 투표소 지역으로 이동할것인지 물어본다

#define SRVAL_FAIL_REASON_ALREADYVOTE					26035	// [영훈] 이미 투표를 끝마쳤다
#define SRVAL_FAIL_REASON_DONOTARRIVELEVLE				26036	// [영훈] 투표할 수 있는 레벨에 도달하지 못했다
#define SRVAL_FAIL_DONOTBUY_ELECTIONSTUMPITEM			26037	// [영훈] 투표기간이 아니라서 어깨띠를 사지 못한다

#define SRVAL_FAIL_SEASON2_ATTEND_NOTGIVEITEM			26038	// [영훈] 출석체크 : 보상을 받지못해 출석 도장을 찍을수없음

#define	SRVAL_NOTICE_OPENTHE_SERVEREVENTGATE			26039	// [영훈] 서버이벤트 인던 게이트가 열렸다
#define	SRVAL_NOTICE_CLOSETHE_SERVEREVENTGATE			26040	// [영훈] 서버이벤트 인던 게이트가 닫혔다

// 결혼관련
#define SRVAL_FAIL_PROPOSE_IS_NOT_VALIDCHAR				27000	// [영훈] 청혼할 캐릭터가 존재하지 않거나 접속중이 아님
#define SRVAL_FAIL_PROPOSE_ALREADY_MARRIED				27001	// [영훈] 청혼할 캐릭터가 이미 결혼했다
#define SRVAL_FAIL_PROPOSE_LESSTHAN_MARRY_LEVEL			27002	// [영훈] 청혼할 캐릭터가 결혼 가능레벨이 아니다
#define SRVAL_FAIL_PROPOSE_SAME_SEX						27003	// [영훈] 청혼할 캐릭터와 성별이 동일하다
#define SRVAL_FAIL_PROPOSE_ANSWER_NO					27004	// [영훈] 청혼할 캐릭터가 청혼을 거절했다
#define SRVAL_SET_MARRY_WITHSAMEMAP						27005	// [영훈] 내 배우자가 같은맵상에 접속했는지 여부를 설정한다
#define SRVAL_REQUEST_WEDDING_ITEMGET					27006	// [영훈] 결혼 기념일 아이템을 받으라고 요청한다
#define SRVAL_WEDDING_MATE1_QUESTION					27007	// [영훈]
#define SRVAL_WEDDING_MATE2_QUESTION					27008	// [영훈]
#define SRVAL_WEDDING_GIVEITEM_QUESTION					27009	// [영훈]
#define SRVAL_WEDDING_GIVEITEM							27010	// [영훈] 결혼식 화관 아이템을 받았다
#define SRVAL_WEDDING_HALL_OUT_NOTICE					27011	// [영훈] 결혼식 주인공이 나갈때

// 휴면 계정 시스템
#define SRVAL_REWARDITEM_FOR_DORMANCYEQUIPBOX			27200	// [기형] 휴면 계정에게 장비 상자 보급 안내
#define SRVAL_FAIL_BUY_DORMANCYITEM						27201	// [기형] 휴면 계정 포인트 아이템 구입 실패
#define SRVAL_FAIL_IS_FULLIVEN							27202	// [기형] 인벤토리가 꽉차서 아이템 수령 실패
#define SRVAL_FAIL_MOVE_ITEM_INHOUSE					27203	// [기형] 휴면 장비 아이템의 창고 이동 실패
#define SRVAL_FAIL_NOT_ENOUGH_POINT						27204	// [기형] 휴면 포인트가 모자름

// 배틀로얄
#define SRVAL_BATTLEROYAL_NOTOPEND						27300	// [기형] 경기장이 열리지 않음
#define SRVAL_BATTLEROYAL_NOTREJOIN						27301	// [기형] 전 경기 참가 후 다음 경기에 참가 불가
#define SRVAL_BATTLEROYAL_NOTREADYFORMAKING				27302	// [기형] 제조 중에는 참가 불가
#define SRVAL_BATTLEROYAL_NOTREADYFORSUMMON				27303	// [기형] 소환수 소환중엔 참가 불가
#define SRVAL_BATTLEROYAL_NOTREADYFORFULLEDGAMER		27304	// [기형] 참가자가 꽉찬 경우 참가 불가
#define SRVAL_BATTLEROYAL_NOTPROPERLEVEL				27305	// [기형] 레벨이 맞지 않다면 참가 불가
#define SRVAL_BATTLEROYAL_JOINPARTY						27306	// [기형] 파티 중이라면 참가 불가

//--------------------------------------------
//[성웅] 라쿤이벤트 군주 이벤트로 변경
//--------------------------------------------
#define SRVAL_SUCCES_SET_GOONZURACCOONEVENT				28000	// [성웅] 지정된 시간에 자이언트 라쿤 이벤트가 설정 되었습니다
#define SRVAL_FAIL_EVENTSET_SAMETIME					28001	// [성웅] 같은 시간대에는 자이언트 라쿤 이벤트 설정이 불가능합니다 
#define SRVAL_FAIL_EVENTSET_NOT_ENOUGH_TIME				28002	// [성웅] 모든 설정이 완료되지 않았습니다.
#define SRVAL_FAIL_EVENTSET_NOT_OVER_LIMITTIME			28003	// [성웅] 자이언트 라쿤 이벤트 설정은 최초 설정 후 24시간이 지나야 가능 합니다.

//--------------------------------------------
//[성웅] 라쿤월드로 이동 
//--------------------------------------------
#define SRVAL_FAIL_GORACCOONWORLD_UNDER_18				29000	// [성웅] 18세 이하는 라쿤월드로 이동 불가능
#define SRVAL_FAIL_GORACCOONWORLD_WARP					29001	// [성웅] 해당지역으로는 워프 할수 없습니다 

#define SRVAL_NOTICE_CLOSE_PERSONALSHOP_BY_WARP			29002	// [영훈] 개인상점 제한지역으로 워프해서 상점이 종료됨

//TradeServer
#define SRVAL_FAIL_TRADESERVER_NOMORE_SELL				30000	// 더 이상 주문을 못 올린다.
#define SRVAL_FAIL_TRADESERVER_SESSION					30001	// 현재 서버랑 연결이 끊어져 있다.
#define SRVAL_FAIL_TRADESERVER_GM						30002	// 운영자 명령어로 못하게 되어 있다.
#define SRVAL_FAIL_TRADESERVER_LIMITTIME				30003	// 현재 할 수 있는 시간이 아니다.
#define SRVAL_FAIL_TRADESERVER_NOT_BUY					30004	// 살 수 있는 물건이 아니다. - 이미 팔렸거나 했다.
#define SRAVL_SUCCESS_TRADESERVER_SETTIME				30005	// 운영자 명령어로 가능 시간을 설정했다.
#define SRVAL_SUCCESS_SET_TRADESERVER					30006	// 운영자 명령어로 무역 기능을 실행 또는 중지시켰다.

#define SRVAL_FAIL_TRADESERVER_CANCEL_NOT				30007	// 물건 회수했지만 이미 팔려서 물건이 없다
#define SRVAL_FAIL_TRADESERVER_CANCEL_ERROR				30008	// 물건 회수했지만 알 수 없는 에러로 회수 실패했다.
#define SRVAL_FAIL_TRADESERVER_BUY_FULLINV				30009	// 사기 했지만 인벤이 다 찼다

#define SRVAL_NOTICE_TRADESERVER_STATE					30010	// [영훈] 현재 무역 서버의 상태를 알려준다

#define	SRVAL_FAIL_WORLDPASSPORT_OVERLAPPING			30011	// [영훈] 여권이름이 중복이라서 여권발급 실패

#define	SRVAL_NOTICE_EVENT_TOGGLE_SWITCH				30012	// [영훈] 이벤트 관련 스위치 On/Off여부

#define	SRVAL_NOTICE_LEVELUP_REWARD						30013	// [영훈] 레벨 업으로 아이템을 받음

//[진성] 아티펙트. 관련 정보 오류로 서버에서 리턴하는 메시지.
#define SRVAL_FAIL_ARTIFACT								30015
#define SRVAL_FAIL_ARTIFACT_REMOVE_GETITEM_NOTINV		30016

//[진성] 파티홀. 관련 정보 오류로 서버에서 리턴하는 메시지.
#define SRVAL_FAIL_PARTYHALL							30017

//[진성] 마을 이벤트 - 몬스터 습격. 서버 메시지. => 2008-3-27
#define SRVAL_MONSTER_ATTACK_EVENT						30018

//[진성] 매도가능한 주식수를 초과해서 클라에서 받을 메시지. => 2008-3-27
#define SRVAL_OVER_STOCK_ORDER							30019

//[진성] 개인상점 모드 변경 오류 메시지. => 2008-3-27
#define SRVAL_PERSONAL_SHOP_MODE_CHANGE_FAIL			30020

//[진성] 서버 이벤트 서버순위가 1등 알림. => 2008-6-24
#define SRVAL_SERVER_EVENT_RANK_1						30021
#define SRVAL_SERVER_EVENT_END							30022
#define SRVAL_SERVER_EVENT_BUF							30023

//[강형] 이관 보상 아이템 사용시 메시지 출력
#define SRVAL_ITEMUSE_REWARDBONUS						30024

//[진성] 명친 여신 퀘스트가 이미 부여 됐다면 퀘스트를 받지 못한다는 것을 표시. => 2008-7-10
#define SRVAL_NAMINGQUEST_START_FAIL					30025
// 명칭 퀘스트의 퀘스트가 성공적으로 부여 되었다는 것을 표시
#define SRVAL_NAMINGQUEST_START_SUCCESS					30026

// Recommand  추천
#define SRVAL_FAIL_RECOMMANDPERSON						30100	// 유저 검색 실패
#define SRVAL_FAIL_RECOMMANDPERSON_CANCEL				30101	// 추천 취소
#define SRVAL_GIVEITEM_RECOMMANDER						30102	// 추천을 한사람이 아이템을 받는다
#define SRVAL_GIVEITEM_RECOMMAND						30103	// 추천을 당한사람이 아이템을 받는다
// 인던 메시지
#define SRVAL_INSTANTDUNGEON_MAKEGATE					30110	// 인스턴스 던전에서 게이트가 생성됨.

#define SRVAL_GIANTRACCOONRESURRECTEVENT_GIVE_ITEM		30111	// 자이언트 라쿤 부활 이벤트에서 빵기부하여 얻는 아이템이 있을때

//KHY - 1211 - 과금상품 - 만능 도구
#define SRVAL_RETURN_USE_ALLPURPOSETOOL					30200	// 만능 도구를 사용 하였다.


#define SRVAL_RETURN_NOT_USE_FOURLEAF					30201	//[진성] 네잎크로버가 현재 사용할수 없는 거면 꺼내지 몬한다. => 2008-8-14
#define SRVAL_EVENTDAY_OVER_NOT_MAKE_ITEM				30202	//[진성] 이벤트 시간이 지나서 아이템을 만들 수 없다. => 2008-9-8

#define SRVAL_GIVE_MENTOR_BUF							30203	//[진성] 스승과 제자 버프를 준다고 메시지 보내기. => 2008-10-9
#define SRVAL_VILLAGE_RESIDENT_INVITE					30204	//[진성] 마을로 상대방을 초대 했을 때 메시지. => 2008-10-9
#define SRVAL_VILLAGE_RESIDENT_RESULT					30205	//[진성] 주민 가입 신청 결과.

#define SRVAL_VILLAGE_HELPER							30206	//[진성] 도우미 관련 메시지. => 2008-11-10
#define SRVAL_DIVINATION_MSG							30207	//[진성] 운세 관련 메시지.

#define SRVAL_ELIXIR_CHARGE_ITEM_NUM_ZERO_FAIL			30208	//[진성] 엘릭서 NPC가 교화해주려는 아이템이 마력의 비약 수량이 0개 일때 실패

#define SRVAL_PROMOTER_NOT_CELLULAR_PHONE_AUTH			30209	//[진성] 중국 핸드폰 인증 안해서 아이템을 못 줌.

#define SRVAL_NEWMARKET_REPAIR_FAIL						30210	//[진성] 통합마켓 수리 실패

#define SRVAL_TRANSFORM_SYNCRO_UP						30211	//[진성] 변신 싱크로 증가제 관련 메시지.

#define SRVAL_PROMOTER_CHAR_LEVELUP_MINI_BUTTON			30212	//[진성] 중국에서는 캐릭터가 레벨업하면 프로모터 정보창과 연결되는 작은 버튼을 보여준다.


#define SRVAL_RECOMMEND_GIVE_ITEM_SUCCEED					30213	//[진성] 추천인이 레벨업으로 아이템 획득.
#define SRVAL_RECOMMENDEE_GIVE_ITEM_SUCCEED					30214	//[진성] 피추천인에게 추천인이 아이템 획득 실패 알림.
#define SRVAL_RECOMMEND_GIVE_ITEM_FAIL						30215	//[진성] 추천인이 레벨업으로 아이템 획득.
#define SRVAL_RECOMMENDEE_GIVE_ITEM_FAIL					30216	//[진성] 피추천인에게 추천인이 아이템 획득 실패 알림.
#define SRVAL_RECOMMEND_GIVE_ITEM_SUCCEED_NO_AUTH			30217	//[진성] 추천인이 레벨업으로 아이템 획득. 인증안된거.
#define SRVAL_RECOMMENDEE_GIVE_ITEM_SUCCEED_NO_AUTH			30218	//[진성] 피추천인에게 추천인이 아이템 획득 실패 알림. 인증안된거.

#define SRVAL_REPAIR_REQUEST_FAIL							30219	//[진성] 수리의뢰 이용권이 없습니다.

#define SRVAL_GIANTRACCOONRESURRECTEVENT_NO_TIME			30220	//[진성] 자이언트 라쿤 이벤트 시간이 아니어서 불가능.
#define SRVAL_GIANTRACCOONRESURRECTEVENT_ALREAY_MAKE_NPC	30221	//[진성] 자이언트 라쿤 이벤트 이미 다른 마을에서 건물 설치를 완료함.
#define SRVAL_GIANTRACCOONRESURRECTEVENT_EVENT_START		30222	//[진성] 자이언트 라쿤 이벤트 시간이 되어 시작을 알림.
#define SRVAL_GIANTRACCOONRESURRECTEVENT_FAIL_USE_ITEM		30223	//[진성] 자이언트 라쿤 이벤트에 획득하는 자이언트 라쿤 세트 먹다 실패.


#define SRVAL_VALENITNEDAY_ALREADY_BUF						30224   //[진성] 달콤한 초콜릿으로 고백하여 버프를 획득하고자 하나 이미 버프가 존재함을 알림.
#define SRVAL_VALENITNEDAY_NOT_MYSELF						30225   //[진성] 달콤한 초콜릿으로 고백하여 버프를 획득하고자 하나 이미 버프가 존재함을 알림.
#define SRVAL_VALENITNEDAY_NOT_USER							30226   //[진성] 달콤한 초콜릿으로 고백하여 버프를 획득하고자 하나 이미 버프가 존재함을 알림.


// 길드 - SJY
#define SRVAL_FAIL_CONFIRMCREATEGUILD_NOMONEY				31000	// 소지금 부족으로 길드 허가가 실패해서 길드가 없어졌다
#define SRVAL_FAIL_ALEADYRECEIVE_BEGINNERWEAPON				32000	// 이미 초보자 무기를 받았다!
#define SRVAL_FAIL_MANUFATURENPC_NOMONEY					32001	// 소지금 부족으로 NPC를 통한 제조 불가능
#define SRVAL_FAIL_GIVEITEM_NOINVEN							32002	// 인벤이 꽉차서 아이템을 지급할수 없음(GM)
#define SRVAL_RETURN_USERCOUNT								32003	// 해당 맵의 유저 숫자

#define SRVAL_RETURN_BEFORE_REG_MAPWAR						32004	// 이미 길드전을 신청 했다.
#define SRVAL_RETURN_REG_CANTSTATUSGUILD					32005	// 길드렙 4이상 인원이 5이상이 안된다- 길드전 가능 상태가 아니다.
#define SRVAL_RETURN_NOTMASTER								32006	// 길드 마스터가 아닙니다.
#define SRVAL_RETURN_NOT_RED_HUTMAP							32007	// 길드맵을 신청 하지 않았다.
#define SRVAL_RETURN_NOT_REG_TIME							32008	// 길드전을 신청할수 없는 시간이다.
#define SRVAL_RETURN_CANNOTGUILDWAR_NOGUILD					32009	// 매칭된 길드가 없다. 길드전 진행 불가능 
#define SRVAL_RETURN_CANNOTGUILDWAR_RESEASON				32010	// 길드전이 초기화 되었따 새롭게 등록해라.

// 잃어버린 세계 퀘스트
#define SRVAL_RETURN_LOSTWORLDQUEST_COMPLETE				32011	// 잃어버린 세계 퀘스트 - 뼈티라노 리젠 위치로 가면 완료

// 수호정령 특수능력 : 수호정령의 특수 능력으로 마법을 발사 한다
#define SRVAL_RETURN_SOULGUARD_SHOT_MAGIC					32012

// 라쿤 이벤트 종료 메세지 돈을 받았음을 알려 준다 
#define SRVAL_RETURN_RACOONDODGEEVENT_CLEAR					32013

#define SRVAL_RETURN_GAWIBAWIBO_SRPWIN						32014	// [종호_GAWIBAWIBO] 연승 토글
#define SRVAL_RETURN_GAWIBAWIBO_SRPLIMIT					32015	// [종호_GAWIBAWIBO] 연승 제한

// 복불복 게임 클리어 메세지 (돈 보상 안내)
#define SRVAL_RETURN_BOKBULBOK_CLEAR_REWARD					32016	// 아직 32767이...

#define SRVAL_RETURN_SUMMON_TRANSFORM_USE_BONUS				32017	// 소화수가 변신 상태에서 스텟을 변경하려고 할 때 사용 불가 메시지.
#define SRVAL_RETURN_SUMMON_TRANSFORM_USE_STAMINA_ITEM		32018	// 소화수가 변신 상태에서 스테미너 약을 먹었을 경우 사용 불가 메시지.

// 몬스터 생성창을 띄우기
#define SRVAL_RETURN_MONCREATE_DLG							32019
// 장비 생성창을 띄우기
#define SRVAL_RETURN_EQUIPMENTCREATE_DLG					32020
// /CMT 명령어로 몬스터 생성 성공
#define SRVAL_RETURN_MONCREATE_DLG_SUCCESS					32021
// /SEQ 명령어로 장비 생성 성공
#define SRVAL_RETURN_EQUIPMENTCREATE_DLG_SUCCESS			32022
// [춘기] 크리스마스 이벤트 - 엔펠리스에 1단계 트리 생겼을때 알림
#define SRVAL_RETURN_WORLDCHRISTMASTREE_SUCCESS				32023
// [춘기] 크리스마스 이벤트 - 엔펠리스에 2단계 트리 생겼을때 알림
#define SRVAL_RETURN_WORLDCHRISTMASTREE2_SUCCESS			32024
// [춘기] 크리스마스 이벤트 - 엔펠리스에 1단계 트리 생겼을때 알림 (메시지 안띄움)
#define SRVAL_RETURN_WORLDCHRISTMASTREE3_SUCCESS			32025
// [춘기] 크리스마스 이벤트 - 엔펠리스에 대형 트리 삭제되면 알림
#define SRVAL_RETURN_WORLDCHRISTMASTREE_DEL					32026

//----------------------------------------------------------------------------------------------
// !!! 중간에 빈곳이 많습니다. SI16이니 더 이상은 추가하지 마십시요. !!!!
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// 진성 
// 2008-3-28
//
// 서버리턴 메시지의 파라미터 값.
//----------------------------------------------------------------------------------------------

//[진성] 파티홀 관련 메시지 파라미터. => 2008-3-28
//SRVAL_FAIL_PARTYHALL
enum 
{ 
	PARTYHALL_ALREADY_USER,				// 이미 예약한 유저임.
	PARTYHALL_ETRANCE_RESERVATION,		// 입장 예약 완료.
	PARTYHALL_ETRANCE_RESERVATION_FAIL, // 입장 예약 실패.
	PARTYHALL_ALREADY_RESERVATION,		// 이미 예약 되있음.
	PARTYHALL_OPEN,						// 파티가 시작.
	PARTYHALL_CLOSE,					// 파티가 종료.
	
	PARTYHALL_MSG_NUM,					// 메시지 개수.
};

//[진성] 마을 이벤트 메시지 파리미터. => 2008-3-28
//SRVAL_MONSTER_ATTACK_EVENT
enum
{
	MONSTER_ATTACK_EVENT_START,			// 마을 이벤트 시작.
	MONSTER_ATTACK_EVENT_END_SUCCED,	// 마을 이벤트 종료.
	MONSTER_ATTACK_EVENT_END_FAIL,		// 마을 이벤트 종료.
	MONSTER_ATTACK_EVENT_MONSTER_DIE,	// 몬스터 디졌쓰~ 
	MONSTER_ATTACK_EVENT_ALREADY_RUN,	// 이미 이벤트를 다른 마을이 진행중. 
	MONSTER_ATTACK_EVENT_REMAIN_TIME,	// 남은시간을 클라에게.
};

//[진성] 도우미. => 2008-3-28
//SRVAL_VILLAGE_HELPER
enum
{
	 SRVAL_VILLAGE_HELPER_ALREADY_QUESTION		// 이미 질문이 있습니다.
	,SRVAL_VILLAGE_HELPER_ALREADY_ANSWER_ME		// 이미 내가 답변중입니다.
	,SRVAL_VILLAGE_HELPER_ALREADY_ANSWER_OTHER	// 이미 다른 답변자가 답변중입니다.
	,SRVAL_VILLAGE_HELPER_QUESTION_ADD_SUCCESS	// 질문 등록 성공.
	,SRVAL_VILLAGE_HELPER_QUESTION_ADD_FAIL		// 질문 등록 실패.
	,SRVAL_VILLAGE_HELPER_QUESTION_DEL_SUCCESS	// 질문 삭제.
	,SRVAL_VILLAGE_HELPER_ANSWER_ADD_SUCCESS	// 답변자 등록 성공.
	,SRVAL_VILLAGE_HELPER_ANSWER_ADD_FAIL		// 답변자 등록 실패.
	,SRVAL_VILLAGE_HELPER_QUESTION_FULL			// 질문이 꽉 차있음.
	,SRVAL_VILLAGE_HELPER_ALREADY_DEL_QUESTION	// 질문이 삭제 되었음.
	,SRVAL_VILLAGE_HELPER_LIMIT_COUNT_QUESTION	// 질문이 하루 제한 수가 되었음.
	,SRVAL_VILLAGE_HELPER_ANSWER_SEND_SUCCESS	// 질문자에게 답변을 보냈음을 답변자에게 알림. 
};


//_______________________________________________________
#endif
