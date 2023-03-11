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
	void decodeGifFrame(std::unique_ptr<SkCodec> _codec);
	int frameCount{ 0 };
	unsigned currentFrame{ 0 };
	std::vector<int> durations;
	std::vector<sk_sp<SkImage>> frameImages;
	bool running{ true };
	std::shared_future<void> animateThreadResult;
};

