#ifndef UI_CORE_RENDER_FACTORY_H_
#define UI_CORE_RENDER_FACTORY_H_
#pragma once

namespace ui 
{

class UILIB_API RenderFactory_GdiPlus : public IRenderFactory
{
public:
	virtual ui::IPen* CreatePen(DWORD color, int width = 1) override;
	virtual ui::IBrush* CreateBrush(DWORD corlor) override;
	virtual ui::IBrush* CreateBrush(HBITMAP bitmap) override;
	virtual ui::IMatrix* CreateMatrix() override;
	virtual ui::IPath* CreatePath() override;
	virtual ui::IBitmap* CreateBitmap() override;
	virtual ui::IRenderContext* CreateRenderContext() override;
};

} // namespace ui

#endif // UI_CORE_RENDER_FACTORY_H_
