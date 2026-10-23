#include "stdafx.h"
#include "resource.h"
#include "CharacterDrawing.h"
#include "XSPRMgr.h"

#include "Graphic/Graphic.h"
#include "Graphic/Graphic_Capsule.h"
#include "NGraphics/Graphics.h"

char strPart[][64] = { "안찍음", "머리", "몸통 및 다리", "오른손", "왼손", "망토", "장비", "말", "말 머리"};
enum { NONE, HEAD, BODY, RIGHTHAND, LEFTHAND, MANTLE, EQUIPMENT, HORSE };
char strDirection[][64] = { "0", "1", "2", "3", "4", "5", "6", "7" };
char strSex[][10] = { "일본남", "일본여", "서양남", "서양여" };

LPDIRECTDRAWSURFACE7 lpSurface = NULL;

//#define	TILE_WIDTH				220
//#define	TILE_HEIGHT				200

CXSPRMgr	g_XSPRMgr;
HINSTANCE	g_hInst;								// current instance
HWND		g_hDlg;


int					g_DrawingSequence[ 5 ][ MAX_DRAWING_SEQUENCE_NUM ];
bool				g_bAnimation = false;
bool				g_bStartEnd = false;
bool				g_bWaitAnimation = false;
char				g_cActionIndex = 0;
char				g_cDirectionIndex = SOUTH_EAST;
int					g_cHelmetIndex = 0;
int					g_cArmorIndex = 0;
int					g_cWeaponIndex = 0;
int					g_cHorseIndex = 0;
int					g_cMantleIndex = 0;
char				g_cSex = 1;
char				g_cDelayTimeIndex = 2;
int					g_siDelayTime = 100;

CCharacterDrawing	g_CharacterDrawing;


BOOL		CALLBACK		CharacterCombinationProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



void Initialize()
{
	if ( g_bAnimation )
	{
		KillTimer( g_hDlg, 0 );
		g_bAnimation = false;
	}

	g_bWaitAnimation = false;
	
	g_cActionIndex = 0;

	g_CharacterDrawing.Initialize();

	HWND hCheckBox = GetDlgItem( g_hDlg, IDC_ANIMATION_CHECK );

	if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
	{
		SendMessage( hCheckBox, BM_SETCHECK, BST_UNCHECKED, NULL );
	}

	HWND hCombo;

	
	for ( int j = 0; j < MAX_DRAWING_SEQUENCE_NUM - 1; ++j )
	{
		hCombo = GetDlgItem( g_hDlg, IDC_FIRST_COMBO + j );
	
		for ( int i = 0; i < MAX_DRAWING_SEQUENCE_NUM - 1; ++i )
		{
			SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
		}
	}

	hCombo = GetDlgItem( g_hDlg, IDC_SEX_COMBO );
	SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
	g_cSex = 1;

	g_cHelmetIndex = 0;
	g_cArmorIndex = 0;
	g_cWeaponIndex = 0;
	g_cHorseIndex = 0;
	g_cMantleIndex = 0;

	int siTempCount = 0;

	hCombo = GetDlgItem( g_hDlg, IDC_ACTION_COMBO );
	siTempCount = SendMessage( hCombo, CB_GETCOUNT, 0, 0 );

	for ( int tt = 0; tt < siTempCount; ++tt )
	{
		SendMessage( hCombo, CB_DELETESTRING, 0, 0 );
	}
	
	for ( int k = 0; k < g_CharacterDrawing.GetActionNum(); ++k )
	{
		SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) g_CharacterDrawing.GetActionName( k ) );
		SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
	}

	unsigned int m = 0;
	hCombo = GetDlgItem( g_hDlg, IDC_HORSE_COMBO );

	siTempCount = SendMessage( hCombo, CB_GETCOUNT, 0, 0 );

	for ( k = 0; k < siTempCount; ++k )
	{
		SendMessage( hCombo, CB_DELETESTRING, 0, 0 );
	}

	for ( m = 0; m < g_CharacterDrawing.GetHorseNum(); ++m )
	{
		SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) g_CharacterDrawing.GetHorseName( m ) );
		SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
		EnableWindow( hCombo, FALSE );
	}
		
	memset( g_DrawingSequence, 0, sizeof( int ) * 5 * MAX_DRAWING_SEQUENCE_NUM );

	g_graphics.ClearSurface( lpSurface, RGB16_TRANS );
	GP.UpdateScreen( g_hDlg, lpSurface, TILE_WIDTH, TILE_HEIGHT, 0, 0, 26, 351, true, TILE_WIDTH, TILE_HEIGHT );


}

