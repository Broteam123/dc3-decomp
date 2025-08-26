#pragma once
#include "obj/Data.h"
#include "obj/Msg.h"
#include "obj/Object.h"
#include "os/Timer.h"
#include "rndobj/Cam.h"
#include "rndobj/Env.h"
#include "rndobj/Overlay.h"
#include "ui/UIScreen.h"
#include "ui/UIComponent.h"

class Automator;
class JoypadClient;

class UIManager : public Hmx::Object {
public:
    enum TransitionState {
        kTransitionNone = 0,
        kTransitionTo = 1,
        kTransitionFrom = 2,
        kTransitionPop = 3,
    };

    UIManager();
    virtual ~UIManager();
    virtual DataNode Handle(DataArray *, bool);
    virtual void Init();
    virtual void Terminate();
    virtual void Poll();
    virtual void Draw();
    virtual void GotoScreen(const char *, bool, bool);
    virtual void GotoScreen(UIScreen *, bool, bool);
    virtual void PushScreen(UIScreen *);
    virtual void PopScreen(UIScreen *);
    virtual void ResetScreen(UIScreen *);
    virtual bool InComponentSelect();
    virtual bool IsBlockingTransition() { return false; }
    virtual bool IsTimelineResetAllowed() const { return true; }

    UIScreen *CurrentScreen() { return mCurrentScreen; }
    UIScreen *TransitionScreen() { return mTransitionScreen; }
    bool InTransition() { return mTransitionState != kTransitionNone; }
    TransitionState GetTransitionState() const { return mTransitionState; }
    void SetScreenBlacklghtDisabled(bool);
    UIPanel *FocusPanel();
    UIComponent *FocusComponent();
    void GotoFirstScreen();
    UIScreen *BottomScreen();
    UIScreen *ScreenAtDepth(int);
    int PushDepth() const;

private:
    void ToggleLoadTimes();
    void CancelTransition();

protected:
    TransitionState mTransitionState; // 0x2c
    bool mWentBack; // 0x30
    std::vector<UIScreen *> mPushedScreens; // 0x34
    int mMaxPushDepth; // 0x40
    JoypadClient *mJoyClient; // 0x44
    UIScreen *mCurrentScreen; // 0x48
    UIScreen *mTransitionScreen; // 0x4c
    int unk50;
    RndCam *mCam; // 0x54
    RndEnviron *mEnv; // 0x58
    int unk5c;
    Timer mTimer; // 0x60
    bool mOverloadHorizontalNav; // 0x90
    bool mCancelTransitionNotify; // 0x91
    bool mDefaultAllowEditText; // 0x92
    bool mDisableScreenBlacklight; // 0x93
    int unk94;
    Timer mLoadTimer; // 0x98
    RndOverlay *mOverlay; // 0xc8
    Automator *mAutomator; // 0xcc
    bool unkd0;
};

extern UIManager *TheUI;

class Automator : public Hmx::Object {
public:
    Automator(UIManager &);
    virtual ~Automator();
    virtual DataNode Handle(DataArray *, bool);

    const char *ToggleAuto();
    const char *AutoScript() {
        if (mScreenScripts && !mRecord)
            return mAutoPath.c_str();
        else
            return "OFF";
    }

private:
    void FinishRecord();
    void HandleMessage(Symbol);
    void StartAuto(UIScreen *);
    void AddRecord(Symbol, DataArray *);
    Symbol CurRecordScreen();

    DataNode OnCustomMsg(const Message &);

    UIManager &mUIManager; // 0x2c
    DataArray *mScreenScripts; // 0x30
    DataArray *mRecord; // 0x34
    String mRecordPath; // 0x38
    String mAutoPath; // 0x40
    DataArray *mCurScript; // 0x48
    int mCurScreenIndex; // 0x4c
    int mCurMsgIndex; // 0x50
    int mFramesSinceAdvance; // 0x54
    bool mSkipNextQuickCheat; // 0x58
    std::list<Symbol> mCustomMsgs; // 0x5c
};
