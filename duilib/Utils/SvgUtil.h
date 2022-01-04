#ifndef UI_UTILS_SVGUTIL_H_
#define UI_UTILS_SVGUTIL_H_

namespace ui
{

class UILIB_API SvgUtil
{
public:
	static bool IsSvgFile(const std::wstring& strImageFullPath);
	static std::unique_ptr<ImageInfo> LoadSvg(const std::wstring& strImageFullPath);
	static std::unique_ptr<ImageInfo> LoadSvg(HGLOBAL hGlobal, const std::wstring& strImageFullPath);

private:
	static std::unique_ptr<ImageInfo> LoadImageBySvg(void *svg, const std::wstring& strImageFullPath);
};

}

#endif // UI_UTILS_SVGUTIL_H_
