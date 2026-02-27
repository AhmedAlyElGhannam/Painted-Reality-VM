cmake_minimum_required(VERSION 3.22)

project(PaintedRealityVM)

file(GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} "${${PROJECT_NAME}_SRC_FILES}")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall -std=c2x)

target_link_options(${PROJECT_NAME} PRIVATE -fPIC -shared -ldl -D_GNU_SOURCE)

target_link_libraries(${PROJECT_NAME} PRIVATE spob_memory spob_string)

