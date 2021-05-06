#include "stdafx.h"
#include "popover.h"
#include "alert.h"

namespace nim_comp {

AlertFooter::AlertFooter(const std::wstring& strOk,
  const std::wstring& strCancel) :
  PopoverFooter(strOk, strCancel)
{
  m_pLayout->SetChildMargin(0);
}

std::wstring AlertFooter::GetType() const
{
  return L"AlertFooter";
}

void AlertFooter::DoInit()
{
  if (m_bInited)
    return;

  if (m_strCancel.length()) {
    m_pButtonCancel = new ui::Button();
    m_pButtonCancel->SetName(L"btn_cancel");
    m_pButtonCancel->SetClass(L"popover_btn_cancel alert_btn_cancel");
    m_pButtonCancel->SetText(m_strCancel);
    m_pButtonCancel->AttachClick(ToWeakCallback([this](ui::EventArgs* args) {
      return OnCancel(args);
      }));

    Add(m_pButtonCancel);
  }

  if (m_strOk.length()) {
    m_pButtonOk = new ui::Button();
    m_pButtonOk->SetName(L"btn_ok");
    m_pButtonOk->SetClass(L"popover_btn_ok alert_btn_ok");
    m_pButtonOk->SetText(m_strOk);
    m_pButtonOk->AttachClick(ToWeakCallback([this](ui::EventArgs* args) {
      return OnOk(args);
      }));

    Add(m_pButtonOk);
  }

  m_bInited = true;
}

}