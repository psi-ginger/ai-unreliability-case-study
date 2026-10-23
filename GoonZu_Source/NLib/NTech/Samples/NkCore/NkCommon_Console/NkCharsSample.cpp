#include <NkChars.h>

#include "display_helper.h"

#define EXCUTE_NKCHARS(s, a) a; CDispHelper::Get().PrintSource(#a); CDispHelper::disp_string((char*)kChars.GetString(), s); _getch();

#define CHARS_SIZE 10

void NkCharsSample()
{
	const size_t cnt = 10;
	EXCUTE_NKCHARS( CHARS_SIZE,		NkCharsA< cnt > kChars );
	EXCUTE_NKCHARS( CHARS_SIZE,		kChars = "hello!" );
	EXCUTE_NKCHARS( CHARS_SIZE,		kChars.Format("age=%d", 30) );
	EXCUTE_NKCHARS( CHARS_SIZE,		kChars.ToUpper() );
	EXCUTE_NKCHARS( CHARS_SIZE,		kChars = "abcdefghijklmnopqrstuvwxyz" );
	EXCUTE_NKCHARS( CHARS_SIZE,		kChars.Empty() );
	EXCUTE_NKCHARS( CHARS_SIZE,		kChars = "  JEWAN" );
	EXCUTE_NKCHARS( CHARS_SIZE,		kChars.TrimLeft(' ') );
	EXCUTE_NKCHARS( CHARS_SIZE,		kChars.ToLower() );
	EXCUTE_NKCHARS( CHARS_SIZE,		kChars.Append(" Hi") );
	EXCUTE_NKCHARS( CHARS_SIZE,		kChars += "!" );
	EXCUTE_NKCHARS( CHARS_SIZE,		kChars.Reverse() );

}