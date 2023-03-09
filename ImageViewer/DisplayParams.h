#pragma once
#include "include/core/SkImageInfo.h"
#include "include/core/SkSurfaceProps.h"
#include "include/gpu/GrContextOptions.h"
class DisplayParams {
public:
    DisplayParams()
        : colorType(kN32_SkColorType)
        , fColorSpace(nullptr) 
        , fMSAASampleCount(8)  //��Ҫ
        , fSurfaceProps(0, kRGB_H_SkPixelGeometry)
        , fDisableVsync(false)
        , fDelayDrawableAcquisition(false)
        , fEnableBinaryArchive(false)
    {}
    /// <summary>
    /// ��ɫ���ͣ�Ĭ��ΪBGRA���е����
    /// </summary>
    SkColorType         colorType;
    /// <summary>
    /// ��ɫ�ռ䣬��֪����ɶ�ģ�����ָ��
    /// </summary>
    sk_sp<SkColorSpace> fColorSpace;
    /// <summary>
    /// ����
    /// </summary>
    int                 fMSAASampleCount;
    GrContextOptions    fGrContextOptions;
    SkSurfaceProps      fSurfaceProps;
    bool                fDisableVsync;
    bool                fDelayDrawableAcquisition;
    bool                fEnableBinaryArchive;
};