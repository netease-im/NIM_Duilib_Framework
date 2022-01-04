#ifndef UI_CONTROL_TREEVIEW_H_
#define UI_CONTROL_TREEVIEW_H_

#pragma once

namespace ui
{

#define ROOT_NODE_DEPTH  -1

class TreeView;
class UILIB_API TreeNode : public ListContainerElement
{
public:
	TreeNode();
	TreeNode(const TreeNode& r) = delete;
	TreeNode& operator=(const TreeNode& r) = delete;

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual std::wstring GetType() const override;
	virtual UIAControlProvider* GetUIAProvider() override;
	virtual bool IsVisible() const override;
	virtual void SetInternVisible(bool bVisible) override;
	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;

	/**
	 * @brief 子项被点击时触发
	 * @param[in] param 消息体
	 * @return 始终返回 true
	 */
	virtual bool OnClickItem(EventArgs* param);

	/**
	 * @brief 设置子项所属的属性列表容器
	 * @param[in] pTreeView 容器指针
	 * @return 无
	 */
	void SetTreeView(TreeView* pTreeView);

	/**
	 * @brief 获取父节点指针
	 * @return 返回父节点指针
	 */
	TreeNode* GetParentNode();

	/**
	 * @brief 设置父节点
	 * @param[in] pParentTreeNode 父节点指针
	 * @return 无
	 */
	void SetParentNode(TreeNode* pParentTreeNode);

	/**
	 * @brief 添加子节点
	 * @param[in] pTreeNode 子节点指针
	 * @return 成功返回 true，失败返回 false
	 */
	bool AddChildNode(TreeNode* pTreeNode);

	/**
	 * @brief 在指定位置添加子节点
	 * @param[in] pTreeNode 子节点指针
	 * @param[in] iIndex 要插入哪个位置之后
	 * @return 成功返回 true，失败返回 false
	 */
	bool AddChildNodeAt(TreeNode* pTreeNode, std::size_t iIndex);

	/**
	 * @brief 从指定位置移除一个子节点
	 * @param[in] index 要移除的子节点索引
	 * @return 成功返回 true，失败返回 false
	 */
	bool RemoveChildNodeAt(std::size_t index);

	/**
	 * @brief 根据子节点指针移除一个子节点
	 * @param[in] pTreeNode 子节点指针
	 * @return 成功返回 true，失败返回 false
	 */
	bool RemoveChildNode(TreeNode* pTreeNode);

	/**
	 * @brief 移除所有子节点
	 * @return 无
	 */
	void RemoveAllChildNode();

	/**
	 * @brief 递归获取所有子节点个数
	 * @return 返回所有子节点个数
	 */
	int GetDescendantNodeCount();

	/**
	 * @brief 获取下一级子节点个数
	 * @return 返回子节点个数
	 */
	std::size_t GetChildNodeCount();

	/**
	 * @brief 获取一个子节点指针
	 * @param[in] iIndex 要获取的子节点索引
	 * @return 返回子节点指针
	 */
	TreeNode* GetChildNode(std::size_t iIndex);

	/**
	 * @brief 根据子节点指针获取子节点位置索引
	 * @param[in] pTreeNode 子节点指针
	 * @return 返回位置索引
	 */
	int	GetChildNodeIndex(TreeNode* pTreeNode);

	/**
	 * @brief 判断是否展开状态
	 * @return 返回 true 为展开状态，否则为 false
	 */
	bool IsExpand() const; 

	/**
	 * @brief 设置是否展开显示子节点
	 * @param[in] bExpand 为 true 时展开，为 false 是不展开
	 * @return 无
	 */
	void SetExpand(bool bExpand, bool bTriggerEvent = false);

	/**
	 * @brief 获取子项层级
	 * @return 返回当前层级
	 */
	int GetDepth();

	/**
	 * @brief 监听子项展开事件
	 * @param[in] callback 子项展开时触发的回调函数
	 * @return 无
	 */
	void AttachExpand(const EventCallback& callback) { OnEvent[kEventExpand] += callback; }

	/**
	 * @brief 监听子项收缩事件
	 * @param[in] callback 子项收缩时触发的回调函数
	 * @return 无
	 */
	void AttachUnExpand(const EventCallback& callback) { OnEvent[kEventUnExpand] += callback; }

private:
	/**
	 * @brief 删除自身
	 * @return 成功返回 true，失败返回 false
	 */
	bool RemoveSelf();

private:
	int m_iDepth;
	bool m_bExpand;
	TreeView *m_pTreeView;
	TreeNode *m_pParentTreeNode;
	std::vector<TreeNode*> m_aTreeNodes;
};

class UILIB_API TreeView : public ListBox
{
public:
	TreeView(void);

	/// 重写父类方法，提供个性化功能，请参考父类声明
	virtual std::wstring GetType() const override;
	virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

	/**
	 * @brief 获取根节点
	 * @return 返回根节点指针
	 */
	TreeNode* GetRootNode()	{ return m_rootNode.get(); }

	/**
	 * @brief 获取子节点缩进值
	 * @return 返回子节点缩进值
	 */
	int GetIndent() { return m_iIndent;	}

	/**
	 * @brief 设置子节点缩进值
	 * @param[in] indent 要设置的缩进值
	 * @return 无
	 */
	void SetIndent(int indent) { m_iIndent = indent; }

private:
	//以下函数故意私有化，表明禁止使用；应该使用TreeNode中的相关函数
	bool Add(Control* pControl) override;
	bool AddAt(Control* pControl, std::size_t iIndex) override;
	bool Remove(Control* pControl) override;
	bool RemoveAt(std::size_t iIndex) override;
	void RemoveAll() override;

	virtual void SetWindow(Window* pManager, Box* pParent, bool bInit = true) override;

private:
	int m_iIndent;
	std::unique_ptr<TreeNode> m_rootNode;
};

}

#endif // UI_CONTROL_TREEVIEW_H_
