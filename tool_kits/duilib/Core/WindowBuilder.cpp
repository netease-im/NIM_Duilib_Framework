#include "StdAfx.h"

namespace ui 
{

WindowBuilder::WindowBuilder()
{

}

Box* WindowBuilder::Create(STRINGorID xml, CreateControlCallback pCallback, 
	Window* pManager, Box* pParent, Box* pUserDefinedBox)
{
	//��ԴIDΪ0-65535�������ֽڣ��ַ���ָ��Ϊ4���ֽ�
	//�ַ�����<��ͷ��Ϊ��XML�ַ�����������Ϊ��XML�ļ�
	//���ʹ���� zip ѹ����������ڴ��ж�ȡ
	if (HIWORD(xml.m_lpstr) != NULL) {
		if (*(xml.m_lpstr) == _T('<')) {
			if (!m_xml.Load(xml.m_lpstr)) return NULL;
		}
		else if (GlobalManager::IsUseZip()) {
			std::wstring sFile = GlobalManager::GetResourcePath();
			sFile += xml.m_lpstr;
			HGLOBAL hGlobal = GlobalManager::GetData(sFile);
			if (hGlobal)
			{
				std::string src((LPSTR)GlobalLock(hGlobal), GlobalSize(hGlobal));
				std::wstring string_resourse;
				StringHelper::MBCSToUnicode(src.c_str(), string_resourse, CP_UTF8);
				GlobalFree(hGlobal);
				if (!m_xml.Load(string_resourse.c_str())) return NULL;
			}
			else
			{
				if (!m_xml.LoadFromFile(xml.m_lpstr)) return NULL;
			}
		}
		else {
			if (!m_xml.LoadFromFile(xml.m_lpstr)) return NULL;
		}
	}
	else {
		ASSERT(FALSE);
	}

	return Create(pCallback, pManager, pParent, pUserDefinedBox);
}

Box* WindowBuilder::Create(CreateControlCallback pCallback, Window* pManager, Box* pParent, Box* pUserDefinedBox)
{
	m_createControlCallback = pCallback;
	CMarkupNode root = m_xml.GetRoot();
	if( !root.IsValid() ) return NULL;

	if( pManager ) {
		std::wstring strClass;
		int nAttributes = 0;
		std::wstring strName;
		std::wstring strValue;
		LPTSTR pstr = NULL;
		strClass = root.GetName();

		if( strClass == _T("Global") )
		{
			int nAttributes = root.GetAttributeCount();
			for( int i = 0; i < nAttributes; i++ ) {
				strName = root.GetAttributeName(i);
				strValue = root.GetAttributeValue(i);
				if( strName == _T("disabledfontcolor") ) {
					GlobalManager::SetDefaultDisabledTextColor(strValue);
				} 
				else if( strName == _T("defaultfontcolor") ) {	
					GlobalManager::SetDefaultTextColor(strValue);
				}
				else if( strName == _T("linkfontcolor") ) {
					DWORD clrColor = GlobalManager::GetTextColor(strValue);
					GlobalManager::SetDefaultLinkFontColor(clrColor);
				} 
				else if( strName == _T("linkhoverfontcolor") ) {
					DWORD clrColor = GlobalManager::GetTextColor(strValue);
					GlobalManager::SetDefaultLinkHoverFontColor(clrColor);
				} 
				else if( strName == _T("selectedcolor") ) {
					DWORD clrColor = GlobalManager::GetTextColor(strValue);
					GlobalManager::SetDefaultSelectedBkColor(clrColor);
				}
			}
		}
		else if( strClass == _T("Window") ) {
			if( pManager->GetHWND() ) {
				int nAttributes = root.GetAttributeCount();
				for( int i = 0; i < nAttributes; i++ ) {
					strName = root.GetAttributeName(i);
					strValue = root.GetAttributeValue(i);
					if( strName == _T("size") ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(strValue.c_str(), &pstr, 10);	ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);	ASSERT(pstr); 
						pManager->SetInitSize(cx, cy);
					} 
					else if( strName == _T("heightpercent") ) {
						double lfHeightPercent = _ttof(strValue.c_str());
						pManager->SetHeightPercent(lfHeightPercent);

						MONITORINFO oMonitor = {}; 
						oMonitor.cbSize = sizeof(oMonitor);
						::GetMonitorInfo(::MonitorFromWindow(pManager->GetHWND(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
						int nWindowHeight = int((oMonitor.rcWork.bottom - oMonitor.rcWork.top) * lfHeightPercent);
						int nMinHeight = pManager->GetMinInfo().cy;
						int nMaxHeight = pManager->GetMaxInfo().cy;
						if (nMinHeight != 0 && nWindowHeight < nMinHeight) {
							nWindowHeight = nMinHeight;
						}
						if (nMaxHeight != 0 && nWindowHeight > nMaxHeight) {
							nWindowHeight = nMaxHeight;
						}

						CSize xy = pManager->GetInitSize();
						pManager->SetInitSize(xy.cx, nWindowHeight, false, false);
					}
					else if( strName == _T("sizebox") ) {
						UiRect rcSizeBox;
						LPTSTR pstr = NULL;
						rcSizeBox.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);    
						rcSizeBox.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
						rcSizeBox.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
						rcSizeBox.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
						pManager->SetSizeBox(rcSizeBox);
					}
					else if( strName == _T("caption") ) {
						UiRect rcCaption;
						LPTSTR pstr = NULL;
						rcCaption.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);    
						rcCaption.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
						rcCaption.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
						rcCaption.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
						pManager->SetCaptionRect(rcCaption);
					}
					else if( strName == _T("textid") ) {
						pManager->SetTextId(strValue);
					}
					else if( strName == _T("roundcorner") ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pManager->SetRoundCorner(cx, cy);
					} 
					else if( strName == _T("mininfo") ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pManager->SetMinInfo(cx, cy);
					}
					else if( strName == _T("maxinfo") ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pManager->SetMaxInfo(cx, cy);
					}
					else if( strName == _T("shadowattached") ) {
						pManager->SetShadowAttached(strValue == _T("true"));
					}
					else if (strName == _T("shadowimage")) {
						pManager->SetShadowImage(strValue);
					}
					else if (strName == _T("shadowcorner")) {
						UiRect rc;
						LPTSTR pstr = NULL;
						rc.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);
						rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
						rc.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
						rc.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
						pManager->SetShadowCorner(rc);
					}
					else if (strName == _T("alphafixcorner") || strName == _T("custom_shadow")) {
						UiRect rc;
						LPTSTR pstr = NULL;
						rc.left = _tcstol(strValue.c_str(), &pstr, 10);  ASSERT(pstr);
						rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
						rc.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
						rc.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
						pManager->SetAlphaFixCorner(rc);
					}
				}
			}
		}

		if( strClass == _T("Global") ) {
			for( CMarkupNode node = root.GetChild() ; node.IsValid(); node = node.GetSibling() ) {
				strClass = node.GetName();
				if( strClass == _T("Image") ) {
					ASSERT(FALSE);	//����
				}
				else if( strClass == _T("Font") ) {
					nAttributes = node.GetAttributeCount();
					std::wstring strFontId;
					std::wstring strFontName;
					int size = 12;
					bool bold = false;
					bool underline = false;
					bool strikeout = false;
					bool italic = false;
					bool isDefault = false;
					for( int i = 0; i < nAttributes; i++ ) {
						strName = node.GetAttributeName(i);
						strValue = node.GetAttributeValue(i);
						if (strName == _T("id"))
						{
							strFontId = strValue;
						}
						else if( strName == _T("name") ) {
							strFontName = strValue;
						}
						else if( strName == _T("size") ) {
							size = _tcstol(strValue.c_str(), &pstr, 10);
						}
						else if( strName == _T("bold") ) {
							bold = (strValue == _T("true"));
						}
						else if( strName == _T("underline") ) {
							underline = (strValue == _T("true"));
						}
						else if (strName == _T("strikeout")) {
							strikeout = (strValue == _T("true"));
						}
						else if( strName == _T("italic") ) {
							italic = (strValue == _T("true"));
						}
						else if( strName == _T("default") ) {
							isDefault = (strValue == _T("true"));
						}
					}
					if( !strFontName.empty() ) {
						GlobalManager::AddFont(strFontId, strFontName, size, bold, underline,strikeout, italic, isDefault);
					}
				}
				else if( strClass == _T("Class") ) {
					nAttributes = node.GetAttributeCount();
					std::wstring strClassName;
					std::wstring strAttribute;
					for( int i = 0; i < nAttributes; i++ ) {
						strName = node.GetAttributeName(i);
						strValue = node.GetAttributeValue(i);
						if( strName == _T("name") ) {
							strClassName = strValue;
						}
						else if( strName == _T("value") ) {
							strAttribute.append(strValue);
						}
						else if (strName == _T("_value")) {
							strAttribute.append(StringHelper::Printf(L" value=\"%s\"",strValue.c_str()));
						}
						else {
							strAttribute.append(StringHelper::Printf(L" %s=\"%s\"",
								strName.c_str(), strValue.c_str()));
						}
					}
					if( !strClassName.empty() ) {
						StringHelper::TrimLeft(strAttribute);
						GlobalManager::AddClass(strClassName, strAttribute);
					}
				}
				else if( strClass == _T("TextColor") ) {
					nAttributes = node.GetAttributeCount();
					std::wstring strColorName;
					std::wstring strColor;
					for( int i = 0; i < nAttributes; i++ ) {
						strName = node.GetAttributeName(i);
						strValue = node.GetAttributeValue(i);
						if( strName == _T("name") ) {
							strColorName = strValue;
						}
						else if( strName == _T("value") ) {
							strColor = strValue;
						}
					}
					if( !strColorName.empty()) {
						GlobalManager::AddTextColor(strColorName, strColor);
					}
				}
			}
		}
		else if ( strClass == _T("Window") )
		{
			for( CMarkupNode node = root.GetChild() ; node.IsValid(); node = node.GetSibling() ) {
				strClass = node.GetName();
				if( strClass == _T("Class") ) {
					nAttributes = node.GetAttributeCount();
					std::wstring strClassName;
					std::wstring strAttribute;
					for( int i = 0; i < nAttributes; i++ ) {
						strName = node.GetAttributeName(i);
						strValue = node.GetAttributeValue(i);
						if( strName == _T("name") ) {
							strClassName = strValue;
						}
						else if( strName == _T("value") ) {
							strAttribute.append(strValue);
						}
						else if (strName == _T("_value")) {
							strAttribute.append(StringHelper::Printf(L" value=\"%s\"", strValue.c_str()));
						}
						else {
							strAttribute.append(StringHelper::Printf(L" %s=\"%s\"",
								strName.c_str(), strValue.c_str()));
						}
					}
					if( !strClassName.empty() ) {
						ASSERT( GlobalManager::GetClassAttributes(strClassName).empty() );	//�����е�Class������ȫ�ֵ�����
						StringHelper::TrimLeft(strAttribute);
						pManager->AddClass(strClassName, strAttribute);
					}
				}
			}
		}
	}

