#include "Error.h"

BaseError::BaseError(std::string message) : message(std::move(message)) {
}

const char *BaseError::what() const noexcept {
    return message.c_str();
}

EntityError::EntityError(std::string message) : BaseError(std::move(message)) {
}

MonsterError::MonsterError(const std::string &message)
    : EntityError("Monster Error: " + message) {
}

HeroError::HeroError(const std::string &message)
    : EntityError("Hero Error: " + message) {}

TextureLoadError::TextureLoadError(const std::string &textureName)
    : message("Error loading texture: " + textureName) {}

const char *TextureLoadError::what() const noexcept {
    return message.c_str();
}

GameError::GameError(const std::string &message)
    : message("Game Error: " + message) {}

const char *GameError::what() const noexcept {
    return message.c_str();
}

FireballError::FireballError(const std::string &message)
    : message("Fireball Error: " + message) {}

const char *FireballError::what() const noexcept {
    return message.c_str();
}
