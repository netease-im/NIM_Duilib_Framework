#include "stdafx.h"
#include "base/win32/path_util.h"
#include "base/thread/thread_manager.h"

// These constants are for backward compatibility. They are the 
// sizes used for initialization and reset in RichEdit 1.0

namespace ui {

const LONG cInitTextMax = (32 * 1024) - 1;

EXTERN_C const IID IID_ITextServices = { // 8d33f740-cf58-11ce-a89d-00aa006cadc5
    0x8d33f740,
    0xcf58,
    0x11ce,
    {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

EXTERN_C const IID IID_ITextHost = { /* c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 */
    0xc5bdd8d0,
    0xd26e,
    0x11ce,
    {0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

#ifndef LY_PER_INCH
#define LY_PER_INCH 1440
#endif

#ifndef HIMETRIC_PER_INCH
#define HIMETRIC_PER_INCH 2540
#endif

#include <textserv.h>

class CTxtWinHost : public ITextHost
{
public:
    CTxtWinHost();
    BOOL Init(RichEdit *re , const CREATESTRUCT *pcs);
    virtual ~CTxtWinHost();

    ITextServices* GetTextServices(void) { return pserv; }
    void SetClientRect(UiRect *prc);
    UiRect* GetClientRect() { return &rcClient; }
    BOOL GetWordWrap(void) { return fWordWrap; }
    void SetWordWrap(BOOL fWordWrap);
    BOOL GetReadOnly();
    void SetReadOnly(BOOL fReadOnly);
	BOOL IsPassword();
	void SetPassword(BOOL bPassword);
    void SetFont(HFONT hFont);
    void SetColor(DWORD dwColor);
    SIZEL* GetExtent();
    void SetExtent(SIZEL *psizelExtent);
    void LimitText(LONG nChars);
    BOOL IsCaptured();

    BOOL GetAllowBeep();
    void SetAllowBeep(BOOL fAllowBeep);
    WORD GetDefaultAlign();
    void SetDefaultAlign(WORD wNewAlign);
    BOOL GetRichTextFlag();
    void SetRichTextFlag(BOOL fNew);
    LONG GetDefaultLeftIndent();
    void SetDefaultLeftIndent(LONG lNewIndent);
    BOOL SetSaveSelection(BOOL fSaveSelection);
    HRESULT OnTxInPlaceDeactivate();
    HRESULT OnTxInPlaceActivate(LPCRECT prcClient);
    BOOL GetActiveState(void) { return fInplaceActive; }
    BOOL DoSetCursor(UiRect *prc, POINT *pt);
    void SetTransparent(BOOL fTransparent);
    void GetControlRect(LPRECT prc);
    LONG SetAccelPos(LONG laccelpos);
    WCHAR SetPasswordChar(WCHAR chPasswordChar);
    void SetDisabled(BOOL fOn);
    LONG SetSelBarWidth(LONG lSelBarWidth);
    BOOL GetTimerState();

    void SetCharFormat(CHARFORMAT2W &c);
    void SetParaFormat(PARAFORMAT2 &p);

	ITextHost * GetTextHost()
	{
		AddRef();
		return this;
	}

	ITextServices * GetTextServices2()
	{
		if (NULL == pserv)
			return NULL;
		pserv->AddRef();
		return pserv;
	}

	BOOL SetOleCallback(IRichEditOleCallback* pCallback)
	{
		if (NULL == pserv)
			return FALSE;
		LRESULT lRes = 0;
		pserv->TxSendMessage(EM_SETOLECALLBACK, 0, (LPARAM)pCallback, &lRes);
		return (BOOL)lRes;
	}

	BOOL CanPaste(UINT nFormat = 0)
	{
		if (NULL == pserv)
			return FALSE;
		LRESULT lRes = 0;
		pserv->TxSendMessage(EM_CANPASTE, nFormat, 0L, &lRes);
		return (BOOL)lRes;
	}

	void PasteSpecial(UINT uClipFormat, DWORD dwAspect = 0, HMETAFILE hMF = 0)
	{
		if (NULL == pserv)
			return;
		REPASTESPECIAL reps = { dwAspect, (DWORD_PTR)hMF };
		pserv->TxSendMessage(EM_PASTESPECIAL, uClipFormat, (LPARAM)&reps, NULL);
	}

    // -----------------------------
    //	IUnknown interface
    // -----------------------------
    virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject);
    virtual ULONG _stdcall AddRef(void);
    virtual ULONG _stdcall Release(void);

    // -----------------------------
    //	ITextHost interface
    // -----------------------------
    virtual HDC TxGetDC();
    virtual INT TxReleaseDC(HDC hdc);
    virtual BOOL TxShowScrollBar(INT fnBar, BOOL fShow);
    virtual BOOL TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags);
    virtual BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw);
    virtual BOOL TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw);
    virtual void TxInvalidateRect(LPCRECT prc, BOOL fMode);
    virtual void TxViewChange(BOOL fUpdate);
    virtual BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight);
    virtual BOOL TxShowCaret(BOOL fShow);
    virtual BOOL TxSetCaretPos(INT x, INT y);
    virtual BOOL TxSetTimer(UINT idTimer, UINT uTimeout);
    virtual void TxKillTimer(UINT idTimer);
    virtual void TxScrollWindowEx (INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll);
    virtual void TxSetCapture(BOOL fCapture);
    virtual void TxSetFocus();
    virtual void TxSetCursor(HCURSOR hcur, BOOL fText);
    virtual BOOL TxScreenToClient (LPPOINT lppt);
    virtual BOOL TxClientToScreen (LPPOINT lppt);
    virtual HRESULT TxActivate( LONG * plOldState );
    virtual HRESULT TxDeactivate( LONG lNewState );
    virtual HRESULT TxGetClientRect(LPRECT prc);
    virtual HRESULT TxGetViewInset(LPRECT prc);
    virtual HRESULT TxGetCharFormat(const CHARFORMATW **ppCF );
    virtual HRESULT TxGetParaFormat(const PARAFORMAT **ppPF);
    virtual COLORREF TxGetSysColor(int nIndex);
    virtual HRESULT TxGetBackStyle(TXTBACKSTYLE *pstyle);
    virtual HRESULT TxGetMaxLength(DWORD *plength);
    virtual HRESULT TxGetScrollBars(DWORD *pdwScrollBar);
    virtual HRESULT TxGetPasswordChar(TCHAR *pch);
    virtual HRESULT TxGetAcceleratorPos(LONG *pcp);
    virtual HRESULT TxGetExtent(LPSIZEL lpExtent);
    virtual HRESULT OnTxCharFormatChange (const CHARFORMATW * pcf);
    virtual HRESULT OnTxParaFormatChange (const PARAFORMAT * ppf);
    virtual HRESULT TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits);
    virtual HRESULT TxNotify(DWORD iNotify, void *pv);
    virtual HIMC TxImmGetContext(void);
    virtual void TxImmReleaseContext(HIMC himc);
    virtual HRESULT TxGetSelectionBarWidth (LONG *lSelBarWidth);

private:
    RichEdit *m_re;
    ULONG	cRefs;					// Reference Count
    ITextServices	*pserv;		    // pointer to Text Services object
    // Properties

    DWORD		dwStyle;				// style bits

    unsigned	fEnableAutoWordSel	:1;	// enable Word style auto word selection?
    unsigned	fWordWrap			:1;	// Whether control should word wrap
    unsigned	fAllowBeep			:1;	// Whether beep is allowed
    unsigned	fRich				:1;	// Whether control is rich text
    unsigned	fSaveSelection		:1;	// Whether to save the selection when inactive
    unsigned	fInplaceActive		:1; // Whether control is inplace active
    unsigned	fTransparent		:1; // Whether control is transparent
    unsigned	fTimer				:1;	// A timer is set
    unsigned    fCaptured           :1;

    LONG		lSelBarWidth;			// Width of the selection bar
    LONG  		cchTextMost;			// maximum text size
    DWORD		dwEventMask;			// HandleMessage mask to pass on to parent window
    LONG		icf;
    LONG		ipf;
    UiRect		rcClient;				// Client Rect for this control
    SIZEL		sizelExtent;			// Extent array
    CHARFORMAT2W cf;					// Default character format
    PARAFORMAT2	pf;					    // Default paragraph format
    LONG		laccelpos;				// Accelerator position
    WCHAR		chPasswordChar;		    // Password character
};

// Convert Pixels on the X axis to Himetric
LONG DXtoHimetricX(LONG dx, LONG xPerInch)
{
    return (LONG) MulDiv(dx, HIMETRIC_PER_INCH, xPerInch);
}

// Convert Pixels on the Y axis to Himetric
LONG DYtoHimetricY(LONG dy, LONG yPerInch)
{
    return (LONG) MulDiv(dy, HIMETRIC_PER_INCH, yPerInch);
}

HRESULT InitDefaultCharFormat(RichEdit* re, CHARFORMAT2W* pcf, HFONT hfont) 
{
    memset(pcf, 0, sizeof(CHARFORMAT2W));
    LOGFONT lf;
    if( !hfont )
        hfont = GlobalManager::GetFont(re->GetFont());
    ::GetObject(hfont, sizeof(LOGFONT), &lf);

	DWORD dwColor = re->GetTextColorValue();
    pcf->cbSize = sizeof(CHARFORMAT2W);
    pcf->crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
    LONG yPixPerInch = GetDeviceCaps(re->GetWindowDC(), LOGPIXELSY);
	if (yPixPerInch == 0)
		yPixPerInch = 96;
    pcf->yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
    pcf->yOffset = 0;
    pcf->dwEffects = 0;
	pcf->dwMask = CFM_SIZE | CFM_OFFSET | CFM_FACE | CFM_CHARSET | CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT;
    if(lf.lfWeight >= FW_BOLD)
        pcf->dwEffects |= CFE_BOLD;
    if(lf.lfItalic)
        pcf->dwEffects |= CFE_ITALIC;
    if(lf.lfUnderline)
        pcf->dwEffects |= CFE_UNDERLINE;
	if (lf.lfStrikeOut)
		pcf->dwEffects |= CFE_STRIKEOUT;
    pcf->bCharSet = lf.lfCharSet;
    pcf->bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
    _tcscpy(pcf->szFaceName, lf.lfFaceName);
#else
    //need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
    MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, pcf->szFaceName, LF_FACESIZE) ;
#endif

    return S_OK;
}

HRESULT InitDefaultParaFormat(RichEdit* re, PARAFORMAT2* ppf) 
{	
    memset(ppf, 0, sizeof(PARAFORMAT2));
    ppf->cbSize = sizeof(PARAFORMAT2);
    ppf->dwMask = PFM_ALL;
    ppf->wAlignment = PFA_LEFT;
    ppf->cTabCount = 1;
    ppf->rgxTabs[0] = lDefaultTab;

    return S_OK;
}

HRESULT CreateHost(RichEdit *re, const CREATESTRUCT *pcs, CTxtWinHost **pptec)
{
    HRESULT hr = E_FAIL;
    //GdiSetBatchLimit(1);

    CTxtWinHost *phost = new CTxtWinHost();
    if(phost)
    {
        if (phost->Init(re, pcs))
        {
            *pptec = phost;
            hr = S_OK;
        }
    }

    if (FAILED(hr))
    {
        delete phost;
    }

    return TRUE;
}

CTxtWinHost::CTxtWinHost() : m_re(NULL)
{
    ::ZeroMemory(&cRefs, sizeof(CTxtWinHost) - offsetof(CTxtWinHost, cRefs));
    cchTextMost = cInitTextMax;
    laccelpos = -1;
}

CTxtWinHost::~CTxtWinHost()
{
    pserv->OnTxInPlaceDeactivate();
    pserv->Release();
}

////////////////////// Create/Init/Destruct Commands ///////////////////////

