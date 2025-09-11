#include "char/Character.h"
#include "char/CharacterTest.h"
#include "obj/Object.h"
#include "rndobj/Dir.h"
#include "rndobj/Trans.h"
#include "utl/BinStream.h"

Character *Character::sCurrent;

Character::Character()
    : mLods(this), mLastLod(0), mForceLod(kLOD0), mShadow(this), mTranslucent(this),
      mDriver(0), mSelfShadow(0), unk251(0), unk252(1), mSphereBase(this, this),
      mBounding(Vector3(0, 0, 0), 0), unk288(0), mTest(new CharacterTest(this)),
      mFrozen(0), unk294(3), unk298(1), unk2a0(this), mShowableProps(this),
      mDebugDrawInterestObjects(false) {}

Character::~Character() {
    UnhookShadow();
    delete mTest;
}

BEGIN_HANDLERS(Character)
END_HANDLERS

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

// class BinStream & __cdecl operator<<(class BinStream &, struct Character::Lod const &)
BinStream &operator<<(BinStream &bs, const Character::Lod &lod) {
    bs << lod.mScreenSize;
    bs << lod.mOpaque;
    bs << lod.mTranslucent;
    return bs;
}

BEGIN_SAVES(Character)
    SAVE_REVS(0x15, 0)
    SAVE_SUPERCLASS(RndDir)
    if (!IsProxy()) {
        bs << mLods;
        bs << mShadow;
        bs << mSelfShadow;
        ObjPtr<RndTransformable> ptr(this);
        ptr = mSphereBase;
        bs << ptr;
        bs << mBounding;
        bs << mFrozen;
        bs << mForceLod;
        bs << mTranslucent;
        bs << mShowableProps;
    }
    mTest->Save(bs);
END_SAVES

BEGIN_COPYS(Character)
    COPY_SUPERCLASS(RndDir)
    CREATE_COPY(Character)
    BEGIN_COPYING_MEMBERS
        if (ty != kCopyFromMax) {
            COPY_MEMBER(mLastLod)
            COPY_MEMBER(mForceLod)
            COPY_MEMBER(mDriver)
            COPY_MEMBER(mSelfShadow)
            SetSphereBase(c->mSphereBase);
            COPY_MEMBER(mFrozen)
            COPY_MEMBER(mForceLod)
            COPY_MEMBER(mLods)
            COPY_MEMBER(mTranslucent)
            COPY_MEMBER(mShadow)
            COPY_MEMBER(mShowableProps)
        }
    END_COPYING_MEMBERS
END_COPYS

void Character::Init() { REGISTER_OBJ_FACTORY(Character) }
