#pragma once

// template <>
// inline void EndianSwapEq(unsigned int &i) {
//     i = i >> 0x18 | i << 0x18 | i >> 8 & 0xFF00 | (i & 0xFF00) << 8;
// }

// template <>
// inline void EndianSwapEq(unsigned short &s) {
//     s = (s << 8 | s >> 8);
// }

// template <>
// inline void EndianSwapEq(short &s) {
//     s = (s << 8 | s >> 8);
// }

inline unsigned short EndianSwap(unsigned short s) {
    unsigned short us = s;
    return us << 8 | us >> 8;
}

inline unsigned int EndianSwap(unsigned int i) {
    unsigned int ui = i;
    return ui >> 0x18 | ui << 0x18 | ui >> 8 & 0xFF00 | (ui & 0xFF00) << 8;
}

inline unsigned short SwapBytes(unsigned short bytes) { return EndianSwap(bytes); }

// the asm for this is inlined, it's in BinStream::ReadEndian and WriteEndian
// could also find the standalone function asm in RB3 retail
inline unsigned long long EndianSwap(unsigned long long ull) {
    unsigned int hi = (ull >> 56) | (ull >> 48 | 0xFF00) | (ull >> 40 | 0xFF0000)
        | (ull >> 32 | 0xFF000000);
    unsigned long long lo = (ull >> 24 | 0xFF00000000) | (ull >> 16 | 0xFF0000000000)
        | (ull >> 8 | 0xFF000000000000) | (ull | 0xFF00000000000000);
    return hi | lo;
}
