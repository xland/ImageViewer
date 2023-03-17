#pragma once
#include "ImageViewer.h"
#include <thread>
#include <chrono>
#include <future>
#include "include/core/SkCanvas.h"
#include <mutex> 
#include <condition_variable>
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
	void animateThread();
	int frameCount{ 0 };
	unsigned currentFrame{ 0 };
	std::vector<int> durations;
	std::vector<sk_sp<SkImage>> frameImages;
	bool running{ true };
	std::shared_future<void> animateThreadResult;
	std::thread decodeThread;
	std::mutex locker;
	std::condition_variable cv;
};

