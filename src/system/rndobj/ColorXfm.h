#pragma once
#include "math/Color.h"
#include "math/Mtx.h"
#include "utl/BinStream.h"

class RndColorXfm {
public:
    RndColorXfm();
    void Reset();
    void AdjustHue();
    void AdjustSaturation();
    void AdjustLightness();
    void AdjustContrast();
    void AdjustBrightness();
    void AdjustLevels();
    void AdjustColorXfm();
    void Save(BinStream &) const;
    bool Load(BinStream &);

    float &Hue() { return mHue; }
    float &Saturation() { return mSaturation; }
    float &Lightness() { return mLightness; }
    float &Contrast() { return mContrast; }
    float &Brightness() { return mBrightness; }
    Hmx::Color &LevelInLo() { return mLevelInLo; }
    Hmx::Color &LevelInHi() { return mLevelInHi; }
    Hmx::Color &LevelOutLo() { return mLevelOutLo; }
    Hmx::Color &LevelOutHi() { return mLevelOutHi; }
    const Transform &ColorXfm() const { return mColorXfm; }

    RndColorXfm &operator=(const RndColorXfm &c) {
        memcpy(this, &c, sizeof(*this));
        return *this;
    }

private:
    float mHue; // 0x0
    float mSaturation; // 0x4
    float mLightness; // 0x8
    float mContrast; // 0xc
    float mBrightness; // 0x10
    Hmx::Color mLevelInLo; // 0x14
    Hmx::Color mLevelInHi; // 0x24
    Hmx::Color mLevelOutLo; // 0x34
    Hmx::Color mLevelOutHi; // 0x44
    Transform mColorXfm; // 0x54
};

// inline BinStream &operator>>(BinStream &bs, RndColorXfm &xfm) {
//     xfm.Load(bs);
//     return bs;
// }
