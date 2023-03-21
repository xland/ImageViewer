#pragma once
#include "include/core/SkCanvas.h"
#include <thread>
class Loading
{
public:
	Loading();
	~Loading();
	void Paint(SkCanvas* canvas);
	void Show();
	void Hide();
public:
private:
	float roateAngle{ 0.f };
	bool running{ false };
	std::thread rotateThread;
};

