#include "stdafx.h"
#include "popover.h"
#include "notification.h"

namespace nim_comp {

NotificationHeader::NotificationHeader(const std::wstring& strTitle,
  bool bShowClose,
  PopoverIconType nIconType) :
  PopoverHeader(strTitle, bShowClose, nIconType)
{
  m_bUseMaxSize = true;

  if (m_pRichEditTitle)
    m_pRichEditTitle->SetClass(L"popover_label notification_label_title");
}

NotificationHeader::~NotificationHeader()
{
}

NotificationBody::NotificationBody(const std::wstring& strContent)
  :PopoverBody(strContent)
{
  m_bUseMaxSize = true;
}

NotificationBody::~NotificationBody()
{
}

Notification::Notification(ui::Control* pAnchor,
  int nPlacement,
  int nShowType,
  int nDisappearType,
  bool bRemoveOnClose,
  int nShowTimeouts,
  int nDisappearTimeouts,
  PopoverHeader* header,
  PopoverBody* body) :
  Popover(pAnchor,
    nPlacement,
    nShowType,
    nDisappearType,
    bRemoveOnClose,
    nShowTimeouts,
    nDisappearTimeouts,
    header,
    body,
    nullptr,
    nullptr)
{
  
}

Notification::~Notification()
{
}

void Notification::SetPos(ui::UiRect rc)
{
  auto size = EstimateSize({ 0,0 });

  if (rc.GetWidth() < size.cx) {
    rc.left = rc.right - size.cx;
    rc.right = rc.left + size.cx;
  }

  ui::VBox::SetPos(rc);
}

void Notification::InitializePosition()
{
  SetFloat(false);
  SetHorAlignType(GetHorAlignType());
  SetVerAlignType(GetVerAlignType());
}



}