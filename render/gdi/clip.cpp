#include "framework.h"

namespace dui {
namespace render {

Clicp::Clicp()
{
}

Clicp::~Clicp()
{
}

void Clicp::CreateClip(HDC hdc, UiRect rc)
{
	if (hdc != NULL) {
		CPoint ptWinOrg;
		GetWindowOrgEx(hdc, &ptWinOrg);
		rc.Offset(-ptWinOrg.x, -ptWinOrg.y);

		HRGN hRgn = ::CreateRectRgnIndirect(&rc);
		::SaveDC(hdc);
		::ExtSelectClipRgn(hdc, hRgn, RGN_AND);
		::DeleteObject(hRgn);
	}
}

void Clicp::CreateRoundClip(HDC hdc, UiRect rc, int width, int height)
{
	if (hdc != NULL) {
		CPoint ptWinOrg;
		GetWindowOrgEx(hdc, &ptWinOrg);
		rc.Offset(-ptWinOrg.x, -ptWinOrg.y);

		HRGN hRgn = ::CreateRoundRectRgn(rc.left, rc.top, rc.right + 1, rc.bottom + 1, width, height);
		::SaveDC(hdc);
		::ExtSelectClipRgn(hdc, hRgn, RGN_AND);
		::DeleteObject(hRgn);
	}
}

void Clicp::ClearClip(HDC hdc)
{
	if (hdc != NULL) {
		::RestoreDC(hdc, -1);
	}
}

}
}