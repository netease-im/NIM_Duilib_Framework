#include "stdafx.h"
#include "DateTime.h"
#include <windowsx.h>
#include <OleCtl.h>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace ui
{
#define NIM_DTS_NONE   0
#define NIM_DTS_UPDATE 1
#define NIM_DTS_DELETE 2
#define DT_KEEP   3

    class DateTimeWnd : public Window
    {

    public:
        DateTimeWnd();
        void InitDateTime(DateTime* pOwner);

        virtual std::wstring GetWindowClassName() const override;
        virtual std::wstring GetSuperClassName() const override;
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    protected:
        virtual void OnFinalMessage(HWND hWnd) override;


    protected:
        DateTime* m_pOwner;
        bool m_bInit;
        bool m_bDropOpen;
        SYSTEMTIME m_oldSysTime;
    };

    

    DateTimeWnd::DateTimeWnd()
        : m_pOwner(nullptr)
        , m_bInit(false)
        , m_bDropOpen(false)
    {

    }

    void DateTimeWnd::InitDateTime(DateTime* pOwner)
    {
        m_pOwner = pOwner;
        m_pOwner->m_nDTUpdateFlag = NIM_DTS_NONE;

        if (m_hWnd == NULL)
        {
            Create(m_pOwner->m_pWindow->GetHWND(), NULL, WS_CHILD, 0, false, m_pOwner->GetPos());
            SetWindowFont(m_hWnd, GlobalManager::GetFont(m_pOwner->GetFont()), TRUE);
        }

        if (m_pOwner->GetText().empty())
        {
            ::GetLocalTime(&m_pOwner->m_sysTime);
        }

        memcpy(&m_oldSysTime, &m_pOwner->m_sysTime, sizeof(SYSTEMTIME));
        DateTime_SetSystemtime(m_hWnd, 0, &m_pOwner->m_sysTime);
        DateTime_SetFormat(m_hWnd, m_pOwner->m_sDateTimeFormat.c_str());
        ::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
        ::SetFocus(m_hWnd);

        m_bInit = true;

    }

    std::wstring DateTimeWnd::GetWindowClassName() const
    {
        return L"NimDateTimeWnd";
    }

    std::wstring DateTimeWnd::GetSuperClassName() const
    {
        return DATETIMEPICK_CLASS;
    }

    LRESULT DateTimeWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        if (uMsg == WM_CREATE) {
            bHandled = FALSE;
        }
        else if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE)
        {
            memcpy(&m_pOwner->m_sysTime, &m_oldSysTime, sizeof(SYSTEMTIME));
            m_pOwner->m_nDTUpdateFlag = NIM_DTS_UPDATE;
            m_pOwner->UpdateText();
            PostMessage(WM_CLOSE);
            return lRes;
        }
        else if (uMsg == OCM_NOTIFY)
        {
            NMHDR* pHeader = (NMHDR*)lParam;
            if (pHeader != NULL && pHeader->hwndFrom == m_hWnd) {
                if (pHeader->code == DTN_DATETIMECHANGE) {
                    DateTime_GetSystemtime(m_hWnd, &m_pOwner->m_sysTime);
                    m_pOwner->m_nDTUpdateFlag = NIM_DTS_UPDATE;
                    m_pOwner->UpdateText();
                }
                else if (pHeader->code == DTN_DROPDOWN) {
                    m_bDropOpen = true;

                }
                else if (pHeader->code == DTN_CLOSEUP) {
                    DateTime_GetSystemtime(m_hWnd, &m_pOwner->m_sysTime);
                    m_pOwner->m_nDTUpdateFlag = NIM_DTS_UPDATE;
                    m_pOwner->UpdateText();
                    PostMessage(WM_CLOSE);
                    m_bDropOpen = false;
                }
            }
            bHandled = FALSE;
        }
        else if (uMsg == WM_KILLFOCUS)
        {
            if (!m_bDropOpen) {
                PostMessage(WM_CLOSE);
            }
            bHandled = FALSE;
        }
        else if (uMsg == WM_PAINT) {
            bHandled = FALSE;
        }
        else bHandled = FALSE;
        if (!bHandled) return this->CallWindowProc(uMsg, wParam, lParam);
        return lRes;
    }

    void DateTimeWnd::OnFinalMessage(HWND hWnd)
    {
        __super::OnFinalMessage(hWnd);
        m_pOwner->m_pDateTimeWnd = nullptr;
        delete this;
    }


    DateTime::DateTime()
    {

        m_sFormat = L"%Y-%m-%d";
        m_sDateTimeFormat = L"yyyy-MM-dd";

        SetAttribute(L"bordersize", L"1");
        SetAttribute(L"bordercolor", L"gray");
        SetAttribute(L"align", L"vcenter");

        ::GetLocalTime(&m_sysTime);
        m_pDateTimeWnd = nullptr;
        m_nDTUpdateFlag = NIM_DTS_UPDATE;
        UpdateText();
        m_nDTUpdateFlag = NIM_DTS_NONE;
    }

    const SYSTEMTIME& DateTime::GetTime() const
    {
        return m_sysTime;
    }

    void DateTime::SetTime(const SYSTEMTIME& val)
    {
        m_sysTime = val;
        Invalidate();
        m_nDTUpdateFlag = NIM_DTS_UPDATE;
        UpdateText();
        m_nDTUpdateFlag = NIM_DTS_NONE;
    }

    void DateTime::UpdateText()
    {
        if (m_nDTUpdateFlag == NIM_DTS_DELETE)
        {
            SetText(L"");
        }
        else if (m_nDTUpdateFlag == NIM_DTS_UPDATE)
        {
            std::tm tmSystemDate;
            tmSystemDate.tm_sec = m_sysTime.wSecond;
            tmSystemDate.tm_min = m_sysTime.wMinute;
            tmSystemDate.tm_hour = m_sysTime.wHour;
            tmSystemDate.tm_mday = m_sysTime.wDay;
            tmSystemDate.tm_mon = m_sysTime.wMonth - 1;
            tmSystemDate.tm_year = m_sysTime.wYear - 1900;
            tmSystemDate.tm_isdst = -1;

            std::stringstream ss;
            ss << std::put_time(&tmSystemDate, nbase::UTF16ToUTF8(m_sFormat).c_str());
            SetText(nbase::UTF8ToUTF16(ss.str()));
        }
    }


    const std::wstring& DateTime::GetFormat() const
    {
        return m_sFormat;
    }

    void DateTime::SetFormat(std::wstring val)
    {
        if (m_sFormat == val) return;
        m_sFormat = val;
        Invalidate();
        m_nDTUpdateFlag = NIM_DTS_UPDATE;
        UpdateText();
        m_nDTUpdateFlag = NIM_DTS_NONE;
    }

    const std::wstring& DateTime::GetDateTimeFormat() const
    {
        return m_sDateTimeFormat;
    }

    void DateTime::SetDateTimeFormat(std::wstring val)
    {
        m_sDateTimeFormat = val;
    }

    void DateTime::HandleMessage(EventArgs& event)
    {
        if (!IsMouseEnabled() && event.Type > kEventMouseBegin && event.Type < kEventMouseEnd) {
            if (m_pParent != NULL) m_pParent->HandleMessageTemplate(event);
            else __super::HandleMessage(event);
            return;
        }

        if (event.Type == kEventSetCursor && IsEnabled())
        {
            ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
            return;
        }

        if (event.Type == kEventInternalSetFocus && IsEnabled())
        {
            if (m_pDateTimeWnd) return;
            m_pDateTimeWnd = new DateTimeWnd();
            ASSERT(m_pDateTimeWnd);
            m_pDateTimeWnd->InitDateTime(this);
            m_pDateTimeWnd->ShowWindow();
        }
        if (event.Type == kEventInternalKillFocus && IsEnabled())
        {
            Invalidate();
        }

        if (event.Type == kEventMouseButtonDown || event.Type == kEventClick)
        {
            if (IsEnabled())
            {
                m_pWindow->ReleaseCapture();
                if (IsFocused() && m_pDateTimeWnd == nullptr)
                {
                    m_pDateTimeWnd = new DateTimeWnd();
                    ASSERT(m_pDateTimeWnd);
                }
                if (m_pDateTimeWnd != nullptr)
                {
                    m_pDateTimeWnd->InitDateTime(this);
                    m_pDateTimeWnd->ShowWindow();
                }
            }
            return;
        }

        if (event.Type == kEventMouseMove)
        {
            return;
        }
        if (event.Type == kEventMouseButtonUp)
        {
            return;
        }
        if (event.Type == kEventMouseEnter)
        {
            return;
        }
        if (event.Type == kEventMouseLeave)
        {
            return;
        }

        __super::HandleMessage(event);
    }

    void DateTime::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
    {
        if (strName == L"format") SetFormat(strValue);
        else if (strName == L"datetimeformat") SetDateTimeFormat(strValue);
        else __super::SetAttribute(strName, strValue);
    }

}

