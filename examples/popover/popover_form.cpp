#include "framework.h"
#include "popover_form.h"

const std::wstring PopoverForm::kClassName = L"Popover";

PopoverForm::PopoverForm() :
  m_pPopoverLayer(nullptr)
{
}


PopoverForm::~PopoverForm()
{
}

std::wstring PopoverForm::GetSkinFolder()
{
  return L"popover";
}

std::wstring PopoverForm::GetSkinFile()
{
  return L"popover.xml";
}

std::wstring PopoverForm::GetWindowClassName() const
{
  return kClassName;
}

ui::Control* PopoverForm::CreateControl(const std::wstring& pstrClass)
{
  ui::Control* control = nullptr;
  if (pstrClass == L"PopoverLayer")
  {
    control = new nim_comp::PopoverLayer();
  }

  return control;
}

void PopoverForm::InitWindow()
{
  m_pPopoverLayer = static_cast<nim_comp::PopoverLayer*>(FindControl(L"popover_layer"));

  btn_updatetitle = static_cast<ui::Button*>(FindControl(L"btn_updatetitle"));
  btn_updatecontent = static_cast<ui::Button*>(FindControl(L"btn_updatecontent"));
  btn_tl = static_cast<ui::Button*>(FindControl(L"btn_tl"));
  btn_top = static_cast<ui::Button*>(FindControl(L"btn_top"));
  btn_tr = static_cast<ui::Button*>(FindControl(L"btn_tr"));
  btn_lt = static_cast<ui::Button*>(FindControl(L"btn_lt"));
  btn_rt = static_cast<ui::Button*>(FindControl(L"btn_rt"));
  btn_left = static_cast<ui::Button*>(FindControl(L"btn_left"));
  btn_right = static_cast<ui::Button*>(FindControl(L"btn_right"));
  btn_lb = static_cast<ui::Button*>(FindControl(L"btn_lb"));
  btn_rb = static_cast<ui::Button*>(FindControl(L"btn_rb"));
  btn_bl = static_cast<ui::Button*>(FindControl(L"btn_bl"));
  btn_bottom = static_cast<ui::Button*>(FindControl(L"btn_bottom"));
  btn_br = static_cast<ui::Button*>(FindControl(L"btn_br"));


  TestAllPlacement();
  TestOthers();
}

