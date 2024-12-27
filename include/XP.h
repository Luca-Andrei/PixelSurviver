#ifndef XP_H
#define XP_H

class XP {
public:
    explicit XP(int startingXP = 0);

    ~XP();

    [[nodiscard]] int getXP() const;

private:
    int xp;
};

#endif // XP_H
