#include "XP.h"
#include <iostream>

XP::XP(int maxXP) : xp(0), maxXP(maxXP), level(1) {
}

void XP::addXP(int amount) {
    xp += amount;
    if (xp >= maxXP) {
        levelUp();
    }
}

void XP::resetXP() {
    xp = 0;
}

void XP::levelUp() {
    if (xp >= maxXP) {
        level++;
        xp = 0;
        std::cout << "XP Level Up! New level: " << level << std::endl;
    }
}
