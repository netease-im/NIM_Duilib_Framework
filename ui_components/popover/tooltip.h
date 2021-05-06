#pragma once
namespace nim_comp {
class PopoverBody;

class TooltipBody :public PopoverBody
{
public:
  TooltipBody(const std::wstring& content);
  ~TooltipBody() = default;

  std::wstring GetType() const override;
  virtual ui::CSize EstimateSize(ui::CSize szAvailable) override;
  void DoInit() override;
};

}