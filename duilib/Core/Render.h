#ifndef UI_CORE_RENDER_H_
#define UI_CORE_RENDER_H_
#pragma once

#include "render/render_interface.h"

namespace ui {

enum RenderFactoryType {
	kRenderFactoryGdi = 0,
	kRenderFactorySkia
};

class RenderFactoryLoader
{
	using fCreateFactory = dui::render::IRenderFactory* (*)();
public:
	RenderFactoryLoader();
	~RenderFactoryLoader();

	bool CreateInstance(RenderFactoryType type, dui::render::IRenderFactory** ppFactory);

private:
	static const std::string kCreateFactoryName;
	static const std::wstring kRenderFactoryGdiName;
	static const std::wstring kRenderFactorySkiaName;

	HMODULE m_hModule;
	fCreateFactory m_fCreateFactory;
};



class RenderFactoryImpl : public dui::render::IRenderFactory 
{
public:
	RenderFactoryImpl();
	~RenderFactoryImpl();

	bool Initialize(RenderFactoryType type);

	dui::common::dui_refptr<dui::render::IPen> CreatePen(DWORD color, int width = 1) override;
	dui::common::dui_refptr<dui::render::IBrush> CreateBrush(DWORD color) override;
	dui::common::dui_refptr<dui::render::IBrush> CreateBrush(HBITMAP bitmap) override;
	dui::common::dui_refptr<dui::render::IMatrix> CreateMatrix() override;
	dui::common::dui_refptr<dui::render::IPath> CreatePath() override;
	dui::common::dui_refptr<dui::render::IBitmap> CreateBitmap() override;
	dui::common::dui_refptr<dui::render::IRenderContext> CreateRenderContext() override;

private:
	bool m_bInited;
	RenderFactoryType m_nType;
	RenderFactoryLoader m_objLoader;
	dui::render::IRenderFactory* m_pFactory;
};

}




#endif
