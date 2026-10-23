#include "../Client/FeastEffect/FeastEffect.h"
#include "../Client/FeastEffect/FeastEffectManager.h"
#include "..\client.h"
#include "../Client/Music/music.h"
#include <functional>
#include <algorithm>

#include "../common/Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;

CFeastEffectManager::CFeastEffectManager()
{
	m_FeastEffectVector.reserve(100);
	m_FeastEffectVector.clear();


	m_siActionType = -1;
	m_siActionKind = -1;

	m_bEffectLive = true;

	for(SI32 i=0; i<11; ++i)
	{
		m_fEffectCurrentTime[i] = 0.0f;
		m_fEffectLastTime[i]= 0.0f;
		m_bEffectPlay[i] = false;
		m_siEffectCount[i] = 0;
		m_siEffectLifeTime[i] = 0;

		m_stEffectInfo[i].Init();
	}
	srand( (unsigned)time( NULL ) );
}

CFeastEffectManager::~CFeastEffectManager()
{
	FeastEffectVectorIter Iter;
	for(Iter = m_FeastEffectVector.begin(); Iter != m_FeastEffectVector.end(); ++Iter)
	{
		delete *Iter;
	}
	m_FeastEffectVector.clear();
}

void CFeastEffectManager::AddEffect(SI32 siKind, SI32 siStartPositionX/* =0 */, SI32 siStartPositionY/* =0 */)
{
	if( siKind < 0)
		return;

	if( siKind < 3)
		m_siActionType = 0;
	else if( siKind < 8)
		m_siActionType = 1;
	else if( siKind < 11)
		m_siActionType = 2;
	else
		m_siActionType = 0;

	m_bEffectLive = true;

	cltClient *pclclient = (cltClient *)pclClient;

	switch( m_siActionType )
	{
	case 0:
		{
			switch( siKind )		
			{
			case 0:
				{
					m_bEffectPlay[0] = TRUE;
					m_siActionKind = siKind;

					FILE *pFile;

					pFile = _tfopen(TEXT("FeastEffect/FeastEffect00.dat"), TEXT("r"));

					TCHAR strLine[256] = { 0, };

					float  TempDelay = 0.0f;
					int	   TempCreateType = 0;
					int    TempLifeTime = 0;
					int    TempMaxCount = 0;

					_fgetts(strLine, sizeof(strLine), pFile);
					_stscanf(strLine, TEXT("%d"), &TempCreateType);

					_fgetts(strLine, sizeof(strLine), pFile);
					_stscanf(strLine, TEXT("%f %d %d"), &TempDelay, &TempLifeTime, &TempMaxCount);

					m_stEffectInfo[0].m_siEffectCreateType = TempCreateType;
					m_stEffectInfo[0].m_fEffectDelay = TempDelay;
					m_stEffectInfo[0].m_siEffectLifeTime = TempLifeTime;
					m_stEffectInfo[0].m_siEffectMaxCount = TempMaxCount;

					m_stEffectInfo[0].m_bEffectPlay = true;

					fclose(pFile);
				
					cltClient *pclclient = (cltClient *)pclClient;

					for(SI32 y=0;y<4;++y)
					{

						for(SI32 x=0;x<5;++x)
						{
						//	SI32 randx = rand() % 7;
							m_arrayPositionX[(y*5) + x] = 415 + rand()%	pclclient->GetScreenXSize();// y*50 + 200*x;
							m_arrayPositionY[(y*5) + x] = -100 + (rand()%600 *-1);

							if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
							{
								m_arrayPositionX[(y*5) + x] += 100;
							}
						}

					}
			//pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_0")), 0, 0);
				}
				break;
			case 1:
				{
					m_bEffectPlay[1] = TRUE;
					m_siActionKind = siKind;

					FILE *pFile;

					pFile = _tfopen(TEXT("FeastEffect/FeastEffect01.dat"), TEXT("r"));

					TCHAR strLine[256] = { 0, };

					float  TempDelay = 0.0f;
					int	   TempCreateType = 0;
					int    TempLifeTime = 0;
					int    TempMaxCount = 0;

					_fgetts(strLine, sizeof(strLine), pFile);
					_stscanf(strLine, TEXT("%d"), &TempCreateType);

					_fgetts(strLine, sizeof(strLine), pFile);
					_stscanf(strLine, TEXT("%f %d %d"), &TempDelay, &TempLifeTime, &TempMaxCount);

					m_stEffectInfo[1].m_siEffectCreateType = TempCreateType;
					m_stEffectInfo[1].m_fEffectDelay = TempDelay;
					m_stEffectInfo[1].m_siEffectLifeTime = TempLifeTime;
					m_stEffectInfo[1].m_siEffectMaxCount = TempMaxCount;

					m_stEffectInfo[1].m_bEffectPlay = true;

					fclose(pFile);

					cltClient *pclclient = (cltClient *)pclClient;

					for(SI32 y=0;y<4;++y)
					{
						for(SI32 x=0;x<5;++x)
						{
						//	SI32 randx = rand() % 7;
							m_arrayPositionX[(y*5) + x] = 300 + rand()%	pclclient->GetScreenXSize();// y*50 + 200*x;
							m_arrayPositionY[(y*5) + x] = -50 + (rand()%600 *-1);

							if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
							{
								m_arrayPositionX[(y*5) + x] += 100;
							}
						}

					}
		//			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_1")), 0, 0);
				}
				break;
			case 2:
				{
					m_bEffectPlay[2] = TRUE;
					m_siActionKind = siKind;

					FILE *pFile;

					pFile = _tfopen(TEXT("FeastEffect/FeastEffect02.dat"), TEXT("r"));

					TCHAR strLine[256] = { 0, };

					float  TempDelay = 0.0f;
					int	   TempCreateType = 0;
					int    TempLifeTime = 0;
					int    TempMaxCount = 0;

					_fgetts(strLine, sizeof(strLine), pFile);
					_stscanf(strLine, TEXT("%d"), &TempCreateType);

					_fgetts(strLine, sizeof(strLine), pFile);
					_stscanf(strLine, TEXT("%f %d %d"), &TempDelay, &TempLifeTime, &TempMaxCount);

					m_stEffectInfo[2].m_siEffectCreateType = TempCreateType;
					m_stEffectInfo[2].m_fEffectDelay = TempDelay;
					m_stEffectInfo[2].m_siEffectLifeTime = TempLifeTime;
					m_stEffectInfo[2].m_siEffectMaxCount = TempMaxCount;

					m_stEffectInfo[2].m_bEffectPlay = true;

					fclose(pFile);

					cltClient *pclclient = (cltClient *)pclClient;

					for(SI32 y=0;y<4;++y)
					{
						for(SI32 x=0;x<5;++x)
						{
							//	SI32 randx = rand() % 7;
							m_arrayPositionX[(y*5) + x] = 300 + rand()%	pclclient->GetScreenXSize();// y*50 + 200*x;
							m_arrayPositionY[(y*5) + x] = -50 + (rand()%600 *-1);

							if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
							{
								m_arrayPositionX[(y*5) + x] += 100;
							}
						}

					}
		//			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_2")), 0, 0);
				}
				break;
			}

		}
		break;
	case 1:
		{
			switch( siKind )		
			{
			case 3:
				{
					m_bEffectPlay[3] = true;
					m_siActionKind = siKind;

					LoadText(TEXT("FeastEffect/FeastEffect03.dat"), &m_stEffectInfo[3]);//], m_Type3PositionX, m_Type3PositionY);

					m_stEffectInfo[3].m_siStartPositionX = siStartPositionX;
					m_stEffectInfo[3].m_siStartPositionY = siStartPositionY;
	//				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_3")), 0, 0);
				}
				break;

			case 4:
				{
					m_bEffectPlay[4] = TRUE;
					m_siActionKind = siKind;

					LoadText(TEXT("FeastEffect/FeastEffect04.dat"), &m_stEffectInfo[4]);//, m_Type4PositionX, m_Type4PositionY);

					m_stEffectInfo[4].m_siStartPositionX = siStartPositionX;
					m_stEffectInfo[4].m_siStartPositionY = siStartPositionY;
	//				pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_4")), 0, 0);
				}
				break;

			case 5:
				{
					m_bEffectPlay[5] = TRUE;
					m_siActionKind = siKind;

					LoadText(TEXT("FeastEffect/FeastEffect05.dat"), &m_stEffectInfo[5]);//, m_Type5PositionX, m_Type5PositionY);

					m_stEffectInfo[5].m_siStartPositionX = siStartPositionX;
					m_stEffectInfo[5].m_siStartPositionY = siStartPositionY;
		//			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_5")), 0, 0);


				}
				break;

			case 6:
				{
					m_bEffectPlay[6] = TRUE;
					m_siActionKind = siKind;

					LoadText(TEXT("FeastEffect/FeastEffect06.dat"), &m_stEffectInfo[6]);//, m_Type6PositionX, m_Type6PositionY);

					m_stEffectInfo[6].m_siStartPositionX = siStartPositionX;
					m_stEffectInfo[6].m_siStartPositionY = siStartPositionY;
		//			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_6")), 0, 0);
				}
				break;

			case 7:
				{
					m_bEffectPlay[7] = TRUE;
					m_siActionKind = siKind;

					LoadText(TEXT("FeastEffect/FeastEffect07.dat"), &m_stEffectInfo[7]);//, m_Type7PositionX, m_Type7PositionY);

					m_stEffectInfo[7].m_siStartPositionX = siStartPositionX;
					m_stEffectInfo[7].m_siStartPositionY = siStartPositionY;
		//			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_7")), 0, 0);
				}
				break;
			}
		}
		break;
	case 2:
		{
			switch( siKind )
			{
			case 8:
				{
					m_bEffectPlay[8] = TRUE;
					m_siActionKind = siKind;

					LoadText(TEXT("FeastEffect/FeastEffect08.dat"), &m_stEffectInfo[8]);//, m_Type8PositionX, m_Type8PositionY);
		//			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_8")), 0, 0);
				}
				break;
			case 9:
				{
					m_bEffectPlay[9] = TRUE;
					m_siActionKind = siKind;

					LoadText(TEXT("FeastEffect/FeastEffect09.dat"), &m_stEffectInfo[9]);//, m_Type9PositionX, m_Type9PositionY);
		//			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_9")), 0, 0);
				}
				break;
			case 10:
				{
					m_bEffectPlay[10] = TRUE;
					m_siActionKind = siKind;

					LoadText(TEXT("FeastEffect/FeastEffect10.dat"), &m_stEffectInfo[10]);//, m_Type10PositionX, m_Type10PositionY);
		//			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_10")), 0, 0);
				}
				break;
			}
		}
		break;
	default:
		break;
	}
}

