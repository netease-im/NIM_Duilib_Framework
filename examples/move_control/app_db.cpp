#include "stdafx.h"
#include "app_db.h"

CAppDb::CAppDb()
{
	//这里可以打开db的一些操作 
}


CAppDb::~CAppDb()
{
	//这里可以关闭db的一些操作
}

bool CAppDb::LoadFromDb(std::vector<AppItem>& app_list)
{
	app_list.clear();
	//select from....
	//目前写死
	{
		AppItem item;
		item._id = "1";
		item._icon = L"1.png";
		item._isFrequent = false;
		item._name = L"城市服务";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "2";
		item._icon = L"2.png";
		item._isFrequent = false;
		item._name = L"天气预报";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "3";
		item._icon = L"3.png";
		item._isFrequent = true;
		item._name = L"考勤";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "4";
		item._icon = L"4.png";
		item._isFrequent = true;
		item._name = L"工资单";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "5";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"测试5";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "6";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"测试6";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "7";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"测试7";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "8";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"测试8";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "9";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"测试9";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "10";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"测试10";
		app_list.emplace_back(item);
	}
	{
		AppItem item;
		item._id = "11";
		item._icon = L"5.png";
		item._isFrequent = false;
		item._name = L"测试11";
		app_list.emplace_back(item);
	}
	
	return true;
}

bool CAppDb::SaveToDb(const AppItem& item)
{
	// insert or replace into ....
	return true;
}
