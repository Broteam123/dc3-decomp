#pragma once
#include "synth/StreamReceiver.h"

#define kStreamBufSize 0x4000

class StreamReceiverFile : public StreamReceiver {
public:
    StreamReceiverFile(int, bool);
    virtual ~StreamReceiverFile() {}
    virtual void SetVolume(float) {}
    virtual void SetPan(float) {}
    virtual void SetSpeed(float) {}
    virtual void SetSlipOffset(float) {}
    virtual void SlipStop() {}
    virtual void SetSlipSpeed(float) {}
    virtual float GetSlipOffset();

    static int sPlayCursor;

protected:
    virtual int GetPlayCursor();
    virtual void PauseImpl(bool) {}
    virtual void PlayImpl() {}
    virtual void StartSendImpl(unsigned char *, int, int);
    virtual bool SendDoneImpl() { return true; }

    unsigned char *mTargetBuffer; // 0x802c
    int mBufSize; // 0x8030
};
