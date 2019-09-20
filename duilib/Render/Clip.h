#ifndef UI_RENDER_CLIP_H_
#define UI_RENDER_CLIP_H_

#pragma once

namespace ui 
{

class UILIB_API GdiClip : public IClip
{
public:
	GdiClip();
    ~GdiClip();

	virtual void CreateClip(HDC hDC, UiRect rc) override;
	virtual void CreateRoundClip(HDC hDC, UiRect rc, int width, int height) override;
	virtual void ClearClip(HDC hDC) override;
};


} // namespace ui

#endif // UI_RENDER_CLIP_H_
