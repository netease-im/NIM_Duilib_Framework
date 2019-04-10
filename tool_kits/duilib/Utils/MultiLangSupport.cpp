#include "StdAfx.h"

namespace ui 
{

MutiLanSupport* MutiLanSupport::GetInstance()
{
	static MutiLanSupport mutiLanSupport;
	return &mutiLanSupport;
}

bool MutiLanSupport::LoadStringTable(const std::wstring &strFilePath)
{
	ClearAll();

	FILE *hFile;
	_wfopen_s(&hFile, strFilePath.c_str(), L"r");
	if (hFile == NULL) {
		return false;
	}

	char strRead[4096];
	std::vector<std::wstring> stringList;
	while (fgets(strRead, 4096, hFile) != NULL)
	{
		std::wstring strResource;
		std::string src = strRead;

		StringHelper::MBCSToUnicode(src.c_str(), strResource, CP_UTF8);
		strResource = StringHelper::TrimLeft(strResource);
		strResource = StringHelper::TrimRight(strResource);
		if (!strResource.empty()) {
			stringList.push_back(strResource);
		}
	}
	fclose(hFile);

	AnalyzeStringTable(stringList);
	return true;
}

bool MutiLanSupport::LoadStringTable(const HGLOBAL& hGlobal)
{
	std::vector<std::wstring> string_list;
	std::string fragment((LPSTR)GlobalLock(hGlobal), GlobalSize(hGlobal));
	fragment.append("\n");
	std::string src;
	for (auto& it : fragment)
	{
		if (it == '\0' || it == '\n')
		{
			std::wstring string_resourse;
			StringHelper::MBCSToUnicode(src.c_str(), string_resourse, CP_UTF8);
			string_resourse = StringHelper::TrimLeft(string_resourse);
			string_resourse = StringHelper::TrimRight(string_resourse);
			if (!string_resourse.empty())
			{
				string_list.push_back(string_resourse);
			}
			src.clear();
			continue;
		}
		src.push_back(it);
	}

	GlobalUnlock(hGlobal);
	AnalyzeStringTable(string_list);

	return true;
}

std::wstring MutiLanSupport::GetStringViaID(const std::wstring& id)
{
	std::wstring text;
	if(id.empty())
		return text;

	auto it = m_stringTable.find(id);
	if (it == m_stringTable.end()) {
		ASSERT(FALSE);
		return text;
	}
	else {
		text = it->second;
		StringHelper::ReplaceAll(L"\\r", L"\r", text);
		StringHelper::ReplaceAll(L"\\n", L"\n", text);
	}

	return text;
}

bool MutiLanSupport::AnalyzeStringTable(const std::vector<std::wstring> &list)
{
	int	nCount = (int)list.size();
	if (nCount <= 0)
		return false;
	
	for(int i = 0; i < nCount; i++)
	{
		std::wstring strSrc = list[i];
		std::list<std::wstring> idAndString = StringHelper::Split(strSrc, L"=");
		if (idAndString.size() != 2) {
			continue;
		}

		std::wstring id = *(idAndString.begin());
		id = StringHelper::TrimLeft(id);
		id = StringHelper::TrimRight(id);

		idAndString.pop_front();

		std::wstring strResource = *(idAndString.begin());	
		strResource = StringHelper::TrimLeft(strResource);
		strResource = StringHelper::TrimRight(strResource);

		if (id.find(L";") == -1) {
			m_stringTable[id] = strResource;
		}
	}

	return true;
}

void MutiLanSupport::ClearAll()
{
	m_stringTable.clear();
}

}