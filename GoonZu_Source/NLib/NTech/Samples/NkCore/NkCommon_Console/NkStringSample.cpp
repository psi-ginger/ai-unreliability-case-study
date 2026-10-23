#include <NkString.h>

#include "display_helper.h"

#define EXCUTE_NKSTRING(a) a; CDispHelper::Get().PrintSource(#a); CDispHelper::disp_string((char*)kStr.GetString()); _getch();

void NkStringSample()
{
	EXCUTE_NKSTRING( NkStringA kStr );
	EXCUTE_NKSTRING( kStr = "Hello! World!" );
	EXCUTE_NKSTRING( kStr = "My name is Jewan." );
	EXCUTE_NKSTRING( kStr.Replace("Jewan", "Jeremy") );
	
}