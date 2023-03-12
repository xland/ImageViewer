#include "ImageViewer.h"
#include "GifViewer.h"
#include "MainWindow.h"
ImageViewer::ImageViewer()
{

}
ImageViewer::~ImageViewer()
{

}
SkRect ImageViewer::CaculatePosition(sk_sp<SkImage> image)
{
	SkRect result;
	auto imageWidth = (float)image->width();
	auto imageHeight = (float)image->height();
	auto clientWidth = (float)win->clientWidth;
	auto clientHeight = (float)(win->clientHeight - win->toolBarHeight);
	float x=0.f, y = 0.f, w = 0.f, h = 0.f;
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
			h = imageHeight/widthRatio;
			w = clientWidth;
			y = (clientHeight - h)/2;
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
	result.setXYWH(x, y, w, h);
	return result;
}
void ImageViewer::Paint(SkCanvas* canvas)
{
	auto rect = CaculatePosition(image);
	SkSamplingOptions option;
	canvas->drawImageRect(image, rect,option);
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