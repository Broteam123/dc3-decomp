#pragma once
#include "gesture/IdentityInfo.h"
#include "gesture/Skeleton.h"
#include "obj/Object.h"

class GestureMgr : public Hmx::Object, public SkeletonCallback {
public:
    GestureMgr();
    // Hmx::Object
    virtual ~GestureMgr() {}
    virtual DataNode Handle(DataArray *, bool);
    // SkeletonCallback
    virtual void PostUpdate(const struct SkeletonUpdateData *);

    bool IsSkeletonValid(int) const;
    bool IsSkeletonSitting(int) const;
    bool IsSkeletonSideways(int) const;
    Skeleton *GetActiveSkeleton();
    IdentityInfo *GetIdentityInfo(int);

    static bool sIdentityOpInProgress;
};

extern GestureMgr *TheGestureMgr;
