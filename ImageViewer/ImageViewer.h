#pragma once
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/codec/SkCodec.h"
class MainWindow;
class ImageViewer
{
public:
	ImageViewer();
	~ImageViewer();
	void SetCustomSizeRect();
	void SetAutoSizeRect();
	void CheckMouseEnter(int x, int y);
	void CheckMouseDown(int x, int y);
	void CheckMouseUp(int x, int y);
	virtual void SaveImage(std::string& path);
	void AutoSize();
	virtual void Zoom(float scalNum);
	virtual void Rotate();
	static void MakeImageViewer(std::string& path);
	virtual void Paint(SkCanvas* canvas);
public:
	SkSamplingOptions ImageOption{ SkFilterMode::kLinear, SkMipmapMode::kLinear };
	SkRect ImageRect{ SkRect::MakeEmpty()};
	sk_sp<SkShader> shader;
	SkPaint paint;
	sk_sp<SkImage> image{ nullptr };
	bool IsAutoSize{ true };
	bool IsAutoPosition{ true };
private:
	void initMaskShader();
private:
	bool isMouseEnter{ false };
	bool isMouseDown{ false };
	float xSpan{ 0.f };
	float ySpan{ 0.f };
	float scalNum = 1.f;
};

