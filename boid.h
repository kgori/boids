//
// Created by Kevin Gori on 18/09/2020.
//
#include <cmath>
#include <SFML/Graphics.hpp>
#include "vector_utils.h"
#ifndef BOIDS_BOID_H
#define BOIDS_BOID_H

class Boid
{
public:
    Boid(sf::Vector2f position,
         sf::Vector2f initial_velocity,
         float max_speed,
         float max_force,
         float perception_radius,
         float height,
         float width,
         sf::Color colour,
         int ID);

    // Public methods
    sf::ConvexShape get_drawable();
    void apply_force(sf::Vector2f force);
    void update(sf::Time tick);
    void print();

    inline sf::Vector2f get_position() const {
        return position;
    }

    inline sf::Vector2f get_velocity() const {
        return velocity;
    }

    inline void set_velocity(sf::Vector2f vel) {
        velocity = vel;
    }

    bool operator == (const Boid &rhs) const {
        return position == rhs.position;
    }

    bool operator != (const Boid &rhs) const {
        return position != rhs.position;
    }

    float perception;
    float max_speed;
    float max_force = 1;
    int ID;

private:
    // Private data
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::ConvexShape sprite;

    // Private methods
    sf::ConvexShape create_sprite(float height, float width, sf::Color colour);

    inline float velocity_to_rotation() {
        return vector_to_rotation(velocity);
    }

    inline float acceleration_to_rotation() {
        return vector_to_rotation(acceleration);
    }
};

#endif //BOIDS_BOID_H