void CFeastEffectManager::DeleteEffect()
{
}


void CFeastEffectManager::SetAction( SI32 siKind, SI32 siStartPositionX/* =0 */, SI32 siStartPositionY/* =0 */)
{
	AddEffect( siKind, siStartPositionX, siStartPositionY );
}

void CFeastEffectManager::Action()
{
	SI32 i;
	for( i=0; i<3; ++i )
	{
		UpdateEffectOne( &m_stEffectInfo[i], i);
	}
	for( i=0; i<5; ++i )
	{
		UpdateEffectTwo( &m_stEffectInfo[i+3], i);
	}
	for( i=0; i<3; ++i )
	{
		UpdateEffectThree( &m_stEffectInfo[i+8], i);
	}
	
 	FeastEffectVectorIter Iter;
	for(Iter = m_FeastEffectVector.begin(); Iter != m_FeastEffectVector.end();)
	{
		(*Iter)->Action();

		if( (*Iter)->GetEffectLive() == FALSE )
		{
			delete (*Iter);
			Iter = m_FeastEffectVector.erase(Iter);
		}
		else
		{
			++Iter;
		}
	}
}

void CFeastEffectManager::Draw()
{
	/*	FeastEffectVectorIter Iter;
	for(Iter = m_FeastEffectVector.begin(); Iter != m_FeastEffectVector.end(); ++Iter)
	{
	if( (*Iter)->GetEffectLive() )
	(*Iter)->Draw();
	}*/
	for_each(m_FeastEffectVector.begin(), m_FeastEffectVector.end(), std::mem_fun(&CFeastEffect::Draw));
}

