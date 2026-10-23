#include "CBattleRoyalPersonChangedData.h"

CBattleRoyalPersonChangedData::CBattleRoyalPersonChangedData()
{
	ZeroMemory(this, sizeof(CBattleRoyalPersonChangedData));
}
CBattleRoyalPersonChangedData::~CBattleRoyalPersonChangedData()
{
}
const SI32	CBattleRoyalPersonChangedData::GetMaxHP() const
{
	return m_siMaxHP;
}
const SI32	CBattleRoyalPersonChangedData::GetHP() const
{
	return m_siHP;
}
const SI32	CBattleRoyalPersonChangedData::GetMinDamage() const
{
	return m_siMinDamage;
}
const SI32	CBattleRoyalPersonChangedData::GetMaxDamage() const
{
	return m_siMaxDamage;
}
const SI32	CBattleRoyalPersonChangedData::GetMag() const
{
	return m_siMag;
}
const SI32	CBattleRoyalPersonChangedData::GetCharMode() const
{
	return m_siCharMode;
}
const SI32	CBattleRoyalPersonChangedData::GetKillMode() const
{
	return m_siKillMode;
}
const SI32	CBattleRoyalPersonChangedData::GetPersonID() const
{
	return m_siPersonID;
}
const SI32	CBattleRoyalPersonChangedData::GetTransformBufIndex() const
{
	return m_siTransformBufIndex;
}
void	CBattleRoyalPersonChangedData::SetMaxHP(SI32	siMaxHP)
{
	m_siMaxHP = siMaxHP;
}
void	CBattleRoyalPersonChangedData::SetHP(SI32	siHP)
{
	m_siHP = siHP;
}
void	CBattleRoyalPersonChangedData::SetMinDamage(SI32	siMinDamage)
{
	m_siMinDamage = siMinDamage;
}
void	CBattleRoyalPersonChangedData::SetMaxDamage(SI32	siMaxDamage)
{
	m_siMaxDamage = siMaxDamage;
}
void	CBattleRoyalPersonChangedData::SetMag(SI32	siMag)
{
	m_siMag = siMag;
}
void	CBattleRoyalPersonChangedData::SetCharMode(SI32	siCharMode)
{
	m_siCharMode = siCharMode;
}
void	CBattleRoyalPersonChangedData::SetKillMode(SI32	siKillMode)
{
	m_siKillMode = siKillMode;
}
void	CBattleRoyalPersonChangedData::SetPersonID(SI32	siPersonID)
{
	m_siPersonID = siPersonID;
}	
void	CBattleRoyalPersonChangedData::SetTransformBufIndex(SI32	siTransformBufIndex)
{
	m_siTransformBufIndex = siTransformBufIndex;
}	