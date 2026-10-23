#include "EtcTool.h"


void MsgBox(char * szCaption, char*szFormat,... )
{
	char szBuffer[256];
    char *pArguments;

	pArguments=( char * )&szFormat + sizeof( szFormat );
	vsprintf( szBuffer, szFormat, pArguments );

	MessageBox(NULL, szBuffer, szCaption, MB_OK);
}
