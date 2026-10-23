#pragma once

#include "stdafx.h"

#include "GoonZuMC.h"
#include "StaticBuffer.h"

const SI32 ConstMaxOrderSize = 256;
const SI32 ConstIDSize = 20;
const SI32 ConstIPSize = 20;
const SI32 ConstTimeSize = 30;
const SI32 ConstServerName = 30;
const SI32 ConstGMOrderBufferSize = 256;

//***************************************************************************************************
//
// Class Name 		: CGMOrderLog
//
// Last Modified 	: 2009 / 07 / 02
// Created		 	: 박기형
//
// Function			: GM명령어 정보 클래스
//
//***************************************************************************************************
class CGMOrderLog
{
protected:
	TCHAR	m_szOrder[ConstMaxOrderSize];
	TCHAR	m_szOrderPersonID[ConstIDSize];
	TCHAR   m_szOrderPersonIP[ConstIPSize];
	TCHAR   m_szOrderTime[ConstTimeSize];
	TCHAR   m_szGameServerName[ConstServerName];
	SI32    m_siWorldId;
	SI32	m_siResult;
public:

	TCHAR*	GetOrder()
	{
		return m_szOrder;
	}
	TCHAR*	GetOrderPersonID()
	{
		return m_szOrderPersonID;
	}
	TCHAR*	GetOrderPersonIP()
	{
		return m_szOrderPersonIP;
	}
	TCHAR*	GetOrderTime()
	{
		return m_szOrderTime;
	}
	TCHAR*	GetGameServerName()
	{
		return m_szGameServerName;
	}
	SI32	GetWorldId()
	{
		return m_siWorldId;
	}
	BOOL	GetResult()
	{
		return m_siResult;
	}

	void	SetResult(SI32 siResult)
	{
		m_siResult = siResult;
	}

	void	SetOrder(TCHAR* pszText)
	{
		ZeroMemory(m_szOrder,ConstMaxOrderSize * sizeof(TCHAR));
		StringCchCopy(m_szOrder,ConstMaxOrderSize,pszText);
		return;
	}
	void	SetOrderPersonID(TCHAR* pszText)
	{
		ZeroMemory(m_szOrderPersonID,ConstIDSize * sizeof(TCHAR));
		StringCchCopy(m_szOrderPersonID,ConstIDSize,pszText);
		return;
	}
	void	SetOrderPersonIP(TCHAR* pszText)
	{
		ZeroMemory(m_szOrderPersonIP,ConstIPSize * sizeof(TCHAR));
		StringCchCopy(m_szOrderPersonIP,ConstIPSize,pszText);
		return;
	}
	void	SetOrderTime(TCHAR* pszText)
	{
		ZeroMemory(m_szOrderTime,ConstTimeSize * sizeof(TCHAR));
		StringCchCopy(m_szOrderTime,ConstTimeSize,pszText);
		return;
	}
	void	SetGameServerName(TCHAR* pszText)
	{
		ZeroMemory(m_szGameServerName,ConstServerName * sizeof(TCHAR));
		StringCchCopy(m_szGameServerName,ConstServerName,pszText);
		return;
	}
	void	SetWorldId(SI32 siWorldId)
	{
		m_siWorldId = siWorldId;
		return;
	}

	void	Init()
	{
		ZeroMemory(m_szOrder,ConstMaxOrderSize * sizeof(TCHAR));
		ZeroMemory(m_szOrderPersonID,ConstIDSize * sizeof(TCHAR));
		ZeroMemory(m_szOrderPersonIP,ConstIPSize * sizeof(TCHAR));
		ZeroMemory(m_szOrderTime,ConstTimeSize * sizeof(TCHAR));
		ZeroMemory(m_szGameServerName,ConstServerName * sizeof(TCHAR));
		m_siWorldId = -1;
	}
};
//***************************************************************************************************
//
// Class Name 		: CGMOrderLogManager
//
// Last Modified 	: 2009 / 07 / 02
// Created		 	: 박기형
//
// Function			: GM명령어 로그 관리 클래스로써 추후 추가 개발할 예정
//
//***************************************************************************************************
class CGMOrderLogManager
{
public:
	CGMOrderLogManager(void);
	~CGMOrderLogManager(void);

	BOOL	Create();
public:
	StaticBuffer*	m_pGMOrderLogBuffer; // 실시간으로 들어오는 GM명령어 결과를 담는 버퍼

};
