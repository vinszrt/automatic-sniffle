cmake_minimum_required(VERSION 3.11)

project(asteroids)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp asteroids.cpp bullets.cpp ship.cpp starlayers.cpp)

enable_abcg(${PROJECT_NAME})