void CFeastEffectManager::StopFeastEffect()
{
	m_bEffectLive = FALSE;

	/*	for(SI32 i=0; i<11; ++i)
	{
	m_fEffectCurrentTime[i] = 0.0f;
	m_fEffectLastTime[i]= 0.0f;
	m_bEffectPlay[i] = FALSE;
	m_siEffectCount[i] = 0;
	}*/
}

void CFeastEffectManager::LoadText(TCHAR* pFineName, stEffectInfo* pEffectInfo)//, SI32* arrayX, SI32* arrayY)
{
	FILE *pFile;

	pFile = _tfopen(pFineName, "r");

	TCHAR strLine[256] = { 0, };

	float  TempDelay = 0.0f;
	int   TempCreateType = 0;
	int   TempLifeTime = 0;
	int   TempMaxCount = 0;

	_fgetts(strLine, sizeof(strLine), pFile);
	_stscanf(strLine, TEXT("%d"), &TempCreateType);

	_fgetts(strLine, sizeof(strLine), pFile);
	_stscanf(strLine, TEXT("%f %d %d"), &TempDelay, &TempLifeTime, &TempMaxCount);

	pEffectInfo->m_siEffectCreateType = TempCreateType;
	pEffectInfo->m_fEffectDelay = TempDelay;
	pEffectInfo->m_siEffectLifeTime = TempLifeTime;
	pEffectInfo->m_siEffectMaxCount = TempMaxCount;
	pEffectInfo->m_bEffectPlay = true;

	SI32 x,y;
	SI32 i;
	cltClient *pclclient = (cltClient *)pclClient;

	for(i=0; i<pEffectInfo->m_siEffectMaxCount; ++i)
	{
		_fgetts(strLine, sizeof(strLine), pFile);
		_stscanf(strLine, TEXT("%d %d"), &x, &y);

		pEffectInfo->m_ArrayX[i] = pclclient->GetScreenXSize() - x;
		pEffectInfo->m_ArrayY[i] = pclclient->GetScreenYSize() - y;

		if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		{
			pEffectInfo->m_ArrayX[i] -= 100;
			pEffectInfo->m_ArrayY[i] -= 50;
		}
	}

	fclose(pFile);
}

