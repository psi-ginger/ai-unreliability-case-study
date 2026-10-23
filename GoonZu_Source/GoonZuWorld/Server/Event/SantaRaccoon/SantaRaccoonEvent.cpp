#include "SantaRaccoonEvent.h"

#include "../CommonLogic/CommonLogic.h"
#include "../Server/Server.h"

//-----------------------------------------
// Common
//-----------------------------------------
#include "../Common/Char/KindInfo/KindInfo.h"
#include "../Common/Order/Order.h"
#include "Char/CharManager/CharManager.h"

#include "MsgType-System.h"

#include "../../Client/Music/Music.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Village.h"

#include <MsgType-Person.h>

extern cltCommonLogic* pclClient;

CSantaRaccoon::CSantaRaccoon() : cltEvent()
{
	SI32 i;
	for( i = 0; i < MAX_SANTARACCOON_NUM; ++i )
	{
		if ( i >= MAX_SANTARACCOON_NUM )	continue	;	// �迭�� �Ѿ�� ���������� �д´�� �ѱ��.	�ʱ�ȭ�� �� ���ݾ�.
		m_bKilledSantaRaccoon[i] = false;
		m_siSantaRaccoonID[i] = 0;
	}

	// ��ձ������ ���� ������ �����ϹǷ� ��ǥ������ ������ ���� �̿��Ѵ�.
	FILE* fp = fopen("ServerData\\KingGuri\\KingGuriInfo.txt", "rt");
	if(fp == NULL)
	{
#ifndef _SERVER
		MessageBox( NULL, "SantaRaccoon Failed", "SantaRaccoon", MB_OK );
#else
		OutputDebugString( TEXT("[SantaRaccoon] Failed to open ServerData\\KingGuri\\KingGuriInfo.txt\n") );
#endif
		return;
	}

	TCHAR buffer[256] = {'\0'};
	for( i = 0;i< 7;i++ )
	{
		fgets(buffer, 256, fp);
	}

	SI32 count = 0;
	while( fgets( buffer, 256, fp ) != NULL )
	{
		if( _tcscmp( buffer, "<END>") == 0 )
		{
			break;
		}
		if ( count >= MAX_SANTARACCOON_POSITION_NUM )	continue	;	// ���ҽ��� �߸��������� ����ؼ� �ƽ���ŭ �о� ������ ���̻� �迭�� ���� ���� �ʰ� ��Ƽ�� �ؼ� ������ Ŭ���� ��ŵ�ϴ�
		sscanf( buffer, "%s %d %d", m_szAreaName[count], &m_ptPosition[count].x, &m_ptPosition[count].y );
		count++;
	}

	fclose(fp);
}

CSantaRaccoon::~CSantaRaccoon()
{
}

void CSantaRaccoon::Init()
{
	for( SI32 i = 0; i < MAX_SANTARACCOON_NUM; ++i )
	{
		m_bKilledSantaRaccoon[i] = false;
		m_siSantaRaccoonID[i] = 0;
	}

	cltEvent::Init();
}

void CSantaRaccoon::Start()
{
	Init();

	bEventSwitch = true;

	dwLeftClock = 1000 * 60 * 50; 

	dwStartClock = pclClient->CurrentClock;
	dwLastInfoClock = pclClient->CurrentClock;
}	

void CSantaRaccoon::Action()						
{
	if( false == bEventSwitch )
	{
		cltServer* pclserver = (cltServer*)pclClient;

		if(0 == (pclserver->sTime.wMinute%60))
		{
			MakeSantaRaccoon();
			pclserver->pclLog->FileConfigPrint( "SantaRaccoon.txt", "Start" );
		}
	}
	else
	{
		if( pclClient->CurrentClock - dwStartClock > dwLeftClock )
		{
			End();
		}
		//[�߰� : Ȳ���� 2007. 11. 29 ��Ÿ���� ���Ͱ� �� �׾����� �̺�Ʈ ����.]
		else if(true == MonsterAllDie())
		{
			End();
		}
		else
		{
			DWORD SendTime = 1000 * 60 * 3; //3�� 20��

			// ��ü�������� �޼��� ����
			if( pclClient->CurrentClock - dwLastInfoClock > SendTime )
			{
				DWORD leftClock = dwLeftClock - ( pclClient->CurrentClock - dwStartClock ); 
				SendInfo( ACTION, leftClock );
				dwLastInfoClock = pclClient->CurrentClock;
			}
		}
	}
}

void CSantaRaccoon::End()
{
	bEventSwitch = false;

	SI32 index = 0;
	SI32 id = 0;
	while( id = pclClient->pclCM->GetCharOrder(index) )
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

		if( NULL != pclchar)
		{
			if( pclClient->pclCM->IsValidID(id) == false )		
				continue;

			if( pclchar->siCharMode == CHARMODE_SANTARACCOON )	
			{
				// �� �������� ������ �����Ѵ�. 
				pclClient->pclCM->CR[id]->DeleteInCell();
				if( pclClient->pclCM->DeleteChar( id ) == TRUE )
				{
					index--;
				}
			}
		}
	}

	SendInfo( END, 0 );

	Init();
}


