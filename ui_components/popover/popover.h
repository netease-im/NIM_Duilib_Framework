#pragma once

#include <atomic>

namespace nim_comp {

class PopoverArrow;
class PopoverHeader;
class PopoverBody;
class PopoverFooter;
class PopoverRoot;
class Popover;
class PopoverHolderLayer;
class PopoverLayer;


/**
* @brief 弹出框位置和方向，默认指向控件中心点或坐标点；
*        kPlaceInner 和 kPlaceOuter 表示在控件内还是控件外，
*        一般popover和tooltip是控件外，alert和notification是控件内；
*        kPlaceLeftTop等表示弹窗方向
*        Left表示在锚点左侧，Top表示箭头在弹窗偏上且指向锚点。
*/
enum PopoverPlacement {
  kPlaceInner = 1,
  kPlaceOuter = 1 << 1,
  kPlaceLeftTop = 1 << 2,
  kPlaceLeft = 1 << 3,
  kPlaceLeftBottom = 1 << 4,
  kPlaceTopLeft = 1 << 5,
  kPlaceTop = 1 << 6,
  kPlaceTopRight = 1 << 7,
  kPlaceRightTop = 1 << 8,
  kPlaceRight = 1 << 9,
  kPlaceRightBottom = 1 << 10,
  kPlaceBottomLeft = 1 << 11,
  kPlaceBottom = 1 << 12,
  kPlaceBottomRight = 1 << 13,
  kPlaceCenter = 1 << 14
};

/* 弹窗触发方式 */
enum PopoverTriggerType {
  /* 用户自己控制 */
  kTriggerNone = 0,
  /* 锚点获取焦点 */
  kTriggerSetFocus = 1,
  /* 锚点失去焦点 */
  kTriggerKillFocus = 1 << 1,
  /* 点击事件 */
  kTriggerClick = 1 << 2,
  /* 定时 */
  kTriggerTimeout = 1 << 3,
  /* 点击关闭、取消、确认等按钮 */
  kTriggerConfirm = 1 << 4,
  /* 鼠标进入 */
  kTriggerEnter = 1 << 5,
  /* 鼠标停留 */
  kTriggerHover = 1 << 6,
  /* 鼠标离开 */
  kTriggerLeave = 1 << 7,
  /* 鼠标点击非弹窗区域 */
  kTriggerClickLayer = 1 << 8,
  /* 立即展示 */
  kTriggerImmediately = 1 << 9,
  /* 锚点visible属性发生变化时 */
  kTriggerVisible = 1 << 10
};

/* 弹窗图标 */
enum PopoverIconType {
  /* 不显示图标 */
  kIconNone,
  /* 信息 */
  kIconInfo,
  /* 警告 */
  kIconWarning,
  /* 错误 */
  kIconError
};

/* 默认按钮 */
enum PopoverButtonType {
  kButtonNone = 0,
  /* 弹窗头中的关闭按钮 */
  kButtonClose = 1,
  /* 弹窗尾的确认按钮 */
  kButtonOk = 1 << 1,
  /* 弹窗尾的取消按钮 */
  kButtonCancel = 1 << 2
};

/* 默认弹窗结果类型 */
enum PopoverResultType {
  /* 无操作，自动关闭弹窗 */
  kResultNone,
  /* 确认 */
  kResultOk,
  /* 取消 */
  kResultCancel
};

/* 弹窗关闭时回调函数参数类型 */
struct PopoverResult {
  PopoverResultType type;
};

/* 弹窗关闭时回调函数 */
typedef std::function<bool(const PopoverResult&)> PopoverCallback;

class PopoverCallbackSource : public std::vector<PopoverCallback>
{
public:
  PopoverCallbackSource& operator += (const PopoverCallback& callback)
  {
    push_back(callback);
    return *this;
  }

  bool operator() (const PopoverResult& result) const
  {
    for (auto it = this->begin(); it != this->end(); it++) {
      if (!(*it)(result)) return false;
    }
    return true;
  }
};

/* 弹窗箭头 */
class PopoverArrow :public ui::Control
{
public:
  PopoverArrow(int nPlaceMent);
  virtual ~PopoverArrow();

  virtual std::wstring GetType() const override;
  virtual void DoInit() override;

  void SetArrowArea(ui::UiRect rcArea);

  static const int kWidth;
  static const int kHeight;
  static const int kMargin;
private:
  ui::HorAlignType GetHorAlignType();
  ui::VerAlignType GetVerAlignType();

protected:
  ui::UiRect GetMarginByPlacement();