bool IsSameDrawingSequenceExist( int *TempDrawingSequence )
{
	int i, j;

	for ( i = 0; i < 5; ++i )
	{
		for ( j = i + 1; j < MAX_DRAWING_SEQUENCE_NUM - 1; ++j )
		{ 
			if ( TempDrawingSequence[ i ] != -1 && TempDrawingSequence[ i ] != 0 )
			{
				if ( TempDrawingSequence[ i ] == TempDrawingSequence[ j ] )
				{
					return true;
				}
			}
		}
	}
	
	return false;
}


bool GetSequenceInfo()
{
	HWND hCombo;

	int TempDrawingSequence[ MAX_DRAWING_SEQUENCE_NUM ];

	for ( int i = 0; i < MAX_DRAWING_SEQUENCE_NUM - 1 ; ++i )
	{
		hCombo = GetDlgItem( g_hDlg, IDC_FIRST_COMBO + i );

		TempDrawingSequence[ i ] = SendMessage( hCombo, CB_GETCURSEL, 0, 0 );

		if ( g_cActionIndex < HSR1 )
		{
			if ( TempDrawingSequence[ i ] == HORSE || TempDrawingSequence[ i ] <= 0)
			{
				TempDrawingSequence[ i ] = 0;
				SendMessage( hCombo, CB_SETCURSEL, 0, 0 );				
			}
		}
	}

	if ( IsSameDrawingSequenceExist( TempDrawingSequence ) )
	{
		MessageBox( g_hDlg, "선택 항목이 중복되어 있습니다.\n확인해 주십시오.", "선택 확인 요망", MB_OK );
		return false;
	}
	
	for ( int k = 0; k < MAX_DRAWING_SEQUENCE_NUM; ++k )
	{
		if ( g_cDirectionIndex > NORTH_WEST )
		{
			g_DrawingSequence[ NORTH_WEST - ( g_cDirectionIndex - NORTH_WEST ) ][ k ] = TempDrawingSequence[ k ];
		}
		else
		{
			g_DrawingSequence[ g_cDirectionIndex ][ k ] = TempDrawingSequence[ k ];
		}
	}

	return true;
}

void LoadSequenceData()
{
	HWND hCombo;

	for ( int i = 0; i < MAX_DRAWING_SEQUENCE_NUM - 1; ++i )
	{
		hCombo = GetDlgItem( g_hDlg, IDC_FIRST_COMBO + i );

		if ( g_cDirectionIndex > NORTH_WEST )
		{
			SendMessage( hCombo, CB_SETCURSEL, g_DrawingSequence[ NORTH_WEST - ( g_cDirectionIndex - NORTH_WEST ) ][ i ], 0 );
		}
		else
		{
			SendMessage( hCombo, CB_SETCURSEL, g_DrawingSequence[ g_cDirectionIndex ][ i ], 0 );
		}
	}

	return;
}

void GetActionInfo()
{
	HWND hCombo;
	hCombo = GetDlgItem( g_hDlg, IDC_ACTION_COMBO );

	int tmpindex = (char)SendMessage( hCombo, CB_GETCURSEL, 0, 0 );

	if (tmpindex > 0)
        g_cActionIndex = tmpindex;
	
	return;
}

bool GetItemInfo()
{
	HWND hCombo;
	hCombo = GetDlgItem( g_hDlg, IDC_HELMET_COMBO );
	g_cHelmetIndex = SendMessage( hCombo, CB_GETCURSEL, 0, 0 );

	hCombo = GetDlgItem( g_hDlg, IDC_ARMOR_COMBO );
	g_cArmorIndex = SendMessage( hCombo, CB_GETCURSEL, 0, 0 );

	hCombo = GetDlgItem( g_hDlg, IDC_WEAPON_COMBO );
	g_cWeaponIndex = SendMessage( hCombo, CB_GETCURSEL, 0, 0 );
	
	hCombo = GetDlgItem( g_hDlg, IDC_MANTLE_COMBO );
	g_cMantleIndex = SendMessage( hCombo, CB_GETCURSEL, 0, 0 );

	hCombo = GetDlgItem( g_hDlg, IDC_HORSE_COMBO );
	if ( g_cActionIndex > SPCU )
	{
		EnableWindow( hCombo, TRUE );
		g_cHorseIndex = (char)SendMessage( hCombo, CB_GETCURSEL, 0, 0 );
	}
	else
	{
		EnableWindow( hCombo, FALSE );
	}

		if ( g_cActionIndex > SPCU )
	{
		if (  g_cActionIndex > SPCU && g_cActionIndex < HITR  )
		{
			if ( g_cActionIndex - HSR1 != g_CharacterDrawing.GetWeaponActionIndex( g_cWeaponIndex )  )
			{
				MessageBox( g_hDlg, "액션과 아이템이 틀렸습니다.\n확인해 주십시오.", "Error", MB_OK );
				return false;
			}
		}
	}
	else
	{
		if ( g_cActionIndex < HITU )
		{
			if ( g_cActionIndex != g_CharacterDrawing.GetWeaponActionIndex( g_cWeaponIndex ) )
			{
				MessageBox( g_hDlg, "액션과 아이템이 틀렸습니다.\n확인해 주십시오.", "Error", MB_OK );
				return false;
			}
		}
	}

	/*
	if ( g_cActionIndex > SPCU && g_cWeaponIndex == 0 )
	{
		MessageBox( g_hDlg, "맨손인 경우 말을 탈 수 없습니다.", "Error", MB_OK );
		return false;
	}
	*/

	return true;
}