void CFeastEffectManager::UpdateEffectOne(stEffectInfo* pEffectInfo, SI32 siKind)
{
	cltClient *pclclient = (cltClient *)pclClient;

	if(pEffectInfo->m_bEffectPlay)
	{
		if(siKind == 0)
			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_0")), 0, 0);
		else if(siKind == 1)
			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_1")), 0, 0);
		else if(siKind == 2)
			pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_2")), 0, 0);

		if( m_bEffectLive == FALSE )
		{
			pEffectInfo->m_bEffectPlay = FALSE;
		}
		else
		{
			LARGE_INTEGER m_ticksPerSecond;

			if(!QueryPerformanceFrequency(&m_ticksPerSecond))
			{
				return;
			}

			LARGE_INTEGER currentTime;
			QueryPerformanceCounter(&currentTime);

			pEffectInfo->m_fEffectCurrentTime = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

			if(pEffectInfo->m_fEffectCurrentTime - pEffectInfo->m_fEffectLastTime >= pEffectInfo->m_fEffectDelay)
			{
				pEffectInfo->m_fEffectLastTime = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

				for(SI32 i = 0; i< pEffectInfo->m_siEffectMaxCount; ++i)
				{
					CFeastEffect*	TempFeastEffect = new CFeastEffectOne( siKind );
					TempFeastEffect->SetPositionX( m_arrayPositionX[i] );
					TempFeastEffect->SetPositionY( m_arrayPositionY[i] );
					m_FeastEffectVector.push_back( TempFeastEffect );
				}

				++pEffectInfo->m_siEffectLifeCount;
				if(pEffectInfo->m_siEffectLifeCount >= pEffectInfo->m_siEffectLifeTime)
				{
					pEffectInfo->m_siEffectLifeCount = 0;
					pEffectInfo->m_bEffectPlay = false;
					pEffectInfo->Init();
					return;
				}

			}
		}
	}
}

