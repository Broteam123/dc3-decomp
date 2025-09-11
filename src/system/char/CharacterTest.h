#pragma once
#include "rndobj/Overlay.h"
#include "obj/Object.h"
#include "utl/BinStream.h"

class Character;

class CharacterTest : public RndOverlay::Callback {
public:
    CharacterTest(Character *);
    void Save(BinStream &);
};

bool PropSync(CharacterTest &, DataNode &, DataArray *, int, PropOp);
