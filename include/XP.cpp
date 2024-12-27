#include "XP.h"
#include <iostream>

XP::XP(int startingXP) : xp(startingXP) {
}

int XP::getXP() const {
    return xp;
}

XP::~XP() {
    std::cout << "XP class is being destroyed! Current XP: " << xp << std::endl;
    xp = 0;
    std::cout << "XP class has been destroyed! Current XP: " << xp << std::endl;
}