BOOL CTxtWinHost::Init(RichEdit *re, const CREATESTRUCT *pcs)
{
    IUnknown *pUnk = nullptr;
    HRESULT hr;
	std::wstring edit_dll(L"msftedit.dll");
    m_re = re;
    // Initialize Reference count
    cRefs = 1;

    // Create and cache CHARFORMAT for this control
    if(FAILED(InitDefaultCharFormat(re, &cf, NULL)))
        goto err;

    // Create and cache PARAFORMAT for this control
    if(FAILED(InitDefaultParaFormat(re, &pf)))
        goto err;

    // edit controls created without a window are multiline by default
    // so that paragraph formats can be
    dwStyle = ES_MULTILINE;

    // edit controls are rich by default
    fRich = re->IsRich();

    cchTextMost = re->GetLimitText();

    if (pcs )
    {
        dwStyle = pcs->style;

        if ( !(dwStyle & (ES_AUTOHSCROLL | WS_HSCROLL)) )
        {
            fWordWrap = TRUE;
        }
    }

    if( !(dwStyle & ES_LEFT) )
    {
        if(dwStyle & ES_CENTER)
            pf.wAlignment = PFA_CENTER;
        else if(dwStyle & ES_RIGHT)
            pf.wAlignment = PFA_RIGHT;
    }

    fInplaceActive = TRUE;

    // Create Text Services component
    //if(FAILED(CreateTextServices(NULL, this, &pUnk)))
    //    goto err;

	PCreateTextServices TextServicesProc = nullptr;
	//解决32位系统下跨窗口间拖拽文字在win7及win7以下系统上会出现重复的问题（64位暂不修复） lty 20170714
#if defined(_M_X64) || defined(__x86_64__)
	edit_dll = L"msftedit.dll";
#else
	edit_dll = nbase::win32::GetCurrentModuleDirectory() + L"msftedit50.dll";
	if (!(::GetFileAttributesW(edit_dll.c_str()) != INVALID_FILE_ATTRIBUTES))
		edit_dll = L"msftedit.dll";
#endif
	HMODULE hmod = LoadLibraryW(edit_dll.c_str()); //msftedit.dll
	if (hmod)
	{
		TextServicesProc = (PCreateTextServices)GetProcAddress(hmod,"CreateTextServices");
	}

	if (TextServicesProc)
	{
		TextServicesProc(NULL, this, &pUnk);
	}

    hr = pUnk->QueryInterface(IID_ITextServices,(void **)&pserv);

    // Whether the previous call succeeded or failed we are done
    // with the private interface.
    pUnk->Release();

    if(FAILED(hr))
    {
        goto err;
    }

    // Set window text
    if(pcs && pcs->lpszName)
    {
#ifdef _UNICODE		
        if(FAILED(pserv->TxSetText((TCHAR *)pcs->lpszName)))
            goto err;
#else
        std::size_t iLen = _tcslen(pcs->lpszName);
        LPWSTR lpText = new WCHAR[iLen + 1];
        ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
        ::MultiByteToWideChar(CP_ACP, 0, pcs->lpszName, -1, (LPWSTR)lpText, iLen) ;
        if(FAILED(pserv->TxSetText((LPWSTR)lpText))) {
            delete[] lpText;
            goto err;
        }
        delete[] lpText;
#endif
    }

    return TRUE;

err:
    return FALSE;
}

/////////////////////////////////  IUnknown ////////////////////////////////


HRESULT CTxtWinHost::QueryInterface(REFIID riid, void **ppvObject)
{
    HRESULT hr = E_NOINTERFACE;
    *ppvObject = NULL;

    if (IsEqualIID(riid, IID_IUnknown) 
        || IsEqualIID(riid, IID_ITextHost)) 
    {
        AddRef();
        *ppvObject = (ITextHost *) this;
        hr = S_OK;
    }

    return hr;
}

ULONG CTxtWinHost::AddRef(void)
{
    return ++cRefs;
}

ULONG CTxtWinHost::Release(void)
{
    ULONG c_Refs = --cRefs;

    if (c_Refs == 0)
    {
        delete this;
    }

    return c_Refs;
}

/////////////////////////////////  Far East Support  //////////////////////////////////////

HIMC CTxtWinHost::TxImmGetContext(void)
{
    return NULL;
}

void CTxtWinHost::TxImmReleaseContext(HIMC himc)
{
    //::ImmReleaseContext( hwnd, himc );
}

//////////////////////////// ITextHost Interface  ////////////////////////////

HDC CTxtWinHost::TxGetDC()
{
    return m_re->GetWindowDC();
}

int CTxtWinHost::TxReleaseDC(HDC hdc)
{
    return 1;
}

BOOL CTxtWinHost::TxShowScrollBar(INT fnBar, BOOL fShow)
{
	ASSERT(FALSE); //暂时注释掉，不知道这代码有啥用   by panqinke 2014.5.6
    //ScrollBar* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    //ScrollBar* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
    //if( fnBar == SB_VERT && pVerticalScrollBar ) {
    //    pVerticalScrollBar->SetVisible(fShow == TRUE);
    //}
    //else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
    //    pHorizontalScrollBar->SetVisible(fShow == TRUE);
    //}
    //else if( fnBar == SB_BOTH ) {
    //    if( pVerticalScrollBar ) pVerticalScrollBar->SetVisible(fShow == TRUE);
    //    if( pHorizontalScrollBar ) pHorizontalScrollBar->SetVisible(fShow == TRUE);
    //}
    return TRUE;
}

BOOL CTxtWinHost::TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags)
{
	if( fuSBFlags == SB_VERT ) {
		if (fuArrowflags == ESB_DISABLE_BOTH) {
			m_re->GetVerticalScrollBar()->SetScrollRange(0);
		}		
	}
	else if( fuSBFlags == SB_HORZ ) {
		if (fuArrowflags == ESB_DISABLE_BOTH) {
			m_re->GetHorizontalScrollBar()->SetScrollRange(0);
		}
	}
	else if( fuSBFlags == SB_BOTH ) {
		if (fuArrowflags == ESB_DISABLE_BOTH) {
			m_re->GetVerticalScrollBar()->SetScrollRange(0);
		}
		if (fuArrowflags == ESB_DISABLE_BOTH) {
			m_re->GetHorizontalScrollBar()->SetScrollRange(0);
		}
	}
	
	m_re->SetPos(m_re->GetPos());

    return TRUE;
}

BOOL CTxtWinHost::TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw)
{
    ScrollBar* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    ScrollBar* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
	bool bArrange = false;
    if( fnBar == SB_VERT && pVerticalScrollBar ) {
        if( nMaxPos - nMinPos - rcClient.bottom + rcClient.top <= 0 ) {
			pVerticalScrollBar->SetScrollRange(0);
        }
        else {
			if (!pVerticalScrollBar->IsValid()) {
				bArrange = true;
			}
            pVerticalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.bottom + rcClient.top);
        }
    }
    else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
        if( nMaxPos - nMinPos - rcClient.right + rcClient.left <= 0 ) {
			pHorizontalScrollBar->SetScrollRange(0);
        }
        else {
			if (!pHorizontalScrollBar->IsValid()) {
				bArrange = true;
			}
            pHorizontalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.right + rcClient.left);
        }   
    }

	if (bArrange) {
		m_re->SetPos(m_re->GetPos());
	}

    return TRUE;
}

BOOL CTxtWinHost::TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw)
{
    ScrollBar* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    ScrollBar* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
    if( fnBar == SB_VERT && pVerticalScrollBar ) {
        pVerticalScrollBar->SetScrollPos(nPos);
    }
    else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
        pHorizontalScrollBar->SetScrollPos(nPos);
    }
    return TRUE;
}

void CTxtWinHost::TxInvalidateRect(LPCRECT prc, BOOL fMode)
{
	if (!m_re->GetWindow())
		return;

	CPoint scrollOffset = m_re->GetScrollOffset();
    if( prc == NULL ) {
		UiRect newRcClient = rcClient;
		newRcClient.Offset(-scrollOffset.x, -scrollOffset.y);
        m_re->GetWindow()->Invalidate(newRcClient);
        return;
    }
    UiRect rc = *prc;
	rc.Offset(-scrollOffset.x, -scrollOffset.y);
    m_re->GetWindow()->Invalidate(rc);
}

void CTxtWinHost::TxViewChange(BOOL fUpdate) 
{

}

BOOL CTxtWinHost::TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight)
{
	return m_re->CreateCaret(xWidth, yHeight);
}

BOOL CTxtWinHost::TxShowCaret(BOOL fShow)
{
	return true; // m_re->ShowCaret(fShow);
}

BOOL CTxtWinHost::TxSetCaretPos(INT x, INT y)
{
	return m_re->SetCaretPos(x, y);
}

BOOL CTxtWinHost::TxSetTimer(UINT idTimer, UINT uTimeout)
{
    fTimer = TRUE;
	m_re->SetTimer(idTimer, uTimeout);
    return TRUE;
}

void CTxtWinHost::TxKillTimer(UINT idTimer)
{
	m_re->KillTimer(idTimer);
    fTimer = FALSE;
}

void CTxtWinHost::TxScrollWindowEx (INT dx, INT dy, LPCRECT lprcScroll,	LPCRECT lprcClip,	HRGN hrgnUpdate, LPRECT lprcUpdate,	UINT fuScroll)	
{
    return;
}

void CTxtWinHost::TxSetCapture(BOOL fCapture)
{
	if (!m_re->GetWindow())
		return;
	
    if (fCapture) m_re->GetWindow()->SetCapture();
    else m_re->GetWindow()->ReleaseCapture();
    fCaptured = fCapture;
}

void CTxtWinHost::TxSetFocus()
{
    m_re->SetFocus();
}

void CTxtWinHost::TxSetCursor(HCURSOR hcur,	BOOL fText)
{
    ::SetCursor(hcur);
}

BOOL CTxtWinHost::TxScreenToClient(LPPOINT lppt)
{
    return ::ScreenToClient(m_re->GetWindowHandle(), lppt);	
}

BOOL CTxtWinHost::TxClientToScreen(LPPOINT lppt)
{
    return ::ClientToScreen(m_re->GetWindowHandle(), lppt);
}

