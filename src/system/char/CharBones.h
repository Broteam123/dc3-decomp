#pragma once
#include "obj/Object.h"
#include "utl/Symbol.h"
#include <vector>
#include <list>

class CharClip;

class CharBones {
public:
    enum Type {
        TYPE_POS = 0,
        TYPE_SCALE = 1,
        TYPE_QUAT = 2,
        TYPE_ROTX = 3,
        TYPE_ROTY = 4,
        TYPE_ROTZ = 5,
        TYPE_END = 6,
        NUM_TYPES = 7,
    };

    enum CompressionType {
        kCompressNone,
        kCompressRots,
        kCompressVects,
        kCompressQuats,
        kCompressAll
    };

    struct Bone {
        Bone() : name(), weight(1.0f) {}
        Bone(Symbol s, float w) : name(s), weight(w) {}

        /** "Bone to blend into" */
        Symbol name;
        /** "Weight to blend with" */
        float weight;
    };

    CharBones();
    virtual ~CharBones() {}
    virtual void ScaleAdd(CharClip *, float, float, float);
    virtual void Print();

    void Zero();
    int TypeSize(int) const;
    void SetCompression(CompressionType);
    int FindOffset(Symbol) const;
    void *FindPtr(Symbol) const;
    const char *StringVal(Symbol);
    void SetWeights(float);
    void ListBones(std::list<Bone> &) const;
    void ClearBones();
    void AddBones(const std::vector<Bone> &);
    void AddBones(const std::list<Bone> &);
    void Enter() {
        Zero();
        SetWeights(0);
    }

    static Type TypeOf(Symbol);
    static const char *SuffixOf(Type);
    static Symbol ChannelName(const char *, Type);
    static void SetWeights(float, std::vector<Bone> &);

protected:
    virtual void ReallocateInternal() {}

    void RecomputeSizes();

    CompressionType mCompression; // 0x4
    std::vector<Bone> mBones; // 0x8
    char *mStart; // 0x14
    int mCounts[NUM_TYPES]; // 0x18 - 0x30
    int mOffsets[NUM_TYPES]; // 0x34 - 0x4c
    int mTotalSize; // 0x50
};

/** "Holds state for a set of bones" */
class CharBonesObject : public CharBones, public virtual Hmx::Object {
public:
    OBJ_CLASSNAME(CharBonesObject);
    OBJ_SET_TYPE(CharBonesObject);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);

    NEW_OBJ(CharBonesObject)
    static void Init() { REGISTER_OBJ_FACTORY(CharBonesObject) }
};

/** "Holds state for a set of bones, and allocates own space" */
class CharBonesAlloc : public CharBonesObject {
public:
    virtual ~CharBonesAlloc();

protected:
    virtual void ReallocateInternal();
};
