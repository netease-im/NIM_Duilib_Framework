

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jul 20 13:21:34 2017
 */
/* Compiler settings for image_ole.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef __image_ole_i_h__
#define __image_ole_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IComponentRegistrar_FWD_DEFINED__
#define __IComponentRegistrar_FWD_DEFINED__
typedef interface IComponentRegistrar IComponentRegistrar;
#endif 	/* __IComponentRegistrar_FWD_DEFINED__ */


#ifndef __IImageOle_FWD_DEFINED__
#define __IImageOle_FWD_DEFINED__
typedef interface IImageOle IImageOle;
#endif 	/* __IImageOle_FWD_DEFINED__ */


#ifndef __CompReg_FWD_DEFINED__
#define __CompReg_FWD_DEFINED__

#ifdef __cplusplus
typedef class CompReg CompReg;
#else
typedef struct CompReg CompReg;
#endif /* __cplusplus */

#endif 	/* __CompReg_FWD_DEFINED__ */


#ifndef ___IImageOleEvents_FWD_DEFINED__
#define ___IImageOleEvents_FWD_DEFINED__
typedef interface _IImageOleEvents _IImageOleEvents;
#endif 	/* ___IImageOleEvents_FWD_DEFINED__ */


#ifndef __ImageOle_FWD_DEFINED__
#define __ImageOle_FWD_DEFINED__

#ifdef __cplusplus
typedef class ImageOle ImageOle;
#else
typedef struct ImageOle ImageOle;
#endif /* __cplusplus */

#endif 	/* __ImageOle_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IComponentRegistrar_INTERFACE_DEFINED__
#define __IComponentRegistrar_INTERFACE_DEFINED__

