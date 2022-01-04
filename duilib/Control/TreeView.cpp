#include "StdAfx.h"
#include "TreeView.h"


namespace ui
{

TreeNode::TreeNode() :
	m_bExpand(true),
	m_pTreeView(nullptr),
	m_pParentTreeNode(nullptr),
	m_iDepth(ROOT_NODE_DEPTH),
	m_aTreeNodes()
{
	
}

std::wstring TreeNode::GetType() const
{
	return DUI_CTR_TREENODE;
}

UIAControlProvider* TreeNode::GetUIAProvider()
{
#if defined(ENABLE_UIAUTOMATION)
	if (m_pUIAProvider == nullptr)
	{
		m_pUIAProvider = static_cast<UIAControlProvider*>(new (std::nothrow) UIATreeNodeProvider(this));
	}
	return m_pUIAProvider;
#else
	return nullptr;
#endif
}

void TreeNode::SetTreeView(TreeView* pTreeView)
{
    m_pTreeView = pTreeView;
}

bool TreeNode::OnClickItem(EventArgs* pMsg)
{
    TreeNode* pItem = static_cast<TreeNode*>(pMsg->pSender);
    pItem->SetExpand(!pItem->IsExpand(), true);
    return true;
}

bool TreeNode::IsVisible() const
{
	return ListContainerElement::IsVisible()
		&& (!m_pParentTreeNode || (m_pParentTreeNode && m_pParentTreeNode->IsExpand() && m_pParentTreeNode->IsVisible()));
}

void TreeNode::SetInternVisible(bool bVisible)
{
	Control::SetInternVisible(bVisible);
	if (m_items.empty()) return;

	for (auto it = m_items.begin(); it != m_items.end(); it++)
	{
		auto pControl = *it;
		// 控制子控件显示状态
		// InternVisible状态应由子控件自己控制
		pControl->SetInternVisible(Control::IsVisible());
	}
}

void TreeNode::SetWindow(Window* pManager, Box* pParent, bool bInit)
{
	ListContainerElement::SetWindow(pManager, pParent, bInit);
}

TreeNode* TreeNode::GetParentNode()
{
	return m_pParentTreeNode;
}

void TreeNode::SetParentNode(TreeNode* pParentTreeNode)
{
	m_pParentTreeNode = pParentTreeNode;
}

bool TreeNode::AddChildNode(TreeNode* pTreeNode)
{
	return AddChildNodeAt(pTreeNode, GetChildNodeCount());
}

bool TreeNode::AddChildNodeAt(TreeNode* pTreeNode, std::size_t iIndex)
{
	if( iIndex < 0 || iIndex > m_aTreeNodes.size() ) return false;
	m_aTreeNodes.insert(m_aTreeNodes.begin() + iIndex, pTreeNode);
		
	pTreeNode->m_iDepth = m_iDepth + 1;
	pTreeNode->SetParentNode(this);
	pTreeNode->SetTreeView(m_pTreeView);
	if( m_pWindow != NULL ) m_pWindow->InitControls(pTreeNode, NULL);
	pTreeNode->OnEvent[kEventClick] += nbase::Bind(&TreeNode::OnClickItem, this, std::placeholders::_1);

	UiRect padding = m_pLayout->GetPadding();
	int nodeIndex = -1;
	if (m_iDepth != ROOT_NODE_DEPTH) {
		nodeIndex = GetIndex();
		padding.left += m_pTreeView->GetIndent();
	}
	pTreeNode->m_pLayout->SetPadding(padding);

	std::size_t nGlobalIndex = iIndex;
	for (std::size_t i = 0; i < iIndex; i++)
	{
		nGlobalIndex += ((TreeNode*)m_aTreeNodes[i])->GetDescendantNodeCount();
	}

	return m_pTreeView->ListBox::AddAt(pTreeNode, (int)(nodeIndex + nGlobalIndex + 1));
}

bool TreeNode::RemoveChildNodeAt(std::size_t iIndex)
{
	if (iIndex < 0 || iIndex >= m_aTreeNodes.size()) {
		return false;
	}

	TreeNode* pTreeNode = ((TreeNode*)m_aTreeNodes[iIndex]);
	m_aTreeNodes.erase(m_aTreeNodes.begin() + iIndex);

	return pTreeNode->RemoveSelf();
}

bool TreeNode::RemoveChildNode(TreeNode* pTreeNode)
{
	auto it = std::find(m_aTreeNodes.begin(), m_aTreeNodes.end(), pTreeNode);
	if (it == m_aTreeNodes.end()) {
		return false;
	}
		
	int iIndex = it - m_aTreeNodes.begin();
	return RemoveChildNodeAt(iIndex);
}
	
void TreeNode::RemoveAllChildNode()
{
	while (m_aTreeNodes.size() > 0)
	{
		RemoveChildNodeAt(0);
	}
}

bool TreeNode::RemoveSelf()
{
	for( auto it = m_aTreeNodes.begin(); it != m_aTreeNodes.end(); it++ ) 
	{
		(*it)->RemoveSelf();
	}
	m_aTreeNodes.clear();

	if (m_iDepth != ROOT_NODE_DEPTH) {
		return m_pTreeView->ListBox::RemoveAt(GetIndex());
	}

	return false;
}

int TreeNode::GetDescendantNodeCount()
{
	int nodeCount = (int)GetChildNodeCount();
	for( auto it = m_aTreeNodes.begin(); it != m_aTreeNodes.end(); it++ )
	{
		nodeCount += (*it)->GetDescendantNodeCount();
	}

	return nodeCount;
}

std::size_t TreeNode::GetChildNodeCount()
{
	return m_aTreeNodes.size();
}
	
TreeNode* TreeNode::GetChildNode(std::size_t iIndex)
{
	if( iIndex < 0 || iIndex >= m_aTreeNodes.size() ) return NULL;
	return static_cast<TreeNode*>(m_aTreeNodes[iIndex]);
}
	
int TreeNode::GetChildNodeIndex(TreeNode* pTreeNode)
{
	auto it = std::find(m_aTreeNodes.begin(), m_aTreeNodes.end(), pTreeNode);
	if (it == m_aTreeNodes.end()) {
		return -1;
	}
	return it - m_aTreeNodes.begin();
}

bool TreeNode::IsExpand() const
{
	return m_bExpand;
}

void TreeNode::SetExpand(bool bExpand, bool bTriggerEvent)
{
	if(m_bExpand == bExpand) {
		return;
	}
	m_bExpand = bExpand;

	if (m_pWindow != NULL) {
		if (bTriggerEvent) {
			m_pWindow->SendNotify(this, m_bExpand ? kEventExpand : kEventUnExpand);
		}
	}

	m_pTreeView->Arrange();
}

int TreeNode::GetDepth()
{
	return m_iDepth;
}

TreeView::TreeView() :
	ListBox(new VLayout),
	m_iIndent(0),
	m_rootNode()
{
	m_rootNode.reset(new TreeNode());
	m_rootNode->SetTreeView(this);
}

std::wstring TreeView::GetType() const
{
	return DUI_CTR_TREEVIEW;
}

void TreeView::SetAttribute(const std::wstring& strName, const std::wstring& strValue)
{
	if( strName == _T("indent") ) {
		SetIndent(_ttoi(strValue.c_str()));
	}
	else {
		ListBox::SetAttribute(strName, strValue);
	}
}

bool TreeView::Add(Control* pControl)
{
	ASSERT(FALSE);
	return true;
}

bool TreeView::AddAt(Control* pControl, std::size_t iIndex)
{
	ASSERT(FALSE);
	return true;
}

bool TreeView::Remove(Control* pControl)
{
	ASSERT(FALSE);
	return true;
}

bool TreeView::RemoveAt(std::size_t iIndex)
{
	ASSERT(FALSE);
	return true;
}

void TreeView::RemoveAll()
{
	ASSERT(FALSE);
}

void TreeView::SetWindow(Window* pManager, Box* pParent, bool bInit)
{
	ListBox::SetWindow(pManager, pParent, bInit);
	m_rootNode->SetWindow(pManager, pParent, bInit);
}

}