  virtual void CalcTrianglePoints();
  virtual void PaintBkColor(ui::IRenderContext* pRender) override;
protected:
  bool m_bInited;
  int m_nPlacement;
  ui::UiRect m_rcArea;
  ui::CPoint m_ptTriangle[3];
  std::unique_ptr<ui::IPath> m_pPath;
  std::unique_ptr<ui::IPen> m_pPen;
  std::unique_ptr<ui::IBrush> m_pBrush;
};

/* 弹窗头 */
class PopoverHeader :public ui::HBox
{
public:
  PopoverHeader(const std::wstring& strTitle,
    bool bShowClose = false,
    PopoverIconType nIconType = kIconNone);
  virtual ~PopoverHeader();

  // 重载父类
  virtual std::wstring GetType() const override;
  virtual ui::CSize EstimateSize(ui::CSize szAvailable) override;
  virtual void DoInit() override;

  // 更新内容
  virtual void UpdateTitle(const std::wstring& strTitle);

  // 监听关闭按钮点击事件
  void AttachCloseClicked(const ui::EventCallback& callback) { OnClose += callback; }

protected:


protected:
  bool m_bInited;
  bool m_bUseMaxSize;
  ui::CEventSource OnClose;
  std::wstring m_strTitle;
  bool m_bShowClose;
  PopoverIconType m_nIconType;

  ui::Control* m_pControlIcon;
  ui::RichEdit* m_pRichEditTitle;
  ui::Button* m_pButtonClose;
};

/* 弹窗体 */
class PopoverBody :public ui::HBox
{
public:
  PopoverBody(const std::wstring& content, const std::wstring& colorId = L"");
  virtual ~PopoverBody();

  virtual std::wstring GetType() const override;
  virtual ui::CSize EstimateSize(ui::CSize szAvailable) override;
  virtual void DoInit() override;

  virtual void UpdateContent(const std::wstring& strContent);
protected:
  bool m_bInited;
  bool m_bUseMaxSize;
  ui::RichEdit* m_pRichEditContent;
};

/* 弹窗尾 */
class PopoverFooter :public ui::HBox
{
public:
  PopoverFooter(const std::wstring& strOk, const std::wstring& strCancel);
  virtual ~PopoverFooter();

  virtual std::wstring GetType() const override;
  virtual ui::CSize EstimateSize(ui::CSize szAvailable) override;
  virtual void DoInit() override;

  void AttachCancelClicked(const ui::EventCallback& callback) { OnCancel += callback; }
  void AttachOkClicked(const ui::EventCallback& callback) { OnOk += callback; }

protected:
  bool m_bInited;
  bool m_bUseMaxSize;
  std::wstring m_strOk;
  std::wstring m_strCancel;
  ui::CEventSource OnCancel;
  ui::CEventSource OnOk;
  ui::Button* m_pButtonOk;
  ui::Button* m_pButtonCancel;
};

class PopoverRoot :public ui::VBox 
{
public:
  PopoverRoot() {
    // DO NOT DELAY DESTROY
    // Coz AttachLastEvent
    SetDelayedDestroy(false);
  }

  virtual ~PopoverRoot() = default;

  ui::CSize EstimateSize(ui::CSize szAvailable) override;

public:
  std::wstring GetType() const override {
    return L"PopoverRoot";
  }
};

/* 弹窗 */
class Popover :public ui::VBox
{
public:
  /*
  * @brief 弹窗基类构造函数
  * @param[in] pAnchor 锚点控件,为空表示弹窗层居中显示;
  *                    不为空时将根据nShowType自动设置触发器，并在锚点控件被销毁时自动关闭弹窗
  */
  Popover(ui::Control* pAnchor,
    int nPlacement,
    int nShowType,
    int nDisappearType,
    bool bRemoveOnClose,
    int nShowTimeouts,
    int nDisappearTimeouts,
    PopoverHeader* header,
    PopoverBody* body,
    PopoverFooter* footer,
    PopoverArrow* arrow);

  virtual ~Popover();

  // 重载父类
  virtual std::wstring GetType() const override;
  virtual ui::CSize EstimateSize(ui::CSize szAvailable) override;
  virtual void DoInit() override;
  virtual void SetPos(ui::UiRect rc) override;

  ui::Control* GetAnchor() const { return m_pAnchor; }
  PopoverHeader* GetHeader() const { return m_pPopoverHeader; }
  PopoverBody* GetBody() const { return m_pPopoverBody; }
  PopoverFooter* GetFooter() const { return m_pPopoverFooter; }
  PopoverRoot* GetRoot() const { return m_pPopoverRoot; }

