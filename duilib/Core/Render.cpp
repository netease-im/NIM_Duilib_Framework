#include "StdAfx.h"

namespace ui {

const std::string RenderFactoryLoader::kCreateFactoryName = "CreateFactory";
#ifndef _DEBUG
const std::wstring RenderFactoryLoader::kRenderFactoryGdiName = L"render_gdi.dll";
const std::wstring RenderFactoryLoader::kRenderFactorySkiaName = L"render_skia.dll";
#else
const std::wstring RenderFactoryLoader::kRenderFactoryGdiName = L"render_gdi_d.dll";
const std::wstring RenderFactoryLoader::kRenderFactorySkiaName = L"render_skia_d.dll";
#endif

RenderFactoryLoader::RenderFactoryLoader()
	: m_hModule(nullptr)
	, m_fCreateFactory(nullptr)
{
}

RenderFactoryLoader::~RenderFactoryLoader()
{
	if (m_hModule)
		FreeLibrary(m_hModule);
}

bool RenderFactoryLoader::CreateInstance(RenderFactoryType type, dui::render::IRenderFactory** ppFactory)
{
	if (!m_fCreateFactory) {
		
		std::wstring strModulePath = L"";
		switch (type)
		{
		case ui::kRenderFactoryGdi:
			strModulePath = kRenderFactoryGdiName;
			break;
		case ui::kRenderFactorySkia:
			strModulePath = kRenderFactorySkiaName;
			break;
		default:
			break;
		}


		m_hModule = LoadLibrary(strModulePath.c_str());
		if (!m_hModule)
			return false;

		m_fCreateFactory = (fCreateFactory)(GetProcAddress(m_hModule, kCreateFactoryName.c_str()));
		if (!m_fCreateFactory)
		{
			FreeLibrary(m_hModule);
			m_hModule = nullptr;
			return false;
		}
	}

	if (!m_fCreateFactory)
		return false;

	*ppFactory = m_fCreateFactory();

	return true;
}



RenderFactoryImpl::RenderFactoryImpl()
	: m_bInited(false)
	, m_nType(kRenderFactoryGdi)
	, m_pFactory(nullptr)
{

}

RenderFactoryImpl::~RenderFactoryImpl()
{

}

bool RenderFactoryImpl::Initialize(RenderFactoryType type)
{
	if (m_bInited)
		return true;

	if (!m_objLoader.CreateInstance(type, &m_pFactory) || !m_pFactory)
		return false;

	m_nType = type;
	m_bInited = true;

	return true;
}

dui::common::dui_refptr<dui::render::IPen> RenderFactoryImpl::CreatePen(DWORD color, int width)
{
	if (!m_bInited)
		return nullptr;

	return m_pFactory->CreatePen(color, width);
}

dui::common::dui_refptr<dui::render::IBrush> RenderFactoryImpl::CreateBrush(DWORD color)
{
	if (!m_bInited)
		return nullptr;

	return m_pFactory->CreateBrush(color);
}

dui::common::dui_refptr<dui::render::IBrush> RenderFactoryImpl::CreateBrush(HBITMAP bitmap)
{
	if (!m_bInited)
		return nullptr;

	return m_pFactory->CreateBrush(bitmap);
}

dui::common::dui_refptr<dui::render::IMatrix> RenderFactoryImpl::CreateMatrix()
{
	if (!m_bInited)
		return nullptr;

	return m_pFactory->CreateMatrix();
}

dui::common::dui_refptr<dui::render::IPath> RenderFactoryImpl::CreatePath()
{
	if (!m_bInited)
		return nullptr;

	return m_pFactory->CreatePath();
}

dui::common::dui_refptr<dui::render::IBitmap> RenderFactoryImpl::CreateBitmap()
{
	if (!m_bInited)
		return nullptr;

	return m_pFactory->CreateBitmap();
}

dui::common::dui_refptr<dui::render::IRenderContext> RenderFactoryImpl::CreateRenderContext()
{
	if (!m_bInited)
		return nullptr;

	return m_pFactory->CreateRenderContext();
}

}