#ifndef XP_H
#define XP_H

#include "Entity.h"

class XP : public Entity {
public:
    explicit XP(int maxXP = 100);

    void addXP(int amount);

    void resetXP();

    int getXP() const { return xp; }
    int getMaxXP() const { return maxXP; }
    int getLevel() const { return level; }

    void resetLevel();

    void levelUp();

private:
    int xp;
    int maxXP;
    int level;
};

#endif // XP_H
