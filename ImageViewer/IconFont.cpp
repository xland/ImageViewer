#include "IconFont.h"
#include "resource.h"
#include <Windows.h>
#include "include/core/SkData.h"
namespace {
	static std::shared_ptr<SkFont> font;
}
namespace IconFont {
	void Init()
	{
		HMODULE instance = GetModuleHandle(NULL);
		HRSRC resID = FindResource(instance, MAKEINTRESOURCE(IDR_BTNFONT1), L"BTNFONT");
		if (resID == 0) {
			return;
		}
		size_t resSize = SizeofResource(instance, resID);
		HGLOBAL res = LoadResource(instance, resID);
		if (res == 0) {
			return;
		}
		void* resPointer = LockResource(res);
		auto fontData = SkData::MakeWithoutCopy(resPointer, resSize);
		auto fontTypeFace = SkTypeface::MakeFromData(fontData);
		font = std::make_shared<SkFont>(fontTypeFace);
	}
	SkFont* Get()
	{
		return font.get();
	}
}