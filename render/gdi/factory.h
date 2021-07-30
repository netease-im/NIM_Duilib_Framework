#pragma once

namespace dui {
namespace render {

class RenderFactory : public IRenderFactory
{
private:
	RenderFactory() {}
	~RenderFactory() {}

public:
	static RenderFactory* GetInstance();
	static void Release();

public:
	common::dui_refptr<IPen> CreatePen(DWORD color, int width = 1) override;
	common::dui_refptr<IBrush> CreateBrush(DWORD color) override;
	common::dui_refptr<IBrush> CreateBrush(HBITMAP bitmap) override;
	common::dui_refptr<IMatrix> CreateMatrix() override;
	common::dui_refptr<IPath> CreatePath() override;
	common::dui_refptr<IBitmap> CreateBitmap() override;
	common::dui_refptr<IRenderContext> CreateRenderContext() override;


};

}
}