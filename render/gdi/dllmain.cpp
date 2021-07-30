#include "framework.h"

namespace {

ULONG_PTR g_gdiplus_token_;
Gdiplus::GdiplusStartupInput g_gdiplus_startup_;

bool InitlizeGdiPlus() {
	auto status = Gdiplus::GdiplusStartup(&g_gdiplus_token_, &g_gdiplus_startup_, nullptr);
	assert(status == Gdiplus::Ok);
	return status == Gdiplus::Ok;
}

void ReleaseGdiPlus() {
	if (g_gdiplus_token_)
		Gdiplus::GdiplusShutdown(g_gdiplus_token_);
}

}

bool APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitlizeGdiPlus();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		ReleaseGdiPlus();
		dui::render::RenderFactory::Release();
		break;
	}
	return TRUE;
}