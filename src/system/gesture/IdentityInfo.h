#pragma once

class IdentityInfo {
public:
    void PostUpdate();

    bool ProfileMatched() const { return mProfileMatched; }
    int EnrollmentIndex() const { return mEnrollmentIdx; }

private:
    void Identified(unsigned int);

    bool unk0;
    int mEnrollmentIdx; // 0x4
    bool mProfileMatched; // 0x8
    bool unk9; // 0x9
    int unkc; // 0xc
};
