#ifndef _HELPER_H
#define _HELPER_H

#define _delete(p)			{ if(p) { delete (p); (p)=NULL; } }
#define _delete_array(p)	{ if(p) { delete [] (p); (p)=NULL; } }
#define _release(p)			{ if(p) { (p)->Release(); (p)=NULL; } }

#endif