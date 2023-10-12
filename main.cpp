#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include "boid.h"
#include "rule.h"
#include "vector_utils.h"
#include "include/random.h"
#include "include/quadtree.h"

using BoidList = std::vector<std::unique_ptr<Boid>>;


const int NBOIDS = 200;
const int BOID_HEIGHT = 12;
const int BOID_WIDTH = 8;
const int BOID_MAX_SPEED = 150;
const int BOID_MAX_FORCE = 300;
const int BOID_PERCEPTION_RADIUS = 90;
const int BOID_SEPARATION_RADIUS = 60;
const sf::Color BOID_COLOUR = sf::Color(100, 160, 200);
const float ACCEL_WT = 0.0;
const float ALIGN_WT = 4.0;
const float COHES_WT = 0.9;
const float SEPAR_WT = 2.0;
const float BOUND_WT = 1.0;


#undef DEBUG_SHOW_BOID_FORCES
#undef DEBUG_SHOW_BOID_AWARENESS_RADII
#define DEBUG_SHOW_QUADTREE

int main() {
    RandomVector2fGenerator rg;
    RandomColourGenerator rc;

    using Vec=sf::Vector2f;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Boids!");
    window.setVerticalSyncEnabled(true);

    // Generates NBOIDS boids at random positions on the screen, and with a small random initial velocity
    BoidList boids;

    for (int i = 0; i < NBOIDS; ++i) {
        auto pos = rg.generate(0, 1920, 0, 1080);
        auto vel = rg.generate(-1, 1, -1, 1);
        auto boid = std::make_unique<Boid>(pos, normalise(vel) * 20.f,
                           BOID_MAX_SPEED, BOID_MAX_FORCE, BOID_PERCEPTION_RADIUS,
                           BOID_HEIGHT, BOID_WIDTH, rc.generate(), i);
        boids.push_back(std::move(boid));
    }

    sf::Clock clock;
    sf::Clock ticker;
    sf::Music music;
    if (!music.openFromFile("/Users/kg8/code/c++/boids/sounds/flock-of-seagulls_daniel-simion.wav")) {
        return -1;
    }
    music.setVolume(15.f);
    music.setLoop(true);
    music.play();

    std::vector<std::unique_ptr<Rule>> rules;
    rules.push_back(std::make_unique<Accelerate>(ACCEL_WT));
    rules.push_back(std::make_unique<Alignment>(ALIGN_WT));
    rules.push_back(std::make_unique<Cohesion>(COHES_WT));
    rules.push_back(std::make_unique<Separation>(BOID_SEPARATION_RADIUS, SEPAR_WT));
//    rules.push_back(std::make_unique<BoundingBox>(
//            sf::Vector2f(100, 100),
//            sf::Vector2f(1820, 980),
//            50, BOUND_WT));

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int id = !boids.empty() ? boids.back()->ID + 1 : 1;
                    sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                    sf::Vector2f vel = rg.generate(-1, 1, -1, 1);
                    auto boid = std::make_unique<Boid>(pos, normalise(vel) * 50.f,
                                       BOID_MAX_SPEED, BOID_MAX_FORCE, BOID_PERCEPTION_RADIUS,
                                       BOID_HEIGHT, BOID_WIDTH, rc.generate(), id);
                    boids.push_back(std::move(boid));
                }
            }
        }

        sf::Time dt = clock.restart();
        sf::Time tick = ticker.getElapsedTime();

        window.clear(sf::Color(235, 230, 225));

        // Add all boids to quadtree
        Quadtree<Boid*> quadtree(0, 1920, 0, 1080);
        for (auto& boid : boids) {
            quadtree.add(boid.get());
        }

        for (auto & boid : boids) {
            sf::Vector2f resultant_force(0, 0);
            auto boidPos = boid->get_position();
            auto neighbours = quadtree.getPointsWithinCircle(boidPos.x, boidPos.y, BOID_PERCEPTION_RADIUS);
            for (auto &rule : rules) {
                sf::Vector2f force_added = normalise(rule->apply_rule(*boid, neighbours)) * rule->weight;
                resultant_force += force_added;
            }
            resultant_force = normalise(resultant_force);
            boid->apply_force(resultant_force);

#ifdef DEBUG_SHOW_BOID_FORCES
            sf::Vertex line[] =
            {
                sf::Vertex(boid->get_position()),
                sf::Vertex(boid->get_position() + resultant_force * 0.5f * (float)BOID_PERCEPTION_RADIUS)
            };
            line[0].color = sf::Color::Black;
            line[1].color = sf::Color::Black;
            window.draw(line, 2, sf::Lines);
            sf::Vertex velocity_line[] =
            {
                sf::Vertex(boid->get_position()),
                sf::Vertex(boid->get_position() + boid->get_velocity())
            };
            velocity_line[0].color = sf::Color::Red;
            velocity_line[1].color = sf::Color::Red;
            window.draw(velocity_line, 2, sf::Lines);
#endif

        }

        for (auto & boid : boids) {
            boid->update(dt);
            window.draw(boid->get_drawable());
#ifdef DEBUG_SHOW_BOID_AWARENESS_RADII
            sf::CircleShape circle(BOID_PERCEPTION_RADIUS*0.5f);
            circle.setPosition(boid->get_position() - 0.5f*sf::Vector2f(BOID_PERCEPTION_RADIUS, BOID_PERCEPTION_RADIUS));
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineColor(sf::Color(240.0f, 20.0f, 20.0f, 60.0f));
            circle.setOutlineThickness(1.0);
            window.draw(circle);
            circle.setRadius(BOID_SEPARATION_RADIUS*0.5f);
            circle.setPosition(boid->get_position() - 0.5f*sf::Vector2f(BOID_SEPARATION_RADIUS, BOID_SEPARATION_RADIUS));
            window.draw(circle);
#endif
        }


#ifdef DEBUG_SHOW_QUADTREE
        for (auto bounds : quadtree.getAllRectangleBounds()) {
            sf::RectangleShape newRectangle(sf::Vector2f(bounds.xmax - bounds.xmin, bounds.ymax - bounds.ymin));
            newRectangle.setPosition(bounds.xmin, bounds.ymin);
            newRectangle.setFillColor(sf::Color::Transparent);
            newRectangle.setOutlineColor(sf::Color::Green);
            newRectangle.setOutlineThickness(1);
            window.draw(newRectangle);
        }
#endif

        window.display();
    }
    return 0;
}
