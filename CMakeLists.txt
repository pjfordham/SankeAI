cmake_minimum_required(VERSION 3.10)

# set the project name
project(SnakeAI VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(snake_ai button.cc  evolution_graph.cc  matrix.cc  neural_net.cc  population.cc  snake_ai.cc  snake.cc)
target_link_libraries(snake_ai sfml-system sfml-window sfml-graphics)
