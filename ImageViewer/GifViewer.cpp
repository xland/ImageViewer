#include "GifViewer.h"
#include "include/core/SkBitmap.h"
#include "MainWindow.h"
#include "BottomBar.h"
#include "App.h"
#include "FileHelper.h"
#include "Converter.h"
#include <urlmon.h>
namespace {
    std::mutex locker;
    bool copy_to(SkBitmap* dst, SkColorType dstColorType, const SkBitmap& src) {
        SkPixmap srcPM;
        if (!src.peekPixels(&srcPM)) {
            return false;
        }

        SkBitmap    tmpDst;
        SkImageInfo dstInfo = srcPM.info().makeColorType(dstColorType);
        if (!tmpDst.setInfo(dstInfo)) {
            return false;
        }

        if (!tmpDst.tryAllocPixels()) {
            return false;
        }

        SkPixmap dstPM;
        if (!tmpDst.peekPixels(&dstPM)) {
            return false;
        }

        if (!srcPM.readPixels(dstPM)) {
            return false;
        }

        dst->swap(tmpDst);
        return true;
    }
}
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
    if (!frameImage) return;
    auto win = App::get()->mainWindow.get();
    if (scalNum == 1.f) {
        ImageRect = SkRect::Make(frameImage->info().bounds());
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

    if (!frameImage) return;
    std::unique_lock guard(locker);
    auto image = frameImage->asImage();
    if (!isCustomPosition) {
        CaculatePosition(image);
    }
    canvas->drawImageRect(image, ImageRect, ImageOption);
    guard.unlock();
    SkPaint paint;
    paint.setShader(shader);
    canvas->drawPaint(paint);
}
void GifViewer::DecodeGif(std::unique_ptr<SkCodec> codec)
{
    decodeThread = std::thread([this](std::unique_ptr<SkCodec> codec){       
        auto frameCount = codec->getFrameCount();
        SkCodec::Options option;
        option.fFrameIndex = 0;
        option.fPriorFrame = -1;
        auto imageInfo = codec->getInfo().makeColorType(kN32_SkColorType);
        auto frameInfo = codec->getFrameInfo();
        while (running)
        {
            auto start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();            
            std::shared_ptr<SkBitmap> image = std::make_shared<SkBitmap>();
            image->allocPixels(imageInfo);
            if (option.fFrameIndex != 0)
            {
                copy_to(image.get(), frameImage->colorType(), *frameImage.get());
            }
            codec->getPixels(imageInfo, image->getPixels(), image->rowBytes(), &option);
            std::unique_lock guard(locker);
            frameImage = image;
            guard.unlock();
            App::get()->mainWindow->Refresh();
            auto end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            auto span = frameInfo[option.fFrameIndex].fDuration - (end - start);
            std::this_thread::sleep_for(std::chrono::milliseconds(span));
            if (option.fFrameIndex == frameCount - 1)
            {
                option.fPriorFrame = -1;
                option.fFrameIndex = 0;
            }
            else
            {
                option.fPriorFrame = option.fPriorFrame + 1;
                option.fFrameIndex = option.fFrameIndex + 1;
            }            
        }
    },std::move(codec));
}