HRESULT CTxtWinHost::TxActivate(LONG *plOldState)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxDeactivate(LONG lNewState)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxGetClientRect(LPRECT prc)
{
    *prc = rcClient;
    GetControlRect(prc);
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetViewInset(LPRECT prc) 
{
    prc->left = prc->right = prc->top = prc->bottom = 0;
    return NOERROR;	
}

HRESULT CTxtWinHost::TxGetCharFormat(const CHARFORMATW **ppCF)
{
    *ppCF = &cf;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetParaFormat(const PARAFORMAT **ppPF)
{
    *ppPF = &pf;
    return NOERROR;
}

COLORREF CTxtWinHost::TxGetSysColor(int nIndex) 
{
    return ::GetSysColor(nIndex);
}

HRESULT CTxtWinHost::TxGetBackStyle(TXTBACKSTYLE *pstyle)
{
    *pstyle = !fTransparent ? TXTBACK_OPAQUE : TXTBACK_TRANSPARENT;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetMaxLength(DWORD *pLength)
{
    *pLength = cchTextMost;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetScrollBars(DWORD *pdwScrollBar)
{
    *pdwScrollBar =  dwStyle & (WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | 
        ES_AUTOHSCROLL | ES_DISABLENOSCROLL);

    return NOERROR;
}

HRESULT CTxtWinHost::TxGetPasswordChar(TCHAR *pch)
{
#ifdef _UNICODE
    *pch = chPasswordChar;
#else
    ::WideCharToMultiByte(CP_ACP, 0, &chPasswordChar, 1, pch, 1, NULL, NULL) ;
#endif
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetAcceleratorPos(LONG *pcp)
{
    *pcp = laccelpos;
    return S_OK;
} 										   

HRESULT CTxtWinHost::OnTxCharFormatChange(const CHARFORMATW *pcf)
{
    return S_OK;
}

HRESULT CTxtWinHost::OnTxParaFormatChange(const PARAFORMAT *ppf)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits) 
{
    DWORD dwProperties = 0;

    if (fRich)
    {
        dwProperties = TXTBIT_RICHTEXT;
    }

    if (dwStyle & ES_MULTILINE)
    {
        dwProperties |= TXTBIT_MULTILINE;
    }

    if (dwStyle & ES_READONLY)
    {
        dwProperties |= TXTBIT_READONLY;
    }

    if (dwStyle & ES_PASSWORD)
    {
        dwProperties |= TXTBIT_USEPASSWORD;
    }

    if (!(dwStyle & ES_NOHIDESEL))
    {
        dwProperties |= TXTBIT_HIDESELECTION;
    }

    if (fEnableAutoWordSel)
    {
        dwProperties |= TXTBIT_AUTOWORDSEL;
    }

    if (fWordWrap)
    {
        dwProperties |= TXTBIT_WORDWRAP;
    }

    if (fAllowBeep)
    {
        dwProperties |= TXTBIT_ALLOWBEEP;
    }

    if (fSaveSelection)
    {
        dwProperties |= TXTBIT_SAVESELECTION;
    }

    *pdwBits = dwProperties & dwMask; 
    return NOERROR;
}


HRESULT CTxtWinHost::TxNotify(DWORD iNotify, void *pv)
{
    if( iNotify == EN_REQUESTRESIZE ) {
        UiRect rc;
        REQRESIZE *preqsz = (REQRESIZE *)pv;
        GetControlRect(&rc);
        rc.bottom = rc.top + preqsz->rc.bottom;
        rc.right  = rc.left + preqsz->rc.right;
        SetClientRect(&rc);
    }
    m_re->OnTxNotify(iNotify, pv);
    return S_OK;
}

HRESULT CTxtWinHost::TxGetExtent(LPSIZEL lpExtent)
{
    *lpExtent = sizelExtent;
    return S_OK;
}

HRESULT	CTxtWinHost::TxGetSelectionBarWidth (LONG *plSelBarWidth)
{
    *plSelBarWidth = lSelBarWidth;
    return S_OK;
}

void CTxtWinHost::SetWordWrap(BOOL fWordWrap)
{
    fWordWrap = fWordWrap;
    pserv->OnTxPropertyBitsChange(TXTBIT_WORDWRAP, fWordWrap ? TXTBIT_WORDWRAP : 0);
}

BOOL CTxtWinHost::GetReadOnly()
{
    return (dwStyle & ES_READONLY) != 0;
}

void CTxtWinHost::SetReadOnly(BOOL fReadOnly)
{
    if (fReadOnly)
    {
        dwStyle |= ES_READONLY;
    }
    else
    {
        dwStyle &= ~ES_READONLY;
    }

    pserv->OnTxPropertyBitsChange(TXTBIT_READONLY, 
        fReadOnly ? TXTBIT_READONLY : 0);
}

BOOL CTxtWinHost::IsPassword()
{
	return (dwStyle & ES_PASSWORD) != 0;
}

void CTxtWinHost::SetPassword(BOOL bPassword)
{
	if (bPassword)
	{
		dwStyle |= ES_PASSWORD;
	}
	else
	{
		dwStyle &= ~ES_PASSWORD;
	}

	pserv->OnTxPropertyBitsChange(TXTBIT_USEPASSWORD, 
		bPassword ? TXTBIT_USEPASSWORD : 0);
}

void CTxtWinHost::SetFont(HFONT hFont) 
{
    if( hFont == NULL ) return;
    LOGFONT lf;
    ::GetObject(hFont, sizeof(LOGFONT), &lf);
    LONG yPixPerInch = ::GetDeviceCaps(m_re->GetWindowDC(), LOGPIXELSY);
	if (yPixPerInch == 0)
		yPixPerInch = 96;
    cf.yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
    if(lf.lfWeight >= FW_BOLD)
        cf.dwEffects |= CFE_BOLD;
    if(lf.lfItalic)
        cf.dwEffects |= CFE_ITALIC;
    if(lf.lfUnderline)
        cf.dwEffects |= CFE_UNDERLINE;
	if (lf.lfStrikeOut)
		cf.dwEffects |= CFE_STRIKEOUT;
    cf.bCharSet = lf.lfCharSet;
    cf.bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
    _tcscpy(cf.szFaceName, lf.lfFaceName);
#else
    //need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
    MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, cf.szFaceName, LF_FACESIZE) ;
#endif

    pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
        TXTBIT_CHARFORMATCHANGE);
}

void CTxtWinHost::SetColor(DWORD dwColor)
{
    cf.crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
    pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
        TXTBIT_CHARFORMATCHANGE);
}

SIZEL* CTxtWinHost::GetExtent() 
{
    return &sizelExtent;
}

void CTxtWinHost::SetExtent(SIZEL *psizelExtent) 
{ 
    sizelExtent = *psizelExtent; 
    pserv->OnTxPropertyBitsChange(TXTBIT_EXTENTCHANGE, TXTBIT_EXTENTCHANGE);
}

void CTxtWinHost::LimitText(LONG nChars)
{
    cchTextMost = nChars;
    if( cchTextMost <= 0 ) cchTextMost = cInitTextMax;
    pserv->OnTxPropertyBitsChange(TXTBIT_MAXLENGTHCHANGE, TXTBIT_MAXLENGTHCHANGE);
}

BOOL CTxtWinHost::IsCaptured()
{
    return fCaptured;
}

BOOL CTxtWinHost::GetAllowBeep()
{
    return fAllowBeep;
}

void CTxtWinHost::SetAllowBeep(BOOL fAllowBeep)
{
    fAllowBeep = fAllowBeep;

    pserv->OnTxPropertyBitsChange(TXTBIT_ALLOWBEEP, 
        fAllowBeep ? TXTBIT_ALLOWBEEP : 0);
}

WORD CTxtWinHost::GetDefaultAlign()
{
    return pf.wAlignment;
}

void CTxtWinHost::SetDefaultAlign(WORD wNewAlign)
{
    pf.wAlignment = wNewAlign;

    // Notify control of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

BOOL CTxtWinHost::GetRichTextFlag()
{
    return fRich;
}

void CTxtWinHost::SetRichTextFlag(BOOL fNew)
{
    fRich = fNew;

    pserv->OnTxPropertyBitsChange(TXTBIT_RICHTEXT, 
        fNew ? TXTBIT_RICHTEXT : 0);
}

LONG CTxtWinHost::GetDefaultLeftIndent()
{
    return pf.dxOffset;
}

void CTxtWinHost::SetDefaultLeftIndent(LONG lNewIndent)
{
    pf.dxOffset = lNewIndent;

    pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

void CTxtWinHost::SetClientRect(UiRect *prc) 
{
    rcClient = *prc;

    LONG xPerInch = ::GetDeviceCaps(m_re->GetWindowDC(), LOGPIXELSX); 
    LONG yPerInch =	::GetDeviceCaps(m_re->GetWindowDC(), LOGPIXELSY); 
	if (xPerInch == 0)
		xPerInch = 96;
	if (yPerInch == 0)
		yPerInch = 96;
    sizelExtent.cx = DXtoHimetricX(rcClient.right - rcClient.left, xPerInch);
    sizelExtent.cy = DYtoHimetricY(rcClient.bottom - rcClient.top, yPerInch);

    pserv->OnTxPropertyBitsChange(TXTBIT_VIEWINSETCHANGE, TXTBIT_VIEWINSETCHANGE);
}

BOOL CTxtWinHost::SetSaveSelection(BOOL f_SaveSelection)
{
    BOOL fResult = f_SaveSelection;

    fSaveSelection = f_SaveSelection;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_SAVESELECTION, 
        fSaveSelection ? TXTBIT_SAVESELECTION : 0);

    return fResult;		
}

HRESULT	CTxtWinHost::OnTxInPlaceDeactivate()
{
    HRESULT hr = pserv->OnTxInPlaceDeactivate();

    if (SUCCEEDED(hr))
    {
        fInplaceActive = FALSE;
    }

    return hr;
}

HRESULT	CTxtWinHost::OnTxInPlaceActivate(LPCRECT prcClient)
{
    fInplaceActive = TRUE;

    HRESULT hr = pserv->OnTxInPlaceActivate(prcClient);

    if (FAILED(hr))
    {
        fInplaceActive = FALSE;
    }

    return hr;
}

BOOL CTxtWinHost::DoSetCursor(UiRect *prc, POINT *pt)
{
    UiRect rc = (prc != NULL) ? *prc : rcClient;

    // Is this in our rectangle?
	CPoint newPt = *pt;
	newPt.Offset(m_re->GetScrollOffset());
    if (PtInRect(&rc, newPt))
    {
        UiRect *prcClient = (!fInplaceActive || prc) ? &rc : NULL;
        pserv->OnTxSetCursor(DVASPECT_CONTENT,	-1, NULL, NULL,  m_re->GetWindowDC(),
            NULL, prcClient, newPt.x, newPt.y);

        return TRUE;
    }

    return FALSE;
}

void CTxtWinHost::GetControlRect(LPRECT prc)
{
	UiRect rc = rcClient;
	VerAlignType alignType = m_re->GetTextVerAlignType();
	if (alignType != kVerAlignTop) {
		LONG iWidth = rc.right - rc.left;
		LONG iHeight = 0;
		SIZEL szExtent = { -1, -1 };
		GetTextServices()->TxGetNaturalSize(
			DVASPECT_CONTENT, 
			m_re->GetWindowDC(), 
			NULL,
			NULL,
			TXTNS_FITTOCONTENT,
			&szExtent,
			&iWidth,
			&iHeight);
		if (alignType == kVerAlignCenter) {
			rc.Offset(0, (rc.bottom - rc.top - iHeight) / 2);
		}
		else if (alignType == kVerAlignBottom) {
			rc.Offset(0, prc->bottom - prc->top - iHeight);
		}
	}

	prc->left = rc.left;
	prc->top = rc.top;
	prc->right = rc.right;
	prc->bottom = rc.bottom;
}

void CTxtWinHost::SetTransparent(BOOL f_Transparent)
{
    fTransparent = f_Transparent;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_BACKSTYLECHANGE, 0);
}

LONG CTxtWinHost::SetAccelPos(LONG l_accelpos)
{
    LONG laccelposOld = l_accelpos;

    laccelpos = l_accelpos;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_SHOWACCELERATOR, 0);

    return laccelposOld;
}

WCHAR CTxtWinHost::SetPasswordChar(WCHAR ch_PasswordChar)
{
    WCHAR chOldPasswordChar = chPasswordChar;

    chPasswordChar = ch_PasswordChar;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_USEPASSWORD, 
        (chPasswordChar != 0) ? TXTBIT_USEPASSWORD : 0);

    return chOldPasswordChar;
}

void CTxtWinHost::SetDisabled(BOOL fOn)
{
    cf.dwMask	 |= CFM_COLOR | CFM_DISABLED;
    cf.dwEffects |= CFE_AUTOCOLOR | CFE_DISABLED;

    if( !fOn )
    {
        cf.dwEffects &= ~CFE_DISABLED;
    }

    pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
        TXTBIT_CHARFORMATCHANGE);
}

LONG CTxtWinHost::SetSelBarWidth(LONG l_SelBarWidth)
{
    LONG lOldSelBarWidth = lSelBarWidth;

    lSelBarWidth = l_SelBarWidth;

    if (lSelBarWidth)
    {
        dwStyle |= ES_SELECTIONBAR;
    }
    else
    {
        dwStyle &= (~ES_SELECTIONBAR);
    }

    pserv->OnTxPropertyBitsChange(TXTBIT_SELBARCHANGE, TXTBIT_SELBARCHANGE);

    return lOldSelBarWidth;
}

BOOL CTxtWinHost::GetTimerState()
{
    return fTimer;
}

void CTxtWinHost::SetCharFormat(CHARFORMAT2W &c)
{
    cf = c;
}

