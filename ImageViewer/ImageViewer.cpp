#include "ImageViewer.h"
#include "GifViewer.h"
#include "MainWindow.h"
#include "include/core/SkFont.h"
#include "include/core/SkColorFilter.h"
#include "include/core/SkMaskFilter.h"
#include "BottomBar.h"
#include "Color.h"
#include <math.h>
#include "App.h"

ImageViewer::ImageViewer()
{
	initMaskShader();
}
ImageViewer::~ImageViewer()
{

}
void ImageViewer::CheckMouseUp(int x, int y)
{
	isMouseDown = false;
}
void ImageViewer::CheckMouseDown(int x, int y)
{
	if (!isMouseEnter) return;
	xSpan = (float)x - ImageRect.x();
	ySpan = (float)y - ImageRect.y();
	isMouseDown = true;
}
void ImageViewer::CheckMouseEnter(int x, int y)
{
	if (ImageRect.isEmpty()) return;
	if (isMouseDown) {

		float xTemp = ImageRect.x();
		float yTemp = ImageRect.y();
		float x1 = (float)x - xSpan;
		float y1 = (float)y - ySpan;
		float w = ImageRect.width();
		float h = ImageRect.height();
		auto win = App::get()->mainWindow.get();
		int edge = 88;
		if (w + x1 < edge || h + y1 < edge || win->clientWidth - x1 < edge || win->clientHeight-win->bottomBarHeight - y1 < edge) return;
		ImageRect.setXYWH(x1, y1, w, h);
		isCustomPosition = true;
		win->Refresh();
		return;
	}
	if (x > ImageRect.x() && x < ImageRect.right() && y > ImageRect.y() && y < ImageRect.bottom()) {
		isMouseEnter = true;
	}
	else
	{
		isMouseEnter = false;
	}
}
void ImageViewer::AutoSize()
{
	if (!image) return;
	IsAutoSize = true;
	isCustomPosition = false;
	App::get()->mainWindow->Refresh();
}
void ImageViewer::initMaskShader()
{
	SkBitmap bitmap;
	//auto info = SkImageInfo::Make(300, 300, kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
	//bitmap.allocPixels(info);
	bitmap.allocN32Pixels(300, 300);
	SkCanvas canvas(bitmap);
	auto font = App::get()->textFont;
	font->setSize(16.f);
	SkPaint paint;
	paint.setColor(GetColor(187, 187, 187, 50));
	paint.setAntiAlias(true);
	std::wstring Text = L"ÁõÏþÂ×";
	canvas.drawSimpleText(Text.data(), wcslen(Text.data()) * 2, SkTextEncoding::kUTF16, 90.f, 60.f, *font, paint);
	bitmap.setImmutable();
	sk_sp<SkImage> mask = bitmap.asImage();
	SkMatrix matrix;
	matrix.preRotate(-45.f);
	shader = mask->makeShader(SkTileMode::kRepeat, SkTileMode::kRepeat, ImageOption, matrix);
}
void ImageViewer::Zoom(float scalNum)
{
	if (!image) return;
	auto win = App::get()->mainWindow.get();
	if (scalNum == 1.f) {
		ImageRect = SkRect::Make(image->imageInfo().bounds());
	}	
	float w = ImageRect.width() * scalNum;
	float h = ImageRect.height() * scalNum;
	float x = ((float)win->clientWidth - w) / 2;
	float y = ((float)win->clientHeight - (float)win->bottomBarHeight - h) / 2;
	ImageRect.setXYWH(x, y, w, h);
	isCustomPosition = false;
	IsAutoSize = false;
	App::get()->mainWindow->Refresh();
}
void ImageViewer::Rotate()
{
	IsAutoSize = true;
	SkBitmap bitmap;
	bitmap.allocN32Pixels(image->height(), image->width());
	SkCanvas canvas(bitmap);
	canvas.translate(0.f, (float)bitmap.height());
	canvas.rotate(-90.f);
	canvas.drawImage(image, 0.f, 0.f);
	bitmap.setImmutable();
	image = bitmap.asImage();
	App::get()->mainWindow->Refresh();
}
void ImageViewer::CaculatePosition(sk_sp<SkImage> image)
{ 	
	auto win = App::get()->mainWindow.get();
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
				y = 0.f;
				x = (clientWidth - w) / 2;
			}
			else
			{
				h = imageHeight / widthRatio;
				w = clientWidth;
				y = (clientHeight - h) / 2;
				x = 0.f;
			}
		}
		else if (imageHeight > clientHeight && imageWidth <= clientWidth)
		{
			float heightRatio = imageHeight / clientHeight;
			h = clientHeight;
			w = imageWidth / heightRatio;
			y = 0.f;
			x = (clientWidth - w) / 2;
		}
		else if (imageHeight <= clientHeight && imageWidth > clientWidth) {
			auto widthRatio = imageWidth / clientWidth;
			h = imageHeight / widthRatio;
			w = clientWidth;
			y = (clientHeight - h) / 2;
			x = 0.f;
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
	if (!isCustomPosition) {
		CaculatePosition(image); 
	}	
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
void ImageViewer::MakeImageViewer(std::string& path)
{
	auto skData = SkData::MakeFromFileName(path.c_str());
	std::unique_ptr<SkCodec> codec = SkCodec::MakeFromData(skData);
	if (!codec) {
		//todo log
		//todo svg and other format
		return;
	}
	auto imgFormat = codec->getEncodedFormat();
	auto app = App::get();
	if (imgFormat == SkEncodedImageFormat::kGIF) {
		auto gifViewer = new GifViewer();		
		gifViewer->DecodeGif(std::move(codec));
		app->imageViewer.reset(gifViewer);
	}
	else
	{
		auto imageViewer = new ImageViewer();
		imageViewer->image = SkImage::MakeFromEncoded(skData);
		app->imageViewer.reset(imageViewer);
	}
	auto& code = std::get<1>(app->bottomBar->btns[6]);
	code = (const char*)u8"\ue6be";
	app->mainWindow->Refresh();

}