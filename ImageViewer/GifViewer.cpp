#include "GifViewer.h"
#include "include/core/SkBitmap.h"
#include "MainWindow.h"
#include "BottomBar.h"
#include "App.h"
#include "FileHelper.h"
#include "Converter.h"
#include <future>
#include <condition_variable>
#include <math.h>

namespace {
    std::mutex locker;
    std::condition_variable cv;
}

GifViewer::GifViewer()
{
}
GifViewer::~GifViewer()
{
    running = false;
    cv.notify_all();
    if (decodeThread.joinable()) {
        decodeThread.join();
    }
}
void GifViewer::Zoom(float scalNum)
{
    auto win = App::get()->mainWindow.get();
    if (scalNum == 1.f) {
        //ImageRect = SkRect::Make(currentFrameImage->imageInfo().bounds());
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
    auto pathSrc = ConvertWideToUtf8(App::get()->fileHelper->currentPath.wstring());
    auto data = SkData::MakeFromFileName(pathSrc.c_str());
    SkFILEWStream fileStream(path.c_str());
    fileStream.write(data->data(), data->size());
    fileStream.flush();
}
void GifViewer::Paint(SkCanvas* canvas)
{
    std::unique_lock guard(locker);
    if (!isCustomPosition) {
        //CaculatePosition(currentFrameImage);
    }
    //canvas->drawImageRect(currentFrameImage, ImageRect, ImageOption);
    guard.unlock();
    SkPaint paint;
    paint.setShader(shader);
    canvas->drawPaint(paint);
}
void GifViewer::DecodeGif(std::unique_ptr<SkCodec> _codec)
{
    auto codec = _codec.release();
    frameCount = codec->getFrameCount();        
    auto frameInfo = codec->getFrameInfo();
    auto frameCountPerThread = 8;
    auto threadCount = (int)std::ceil(frameCount / frameCountPerThread);
    for (size_t i = 0; i <= threadCount; i++)
    {
        size_t index = i * frameCountPerThread;
        size_t endIndex = index + frameCountPerThread;
        if (endIndex >= frameCount) endIndex = frameCount - 1;
        auto decodeThread = std::thread([this](size_t index, size_t endIndex,SkCodec* codec) {
            for (; index < endIndex; index++)
            {
                SkCodec::Options option;
                option.fFrameIndex = index;
                //auto start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                auto imageInfo = codec->getInfo();
                std::unique_lock guard(locker);
                auto result = codec->getImage(imageInfo, &option);
                frames.insert({ index, std::get<0>(result) });
                guard.unlock();
                auto str = std::to_string(index) + "\r\n";
                OutputDebugStringA(str.c_str());
            }
        }, index, endIndex,codec);
        decodeThread.detach();
    }

    
}