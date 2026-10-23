

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Thu Oct 15 14:07:03 2009
 */
/* Compiler settings for .\NdoorsStarter.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_INdoorsGameStarter,0xED9E837D,0x9A64,0x4D2A,0x99,0x9D,0x47,0xA9,0xEF,0x73,0x99,0xF9);


MIDL_DEFINE_GUID(IID, LIBID_NdoorsStarterLib,0x71CC369F,0x2803,0x407B,0xB6,0x0D,0xED,0x76,0x4B,0x92,0x33,0xDA);


MIDL_DEFINE_GUID(IID, DIID__INdoorsGameStarterEvents,0x1121DC70,0x9B47,0x4D7E,0x91,0xD9,0xCD,0x17,0x53,0x56,0x2A,0x14);


MIDL_DEFINE_GUID(CLSID, CLSID_NdoorsGameStarter,0xBDAF9A81,0x8FDE,0x4F9B,0xA9,0x1B,0xCC,0xCD,0x3C,0xF0,0x03,0xA4);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