void CTxtWinHost::SetParaFormat(PARAFORMAT2 &p)
{
    pf = p;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

RichEdit::RichEdit() : 
	ScrollableBox(new Layout),
	m_pTwh(nullptr), 
	m_bVScrollBarFixing(false), 
	m_bWantTab(true),
	m_bNeedReturnMsg(false),
	m_bReturnMsgWantCtrl(false),
	m_bRich(false),
	m_bReadOnly(false),
	m_bPassword(false),
	m_bWordWrap(false),
	m_bNumberOnly(false),
	m_bInited(false),
	m_bAllowPrompt(false),
	m_bSelAllEver(false),		 
	m_bNoSelOnKillFocus(true), 
	m_bSelAllOnFocus(false),	
	m_bNoCaretReadonly(false),
	m_bIsCaretVisiable(false),
	m_bIsComposition(false),
	m_iCaretPosX(0),
	m_iCaretPosY(0),
	m_iCaretWidth(0),
	m_iCaretHeight(0),
	m_sFontId(),
	m_iLimitText(0),
	m_lTwhStyle(ES_MULTILINE),
	m_textVerAlignType(kVerAlignTop),
	m_sCurrentColor(),
	m_sTextColor(),
	m_sDisabledTextColor(),
	m_sPromptColor(),
	m_sText(),
	m_sPromptText(),
	m_drawCaretFlag(),
	m_timeFlagMap(),
	m_linkInfo(),
	m_sFocusedImage()
{
	m_iLimitText = cInitTextMax;
	m_sCurrentColor = GlobalManager::GetDefaultTextColor();
	m_sTextColor = m_sCurrentColor;
	m_sDisabledTextColor = m_sCurrentColor;
}

RichEdit::~RichEdit()
{
    if( m_pTwh ) {
        m_pTwh->Release();
    }
}

bool RichEdit::IsWantTab()
{
    return m_bWantTab;
}

void RichEdit::SetWantTab(bool bWantTab)
{
    m_bWantTab = bWantTab;
}

bool RichEdit::IsNeedReturnMsg()
{
    return m_bNeedReturnMsg;
}

void RichEdit::SetNeedReturnMsg(bool bNeedReturnMsg)
{
    m_bNeedReturnMsg = bNeedReturnMsg;
}

bool RichEdit::IsReturnMsgWantCtrl()
{
    return m_bReturnMsgWantCtrl;
}

void RichEdit::SetReturnMsgWantCtrl(bool bReturnMsgWantCtrl)
{
    m_bReturnMsgWantCtrl = bReturnMsgWantCtrl;
}

bool RichEdit::IsRich()
{
    return m_bRich;
}

void RichEdit::SetRich(bool bRich)
{
    m_bRich = bRich;
    if( m_pTwh ) m_pTwh->SetRichTextFlag(bRich);
}

bool RichEdit::IsReadOnly()
{
    return m_bReadOnly;
}

void RichEdit::SetReadOnly(bool bReadOnly)
{
    m_bReadOnly = bReadOnly;
    if( m_pTwh ) m_pTwh->SetReadOnly(bReadOnly);
}

bool RichEdit::IsPassword()
{
	return m_bPassword;
}

void RichEdit::SetPassword( bool bPassword )
{
	m_bPassword = bPassword;
	if( m_pTwh ) m_pTwh->SetPassword(bPassword);
}

bool RichEdit::GetWordWrap()
{
    return m_bWordWrap;
}

void RichEdit::SetWordWrap(bool bWordWrap)
{
    m_bWordWrap = bWordWrap;
    if( m_pTwh ) m_pTwh->SetWordWrap(bWordWrap);
}

std::wstring RichEdit::GetFont() const
{
    return m_sFontId;
}

void RichEdit::SetFont(const std::wstring& strFontId)
{
    m_sFontId = strFontId;
    if( m_pTwh ) {
		m_pTwh->SetFont(GlobalManager::GetFont(m_sFontId));
    }
}

void RichEdit::SetFont(const std::wstring& pStrFontName, int nSize, bool bBold, bool bUnderline, bool bStrikeout, bool bItalic)
{
    if( m_pTwh ) {
        LOGFONT lf = { 0 };
        ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        _tcscpy(lf.lfFaceName, pStrFontName.c_str());
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfHeight = -nSize;
        if( bBold ) lf.lfWeight += FW_BOLD;
        if( bUnderline ) lf.lfUnderline = TRUE;
		if (bStrikeout) lf.lfStrikeOut = TRUE;
        if( bItalic ) lf.lfItalic = TRUE;
        HFONT hFont = ::CreateFontIndirect(&lf);
        if( hFont == NULL ) return;
        m_pTwh->SetFont(hFont);
        ::DeleteObject(hFont);
    }
}

void RichEdit::SetFont(HFONT font)
{
	if (m_pTwh)	{
		m_pTwh->SetFont(font);
	}
}

LONG RichEdit::GetWinStyle()
{
    return m_lTwhStyle;
}

void RichEdit::SetWinStyle(LONG lStyle)
{
    m_lTwhStyle = lStyle;
}

ui::VerAlignType RichEdit::GetTextVerAlignType()
{
	return m_textVerAlignType;
}

void RichEdit::SetTextColor(const std::wstring& dwTextColor)
{
	if(m_sCurrentColor == dwTextColor)
		return;
	m_sCurrentColor = dwTextColor;

	DWORD dwTextColor2 = this->GetWindowColor(dwTextColor);
    if( m_pTwh ) {
        m_pTwh->SetColor(dwTextColor2);
    }
}

void RichEdit::SetTextColor(DWORD color)
{
	if (m_pTwh) {
		m_pTwh->SetColor(color);
	}
}

std::wstring RichEdit::GetTextColor()
{
	return m_sCurrentColor;
}

DWORD RichEdit::GetTextColorValue()
{
	return this->GetWindowColor(m_sCurrentColor);
}

int RichEdit::GetLimitText()
{
    return m_iLimitText;
}

void RichEdit::SetLimitText(int iChars)
{
    m_iLimitText = iChars;
    if( m_pTwh ) {
        m_pTwh->LimitText(m_iLimitText);
    }
}

long RichEdit::GetTextLength(DWORD dwFlags) const
{
    GETTEXTLENGTHEX textLenEx;
    textLenEx.flags = dwFlags;
#ifdef _UNICODE
    textLenEx.codepage = 1200;
#else
    textLenEx.codepage = CP_ACP;
#endif
    LRESULT lResult;
    TxSendMessage(EM_GETTEXTLENGTHEX, (WPARAM)&textLenEx, 0, &lResult);
    return (long)lResult;
}

std::wstring RichEdit::GetText() const
{
    long lLen = GetTextLength(GTL_DEFAULT);
    LPTSTR lpText = NULL;
    GETTEXTEX gt;
    gt.flags = GT_DEFAULT;
#ifdef _UNICODE
    gt.cb = sizeof(TCHAR) * (lLen + 1) ;
    gt.codepage = 1200;
    lpText = new TCHAR[lLen + 1];
    ::ZeroMemory(lpText, (lLen + 1) * sizeof(TCHAR));
#else
    gt.cb = sizeof(TCHAR) * lLen * 2 + 1;
    gt.codepage = CP_ACP;
    lpText = new TCHAR[lLen * 2 + 1];
    ::ZeroMemory(lpText, (lLen * 2 + 1) * sizeof(TCHAR));
#endif
    gt.lpDefaultChar = NULL;
    gt.lpUsedDefChar = NULL;
    TxSendMessage(EM_GETTEXTEX, (WPARAM)&gt, (LPARAM)lpText, 0);
    std::wstring sText(lpText);
    delete[] lpText;
    return sText;
}

std::string RichEdit::GetUTF8Text() const
{
	std::string strOut;
	StringHelper::UnicodeToMBCS(GetText(), strOut, CP_UTF8);
	return strOut;
}

void RichEdit::SetText(const std::wstring& strText)
{
	m_sText = strText;
	if( !m_bInited )
		return;

    SetSel(0, -1);

	std::wstring oldText = GetText();

    ReplaceSel(strText, FALSE);

	RaiseUIAValueEvent(oldText, strText);

	m_linkInfo.clear();
}

void RichEdit::SetTextId(const std::wstring& strTextId)
{
	MutiLanSupport* mutilan = MutiLanSupport::GetInstance();
	if (mutilan)
	{
		std::wstring strText = mutilan->GetStringViaID(strTextId);
		SetText(strText);
	}
}

void RichEdit::SetUTF8Text( const std::string& strText )
{
	std::wstring strOut;
	StringHelper::MBCSToUnicode(strText, strOut, CP_UTF8);
	SetText(strOut);
}

bool RichEdit::GetModify() const
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_GETMODIFY, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

void RichEdit::SetModify(bool bModified) const
{ 
    TxSendMessage(EM_SETMODIFY, bModified, 0, 0);
}

void RichEdit::GetSel(CHARRANGE &cr) const
{ 
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0); 
}

void RichEdit::GetSel(long& nStartChar, long& nEndChar) const
{
    CHARRANGE cr;
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0); 
    nStartChar = cr.cpMin;
    nEndChar = cr.cpMax;
}

int RichEdit::SetSel(CHARRANGE &cr)
{ 
    LRESULT lResult;
    TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult); 
    return (int)lResult;
}

int RichEdit::SetSel(long nStartChar, long nEndChar)
{
    CHARRANGE cr;
    cr.cpMin = nStartChar;
    cr.cpMax = nEndChar;
    LRESULT lResult;
    TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult); 
    return (int)lResult;
}

void RichEdit::ReplaceSel(const std::wstring& lpszNewText, bool bCanUndo)
{
#ifdef _UNICODE		
    TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText.c_str(), 0); 
#else
	std::wstring strOut;
	StringHelper::MBCSToUnicode(lpszNewText, strOut, CP_ACP);
	TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)strOut.c_str(), 0); 
#endif
}

void RichEdit::ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo)
{
    TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText, 0); 
}

std::wstring RichEdit::GetSelText() const
{
    if( !m_pTwh ) return std::wstring();
    CHARRANGE cr;
    cr.cpMin = cr.cpMax = 0;
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0);
    LPWSTR lpText = NULL;
    lpText = new WCHAR[cr.cpMax - cr.cpMin + 1];
    ::ZeroMemory(lpText, (cr.cpMax - cr.cpMin + 1) * sizeof(WCHAR));
    TxSendMessage(EM_GETSELTEXT, 0, (LPARAM)lpText, 0);
    std::wstring sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

int RichEdit::SetSelAll()
{
    return SetSel(0, -1);
}

int RichEdit::SetSelNone()
{
    return SetSel(-1, 0);
}

bool RichEdit::GetZoom(int& nNum, int& nDen) const
{
    LRESULT lResult;
    TxSendMessage(EM_GETZOOM, (WPARAM)&nNum, (LPARAM)&nDen, &lResult);
    return (BOOL)lResult == TRUE;
}

bool RichEdit::SetZoom(int nNum, int nDen)
{
    if (nNum < 0 || nNum > 64) return false;
    if (nDen < 0 || nDen > 64) return false;
    LRESULT lResult;
    TxSendMessage(EM_SETZOOM, nNum, nDen, &lResult);
    return (BOOL)lResult == TRUE;
}

bool RichEdit::SetZoomOff()
{
    LRESULT lResult;
    TxSendMessage(EM_SETZOOM, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

WORD RichEdit::GetSelectionType() const
{
    LRESULT lResult;
    TxSendMessage(EM_SELECTIONTYPE, 0, 0, &lResult);
    return (WORD)lResult;
}

bool RichEdit::GetAutoURLDetect() const
{
    LRESULT lResult;
    TxSendMessage(EM_GETAUTOURLDETECT, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

bool RichEdit::SetAutoURLDetect(bool bAutoDetect)
{
    LRESULT lResult;
    TxSendMessage(EM_AUTOURLDETECT, bAutoDetect, 0, &lResult);
    return (BOOL)lResult == FALSE;
}

DWORD RichEdit::GetEventMask() const
{
    LRESULT lResult;
    TxSendMessage(EM_GETEVENTMASK, 0, 0, &lResult);
    return (DWORD)lResult;
}

DWORD RichEdit::SetEventMask(DWORD dwEventMask)
{
    LRESULT lResult;
    TxSendMessage(EM_SETEVENTMASK, 0, dwEventMask, &lResult);
    return (DWORD)lResult;
}

std::wstring RichEdit::GetTextRange(long nStartChar, long nEndChar) const
{
    TEXTRANGEW tr = { 0 };
    tr.chrg.cpMin = nStartChar;
    tr.chrg.cpMax = nEndChar;
    LPWSTR lpText = NULL;
    lpText = new WCHAR[nEndChar - nStartChar + 1];
    ::ZeroMemory(lpText, (nEndChar - nStartChar + 1) * sizeof(WCHAR));
    tr.lpstrText = lpText;
    TxSendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr, 0);
    std::wstring sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

void RichEdit::HideSelection(bool bHide, bool bChangeStyle)
{
    TxSendMessage(EM_HIDESELECTION, bHide, bChangeStyle, 0);
}

void RichEdit::ScrollCaret()
{
    TxSendMessage(EM_SCROLLCARET, 0, 0, 0);
}

int RichEdit::InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo)
{
    int nRet = SetSel(nInsertAfterChar, nInsertAfterChar);

	std::wstring oldText = GetText();

    ReplaceSel(lpstrText, bCanUndo);

	RaiseUIAValueEvent(oldText, GetText());
    
	return nRet;
}

int RichEdit::AppendText(const std::wstring& strText, bool bCanUndo)
{
    int nRet = SetSel(-1, -1);

	std::wstring oldText = GetText();
    
	ReplaceSel(strText, bCanUndo);

	RaiseUIAValueEvent(oldText, GetText());

	return nRet;
}

DWORD RichEdit::GetDefaultCharFormat(CHARFORMAT2 &cf) const
{
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
    return (DWORD)lResult;
}

bool RichEdit::SetDefaultCharFormat(CHARFORMAT2 &cf)
{
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
    if( (BOOL)lResult == TRUE ) {
        CHARFORMAT2W cfw;
        cfw.cbSize = sizeof(CHARFORMAT2W);
        TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cfw, 0);
        m_pTwh->SetCharFormat(cfw);
        return true;
    }
    return false;
}

DWORD RichEdit::GetSelectionCharFormat(CHARFORMAT2 &cf) const
{
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cf, &lResult);
    return (DWORD)lResult;
}

bool RichEdit::SetSelectionCharFormat(CHARFORMAT2 &cf)
{
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &lResult);
    return (BOOL)lResult == TRUE;
}

bool RichEdit::SetWordCharFormat(CHARFORMAT2 &cf)
{
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION|SCF_WORD, (LPARAM)&cf, &lResult); 
    return (BOOL)lResult == TRUE;
}

DWORD RichEdit::GetParaFormat(PARAFORMAT2 &pf) const
{
    pf.cbSize = sizeof(PARAFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
    return (DWORD)lResult;
}

bool RichEdit::SetParaFormat(PARAFORMAT2 &pf)
{
    if( !m_pTwh ) return false;
    pf.cbSize = sizeof(PARAFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
    if( (BOOL)lResult == TRUE ) {
        m_pTwh->SetParaFormat(pf);
        return true;
    }
    return false;
}

bool RichEdit::Redo()
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_REDO, 0, 0, &lResult);
    return (BOOL)lResult == TRUE; 
}

bool RichEdit::Undo()
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_UNDO, 0, 0, &lResult);
    return (BOOL)lResult == TRUE; 
}

void RichEdit::Clear()
{ 
    TxSendMessage(WM_CLEAR, 0, 0, 0); 
}