	for( CMarkupNode node = root.GetChild() ; node.IsValid(); node = node.GetSibling() ) {
		std::wstring strClass = node.GetName();
		if( strClass == _T("Image") || strClass == _T("Font")
			|| strClass == _T("Class") || strClass == _T("TextColor") ) {

		}
		else {
			if (!pUserDefinedBox) {
				return (Box*)_Parse(&root, pParent, pManager);
			}
			else {
				int nAttributes = node.GetAttributeCount();
				for( int i = 0; i < nAttributes; i++ ) {
					ASSERT(i == 0 || _tcscmp(node.GetAttributeName(i), _T("class")) != 0);	//class�����ǵ�һ������
					pUserDefinedBox->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
				}
				
				_Parse(&node, pUserDefinedBox, pManager);
				return pUserDefinedBox;
			}
		}
	}

	return nullptr;
}

CMarkup* WindowBuilder::GetMarkup()
{
    return &m_xml;
}

void WindowBuilder::GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const
{
    return m_xml.GetLastErrorMessage(pstrMessage, cchMax);
}

void WindowBuilder::GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const
{
    return m_xml.GetLastErrorLocation(pstrSource, cchMax);
}

Control* WindowBuilder::_Parse(CMarkupNode* pRoot, Control* pParent, Window* pManager)
{
    Control* pReturn = NULL;
    for( CMarkupNode node = pRoot->GetChild() ; node.IsValid(); node = node.GetSibling() ) {
        std::wstring strClass = node.GetName();
		if( strClass == _T("Image") || strClass == _T("Font")
			|| strClass == _T("Class") || strClass == _T("TextColor") ) {
				continue;
		}

        Control* pControl = NULL;
        if( strClass == _T("Include") ) {
            if( !node.HasAttributes() ) continue;
            int nCount = 1;
            LPTSTR pstr = NULL;
            TCHAR szValue[500] = { 0 };
            SIZE_T cchLen = lengthof(szValue) - 1;
            if ( node.GetAttributeValue(_T("count"), szValue, cchLen) )
                nCount = _tcstol(szValue, &pstr, 10);
            cchLen = lengthof(szValue) - 1;
            if ( !node.GetAttributeValue(_T("source"), szValue, cchLen) ) continue;
            for ( int i = 0; i < nCount; i++ ) {
                WindowBuilder builder;
                pControl = builder.Create((LPCTSTR)szValue, m_createControlCallback, pManager, (Box*)pParent);
            }
            continue;
        }
        else {
			pControl = CreateControlByClass(strClass);
			if (pControl == nullptr) {
				if (strClass == L"Event" || strClass == L"BubbledEvent") {
					bool bBubbled = (strClass == L"BubbledEvent");
					AttachXmlEvent(bBubbled, node, pParent);
					continue;
				}
			}

            // User-supplied control factory
            if( pControl == NULL ) {
				pControl = GlobalManager::CreateControl(strClass);
            }

            if( pControl == NULL && m_createControlCallback ) {
                pControl = m_createControlCallback(strClass);
            }
        }

		if( pControl == NULL ) {
			ASSERT(FALSE);
			continue;
		}

		pControl->SetWindow(pManager);
		// Process attributes
		if( node.HasAttributes() ) {
			// Set ordinary attributes
			int nAttributes = node.GetAttributeCount();
			for( int i = 0; i < nAttributes; i++ ) {
				ASSERT(i == 0 || _tcscmp(node.GetAttributeName(i), _T("class")) != 0);	//class�����ǵ�һ������
				pControl->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
			}
		}

        // Add children
        if( node.HasChildren() ) {
            _Parse(&node, (Box*)pControl, pManager);
        }

		// Attach to parent
        // ��ΪĳЩ���Ժ͸�������أ�����selected��������Add��������
		if( pParent != NULL ) {
			Box* pContainer = dynamic_cast<Box*>(pParent);
			ASSERT(pContainer);
			if( pContainer == NULL ) return NULL;
			if( !pContainer->Add(pControl) ) {
				ASSERT(FALSE);
				delete pControl;
				continue;
			}
		}
        
        // Return first item
        if( pReturn == NULL ) pReturn = pControl;
    }
    return pReturn;
}