void CSantaRaccoon::MakeSantaRaccoon()
{
	Start();

	// ���͸� ��ġ�� ������ ���� ã�´�. 
	SI32 count = 0;
	SI32 randValue = 0;
	SI32 oldRandValue = -1;
	bool created = false;
	while( !created )
	{
	 	randValue = rand() % MAX_SANTARACCOON_POSITION_NUM;
		if( randValue == oldRandValue )
			continue;

		oldRandValue = randValue;

		SI32 Kind = pclClient->GetUniqueFromHash(TEXT("KIND_SANTARACCOON_ZIQ"));
		
		SI32 tempX = m_ptPosition[randValue].x; 
		SI32 tempY = m_ptPosition[randValue].y;

		SI32 x, y;
		if( pclClient->pclMapManager->pclMapArray[0]->FindEmptyAreaForCharLocate(Kind, tempX, tempY, 10, &x, &y, true) == true )
		{

			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

			cltServer* pclserver = (cltServer*)pclClient;

			SI32 id = pclserver->SetNPCChar(Kind, VILLAGE_NONE, NULL, 1, 0, x, y, 0, NPCITEMMODE_NORMAL, &clinnerstatus );
			if(id)
			{
				m_siSantaRaccoonID[count] = id;
				if ( count >= MAX_SANTARACCOON_NUM)	continue	;
				StringCchCopy( m_szSelectedAreaName[count], 32, m_szAreaName[randValue] );

				//[�߰� : Ȳ���� 2007. 11. 29 �� �ε��� ��ȣ.]
				m_siAreaIndex[count] = randValue;

				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

				pclchar->siCharMode = CHARMODE_SANTARACCOON;
				count++;

				if( count == MAX_SANTARACCOON_NUM )
					created = true;
			}
		}
	}


	SendInfo( START, dwLeftClock );
}

void CSantaRaccoon::SendInfo( SI32 Type, DWORD LeftTime, TCHAR* UserNmae )
{
	cltGameMsgResponse_SantaRaccoonInfo clSantaRaccoonInfo( Type, LeftTime, m_szSelectedAreaName, UserNmae, &m_ptPosition[0], &m_siAreaIndex[0]);
	cltMsg clMsg( GAMEMSG_RESPONSE_SANTARACCOONINFO, sizeof(clSantaRaccoonInfo), (BYTE*)&clSantaRaccoonInfo );

	SI32 index = 0;
	SI32 id = 0;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		// ������� �Ŀ��Ը� �����Ѵ�. 
		if( true == pclClient->pclCM->CR[id]->IsValidConnection() )
		{
			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

void CSantaRaccoon::SetDieSantaRaccoon( bool Flag, SI32 SantaRaccoonID, SI32 UserID )
{
	for( SI32 i = 0; i < MAX_SANTARACCOON_NUM; ++i )
	{
		if( m_siSantaRaccoonID[i] == SantaRaccoonID )
		{
			if( false == m_bKilledSantaRaccoon[i] )
			{
				m_bKilledSantaRaccoon[i] = Flag;

				//[�߰� : Ȳ���� 2007. 11. 13. ��Ÿ������ �׾ �ش� ���̵� ���ſ� ���̸�.]
				m_siSantaRaccoonID[i]	 = 0;
				ZeroMemory(m_szSelectedAreaName[i], sizeof(m_szSelectedAreaName[i]));

				if( true == pclClient->pclCM->IsValidID(UserID) )		
				{
					TCHAR* userName = (TCHAR*)pclClient->pclCM->CR[UserID]->GetName();
					SendInfo( DIE, 0, userName );
				}
			}
		}
	}

	// ���Ͱ� ��� �׾����� �̺�Ʈ ����.
	if(true == MonsterAllDie())
	{
		SendInfo( END, 0 );
	}
	// ���Ͱ� ��� ������ ������ ��ġ�� �ٽ� ����.
	else
	{
		//[�߰��� : Ȳ���� 2007. 11. 13 ��Ÿ������ �׾ ��ġ�� ������.]
		DWORD leftClock = dwLeftClock - ( pclClient->CurrentClock - dwStartClock ); 
		SendInfo( ACTION, leftClock );
		dwLastInfoClock = pclClient->CurrentClock;
	}
}

bool CSantaRaccoon::MonsterAllDie(void)
{
	// ���Ͱ� ������ �׾��°� �ƴѰ�?
	bool MonAllDie = true;
	for( SI32 i = 0; i < MAX_SANTARACCOON_NUM; ++i )
	{
		if(0 != m_siSantaRaccoonID[i])	
		{
			MonAllDie = false;
			break;
		}
	}

	return MonAllDie;
}