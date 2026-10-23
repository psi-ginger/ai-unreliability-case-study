#include "../Client/DLGPositionSave/DLGPositionSave.h"
#include "../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern SI32 gsiResolution;

CDlgPositionSave::CDlgPositionSave()
{
	for(SI32 i=0; i<DIALOG_END; ++i)
	{
		DLG_Position[i].x = 0;
		DLG_Position[i].y = 0;

		siSaveState[i] = 0;
	}
}

CDlgPositionSave::~CDlgPositionSave()
{
}

void CDlgPositionSave::LoadPosition()
{
	FILE *pFile = NULL;
	
	// 800 X 768
//	if(gsiResolution == 1)
//		pFile = _tfopen(TEXT("Config/DLGPOS800.dat"), TEXT("rt"));
	// 1024 X 768
	if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
	{	
		// 풀스크린 좌표와 창모드 좌표를 다른 파일에 세브 & 로드
		if(g_SoundOnOffDlg.m_bMaxWindowMode)
			pFile = _tfopen(TEXT("Config/DLGPOS1024Full.dat"), TEXT("rt"));
		else
			pFile = _tfopen(TEXT("Config/DLGPOS1024.dat"), TEXT("rt"));
	}
	if( pFile == NULL )
		return;
	else
	{
		for( SI32 i=0; i<DIALOG_END; ++i )
		{
			TCHAR strLine[256] = TEXT("");
			_fgetts(strLine, sizeof(strLine), pFile);
			_stscanf(strLine, TEXT("%d %d %d"), &siSaveState[i], &DLG_Position[i].x, &DLG_Position[i].y);
		}

		fclose(pFile);
	}
}

void CDlgPositionSave::SavePosition()
{
	FILE *pFile = NULL;

	// 800 X 768
//	if(gsiResolution == 1)	
//		pFile = _tfopen(TEXT("Config/DLGPOS800.dat"), TEXT("wt"));
	// 1024 X 768
//	else 
	if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
	{
		if(g_SoundOnOffDlg.m_bMaxWindowMode)
			pFile = _tfopen(TEXT("Config/DLGPOS1024Full.dat"), TEXT("wt"));
		else
			pFile = _tfopen(TEXT("Config/DLGPOS1024.dat"), TEXT("wt"));
	}
	
	if(pFile == NULL)
		return;

	for(SI32 i=0; i<DIALOG_END; ++i)
	{
		_ftprintf(pFile, TEXT("%d %d %d\n"), siSaveState[i], DLG_Position[i].x, DLG_Position[i].y);
	}

	fclose(pFile);
}