void DirectionChange( char cDirection )
{

	if ( g_cActionIndex == DTHU || g_cActionIndex == DTHR )
	{
		MessageBox( g_hDlg, "한 방향 입니다.", "다른 방향 없음", MB_OK );
		return;
	}
	else
	{
		g_cDirectionIndex = cDirection;

		LoadSequenceData();

		if ( ! GetSequenceInfo() ) 
		{
			return;
		}

		if ( cDirection > NORTH_WEST )
		{
			g_CharacterDrawing.SetCurrentDirection( cDirection, g_DrawingSequence[ NORTH_WEST - ( g_cDirectionIndex - NORTH_WEST ) ] );
		}
		else
		{
			g_CharacterDrawing.SetCurrentDirection( cDirection, g_DrawingSequence[ cDirection ] );
		}
		

		//여기 아래 주석 부분은 예외 처리 적인 기능임 솔직히 필요는 없지만...
		/////////////////////////////////////////////////////////////////////
		GetActionInfo();

	
		if ( ! GetItemInfo() )
		{
			return;
		}

		char*	pszHorseHeadCode = NULL;
		HWND hHorseHeadCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_HORSEHEAD_SHOW );
		if ( SendMessage( hHorseHeadCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
		{
			pszHorseHeadCode = g_CharacterDrawing.GetHorseHeadCode( g_cHorseIndex );
			if(pszHorseHeadCode == NULL || strlen(pszHorseHeadCode) <= 0)
				pszHorseHeadCode = NULL;
		}

		if ( g_cActionIndex > SPCU )
		{
			HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );
			
			if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
			{
				g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex ), g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
			}
			else
			{
				g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
			}
		}
		else
		{
			HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );
			
			if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
			{
				g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex ), NULL, NULL);
			}
			else
			{
				g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, NULL, NULL);
			}			
		}	

		if ( g_cDirectionIndex > NORTH_WEST )
		{
			g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ NORTH_WEST - ( g_cDirectionIndex - NORTH_WEST ) ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
		}
		else
		{
			g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ g_cDirectionIndex ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
		}
		//////////////////////////////////////////////////////////////////////
		
		
		g_CharacterDrawing.DrawNextFrame();
	}
	
	return;
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	g_hInst = hInstance;
	
	DialogBox( g_hInst, MAKEINTRESOURCE( IDD_CHARACTERCOMBINATION_DIALOG ), HWND_DESKTOP, CharacterCombinationProc );
	
	return 0;
}

