#include "GifViewer.h"
#include "include/core/SkBitmap.h"
#include "MainWindow.h"

GifViewer::GifViewer()
{

}
GifViewer::~GifViewer()
{

}
void GifViewer::Paint(SkCanvas* canvas)
{
    auto dataPointer = bitmap->getPixels();
    codec->getPixels(imageInfo, dataPointer, imageInfo.minRowBytes(), option.get());    
    canvas->drawImage(bitmap->asImage(), 0, 0);
    if (!running) {
        running = true;
        animateThreadResult = std::async(&GifViewer::animateThread, this);
    }
}
void GifViewer::DecodeGif(std::unique_ptr<SkCodec> _codec)
{
    codec = std::move(_codec);
    imageInfo = codec->getInfo();
    bitmap = std::make_shared<SkBitmap>();
    bitmap->setInfo(imageInfo);
    bitmap->allocPixels();
	frameCount = codec->getFrameCount();
    option = std::make_shared<SkCodec::Options>();
    option->fFrameIndex = 0;
    auto frameInfo = codec->getFrameInfo();
    for (unsigned frame = 0; frame < frameCount; frame++)
    {
        auto duration = frameInfo[frame].fDuration;
        durations.push_back(duration);        
    }
    
}
void GifViewer::animateThread()
{
    while (running)
    {
        auto duration = durations[option->fFrameIndex];
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
        option->fFrameIndex += 1;
        if (option->fFrameIndex >= durations.size()) {
            option->fFrameIndex = 0;
        }
        InvalidateRect(win->hwnd, nullptr, false);
    }
}