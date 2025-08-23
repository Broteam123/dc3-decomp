#pragma once
#include "rndobj/Poll.h"
#include "rndobj/Trans.h"

/** "Vertex-based shockwave effect" */
class RndShockwave : public RndPollable, public RndTransformable {
public:
    virtual ~RndShockwave() {
        if (sSelected == this)
            sSelected = nullptr;
    }
    OBJ_CLASSNAME(Shockwave);
    OBJ_SET_TYPE(Shockwave);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void Enter();
    virtual void Exit();

private:
    static RndShockwave *sSelected;

protected:
    RndShockwave();

    /** "Automatically select this shockwave object on Enter." */
    bool mAutoSelect; // 0xc8
    /** "Shockwave radius (distance from center) (min 0).".
        Ranges from 0 to 1000. */
    float mRadius; // 0xcc
    /** "Shockwave amplitude." */
    float mAmplitude; // 0xd0
    /** "Length of the shockwave in the radial dimension (min 0).".
        Ranges from 1.0e-3 to 1000. */
    float mWavelength; // 0xd4

    // selected:
    /** "Whether or not this shockwave object
        is the currently selected one (there can be only one!)
        This state will NOT be saved." */
};
