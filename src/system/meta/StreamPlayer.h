#pragma once
#include "obj/Data.h"
#include "obj/Object.h"
#include "synth/Stream.h"

class StreamPlayer : public Hmx::Object{
    public:
        StreamPlayer();
        void StopPlaying();
        virtual ~StreamPlayer();
        void PlayFile(char const *, float, float, bool);
        void Poll();
        virtual DataNode Handle(DataArray *, bool);

        float mMasterVol;
        float mStreamVol;
        bool mLoop;
        bool mStarted;
        bool mPaused;
        Stream *mStream;
        

    private:
        void Delete();
        void Init();
};
