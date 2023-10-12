//
// Created by Kevin Gori on 20/09/2020.
//

#ifndef BOIDS_VECTOR_UTILS_H
#define BOIDS_VECTOR_UTILS_H
#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>

const float PI = 3.14159265358979323846;

inline float magnitude(sf::Vector2f v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

inline sf::Vector2f normalise(sf::Vector2f v) {
    float mag = magnitude(v);
    return mag > 0 ? v / magnitude(v) : v;
}

inline float vector_to_rotation(sf::Vector2f vector) {
    float x = vector.x;
    float y = vector.y;

    if (x == 0) {
        if (y > 0) {
            return 90;
        } else if (y == 0) {
            return 0;
        } else {
            return 270;
        }
    } else if (y == 0) {
        if (x >= 0) {
            return 180;
        } else {
            return 0;
        }
    }
    float radians = atanf(y / x);
    float degrees = radians * 180 / PI;

    if (x < 0 && y < 0) {
        return degrees + 180;
    } else if (x < 0) {
        return degrees + 180;
    } else if (y < 0) {
        return degrees + 360;
    } else {
        return degrees;
    }
}

inline std::string to_str(sf::Vector2f vec) {
    std::stringstream ss;
    ss << "(" << vec.x << ", " << vec.y << ") [" << magnitude(vec) << "]";
    return ss.str();
}

#endif //BOIDS_VECTOR_UTILS_H
