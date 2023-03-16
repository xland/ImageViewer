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
	void CaculatePosition(sk_sp<SkImage> image);
	void SaveImage(std::string& path);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="isBigger">
	/// 0 1:1
	/// 1 bigger
	/// 2 smaller
	/// </param>
	void Zoom(float scalNum);
	void Rotate();
	static void MakeImageViewer(std::string& path);
	virtual void Paint(SkCanvas* canvas);
	SkSamplingOptions ImageOption{ SkFilterMode::kLinear, SkMipmapMode::kLinear };
	SkRect ImageRect{ SkRect::MakeEmpty()};
	sk_sp<SkShader> shader;
	SkPaint paint;
	bool IsAutoSize{ true };
private:
	void initMaskShader();
	sk_sp<SkImage> image{nullptr};
	float scalNum = 1.f;
};