void RichEdit::Copy()
{ 
    TxSendMessage(WM_COPY, 0, 0, 0); 
}

void RichEdit::Cut()
{ 
    TxSendMessage(WM_CUT, 0, 0, 0); 
}

void RichEdit::Paste()
{ 
    TxSendMessage(WM_PASTE, 0, 0, 0); 
}

BOOL RichEdit::CanPaste(UINT nFormat/* = 0*/)
{
	if (NULL == m_pTwh)
		return FALSE;
	return m_pTwh->CanPaste(nFormat);
}

void RichEdit::PasteSpecial(UINT uClipFormat, DWORD dwAspect/* = 0*/, HMETAFILE hMF/* = 0*/)
{
	if (NULL == m_pTwh)
		return;
	m_pTwh->PasteSpecial(uClipFormat, dwAspect, hMF);
}

int RichEdit::GetLineCount() const
{ 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_GETLINECOUNT, 0, 0, &lResult);
    return (int)lResult; 
}

std::wstring RichEdit::GetLine(int nIndex, int nMaxLength) const
{
    LPWSTR lpText = NULL;
    lpText = new WCHAR[nMaxLength + 1];
    ::ZeroMemory(lpText, (nMaxLength + 1) * sizeof(WCHAR));
    *(LPWORD)lpText = (WORD)nMaxLength;
    TxSendMessage(EM_GETLINE, nIndex, (LPARAM)lpText, 0);
    std::wstring sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

int RichEdit::LineIndex(int nLine) const
{
    LRESULT lResult;
    TxSendMessage(EM_LINEINDEX, nLine, 0, &lResult);
    return (int)lResult;
}

int RichEdit::LineLength(int nLine) const
{
    LRESULT lResult;
    TxSendMessage(EM_LINELENGTH, nLine, 0, &lResult);
    return (int)lResult;
}

bool RichEdit::LineScroll(int nLines, int nChars)
{
    LRESULT lResult;
    TxSendMessage(EM_LINESCROLL, nChars, nLines, &lResult);
    return (BOOL)lResult == TRUE;
}

CPoint RichEdit::GetCharPos(long lChar) const
{ 
    CPoint pt; 
    TxSendMessage(EM_POSFROMCHAR, (WPARAM)&pt, (LPARAM)lChar, 0); 
    return pt;
}

long RichEdit::LineFromChar(long nIndex) const
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_EXLINEFROMCHAR, 0, nIndex, &lResult);
    return (long)lResult;
}

CPoint RichEdit::PosFromChar(UINT nChar) const
{ 
    POINTL pt; 
    TxSendMessage(EM_POSFROMCHAR, (WPARAM)&pt, nChar, 0); 
    return CPoint(pt.x, pt.y); 
}

int RichEdit::CharFromPos(CPoint pt) const
{ 
    POINTL ptl = {pt.x, pt.y}; 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_CHARFROMPOS, 0, (LPARAM)&ptl, &lResult);
    return (int)lResult; 
}

void RichEdit::EmptyUndoBuffer()
{ 
    TxSendMessage(EM_EMPTYUNDOBUFFER, 0, 0, 0); 
}

UINT RichEdit::SetUndoLimit(UINT nLimit)
{ 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_SETUNDOLIMIT, (WPARAM) nLimit, 0, &lResult);
    return (UINT)lResult; 
}

long RichEdit::StreamIn(int nFormat, EDITSTREAM &es)
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_STREAMIN, nFormat, (LPARAM)&es, &lResult);
    return (long)lResult;
}

long RichEdit::StreamOut(int nFormat, EDITSTREAM &es)
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_STREAMOUT, nFormat, (LPARAM)&es, &lResult);
    return (long)lResult; 
}

HRESULT RichEdit::TxSendMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plresult) const
{
    if( m_pTwh ) {
        LRESULT lr =  m_pTwh->GetTextServices()->TxSendMessage(msg, wParam, lParam, plresult);
		return lr;
    }
    return S_FALSE;
}

IDropTarget* RichEdit::GetTxDropTarget()
{
    IDropTarget *pdt = NULL;
    if( m_pTwh->GetTextServices()->TxGetDropTarget(&pdt) == NOERROR ) return pdt;
    return NULL;
}

bool RichEdit::SetDropAcceptFile(bool bAccept)
{
	LRESULT lResult;
	TxSendMessage(EM_SETEVENTMASK, 0, ENM_DROPFILES | ENM_LINK, // ENM_CHANGE| ENM_CORRECTTEXT | ENM_DRAGDROPDONE | ENM_DROPFILES | ENM_IMECHANGE | ENM_LINK | ENM_OBJECTPOSITIONS | ENM_PROTECTED | ENM_REQUESTRESIZE | ENM_SCROLL | ENM_SELCHANGE | ENM_UPDATE,
		&lResult);
	return (BOOL)lResult == FALSE;
}

void RichEdit::OnTxNotify(DWORD iNotify, void *pv)
{
	switch(iNotify)
	{ 
	case EN_LINK:   
		{
			NMHDR* hdr = (NMHDR*) pv;
			ENLINK* link = (ENLINK*)hdr;

			if(link->msg == WM_LBUTTONUP) {
				this->SetSel(link->chrg);
				std::wstring url = GetSelText();	
				wprintf_s(L"[Link]%s\n", url.c_str());

				HWND hwnd = this->GetWindowHandle();
				SendMessage(hwnd, WM_NOTIFY, EN_LINK, (LPARAM)&url);
			}
		}
		break;
	case EN_CHANGE:
		OnTxTextChanged();
		break;
	case EN_DROPFILES:   
	case EN_MSGFILTER:   
	case EN_OLEOPFAILED:    
	case EN_PROTECTED:
	case EN_SAVECLIPBOARD:   
	case EN_SELCHANGE:   
	case EN_STOPNOUNDO:   
	case EN_OBJECTPOSITIONS:   
	case EN_DRAGDROPDONE:   
		{
			if (pv) {   // Fill out NMHDR portion of pv   
				LONG nId =  GetWindowLong(this->GetWindowHandle(), GWL_ID);   
				NMHDR  *phdr = (NMHDR *)pv;   
				phdr->hwndFrom = this->GetWindowHandle();   
				phdr->idFrom = nId;   
				phdr->code = iNotify;  

				SendMessage(this->GetWindowHandle(), WM_NOTIFY, (WPARAM)nId, (LPARAM)pv);
			}    
		}
		break;
	}
}

bool RichEdit::OnTxTextChanged()
{
	if (m_pWindow != NULL) {
		m_pWindow->SendNotify(this, kEventTextChange);
	}

	RaiseUIAValueEvent(GetText(), GetText());

	return true;
}

ITextHost* RichEdit::GetTextHost()
{
	if (NULL == m_pTwh)
		return NULL;
	return m_pTwh->GetTextHost();
}

ITextServices* RichEdit::GetTextServices()
{
	if (NULL == m_pTwh)
		return NULL;
	return m_pTwh->GetTextServices2();
}

HWND RichEdit::GetWindowHandle()
{
	auto window = this->GetWindow();
	return window ? window->GetHWND() : NULL;
}

HDC RichEdit::GetWindowDC()
{
	auto window = this->GetWindow();
	return window ? window->GetPaintDC() : NULL;
}

BOOL RichEdit::SetOleCallback(IRichEditOleCallback* pCallback)
{
	if (NULL == m_pTwh)
		return FALSE;
	return m_pTwh->SetOleCallback(pCallback);
}

CSize RichEdit::GetNaturalSize(LONG width, LONG height)
{
	if (width < 0) {
		width = 0;
	}
	if (height < 0) {
		height = 0;
	}	
	CSize sz(0,0);
	if (m_cbGetNaturalSize != nullptr && m_cbGetNaturalSize(width, height, sz))
		return sz;
	LONG lWidth = width;
	LONG lHeight = height;
	SIZEL szExtent = { -1, -1 };

	if (m_pTwh) {
		m_pTwh->GetTextServices()->TxGetNaturalSize(
			DVASPECT_CONTENT,
			GetWindowDC(),
			NULL,
			NULL,
			TXTNS_FITTOCONTENT,
			&szExtent,
			&lWidth,
			&lHeight);
	}
	sz.cx = (int)lWidth;
	sz.cy = (int)lHeight;
	return sz;
}

void RichEdit::SetImmStatus(BOOL bOpen)
{
	HWND hwnd = GetWindowHandle();
	if (hwnd != NULL)
	{
		// 失去焦点时关闭输入法
		HIMC hImc = ::ImmGetContext(hwnd);
    // 失去焦点是会把关联的输入法去掉，导致无法无法输入中文
		//::ImmAssociateContext(hwnd, bOpen ? hImc : NULL);
		if (hImc != NULL) {
			if (ImmGetOpenStatus(hImc)) {
				if (!bOpen)
					ImmSetOpenStatus(hImc, FALSE);
			}
			else {
				if (bOpen)
					ImmSetOpenStatus(hImc, TRUE);
			}
			ImmReleaseContext(hwnd, hImc);
		}
	}
}

void RichEdit::SetTimer(UINT idTimer, UINT uTimeout)
{
	auto timeFlag = m_timeFlagMap.find(idTimer);
	if (timeFlag != m_timeFlagMap.end()) {
		timeFlag->second.Cancel();
	}

	auto callback = [this, idTimer]() {
		this->TxSendMessage(WM_TIMER, idTimer, 0, 0);
	};
	TimerManager::GetInstance()->AddCancelableTimer(m_timeFlagMap[idTimer].GetWeakFlag(), callback, uTimeout, TimerManager::REPEAT_FOREVER);
}

void RichEdit::KillTimer(UINT idTimer)
{
	auto timeFlag = m_timeFlagMap.find(idTimer);
	if (timeFlag != m_timeFlagMap.end()) {
		timeFlag->second.Cancel();
		m_timeFlagMap.erase(timeFlag);
	}
}

// 多行非rich格式的richedit有一个滚动条bug，在最后一行是空行时，LineDown和SetScrollPos无法滚动到最后
// 引入iPos就是为了修正这个bug
void RichEdit::SetScrollPos(CSize szPos)
{
    int cx = 0;
    int cy = 0;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) {
        int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
        m_pVerticalScrollBar->SetScrollPos(szPos.cy);
        cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
    }
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsValid() ) {
        int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
        m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
        cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
    }
    if( cy != 0 ) {
        int iPos = 0;
        if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) 
            iPos = m_pVerticalScrollBar->GetScrollPos();
        WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, m_pVerticalScrollBar->GetScrollPos());
        TxSendMessage(WM_VSCROLL, wParam, 0L, 0);
        if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) {
            if( cy > 0 && m_pVerticalScrollBar->GetScrollPos() <= iPos )
                m_pVerticalScrollBar->SetScrollPos(iPos);
        }
    }
    if( cx != 0 ) {
        WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, m_pHorizontalScrollBar->GetScrollPos());
        TxSendMessage(WM_HSCROLL, wParam, 0L, 0);
    }
}

void RichEdit::LineUp()
{
    TxSendMessage(WM_VSCROLL, SB_LINEUP, 0L, 0);
}

void RichEdit::LineDown()
{
    int iPos = 0;
    if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) 
        iPos = m_pVerticalScrollBar->GetScrollPos();
    TxSendMessage(WM_VSCROLL, SB_LINEDOWN, 0L, 0);
    if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) {
        if( m_pVerticalScrollBar->GetScrollPos() <= iPos )
            m_pVerticalScrollBar->SetScrollPos(m_pVerticalScrollBar->GetScrollRange());
    }
}

void RichEdit::PageUp()
{
    TxSendMessage(WM_VSCROLL, SB_PAGEUP, 0L, 0);
}

void RichEdit::PageDown()
{
    TxSendMessage(WM_VSCROLL, SB_PAGEDOWN, 0L, 0);
}

void RichEdit::HomeUp()
{
    TxSendMessage(WM_VSCROLL, SB_TOP, 0L, 0);
}

void RichEdit::EndDown()
{
    TxSendMessage(WM_VSCROLL, SB_BOTTOM, 0L, 0);
}

void RichEdit::LineLeft()
{
    TxSendMessage(WM_HSCROLL, SB_LINELEFT, 0L, 0);
}

void RichEdit::LineRight()
{
    TxSendMessage(WM_HSCROLL, SB_LINERIGHT, 0L, 0);
}

void RichEdit::PageLeft()
{
    TxSendMessage(WM_HSCROLL, SB_PAGELEFT, 0L, 0);
}

void RichEdit::PageRight()
{
    TxSendMessage(WM_HSCROLL, SB_PAGERIGHT, 0L, 0);
}

void RichEdit::HomeLeft()
{
    TxSendMessage(WM_HSCROLL, SB_LEFT, 0L, 0);
}

