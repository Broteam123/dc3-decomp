#include "SongMgr.h"
#include "SongMetadata.h"
#include "obj/Data.h"
#include "obj/DataFile.h"
#include "obj/Object.h"
#include "os/ContentMgr.h"
#include "os/Debug.h"
#include "stl/_vector.h"
#include "utl/BufStream.h"
#include "utl/MemMgr.h"
#include "utl/MemStream.h"
#include "utl/Symbol.h"
#include <set>
#include <vector>
#include <map>

SongMgr *TheBaseSongManager;
const char *SONG_CACHE_CONTAINER_NAME = "songcache_bb";

int GetSongID(DataArray *main_arr, DataArray *backup_arr){
    static int sDebugSongID = 99000001;
    int theID = 0;
    return theID;
}

int CountSongsInArray(DataArray *arr){
    int i=0;
    int size = arr->Size();
    for (;i < size && arr->Node(i).Type() != kDataArray; i++)
        ;
    return size - i;
}

void SongMgr::Init(){
    mState = kSongMgr_Nil;
    //mSongCacheID = 0;
    //mSongCache = 0;
    unkbc = false;
    mSongCacheNeedsWrite = false;
    mSongCacheWriteAllowed = true;
}

void SongMgr::ContentDone(){
    if(!unkbc)
        return;
    mSongCacheNeedsWrite = true;
}

SongInfo* SongMgr::SongAudioData(Symbol s) const{
    
    return null;
}

bool SongMgr::IsSongCacheWriteDone() const{
    return mState == kSongMgr_Ready || mState == kSongMgr_Failure;
}

char const * SongMgr::GetCachedSongInfoName() const{
    return SONG_CACHE_CONTAINER_NAME;
}

void SongMgr::ClearSongCacheNeedsWrite(){
    mSongCacheNeedsWrite = false;
}

char const * SongMgr::SongPath(Symbol s, int i) const{
    return null;
}

char const * SongMgr::SongFilePath(Symbol s, char const * path, int i) const{
    return null;
}

void SongMgr::DumpSongMgrContents(bool all){

}

bool SongMgr::HasSong(int id) const{
    return mAvailableSongs.find(id) != mAvailableSongs.end();
}

bool SongMgr::HasSong(Symbol s, bool b) const{
    return true;
}

int SongMgr::GetCachedSongInfoSize() const{
    return 0;
}

bool SongMgr::IsSongMounted(Symbol s) const{
    return true;
}

bool SongMgr::SaveCachedSongInfo(BufStream &bs){
    return true;
}

bool SongMgr::IsContentUsedForSong(Symbol s, int i) const{
    return true;
}

SongMetadata const * SongMgr::Data(int) const{
    return null;
}

void SongMgr::ContentStarted(){

}

void SongMgr::ContentUnmounted(char const *){

}

void SongMgr::StartSongCacheWrite(){

}

bool SongMgr::ContentDiscovered(Symbol){
    return true;
}

void SongMgr::ClearFromCache(Symbol s){
    std::map<Symbol, std::vector<int> >::iterator it;
    it = mSongIDsInContent.find(s);
    MILO_ASSERT_FMT(it != mSongIDsInContent.end(), "Content %s isn't cached!", s);
    if (it != mSongIDsInContent.end()) {
        mSongIDsInContent.erase(it);
    }
}

void SongMgr::ClearCachedContent(){
    mSongIDsInContent.clear();
    for (std::map<int, SongMetadata *>::iterator it = mCachedSongMetadata.begin();
         it != mCachedSongMetadata.end();
         ++it) {
        delete it->second;
    }
    mCachedSongMetadata.clear();
}

SongMgr::~SongMgr(){

}

char const * SongMgr::AlternateSongDir() const{
    return null;
}

void SongMgr::ContentMounted(char const *, char const *){

}

void SongMgr::GetContentNames(Symbol s, std::vector<Symbol> &vec) const{

}

bool SongMgr::LoadCachedSongInfo(BufStream &){
    return true;
}

char const * SongMgr::CachedPath(Symbol, char const *, int) const{
    return null;
}

void SongMgr::SaveMount(){
    
}

void SongMgr::SaveUnmount(){

}

void SongMgr::SaveWrite(){
    
}

void SongMgr::GetSongsInContent(){
    
}

char const * SongMgr::ContentNameRoot(Symbol) const{
    return null;
}

int SongMgr::NumSongsInContent(Symbol) const{
    return 0;
}

void SongMgr::SetState(SongMgrState state){
    
}

void SongMgr::OnCacheMountResult(int){

}

void SongMgr::OnCacheWriteResult(int){

}

void SongMgr::OnCacheUnmountResult(int){

}

void SongMgr::CacheSongData(DataArray *, DataLoader *, ContentLocT, Symbol){

}

void SongMgr::ContentLoaded(Loader *, ContentLocT, Symbol){

}
