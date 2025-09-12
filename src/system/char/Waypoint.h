#pragma once
#include "rndobj/Trans.h"

/** "A waypoint for character movement. Characters walk to
 *  these, start themselves out from these, etc." */
class Waypoint : public RndTransformable {
public:
    virtual ~Waypoint();
    OBJ_CLASSNAME(Waypoint)
    OBJ_SET_TYPE(Waypoint)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void Highlight();

protected:
    Waypoint();
};