Control* WindowBuilder::CreateControlByClass(const std::wstring& strControlClass)
{
	Control* pControl = nullptr;
	SIZE_T cchLen = strControlClass.length();
	switch( cchLen ) {
	case 3:
		if( strControlClass == DUI_CTR_BOX )					pControl = new Box;
		break;
	case 4:
		if( strControlClass == DUI_CTR_HBOX )					pControl = new HBox;
		else if( strControlClass == DUI_CTR_VBOX )				pControl = new VBox;
		break;
	case 5:
		if( strControlClass == DUI_CTR_COMBO )                  pControl = new Combo;
		else if( strControlClass == DUI_CTR_LABEL )             pControl = new Label;
		break;
	case 6:
		if( strControlClass == DUI_CTR_BUTTON )                 pControl = new Button;
		else if( strControlClass == DUI_CTR_OPTION )            pControl = new Option;
		else if( strControlClass == DUI_CTR_SLIDER )            pControl = new Slider;
		else if( strControlClass == DUI_CTR_TABBOX )			pControl = new TabBox;
		break;
	case 7:
		if( strControlClass == DUI_CTR_CONTROL )                pControl = new Control;
		else if( strControlClass == DUI_CTR_TILEBOX )		  	pControl = new TileBox;
		else if (strControlClass == DUI_CTR_LISTBOX)			pControl = new ListBox(new Layout);
		//else if( pstrClass == DUI_CTR_ACTIVEX )				pControl = new ActiveX;
		break;
	case 8:
		if( strControlClass == DUI_CTR_PROGRESS )               pControl = new Progress;
		else if( strControlClass == DUI_CTR_RICHEDIT )          pControl = new RichEdit;
		else if( strControlClass == DUI_CTR_CHECKBOX )			pControl = new CheckBox;
		//else if( pstrClass == DUI_CTR_DATETIME )				pControl = new DateTime;
		else if( strControlClass == DUI_CTR_TREEVIEW )			pControl = new TreeView;
		else if( strControlClass == DUI_CTR_TREENODE )			pControl = new TreeNode;
		else if( strControlClass == DUI_CTR_HLISTBOX )			pControl = new ListBox(new HLayout);
		else if( strControlClass == DUI_CTR_VLISTBOX )          pControl = new ListBox(new VLayout);
		else if ( strControlClass == DUI_CTR_CHILDBOX )			pControl = new ChildBox;
		else if( strControlClass == DUI_CTR_LABELBOX )          pControl = new LabelBox;
		break;
	case 9:
		if( strControlClass == DUI_CTR_SCROLLBAR )				pControl = new ScrollBar; 
		else if( strControlClass == DUI_CTR_BUTTONBOX )         pControl = new ButtonBox;
		else if( strControlClass == DUI_CTR_OPTIONBOX )         pControl = new OptionBox;
		break;
	case 10:
		//if( pstrClass == DUI_CTR_WEBBROWSER )					pControl = new WebBrowser;
		break;
	case 11:
		if( strControlClass == DUI_CTR_TILELISTBOX )			pControl = new ListBox(new TileLayout);
		else if( strControlClass == DUI_CTR_CHECKBOXBOX )		pControl = new CheckBoxBox;
		break;
	case 14:
		if (strControlClass == DUI_CTR_VIRTUALLISTBOX)			pControl = new VirtualListBox;
		else if (strControlClass == DUI_CTR_CIRCLEPROGRESS)     pControl = new CircleProgress;
		break;
	case 15:
		break;
	case 16:
		break;
	case 20:
		if( strControlClass == DUI_CTR_LISTCONTAINERELEMENT )   pControl = new ListContainerElement;
		break;
	}

	return pControl;
}

