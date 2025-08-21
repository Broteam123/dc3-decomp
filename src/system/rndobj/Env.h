#pragma once
#include "BoxMap.h"
#include "Lit.h"
#include "obj/Data.h"
#include "os/Timer.h"
#include "rndobj/ColorXfm.h"
#include "rndobj/Draw.h"
#include "rndobj/Trans.h"
#include "rndobj/Lit.h"
#include "utl/MemMgr.h"

class RndEnviron : public RndTransformable, public RndDrawable {
public:
    virtual ~RndEnviron();
    virtual bool Replace(ObjRef *, Hmx::Object *);
    OBJ_CLASSNAME(Environ);
    OBJ_SET_TYPE(Environ);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void Highlight();
    virtual void Select(const Vector3 *);
    virtual void UpdateApproxLighting(const Vector3 *);
    virtual int NumLights_Real() const { return mLightsReal.size(); }
    virtual int NumLights_Approx() const { return mLightsApprox.size(); }
    virtual bool IsFake(RndLight *) const;
    virtual bool IsReal(RndLight *) const;

    OBJ_MEM_OVERLOAD(0x1B);

    const Transform &ColorXfm() const;
    bool FogEnable() const;
    Transform LRFadeRef() const;
    void RemoveLight(RndLight *);
    void AddLight(RndLight *);
    bool IsValidRealLight(const RndLight *l) const;

protected:
    RndEnviron();

    bool IsLightInList(const RndLight *, const ObjPtrList<RndLight> &) const;
    void OnRemoveAllLights();
    void ReclassifyLights();
    DataNode OnAllowableLights_Real(const DataArray *);
    DataNode OnAllowableLights_Approx(const DataArray *);

    static BoxMapLighting sGlobalLighting;
    static RndEnviron *sCurrent;
    static Vector3 sCurrentPos;
    static bool sCurrentPosSet;

    ObjPtrList<RndLight> mLightsReal; // 0x100
    ObjPtrList<RndLight> mLightsApprox; // 0x114
    ObjPtrList<RndLight> mLightsOld; // 0x128
    Hmx::Color mAmbientColor; // 0x13c
    ObjOwnerPtr<RndEnviron> mAmbientFogOwner; // 0x14c
    bool mFogEnable; // 0x160
    float mFogStart; // 0x164
    float mFogEnd; // 0x168
    Hmx::Color mFogColor; // 0x16c
    bool mFadeOut; // 0x17c
    float mFadeStart; // 0x180
    float mFadeEnd; // 0x184
    float mFadeMax; // 0x188
    ObjPtr<RndTransformable> mFadeRef; // 0x18c
    Vector4 mLRFade; // 0x1a0, mLeftOut, mLeftOpaque, mRightOpaque, mRightOut
    RndColorXfm mColorXfm; // 0x1b0
    bool mUseColorAdjust; // 0x244
    bool mAnimateFromPreset; // 0x245
    bool mAOEnabled; // 0x246
    float mAOStrength; // 0x248
    Timer mUpdateTimer; // 0x250
    float mIntensityAverage; // 0x280
    float mIntensityRate; // 0x284
    float mExposure; // 0x288
    float mWhitePoint; // 0x28c
    bool mUseToneMapping; // 0x290
    bool mUseApprox_Local; // 0x291
    bool mUseApprox_Global; // 0x292
};