LRESULT PopoverForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  PostQuitMessage(0L);
  return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void PopoverForm::TestAllPlacement()
{
  struct PLACE_TEST_BUTTONS {
    ui::Button* anchor;
    nim_comp::PopoverPlacement placement;
    std::wstring content;
  };

  PLACE_TEST_BUTTONS buttons[] = {
    {btn_tl,nim_comp::kPlaceTopLeft,L"TopLeft"},
    {btn_top,nim_comp::kPlaceTop,L"Top"},
    {btn_tr,nim_comp::kPlaceTopRight,L"TopRight"},
    {btn_lt,nim_comp::kPlaceLeftTop,L"LeftTop"},
    {btn_rt,nim_comp::kPlaceRightTop,L"RightTop"},
    {btn_left,nim_comp::kPlaceLeft,L"Left"},
    {btn_right,nim_comp::kPlaceRight,L"Right"},
    {btn_lb,nim_comp::kPlaceLeftBottom,L"LeftBottom"},
    {btn_rb,nim_comp::kPlaceRightBottom,L"RightBottom"},
    {btn_bl,nim_comp::kPlaceBottomLeft,L"BottomLeft"},
    {btn_bottom,nim_comp::kPlaceBottom,L"Bottom"},
    {btn_br,nim_comp::kPlaceBottomRight,L"BottomRight"}
  };

  auto create_popover = [this](PLACE_TEST_BUTTONS button) {
    auto popover = m_pPopoverLayer->CreatePopover(button.anchor,
      button.content,
      L"button.content",
      L"",
      L"",
      nim_comp::kPlaceOuter | button.placement,
      nim_comp::kButtonOk | nim_comp::kButtonCancel | nim_comp::kButtonClose,
      nim_comp::kTriggerClick,
      nim_comp::kTriggerClickLayer | nim_comp::kTriggerConfirm);

    popover->BindResultCallback([this, button](const nim_comp::PopoverResult& result) {
      if (result.type == nim_comp::kResultOk) {
        OutputDebugString(button.content.c_str());
      }
      else {

      }
      return true;
      });

    return popover;
  };

  auto create_alert = [this](PLACE_TEST_BUTTONS button) {
    auto popover = m_pPopoverLayer->CreateAlert(button.anchor,
      button.content,
      L"",
      L"",
      L"",
      nim_comp::kPlaceOuter | button.placement,
      nim_comp::kButtonOk | nim_comp::kButtonCancel | nim_comp::kButtonClose,
      nim_comp::kTriggerClick,
      nim_comp::kTriggerConfirm);

    popover->BindResultCallback([this, button](const nim_comp::PopoverResult& result) {
      if (result.type == nim_comp::kResultOk) {
        OutputDebugString(button.content.c_str());
      }
      else {

      }
      return true;
      });

    return popover;
  };

  auto create_notification = [this](PLACE_TEST_BUTTONS button) {
    auto popover = m_pPopoverLayer->CreateNotification(button.anchor,
      button.content,
      L"",
      nim_comp::kIconInfo, nim_comp::kPlaceOuter | button.placement);

    popover->BindResultCallback([this, button](const nim_comp::PopoverResult& result) {
      if (result.type == nim_comp::kResultOk) {
        OutputDebugString(button.content.c_str());
      }
      else {

      }
      return true;
      });

    return popover;
  };

  auto create_tooltip = [this](PLACE_TEST_BUTTONS button) {
    auto popover = m_pPopoverLayer->CreateToolltip(button.anchor,
      button.content,
      nim_comp::kPlaceOuter | button.placement);

    popover->BindResultCallback([this, button](const nim_comp::PopoverResult& result) {
      if (result.type == nim_comp::kResultOk) {
        OutputDebugString(button.content.c_str());
      }
      else {

      }
      return true;
      });

    return popover;
  };

  enum {
    kTestPopover,
    kTestAlert,
    kTestNotification,
    kTestTooltip
  }testMode = kTestAlert;

  for (int i = 0; i < sizeof(buttons) / sizeof(PLACE_TEST_BUTTONS); i++)
  {
    if (testMode == kTestPopover)
      m_pPopoverLayer->ShowPopover(create_popover(buttons[i]));
    else if (testMode == kTestAlert)
      m_pPopoverLayer->ShowAlert(create_alert(buttons[i]));
    else if (testMode == kTestNotification)
      m_pPopoverLayer->ShowNotification(create_notification(buttons[i]));
    else if (testMode == kTestTooltip)
      m_pPopoverLayer->ShowTooltip(create_tooltip(buttons[i]));
  }


}

void PopoverForm::TestOthers()
{
  {
    auto popover = m_pPopoverLayer->CreateNotification(nullptr,
      L"Title",
      L"", nim_comp::kIconNone, nim_comp::kPlaceOuter | nim_comp::kPlaceRight);

    m_pPopoverLayer->ShowNotification(popover);

    btn_updatetitle->AttachClick(ToWeakCallback([this, popover](ui::EventArgs* args) {
      auto header = popover->GetHeader();
      if (header)
        header->UpdateTitle(L"Title Updated");

      return true;
      }));
  }

  {
    auto popover = m_pPopoverLayer->CreateToolltip(btn_updatecontent,
      L"Content",
      nim_comp::kPlaceOuter | nim_comp::kPlaceBottom);

    m_pPopoverLayer->ShowTooltip(popover);


    btn_updatecontent->AttachClick(ToWeakCallback([this, popover](ui::EventArgs* args) {
      auto body = popover->GetBody();
      if (body)
        body->UpdateContent(L"Content Updated");

      return true;
      }));
  }

}
