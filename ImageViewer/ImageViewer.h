#pragma once
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/codec/SkCodec.h"
class MainWindow;
class ImageViewer
{
public:
	ImageViewer();
	~ImageViewer();
	static std::shared_ptr<ImageViewer> MakeImageViewer(const char* path, MainWindow* win);
	virtual void Paint(SkCanvas* canvas);
	MainWindow* win{ nullptr };
private:
	sk_sp<SkData> imageData{nullptr};
};

