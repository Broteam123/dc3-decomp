#pragma once
#include "obj/Object.h"
#include "synth/MoggClip.h"
#include "utl/BinStream.h"

class MoggClipMap {
public:
    MoggClipMap(Hmx::Object *);
    MoggClipMap(const MoggClipMap &);
    virtual ~MoggClipMap() {}

    void mySave(BinStream &) const;
    void myLoad(BinStreamRev &);
    MoggClipMap &operator=(const MoggClipMap &);

protected:
    /** "Which moggclip to play" */
    ObjPtr<MoggClip> mMoggClip; // 0x4
    /** "Surround pan, between -4 and 4" */
    float mPan; // 0x18
    /** "Surround pan width, between 0 and 4" */
    float mPanWidth; // 0x1c
    /** "Volume in dB (0 is full volume, -96 is silence)" */
    float mVolume; // 0x20
    /** "Is the mogg clip stereo?" */
    bool mIsStereo; // 0x24
};

BinStream &operator<<(BinStream &, const MoggClipMap &);
BinStream &operator>>(BinStreamRev &, MoggClipMap &);