void RichEdit::EndRight()
{
    TxSendMessage(WM_HSCROLL, SB_RIGHT, 0L, 0);
}

std::wstring RichEdit::GetType() const
{
	return DUI_CTR_RICHEDIT;
}

UIAControlProvider* RichEdit::GetUIAProvider()
{
#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider == nullptr)
	{
		m_pUIAProvider = static_cast<UIAControlProvider*>(new (std::nothrow) UIARichEditProvider(this));
	}
	return m_pUIAProvider;
#else
	return nullptr;
#endif
}

void RichEdit::DoInit()
{
	if (m_bInited)
		return;

	CREATESTRUCT cs;
	cs.style = m_lTwhStyle;
	cs.x = 0;
	cs.y = 0;
	cs.cy = 0;
	cs.cx = 0;
	cs.lpszName = m_sText.c_str();
	CreateHost(this, &cs, &m_pTwh);
	if (m_pTwh) {
		m_pTwh->SetTransparent(TRUE);
		LRESULT lResult;
		m_pTwh->GetTextServices()->TxSendMessage(EM_SETLANGOPTIONS, 0, 0, &lResult);
		m_pTwh->GetTextServices()->TxSendMessage(EM_SETEVENTMASK, 0, ENM_CHANGE, &lResult);
		m_pTwh->OnTxInPlaceActivate(NULL);
		if (m_pHorizontalScrollBar) {
			m_pHorizontalScrollBar->SetScrollRange(0);
		}
		if (m_pVerticalScrollBar) {
			m_pVerticalScrollBar->SetScrollRange(0);
		}
	}

	m_bInited = true;
}

void RichEdit::SetEnabled(bool bEnable /*= true*/)
{
	if (m_bEnabled == bEnable)
		return;
	m_bEnabled = bEnable;

	if (bEnable) {
		m_uButtonState = kControlStateNormal;
		SetTextColor(m_sTextColor);
	}
	else {
		m_uButtonState = kControlStateDisabled;
		SetTextColor(m_sDisabledTextColor);
	}
}

CSize RichEdit::EstimateSize(CSize szAvailable)
{
	CSize size(GetFixedWidth(), GetFixedHeight());
	if (size.cx == DUI_LENGTH_AUTO || size.cy == DUI_LENGTH_AUTO) {
		LONG iWidth = size.cx;
		LONG iHeight = size.cy;
		if (size.cx == DUI_LENGTH_AUTO) {
			ASSERT(size.cy != DUI_LENGTH_AUTO);
			iWidth = 0;
		}
		else if (size.cy == DUI_LENGTH_AUTO) {
			ASSERT(size.cx != DUI_LENGTH_AUTO);
			iHeight = 0;
		}

		SIZEL szExtent = { -1, -1 };
		m_pTwh->GetTextServices()->TxGetNaturalSize(
			DVASPECT_CONTENT, 
			GetWindowDC(), 
			NULL,
			NULL,
			TXTNS_FITTOCONTENT,
			&szExtent,
			&iWidth,
			&iHeight);
		
		if (size.cx == DUI_LENGTH_AUTO) {
			size.cx = iWidth + m_pLayout->GetPadding().left + m_pLayout->GetPadding().right;
		}
		else if (size.cy == DUI_LENGTH_AUTO) {
			size.cy = iHeight + m_pLayout->GetPadding().top + m_pLayout->GetPadding().bottom;
		}
	}
    return size;
}

CSize RichEdit::EstimateText(CSize szAvailable)
{
  LONG iWidth = szAvailable.cx;
  LONG iHeight = 0;

  SIZEL szExtent = { -1, -1 };
  m_pTwh->GetTextServices()->TxGetNaturalSize(
	DVASPECT_CONTENT,
	GetWindow()->GetPaintDC(),
	NULL,
	NULL,
	TXTNS_FITTOCONTENT,
	&szExtent,
	&iWidth,
	&iHeight);

  szAvailable.cx = iWidth;
  szAvailable.cy = iHeight;

  return szAvailable;
}

void RichEdit::SetPos(UiRect rc)
{
    Control::SetPos(rc);
    rc = m_rcItem;

    rc.left += m_pLayout->GetPadding().left;
    rc.top += m_pLayout->GetPadding().top;
    rc.right -= m_pLayout->GetPadding().right;
    rc.bottom -= m_pLayout->GetPadding().bottom;
    bool bVScrollBarVisiable = false;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) {
        bVScrollBarVisiable = true;
        rc.right -= m_pVerticalScrollBar->GetFixedWidth();
    }
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsValid() ) {
        rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
    }

    if( m_pTwh ) {
        m_pTwh->SetClientRect(&rc);
        if( bVScrollBarVisiable && (!m_pVerticalScrollBar->IsValid() || m_bVScrollBarFixing) ) {
            LONG lWidth = rc.right - rc.left + m_pVerticalScrollBar->GetFixedWidth();
            LONG lHeight = 0;
            SIZEL szExtent = { -1, -1 };
            m_pTwh->GetTextServices()->TxGetNaturalSize(
                DVASPECT_CONTENT, 
                GetWindowDC(), 
                NULL,
                NULL,
                TXTNS_FITTOCONTENT,
                &szExtent,
                &lWidth,
                &lHeight);
            if( lHeight > rc.bottom - rc.top ) {
                //m_pVerticalScrollBar->SetVisible(true);
                m_pVerticalScrollBar->SetScrollPos(0);
                m_bVScrollBarFixing = true;
            }
            else {
                if( m_bVScrollBarFixing ) {
                    //m_pVerticalScrollBar->SetVisible(false);
					m_pVerticalScrollBar->SetScrollRange(0);
                    m_bVScrollBarFixing = false;
                }
            }
        }
    }

    if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsValid() ) {
        UiRect rcScrollBarPos(rc.right, rc.top, rc.right + m_pVerticalScrollBar->GetFixedWidth(), rc.bottom);
        m_pVerticalScrollBar->SetPos(rcScrollBarPos);
    }
    if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsValid() ) {
        UiRect rcScrollBarPos(rc.left, rc.bottom, rc.right, rc.bottom + m_pHorizontalScrollBar->GetFixedHeight());
        m_pHorizontalScrollBar->SetPos(rcScrollBarPos);
    }

	for( auto it = m_items.begin(); it != m_items.end(); it++ ) {
		auto pControl = *it;
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) {
            Layout::SetFloatPos(pControl, GetPos());
        }
        else {
            pControl->SetPos(rc); // 所有非float子控件放大到整个客户区
        }
    }
}

UINT RichEdit::GetControlFlags() const
{
	return IsEnabled() && IsAllowTabStop() ? UIFLAG_TABSTOP : UIFLAG_DEFAULT;
}

void RichEdit::HandleMessage(EventArgs& event)
{
	if ((!IsMouseEnabled() && event.Type > kEventMouseBegin && event.Type < kEventMouseEnd) ||
		(!IsEnabled()&&!IsReadOnly())){
		if (m_pParent != NULL) m_pParent->HandleMessageTemplate(event);
		else Control::HandleMessage(event);
		return;
	}

	if (event.Type == kEventSetCursor)
	{
		OnSetCursor(event);
		return;
	}

	if (event.Type == kEventChar) {
		OnChar(event);
		return;
	}
	if (event.Type == kEventKeyDown) {
		OnKeyDown(event);
		return;
	}

	if (event.Type == kEventMouseMove) {
		OnMouseMessage(WM_MOUSEMOVE, event);
		return;
	}
	if (event.Type == kEventMouseScrollWheel) {
		if (::GetAsyncKeyState(VK_CONTROL) < 0)
			return;
		
		ScrollableBox::HandleMessage(event);
		//OnMouseMessage(WM_MOUSEWHEEL, event);
		return;
	}

	if (event.Type == kEventMouseButtonDown || event.Type == kEventPointDown) {
		if (m_linkInfo.size() > 0)	{
			std::wstring strLink;
			if (HittestCustomLink(event.ptMouse, strLink))
			{
				//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
				if (m_pWindow != NULL)
					m_pWindow->SendNotify((Control*)this, kEventCustomLinkClick);
				return;
			}
		}

		if (event.Type == kEventPointDown) {
			OnScreenKeyboardManager::GetInstance()->ShowOSK(true);
		}
		OnMouseMessage(WM_LBUTTONDOWN, event);
		return;
	}
	if (event.Type == kEventMouseButtonUp || event.Type == kEventPointUp) {
		if (m_bEnabled && !m_bSelAllEver) {
			m_bSelAllEver = true;

			if (m_bSelAllOnFocus) {
				SetSelAll();
				if (m_lTwhStyle & ES_MULTILINE)
					HomeUp();
				else
					HomeLeft();
			}
		}

		OnMouseMessage(WM_LBUTTONUP, event);
		return;
	}
	if (event.Type == kEventInternalDoubleClick) {
		if (m_bReadOnly) {
			SetSelAll();
			return;
		}

		OnMouseMessage(WM_LBUTTONDBLCLK, event);
		return;
	}
	if (event.Type == kEventMouseRightButtonDown) {
		OnMouseMessage(WM_RBUTTONDOWN, event);
		return;
	}
	if (event.Type == kEventMouseRightButtonUp) {
		OnMouseMessage(WM_RBUTTONUP, event);
		return;
	}

	if (event.Type == kEventImeStartComposition) {
		OnImeStartComposition(event);
		return;
	}
	if (event.Type == kEventImeEndComposition) {
		OnImeEndComposition(event);
		return;
	}

	if (event.Type == kEventInternalSetFocus) {
		OnSetFocus(event);
		SetImmStatus(TRUE);
		return;
	}
	if (event.Type == kEventInternalKillFocus) {
		OnKillFocus(event);
		OnScreenKeyboardManager::GetInstance()->ShowOSK(false);
		SetImmStatus(FALSE);
		return;
	}

	ScrollableBox::HandleMessage(event);
}

void RichEdit::OnSetCursor(EventArgs& event)
{
	std::wstring strLink;
	if (HittestCustomLink(event.ptMouse, strLink))
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return;
	}
	if (m_pTwh && !IsReadOnly() && m_pTwh->DoSetCursor(NULL, &event.ptMouse)) {
		return;
	}
	else {
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IsReadOnly() ? IDC_ARROW : IDC_IBEAM)));
	}
}

void RichEdit::OnSetFocus(EventArgs& event)
{
	if (m_pTwh) {
		m_pTwh->OnTxInPlaceActivate(NULL);
		m_pTwh->GetTextServices()->TxSendMessage(WM_SETFOCUS, 0, 0, 0);
		ShowCaret(true);
	}
	m_bFocused = true;
	Invalidate();
}

void RichEdit::OnKillFocus(EventArgs& event)
{
	if (m_pTwh) {
		m_pTwh->OnTxInPlaceActivate(NULL);
		m_pTwh->GetTextServices()->TxSendMessage(WM_KILLFOCUS, 0, 0, 0);
		ShowCaret(false);
	}

	m_bFocused = false;
	m_bSelAllEver = false;
	if (m_bNoSelOnKillFocus && m_bReadOnly && m_bEnabled) {
		SetSelNone();
	}
	if (m_bSelAllOnFocus && m_bEnabled) {
		SetSelNone();
	}

	Invalidate();
}

void RichEdit::OnChar(EventArgs& event)
{
	//TAB
	if (::GetKeyState(VK_TAB) < 0 && !m_bWantTab) {
		if (m_pWindow != NULL)
			m_pWindow->SendNotify((Control*)this, kEventTab);

		return;
	}
	//Number
	if (m_bNumberOnly) {
		if (event.wParam < '0' || event.wParam > '9')
			return;
	}

	TxSendMessage(WM_CHAR, event.wParam, event.lParam, NULL);
}

void RichEdit::OnKeyDown(EventArgs& event)
{
	if (event.wParam == VK_RETURN && ::GetAsyncKeyState(VK_SHIFT) >= 0)	{
		if (m_bNeedReturnMsg && ((m_bReturnMsgWantCtrl && ::GetAsyncKeyState(VK_CONTROL) < 0) ||
			(!m_bReturnMsgWantCtrl && ::GetAsyncKeyState(VK_CONTROL) >= 0)))
		{
			if (m_pWindow != NULL)
				m_pWindow->SendNotify((Control*)this, kEventReturn);
			return;
		}
	}
	else if (m_bNumberOnly && event.wParam == 'V' && ::GetKeyState(VK_CONTROL) < 0) {
		std::wstring strClipText;
		GetClipboardText(strClipText);
		if (!strClipText.empty()) {
			std::wstring strNum;
			for (auto it = strClipText.begin(); it != strClipText.end(); it++)
			{
				if ((*it) <= L'9' && (*it) >= L'0') {
					strNum.push_back((*it));
				}
			}
			if (strNum.empty())
				return;

			SetClipBoardText(strNum); //修改剪切板内容为纯数字
			nbase::ThreadManager::PostTask([strClipText]() { SetClipBoardText(strClipText); }); //粘贴完后又把剪切板内容改回来
		}
	}

	TxSendMessage(WM_KEYDOWN, event.wParam, event.lParam, NULL);
}

