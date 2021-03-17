#include "drag_drop.h"
#include <shlobj.h>
#include <shlobj.h>
#include <shlwapi.h>

SdkDataObject::SdkDataObject(/*SdkDropSource *pDropSource*/)
{
	m_lRefCount = 1;
}

SdkDataObject::~SdkDataObject(void)
{
	m_lRefCount = 0;

	int nSize = (int)m_dataStorageCL.size();
	for (int i = 0; i < nSize; ++i)
	{
		DATASTORAGE_t dataEntry = m_dataStorageCL.at(i);
		ReleaseStgMedium(dataEntry.m_stgMedium);
		if (dataEntry.m_stgMedium)
		{
			delete dataEntry.m_stgMedium;
		}
		if (dataEntry.m_formatEtc)
		{
			delete dataEntry.m_formatEtc;
		}
	}
}

STDMETHODIMP SdkDataObject::QueryInterface(REFIID riid, void **ppv)
{
	static const QITAB qit[] =
	{
		QITABENT(SdkDataObject, IDataObject),
		{ 0 },
	};
	return QISearch(this, qit, riid, ppv);
	//return E_NOTIMPL;
}

STDMETHODIMP_(ULONG) SdkDataObject::AddRef()
{
	return InterlockedIncrement(&m_lRefCount);
}

STDMETHODIMP_(ULONG) SdkDataObject::Release()
{
	ULONG lRef = InterlockedDecrement(&m_lRefCount);
	if (0 == lRef)
	{
		delete this;
		return 0;
	}
	return m_lRefCount;
}

STDMETHODIMP SdkDataObject::GetData(FORMATETC *pformatetcIn,
	STGMEDIUM *pmedium)
{
	if ((NULL == pformatetcIn) || (NULL == pmedium))
	{
		return E_INVALIDARG;
	}

	pmedium->hGlobal = NULL;

	int nSize = (int)m_dataStorageCL.size();
	for (int i = 0; i < nSize; ++i)
	{
		DATASTORAGE_t dataEntry = m_dataStorageCL.at(i);
		if ((pformatetcIn->tymed & dataEntry.m_formatEtc->tymed) &&
			(pformatetcIn->dwAspect == dataEntry.m_formatEtc->dwAspect) &&
			(pformatetcIn->cfFormat == dataEntry.m_formatEtc->cfFormat))
		{
			return CopyMedium(pmedium,
				dataEntry.m_stgMedium, dataEntry.m_formatEtc);
		}
	}

	return DV_E_FORMATETC;
}

STDMETHODIMP SdkDataObject::SetData(FORMATETC *pformatetc,
	STGMEDIUM *pmedium, BOOL fRelease)
{
	if ((NULL == pformatetc) || (NULL == pmedium))
	{
		return E_INVALIDARG;
	}

	if (pformatetc->tymed != pmedium->tymed)
	{
		return E_FAIL;
	}

	FORMATETC* fetc = new FORMATETC;
	STGMEDIUM* pStgMed = new STGMEDIUM;
	ZeroMemory(fetc, sizeof(FORMATETC));
	ZeroMemory(pStgMed, sizeof(STGMEDIUM));

	*fetc = *pformatetc;

	if (TRUE == fRelease)
	{
		*pStgMed = *pmedium;
	}
	else
	{
		CopyMedium(pStgMed, pmedium, pformatetc);
	}

	DATASTORAGE_t dataEntry = { fetc, pStgMed };
	m_dataStorageCL.push_back(dataEntry);

	return S_OK;
}

STDMETHODIMP SdkDataObject::GetDataHere(
	FORMATETC *pformatetc, STGMEDIUM *pmedium)
{
	UNREFERENCED_PARAMETER(pformatetc);
	UNREFERENCED_PARAMETER(pmedium);
	return E_NOTIMPL;
}

STDMETHODIMP SdkDataObject::QueryGetData(FORMATETC *pformatetc)
{
	if (NULL == pformatetc)
	{
		return E_INVALIDARG;
	}
	if (!(DVASPECT_CONTENT & pformatetc->dwAspect))
	{
		return DV_E_DVASPECT;
	}
	HRESULT hr = DV_E_TYMED;
	int nSize = m_dataStorageCL.size();
	for (int i = 0; i < nSize; ++i)
	{
		DATASTORAGE_t dataEnrty = m_dataStorageCL.at(i);
		if (dataEnrty.m_formatEtc->tymed & pformatetc->tymed)
		{
			if (dataEnrty.m_formatEtc->cfFormat == pformatetc->cfFormat)
			{
				return S_OK;
			}
			else
			{
				hr = DV_E_CLIPFORMAT;
			}
		}
		else
		{
			hr = DV_E_TYMED;
		}
	}
	return hr;
}

