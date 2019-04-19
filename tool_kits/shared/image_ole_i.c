

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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

MIDL_DEFINE_GUID(IID, IID_IComponentRegistrar,0xa817e7a2,0x43fa,0x11d0,0x9e,0x44,0x00,0xaa,0x00,0xb6,0x77,0x0a);


MIDL_DEFINE_GUID(IID, IID_IImageOle,0xFDEEBA15,0x2214,0x4AAA,0x82,0x22,0x73,0x54,0x49,0x1B,0xE2,0x6C);


MIDL_DEFINE_GUID(IID, LIBID_image_oleLib,0x20667E6E,0x41AD,0x40D0,0xBF,0xE9,0x94,0x84,0x0D,0x87,0x2C,0xD4);


MIDL_DEFINE_GUID(CLSID, CLSID_CompReg,0x070D3F66,0x491B,0x4F0D,0x9E,0xCC,0x53,0x58,0xBB,0x7B,0x03,0x03);


MIDL_DEFINE_GUID(IID, DIID__IImageOleEvents,0x280EB118,0xCF3F,0x49C4,0xA6,0xE2,0xDE,0x4B,0x00,0x8C,0x5B,0xBB);


MIDL_DEFINE_GUID(CLSID, CLSID_ImageOle,0xB0C73E85,0x1599,0x4705,0xBD,0x6B,0xDB,0x1D,0xF7,0x67,0xFC,0x6D);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



