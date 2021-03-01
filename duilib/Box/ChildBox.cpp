#include "stdafx.h"
#include "ChildBox.h"

namespace ui
{

ChildBox::ChildBox()
{

}

void ChildBox::Init()
{
	if (!m_strXMLFile.empty())
	{
		Box* pChildWindow = static_cast<Box*>(GlobalManager::CreateBoxWithCache(m_strXMLFile.c_str(), CreateControlCallback()));
		if (pChildWindow) {
			this->Add(pChildWindow);
		}
		else {
			ASSERT(FALSE);
			this->RemoveAll();
		}
	}
}

void ChildBox::SetAttribute( const::std::wstring& strName, const std::wstring& strValue )
{
	if( strName == _T("xmlfile") )
		SetChildLayoutXML(strValue);
	else
		Box::SetAttribute(strName,strValue);
}

void ChildBox::SetChildLayoutXML( std::wstring strXML )
{
	m_strXMLFile = strXML;
}

std::wstring ChildBox::GetChildLayoutXML()
{
	return m_strXMLFile;
}

} // namespace ui
