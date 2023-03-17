#pragma once
#include <string>
class ImageDownloader
{
public:
	ImageDownloader();
	~ImageDownloader();
	void ShowUrlDialog();
	void DownloadImage(std::wstring&& url);
private:

};

