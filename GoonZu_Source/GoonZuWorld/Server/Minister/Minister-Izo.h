#ifndef _MINISTER_IZO_H_
#define _MINISTER_IZO_H_

const SI32 Const_SpecialTaxRange_Min = 100;		//특별세 적용 범위 최소값 ( 100% )
const SI32 Const_SpecialTaxRange_Max = 150;		//특별세 적용 범위 최대값 ( 100% )
const SI32 Const_SpecialTaxRate_Min = 5;		//특별세율 최소값( 5% )
const SI32 Const_SpecialTaxRate_Max = 50;		//특별세율 최대값( 50% )
const SI32 Const_SpecialTaxStep = 5;			//5%씩 올리거나 내릴수 있음

const SI32 Const_MarketDownRate_Min = 1;		// 마켓 할인율 최소값
const SI32 Const_MarketDownRate_Max = 15;		// 마켓 할인율 최대값

class CMinisterIzo : public CMinisterBase
{
private:
	bool	m_bWorldTradeTax;			// 무역_세금 적용 유무
	SI32	m_siSpecialTaxRange;		// 무역_특별세 적용범위( 100% ~ 150% )
	SI32	m_siSpecialTaxRate;			// 무역_특별세율( 10% ~ 100% )

	SI32	m_siMarketDownRate;			//  마켓 자동할인 시스템 할인율 ( 3% ~ 15% )
	SI16	m_siTinTin;					// 틴틴 추가 분해율

public:
	SI32	m_siPriceValue;	// 매입율
	SI32	m_siPriceTime;  // 마지막으로 설정한 시간
	SI64	m_siTotalMoney; // 총통화량
	SI32	m_siCalcTime;	// 총통화량 계산한 시간
	SI32	m_siTradeTax; // 관세율
	GMONEY	m_siTradeLimitMoney; // 비밀거래시 최소가격

	CMinisterIzo();
	~CMinisterIzo();


	void 	InitializeDB();
	void 	DoMsg_DBMsgResponse_GetMinisterInfo(sPacketHeader* pPacket);
	void 	DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID);
	void 	DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID);
	void 	DoMsg_DBMsgResponse_SetMinisterInfo(sPacketHeader* pPacket);

	void	SetIsWorldTradeTax( bool mode ){ m_bWorldTradeTax = mode; }
	bool	GetIsWorldTradeTax(){ return m_bWorldTradeTax; }
	void	SetSpecialTaxRange( SI32 range ){ m_siSpecialTaxRange = range; }
	SI32	GetSpecialTaxRange(){ return m_siSpecialTaxRange; }
	void	SetSpecialTaxRate( SI32 rate ){ m_siSpecialTaxRate = rate; }
	SI32	GetSpecialTaxRate(){ return m_siSpecialTaxRate; }

	void	SetMarketDownRate( SI32 rate ){ m_siMarketDownRate = rate; }
	SI32	GetMarketDownRate(){ return m_siMarketDownRate; }

	SI32	GetTinTin()			{ return m_siTinTin;	}
};

#endif