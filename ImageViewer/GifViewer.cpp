#include "GifViewer.h"
#include "include/core/SkBitmap.h"
#include "MainWindow.h"
#include "BottomBar.h"
#include "App.h"
#include "FileHelper.h"
#include "Converter.h"
#include <urlmon.h>

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
void GifViewer::Zoom(float scalNum)
{
    if (frameImages.empty()) return;
    auto win = App::get()->mainWindow.get();
    if (scalNum == 1.f) {
        ImageRect = SkRect::Make(frameImages[0]->imageInfo().bounds());
    }
    App::get()->bottomBar->btnCodes[6] = (const char*)u8"\ue6f8";
    float w = ImageRect.width() * scalNum;
    float h = ImageRect.height() * scalNum;
    float x = ((float)win->clientWidth - w) / 2;
    float y = ((float)win->clientHeight - (float)win->bottomBarHeight - h) / 2;
    ImageRect.setXYWH(x, y, w, h);
    IsAutoSize = false;
    App::get()->mainWindow->Refresh();
}
void GifViewer::Rotate()
{

}
void GifViewer::SaveImage(std::string& path)
{
    if (!animateThreadResult.valid()) {
        //todo
        return;
    }
    auto pathSrc = ConvertWideToUtf8(App::get()->fileHelper->currentPath.wstring());
    auto data = SkData::MakeFromFileName(pathSrc.c_str());
    SkFILEWStream fileStream(path.c_str());
    fileStream.write(data->data(), data->size());
    fileStream.flush();
}
void GifViewer::Paint(SkCanvas* canvas)
{
    if (frameImages.empty()) return;
    if (!isCustomPosition) {
        CaculatePosition(frameImages[currentFrame]);
    }
    canvas->drawImageRect(frameImages[currentFrame], ImageRect, ImageOption);
    SkPaint paint;
    paint.setShader(shader);
    canvas->drawPaint(paint);
}
void GifViewer::DecodeGif(std::unique_ptr<SkCodec> codec)
{
    decodeThread = std::thread([this](std::unique_ptr<SkCodec> codec){
        auto win = App::get()->mainWindow.get();
        auto imageInfo = codec->getInfo();
        frameCount = codec->getFrameCount();
        auto option = std::make_unique<SkCodec::Options>();
        auto frameInfo = codec->getFrameInfo();
        auto bitmap = std::make_unique<SkBitmap>();
        bitmap->setInfo(imageInfo);
        bitmap->allocPixels();
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
        App::get()->mainWindow->Refresh();
    }
}