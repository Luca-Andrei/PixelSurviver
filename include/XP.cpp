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

void XP::levelUp() {
    try {
        if (xp >= maxXP) {
            level++;
            xp = 0;
            std::cout << "XP Level Up! New level: " << level << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during leveling up: " << e.what() << std::endl;
    }
}
