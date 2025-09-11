#include "char/Character.h"
#include "char/CharacterTest.h"
#include "obj/Object.h"
#include "rndobj/Dir.h"

Character::Character()
    : mLods(this), mLastLod(0), mForceLod(kLOD0), mShadow(this), mTranslucent(this),
      mDriver(0), mSelfShadow(0), unk251(0), unk252(1), mSphereBase(this, this),
      unk2a0(this), mShowableProps(this) {}

Character::~Character() { UnhookShadow(); }

BEGIN_CUSTOM_PROPSYNC(Character::Lod)
    SYNC_PROP(screen_size, o.mScreenSize)
    SYNC_PROP(opaque, o.mOpaque)
    SYNC_PROP(translucent, o.mTranslucent)
END_CUSTOM_PROPSYNC

BEGIN_PROPSYNCS(Character)
    SYNC_PROP_SET(
        sphere_base, mSphereBase.Ptr(), SetSphereBase(_val.Obj<RndTransformable>())
    )
    SYNC_PROP(lods, mLods)
    SYNC_PROP(force_lod, (int &)mForceLod)
    SYNC_PROP(translucent, mTranslucent)
    SYNC_PROP(self_shadow, mSelfShadow)
    SYNC_PROP(bounding, mBounding)
    SYNC_PROP(frozen, mFrozen)
    SYNC_PROP(shadow, mShadow)
    SYNC_PROP_SET(driver, mDriver, )
    SYNC_PROP_MODIFY(
        interest_to_force, mInterestToForce, SetFocusInterest(mInterestToForce, 0)
    )
    SYNC_PROP(showable_props, mShowableProps)
    SYNC_PROP(debug_draw_interest_objects, mDebugDrawInterestObjects)
    SYNC_PROP(CharacterTesting, *mTest)
    SYNC_SUPERCLASS(RndDir)
END_PROPSYNCS
