#include "GifViewer.h"
#include "include/core/SkBitmap.h"
#include "MainWindow.h"
#include "BottomBar.h"
#include "App.h"
#include "FileHelper.h"
#include "Converter.h"

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
    if (frameImages.empty() || currentFrame >= frameImages.size()) return;
    if (!isCustomPosition) {
        //https://camo.githubusercontent.com/ffc8689dce447863f80b1404350456817531767dbe6c78b6a048b85fc999d288/68747470733a2f2f6c66332d7374617469632e62797465646e73646f632e636f6d2f6f626a2f6564656e2d636e2f70746c7a5f7a6c702f6c6a68775a74686c61756b6a6c6b756c7a6c702f73656d692d696e666f2d312e676966
        CaculatePosition(frameImages[currentFrame]); //28Õë»á³ö´í
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
            durations.push_back(frameInfo[frame].fDuration);
            auto dataPointer = bitmap->getPixels();
            codec->getPixels(imageInfo, dataPointer, imageInfo.minRowBytes(), option.get());
            frameImages.push_back(bitmap->asImage());
            if (!running) {
                break;
            }
            if (frame == 0) {
                animateThreadResult = std::async(&GifViewer::animateThread, this);
            }
        }
    },std::move(codec));
}
void GifViewer::animateThread()
{
    while (running)
    {
        if (currentFrame < frameCount && currentFrame >= frameImages.size())
        {
            continue;
        }
        if(currentFrame >= frameCount) 
        {
            currentFrame = 0;
        }
        App::get()->mainWindow->Refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(durations[currentFrame]));
        currentFrame += 1;
    }
}