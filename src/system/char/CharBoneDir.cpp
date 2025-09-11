#include "char/CharBoneDir.h"
#include "obj/Dir.h"
#include "obj/Object.h"
#include "utl/BinStream.h"

CharBoneDir::CharBoneDir()
    : mRecenter(this), mMoveContext(0), mBakeOutFacing(true), mFilterContext(0),
      mFilterBones(this) {}

CharBoneDir::~CharBoneDir() {}

BEGIN_HANDLERS(CharBoneDir)
    HANDLE_EXPR(get_context_flags, GetContextFlags())
    HANDLE_SUPERCLASS(ObjectDir)
END_HANDLERS

BEGIN_CUSTOM_PROPSYNC(CharBoneDir::Recenter)
    SYNC_PROP(targets, o.mTargets)
    SYNC_PROP(average, o.mAverage)
    SYNC_PROP(slide, o.mSlide)
END_CUSTOM_PROPSYNC

BEGIN_PROPSYNCS(CharBoneDir)
    SYNC_PROP(recenter, mRecenter)
    SYNC_PROP_SET(merge_character, "", MergeCharacter(FilePath(_val.Str())))
    SYNC_PROP(move_context, mMoveContext)
    SYNC_PROP(bake_out_facing, mBakeOutFacing)
    SYNC_PROP_MODIFY(filter_context, mFilterContext, SyncFilter())
    SYNC_PROP(filter_bones, mFilterBones)
    SYNC_PROP(filter_names, mFilterNames)
    SYNC_SUPERCLASS(ObjectDir)
END_PROPSYNCS

BinStream &operator<<(BinStream &, const CharBoneDir::Recenter &);

BEGIN_SAVES(CharBoneDir)
    SAVE_REVS(4, 0)
    SAVE_SUPERCLASS(ObjectDir)
    bs << mMoveContext;
    bs << mRecenter;
    bs << mBakeOutFacing;
END_SAVES

BEGIN_COPYS(CharBoneDir)
    COPY_SUPERCLASS(ObjectDir)
    CREATE_COPY(CharBoneDir)
    BEGIN_COPYING_MEMBERS
        COPY_MEMBER(mMoveContext)
        COPY_MEMBER(mRecenter)
        COPY_MEMBER(mBakeOutFacing)
    END_COPYING_MEMBERS
END_COPYS

void CharBoneDir::ListBones(std::list<CharBones::Bone> &bones, int mask, bool b3) {
    if (mMoveContext & mask) {
        bones.push_back(CharBones::Bone("bone_facing.pos", 1.0f));
        bones.push_back(CharBones::Bone("bone_facing.rotz", 1.0f));
        if (b3) {
            bones.push_back(CharBones::Bone("bone_facing_delta.pos", 1.0f));
            bones.push_back(CharBones::Bone("bone_facing_delta.rotz", 1.0f));
        }
    }
    for (ObjDirItr<CharBone> it(this, false); it != 0; ++it) {
        it->StuffBones(bones, mask);
    }
}
