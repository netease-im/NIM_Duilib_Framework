#pragma once

namespace dui {
namespace render {
class Clicp :public IClip {
	DISALLOW_COPY_AND_ASSIGN(Clicp);
	FRIEND_CLASS_REFCOUNTEDOBJECT(Clicp);
protected:
	Clicp();
	~Clicp();

public:
	void CreateClip(HDC hdc, UiRect rc) override;
	void CreateRoundClip(HDC hdc, UiRect rc, int width, int height) override;
	void ClearClip(HDC hdc) override;
};

}
}