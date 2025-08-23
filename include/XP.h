#ifndef XP_H
#define XP_H

class XP {
public:
    explicit XP(int maxXP = 100);

    void addXP(int amount);

    void resetXP();

    int getXP() const { return xp; }
    int getMaxXP() const { return maxXP; }
    int getLevel() const { return level; }

    void resetLevel();

    void levelUp();
    void forceLevelUp(); // Force level up without checking XP condition

    void levelUpAbilities();

private:
    int xp;
    int maxXP;
    int level;
};

#endif // XP_H
