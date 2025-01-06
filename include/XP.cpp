#include "XP.h"
#include <iostream>
#include "Error.h"

XP::XP(int maxXP) : xp(0), level(1) {
    try {
        if (maxXP <= 0) {
            throw GameError("Max XP must be a positive value.");
        }
        this->maxXP = maxXP;
    } catch (const GameError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

std::ostream &operator<<(std::ostream &os, const XP &xp) {
    os << "Current XP: " << xp.getXP() << std::endl;
    os << "Current LVL: " << xp.getLevel() << std::endl;
    return os;
}

void XP::addXP(int amount) {
    try {
        if (amount < 0) {
            throw GameError("XP amount cannot be negative.");
        }

        xp += amount;
        if (xp >= maxXP) {
            levelUp();
        }
    } catch (const GameError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void XP::resetXP() {
    xp = 0;
}

void XP::resetLevel() {
    level = 1;
}

void XP::levelUp() {
    if (xp >= maxXP) {
        level++;
        xp = 0;
    }
}
