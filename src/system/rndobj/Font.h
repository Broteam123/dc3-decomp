#pragma once
#include "obj/Object.h"
#include "rndobj/FontBase.h"
#include "utl/BinStream.h"
#include "utl/MemMgr.h"

class KerningTable {
public:
    class Entry {
    public:
        Entry *next; // 0x0
        int key; // 0x4
        float kerning; // 0x8
    };
    KerningTable();
    ~KerningTable();
    float Kerning(unsigned short, unsigned short);
    void GetKerning(std::vector<RndFontBase::KernInfo> &) const;
    void SetKerning(const std::vector<RndFontBase::KernInfo> &, RndFontBase *);
    Entry *Find(unsigned short, unsigned short);
    void Save(BinStream &);
    void Load(BinStreamRev &, RndFontBase *);
    bool Valid(const RndFontBase::KernInfo &, RndFontBase *);

    int Key(unsigned short us0, unsigned short us2) {
        return (us0 & 0xFFFF) | ((us2 << 0x10) & 0xFFFF0000);
    }
    int Size() const { return mNumEntries * sizeof(Entry) + 0x88; }
    int TableIndex(unsigned short us0, unsigned short us2) { return (us0 ^ us2) & 0x1F; }

    MEM_OVERLOAD(KerningTable, 0x162);

    int mNumEntries; // 0x0
    Entry *mEntries; // 0x4
    Entry *mTable[32]; // 0x8
};

class RndFont : public RndFontBase {
public:
    struct CharInfo {
        int unk0;
        float charWidth;
        float unkc;
        float unk10;
        float unk14;
    };
    virtual ~RndFont() { RELEASE(mKerningTable); }
    virtual bool Replace(ObjRef *, Hmx::Object *);
    OBJ_CLASSNAME(Font);
    OBJ_SET_TYPE(Font);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual float CharWidth(unsigned short) const;
    virtual bool CharAdvance(unsigned short, unsigned short, float &) const;
    virtual float CharAdvance(unsigned short) const;
    virtual bool CharDefined(unsigned short) const;
    virtual float AspectRatio() const { return mCellSize.y / mCellSize.x; }
    virtual RndMat *Mat() const {
        if (mMats.size() > 0)
            return mMats.front();
        else
            return nullptr;
    }
    virtual const RndFontBase *DataOwner() const;
    virtual float FontUnit() const { return mCellSize.x; }
    virtual void Print() const;

    RndMat *Mat(int) const;
    RndTex *ValidTexture(int) const;

protected:
    RndFont();
    virtual bool HasChar(unsigned short) const;
    virtual void SetASCIIChars(String);

    ObjPtrVec<RndMat> mMats; // 0x44
    ObjOwnerPtr<RndFont> mTextureOwner; // 0x60
    std::map<unsigned short, CharInfo> mCharInfoMap; // 0x74
    Vector2 mCellSize; // 0x8c
    float mDeprecatedSize; // 0x94
    std::vector<Vector2> unk98;
    bool unka4;
};