void WindowBuilder::AttachXmlEvent(bool bBubbled, CMarkupNode& node, Control* pParent)
{
	auto nAttributes = node.GetAttributeCount();
	std::wstring strType;
	std::wstring strReceiver;
	std::wstring strApplyAttribute;
	std::wstring strName;
	std::wstring strValue;
	for( int i = 0; i < nAttributes; i++ ) {
		strName = node.GetAttributeName(i);
		strValue = node.GetAttributeValue(i);
		ASSERT(i != 0 || strName == _T("type"));
		ASSERT(i != 1 || strName == _T("receiver"));
		ASSERT(i != 2 || strName == _T("applyattribute"));
		if( strName == _T("type") ) {
			strType = strValue;
		}
		else if( strName == _T("receiver") ) {
			strReceiver = strValue;
		}
		else if( strName == _T("applyattribute") ) {
			strApplyAttribute = strValue;
		}
	}

	auto typeList = StringHelper::Split(strType, L" ");
	auto receiverList = StringHelper::Split(strReceiver, L" ");
	for (auto itType = typeList.begin(); itType != typeList.end(); itType++) {
		for (auto itReceiver = receiverList.begin(); itReceiver != receiverList.end(); itReceiver++) {
			EventType eventType = StringToEnum(*itType);
			auto callback = nbase::Bind(&Control::OnApplyAttributeList, pParent, *itReceiver, strApplyAttribute, std::placeholders::_1);
			if (bBubbled == false) {
				pParent->AttachXmlEvent(eventType, callback);
			}
			else {
				if (Box* tmpParent = dynamic_cast<Box*>(pParent)) {
					tmpParent->AttachXmlBubbledEvent(eventType, callback);
				}
				else {
					ASSERT(FALSE);
				}

			}
		}
	}
}

} // namespace ui
