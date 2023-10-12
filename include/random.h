// Tools for generating random quantities
//
// Created by Kevin Gori on 03/03/2021.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <random>

class RandomGenerator
{
public:
    RandomGenerator() : rng(rd()) {}
private:
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
protected:
    std::mt19937 rng;
};

class RandomVector2fGenerator : public RandomGenerator
{
public:
    RandomVector2fGenerator() : RandomGenerator() {}
    sf::Vector2f generate(float x1, float x2, float y1, float y2) {
        std::uniform_real_distribution<float> x(x1, x2);
        std::uniform_real_distribution<float> y(y1, y2);
        return sf::Vector2f(x(rng), y(rng));
    }
};

class RandomColourGenerator : public RandomGenerator
{
public:
    RandomColourGenerator() : RandomGenerator() {}
    sf::Color generate() {
        std::uniform_int_distribution<unsigned char> dist(0, 255);
        return sf::Color(dist(rng), dist(rng), dist(rng));
    }
};