void RichEdit::OnImeStartComposition(EventArgs& event)
{
	HWND hWnd = GetWindowHandle();
	if (hWnd == NULL)
		return;

	HIMC hImc = ::ImmGetContext(hWnd);
	if (hImc == NULL)
		return;

	COMPOSITIONFORM	cfs;
	CPoint ptScrollOffset = GetScrollOffset();
	POINT pt;
	pt.x = m_iCaretPosX - ptScrollOffset.x;
	pt.y = m_iCaretPosY - ptScrollOffset.y;

	//pt.y += (m_iCaretHeight + lf.lfHeight) / 4;
	cfs.dwStyle = CFS_POINT;
	if (pt.x < 1) pt.x = 1;
	if (pt.y < 1) pt.y = 1;
	cfs.ptCurrentPos = pt;
	::ImmSetCompositionWindow(hImc, &cfs);
	::ImmReleaseContext(hWnd, hImc);
	m_bIsComposition = true;
}

void RichEdit::OnImeEndComposition(EventArgs& event)
{
	m_bIsComposition = false;
}

void RichEdit::OnMouseMessage(UINT uMsg, EventArgs& event)
{
	CPoint pt(GET_X_LPARAM(event.lParam), GET_Y_LPARAM(event.lParam));
	pt.Offset(GetScrollOffset());
	TxSendMessage(uMsg, event.wParam, MAKELPARAM(pt.x, pt.y), NULL);
}

void RichEdit::Paint(IRenderContext* pRender, const UiRect& rcPaint)
{
    UiRect rcTemp;
    if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return;

    Control::Paint(pRender, rcPaint);

    if( m_pTwh ) {
        UiRect rc;
        m_pTwh->GetControlRect(&rc);
        // Remember wparam is actually the hdc and lparam is the update
        // rect because this message has been preprocessed by the window.
        m_pTwh->GetTextServices()->TxDraw(
            DVASPECT_CONTENT,  		// Draw Aspect
            /*-1*/0,				// Lindex
            NULL,					// Info for drawing optimazation
            NULL,					// target device information
            pRender->GetDC(),			        // Draw device HDC
            NULL, 				   	// Target device HDC
            (RECTL*)&rc,			// Bounding client rectangle
            NULL, 		            // Clipping rectangle for metafiles
            (UiRect*)&rcPaint,		// Update rectangle
            NULL, 	   				// Call back function
            NULL,					// Call back parameter
            0);				        // What view of the object

        if( m_bVScrollBarFixing ) {
            LONG lWidth = rc.right - rc.left + m_pVerticalScrollBar->GetFixedWidth();
			//LONG lWidth = rc.right - rc.left;
            LONG lHeight = 0;
            SIZEL szExtent = { -1, -1 };
            m_pTwh->GetTextServices()->TxGetNaturalSize(
                DVASPECT_CONTENT, 
                GetWindowDC(), 
                NULL,
                NULL,
                TXTNS_FITTOCONTENT,
                &szExtent,
                &lWidth,
                &lHeight);

            if( lHeight <= rc.bottom - rc.top ) {
                Arrange();
            }
        }
    }
}

void RichEdit::PaintChild(IRenderContext* pRender, const UiRect& rcPaint)
{
	UiRect rcTemp;
	if (!::IntersectRect(&rcTemp, &rcPaint, &m_rcItem)) return;

	PaintCaret(pRender, rcPaint);

    if( m_items.size() > 0 ) {
        UiRect rc = m_rcItem;
        rc.left += m_pLayout->GetPadding().left;
        rc.top += m_pLayout->GetPadding().top;
        rc.right -= m_pLayout->GetPadding().right;
        rc.bottom -= m_pLayout->GetPadding().bottom;
        if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsValid() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsValid() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

        if( !::IntersectRect(&rcTemp, &rcPaint, &rc) ) {
			for( auto it = m_items.begin(); it != m_items.end(); it++ ) {
				auto pControl = *it;
                if( !pControl->IsVisible() ) continue;
				UiRect controlPos = pControl->GetPos();
				if (!::IntersectRect(&rcTemp, &rcPaint, &controlPos)) continue;
                if( pControl ->IsFloat() ) {
					if( !::IntersectRect(&rcTemp, &m_rcItem, &controlPos )) continue;
                    pControl->AlphaPaint(pRender, rcPaint);
                }
            }
        }
        else {
			AutoClip childClip(pRender, rcTemp);
			for( auto it = m_items.begin(); it != m_items.end(); it++ ) {
				auto pControl = *it;
                if( !pControl->IsVisible() ) continue;
				UiRect controlPos = pControl->GetPos();
				if (!::IntersectRect(&rcTemp, &rcPaint, &controlPos)) continue;
                if( pControl ->IsFloat() ) {
					if( !::IntersectRect(&rcTemp, &m_rcItem, &controlPos) ) continue;
                    pControl->AlphaPaint(pRender, rcPaint);
                }
                else {
					if( !::IntersectRect(&rcTemp, &rc, &controlPos) ) continue;
                    pControl->AlphaPaint(pRender, rcPaint);
                }
            }
        }
    }

    if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() ) {
		UiRect verBarPos = m_pVerticalScrollBar->GetPos();
		if( ::IntersectRect(&rcTemp, &rcPaint, &verBarPos) ) {
            m_pVerticalScrollBar->AlphaPaint(pRender, rcPaint);
        }
    }

    if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() ) {
		UiRect horBarPos = m_pVerticalScrollBar->GetPos();
		if( ::IntersectRect(&rcTemp, &rcPaint, &horBarPos) ) {
            m_pHorizontalScrollBar->AlphaPaint(pRender, rcPaint);
        }
    }
}

void RichEdit::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if (strName == _T("vscrollbar")) {
		if (strValue == _T("true")) {
			m_lTwhStyle |= ES_DISABLENOSCROLL | WS_VSCROLL;
			EnableScrollBar(true, GetHorizontalScrollBar() != NULL);
		}
		else {
			m_lTwhStyle &= ~WS_VSCROLL;
			if (!(m_lTwhStyle & WS_HSCROLL)) m_lTwhStyle &= ~ES_DISABLENOSCROLL;
			EnableScrollBar(false, GetHorizontalScrollBar() != NULL);
		}
	}
	else if (strName == _T("autovscroll")) {
		if (strValue == _T("true")) m_lTwhStyle |= ES_AUTOVSCROLL;
		else if (strValue == _T("false")) m_lTwhStyle &= ~ES_AUTOVSCROLL;
	}
	else if (strName == _T("hscrollbar")) {
		if (strValue == _T("true")) {
			m_lTwhStyle |= ES_DISABLENOSCROLL | WS_HSCROLL;
			EnableScrollBar(GetVerticalScrollBar() != NULL, true);
		}
		else {
			m_lTwhStyle &= ~WS_HSCROLL;
			if (!(m_lTwhStyle & WS_VSCROLL)) m_lTwhStyle &= ~ES_DISABLENOSCROLL;
			EnableScrollBar(GetVerticalScrollBar() != NULL, false);
		}
	}
	else if (strName == _T("autohscroll")) {
		if (strValue == _T("true")) m_lTwhStyle |= ES_AUTOHSCROLL;
		else if (strValue == _T("false")) m_lTwhStyle &= ~ES_AUTOHSCROLL;
	}
	else if (strName == _T("multiline")) {
		if (strValue == _T("false")) m_lTwhStyle &= ~ES_MULTILINE;
		else if (strValue == _T("true")) m_lTwhStyle |= ES_MULTILINE;
	}
	else if (strName == _T("readonly")) {
		if (strValue == _T("true")) {
			m_lTwhStyle |= ES_READONLY;
			m_bReadOnly = true;
		}
		else if (strValue == _T("false")) {
			m_lTwhStyle &= ~ES_READONLY;
			m_bReadOnly = false;
		}
	}
	else if (strName == _T("password")) {
		if (strValue == _T("true")) {
			m_lTwhStyle |= ES_PASSWORD;
			m_bPassword = true;
		}
		else if (strValue == _T("false")) {
			m_lTwhStyle &= ~ES_PASSWORD;
			m_bPassword = false;
		}
	}
	else if (strName == _T("number")) {
		if (strValue == _T("true")) {
			m_lTwhStyle |= ES_NUMBER;
			m_bNumberOnly = true;
		}
		else if (strValue == _T("false")){
			m_lTwhStyle &= ~ES_NUMBER;
			m_bNumberOnly = false;
		}
	}
	else if (strName == _T("align")) {
		if (strValue.find(_T("left")) != std::wstring::npos) {
			m_lTwhStyle &= ~(ES_CENTER | ES_RIGHT);
			m_lTwhStyle |= ES_LEFT;
		}
		if (strValue.find(_T("hcenter")) != std::wstring::npos) {
			m_lTwhStyle &= ~(ES_LEFT | ES_RIGHT);
			m_lTwhStyle |= ES_CENTER;
		}
		if (strValue.find(_T("right")) != std::wstring::npos) {
			m_lTwhStyle &= ~(ES_LEFT | ES_CENTER);
			m_lTwhStyle |= ES_RIGHT;
		}
		if (strValue.find(_T("top")) != std::wstring::npos) {
			m_textVerAlignType = kVerAlignTop;
		}
		if (strValue.find(_T("vcenter")) != std::wstring::npos) {
			m_textVerAlignType = kVerAlignCenter;
		}
		if (strValue.find(_T("bottom")) != std::wstring::npos) {
			m_textVerAlignType = kVerAlignBottom;
		}
	}
	else if (strName == _T("normaltextcolor")) {
		LPCTSTR pValue = strValue.c_str();
		while (*pValue > _T('\0') && *pValue <= _T(' ')) pValue = ::CharNext(pValue);
		m_sTextColor = pValue;
		if (m_bEnabled)
			SetTextColor(m_sTextColor);
	}
	else if (strName == L"disabledtextcolor") {
		LPCTSTR pValue = strValue.c_str();
		while (*pValue > _T('\0') && *pValue <= _T(' ')) pValue = ::CharNext(pValue);
		m_sDisabledTextColor = pValue;
		if (!m_bEnabled)
			SetTextColor(m_sDisabledTextColor);
	}
	else if (strName == L"caretcolor") {
		LPCTSTR pValue = strValue.c_str();
		while (*pValue > _T('\0') && *pValue <= _T(' ')) pValue = ::CharNext(pValue);
		SetCaretColor(pValue);
	}
	else if (strName == _T("promptmode")) {
		if (strValue == _T("true"))
			m_bAllowPrompt = true;
	}
	else if (strName == _T("promptcolor")) {
		LPCTSTR pValue = strValue.c_str();
		while (*pValue > _T('\0') && *pValue <= _T(' ')) pValue = ::CharNext(pValue);
		m_sPromptColor = pValue;
	}
	else if (strName == _T("prompttext")) SetPromptText(strValue);
	else if (strName == _T("prompttextid")) SetPromptTextId(strValue);
	else if (strName == _T("focusedimage")) SetFocusedImage(strValue);
	else if (strName == _T("font")) SetFont(strValue);
	else if (strName == _T("text")) SetText(strValue.c_str());
	else if (strName == _T("textid")) SetTextId(strValue.c_str());
	else if (strName == _T("wanttab")) SetWantTab(strValue == _T("true"));
	else if (strName == _T("wantreturnmsg")) SetNeedReturnMsg(strValue == _T("true"));
	else if (strName == _T("returnmsgwantctrl")) SetReturnMsgWantCtrl(strValue == _T("true"));
	else if (strName == _T("rich")) SetRich(strValue == _T("true"));
	else if (strName == _T("maxchar")) SetLimitText(_ttoi(strValue.c_str()));
	else Box::SetAttribute(strName, strValue);
}

BOOL RichEdit::CreateCaret(INT xWidth, INT yHeight)
{
	m_iCaretWidth = xWidth;
	m_iCaretHeight = yHeight;
	return true;
}

BOOL RichEdit::ShowCaret(BOOL fShow)
{
	if (fShow) {
		m_bIsCaretVisiable = true;
		m_drawCaretFlag.Cancel();
		std::function<void()> closure = nbase::Bind(&RichEdit::ChangeCaretVisiable, this);
		TimerManager::GetInstance()->AddCancelableTimer(m_drawCaretFlag.GetWeakFlag(), closure, 500, TimerManager::REPEAT_FOREVER);
	}
	else {
		m_bIsCaretVisiable = false;
		m_drawCaretFlag.Cancel();
	}

	Invalidate();
	return true;
}

void RichEdit::SetCaretColor(const std::wstring& dwColor)
{
	m_sCaretColor = dwColor;
}

