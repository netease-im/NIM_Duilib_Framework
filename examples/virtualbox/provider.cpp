#include "stdafx.h"
#include "provider.h"
#include "item.h"


int g_index = 1;

Provider::Provider()
:m_nTotal(0)
{

}


Provider::~Provider()
= default;

ui::Control* Provider::CreateElement()
{
	Item* item = new Item;
	ui::GlobalManager::FillBoxWithCache(item, L"virtualbox/item.xml");
	return item;
}

void Provider::FillElement(ui::Control *control, int index)
{
	Item* pItem = dynamic_cast<Item*>(control);

	
	std::wstring img = L"icon.png";
	std::wstring title = nbase::StringPrintf(L"%s [%02d]", m_vTasks[index].sName.c_str(), m_vTasks[index].nId);
	pItem->InitSubControls(img, title, index);

}

int Provider::GetElementtCount()
{
	// 加锁
	nbase::NAutoLock auto_lock(&lock_);
	return m_vTasks.size();
}

void Provider::SetTotal(int nTotal)
{
	if (nTotal == m_nTotal) return;
	if (nTotal <= 0) return;

	// 加锁
	lock_.Lock();
	m_vTasks.clear();
	for (auto i=1; i <= nTotal; i++)
	{
		DownloadTask task;
		task.nId = i;
		task.sName = L"任务名称";
		m_vTasks.emplace_back(task);
	}
	lock_.Unlock();

	// 通知TileBox数据总数变动
	EmitCountChanged();
}

void Provider::RemoveTask(int nIndex)
{	
	lock_.Lock();

	auto iter =	m_vTasks.begin() + nIndex;
	m_vTasks.erase(iter);

	lock_.Unlock();

	// 通知TileBox数据总数变动
	EmitCountChanged();
}

void Provider::ChangeTaskName(int nIndex, const std::wstring& sName)
{
	if (nIndex > 0 && nIndex < (int)m_vTasks.size())
	{
		m_vTasks[nIndex].sName = sName;
	}

	// 发送数据变动通知
	EmitDataChanged(nIndex, nIndex);
}
