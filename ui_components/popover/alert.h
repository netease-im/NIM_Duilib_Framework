#pragma once
namespace nim_comp {
class PopoverFooter;

class AlertFooter :public PopoverFooter
{
public:
  AlertFooter(const std::wstring& strOk, const std::wstring& strCancel);
  ~AlertFooter() = default;

  std::wstring GetType() const override;
  void DoInit() override;
};

}