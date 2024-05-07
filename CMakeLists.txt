cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_PATH "lib/imgui")
set(RLIMGUI_PATH "lib/rlImGui")

project(game_of_life)

include_directories(lib/raylib/src src lib/raygui/src lib/imgui-filebrowser)

add_executable(game_of_life src/main.cpp)

file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
file(GLOB RLIMGUI_SOURCES ${RLIMGUI_PATH}/*.cpp)

add_library("ImGui" STATIC ${IMGUI_SOURCES})
add_library("rlImGui" ${RLIMGUI_SOURCES})

target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})
target_include_directories("rlImGui" PUBLIC ${RLIMGUI_PATH} ${IMGUI_PATH})

if (EMSCRIPTEN)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s ASYNCIFY -sALLOW_MEMORY_GROWTH")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    find_library(RL_LIBRARY NAMES raylib PATHS lib/raylib/src NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
    target_link_libraries(game_of_life ${raylib})
    #set_target_properties(game_of_life PROPERTIES LINK_FLAGS "--preload-file ${CMAKE_SOURCE_DIR}/resources/audio@resources/audio")
else()
    add_subdirectory(lib/raylib)
    file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
endif()

if(EMSCRIPTEN)
    target_link_libraries(game_of_life ${RL_LIBRARY} ImGui rlImGui)
else()
    target_link_libraries(game_of_life raylib ImGui rlImGui)
endif()