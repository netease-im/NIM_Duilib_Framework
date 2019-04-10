#include "stdafx.h"
#include "zoom_image.h"

using namespace Gdiplus;

ZoomImage::ZoomImage(void):width_(0),height_(0),ratio_(0),zoom_type_(by_none),area_zoom_(false)	
	,auto_max_width_(1280),auto_max_height_(1280)
{
}


ZoomImage::~ZoomImage(void)
{
}

void ZoomImage::SetImagePath(const std::wstring &image_path)
{
	image_path_ = image_path;
}

void ZoomImage::SetAutoZoom(bool area_zoom,int auto_max_with,int auto_max_height)
{
	area_zoom_ = area_zoom;
	auto_max_width_ = auto_max_with;
	auto_max_height_ = auto_max_height;
	zoom_type_ = by_auto;
}

void ZoomImage::SetSize(int width,int height)
{
	width_ = width;
	height_ = height;
	zoom_type_ = by_fixed_size;

}

void ZoomImage::SetRatio(float ratio)
{
	ratio_ = ratio;
	zoom_type_ = by_ratio;
}

bool ZoomImage::Zoom(const std::wstring &filepath,std::wstring mime_type)
{
	if( !nbase::FilePathIsExist(filepath, false) )
	{
		assert(0);
		return false;
	}

	if (zoom_type_ == by_none)
		return false;
	if (zoom_type_ == by_fixed_size && (width_ <= 0 ||height_ <= 0))
		return false;
	if (zoom_type_ == by_ratio && ratio_ <= 0.0000001)
		return false;

	Image image_src(filepath.c_str());
	if (image_src.GetLastStatus() != Ok)
		return false;
	GUID guid;
	if (image_src.GetRawFormat(&guid) != Ok)
		return false;
	if (guid == ImageFormatGIF) //不支持GIF文件;
		return false;

	RotateFlipType type = GetRotateFlipType(image_src);
	if (type != RotateNoneFlipNone)
		image_src.RotateFlip(type);
	
	if (!IsNeedZoom(image_src.GetWidth(),image_src.GetHeight()))
		return false;

	CalculateSize(image_src.GetWidth(),image_src.GetHeight());
	Bitmap canvas(width_,height_);
	Graphics graphics(&canvas);
	RectF rf(0.0f, 0.0f, (REAL)width_, (REAL)height_);

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
#if (GDIPVER >= 0x0110)
	graphics.SetSmoothingMode(SmoothingModeAntiAlias8x8);
#endif
	graphics.SetCompositingMode(CompositingModeSourceOver);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

	if (Ok != graphics.DrawImage(&image_src,rf))
    {
		int err = graphics.GetLastStatus();
		QLOG_APP(L"ZoomImage Error: {0}")<<err;
    }
	return SaveImage(canvas, image_path_, mime_type);
}

bool ZoomImage::IsNeedZoom(int width,int height)
{
	bool ret = false;
	switch (zoom_type_)
	{
	case by_fixed_size:
		if (width_ != width || height_ != height)
			ret = true;
		break;
	case by_ratio:
		if (ratio_ < 1.0000000 || ratio_ > 1.0000000)
			ret = true;
		break;
	case by_auto:
		{
			if (area_zoom_)
			{
				if (width * height> auto_max_width_ * auto_max_height_)
					ret = true;
			}
			else
			{
				if (width > auto_max_width_ ||  height > auto_max_height_)
					ret = true;
			}
			
		}
		
		break;
	default:
		break;
	}

	return ret;
}

bool ZoomImage::CalculateSize(int width_src,int height_src)
{
	if (zoom_type_ == by_ratio)
	{
		width_ = (int) (width_src * ratio_);
		height_ = (int) (height_src * ratio_);
	}
	else if (zoom_type_ == by_auto)
	{
		if (area_zoom_)
		{
			width_ = (int) ( sqrtf(auto_max_width_* (float)auto_max_height_*(float)width_src/height_src) );
			height_ = (int) ( sqrtf(auto_max_width_* (float)auto_max_height_*(float)height_src/width_src) );	
		}
		else
		{

			float ratio = min((float)auto_max_width_/(float)width_src,(float)auto_max_height_/(float)height_src);
			
			width_ = (int) (width_src * ratio);
			height_ = (int) (height_src * ratio);
		}
	}
	return true;
}