BOOL CALLBACK CharacterCombinationProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int		i, j, k;
	int		WM_ID;
	HWND	hCombo;
	HWND	hCheckBox;

	switch (message)
	{
	case WM_INITDIALOG:
		{
			g_hDlg = hDlg;
			g_cActionIndex = 0;

			for ( j = 0; j < MAX_DRAWING_SEQUENCE_NUM - 1; ++j )
			{
				hCombo = GetDlgItem( hDlg, IDC_FIRST_COMBO + j );
			
				for ( i = 0; i < MAX_PART_NUM; ++i )
				{
					SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) strPart[ i ] );
					SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
				}
			}


			/*
			for ( k = 0; k < 8; ++k )
			{
				hCombo = GetDlgItem( hDlg, IDC_DIRECTION_COMBO );

				SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) strDirection[ k ] );
				SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
			}
			*/

			for ( k = 0; k < g_CharacterDrawing.GetActionNum(); ++k )
			{
				hCombo = GetDlgItem( hDlg, IDC_ACTION_COMBO );
				
				SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) g_CharacterDrawing.GetActionName( k ) );
				SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
			}

			for ( unsigned int m = 0; m < g_CharacterDrawing.GetHelmetNum(); ++m )
			{
				hCombo = GetDlgItem( hDlg, IDC_HELMET_COMBO );
				
				SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) g_CharacterDrawing.GetHelmetName( m ) );
				SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
			}

			for ( m = 0; m < g_CharacterDrawing.GetArmorNum(); ++m )
			{
				hCombo = GetDlgItem( hDlg, IDC_ARMOR_COMBO );
				
				SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) g_CharacterDrawing.GetArmorName( m ) );
				SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
			}

			

			for ( m = 0; m < g_CharacterDrawing.GetWeaponNum(); ++m )
			{
				hCombo = GetDlgItem( hDlg, IDC_WEAPON_COMBO );

				SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) g_CharacterDrawing.GetWeaponName( m ) );
				SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
			}

			for ( m = 0; m < g_CharacterDrawing.GetMantleNum(); ++m )
			{
				hCombo = GetDlgItem( hDlg, IDC_MANTLE_COMBO );

				SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) g_CharacterDrawing.GetMantleName( m ) );
				SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
			}

			for ( m = 0; m < g_CharacterDrawing.GetHorseNum(); ++m )
			{
				hCombo = GetDlgItem( hDlg, IDC_HORSE_COMBO );
				SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) g_CharacterDrawing.GetHorseName( m ) );
				SendMessage( hCombo, CB_SETCURSEL, 0, 0 );
				EnableWindow( hCombo, FALSE );
			}

			hCombo = GetDlgItem( hDlg, IDC_SEX_COMBO );
			SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) strSex[ 0 ] );
			SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) strSex[ 1 ] );
			SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) strSex[ 2 ] );
			SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) strSex[ 3 ] );
			SendMessage( hCombo, CB_SETCURSEL, 0, 0 );

			int siCount = 0;
			
			hCombo = GetDlgItem( hDlg, IDC_MINFILE_COMBO );

			while ( true )
			{
				char *pTemp = g_CharacterDrawing.FileName( siCount );

				if ( pTemp == NULL )
				{
					break;
				}

				SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) pTemp );
				//SendMessage( hCombo, CB_SETCURSEL, 0, 0 );

				++siCount;
			}


			
			hCombo = GetDlgItem( hDlg, IDC_DELAYTIME_COMBO );

			
			char pTemp[ 10 ] = "";

			for ( int i = 0; i < 5; ++i )
			{
				memset( pTemp, 0, sizeof( pTemp ) );
				 _itoa( i, pTemp, 10 );
				 SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) pTemp );
			}

			SendMessage( hCombo, CB_SETCURSEL, 2, 0 );
			
			if ( GP.InitGP( hDlg, MODE_GRAPHICS800_600, lpSurface, TILE_WIDTH, TILE_HEIGHT ) == TRUE )
			{
			}
			else
			{
				MessageBox( NULL, "CrewateDirectDraw()", NULL, MB_OK );
				//clGrp.Error( "CrewateDirectDraw()" );
				return -1;
			}

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			GP.UpdateScreen( g_hDlg, lpSurface, TILE_WIDTH, TILE_HEIGHT, 0, 0, 26, 351, true, TILE_WIDTH, TILE_HEIGHT );

			return TRUE;
		}
		break;
	case WM_COMMAND:
		{
			WM_ID = LOWORD ( wParam );

			switch( WM_ID )
			{
			case IDOK:
				{
					KillTimer( hDlg, 0 );
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
				}
				break;
			case IDC_ACTION_COMBO:
				{
					hCombo = GetDlgItem( g_hDlg, IDC_ACTION_COMBO );
					switch( HIWORD( wParam ) )
					{
					case CBN_SELCHANGE:
						{
							g_cActionIndex = ( char ) SendMessage( hCombo, CB_GETCURSEL, 0, 0 );

							if ( g_cActionIndex > SPCU )
							{
								hCombo = GetDlgItem( hDlg, IDC_HORSE_COMBO );
								EnableWindow( hCombo, TRUE );
							}
							else
							{
								hCombo = GetDlgItem( hDlg, IDC_HORSE_COMBO );
								EnableWindow( hCombo, FALSE );
							}
						}
						break;						
					}
				}
				break;
			case IDC_SEX_COMBO:
				{
					hCombo = GetDlgItem( g_hDlg, IDC_SEX_COMBO );
					switch( HIWORD( wParam ) )
					{
					case CBN_SELCHANGE:
						{
							g_cSex = ( char ) SendMessage( hCombo, CB_GETCURSEL, 0, 0 );
							++g_cSex;		// 0 이 없고, 1부터 시작되므로...
						}
						break;						
					}
				}
				break;
			case IDC_DELAYTIME_COMBO:
				{
					hCombo = GetDlgItem( g_hDlg, IDC_DELAYTIME_COMBO );
					switch ( HIWORD( wParam ) )
					{
					case CBN_SELCHANGE:
						{
							g_cDelayTimeIndex = (char)SendMessage( hCombo, CB_GETCURSEL, 0, 0 );
							g_siDelayTime = g_cDelayTimeIndex * 25 + 50;

							if ( g_bAnimation )
							{
								KillTimer( hDlg, 0 );
								SetTimer( g_hDlg, 0, g_siDelayTime, NULL );
							}
						}
						break;
					}
				}
				break;
			case IDC_MINFILE_COMBO:
				{
					hCombo = GetDlgItem( g_hDlg, IDC_MINFILE_COMBO );
					switch( HIWORD( wParam ) )
					{
					case CBN_SELCHANGE:
						{
							Initialize();

							int siIndex = (char)SendMessage( hCombo, CB_GETCURSEL, 0, 0 );
							
							g_CharacterDrawing.LoadMinFile( g_CharacterDrawing.FileName( siIndex ), (int**)g_DrawingSequence, &g_cActionIndex, &g_cSex, &g_cDelayTimeIndex );
							hCombo = GetDlgItem( hDlg, IDC_DELAYTIME_COMBO );
							SendMessage( hCombo, CB_SETCURSEL, g_cDelayTimeIndex, 0 );
							g_siDelayTime = 50 + 25 * g_cDelayTimeIndex;

							
							LoadSequenceData();
							
							HWND hCombo;
							hCombo = GetDlgItem( g_hDlg, IDC_ACTION_COMBO );
							SendMessage( hCombo, CB_SETCURSEL, g_cActionIndex, 0 );

							hCombo = GetDlgItem( g_hDlg, IDC_SEX_COMBO );
							SendMessage( hCombo, CB_SETCURSEL, g_cSex - 1, 0 );
							
							if ( ! GetItemInfo() )
							{
								return FALSE;
							}

							char*	pszHorseHeadCode = NULL;
							HWND hHorseHeadCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_HORSEHEAD_SHOW );
							if ( SendMessage( hHorseHeadCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
							{
								pszHorseHeadCode = g_CharacterDrawing.GetHorseHeadCode( g_cHorseIndex );
								if(pszHorseHeadCode == NULL || strlen(pszHorseHeadCode) <= 0)
									pszHorseHeadCode = NULL;
							}
							
							if ( g_cActionIndex > SPCU )
							{
								HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );
								if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
								{
									g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex), g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
								}
								else
								{
									g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
								}
							}
							else
							{
								HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );
								
								if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
								{
									g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex), NULL, NULL );
								}
								else
								{
									g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, NULL, NULL);
								}
							}	

							if ( g_cDirectionIndex > NORTH_WEST )
							{
								g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ NORTH_WEST - ( g_cDirectionIndex - NORTH_WEST ) ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
							}
							else
							{
								g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ g_cDirectionIndex ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
							}

						}
						break;
					}
				}
				break;
			case IDC_INITIALIZE_BUTTON:
				{
					Initialize();
				}
				break;
			case IDC_WAITANIMATION_BUTTON:
				{
					g_bWaitAnimation = ! g_bWaitAnimation;
				}
				break;
			case IDC_APPLY_BUTTON:
				{
					if ( ! GetSequenceInfo() ) 
					{
						return FALSE;
					}
					
					GetActionInfo();

					if ( ! GetItemInfo() )
					{
						return FALSE;
					}

					char*	pszHorseHeadCode = NULL;
					HWND hHorseHeadCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_HORSEHEAD_SHOW );
					if ( SendMessage( hHorseHeadCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
					{
						pszHorseHeadCode = g_CharacterDrawing.GetHorseHeadCode( g_cHorseIndex );
						if(pszHorseHeadCode == NULL || strlen(pszHorseHeadCode) <= 0)
							pszHorseHeadCode = NULL;
					}

					if ( g_cActionIndex > SPCU )
					{
						HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );
			
						if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
						{
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex), g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
						}
						else
						{
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
						}
					}
					else
					{	
						HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );
						
						if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
						{
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex ), NULL, NULL );
						}
						else
						{
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, NULL, NULL );
						}
					}	

					if ( g_cDirectionIndex > NORTH_WEST )
					{
						g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ NORTH_WEST - ( g_cDirectionIndex - NORTH_WEST ) ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
					}
					else
					{
						g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ g_cDirectionIndex ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
					}

				}
				break;
			case IDC_ANIMATION_STARTEND:
				{
					HWND hCheckBox = GetDlgItem( g_hDlg, IDC_ANIMATION_STARTEND );

					if ( g_bStartEnd )
					{
						SendMessage( hCheckBox, BM_SETCHECK, BST_UNCHECKED, NULL );
						g_bStartEnd = false;
					}
					else
					{
						SendMessage( hCheckBox, BM_SETCHECK, BST_CHECKED, NULL );
						g_bStartEnd = true;
					}
				}
				break;
			case IDC_ANIMATION_CHECK:
				{
					if ( ! GetSequenceInfo() ) 
					{
						return FALSE;
					}

					GetActionInfo();

					if ( ! GetItemInfo() )
					{
						return FALSE;
					}

					char*	pszHorseHeadCode = NULL;
					HWND hHorseHeadCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_HORSEHEAD_SHOW );
					if ( SendMessage( hHorseHeadCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
					{
						pszHorseHeadCode = g_CharacterDrawing.GetHorseHeadCode( g_cHorseIndex );
						if(pszHorseHeadCode == NULL || strlen(pszHorseHeadCode) <= 0)
							pszHorseHeadCode = NULL;
					}

					if ( g_cActionIndex > SPCU )
					{
						HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );
						
						if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
						{ 
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex), g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
						}
						else
						{
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
						}
					}
					else
					{						
						HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );
						
						if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
						{
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex), NULL, NULL );
						}
						else
						{
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, NULL, NULL );
						}
					}
					
					if ( g_cDirectionIndex > NORTH_WEST )
					{
						g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ NORTH_WEST - ( g_cDirectionIndex - NORTH_WEST ) ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
					}
					else
					{
						g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ g_cDirectionIndex ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
					}

					hCheckBox = GetDlgItem( g_hDlg, IDC_ANIMATION_CHECK );

					if ( g_bAnimation )
					{
						SendMessage( hCheckBox, BM_SETCHECK, BST_UNCHECKED, NULL );
						KillTimer( hDlg, 0 );
						g_bAnimation = false;
					}
					else
					{
						SendMessage( hCheckBox, BM_SETCHECK, BST_CHECKED, NULL );
						SetTimer( hDlg, 0, g_siDelayTime, NULL );
						g_bAnimation = true;
					}
				}
				break;
			case IDC_PLAY_STOP_BUTTON:
				{
					if ( ! GetSequenceInfo() ) 
					{
						return FALSE;
					}

					GetActionInfo();

					if ( ! GetItemInfo() )
					{
						
						return FALSE;
					}

					char*	pszHorseHeadCode = NULL;
					HWND hHorseHeadCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_HORSEHEAD_SHOW );
					if ( SendMessage( hHorseHeadCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
					{
						pszHorseHeadCode = g_CharacterDrawing.GetHorseHeadCode( g_cHorseIndex );
						if(pszHorseHeadCode == NULL || strlen(pszHorseHeadCode) <= 0)
							pszHorseHeadCode = NULL;
					}

					if ( g_cActionIndex > SPCU )
					{
						HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );
						
						if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
						{
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex ), g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
						}
						else
						{
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
						}
						
					}
					else
					{	
						HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );
						
						if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
						{
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex ), NULL, NULL );
						}
						else
						{
							g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, NULL, NULL );
						}
					}

					if ( g_cDirectionIndex > NORTH_WEST )
					{
						g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ NORTH_WEST - ( g_cDirectionIndex - NORTH_WEST ) ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
					}
					else
					{
						g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ g_cDirectionIndex ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
					}

					hCheckBox = GetDlgItem( g_hDlg, IDC_ANIMATION_CHECK );

					if ( g_bAnimation )
					{
						SendMessage( hCheckBox, BM_SETCHECK, BST_UNCHECKED, NULL );
						KillTimer( hDlg, 0 );
						g_bAnimation = false;
					}
					else
					{
						SendMessage( hCheckBox, BM_SETCHECK, BST_CHECKED, NULL );						
						SetTimer( hDlg, 0, g_siDelayTime, NULL );
						g_bAnimation = true;
					}
				}
				break;
			case IDC_PRE_BUTTON:
				{
					if ( g_bAnimation )
					{
						hCheckBox = GetDlgItem( g_hDlg, IDC_ANIMATION_CHECK );
						SendMessage( hCheckBox, BM_SETCHECK, BST_UNCHECKED, NULL );
						KillTimer( hDlg, 0 );
						g_bAnimation = false;
					}

					g_CharacterDrawing.DrawPreFrame();
				}
				break;
			case IDC_NEXT_BUTTON:
				{
					if ( g_bAnimation )
					{
						hCheckBox = GetDlgItem( g_hDlg, IDC_ANIMATION_CHECK );
						SendMessage( hCheckBox, BM_SETCHECK, BST_UNCHECKED, NULL );
						KillTimer( hDlg, 0 );
						g_bAnimation = false;
					}

					g_CharacterDrawing.DrawNextFrame();
				}
				break;
			case IDC_SOUTH_BUTTON:
				{
					DirectionChange( SOUTH_EAST );					
				}
				break;
			case IDC_SOUTH_WEST_BUTTON:
				{
					DirectionChange( SOUTH );
				}
				break;
			case IDC_WEST_BUTTON:
				{
					DirectionChange( SOUTH_WEST );
				}
				break;
			case IDC_NORTH_WEST_BUTTON:
				{
					DirectionChange( WEST );
				}
				break;
			case IDC_NORTH_BUTTON:
				{
					DirectionChange( NORTH_WEST );
				}
				break;
			case IDC_NORTH_EAST_BUTTON:
				{
					DirectionChange( NORTH );
				}
				break;
			case IDC_EAST_BUTTON:
				{
					DirectionChange( NORTH_EAST );
				}
				break;
			case IDC_SOUTH_EAST_BUTTON:
				{
					DirectionChange( EAST );
				}
				break;
			case IDC_SAVE_BUTTON:
				{
					g_CharacterDrawing.SaveMinFile( g_cDelayTimeIndex );
					
					
					hCombo = GetDlgItem( hDlg, IDC_MINFILE_COMBO );
					int siCount = SendMessage( hCombo, CB_GETCOUNT, 0, 0);

					for ( i = 0; i < siCount; ++i )
					{
						SendMessage( hCombo, CB_DELETESTRING, 0, 0);
					}

					siCount = 0;

					g_CharacterDrawing.ReloadMinFileList();

					
					
					while ( true )
					{
						char *pTemp = g_CharacterDrawing.FileName( siCount );

						if ( pTemp == NULL )
						{
							break;
						}

						SendMessage( hCombo, CB_ADDSTRING, 0, (WPARAM) pTemp );
						SendMessage( hCombo, CB_SETCURSEL, -1, 0 );

						++siCount;
					}
				}
				break;
			default:
				{
				}
				break;
			}
		}
		break;
	case WM_PAINT:
		{
			//clGrp.FillScreen( COLOR_TRANS );
			//clGrp.UpdateScreen( g_hDlg, lpSurface, 26, 351, 0, 0, 0 );
			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );
			GP.UpdateScreen( g_hDlg, lpSurface, TILE_WIDTH, TILE_HEIGHT, 0, 0, 26, 351, true, TILE_WIDTH, TILE_HEIGHT );
		}
		break;
	case WM_TIMER:
		{
			/*
			if ( ! GetSequenceInfo() ) 
			{
				return FALSE;
			}

			GetActionInfo();

			if ( ! GetItemInfo() )
			{
				return FALSE;
			}

			if ( g_cActionIndex > SPCU )
			{
				HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );

				if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
				{
					g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex ), g_CharacterDrawing.GetHorseCode( g_cHorseIndex ) );
				}
				else
				{
					g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, g_CharacterDrawing.GetHorseCode( g_cHorseIndex ) );
				}
			}
			else
			{	
				HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );

				if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
				{
					g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex ), NULL );
				}
				else
				{
					g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, NULL );
				}
			}

			if ( g_cDirectionIndex > NORTH_WEST )
			{
				g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ NORTH_WEST - ( g_cDirectionIndex - NORTH_WEST ) ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
			}
			else
			{
				g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ g_cDirectionIndex ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
			}

			hCheckBox = GetDlgItem( g_hDlg, IDC_ANIMATION_CHECK );

			if ( g_bAnimation )
			{
				SendMessage( hCheckBox, BM_SETCHECK, BST_UNCHECKED, NULL );
				KillTimer( hDlg, 0 );
				g_bAnimation = false;
			}
			else
			{
				SendMessage( hCheckBox, BM_SETCHECK, BST_CHECKED, NULL );						
				SetTimer( hDlg, 0, g_siDelayTime, NULL );
				g_bAnimation = true;
			}
			*/
			bool AnimationChange = false;

			if ( g_CharacterDrawing.GetCurrentAnimationIndex() + 1 >= g_CharacterDrawing.GetAnimationEndIndex() )
			{
				AnimationChange = true;
			}
			/*
			if ( g_bWaitAnimation )
			{
				if ( g_cActionIndex < HSR1 )
				{
				if (  + 1 >=	g_CharacterDrawing.GetAnimationEndIndex()
				g_cActionIndex = WT1U;
				g_cActionIndex = WT2U;
				}
				else
				{
				g_cActionIndex = WT1R;
				g_cActionIndex = WT2R;
				}
				}
				*/
			
			if(g_bStartEnd)
			{
				if( g_CharacterDrawing.GetCurrentAnimationIndex() > g_CharacterDrawing.GetAnimationStartIndex() &&
					g_CharacterDrawing.GetCurrentAnimationIndex() < g_CharacterDrawing.GetAnimationEndIndex())
				{
					g_CharacterDrawing.SetCurrentAnimationIndex(g_CharacterDrawing.GetAnimationEndIndex() - 1);
				}
				g_CharacterDrawing.DrawNextFrame();
			}
			else
			{
				g_CharacterDrawing.DrawNextFrame();
			}

			if ( g_bWaitAnimation )
			{
				// 애니메이션 한번 다 된 경우
				if ( AnimationChange )
				{
					if ( g_CharacterDrawing.GetCurrentAnimationIndex() == g_CharacterDrawing.GetAnimationStartIndex() )
					{

						if ( g_cActionIndex < HSR1 )
						{
							if ( g_cActionIndex == WT1U )
							{
								g_cActionIndex = WT2U;
							}
							else if ( g_cActionIndex == WT2U )
							{
								g_cActionIndex = WT1U;
							}
							else
							{
								g_cActionIndex = WT1U;
							}
						}
						else
						{
							if ( g_cActionIndex == WT1R )
							{
								g_cActionIndex = WT2R;
							}
							else if ( g_cActionIndex == WT2R )
							{
								g_cActionIndex = WT1R;
							}
							else
							{
								g_cActionIndex = WT1R;
							}
						}

						HWND hCombo = GetDlgItem( g_hDlg, IDC_ACTION_COMBO );
						SendMessage( hCombo, CB_SETCURSEL, g_cActionIndex, 0 );

						if ( ! GetSequenceInfo() ) 
						{
							break;
						}

						GetActionInfo();

						if ( ! GetItemInfo() )
						{
							break;
						}

						char*	pszHorseHeadCode = NULL;
						HWND hHorseHeadCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_HORSEHEAD_SHOW );
						if ( SendMessage( hHorseHeadCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
						{
							pszHorseHeadCode = g_CharacterDrawing.GetHorseHeadCode( g_cHorseIndex );
							if(pszHorseHeadCode == NULL || strlen(pszHorseHeadCode) <= 0)
								pszHorseHeadCode = NULL;
						}

						if ( g_cActionIndex > SPCU )
						{
							HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );

							if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
							{
								g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex ), g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
							}
							else
							{
								g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, g_CharacterDrawing.GetHorseCode( g_cHorseIndex ), pszHorseHeadCode );
							}
						}
						else
						{	
							HWND hCheckBox = GetDlgItem( g_hDlg, IDC_CHECK_MANTLE_SHOW );

							if ( SendMessage( hCheckBox, BM_GETCHECK, NULL, NULL ) == BST_CHECKED )
							{
								g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), g_CharacterDrawing.GetMantleCode( g_cMantleIndex ), NULL, NULL );
							}
							else
							{
								g_CharacterDrawing.LoadItemCodeData( g_cSex, g_CharacterDrawing.GetHelmetCode( g_cHelmetIndex ), g_CharacterDrawing.GetArmorCode( g_cArmorIndex ), g_CharacterDrawing.GetWeaponCode( g_cWeaponIndex ), NULL, NULL, NULL );
							}
						}

						if ( g_cDirectionIndex > NORTH_WEST )
						{
							g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ NORTH_WEST - ( g_cDirectionIndex - NORTH_WEST ) ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
						}
						else
						{
							g_CharacterDrawing.DrawCharacter( g_DrawingSequence[ g_cDirectionIndex ], g_cActionIndex, g_cDirectionIndex, 0, 0 );
						}

					}
				}
			}

		}
		break;
	case WM_CLOSE:
		{
			KillTimer( hDlg, 0 );
			EndDialog( hDlg, WM_CLOSE );
			return TRUE;
		}
		break;
	}

    return FALSE;
}