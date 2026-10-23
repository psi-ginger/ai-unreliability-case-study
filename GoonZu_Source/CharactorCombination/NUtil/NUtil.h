#ifndef _NUTIL_H
#define _NUTIL_H

#define _delete(p)			{ if(p) { delete (p); (p)=NULL; } }
#define _delete_array(p)	{ if(p) { delete [] (p); (p)=NULL; } }
#define _release(p)			{ if(p) { (p)->Release(); (p)=NULL; } }

#endif

