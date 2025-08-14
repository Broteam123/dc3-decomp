#pragma once
#include "obj/Data.h"
#include "obj/Object.h"
#include "midi/DataEventList.h"
#include <vector>
#include <list>

class GemListInterface; // forward dec

// size 0xC0
class MidiParser : public Hmx::Object {
public:
    struct PostProcess {
        PostProcess()
            : zeroLength(false), startOffset(0), endOffset(0), minLength(0),
              maxLength(kHugeFloat), minGap(-kHugeFloat), maxGap(kHugeFloat),
              useRealtimeGaps(false), variableBlendPct(0) {}
        bool zeroLength;
        float startOffset;
        float endOffset;
        float minLength;
        float maxLength;
        float minGap;
        float maxGap;
        bool useRealtimeGaps;
        float variableBlendPct;
    };

    struct Note {
        Note(int x, int y, int z) : note(z), startTick(x), endTick(y) {}
        int note; // 0x0
        int startTick; // 0x4
        int endTick; // 0x8
    };

    struct VocalEvent {
        // because midis can store text as either Text or Lyric types
        enum TextType {
            kText,
            kLyric
        };

        DataNode mTextContent; // 0x0
        int mTick; // 0x8

        TextType GetTextType() const {
            return mTextContent.Type() == kDataString ? kLyric : kText;
        }
    };

private:
    DataEventList *mEvents; // 0x2c
    /** The midi track's name. */
    Symbol mTrackName; // 0x30
    /** The typedef array to use when parsing gems. */
    DataArray *mGemParser; // 0x34
    /** The typedef array to use when parsing midi notes. */
    DataArray *mNoteParser; // 0x38
    /** The typedef array to use when parsing text. */
    DataArray *mTextParser; // 0x3c
    /** The typedef array to use when parsing lyrics. */
    DataArray *mLyricParser; // 0x40
    /** The typedef array to use when inserting idle events. */
    DataArray *mIdleParser; // 0x44
    /** The current parser in use. */
    DataArray *mCurParser; // 0x48
    /** The list of allowed midi notes for this track. */
    DataArray *mAllowedNotes; // 0x4c
    /** The list of vocal events for this track. */
    std::vector<VocalEvent> *mVocalEvents; // 0x40
    /** The list of midi notes for this track. */
    std::vector<Note> mNotes; // 0x54
    /** The list of gems for this track. */
    GemListInterface *mGems; // 0x60
    bool mInverted; // 0x64
    PostProcess mProcess; // 0x68
    float mLastStart; // 0x8c
    float mLastEnd; // 0x90
    float mFirstEnd; // 0x94
    DataEvent *mEvent; // 0x98
    Symbol mMessageType; // 0x9c
    bool mAppendLength; // 0xa0
    bool mUseVariableBlending; // 0xa1
    float mVariableBlendPct; // 0xa4
    bool mMessageSelf; // 0xa8
    bool mCompressed; // 0xa9
    /** The index of the current gem being parsed. */
    int mGemIndex; // 0xac
    /** The index of the current note being parsed. */
    int mNoteIndex; // 0xb0
    /** The index of the current vocal event being parsed. */
    int mVocalIndex; // 0xb4
    float mStart; // 0xb8
    int mBefore; // 0xbc

    static DataNode *mpStart;
    static DataNode *mpEnd;
    static DataNode *mpLength;
    static DataNode *mpPrevStartDelta;
    static DataNode *mpPrevEndDelta;
    static DataNode *mpVal;
    static DataNode *mpSingleBit;
    static DataNode *mpLowestBit;
    static DataNode *mpLowestSlot;
    static DataNode *mpHighestSlot;
    static DataNode *mpData;
    static DataNode *mpOutOfBounds;
    static DataNode *mpBeforeDeltaSec;
    static DataNode *mpAfterDeltaSec;
    static std::list<MidiParser *> sParsers;

    bool AllowedNote(int);
    int GetIndex();
    float GetStart(int);
    float GetEnd(int);
    void FixGap(float *);
    void SetIndex(int);
    float ConvertToBeats(float f1, float f2);
    bool InsertIdle(float, int);
    void PushIdle(float, float, int, Symbol);
    void SetGlobalVars(int, int, const DataNode &);
    void HandleEvent(int startTick, int endTick, const DataNode &data);
    void InsertDataEvent(float start, float end, const DataNode &ev);
    bool AddMessage(float start, float end, DataArray *msg, int firstArg);

    DataNode OnGetStart(DataArray *);
    DataNode OnGetEnd(DataArray *);
    DataNode OnNextStartDelta(DataArray *);
    DataNode OnDebugDraw(DataArray *);
    DataNode OnInsertIdle(DataArray *);
    DataNode OnBeatToSecLength(DataArray *);
    DataNode OnSecOffsetAll(DataArray *);
    DataNode OnSecOffset(DataArray *);
    DataNode OnPrevVal(DataArray *);
    DataNode OnNextVal(DataArray *);
    DataNode OnDelta(DataArray *);
    DataNode OnHasSpace(DataArray *);
    DataNode OnRtComputeSpace(DataArray *);

public:
    MidiParser();
    virtual ~MidiParser();
    OBJ_CLASSNAME(MidiParser);
    OBJ_SET_TYPE(MidiParser);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void SetTypeDef(DataArray *);

    void Clear();
    void Reset(float);
    void Poll();
    void ParseNote(int startTick, int endTick, unsigned char data1);
    void PrintEvents();
    int ParseAll(class GemListInterface *, std::vector<VocalEvent> &);

    static void Init();
    static void ClearManagedParsers();

    NEW_OBJ(MidiParser);
};