std::wstring RichEdit::GetCaretColor()
{
	return m_sCaretColor;
}

RECT RichEdit::GetCaretRect()
{
	RECT rc = { m_iCaretPosX, m_iCaretPosY, m_iCaretPosX + m_iCaretWidth, m_iCaretPosY + m_iCaretHeight };
	return rc;
}

BOOL RichEdit::SetCaretPos(INT x, INT y)
{
	m_iCaretPosX = x;
	m_iCaretPosY = y;
	ShowCaret(GetSelText().empty());

	return true;
}

void RichEdit::ChangeCaretVisiable()
{
	m_bIsCaretVisiable = !m_bIsCaretVisiable;
	Invalidate();
}

void RichEdit::PaintCaret(IRenderContext* pRender, const UiRect& rcPaint)
{
	if (m_bReadOnly && m_bNoCaretReadonly)
		return;

	if (m_bIsCaretVisiable && !m_bIsComposition) {
		UiRect rect(m_iCaretPosX, m_iCaretPosY, m_iCaretPosX, m_iCaretPosY + m_iCaretHeight);
		DWORD dwClrColor = 0xff000000;

		if (!m_sCaretColor.empty())
			dwClrColor = this->GetWindowColor(m_sCaretColor);

		pRender->DrawLine(rect, m_iCaretWidth, dwClrColor);
	}
}

void RichEdit::SetPromptMode(bool bPrompt)
{
	if(bPrompt == m_bAllowPrompt)
		return;
	m_bAllowPrompt = bPrompt;
	Invalidate();
}

std::wstring RichEdit::GetPromptText() const
{
	std::wstring strText = m_sPromptText;
	if (strText.empty() && !m_sPromptTextId.empty()) {
		strText = MutiLanSupport::GetInstance()->GetStringViaID(m_sPromptTextId);
	}

	return strText;
}

std::string RichEdit::GetUTF8PromptText() const
{
	std::string strOut;
	StringHelper::UnicodeToMBCS(GetPromptText(), strOut, CP_UTF8);
	return strOut;
}

void RichEdit::SetPromptText(const std::wstring& strText)
{
	if (m_sPromptText == strText) return;
	m_sPromptText = strText;

	Invalidate();
}

void RichEdit::SetUTF8PromptText(const std::string& strText)
{
	std::wstring strOut;
	StringHelper::MBCSToUnicode(strText, strOut, CP_UTF8);
	SetPromptText(strOut);
}

void RichEdit::SetPromptTextId(const std::wstring& strTextId)
{
	if (m_sPromptTextId == strTextId) return;
	m_sPromptTextId = strTextId;

	Invalidate();
}

void RichEdit::SetUTF8PromptTextId(const std::string& strTextId)
{
	std::wstring strOut;
	StringHelper::MBCSToUnicode(strTextId, strOut, CP_UTF8);
	SetPromptTextId(strOut);
}

void RichEdit::PaintPromptText(IRenderContext* pRender)
{
	long len = GetTextLength(GTL_DEFAULT);
	if (len != 0)
		return;

	if (!m_pTwh)
		return;

	std::wstring strPrompt = GetPromptText();
	if (strPrompt.empty() || m_sPromptColor.empty())
		return;

	UiRect rc;
	m_pTwh->GetControlRect(&rc);

	DWORD dwClrColor = this->GetWindowColor(m_sPromptColor);
	UINT dwStyle = DT_NOCLIP;
	pRender->DrawText(rc, strPrompt, dwClrColor, m_sFontId, dwStyle);
}

std::wstring RichEdit::GetFocusedImage()
{
	return m_sFocusedImage.imageAttribute.simageString;
}

void RichEdit::SetFocusedImage( const std::wstring& strImage )
{
	m_sFocusedImage.SetImageString(strImage);
	Invalidate();
}

void RichEdit::PaintStatusImage(IRenderContext* pRender)
{
	if( IsReadOnly() )
		return;

	if(IsFocused()) {
		DrawImage(pRender, m_sFocusedImage);
		PaintPromptText(pRender);
		return;
	}

	__super::PaintStatusImage(pRender);
	PaintPromptText(pRender);
}

void RichEdit::SetNoSelOnKillFocus(bool bNoSel)
{
	m_bNoSelOnKillFocus = bNoSel;
}

void RichEdit::SetSelAllOnFocus(bool bSelAll)
{
	m_bSelAllOnFocus = bSelAll;
}

void RichEdit::SetNoCaretReadonly()
{
	m_bNoCaretReadonly = true;
}

void RichEdit::AddColorText(const std::wstring &str, const std::wstring &color)
{
	if( !m_bRich || str.empty() || color.empty() ) {
		ASSERT(FALSE);
		return;
	}
	DWORD dwColor = this->GetWindowColor(color);

	CHARFORMAT2W cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.cbSize = sizeof(CHARFORMAT2W);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0;
	cf.crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));

	this->ReplaceSel(str, FALSE);
	int len = GetTextLength();
	this->SetSel(len - (int)str.size(), len);
	this->SetSelectionCharFormat(cf);

	this->SetSelNone();
	GetDefaultCharFormat(cf);
	SetSelectionCharFormat(cf);
}

void RichEdit::AddLinkColorText(const std::wstring &str, const std::wstring &color, const std::wstring &linkInfo)
{
	if( !m_bRich || str.empty() || color.empty() ) {
		ASSERT(FALSE);
		return;
	}
	DWORD dwColor = this->GetWindowColor(color);

	CHARFORMAT2W cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.cbSize = sizeof(CHARFORMAT2W);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));

	this->ReplaceSel(str, FALSE);
	int len = GetTextLength();
	this->SetSel(len - (int)str.size(), len);
	this->SetSelectionCharFormat(cf);
	LinkInfo info;
	info.info = linkInfo;
	TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&(info.cr), NULL);
	m_linkInfo.push_back(info);

	this->SetSelNone();
	GetDefaultCharFormat(cf);
	SetSelectionCharFormat(cf);
}
void  RichEdit::AddLinkColorTextEx(const std::wstring& str, const std::wstring &color, const std::wstring &linkInfo, const std::wstring& strFontId)
{
	if (!m_bRich || str.empty() || color.empty()) {
		ASSERT(FALSE);
		return;
	}
	
	std::string link;
	std::string text;
	std::string font_face;
	StringHelper::UnicodeToMBCS(linkInfo, link);
	StringHelper::UnicodeToMBCS(str, text);
	auto hFont = GlobalManager::GetFont(strFontId);
	if (hFont == NULL)
		hFont = GlobalManager::GetFont(m_sFontId);
	if (hFont == NULL)
		hFont = GlobalManager::GetFont(L"");
	LOGFONT lf;
	::GetObject(hFont, sizeof(LOGFONT), &lf);
	StringHelper::UnicodeToMBCS(lf.lfFaceName, font_face);
	DWORD dwTextColor = GlobalManager::GetTextColor(color);
	static std::string font_format = "{\\fonttbl{\\f0\\fnil\\fcharset%d %s;}}";
	static std::string color_format = "{\\colortbl ;\\red%d\\green%d\\blue%d;}";
	static std::string link_format = "{\\rtf1%s%s\\f0\\fs%d{\\field{\\*\\fldinst{HYPERLINK \"%s\"}}{\\fldrslt{\\cf1 %s}}}}";
	char sfont[255];
	sprintf(sfont, font_format.c_str(), lf.lfCharSet, font_face.c_str());
	char scolor[255];
	sprintf(scolor, color_format.c_str(), GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor));
	char slinke[1024];
	sprintf(slinke, link_format.c_str(), sfont, scolor, ((int)(-lf.lfHeight *1.5))/2*2, link.c_str(), text.c_str());
	std::wstring temp;
	StringHelper::MBCSToUnicode(slinke, temp);
	SETTEXTEX st;
	st.codepage = ((UINT32)~((UINT32)0));
	st.flags = ST_SELECTION | ST_KEEPUNDO;
	TxSendMessage(EM_SETTEXTEX, (WPARAM)&st, (LPARAM)(LPCTSTR)slinke, NULL);
	return;
}
void RichEdit::AddLinkInfo(const CHARRANGE cr, const std::wstring &linkInfo)
{
	LinkInfo info;
	info.info = linkInfo;
	info.cr = cr;
	m_linkInfo.push_back(info);
}

void RichEdit::AddLinkInfoEx(const CHARRANGE cr, const std::wstring& linkInfo)
{
	CHARFORMAT2 cf2;
	ZeroMemory(&cf2, sizeof(CHARFORMAT2));
	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2.dwMask = CFM_LINK;
	cf2.dwEffects |= CFE_LINK;

	SetSel(cr.cpMin, cr.cpMax);
	SetSelectionCharFormat(cf2);

	AddLinkInfo(cr, linkInfo);
}

//根据point来hittest自定义link的数据，返回true表示在link上，info是link的自定义属性
bool RichEdit::HittestCustomLink(CPoint pt, std::wstring& info)
{
	bool bLink = false;
	info.clear();
	if (m_linkInfo.size() > 0) {
		pt.Offset(GetScrollOffset());
		int nCharIndex = CharFromPos(pt);
		for (auto it = m_linkInfo.begin(); it != m_linkInfo.end(); it++)
		{
			if ((*it).cr.cpMin <= nCharIndex && (*it).cr.cpMax > nCharIndex) {
				info = (*it).info;
				bLink = true;
				break;
			}
		}
	}

	return bLink;
}

void RichEdit::ClearImageCache()
{
	__super::ClearImageCache();
	m_sFocusedImage.ClearCache();
}


void RichEdit::RaiseUIAValueEvent(const std::wstring oldText, const std::wstring newText)
{
#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider != nullptr && UiaClientsAreListening()) {
		VARIANT vtOld = { 0 }, vtNew = { 0 };
		vtOld.vt = vtNew.vt = VT_BSTR;
		vtOld.bstrVal = SysAllocString(oldText.c_str());
		vtNew.bstrVal = SysAllocString(newText.c_str());

		UiaRaiseAutomationPropertyChangedEvent(m_pUIAProvider, UIA_ValueValuePropertyId, vtOld, vtNew);
	}
#endif
}


//----------------下面函数用作辅助 字节数限制
bool IsAsciiChar(const wchar_t ch)
{
	return (ch <= 0x7e && ch >= 0x20);
}

int GetAsciiCharNumber(const std::wstring &str)
{
	int len = (int)str.size(), sum = 0;
	for( int i = 0; i < len; i++ )
	{
		if( IsAsciiChar(str[i]) )
			sum += 1;
		else
			sum += 2;
	}
	return sum;
}

void LimitAsciiNumber(std::wstring &src, int limit)
{
	int len = (int)src.size(), sum = 0;
	for( int i = 0; i < len; i++ )
	{
		if( IsAsciiChar(src[i]) )
			sum += 1;
		else
			sum += 2;
		if( sum > limit ) {
			src.erase(i);
			break;
		}
	}
}

void GetClipboardText( std::wstring &out )
{
	out.clear();

	BOOL ret = ::OpenClipboard(NULL);
	if(ret) {
		if(::IsClipboardFormatAvailable(CF_UNICODETEXT)) {
			HANDLE h = ::GetClipboardData(CF_UNICODETEXT);
			if(h != INVALID_HANDLE_VALUE) {
				wchar_t* buf = (wchar_t*)::GlobalLock(h);
				if(buf != NULL)	{
					std::wstring str(buf, GlobalSize(h)/sizeof(wchar_t));
					out = str;
					::GlobalUnlock(h);
				}
			}
		}
		else if(::IsClipboardFormatAvailable(CF_TEXT)) {
			HANDLE h = ::GetClipboardData(CF_TEXT);
			if(h != INVALID_HANDLE_VALUE) {
				char* buf = (char*)::GlobalLock(h);
				if(buf != NULL)	{
					std::string str(buf, GlobalSize(h));
					StringHelper::MBCSToUnicode(str, out);

					::GlobalUnlock(h);
				}
			}
		}
		::CloseClipboard();
	}
}

void SetClipBoardText(const std::wstring &str)
{
	if (!::OpenClipboard(NULL))
		return;

	if (!::EmptyClipboard()) {
		::CloseClipboard();
		return;
	}

	size_t len = str.length();
	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, (len + 1) * sizeof(wchar_t)); //分配全局内存  
	if (!hMem) {
		::CloseClipboard();
		return;
	}

	wchar_t* lpStr = (wchar_t*)::GlobalLock(hMem); //锁住内存区 
	::memcpy(lpStr, str.c_str(), len * sizeof(wchar_t)); //把数据拷贝考全局内存中
	lpStr[len] = wchar_t(0); //字符串末尾设为'\0'
	::GlobalUnlock(hMem); //释放锁 

	::SetClipboardData(CF_UNICODETEXT, hMem); //把内存中的数据放到剪切板上
	::CloseClipboard(); //关闭剪切板	
}

} // namespace ui
