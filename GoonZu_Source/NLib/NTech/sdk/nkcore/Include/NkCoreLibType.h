#ifndef _NKCORELIBTYPE_H
#define _NKCORELIBTYPE_H

#ifdef NKCORE_EXPORT
#define NKCORE_ENTRY __declspec(dllexport)
#else
#ifdef NKCORE_IMPORT
#define NKCORE_ENTRY __declspec(dllimport)
#else

#define NKCORE_ENTRY
#endif
#endif

#endif