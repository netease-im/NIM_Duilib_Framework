#pragma once

namespace ui {

typedef std::function<void(int nStartIndex, int nEndIndex)> DataChangedNotify;
typedef std::function<void()> CountChangedNotify;

class UILIB_API VirtualTileInterface : public nbase::SupportWeakCallback
{
public:
  VirtualTileInterface();
  /**
  * @brief 创建一个子项
  * @return 返回创建后的子项指针
  */
  virtual ui::Control* CreateElement() = 0;

  /**
  * @brief 填充指定子项
  * @param[in] control 子项控件指针
  * @param[in] index 索引
  * @return 返回创建后的子项指针
  */
  virtual void FillElement(ui::Control* control, int index) = 0;

  /**
  * @brief 获取子项总数
  * @return 返回子项总数
  */
  virtual int GetElementtCount() = 0;

  void RegNotifys(const DataChangedNotify& dcNotify, const CountChangedNotify& ccNotify);

protected:
  void EmitDataChanged(int nStartIndex, int nEndIndex);
  void EmitCountChanged();

private:
  DataChangedNotify m_DataChangedNotify;
  CountChangedNotify m_CountChangedNotify;
};

class UILIB_API VirtualTileLayout : public ui::TileLayout
{
public:
  VirtualTileLayout();
  virtual ui::CSize ArrangeChild(const std::vector<ui::Control*>& items, ui::UiRect rc) override;
  virtual ui::CSize AjustSizeByChild(const std::vector<ui::Control*>& items, ui::CSize szAvailable) override;
  virtual bool SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;
  virtual int GetElementsHeight(int nCount);
  virtual void LazyArrangeChild();
  virtual int AjustMaxItem();

private:
  bool m_bAutoCalcColumn;
};

class UILIB_API VirtualTileBox : public ui::ListBox
{
  friend class VirtualTileLayout;
public:
  VirtualTileBox(ui::Layout* pLayout = new VirtualTileLayout);

  /**
  * @brief 设置数据代理对象
  * @param[in] pProvider 开发者需要重写 VirtualListInterface 的接口来作为数据代理对象
  * @return 无
  */
  virtual void SetDataProvider(VirtualTileInterface* pProvider);

  virtual VirtualTileInterface* GetDataProvider();

  /**
  * @brief 刷新列表
  * @return 无
  */
  virtual void Refresh();

  /**
  * @brief 删除所有子项
  * @return 无
  */
  virtual void RemoveAll() override;

  /**
  * @brief 设置是否强制重新布局
  * @param[in] bForce 设置为 true 为强制，否则为不强制
  * @return 无
  */
  void SetForceArrange(bool bForce);

  /**
  * @brief 获取当前所有可见控件的索引
  * @param[out] collection 索引列表
  * @return 无
  */
  void GetDisplayCollection(std::vector<int>& collection);

  /**
  * @brief 让控件在可见范围内
  * @param[in] iIndex 控件索引
  * @param[in] bToTop 是否在最上方
  * @return 无
  */
  void EnsureVisible(int iIndex, bool bToTop = false);

  /**
  * @brief 获取列表内最大真实控件数量
  * @return 最大真实控件数量
  */
  int GetMaxItemCount() { return m_nMaxItemCount; }

protected:
  /// 重写父类接口，提供个性化功能
  virtual void SetScrollPos(ui::CSize szPos) override;
  virtual void HandleMessage(ui::EventArgs& event) override;
  virtual void SetPos(ui::UiRect rc) override;

  /**
  * @brief 重新布局子项
  * @param[in] bForce 是否强制重新布局
  * @return 无
  */
  void ReArrangeChild(bool bForce);

  /**
  * @brief 键盘按下通知
  * @param[in] ch 按键
  * @return 无
  */
  virtual void OnKeyDown(TCHAR ch) {}

  /**
  * @brief 键盘弹起通知
  * @param[in] ch 按键
  * @return 无
  */
  virtual void OnKeyUp(TCHAR ch) {}

private:
  enum ScrollDirection
  {
    kScrollUp = -1,
    kScrollDown = 1
  };

  /**
  * @brief 创建一个子项
  * @return 返回创建后的子项指针
  */
  Control* CreateElement();

  /**
  * @brief 填充指定子项
  * @param[in] control 子项控件指针
  * @param[in] index 索引
  * @return 返回创建后的子项指针
  */
  void FillElement(Control* pControl, int iIndex);

  /**
  * @brief 获取元素总数
  * @return 返回元素总指数
  */
  int GetElementCount();

  /**
  * @brief 得到n个元素对应的高度和，
  * @param[in] nCount 要得到多少元素的高度，-1表示全部元素
  * @return 返回指定数量元素的高度和
  */
  int CalcElementsHeight(int nCount);

  /**
  * @brief 得到可见范围内第一个元素的前一个元素索引
  * @param[out] bottom 返回上一个元素的 bottom 值
  * @return 返回上一个元素的索引
  */
  int GetTopElementIndex(int& bottom);

  /**
  * @brief 判断某个元素是否在可见范围内
  * @param[in] iIndex 元素索引
  * @return 返回 true 表示可见，否则为不可见
  */
  bool IsElementDisplay(int iIndex);

  /**
  * @brief 判断是否要重新布局
  * @param[out] direction 向上滚动还是向下滚动
  * @return true 为需要重新布局，否则为 false
  */
  bool NeedReArrange(ScrollDirection& direction);

  VirtualTileLayout* GetTileLayout();

  int GetRealElementHeight();

  int GetColumns();

  void LazyArrangeChild();

  void OnModelDataChanged(int nStartIndex, int nEndIndex);

  void OnModelCountChanged();

  int ElementIndexToItemIndex(int nElementIndex);

  int ItemIndexToElementIndex(int nItemIndex);

private:
  VirtualTileInterface* m_pDataProvider;
  int m_nMaxItemCount;	// 列表真实控件数量上限  
  int m_nOldYScrollPos;
  bool m_bArrangedOnce;
  bool m_bForceArrange;	// 强制布局标记
};

}
