#ifndef ABILITYFACTORY_H
#define ABILITYFACTORY_H

#include "Ability.h"
#include <string>
#include <memory>
#include <map>

class AbilityCreator {
public:
    virtual ~AbilityCreator() = default;
    virtual std::unique_ptr<Ability> createAbility() = 0;

};

class FireballCreator : public AbilityCreator {
public:
    std::unique_ptr<Ability> createAbility() override;

};

class IceSpireCreator : public AbilityCreator {
public:
    std::unique_ptr<Ability> createAbility() override;

};

class PlasmaStormCreator : public AbilityCreator {
public:
    std::unique_ptr<Ability> createAbility() override;

};

class AbilityFactory {
public:
    static std::unique_ptr<Ability> createAbility(const std::string& abilityType);
    static void registerCreator(const std::string& abilityType, std::unique_ptr<AbilityCreator> creator);

private:
    static std::map<std::string, std::unique_ptr<AbilityCreator>> creators;
    static bool initialized;
    static void initializeCreators();
};

#endif