STDMETHODIMP SdkDataObject::GetCanonicalFormatEtc(FORMATETC *pformatetcIn, FORMATETC *pformatetcOut)
{
	return E_NOTIMPL;
}

STDMETHODIMP SdkDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppenumFormatEtc)
{
	if (NULL == ppenumFormatEtc)
	{
		return E_INVALIDARG;
	}
	*ppenumFormatEtc = NULL;
	HRESULT hr = E_NOTIMPL;
	if (DATADIR_GET == dwDirection)
	{
		int nSize = m_dataStorageCL.size();
		FORMATETC* rgfmtetc = new FORMATETC[nSize];

		for (int i = 0; i < nSize; ++i)
		{
			DATASTORAGE_t dataEnrty = m_dataStorageCL.at(i);
			rgfmtetc[i] = *dataEnrty.m_formatEtc;
		}
		hr = SHCreateStdEnumFmtEtc(nSize, rgfmtetc, ppenumFormatEtc);
	}
	return hr;
}

STDMETHODIMP SdkDataObject::DAdvise(FORMATETC *pformatetc, DWORD advf, IAdviseSink *pAdvSnk, DWORD *pdwConnection)
{
	UNREFERENCED_PARAMETER(pformatetc);
	UNREFERENCED_PARAMETER(advf);
	UNREFERENCED_PARAMETER(pAdvSnk);
	UNREFERENCED_PARAMETER(pdwConnection);
	return E_NOTIMPL;
}

STDMETHODIMP SdkDataObject::DUnadvise(DWORD dwConnection)
{
	UNREFERENCED_PARAMETER(dwConnection);
	return E_NOTIMPL;
}

STDMETHODIMP SdkDataObject::EnumDAdvise(IEnumSTATDATA **ppenumAdvise)
{
	UNREFERENCED_PARAMETER(ppenumAdvise);
	return E_NOTIMPL;
}

HRESULT SdkDataObject::CopyMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc)
{
	if ((NULL == pMedDest) || (NULL == pMedSrc) || (NULL == pFmtSrc))
	{
		return E_INVALIDARG;
	}
	switch (pMedSrc->tymed)
	{
	case TYMED_HGLOBAL:
		pMedDest->hGlobal = (HGLOBAL)OleDuplicateData(pMedSrc->hGlobal, pFmtSrc->cfFormat, NULL);
		break;
	case TYMED_GDI:
		pMedDest->hBitmap = (HBITMAP)OleDuplicateData(pMedSrc->hBitmap, pFmtSrc->cfFormat, NULL);
		break;
	case TYMED_MFPICT:
		pMedDest->hMetaFilePict = (HMETAFILEPICT)OleDuplicateData(pMedSrc->hMetaFilePict, pFmtSrc->cfFormat, NULL);
		break;
	case TYMED_ENHMF:
		pMedDest->hEnhMetaFile = (HENHMETAFILE)OleDuplicateData(pMedSrc->hEnhMetaFile, pFmtSrc->cfFormat, NULL);
		break;
	case TYMED_FILE:
		pMedDest->lpszFileName = (LPOLESTR)OleDuplicateData(pMedSrc->lpszFileName, pFmtSrc->cfFormat, NULL);
		break;
	case TYMED_ISTREAM:
		pMedDest->pstm = pMedSrc->pstm;
		pMedSrc->pstm->AddRef();
		break;
	case TYMED_ISTORAGE:
		pMedDest->pstg = pMedSrc->pstg;
		pMedSrc->pstg->AddRef();
		break;
	case TYMED_NULL:
	default:
		break;
	}
	pMedDest->tymed = pMedSrc->tymed;
	pMedDest->pUnkForRelease = NULL;
	if (pMedSrc->pUnkForRelease != NULL)
	{
		pMedDest->pUnkForRelease = pMedSrc->pUnkForRelease;
		pMedSrc->pUnkForRelease->AddRef();
	}
	return S_OK;
}

