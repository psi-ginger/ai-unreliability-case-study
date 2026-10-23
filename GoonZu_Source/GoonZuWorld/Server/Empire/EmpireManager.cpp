#include "EmpireManager.h"

CEmpireManager::CEmpireManager()
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		m_EmpireUnitInfo[i].InitEmpireUnitInfo();
	}

}

CEmpireManager::~CEmpireManager()
{
}

void CEmpireManager::AddEmpireUnit( CEmpireUnitInfo* EmpireUnitInfo )
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		// 제국 유니크가 0이면 매니저에 포함이 안되어 있으니 추가시킨다.
		if( 0 == m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			m_EmpireUnitInfo[i].SetEmpireUnitInfo( EmpireUnitInfo );
			return;
		}
	}
}

void CEmpireManager::DeleteEmpireUnit( SI32 EmpireUnique )
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		// 제국 유니크가 같으면 삭제 시킨다.
		if( EmpireUnique == m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			m_EmpireUnitInfo[i].InitEmpireUnitInfo();
			return;
		}
	}
}

void CEmpireManager::DeleteEmpireUnit( CEmpireUnitInfo* EmpireUnitInfo )
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		// 제국 유니크가 같으면 삭제 시킨다.
		if( EmpireUnitInfo->GetEmpireUnique() == m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			m_EmpireUnitInfo[i].InitEmpireUnitInfo();
			return;
		}
	}
}

void CEmpireManager::UpdateAddEmpireUnit( SI32 EmpireUnique, SI32 VillageUnique, SI32 GuildUnique )
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		// 제국 유니크가 같으면 업데이트 해준다.
		if( EmpireUnique == m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			if( 0 < VillageUnique )
			{
				for( SI32 j = 0; j < MAX_EMPIRE_IN_VILLAGE; ++j )
				{
					if( 0 == m_EmpireUnitInfo[i].GetVillageUnique(j) )
					{
						m_EmpireUnitInfo[i].SetVillageUnique( j, VillageUnique );
						return;
					}
				}
			}
			else if( 0 < GuildUnique )
			{
				for( SI32 j = 0; j < MAX_EMPIRE_IN_GUILD; ++j )
				{
					if( 0 == m_EmpireUnitInfo[i].GetGuildUnique(j) )
					{
						m_EmpireUnitInfo[i].SetGuildUnique( j, GuildUnique );
						return;
					}
				}
			}
		}
	}
}

void CEmpireManager::UpdateDeleteEmpireUnit( SI32 EmpireUnique, SI32 VillageUnique, SI32 GuildUnique )
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		// 제국 유니크가 같으면 업데이트 해준다.
		if( EmpireUnique == m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			if( 0 < VillageUnique )
			{
				for( SI32 j = 0; j < MAX_EMPIRE_IN_VILLAGE; ++j )
				{
					if( VillageUnique == m_EmpireUnitInfo[i].GetVillageUnique(j) )
					{
						m_EmpireUnitInfo[i].SetVillageUnique( j, 0 );
						m_EmpireUnitInfo[i].SetInputTotalEmpireMoney( j, 0 );
						return;
					}
				}
			}
			else if( 0 < GuildUnique )
			{
				for( SI32 j = 0; j < MAX_EMPIRE_IN_GUILD; ++j )
				{
					if( GuildUnique == m_EmpireUnitInfo[i].GetGuildUnique(j) )
					{
						m_EmpireUnitInfo[i].SetGuildUnique( j, 0 );
						return;
					}
				}
			}
		}
	}
}

void CEmpireManager::UpdateEmpireBaseMoney( SI32 EmpireUnique, SI64 LeftEmpireBaseMoney, SI32 VillageUnique, SI64 TotalInputEmpireBaseMoney )
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		// 제국 유니크가 같으면 업데이트 해준다.
		if( EmpireUnique == m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			if( 0 < VillageUnique )
			{
				for( SI32 j = 0; j < MAX_EMPIRE_IN_VILLAGE; ++j )
				{
					if( VillageUnique == m_EmpireUnitInfo[i].GetVillageUnique(j) )
					{
						m_EmpireUnitInfo[i].SetEmpireBaseMoney( LeftEmpireBaseMoney );
						m_EmpireUnitInfo[i].SetInputTotalEmpireMoney( j, TotalInputEmpireBaseMoney );
						return;
					}
				}
			}
		}
	}
}

SI32 CEmpireManager::GetEmperorVillageUniqueByIndex( SI32 Index )
{
	if( Index >= MAX_EMPIRENUMBER )
		return 0;

	if( 0 < m_EmpireUnitInfo[Index].GetEmpireUnique() )
	{
		if( 0 < m_EmpireUnitInfo[Index].GetEmpireUnique() )
		{
			return m_EmpireUnitInfo[Index].GetEmperorVillageUnique();
		}
		else
			return 0;
	}
	else
		return 0;
}

SI32 CEmpireManager::GetEmperorVillageUniqueByEmpireUnique( SI32 EmpireUnique )
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		// 제국 유니크가 같으면 리턴시킨다.
		if( EmpireUnique == m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			return m_EmpireUnitInfo[i].GetEmperorVillageUnique();
		}
	}

	return 0;
}

SI32 CEmpireManager::FindEmpireUnique( SI32 VillageUnique )
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		// 제국 유니크가 같으면 리턴시킨다.
		if( 0 < m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			for( SI32 j = 0; j < MAX_EMPIRE_IN_VILLAGE; ++j )
			{
				if( VillageUnique == m_EmpireUnitInfo[i].GetVillageUnique(j) )
				{
					return m_EmpireUnitInfo[i].GetEmpireUnique();
				}
			}
		}
	}

	return 0;
}

CEmpireUnitInfo* CEmpireManager::FindEmpire( SI32 EmpireUnique )
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		// 제국 유니크가 같으면 리턴시킨다.
		if( EmpireUnique == m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			return &m_EmpireUnitInfo[i];
		}
	}

	return NULL;
}

CEmpireUnitInfo* CEmpireManager::FirstEmpire()
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		// 제국 유니크가 처음 있는것을 리턴한다.
		if( 0 < m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			return &m_EmpireUnitInfo[i];
		}
	}

	return NULL;
}

bool CEmpireManager::IsInVillageUnique( SI32 EmpireUnique, SI32 VillageUnique )
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		if( EmpireUnique == m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			for( SI32 j = 0; j < MAX_EMPIRE_IN_VILLAGE; ++j )
			{
				if( VillageUnique == m_EmpireUnitInfo[i].GetVillageUnique(j) )
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool CEmpireManager::IsInGuildUnique( SI32 EmpireUnique, SI32 GuildUnique )
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		if( EmpireUnique == m_EmpireUnitInfo[i].GetEmpireUnique() )
		{
			for( SI32 j = 0; j < MAX_EMPIRE_IN_GUILD; ++j )
			{
				if( GuildUnique == m_EmpireUnitInfo[i].GetGuildUnique(j) )
				{
					return true;
				}
			}
		}
	}

	return false;
}