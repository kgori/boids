cmake_minimum_required(VERSION 3.15)
project(boids)

find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(boids main.cpp boid.cpp rule.cpp vector_utils.h include/random.h include/quadtree.h)
target_link_libraries(boids PRIVATE sfml-system sfml-graphics sfml-audio sfml-window)