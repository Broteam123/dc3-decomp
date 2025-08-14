#pragma once
#include "os/File.h"
#include "types.h"
#include "utl/MemMgr.h"
#include "utl/Str.h"

class ArkFile : public File {
public:
    ArkFile(const char *, int);
    virtual ~ArkFile();
    virtual String Filename() const { return mFilename; }
    virtual int Read(void *, int);
    virtual bool ReadAsync(void *, int);
    virtual int Write(const void *, int);
    virtual int Seek(int, int);
    virtual int Tell();
    virtual void Flush() {}
    virtual bool Eof();
    virtual bool Fail();
    virtual int Size();
    virtual int UncompressedSize();
    virtual bool ReadDone(int &);
    virtual bool GetFileHandle(void *&);

    void TaskDone(int);

    MEM_OVERLOAD(ArkFile, 0x19);

    int mArkfileNum; // 0x4
    u64 mByteStart; // 0x8
    int mSize; // 0x10
    int mUCSize; // 0x14
    int mNumOutstandingTasks; // 0x18
    int mBytesRead; // 0x1C
    int mTell; // 0x20
    bool mFail; // 0x24
    float mReadStartTime; // 0x28
    bool mReadAhead; // 0x2C
    String mFilename; // 0x30
};
