//
//
//  @ File Name : CAbstractState.h
//  @ Date : 2009-09-28
//  @ Author : 박기형
//	@ Desc : 추상 상태 클래스
//


#if !defined(_CBATTLEROYALPERSONCHANGEDDATA_H)
#define _CBATTLEROYALPERSONCHANGEDDATA_H

#include "../CommonLogic.h"

class CBattleRoyalPersonChangedData
{
public:
	CBattleRoyalPersonChangedData();
	~CBattleRoyalPersonChangedData();

	const SI32	GetMaxHP() const;
	const SI32	GetHP() const;
	const SI32	GetMinDamage() const;
	const SI32	GetMaxDamage() const;
	const SI32	GetMag() const;
	const SI32	GetCharMode() const;
	const SI32	GetKillMode() const;
	const SI32	GetPersonID() const;
	const SI32	GetTransformBufIndex() const;

	void	SetMaxHP(SI32	siMaxHP);
	void	SetHP(SI32	siHP);
	void	SetMinDamage(SI32	siMinDamage);
	void	SetMaxDamage(SI32	siMaxDamage);
	void	SetMag(SI32	siMag);
	void	SetCharMode(SI32	siCharMode);
	void	SetKillMode(SI32	siKillMode);
	void	SetPersonID(SI32	siPersonID);
	void	SetTransformBufIndex(SI32	siTransformBufIndex);

private:
	SI32	m_siMaxHP;
	SI32	m_siHP;
	SI32	m_siMinDamage;
	SI32	m_siMaxDamage;
	SI32	m_siMag;
	SI32	m_siCharMode;
	SI32	m_siKillMode;
	SI32	m_siPersonID;
	SI32	m_siTransformBufIndex;
};

#endif  //_CBATTLEROYALPERSONCHANGEDDATA_H
