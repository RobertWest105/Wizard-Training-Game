# Minimum version number
cmake_minimum_required(VERSION 3.0)

set(SOURCE_DIR src)

project(Wizard-Training)

add_executable(Wizard-Training ${SOURCE_DIR}/main.cpp ${SOURCE_DIR}/Collider.cpp ${SOURCE_DIR}/Player.cpp ${SOURCE_DIR}/Fireball.cpp)

target_link_libraries(Wizard-Training sfml-system sfml-window sfml-graphics)
