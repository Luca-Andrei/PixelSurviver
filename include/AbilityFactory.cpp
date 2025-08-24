#include "AbilityFactory.h"
#include "Ability.h"

std::unique_ptr<Ability> FireballCreator::createAbility() {
    std::vector<std::string> fireballTextures = {
        "assets/Fireball/Fireball 0001.png",
        "assets/Fireball/Fireball 0002.png",
        "assets/Fireball/Fireball 0003.png",
        "assets/Fireball/Fireball 0004.png",
        "assets/Fireball/Fireball 0005.png",
        "assets/Fireball/Fireball 0006.png",
        "assets/Fireball/Fireball 0007.png",
        "assets/Fireball/Fireball 0008.png",
        "assets/Fireball/Fireball 0009.png",
        "assets/Fireball/Fireball 0010.png",
        "assets/Fireball/Fireball 0011.png",
        "assets/Fireball/Fireball 0012.png"
    };
    return std::make_unique<Ability>(fireballTextures);
}

std::unique_ptr<Ability> IceSpireCreator::createAbility() {
    std::vector<std::string> iceSpireTextures = {
        "assets/Ice Spire/Ice Spire 0001.png",
        "assets/Ice Spire/Ice Spire 0002.png",
        "assets/Ice Spire/Ice Spire 0003.png",
        "assets/Ice Spire/Ice Spire 0004.png",
        "assets/Ice Spire/Ice Spire 0005.png",
        "assets/Ice Spire/Ice Spire 0006.png",
        "assets/Ice Spire/Ice Spire 0007.png",
        "assets/Ice Spire/Ice Spire 0008.png",
        "assets/Ice Spire/Ice Spire 0009.png",
        "assets/Ice Spire/Ice Spire 0010.png"
    };
    return std::make_unique<Ability>(iceSpireTextures);
}

std::unique_ptr<Ability> PlasmaStormCreator::createAbility() {
    std::vector<std::string> plasmaStormTextures = {
        "assets/Plasma Storm/Plasma Storm 0001.png",
        "assets/Plasma Storm/Plasma Storm 0002.png",
        "assets/Plasma Storm/Plasma Storm 0003.png",
        "assets/Plasma Storm/Plasma Storm 0004.png",
        "assets/Plasma Storm/Plasma Storm 0005.png",
        "assets/Plasma Storm/Plasma Storm 0006.png",
        "assets/Plasma Storm/Plasma Storm 0007.png",
        "assets/Plasma Storm/Plasma Storm 0008.png",
        "assets/Plasma Storm/Plasma Storm 0009.png",
        "assets/Plasma Storm/Plasma Storm 0010.png",
        "assets/Plasma Storm/Plasma Storm 0011.png"
    };
    return std::make_unique<Ability>(plasmaStormTextures);
}

std::map<std::string, std::unique_ptr<AbilityCreator>> AbilityFactory::creators;
bool AbilityFactory::initialized = false;

std::unique_ptr<Ability> AbilityFactory::createAbility(const std::string& abilityType) {
    if (!initialized) {
        initializeCreators();
    }
    
    auto it = creators.find(abilityType);
    if (it != creators.end()) {
        return it->second->createAbility();
    }
    
    return std::make_unique<FireballCreator>()->createAbility();
}

void AbilityFactory::registerCreator(const std::string& abilityType, std::unique_ptr<AbilityCreator> creator) {
    creators[abilityType] = std::move(creator);
}

void AbilityFactory::initializeCreators() {
    if (initialized) return;
    
    registerCreator("Fireball", std::make_unique<FireballCreator>());
    registerCreator("Ice Spire", std::make_unique<IceSpireCreator>());
            registerCreator("Plasma Storm", std::make_unique<PlasmaStormCreator>());
    
    initialized = true;
}