  // 设置Tag用以标识同类型弹窗
  void SetTag(const std::wstring& strTag) { m_strTag = strTag; }
  const std::wstring GetTag() const { return m_strTag; }

  // 更新弹窗内容
  virtual void Update(PopoverHeader* header, PopoverBody* body, PopoverFooter* footer);
  virtual void UpdateContent(const std::wstring& content);
  virtual void UpdateNotification(const std::wstring& strTitle,
    const std::wstring& strContent = L"");
  virtual void UpdateTooltip(const std::wstring& tooltip);
  virtual void UpdateShowType(int nType, int nTimeouts = 0);
  virtual void UpdateDisappearType(int nType, int nTimeouts = 0);

  // 监听结果回调
  void BindResultCallback(const PopoverCallback& callback) { m_cbResult += callback; }

  // 监听关闭回调
  void BindCloseCallback(const PopoverCallback& callback) { m_cbClose += callback; }

protected:
  ui::HorAlignType GetHorAlignType();
  ui::VerAlignType GetVerAlignType();
  virtual ui::UiRect ReCalcByAnchorPos(ui::UiRect rc);

protected:
  // 初始化元素
  virtual void InitializeElements();

  // 初始化位置，并绑定锚点事件
  virtual void InitializePosition();

  // 初始化展示和关闭触发器
  bool HasTrigger(int nType, PopoverTriggerType nTrigger) { return nType & nTrigger; }
  virtual void InitializeShowTriggers(int nOldType, int nNewType, int nTimeouts = 0);
  virtual void InitializeDisappearTriggers(int nOldType, int nNewType, int nTimeouts = 0);
  virtual void OnShowTriggerTimeout(int index);
  virtual void OnDisappearTriggerTimeout(int index);

  // 触发关闭弹窗
  virtual void TriggerResult(const PopoverResult& result, bool bForceClose = false);
  virtual void TriggerClose();

  // 锚点事件
  virtual bool OnAnchorResize(ui::EventArgs* args);
  virtual bool OnAnchorSetFocus(ui::EventArgs* args);
  virtual bool OnAnchorKillFocus(ui::EventArgs* args);
  virtual bool OnAnchorMouseClick(ui::EventArgs* args);
  virtual bool OnAnchorMouseEnter(ui::EventArgs* args);
  virtual bool OnAnchorMouseHover(ui::EventArgs* args);
  virtual bool OnAnchorMouseLeave(ui::EventArgs* args);
  virtual bool OnAnchorLastEvent(ui::EventArgs* args);
  virtual bool OnAnchorVisibleChange(ui::EventArgs* args);

  // 定时器

protected:
  std::wstring m_strTag;
  bool m_bInited;
  ui::Control* m_pAnchor;
  int m_nPlacement;
  int m_nShowType;
  int m_nDisappearType;
  bool m_bRemoveOnClose;

  int m_nShowTimeouts;
  int m_nDisappearTimeouts;

  std::atomic<int> m_nShowTriggerIndex;
  std::atomic<int> m_nDisappearTriggerIndex;

  PopoverRoot* m_pPopoverRoot;
  PopoverHeader* m_pPopoverHeader;
  PopoverBody* m_pPopoverBody;
  PopoverFooter* m_pPopoverFooter;
  PopoverArrow* m_pPopoverArrow;

  PopoverCallbackSource m_cbResult;
  PopoverCallbackSource m_cbClose;

  friend class PopoverLayer;
};

/*
* 弹窗容器层
* Popover 和 Alert 类型普通Box即可
* Notification 类型需要有垂直布局或其他特殊布局
*/
class PopoverHolderLayer :public ui::Box
{
public:
  PopoverHolderLayer(std::wstring type,
    ui::Layout* pLayout = new ui::Layout()) :ui::Box(pLayout),
    m_strType(type)
  {
    SetMouseEnabled(false);
    SetKeyboardEnabled(false);

    // DO NOT DELAY DESTROY
    // Coz AttachLastEvent
    SetDelayedDestroy(false);
  }

  virtual ~PopoverHolderLayer()
  {

  }

public:
  static const std::wstring kHolderTypeNotification;
  static const std::wstring kHolderTypePopover;
  static const std::wstring kHolderTypeAlert;

public:
  std::wstring GetType() const override {
    return m_strType;
  }

private:
  std::wstring m_strType;
};



class NotificationHolderLayer :public PopoverHolderLayer
{
public:
  NotificationHolderLayer(ui::Control* pAnchor) :
    PopoverHolderLayer(PopoverHolderLayer::kHolderTypeNotification, new ui::VLayout()),
    m_pAnchor(pAnchor), m_bInited(false)
  {
  }
  
