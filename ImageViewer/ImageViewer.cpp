#include "ImageViewer.h"
#include "GifViewer.h"
#include "MainWindow.h"
ImageViewer::ImageViewer()
{

}
ImageViewer::~ImageViewer()
{

}
std::pair<unsigned, unsigned> ImageViewer::CaculatePosition(sk_sp<SkImage> image)
{
	int x = (win->clientWidth - image->width()) / 2;
	int y = (win->clientHeight - win->toolBarHeight - image->height()) / 2;
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	return { x,y };
}
void ImageViewer::Paint(SkCanvas* canvas)
{
	auto [x,y] = CaculatePosition(image);
	canvas->drawImage(image, x, y);
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