#pragma once
#include <string>
#include <thread>
class ImageDownloader
{
public:
	ImageDownloader();
	~ImageDownloader();
	void ShowUrlDialog();
	void DownloadImage(std::wstring&& url);
public:
	bool abortDownloadFlag{ false };
	std::wstring imageUrl;
private:
	std::thread downloadThread;
};