  ~NotificationHolderLayer()
  {

  }

  bool OnAnchorResize(ui::EventArgs* args) {
    ui::UiRect rc = m_pAnchor->GetPos();

    SetFixedWidth(rc.GetWidth());
    SetFixedHeight(rc.GetHeight());

    SetPos(rc);

    return true;
  }

  virtual void SetPos(ui::UiRect rc) override {
    rc = m_pAnchor->GetPos();

    SetFixedWidth(rc.GetWidth());
    SetFixedHeight(rc.GetHeight());

    ui::Control::SetPos(rc);

    ui::CSize requiredSize;
    if (m_items.size() == 0) {
      requiredSize.cx = 0;
      requiredSize.cy = 0;
    }
    else {
      requiredSize = m_pLayout->ArrangeChild(m_items, rc);
    }
  }

  void DoInit() override {
    if (m_bInited)
      return;

    ASSERT(m_pAnchor);

    m_pAnchor->AttachResize(nbase::Bind(&NotificationHolderLayer::OnAnchorResize, this, std::placeholders::_1));

    m_bInited = true;
  }

private:
  ui::Control* m_pAnchor;
  bool m_bInited;
};

/**
* 弹窗层
* 需要确保弹窗层能够接收到鼠标键盘事件
* 使用时需要将弹窗层挂载到root节点，且保证层级为最上层
*/
class PopoverLayer :public ui::IUIMessageFilter, public ui::Box {
public:
  PopoverLayer();
  ~PopoverLayer();

  /*
  * 位于弹窗层最顶层
  */
  void ShowAlert(Popover* popover);

  /*
  * 位于弹窗层中间层
  */
  void ShowPopover(Popover* popover);

  /*
  * 位于弹窗层中间层
  */
  void ShowTooltip(Popover* popover);

  /*
  * 位于弹窗层最底层
  */
  void ShowNotification(Popover* popover);

  /**
  * @brief 用默认样式创建弹出气泡，默认阴影白底黑字, 且带有箭头
  * @param[in] pAnchor 锚点控件,为空表示弹窗层居中显示
  * @param[in] strTitle 标题，不需要则为空
  * @param[in] strOk 确认按钮文字，默认使用 STRING_OK
  * @param[in] strCancel 取消按钮文字，默认使用 STRING_CANCEL
  * @param[in] strContent 内容，不需要则为空
  * @param[in] nPlacement 弹出方向，默认为锚点控件外和锚点控件上
  * @param[in] nButtons 按钮类型组合，kButtonOk | kButtonCancel 表示显示确认和取消按钮
  * @param[in] nShowType 自动显示的触发方式，kTriggerNone表示不自动触发
  * @param[in] nDisappearType 自动关闭的触发方式，kTriggerNone表示不自动触发
  * @param[in] bRemoveOnClose 关闭后是否删除
  * @param[in] pCustomHeader 自定义PopoverHeader
  * @param[in] pCustomBody 自定义PopoverBody
  * @param[in] pCustomFooter 自定义PopoverFooter
  */
  static Popover* CreatePopover(ui::Control* pAnchor,
    const std::wstring& strTitle,
    const std::wstring& strContent,
    const std::wstring& strOk = L"",
    const std::wstring& strCancel = L"",
    int nPlacement = kPlaceOuter | kPlaceTop,
    int nButtons = kButtonOk | kButtonCancel,
    int nShowType = kTriggerClick,
    int nDisappearType = kTriggerConfirm | kTriggerClickLayer | kTriggerKillFocus,
    bool bShowArrow = true,
    bool bRemoveOnClose = false,
    int nShowTimeouts = 0,
    int nDisappearTimeouts = 0,
    PopoverHeader* pCustomHeader = nullptr,
    PopoverBody* pCustomBody = nullptr,
    PopoverFooter* pCustomFooter = nullptr,
    PopoverArrow* pCustomArrow = nullptr);

