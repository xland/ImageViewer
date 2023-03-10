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
	
private:
	void animateThread();
	unsigned frameCount{ 0 };
	std::vector<int> durations;
	std::shared_ptr<SkBitmap> bitmap;
	SkImageInfo imageInfo;
	std::unique_ptr<SkCodec> codec;
	std::shared_ptr<SkCodec::Options> option;
	bool running{ false };
	std::shared_future<void> animateThreadResult;
};

