#pragma once
//#include "util.h"
#include "log.h"
#include <windows.h>
#include <gdiplus.h>

static const std::wstring kImageJPEG = L"image/jpeg";
static const std::wstring kImagePNG  = L"image/png";
static const std::wstring kImageBMP  = L"image/bmp";
static const std::wstring kImageGIF  = L"image/gif";


class ZoomImage
{
public:
	ZoomImage(void);
	~ZoomImage(void);
	//设置缩放后的图片保存路径
	void SetImagePath(const std::wstring &image_path);
	//设置为自动缩放模式
	void SetAutoZoom(bool area_zoom,int auto_max_with,int auto_max_height);
	//设置为固定大小缩放模式
	void SetSize(int width,int height);
	//设置为按比例缩放模式
	void SetRatio(float ratio);
	
	/*
	 *	@brief 缩放某一个图片，并保存到为指定格式
	 *	@param filepath:要缩放的源图片路径
	 *	@param mime_type:保存的图片类型
	 */
	bool Zoom(const std::wstring& filepath,std::wstring mime_type=L"image/jpeg");
	//把图片保存为指定类型，并不缩放图片
	bool ConvertImageFormat(std::wstring& filepath, std::wstring mime_type);
	//获取图像的翻转类型
	static Gdiplus::RotateFlipType GetRotateFlipType(Gdiplus::Image& image);
	static std::wstring GetMimeType(const std::wstring& filepath);
	static void GetMimeType(const std::wstring& filepath, std::wstring& mime_type_out, std::wstring& exten_out);
	/*
	*	@brief 以width、height的比例，截取源图片中同比例的最大范围的图片部分，并保存到为指定格式
	*	@param filepath:源图片路径
	*	@param image_path_out:输出图片路径
	*	@param width:要缩放的源图片路径
	*	@param height:要缩放的源图片路径
	*	@param mime_type:保存的图片类型
	*/
	static bool SavePaddingImage(const std::wstring& image_path, const std::wstring& image_path_out, int width, int height, std::wstring mime_type = L"image/jpeg");

protected:
	//判断是否需要进行缩放
	bool IsNeedZoom(int width,int height);
	//计算缩放后的大小，结果保存到width_和height_
	bool CalculateSize(int width_src,int height_src);
	static bool GetImageCLSID(const wchar_t* format, CLSID* pCLSID);
	static bool SaveImage(Gdiplus::Image& image, const std::wstring& file_path,std::wstring mime_type,long quality_num = 95);
	static Gdiplus::RotateFlipType GetRotateFlipType(short orientation);
	static std::wstring GetMimeType(GUID& clsid);

private:
	enum ZoomType
	{
		by_fixed_size = 0,
		by_ratio,
		by_auto,by_none
	};//by_fixed_size:按固定尺寸,by_ratio:按比率缩放;

	std::wstring file_path_save_;
	ZoomType zoom_type_;

	bool area_zoom_; //是否按面积缩放width*height;
	int auto_max_width_;
	int auto_max_height_;

	int width_;
	int height_;

	float ratio_;

	std::wstring image_path_;
};

//缩略图不超过max_width * max_height的情况下：返回值为true表示需要裁剪，同时按图片比例返回size
bool CalculateImageSize(const std::wstring& file, SIZE& size, int max_width, int max_height);

//生成缩略图，大小cx * cy
void ZoomImageF(const std::wstring &src, const std::wstring &dest, int cx, int cy);