  /**
  * @brief 用默认样式创建弹出气泡，默认阴影白底黑字, 且带有箭头
  * @param[in] pAnchor 锚点控件,为空表示弹窗层居中显示
  * @param[in] strTitle 标题，不需要则为空
  * @param[in] strOk 确认按钮文字，默认使用 STRING_OK
  * @param[in] strCancel 取消按钮文字，默认使用 STRING_CANCEL
  * @param[in] strContent 内容，不需要则为空
  * @param[in] nPlacement 弹出方向，默认为锚点控件外和锚点控件上
  * @param[in] nButtons 按钮类型组合，kButtonOk | kButtonClose 表示显示确认和关闭按钮
  * @param[in] nShowType 自动显示的触发方式，kTriggerNone表示不自动触发
  * @param[in] nDisappearType 自动关闭的触发方式，kTriggerNone表示不自动触发
  * @param[in] bRemoveOnClose 关闭后是否删除
  * @param[in] pCustomHeader 自定义PopoverHeader
  * @param[in] pCustomBody 自定义PopoverBody
  * @param[in] pCustomFooter 自定义PopoverFooter
  */
  static Popover* CreateAlert(ui::Control* pAnchor,
    const std::wstring& strTitle,
    const std::wstring& strContent,
    const std::wstring& strOk = L"",
    const std::wstring& strCancel = L"",
    int nPlacement = kPlaceInner | kPlaceCenter,
    int nButtons = kButtonOk | kButtonClose,
    int nShowType = kTriggerImmediately,
    int nDisappearType = kTriggerConfirm,
    bool bRemoveOnClose = true,
    PopoverHeader* pCustomHeader = nullptr,
    PopoverBody* pCustomBody = nullptr,
    PopoverFooter* pCustomFooter = nullptr);

  /**
  * @brief 用默认样式创建弹出气泡，默认阴影白底黑字, 且带有箭头
  * @param[in] pAnchor 锚点控件,为空表示弹窗层居中显示
  * @param[in] strContent 标题，不需要则为空
  * @param[in] nPlacement 弹出方向，默认为锚点控件外和锚点控件上
  * @param[in] nButtons 按钮类型组合，仅支持 kButtonClose 和 kButtonNone
  * @param[in] nShowType 自动显示的触发方式，kTriggerNone表示不自动触发
  * @param[in] nDisappearType 自动关闭的触发方式，kTriggerNone表示不自动触发
  * @param[in] bRemoveOnClose 关闭后是否删除
  * @param[in] pCustomHeader 自定义PopoverHeader
  * @param[in] pCustomBody 自定义PopoverBody
  */
  static Popover* CreateNotification(ui::Control* pAnchor,
    const std::wstring& strTitle,
    const std::wstring& strContent,
    PopoverIconType nIconType = kIconInfo,
    int nPlacement = kPlaceInner | kPlaceTopRight,
    int nButtons = kButtonNone,
    int nShowType = kTriggerImmediately,
    int nDisappearType = kTriggerConfirm,
    bool bRemoveOnClose = true,
    int nShowTimeouts = 0,
    int nDisappearTimeouts = 0,
    PopoverHeader* pCustomHeader = nullptr,
    PopoverBody* pCustomBody = nullptr);

  /**
  * @brief 用默认样式创建弹出气泡，默认阴影白底黑字, 且带有箭头
  * @param[in] pAnchor 锚点控件,为空表示弹窗层居中显示
  * @param[in] strContent 内容，不需要则为空
  * @param[in] nPlacement 弹出方向，默认为锚点控件外和锚点控件上
  * @param[in] pCustomBody 自定义PopoverBody
  */
  static Popover* CreateToolltip(ui::Control* pAnchor,
    const std::wstring& strContent,
    int nPlacement = kPlaceOuter | kPlaceTop,
    int nShowType = kTriggerHover,
    int nDisappearType = kTriggerLeave,
    PopoverBody* pCustomBody = nullptr);

public:
  // 重载
  bool Remove(Control* pControl) override;

  // 清除所有弹窗，保留弹窗Holder
  void ClearAll();

private:
  // 重载
  std::wstring GetType() const;
  void DoInit() override;
  LRESULT MessageHandler(UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    BOOL& bHandled) override;

  /**
  * 设置是否显示半透明遮罩
  */
  void SetShowMask(bool show);

  /**
  * 是否显示半透明遮罩
  */
  void IsShowMask() const { m_bShowMask; }

  /*
  * @brief 窗体鼠标左键单击事件，会关闭所有nDisappearType
                   为kTriggerClickLayer的弹窗
  * @param[in] pt 鼠标点击坐标
  */
  void OnMouseEventButtonDown(POINT pt);

private:
  bool m_bInited;
  bool m_bShowMask;
  bool m_bDisableWindow;
  int alert_count;

  // should be base of VBox
  std::map<ui::Control*, NotificationHolderLayer*> m_NotifyLayerMap;
  PopoverHolderLayer* m_pPopoverLayer;
  PopoverHolderLayer* m_pAlertLayer;
};
}