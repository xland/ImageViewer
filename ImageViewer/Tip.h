#pragma once
#include <memory>
#include <thread>
#include "include/core/SkCanvas.h"
class Tip
{
public:	
	Tip();
	~Tip();
	void Paint(SkCanvas* canvas);
	void Show(std::wstring&& tipText);
private:
	bool visible{ false };
	std::wstring tipText;
	float textX{ 0.f };
	float textY{ 0.f };
	size_t textLength{0};
	unsigned remainTime{ 0 };
	std::shared_ptr<std::thread> waitingTread{ nullptr };
	void waitingFunc();
};

