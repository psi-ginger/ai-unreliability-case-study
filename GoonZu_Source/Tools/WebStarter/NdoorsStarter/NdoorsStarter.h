

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __NdoorsStarter_h__
#define __NdoorsStarter_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __INdoorsGameStarter_FWD_DEFINED__
#define __INdoorsGameStarter_FWD_DEFINED__
typedef interface INdoorsGameStarter INdoorsGameStarter;
#endif 	/* __INdoorsGameStarter_FWD_DEFINED__ */


#ifndef ___INdoorsGameStarterEvents_FWD_DEFINED__
#define ___INdoorsGameStarterEvents_FWD_DEFINED__
typedef interface _INdoorsGameStarterEvents _INdoorsGameStarterEvents;
#endif 	/* ___INdoorsGameStarterEvents_FWD_DEFINED__ */


#ifndef __NdoorsGameStarter_FWD_DEFINED__
#define __NdoorsGameStarter_FWD_DEFINED__

#ifdef __cplusplus
typedef class NdoorsGameStarter NdoorsGameStarter;
#else
typedef struct NdoorsGameStarter NdoorsGameStarter;
#endif /* __cplusplus */

#endif 	/* __NdoorsGameStarter_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __INdoorsGameStarter_INTERFACE_DEFINED__
#define __INdoorsGameStarter_INTERFACE_DEFINED__

/* interface INdoorsGameStarter */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_INdoorsGameStarter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ED9E837D-9A64-4D2A-999D-47A9EF7399F9")
    INdoorsGameStarter : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitStarter( 
            /* [in] */ BSTR GameTitle,
            /* [in] */ BSTR FileName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Run( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GameInstalled( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RunResult( 
            /* [retval][out] */ LONG *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct INdoorsGameStarterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INdoorsGameStarter * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INdoorsGameStarter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INdoorsGameStarter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            INdoorsGameStarter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            INdoorsGameStarter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            INdoorsGameStarter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            INdoorsGameStarter * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InitStarter )( 
            INdoorsGameStarter * This,
            /* [in] */ BSTR GameTitle,
            /* [in] */ BSTR FileName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Run )( 
            INdoorsGameStarter * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GameInstalled )( 
            INdoorsGameStarter * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RunResult )( 
            INdoorsGameStarter * This,
            /* [retval][out] */ LONG *pVal);
        
        END_INTERFACE
    } INdoorsGameStarterVtbl;

    interface INdoorsGameStarter
    {
        CONST_VTBL struct INdoorsGameStarterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INdoorsGameStarter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define INdoorsGameStarter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define INdoorsGameStarter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define INdoorsGameStarter_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define INdoorsGameStarter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define INdoorsGameStarter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define INdoorsGameStarter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define INdoorsGameStarter_InitStarter(This,GameTitle,FileName)	\
    (This)->lpVtbl -> InitStarter(This,GameTitle,FileName)

#define INdoorsGameStarter_Run(This)	\
    (This)->lpVtbl -> Run(This)

#define INdoorsGameStarter_get_GameInstalled(This,pVal)	\
    (This)->lpVtbl -> get_GameInstalled(This,pVal)

#define INdoorsGameStarter_get_RunResult(This,pVal)	\
    (This)->lpVtbl -> get_RunResult(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE INdoorsGameStarter_InitStarter_Proxy( 
    INdoorsGameStarter * This,
    /* [in] */ BSTR GameTitle,
    /* [in] */ BSTR FileName);


void __RPC_STUB INdoorsGameStarter_InitStarter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE INdoorsGameStarter_Run_Proxy( 
    INdoorsGameStarter * This);


void __RPC_STUB INdoorsGameStarter_Run_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE INdoorsGameStarter_get_GameInstalled_Proxy( 
    INdoorsGameStarter * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB INdoorsGameStarter_get_GameInstalled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE INdoorsGameStarter_get_RunResult_Proxy( 
    INdoorsGameStarter * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB INdoorsGameStarter_get_RunResult_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __INdoorsGameStarter_INTERFACE_DEFINED__ */



#ifndef __NdoorsStarterLib_LIBRARY_DEFINED__
#define __NdoorsStarterLib_LIBRARY_DEFINED__

/* library NdoorsStarterLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_NdoorsStarterLib;

#ifndef ___INdoorsGameStarterEvents_DISPINTERFACE_DEFINED__
#define ___INdoorsGameStarterEvents_DISPINTERFACE_DEFINED__

/* dispinterface _INdoorsGameStarterEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__INdoorsGameStarterEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1121DC70-9B47-4D7E-91D9-CD1753562A14")
    _INdoorsGameStarterEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _INdoorsGameStarterEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _INdoorsGameStarterEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _INdoorsGameStarterEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _INdoorsGameStarterEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _INdoorsGameStarterEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _INdoorsGameStarterEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _INdoorsGameStarterEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _INdoorsGameStarterEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _INdoorsGameStarterEventsVtbl;

    interface _INdoorsGameStarterEvents
    {
        CONST_VTBL struct _INdoorsGameStarterEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _INdoorsGameStarterEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _INdoorsGameStarterEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _INdoorsGameStarterEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _INdoorsGameStarterEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _INdoorsGameStarterEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _INdoorsGameStarterEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _INdoorsGameStarterEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___INdoorsGameStarterEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_NdoorsGameStarter;

#ifdef __cplusplus

class DECLSPEC_UUID("BDAF9A81-8FDE-4F9B-A91B-CCCD3CF003A4")
NdoorsGameStarter;
#endif
#endif /* __NdoorsStarterLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