HRESULT SdkDataObject::SetBlob(CLIPFORMAT cf, const void *pvBlob, UINT cbBlob)
{
	void *pv = GlobalAlloc(GPTR, cbBlob);
	HRESULT hr = pv ? S_OK : E_OUTOFMEMORY;
	if (SUCCEEDED(hr))
	{
		CopyMemory(pv, pvBlob, cbBlob);
		FORMATETC fmte = { cf, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
		// The STGMEDIUM structure is used to define how to handle a global memory transfer.
		// This structure includes a flag, tymed, which indicates the medium
		// to be used, and a union comprising pointers and a handle for getting whichever
		// medium is specified in tymed.
		STGMEDIUM medium = {};
		medium.tymed = TYMED_HGLOBAL;
		medium.hGlobal = pv;
		hr = this->SetData(&fmte, &medium, TRUE);
		if (FAILED(hr))
		{
			GlobalFree(pv);
		}
	}
	return hr;
}

HGLOBAL CreateGlobalHandle(IN void* ptr, int size)
{
	HGLOBAL hGlobal = NULL;
	hGlobal = GlobalAlloc(GMEM_FIXED, size);
	if (NULL != hGlobal)
	{
		LPVOID pdest = GlobalLock(hGlobal);
		if (NULL != pdest)
		{
			memcpy_s(pdest, size, ptr, size);
		}
		GlobalUnlock(hGlobal);
	}
	return hGlobal;
}

//////////////////////////////////////////////////////////////////////
// SdkDropSource Class
//////////////////////////////////////////////////////////////////////
SdkDropSource::SdkDropSource()
{
	m_cRefCount = 1;
	m_bDropped = false;
	m_feedbackCursor = NULL;
}

STDMETHODIMP SdkDropSource::QueryInterface(/* [in] */ REFIID riid,
										 /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
{
   *ppvObject = NULL;
   if (IID_IUnknown==riid || IID_IDropSource==riid)
       *ppvObject=this;

    if (*ppvObject != NULL)
    {
       ((LPUNKNOWN)*ppvObject)->AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) SdkDropSource::AddRef( void)
{
	return ++m_cRefCount;
}

STDMETHODIMP_(ULONG) SdkDropSource::Release( void)
{
  long nTemp;
   nTemp = --m_cRefCount;
   ASSERT(nTemp >= 0);
   if(nTemp==0)
      delete this;
   return nTemp;
}

STDMETHODIMP SdkDropSource::QueryContinueDrag(/* [in] */ BOOL fEscapePressed, /* [in] */ DWORD grfKeyState)
{
   if(fEscapePressed)
      return DRAGDROP_S_CANCEL;

   if(!(grfKeyState & (MK_LBUTTON|MK_RBUTTON)))
   {
	  m_bDropped = true;
      return DRAGDROP_S_DROP;
   }

   return S_OK;

}

STDMETHODIMP SdkDropSource::GiveFeedback( /* [in] */ DWORD dwEffect)
{
	if (NULL != m_feedbackCursor)
	{
		::SetCursor(m_feedbackCursor);
		return S_OK;
	}
	else
	{
		return DRAGDROP_S_USEDEFAULTCURSORS;
	}
}

void SdkDropSource::SetFeedbackCursor(HCURSOR cursor)
{
	ASSERT(NULL != cursor);
	m_feedbackCursor = cursor;
}

//////////////////////////////////////////////////////////////////////
// SdkDragSourceHelper Class
//////////////////////////////////////////////////////////////////////
SdkDragSourceHelper::SdkDragSourceHelper()
{
	if (FAILED(CoCreateInstance(CLSID_DragDropHelper, NULL, CLSCTX_INPROC_SERVER, IID_IDragSourceHelper, (void**)&m_pDragSourceHelper)))
		m_pDragSourceHelper = NULL;
}

SdkDragSourceHelper::~SdkDragSourceHelper()
{
	if (m_pDragSourceHelper != NULL)
	{
		m_pDragSourceHelper->Release();
		m_pDragSourceHelper = NULL;
	}
}

HRESULT SdkDragSourceHelper::InitializeFromBitmap(HBITMAP hBitmap, POINT& pt_offset, SIZE& bitmap_size, IDataObject* pDataObject, COLORREF crColorKey)
{
	if (m_pDragSourceHelper == NULL)
		return E_FAIL;

	SHDRAGIMAGE di;
	di.sizeDragImage = bitmap_size;
	di.hbmpDragImage = hBitmap;
	di.crColorKey = crColorKey;
	di.ptOffset = pt_offset;
	return m_pDragSourceHelper->InitializeFromBitmap(&di, pDataObject);
}

HRESULT SdkDragSourceHelper::InitializeFromWindow(HWND hwnd, POINT& pt, IDataObject* pDataObject)
{
	if (m_pDragSourceHelper == NULL)
		return E_FAIL;
	return m_pDragSourceHelper->InitializeFromWindow(hwnd, &pt, pDataObject);
}