bool ZoomImage::SavePaddingImage(const std::wstring& image_path, const std::wstring& image_path_out, int width, int height, std::wstring mime_type)
{
	bool ret = false;
	if (image_path.empty())
	{
		return false;
	}
	Gdiplus::Image image_src(image_path.c_str());
	if (image_src.GetLastStatus() != Gdiplus::Ok)
	{
		return false;
	}
	Gdiplus::REAL width_src = (REAL) image_src.GetWidth();
	Gdiplus::REAL height_src = (REAL) image_src.GetHeight();
	if (width_src > 0 && height_src > 0)
	{
		int fixed_w = width;
		int fixed_h = height;
		if (fixed_h > 0 && fixed_w > 0)
		{
			Gdiplus::REAL left = 0;
			Gdiplus::REAL top = 0;
			if (width_src * fixed_h > height_src * fixed_w)
			{
				left = width_src - (height_src * fixed_w / fixed_h);
				width_src -= left;
				left /= 2;
			}
			else
			{
				top = height_src - (width_src * fixed_h / fixed_w);
				height_src -= top;
				top /= 2;
			}

			HDC hdc = ::GetDC(NULL);
			HDC hMemDC = CreateCompatibleDC(hdc);
			HBITMAP hbmp = CreateCompatibleBitmap(hdc, fixed_w, fixed_h);
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbmp);
			Gdiplus::Graphics graphics(hMemDC);
			Gdiplus::RectF rectf(0.0f, 0.0f, (REAL)fixed_w, (REAL)fixed_h);
			graphics.DrawImage(&image_src, rectf, left, top, width_src, height_src, Gdiplus::UnitPixel);
			hbmp = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
			//清除
			DeleteDC(hMemDC);
			::ReleaseDC(NULL, hdc);
			Gdiplus::Bitmap bmp(hbmp, NULL);
			ret = SaveImage(bmp, image_path_out, mime_type, 100);
			DeleteObject(hbmp);
		}
	}
	return ret;
}

bool ZoomImage::GetImageCLSID(const wchar_t* format, CLSID* pCLSID)
{
	UINT num = 0; 
	UINT size = 0; 
	ImageCodecInfo* pImageCodecInfo = NULL; 
	GetImageEncodersSize(&num, &size); 

	if (size == 0) 
		return false; 

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL) 
		return false; 
	GetImageEncoders(num, size, pImageCodecInfo); 

	for (UINT i = 0; i < num; ++i) 
	{
		if ( wcscmp(pImageCodecInfo[i].MimeType, format) == 0 ) 
		{ 
			*pCLSID = pImageCodecInfo[i].Clsid; 
			free(pImageCodecInfo); 
			return true; 
		} 
	}
	free(pImageCodecInfo); 

	return false; 
}

bool ZoomImage::ConvertImageFormat(std::wstring& filepath,std::wstring mime_type)
{
	if( !nbase::FilePathIsExist(filepath, false) )
	{
		assert(0);
		return false;
	}

	Image image(filepath.c_str());
	if (image.GetLastStatus() != Ok)
	{
		ASSERT(0);
		return false;
	}
	return SaveImage(image,image_path_,mime_type);
}

bool ZoomImage::SaveImage(Gdiplus::Image& image, const std::wstring& file_path,std::wstring mime_type,long quality_num/* = 90*/)
{
	CLSID clsid;
	if (!GetImageCLSID(mime_type.c_str(),&clsid))
		return false;

	EncoderParameters encoder_paramters;
	encoder_paramters.Count = 1;
	encoder_paramters.Parameter[0].Guid = EncoderQuality;
	encoder_paramters.Parameter[0].NumberOfValues = 1;
	encoder_paramters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoder_paramters.Parameter[0].Value = (void*)&quality_num;
	Status status = image.Save(file_path.c_str(),&clsid,&encoder_paramters);
    if (status != Ok)
    {
		int err = status;
		QLOG_APP(L"SaveImage Error: {0}")<<err;
    }
	return status == Ok;
}

Gdiplus::RotateFlipType ZoomImage::GetRotateFlipType(Gdiplus::Image& image)
{
	short orientation = 0;
	UINT nSize		=	image.GetPropertyItemSize(PropertyTagOrientation);
	if (nSize > 0)
	{
		PropertyItem *property_item	=	(PropertyItem*) malloc( nSize );
		if (image.GetPropertyItem( PropertyTagOrientation, nSize, property_item ) == Ok)
		{
			orientation = *(short*)property_item->value;
		}

        free(property_item);
	}
	return GetRotateFlipType(orientation);
}