void CFeastEffectManager::UpdateEffectTwo(stEffectInfo* pEffectInfo, SI32 siKind)
{
	cltClient *pclclient = (cltClient *)pclClient;

	if(pEffectInfo->m_bEffectPlay)
	{
		LARGE_INTEGER m_ticksPerSecond;

		if(!QueryPerformanceFrequency(&m_ticksPerSecond))
		{
			return;
		}

		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);

		pEffectInfo->m_fEffectCurrentTime = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

		if(pEffectInfo->m_fEffectCurrentTime - pEffectInfo->m_fEffectLastTime >= pEffectInfo->m_fEffectDelay)
		{
			pEffectInfo->m_fEffectLastTime = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

			// [영훈] 폭죽 터지는 위치 고정되도록 변경 - 2008.02.23
			SI32 siRealX = 0;
			SI32 siRealY = 0;

			if ( pclclient->IsCountrySwitch( Switch_FixedPositionFeastEffect ) == true )
			{
				if ( (pEffectInfo->m_siStartPositionX > 0) && (pEffectInfo->m_siStartPositionY > 0) )
				{
					SI32 siStartX = pEffectInfo->m_siStartPositionX - pclclient->pclCM->CR[1]->GetX();
					SI32 siStartY = pEffectInfo->m_siStartPositionY - pclclient->pclCM->CR[1]->GetY();

					siRealX = siStartX * MAP_TILE_XSIZE_HALF - siStartY * MAP_TILE_XSIZE_HALF;
					siRealY = siStartY * MAP_TILE_YSIZE_HALF + siStartX * MAP_TILE_YSIZE_HALF;
				}
			}

			if( pEffectInfo->m_siEffectCreateType )	// 순차적으로 하나씩 생성
			{
				CFeastEffect*	TempFeastEffect = new CFeastEffectTwo( siKind );
				
				TempFeastEffect->SetPositionX( pEffectInfo->m_ArrayX[pEffectInfo->m_siEffectCount] + siRealX );
				TempFeastEffect->SetPositionY( pEffectInfo->m_ArrayY[pEffectInfo->m_siEffectCount] + siRealY );
				m_FeastEffectVector.push_back(TempFeastEffect);

				if(siKind == 0)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_3")), 0, 0);
				else if(siKind == 1)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_4")), 0, 0);
				else if(siKind == 2)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_5")), 0, 0);
				else if(siKind == 3)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_6")), 0, 0);
				else if(siKind == 4)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_7")), 0, 0);

				++pEffectInfo->m_siEffectCount;
				if(pEffectInfo->m_siEffectCount >= pEffectInfo->m_siEffectMaxCount)
				{ 
					if( m_bEffectLive == FALSE )
					{
						//	pEffectInfo->m_siEffectCount = 0;
						//	pEffectInfo->m_bEffectPlay = false;
						pEffectInfo->Init();

						return;
					}
					else
					{
						++pEffectInfo->m_siEffectLifeCount;
						pEffectInfo->m_siEffectCount = 0;
						if(pEffectInfo->m_siEffectLifeCount >= pEffectInfo->m_siEffectLifeTime)
						{
							//	pEffectInfo->m_siEffectLifeCount = 0;
							//	pEffectInfo->m_bEffectPlay = false;
							pEffectInfo->Init();

							return;
						}
					}
				}
			}
			else
			{
				for(SI32 i = 0; i<pEffectInfo->m_siEffectMaxCount; ++i)
				{
					CFeastEffect*	TempFeastEffect = new CFeastEffectTwo( siKind );
					TempFeastEffect->SetPositionX( pEffectInfo->m_ArrayX[i] + siRealX );
					TempFeastEffect->SetPositionY( pEffectInfo->m_ArrayY[i] + siRealY );
					m_FeastEffectVector.push_back(TempFeastEffect);
				}

				if(siKind == 0)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_3")), 0, 0);
				else if(siKind == 1)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_4")), 0, 0);
				else if(siKind == 2)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_5")), 0, 0);
				else if(siKind == 3)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_6")), 0, 0);
				else if(siKind == 4)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_7")), 0, 0);

				++pEffectInfo->m_siEffectLifeCount;
				if(pEffectInfo->m_siEffectLifeCount >= pEffectInfo->m_siEffectLifeTime)
				{
					//	pEffectInfo->m_siEffectLifeCount = 0;
					//	pEffectInfo->m_bEffectPlay = false;
					pEffectInfo->Init();

					return;
				}
			}
		}
	}
}

