//
// Created by Kevin Gori on 20/09/2020.
//

#ifndef BOIDS_RULE_H
#define BOIDS_RULE_H

#include <vector>
#include "boid.h"

struct Rule
{
    Rule(float p_weight) : weight(p_weight) {}
    virtual sf::Vector2f apply_rule(Boid b, const std::vector<Boid*>& boids) = 0;
    virtual ~Rule() = default;
    virtual std::string get_name() = 0;
    float weight;
};

struct Cohesion : Rule
{
    Cohesion(float p_weight) : Rule(p_weight) {}
    sf::Vector2f apply_rule(Boid b, const std::vector<Boid*>& boids) override;
    std::string get_name() override { return std::string("Cohesion"); };
};

struct Separation : Rule
{
    Separation(float sep, float p_weight) : Rule(p_weight), separation_threshold(sep) {}
    sf::Vector2f apply_rule(Boid b, const std::vector<Boid*>& boids) override;
    float separation_threshold;
    std::string get_name() override { return std::string("Separation"); };
};

struct Alignment : Rule
{
    Alignment(float p_weight) : Rule(p_weight) {}
    sf::Vector2f apply_rule(Boid b, const std::vector<Boid*>& boids) override;
    std::string get_name() override { return std::string("Alignment"); };
};

struct Accelerate : Rule
{
    Accelerate(float p_weight) :  Rule(p_weight) {}
    sf::Vector2f apply_rule(Boid b, const std::vector<Boid*>& boids) override;
    std::string get_name() override { return std::string("Accelerate"); };
};

struct Seek : Rule
{
    Seek(sf::Vector2f point, float p_weight) : Rule(p_weight), target(point) {}
    sf::Vector2f apply_rule(Boid b, const std::vector<Boid*>& boids) override;
    sf::Vector2f target;
    std::string get_name() override { return std::string("Seek"); };
};

struct Avoid : Rule
{
    Avoid(sf::Vector2f point, float p_weight) :  Rule(p_weight), target(point) {}
    sf::Vector2f apply_rule(Boid b, const std::vector<Boid*>& boids) override;
    sf::Vector2f target;
    std::string get_name() override { return std::string("Avoid"); };
};

struct BoundingBox : Rule
{
    BoundingBox(sf::Vector2f p_topleft, sf::Vector2f p_bottomright, float p_area, float p_weight)
    :  Rule(p_weight), topleft(p_topleft), bottomright(p_bottomright), area_of_effect(p_area){}
    sf::Vector2f apply_rule(Boid b, const std::vector<Boid*>& boids) override;
    sf::Vector2f topleft, bottomright;
    float area_of_effect;
    std::string get_name() override { return std::string("BoundingBox"); };
};

struct Gravity : Rule
{
    Gravity(float ground, float p_weight) : Rule(p_weight), ground(ground) {}
    sf::Vector2f apply_rule(Boid b, const std::vector<Boid*>& boids) override;
    float ground;
    std::string get_name() override { return std::string("Gravity"); };
};

#endif //BOIDS_RULE_H
