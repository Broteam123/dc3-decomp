#pragma once
#include "obj/Data.h"
#include "obj/Dir.h"
#include "obj/Object.h"
#include "os/ContentMgr.h"
#include "os/VirtualKeyboard.h"
#include "synth/Faders.h"
#include "utl/Symbol.h"
#include "synth/Stream.h"

class SongPreview : public ContentMgr::Callback, public Hmx::Object{
    public:
        enum State {
            kIdle = 0,
            kMountingSong = 1,
            kPreparingSong = 2,
            kDeletingSong = 3,
            kPlayingSong = 4,
            kFadingOutSong = 5,
        };

        SongPreview(class SongMgr const &);
        virtual void ContentMounted(char const *, char const *);
        virtual void ContentFailed(char const *);
        //virtual ObjPtr<class TexMovie>::ObjPtr<class TexMovie>(void);
        virtual DataNode Handle(DataArray *, bool);
        //virtual void ObjRefConcrete<class TexMovie, class ObjectDir>::SetObj();
        virtual ~SongPreview();
        
        bool IsWaitingToDelete() const;
        bool IsFadingOut() const;
        void SetMusicVol(float);
        void Init();
        void Terminate();
        void Start(class Symbol, class TexMovie *);
        void PreparePreview();
        void Poll();
        DataNode OnStart(DataArray *);

    private:
        void DetachFader(Fader*);
        void PrepareFaders(class SongInfo const *);
        void PrepareSong(Symbol);
        //class Hmx::Object * ObjRefConcrete<class TexMovie, class ObjDir>::SetObj(class Hmx::Object*);
    
    const SongMgr &mSongMgr; 
    Stream *mStream; 
    Fader *mFader; 
    Fader *mMusicFader; 
    Fader *mCrowdSingFader; 
    int unk34; 
    float mAttenuation; 
    float mFadeTime; 
    bool mRestart; 
    bool mLoopForever; 
    State mState; 
    Symbol mSong; 
    Symbol mSongContent; 
    float mStartMs; 
    float mEndMs; 
    float mStartPreviewMs; 
    float mEndPreviewMs; 
    int unk60; 
    bool unk64; 
    float mPreviewRequestedMs; 
    Symbol unk6c; 
    bool unk70; 
    bool unk71; 
    bool unk72; 
    bool mSecurePreview; 
};