/* interface IComponentRegistrar */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_IComponentRegistrar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a817e7a2-43fa-11d0-9e44-00aa00b6770a")
    IComponentRegistrar : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Attach( 
            /* [in] */ BSTR bstrPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterAll( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnregisterAll( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetComponents( 
            /* [out] */ SAFEARRAY * *pbstrCLSIDs,
            /* [out] */ SAFEARRAY * *pbstrDescriptions) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterComponent( 
            /* [in] */ BSTR bstrCLSID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnregisterComponent( 
            /* [in] */ BSTR bstrCLSID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IComponentRegistrarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IComponentRegistrar * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IComponentRegistrar * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IComponentRegistrar * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IComponentRegistrar * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IComponentRegistrar * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IComponentRegistrar * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IComponentRegistrar * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Attach )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterAll )( 
            IComponentRegistrar * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterAll )( 
            IComponentRegistrar * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetComponents )( 
            IComponentRegistrar * This,
            /* [out] */ SAFEARRAY * *pbstrCLSIDs,
            /* [out] */ SAFEARRAY * *pbstrDescriptions);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterComponent )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrCLSID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterComponent )( 
            IComponentRegistrar * This,
            /* [in] */ BSTR bstrCLSID);
        
        END_INTERFACE
    } IComponentRegistrarVtbl;

    interface IComponentRegistrar
    {
        CONST_VTBL struct IComponentRegistrarVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IComponentRegistrar_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IComponentRegistrar_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IComponentRegistrar_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IComponentRegistrar_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IComponentRegistrar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IComponentRegistrar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IComponentRegistrar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IComponentRegistrar_Attach(This,bstrPath)	\
    ( (This)->lpVtbl -> Attach(This,bstrPath) ) 

#define IComponentRegistrar_RegisterAll(This)	\
    ( (This)->lpVtbl -> RegisterAll(This) ) 

#define IComponentRegistrar_UnregisterAll(This)	\
    ( (This)->lpVtbl -> UnregisterAll(This) ) 

#define IComponentRegistrar_GetComponents(This,pbstrCLSIDs,pbstrDescriptions)	\
    ( (This)->lpVtbl -> GetComponents(This,pbstrCLSIDs,pbstrDescriptions) ) 

#define IComponentRegistrar_RegisterComponent(This,bstrCLSID)	\
    ( (This)->lpVtbl -> RegisterComponent(This,bstrCLSID) ) 

#define IComponentRegistrar_UnregisterComponent(This,bstrCLSID)	\
    ( (This)->lpVtbl -> UnregisterComponent(This,bstrCLSID) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IComponentRegistrar_INTERFACE_DEFINED__ */


#ifndef __IImageOle_INTERFACE_DEFINED__
#define __IImageOle_INTERFACE_DEFINED__

/* interface IImageOle */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IImageOle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FDEEBA15-2214-4AAA-8222-7354491BE26C")
    IImageOle : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LoadFromFile( 
            /* [in] */ BSTR lpszPathName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFaceId( 
            /* [in] */ LONG face_id) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFaceId( 
            LONG *face_id) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFaceIndex( 
            LONG face_index) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFaceIndex( 
            LONG *face_index) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetImageFile( 
            BSTR *file_name) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetImageRect( 
            LPRECT rect) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetImageFomat( 
            GUID *guid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SaveAsFile( 
            BSTR file_path) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetScaleSize( 
            BOOL bScale,
            int nWidth,
            int nHeight) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetBgColor( 
            COLORREF clrBg) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Draw( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFaceTag( 
            /* [in] */ BSTR face_tag) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFaceTag( 
            /* [out] */ BSTR *face_tag) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFont( 
            /* [in] */ BSTR font_name,
            /* [in] */ LONG font_size,
            /* [in] */ COLORREF clrText) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetGUID( 
            BSTR guid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGUID( 
            BSTR *guid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetScale( 
            LONG *scale) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LoadFromDescription( 
            /* [in] */ BSTR des) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IImageOleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IImageOle * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IImageOle * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IImageOle * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IImageOle * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IImageOle * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IImageOle * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IImageOle * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LoadFromFile )( 
            IImageOle * This,
            /* [in] */ BSTR lpszPathName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFaceId )( 
            IImageOle * This,
            /* [in] */ LONG face_id);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFaceId )( 
            IImageOle * This,
            LONG *face_id);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFaceIndex )( 
            IImageOle * This,
            LONG face_index);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFaceIndex )( 
            IImageOle * This,
            LONG *face_index);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetImageFile )( 
            IImageOle * This,
            BSTR *file_name);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetImageRect )( 
            IImageOle * This,
            LPRECT rect);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetImageFomat )( 
            IImageOle * This,
            GUID *guid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SaveAsFile )( 
            IImageOle * This,
            BSTR file_path);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetScaleSize )( 
            IImageOle * This,
            BOOL bScale,
            int nWidth,
            int nHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetBgColor )( 
            IImageOle * This,
            COLORREF clrBg);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Draw )( 
            IImageOle * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFaceTag )( 
            IImageOle * This,
            /* [in] */ BSTR face_tag);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFaceTag )( 
            IImageOle * This,
            /* [out] */ BSTR *face_tag);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFont )( 
            IImageOle * This,
            /* [in] */ BSTR font_name,
            /* [in] */ LONG font_size,
            /* [in] */ COLORREF clrText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IImageOle * This,
            BSTR guid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IImageOle * This,
            BSTR *guid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetScale )( 
            IImageOle * This,
            LONG *scale);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LoadFromDescription )( 
            IImageOle * This,
            /* [in] */ BSTR des);
        
        END_INTERFACE
    } IImageOleVtbl;

    interface IImageOle
    {
        CONST_VTBL struct IImageOleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IImageOle_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IImageOle_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IImageOle_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IImageOle_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IImageOle_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IImageOle_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IImageOle_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IImageOle_LoadFromFile(This,lpszPathName)	\
    ( (This)->lpVtbl -> LoadFromFile(This,lpszPathName) ) 

#define IImageOle_SetFaceId(This,face_id)	\
    ( (This)->lpVtbl -> SetFaceId(This,face_id) ) 

#define IImageOle_GetFaceId(This,face_id)	\
    ( (This)->lpVtbl -> GetFaceId(This,face_id) ) 

#define IImageOle_SetFaceIndex(This,face_index)	\
    ( (This)->lpVtbl -> SetFaceIndex(This,face_index) ) 

#define IImageOle_GetFaceIndex(This,face_index)	\
    ( (This)->lpVtbl -> GetFaceIndex(This,face_index) ) 

#define IImageOle_GetImageFile(This,file_name)	\
    ( (This)->lpVtbl -> GetImageFile(This,file_name) ) 

#define IImageOle_GetImageRect(This,rect)	\
    ( (This)->lpVtbl -> GetImageRect(This,rect) ) 

#define IImageOle_GetImageFomat(This,guid)	\
    ( (This)->lpVtbl -> GetImageFomat(This,guid) ) 

#define IImageOle_SaveAsFile(This,file_path)	\
    ( (This)->lpVtbl -> SaveAsFile(This,file_path) ) 

#define IImageOle_SetScaleSize(This,bScale,nWidth,nHeight)	\
    ( (This)->lpVtbl -> SetScaleSize(This,bScale,nWidth,nHeight) ) 

#define IImageOle_SetBgColor(This,clrBg)	\
    ( (This)->lpVtbl -> SetBgColor(This,clrBg) ) 

#define IImageOle_Draw(This)	\
    ( (This)->lpVtbl -> Draw(This) ) 

#define IImageOle_SetFaceTag(This,face_tag)	\
    ( (This)->lpVtbl -> SetFaceTag(This,face_tag) ) 

#define IImageOle_GetFaceTag(This,face_tag)	\
    ( (This)->lpVtbl -> GetFaceTag(This,face_tag) ) 

#define IImageOle_SetFont(This,font_name,font_size,clrText)	\
    ( (This)->lpVtbl -> SetFont(This,font_name,font_size,clrText) ) 

#define IImageOle_SetGUID(This,guid)	\
    ( (This)->lpVtbl -> SetGUID(This,guid) ) 

#define IImageOle_GetGUID(This,guid)	\
    ( (This)->lpVtbl -> GetGUID(This,guid) ) 

#define IImageOle_GetScale(This,scale)	\
    ( (This)->lpVtbl -> GetScale(This,scale) ) 

#define IImageOle_LoadFromDescription(This,des)	\
    ( (This)->lpVtbl -> LoadFromDescription(This,des) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IImageOle_INTERFACE_DEFINED__ */



#ifndef __image_oleLib_LIBRARY_DEFINED__
#define __image_oleLib_LIBRARY_DEFINED__

/* library image_oleLib */
/* [custom][version][uuid] */ 


EXTERN_C const IID LIBID_image_oleLib;

EXTERN_C const CLSID CLSID_CompReg;

#ifdef __cplusplus

class DECLSPEC_UUID("070D3F66-491B-4F0D-9ECC-5358BB7B0303")
CompReg;
#endif

#ifndef ___IImageOleEvents_DISPINTERFACE_DEFINED__
#define ___IImageOleEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IImageOleEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IImageOleEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("280EB118-CF3F-49C4-A6E2-DE4B008C5BBB")
    _IImageOleEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IImageOleEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IImageOleEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IImageOleEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IImageOleEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IImageOleEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IImageOleEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IImageOleEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IImageOleEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IImageOleEventsVtbl;

    interface _IImageOleEvents
    {
        CONST_VTBL struct _IImageOleEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IImageOleEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IImageOleEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IImageOleEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IImageOleEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IImageOleEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IImageOleEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IImageOleEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IImageOleEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ImageOle;

#ifdef __cplusplus

class DECLSPEC_UUID("B0C73E85-1599-4705-BD6B-DB1DF767FC6D")
ImageOle;
#endif
#endif /* __image_oleLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


