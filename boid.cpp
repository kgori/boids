//
// Created by Kevin Gori on 18/09/2020.
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include "boid.h"
#include "vector_utils.h"

Boid::Boid(sf::Vector2f initial_position,
           sf::Vector2f initial_velocity,
           float max_speed,
           float max_force,
           float perception_radius,
           float height,
           float width,
           sf::Color colour,
           int ID)
   : position(initial_position),
     velocity(initial_velocity),
     max_speed(max_speed),
     max_force(max_force),
     perception(perception_radius),
     ID(ID)
{
    sprite = create_sprite(height, width, colour);
}

sf::ConvexShape Boid::get_drawable() {
    float rotation = velocity_to_rotation();
    sprite.setRotation(rotation);
    sprite.setPosition(position);
    return sprite;
}

void Boid::apply_force(sf::Vector2f force) {
    acceleration = normalise(force) * max_force;
}

void Boid::update(sf::Time tick) {
    position += velocity * (float)tick.asSeconds();

    if (position.x < 0) position.x += 1920.0f;
    if (position.x > 1920.0f) position.x -= 1920.0f;
    if (position.y < 0) position.y += 1080.0f;
    if (position.y > 1080.0f) position.y -= 1080.0f;


    velocity += acceleration * (float)tick.asSeconds();

    if (magnitude(velocity) > max_speed) {
        velocity = velocity / magnitude(velocity) * max_speed;
    }

    acceleration = sf::Vector2f(0.0, 0.0);
}

sf::ConvexShape Boid::create_sprite(float height, float width, sf::Color colour) {
    sf::ConvexShape shape(4);
    shape.setPoint(0, sf::Vector2f(2.0 * height / 3.0, 0));
    shape.setPoint(1, sf::Vector2f(-1.0 * height / 3.0, -width / 2.0));
    shape.setPoint(2, sf::Vector2f(0, 0));
    shape.setPoint(3, sf::Vector2f(-1.0 * height / 3.0, width / 2.0));
    shape.setFillColor(colour); // 100 160 200
    return shape;
}

void Boid::print() {
    std::cout << "Boid #" << ID << ": pos" << to_str(position)
              << "; vel(" << to_str(velocity)
              << "; acc(" << to_str(acceleration)
              << "; dir(" << vector_to_rotation(velocity) << ")" << std::endl;
}