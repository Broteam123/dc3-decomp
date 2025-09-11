#pragma once
#include "rndobj/Overlay.h"
#include "obj/Object.h"

class Character;

class CharacterTest : public RndOverlay::Callback {
public:
    CharacterTest(Character *);
};

bool PropSync(CharacterTest &, DataNode &, DataArray *, int, PropOp);
