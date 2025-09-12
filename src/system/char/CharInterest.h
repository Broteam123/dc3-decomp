#pragma once
#include "rndobj/Trans.h"

class CharInterest : public RndTransformable {
public:
    // Hmx::Object
    virtual ~CharInterest();
    OBJ_CLASSNAME(CharInterest);
    OBJ_SET_TYPE(CharInterest);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void Highlight();

protected:
    CharInterest();
};
