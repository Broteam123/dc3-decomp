#pragma once
#include "ui/UIListWidget.h"

enum UIListSlotDrawType {
    kUIListSlotDrawAlways,
    kUIListSlotDrawHighlight,
    kUIListSlotDrawNoHighlight
};

class UIListSlotElement {
public:
    UIListSlotElement() {}
    virtual ~UIListSlotElement() {}
    virtual void Fill(const UIListProvider &, int, int) = 0;
    virtual void Draw(const Transform &, float, UIColor *, Box *) = 0;
    virtual void Poll() {}
};

/** "Base functionality for UIList slots" */
class UIListSlot : public UIListWidget {
public:
    // Hmx::Object
    virtual ~UIListSlot() { ClearElements(); }
    OBJ_CLASSNAME(UIListSlot)
    OBJ_SET_TYPE(UIListSlot)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    // UIListWidget
    virtual void ResourceCopy(const UIListWidget *);
    virtual void CreateElements(UIList *, int);
    virtual void Draw(
        const UIListWidgetDrawState &,
        const UIListState &,
        const Transform &,
        UIComponent::State,
        Box *,
        DrawCommand
    );
    virtual void Fill(const class UIListProvider &, int, int, int);
    virtual void StartScroll(int, bool);
    virtual void CompleteScroll(const UIListState &, int);
    virtual void Poll();

    bool Matches(const char *) const;
    const char *MatchName() const;

private:
    void ClearElements();

protected:
    UIListSlot();

    virtual UIListSlotElement *CreateElement(UIList *) { return nullptr; }
    virtual RndTransformable *RootTrans() { return nullptr; }

    std::vector<UIListSlotElement *> mElements; // 0x40
    UIListSlotDrawType mSlotDrawType; // 0x48
    UIListSlotElement *mNextElement; // 0x4c
    String mMatchName; // 0x50
};
