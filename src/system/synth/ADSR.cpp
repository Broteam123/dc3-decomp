#include "synth/ADSR.h"
#include "obj/Object.h"
#include "utl/BinStream.h"

ADSRImpl::ADSRImpl()
    : mAttackRate(0.001f), mDecayRate(0.0001f), mSustainRate(0.001f),
      mReleaseRate(0.005f), mSustainLevel(1.0f), mAttackMode(kAttackExp),
      mSustainMode(kSustainLinInc), mReleaseMode(kReleaseLinear), mSynced(0) {}

float ADSRImpl::GetAttackRate() const { return mAttackRate; }
float ADSRImpl::GetReleaseRate() const { return mReleaseRate; }

ADSR::ADSR() : mADSR() {}

BEGIN_COPYS(ADSR)
    COPY_SUPERCLASS(Hmx::Object)
    CREATE_COPY(ADSR)
    BEGIN_COPYING_MEMBERS
        if (ty != kCopyFromMax) {
            COPY_MEMBER(mADSR)
        }
    END_COPYING_MEMBERS
END_COPYS

void ADSRImpl::Save(BinStream &bs) const {
    bs << 1;
    bs << mAttackRate << mDecayRate << mSustainRate << mReleaseRate << mSustainLevel;
    bs << mAttackMode << mSustainMode << mReleaseMode;
}

BinStream &operator<<(BinStream &bs, const ADSRImpl &adsr) {
    adsr.Save(bs);
    return bs;
}

BinStream &operator>>(BinStream &bs, ADSRImpl &adsr) {
    adsr.Load(bs, nullptr);
    return bs;
}

void ADSR::Save(BinStream &bs) { mADSR.Save(bs); }
void ADSR::Load(BinStream &bs) { mADSR.Load(bs, this); }

BEGIN_PROPSYNCS(ADSR)
    SYNC_SUPERCLASS(Hmx::Object)
    SYNC_PROP(attack_mode, (int &)mADSR.mAttackMode)
    SYNC_PROP(attack_rate, mADSR.mAttackRate)
    SYNC_PROP(decay_rate, mADSR.mDecayRate)
    SYNC_PROP(sustain_mode, (int &)mADSR.mSustainMode)
    SYNC_PROP(sustain_rate, mADSR.mSustainRate)
    SYNC_PROP(sustain_level, mADSR.mSustainLevel)
    SYNC_PROP(release_mode, (int &)mADSR.mReleaseMode)
    SYNC_PROP(release_rate, mADSR.mReleaseRate)
END_PROPSYNCS
