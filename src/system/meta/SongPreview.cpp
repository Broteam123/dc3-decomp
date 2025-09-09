#include "meta/SongPreview.h"
#include "macros.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include "obj/Task.h"
#include "os/ContentMgr.h"
#include "os/Debug.h"
#include "os/System.h"
#include "synth/Faders.h"
#include "utl/SongInfoCopy.h"
#include "utl/Symbol.h"

SongPreview::SongPreview(const SongMgr &mgr)
    : mSongMgr(mgr), mStream(0), mFader(0), mMusicFader(0), mCrowdSingFader(0), unk34(0),
      mAttenuation(0.0f), mState(kIdle), mStartMs(0.0f), mEndMs(0.0f),
      mStartPreviewMs(0.0f), mEndPreviewMs(0.0f), unk64(0), mPreviewRequestedMs(0.0f),
      unk70(0), unk71(0), unk72(0), mSecurePreview(0) {


      }

void SongPreview::ContentMounted(char const * contentName, char const * cc2){
    MILO_ASSERT(contentName, 0xBF);
    Symbol sym = Symbol(contentName);
    if(sym==mSongContent){
        mSongContent = Symbol(0);
    }
}

void SongPreview::ContentFailed(char const * contentName){
    MILO_ASSERT(contentName, 0xcb);
    Symbol sym = Symbol(contentName);
    if(sym == mSongContent){
        mSong = Symbol(0);
        mSongContent = Symbol(0);
        mState = kIdle;
    }
}

/*DataNode SongPreview::Handle(DataArray *, bool){

}*/

SongPreview::~SongPreview(){

}

bool SongPreview::IsWaitingToDelete () const{
    return true;
}

bool SongPreview::IsFadingOut () const{
    return true;
}

void SongPreview::SetMusicVol(float f){
    
}

void SongPreview::Init(){
    mSong = Symbol(0);
    mSongContent = Symbol(0);
    RELEASE(mStream);
    mState = kIdle;
    mRestart = true;
    DataArray *cfg = SystemConfig("sound", "song_select");
    cfg->FindData("loop_forever", mLoopForever, true);
    cfg->FindData("fade_time", mFadeTime, true);
    cfg->FindData("attenuation", mAttenuation, true);
    mFadeTime *= 1000.0f;
    mFader = Hmx::Object::New<Fader>();
    mMusicFader = Hmx::Object::New<Fader>();
    mCrowdSingFader = Hmx::Object::New<Fader>();
}

void SongPreview::Terminate(){
    mSong = Symbol(0);
    mSongContent = Symbol(0);
    RELEASE(mStream);
    RELEASE(mFader);
    RELEASE(mMusicFader);
    RELEASE(mCrowdSingFader);
    if (unk72){
        TheContentMgr->UnregisterCallback(this, true);
        unk72 = 0;
    }
}

void SongPreview::Start(Symbol, TexMovie *){
    
}

void SongPreview::PreparePreview(){
    
}

void SongPreview::Poll(){
    
}

DataNode SongPreview::OnStart(DataArray * arr){
    mSecurePreview = false;
    if(arr->Size() == 3){
        mStartPreviewMs = 0.0f;
        mEndPreviewMs = 0.0f;
        //Start(arr->ForceSym(2));
    } else{
        mStartPreviewMs = arr->Float(3);
        mEndPreviewMs = arr->Float(4);
        if(arr->Size() >= 6){
            mSecurePreview = arr->Int(5);
        }
        mSong = Symbol(gNullStr);
        //Start(arr->ForceSym(2));
    }
    return DataNode(1);
}

void SongPreview::DetachFader(Fader* f){
    if (mStream && f){
        for(int i=0;i<unk34;i++){
            mStream->ChannelFaders(i)->Remove(f);
        }
    }
}

void SongPreview::PrepareFaders(SongInfo const * info){
    for(int i=0;i<unk34;i++){
        FaderGroup *grp = mStream->ChannelFaders(i);
        grp->Add(mMusicFader);
    }
}

void SongPreview::PrepareSong(Symbol s){
    
}

BEGIN_HANDLERS(SongPreview)
    HANDLE(start, OnStart)
    HANDLE_ACTION(set_music_vol, SetMusicVol(_msg->Float(2)))
    HANDLE_SUPERCLASS(Hmx::Object)
END_HANDLERS
