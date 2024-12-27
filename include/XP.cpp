#include "XP.h"

XP::XP(int startingXP) : xp(startingXP) {
}

void XP::gainXP(int amount) {
    xp += amount;
}

int XP::getXP() const {
    return xp;
}