void CFeastEffectManager::UpdateEffectThree(stEffectInfo* pEffectInfo, SI32 siKind)
{
	cltClient *pclclient = (cltClient *)pclClient;

	if(pEffectInfo->m_bEffectPlay)	// 박
	{
		LARGE_INTEGER m_ticksPerSecond;

		if(!QueryPerformanceFrequency(&m_ticksPerSecond))
		{
			return;
		}

		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);

		pEffectInfo->m_fEffectCurrentTime = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

		if(pEffectInfo->m_fEffectCurrentTime - pEffectInfo->m_fEffectLastTime >= pEffectInfo->m_fEffectDelay)
		{
			pEffectInfo->m_fEffectLastTime = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

			if( pEffectInfo->m_siEffectCreateType )	// 순차적으로 하나씩 생성
			{
				CFeastEffect*	TempFeastEffect = new CFeastEffectThree(siKind);
				TempFeastEffect->SetPositionX(pEffectInfo->m_ArrayX[pEffectInfo->m_siEffectCount]);
				TempFeastEffect->SetPositionY(pEffectInfo->m_ArrayY[pEffectInfo->m_siEffectCount]);
				m_FeastEffectVector.push_back(TempFeastEffect);

				TempFeastEffect = new CFeastEffectThree(siKind + 3);
				TempFeastEffect->SetPositionX(pEffectInfo->m_ArrayX[pEffectInfo->m_siEffectCount]);
				TempFeastEffect->SetPositionY(pEffectInfo->m_ArrayY[pEffectInfo->m_siEffectCount]);
				m_FeastEffectVector.push_back(TempFeastEffect);

				if(siKind == 0)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_8")), 0, 0);
				else if(siKind == 1)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_9")), 0, 0);
				else if(siKind == 2)
					pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FEAST_KIND_10")), 0, 0);
				
				++pEffectInfo->m_siEffectCount;
				if(pEffectInfo->m_siEffectCount >= pEffectInfo->m_siEffectMaxCount)
				{ 
					if( m_bEffectLive == FALSE )
					{
						//	pEffectInfo->m_siEffectCount = 0;
						//	pEffectInfo->m_bEffectPlay = false;
						pEffectInfo->Init();
						return;
					}
					else
					{
						++pEffectInfo->m_siEffectLifeCount;
						pEffectInfo->m_siEffectCount = 0;
						if(pEffectInfo->m_siEffectLifeCount >= pEffectInfo->m_siEffectLifeTime)
						{
							//	pEffectInfo->m_siEffectLifeCount = 0;
							//	pEffectInfo->m_bEffectPlay = false;
							pEffectInfo->Init();
							return;
						}
					}
				}
			}
			else
			{
				for(SI32 i = 0; i<pEffectInfo->m_siEffectMaxCount; ++i)
				{
					CFeastEffect*	TempFeastEffect = new CFeastEffectThree(siKind);
					TempFeastEffect->SetPositionX(pEffectInfo->m_ArrayX[pEffectInfo->m_siEffectCount]);
					TempFeastEffect->SetPositionY(pEffectInfo->m_ArrayY[pEffectInfo->m_siEffectCount]);
					m_FeastEffectVector.push_back(TempFeastEffect);

					TempFeastEffect = new CFeastEffectThree(siKind + 3);
					TempFeastEffect->SetPositionX(pEffectInfo->m_ArrayX[pEffectInfo->m_siEffectCount]);
					TempFeastEffect->SetPositionY(pEffectInfo->m_ArrayY[pEffectInfo->m_siEffectCount]);
					m_FeastEffectVector.push_back(TempFeastEffect);
				}

				++pEffectInfo->m_siEffectLifeCount;
				if(pEffectInfo->m_siEffectLifeCount >= pEffectInfo->m_siEffectLifeTime)
				{
					//	pEffectInfo->m_siEffectLifeCount = 0;
					//	pEffectInfo->m_bEffectPlay = false;
					pEffectInfo->Init();

					return;
				}

			}
		}
	}
}