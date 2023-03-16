#pragma once
#include "include/core/SkCanvas.h"
#include <sstream>
#include <filesystem>
class MainWindow;
class BottomBar
{
public:
	BottomBar();
	~BottomBar();
	void Paint(SkCanvas* canvas);
	void CheckMouseEnter(int mouseX, int mouseY);
	void CheckMouseUp(int mouseX, int mouseY);
	void loopFile(bool isNext);
	std::vector<const char*> btnCodes{ (const char*)u8"\ueabe"
		,(const char*)u8"\ue6bf"
		,(const char*)u8"\ue6ad"
		,(const char*)u8"\ue6bd"
		,(const char*)u8"\ue6c0"
		,(const char*)u8"\ue6be"
		,(const char*)u8"\ue6c2"
		,(const char*)u8"\ue6c1" };
private:
	std::filesystem::path imagePath{};
	std::string openFileDialog(bool isSave);
	/// <summary>
	/// 0 show nothing
	/// 1 show first
	/// 2 show last
	/// </summary>
	unsigned lastFirstFlag{ 0 };
	unsigned lastFirstWaitingTime{ 2000 };
	std::shared_ptr<std::thread> lastFirstWaitingTread{nullptr};
	void lastFirstWaitingFunc();



	unsigned fontSize{ 22 };
	float btnWidth{ 68.f };
	float x{ 0.f };
	float y{ 0.f };
	float w{ 8*btnWidth };
	int mouseEnterIndex{ -1 };
};

