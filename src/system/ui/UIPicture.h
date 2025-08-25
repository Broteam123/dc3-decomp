#pragma once
#include "ui/UIComponent.h"
#include "ui/UITransitionHandler.h"
#include "rndobj/Mesh.h"
#include "utl/FilePath.h"

/** "A picture object with asynchronously loading texture" */
class UIPicture : public UIComponent, public UITransitionHandler {
public:
    // Hmx::Object
    virtual ~UIPicture();
    OBJ_CLASSNAME(UIPicture)
    OBJ_SET_TYPE(UIPicture)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void SetTypeDef(DataArray *);
    virtual void PreLoad(BinStream &);
    virtual void PostLoad(BinStream &);
    // RndPollable
    virtual void Poll();
    virtual void Exit();
    // UITransitionHandler
    virtual void FinishValueChange();
    virtual bool IsEmptyValue() const;

protected:
    UIPicture();

    void CancelLoading();

    /** "Mesh to show loaded tex on (should have Mat)" */
    ObjPtr<RndMesh> mMesh; // 0x12c
    /** "Path of texture to load" */
    FilePath mTexFile; // 0x138
    FilePath mLoadedFile; // 0x144
    RndTex *mTex; // 0x150
    FileLoader *mLoader; // 0x154
    bool mHookTex; // 0x158
    FilePath mDelayedTexFile; // 0x15c
};
