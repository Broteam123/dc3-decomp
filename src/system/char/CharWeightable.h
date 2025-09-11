#pragma once
#include "obj/Object.h"

/** "Base class for any object that can have weight set on it,
 *  or have a Weightable parent from which it gets that state." */
class CharWeightable : public virtual Hmx::Object {
public:
    virtual ~CharWeightable() {}
    virtual bool Replace(ObjRef *, Hmx::Object *);
    OBJ_CLASSNAME(CharWeightable);
    OBJ_SET_TYPE(CharWeightable);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void SetWeight(float wt) { mWeight = wt; }

protected:
    CharWeightable();

    /** "Weight to blend in by" */
    float mWeight; // 0x8
    /** "object to get weight from" */
    ObjOwnerPtr<CharWeightable> mWeightOwner; // 0xc
};
