// Copyright (c) 2015, NetEase Inc.
// All rights reserved.
//
// Author: Redrain
// Date: 2015/09/24
//
// brife: 实现控件的拖拽和窗体的拖放功能
#pragma once
#include "OleIdl.h"
#include "ShObjIdl.h"
#include <shlobj.h>

typedef struct _DATASTORAGE
{
	FORMATETC *m_formatEtc;
	STGMEDIUM *m_stgMedium;

} DATASTORAGE_t, *LPDATASTORAGE_t;

class SdkDataObject : public IDataObject
{
public:
	SdkDataObject(/*SdkDropSource *pDropSource = NULL*/);
	BOOL IsDataAvailable(CLIPFORMAT cfFormat);
	BOOL GetGlobalData(CLIPFORMAT cfFormat, void **ppData);
	BOOL GetGlobalDataArray(CLIPFORMAT cfFormat,
		HGLOBAL *pDataArray, DWORD dwCount);
	BOOL SetGlobalData(CLIPFORMAT cfFormat, void *pData, BOOL fRelease = TRUE);
	BOOL SetGlobalDataArray(CLIPFORMAT cfFormat,
		HGLOBAL *pDataArray, DWORD dwCount, BOOL fRelease = TRUE);
	//BOOL SetDropTip(DROPIMAGETYPE type, PCWSTR pszMsg, PCWSTR pszInsert);

	// The com interface.
	IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium);
	IFACEMETHODIMP SetData(FORMATETC *pformatetc,
		STGMEDIUM *pmedium, BOOL fRelease);
	IFACEMETHODIMP GetDataHere(FORMATETC *pformatetc, STGMEDIUM *pmedium);
	IFACEMETHODIMP QueryGetData(FORMATETC *pformatetc);
	IFACEMETHODIMP GetCanonicalFormatEtc(FORMATETC *pformatetcIn,
		FORMATETC *pformatetcOut);
	IFACEMETHODIMP EnumFormatEtc(DWORD dwDirection,
		IEnumFORMATETC **ppenumFormatEtc);
	IFACEMETHODIMP DAdvise(FORMATETC *pformatetc, DWORD advf,
		IAdviseSink *pAdvSnk, DWORD *pdwConnection);
	IFACEMETHODIMP DUnadvise(DWORD dwConnection);
	IFACEMETHODIMP EnumDAdvise(IEnumSTATDATA **ppenumAdvise);

private:
	~SdkDataObject(void);
	SdkDataObject(const SdkDataObject&);
	SdkDataObject& operator = (const SdkDataObject&);
	HRESULT CopyMedium(STGMEDIUM* pMedDest,
		STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc);
	HRESULT SetBlob(CLIPFORMAT cf, const void *pvBlob, UINT cbBlob);

private:
	volatile LONG           m_lRefCount;
	std::vector<DATASTORAGE_t>   m_dataStorageCL;
};

class SdkDropSource : public IDropSource
{
public:
	SdkDropSource();
	//IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);        
    virtual ULONG STDMETHODCALLTYPE AddRef( void);
    virtual ULONG STDMETHODCALLTYPE Release( void);
	//IDropSource
    virtual HRESULT STDMETHODCALLTYPE QueryContinueDrag( 
        /* [in] */ BOOL fEscapePressed,
        /* [in] */ DWORD grfKeyState);
    
    virtual HRESULT STDMETHODCALLTYPE GiveFeedback( 
        /* [in] */ DWORD dwEffect);

	// 设置拖拽时鼠标光标的样式，如果不设置则使用系统默认光标
	void SetFeedbackCursor(HCURSOR cursor);
private:
	long	m_cRefCount;
	bool	m_bDropped;
	HCURSOR	m_feedbackCursor;
};

class SdkDragSourceHelper
{
public:
	SdkDragSourceHelper();
	virtual ~SdkDragSourceHelper();
    
	// IDragSourceHelper
	HRESULT InitializeFromBitmap(HBITMAP hBitmap,
		POINT& pt_offset,	// cursor position offset of the bitmap
		SIZE& bitmap_size,	// hBitmap's size
		IDataObject* pDataObject,
		COLORREF crColorKey = GetSysColor(COLOR_WINDOW) // color of the window used for transparent effect.
		);

	HRESULT InitializeFromWindow(HWND hwnd, POINT& pt, IDataObject* pDataObject);

private:
	IDragSourceHelper* m_pDragSourceHelper;
};