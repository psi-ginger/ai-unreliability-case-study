#ifndef _MINISTER_GONGZO_H_
#define _MINISTER_GONGZO_H_

//#define FIRST_GONGZO_SKILL	SKILL_MAKESWORD1
//#define END_GONGZO_SKILL	SKILL_MAKEGUN1

// 비율로 변경..변수명 안바꿈..
const GMONEY ConstCraftFund_Min = 0;
const GMONEY ConstCraftFund_Max = 100;

class CMinisterGongzo : public CMinisterBase
{
public:
	SI32	m_siPriceTime;  // 마지막으로 설정한 시간
	SI16	m_siLeftOverValue;
	SI16	m_siLeftOverValue_Armor;
	GongzoMember m_stGongzoMember; // 공조 변수들 - 스킬 추가값 , 스킬 사용빈도
//	SI16	m_siPriceValue[GONGZO_SKILL_TOTAL]; // 모든 스킬이 들어가는 배열
//	SI16	m_siSkillValue[GONGZO_SKILL_TOTAL]; // 스킬 사용빈도
/*	SI16	m_siPriceSword; // 검스킬
	SI16	m_siPriceSpear; // 창스킬
	SI16	m_siPriceAxe;	// 도끼스킬
	SI16	m_siPriceStaff; // 지팡이스킬
	SI16	m_siPriceBow;	// 활스킬
	SI16	m_siPriceGun;	// 총스킬*/

	CMinisterGongzo();
	~CMinisterGongzo();

	void 	InitializeDB();
	void 	DoMsg_DBMsgResponse_GetMinisterInfo(sPacketHeader* pPacket);

	void 	DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID);

	void 	DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID);

	void 	DoMsg_DBMsgResponse_SetMinisterInfo(sPacketHeader* pPacket);

	// gongzomembervalue = GONGZO_SKILL_SWORD,GONGZO_SKILL_SPEAR,GONGZO_SKILL_STAFF,GONGZO_SKILL_AXE,GONGZO_SKILL_BOW,GONGZO_SKILL_GUN
	SI32	GetGongzoSkillValue(SI16 gongzomembervalue);
	SI32	GetSkillExpPercent(SI32 siGoongzoSkill)
	{
		if( siGoongzoSkill < GONGZO_SKILL_SWORD || siGoongzoSkill > GONGZO_SKILL_RESERVED3 )
			return 0;

		return m_stGongzoMember.siPriceValue[siGoongzoSkill];
	}
};

#endif