Gdiplus::RotateFlipType ZoomImage::GetRotateFlipType(short orientation)
{
	RotateFlipType type = RotateNoneFlipNone;
	switch(orientation)
	{
	case 1:
		break;
	case 2:
		type = RotateNoneFlipX;
		break;
	case 3:
		type = RotateNoneFlipXY;
		break;
	case 4:
		type = RotateNoneFlipY;
		break;
	case 5:
		type = Rotate90FlipX;
		break;
	case 6:
		type = Rotate90FlipNone;
		break;
	case 7:
		type = Rotate90FlipY;
		break;
	case 8:
		type = Rotate270FlipNone;
		break;
	default:
		break;
	}
	return type;
}

void ZoomImage::GetMimeType(const std::wstring& filepath, std::wstring& mime_type_out, std::wstring& exten_out)
{
	if (filepath.empty())
	{
		return;
	}
	Image image_src(filepath.c_str());
	if (image_src.GetLastStatus() != Ok)
	{
		return;
	}
	GUID guid;
	if (image_src.GetRawFormat(&guid) != Ok)
	{
		return;
	}
	mime_type_out = GetMimeType(guid);
	if (mime_type_out.empty())
	{
		return;
	}
	if (mime_type_out == kImageGIF)
	{
		exten_out = L".gif";
	}
	else if (mime_type_out == kImageJPEG)
	{
		exten_out = L".jpg";
	}
	else if (mime_type_out == kImagePNG)
	{
		exten_out = L".png";
	}
	else if (mime_type_out == kImageBMP)
	{
		exten_out = L".bmp";
	}
}

std::wstring ZoomImage::GetMimeType(const std::wstring& filepath)
{
	if (filepath.empty())
		return L"";
	Image image_src(filepath.c_str());
	Gdiplus::Status status = image_src.GetLastStatus();
	if(status != Ok)
	{
		QLOG_ERR(L"Image {0} error {1}") <<filepath <<status ;
		return L"";
	}
	GUID guid;
	if (image_src.GetRawFormat(&guid) != Ok)
	{
		QLOG_ERR(L"Image {0} error {1}") <<filepath <<status ;
		return L"";
	}
	return GetMimeType(guid);	
}

std::wstring ZoomImage::GetMimeType(GUID& clsid)
{
	std::wstring mime_type;
	if (clsid == ImageFormatGIF) //不支持GIF文件;
		mime_type = kImageGIF;
	else if (clsid == ImageFormatJPEG)
		mime_type = kImageJPEG;
	else if (clsid == ImageFormatPNG)
		mime_type = kImagePNG;
	else if (clsid == ImageFormatBMP)
		mime_type = kImageBMP;
	return mime_type;
}


bool CalculateImageSize(const std::wstring& file, SIZE& size, int max_width, int max_height)
{
	bool need_resize = false;
	if( file.empty() )
	{
		assert(0);
	}
	else if( nbase::FilePathIsExist(file, false) )
	{
		std::auto_ptr<Gdiplus::Image> img;
		img.reset( Gdiplus::Image::FromFile( file.c_str() ) );
		if(img.get() != NULL)
		{
			Gdiplus::RotateFlipType type = ZoomImage::GetRotateFlipType(*img);
			if (type != RotateNoneFlipNone) //旋转图片;
				img->RotateFlip(type);

			size.cx = img->GetWidth();
			size.cy = img->GetHeight();
			if (size.cx == 0 || size.cy == 0)
			{
				size.cx = max_width;
				size.cy = (int) (size.cx * 0.618);
				need_resize = true;
			}
			else
			{
				if (size.cx > max_width || size.cy > max_height)
				{
					float index_x = (float)max_width / (float)size.cx;
					float index_y = (float)max_height / (float)size.cy;
					float index = min(index_x, index_y);
					size.cx = (int)(size.cx * index);
					size.cy = (int)(size.cy * index);
					need_resize = true;
				}
			}
		}
	}
	else
	{
		assert(0);
	}
	return need_resize;
}

void ZoomImageF( const std::wstring &src, const std::wstring &dest, int cx, int cy )
{
	ZoomImage zoom;
	zoom.SetImagePath(dest);
	zoom.SetSize(cx, cy);
	zoom.Zoom(src);
}
