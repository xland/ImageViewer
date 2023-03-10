#include "ImageViewer.h"
#include "GifViewer.h"
#include "MainWindow.h"
ImageViewer::ImageViewer()
{

}
ImageViewer::~ImageViewer()
{

}
void ImageViewer::Paint(SkCanvas* canvas)
{
	auto skImage = SkImage::MakeFromEncoded(imageData);
	canvas->drawImage(skImage, 0, 0);
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
		gifViewer->win = win;
		gifViewer->DecodeGif(std::move(codec));
		result = gifViewer;
	}
	else
	{
		result = std::make_shared<ImageViewer>();
		result->imageData = skData;
		result->win = win;
	}
	return result;
}