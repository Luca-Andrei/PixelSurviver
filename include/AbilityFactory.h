#ifndef ABILITYFACTORY_H
#define ABILITYFACTORY_H

#include "Ability.h"
#include <string>
#include <memory>
#include <map>
#include <vector>

// Factory Pattern - Base Creator interface
class AbilityCreator {
public:
    virtual ~AbilityCreator() = default;
    virtual std::unique_ptr<Ability> createAbility() = 0;
    virtual std::string getAbilityName() const = 0;
};

// Factory Pattern - Concrete Creators
class FireballCreator : public AbilityCreator {
public:
    std::unique_ptr<Ability> createAbility() override;
    std::string getAbilityName() const override { return "Fireball"; }
};

class IceSpireCreator : public AbilityCreator {
public:
    std::unique_ptr<Ability> createAbility() override;
    std::string getAbilityName() const override { return "Ice Spire"; }
};

class PlasmaStormCreator : public AbilityCreator {
public:
    std::unique_ptr<Ability> createAbility() override;
    std::string getAbilityName() const override { return "Plasma Storm"; }
};

// Factory Pattern - Main Factory class
class AbilityFactory {
public:
    static std::unique_ptr<Ability> createAbility(const std::string& abilityType);
    static void registerCreator(const std::string& abilityType, std::unique_ptr<AbilityCreator> creator);

private:
    static std::map<std::string, std::unique_ptr<AbilityCreator>> creators;
    static bool initialized;
    static void initializeCreators();
};

#endif // ABILITYFACTORY_H
