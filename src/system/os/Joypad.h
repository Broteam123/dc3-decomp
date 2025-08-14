#pragma once
#include "obj/Object.h"
#include "utl/Symbol.h"

enum {
    kNumAnalogSticks = 2,
    kNumJoypads = 4,
    kNumPressureButtons = 8,
};

enum JoypadAction {
    kAction_None = 0,
    kAction_Confirm = 1,
    kAction_Cancel = 2,
    kAction_Option = 3,
    kAction_Start = 4,
    kAction_ViewModify = 5,
    kAction_Up = 6,
    kAction_Right = 7,
    kAction_Down = 8,
    kAction_Left = 9,
    kAction_PageUp = 10,
    kAction_PageDown = 11,
    kAction_ExtendedOption1 = 12,
    kAction_ExtendedOption2 = 13,
    kAction_ShellOption = 14,
    kAction_WiiHomeMenu = 15,
    kAction_NumActions = 16,
};

enum JoypadButton {
    kPad_L2 = 0,
    kPad_R2 = 1,
    kPad_L1 = 2,
    kPad_R1 = 3,
    kPad_Tri = 4,
    kPad_Circle = 5,
    kPad_X = 6,
    kPad_Square = 7,
    kPad_Select = 8,
    kPad_L3 = 9,
    kPad_R3 = 10,
    kPad_Start = 11,
    kPad_DUp = 12,
    kPad_DRight = 13,
    kPad_DDown = 14,
    kPad_DLeft = 15,
    kPad_LStickUp = 16,
    kPad_LStickRight = 17,
    kPad_LStickDown = 18,
    kPad_LStickLeft = 19,
    kPad_RStickUp = 20,
    kPad_RStickRight = 21,
    kPad_RStickDown = 22,
    kPad_RStickLeft = 23,
    // from config/macros.dta:
    //    #ifdef HX_PC
    //    #define kPad_ConsoleButton (24)
    //    #define kPad_NumButtons (25)
    //    #else
    //    #define kPad_NumButtons (24)
    //    #endif
    kPad_NumButtons = 24,
    kPad_Xbox_LT = 0,
    kPad_Xbox_RT = 1,
    kPad_Xbox_LB = 2,
    kPad_Xbox_RB = 3,
    kPad_Xbox_Y = 4,
    kPad_Xbox_B = 5,
    kPad_Xbox_A = 6,
    kPad_Xbox_X = 7,
    kPad_Xbox_LS = 9,
    kPad_Xbox_RS = 10,
    kPad_3ds_ZL = 0,
    kPad_3ds_ZR = 1,
    kPad_3ds_L = 2,
    kPad_3ds_R = 3,
    kPad_3ds_X = 4,
    kPad_3ds_A = 5,
    kPad_3ds_B = 6,
    kPad_3ds_Y = 7
};

enum JoypadType {
    kJoypadNone = 0,
    kJoypadDigital = 1,
    kJoypadAnalog = 2,
    kJoypadDualShock = 3,
    kJoypadMidi = 4,
    kJoypadXboxHxGuitar = 5,
    kJoypadXboxHxGuitarRb2 = 6,
    kJoypadXboxRoGuitar = 7,
    kJoypadXboxDrums = 8,
    kJoypadXboxDrumsRb2 = 9,
    kJoypadXboxRoDrums = 10,
    kJoypadXboxStageKit = 11,
    kJoypadPs3HxGuitar = 12,
    kJoypadPs3HxGuitarRb2 = 13,
    kJoypadPs3HxDrums = 14,
    kJoypadPs3HxDrumsRb2 = 15,
    kJoypadPs3RoGuitar = 16,
    kJoypadPs3RoDrums = 17,
    kJoypadPs3KonamiDrums = 18,
    kJoypadWiiCore = 19,
    kJoypadWiiFS = 20,
    kJoypadWiiClassic = 21,
    kJoypadWiiGamecube = 22,
    kJoypadWiiGuitar = 23,
    kJoypadWiiDrums = 24,
    kJoypadWiiHxGuitar = 25,
    kJoypadWiiHxGuitarRb2 = 26,
    kJoypadWiiHxDrums = 27,
    kJoypadWiiHxDrumsRb2 = 28,
    kJoypadXboxCoreGuitar = 29,
    kJoypadXboxButtonGuitar = 30,
    kJoypadXboxRealGuitar22Fret = 31,
    kJoypadXboxMidiBoxKeyboard = 32,
    kJoypadXboxMidiBoxDrums = 33,
    kJoypadXboxKeytar = 34,
    kJoypadPs3CoreGuitar = 35,
    kJoypadPs3ButtonGuitar = 36,
    kJoypadPs3RealGuitar22Fret = 37,
    kJoypadPs3MidiBoxKeyboard = 38,
    kJoypadPs3MidiBoxDrums = 39,
    kJoypadPs3Keytar = 40,
    kJoypadWiiCoreGuitar = 41,
    kJoypadWiiButtonGuitar = 42,
    kJoypadWiiRealGuitar22Fret = 43,
    kJoypadWiiMidiBoxKeyboard = 44,
    kJoypadWiiMidiBoxDrums = 45,
    kJoypadWiiKeytar = 46,
    kJoypad3ds = 47,
    kJoypad3dsDebug = 48,
    kJoypadNumTypes = 49
};

