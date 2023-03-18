#pragma once
#include "ImageViewer.h"
#include <thread>
#include <chrono>
#include <future>
#include "include/core/SkCanvas.h"
#include <vector>
#include <map>
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
	std::map<int, sk_sp<SkImage>> frames;
	unsigned currentFrameIndex {0};
	unsigned frameCount{ 0 };
	bool running{ true };
	std::thread decodeThread;
};

