#include "GifViewer.h"
#include "include/core/SkBitmap.h"
#include "MainWindow.h"

GifViewer::GifViewer()
{

}
GifViewer::~GifViewer()
{
    running = false;
    if (decodeThread.joinable()) {
        decodeThread.join();
    }
    if (animateThreadResult.valid()) {
        animateThreadResult.wait();
    }    
}

void GifViewer::Paint(SkCanvas* canvas)
{
    if (frameImages.empty()) return;
    CaculatePosition(frameImages[currentFrame]);
    canvas->drawImageRect(frameImages[currentFrame], ImageRect, ImageOption);
    SkPaint paint;
    paint.setShader(shader);
    canvas->drawPaint(paint);
}
void GifViewer::DecodeGif(std::unique_ptr<SkCodec> codec)
{
    decodeThread = std::thread([this](std::unique_ptr<SkCodec> codec){
        auto imageInfo = codec->getInfo();
        frameCount = codec->getFrameCount();
        auto option = std::make_unique<SkCodec::Options>();
        auto frameInfo = codec->getFrameInfo();
        auto bitmap = std::make_unique<SkBitmap>();
        bitmap->setInfo(imageInfo);
        bitmap->allocPixels();
        auto totalTime = 0;
        for (unsigned frame = 0; frame < frameCount; frame++)
        {
            option->fFrameIndex = frame;
            auto duration = frameInfo[frame].fDuration;
            durations.push_back(duration);
            auto dataPointer = bitmap->getPixels();
            codec->getPixels(imageInfo, dataPointer, imageInfo.minRowBytes(), option.get());
            frameImages.push_back(bitmap->asImage());
            if (!running) {
                break;
            }
            if (frame == 0) {
                InvalidateRect(win->hwnd, nullptr, false);
                animateThreadResult = std::async(&GifViewer::animateThread, this);
            }
        }
    },std::move(codec));
}
void GifViewer::decodeGifFrame(std::unique_ptr<SkCodec> codec) {
        
}
void GifViewer::animateThread()
{
    while (running)
    {
        auto duration = durations[currentFrame];
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
        currentFrame += 1;
        if (currentFrame >= durations.size()) {
            if (currentFrame < frameCount) {
                currentFrame -= 1;
                continue;
            }
            currentFrame = 0;
        }
        InvalidateRect(win->hwnd, nullptr, false);
    }
}