#include "ImageViewer.h"
#include "GifViewer.h"
#include "MainWindow.h"
#include "include/core/SkFont.h"
#include "include/core/SkColorFilter.h"
#include "include/core/SkMaskFilter.h"
#include "Color.h"
#include <math.h>

//todo 不支持svg

ImageViewer::ImageViewer()
{
	initMaskShader();
}
ImageViewer::~ImageViewer()
{

}
void ImageViewer::initMaskShader()
{
	SkBitmap bitmap;
	//auto info = SkImageInfo::Make(300, 300, kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
	//bitmap.allocPixels(info);
	bitmap.allocN32Pixels(300, 300);
	SkCanvas canvas(bitmap);
	SkFont font(SkTypeface::MakeFromName("Microsoft YaHei", SkFontStyle::Normal()), 16);
	SkPaint paint;
	paint.setColor(GetColor(187, 187, 187, 50));
	paint.setAntiAlias(true);
	std::wstring Text = L"刘晓伦";
	canvas.drawSimpleText(Text.data(), wcslen(Text.data()) * 2, SkTextEncoding::kUTF16, 90, 60, font, paint);
	bitmap.setImmutable();
	sk_sp<SkImage> mask = bitmap.asImage();
	SkMatrix matrix;
	matrix.preRotate(-45.f);
	shader = mask->makeShader(SkTileMode::kRepeat, SkTileMode::kRepeat, ImageOption, matrix);
}
void ImageViewer::Zoom(float scalNum)
{
	if (scalNum == 1.f) {
		ImageRect = SkRect::Make(image->imageInfo().bounds());
	}
	win->bottomBar->btnCodes[5] = (const char*)u8"\ue6f8";
	float w = ImageRect.width() * scalNum;
	float h = ImageRect.height() * scalNum;
	float x = ((float)win->clientWidth - w) / 2;
	float y = ((float)win->clientHeight - (float)win->bottomBarHeight - h) / 2;
	ImageRect.setXYWH(x, y, w, h);
	IsAutoSize = false;
	InvalidateRect(win->hwnd, nullptr, false);	
}
void ImageViewer::Rotate()
{
	IsAutoSize = true;
	SkBitmap bitmap;
	bitmap.allocN32Pixels(image->height(), image->width());
	SkCanvas canvas(bitmap);
	canvas.translate(0, bitmap.height());
	canvas.rotate(-90);
	canvas.drawImage(image, 0, 0);
	bitmap.setImmutable();
	image = bitmap.asImage();
	InvalidateRect(win->hwnd, nullptr, false);
}
void ImageViewer::CaculatePosition(sk_sp<SkImage> image)
{ 	
	auto clientWidth = (float)win->clientWidth;
	auto clientHeight = (float)(win->clientHeight - win->bottomBarHeight);
	if (IsAutoSize) {
		auto imageWidth = (float)image->width();
		auto imageHeight = (float)image->height();
		float x = 0.f, y = 0.f, w = 0.f, h = 0.f;
		if (imageHeight > clientHeight && imageWidth > clientWidth)
		{
			auto heightRatio = imageHeight / clientHeight;
			auto widthRatio = imageWidth / clientWidth;
			if (heightRatio > widthRatio) {
				h = clientHeight;
				w = imageWidth / heightRatio;
				y = 0;
				x = (clientWidth - w) / 2;
			}
			else
			{
				h = imageHeight / widthRatio;
				w = clientWidth;
				y = (clientHeight - h) / 2;
				x = 0;
			}
		}
		else if (imageHeight > clientHeight && imageWidth <= clientWidth)
		{
			float heightRatio = imageHeight / clientHeight;
			h = clientHeight;
			w = imageWidth / heightRatio;
			y = 0;
			x = (clientWidth - w) / 2;
		}
		else if (imageHeight <= clientHeight && imageWidth > clientWidth) {
			auto widthRatio = imageWidth / clientWidth;
			h = imageHeight / widthRatio;
			w = clientWidth;
			y = (clientHeight - h) / 2;
			x = 0;
		}
		else
		{
			x = (clientWidth - imageWidth) / 2;
			y = (clientHeight - imageHeight) / 2;
			w = imageWidth;
			h = imageHeight;
		}
		ImageRect.setXYWH(x, y, w, h);
	}
	else
	{
		float w = ImageRect.width();
		float h = ImageRect.height();
		float x = ((float)win->clientWidth - w) / 2;
		float y = ((float)win->clientHeight - (float)win->bottomBarHeight - h) / 2;
		ImageRect.setXYWH(x, y, w, h);
	}
	
}
void ImageViewer::Paint(SkCanvas* canvas)
{
	CaculatePosition(image); //todo 有些时候没必要做这个计算
	canvas->drawImageRect(image, ImageRect, ImageOption);
	SkPaint paint;	
	paint.setShader(shader);
	canvas->drawPaint(paint);
}
void ImageViewer::SaveImage(std::string& path)
{
	auto data = image->encodeToData();
	SkFILEWStream fileStream(path.c_str());
	fileStream.write(data->data(), data->size());
	fileStream.flush();
}
std::shared_ptr<ImageViewer> ImageViewer::MakeImageViewer(const char* path,MainWindow* win)
{
	auto skData = SkData::MakeFromFileName(path);
	std::unique_ptr<SkCodec> codec = SkCodec::MakeFromData(skData);
	if (!codec) {
		//todo log
		return nullptr;
	}
	auto imgFormat = codec->getEncodedFormat();
	std::shared_ptr<ImageViewer> result;
	if (imgFormat == SkEncodedImageFormat::kGIF) {
		auto gifViewer = std::make_shared<GifViewer>();		
		gifViewer->DecodeGif(std::move(codec));
		result = gifViewer;
	}
	else
	{
		result = std::make_shared<ImageViewer>();
		result->image = SkImage::MakeFromEncoded(skData);
	}
	result->win = win;	
	return result;
}