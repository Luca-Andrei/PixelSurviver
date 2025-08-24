#ifndef XP_H
#define XP_H

class XP {
public:
    explicit XP(int maxXP = 100);

    void addXP(int amount);

    void resetXP();

    [[nodiscard]] int getXP() const { return xp; }
    [[nodiscard]] int getMaxXP() const { return maxXP; }
    [[nodiscard]] int getLevel() const { return level; }

    void resetLevel();

    void levelUp();

    void forceLevelUp();

    void levelUpAbilities();

private:
    int xp;
    int maxXP;
    int level;
};

#endif
