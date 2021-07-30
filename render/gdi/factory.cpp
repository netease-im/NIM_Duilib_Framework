#include "framework.h"

#include <atomic>
#include <mutex>

namespace dui {
namespace render {

static std::mutex g_factory_mutex;

static RenderFactory* g_factory = nullptr;


RenderFactory* RenderFactory::GetInstance()
{
	std::lock_guard<std::mutex> lock(g_factory_mutex);

	if (g_factory == nullptr) {
		g_factory = new RenderFactory();
	}

	return g_factory;
}

void RenderFactory::Release()
{
	std::lock_guard<std::mutex> lock(g_factory_mutex);

	if (g_factory)
		delete g_factory;

	g_factory = nullptr;
}

common::dui_refptr<IPen> RenderFactory::CreatePen(DWORD color, int width)
{
	return new common::RefCountedObject<Pen>(color, width);
}

common::dui_refptr<IBrush> RenderFactory::CreateBrush(DWORD color)
{
	return new common::RefCountedObject<Brush>(color);
}

common::dui_refptr<IBrush> RenderFactory::CreateBrush(HBITMAP bitmap)
{
	return new common::RefCountedObject<Brush>(bitmap);
}

common::dui_refptr<IMatrix> RenderFactory::CreateMatrix()
{
	return new common::RefCountedObject<Matrix>();
}

common::dui_refptr<IPath> RenderFactory::CreatePath()
{
	return new common::RefCountedObject<Path>();
}

common::dui_refptr<IBitmap> RenderFactory::CreateBitmap()
{
	return new common::RefCountedObject<Bitmap>();
}

common::dui_refptr<IRenderContext> RenderFactory::CreateRenderContext()
{
	return new common::RefCountedObject<RenderContext>();
}

IRenderFactory* CreateFactory()
{
	return dynamic_cast<IRenderFactory*>(RenderFactory::GetInstance());
}

} // namespace render
} // namespace dui