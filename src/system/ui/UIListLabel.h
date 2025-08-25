#pragma once
#include "ui/UIListSlot.h"
#include "ui/UILabel.h"

/** "Custom slot for use with UIList" */
class UIListLabel : public UIListSlot {
public:
    OBJ_CLASSNAME(UIListLabel)
    OBJ_SET_TYPE(UIListLabel)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    virtual UIListSlotElement *CreateElement(UIList *);
    virtual RndTransformable *RootTrans();

    const char *GetDefaultText() const;
    UILabel *ElementLabel(int) const;

protected:
    UIListLabel();

    /** "label to draw/transform" */
    ObjPtr<UILabel> mLabel; // 0x78
    bool unk8c;
};
