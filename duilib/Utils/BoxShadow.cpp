#include "StdAfx.h"
#include "BoxShadow.h"

ui::BoxShadow::BoxShadow() :
	m_cpOffset(0, 0),
	m_nBlurRadius(2),
	m_nBlurSize(2),
	m_nSpreadSize(2),
	m_bExclude(true)
{

}

ui::BoxShadow::~BoxShadow()
{
}

void ui::BoxShadow::SetBoxShadowString(const std::wstring& strBoxShadow)
{
	std::wstring sItem;
	std::wstring sValue;
	LPTSTR pstr = NULL;
	bool bScaleDest = true;

	LPCTSTR pStrKey = strBoxShadow.c_str();
	while (*pStrKey != _T('\0')) {
		sItem.clear();
		sValue.clear();
		while (*pStrKey > _T('\0') && *pStrKey <= _T(' ')) pStrKey = ::CharNext(pStrKey);
		while (*pStrKey != _T('\0') && *pStrKey != _T('=') && *pStrKey > _T(' ')) {
			LPTSTR pstrTemp = ::CharNext(pStrKey);
			while (pStrKey < pstrTemp) {
				sItem += *pStrKey++;
			}
		}
		while (*pStrKey > _T('\0') && *pStrKey <= _T(' ')) pStrKey = ::CharNext(pStrKey);
		if (*pStrKey++ != _T('=')) break;
		while (*pStrKey > _T('\0') && *pStrKey <= _T(' ')) pStrKey = ::CharNext(pStrKey);
		if (*pStrKey++ != _T('\'')) break;
		while (*pStrKey != _T('\0') && *pStrKey != _T('\'')) {
			LPTSTR pstrTemp = ::CharNext(pStrKey);
			while (pStrKey < pstrTemp) {
				sValue += *pStrKey++;
			}
		}
		if (*pStrKey++ != _T('\'')) break;
		if (!sValue.empty()) {
			if (sItem == _T("color") || sItem == _T("res")) {
				m_strColor = sValue;
			}
			else if (sItem == _T("offset")) {
				m_cpOffset.x = _tcstol(sValue.c_str(), &pstr, 10);  ASSERT(pstr);
				m_cpOffset.y = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			}
			else if (sItem == _T("blurradius")) {
				m_nBlurRadius = _tcstol(sValue.c_str(), &pstr, 10); ASSERT(pstr);
			}
			else if (sItem == _T("blursize")) {
				m_nBlurSize = _tcstol(sValue.c_str(), &pstr, 10); ASSERT(pstr);
			}
			else if (sItem == _T("spreadsize")) {
				m_nSpreadSize = _tcstol(sValue.c_str(), &pstr, 10); ASSERT(pstr);
			}
			else if (sItem == _T("exclude")) {
				m_bExclude = (_tcscmp(pstr, _T("true")) == 0);
			}
		}
		if (*pStrKey++ != _T(' ')) break;
	}
}

bool ui::BoxShadow::HasShadow() const
{
	return !m_strColor.empty();
}
