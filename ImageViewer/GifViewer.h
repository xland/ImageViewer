#pragma once
#include "ImageViewer.h"
#include <thread>
#include <chrono>
#include <future>
#include "include/core/SkCanvas.h"
class GifViewer:public ImageViewer
{
public:
	GifViewer();
	~GifViewer();
	void Paint(SkCanvas* canvas);
	void DecodeGif(std::unique_ptr<SkCodec> codec);
	void Zoom(float scalNum) override;	
	void Rotate() override;
	void SaveImage(std::string& path) override;
private:
	std::shared_ptr<SkBitmap> frameImage;
	bool running{ true };
	std::thread decodeThread;
};

