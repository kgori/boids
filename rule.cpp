//
// Created by Kevin Gori on 20/09/2020.
//
#include <iostream>
#include "boid.h"
#include "rule.h"
#include "vector_utils.h"

sf::Vector2f get_acceleration_towards_position(const Boid& b, sf::Vector2f target) {
    auto current_location = b.get_position();
    auto current_velocity = b.get_velocity();
    sf::Vector2f acceleration = target - current_location - current_velocity;

    return magnitude(acceleration) > 0 ? normalise(acceleration) : acceleration;
}

sf::Vector2f Cohesion::apply_rule(Boid b, const std::vector<Boid*>& boids) {
    sf::Vector2f steering(0, 0);
    sf::Vector2f centre_of_mass(0, 0);
    float N = 0.0f;

    for (const auto& boid : boids) {
        if (b.ID == boid->ID) continue;
        if (magnitude(boid->get_position() - b.get_position()) < b.perception) {
            centre_of_mass += boid->get_position();
            N++;
        }
    }
    if (N > 0) {
        centre_of_mass /= N;
        steering = get_acceleration_towards_position(b, centre_of_mass);
    }
    return steering;
}

sf::Vector2f Separation::apply_rule(Boid me, const std::vector<Boid*>& boids) {
    sf::Vector2f target = me.get_position();
    sf::Vector2f direction_to_move;
    bool rule_activated = false;
    for (const auto& neighbour : boids) {
        if (neighbour->ID == me.ID) continue;
        float distance_from_neighbour = magnitude(neighbour->get_position() - me.get_position());
        if (distance_from_neighbour < separation_threshold) {
            rule_activated = true;
            direction_to_move = normalise(me.get_position() - neighbour->get_position());
            float distance_to_move = separation_threshold;
            target += direction_to_move * distance_to_move;
        }
    }
    return rule_activated ? get_acceleration_towards_position(me, target) : sf::Vector2f(0, 0);
}

sf::Vector2f Alignment::apply_rule(Boid b, const std::vector<Boid*>& boids) {
    sf::Vector2f average_velocity(0, 0);
    float N = 0;
    for (const auto& boid : boids) {
        //if (boid->ID == b.ID) continue;
        float distance = magnitude(boid->get_position() - b.get_position());
        if (distance < b.perception) {
            average_velocity += boid->get_velocity();
            N += 1;
        }
    }

    if (N > 0) {
        average_velocity /= N;
        auto steer = average_velocity;// - b.get_velocity();
        return normalise(steer);
    }
    return sf::Vector2f(0, 0);

}

sf::Vector2f Seek::apply_rule(Boid b, const std::vector<Boid*>& boids) {
    return get_acceleration_towards_position(b, this->target);
}

sf::Vector2f Accelerate::apply_rule(Boid b, const std::vector<Boid*>& boids) {
    int N = 0;
    for (const auto& boid : boids) {
        if (boid->ID == b.ID) continue;
        float distance = magnitude(boid->get_position() - b.get_position());
        if (distance < b.perception) {
            N++;
        }
    }
    return N == 0 ? normalise(b.get_velocity()) : sf::Vector2f(0.0f, 0.0f);
}

sf::Vector2f BoundingBox::apply_rule(Boid b, const std::vector<Boid*>& boids) {
    float x_repulsion = (b.get_position().x - topleft.x) < area_of_effect ?
            area_of_effect - (b.get_position().x - topleft.x) : 0;
    x_repulsion += (bottomright.x - b.get_position().x) < area_of_effect ?
            bottomright.x - b.get_position().x - area_of_effect : 0;
    float y_repulsion = (b.get_position().y - topleft.y) < area_of_effect ?
                        area_of_effect - (b.get_position().y - topleft.y) : 0;
    y_repulsion += (bottomright.y - b.get_position().y) < area_of_effect ?
                   bottomright.y - b.get_position().y - area_of_effect : 0;
    sf::Vector2f force(x_repulsion, y_repulsion);
    return force;
}

sf::Vector2f Avoid::apply_rule(Boid b, const std::vector<Boid*>& boids) {
    float distance = magnitude((b.get_position() - target));
    return -get_acceleration_towards_position(b, this->target) / (distance * distance);
}

sf::Vector2f Gravity::apply_rule(Boid b, const std::vector<Boid *> &boids) {
    auto pos = b.get_position();
    if (pos.y < ground) {
        return get_acceleration_towards_position(b, sf::Vector2f(pos.x, ground));
    }
    return sf::Vector2f(0.0f, 0.0f);
}