enum JoypadBreedDataStatus {
};

// size 0xDC
class JoypadData {
public:
    unsigned int mButtons; // 0x0
    unsigned int mNewPressed; // 0x4
    unsigned int mNewReleased; // 0x8
    float mSticks[2][2]; // 0xC = LX; 0x10 = LY; 0x14 = RX; 0x18 = RY
    float mTriggers[2]; // 0x1C = LT; 0x20 = RT
    float mSensors[3]; // 0x24 = SX; 0x28 = SY; 0x2C = SZ
    // float mPressures[8]; // 0x30 - 0x50
    // 0x30-0x40 seems to be the pro union we deduced in RB3
    int propadding[4]; // 0x30
    int unk40;
    class LocalUser *mUser; // 0x44
    bool mConnected; // 0x48
    bool mVibrateEnabled; // 0x49
    bool unk4a, unk4b, unk4c;
    int mNumAnalogSticks; // 0x50
    bool mTranslateSticks; // 0x54
    int mIgnoreButtonMask; // 0x58
    int mGreenCymbalMask; // 0x5c
    int mYellowCymbalMask; // 0x60
    int mBlueCymbalMask; // 0x64
    int mSecondaryPedalMask; // 0x68
    int mCymbalMask; // 0x6c
    bool mIsDrum; // 0x70
    JoypadType mType; // 0x74
    Symbol mControllerType; // 0x78
    float mDistFromRest; // 0x7c
    bool mHasGreenCymbal; // 0x80
    bool mHasYellowCymbal; // 0x81
    bool mHasBlueCymbal; // 0x82
    bool mHasSecondaryPedal; // 0x83
    int unk84;
    int unk88;
    int unk8c;
    int unk90;
    int unk94;
    int unk98;
    int unk9c;
    int unka0;
    int unka4;
    int unka8;
    int unkac;
    int unkb0;
    int unkb4;
    int unkb8;
    int unkbc;
    bool unkc0;
    int unkc4;
    int unkc8;
    int unkcc;
    int unkd0;
    int unkd4;
    int unkd8;

    JoypadData();
    float GetAxis(Symbol) const;
    int GetVelocityBucket(Symbol) const;

    float GetLX() const { return mSticks[0][0]; }
    float GetLY() const { return mSticks[0][1]; }
    float GetRX() const { return mSticks[1][0]; }
    float GetRY() const { return mSticks[1][1]; }
    float GetLT() const { return mTriggers[0]; }
    float GetRT() const { return mTriggers[1]; }
    float GetSX() const { return mSensors[0]; }
    float GetSY() const { return mSensors[1]; }
    float GetSZ() const { return mSensors[2]; }

private:
    int FloatToBucket(float) const;
};

struct WaitInfo {
    WaitInfo(int pad);
    int mPadNum; // 0x0
    unsigned int mButtons; // 0x4
};

extern "C" {
void JoypadInitCommon(DataArray *);
void JoypadTerminateCommon();
void JoypadPollCommon();
bool JoypadIsCalbertGuitar(int);
}

void JoypadReset();
JoypadData *JoypadGetPadData(int);
Symbol JoypadControllerTypePadNum(int);

void JoypadSubscribe(Hmx::Object *);
void JoypadUnsubscribe(Hmx::Object *);
void JoypadSetVibrate(int, bool);
