#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <string>

class BaseError : public std::exception {
public:
    explicit BaseError(std::string message);

    [[nodiscard]] const char *what() const noexcept override;

protected:
    std::string message;
};

class EntityError : public BaseError {
public:
    explicit EntityError(std::string message);
};

class MonsterError : public EntityError {
public:
    explicit MonsterError(const std::string &message);
};

class HeroError : public EntityError {
public:
    explicit HeroError(const std::string &message);
};

class TextureLoadError : public std::exception {
public:
    explicit TextureLoadError(const std::string &textureName);

    [[nodiscard]] const char *what() const noexcept override;

private:
    std::string message;
};

class GameError : public std::exception {
public:
    explicit GameError(const std::string &message);

    [[nodiscard]] const char *what() const noexcept override;

private:
    std::string message;
};

class FireballError : public std::exception {
public:
    explicit FireballError(const std::string &message);

    [[nodiscard]] const char *what() const noexcept override;

private:
    std::string message;
};

#endif
