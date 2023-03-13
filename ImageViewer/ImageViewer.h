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
	void CaculatePosition(sk_sp<SkImage> image);
	static std::shared_ptr<ImageViewer> MakeImageViewer(const char* path, MainWindow* win);
	virtual void Paint(SkCanvas* canvas);
	MainWindow* win{ nullptr };
	SkSamplingOptions ImageOption{ SkFilterMode::kLinear, SkMipmapMode::kLinear };
	SkRect ImageRect{ SkRect::MakeEmpty()};
	sk_sp<SkShader> shader;
	SkPaint paint;
private:
	sk_sp<SkImage> image{nullptr};
};

